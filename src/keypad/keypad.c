#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"

#define GPIO_BASE_DIR "/sys/class/gpio"
#define NUM_KEYPAD_GPIO 12
#define MAX_BUFFER_SIZE 256
#define SLEEP_TIME_MS 100

static const char *KEYPAD_GPIO[NUM_KEYPAD_GPIO] = {
    "88", "89", "86", "87", "10", "9", "8", "78", "76", "74", "72", "70"
};

typedef enum keypad_keys {
    star, seven, four, one, zero, eight, give, two, hash, nine, six, three
} keypad_element;



void Keypad_init(void) {
    for (int i = 0; i < NUM_KEYPAD_GPIO; i++) {
        export_to_gpio(KEYPAD_GPIO[i]);

        // set direction
        char tempString[MAX_BUFFER_SIZE];
        snprintf(tempString, 2, "/sys/class/gpio/gpio%s/direction", KEYPAD_GPIO[i]);
        file_write(tempString, "in");
    }

    my_sleep_ms(SLEEP_TIME_MS);
}
