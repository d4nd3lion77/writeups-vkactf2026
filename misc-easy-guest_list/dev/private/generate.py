#!/usr/bin/env python3
import gzip
import argparse
import os
import random
import re
import shutil
import sqlite3
import struct
import tarfile
import time
import zlib
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DIST = ROOT / "dist"
DEFAULT_FLAG = "vkactf{the_guest_list_was_never_deleted}"


def pdf_escape(text: str) -> str:
    return text.replace("\\", "\\\\").replace("(", "\\(").replace(")", "\\)")


def build_pdf(flag: str) -> bytes:
    guests = [
        ("Amina El-Sayed", "EGY-1042", "Protocol guest"),
        ("Bruno Caruso", "ITA-2311", "Broadcast delegation"),
        ("Claire Moreau", "FRA-1780", "Cultural program"),
        ("Dmitri Sokolov", "AIN-0208", "Medical observer"),
        ("Elena Ruiz", "ESP-2041", "Protocol guest"),
        ("Farah Haddad", "LBN-4420", "Cultural program"),
        ("Hiro Tanaka", "JPN-1188", "Technical delegation"),
        ("Ines Duarte", "POR-3019", "Broadcast delegation"),
        ("Marta Novak", "CZE-1882", "Protocol guest"),
        ("Noah Stein", "GER-2250", "Medical observer"),
        ("Oksana Petrenko", "UKR-1907", "Cultural program"),
        ("Viktor Hale", "IOC-0007", "Special access"),
        ("Yara Haddad", "JOR-4421", "Cultural program"),
    ]

    ops = [
        "BT",
        "/F1 18 Tf",
        "72 780 Td",
        "(Official Olympic Guest List) Tj",
        "/F1 9 Tf",
        "0 -24 Td",
        "(Accreditation Office / night shift export / job #42) Tj",
        "/F1 11 Tf",
    ]
    for name, acc_id, note in guests:
        ops.extend(["0 -22 Td", f"({pdf_escape(f'{name:<20} {acc_id:<9} {note}')}) Tj"])

    # The flag is intentionally not visible when the PDF is opened. It is also
    # inside a Flate-compressed stream, so a raw strings pass over dist/ will not
    # expose it. Solvers recover it by repairing/reassembling the printed PDF and
    # inflating the page content stream or using pdftotext-like tooling.
    ops.extend(
        [
            "1 1 1 rg",
            "0 -30 Td",
            f"({pdf_escape('audit marker: ' + flag)}) Tj",
            "0 0 0 rg",
            "ET",
        ]
    )

    content = "\n".join(ops).encode()
    compressed = zlib.compress(content, 9)

    objects = [
        b"<< /Type /Catalog /Pages 2 0 R >>",
        b"<< /Type /Pages /Kids [3 0 R] /Count 1 >>",
        b"<< /Type /Page /Parent 2 0 R /MediaBox [0 0 595 842] /Resources << /Font << /F1 4 0 R >> >> /Contents 5 0 R >>",
        b"<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>",
        b"<< /Length %d /Filter /FlateDecode >>\nstream\n%s\nendstream" % (len(compressed), compressed),
    ]

    out = bytearray(b"%PDF-1.4\n%\xe2\xe3\xcf\xd3\n")
    offsets = [0]
    for i, obj in enumerate(objects, start=1):
        offsets.append(len(out))
        out += f"{i} 0 obj\n".encode() + obj + b"\nendobj\n"
    xref = len(out)
    out += f"xref\n0 {len(objects) + 1}\n0000000000 65535 f \n".encode()
    for offset in offsets[1:]:
        out += f"{offset:010d} 00000 n \n".encode()
    out += (
        b"trailer\n"
        + f"<< /Size {len(objects) + 1} /Root 1 0 R >>\nstartxref\n{xref}\n%%EOF\n".encode()
    )
    return bytes(out)


