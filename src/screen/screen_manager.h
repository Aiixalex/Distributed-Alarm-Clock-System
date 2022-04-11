#ifndef SCREEN_H_
#define SCREEN_H_
#include <stdbool.h>
#include "../puzzle/puzzle.h"
#include "OLED_text.h"

// Initilizers and Destructors
void Screen_init(void);
void Screen_destroy(void);
// Setters
void Screen_set_problem(puzzle data);
void Screen_clear(void);

#endif
