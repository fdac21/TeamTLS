from Crypto.Util.number import bytes_to_long, getPrime, isPrime, long_to_bytes
from binascii import hexlify

BIT_LENGTH = 1024


def extended_euclidian(a, b):
    px, py = 1, 0
    x, y = 1, 1

    while True:
        a, b = b, a % b
        q = a // b

        x, px = px - q * x, x
        y, py = py - q * y, y

        if b == 1:
            break

    return a, px, py


def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a


def generatePrime(bit_length):

    p = getPrime(bit_length)

    while (not (isPrime(p))):
        p = getPrime(bit_length)

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

    _, _, d = extended_euclidian(e, totN)

    m = "This is a secret test".encode()
    m = bytes_to_long(m)
    print(m)
    c = pow(m, e, n)

    m = pow(c, d, n)

    print(c, long_to_bytes(m).decode('utf-8'))


if __name__ == '__main__':
    main()
