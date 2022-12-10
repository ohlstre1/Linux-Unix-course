#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

void *thread_routine(void *arg) {
  printf("T Thread starting\n");
// How does the following line change the situation?
//  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
  while (1) {
    printf("T Hello\n");
    pthread_testcancel();
    sleep(1);
  }
  
  return NULL;
}

int main(void) {
  pthread_t thread_id;
  void *thread_result;

  assert((pthread_create(&thread_id,NULL,thread_routine,NULL)) == 0);

  printf("P thread created\n");

  printf("P My thread ID: %u\n",(unsigned int) pthread_self());
  printf("P Child is: %u\n",(unsigned int) thread_id);

  sleep(4);

  printf("P Cancelling thread\n");
  assert(pthread_cancel(thread_id) == 0);

  // This should block until child returns or is canceled
  assert((pthread_join(thread_id,&thread_result)) == 0);

  if (thread_result == NULL)
    printf("P Error on pthread_join\n");
  else if ( thread_result == PTHREAD_CANCELED) 
    printf("P Received confirmation that child is canceled\n");
  else
    printf("P We received something weird\n");

  exit(0);
}
