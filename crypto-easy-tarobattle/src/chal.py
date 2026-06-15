#!/usr/bin/env python3
import random
import os
import signal
import sys
import time
from re import findall

TAROT_CARDS = [
    "Шут", "Маг", "Жрица", "Императрица", "Император",
    "Иерофант", "Влюблённые", "Колесница", "Сила", "Отшельник",
    "Колесо Фортуны", "Справедливость", "Повешенный", "Смерть", "Умеренность",
    "Дьявол", "Башня", "Звезда", "Луна", "Солнце",
    "Суд", "Мир"
]
with open("flag", "r") as f:
    FLAG = f.read()

substitutionDict = {"a": "aA4", "b": "bB", "c": "cC", "d": "dD", "e": "eE3", "f":"fF", "g": "gG6", "h": "hH", "i": "iI1", "j": "jJ", "k": "kK", "l":"lL", "m": "mM", "n": "nN", "o": "oO0", "p": "pP", "q": "qQ", "r": "rR", "s":"sS5", "t": "tT7", "u": "uU", "v": "vV", "w":"wW", "x": "xX", "y": "yY", "z": "zZ"}

leetEncodedSymbols = "4361057"
leetSymbols = "aegiost"
ordinarySymbols = "bcdfhjklmnpqruvwxyz"

def preCalculate(s):
    devidorsSequence = []
    module = 1
    for i in s:
        if i in leetSymbols:
            module *= 3
            devidorsSequence.append(3)
        elif i in ordinarySymbols:
            module *= 2
            devidorsSequence.append(2)
        else:
            devidorsSequence.append(1)
    module -= 1
    return module, devidorsSequence

def mutate(fl):
    payload = findall(r"{(.+)}", fl)
    if len(payload) == 0:
        s = fl
    else:
        s = payload[0]
    enc = ""
    value = 1
    module, devidorsSequence = preCalculate(s)
    secret = random.randint(0, module)
    for counter, i in enumerate(devidorsSequence):
        if value >= module:
            if i ==  1:
                return fl.replace(s, enc + s[counter:])
            return fl.replace(s, enc + s[counter + 1:])
        if i == 1:
            enc += s[counter]
            continue
        el = secret % i
        value *= i
        enc += substitutionDict[s[counter]][el]
        secret = secret // i
        
    return fl.replace(s, enc + s[counter + 1:])

BANNER = r"""
=================================================
   Б И Т В А   Э К С Т Р А С Е Н С О В
   Криптографический Сеанс
=================================================

Великий Рандомус черпает силу из самого Времени.
В момент твоего подключения колода была перетасована
энергией текущего момента.

Назови следующие 10 карт, которые будут вытянуты
из этой колоды.

У тебя есть 60 секунд.
=================================================
"""

NUM_PREDICT = 10


def timeout_handler(signum, frame):
    print("\n[!] Время вышло. Карты безмолвствуют...")
    sys.exit(1)


def main():
    sys.stdout.reconfigure(line_buffering=True)
    print(BANNER)
    seed = int(time.time())
    rng = random.Random(seed)
    print("[*] Колода перетасована. Энергия момента зафиксирована...\n")

    signal.signal(signal.SIGALRM, timeout_handler)
    signal.alarm(60)

    print(f"Предскажи следующие {NUM_PREDICT} карт (индексы 0-21, через запятую).")
    print("Пример: 5,12,0,21,3,3,3,17,8,19")
    print()

    try:
        guess_line = input("Твоё предсказание > ").strip()
    except EOFError:
        print("\n[!] Сеанс прерван.")
        sys.exit(1)

    signal.alarm(0)

    try:
        guesses = [int(x.strip()) for x in guess_line.split(",")]
    except ValueError:
        print("[!] Неверный формат ввода. Карты не приняли твою энергию.")
        sys.exit(1)

    if len(guesses) != NUM_PREDICT:
        print(f"[!] Нужно ровно {NUM_PREDICT} предсказаний, получено {len(guesses)}.")
        sys.exit(1)

    actual_cards = [rng.randint(0, len(TAROT_CARDS) - 1) for _ in range(NUM_PREDICT)]

    print("\n--- Сверка с картами судьбы ---")
    correct = 0
    for i, (g, a) in enumerate(zip(guesses, actual_cards)):
        g_name = TAROT_CARDS[g] if 0 <= g <= 21 else "???"
        a_name = TAROT_CARDS[a]
        status = "ВЕРНО" if g == a else "неверно"
        if g == a:
            correct += 1
        print(f"  Карта {i+1}: ты сказал [{g_name}] -> выпало [{a_name}] -- {status}")

    print(f"\nИтог: {correct}/{NUM_PREDICT}")

    if correct == NUM_PREDICT:
        print("\nВеликий Рандомус признал твою силу над Временем!")
        print(f"Флаг: {mutate(FLAG)}")
    else:
        print("\nВремя ускользнуло от тебя. Попробуй снова.")

    sys.exit(0)


if __name__ == "__main__":
    main()
