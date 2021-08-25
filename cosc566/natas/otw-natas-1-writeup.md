# OverTheWire Natas Project Part #1

## Student Information

* Student name: **Benjamin Greenberg**
* NetId: **bgreenb3**


## Project Description

In this project, you will be complete the Natas challenges at <https://overthewire.org/wargames/natas/>. In this part of the lab, you will complete challenges Natas 0–Natas 13. For each challenge, you will complete the following worksheet describing:
1. The credentials found in the challenge.
2. How you solved the challenge. Include any JavaScript or other scripts necessary to retrieve the credentials.
3. What sins (as found in the *24 Deadly Sins* book) are evidenced in the challenge.
4. How those sins should be addressed to address your exploit.

In the workshop, the challenges are named based on the username used to login to the shell for that challenge. For example, the challenge labeled "Natas 10" will be logged into with the username natas10, and the solution will be the credentials for the user Natas11.

You can find a basic guide for markdown formatting [here](https://www.markdownguide.org/basic-syntax/). In particular, note the [code section](https://www.markdownguide.org/basic-syntax/#code) which should be used to annotate any code or commands used in your writeup.


## Hints
* Natas 11: `plaintext ^ key` gives the ciphertext. What does `plaintext ^ ciphertext` equal?


## Challenges

---
### Natas 00

#### Credentials
Password: gtVrDuiDfck831PqWsLEZy5gyDz1clto 

#### How you passed the challenge
1) Look in source code

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Put the password somewhere not client readable 
Also encrypt the password wherever it's stored 



---
### Natas 01

#### Credentials
Password: ZluruAthQk7Q2MqmDeTiUij2ZvWy2mBi

#### How you passed the challenge
1) Open devtools and look at the source code

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Put the password somewhere not client readable 
Also encrypt the password wherever it's stored 



---
### Natas 02

#### Credentials
Password: sJIJNW6ucpu6HPZ1ZAchaDtwd7oGrD14

#### How you passed the challenge
1) I looked in the source code and found that there was a ```files``` directory
2) I looked at the ```users.txt``` file in the files directory

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Put the password somewhere not client readable 
Also encrypt the password wherever it's stored 



---
### Natas 03

#### Credentials
Password: Z9tkRkWmpt9Qr7XrR5jWRkgOU901swEZ

#### How you passed the challenge
1) I looked at the source code and saw the comment saying Google (webcrawlers) couldn't find the secret
2) I looked in the robots.txt file and saw that there was a ```s3cr3t``` directory
3) I found the ```users.txt``` file in the ```s3cr3t``` directory
   
#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Put the password somewhere not client readable 
Also encrypt the password wherever it's stored 



---
### Natas 04

#### Credentials
Password: iX6IOfmpN7AYOQGPwtn3fXpbaJVJcHfq

#### How you passed the challenge
1) Use BURP suite's browser to access natas4 and get to the index.php
2) Intercept the refresh request and change the referrer to ```natas5.natas.labs.overthewire.org/```

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't leak too much information
Don't trust user input
Put the password somewhere not client readable 
Also encrypt the password wherever it's stored 


---
### Natas 05

#### Credentials
Password: aGoY4q2Dc6MgDq4oL4YtoKtyAg9PeHa1

#### How you passed the challenge
1) Checked in the cookies and noticed that there was a cookie unlike in natas4
2) Changed the cookie ```loggedin``` to ```1```

#### What sins are evidenced in this challenge
Sin #4, Sin#12

#### How could those sins be mitigated
Put the password somewhere not client readable 
Don't trust user input 
Also encrypt the password wherever it's stored 



---
### Natas 06

#### Credentials
Password: 7z3hEENjQtflzgnT29q7wAvMNfZdh0i9

#### How you passed the challenge
1) I clicked on the very friendly ```View sourcecode``` link and I saw that the input is directly compared to the secret in ```includes/secret.inc```
2) I went to ```includes/secret.inc``` and copied the secret ```FOEIUWGHFEEUHOFUOIU``` into the input to get the password

#### What sins are evidenced in this challenge
Sin #12, Sin #17

#### How could those sins be mitigated
Use a proper backend for authentication 
Don't make secret files readable by unauthorized users
Put the password somewhere not client readable 




---
### Natas 07

#### Credentials
Password: DBfUBfqQG69KvJvJ1iAbMoIpwSNQ9bWe

#### How you passed the challenge
1) I went to About and then tried putting a bad link in the page parameter in the URL
2) I looked at the source code and saw the helpful hint about the password's location
3) I set ```page=/etc/natas_webpass/natas8``` and got the password

#### What sins are evidenced in this challenge
Sin 2, Sin #12

