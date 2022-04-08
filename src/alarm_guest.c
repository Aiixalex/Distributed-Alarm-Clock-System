#include <stdlib.h>
#include <stdio.h>

#include "puzzle/puzzle.h"
#include "keypad/keypad.h"

int main() {
    printf("This is alarm_guest\n");

    // Puzzle_printPuzzle(Puzzle_generate(easy));
    // Puzzle_printPuzzle(Puzzle_generate(medium));
    // Puzzle_printPuzzle(Puzzle_generate(hard));
    // Puzzle_printPuzzle(Puzzle_generate(genius));
    Keypad_init();



    while(1) {
        printf("Keypad: %d\n", Keypad_readUserInput());
    }

    return 0;
}
