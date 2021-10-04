import random

for i in range(10):
    op = random.choice(['R','W'])
    addr = hex(random.randrange(0, 0xfff, 0xff ))[2:]
    print(f'{op}:{addr}')