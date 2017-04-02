#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int pid,fd[2];
char str[20];


int main(void){

	if( (pid = pipe(fd)) == -1){ // create a pipe
		printf("Error creating PIPE!\n");
		exit(1);
	}

	printf("File desc %d %d\n",fd[0],fd[1] ); // fd[0] - for reading & fd[1] - for writing
	printf("About to fork\n");

	if( (pid = fork()) < 0){ // fork the process
		printf("Error forking then process!\n");
	}else if(pid == 0){

		// child process (also recieves a copy of parent's file descriptors)

		printf("Hi from Child Process!\n");

		write(fd[1],"Hi!",3); // write to the pipe 
	}else{

		// parent process

		wait(0); // wait for the child to exit

		printf("Hi from Parent Process!\n");

		read(fd[0],str,3); // read from the pipe

		printf("Message from child: %s\n",str);
		// write(1,str,3);
		close(fd[0]); // close the pipe
		close(fd[1]);
	}
	return 0;
}