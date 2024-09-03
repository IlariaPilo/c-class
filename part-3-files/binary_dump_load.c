#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// a global variable, just for fun
int N;

void dump(int* array, char* path) {
    // open `path` in write mode
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    // write N
    write(fd, &N, sizeof(int));
    // write the array
    write(fd, array, sizeof(int)*N);
    // close
    close(fd);
}

int* load(char* path) {
    // open `path` in read mode
    int fd = open(path, O_RDONLY);
    // read N
    read(fd, &N, sizeof(int));
    // allocate space for the array
    int* array = malloc(N*sizeof(int));
    // read the array
    read(fd, array, sizeof(int)*N);
    // close
    close(fd);
    return array;
}

void print_fibonacci(int* array) {
    printf("First %d Fibonacci numbers:\n", N);
    for (int i=0; i<N; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int* make_fibonacci() {
    int* array = malloc(N*sizeof(int));
    array[0] = 1;
    if (N==1) {
        return array;
    }
    array[1] = 1;
    for (int i=2; i<N; i++) {
        array[i] = array[i-1] + array[i-2];
    }
    return array;
}

int main() {
    char choice;
    int *fib;
    printf("** Fibonacci numbers generator **\n");
    printf("Do you want to load the numbers? [y/N]: ");
    scanf("%c%*c", &choice);
    // if yes, load
    if (choice=='y' || choice=='Y') {
        fib = load("fibonacci.bin");
    } else {
        //generate, then dump
        printf("How many numbers? ");
        scanf("%d%*c", &N);
        if (N<=0) {
            printf("N must be positive, sorry.\n");
            return EXIT_FAILURE;
        }
        fib = make_fibonacci();
        dump(fib, "fibonacci.bin");
    }
    // print
    print_fibonacci(fib);
    free(fib);
    return EXIT_SUCCESS;
}