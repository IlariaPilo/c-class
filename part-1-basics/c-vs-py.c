// compile with:
// gcc -o c-vs-py c-vs-py.c

#include <stdio.h>
#include <stdlib.h>

void while_vs_do_while(int param) {
    int og_param = param;
    // try while
    while (param > 22) {
        printf("\t[while] %d\n", param);
        param /= 2;
    }
    printf("while done cause %d <= 22\n", param);
    // try do-while
    param = og_param;
    do {
        printf("\t[do while] %d\n", param);
        param /= 2;
    } while (param > 22);
    printf("do while done cause %d <= 22\n\n", param);
    return;
}

int switch_operation() {
    char op;

    do {
        printf("About to compute 19 ? 3. Which operation do you want me to use?\n");
        printf("Options: +, -, *, /, %%\n");
        // read from stdin
        scanf(" %c", &op);
        switch (op) {
        case '+':
            return 19+3;
        case '-':
            return 19+3;
        case '*':
            return 19*3;
        case '/':
            return 19/3;
        case '%':
            return 19%3;
        default:
            printf("sorry, I don't know what to do\n");
        }
    } while(1);
}

int main(int argc, char*argv[]) {

    if (argc != 2) {
        printf("Usage: %s INT\n", argv[0]);
        return EXIT_FAILURE;
    }
    // get the param
    int param = atoi(argv[1]);

    // call while_vs_do_while
    printf("\nCalling while_vs_do_while with param %d\n", param);
    while_vs_do_while(param);

    // call switch_operation
    int res = switch_operation();
    printf("Got %d\n", res);

    return EXIT_SUCCESS;
}