#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sched.h>
#include <pthread.h>
#include <assert.h>

#define MAXTHREADS 100

static unsigned long global_seed = 0;

typedef struct {
  pthread_mutex_t mutex;
  int id;
  unsigned long array[MAXTHREADS+1];
} my_array_t;

my_array_t *thread_array;

void *thread_routine(void *arg) {
  int id;

  if ((intptr_t) arg > 0) {
    assert((pthread_mutex_lock(&thread_array->mutex)) == 0);
    printf("Thread %lu got mutex\n",pthread_self());
    fflush(stdout);
  }

  printf("Thread %lu in thread_routine, request mutex: %s\n",pthread_self(),
          (intptr_t) arg > 0 ? "yes":"no");
  
  id = global_seed;

  /* Give up using processor */
  sched_yield();
    
  id++;

  global_seed=id;

  if ((intptr_t) arg > 0) {
    assert((pthread_mutex_unlock(&thread_array->mutex)) == 0);
    printf("Thread %lu unlock mutex\n",pthread_self());
  }
  fflush(stdout);

  pthread_exit(&id);
}

int main(int argc, char *argv[]) {
  pthread_t thread_id[MAXTHREADS];
  void *thread_result;
  int i;
  intptr_t id;

  if (argc != 2) {
    printf("Usage: %s number\n",argv[0]); return 0;
  }
  id = atoi(argv[1]);
  	
  assert((thread_array = (my_array_t *)malloc(sizeof(my_array_t))) != NULL);
  memset(thread_array,0,sizeof(my_array_t));
  assert((pthread_mutex_init(&thread_array->mutex,NULL)) == 0);
  
  for (i=0; i < MAXTHREADS;i++) {
    printf("Mama ready to create thread (%d)\n",(int) id);
    fflush(stdout);

    assert((pthread_create(&(thread_id[i]),NULL,
			   thread_routine,(void *) id)) == 0);
    printf("Mama created thread %lu\n",(thread_id[i]));
    fflush(stdout);
  }

  for (i=0; i < MAXTHREADS;i++) {
    printf("Mama waiting for thread  (%d/100) %lu to end\n",i,thread_id[i]);
    fflush(stdout);
    assert((pthread_join(thread_id[i],&thread_result)) == 0);
  }

  printf("Sum of changes variables was %lu\n", global_seed);
  fflush(stdout);
  assert((pthread_mutex_destroy(&thread_array->mutex)) == 0);
  free(thread_array);
  
  exit(0);
}
