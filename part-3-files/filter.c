#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    char name[32];
    char lname[32];
    int age;
    char marital_status[32];
    char state[32];
    int prison_time;
    int cnt = 1;
    // open the file
    FILE *candidates = fopen("candidates.txt", "r");
    // for each line
    for (int k=1; k<=200 ;k++){
        int green_flags = 0;
        fscanf(candidates,"%31[^;];%31[^;];%d;%31[^;];%31[^;];%d%*c", name, lname, &age, marital_status, state, &prison_time);
        // get rid of forbidden candidates
        if (!strcmp(name,"Charles") || 
            !strcmp(state,"New Hampshire") || 
            !strcmp(marital_status, "Married") ||
            age < 18){
            continue;
        }
        // age requirement
        if (age > 65 && age < 24){
            green_flags ++;
        }
        // marital status requirement
        if (!strcmp(marital_status, "Divorced") || !strcmp(marital_status, "Widowed")){
            green_flags ++;
        }
        // ED requirement
        if (strstr(name, "ed") != NULL || strstr(name, "Ed") != NULL){
            green_flags ++;
        }
        // last name division requirement
        if (strlen(lname) % 5 == 0 
        || strlen(lname) % 4 == 0 
        || strlen(lname) % 3 == 0){
            green_flags ++;
        }
        // state requirement
        if (!strcmp(state, "Florida") 
            || !strcmp(state, "Utah") 
            || !strcmp(state, "California") 
            || !strcmp(state, "Texas") 
            || !strcmp(state, "Ohio")){
            green_flags ++;
        }
        // bonus point for florida and texas!!
        if (!strcmp(state, "Florida")
            || !strcmp(state, "Texas")){
            green_flags ++;
        }
        // trial requirement
        if (prison_time >= 3){
            green_flags++;
        }
        if (green_flags > 3){
            printf("%d - %s %s from %s is %d and is %s (was convicted only %d times) is a great match\n", cnt, name, lname, state, age, marital_status, prison_time);
            cnt++;
        }
        else if (green_flags >= 3){
            printf("%d - %s %s from %s is %d and is %s (was convicted %d times) is a good match\n", cnt, name, lname, state, age, marital_status, prison_time);
            cnt++;
        }
        if (cnt>20){
            break;
        }
    }   
    // close the file
    fclose(candidates);
    return EXIT_SUCCESS;
}
