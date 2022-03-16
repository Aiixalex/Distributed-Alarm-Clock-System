#include <stdlib.h>
#include <stdio.h>

#include "puzzle/puzzle.h"

int main() {
    printf("This is alarm_guest\n");

    char *myPuzzle = Puzzle_generate();
    printf("Puzzle %s\n", myPuzzle);

    printf("Puzzle %d\n", Puzzle_validate(myPuzzle, 28));

    free(myPuzzle);

    return 0;
}
