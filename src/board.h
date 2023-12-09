#ifndef BACKGAMMON_BOARD_H
#define BACKGAMMON_BOARD_H

#include <stdio.h>
#include <malloc.h>

#include "field.h"
#include "move.h"

#define BOARD_PATTERN   "B0000E0C000EE000C0E0000B"
#define PAWN_COLORS     "W0000R0R000WR000W0W0000R"

#define BAR 1

#define FIELDS 24
#define HOME_FIELDS 6
#define DICE_AMOUNT 2
#define MAX_DICES 4

typedef struct BOARD {
    field fields[24];
} board;

typedef struct SECTION_WITH_COUNTER {
    field red_pawns;
    field white_pawns;
} bar, finish;

void InitBoard(board* board);
void PrintBoard(board board, bar bar, finish finish);
pawn_move IsThereForcedMove(board board, bar bar, finish finish, char color, int moveSize[MAX_DICES], int dice[MAX_DICES]);
int CountPawnsOnBoard(board board, char color, int category);
void ForcedAttack(board board, char color, int moveSize[MAX_DICES], pawn_move* move);
int IsBarInitAttackPossible(bar bar, board board, char color, int dice[MAX_DICES]);
void MovePawnOnBoard(board* board, pawn_move move);
void MovePawnFromBar(bar* bar, board* board, char color, int fieldId);
void MovePawnToFinish(board* board, finish* finish, char color, int fieldId);
void BeatPawn(board* board, bar* bar, pawn_move move);
int IsBarEmpty(bar bar, char color);
int ReversedFieldId(int id);
int FieldIdByColor(int fieldId, char color);

#endif //BACKGAMMON_BOARD_H