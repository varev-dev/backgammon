#ifndef BACKGAMMON_FIELD_H
#define BACKGAMMON_FIELD_H

#include "stack.h"
#include "pawn.h"

#define NEUTRAL_FIELD 'N'
#define CLEAN_MOVE 1
#define CAPTURE_MOVE 2
#define NOT_POSSIBLE 3
#define MAX_PAWNS_TO_CAPTURE 1
#define STORED_ITEM pawn *

typedef struct FIELD {
    stack pawns;
} field;

void InitField(field* field);
char FieldColor(field field);
int IsMovePossible(field field, pawn pawn);
pawn* RemovePawn(field* field);
void AppendPawn(field* field, pawn* pawn);
int GetNumberOfPawnsOnField(field field);

#endif //BACKGAMMON_FIELD_H
