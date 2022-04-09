#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include "../utility/utility.h"
#include "../clock/clock.h"
#include "alarm.h"
#include "wave_player.h"

#define SLEEP_TIME_SEC 1

static int scheduled_hour = 17;
static int scheduled_minute = 0;
static int scheduled_day = 0;
static bbgType myBBGType = -1;
static bool scheduled_day_of_week[DAYS_IN_A_WEEK] = {false};
static pthread_t threadID;

static void *alarmThread(void *args) {
    while (true) {
        struct tm *now_tm;
        time_t now = time(NULL);
        now_tm = localtime(&now);

        if (scheduled_day_of_week[now_tm->tm_wday] 
            && now_tm->tm_mday >= scheduled_day) {
            if (now_tm->tm_hour >= scheduled_hour 
                && now_tm->tm_min >= scheduled_minute) {
                WavePlayer_start();

                now_tm->tm_mday += 1;
                time_t tmr = mktime(now_tm);
                struct tm *tmr_tm = localtime(&tmr);
                scheduled_day = tmr_tm->tm_mday;
                printf("Alarm triggered, alarm will reactivate on day %d\n", scheduled_day);
            }
        }

        my_sleep_sec(SLEEP_TIME_SEC);
    }

    pthread_exit(NULL);
}

static void printAlarmInfo() {
    if (myBBGType = host)
        printf("Alarm %s:\n", "host");
    else
        printf("Alarm %s:\n", "guest");
    printf("Time set to day %d and %d:%d\n", scheduled_day, scheduled_hour, scheduled_minute);
    printf("Alarm activiated to (Sunday - Saturaday): ");
    for (int i = 0; i < DAYS_IN_A_WEEK - 1; i++) {
        printf("%d, ", scheduled_day_of_week[i]);
    }
    printf("%d\n", scheduled_day_of_week[DAYS_IN_A_WEEK]);
}

void Alarm_init(int myHour, int myMinute, bool scheduledDayOfWeek[DAYS_IN_A_WEEK], bbgType BBGType) {
    WavePlayer_init();
    Clock_initialize();
    Alarm_changeTime(myHour, myMinute);
    Alarm_changeDayOfTheWeek(scheduledDayOfWeek);
    struct tm *now_tm;
    time_t now = time(NULL);
    now_tm = localtime(&now);
    scheduled_day = now_tm->tm_mday;
    myBBGType = BBGType;
    //TODO: display time based on BBG type
    printAlarmInfo();

    pthread_create(&threadID, NULL, &alarmThread, NULL);
}

void Alarm_stopRinging(void) {
    WavePlayer_stop();
}

void Alarm_changeTime(int hour, int minute) {
    if (hour <= 24 && hour >= 1 
        && minute >= 0 && minute <= 60) {
        scheduled_hour = hour;
        scheduled_minute = minute;
    }
}

void Alarm_changeDayOfTheWeek(bool scheduledDayOfWeek[DAYS_IN_A_WEEK]) {
    for (int i = 0; i < DAYS_IN_A_WEEK; i++) {
        scheduled_day_of_week[i] = scheduledDayOfWeek[i];
    }
}
