// keypad.h
// Module to read inputs from keypad
#ifndef _KEYPAD_H
#define _KEYPAD_H

#define MAX_READ_LENGTH 5

// Initilize keypad
void Keypad_init(void);

// read userinput from keypad
// Note: will read max 5numbers, press # will make this function return
int Keypad_readUserInput(void);

#endif
