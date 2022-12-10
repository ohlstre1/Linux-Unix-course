#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

void *thread_routine(void *arg) {
  /*
  // What if we detach ourself?
  pthread_t some;
  some = pthread_self();
  pthread_detach(some);
  */
  int pid;

  pid=fork();
  if (pid==0) {
    printf("child\n");
    printf("T Child after fork says: My thread ID: %u\n",(unsigned int) pthread_self());
    exit(0);
  } else
    printf("mother\n");
  
  printf("T I'm thread\n");
  printf("T My thread ID: %u\n",(unsigned int) pthread_self());
  printf("T void *arg was: %s\n",(char *)arg);
  pthread_exit(arg);
}

int main(void) {
  pthread_t thread_id;
  void *thread_result;

  assert((pthread_create(&thread_id,NULL,thread_routine,(void *)"foobar")) == 0);

  printf("P thread created\n");

  printf("P My thread ID: %u\n",(unsigned int) pthread_self());
  printf("P Child is: %u\n",(unsigned int) thread_id);

  /* What happens if you comment out the following lines before the exit call? */
  assert((pthread_join(thread_id,&thread_result)) == 0);

  if ( thread_result == NULL)
    printf("P Error on pthread_join\n");
  else
    printf("P Result was %s\n",(char *)thread_result);

  exit(0);
}
