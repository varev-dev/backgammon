#include "field.h"

void InitField(field* field) {
    field->color = NEUTRAL;
    field->pawns_counter = 0;
}

int RemovePawn(field* field) {
    if (field->pawns_counter == 0)
        return 0;

    field->pawns_counter--;

    if (field->pawns_counter == 0)
        field->color = NEUTRAL;

    return 1;
}

void AppendPawn(field* field, char color, int amount) {
    if (amount <= 0)
        return;

    if (field->color != color && field->pawns_counter > 0)
        return;

    if (field->pawns_counter == 0)
        field->color = color;

    field->pawns_counter += amount;
}

int CheckIsMovePossible(field field, char color) {
    if (field.color == color || field.color == NEUTRAL || !field.pawns_counter)
        return CLEAN_MOVE;

    if (field.pawns_counter > 0 && field.pawns_counter <= MAX_PIECES_TO_BEAT)
        return ATTACK_MOVE;

    return NOT_POSSIBLE_MOVE;
}