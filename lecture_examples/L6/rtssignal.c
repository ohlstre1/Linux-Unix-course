#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

static void handler(int signum, siginfo_t *info, void *context) {
  /* Try what happens when you uncomment the following lines */
  /* Why ? */

/*
  assert((kill(getpid(), SIGUSR1)) == 0 ); 
  sleep(1);
 */

  printf("signal number: %d\n", info->si_signo);
  printf("sending process ID: %ld\n", (long)info->si_pid);
  printf("real user ID of sending process: %ld\n", (long)info->si_uid);

  switch (info->si_code) {
    case SI_USER:
      printf("Signal from user\n"); break;
    case SI_QUEUE:
      printf("Signal from sigqueue; value = %d\n",
             info->si_value.sival_int); break;
    case SI_TIMER:
      printf("Signal from timer expiration; value = %d\n",
             info->si_value.sival_int); break;
    case SI_ASYNCIO:
      printf("Signal from asynchronous I/O completion; value = %d\n",
             info->si_value.sival_int); break;
    case SI_MESGQ:
      printf("Signal from message arrival; value = %d\n",
             info->si_value.sival_int); break;
    default:
      printf("Other signal\n"); break;
  }
}

int main(void) {
  struct sigaction act;
  union sigval val;

  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_SIGINFO;
  act.sa_sigaction = handler;

  assert((sigaction(SIGUSR1, &act, NULL)) == 0 );
  assert((sigaction(SIGRTMIN, &act, NULL)) == 0 );
  assert((kill(getpid(), SIGUSR1)) == 0 );

  printf("--------------------------\n");
  
  val.sival_int = 1234;

  assert((sigqueue(getpid(), SIGRTMIN, val)) == 0 );

  exit(0);
}
