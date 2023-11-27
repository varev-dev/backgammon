#include "field.h"

void InitField(field* field) {
    field->color = NEUTRAL_FIELD;
    field->pawns_counter = 0;
}

int RemovePawn(field* field) {
    if (field->pawns_counter == 0)
        return 0;

    field->pawns_counter--;

    if (field->pawns_counter == 0)
        field->color = NEUTRAL_FIELD;

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