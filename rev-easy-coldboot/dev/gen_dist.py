from pathlib import Path
import argparse
import random
import struct


ROOT = Path(__file__).resolve().parent
DIST = ROOT / "dist" / "rootfs"
SRC = ROOT / "src"

DEFAULT_FLAG = b"vkactf{coldboot_mips_nvram_recovery}"
MODEL = "AR9344-EDGE"
SERIAL = "VK26-MP-83AF19"
REGION = "RU"
HWREV = 4
BOOT_SLOT = b"B"


def rol32(x, n):
    n &= 31
    return ((x << n) | (x >> ((32 - n) & 31))) & 0xFFFFFFFF


def xorshift32(x):
    x &= 0xFFFFFFFF
    x ^= (x << 13) & 0xFFFFFFFF
    x ^= x >> 17
    x ^= (x << 5) & 0xFFFFFFFF
    return x & 0xFFFFFFFF


def crc32_update(crc, data):
    crc = (~crc) & 0xFFFFFFFF
    for b in data:
        crc ^= b
        for _ in range(8):
            mask = -(crc & 1) & 0xFFFFFFFF
            crc = ((crc >> 1) ^ (0xEDB88320 & mask)) & 0xFFFFFFFF
    return (~crc) & 0xFFFFFFFF


def crc32(data):
    return crc32_update(0, data)


def board_crc():
    c = 0
    c = crc32_update(c, MODEL.encode())
    c = crc32_update(c, b"|")
    c = crc32_update(c, SERIAL.encode())
    c = crc32_update(c, b"|")
    c = crc32_update(c, REGION.encode())
    c = crc32_update(c, b"|")
    c = crc32_update(c, struct.pack(">I", HWREV))
    return c


def mac128(key, data, extra):
    s = [0x243F6A88 ^ extra, 0x85A308D3, 0x13198A2E, 0x03707344]
    for i, b in enumerate(key):
        j = i & 3
        s[j] = (rol32(s[j] ^ b ^ i, 5) + s[(i + 1) & 3]) & 0xFFFFFFFF
    for i, b in enumerate(data):
        j = i & 3
        s[j] ^= (b + 0x9E3779B9 + (i << 8)) & 0xFFFFFFFF
        s[j] &= 0xFFFFFFFF
        s[(i + 1) & 3] = rol32((s[(i + 1) & 3] + s[j]) & 0xFFFFFFFF, 9)
    for r in range(24):
        s[r & 3] = rol32((s[r & 3] + s[(r + 1) & 3] + 0x7F4A7C15) & 0xFFFFFFFF, 7)
        s[(r + 2) & 3] ^= s[r & 3]
        s[(r + 2) & 3] &= 0xFFFFFFFF
    return b"".join(struct.pack(">I", x) for x in s)


def flag_stream(token_key, calib_crc, length):
    out = bytearray()
    ctr = 0
    while len(out) < length:
        data = f"coldboot-flag|{SERIAL}|{calib_crc:08x}".encode() + struct.pack(">I", ctr)
        out.extend(mac128(token_key, data, 0x636F6C64))
        ctr += 1
    return bytes(out[:length])


def stream_seed(calib, seed):
    k = crc32(SERIAL.encode())
    k ^= crc32(calib)
    k ^= struct.unpack(">I", seed[0:4])[0]
    k ^= rol32(struct.unpack(">I", seed[8:12])[0], 11)
    k = rol32(k, HWREV)
    k ^= 0x42424242 if BOOT_SLOT == b"B" else 0x41414141
    return k or 0x4D495053


def crypt_state(state, k):
    out = bytearray(state)
    for i in range(len(out)):
        k = xorshift32((k + 0x9E3779B9 + i) & 0xFFFFFFFF)
        out[i] ^= (k >> ((i & 3) * 8)) & 0xFF
    return bytes(out)


def c_array(name, data):
    lines = [f"static const uint8_t {name}[FLAG_LEN] = {{"]
    for i in range(0, len(data), 8):
        chunk = ", ".join(f"0x{x:02x}" for x in data[i:i + 8])
        comma = "," if i + 8 < len(data) else ""
        lines.append(f"    {chunk}{comma}")
    lines.append("};")
    return "\n".join(lines)


def write_header(enc_flag, mask_a, flag_len):
    text = "\n".join([
        "#ifndef GENERATED_SECRETS_H",
        "#define GENERATED_SECRETS_H",
        "",
        "#include <stdint.h>",
        "",
        f"#define FLAG_LEN {flag_len}",
        "",
        c_array("ENC_FLAG", enc_flag),
        "",
        c_array("MASK_A", mask_a),
        "",
        "#endif",
        "",
    ])
    (SRC / "generated_secrets.h").write_text(text, encoding="ascii")


