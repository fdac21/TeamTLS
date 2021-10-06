from Crypto.Util.number import getPrime, isPrime
from math import gcd

BIT_LENGTH = 1024


def fastExp(b, e, n):
    product = 1

    while e > 0:
        if e & 1:
            product = (product * b) % n
        b = (b * b) % n
        e >>= 1

    return product


def EEGCD(a, b):
    s, t = 0, 0

    r = [(a, b)]
    while t != 1:
        r.append()


def generatePrime(bit_length):

    p = getPrime(bit_length)

    # while (not (isPrime(p) and isPrime((p - 1) // 2))):
    #     p = getPrime(bit_length)

    return p


def main():
    e = 65537

    while True:

        p = generatePrime(BIT_LENGTH)
        q = generatePrime(BIT_LENGTH)
        n = p * q
        totN = (p - 1) * (q - 1)
        if gcd(e, totN) == 1:
            break

    print(gcd(e, totN))


if __name__ == '__main__':
    main()
