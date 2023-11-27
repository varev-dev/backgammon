#ifndef BACKGAMMON_FIELD_H
#define BACKGAMMON_FIELD_H

#include <stdio.h>

#define NEUTRAL_FIELD 'N'
#define RED_FIELD 'R'
#define WHITE_FIELD 'W'

#define MAX_PIECES_TO_BEAT 1

typedef struct FIELD {
    char color;
    int pawns_counter;
} field;

void InitField(field* field);
int RemovePawn(field* field);
void AppendPawn(field* field, char color, int amount);

#endif //BACKGAMMON_FIELD_H
