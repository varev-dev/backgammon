#ifndef BACKGAMMON_GAME_H
#define BACKGAMMON_GAME_H

#include <stdlib.h>

#include "board.h"
#include "move.h"

#define MAX_ROUNDS 15
#define DICE_AMOUNT 2
#define SINGLE_TURN_MAX_MOVES 4

typedef struct GAME {
    int rounds;
    int currentRound;
    board board;
    bar bar;
    finish finish;
    int dice[DICE_AMOUNT];
    int moveSize[SINGLE_TURN_MAX_MOVES];
    char turn;
} game;

int RoundMenu(game* game);
void InitGame(game* game);
void RollDice(game* game, int amount);
void SetFirstTurn(game* game);
void ChangeTurn(game* game);
void StartRound(game* game);
void MovePawnOnBoard(board* board, char color, int initialField, int finalField);
void MovePawnFromBar(bar* bar, board* board, char color, int fieldId);
void BeatPawn(board* board, bar* bar, char color, int fieldId);
int CheckWinner(game game);

#endif //BACKGAMMON_GAME_H
