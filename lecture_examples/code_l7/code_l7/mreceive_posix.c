#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>

#define MQ_NAME       "/testmsg"
#define MSG_SIZE        128
#define MAX_MSG         3

#define MQ_MODE 0777

int main(int argc, char *argv[]) {
  typedef struct { 
    long mtype;
    char mtext[1];
  } msg_t;

  size_t msize;     /* amount of reserved space */
  msg_t *mess;
  int mlen;         /* actual length */

  mqd_t queue;
  unsigned prio;
  struct mq_attr mqstat;

  if ( argc > 1 ) { printf("Usage: mreceive\n"); exit(1); }

  memset(&mqstat, 0, sizeof(mqstat));
  mqstat.mq_maxmsg = MAX_MSG;
  mqstat.mq_msgsize = MSG_SIZE;
  mqstat.mq_flags = 0;
  
  queue = mq_open(MQ_NAME,O_CREAT|O_RDWR, MQ_MODE, &mqstat);

  if(queue == -1) {
    perror("Failed to open message queue");
    return 0;
  }  

  /* allocate space for message */
  msize = 200;

  assert((mess = (msg_t *)malloc(sizeof(long) + msize + 1)));

  printf("Process %d ready to receive messages. msq_t %ld\n",(int)getpid(),(long) queue);

  do {
    /* Get messages for this process ! */
    if ((mlen=mq_receive(queue, (char *) mess, msize, &prio)) == -1) {
      if (errno == EINTR) continue;

      if (errno == EMSGSIZE) {
        free(mess);
        msize *= 2;
        printf("%d message length -> %d\n", getpid(),(int) msize);
        assert((mess = (msg_t *)malloc(sizeof(long)+msize+1)));
        continue;
      }
      assert(errno == 0);
    }

    mess->mtext[mlen-sizeof(long)] = (char) 0;
    printf("Msg type %ld prio %u: '%s'\n", mess->mtype, prio, mess->mtext);  

  } while (mlen != 0);
  
  free(mess);

  exit(0);
}

