// use gcc -E to get pre-processor output
#include <stdio.h>
#include <stdlib.h>

#define CHECK_FAIL(n)                               \
    if (n<0) {                                      \
        printf("Your mother is not born yet.\n");   \
        exit(EXIT_FAILURE);                         \
    }

#define MAKE_FLAG(suf) int flag_##suf = 0;

int main() {
    int age;
    printf("Insert your mother's age: ");
    scanf("%d%*c", &age);
    CHECK_FAIL(age)
    MAKE_FLAG(menopause) // flag_menopause
    MAKE_FLAG(underage)  // flag_underage
    if (age < 18)
        flag_underage = 1;
    else if (age > 51)
        flag_menopause = 1;
    if (flag_menopause || flag_underage)
        printf("You won't have a brother!!\n");
    return EXIT_SUCCESS;
}

