#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <time.h>
#include <unistd.h>

//CHANGE BELOW TO YOUR PHONE NUMBER FOLLOW FORMAT "+11234567890"
//important to include "+1"
#define to_number "+11234567890"


/* ************************************************************** 
 * Function that makes it so we do not output the HTML POST 
 * request to the screen
 * Provided by TWILIO. Without this I managed to get my account
 * suspended.
 * ************************************************************** */
size_t _twilio_null_write(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    return size * nmemb;	//returns size * nmemb
    				//size is one data item, nmemb is the number of data items
}

/* ************************************************************** 
 * Stripped the unnecessary code from twilio_send_message
 * ************************************************************** */
int main(int argc, char *argv[]){

  
	char * account_sid = "ACCOUNT SID";	//Account_sid
	char * auth_token = "AUTH TOKEN";	//Auth_token
	char * from_number = "TWILIO PHONE NUMBER";				//from_number
	//char * to_number = "";


	CURL *curl = curl_easy_init();				//Initializes the curl library using datatype CURL

	//CURLcode res;							//initializes res using datatype CURLcode, This is 
									//the response from CURL
	/*Build our URL*/
	char url[10000];
	snprintf(url,							//We are using the twilio API to make this request
		 sizeof(url),						//Pushing to URL three strings
		 "%s%s%s",
		 "https://api.twilio.com/2010-04-01/Accounts/",	//url of accounts for twilio
		 account_sid,						//the SID
		 "/Messages");						//Messages
		 							//this builds our url for the HTTP to interact with
	/*Set parameters*/
	char parameters[10000];
	snprintf(parameters,						//Pushing to parameters 6 parameters
		sizeof(parameters),
		"%s%s%s%s%s%s",
		"To=",							//init To=
		to_number,						//our to_number which is defined above
		"&From=",						//init From=
		from_number,						//our from number
		"&Body=",						//init Body
		argv[1]);						//This is the message we pass through execvp arguments
	/*Set options for our HTTP POST Request*/
	curl_easy_setopt(curl, CURLOPT_POST, 1);			//This requests an HTTP Post request
        curl_easy_setopt(curl, CURLOPT_URL, url);			//This is the URL that we will be talking with
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, parameters);	//This is the data being posted to server
        curl_easy_setopt(curl, CURLOPT_USERNAME, account_sid);	//This is our username for authentication
        curl_easy_setopt(curl, CURLOPT_PASSWORD, auth_token);	//This is our password for authentication
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, 		//This is our write function which will store the data recieved
        _twilio_null_write);						//from the URL
        
        curl_easy_perform(curl);					//Perform the functions of HTTP POST Request
        
        								//returns Error or Ok to res
        curl_easy_cleanup(curl);					//Ends libcurl
        
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);//Gets the information from the response
  return 0;
}
