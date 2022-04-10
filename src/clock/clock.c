#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include "clock.h"
#include "i2c_display.h"
#include "../utility/utility.h"

#define USER_ANSWER_DISPLAY_TIME_SEC 2

static pthread_t clockThreadID;
static displayStyle currentStyle = second;
static bool displayDigits = false;
static int digits = 0;

static void *clockThread(void *args) {
    while (true) {
        time_t now;
        struct tm *now_tm;

        now = time(NULL);
        now_tm = localtime(&now);

        if (displayDigits)
            Display_update_number(digits);
        else if (currentStyle == hour)
            Display_update_number(now_tm->tm_hour);
        else if (currentStyle == minute)
            Display_update_number(now_tm->tm_min);
        else if (currentStyle == rectangle)
            Display_show_rectangle();
        else
            Display_update_number(now_tm->tm_sec);
    }

    pthread_exit(NULL);
}

void Clock_initialize(void) {
    Display_start();
    pthread_create(&clockThreadID, NULL, &clockThread, NULL);
}

void Clock_setDisplayType(displayStyle myStyle) {
    displayDigits = false;
    currentStyle = myStyle;
}

void Clock_displayNumber(int number) {
    digits = number % 100;
    displayDigits = true;
    my_sleep_sec(USER_ANSWER_DISPLAY_TIME_SEC);
    displayDigits = false;
}
