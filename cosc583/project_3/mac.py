
# Make the registers global to keep state inbetween original hash and extended hash
h = [
    0x67452301,
    0xEFCDAB89,
    0x98BADCFE,
    0x10325476,
    0xC3D2E1F0,
]


def toBytes(data):
    return ''.join('{0:08b}'.format(ord(n)) for n in data)


def prepData(data):
    bytes = data
    bits = bytes+"1"
    pBits = bits

    # pad until length equals 448 mod 512
    while len(pBits) % 512 != 448:
        pBits += "0"

    # append the original length
    pBits += '{0:064b}'.format(len(bits) - 1)
    return pBits[len(bits)-1:]


def sha1(data):

    def chunks(l, n):
        return [l[i:i+n] for i in range(0, len(l), n)]

    def rol(n, b):
        return ((n << b) | (n >> (32 - b))) & 0xffffffff

    for c in chunks(data, 512):
        print("Chunk:", hex(int(c, 2))[2:].zfill(128))
        words = chunks(c, 32)
        w = [0]*80
        for n in range(0, 16):
            w[n] = int(words[n], 2)
        for i in range(16, 80):
            w[i] = rol((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1)

        a = h[0]
        b = h[1]
        c = h[2]
        d = h[3]
        e = h[4]

        # Main loop - SHA1
        for i in range(0, 80):
            if 0 <= i <= 19:
                f = (b & c) | ((~b) & d)
                k = 0x5A827999
            elif 20 <= i <= 39:
                f = b ^ c ^ d
                k = 0x6ED9EBA1
            elif 40 <= i <= 59:
                f = (b & c) | (b & d) | (c & d)
                k = 0x8F1BBCDC
            elif 60 <= i <= 79:
                f = b ^ c ^ d
                k = 0xCA62C1D6

            temp = rol(a, 5) + f + e + k + w[i] & 0xffffffff
            e = d
            d = c
            c = rol(b, 30)
            b = a
            a = temp

        h[0] = h[0] + a & 0xffffffff
        h[1] = h[1] + b & 0xffffffff
        h[2] = h[2] + c & 0xffffffff
        h[3] = h[3] + d & 0xffffffff
        h[4] = h[4] + e & 0xffffffff

    # Return the MAC and the length of the message
    return '%08x%08x%08x%08x%08x' % (h[0], h[1], h[2], h[3], h[4])


key = "0123456789abcdef"
keySize = len(key) * 8
m = "Send Tina $100"
m1 = toBytes(key + m)
temp = h[0]
# print(f"Original Message Hex: {hex(int(m1, 2))[2:]}")

#  Original MAC
m1 = m1 + prepData(m1)
mac = sha1(m1)

m2 = "Send Malory $1M"
mP = m1 + toBytes(m2)
print(f"Modified Message Hex: {hex(int(mP, 2))[2:]}")

# Malicious MAC
mP = mP + prepData(mP)


print(f"Final Message Hex: {hex(int(mP[keySize:], 2))[2:]}")
mac = sha1(mP[keySize:])
print(mac)
print(f"Final Message Hex: {hex(int(mP, 2))[2:]}")
