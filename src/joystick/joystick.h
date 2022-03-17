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
    IN,
    NO_DIRECTION
};

// Create/Join the joystick thread.
void Joystick_init();
void Joystick_quit();

#endif
