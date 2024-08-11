# Part 1
Hello and welcome to the C insanity! I love this language and I hope you will, too!

## TOC
1. [Differences wrt Python](#1-differences-wrt-python)
2. [Basic types and variable scope](#2-basic-types-and-variable-scope)
3. [Compiling and debugging](#3-compiling-and-debugging)
4. [Pointers](#4-pointers)
5. Static arrays and strings; Structures
6. Dynamic arrays with `malloc`

## 1. Differences wrt Python
C and Python are quite different, duh. More specifically:
- C is more low level, meaning that some stuff you do in 1 Python line takes 3 hours in C (fun!)
- C is _compiled_, while Python is interpreted. This means C needs to be, well, compiled (more on this later). This also means it's way faster to run than Python.
- C is a pure _procedural_ language, so forget about objects (you can still try to emulate object programming in C, tho)
- C is a _weakly typed_ language (more on this later)

### C syntax stuff
After every instruction you need to add a `;`. If you forget it, the compiler will complain. Blocks of code are determined by using curly brackets instead of indentation. 

You get the same constructs of Python (if, for, while), with the following differences:
- *__NO for each!__* : Too high level, forget about it.
- *__do...while__* : A variation of while where the first iteration is always executed, no matter the condition.
- *__switch...case__* : The supreme selection thing. When I discovered Python didn't have it I had a stroke.

> See this stuff in action in [c-vs-py.c](./c-vs-py.c)!

## 2. Basic types and variable scope
Here we talk about types and variable!

### Types
I said that C is a _weakly typed_ language. What does this mean? 

Well, _typed_ means that every variable is associated to a type. Before being used, variables must be _declared_, meaning you must give them a name and a type. Types are many: `int`, `float`, `char` are just a few examples. You can also create your own types! (more on this later).

_Weakly_ means that you can do whatever you want with these types, and convert each type into another. This is called a _cast_. Casting a variable (ie, changing its type) is dangerous because there are *__FEW TO NO RULES__*, 
and, in any case, you can always avoid them. This could be confusing in the beginning, but you'll get use to it.
```c
// casting a char to an int
char c = 'B';
int c_int = (int)c;

printf("Char: %c\nHex value: %#x\n", c, c_int);
// Char: B
// Hex value: 0x42
```

Before casting, it's important to know the size of the variables, because if you cast a larger variable into a smaller one you'll make a real mess (usually). This can be done with the `sizeof()` function.

### Scope
When you declare a variable, you must keep in mind that it will exist only in a specific portion of the code. That portion is called _scope_, and it's usually delimited by curly brackets. If you attempt to call a variable outside its scope, the compiler will kidnap your mum.

```c
int v1 = 0;
if (v1 == 0) {
    char v2 = 'A';
    // here v1 exists
}
// v1 still exists
// v2 doesn't exist anymore :(
```

## 3. Compiling and debugging

### Compiling
I spoke a lot about the C compiler. Let's look into it.

To compile a single file program, you can run
```sh
gcc -o <program> <program.c> 
```
If you have multiple files, it's better to write a Makefile (spoiler for next lessons).

The compiler has a bunch of useful options (I don't even know 1/4 of them). These are some you might like:
- `-g` to keep debug symbols (more on this later);
- `-O` to handle optimization.

There are many optimization levels:
- `-O0` : No optimization;
- `-O1` : Minimal optimizations which don't increase compilation time;
- `-O2` : More aggressive optimizations that improve performance without excessively increasing executable size or compilation time [used for production builds];
- `-O3` : All the possible optimization, thus reducing compilation time and executable size;
- `-Os` : Reduce the executable size as much as possible (*__sometimes very useful for embedding systems!!__*);

### Debugging
The most C relatable experience is getting a segmentation fault out of nowhere and being lost. This is where the lovely `gdb` debugger comes in! 

> Since vanilla `gdb` is a bit overwhelming, I recommend [`gef`](https://github.com/hugsy/gef).

Some useful commands:
- `gdb <program>` : starts the debugger
- `b` : places a breakpoint. Examples - `b <function>`, `b <file>:<line>`, `b *0x<address>`.
- `run <program arguments>` : starts the program
- `n` : executes next line
- `ni` : executes next program instruction
- `c` : continues until a breakpoint is met
- `p <variable>` : print variable content

## 4. Pointers
This is where it gets real dirty real fast. 

Up to now, we considered a bunch of variables. These variables are saved somewhere in memory, meaning they have an _address_. The address of a variable can be accessed via the `&` symbol.
```c
int response = 42;
printf("The response is %d and lies at address %p\n", response, &response);
// The response is 42 and lies at address 0x7fffffffd614
```
Addresses can be saved in special typed variables, called _pointers_.
```c
int response = 42;
int *resp_addr = &response;
printf("The response is %d and lies at address %p\n", response, resp_addr);
// The response is 42 and lies at address 0x7fffffffd614
```

Since pointers are just addresses, every pointer has the same size, even though the pointed variables have different sizes.
```c
int v1 = 42;
double v2 = 3.14159265;
printf("v1 is an int and has size %ld\n", sizeof(v1));
printf("v2 is a double and has size %ld\n", sizeof(v2));
printf("********************\n");
printf("&v1 is an int* and has size %ld\n", sizeof(&v1));
printf("&v2 is a double* and has size %ld\n", sizeof(&v2));
// v1 is an int and has size 4
// v2 is a double and has size 8
// ********************
// &v1 is an int* and has size 8
// &v2 is a double* and has size 8
```
Pretty cool, right? This is just the beginning.

> Try this stuff with [pointers.c](./pointers.c)!