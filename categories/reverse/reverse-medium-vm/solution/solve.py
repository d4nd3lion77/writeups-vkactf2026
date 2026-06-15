import string

flag = [
    0x0e, 0x05, 0x58, 0x1a, 0x0b, 0xef, 0x1b, 0x00, 0xdf, 0xdc, 0xdb, 0xff, 0xe1, 0x3d, 0xd8, 0x05, 0xe9, 0x03, 0x1c, 0xf9, 0xe3, 0x8e, 0xd7, 0x1a, 0xff, 0x03, 0xd7, 0x14, 0xe1, 0xc1, 0xd8, 0x02, 0xeb, 0xed, 0x99, 0x0f, 0xf0, 0xb0, 0x5e, 0xe6, 0xc3, 0x1e, 0x58, 0x02, 0xcf, 0x87, 0x19, 0xfb, 0xde, 0x87, 0x9c, 0x02, 0xd9, 0x5b, 0x1b, 0xf3, 0xdd, 0xf7, 0xd8, 0x05, 0xdd, 0x56, 0x59, 0x18
]

def decrypt(code, key):
    encrypted = bytearray()
    
    key1 = key & 0xFF
    key2 = (key >> 8) & 0xFF
    key3 = (key >> 16) & 0xFF
    key4 = (key >> 24) & 0xFF
    
    for i, byte in enumerate(code):
        if i % 4 == 0:
            result = (byte ^ (key1 + i)) & 0xFF
        elif i % 4 == 1:
            result = ((byte * key2) ^ (i + key2)) & 0xFF
        elif i % 4 == 2:
            shifted = ((byte << 2) | (byte >> 6)) & 0xFF
            result = shifted ^ key3
        else:
            if i > 0:
                result = (byte + key4 + encrypted[i-1]) & 0xFF
            else:
                result = (byte + key4) & 0xFF
        
        encrypted.append(result)
    
    return bytes(encrypted)


def is_printable(bs):
    try:
        return all(chr(b) in string.printable for b in bs)
    except:
        return False

candidates_k1 = []
for k1 in range(256):
    ok = True
    for i in range(0, len(flag), 4):
        b = flag[i]
        res = (b ^ (k1 + i)) & 0xFF
        if chr(res) not in string.printable:
            ok = False
            break
    if ok:
        candidates_k1.append(k1)

print("[+] k1 candidates:", candidates_k1)

candidates_k2 = []
for k2 in range(256):
    ok = True
    for i in range(1, len(flag), 4):
        b = flag[i]
        res = ((b * k2) ^ (i + k2)) & 0xFF
        if chr(res) not in string.printable:
            ok = False
            break
    if ok:
        candidates_k2.append(k2)

print("[+] k2 candidates:", candidates_k2)

candidates_k3 = []
for k3 in range(256):
    ok = True
    for i in range(2, len(flag), 4):
        b = flag[i]
        shifted = ((b << 2) | (b >> 6)) & 0xFF
        res = shifted ^ k3
        if chr(res) not in string.printable:
            ok = False
            break
    if ok:
        candidates_k3.append(k3)

print("[+] k3 candidates:", candidates_k3)

for k1 in candidates_k1:
    for k2 in candidates_k2:
        for k3 in candidates_k3:
            for k4 in range(256):
                key = k1 | (k2 << 8) | (k3 << 16) | (k4 << 24)
                res = decrypt(flag, key)

                if is_printable(res):
                    s = res.decode(errors='ignore')
                    if s.startswith("vka{") and s.endswith("}"):
                        print("\n[+] FOUND!")
                        print("KEY =", hex(key))
                        print("FLAG =", s)
                        exit()