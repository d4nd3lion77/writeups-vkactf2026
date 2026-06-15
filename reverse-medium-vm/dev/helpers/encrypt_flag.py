def decrypt(code, key):
    encrypted = bytearray()
    
    key1 = key & 0xFF
    key2 = (key >> 8) & 0xFF
    key3 = (key >> 16) & 0xFF
    key4 = (key >> 24) & 0xFF
    
    for i, byte in enumerate(code):
        if i % 4 == 0:
            result = (byte ^ key1 + i) & 0xFF
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

def encrypt(encrypted_code, key):
    decrypted = bytearray()
    
    key1 = key & 0xFF
    key2 = (key >> 8) & 0xFF
    key3 = (key >> 16) & 0xFF
    key4 = (key >> 24) & 0xFF
    
    for i, byte in enumerate(encrypted_code):
        if i % 4 == 0:
            result = byte ^ (key1 + i)
        elif i % 4 == 1:
            temp = byte ^ (i + key2)
            inv = 0
            for j in range(256):
                if (key2 * j) % 256 == 1:
                    inv = j
                    break
            if inv != 0:
                result = (temp * inv) & 0xFF
            else:
                found = False
                for candidate in range(256):
                    if ((candidate * key2) & 0xFF) == temp:
                        result = candidate
                        found = True
                        break
                if not found:
                    result = 0
        elif i % 4 == 2:
            shifted = byte ^ key3
            result = ((shifted >> 2) | ((shifted & 0x03) << 6)) & 0xFF
        else:
            if i > 0:
                result = (byte - key4 - encrypted_code[i-1]) & 0xFF
            else:
                result = (byte - key4) & 0xFF
        
        decrypted.append(result & 0xFF)
    
    return bytes(decrypted)

input_string = "vka{well_done_champion_you_successfully_cracked_virtual_machine}"
dump = input_string.encode('utf-8')

print("[+] Original string:")
print("-"*50)
print(f"String: {input_string}")
print(f"Bytes: {dump}")
print(f"Hex: {', '.join(f'0x{byte:02x}' for byte in dump)}")
print("-"*50)

key = 0x9778
print("[*] Encrypting string...")
print(f"[*] Key: {key}")

encrypted = encrypt(dump, key)
print(f"[+] Encrypted:")
print("-"* 50)
hex_bytes = ', '.join(f'0x{byte:02x}' for byte in encrypted)
print(f"Encrypted bytes: {hex_bytes}")
print(f"Length: {len(encrypted)}")
print("-"* 50)

print("[*] Trying to decrypt...")
decrypted = decrypt(encrypted, key)
print(f"[+] Decrypted:")
print("-"* 50)
decoded_string = decrypted.decode('utf-8')
print(f"Decoded string: {decoded_string}")
print(f"Decrypted bytes: {', '.join(f'0x{byte:02x}' for byte in decrypted)}")
print("-"* 50)

if decrypted == dump:
    print("[+] Success! Original and decrypted strings match!")
else:
    print("[-] Original and decrypted are different!")