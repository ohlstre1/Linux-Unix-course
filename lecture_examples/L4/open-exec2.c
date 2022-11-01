#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int fd,n;
	char buf[129];

	fd = atoi(argv[1]);	
	printf("I'm open-exec2 and fd is %d\n",fd);
	while ((n = read(fd,buf,128)) > 0) {
		buf[n] = '\0';  // re-terminate
		printf("%s",buf);
	}

	return 0;
}

