// compile with:
// gcc -g -o arrays arrays.c

#include <stdio.h>
#include <stdlib.h>

#define N 10

int main(int argc, char*argv[]) {

    int digits[N];
    int i;
    // init `digits`
    for (i=0; i<N; i++) {
        digits[i] = i;
    }
    // print content
    printf("Decimal system digits: ");
    for (i=0; i<N; i++) {
        printf("%d ", digits[i]);
    }
    printf("\n");
    // play with pointers
    int *also_digits = digits;  
    // notice we don't need & because `digits` is already a pointer!
    printf("The third digit of the decimal system is %d\n", *(also_digits+2));

    return EXIT_SUCCESS;
}