#ifndef BACKGAMMON_FIELD_H
#define BACKGAMMON_FIELD_H

#include <stdio.h>

#define NEUTRAL 'N'
#define RED 'R'
#define WHITE 'W'

#define MAX_PIECES_TO_BEAT 1

#define CLEAN_MOVE 2
#define ATTACK_MOVE 1
#define NOT_POSSIBLE_MOVE 0

typedef struct FIELD {
    char color;
    int pawnsCounter;
} field;

void InitField(field* field);
int RemovePawn(field* field);
void AppendPawn(field* field, char color, int amount);
int CheckIsMovePossible(field field, char color);

#endif //BACKGAMMON_FIELD_H