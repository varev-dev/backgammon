//
// Created by varev on 11/24/23.
//

#ifndef BACKGAMMON_STACK_H
#define BACKGAMMON_STACK_H

#include "board.h"

#define SIZE_MULTIPLIER 2
#define STACK_DEFAULT_SIZE 5
#define STACK_SIZE_DOWN_SIZE 3

#define VERIFY_SIZE_UP 1
#define VERIFY_SIZE_DOWN 2

typedef struct STACK {
    pawn ** elements;
    int counter;
} stack;

void initStack(stack* stack);
void appendElementToStack(stack* stack, pawn* element);
pawn* getElementFromStack(stack* stack);

#endif //BACKGAMMON_STACK_H
