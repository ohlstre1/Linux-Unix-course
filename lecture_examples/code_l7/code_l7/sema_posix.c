#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#define SEMA "/foo-bar"

int semheld = 0;

void release(sem_t *id);
void request(sem_t *id);
void try(sem_t *id);
void getvalue(sem_t *id);

int main(int argc, char *argv[]) 
{
  sem_t *id;

  /* No arguments: "server". */
  if (argc < 2) {
    /* Request a semaphore. */
    assert((id = sem_open(SEMA,O_CREAT,0777,2)) != SEM_FAILED);
  } else { /* client */
    /* Open up the existing one. */
    assert((id = sem_open(SEMA,0)) != SEM_FAILED);
    printf("Using existing semaphore %p.\n", (void *)id);
  }

  printf("Successfully allocated semaphore id %p\n", (void *)id);
 
  while (1) {
    int selection;
    printf("\nStatus: %d resources held by this process.\n", semheld);
    printf("Menu:\n");
    printf("1. Release a resource\n");
    printf("2. Request a resource\n");
    printf("3. Try to request\n");
    printf("4. Get value\n");
    printf("5. Exit this process\n");
    printf("6. Remove resource and exit\n");
    printf("Your choice: ");

    scanf("%d", &selection);

    switch(selection) {
      case 1: release(id); break;
      case 2: request(id); break;
      case 3: try(id); break;
      case 4: getvalue(id); break;
      case 5: { sem_close(id);exit(0); break; }
      case 6: {sem_unlink(SEMA); exit(0); break; }
    }
  }

  return 0;
}

void release(sem_t *id) {
  if (semheld < 1) {
    printf("I don't have any resources; nothing to release.\n");
    return;
  }

  sem_post(id);
  semheld--;

  printf("Resource released.\n");
}

void request(sem_t *id) {
  if (semheld > 0) {
    printf("I already hold the resource; not requesting another one.\n");
    return;
  }

  printf("Requesting resource...");
  fflush(stdout);

  sem_wait(id);
  semheld++;

  printf(" done.\n");
}

void try(sem_t *id)
{
  if (semheld > 0) {
    printf("I already hold the resource; not requesting another one.\n");
    return;
  }

  printf("Trying to request resource...");
  fflush(stdout);

  if(sem_trywait(id) == -1)
  {
  	if(errno == EAGAIN)
  		printf("Semaphore was locked\n");
  	else
  		perror("Semphore error");
  }
  else
  {
  	semheld++;
  	printf(" done.\n");	
  }

}

void getvalue(sem_t *id)
{
	int sval;
	if(sem_getvalue(id,&sval) == -1)
	{
		perror("sem_getvalue");
	}
	else
	{
		if(sval > 0)
			printf("Semaphore value %d\n",sval);
		else if (sval == 0)
			printf("Semaphore locked, no processes waiting\n");
		else
			printf("Semaphore locked, %d processes waiting\n",sval*-1);
	}
}

