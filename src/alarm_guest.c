#include <stdlib.h>
#include <stdio.h>

#include "puzzle/puzzle.h"
#include "keypad/keypad.h"
#include "clock/clock.h"
#include "alarm/alarm.h"

int main() {
    printf("This is alarm_guest\n");

    // Puzzle_printPuzzle(Puzzle_generate(easy));
    // Puzzle_printPuzzle(Puzzle_generate(medium));
    // Puzzle_printPuzzle(Puzzle_generate(hard));
    // Puzzle_printPuzzle(Puzzle_generate(genius));
    // Keypad_init();

    // Clock_initialize();
    // Clock_displayTime(second);
    // WavePlayer_init();
    // WavePlayer_start();
    
    int hour = 6, minute = 27; // pm test
    bool activeDayOfWeek[7] = {0, 0, 0, 0, 1, 1, 1};
    Alarm_init(hour + 12, minute, activeDayOfWeek);

    while (1) {
        // printf("Input: %d\n", Keypad_readUserInput());
    }
    

    return 0;
}
