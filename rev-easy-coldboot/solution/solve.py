#!/usr/bin/env python3
import argparse
import shutil
import struct
import subprocess
from pathlib import Path


SERIAL = b"VK26-MP-83AF19"
HWREV = 4


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


def stream_seed(calib, seed, slot):
    k = crc32(SERIAL)
    k ^= crc32(calib)
    k ^= struct.unpack(">I", seed[0:4])[0]
    k ^= rol32(struct.unpack(">I", seed[8:12])[0], 11)
    k = rol32(k, HWREV)
    k ^= 0x42424242 if slot == b"B" else 0x41414141
    return k or 0x4D495053


def crypt_state(state, k):
    out = bytearray(state)
    for i in range(len(out)):
        k = xorshift32((k + 0x9E3779B9 + i) & 0xFFFFFFFF)
        out[i] ^= (k >> ((i & 3) * 8)) & 0xFF
    return bytes(out)


def patch_nvram(rootfs):
    nvram_path = rootfs / "etc" / "nvram.bin"
    calib = (rootfs / "etc" / "calib.dat").read_bytes()
    slot = (rootfs / "var" / "state" / "boot.slot").read_bytes()[:1]
    nv = bytearray(nvram_path.read_bytes())
    if len(nv) != 100 or nv[:4] != b"NVR2":
        raise SystemExit("unexpected nvram format")

    seed = nv[16:32]
    k = stream_seed(calib, seed, slot)
    state = bytearray(crypt_state(nv[32:96], k))
    print(f"state mode before: {state[:4].decode('ascii', 'replace')}, level={struct.unpack('>I', state[4:8])[0]}")

    state[:4] = b"DIAG"
    state[4:8] = struct.pack(">I", 0x1337)
    nv[32:96] = crypt_state(state, k)
    nv[-4:] = struct.pack(">I", crc32(nv[:-4]) ^ 0x4D495053)
    nvram_path.write_bytes(nv)


def main():
    parser = argparse.ArgumentParser(description="Patch coldboot rootfs to diagnostic mode")
    parser.add_argument("rootfs", type=Path)
    parser.add_argument("--no-run", action="store_true")
    parser.add_argument("--qemu", default="qemu-mips")
    args = parser.parse_args()

    patch_nvram(args.rootfs)
    print("patched nvram.bin")

    if not args.no_run:
        if shutil.which(args.qemu):
            subprocess.run([args.qemu, str(args.rootfs / "sbin" / "watchdogd")], check=True)
        else:
            print(f"{args.qemu} not found; run it manually:")
            print(f"{args.qemu} {args.rootfs / 'sbin' / 'watchdogd'}")


if __name__ == "__main__":
    main()
