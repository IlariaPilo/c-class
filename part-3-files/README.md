# Part 3
Today we are going to talk about structures and files. Enjoy!

## Structures
Structures can be used to pack together different variables, using an object-programming-kinda fashion. 

Defining a `struct` is pretty straightforward:
```c
struct _student_ {
    char name[33]; // 32 + 1, string '\0'
    int age;
    float gpa;
};
```
Fields in a structure can be accessed with the `.` operator:
```c
// in the main
struct _student_ jean_luc;
jean_luc.age = 69;
```

Now, we just defined a new `struct _student_` type. That's great! 
However, writing `struct _student_` every time we want to deal with a student variable is long, and makes the code heavier.

For this reason, we can create an alias using `typedef`:
```c
typedef struct _student_ student;
```
In C libraries, you'll find `typedef` _everywhere_, since many weird types are just standard ints and longs in disguise. Some examples?
```c
// the value returned by sizeof() is just an unsigned long. But size_t sounds a lot better!
typedef unsigned long size_t
// the time (in seconds) is nothing but a long with a fancier name.
typedef long time_t
// the id of a process is a simple int. Who could have seen that coming!
typedef int pid_t
```

But let's go back to structs.

Sometimes it could be useful to define functions to initialize and print a struct:
```c
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
```

### `struct` + `malloc`
Of course, it's possible to dynamically allocate structures. For example, if you want to create an array of `N` students, you can rely on the classic:
```c
// CORRECT ✅
student* class = malloc(N * sizeof(student));
// do some computations
free(class);
```
You can also create structures with dynamically allocated fields. *__This is where you must be VERY CAREFUL__*.

