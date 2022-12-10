
#include <stdio.h>
#include <omp.h>

int main(void) {
  int tmp = 0;
// Try private(tmp), firstprivate(tmp, lastprivate(tmp)
  #pragma omp parallel for
  for (int i=0; i<1000; i++) {
    tmp += i;
  }
  printf("%d\n",tmp);
  return 0;
}
