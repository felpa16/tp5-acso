#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "inode.h"
#include "diskimg.h"


int main(void) {
    // int a = 4;
    int* pointer = malloc(sizeof(int));
    int* x = (int*) malloc(3*sizeof(int));
    x[0] = 3;
    *pointer = x[0];
    free(x);
    printf("%i", *pointer);
    return 0;
}