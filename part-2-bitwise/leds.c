// led.c
// use this script to play with a simulated led register!

// compile as: YOU SHOULD KNOW IT BY NOW, YOU DINGUS!
#include<stdio.h>
#include<stdlib.h>

#define P 365   // binary = 101101101 ; morse = .--.
#define O 219   // binary = 11011011  ; morse = ---
#define G 109   // binary = 1101101   ; morse = --.

#define len_P 9+2
#define len_O 8+2
#define len_G 7+2

void print_line() {
    printf("+");
    for (int i=0; i<(sizeof(int)*8+2); i++)
        printf("-");
    printf("+\n");
}

void print_leds(int leds) {
    print_line();
    printf("| ");
    for (int i=0; i<(sizeof(int)*8); i++) {
        int led = (leds >> (31-i)) & 1;
        if (led == 0)       // print light shade ░
            printf("\u2591");
        else                // print dark shade ▓
            printf("\u2593");
    }
    printf(" |\n");
    print_line();
}


int main(){
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
    return EXIT_SUCCESS;
}