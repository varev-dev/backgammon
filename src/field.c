#include "field.h"

void InitField(field* field) {
    field->color = NEUTRAL;
    field->pawnsCounter = 0;
}

int RemovePawn(field* field) {
    if (field->pawnsCounter == 0)
        return 0;

    field->pawnsCounter--;

    if (field->pawnsCounter == 0)
        field->color = NEUTRAL;

    return 1;
}

void AppendPawn(field* field, char color, int amount) {
    if (amount <= 0)
        return;

    if (field->color != color && field->pawnsCounter > 0)
        return;

    if (field->pawnsCounter == 0)
        field->color = color;

    field->pawnsCounter += amount;
}

int CheckIsMovePossible(field field, char color) {
    if (field.color == color || field.color == NEUTRAL || !field.pawnsCounter)
        return CLEAN_MOVE;

    if (field.pawnsCounter > 0 && field.pawnsCounter <= MAX_PIECES_TO_BEAT)
        return ATTACK_MOVE;

    return NOT_POSSIBLE_MOVE;
}