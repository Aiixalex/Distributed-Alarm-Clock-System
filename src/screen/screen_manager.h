#ifndef SCREEN_H_
#define SCREEN_H_
#include <stdbool.h>
#include "OLED_text.h"

#define MAX_STRING_LENGTH 50

typedef enum diffculity {
    easy = 1, medium = 2, hard = 3, genius
} diffculity;

typedef struct puzzle {
    diffculity diffculityLevel;
    char problem[MAX_STRING_LENGTH];
    char varOne[MAX_STRING_LENGTH];
    char varTwo[MAX_STRING_LENGTH];
    char varThree[MAX_STRING_LENGTH];
    char varFour[MAX_STRING_LENGTH];
    int answer;
} puzzle;
// Initilizers and Destructors
void Screen_init(void);
void Screen_destroy(void);
// Setters
void Screen_set_problem(puzzle data);

#endif
