#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	
	/*
	printf("Gen Code\n");
	srand(time(0));
	int code = (rand() % 9000)+1000;
	char codeString [4];
	snprintf(codeString, 5, "%d", code);
	char * code_string_flag = "-m ";*/
	/*
	strcpy(code_string_final,codeString);*/
	/*
	char * account_sid = "-a \"AC1b5d426f372db3ebf59e12daacf31e6a\"";
	char * auth_token = "-s \"51656ea60642eed3b734f9a0dd2f68f1\"";
	char * from_number = "-f \"+14102641778\"";
	char * to_number = "-t \"+14439261073\"";
	char * quote = "\"";
	
	char *cmd = "/usr/bin/c_sms";
	
	char *args[10];
	args[0] = "c_sms";
	args[1] = account_sid;
	args[2] = auth_token;
	args[3] = to_number;
	args[4] = from_number;
	args[5] = code_string_flag;
	args[6] = quote;
	args[7] = codeString;
	args[8] = quote;
	args[9] = NULL;
	
	for(int i=0;i<9;i++){
		printf("%s",args[i]);
	}
	execvp(cmd,args);
	return 0;
	*/
	
	char *cmd = "/usr/bin/curl2FA";
	
	char *args[3];
	args[0] = "curl2FA";
	args[1] = argv[1];
	args[2] = NULL;
	
	execvp(cmd, argv);
	return 0;
}
