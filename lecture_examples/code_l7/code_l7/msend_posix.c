#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <assert.h>

#define MQ_NAME       "/testmsg"
#define MSG_SIZE        128
#define MAX_MSG         3

int main(int argc, char *argv[]) {
  long type;

  typedef struct {        /* This is standard method */
    long mtype;    
    char mtext[128];
  } msg_t;

  mqd_t queue;
  unsigned prio;
  
  size_t mlen;
  msg_t *mess;

  if ( argc != 4 ) { printf("Usage: msend_posix msg_type msg_priority message\n"); exit(1); }

  queue = mq_open(MQ_NAME,O_WRONLY);

  if(queue == -1) {
    perror("Failed to open message queue");
    return 0;
  }

  assert((type = atol(argv[1])) > 0); /* message type */
  assert((prio = atoi(argv[2])) > 0); /* message prio */

  mlen = strlen(argv[3]);

  assert((mess = (msg_t *) malloc(sizeof(long) + mlen)));

  mess->mtype = type;
  memcpy(mess->mtext, argv[3], mlen);
  mlen += 4;

  assert(mq_send(queue, (char *) mess, mlen, prio) == 0); /* send message to process */

  free(mess);

  exit(0);
}
