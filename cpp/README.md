# C++

In this file, I'll focus on the most important aspects of C++, so that you can learn it _as fast as possible_.

Ready? Go!

## TOC
1. [Types & Program flow](#types--program-flow)
2. [C++ containers](#c-containers)

## Types & Program flow
C++ is, in a way, an extension of C: everything you can do in C, you can do it in C++. However, C++ is newer than C, meaning it makes some things much easier than C. 

### Types
Let's start from the available _types_:
- Basic types (`int`, `long`, `float`, `char`) are still there;
- Introduction of `bool` (either `true` or `false`);
- Introduction of `string` (bye bye `char*`!).

The `string` type is a great improvement over C's clunky `string.h`. Take a look at the [documentation](https://cplusplus.com/reference/string/string/) to see what you can do with C++'s strings.
```cpp
// initialize string
string name = "Jean-Luc";
// copy the content into another string => just assign it
string name_copy = name;
// concatenate => classic +
name_copy = "Prof. " + name;
// compare => use the compare member function, works like strcmp
if (name_copy.compare(name) != 0) {
    cout << "They are different! This is because C++ strings aren't pointers." << endl;
}
```
You can still have good, old-fashioned _pointers_:
```cpp
int i = 0;
// save the address of i
int* i_address = &i;
// increment the value found at the address of i (aka, i)
(*i_address)++;
cout << i << endl;
// 1
```
Moreover, C++ introduces a simplified version of pointers, _references_. References work like pointers (meaning that they can be used to change the variable they are pointing to), but without the complexity of pointers (aka, the `&` and `*` operations):
```cpp
int i = 0;
// create a reference to i
int& i_reference = i;
// increment i_reference
i_reference++;
cout << i << endl;
// 1
```
As you can see, the `int&` type represents a reference to an `int`, and changing that reference results in changing the original variable. Cool, right?

Structures are still there, but they are mostly useless, since C++ has classes and objects (more on this later). 

C-style arrays are also still there, and they are also mostly useless. You should use C++ _containers_ instead (more on this later). If you have arrays with millions of entries, then it makes sense to keep using C-style arrays, since they are less abstract (for example, you know they are allocated in consecutive memory slots).

Finally, C++ has the laziest type of them all: `auto`. Let's say you are defining a variable and setting it to the return value of some function. However, you don't remember if it's a `size_t`, a `long` or a vanilla `int`. No problem: `auto` is there for you! The compiler will automatically detect what type is your function returning, and you don't have to worry about anything at all.
```cpp
string name = "Jean-Luc";
auto some_kind_of_number = name.length();
// since .length() returns a size_t value, some_kind_of_number is a size_t variable
```
⚠️ *__Important :__* DO NOT ABUSE `auto`! Types are very useful to catch nasty errors at compile time, and they make the code much more readable. Sometimes, explicitly writing the type could save you from hours of debugging.

### Program flow structures
As with types, classic C structures are still there, unchanged (`if...else`, `while`, `do...while`, `switch...case`). `break` and `continue` statements are also available.

The new entry is the `for each` kinda loop! This structure can be used with any type that has an iterator function, like in Java. If you don't know what that is, don't worry. If it's a C++ container, or a string, the for each will most likely work.
```cpp
// old, C-style for
int old_v[5] = {1, 2, 4, 8, 16};
for (int k=0; k<5; k++) {
    // do something
}
// C++ for each!
vector<int> new_v = {1, 2, 4, 8, 16};
// FOR EACH
for (int element : new_v) {
    // do something
}
```
`vector<int>` is a C++ container (more on this later). For now, think about it as an array 2.0. With this kind of structure, you can use the new `for each`.
```cpp
// generic structure
for (type element : array) {
    // `element` contains A COPY of the current element
    // if you change it, nothing happens to `array`
}
// lazy structure => use `auto` not to worry about the type
for (auto element : array) {
    // ...
}
// reference structure
for (type& element : array) {
    // `element` contains A REFERENCE to the current element
    // you can modify it!
}
```

## C++ containers
A container is a C++ object
