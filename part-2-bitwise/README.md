# Part 2
Today we are going to talk about bitwise and binary operations, and why they are important for embedded systems! 

For each operation, we will see its behavior, and how it is often used in this use case.

## Bitwise NOT
Bitwise NOT (`~`) is just, well, a NOT. Nothing to see here!
```c
int v = 69;
printf("    v = %#010x\n",v);
printf("not_v = %#x\n", ~v);
//     v = 0x00000045
// not_v = 0xffffffba
```

## XOR
XOR (`^`) is a useful operation in cryptography and assembly. I don't know about embedding systems. Anyway, if you don't remember, XOR computes _diversity_, meaning that if 2 bits are different, XOR will be 1, 0 otherwise. 

I know, not the clearest explanation. But maybe the logic table will refresh your memory:

|X | Y | X^Y |
|:-|:--|:---:|
|0|0|0|
|0|1|1|
|1|0|1|
|1|1|0|

So, XOR is used in cryptography for its funky property:
```
if Z = X^Y then
    X^Z == Y
    Y^Z == X
```
And it's used in assembly to set registers to 0:
```s
r0 = r0 XOR r0
```
This is better than just loading the value 0, as the XOR function is faster and typically smaller in size.

## Bitwise OR and AND
I grouped up bitwise OR (`|`) and AND (`&`) since they have the same, opposite usage in embedding systems. 
### Bitwise OR
I'm sure you remember that the OR of two bits is 0 if and only if both bits are 0:

|X | Y | X\|Y |
|:-|:--|:---:|
|0|0|0|
|0|1|1|
|1|0|1|
|1|1|1|

For this reason, it can be interpreted as a sort of sum. 

From the definition of OR, the following holds: `X|1 == 1`, whatever the value of `X`. This is why bitwise OR is used to _set_ specific bits. In embedding systems, as well as in system calls (more on syscalls later), options are configured by setting (or resetting) positional bits. I know this sounds like incomprehensible mumbo-jumbo, so I have an example for you!

