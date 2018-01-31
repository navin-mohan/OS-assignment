#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void die(char* err){
	perror(err);
	exit(1);
}


int main(void){

	int perm = IPC_CREAT | IPC_EXCL | 0600; // Create if not already exists , fail if exists , owner can read and write

	int msgid,pid;
	char msg[3];

	if( (msgid = msgget(IPC_PRIVATE,perm)) == -1) die("error creating msgq"); // create a message queue

	if( (pid = fork()) < 0) die("error forking the process"); // fork the process

	if(pid == 0){
		// child process

		printf("Sending message from child process\n");
		for(int i=0;i<3;++i) msg[i] = "Hi!"[i]; // initialize the message to be send
		if( msgsnd(msgid,&msg,sizeof(char)*3,IPC_NOWAIT) == -1 ) die("msgsnd error"); // send the message

	}else{

		wait(0);
		if( msgrcv(msgid,&msg,sizeof(char)*3,0,IPC_NOWAIT) == -1 ) die("msgrcv error"); // recieve the message 

		printf("Recieved message from child: %s\n",msg);

		msgctl(msgid,IPC_RMID,0); // remove the message queue

	}



	return 0;
}
