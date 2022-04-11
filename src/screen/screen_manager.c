#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "screen_manager.h" 
#include "OLED_text.h"

#define MAX_CHAR_PER_LINE 12

#define VARIABLE_START_LINE 0
#define PROBLEM_START_LINE  HOW_TO_START_LINE+1

#define HOW_TO_START_LINE 4
#define END_LINE  11
#define DEBUG 1

#define CHAR_HEIGHT 8
#define SCREEN_ROW_START 8
#define SCREEN_ROW_END 96

static void set_problem(char* problem, int amt);
static void set_variables(puzzle problem);
static void sleep_ms(unsigned int delayMs)
{
	const unsigned int NS_PER_MS = 1000 * 1000;
	const unsigned int NS_PER_SECOND = 1000000000;

	unsigned long long delayNs = delayMs * NS_PER_MS;
	int seconds = delayNs / NS_PER_SECOND;
	int nanoseconds = delayNs % NS_PER_SECOND;
	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}
// Init
void Screen_init(void)
{
    if(DEBUG)
        printf("Screen init\n");
    OLED_text_init(SSD1327);
    OLED_text_clearDisplay();
    sleep_ms(1000);
    OLED_text_setVerticalMode();
}
//Destroy
void Screen_destroy(void)
{
    OLED_text_clearDisplay();
    sleep_ms(1000);
    OLED_text_destroy();
}
void Screen_clear(void){
    OLED_text_deactivateScroll();
    OLED_text_clearDisplay();
}
void Screen_set_problem(puzzle data){
    OLED_text_clearDisplay();
    OLED_text_deactivateScroll();

    OLED_text_setTextXY(END_LINE-1,0);
    OLED_text_putString("Use Integer");
    OLED_text_setTextXY(END_LINE,0);
    OLED_text_putString(" Division :)");
    switch (data.diffculityLevel){
    case 1: 
        if(DEBUG)
            printf("Difficulty 1\n");
        set_problem(data.problem, 1);
        break;
    case 2:
        if(DEBUG)
            printf("Difficulty 2\n");
        set_problem(data.problem, 2);
        break;
    case 3:
        if(DEBUG)
            printf("Difficulty 3\n");
        set_problem(data.problem, 3);
        break;
    case 4:
        if(DEBUG)
            printf("Difficulty 4\n");
        set_variables(data);
        set_problem(data.problem, 2);
        OLED_text_setHorizontalScrollProperties(Scroll_Left, 40,
                    47, 0, 47,
                    Scroll_2Frames);
        OLED_text_activateScroll();
        sleep_ms(1000);
        OLED_text_deactivateScroll();
        OLED_text_setHorizontalScrollProperties(Scroll_Left, 40,
            54, 0, 47,
            Scroll_2Frames);
        OLED_text_activateScroll();
        break;
    
    default:
        break;
    }
}
static void set_problem(char* problem, int amt){
    OLED_text_setTextXY(HOW_TO_START_LINE,0);
    OLED_text_putString("Solve->Peace");
    int problem_i = 0;
    for (int i = 0; i < amt; i++){
        OLED_text_setTextXY(PROBLEM_START_LINE+i,0);
        char line[MAX_CHAR_PER_LINE + 1];
        int line_i = 0;
        if(i == 0){
            line[0] = ' ';
            line[1] = ' ';
            line_i = 2;
        }
        char c;
        do{
            c = problem[problem_i];
            line[line_i] = c;
            line_i++;
            problem_i++;
            if(c == ')'){
                problem_i++;
                line[line_i] = 0;
                break;
            }
        }while(c != 0);
        OLED_text_putString(line);
    }
    
}
static void set_variables(puzzle problem){
    OLED_text_setTextXY(VARIABLE_START_LINE,0);
    char line1[MAX_CHAR_PER_LINE+1] = "";
    strncat(line1,problem.varOne, 5);
    strncat(line1, "   ",1);
    strncat(line1, problem.varTwo, 5);
    OLED_text_putString(line1);

    OLED_text_setTextXY(VARIABLE_START_LINE+1,0);
    char line2[MAX_CHAR_PER_LINE+1] = "";
    strncat(line2,problem.varThree, 5);
    strncat(line2, "   ",1);
    strncat(line2, problem.varFour, 5);
    OLED_text_putString(line2);
}


