#include <stdbool.h>
// An alarm module which will ring the alarm on scheduled times
#ifndef _ALARM_H_
#define _ALARM_H_

#define DAYS_IN_A_WEEK 7
#define MAX_STRING_LENGTH 150

typedef enum bbgType {
    host, guest
} bbgType;

// initialize the alarm
// scheduledDayOfWeek is an array of 7 bools from Sunday to Saturday
void Alarm_init(int myHour, int myMinute, bool scheduledDayOfWeek[DAYS_IN_A_WEEK], bbgType myBBGType);

// stop the ringing
void Alarm_stopRinging(void);

// change the hour and minute of the alarm
void Alarm_changeTime(int hour, int minute);

// change day of the week
void Alarm_changeDayOfTheWeek(bool scheduledDayOfWeek[DAYS_IN_A_WEEK]);

// get the scheduled alarm in a string format HH:MM
// ! User need to free the ptr returned
char *Alarm_getScheduledTime(void);

// manually trigger the alarm
void Alarm_trigger_alarm_manually(void);

void Alarm_stop_trigger_manually(void);

void Alarm_changeOneDayOfTheWeek(int day);

void Alarm_clear(void);

#endif
