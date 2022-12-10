#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#define MAXTHREADS 10

pthread_barrier_t my_barrier;

void* b(void *arg) {
  printf("Thread %lu executing function b\n", pthread_self());
  fflush(stdout);
  pthread_exit(arg);
}

void *a(void *arg) {
  int res;
  printf("Thread %lu executing function a\n", pthread_self());
  fflush(stdout);
  res = pthread_barrier_wait(&my_barrier);

  if ( res != 0) {
    if ( res == PTHREAD_BARRIER_SERIAL_THREAD ) {
      printf("Thread %lu got PTHREAD_BARRIER_SERIAL_THREAD (%d)\n",
             pthread_self(),PTHREAD_BARRIER_SERIAL_THREAD);
      fflush(stdout);
    }
    else {
      perror("thread_barrier_wait");
      exit(-1);
    }
  }

  printf("Thread %lu has executed wait on barrier\n",pthread_self());
  fflush(stdout);
  b(arg);
  return(arg); /* not reached */
}

int main(void) {
  pthread_t thread_id[MAXTHREADS] = {0};
  int i;
  void *res;

/* Try with value 5?

  assert((pthread_barrier_init(&my_barrier,NULL,5)) == 0);
 */


  assert((pthread_barrier_init(&my_barrier,NULL,MAXTHREADS)) == 0);
  printf("Mama created barrier\n");
  fflush(stdout);

  for(i=0;i < MAXTHREADS; i++) {
    assert((pthread_create(&(thread_id[i]),NULL,a,NULL)) == 0);
    printf("Mama created thread %lu\n",thread_id[i]);
    fflush(stdout);
  }

  for(i=0;i < MAXTHREADS; i++) {
    printf("Mama waits for thread %lu to end\n", thread_id[i]);
    fflush(stdout);
    assert((pthread_join(thread_id[i],&res)) == 0);
    printf("Thread %lu ended\n", thread_id[i]);
    fflush(stdout);
  }

  assert((pthread_barrier_destroy(&my_barrier)) == 0);
  exit(0);
}
