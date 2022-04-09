#include <stdbool.h>
// An alarm module which will ring the alarm on scheduled times
#ifndef _ALARM_H_
#define _ALARM_H_

#define DAYS_IN_A_WEEK 7

// initialize the alarm
// scheduledDayOfWeek is an array of 7 bools from Sunday to Saturday
void Alarm_init(int myHour, int myMinute, bool scheduledDayOfWeek[DAYS_IN_A_WEEK]);

// stop the ringing
void Alarm_stopRinging(void);

// change the hour and minute of the alarm
void Alarm_changeTime(int hour, int minute);

// change day of the week
void Alarm_changeDayOfTheWeek(bool scheduledDayOfWeek[DAYS_IN_A_WEEK]);

#endif
