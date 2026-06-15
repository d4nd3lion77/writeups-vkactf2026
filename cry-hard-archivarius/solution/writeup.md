## ДРЕВНИЙ БОГ АРХИВАРИУС

| Событие | Название | Категория | 
| :------ | ---- | ---- | 
| А МОЖЕТ НЕТ, А МОЖЕТ ДА | А МОЖЕТ ЭТО ВСЕ СЛОВА | Crypto | 

  
### Описание


> Автор: Alhpabetic
>
TL;DR

<!-- ### Решение
Игроку дан source-код таска и зашифрованный флаг в виде вывода терминала. Изучим функционал криптосистемы:

1. Имеются три функции:
```python 
def runnerencode(self, number):

def runnerencrypt(self, char):

def runnerxor(self, char):
```

Функции выполняются последовательно при запуске программы и выборе флага `-e`.

2. Дан алфавит и маппинг символов:
```python
runner_alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~"
runner = [chr(x) + chr(y) for x in range(32, 128) for y in range(32, 128)]
```

Изучим алгоритм шифрования:

1. Исходя из алфавита, можно понять, что используется `base85`, это и есть первый шаг нашего шифрования:
```python
    def runnerencode(self, number):
        if not isinstance(number, (int)):
            raise TypeError("must be an integer")
        if number < 0:
            raise ValueError("must be positive")
        encoded_string = ""
        while number:
            number, i = divmod(number, 85)
            encoded_string = self.runner_alphabet[i] + encoded_string
        return encoded_string or self.runner_alphabet[0]
```

2. Далее текст разбивается на пары символов и для каждой из них находит индекс в маппинге `runner`, после чего кодирует в `base85` и дополняет пробелами:
```python
    def runnerencrypt(self, char):
        flag_out = ""
        for i in range(0, len(char), 2):
            pair = char[i : i + 2]
            if len(pair) < 2:
                pair += " "
            flag_out += self.runnerencode(self.runner.index(pair)).ljust(2, " ")
        return flag_out
```

3. XOR с `runner_secret_key`, притом секретный ключ нам неизвестен:
```python
    def runnerxor(self, char):
        flag_out = self.runnerencrypt(char)        
        flag = ''.join(format(int(a, 16) ^ int(b, 16), 'x') 
                    for a, b in zip(flag_out.encode().hex(), self.runner_secret_key.encode().hex() * (len(flag_out.encode().hex()) // len(self.runner_secret_key.encode().hex()) + 1)))
        return flag
```

В силу того, что перевод из `base85` и возвращение начальных значений по маппингу не является тяжелой задачей, для дешифрования имеющегося флага нам необходимо "победить" операцию `XOR` с неизвестным ключом.

Для этого существует атака на основе известного текста, а, так как мы знаем, что зашифрован флаг формата: `VKA{`, мы можем провести ее через 
`runnerencode`, `runnerencrypt` получив последовательность: 'zgcT'.

После этого начинаем XOR'ить с шфиротекстом и получаем первые 4 символа секретного ключа: `blad`. Исходя из тематики CTF добавляем символ `e` и получаем валидный секретный ключ для XOR и восстанавливаем флаг. -->



[Решение](solution.py)
### Флаг

```
vka{u_will_never_be_so_powerful_as_archivarius}
```