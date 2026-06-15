#!/usr/bin/env python3
import argparse
import os
import random
import re
import shutil
import subprocess
import sys
from pathlib import Path

import gen_dist


ROOT = Path(__file__).resolve().parent

SUBSTITUTION = {
    "a": "aA4", "b": "bB", "c": "cC", "d": "dD", "e": "eE3", "f": "fF",
    "g": "gG6", "h": "hH", "i": "iI1", "j": "jJ", "k": "kK", "l": "lL",
    "m": "mM", "n": "nN", "o": "oO0", "p": "pP", "q": "qQ", "r": "rR",
    "s": "sS5", "t": "tT7", "u": "uU", "v": "vV", "w": "wW", "x": "xX",
    "y": "yY", "z": "zZ",
}
LEET_ENCODED = "4361057"
LEET_SYMBOLS = "aegiost"
ORDINARY_SYMBOLS = "bcdfhjklmnpqruvwxyz"


def precalculate(s):
    divisors = []
    module = 1
    for ch in s:
        if ch in LEET_SYMBOLS:
            module *= 3
            divisors.append(3)
        elif ch in ORDINARY_SYMBOLS:
            module *= 2
            divisors.append(2)
        else:
            divisors.append(1)
    return module - 1, divisors


def mutate(flag):
    payload = re.findall(r"{(.+)}", flag)
    s = payload[0] if payload else flag
    enc = ""
    value = 1
    module, divisors = precalculate(s)
    secret = random.randint(0, module)
    for counter, div in enumerate(divisors):
        if value >= module:
            if div == 1:
                return flag.replace(s, enc + s[counter:])
            return flag.replace(s, enc + s[counter + 1:])
        if div == 1:
            enc += s[counter]
            continue
        el = secret % div
        value *= div
        enc += SUBSTITUTION[s[counter]][el]
        secret //= div
    return flag.replace(s, enc + s[counter + 1:])


def normalize(s):
    s = s.lower()
    for a, b in zip(LEET_ENCODED, LEET_SYMBOLS):
        s = s.replace(a, b)
    return s


def copy_static_files(dst):
    for rel in ["usr/bin/diag_ping", "usr/bin/nvramctl"]:
        src = ROOT / "dist" / "rootfs" / rel
        out = dst / "rootfs" / rel
        out.parent.mkdir(parents=True, exist_ok=True)
        if src.exists():
            shutil.copy2(src, out)


def build_watchdog(cc, rootfs):
    out = rootfs / "sbin" / "watchdogd"
    out.parent.mkdir(parents=True, exist_ok=True)
    cmd = [
        cc,
        "-O2",
        "-Wall",
        "-Wextra",
        "-std=c99",
        "-s",
        "-static",
        "-o",
        str(out),
        str(ROOT / "src" / "watchdogd.c"),
    ]
    subprocess.run(cmd, cwd=ROOT, check=True)


def main():
    parser = argparse.ArgumentParser(description="Generate mutated coldboot task archives")
    parser.add_argument("--number", required=True, type=int)
    parser.add_argument("--flag", required=True)
    parser.add_argument("--cc", default=os.environ.get("MIPS_CC", "mips-linux-gnu-gcc"))
    parser.add_argument("--out-dir", default=None)
    parser.add_argument("--seed", type=int, default=None)
    args = parser.parse_args()

    if args.seed is not None:
        random.seed(args.seed)

    chunk = os.urandom(4).hex()
    base = Path(args.out_dir) if args.out_dir else ROOT / f"coldboot.{chunk}.ARCHIVE"
    if base.exists():
        shutil.rmtree(base)
    base.mkdir(parents=True)

    flags = []
    for i in range(1, args.number + 1):
        while True:
            mf = mutate(args.flag)
            if mf not in flags:
                break
        assert args.flag == normalize(mf)
        flags.append(mf)

        task_dir = base / str(i)
        rootfs = task_dir / "rootfs"
        gen_dist.generate(mf.encode("ascii"), rootfs)
        copy_static_files(task_dir)
        build_watchdog(args.cc, rootfs)
        print(f"[{i}/{args.number}] {mf}")

    flags_path = base.with_suffix(".FLAGS.txt")
    flags_path.write_text("\n".join(flags) + "\n", encoding="ascii")
    archive_path = shutil.make_archive(str(base), "zip", base)
    print(f"archive: {archive_path}")
    print(f"flags:   {flags_path}")


if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as e:
        print(f"build failed: {' '.join(e.cmd)}", file=sys.stderr)
        raise

