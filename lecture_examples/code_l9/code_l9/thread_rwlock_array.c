#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#define MAXTHREADS 10

typedef struct {
  pthread_rwlock_t	rwlock;
  unsigned long int	sum;
  unsigned long 	array[MAXTHREADS];
  int                   readers;
  int                   writers;
} my_array_t;

my_array_t *thread_array;

void arr_fill(void);
void arr_read(void);
void arr_write(void);

/* Fill up the array with random values */
void arr_fill(void) {
  int i;

  for (i=0;i < MAXTHREADS;i++) {
    thread_array->array[i] = random();
  }
}

/* Read the array and print information to stdout */
void arr_read(void) {
  int i;

  printf("Thread %lu getting the read lock to array\n", pthread_self());
  fflush(stdout);
  assert((pthread_rwlock_rdlock(&thread_array->rwlock)) == 0);
  printf("Thread %lu joining on readers\n", pthread_self());
  fflush(stdout);
  thread_array->readers++;
  assert(thread_array->writers == 0);
  printf("Now %d readers %d writers\n",thread_array->readers, thread_array->writers);
  fflush(stdout);

  for(i = 0; i < MAXTHREADS; i++) {
    printf("Value of array[%d] = %lu\n",i,thread_array->array[i]);
    fflush(stdout);
    sched_yield(); /* Give up prosessor */
  }

  printf("Sum of array is %lu\n",thread_array->sum);
  fflush(stdout);
  thread_array->readers--;
  assert((pthread_rwlock_unlock(&thread_array->rwlock)) == 0);
  printf("Thread %lu exits from read\n", pthread_self());
  fflush(stdout);
}

void arr_write(void) {
  int i;

  printf("Thread %lu getting the write lock to array\n", pthread_self());
  fflush(stdout);
  assert((pthread_rwlock_wrlock(&thread_array->rwlock)) == 0);
  printf("Thread %lu joining on writers\n", pthread_self());
  fflush(stdout);
  thread_array->writers++;
  assert(thread_array->writers == 1);
  assert(thread_array->readers == 0);
  printf("Now %d readers %d writers \n", thread_array->readers,thread_array->writers);
  fflush(stdout);

  for (i = 0; i < MAXTHREADS; i++) {
    printf("Value of array[%d] = %lu\n",i,thread_array->array[i]);
    thread_array->array[i]++;
    thread_array->sum+=thread_array->array[i];
    fflush(stdout);
    sched_yield(); /* Give up prosessor */
  }
  printf("Sum of array is now %lu\n",thread_array->sum);
  thread_array->writers--;
  assert((pthread_rwlock_unlock(&thread_array->rwlock)) == 0);
  printf("Thread %lu exits from write\n", pthread_self());
  fflush(stdout);
}

void *thread_routine(void *arg) {
  int i;
  printf("Thread %lu in thread_routine, request mutex\n", pthread_self());
  fflush(stdout);

  for(i=0;i < 1000;i++) {
    if ( random() % 2) arr_read();
    else arr_write();

    sched_yield(); /* Give up the prosessor */
  }

  pthread_exit(arg);
}

int main(void) {
  pthread_t thread_id[MAXTHREADS];
  void *thread_result;
  int i;

  assert((thread_array = (my_array_t *)malloc(sizeof(my_array_t))) != NULL);
  memset(thread_array,0,sizeof(my_array_t));
  arr_fill();
  assert((pthread_rwlock_init(&thread_array->rwlock,NULL)) == 0);

  for (i=0; i < MAXTHREADS;i++) {
    assert((pthread_create(&(thread_id[i]),NULL,
                           thread_routine,NULL)) == 0);
    printf("Mama created thread %lu\n",(thread_id[i]));
    fflush(stdout);
  }

  for (i=0; i < MAXTHREADS;i++) {
    printf("Mama waiting for thread %lu to end\n", thread_id[i]);
    fflush(stdout);
    assert((pthread_join(thread_id[i],&thread_result)) == 0);
  }

  assert((printf("Sum of random variables was %lu\n",
                 thread_array->sum)) > 0);
  fflush(stdout);
  assert((pthread_rwlock_destroy(&thread_array->rwlock)) == 0);
  free(thread_array);
  
  exit(0);
}
