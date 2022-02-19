// puzzle.h
// Module to genreate and validate puzzles
#include <stdbool.h>

#ifndef _PUZZLE_H
#define _PUZZLE_H

// generate a random puzzle
char *puzzle_generate();

// validate if answer_key is correct
bool puzzle_validate(char *answer_key);

#endif
