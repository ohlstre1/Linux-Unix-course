#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
	pid_t pid;

	if(argc == 1)
	{
	printf("Usage: %s command param1 param2 ...\n",argv[0]);
	return 0;
	}
	
	printf("**********Performing exec() call...\n");

	if ( (pid = fork()) < 0)
		perror("fork error");
	else if (pid == 0) { /* This is the child */
		/* specify pathname, specify environment */
		if(execvp(argv[1],&argv[1]) != 0)
			perror("execvp");
	   	return -1;
	}
	
	if (waitpid(pid, NULL, 0) <= 0) perror("waitpid");

	printf("**********Exec returned\n");
	
	return 0;
}
