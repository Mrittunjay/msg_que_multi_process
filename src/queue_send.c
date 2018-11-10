#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#define  MAX_TEXT 512

struct my_msg_st {
	long int my_msg_type;
	char some_text[MAX_TEXT];
};

struct my_msg_st some_data;

int main(int argc, char *argv[]) {
	key_t my_key;
	long int put_typ;
	int running = 1;

	int msgid;
	char buffer[BUFSIZ];	/*gcc 1024*/
	
	if(argc < 2) {
		printf("\nARGUMENT ERROR");
		exit(1);
	}

	//getting the value of the arg
	put_typ = atoi(argv[1]);
	printf("\n%ld\n", put_typ);
	

	my_key = ftok("for_tok", 'a');

	msgid = msgget( my_key, 0666 | IPC_CREAT);
	if (msgid == -1) {
		fprintf(stderr, "failed to create:\n");
		exit(EXIT_FAILURE);
	}
	
	while(running) {
		printf("Enter Some Text: ");
		fgets(buffer, BUFSIZ, stdin);
		some_data.my_msg_type = put_typ;
		strcpy(some_data.some_text, buffer);

		if(msgsnd(msgid, (void *)&some_data, MAX_TEXT, 2) == -1) {
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
		if(strncmp(buffer, "end", 3) == 0) {
			running = 0;
		}
	}
exit(EXIT_SUCCESS);
}
