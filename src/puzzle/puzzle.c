#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "puzzle.h"

#define NUM_RANGE 10 // 0 - 9
#define NUM_OPERATIONS 4
#define BUFFER_SIZE 100

static char opeartions[NUM_OPERATIONS] = {
    '+', '-', '*', '/'
};

char *Puzzle_generate() {
    srand(time(NULL));

    char *problemString = malloc(BUFFER_SIZE);
    int operation = rand() % NUM_OPERATIONS;
    int leftDigit = rand() % NUM_RANGE;
    int rightDigit = rand() % NUM_RANGE + 1; // so we can't have something like 1 / 0

    snprintf(problemString, BUFFER_SIZE - 1, "%d %c %d = ?\n", 
             leftDigit, opeartions[operation], rightDigit);

    return problemString;
}

bool Puzzle_validate(char *problem, int answer) {
    char operand;
    int leftDigit, rightDigit;
    int correctAnswer;
    sscanf(problem, "%d %c %d = ?\n", &leftDigit, &operand, &rightDigit);
    printf("Test: %d %c %d = ?\n", leftDigit, operand, rightDigit);

        switch (operand)
    {
    case '+':
        correctAnswer = leftDigit + rightDigit;
        break;

    case '-':
        correctAnswer = leftDigit - rightDigit;
        break;
        
    case '*':
        correctAnswer = leftDigit * rightDigit;
        break;

    case '/':
        correctAnswer = leftDigit / rightDigit;
        break;

    default:
        printf("Puzzle: Unknow operand %c from %s\n", operand, problem);
        return false;
        break;
    }

    return answer == correctAnswer;
}
