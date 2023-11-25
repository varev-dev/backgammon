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

void InitStack(stack* stack, size_t size);
void AppendElementToStack(stack* stack, void* element);
void* GetTopElementFromStack(stack* stack);

#endif //BACKGAMMON_STACK_H
