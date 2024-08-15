// write a program that converts from bin to int and vice versa, using ONLY BITWISE OPERATIONS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int bin4_to_int(int* bin){
    int result = 0 ;
    for (int i=0; i<4; i++) {
        // make space for the new bit
        result = result << 1 ; 
        // set the bit with bitwise OR
        result = result | bin[i] ;
    }
    return result ;
}

// we don't want to shadow allocate in the function
// main will allocate bin and pass it as a parameter
void int_to_bin4(int value, int* bin){
    int mask = 1;
    for (int i=3; i>=0; i--) {
        // look only at the lsb
        bin[i] = value & mask;
        // prepare next bit for the following iteration
        value = value >> 1;
    }
    return;
}

int main(){
    int* bin = calloc(4, sizeof(int));
    // init bin to 1101 (13)
    bin[0] = 1;
    bin[1] = 1;
    bin[3] = 1;    
    int result = bin4_to_int(bin);
    // reset bin to make sure everything works fine
    bin[0] = 0;
    bin[1] = 0;
    bin[3] = 0; 
    int_to_bin4(result, bin);
    // print results
    printf("%d\n", result);
    for (int k=0; k<4; k++) {
        printf("%d", bin[k]);
    }
    printf("\n");
    // free
    free(bin);
    return EXIT_SUCCESS;
}