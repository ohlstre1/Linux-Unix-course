
#include <stdio.h>
#include <omp.h>

int main(void) {
//  #pragma omp parallel
  {
//    #pragma omp single
    {
      int id = omp_get_thread_num();
//      #pragma omp task
      { printf("Hello(%d) ",id); }
//      #pragma omp task
      { printf("World(%d) ",id); }
//      #pragma omp taskwait
      printf("\nThank you(%d) ",id);
    }
  } // End of parallel region
  printf("\n");
  return 0;
}
