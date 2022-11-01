#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

/*
 * Good version of signal.c example.
 * Here the signal handler has been minimized, like it always should be.
 */

static volatile sig_atomic_t mysignal = 0;

static void sig_usr(int signo) {
	mysignal = signo;
}

int main(void) {
	struct sigaction sig;

	sigemptyset(&sig.sa_mask);	
	sig.sa_flags=0;
	sig.sa_handler = sig_usr;

	assert((sigaction(SIGUSR1,&sig,NULL)) == 0); 
	assert((sigaction(SIGUSR2,&sig,NULL)) == 0);

	for ( ; ; ) {
                pause();
		/* Still a small bug: If new signal is received here,
		   type of the old signal is lost... :) */
        	if (mysignal == SIGUSR1)
			printf("received SIGUSR1: %d\n", mysignal);
		else if (mysignal == SIGUSR2)
			printf("received SIGUSR2\n");
	}

	return 0;
}
