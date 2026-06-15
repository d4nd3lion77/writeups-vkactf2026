#!/usr/bin/env python3
import argparse
import os
import random
import shutil
import subprocess
import sys
from pathlib import Path
from re import findall


parser = argparse.ArgumentParser(description="Guest List flag mutator")
parser.add_argument("--mode", dest="mode", required=True, choices=["print", "file"])
parser.add_argument("--number", dest="number", required=True, type=int, help="Number of flags/files to generate")
parser.add_argument("--flag", dest="flag", required=True)
parser.add_argument(
    "--file",
    dest="file",
    help="Output directory for file mode. If a file path is provided, its parent directory is used.",
)

args = parser.parse_args()

FLAG = args.flag
FILE = args.file
NUMBER = args.number
MODE = args.mode

ROOT = Path(__file__).resolve().parent
GENERATOR = ROOT / "private" / "generate.py"

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
    "z": "zZ",
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
            return fl.replace(s, enc + s[counter + 1 :])
        if i == 1:
            enc += s[counter]
            continue
        el = secret % i
        value *= i
        enc += substitutionDict[s[counter]][el]
        secret = secret // i

    return fl.replace(s, enc + s[counter + 1 :])


def normalize(s):
    s = s.lower()
    for a, b in zip(leetEncodedSymbols, leetSymbols):
        s = s.replace(a, b)
    return s


def output_base() -> Path:
    if not FILE:
        print("Specify file!")
        sys.exit(1)
    path = Path(FILE).resolve()
    if path.exists() and path.is_file():
        return path.parent
    return path


def change_file(directory: Path, variant_id: str, mf: str):
    variant_dir = directory / variant_id
    dist_dir = variant_dir / "dist"
    variant_dir.mkdir(parents=True)
    subprocess.run(
        [
            sys.executable,
            str(GENERATOR),
            "--flag",
            mf,
            "--dist",
            str(dist_dir),
            "--quiet",
        ],
        cwd=str(ROOT),
        check=True,
    )


if MODE == "file":
    base = output_base()
    base.mkdir(parents=True, exist_ok=True)
    chunk = os.urandom(4).hex()
    files_dir = "guest_list." + chunk + ".ARCHIVE"
    files_dir_full = base / files_dir
    files_dir_full.mkdir()

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
        change_file(files_dir_full, str(i + 1), mf)
    flags.append(mf)

if MODE == "file":
    shutil.make_archive(str(files_dir_full), "zip", str(files_dir_full))
    (base / ("guest_list." + chunk + ".FLAGS.txt")).write_text("\n".join(flags), encoding="utf-8")
