#include "stack.h"

void InitStack(stack* stack, size_t size) {
    stack->counter = 0;
    stack->typeSize = size;
    stack->elements = malloc(size * STACK_DEFAULT_SIZE);
}

void ResizeStack(stack* stack, int resizeType) {
    void ** elementsStorage;
    elementsStorage = stack->elements;

    unsigned long size;

    if (resizeType == VERIFY_SIZE_UP)
        size = stack->counter * SIZE_MULTIPLIER;
    else
        size = stack->counter + (STACK_DEFAULT_SIZE - STACK_SIZE_DOWN_SIZE);

    stack->elements = malloc(size * stack->typeSize);

    for (int i = 0; i < stack->counter; i++)
        stack->elements[i] = elementsStorage[i];

    free(elementsStorage);
}

int CheckNumberIsBaseToPower(int number, int base) {
    while (!(number % base))
        number /= base;

    return number == 1;
}

int VerifyResize(int counter, int resizeType) {
    int checkZero = counter - (resizeType == VERIFY_SIZE_DOWN ? STACK_SIZE_DOWN_SIZE : 0);
    int checkSize = counter % STACK_DEFAULT_SIZE == (resizeType == VERIFY_SIZE_DOWN ? STACK_SIZE_DOWN_SIZE : 0);

    int checkNumber;
    if (checkSize && checkZero) {
        checkNumber = CheckNumberIsBaseToPower(
                ((resizeType == VERIFY_SIZE_DOWN ? STACK_SIZE_DOWN_SIZE : 0) + counter) / STACK_DEFAULT_SIZE,
                SIZE_MULTIPLIER);
    }

    return checkZero && checkSize && checkNumber;
}

void AppendElementToStack(stack* stack, void* element) {
    if (VerifyResize(stack->counter, VERIFY_SIZE_UP))
        ResizeStack(stack, VERIFY_SIZE_UP);

    stack->elements[stack->counter++] = element;
}

void * RemoveElementFromStack(stack* stack) {
    void * element = stack->elements[stack->counter - 1];
    stack->elements[(stack->counter--) - 1] = NULL;
    return element;
}

void * GetTopElementFromStack(stack* stack) {
    if (stack->counter == 0)
        return NULL;

    void * element = RemoveElementFromStack(stack);

    if (VerifyResize(stack->counter, VERIFY_SIZE_DOWN))
        ResizeStack(stack, VERIFY_SIZE_DOWN);

    return element;
}