#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// --------- classic student --------- //
struct _student_ {
    char name[33]; // 32 + 1, string '\0'
    int age;
    float gpa;
};
// aliasing
typedef struct _student_ student;
// utilities
student init_student(char* name, int age, float gpa) {
    student new;
    strncpy(new.name, name, 32);
    new.age = age;
    new.gpa = gpa;
    return new;
}
void print_student(student stud) {
    printf("This is %s, they're %d years old and their GPA is %.2f\n", stud.name, stud.age, stud.gpa);
}

// --------- dynamic student --------- //
struct _student_d_ {
    char* name; // we will allocate this field with malloc
    int age;
    float gpa;
};
// aliasing --> notice that now we are aliasing the POINTER!
typedef struct _student_d_* student_d;
// utilities
student_d init_student_d(char* name, int age, float gpa) {
    student_d new = malloc(sizeof(struct _student_d_));
    // strdup == malloc+strcpy
    new->name = strdup(name);
    new->age = age;
    new->gpa = gpa;
    return new;
}
// now we also need the free
void free_student_d(student_d stud) {
    // first, free the name
    free(stud->name);
    // then, free the student
    free(stud);
}
void print_student_d(student_d stud) {
    printf("This is DYNAMIC %s, they're %d years old and their GPA is %.2f\n", stud->name, stud->age, stud->gpa);
}


int main() {
    // classic student
    student jean_luc = init_student("jean-luc", 69, 3.3);
    print_student(jean_luc);

    // dynamic student
    student_d john_locke = init_student_d("john locke", 42, 3.9);
    print_student_d(john_locke);
    free_student_d(john_locke);

    return EXIT_SUCCESS;
}
