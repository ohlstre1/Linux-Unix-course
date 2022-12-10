#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <pthread.h>
#include <assert.h>

#define MAXTHREADS 10

static unsigned long global_seed = 1751;

typedef struct {
  pthread_mutex_t	mutex;
  int                   id;
  unsigned long 	array[MAXTHREADS+1];
} my_array_t;

my_array_t *thread_array;

void *thread_routine(void *arg) {
  register unsigned long tmp_seed;
  int id;

  printf("Thread %lu in thread_routine, request mutex\n",pthread_self());
  fflush(stdout);
  assert((pthread_mutex_lock(&thread_array->mutex)) == 0);
  printf("Thread %lu got mutex\n",pthread_self());
  fflush(stdout);

  tmp_seed = 16807 * ( global_seed % 127773) - 2836 * 
    		     ( global_seed / 127773);

  if ( tmp_seed > 0) global_seed = tmp_seed;
  else global_seed = tmp_seed + 2147483647;

  id = thread_array->id;
  thread_array->array[id] = global_seed;
  thread_array->array[MAXTHREADS] += global_seed;

  assert((printf("Thread %lu storing random value %lu to array pos %d\n",
                 pthread_self(),global_seed,id)) > 0);
  fflush(stdout);

  thread_array->id++;
  
  sched_yield();
  assert((pthread_mutex_unlock(&thread_array->mutex)) == 0);
  printf("Thread %lu unlock mutex\n",pthread_self());
  fflush(stdout);

  pthread_exit(arg);
}

int main(void) {
  pthread_t thread_id[MAXTHREADS];
  void *thread_result;
  int i;

  assert((thread_array = (my_array_t *)malloc(sizeof(my_array_t))) != NULL);
  memset(thread_array,0,sizeof(my_array_t));
  assert((pthread_mutex_init(&thread_array->mutex,NULL)) == 0);

  for(i=0; i < MAXTHREADS;i++) {
    int id = i;
    printf("Mama ready to create thread\n");
    fflush(stdout);

    assert((pthread_create(&(thread_id[i]),NULL,
                           thread_routine,(void *)&id)) == 0);
    printf("Mama created thread %lu\n",(thread_id[i]));
    fflush(stdout);
  }

  for(i=0; i < MAXTHREADS;i++) {
    printf("Mama waiting for thread %lu to end\n",thread_id[i]);
    fflush(stdout);
    assert((pthread_join(thread_id[i],&thread_result)) == 0);
  }

  assert((printf("Sum of random variables was %lu\n",
		thread_array->array[MAXTHREADS])) > 0);
  fflush(stdout);
  assert((pthread_mutex_destroy(&thread_array->mutex)) == 0);
  free(thread_array);
  
  exit(0);
}
