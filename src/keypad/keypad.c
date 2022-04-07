#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utility.h"

#define GPIO_BASE_DIR "/sys/class/gpio"
#define NUM_KEYPAD_GPIO 12
#define MAX_BUFFER_SIZE 256
#define SLEEP_TIME_MS 100
#define MAX_READ_LENGTH 5

static const char *KEYPAD_GPIO[NUM_KEYPAD_GPIO] = {
    "88", "89", "86", "87", "10", "9", "8", "78", "76", "74", "72", "70"
};
static const int KEYPAD_KEYS[NUM_KEYPAD_GPIO] = {
    -1, 7, 4, 1, 0, 8, 5, 2, -1, 9, 6, 3
}

static bool keypad_isResetted[NUM_KEYPAD_GPIO] = { false };
static char userInput[MAX_READ_LENGTH + 1] = "";
static int userInputIndex = 0;

typedef enum keypad_element {
    star, seven, four, one, zero, eight, five, two, hash, nine, six, three
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

int Keypad_readUserInput(void) {
    char userInput[MAX_READ_LENGTH + 1] = "";
    int userInputIndex = 0;

    while (true) {
        for (int i = 0; i < NUM_KEYPAD_GPIO; i++) {
            char tempString[MAX_BUFFER_SIZE];
            snprintf(tempString, 2, "/sys/class/gpio/gpio%s/value", KEYPAD_GPIO[i]);

            int isPressed = file_read_int(tempString);
            if (KEYPAD_KEYS[i] == -1) {
                userInput[MAX_READ_LENGTH] = '\0';
                return atoi(userInput);
            }
            else {
                userInput[userInputIndex % MAX_READ_LENGTH] = KEYPAD_KEYS[i];
                userInputIndex++;
            }            
        }
    }
}
