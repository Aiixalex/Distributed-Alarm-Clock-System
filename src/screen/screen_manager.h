#ifndef SCREEN_H_
#define SCREEN_H_
#include <stdbool.h>
#include "OLED_text.h"
// Initilizers and Destructors
void screen_init(void);
void screen_destroy(void);
// Setters
void screen_setInverseDisplay(void);
void screen_text_setTextXY(unsigned char Row, unsigned char Column);
void screen_setContrastLevel(unsigned char ContrastLevel);
void screen_putString(const char* String);
void screen_setHorizontalScrollProperties(bool direction, unsigned char startRow, unsigned char endRow,
                            unsigned char startColumn, unsigned char endColumn, unsigned char scrollSpeed);
void screen_activateScroll(void);
void screen_deactivateScroll(void);

#endif
