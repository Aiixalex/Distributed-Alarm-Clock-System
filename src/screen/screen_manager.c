#include <stdio.h>
#include <stdlib.h>

#include "screen_manager.h" 
#include "OLED_text.h"

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
// Init
void screen_init(void)
{
    OLED_text_init(SSD1327);
    OLED_text_clearDisplay();
    OLED_text_setVerticalMode();
}
//Destroy
void screen_destroy(void)
{
    OLED_text_clearDisplay();
    OLED_text_destroy();
}


