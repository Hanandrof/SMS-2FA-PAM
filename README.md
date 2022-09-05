# Two Factor Authentication PAM Module

Group 6 has spent the 2021 Spring Semester building a PAM module that will act as two factor authentication for the common-authentication process in PAM. This PAM module was originally meant to be linked to an RFID card reader, but due to implementation issues we opted to scrap the RFID card reader and moved to understanding the PAM module and getting proper 2FA working. 

In order to achieve this we use the libcurl library to send us a message with a code to our phones. We then input this into our authentication method which will then work with the PAM module to determine if the code is right. The PAM module will then interact with the OS to continue the authentication process.

# UPDATE
**As of 4/30/2021 the login function is no longer working, if you want to use this project please make sure you run in a copy of your VM or make sure you keep common-auth open. We cannot guarantee if you log out that your ubuntu version will authenticate you on log in. It seems it works with the terminal and locking your machine, but logging out or switching users does not work ever since the phone number was updated by Twilio.**

# Installation
### Dependencies
This project relies on [libcurl](https://curl.se/) and [PAM](http://www.linux-pam.org/) which have to be installed for this project to run properly. Run the below commands to properly install the dependencies.
```bash
$ sudo apt-get update
$ sudo apt-get install build-essential libpam0g-dev libcurl4-openssl-dev
```
### Building Program
First we need to change the phone number in 2fa/curl.c. On the 9th line you will see a definition of a phone number, change this to your number.
Input needs to be in the specified format. If you have the phone number **123-456-7890**, for example, then you will need to write on line 9 ...
```C
#define to_number "+11234567890"
```

Next we will preview the makefile, please make sure the paths are correct, you may have to change them for your specific system. 
**pam_2fa.so** has to go in the security folder in /lib. This path for this folder is generally **/lib/x86_64-linux-gnu/security**,

**curl2FA** will have to go in your /bin
After you have confirmed the paths are correct for your system please run
```bash
$ make
```
to build the program.

Finally we must navigate to pam.d to set up authentication. Go to /etc/pam.d by running
```bash
$ cd /etc/pam.d
```
Next we are going to open **common-auth** to add our PAM module
```bash
$ sudo gedit common-auth
```
You do not need to use gedit, but I suggest keeping common-auth open in a seperate text window so you can save and change the authentication procedure just in case something goes wrong.

Once you open **common-auth** you will add a line to reference the pam module that we just built and placed in our security folder. At the top of the command stack above the reference to **pam_unix.so** please include the below line
```text
auth    required            pam_2fa.so
```
After this you should be done

For safety I would not close **common-auth** until positive that the program works for you. If you find this program does not work, you can always comment out or delete the code we added in **common-auth** to return to the original authentication process.

### Testing/Results
To test this program open a new terminal. In this run the command 
```bash
$ sudo echo hello
```

You should be prompted for a 1-time code and then you should be prompted for your password similar to the manner below.
```text
1-time code:
[sudo] password for user:
```
Input the 1 time code you receive and then input your password.
    * If this does not happen, please delete the reference to **pam_2fa.so** in **common-auth** for safety reasons. 

# Contributions
This project was built and authored by Alex Cochrane, Emilson Jose, and Zachary Harris of Group 6.

##### Alex Cochrane
* Compiled the code for the PAM Module
* Bought a [Twilio](https://www.twilio.com/docs) phone number to send messages to the user
* Built the PAM Module
* Regularly communicated with Dr. Tessler
* Reviewed deliverables and implementation
##### Zachary Harris
* Built the telephone code **(not yet implemented)**
* Compiled various resources and tutorials for PAM Modules
* Got examples of PAM working to present to group in order to take inspiration for our own project
* Built our Gantt chart  
* Managed project schedule and kept group on track
* Reviewed deliverables and implementation
##### Emilson Jose
* Compiled various resources and tutorials for PAM Modules
* Got examples of PAM working to present to group in order to take inspiration for our own project
* Completed bulk of documentation 
* Reviewed code and helped find various errors
* Reviewed deliverables and implementation
# External Sources
This project is the culmination of many PAM modules. We have compiled a list of the modules we used code from and based our code from
### [Google Authenticator](https://github.com/google/google-authenticator-libpam)
* Purpose:  This project is meant to act as two factor authentication for your machine, after setup in order to login you will have to input a code that you receive through the [Google Authenticator App](https://play.google.com/store/apps/details?id=com.google.android.apps.authenticator2&hl=en_US&gl=US)
* Use:      This project taught us how to make the login screen and that we had to get pam_items in order to interact with the users.

### [Fedetask Tutorials](https://github.com/fedetask/pam-tutorials)
* Purpose:  This project is meant to be a simple module that acts with a simple application to verify the user
* Use:      This project taught us the overall layout of the PAM module, how pam variables were used and the different functions of PAM

### [Simple-PAM](https://github.com/beatgammit/simple-pam)
* Purpose:  This project is meant to be a simple pam module that acts as a tutorial for PAM.
* Use:      This was the first pam module outside of Google Authenticator that worked alongside common-auth. Thus, we used this as a skeleton.

### [2 Factor Authentication & PAM Modules](https://ben.akrin.com/?p=1068)
* Purpose:  This project uses PAM, CURL, and PHP to send a code to the user and they will then authenticate it in ssh
* Use:      This project was very similar to what we wanted to do after we left the RFID card reader behind. We did not want to use PHP but we started out with the Simple-PAM module and added/changed functions from this module to create our own module.
