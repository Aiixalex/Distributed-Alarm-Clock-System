#include <stdlib.h>
#include <stdio.h>

#include "puzzle/puzzle.h"
#include "keypad/keypad.h"
#include "clock/clock.h"

int main() {
    printf("This is alarm_guest\n");

    // Puzzle_printPuzzle(Puzzle_generate(easy));
    // Puzzle_printPuzzle(Puzzle_generate(medium));
    // Puzzle_printPuzzle(Puzzle_generate(hard));
    // Puzzle_printPuzzle(Puzzle_generate(genius));
    // Keypad_init();
    Clock_initialize();
    Clock_displayTime(second);

    while (1)
    {
        /* code */
    }
    

    return 0;
}
