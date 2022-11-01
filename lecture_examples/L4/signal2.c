#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include"mydbg.h"

static void sig_quit(int);

struct sigaction sig;

int main(void)
{
  sigset_t newmask, oldmask, pendmask;

  sigemptyset(&sig.sa_mask);
  sig.sa_flags=0;
  sig.sa_handler = sig_quit;

  CHECK((sigaction(SIGQUIT,&sig,NULL)) == 0);

  sigemptyset(&newmask);

  sigaddset(&newmask, SIGQUIT);

  /* Comment the following line and SIGQUIT is possible again */

  CHECK((sigprocmask(SIG_BLOCK,&newmask,&oldmask)) == 0);

  printf("Going to sleep...\n");

  sleep(10);    /* SIGQUIT here will remain pending */

  printf("I woke up!\n");
    
  CHECK((sigpending(&pendmask)) == 0);

  if (sigismember(&pendmask, SIGQUIT)) 
    printf("\nSIGQUIT pending\n");

  CHECK((sigprocmask(SIG_SETMASK,&oldmask,NULL)) == 0);

  printf("SIGQUIT unblocked\n");

  sleep(5);    /* SIGQUIT here will terminate the process */

  printf("It's all over...\n");
  
  exit(0);
}

static void sig_quit(int signo)
{
  printf("caught SIGQUIT\n");

  sig.sa_handler = SIG_DFL;

  CHECK((sigaction(SIGQUIT,&sig,NULL)) == 0);

  return;
}
