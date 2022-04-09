#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utility/utility.h"
#include "keypad.h"

#define GPIO_BASE_DIR "/sys/class/gpio"
#define NUM_KEYPAD_GPIO 12
#define MAX_BUFFER_SIZE 256
#define SLEEP_TIME_MS 300

static const char *KEYPAD_GPIO[NUM_KEYPAD_GPIO] = {
    "88", "89", "86", "87", "66", "9", 
    "8", "78", "76", "67", "69", "68"
};

typedef enum keypad_element {
    star, seven, four, one, zero, eight, 
    five, two, hash, nine, six, three
} keypad_element;

static const int KEYPAD_KEYS[NUM_KEYPAD_GPIO] = {
    star, 7, 4, 1, 0, 8, 5, 2, hash, 9, 6, 3
};

void Keypad_init(void) {
    for (int i = 0; i < NUM_KEYPAD_GPIO; i++) {
        export_to_gpio(KEYPAD_GPIO[i]);
        my_sleep_ms(SLEEP_TIME_MS);
    }

    my_sleep_ms(SLEEP_TIME_MS);

    for (int i = 0; i < NUM_KEYPAD_GPIO; i++) {        
        // set direction
        char tempString[MAX_BUFFER_SIZE];
        snprintf(tempString, MAX_BUFFER_SIZE - 1, "/sys/class/gpio/gpio%s/direction", KEYPAD_GPIO[i]);
        file_write(tempString, "in");
    }

    printf("Keypad initialized\n");
}

int Keypad_readUserInput(void) {
    int keypad_isResetted[NUM_KEYPAD_GPIO] = {0};
    char userInput[MAX_READ_LENGTH + 1] = "";
    int userInputIndex = 0;

    while (true) {
        for (int i = 0; i < NUM_KEYPAD_GPIO; i++) {
            char tempString[MAX_BUFFER_SIZE];
            snprintf(tempString, MAX_BUFFER_SIZE - 1, "/sys/class/gpio/gpio%s/value", KEYPAD_GPIO[i]);

            int isPressed = file_read_int(tempString);

            if (isPressed && (keypad_isResetted[i] == 0)) {
                keypad_isResetted[i]++;
            } else if (!isPressed && (keypad_isResetted[i] != 0)) {
                if (i == hash) {
                    if (userInputIndex == 0)
                        return -1;

                    userInput[MAX_READ_LENGTH] = '\0';
                    return atoi(userInput);
                }

                userInput[userInputIndex] = '0' + KEYPAD_KEYS[i];
                userInputIndex++;
                keypad_isResetted[i] = 0;
            }
        }
    }
}
