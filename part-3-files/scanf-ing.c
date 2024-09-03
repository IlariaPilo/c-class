#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

// 😊 __Only numbers in the building__ //
void logins(int curr_month) {
    // declare the variables
    int year, month, date, hour, min, count;
    count = 0;
    printf("Logins for the current month [%02d], all-time:\n", curr_month);
    // open in read mode
    FILE* fp = fopen("logins.log", "r");
    // until we reach EOF
    while (fscanf(fp, "%d/%d/%d %d:%d", &year, &month, &date, &hour, &min) != EOF) {
        if (curr_month == month) {
            printf("   %d/%02d/%02d %02d:%02d\n", year, month, date, hour, min);
            count ++;
        }
    }
    fclose(fp);
    printf("TOTAL = %d\n", count);
}

// 🤢 __Character at the beginning of the line__ //
void division_wrong() {
    int n1, n2;
    char type;
    printf("\U0001F6AB\U0001F6AB DIVISION UTILITY \U0001F6AB\U0001F6AB\n");
    printf("n1: ");
    scanf("%d", &n1);   // WRONG 🚫
    printf("n2: ");
    scanf("%d", &n2);   // WRONG 🚫
    if (n2==0) {
        printf("[error] cannot divide by 0!\n");
        return;
    }
    // asks for division type
    printf("Which division do you want? [c = classic, f = floating point]: ");
    scanf("%c", &type);
    if (type == 'C' || type == 'c') {
        printf("%d/%d = %d, remainder %d\n", n1, n2, n1/n2, n1%n2);
    } else if (type == 'F' || type == 'f') {
        printf("%d/%d = %f\n", n1, n2, (float)n1/(float)n2);
    } else {
        printf("Sorry, I don't know option [%c]\n", type);
    }
}
void division_correct() {
    int n1, n2;
    char type;
    printf("\U00002705\U00002705 DIVISION UTILITY \U00002705\U00002705\n");
    printf("n1: ");
    scanf("%d%*c", &n1);   // CORRECT ✅
    printf("n2: ");
    scanf("%d%*c", &n2);   // CORRECT ✅
    if (n2==0) {
        printf("[error] cannot divide by 0!\n");
        return;
    }
    // asks for division type
    printf("Which division do you want? [c = classic, f = floating point]: ");
    scanf("%c%*c", &type);
    if (type == 'C' || type == 'c') {
        printf("%d/%d = %d, remainder %d\n", n1, n2, n1/n2, n1%n2);
    } else if (type == 'F' || type == 'f') {
        printf("%d/%d = %f\n", n1, n2, (float)n1/(float)n2);
    } else {
        printf("Sorry, I don't know option [%c]\n", type);
    }
}

// 🤮 __CSV-like structure (with strings)__ //
void johns() {
    // declare the variables
    char fname[32], lname[32];
    int age, count;
    count = 0;
    // open in read mode
    FILE* fp = fopen("people.txt", "r");
    // until we reach EOF
    while (fscanf(fp, "%31[^,],%31[^,],%d%*c", fname, lname, &age) != EOF) {
        if (strcmp("john", fname)==0) {
            count ++;
        }
    }
    fclose(fp);
    printf("I found %d john%s.\n", count, count==1?"":"s");
}

int main() {
    // get current month
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int curr_month = tm_info->tm_mon + 1;

    printf("\n\033[1;32m\U0001F60A Only numbers in the building\033[0m\n");
    logins(curr_month);

    printf("\n\033[1;33m\U0001F922 Character at the beginning of the line\033[0m\n");
    division_wrong();
    division_correct();

    printf("\n\033[1;31m\U0001F92E CSV-like structure (with strings)\033[0m\n");
    johns();
    
    return EXIT_SUCCESS;
}