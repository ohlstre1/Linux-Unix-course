#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int global = 200;   /*!< Global variable. */

void dowrite(int fd,const char *msg) {
	int len = strlen(msg);
	int res = write(fd,msg,len);
	if (res == -1) {
		perror("Write returned error");
	} else if (res != len) {
		printf("Couldn't print all date");
	}
}

int main(void) {
	int local = 100;   /*!< Local variable. */
	pid_t pid;

	dowrite(STDOUT_FILENO,"Testing stdout\n");

	printf("Next, do fork()\n");

	pid = fork();
	if (pid == -1) {
		perror("Fork failed");
		return -1;
	}

	if (pid == 0) {
		// Only child process would come here
		printf("<child> My pid is %d and my parent is %d\n",getpid(),getppid());
		global += 10;
		local += 20;
		printf("<child> Now, going for a nap\n");
		sleep(10);
		printf("<child> After my nap, my parent is %d\n",getppid());
	} else {
		int status = 0;
		// Only parent process would come here
		printf("<parent> I have pid %d, and my child has %d\n",getpid(),pid);

		printf("<parent> Now, waiting for child %d\n",pid);
		if (waitpid(pid,&status,WUNTRACED) == -1) {
			perror("Waitpid failed");
			return -1;
		}
		printf("<parent> waitpid() returned %d\n",status);

		// Check using macros
		if (WIFEXITED(status) != 0)
			printf("<parent> Child exited normally\n");
		if (WIFSIGNALED(status) != 0)
			printf("<parent> Child exited due to uncaught signal: %d\n",WTERMSIG(status));
		printf("<parent> Child return value is %d\n",WEXITSTATUS(status));
	}

	printf("ppid=%d, pid=%d\n",getppid(),getpid());
	printf("global=%d,local=%d\n",global,local);

	return 0;
} 
