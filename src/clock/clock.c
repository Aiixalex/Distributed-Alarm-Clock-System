#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include "clock.h"
#include "i2c_display.h"

static pthread_t clockThreadID;
static displayStyle currentStyle = second;

static void *clockThread(void *args) {
    while (true) {
        time_t now;
        struct tm *now_tm;

        now = time(NULL);
        now_tm = localtime(&now);

        if (currentStyle == hour)
            Display_update_number(now_tm->tm_hour);
        else if (currentStyle == minute)
            Display_update_number(now_tm->tm_min);
        else
            Display_update_number(now_tm->tm_sec);
    }

    pthread_exit(NULL);
}

void Clock_initialize(void) {
    Display_start();
    pthread_create(&clockThreadID, NULL, &clockThread, NULL);
}

void Clock_displayTime(displayStyle myStyle) {
    currentStyle = myStyle;
}
