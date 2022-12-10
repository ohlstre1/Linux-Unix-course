/* Shared memory with semaphores using semaphores in shared memory region. */
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
  sem_t sem_empty;
  sem_t sem_full;
  char buffer[BUFFERSIZE+1];
} sh_memdata;

void server(void);
void client();
void delete(void);
static void sigdelete(int signum);
void clientwrite(char *buffer);

/* These are global for signal handler */
void *shmdata;
sh_memdata *shmem;


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

  struct sigaction sig;
       
  /* Allocate a shared memory segment. */

  assert((shmid = shm_open("posixshared", O_CREAT|O_RDWR, SHM_R | SHM_W)) != -1);
  assert((ftruncate(shmid,sizeof(sh_memdata))) != -1);
  assert((shmdata = mmap(NULL,sizeof(sh_memdata),PROT_READ|PROT_WRITE,
			MAP_SHARED,shmid, 0)) != MAP_FAILED);

  /* Initialize semaphores to shared memory section. */
  shmem = (sh_memdata *)shmdata;
  assert((sem_init(&shmem->sem_empty, 1, 1)) == 0);
  assert((sem_init(&shmem->sem_full, 1, 0)) == 0);
  buffer = shmem->buffer;

  /* Delete the semaphores when exiting. */
  atexit(&delete);

  memset(&sig,0,sizeof(struct sigaction));
  sig.sa_handler=sigdelete;
  assert(sigaction(SIGINT,&sig,NULL) == 0);

  printf("Server is running ****\n");

  /*****************************************************************
   MAIN SERVER LOOP
   *****************************************************************/

  while (1) {
    printf("Waiting until full...");
    fflush(stdout);
    assert((sem_wait(&shmem->sem_full)) != -1);
    printf(" done.\n");

    printf("Message received: %s\n", buffer);
    fflush(stdout);
    assert((sem_post(&shmem->sem_empty)) != -1);
  }
}

void client() {
  char *buffer;
  int shmid;

  /* Find shared memory segment */
  assert((shmid = shm_open("posixshared", O_RDWR, 0)) != -1);
  assert((shmdata = mmap(NULL,sizeof(sh_memdata),PROT_READ|PROT_WRITE,
			MAP_SHARED,shmid, 0)) != MAP_FAILED);

  /* Find semaphores and buffer from memory */
  shmem = (sh_memdata *)shmdata;
  /* Semaphores now accessible from shared memory region! */
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

  assert((sem_destroy(&shmem->sem_full)) != -1);
  assert((sem_destroy(&shmem->sem_empty)) != -1);
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
  assert((sem_wait(&shmem->sem_empty)) != -1);
  printf(" done.\n");
  fflush(stdout);

  printf("Enter message: ");
  fgets(buffer, BUFFERSIZE, stdin);
  assert((sem_post(&shmem->sem_full)) != -1);
}


