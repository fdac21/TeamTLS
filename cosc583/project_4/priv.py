from hashlib import sha256
from Crypto.Cipher import AES

PRIVATE_KEY = 17638896813801928022731132934844404084539086171483557753476517230027256874738746536786700212991334054063383360364019276091339973205274226688764935043814035479934782097346179269565484465551073164742747955268442565301797682964416848439268471336422068044033705992886104333856251562414302960735860161763956831306

CIPHERTEXT = bytes.fromhex("f645ac3bc56888211a987e3cc5e2e0455283378156d9b95a3b1c40a34cb0574350d649832c4fe88317b2428bbbf42817a1105195449b7e46f085adc75f35f41fbcd0128524d48ce41776940d7adc42e7fb5de303ca7862f408fd3b5baf89fb056fc67ef90dd2f50583cd6ec20d95b7d1f400686702c3365d04819af112cc259eb867a3173b788b0ca98b56dbf1641fc57ed29944dc5a719d152560d3b8df3874102b40f6fee5a7c83078cdc0c9e033da1635a9b1f3d939b65d972fb7c99e1ecb7444c683ef14a48739c346d95d158638ccbfb419351548c937df026eb57ca08d0309149966f34cc9d3da574623ed92334c074a9c6e4868a1908e4637002ddf5e702597935350bd71d6fdfd6f39225a5896eb29b645b4d23fa136721fda7eae2603714aad67c0dcc1451619f778d757d41c6a18889508eb38636607e33167794b42e2dc0e5b60de9a15d221c61bc1931996834dbcd2f83618fa71595504159d34347b687295447ff34f691b0fa0b43fa22c57736c41d08fba965790727ea627f8017e0540878c090052abe9696818dec98a0ce063a0a68ee67f32d4628a6689fb46a2c8c87910dcf817dced63a516b6c61dd7c494fa2fca621fd1d6e380f0ee90")

NONCE = bytes.fromhex("c2653f31a6f36f84fbc1b0a62da9d5bc")

p = 115040858006026030579795419251176149034690225572862270931409304880261403870047401718469843837689557215488730089736239649629696711244938105201102006573063629934949655118773599203840476562360352663640153014010475775566585681817775169753095468596534736132984063301647020244818633501783354909689266641211908638423


def fastExp(b, e, n):
    product = 1

    while e > 0:
        if e & 1:
            product = (product * b) % n
        b = (b * b) % n
        e >>= 1

    return product


a = int("deadbeef", base=16)

shared_key = fastExp(PRIVATE_KEY, a, p)

enc_key = sha256(shared_key.to_bytes(128, 'big')).digest()[:16]


print("Shared Key: ", shared_key)

decipher = AES.new(enc_key, AES.MODE_CBC, iv=NONCE)

print(decipher.decrypt(CIPHERTEXT).decode('utf-8'))