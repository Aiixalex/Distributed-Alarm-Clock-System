#include <stdio.h>
#include <stdlib.h>
#include "screen.h"

int main(void)
{
    printf("Hello World\n");
    init(SSD1327);
    clearDisplay();
    setVerticalMode();
     for (int i = 0; i < 12; i++) {
        setTextXY(i,0);
        setGrayLevel(i);
        putString("hello Henry");
    }

    return 0;
}