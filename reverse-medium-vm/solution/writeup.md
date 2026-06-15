## rev-medium-vm

| Событие | Название | Категория |
| :------ | ---- | ---- |
| VKACTF 2026 | Кейген| reverse |

  
### Описание


> Автор: aquarius
>
Решение реверс тасков можно сравнить с тяжелой атлетикой, этот исключением не является.


### Решение
Программа просит ввести ключ. Очевидно, что нужно найти такой ключ, который заставит проргамму выдать флаг.
Открыв программу в дизассемблере, можно увидеть много анти-ии каши, которая нас не интересует. Интерес представляет стековая виртуальная машина, усложненная децентрализванным обработчиком инструкций, XOR - шифрованием инструкций, MBA - обфускацией арифметики.
Дизассемблер для VM представлен в файле [disasm.py](disasm.py).

По дизассемблированному байткоду можно восстановить алгоритм обработки ключа:

```
def decrypt(bytes, key):
    encrypted = bytearray()
    
    key1 = key & 0xFF
    key2 = (key >> 8) & 0xFF
    key3 = (key >> 16) & 0xFF
    key4 = (key >> 24) & 0xFF
    
    for i, byte in enumerate(bytes):
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
```
Алгоритм не криптостойкий, поэтому зная формат флага можно восстановить его. Решение представлено в файле [solve.py](solve.py)

### Флаг
```
vka{well_done_champion_you_successfully_cracked_virtual_machine}
```
