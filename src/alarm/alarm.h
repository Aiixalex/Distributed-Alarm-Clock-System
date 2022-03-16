// alarm.h
// Module to interact with the alarm on the target
#ifndef _ALARM_H
#define _ALARM_H

// initialie the alarm
void Alarm_init(void);

// make alarm to play sound now
void Alarm_play(void);

// stop the alarm
void Alarm_stop(void);

// destory the alarm and free all resource it is using
void Alarm_destory(void);

// schedule the alarm in the future
void Alarm_schedule(char* time);

// set what content/music the alarm will play
void Alarm_set_content(char* file_path);

#endif
