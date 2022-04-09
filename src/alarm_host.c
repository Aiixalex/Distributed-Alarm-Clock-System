#include <stdlib.h>
#include <stdio.h>
#include "puzzle/puzzle.h"
#include "clock/clock.h"
#include "alarm/alarm.h"
#include "utility/utility.h"

static diffculity puzzleDiffculity = easy;

int main() {
    printf("This is alarm_host\n");
    initialize_my_lock_signal_wait();
    
    int hour = 6, minute = 27;
    bool activeDayOfWeek[7] = {0, 0, 0, 0, 1, 1, 1};
    Alarm_init(hour + 12, minute, activeDayOfWeek, guest);

    while (1) {
        my_lock_signal_wait();

        // received a message which contains the problem struct from BBG 1
        Clock_setDisplayType(rectangle);

        // generate puzzle and send message to BBG1
        Puzzle_generate(puzzleDiffculity);
        // TODO: display on the screen

        // wait for response from BBG2
        // BBG2 should only send message if the answer is correct
        // Or, we can receive answer from BBG2 and display on the clock
        // TODO: need to finish

        // if correct, disable alarm for today
        // sned message to BBG 1 to disbale their alarm
        Alarm_stopRinging();
    }

    return 0;
}
