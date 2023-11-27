#ifndef BACKGAMMON_GAME_H
#define BACKGAMMON_GAME_H

#include <stdlib.h>

#include "board.h"

#define DICE_AMOUNT 2

typedef struct GAME {
    int rounds;
    int currentRound;
    board board1;
    bar bar1;
    finish finish1;
    int dice[DICE_AMOUNT];
    char turn;
} game;

void InitGame(game* game);
void RollDice(game* game, int amount);
void SetFirstTurn(game* game);
void ChangeTurn(game* game);
void StartRound(game* game);

#endif //BACKGAMMON_GAME_H
