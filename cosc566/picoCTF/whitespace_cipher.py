
def whitespace_cipher():
    with open("whitepages.txt", "r") as f:
        ws = f.read()

        string = ""
        char = ""

        for i, c in enumerate(ws):
            bit = "1" if c is " " else "0"
            char += bit
            if((i + 1) % 8 == 0):
                string += chr(int(char, 2))
                char = ""

        print(string)


if __name__ == '__main__':
    whitespace_cipher()
