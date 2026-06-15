import base64
import hashlib

BLOB = "bLuGv0v+ffoE1lLThBDoStgrVZL3RQ09+RDDwcWwKP1ipIKqWvJl9QXO"
SEED = "DarkHackerTheme Pro:theme-cache-v1:win32:x64"

key = hashlib.sha256(SEED.encode()).digest()
data = base64.b64decode(BLOB)

flag = bytearray()

for i, b in enumerate(data):
    flag.append(b ^ key[i % len(key)])

print(flag.decode())