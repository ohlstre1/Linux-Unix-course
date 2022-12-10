#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <semaphore.h>

#define BUFFERSIZE 1000

typedef struct {
  sem_t *sem_empty;
  sem_t *sem_full;
  char buffer[BUFFERSIZE+1];
} sh_memdata;

void server(void);
void client();
void delete(void);
static void sigdelete(int signum);
void clientwrite(char *buffer);

/* These are global for signal handler */
sem_t *sh_empty;
sem_t *sh_full;
void *shmdata;

int main(int argc, char *argv[]) {

  /* No arguments: "server". */
  if (argc < 2) {
    server();
  } else {
    client();
  }
  return 0;
}

void server(void) {
  char *buffer;
  int shmid;
  sh_memdata *shmem;

  struct sigaction sig;
  /* First thing: generate the semaphore. */

  assert((sh_empty = sem_open("posixshared.c", O_CREAT,0777, 1)) != SEM_FAILED);
  assert((sh_full  = sem_open("posixshared", O_CREAT,0777, 0)) != SEM_FAILED);

  /* Delete the semaphores when exiting. */
  atexit(&delete);

  memset(&sig,0,sizeof(struct sigaction));
  sig.sa_handler=sigdelete;
  assert(sigaction(SIGINT,&sig,NULL) == 0);
       
  /* Now allocate a shared memory segment. */

  assert((shmid = shm_open("posixshared", O_CREAT|O_RDWR, SHM_R | SHM_W)) != -1);
  assert((ftruncate(shmid,sizeof(sh_memdata))) != -1);
  assert((shmdata = mmap(NULL,sizeof(sh_memdata),PROT_READ|PROT_WRITE,
			MAP_SHARED,shmid, 0)) != MAP_FAILED);

  /* Write the semaphore id to its beginning. */

  shmem = (sh_memdata *)shmdata;
  shmem->sem_empty = sh_empty;
  shmem->sem_full = sh_full;
  buffer = shmem->buffer;

  printf("Server is running ****\n");

  /*****************************************************************
   MAIN SERVER LOOP
   *****************************************************************/

  while (1) {
    printf("Waiting until full...");
    fflush(stdout);
    assert((sem_wait(sh_full)) != -1);
    printf(" done.\n");

    printf("Message received: %s\n", buffer);
    fflush(stdout);
    assert((sem_post(sh_empty)) != -1);
  }
}

void client() {
  char *buffer;
  int shmid;
  sh_memdata *shmem;

  /* Find shared memory segment */
  assert((shmid = shm_open("posixshared", O_RDWR, 0)) != -1);
  assert((shmdata = mmap(NULL,sizeof(sh_memdata),PROT_READ|PROT_WRITE,
			MAP_SHARED,shmid, 0)) != MAP_FAILED);

  /* Find semaphores and buffer from memory */
  shmem = (sh_memdata *)shmdata;
  assert((sh_empty = sem_open("posixshared.c",0)) != SEM_FAILED);
  assert((sh_full  = sem_open("posixshared", 0)) != SEM_FAILED);
  buffer   = shmem->buffer;

  printf("Client operational\n");

  while (1) {
    char input[3];

    printf("\n\nMenu\n1. Send a message\n");
    printf("2. Exit\n");
    fflush(stdout);

    fgets(input, sizeof(input), stdin);
    fflush(stdout);

    switch(input[0]) {
      case '1': clientwrite(buffer); break;
      case '2': { exit(0); break; }
    }
  }
 
}


void delete(void) {
  printf("\nMaster exiting;\n");

  assert((sem_unlink("posixshared.c")) != -1);
  assert((sem_unlink("posixshared")) != -1);
  assert((shm_unlink("posixshared")) != -1);
}

static void sigdelete(int signum) {
  /* Calling exit will conveniently trigger the normal
     delete item. */

  exit(0);
}

void clientwrite(char *buffer) {
  printf("Waiting until empty...");
  fflush(stdout);
  assert((sem_wait(sh_empty)) != -1);
  printf(" done.\n");
  fflush(stdout);

  printf("Enter message: ");
  fgets(buffer, BUFFERSIZE, stdin);
  assert((sem_post(sh_full)) != -1);
}


