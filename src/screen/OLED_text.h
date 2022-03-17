#ifndef OLED_TEXT_H_
#define OLED_TEXT_H_
#include <stdbool.h>
/*
    SSD1327 Gray OLED Driver Library

    Copyright (c) 2011 seeed technology inc.
    Author        :   Visweswara R
    Create Time   :   Dec 2011
    Change Log    :

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
// Modified by Piercson to work with BeagleBone Green and C language

// SeeedGrayOLED Instruction set addresses

#define SH1107G  1
#define SSD1327  2

#define VERTICAL_MODE                       01
#define HORIZONTAL_MODE                     02

#define SeeedGrayOLED_Address               0x3c
#define SeeedGrayOLED_Command_Mode          0x80
#define SeeedGrayOLED_Data_Mode             0x40

#define SeeedGrayOLED_Display_Off_Cmd       0xAE
#define SeeedGrayOLED_Display_On_Cmd        0xAF

#define SeeedGrayOLED_Normal_Display_Cmd    0xA4
#define SeeedGrayOLED_Normal_Display_Cmd_SH1107G    0xA6
#define SeeedGrayOLED_Inverse_Display_Cmd   0xA7
#define SeeedGrayOLED_Activate_Scroll_Cmd   0x2F
#define SeeedGrayOLED_Dectivate_Scroll_Cmd  0x2E
#define SeeedGrayOLED_Set_ContrastLevel_Cmd 0x81

#define Scroll_Left             0x00
#define Scroll_Right            0x01

#define Scroll_2Frames          0x7
#define Scroll_3Frames          0x4
#define Scroll_4Frames          0x5
#define Scroll_5Frames          0x0
#define Scroll_25Frames         0x6
#define Scroll_64Frames         0x1
#define Scroll_128Frames        0x2
#define Scroll_256Frames        0x3

char addressingMode;

void OLED_text_init(int IC);
void OLED_text_destroy(void);

void OLED_text_setNormalDisplay(void);
void OLED_text_setInverseDisplay(void);

void OLED_text_setGrayLevel(unsigned char grayLevel);

void OLED_text_setVerticalMode(void);
void OLED_text_setHorizontalMode(void);

void OLED_text_setTextXY(unsigned char Row, unsigned char Column);
void OLED_text_clearDisplay(void);
void OLED_text_setContrastLevel(unsigned char ContrastLevel);
void OLED_text_putChar(unsigned char c);
void OLED_text_putString(const char* String);

void OLED_text_setHorizontalScrollProperties(bool direction, unsigned char startRow, unsigned char endRow,
                                    unsigned char startColumn, unsigned char endColumn, unsigned char scrollSpeed);
void OLED_text_activateScroll(void);
void OLED_text_deactivateScroll(void);
#endif
