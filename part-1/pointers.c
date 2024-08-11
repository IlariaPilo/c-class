// compile with:
// gcc -g -o pointers pointers.c

// double-check it with gdb!
// gdb pointers
// b 17
// run
// n
// p /d response
// p &response 
// c

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char*argv[]) {

    int response = 42;
    printf("The response is %d and lies at address %p\n", response, &response);

    int v1 = 42;
    double v2 = 3.14159265;
    printf("v1 is an int and has size %ld\n", sizeof(v1));
    printf("v2 is a double and has size %ld\n", sizeof(v2));
    printf("********************\n");
    printf("&v1 is an int* and has size %ld\n", sizeof(&v1));
    printf("&v2 is a double* and has size %ld\n", sizeof(&v2));

    return EXIT_SUCCESS;
}