#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
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
static bool shouldManualTrigger = false;

static void *alarmThread(void *args) {
    while (true) {
        struct tm *now_tm;
        time_t now = time(NULL);
        now_tm = localtime(&now);

        if ((scheduled_day_of_week[now_tm->tm_wday] 
            && now_tm->tm_mday >= scheduled_day) || shouldManualTrigger) {
            if ((now_tm->tm_hour >= scheduled_hour 
                && now_tm->tm_min >= scheduled_minute) || shouldManualTrigger) {
                WavePlayer_start();

                printf("Test: %d, %d\n", myBBGType, shouldManualTrigger);
                if ((myBBGType == host) && !shouldManualTrigger) {
                    my_lock_signal_signal();
                    printf("Host: signal sent!\n");
                }

                now_tm->tm_mday += 1;
                time_t tmr = mktime(now_tm);
                struct tm *tmr_tm = localtime(&tmr);
                scheduled_day = tmr_tm->tm_mday;
                printf("Alarm triggered, alarm will reactivate on day %d\n", scheduled_day);
                shouldManualTrigger = false;
            }
        }

        my_sleep_sec(SLEEP_TIME_SEC);
    }

    pthread_exit(NULL);
}

static void printAlarmInfo() {
    if (myBBGType == host)
        printf("Alarm(%d) %s:\n", myBBGType, "host");
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

    if (BBGType == host) {
        Clock_setDisplayType(hour);
    } else if (BBGType == guest) {
        Clock_setDisplayType(minute);
    }
    
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
    printAlarmInfo();
}

void Alarm_clear() {
    for (int i = 0; i < DAYS_IN_A_WEEK; i++) {
        scheduled_day_of_week[i] = 0;
    }
    printf("-----------AlarmClear-----------\n");
    printAlarmInfo();
    printf("----------------------\n");
}

void Alarm_changeOneDayOfTheWeek(int day) {
    scheduled_day_of_week[day] = 1;
    printf("-----ChangeOneDay-----\n");
    printAlarmInfo();
    printf("----------------------\n");
}

// TODO: need to double check?
char *Alarm_getScheduledTime(void) {
    // char scheduledTime[MAX_STRING_LENGTH] = "";
    char *scheduledTime = malloc(MAX_STRING_LENGTH);

    int temp = 0;
    for (int i = 0; i < DAYS_IN_A_WEEK; i++) {
        if (scheduled_day_of_week[i]) {
            temp = i;
            break;
        }
    }
    snprintf(scheduledTime, MAX_STRING_LENGTH - 1, "1:%d:%d:%d", scheduled_hour,
             scheduled_minute, temp);
    // snprintf(scheduledTime, MAX_STRING_LENGTH - 1, "%d:%d:%d:%d:%d:%d:%d:%d:%d",
    //          scheduled_hour, scheduled_minute, scheduled_day_of_week[0],
    //          scheduled_day_of_week[1], scheduled_day_of_week[2],
    //          scheduled_day_of_week[3], scheduled_day_of_week[4],
    //          scheduled_day_of_week[5], scheduled_day_of_week[6]);

    return scheduledTime;
}

void Alarm_trigger_alarm_manually(void) {
    shouldManualTrigger = true;
}

void Alarm_stop_trigger_manually(void) {
    shouldManualTrigger = false;
}
