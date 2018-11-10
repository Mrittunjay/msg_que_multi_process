#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>

struct my_msg_st {
	long int my_msg_type;
	char some_text[BUFSIZ];
};

int main() {
	key_t my_key;
	int running = 1;
	int msgid;
	struct my_msg_st some_data;
	long int msg_to_receive = 3;

	my_key = ftok("for_tok", 'a');	
	
 	msgid = msgget(my_key, 0666 | IPC_CREAT);
	if (msgid == -1) {
		fprintf(stderr, "failed to get:\n");
		exit(EXIT_FAILURE);
	}

	while (running) {
		if(msgrcv(msgid, (void *)&some_data, BUFSIZ,msg_to_receive, 2)  == -1) {
			fprintf(stderr, "failedto receive: \n");
			exit(EXIT_FAILURE);
		}

		printf("You Wrote: %s", some_data.some_text);
		if(strncmp(some_data.some_text, "end", 3) == 0) {
			running = 0;
		}
	}
	if (msgctl(msgid, IPC_RMID, 0) == -1) {
		fprintf(stderr, "failed to delete\n");
	exit(EXIT_FAILURE);
	}
exit(EXIT_SUCCESS);
}
