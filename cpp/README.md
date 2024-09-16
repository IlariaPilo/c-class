# C++

In this file, I'll focus on the most important aspects of C++, so that you can learn it _as fast as possible_.

Ready? Go!

## TOC
1. [Types & Program flow](#types--program-flow)
2. [Containers](#c-containers)
3. [Algorithms](#c-algorithms)

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

The new entry is the `for each` kinda loop! This structure can be used with any type that has an iterator function, like in Java. If you don't know what that is, don't worry. If it's a C++ container, a string, or a statically allocated C-style array, the for each will most likely work.
```cpp
// old, C-style for
int v[] = {1, 2, 4, 8, 16};
for (int k=0; k<5; k++) {
    // do something
}
// C++ for each!
for (int element : v) {
    // do something
}
```
You can use the `for each` in many different flavours:
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
A container is a C++ object made to collect other C++ objects. The container manages the space for its elements, and provides functions to access them. I will now talk about what I think are the most common containers. Take a look at the [documentation](https://en.cppreference.com/w/cpp/container) to know more about them (they are very cool).

There are 3 (+1) categories of containers:
- _Sequence containers_ - data structures which can be accessed sequentially; 
- _Associative containers_ - sorted data structures that can be quickly searched (`O(log n)` complexity);
- _Unordered associative containers_ - hashed data structures that can be quickly searched (`O(1)` average, `O(n)` worst-case complexity);
- _Other containers_ - whatever doesn't fit in the previous categories.

All these classes are defined through templates, like in Java. If you don't know what they are, don't worry! It simply means we have to specify the type of the elements:
```cpp
vector<string> names;           // a vector of strings
vector<double> temperatures;    // a vector of doubles
```

All containers (to my knowledge) have members functions to get their size:
- `empty()` returns `true` if the container is...well...empty;
- `size()` returns the number of element (NOT THE ACTUAL SIZE!!).

### `vector`
`vector` is by far the most commonly used container, and it belongs to the _sequence_ category. It's defined by the C++ documentation as:

> **`vector`** dynamic contiguous array

This means that:
- Contiguous elements in the array are saved in contiguous memory cells (as in a C-style array);
- The array can change its size dynamically.

For example, suppose we want to add the first 5 powers of 2 in a vector:
```cpp
// option 1 -- append (like in Python)
vector<int> pow2;
for (int k=1; k<16; k*=2) {
    // push_back allocates space for a new element in the vector
    pow2.push_back(k);
}
cout << "First 5 powers of 2: " << pow2 << endl;
```
However, if you know in advance that you'll need space for 5 elements, you can `reserve` them:
```cpp
// option 2 -- reserve in advance
vector<int> pow2;
// make space for 5 elements
pow2.reserve(5);
// fill (like in C, but use for each cause it's cool)
int k = 1;
for (int& element : pow2) {
    element = k;
    k *= 2;
}
cout << "First 5 powers of 2: " << pow2 << endl;
```

### `map` and associative containers
`map` is a classic key-value pair structure, sorted by key (which means it's an _associative_ container). Keys must be unique.

Since it's a sorted container, binary search can be employed to find an element in logarithmic time. You don't have to worry about implementing it yourself though: all the associative containers provide methods to search for elements, as well as the classic `[]` access. 

The most common are:
- `count`- returns the number of elements matching specific key;
- `find` - finds element with specific key;
- `contains` - checks if the container contains element with specific key.

For example, let's say we want to save some computer stats in a map (inspired by the C++ documentation):
```cpp
// Create a map of three (string, int) pairs
map<string, int> m{{"CPU", 10}, {"GPU", 15}, {"RAM", 20}};
m["CPU"] = 25; // update an existing value
m["SSD"] = 30; // insert a new value
// search for the RAM entry => find() returns an iterator!!
auto search = m.find("RAM");
// search->first is the key
// search->second is the value
cout << "Machine " << search->first << " is " << search->second << endl;
// ## CONSOLE LOG ##
// Machine RAM is 20
```

### Other containers
C++ also implements `stack` (LIFO data structure), `queue` (FIFO data structure) and `priority_queue`. If you need them for some reason, no need to start from scratches!

## C++ algorithms
The `<algorithm>` library provides functions for different purposes, like sort or search (functions that you should implement yourself in C). 

These functions typically take iterators as input, which are "pointers to elements in a container" (in quotes because they aren't ACTUAL POINTER TYPES, but they work in the same way). Iterators have super long and annoying types, so they get along really well with `auto`.

Normally, you might want to start from the beginning of the container, and continue until the very end. In that case, you can use the `begin` and `end` functions. They return the following iterators:

![begin and end iterators](https://upload.cppreference.com/mwiki/images/1/1b/range-begin-end.svg)

```cpp
// suppose that vector `people` is sorted by construction
vector<string> people = /* init somehow */
// begin iterator
auto it = people.begin();
// end iterator
auto it_end = people.end();
// skip the first 2 elements (for some reason) => it works like a pointer!
it += 2;
// search for "jean-luc"
bool has_jean_luc = binary_search(it, it_end, "Jean-Luc");
```

These are some of interesting functions you can find in `<algorithm>`. As per usual, take a look at the [documentation](https://en.cppreference.com/w/cpp/algorithm) if you need something more specific, or if you want to see how these functions work:
- `fill` - assign a value to every element in a range;
- `unique` - remove consecutive duplicate elements in a range;
- `reverse` and `rotate` - reverse / rotate the order of elements in a range;
- `shuffle` - randomly re-order elements in a range;
- `sample` - select N random elements from a sequence;
- `sort` and `stable_sort` - sort the elements in a range (`stable_sort` preserves the order of equal elements);
- `is_sorted` - check whether the elements in a range are sorted;
- `binary_search` - determine if an element exists in a partially-ordered range;
- `lower_bound` and `upper_bound` - return an iterator to the first element _not less_ / _greater_ than the given value;
- All the possible operation on sets;
- `max_element`, `min_element` and `minmax_element` - returns the largest/smallest/both element in a range.
