from Crypto.Util.number import getPrime, isPrime
from random import randint, randrange


def fastExp(b, e, n):
    product = 1

    while e > 0:
        if e & 1:
            product = (product * b) % n
        b = (b * b) % n
        e >>= 1

    return product


BIT_LENGTH = 1024
p = getPrime(BIT_LENGTH)

while (not (isPrime(p) and isPrime((p - 1) // 2))):
    p = getPrime(BIT_LENGTH)

a = int("deadbeef", base=16)
g = 5

pub_key = fastExp(g, a, p)

print("Prime Modulus: ", p)

print("Public Key: ", pub_key)
