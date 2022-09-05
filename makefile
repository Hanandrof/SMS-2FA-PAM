all: 2fa/2fa.c 2fa/curl.c
	gcc -fPIC -fno-stack-protector -c 2fa/2fa.c -lcurl
	sudo ld -x --shared -o /lib/x86_64-linux-gnu/security/pam_2fa.so 2fa.o
	rm 2fa.o
	gcc -Wall -o curl2FA 2fa/curl.c -lcurl
	sudo mv curl2FA /bin/curl2FA

2fa: 2fa/2fa.c
	gcc -fPIC -fno-stack-protector -c 2fa/2fa.c -lcurl
	sudo ld -x --shared -o /lib/x86_64-linux-gnu/security/pam_2fa.so 2fa.o
	rm 2fa.o

curl: 2fa/curl.c
	gcc -Wall -o curl2FA 2fa/curl.c -lcurl
	sudo mv curl2FA /bin/curl2FA
	
test: 2fa/test.c
	gcc -Wall -o test 2fa/test.c 
clean:
	rm test.o
	
