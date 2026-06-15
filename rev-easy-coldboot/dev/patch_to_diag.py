#!/usr/bin/env python3
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parent / "dist" / "rootfs"
SERIAL = b"VK26-MP-83AF19"
HWREV = 4


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
