// display time on the i2c display
#ifndef _CLOCK_H_
#define _CLOCK_H_

typedef enum displayStyle {
    hour, minute, rectangle, second
} displayStyle;

// initialized the clock
void Clock_initialize(void);

// display time(Hours) on the display
void Clock_setDisplayType(displayStyle myStyle);

// display a number on the clock
void Clock_displayNumber(int number);

#endif
