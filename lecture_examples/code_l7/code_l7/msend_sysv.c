#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <assert.h>

int main(int argc, char *argv[]) {
/* arguments: file name, type, text */
  key_t key;
  int id;
  long type;
  typedef struct {        /* This is standard method */
    long mtype;    
    char mtext[1];
  } msg_t;

  size_t mlen;
  msg_t *mess;

  if ( argc < 4 ) { printf("msend <keyfile> process message\n"); exit(1); }

  assert((key = ftok(argv[1],'D')) != -1); 
  assert((id = msgget(key,0)) != -1);

  assert((type = atol(argv[2])) > 0); /* message type: process id */
  mlen = strlen(argv[3]);
  assert((mess = (msg_t *) malloc(sizeof(long) + mlen)));
  mess->mtype = type;
  memcpy(mess->mtext, argv[3], mlen);
  assert(msgsnd(id, mess, mlen, 0) == 0); /* send message to process */
  free(mess);

  exit(0);
}

