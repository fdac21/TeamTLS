import random
import csv
from string import ascii_letters, digits
from hashlib import sha256
import numpy as np
import plotly.express as px


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


def collision() -> dict[int, list[int]]:
    bits = [8, 10, 12, 14, 16, 18, 20, 22]
    hashes = []
    bits = {size: [] for size in bits}
    tries = 0

    for size in bits.keys():
        for i in range(75):
            base = ranString(25)
            res = sha256w(base, size)
            hashes = [res]

            while True:
                tries += 1

                alt = ranString(25)
                test = sha256w(alt, size)
                if test in hashes:
                    break
                else:
                    hashes.append(test)

            bits[size].append(tries)
            tries = 0
    return bits


def preImage() -> dict[int, list[int]]:
    bits = [8, 10, 12, 14, 16, 18, 20, 22]
    bits = {size: [] for size in bits}
    tries = 0

    for size in bits.keys():
        for i in range(75):
            base = ranString(25)
            res = sha256w(base, size)
            while True:
                tries += 1
                alt = ranString(25)
                test = sha256w(alt, size)
                if res == test:
                    break
            # print(
            #     f"Pre-image attack with bit length {size} took {tries} tries")
            bits[size].append(tries)
            tries = 0

    return bits


col_trials = collision()
pre_trials = preImage()

with open('trial_results.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(['Collision Attack Results for 75 Trials'])
    writer.writerow(['Bit Size', 'Avg Iterations'])
    for size, results in col_trials.items():
        writer.writerow([size, np.mean(results), *results])
        print(
            f"Collision avg for bit size {size}: {np.mean(results)}")

    writer.writerow(['Pre-Image Attack Results for 75 Trials'])
    writer.writerow(['Bit Size', 'Avg Iterations'])
    for size, results in pre_trials.items():
        writer.writerow([size, np.mean(results), *results])
        print(
            f"Pre-image avg for bit size {size}: {np.mean(results)}")


px.violin()
