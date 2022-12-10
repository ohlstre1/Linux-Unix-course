
#include <stdio.h>
#include <stdlib.h>

int fib(int n) {
  int x,y;
  if (n<2) return n;
  #pragma omp task shared(x)
  x = fib(n-1);
  #pragma omp task shared(y)
  y = fib(n-2);
  #pragma omp taskwait
  return x+y;
}

int main(int argc,char **argv) {
  if (argc != 2) {
    printf("%s num\n",argv[0]);
    return 1;
  }
  int res = fib(atoi(argv[1]));
  printf("Result: %d\n",res);
  return 0;
}
