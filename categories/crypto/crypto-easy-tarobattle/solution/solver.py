"""
Usage:
    python3 solver.py HOST PORT
"""

import sys
import time
import random

from pwn import remote, context

context.log_level = "warning"


def predict_cards(seed, n=10, max_index=21):
    rng = random.Random(seed)
    return [rng.randint(0, max_index) for _ in range(n)]


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} HOST PORT")
        sys.exit(1)
    host = sys.argv[1]
    port = int(sys.argv[2])
    io = remote(host, port)
    t_guess = int(time.time())
    io.recvuntil(b">")
    io.recv(1)
    predicted = predict_cards(t_guess)
    io.sendline(",".join(map(str, predicted)).encode())
    result = io.recvall(timeout=5).decode("utf-8", errors="replace")
    print(result)
    io.close()


if __name__ == "__main__":
    main()
