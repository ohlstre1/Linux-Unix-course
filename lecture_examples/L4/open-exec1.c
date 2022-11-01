#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


/*
 * Shows how changing the process code with exec leaves
 * the file descriptors open for the new program.
 *
 * Try "open-exec1 text_file"
 *
 * or
 *         "open-exec1 text_file cat"
 */
  
int main(int argc, char **argv) {
	int fd=0;
	char fd_str[8];
	
	if (argc == 1) {
		printf("Usage: %s filename [exec program]...\n",argv[0]);
		return 0;
	}

	fd = open(argv[1],O_RDONLY);
	
	if (fd == -1) { perror("open"); _exit(0); }

	if (argc == 2) {
		snprintf(fd_str,8,"%d",fd);
		if (execl("open-exec2","open-exec2",fd_str,NULL) != 0)
			perror("execv");
	} else {
		dup2(fd,0);
		if (execv(argv[2],&argv[2]) != 0) perror("execv");
	}
	
	return 0;
}
