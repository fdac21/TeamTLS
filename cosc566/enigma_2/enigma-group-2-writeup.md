# Enigma Group Project #2

## Student Information

* Student name: **Benjamin Greenberg**
* NetId: **bgreenb3**
* Enigma group profile: <https://www.enigmagroup.org/profile/66636>


## Project Description

In this project, you will be complete the following challenges on [Enigma Group](https://www.enigmagroup.org/pages/challenges): Basic 16–17 (spoofing) and 21–23 (SQL injection). After completing each challenge you will complete the following worksheet describing:
1. The credentials found in the challenge (if any).
2. How you solved the challenge. Include any JavaScript or other scripts you used to solve these challenges.
3. What sins (as found in the *24 Deadly Sins* book) are evidenced in the challenge.
4. How those sins should be addressed to address your exploit.

You can find a basic guide for markdown formatting [here](https://www.markdownguide.org/basic-syntax/). In particular, note the [code section](https://www.markdownguide.org/basic-syntax/#code) which should be used to annotate any code or commands used in your writeup.


## Challenges

---
### Basic 16

#### Credentials
N/A

#### How you passed the challenge
I searched for a random transparent proxy and set my computer's proxy to that IP address and port

#### What sins are evidenced in this challenge
Sin #12
The website also specifies how the server detects that the proxy being used is the required type
The server also trusts any random proxy that the user is using as long as it is the correct type of proxy

#### How could those sins be mitigated
Don't give the user too much information and also maybe use a specific list of proxies if necessary



---
### Basic 17

#### Credentials
N/A

#### How you passed the challenge
```zsh
curl http://challenges.enigmagroup.org/basics/spoof/2/ -A "EnigmaFox" --cookie "PHPSESSID=hb948026e0k0vudle7vhpfldl0"
```
I used curl to set the user agent to ```EnigmaFox```

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't give out too much information in the error message



---
### Basic 21

#### Credentials
N/A

#### How you passed the challenge
I input the following sql statement after a few tries and realizing the sql syntax error was after my string, so I put the '--' at the end to force the command to end.
```sql
SELECT * FROM users WHERE username='' OR '1'='1' --;
```

#### What sins are evidenced in this challenge
Sin #1

#### How could those sins be mitigated
Use parametrized sql queries instead of string concatenation.



--- 
### Basic 22

#### Credentials
Username: administrator
Password: bl1nd

#### How you found the credentials
1) First I tested which id gave me an error page 
2) Then I tested the number of columns in news articles with ```order by <n>``` until I got an error with n being the number of columns 
3) I tried seeing what happened when I got an error with the columns filled out and saw that the title and description got replaced with the second and third columns
4) Then I replaced the second and third columns with the username and password columns from users while guessing that the table would be similar to the table from Basic 21 
http://challenges.enigmagroup.org/basics/sql/2/index.php?id=4%20union%20select%201,%20username,%20password%20from%20users;
5) Discovered the password didn't work so I stuck it in a MD5 decryptor to get the actual password 

#### What sins are evidenced in this challenge
Sin #1

#### How could those sins be mitigated
Use parametrized sql queries instead of string concatenation.



---
### Basic 23

#### Credentials
Username: administrator
Password: asdfgh

#### How you found the credentials
Same as above, except I used ```UNION ALL``` instead of ```UNION``` to get around the signature check

http://challenges.enigmagroup.org/basics/sql/3/index.php?id=5%20union%20all%20%20select%201,%20username,%20password%20from%20users;

#### What sins are evidenced in this challenge
Sin #1

#### How could those sins be mitigated
Use parametrized sql queries instead of string concatenation.



