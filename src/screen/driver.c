#include <stdio.h>
#include <stdlib.h>
// #include "OLED_text.h"
#include "screen_manager.h"

#define CHAR_HEIGHT 8
#define SCREEN_ROW_START 8
#define SCREEN_ROW_END 96
int main(void)
{
    // printf("Hello World\n");
    // OLED_text_init(SSD1327);
    // OLED_text_clearDisplay();
    // OLED_text_setVerticalMode();
    // OLED_text_setTextXY(0,0);
    // OLED_text_putString("A = 6");

    // OLED_text_setTextXY(1,0);
    // OLED_text_putString("B = 12");

    // OLED_text_setTextXY(2,0);
    // OLED_text_putString("C = 2");

    // OLED_text_setTextXY(3,0);
    // OLED_text_putString("D = 6");

    // OLED_text_setTextXY(5,0);
    // OLED_text_putString("E = D / C");

    // OLED_text_setTextXY(6,0);
    // OLED_text_putString("F = A + B");

    // OLED_text_setTextXY(7,0);
    // OLED_text_putString("G = F / E");

    // OLED_text_setTextXY(9,0);
    // OLED_text_putString("Solve for G");

    // OLED_text_setHorizontalScrollProperties(Scroll_Left, SCREEN_ROW_START + (CHAR_HEIGHT * 6), SCREEN_ROW_START + (CHAR_HEIGHT * 7), 0, 47,
    //         Scroll_2Frames);
    // OLED_text_activateScroll();
    // OLED_text_deactivateScroll();
    // OLED_text_clearDisplay();
    // OLED_text_destroy();
    puzzle p = {
        .diffculityLevel = 4,
        .problem = "(6 * 7) * (7 - 5) - (1 / 4)",
        .varOne = "A = 6",
        .varTwo = "B = 7",
        .varThree = "C = 7",
        .varFour = "D = 5",
        .answer = 84,
    };
    Screen_init();
    Screen_set_problem(p);

    return 0;
}
//Start 8
