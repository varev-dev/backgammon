#ifndef BACKGAMMON_GAME_H
#define BACKGAMMON_GAME_H

#include <stdlib.h>

#include "board.h"

#define SAVE_SIZE 59
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
void StartRound(game* game, int bar[2], int finish[2]);
int CheckWinner(game game);
void SetPossibleMoveSizes(int dice[MAX_DICES], int moveSize[MAX_DICES]);
void SortDice(int dice[DICE_AMOUNT]);
void RemoveDice(int dice[MAX_DICES], int value);
int IsDiceEmpty(const int dice[MAX_DICES]);
void RemoveDiceFromMove(board board, pawn_move move, char color, int dice[MAX_DICES]);
int IsItDouble(const int dice[DICE_AMOUNT]);

#endif //BACKGAMMON_GAME_H