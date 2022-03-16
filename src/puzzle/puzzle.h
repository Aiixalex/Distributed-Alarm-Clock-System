// puzzle.h
// Module to genreate and validate puzzles
#include <stdbool.h>

#ifndef _PUZZLE_H
#define _PUZZLE_H

// generate a random puzzle
// ! the caller must free the pointer returned
char *Puzzle_generate();

// validate if answer_key is correct
bool Puzzle_validate(char *problem, int answer);

#endif
