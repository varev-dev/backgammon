#include "field.h"

void InitField(field* field) {
    InitStack(&field->pawns, sizeof(STORED_ITEM));
}

char FieldColor(field field) {
    if (field.pawns.counter == 0)
        return NEUTRAL_FIELD;

    pawn * pawn = field.pawns.elements[0];

    return pawn->color;
}

int IsMovePossible(field field, pawn pawn) {
    if (FieldColor(field) == pawn.color || FieldColor(field) == NEUTRAL_FIELD)
        return CLEAN_MOVE;

    if (field.pawns.counter <= MAX_PAWNS_TO_CAPTURE)
        return CAPTURE_MOVE;

    return NOT_POSSIBLE;
}

pawn* RemovePawn(field* field) {
    if (field->pawns.counter == 0)
        return NULL;

    return GetTopElementFromStack(&field->pawns);
}

void AppendPawn(field* field, pawn* pawn) {
    AppendElementToStack(&field->pawns, pawn);
}

int GetNumberOfPawnsOnField(field field) {
    return field.pawns.counter;
}