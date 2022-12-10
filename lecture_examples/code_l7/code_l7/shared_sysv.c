#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <semaphore.h>

union semun {
  int val;                    /* value for SETVAL */
  struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
  unsigned short int *array;  /* array for GETALL, SETALL */
  struct seminfo *__buf;      /* buffer for IPC_INFO */
};

#define SHMDATASIZE 1000
#define BUFFERSIZE (SHMDATASIZE - sizeof(int))

#define SM_EMPTY 0
#define SM_FULL  1

void server(void);
void client(int shmid);
void delete(void);
void sigdelete(int signum);
void locksem(int semid, int semnum);
void unlocksem(int semid, int semnum);
void waitzero(int semid, int semnum);
void clientwrite(int shmid, int semid, char *buffer);

int DeleteSemid = 0;

int main(int argc, char *argv[]) {

  /* No arguments: "server". */
  if (argc < 2) {
    server();
  } else {
    client(atoi(argv[1]));
  }
  return 0;
}

void server(void) {
  union semun sunion;
  int semid, shmid;
  void *shmdata;
  char *buffer;
  struct sigaction sig;

  /* First thing: generate the semaphore. */

  assert((semid = semget(IPC_PRIVATE, 2, SHM_R | SHM_W)) != -1);

  DeleteSemid = semid;

  /* Delete the semaphore when exiting. */
  atexit(&delete);

  memset(&sig,0,sizeof(struct sigaction));

  sig.sa_handler=sigdelete;
       
  assert(sigaction(SIGINT,&sig,NULL) == 0);
        
  /* Initially empty should be available and full should not be. */
 
  memset(&sunion,0,sizeof(sunion));
  sunion.val = 1;
  assert((semctl(semid, SM_EMPTY, SETVAL, sunion)) != -1);
  sunion.val = 0;
  assert((semctl(semid, SM_FULL, SETVAL, sunion)) != -1);

  /* Now allocate a shared memory segment. */

  assert((shmid = shmget(IPC_PRIVATE, SHMDATASIZE, IPC_CREAT | SHM_R | SHM_W)) != -1);
 
  /* Map it into memory. */

  assert((shmdata = shmat(shmid, 0, 0)) != NULL);

  /* Mark it to automatically delete when the last holding process exits. */

  assert((shmctl(shmid, IPC_RMID, NULL)) != -1);

  /* Write the semaphore id to its beginning. */
  *(int *)shmdata = semid;
  buffer = (char *)((int *)shmdata + sizeof(int));
  printf("Server is running with SHM id ** %d ** and semaphore id %d\n",
         shmid,semid);

  /*****************************************************************
   MAIN SERVER LOOP
   *****************************************************************/

  while (1) {
    printf("Waiting until full...");
    fflush(stdout);
    locksem(semid, SM_FULL);
    printf(" done.\n");

    printf("Message received: %s\n", buffer);
    unlocksem(semid, SM_EMPTY);
  }
}

void client(int shmid) {
  int semid;
  void *shmdata;
  char *buffer;

  assert((shmdata = shmat(shmid, 0, 0)) != NULL);
  semid = *(int *)shmdata;
  buffer = (char *)((int *)shmdata + sizeof(int));
  printf("Client operational: shm id is %d, sem id is %d\n",
         shmid,
         semid);

  while (1) {
    char input[3];

    printf("\n\nMenu\n1. Send a message\n");
    printf("2. Exit\n");

    fgets(input, sizeof(input), stdin);

    switch(input[0]) {
      case '1': clientwrite(shmid, semid, buffer); break;
      case '2': { exit(0); break; }
    }
  }
  
}


void delete(void) {
  printf("\nMaster exiting; deleting semaphore %d.\n", DeleteSemid);
  assert((semctl(DeleteSemid, 0, IPC_RMID, 0)) != -1);
}

void sigdelete(int signum) {
  /* Calling exit will conveniently trigger the normal
     delete item. */

  exit(0);
}

void locksem(int semid, int semnum) {
  struct sembuf sb;

  memset(&sb,0,sizeof(sb));
  sb.sem_num = semnum;
  sb.sem_op = -1;
  sb.sem_flg = SEM_UNDO;

  assert((semop(semid, &sb, 1)) != -1);
}

void unlocksem(int semid, int semnum) {
  struct sembuf sb;

  memset(&sb,0,sizeof(sb));
  sb.sem_num = semnum;
  sb.sem_op = 1;
  sb.sem_flg = SEM_UNDO;

  assert((semop(semid, &sb, 1)) != -1);
}

void waitzero(int semid, int semnum) {
  struct sembuf sb;

  memset(&sb,0,sizeof(sb));
  sb.sem_num = semnum;
  sb.sem_op = 0;
  sb.sem_flg = 0;           /* No modification so no need to undo */
 
  assert((semop(semid, &sb, 1)) != -1);
}

void clientwrite(int shmid, int semid, char *buffer) {
  printf("Waiting until empty...");
  fflush(stdout);
  locksem(semid, SM_EMPTY);
  printf(" done.\n");

  printf("Enter message: ");
  fgets(buffer, BUFFERSIZE, stdin);
  unlocksem(semid, SM_FULL);
}


