#include <stdlib.h>
#include <stdio.h>
#include "puzzle/puzzle.h"
#include "keypad/keypad.h"
#include "clock/clock.h"
#include "alarm/alarm.h"

int main() {
    printf("This is alarm_guest\n");
    
    Keypad_init();
    initialize_my_lock_signal_wait();
    int hour = 6, minute = 27;
    bool activeDayOfWeek[7] = {0, 0, 0, 0, 1, 1, 1};
    Alarm_init(hour + 12, minute, activeDayOfWeek, guest);

    while (1) {
        //TODO: the UDP module will call "my_lock_signal_signal()" to wake up main
        my_lock_signal_wait();
        // received a message which contains the problem struct from BBG 1
        Clock_setDisplayType(rectangle);

        // read input from user
        int number = Keypad_readUserInput();
        printf("User input: %d\n", number);
        Clock_displayNumber(number);

        // if correct, disable alarm for today
        // sned message to BBG 1 to disbale their alarm
        if (number) {
            Alarm_stopRinging();
        }
    }

    return 0;
}
