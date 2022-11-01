/* Memory allocation test program.  How much can you allocate memory? */

#include <stdio.h>
#include <stdlib.h>

/* Example of Variable-Length Array and alloca() */
int calculator(int size) {
    int number[size];
    int *number2 = alloca(size*sizeof(int));    
    number[0] = number2[0];
    number2[0] = number[0]; // Just something to keep compiler quiet about unused variables
    return 0;
}


int main(int argc,char **argv) {
    long size;
    
    if (argc != 2) { printf("give me one argument\n"); return -1; }
    size = strtol(argv[1],NULL,10);
    char *memory = malloc(size);
    if (memory != NULL) {
        printf("Allocation succeeded\n");
    } else {
        printf("Allocation failed\n");
    }
    long i;
    for (i=0; i<size; i++) {
        memory[i] = 1;
    }
    printf("Write done\n");

    return 0;
}
