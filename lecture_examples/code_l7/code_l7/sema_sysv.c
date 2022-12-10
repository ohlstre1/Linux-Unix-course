#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>

union semun {
  int val;                    /* value for SETVAL */
  struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
  unsigned short int *array;  /* array for GETALL, SETALL */
  struct seminfo *__buf;      /* buffer for IPC_INFO */
};

int semheld = 0;

void release(int id);
void request(int id);

int main(int argc, char *argv[]) 
{
  int id;
  union semun sunion;

  /* No arguments: "server". */
  if (argc < 2) {
    /* Request a semaphore. */
    assert((id = semget(IPC_PRIVATE,1, SHM_R|SHM_W)) != -1);
   
    /* Initialize its resource count to 1. */
   
    memset(&sunion,0,sizeof(sunion));
    sunion.val = 1;
    assert((semctl(id, 0, SETVAL, sunion)) != -1);
  } else { /* client */
    /* Open up the existing one. */
    id = atoi(argv[1]);
    printf("Using existing semaphore %d.\n", id);
  }

  printf("Successfully allocated semaphore id %d\n", id);
 
  while (1) {
    int selection;
    printf("\nStatus: %d resources held by this process.\n", semheld);
    printf("Menu:\n");
    printf("1. Release a resource\n");
    printf("2. Request a resource\n");
    printf("3. Exit this process\n");
    printf("Your choice: ");

    scanf("%d", &selection);

    switch(selection) {
      case 1: release(id); break;
      case 2: request(id); break;
      case 3: 
      {
	if ( argc < 2 ) { /* server removes semaphore */
	  assert((semctl(id,0,IPC_RMID)) != -1);  
	}
	exit(0);
	break;
      }
    }
  }
  return 0;
}

void release(int id) {
  struct sembuf sb;

  if (semheld < 1) {
    printf("I don't have any resources; nothing to release.\n");
    return;
  }

  memset(&sb,0,sizeof(sb));
  sb.sem_num = 0;
  sb.sem_op = 1;
  sb.sem_flg = 0;
 
  assert((semop(id, &sb, 1)) != -1);
  semheld--;

  printf("Resource released.\n");
}

void request(int id) {
  struct sembuf sb;


  if (semheld > 0) {
    printf("I already hold the resource; not requesting another one.\n");
    return;
  }
 
  memset(&sb,0,sizeof(sb));
  sb.sem_num = 0;
  sb.sem_op = -1;
  sb.sem_flg = 0;

  printf("Requesting resource...");
  fflush(stdout);

  assert((semop(id, &sb, 1)) != -1);
  semheld++;

  printf(" done.\n");
}
