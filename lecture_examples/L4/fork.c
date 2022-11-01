#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

/*! Global variable. */
int global = 200;


int main(void) {
	int local = 100;
	pid_t pid;

	printf("<parent> Next, do fork()\n");

	pid = fork();
	printf("should be shown 2 times\n");
	if (pid == -1) {
		perror("<parent> Fork failed");
		return -1;
	}

	if (pid == 0) {
		// Only child process would come here
		printf("<child> I'm alive!\n");
		global += 10;
		local += 20;
	} else {
		// Only parent process would come here
		printf("<parent> Me too!\n");
	}

	printf("ppid=%d, pid=%d, global=%d, local=%d\n",getppid(),getpid(),global,local);

	return 0;
} 
