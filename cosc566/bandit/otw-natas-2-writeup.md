# OverTheWire Natas Project Part #2

## Student Information

* Student name: **enter name here**
* NetId: **enter NetId here**


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
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **



---
### Natas 15

#### Credentials
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **



---
### Natas 16

#### Credentials
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **



---
### Natas 17

#### Credentials
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **



---
### Natas 18

#### Credentials
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **



---
### Natas 19

#### Credentials
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **



---
### Natas 20

#### Credentials
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **



---
### Natas 21

#### Credentials
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **



---
### Natas 22

#### Credentials
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **



---
### Natas 23

#### Credentials
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **



---
### Natas 24

#### Credentials
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **



---
### Natas 25

#### Credentials
** Credentials here **

#### How you passed the challenge
** Steps here **

#### What sins are evidenced in this challenge
** Sins here **

#### How could those sins be mitigated
** Mitigations here **


