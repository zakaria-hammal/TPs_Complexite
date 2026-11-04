# generate_12_digit_primes.py
import random, sys

LOW  = 100_000_000_000       # 1xxxxxxxxxxx borne basse
HIGH = 199_999_999_999       # borne haute (toujours 12 chiffres, 1er chiffre = 1)
BASES = (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37)

def is_prime(n: int) -> bool:
    """Miller–Rabin déterministe (valide bien au-delà de 10^12)."""
    if n < 2:
        return False
    for p in BASES:
        if n % p == 0:
            return n == p
    d, s = n - 1, 0
    while d % 2 == 0:
        d //= 2
        s += 1
    for a in BASES:
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(s - 1):
            x = x * x % n
            if x == n - 1:
                break
        else:
            return False
    return True

def sample(count: int = 10, seed: int = 42, out: str = "primes_12_digit.txt") -> None:
    rng, seen = random.Random(seed), set()
    with open(out, "w", encoding="ascii", newline="\n") as f:
        while len(seen) < count:
            n = rng.randint(LOW, HIGH)
            if n % 2 == 0:
                n += 1
            if n > HIGH or n in seen:
                continue
            if is_prime(n):
                seen.add(n)
                f.write(f"{n}\n")
    print(f"{count} premiers écrits dans {out!r}")

if __name__ == "__main__":
    n    = int(sys.argv[1]) if len(sys.argv) > 1 else 10
    seed = int(sys.argv[2]) if len(sys.argv) > 2 else 42
    sample(n, seed)
