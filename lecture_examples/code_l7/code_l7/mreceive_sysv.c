#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <assert.h>

int main(int argc, char *argv[]) { /* argument: file name */
  key_t key;
  int id;
  typedef struct { 
    long mtype;
    char mtext[1];
  } msg_t;

  size_t msize;     /* amount of reserved space */
  msg_t *mess;
  int mlen;         /* actual length */

  if ( argc < 2 ) { printf("mreceive <keyfile>\n"); exit(1); }

  assert((key = ftok(argv[1],'D')) > 0); 
  assert((id = msgget(key, IPC_CREAT|0777)) != -1);

  /* allocate space for message */
  msize = 20;
  assert((mess = (msg_t *)malloc(sizeof(long) + msize + 1)));
  printf("Process %d ready to receive messages.\n",(int)getpid());

  do {
    /* Get messages for this process ! */
    if ((mlen=msgrcv(id, mess, msize, getpid(), 0)) == -1) {
      if (errno == EINTR) {
        printf("Interrupted... back to work.\n"); /* continue; */
      } else if (errno == E2BIG) {
        free(mess);
        msize *= 2;
        printf("%d message length -> %d\n", getpid(),(int) msize);
        assert((mess = (msg_t *)malloc(sizeof(long)+msize+1)));
        continue;
      } else perror("received error");
	  
      assert(errno == 0);
    }
      
    mess->mtext[mlen] = (char) 0;
    printf("%d '%s'\n", getpid(), mess->mtext);  

  } while (mlen != 0);
  
  free(mess);

  assert((msgctl(id,0,IPC_RMID)) != -1);

  exit(0);
}

