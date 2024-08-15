// A script to allocate and free a dynamic matrix!

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int** malloc_2000(int N, int M){
    int** matrix = malloc(N * sizeof(int*));
    for (int k=0; k<N; k++){
        matrix[k] = malloc(M * sizeof(int));
    }
    return matrix;
}

void free_2000(int** matrix, int N){
    for (int k=0; k<N; k++){
        free(matrix[k]);
    }
    free(matrix);
}