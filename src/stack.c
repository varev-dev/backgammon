//
// Created by varev on 11/24/23.
//
#include "stack.h"
#include "board.h"
#include <stdlib.h>

void initStack(stack* stack) {
    stack->counter = 0;
    stack->elements = malloc(sizeof(pawn *) * STACK_DEFAULT_SIZE);
}

void resizeStack(stack* stack, int resizeType) {
    pawn ** elementsStorage;
    elementsStorage = stack->elements;

    unsigned long size = 1;

    if (resizeType == VERIFY_SIZE_UP)
        size = stack->counter * SIZE_MULTIPLIER;
    else
        size = stack->counter + (STACK_DEFAULT_SIZE - STACK_SIZE_DOWN_SIZE);

    stack->elements = malloc(size * sizeof(pawn *));

    for (int i = 0; i < stack->counter; i++)
        stack->elements[i] = elementsStorage[i];

    free(elementsStorage);
}

int checkNumberIsBaseToPower(int number, int base) {
    while (!(number % base))
        number /= base;

    return number == 1;
}

int verifyResize(int counter, int resizeType) {
    int checkZero = counter - (resizeType == VERIFY_SIZE_DOWN ? STACK_SIZE_DOWN_SIZE : 0);
    int checkSize = counter % STACK_DEFAULT_SIZE == (resizeType == VERIFY_SIZE_DOWN ? STACK_SIZE_DOWN_SIZE : 0);

    int checkNumber;
    if (checkSize && checkZero) {
        checkNumber = checkNumberIsBaseToPower(
                ((resizeType == VERIFY_SIZE_DOWN ? STACK_SIZE_DOWN_SIZE : 0) + counter) / STACK_DEFAULT_SIZE,
                SIZE_MULTIPLIER);
    }

    return checkZero && checkSize && checkNumber;
}

void appendElementToStack(stack* stack, pawn* element) {
    if (verifyResize(stack->counter, VERIFY_SIZE_UP))
        resizeStack(stack, VERIFY_SIZE_UP);

    stack->elements[stack->counter++] = element;
}

pawn * removeElementFromStack(stack* stack) {
    pawn * pawn = stack->elements[stack->counter - 1];
    stack->elements[(stack->counter--) - 1] = NULL;
    return pawn;
}

pawn * getElementFromStack(stack* stack) {
    if (stack->counter == 0)
        return NULL;

    pawn * pawn = removeElementFromStack(stack);

    if (verifyResize(stack->counter, VERIFY_SIZE_DOWN))
        resizeStack(stack, VERIFY_SIZE_DOWN);

    return pawn;
}