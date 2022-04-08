// display time on the i2c display
#ifndef _CLOCK_H_
#define _CLOCK_H_

typedef enum displayStyle {
    hour, minute, rectangle, second
} displayStyle;

// initialized the clock
void Clock_initialize(void);

// display time(Hours) on the display
void Clock_displayTime(displayStyle myStyle);

// display time(Minutes) on the display
void Clock_displayTimeMinutes(void);

// display a rectangle on the clock
void Clock_displayRectangle(void);

#endif
