#include<stdio.h>
#include<stdlib.h>

// FIXME : doesn't work with negative numbers! :c
int fits_in_1B(int x) {
    int x_1B = x & 0xff;
    return x_1B == x;
}

int main(){

    printf("    42 fits in 1B? [y] : %c\n", fits_in_1B(42)?'y':'n');  
    printf("481516 fits in 1B? [n] : %c\n", fits_in_1B(481516)?'y':'n');
    printf("   -42 fits in 1B? [y] : %c\n", fits_in_1B(-42)?'y':'n');

    return EXIT_SUCCESS;
}