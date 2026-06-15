from pathlib import Path
from PIL import Image

MARKER = b"IEND"

ROOT = Path(__file__).resolve().parents[1]
LOGO = ROOT / "src" / "logo.png"

def extract_lsb(image_path):
    img = Image.open(image_path).convert("RGBA")
    data = img.tobytes()

    bits = []
    out = bytearray()

    for byte in data:
        bits.append(byte & 1)

        if len(bits) == 8:
            value = 0
            for bit in bits:
                value = (value << 1) | bit

            out.append(value)
            bits = []

            if out.endswith(MARKER):
                return out[:-len(MARKER)]

    return out

if __name__ == "__main__":
    payload = extract_lsb(LOGO)
    print(payload.decode(errors="ignore"))