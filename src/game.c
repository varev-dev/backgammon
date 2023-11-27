#include "game.h"

void SetUpGame(game* game) {
    int rounds = 0;

    while (rounds <= 0 || rounds > 15) {
        printf("How many rounds (1-15): ");
        scanf("%d", &rounds);
    }

    game->rounds=rounds;
}

void StartRound(game* game) {
    InitBoard(&game->board1);
    InitField(&game->bar1.white_pawns);
    InitField(&game->bar1.red_pawns);
    InitField(&game->finish1.white_pawns);
    InitField(&game->finish1.red_pawns);

    game->turn = NEUTRAL;
    SetFirstTurn(game);
}

void InitGame(game* game) {
    SetUpGame(game);

    PrintBoard(game->board1);
}

void RollDice(game* game, int amount) {
    for (int i = 0; i < amount && i < DICE_AMOUNT; i++) {
        game->dice[i] = rand() % 6 + 1;
    }
}

void SetFirstTurn(game* game) {
    if (game->turn != NEUTRAL)
        return;

    while (game->dice[0] == game->dice[1]) {
        RollDice(game, DICE_AMOUNT);
    }

    game->turn = game->dice[0] > game->dice[1] ? RED : WHITE;
}

void ChangeTurn(game* game) {
    if (game->turn == NEUTRAL)
        return;

    game->turn = game->turn == WHITE ? RED : WHITE;
}