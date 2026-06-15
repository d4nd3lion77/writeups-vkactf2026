#!/usr/bin/env python3
import os
import random
from re import findall
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad

KEY = os.urandom(16)
BLOCK = 16

with open("../flag/flag_not_mut", "rb") as f:
    FLAG = f.read()

#-------------------------------------Античит---------------------
substitutionDict = {"a": "aA4", "b": "bB", "c": "cC", "d": "dD", "e": "eE3", "f":"fF", "g": "gG6", "h": "hH", "i": "iI1", "j": "jJ", "k": "kK", "l":"lL", "m": "mM", "n": "nN", "o": "oO0", "p": "pP", "q": "qQ", "r": "rR", "s":"sS5", "t": "tT7", "u": "uU", "v": "vV", "w":"wW", "x": "xX", "y": "yY", "z": "zZ"}

leetEncodedSymbols = "4361057"
leetSymbols = "aegiost"
ordinarySymbols = "bcdfhjklmnpqruvwxyz"

def preCalculate(s):
    devidorsSequence = []
    module = 1
    for i in s:
        if i in leetSymbols:
            module *= 3
            devidorsSequence.append(3)
        elif i in ordinarySymbols:
            module *= 2
            devidorsSequence.append(2)
        else:
            devidorsSequence.append(1)
    module -= 1
    return module, devidorsSequence

def mutate(fl):
    payload = findall(r"{(.+)}", fl)
    if len(payload) == 0:
        s = fl
    else:
        s = payload[0]
    enc = ""
    value = 1
    module, devidorsSequence = preCalculate(s)
    secret = random.randint(0, module)
    for counter, i in enumerate(devidorsSequence):
        if value >= module:
            if i ==  1:
                return fl.replace(s, enc + s[counter:])
            return fl.replace(s, enc + s[counter + 1:])
        if i == 1:
            enc += s[counter]
            continue
        el = secret % i
        value *= i
        enc += substitutionDict[s[counter]][el]
        secret = secret // i
        
    return fl.replace(s, enc + s[counter + 1:])
#----------------------------------------------------------


def clean_email(data: bytes) -> bytes:
    return data.replace(b"&", b"").replace(b"=", b"")


def encrypt_profile(email: bytes) -> bytes:
    email = clean_email(email)
    profile = b"email=" + email + b"&uid=10&role=user"

    cipher = AES.new(KEY, AES.MODE_ECB)
    return cipher.encrypt(pad(profile, BLOCK))


def decrypt_profile(token: bytes) -> dict:
    cipher = AES.new(KEY, AES.MODE_ECB)
    plaintext = unpad(cipher.decrypt(token), BLOCK)

    result = {}

    for part in plaintext.split(b"&"):
        if b"=" in part:
            key, value = part.split(b"=", 1)
            result[key.decode(errors="ignore")] = value.decode(errors="ignore")

    return result


def menu():
    print("=== Cookie Factory ===")
    print()
    print("1) Create token")
    print("2) Check token")
    print("3) Exit")


def main():
    while True:
        menu()
        choice = input("> ").strip()

        if choice == "1":
            raw = input("Send email as hex:\n> ").strip()

            try:
                email = bytes.fromhex(raw)
            except ValueError:
                print("Invalid hex.")
                print()
                continue

            token = encrypt_profile(email)
            print("Token:")
            print(token.hex())
            print()

        elif choice == "2":
            raw = input("Send token as hex:\n> ").strip()

            try:
                token = bytes.fromhex(raw)
                profile = decrypt_profile(token)
            except Exception:
                print("Invalid token.")
                print()
                continue

            role = profile.get("role", "")

            if role == "admin":
                print("Welcome, admin!")
                print(f"Flag: {mutate(FLAG.decode())}")
                print()
            else:
                print(f"Hello, {role or 'guest'}.")
                print()

        elif choice == "3":
            print("Bye.")
            break

        else:
            print("Invalid option.")
            print()


if __name__ == "__main__":
    main()