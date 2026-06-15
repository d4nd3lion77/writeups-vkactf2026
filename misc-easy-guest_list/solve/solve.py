#!/usr/bin/env python3
import argparse
import re
import struct
import sys
import tarfile
import zlib
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def iter_pcap_payloads(path: Path):
    data = path.read_bytes()
    if data[:4] != b"\xd4\xc3\xb2\xa1":
        raise ValueError("expected little-endian pcap")
    off = 24
    while off + 16 <= len(data):
        _ts_sec, _ts_usec, incl_len, _orig_len = struct.unpack_from("<IIII", data, off)
        off += 16
        frame = data[off : off + incl_len]
        off += incl_len
        if len(frame) < 54 or frame[12:14] != b"\x08\x00":
            continue
        ihl = (frame[14] & 0x0F) * 4
        proto = frame[23]
        if proto != 6:
            continue
        ip_start = 14
        tcp_start = ip_start + ihl
        tcp = frame[tcp_start:]
        if len(tcp) < 20:
            continue
        src_port, dst_port = struct.unpack_from("!HH", tcp, 0)
        data_offset = (tcp[12] >> 4) * 4
        payload = tcp[data_offset:]
        if payload and (src_port == 631 or dst_port == 631):
            yield payload


def extract_pdf_prefix(dist: Path) -> bytes:
    stream = b"".join(iter_pcap_payloads(dist / "olympic-print.pcap"))
    marker = b"\x1f\x8b\x08"
    start = stream.index(marker)
    decompressor = zlib.decompressobj(16 + zlib.MAX_WBITS)
    return decompressor.decompress(stream[start:])


def extract_pdf_tail(dist: Path) -> bytes:
    with tarfile.open(dist / "spool_recovered.tar.gz", "r:gz") as tar:
        member = tar.getmember("d00042-001.tail")
        return tar.extractfile(member).read()


def extract_flag(pdf: bytes) -> str:
    streams = re.findall(rb"stream\r?\n(.*?)\r?\nendstream", pdf, flags=re.S)
    for raw in streams:
        try:
            inflated = zlib.decompress(raw)
        except zlib.error:
            continue
        match = re.search(rb"vkactf\{[^}]+\}", inflated)
        if match:
            return match.group().decode()
    raise ValueError("flag not found")


def main():
    parser = argparse.ArgumentParser(description="Recover Guest List flag")
    parser.add_argument("--dist", type=Path, default=ROOT / "dist")
    args = parser.parse_args()

    prefix = extract_pdf_prefix(args.dist)
    tail = extract_pdf_tail(args.dist)
    pdf = prefix + tail
    out = ROOT / "solve" / "recovered_guest_list.pdf"
    out.write_bytes(pdf)
    print(extract_flag(pdf))
    print(f"wrote {out}")


if __name__ == "__main__":
    sys.exit(main())
