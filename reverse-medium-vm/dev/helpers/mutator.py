#!/usr/bin/env python3

import random
from re import findall
import os
import argparse
import shutil


parser = argparse.ArgumentParser(description='Скрипт для мутации флагов')

parser.add_argument(
    '--mode',
    dest="mode",
    required=True,
    choices=['print', 'file'],
    help='Режим работы: print — вывести флаги, file — создать файлы'
)

parser.add_argument(
    '--number',
    dest="number",
    required=True,
    type=int,
    help='Количество генерируемых файлов/флагов'
)

parser.add_argument(
    '--flag',
    dest="flag",
    required=True,
    help='Исходный флаг'
)

parser.add_argument(
    '--file',
    dest="file",
    help='Файл, в котором нужно заменить закодированный флаг'
)

parser.add_argument(
    '--endian',
    dest="endian",
    choices=['little', 'big'],
    default='little',
    help='Порядок байтов WORD: little = XX 00, big = 00 XX'
)

args = parser.parse_args()

FLAG = args.flag
FILE = args.file
NUMBER = args.number
MODE = args.mode
ENDIAN = args.endian

KEY = 0x9778


# =====================================================
# Кодирование флага именно твоим алгоритмом
# =====================================================
def encrypt(encrypted_code, key):
    decrypted = bytearray()

    key1 = key & 0xFF
    key2 = (key >> 8) & 0xFF
    key3 = (key >> 16) & 0xFF
    key4 = (key >> 24) & 0xFF

    for i, byte in enumerate(encrypted_code):
        if i % 4 == 0:
            result = byte ^ (key1 + i)

        elif i % 4 == 1:
            temp = byte ^ (i + key2)
            inv = 0

            for j in range(256):
                if (key2 * j) % 256 == 1:
                    inv = j
                    break

            if inv != 0:
                result = (temp * inv) & 0xFF
            else:
                found = False

                for candidate in range(256):
                    if ((candidate * key2) & 0xFF) == temp:
                        result = candidate
                        found = True
                        break

                if not found:
                    result = 0

        elif i % 4 == 2:
            shifted = byte ^ key3
            result = ((shifted >> 2) | ((shifted & 0x03) << 6)) & 0xFF

        else:
            if i > 0:
                result = (byte - key4 - encrypted_code[i - 1]) & 0xFF
            else:
                result = (byte - key4) & 0xFF

        decrypted.append(result & 0xFF)

    return bytes(decrypted)


# =====================================================
# Перевод байтов в WORD-формат
# little: 0x41 -> 41 00
# big:    0x41 -> 00 41
# =====================================================
def bytes_to_words(data, endian="little"):
    result = bytearray()

    for b in data:
        if endian == "little":
            result += bytes([b, 0x00])
        else:
            result += bytes([0x00, b])

    return bytes(result)


# =====================================================
# Замена закодированного WORD-флага в бинаре
# =====================================================
def change_file(directory, id, file, mf):
    with open(FILE, "rb") as f:
        data_to_change = f.read()

    old_encoded_flag = encrypt(FLAG.encode(), KEY)
    new_encoded_flag = encrypt(mf.encode(), KEY)

    old_encoded_words = bytes_to_words(old_encoded_flag, ENDIAN)
    new_encoded_words = bytes_to_words(new_encoded_flag, ENDIAN)

    offset = data_to_change.find(old_encoded_words)

    if offset == -1:
        print("[!] Закодированный исходный флаг в WORD-формате не найден")
        print("[!] Исходный флаг:", FLAG)
        print("[!] Искомая WORD-последовательность:")
        print(old_encoded_words.hex())
        print()
        print("[?] Возможно, в бинаре другой endian.")
        print("[?] Попробуй запустить с параметром:")
        print("    --endian big")
        exit(1)

    print(f"[+] Флаг найден по смещению: 0x{offset:X}")

    new_data = (
        data_to_change[:offset] +
        new_encoded_words +
        data_to_change[offset + len(old_encoded_words):]
    )

    os.mkdir(os.path.join(directory, id))

    with open(os.path.join(directory, id, file), "wb") as f:
        f.write(new_data)


# =====================================================
# Таблицы для мутации флага
# =====================================================
substitutionDict = {
    "a": "aA4",
    "b": "bB",
    "c": "cC",
    "d": "dD",
    "e": "eE3",
    "f": "fF",
    "g": "gG6",
    "h": "hH",
    "i": "iI1",
    "j": "jJ",
    "k": "kK",
    "l": "lL",
    "m": "mM",
    "n": "nN",
    "o": "oO0",
    "p": "pP",
    "q": "qQ",
    "r": "rR",
    "s": "sS5",
    "t": "tT7",
    "u": "uU",
    "v": "vV",
    "w": "wW",
    "x": "xX",
    "y": "yY",
    "z": "zZ"
}

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
            if i == 1:
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


def normalize(s):
    s = s.lower()

    for a, b in zip(leetEncodedSymbols, leetSymbols):
        s = s.replace(a, b)

    return s


# =====================================================
# Проверки для режима file
# =====================================================
if MODE == "file":
    if not FILE:
        print("[!] Specify file!")
        exit(1)

    if not os.path.isfile(FILE):
        print("[!] No such file")
        exit(1)

    file_name = os.path.basename(os.path.abspath(FILE))
    chunk = os.urandom(4).hex()

    files_dir = file_name + "." + chunk + ".ARCHIVE"
    files_dir_full = os.path.join(os.path.dirname(os.path.abspath(FILE)), files_dir)

    os.mkdir(files_dir_full)


# =====================================================
# Основной цикл генерации
# =====================================================
flags = []

for i in range(NUMBER):
    while True:
        mf = mutate(FLAG)

        if mf not in flags:
            break

    assert FLAG == normalize(mf)

    if MODE == "print":
        print(mf)
    else:
        change_file(files_dir_full, str(i + 1), file_name, mf)

    flags.append(mf)


# =====================================================
# Архивация результата
# =====================================================
if MODE == "file":
    shutil.make_archive(files_dir_full, 'zip', files_dir_full)

    flags_file = os.path.join(
        os.path.dirname(os.path.abspath(FILE)),
        file_name + "." + chunk + ".FLAGS.txt"
    )

    with open(flags_file, "w") as f:
        f.write("\n".join(flags))

    print("[+] Archive created:", files_dir_full + ".zip")
    print("[+] Flags saved:", flags_file)