Let's say you want to dynamically allocate the name, so that there is no 32-char limit on it. In that case, you might create a new, dynamic version of `struct _student_`:
```c
struct _student_d_ {
    char* name; // we will allocate this field with malloc
    int age;
    float gpa;
};
```
Now, you _might_ be tempted to do this, but you _shouldn't_:
```c
// WRONG 🚫
struct _student_d_ john_locke;
john_locke.name = strdup("john locke");
// strdup == malloc+strcpy
```
This is not a good programming practice, for 2 reasons:
1. A manual variable inside an automatic package -- Since `name` is dynamically allocated, you (the programmer) are the one that chooses when to create and free it. On the other hand, `john_locke` is a static variable, so it follows the usual scope rules.
2. Splitting responsibility (see [Part 1](../part-1-basics/README.md#6-dynamic-arrays-with-malloc)) -- `name` is, in a way, _hidden_ inside a nice struct package, therefore the struct should be in charge of allocating and freeing it!

To avoid confusion (and memory leaks), a structure with a dynamic field should be dynamically allocated altogether.
```c
// CORRECT ✅
// aliasing --> notice that now we are aliasing the POINTER!
typedef struct _student_d_* student_d;
```
Since now `student_d` is a _pointer_, accessing each field requires dereferencing (`*` operator). However, writing `(*stud).name` is horrendously complex. This is where the `->` operator comes in!
```c
// CORRECT ✅
student_d john_locke = init_student_d("john locke", 42, 3.9);
printf("%s\n", john_locke->name);
// this is the equivalent to (*john_locke).name, but much more readable!
```
Now that all the pieces are at their place, we can look at the utilities' implementation:
```c
// CORRECT ✅
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
```
Overall, allocating fields with `malloc` makes the whole struct deal much more complex, so you should take it into account when designing a program

And that's all for structures!

>See these examples in action in [student.c](./student.c)!

## Files
Time to talk about files! In C, there are 2 ways to interact with files:
1. Binary I/O (`read()`/`write()`);
2. Formatted I/O (`scanf()`/`printf()` family).

If you have a binary file, you should use binary I/O. If you have a textual file (e.g., a csv), go with formatted I/O.

### Formatted I/O
In most cases, formatted I/O is the way to go, so I'll start from here. Files can be quite boring, so I'll keep it simple.

#### Opening and closing the file
First, you need to open the file. This can be done with [`fopen()`](https://linux.die.net/man/3/fopen) (formatted open).
```c
#include <stdio.h>
FILE *fopen(const char *path, const char *mode);
```
`path` is the name of the file, `mode` is a string that determines whether we want to read (`"r"`), write (`"w"`), or append (`"a"`) to the file. There are other, more specific modes. If you are curious, look at the documentation.

In the formatted I/O approach, files are `FILE *` objects. `FILE` is a typedef for a structure that represents streams:
```c
// Told you, this typedef fella is everywhere
typedef struct _IO_FILE FILE
```
This structure is _opaque_, meaning that its fields are kept hidden to the programmer, and shouldn't be directly accessed/manipulated. Some `FILE *` exists for standard input (`stdin`), output (`stdout`) and error (`stderr`).

Once you are done, you have to close the file.
```c
int fclose(FILE *fp); 
```

#### `fscanf` and `fprintf`
Now it's time to actually read/write. To do so, there are 2 families of functions: `scanf()` to read and `printf()` to write.

They are _families_ because each of them exists in 3 different flavours: classic (e.g., `printf`), string (`sprintf`) and file (`fprintf`). The first letter determines the type of I/O stream:
- No prefix : standard I/O (the terminal);
- `s` : a string;
- `f` : a file.

I will mainly focus only on the `f` variations, but no worries! They all work the same way.

Introducing `fprintf`!
```c
int fprintf(FILE *stream, const char *format_string, ...);
```
- `stream` is the file pointer we got from `fopen`. Easy. 
- `format_string` is the message we want to write. In the message, each variable is substituted with a placeholder according to its type (this is why it's called "format").
- Finally, you need to pass the variables that must be inserted in the format string, in the order in which they appear.

All this complex mumbo-jumbo to simply say:
```c
int result;
// do some computations
fprintf(stdout, "Result: %d\n", result);
//         ^              ^        ^
//         |              |        |
//         `- file        |        `- variable to substitute in the first placeholder
//                        `- placeholder for `int` variables 
```
There are a lot of format specifiers. Some of the most common are:

| Specifier | Type |
| :-------: | :--- |
| `%d` | int |
| `%x` | int (hexadecimal) |
| `%f` | float |
| `%s` | string |
| `%c` | char |
| `%p` | pointer |

It's also possible to specify the number of fractional digits, the minimum number of overall digits and a lot of other things. If you have specific requirements on how your output must look like, Stack Overflow is your friend! 

Here comes the annoying bit, `fscanf`. On the surface, it looks like the same old `printf` deal.
```c
int fscanf(FILE *stream, const char *format_string, ...);
```
- `stream` is the file pointer. Basic. 
- `format_string` is how the message we want to read looks like. This is the complex part.
- In a dual way to `printf`, you need to pass the *__addresses__* where you want the values to be stored, one per placeholder.

This function returns an integer value representing the number of input items successfully matched and assigned. If the input ends before the first matching failure or if an error occurs, EOF (End Of File) is returned.

`scanf` can be really painful to use, since sometimes it behaves quite unpredictably. Here's a quick guideline on how to handle it.

|   | Category | Comment | File line | Format |
|:-:| :--------|:--| :-------- | :----- |
|😊| _Only numbers in the building_ | The most normal `scanf` situation | 13:24 | `"%d:%d"` |
|🤢| _Character at the beginning of the line_ | Don't forget to discard the `\n` with `%*c` | A 13:24 | `"%c %d:%d%*c"` |
|🤮| _CSV-like structure (with strings)_ | Use regex-style syntax, explicit the maximum length | john,locke,42 | `"%31[^,],%31[^,],%d%*c"` |

The main issue with `scanf` format strings is ambiguity. Let's looks at the _only numbers_ example: in the line "13:24", "13" and "14" are evidently numbers, while ":" is evidently not-a-number. Since there is no ambiguity, everything works as expected.

Characters are more complex, since the ASCII table contains a bunch of non-printable characters. The `\n` at the end of each line is clearly a character, therefore it will match with the first `%c` in the _character at the beginning of the line_ example, unless you discard it with `%*c`. If you are using Windows it will be even worse, as you'll get both a carriage return (`\r`) and a newline you have to get rid of.

Strings are arguably the worst, in terms of ambiguity. In C, a string terminates when `s[i] == \0`. But this isn't true in a file, right? You can have files where each string is a line, files where strings are separated by space, or comma, or dash, or whatever your heart desires. So, to get the behavior you like, you have to stick to the regex syntax.

I'll put here some examples, if you don't care feel free to skip ahead!

😊 __Only numbers in the building__

Suppose you have a log file collecting some login timestamps, and you want to count the number of logins in the current month.
```c
/* -------------
logins.log
2024/09/02 13:24
2024/08/31 09:59
------------- */

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
```
Then, you will get:
```
Logins for the current month [09], all-time:
   2024/09/02 13:24
TOTAL = 1
```

🤢 __Character at the beginning of the line__

Suppose you want to create a division utility that reads two numbers from the terminal, and then asks whether the division should be floating point or result & remainder.
```c
void division_wrong() {
    int n1, n2;
    char type;
    printf("### DIVISION UTILITY ###\n");
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
```
In this case, you have to input two numbers and a character, meaning the stream will look like this:
```py
stream = "42\n3\nc\n"
#         ^   ^  ^
#         |   |  |
#         n1  n2 type
```
However, your overall format string will look like this:
```py
format_string = "%d%d%c"
```
If we put everything together, we get:
```py
stream
 "42"   # a number (matches %d, "%d%c" remaining)
 "\n"   # not-a-number (skipped, "%d%c" remaining)
 "3"    # a number (matches %d, "%c" remaining)
 "\n"   # a character (matched %c, format string is over)
 "c"    # skipped
 "\n"   # skipped
```
With the wrong implementation, `\n` is interpreted as the option, and you'll get:
```
### DIVISION UTILITY ###
n1: 42
n2: 3
Which division do you want? [c = classic, f = floating point]: Sorry, I don't know option [
]
```
Let's now add the discard operator `%*c`:
```c
void division_correct() {
    int n1, n2;
    char type;
    printf("### DIVISION UTILITY ###\n");
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
```
Now everything works fine, and the option is correctly parsed:
```
### DIVISION UTILITY ###
n1: 42
n2: 3
Which division do you want? [c = classic, f = floating point]: c
42/3 = 14, remainder 0
```

🤮 __CSV-like structure (with strings)__

Suppose you want to read a CSV file containing `first_name,last_name,age`, and you want to count the number of people named "John".
```c
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
    printf("I found %d john%s.", count, count==1?"":"s");
}
```
Since the separator is `,`, each string is formatted as `%31[^,]` (meaning "at most 31 characters which are not comma"). Since the comma is skipped, we write it down. Finally, we read the age as integer and discard the `\n`.

>You can find these examples in [scanf-ing.c](./scanf-ing.c).

#### Some facts about the `s` variations that I have to put somewhere

TODO


### Binary I/O

TODO


