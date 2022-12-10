#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

struct test { int a; int b; };

/*
 * This function returns values that have been allocated from
 * thread stack.  It will explode.
 */
void *thread_routine(void *arg) {
  struct test stuff = {1,2};
  char *msg = malloc(15);
  strncpy(msg,"hello parent!!",14);
  msg[14] = '\0';
  if (arg != NULL)
    pthread_exit((void *)msg);
  else {
    printf("Should return: %d/%d\n",stuff.a,stuff.b);
    pthread_exit((void *)&stuff);
  }
}

/*
 * This function allocates separate memory for return value.  Works.
 */ 
void *thread_routine2(void *arg) {
  struct test *stuff = malloc(sizeof(struct test));

  stuff->a = 1;
  stuff->b = 2;

  printf("Should return: %d/%d\n",stuff->a,stuff->b);

  pthread_exit((void *)stuff);
}

int main(void) {
  pthread_t thread_id;
  struct test *stuff2;

  void *thread_result;

  assert((pthread_create(&thread_id,NULL,thread_routine,(void *) 1)) == 0);

  assert((pthread_join(thread_id,&thread_result)) == 0);

  if ( thread_result == NULL)
    printf("Error on pthread_join, child passed NULL\n");
  else {
    printf("Result was %s\n", (char *)thread_result);
    free(thread_result);  /* Remember to free the memory allocated by the thread */
  }

  assert((pthread_create(&thread_id,NULL,thread_routine,(void *) 0)) == 0);

  assert((pthread_join(thread_id,&thread_result)) == 0);

  if ( thread_result == NULL)
    printf("Error on pthread_join, child passed NULL\n");
  else {
    stuff2 = (struct test *) thread_result;
    printf("Result was %d/%d\n",stuff2->a,stuff2->b);
  }

  assert((pthread_create(&thread_id,NULL,thread_routine2,(void *) 0)) == 0);

  assert((pthread_join(thread_id,&thread_result)) == 0);

  if ( thread_result == NULL)
    printf("Error on pthread_join, child passed NULL\n");
  else {
    stuff2 = (struct test *) thread_result;
    printf("Result was %d/%d\n",stuff2->a,stuff2->b);
    free(thread_result); /* Remember to free the memory allocated by the thread */
  }

  exit(0);
}
