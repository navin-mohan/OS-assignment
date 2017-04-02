#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define MAX_SIZE 30

void die(char* err){
	perror(err);
	exit(1);
}


int main(void){

	int shmid,pid;
	char* shm,*p,c;

	if(( shmid = shmget(IPC_PRIVATE, MAX_SIZE, IPC_CREAT | 0666)) < 0) die("shmget error"); //create a shared memory segment

	if(( shm = shmat(shmid, NULL, 0)) == (char*) -1) die("shmat error"); // attach the shm segment to the current process addr space

	if((pid = fork()) < 0) die("fork error"); // fork the processs

	if(pid == 0){
		// child process (inherits the shm)

		p = shm;

		printf("child writing data to the shared memory segment\n");

		for(c=0;c<13;c++) // write to the shm
			*p++ = "Hello World!\0"[c];
		printf("done\n");
	}else{

		// parent process

		wait(0); // wait for child process to exit (otherwise read won't be synchronized)  
		
		printf("Message from child: ");
		for(p=shm;*p!='\0';p++) // read from the shm
			putchar(*p);
		printf("\n");
	}

	shmdt(shm); // detach the shm segment

	if(pid != 0) shmctl(shmid,IPC_RMID, NULL); // remove the shm segment 

	return 0;
}