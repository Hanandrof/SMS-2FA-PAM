#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <unistd.h>
#include <curl/curl.h>
#include <sys/wait.h>

/*
Author: Alex Cochrane, Emilson Jose, Zachary Harris
*/


/* ************************************************************** 
 * Converse function provided by Ben Servoz which is cited to be  
 * from pam_unix/support.c					     
 *
 * Allows for conversations to happen between the user and the 
 * PAM module
 * ************************************************************** */
 
int converse( pam_handle_t *pamh, int nargs, struct pam_message **message, struct pam_response **response ) {
	int retval ;			//define retval
	struct pam_conv *conv ;	//This is the pam conversation function provided by PAM

	retval = pam_get_item( pamh, PAM_CONV, (const void **) &conv ) ; 
	//Setting retval to the return value of pam_get_item which allows pam modules to recieve
	//item types
	if( retval==PAM_SUCCESS ) {
		retval = conv->conv( nargs, (const struct pam_message **) message, response, conv->appdata_ptr ) ;
		//sets conv to the conv structure/function from above sets output to retval.
	}

	return retval ;
}

/* ************************************************************** 
 * Converse function provided by Ben Servoz which is cited to be  
 * from pam_unix/support.c					     
 *
 * Allows for conversations to happen between the user and the 
 * PAM module
 * ************************************************************** */

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *handle, int flags, int argc,
				   const char **argv)
{
	int pam_code;				//similar to retval (used to get int return value from PAM functions)
	int retval;				//reference above
	
	char *input ;				//input character pointer
	struct pam_message msg[1],*pmsg[1];	//building pam_message structures (used in conversation)
	struct pam_response *resp;		//building pam_response structures (used in conversation)
	const char *username = NULL;		//init username to null

	/* Asking the application for an  username */
	pam_code = pam_get_user(handle, &username, "USERNAME: ");	//PAM function prompts for username 
	if (pam_code != PAM_SUCCESS) {
		fprintf(stderr, "Can't get username");
		return PAM_PERM_DENIED;
	}

	/* Generating code to be sent */
	srand(time(0));			//Initialize timer for random code
	int code = (rand() % 9000)+1000;	//Make random 4 digit code
	char codeString [4];			
	snprintf(codeString, 5, "%d", code);	//Make character array with our code
	
	/* Sending code */
		
	char *cmd = "/usr/bin/curl2FA";	//We are going to use execvp to run this command
						//Which is why we stored curl2FA in root bin
	char *args[3];				//Create array
	args[0] = "curl2FA";
	args[1] = codeString;
	args[2] = NULL;
	pid_t pid = fork();			//PAM does not like us using its process to execute execvp
						//So we will create a child process that will run it for us
	if(pid==0){				//Child process runs command
	  execvp(cmd, args);			
	  exit(0);				//Child process exits
	}else{
	  wait(NULL);				//Parent process waits for child to exit
	}

	/* Asking the user for our generated code */
	
	/* -------------------------------------------------------------- 
	   Uses Ben Servoz message setup. Ben also showed how to get 
	   user input and how to compare with a code compliant with PAM. 
	   --------------------------------------------------------------*/
	
	pmsg[0] = &msg[0] ;				//set pmsg to the address of msg
	msg[0].msg_style = PAM_PROMPT_ECHO_ON ;	//This allows for us to see what we are typing when we authenticate
	msg[0].msg = "1-time code: " ;		//This is the message that is going to be displayed for the user
	resp = NULL ;
							//Below calls converse function to interact with user
	if( (retval = converse(handle, 1 , pmsg, &resp))!=PAM_SUCCESS ) {
		//If this part runs then something went wrong
		return retval ;
	}
	
	/* retrieving the user input */
	if( resp ) {					//If the above call was successful
		if( (flags & PAM_DISALLOW_NULL_AUTHTOK) && resp[0].resp == NULL ) {
			//if we put in flags dissallowing empty authentication token and the response was null
	    		free( resp );			//We free resp
	    		return PAM_AUTH_ERR;		//and return an error
		}
		input = resp[ 0 ].resp;		//set our input variable to our response
		resp[ 0 ].resp = NULL;			//Set response to Null
    	} else {
		return PAM_CONV_ERR;			//If not successful return error
	}
		
	/* comparing user input with known code */
	if( strcmp(input, codeString)==0 ) {		//compares our input and the codeString
		/* good to go! */
		free( input ) ;			//frees our input
		return PAM_SUCCESS ;			//returns success
	} else {
		/* wrong code */
		free( input ) ;			//frees our input
		return PAM_AUTH_ERR ;			//returns error
	}
}
