import base64
import hashlib

FLAG = b"vka{beware_of_malicious_vscode_extensions}"
SEED = "DarkHackerTheme Pro:theme-cache-v1:win32:x64"

key = hashlib.sha256(SEED.encode()).digest()

encrypted = bytearray()

for i, b in enumerate(FLAG):
    encrypted.append(b ^ key[i % len(key)])

blob = base64.b64encode(encrypted).decode()

print("seed:", SEED)
print("blob:", blob)