def checksum16(data: bytes) -> int:
    if len(data) % 2:
        data += b"\0"
    total = sum(struct.unpack("!%dH" % (len(data) // 2), data))
    total = (total >> 16) + (total & 0xFFFF)
    total += total >> 16
    return (~total) & 0xFFFF


def packet(src_ip, dst_ip, src_port, dst_port, seq, ack, flags, payload, ts):
    eth = bytes.fromhex("00112233445566778899aabb0800")
    src = bytes(map(int, src_ip.split(".")))
    dst = bytes(map(int, dst_ip.split(".")))
    tcp_len = 20 + len(payload)
    ip_header = struct.pack("!BBHHHBBH4s4s", 0x45, 0, 20 + tcp_len, 0x4242, 0, 64, 6, 0, src, dst)
    ip_header = ip_header[:10] + struct.pack("!H", checksum16(ip_header)) + ip_header[12:]
    tcp_header = struct.pack("!HHLLBBHHH", src_port, dst_port, seq, ack, 5 << 4, flags, 64240, 0, 0)
    pseudo = src + dst + struct.pack("!BBH", 0, 6, tcp_len)
    tcp_sum = checksum16(pseudo + tcp_header + payload)
    tcp_header = tcp_header[:16] + struct.pack("!H", tcp_sum) + tcp_header[18:]
    frame = eth + ip_header + tcp_header + payload
    sec = int(ts)
    usec = int((ts - sec) * 1_000_000)
    return struct.pack("<IIII", sec, usec, len(frame), len(frame)) + frame


def write_pcap(path: Path, pdf_prefix: bytes):
    gz = gzip.compress(pdf_prefix, compresslevel=9, mtime=0)
    ipp_body = (
        b"IPP/2.0 Print-Job\r\n"
        b"printer-uri=ipp://printer-03.local/printers/accreditation\r\n"
        b"requesting-user-name=night.operator\r\n"
        b"job-name=olympic_guest_list_final.pdf\r\n"
        b"document-format=application/pdf\r\n"
        b"content-encoding=gzip\r\n"
        b"\r\n"
        + gz
    )
    http = (
        b"POST /printers/accreditation HTTP/1.1\r\n"
        b"Host: printer-03.local:631\r\n"
        b"User-Agent: CUPS/2.4.7\r\n"
        b"Content-Type: application/ipp\r\n"
        + f"Content-Length: {len(ipp_body)}\r\n".encode()
        + b"Connection: close\r\n\r\n"
        + ipp_body
    )

    global_header = struct.pack("<IHHIIII", 0xA1B2C3D4, 2, 4, 0, 0, 65535, 1)
    src_ip, dst_ip = "10.24.7.18", "10.24.7.63"
    sport, dport = 49152, 631
    seq, ack = 1000, 7000
    chunks = [http[i : i + 420] for i in range(0, len(http), 420)]
    ts = 1718166420.0
    data = bytearray(global_header)
    data += packet(src_ip, dst_ip, sport, dport, seq, 0, 0x02, b"", ts)
    seq += 1
    ts += 0.020
    data += packet(dst_ip, src_ip, dport, sport, ack, seq, 0x12, b"", ts)
    ack += 1
    ts += 0.020
    data += packet(src_ip, dst_ip, sport, dport, seq, ack, 0x10, b"", ts)
    for chunk in chunks:
        ts += 0.045
        data += packet(src_ip, dst_ip, sport, dport, seq, ack, 0x18, chunk, ts)
        seq += len(chunk)
    ts += 0.060
    resp = b"HTTP/1.1 200 OK\r\nContent-Type: application/ipp\r\nContent-Length: 32\r\n\r\njob-id=42\njob-state=processing\n"
    data += packet(dst_ip, src_ip, dport, sport, ack, seq, 0x18, resp, ts)
    path.write_bytes(data)


def write_logs(path: Path, prefix_len: int, tail_len: int):
    tmp = ROOT / "private" / "_logs"
    if tmp.exists():
        shutil.rmtree(tmp)
    tmp.mkdir(parents=True)
    (tmp / "access_log").write_text(
        "\n".join(
            [
                '10.24.7.18 - night.operator [12/Jun/2026:02:07:00 +0300] "POST /printers/accreditation HTTP/1.1" 200 876 Create-Job successful-ok',
                '10.24.7.18 - night.operator [12/Jun/2026:02:07:01 +0300] "POST /printers/accreditation HTTP/1.1" 200 219 Send-Document successful-ok',
                '10.24.7.18 - night.operator [12/Jun/2026:02:09:13 +0300] "POST /admin/ HTTP/1.1" 200 142 Cancel-Job successful-ok',
            ]
        )
        + "\n",
        encoding="utf-8",
    )
    (tmp / "page_log").write_text(
        "printer-03 night.operator 42 [12/Jun/2026:02:07:12 +0300] 1 1 - localhost olympic_guest_list_final.pdf - -\n",
        encoding="utf-8",
    )
    (tmp / "error_log").write_text(
        "\n".join(
            [
                "D [12/Jun/2026:02:07:00 +0300] [Job 42] argv[3]=\"olympic_guest_list_final.pdf\"",
                f"D [12/Jun/2026:02:07:01 +0300] [Job 42] gzip prefix accepted, {prefix_len} bytes before spool handoff",
                f"W [12/Jun/2026:02:09:14 +0300] [Job 42] queue entry removed, recovered tail is {tail_len} bytes",
                "D [12/Jun/2026:02:12:53 +0300] [Job 43] test page discarded",
            ]
        )
        + "\n",
        encoding="utf-8",
    )
    with tarfile.open(path, "w:gz") as tar:
        for file in sorted(tmp.iterdir()):
            tar.add(file, arcname=file.name)
    shutil.rmtree(tmp)


def write_spool(path: Path, pdf_tail: bytes):
    tmp = ROOT / "private" / "_spool"
    if tmp.exists():
        shutil.rmtree(tmp)
    tmp.mkdir(parents=True)
    (tmp / "c00042").write_text(
        "\n".join(
            [
                "Hprinter-03.local",
                "Pnight.operator",
                "Jolympic_guest_list_final.pdf",
                "NAccreditation Office final export",
                "Tapplication/pdf",
                "Uguest-list-stage",
            ]
        )
        + "\n",
        encoding="utf-8",
    )
    (tmp / "d00042-001.tail").write_bytes(pdf_tail)
    (tmp / "d00043-001.decoy").write_bytes(b"%PDF-1.4\n% decoy test page removed before rendering\n")
    with tarfile.open(path, "w:gz") as tar:
        for file in sorted(tmp.iterdir()):
            tar.add(file, arcname=file.name)
    shutil.rmtree(tmp)


def write_history(path: Path):
    if path.exists():
        path.unlink()
    db = sqlite3.connect(path)
    db.execute("create table urls(id integer primary key, url text, title text, last_visit_time integer)")
    rows = [
        ("https://intra.olympics.local/accreditation/preview?draft=37", "Guest list preview", 13362502005000000),
        ("https://intra.olympics.local/accreditation/print-preview?job=42&stage=final", "Olympic Guest List - final print", 13362502014000000),
        ("https://intra.olympics.local/accreditation/search?q=Viktor+Hale", "Accreditation search", 13362502031000000),
        ("http://printer-03.local:631/jobs?which_jobs=completed", "CUPS Jobs", 13362502095000000),
    ]
    db.executemany("insert into urls(url,title,last_visit_time) values(?,?,?)", rows)
    db.commit()
    db.close()


def write_readme(path: Path):
    path.write_text(
        "\n".join(
            [
                "The Olympic accreditation office printed a guest list after midnight.",
                "The print job was deleted, but security recovered several artifacts from the workstation and printer.",
                "",
                "Recover the printed guest list and identify the unauthorized invitee.",
                "",
                "Flag format uses the lowercase vkactf prefix.",
            ]
        )
        + "\n",
        encoding="utf-8",
    )


def main():
    parser = argparse.ArgumentParser(description="Build Guest List challenge artifacts")
    parser.add_argument("--flag", default=DEFAULT_FLAG)
    parser.add_argument("--dist", type=Path, default=DIST)
    parser.add_argument("--quiet", action="store_true")
    args = parser.parse_args()

    random.seed(42)
    dist = args.dist
    flag = args.flag

    dist.mkdir(parents=True, exist_ok=True)
    for path in dist.iterdir():
        if path.is_file():
            path.unlink()

    pdf = build_pdf(flag)
    split = int(len(pdf) * 0.62)
    # Keep the split away from the compressed stream boundary to force actual
    # reassembly before PDF tools can inflate the page content.
    if pdf[split : split + 6] == b"stream":
        split += 32
    prefix, tail = pdf[:split], pdf[split:]

    write_pcap(dist / "olympic-print.pcap", prefix)
    write_logs(dist / "printer_logs.tar.gz", len(prefix), len(tail))
    write_spool(dist / "spool_recovered.tar.gz", tail)
    write_history(dist / "browser_history.sqlite")
    write_readme(dist / "README.txt")

    leaked = []
    for file in dist.iterdir():
        if file.is_file() and re.search(rb"vkactf\{", file.read_bytes()):
            leaked.append(file.name)
    if leaked:
        raise SystemExit(f"flag leaked in dist files: {leaked}")
    if not args.quiet:
        print(f"generated {dist}")
        print(f"flag: {flag}")


if __name__ == "__main__":
    main()
