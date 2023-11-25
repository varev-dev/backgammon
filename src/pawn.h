#ifndef BACKGAMMON_PAWN_H
#define BACKGAMMON_PAWN_H

#define WHITE_PAWN 'W'
#define RED_PAWN 'R'

typedef struct PAWN {
    char color;
} pawn;

void InitPawn(pawn * pawn, char color);
void InitManyPawns(pawn * pawn, char color, int amount);

#endif //BACKGAMMON_PAWN_H
