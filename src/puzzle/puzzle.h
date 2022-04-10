// puzzle.h
// Module to genreate and validate puzzles
#include <stdbool.h>

#ifndef _PUZZLE_H
#define _PUZZLE_H

#define MAX_STRING_LENGTH_PUZZLE 50

typedef enum diffculity {
    easy = 1, medium = 2, hard = 3, genius
} diffculity;

// varOne, varTwo, varThree is only used if the diffculity is genius
typedef struct puzzle {
    diffculity diffculityLevel;
    char problem[MAX_STRING_LENGTH_PUZZLE];
    char varOne[MAX_STRING_LENGTH_PUZZLE];
    char varTwo[MAX_STRING_LENGTH_PUZZLE];
    char varThree[MAX_STRING_LENGTH_PUZZLE];
    char varFour[MAX_STRING_LENGTH_PUZZLE];
    int answer;
} puzzle;

// generate a random puzzle
// ! the answer to the problem would be in abs(int(answer))
puzzle Puzzle_generate(diffculity myDiffculity);

// print puzzle
void Puzzle_printPuzzle(puzzle myPuzzle);

#endif
