// joystick.h
// Module to handle joystick input
#ifndef _JOYSTICK_H
#define _JOYSTICK_H

enum JoystickDirection
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    NO_DIRECTION
};

// Initilize joystick
void js_init();

// Get the direction of zen-cape joystick from GPIO
enum JoystickDirection getJoystickDir();

#endif
