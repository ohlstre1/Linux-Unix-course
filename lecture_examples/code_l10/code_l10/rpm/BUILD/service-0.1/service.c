#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

static int sigpipe = 0;

static void sig_usr(int signo) {
	char ret = signo;
	write(sigpipe,&ret,1);
}

int main(void) {
	struct sigaction sig;
	int pipefd[2];

	FILE *outfile = fopen("/tmp/mylog.txt","a");
	assert(outfile != NULL);

	assert(pipe(pipefd) == 0);
	sigpipe = pipefd[1];

	sigemptyset(&sig.sa_mask);
  	sig.sa_flags=0;
	sig.sa_handler = sig_usr;

	assert((sigaction(SIGUSR1,&sig,NULL)) == 0); 
	assert((sigaction(SIGUSR2,&sig,NULL)) == 0);

	for ( ; ; ) {
		char mysignal;
		int res = read(pipefd[0],&mysignal,1);
		// When read is interrupted by a signal, it will return -1 and errno is EINTR.
		if (res == 1) {
                	fprintf(outfile,"Received signal %d\n",mysignal);
			fflush(outfile);
                }
	}
	fclose(outfile);
	return 0;
}
