#ifndef BACKGAMMON_GAME_H
#define BACKGAMMON_GAME_H

#include <stdlib.h>

#include "board.h"

#define BAR_INIT (-2)
#define NO_FORCED_MOVE (-1)

#define MAX_ROUNDS 15

typedef struct GAME {
    int score[2];
    int rounds;
    int currentRound;
    board board;
    bar bar;
    finish finish;
    int dice[MAX_DICES];
    int moveSize[MAX_DICES];
    char turn;
} game;

int RoundMenu(game* game);
void InitGame(game* game);
void RollDice(game* game);
void SetDicesIfDouble(int dice[DICE_AMOUNT]);
void SetFirstTurn(game* game);
void ChangeTurn(game* game);
void StartRound(game* game);
pawn_move GetNearestAttackFieldId(board board, char color, int moveSize);
void MovePawnOnBoard(board* board, char color, int initialField, int finalField);
void MovePawnFromBar(bar* bar, board* board, char color, int fieldId);
void MovePawnToFinish(board* board, finish* finish, char color, int fieldId);
void BeatPawn(board* board, bar* bar, char color, int fieldId);
int CheckWinner(game game);
int IsAnyMovePossible(board board, bar bar, char color, int dice[DICE_AMOUNT]);
void SetPossibleMoveSizes(int dice[MAX_DICES], int moveSize[MAX_DICES]);
void SortDice(int dice[DICE_AMOUNT]);

#endif //BACKGAMMON_GAME_H