def write_solver(seed, calib, token_key):
    text = f'''#!/usr/bin/env python3
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parent / "dist" / "rootfs"
SERIAL = b"{SERIAL}"
HWREV = {HWREV}


def rol32(x, n):
    n &= 31
    return ((x << n) | (x >> ((32 - n) & 31))) & 0xffffffff


def xorshift32(x):
    x &= 0xffffffff
    x ^= (x << 13) & 0xffffffff
    x ^= x >> 17
    x ^= (x << 5) & 0xffffffff
    return x & 0xffffffff


def crc32_update(crc, data):
    crc = (~crc) & 0xffffffff
    for b in data:
        crc ^= b
        for _ in range(8):
            mask = -(crc & 1) & 0xffffffff
            crc = ((crc >> 1) ^ (0xedb88320 & mask)) & 0xffffffff
    return (~crc) & 0xffffffff


def crc32(data):
    return crc32_update(0, data)


def stream_seed(calib, seed, slot):
    k = crc32(SERIAL)
    k ^= crc32(calib)
    k ^= struct.unpack(">I", seed[0:4])[0]
    k ^= rol32(struct.unpack(">I", seed[8:12])[0], 11)
    k = rol32(k, HWREV)
    k ^= 0x42424242 if slot == b"B" else 0x41414141
    return k or 0x4d495053


def crypt_state(state, k):
    out = bytearray(state)
    for i in range(len(out)):
        k = xorshift32((k + 0x9e3779b9 + i) & 0xffffffff)
        out[i] ^= (k >> ((i & 3) * 8)) & 0xff
    return bytes(out)


nv = bytearray((ROOT / "etc" / "nvram.bin").read_bytes())
calib = (ROOT / "etc" / "calib.dat").read_bytes()
slot = (ROOT / "var" / "state" / "boot.slot").read_bytes()[:1]
seed = nv[16:32]
state = bytearray(crypt_state(nv[32:96], stream_seed(calib, seed, slot)))

assert state[:4] == b"SAFE"
state[:4] = b"DIAG"
state[4:8] = struct.pack(">I", 0x1337)

nv[32:96] = crypt_state(state, stream_seed(calib, seed, slot))
chk = crc32(nv[:-4]) ^ 0x4d495053
nv[-4:] = struct.pack(">I", chk)
(ROOT / "etc" / "nvram.bin").write_bytes(nv)
print("patched nvram.bin")
'''
    p = ROOT / "patch_to_diag.py"
    p.write_text(text, encoding="ascii")


def generate(flag, out_root):
    random.seed(0xC01DB007 ^ crc32(flag))
    for p in [
        out_root / "etc",
        out_root / "sbin",
        out_root / "usr" / "bin",
        out_root / "var" / "state",
    ]:
        p.mkdir(parents=True, exist_ok=True)

    calib = bytes(random.randrange(0, 256) for _ in range(256))
    mask_a = bytes(random.randrange(0, 256) for _ in flag)
    seed = bytes(random.randrange(0, 256) for _ in range(16))
    token_key = bytes(random.randrange(0, 256) for _ in range(32))

    bcrc = board_crc()
    ccrc = crc32(calib)
    tag_data = f"{SERIAL}|{ccrc:08x}|{bcrc:08x}".encode()
    tag = mac128(token_key, tag_data, 0x626F6F74 ^ struct.unpack(">I", seed[4:8])[0])

    state = bytearray(64)
    state[0:4] = b"SAFE"
    state[4:8] = struct.pack(">I", 1)
    state[8:40] = token_key
    state[40:56] = tag
    state[56:64] = b"coldboot"
    enc_state = crypt_state(state, stream_seed(calib, seed))

    nv = bytearray()
    nv += b"NVR2"
    nv += struct.pack(">H", 2)
    nv += struct.pack(">H", 7)
    nv += struct.pack(">I", bcrc)
    nv += struct.pack(">I", 0x26)
    nv += seed
    nv += enc_state
    nv += struct.pack(">I", crc32(nv) ^ 0x4D495053)
    assert len(nv) == 100

    ks = flag_stream(token_key, ccrc, len(flag))
    mask_b = calib[64:64 + len(flag)]
    enc_flag = bytes(f ^ a ^ b ^ k for f, a, b, k in zip(flag, mask_a, mask_b, ks))

    write_header(enc_flag, mask_a, len(flag))
    write_solver(seed, calib, token_key)

    (out_root / "etc" / "board.conf").write_text(
        f"model={MODEL}\nserial={SERIAL}\nregion={REGION}\nhwrev={HWREV}\n",
        encoding="ascii",
    )
    (out_root / "etc" / "calib.dat").write_bytes(calib)
    (out_root / "etc" / "nvram.bin").write_bytes(nv)
    (out_root / "var" / "state" / "boot.slot").write_bytes(BOOT_SLOT + b"\n")

    print(f"generated rootfs={out_root}, flag_len={len(flag)}, board_crc={bcrc:08x}")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--flag", default=DEFAULT_FLAG.decode("ascii"))
    parser.add_argument("--out-root", default=str(DIST))
    args = parser.parse_args()
    generate(args.flag.encode("ascii"), Path(args.out_root))


if __name__ == "__main__":
    main()


