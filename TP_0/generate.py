import random
from sympy import isprime, nextprime

LOWER_BOUND = 10**11
UPPER_BOUND = 10**11 + 10**10

def generate_random_12_digit_primes(count=1000):
    primes = set()
    while len(primes) < count:
        num = random.randint(LOWER_BOUND, UPPER_BOUND)
        prime_candidate = nextprime(num)
        if prime_candidate <= UPPER_BOUND:
            primes.add(prime_candidate)
    return list(primes)

dataset = generate_random_12_digit_primes(1000)

with open("data/input/primes_12_digit.txt", "w") as f:
    for prime in dataset:
        f.write(f"{prime}\n")
