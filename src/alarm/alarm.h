// alarm.h
// Module to interact with the alarm on the target
#ifndef _ALARM_H
#define _ALARM_H

// initialie the alarm
void alarm_init(void);

// make alarm to play sound now
void alarm_play(void);

// stop the alarm
void alarm_stop(void);

// destory the alarm and free all resource it is using
void alarm_destory(void);

// schedule the alarm in the future
void alarm_schedule(char* time);

// set what content/music the alarm will play
void alarm_set_content(char* file_path);

#endif