Let's look at the `open()` system call, which is used to open files. 
```c
#include <fcntl.h>
int open(const char *pathname, int flags);
```
According to the [C documentation](https://man7.org/linux/man-pages/man2/open.2.html), we have to choose some flags to configure what the syscall will do. For example, if we want to open a file for writing, we can select:
- `O_WRONLY`, to open the file for writing;
- `O_CREAT`, to create the file if it doesn't exist;
- `O_TRUNC`, to empty the content of the file if it exists.

But what are these flags, really?
```c
printf("O_WRONLY = %d\n", O_WRONLY);
printf("O_CREAT  = %d\n", O_CREAT);
printf("O_TRUNC  = %d\n", O_TRUNC);
// O_WRONLY = 1
// O_CREAT  = 64
// O_TRUNC  = 512
```
Maybe printing them as integer didn't give it away, but converting them to binary surely will!
```
O_WRONLY = 0000000001
O_CREAT  = 0001000000
O_TRUNC  = 1000000000
```
Basically, the `flags` parameter has each bit associated with an action. From the value of the constants we printed, we can deduce that:
- The bit in position 0 controls the "write only" option;
- The bit in position 6 controls the "create" option;
- The bit in position 9 controls the "truncate" option.

So, if you want to open a file in writing only, create it if it doesn't exist, and clear it if it exists, you should set `flags = 1001000001`. Or, in other words:
```c
int fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC);
```
Bitwise OR makes sure that each positional bit is set, so that you end up with the command you want!

Sure, you could also write:
```c
int fd = open("file.txt", 577);
```
But let's be honest, who the heck understands it?

Similarly, when dealing with embedding systems, you might have to set some positional bits to control the behavior of components of the system. For example, `leds` might be a variable controlling 32 tiny LEDs on a board. Let's say you are a nerd and want to print POG in morse code. Then, your pattern is going to be:
```c
#define P 365   // binary = 101101101 ; morse = .--.
#define O 219   // binary = 11011011  ; morse = ---
#define G 109   // binary = 1101101   ; morse = --.

#define len_P 9+2
#define len_O 8+2
#define len_G 7+2

int leds = 0;
// set P and shift
leds |= P;
leds <<= len_P;
// set O and shift
leds |= O;
leds <<= len_O;
// set G (word is done, no need to shift)
leds |= G;

print_leds(leds);
// +----------------------------------+
// | ░░▓░▓▓░▓▓░▓░░░▓▓░▓▓░▓▓░░░▓▓░▓▓░▓ |
// +----------------------------------+
```
>Try this yourself in [leds.c](./leds.c)! You can also add more [morse letters](https://en.wikipedia.org/wiki/Morse_code) and print whatever you like!

### Bitwise AND
I see bitwise AND as the dual of OR, since the AND of two bits is 1 only if both the bits are 1:
|X | Y | X&Y |
|:-|:--|:---:|
|0|0|0|
|0|1|0|
|1|0|0|
|1|1|1|

It can be seen as a sort of multiplication. Moreover, since `X&0 == 0` whatever the value of `X`, it can be used to _reset_ a positional bit (this is why I called it the _dual of OR_).

Bitwise AND can also be used to _mask_ bits. This means that, if you want to focus on only a portion of a variable, you simply zero the rest. For example, if you want to check whether an _unsigned_ integer fits in 1 byte, you could do:
```c
int fits_in_1B(int x) {
    int x_1B = x & 0xff;
    return x_1B == x;
}
printf("    42 fits in 1B? [y] : %c\n", fits_in_1B(42)?'y':'n');
printf("481516 fits in 1B? [n] : %c\n", fits_in_1B(481516)?'y':'n');
//     42 fits in 1B? [y] : y
// 481516 fits in 1B? [n] : n
```
In this example, `x & 0xff` zeros all the bytes in `x`, except for the least significant one. If the value didn't change, then it can fit perfectly in a single byte.

Interestingly enough, this code breaks when you try it with a negative number.
```c
printf("   -42 fits in 1B? [y] : %c\n", fits_in_1B(42)?'y':'n');
//    -42 fits in 1B? [y] : n
```
> Do you know why? And can you fix it so that it always works, no matter the sign? You can look at the code in [one_byte.c](./one_byte.c).

## Shifts and bitwise math
### Left shift
Left shift (`<<`), well, shifts all the bits towards left, and add 0s in the now empty positions.
```c
int v = 69;
printf("     v = %#05x\n", v);
printf("v << 4 = %#x\n", v<<4);
//      v = 0x045
// v << 4 = 0x450
```
Since we are in binary, every time we add a 0 to the right we multiply by 2, meaning that:
```math
x << n = x \cdot 2^n
```
```c
int x = 32;
printf("     x = %d\n", x);
printf("x << 1 = %d\n", x<<1);
//      x = 32
// x << 1 = 64
```
### Right shift
Right shift (`>>`), you guessed it, shifts all the bits towards right.
```c
int v = 69;
printf("     v = %#x\n", v);
printf("v >> 4 = %#04x\n", v>>4);
//      v = 0x45
// v >> 4 = 0x04
```
In a dual way wrt the left shift, this represents a division by some power of 2:
```math
x >> n = x / 2^n
```
```c
int x = 32;
printf("     x = %d\n", x);
printf("x >> 1 = %d\n", x>>1);
//      x = 32
// x >> 1 = 16
```

In C, the right shift is interpreted as a _mathematical operation_, meaning that it needs to keep the sign consistent. 

For this reason, if you are shifting a _signed_ variable, the shift will add 0s if the msb is 0 (so the sign is +), 1s otherwise. This is called an _arithmetical right shift_. A shift that blindly adds 0s is known as a _logical right shift_.
```c
int v = -69;
printf("     v = %#x\n", v);
printf("v >> 4 = %#x\n", v>>4);
//      v = 0xffffffbb
// v >> 4 = 0xfffffffb
```
Since shifts are much faster than actual multiplications and divisions, you should always use them whenever you are dealing with powers of 2. They are not super readable though, so add many comments!

### Bitwise AND. Again. Dude, what are you even doing here!
Since I talked a bit about bitwise math, it's time to bring back the good old AND, since it can also be used to perform MOD operations!
```math
x \& (n-1) = x \% 2^n
```

This can look pretty obscure, but bear with me. In base 10, dividing by a power of 10 means simply shifting right. For example:
```
168 / 10 = 16
```
But where did the 8 go? Well, that's easy:
```
168 % 10 = 8
```
Of course, the same holds for base 2 (actually, every base):
```
10011 / 100 = 100
10011 % 100 = 11
```
Ok, the MOD result is the bits I removed when dividing, right? 
This means I don't need to perform the division at all, I can just mask them!
```
10011 % 100 = 10011 & 11 = 11
```
Now, since I'm dealing with powers of 2, the divisor will always look like this: `1<one or more 0s>`, meaning we can get the all 1s mask by simply removing 1 from it. 

And there you have it, I hope I convinced you!

> Your turn! Try to write a program that converts from binary to integer representation (and vice versa) using _only_ bitwise operations! [see [bin_converter.c](./bin_converter.c)]

## And that's all!
See you next time <3