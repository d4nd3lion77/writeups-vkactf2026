#!/usr/bin/env python3
import base64
import re
import subprocess
import sys
from pathlib import Path


B64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
LINE_RE = re.compile(r"PROG=([0-7]+)\s+NOUN=([0-7]+)")


def main() -> int:
    if len(sys.argv) != 2:
        print(f"usage: {Path(sys.argv[0]).name} <Message.exe>")
        return 1

    exe = Path(sys.argv[1]).expanduser().resolve()
    out = subprocess.check_output([str(exe)], text=True, errors="ignore")

    state = 0o27
    syms = []
    for prog, noun in LINE_RE.findall(out):
        state = (state + 0o13) & 0o77
        syms.append((int(prog, 8) - state) & 0o77)
        syms.append((int(noun, 8) - state - 0o07) & 0o77)

    payload_len = syms[0]
    payload = syms[2 : 2 + payload_len]

    enc = "".join(B64[x] for x in payload)
    enc += "=" * (-len(enc) % 4)

    data = base64.b64decode(enc)
    flag = bytes(b ^ ((i * 37 + 91) & 0xFF) for i, b in enumerate(data))
    print(flag.decode())
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
