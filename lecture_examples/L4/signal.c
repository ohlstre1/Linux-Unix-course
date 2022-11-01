#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>

static void sig_usr(int signo) {
	/* Note: else branch is never reached in this example. */
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else printf("received signal %d\n",signo);
	return;
}

int  main(void) {
	struct sigaction sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_flags=0;
	sig.sa_handler = sig_usr;

	assert((sigaction(SIGUSR1,&sig,NULL)) == 0); 

	assert((sigaction(SIGUSR2,&sig,NULL)) == 0);

	assert((sigaction(SIGINT,&sig,NULL)) == 0);

	for ( ; ; ) {
		printf("Sleeping...\n");
		sleep(5);
		printf("I woke up, now I'm pausing...\n");
		pause();
	}
}
