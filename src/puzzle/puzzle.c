#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "puzzle.h"

#define NUM_RANGE 10 // 0 - 9
#define NUM_OPERATIONS 4
#define NUM_SYMBOLS 4
#define BUFFER_SIZE 100

static diffculity myDiffculity = easy;

enum op {
    plus, minus, multiply, divide
};

static char opeartions[NUM_OPERATIONS] = {
    '+', '-', '*', '/'
};

void Puzzle_printPuzzle(puzzle myPuzzle) {
    printf("-------------------------\n");
    if (myPuzzle.diffculityLevel == genius) {
        printf("%s\n%s\n%s\n%s\n", 
               myPuzzle.varOne, 
               myPuzzle.varTwo,
               myPuzzle.varThree,
               myPuzzle.varFour);
        
    }
    printf("Problem: %s\nAnswer: %d\n", myPuzzle.problem, myPuzzle.answer);
    printf("-------------------------\n");
}

static int get_answer_to_simple_problem(char *problem) {
    char operand;
    int leftDigit, rightDigit;
    int correctAnswer;
    sscanf(problem, "(%d %c %d)\n", &leftDigit, &operand, &rightDigit);

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

    return correctAnswer;
}

static void generate_simple_random_problem(char *problem, int size) {
    int operation = rand() % NUM_OPERATIONS;
    int leftDigit = rand() % NUM_RANGE;
    int rightDigit = rand() % NUM_RANGE + 1; // so we can't have something like 1 / 0

    snprintf(problem, size - 1, "(%d %c %d)", leftDigit, opeartions[operation], rightDigit);
}

static void transformPuzzle(puzzle *myPuzzle) {
    int digitArray[4] = { 0 };
    char opArray[3] = { 0 };
    sscanf(myPuzzle->problem, "(%d %c %d) %c (%d %c %d)", 
           &digitArray[0], &opArray[0], &digitArray[1],
           &opArray[1],
           &digitArray[2], &opArray[2], &digitArray[3]);

    snprintf(myPuzzle->varOne, MAX_STRING_LENGTH_PUZZLE - 1, "A = %d", digitArray[0]);
    snprintf(myPuzzle->varTwo, MAX_STRING_LENGTH_PUZZLE - 1, "B = %d", digitArray[1]);
    snprintf(myPuzzle->varThree, MAX_STRING_LENGTH_PUZZLE - 1, "C = %d", digitArray[2]);
    snprintf(myPuzzle->varFour, MAX_STRING_LENGTH_PUZZLE - 1, "D = %d", digitArray[3]);
    snprintf(myPuzzle->problem, MAX_STRING_LENGTH_PUZZLE - 1, "(A %c B) %c (C %c D)", 
             opArray[0], opArray[1], opArray[2]);
}

puzzle Puzzle_generate() {
    srand(time(NULL));
    int answer = 0;
    puzzle myPuzzle;
    myPuzzle.diffculityLevel = myDiffculity;

    // so it will only generate 2 math equations
    if (myPuzzle.diffculityLevel == genius)
        myDiffculity = 2;

    char *tempString = malloc(MAX_STRING_LENGTH_PUZZLE);
    tempString[0] = '\0';

    generate_simple_random_problem(tempString, MAX_STRING_LENGTH_PUZZLE);
    snprintf(myPuzzle.problem, MAX_STRING_LENGTH_PUZZLE - 1, "%s", tempString);
    answer = get_answer_to_simple_problem(tempString);

    for (int i = 0; i < myDiffculity - 1; i++){
        tempString[0] = '\0';
        int myOp = rand() % NUM_OPERATIONS;
        generate_simple_random_problem(tempString, MAX_STRING_LENGTH_PUZZLE);

        int tempAnswer = get_answer_to_simple_problem(tempString);
        if (myOp == divide && tempAnswer == 0) {
            i -= 1;
            continue;
        }

        char opString[MAX_STRING_LENGTH_PUZZLE];
        snprintf(opString, MAX_STRING_LENGTH_PUZZLE - 1, " %c ", opeartions[myOp]);
        strncat(myPuzzle.problem, opString, MAX_STRING_LENGTH_PUZZLE - 1);
        strncat(myPuzzle.problem, tempString, MAX_STRING_LENGTH_PUZZLE - 1);
        
        switch (myOp)
        {
        case plus:
            answer += tempAnswer;
            break;

        case minus:
            answer -= tempAnswer;
            break;

        case divide:
            answer /= tempAnswer;
            break;

        case multiply:
            answer *= tempAnswer;
            break;
        }
    }
    free(tempString);

    myPuzzle.answer = abs(answer);

    if (myPuzzle.diffculityLevel == genius)
        transformPuzzle(&myPuzzle);

    return myPuzzle;
}

void Puzzle_updateDiffculity(diffculity myDiff) {
    myDiffculity = myDiff;
}
