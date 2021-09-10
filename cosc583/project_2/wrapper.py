from hashlib import sha256
import os
import random
from string import ascii_letters, digits

ALPHANUM: str = ascii_letters + digits


def hex2bin(data: hex) -> bin:
    return bin(int(data, 16))


def truncateHex(data: hex, bits: int = 256) -> hex:
    return hex(int(hex2bin(data)[0:bits+2], 2))


def ranString(length: int = 16) -> str:
    return ''.join(random.choice(ALPHANUM) for i in range(length))


def sha256w(data: str = "", bits: int = 256) -> str:

    sha = sha256(data.encode())
    enc = truncateHex(sha.hexdigest(), bits)
    return enc


def collision() -> None:
    bits = [8, 10, 12, 14, 16, 18, 20, 22]
    tries = 0

    for bit in bits:
        
        while True:
            tries += 1
            alt = ranString(25)
            base = ranString(25)
            res = sha256w(base, bit)
            test = sha256w(alt, bit)
            if res == test:
                break
        print(
            f"Bit length {bit} took {tries} tries to collide with strings {base} and {alt}")
        tries = 0


def preImage() -> None:
    bits = [8, 10, 12, 14, 16, 18, 20, 22]
    tries = 0

    for bit in bits:
        base = ranString(15)
        res = sha256w(base, bit)
        while True:
            tries += 1
            alt = ranString(15)
            test = sha256w(alt, bit)
            if res == test:
                break
        print(
            f"Pre-image attack with bit length {bit} took {tries} tries")
        tries = 0


collision()
preImage()

# sha256w("TEST", 32)
