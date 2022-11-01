#include <sys/types.h>
#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"mydbg.h"

int  glob = 6;             /* external variable in initialized data */

char buf[] = "a write to stdout\n";

int main(void)
{
  int      var;    /* automatic variable on the stack */
  pid_t pid;
  int status=0;

  var = 88;

  CHECK((write(STDOUT_FILENO, buf, sizeof(buf)-1)) == sizeof(buf)-1);

  printf("before fork\n");

  CHECK((pid = fork()) >= 0);

  if (pid == 0) {   /* child */
    glob++;          /* modify variables */
    var++;
    printf("Child: Sleeping for 10 seconds\n");
    sleep(10);
  } 
  else          /* parent */
  {
	printf("Waiting for child %d\n",pid);


	if(waitpid(pid,&status,WUNTRACED) == -1)
	{
		perror("waitpid");
		exit(-1);
	}

	printf("Status: %d\n",status);
	
	if(WIFEXITED(status) != 0)
		printf("Child exited normally\n");


	printf("Child returned: %d\n",WEXITSTATUS(status));

	if(WIFSIGNALED(status))
		printf("Child returned due to a signal which was not caucht: %d\n",WTERMSIG(status));
  }

  printf("ppid=%d, pid=%d,glob=%d,var=%d\n",getppid(),getpid(), glob, var);  

  if((pid==0) && (getppid() == 1)) printf("My parent was Init!\n");
  if(pid==0)   exit(255);
  else exit(1);
} 
