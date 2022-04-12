#include <stdbool.h>
// An alarm module which will ring the alarm on scheduled times
// User can also update the settings
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

// manually trigger the alarm (for demo purpose)
void Alarm_trigger_alarm_manually(void);

// stop manual trigger
void Alarm_stop_trigger_manually(void);

// Change day of the week that the alarm will trigger
// receive one int, different than changeDayOfTheWeek
// which receive an array of ints
void Alarm_changeOneDayOfTheWeek(int day);

// clear the alarm, change everything to default settings
void Alarm_clear(void);

#endif
