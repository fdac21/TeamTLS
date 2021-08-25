# Enigma Group Project #1

## Student Information

* Student name: **Benjamin Greenberg**
* NetId: **bgreenb3**
* Enigma group profile: <https://www.enigmagroup.org/profile/66636>


## Project Description


In this project, you will be complete the following challenges on [Enigma Group](https://www.enigmagroup.org/pages/challenges): Basic 1–3 (starter) and 4–8 (JavaScript). After completing each challenge you will complete the following worksheet describing:
1. The credentials found in the challenge (if any).
2. How you solved the challenge. Include any JavaScript or other scripts you used to solve these challenges. 
3. What sins (as found in the *24 Deadly Sins* book) are evidenced in the challenge.
4. How those sins should be addressed to address your exploit.

You can find a basic guide for markdown formatting [here](https://www.markdownguide.org/basic-syntax/). In particular, note the [code section](https://www.markdownguide.org/basic-syntax/#code) which should be used to annotate any code or commands used in your writeup.


## Challenges

---
### Basic 1

#### Credentials
Password: 39f13b

#### How you found the credentials
Looked in the HTML file

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Put the password somewhere not client readable 
Also encrypt the password wherever it's stored 



---
### Basic 2

#### Credentials
Username: admin
Password: rosebud101z

#### How you found the credentials
Look in the directory from the url where the login script expected the password to be.

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Log the error somewhere on server and don't give the user too much information about the cause of the error
Also don't make the password file readable by users\
Also encrypt the password wherever it's stored 



---
### Basic 3

#### Credentials
Username: admin
Password: f0rkblork

#### How you found the credentials
1) Check robots.txt for where robots are directed
2) Use the login credentials stored at /f0rk/config.inc

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't give unauthorized users access to the password file
Don't specify where the password file is in a blacklist




--- 
### Basic 4

#### Credentials
N/A

#### How you passed the challenge
Change the value of one of the select options to "Jane"

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
If the login has to be protected with a drop down, make the value for each user a hash that can't be guessed so the login can't be broken just be editing the source code.



---
### Basic 5

#### Credentials
Password: skriptkid

#### How you found the credentials
Look at the javascript used for securing the login that has the pasword in plaintext

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Use a proper backend for authentication some secret key so the user can't look into the source code and just find the password in plaintext in the browser



---
### Basic 6

#### Credentials
Password: Sauc3

#### How you found the credentials
Look in the source code after the long comment

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Use a proper backend for authentication with some secret key so the user can't look into the source code and just find the password in plaintext in the browser



---
### Basic 7

#### Credentials
Password: shifted

#### How you found the credentials
1) Noticed the wrong password in the source code of the linked index page
2) Played around with the url till I realized it was misspelled 
3) Looked at the source code of the actual index.php and found the password in the source code there

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Use a proper backend for authentication with some secret key so the user can't look into the source code and just find the password in plaintext in the browser


---
### Basic 8

#### Credentials
Password: ASCII-Chart

#### How you found the credentials
1) Looked in the source code and found the makeshift encryption and decryption function in plaintext
2) Used the __unescape()__ function to decrypt the password

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Use a proper backend for authentication with some secret key so the user can't look into the source code and just find the password in plaintext in the browser
Also use a strong authentication scheme that can't be broken just by knowing the method used to encrypt the password


