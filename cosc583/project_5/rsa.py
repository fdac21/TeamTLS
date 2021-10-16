from Crypto.Util.number import bytes_to_long, getPrime, isPrime, long_to_bytes

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


def gcdExtended(a, b):
    # Base Case
    if a == 0:
        return b, 0, 1
    c = b % a
    if (a != 0 and c != 0 and a % c != 0):

        print(f'{a} = {c} * {a // c} + {a % c}')

    gcd, x1, y1 = gcdExtended(b % a, a)

    # Update x and y using results of recursive
    # call
    x = y1 - (b//a) * x1
    y = x1

    return gcd, x, y


def main():
    es = range(3, 6)
    e = 0
    while True:

        # p = generatePrime(BIT_LENGTH)
        # q = generatePrime(BIT_LENGTH)
        p, q = 13, 17
        n = p * q
        totN = (p - 1) * (q - 1)
        for i in es:
            e = i
            if gcd(e, totN) == 1:
                break
        if e != 0:
            break
    _, _, d = extended_euclidian(e, totN)
    gcdExtended(e, totN)

    print("Prime p: ", p)

    print("Prime q: ", q)

    print("E: ", e)

    print("RSA Modulus n: ", n)

    print("Euler's Totient φ(n): ", totN)

    print("RSA Private Exponent d: ", d)

    m = "monopolize".encode()
    m = bytes_to_long(m)
    e = 65537

    d = 6311129733497464152370570200677113516452348915379904305876535882031806305347109765126960086863452294777688118151290442942980521565176676782611016553936274506696849462370216648090135545032558807460412825482842903732990045864100180702489403353606382400902782169378021351111777232685830733777846597590814188842456968218422148378057331493195802345995197232315171105172625165390330913831578751064212359739342782927084812860734070358413272884428992588238062684244142213786460484730544292926647772936200297921060706815943375375625222490551361572747418260465038132538966073773105629773457831917827754942813045374348672441985
    n = 12700746463926282262295340515929987979111269141658563793349829027228351342920024954772633520013820335406416084237582809038694173119725599284590621872361408289178634871195630057909574808536473824373060103901893858071269687274935010216147148178631133188232071333124343956513312795508545378603412530440004590498540324585386536323043245706924498864190324033930580956963397421524161026688827015020991926657755731990722255141119035039531616614206701983676786781269502630173901222469499526129576776863431288845533604167295485465387089349811857691842533289300642992464901336676536111923859991313667987090381183360856009323829

    c = pow(m, e, n)
    c = 9508749600388752613253576939167956157825381268003266926551919167628557010155069887277784973900234548590700885766687445280590314383690669317546702715481305495912395604540676446588460572411153065028954003688754661647904095435125625231169242157397784915650547021432904620596635640210257589350820283512524035983401980523577722976978687389404323938225915333044586826865613951109241073408233600322948413720491699824364338426443579917779037081993626434710779292984433497214615213769754686564643282671372510598427757248938006662156523048028449465147672634281491248465887143259016033304046746740716416294925846218061075422362

    m = pow(c, d, n)

    print("CipherText c: ", c)
    print("Decrypted Plaintext m: ", long_to_bytes(m).decode('utf-8'))


if __name__ == '__main__':
    # main()
    # a, b = 40, 17
    print(pow(807504161352, 112677787929391516643265970286600007391, 112677787929391516643265970286600007391)
          )
    # print(807504161352, 112677787929391516643265970286600007391)
    # print(a, b)
    # ex = list(gcdExtended(a, b))
    # ex[-1] = ex[-1] if ex[-1] > 0 else a + ex[-1]
    # # print(extended_euclidian(a, b))
    # print(ex)
