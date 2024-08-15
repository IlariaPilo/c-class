// compile with:
// gcc -g -o treasure treasure.c

#include <stdio.h>
#include <stdlib.h>

void find_treasure(int *map) {
    printf("I found the treasure! It's %d\n\n", *map);
}

void steal_treasure(int treasure) {
    treasure = 0;
    printf("The treasure is mine, muhahaha! %d\n", treasure);
}
void steal_treasure_fr(int *map) {
    *map = 0;
    printf("The treasure is mine fr, muhahaha! %d\n", *map);
}

int main(int argc, char*argv[]) {

    int treasure = 42;

    int *map = &treasure;
    find_treasure(map);

    steal_treasure(treasure);
    printf("treasure = %d\nYou did nothing, you loser! lol\n", treasure);
    steal_treasure_fr(&treasure);
    printf("treasure = %d\nOps, you took it :(\n", treasure);

    return EXIT_SUCCESS;
}