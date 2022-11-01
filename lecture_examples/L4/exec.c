#include <sys/types.h>                                            
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Usage: %s command param1 param2 ...\n",argv[0]);
		return 0;
	}
	
	printf("**********Performing exec() call...\n");

	/* Specify pathname, specify environment */
	if (execv(argv[1],&argv[1]) != 0)
		perror("execv");

	printf("**********Exec returned\n");
	
	return 0;
}
