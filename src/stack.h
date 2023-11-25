//
// Created by varev on 11/24/23.
//

#ifndef BACKGAMMON_STACK_H
#define BACKGAMMON_STACK_H

#include <stdlib.h>

#define SIZE_MULTIPLIER 2
#define STACK_DEFAULT_SIZE 5
#define STACK_SIZE_DOWN_SIZE 3

#define VERIFY_SIZE_UP 1
#define VERIFY_SIZE_DOWN 2

typedef struct STACK {
    void ** elements;
    int counter;
    size_t typeSize;
} stack;

void initStack(stack* stack, size_t size);
void appendElementToStack(stack* stack, void* element);
void* getElementFromStack(stack* stack);

#endif //BACKGAMMON_STACK_H