#### How could those sins be mitigated
Properly restrict read/write/execute access to sensitive information
Don't give away the location of the password in the source code



---
### Natas 08

#### Credentials
Password: W0mMhUcRRnG8dcghE4qvk3JA9lGt8nDl

#### How you passed the challenge
1) I clicked on the very friendly ```View sourcecode``` link and I saw that the input is directly compared to the "encrypted" secret that happens to be in the source code
2) I ran the following command to "decrypt" the secret and input the secret to get the password
3) ```echo "3d3d516343746d4d6d6c315669563362" | xxd -r -p | rev | base64 -d```

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Use a proper backend for authentication
Put the password somewhere not client readable 
Use proper encryption



---
### Natas 09

#### Credentials
Password: nOpp1igQAkUzaI1GUUjzn1bFVj7xCNzu

#### How you passed the challenge
1) I looked at the source code and saw that the script directly puts ```$needle``` into the grep command
2) I input ```; cat /etc/natas_webpass/natas10;``` into ```$needle``` and got the password

#### What sins are evidenced in this challenge
Sin #10, Sin #12

#### How could those sins be mitigated
Properly validate and verify user input
Don't directly input user input into commands



---
### Natas 10

#### Credentials
Password: U82q5TCMMQ9xuFoI3dYX61s7OZD9JKoK

#### How you passed the challenge
1) I looked at the source code and saw that the script directly puts ```$needle``` into the grep command, but this time I saw that it won't accept strings with ';' or '&' in it
2) I input ```-E [[:alnum:]]  /etc/natas_webpass/natas11``` into ```$needle``` and got the password

#### What sins are evidenced in this challenge
Sin #10, Sin #12

#### How could those sins be mitigated
Don't rely on blacklist for validating input



---
### Natas 11

#### Credentials
Password: EDXp0pS26wLKHZy1rDBPUZk0RKfLGIR3

#### How you passed the challenge
1) I looked at the source code and saw that xor_encrypt was used in the same step regardless if it was for encryption or decryption, so I wrote a dummy xor_encryption with the data for the default cookie as the key to get the key used for encryption
2) I noticed that the result I got wasn't working but it did follow a pattern so I just manually edited the key I got to finish the pattern
3) Then I created input data with ```showpassword: yes``` as the input and the key as what I obtained in the previous step and I got the website to show the password

```xor.php```
```php
<?php
$cookie = "ClVLIh4ASCsCBE8lAxMacFMZV2hdVVotEhhUJQNVAmhSEV4sFxFeaAw=";

function xor_enc($data) {
    // $key = json_encode(array("showpassword"=>"no", "bgcolor"=>"#ffffff"));
    $key = "qw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8J";
    $out = '';

    for ($i=0;$i<strlen($data); $i++) {
        $out .= $data[$i] ^ $key[$i % strlen($key)];
    }

    return $out;
}

printf("%s\n", base64_encode(xor_enc(json_encode(array("showpassword"=>"yes", "bgcolor"=>"#ffffff"))));
// printf("%s\n", xor_enc(base64_decode($cookie)));

?>
```

#### What sins are evidenced in this challenge
Sin #12

#### How could those sins be mitigated
Don't roll your own crypto
Don't show your crypto methods in the source code



---
### Natas 12

#### Credentials
Password: jmLTY0qiPZBbaKc9341cqPQZBJv7MQbY

#### How you passed the challenge
1) I looked through the source code and tried uploading a few different files
2) I saw that while the filename is changed, the extension in the hidden filename field is preserved so I changed the extension of the filename to php
3) I uploaded the following script to display the password at the link of the uploaded file

```image.php```
```php
<?php
    system("cat /etc/natas_webpass/natas13");
?>
```

#### What sins are evidenced in this challenge
Sin #4, Sin #12, Sin #16

#### How could those sins be mitigated
Properly verify contents of uploaded file
Don't rely on hidden input fields that can be changed
Don't make uploaded files executable


---
### Natas 13

#### Credentials
Password: Lg96M10TdfaPyVBkJdjymbllQ5L6qdl1

#### How you passed the challenge
1) I followed the same steps as Natas 12, except I prepended the file with four characters
2) Then I opened the file with the hex editor in VSCode and replaced those four characters with the magic number for JPEG files that ```exif_imagetype``` was looking for 

##### Magic Number
````0xFF 0xD8 0xFF 0xE0```

#### What sins are evidenced in this challenge
Sin #4, Sin #12, Sin #16


#### How could those sins be mitigated
Properly verify contents of uploaded file
Have multiple layers of verification and not just a simple check that only checks the first few bytes
Don't rely on hidden input fields that can be changed
Don't make uploaded files executable


