#!/usr/bin/env python3
from pwn import *
import sys

BLOCK = 16

# Локально:
#   python3 solve.py
#
# Если потом завернешь chall.py в nc:
#   python3 solve.py host port

if len(sys.argv) == 3:
    io = remote(sys.argv[1], int(sys.argv[2]))
else:
    io = process(["python", "task.py"])


def create_token(email: bytes) -> bytes:
    io.recvuntil(b"> ")
    io.sendline(b"1")

    io.recvuntil(b"> ")
    io.sendline(email.hex().encode())

    io.recvuntil(b"Token:\n")
    token_hex = io.recvline().strip().decode()
    return bytes.fromhex(token_hex)


def check_token(token: bytes):
    io.recvuntil(b"> ")
    io.sendline(b"2")

    io.recvuntil(b"> ")
    io.sendline(token.hex().encode())

    print(io.recvuntil(b"\n\n", timeout=2).decode(errors="ignore"))


# ------------------------------------------------------------
# 1. Получаем отдельный ECB-блок:
#
# plaintext block:
#   admin + PKCS#7 padding
#
# Нужно, чтобы этот блок начинался ровно после "email=".
# len("email=") = 6
# 16 - 6 = 10
# ------------------------------------------------------------

email_for_admin_block = (
    b"A" * 10 +
    b"admin" +
    bytes([11]) * 11
)

token1 = create_token(email_for_admin_block)

admin_block = token1[BLOCK:BLOCK * 2]

log.success(f"admin block = {admin_block.hex()}")


# ------------------------------------------------------------
# 2. Получаем нормальный токен, где role= начинается с нового блока:
#
# plaintext:
#   email=<input>&uid=10&role=user
#
# Нужно:
#   len("email=") + len(input) + len("&uid=10&role=") == 0 mod 16
#
# len("email=") = 6
# len("&uid=10&role=") = 13
# 6 + 13 = 19
# Нужно input длиной 13 байт.
# ------------------------------------------------------------

email_for_prefix = b"B" * 13

token2 = create_token(email_for_prefix)

prefix_blocks = token2[:BLOCK * 2]

forged_token = prefix_blocks + admin_block

log.success(f"forged token = {forged_token.hex()}")

check_token(forged_token)

io.close()