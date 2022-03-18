#include <stdlib.h>
#include <stdio.h>

#include "puzzle/puzzle.h"

int main() {
    printf("This is alarm_guest\n");

    Puzzle_printPuzzle(Puzzle_generate(easy));
    Puzzle_printPuzzle(Puzzle_generate(medium));
    Puzzle_printPuzzle(Puzzle_generate(hard));
    Puzzle_printPuzzle(Puzzle_generate(genius));

    return 0;
}
