
def rev():
    output = "picoCTF{"
    with open("rev_this", "rb") as f:
        bits = f.read()
        for i, bit in enumerate(bits[8: len(bits) - 1]):
            if((i & 1) == 0):
                bit -= 0x05
            else:
                bit += 2
            output += chr(bit)
    output += "}"

    print(output)


if __name__ == '__main__':
    rev()
