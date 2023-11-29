#include "game.h"

int RoundMenu(game* game) {

}

void PlayRound(game* game) {
    while (!CheckWinner(*game)) {
        printf("%s's turn, press key to roll the dice...", game->turn == RED ? "RED" : "WHITE");
        scanf("");

        if (IsThereForcedMove(*game)) {
            printf("There is FORCED move, press key to continue");
        }

    }

    printf("Winner: %c", game->turn);
}

void SetUpGame(game* game) {
    int rounds;
    game->score[0] = game->score[1] = rounds = game->currentRound = 0;

    while (rounds < 1 || rounds > MAX_ROUNDS) {
        printf("How many rounds (1-%d): ", MAX_ROUNDS);
        scanf("%d", &rounds);
    }

    game->rounds=rounds;
}

void InitGame(game* game) {
    SetUpGame(game);
    StartRound(game);
    PrintBoard(game->board);
    PrintSectionWithCounter(game->bar, "Bar");
    PrintSectionWithCounter(game->finish, "Finish");
}

void RollDice(game* game, int amount) {
    for (int i = 0; i < amount && i < DICE_AMOUNT; i++) {
        game->dice[i] = rand() % 6 + 1;
    }
}

int IsItDouble(const int dice[DICE_AMOUNT]) {
    for (int i = 0; i < DICE_AMOUNT - 1; i++) {
        if (dice[i] != dice[i+1])
            return 0;
    }

    return 1;
}

void SetMovesSize(int dice[DICE_AMOUNT], int* moveSize) {
    for (int i = 0; i < SINGLE_TURN_MAX_MOVES; i++) {
        if (i < DICE_AMOUNT)
            moveSize[i] = dice[i];
        else if (IsItDouble(dice))
            moveSize[i] = dice[0];
        else
            moveSize[i] = 0;
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

void StartRound(game* game) {
    game->currentRound++;

    InitBoard(&game->board);
    InitField(&game->bar.white_pawns);
    InitField(&game->bar.red_pawns);
    InitField(&game->finish.white_pawns);
    InitField(&game->finish.red_pawns);

    game->turn = NEUTRAL;
    SetFirstTurn(game);
}

int GetNearestAttackFieldId(board board, char color, int moveSize) {
    int fieldId = color == RED ? FIELDS - 1 : 0;
    moveSize *= (color == RED ? -1 : 1);

    while (fieldId < FIELDS && fieldId >= 0) {
        if (board.fields[fieldId].color == color)
            continue;

        if (fieldId + moveSize >= 24 || fieldId + moveSize < 0)
            continue;

        int moveRating = CheckIsMovePossible(board.fields[fieldId + moveSize], color);

        if (moveRating == ATTACK_MOVE)
            return fieldId;

        moveSize > 0 ? fieldId++ : fieldId--;
    }

    return NO_FORCED_MOVE;
}

int IsMoveFromBarPossible(board board, char color, int dice[DICE_AMOUNT]) {
    for (int i = 0; i < DICE_AMOUNT; i++) {
        int fieldId = color == RED ? ReversedFieldId(dice[i]) : dice[i];
        if (CheckIsMovePossible(board.fields[fieldId], color))
            return 1;
    }

    return 0;
}

int IsMoveWithSizeFromBoardPossible(board board, char color, int moveSize) {
    for (int i = 0; i < FIELDS - moveSize; i++) {
        int fieldId = color == RED ? ReversedFieldId(i) - moveSize : i + moveSize;

        if (CheckIsMovePossible(board.fields[fieldId], color))
            return 1;
    }

    return 0;
}

int IsMoveToFinishPossible(board board, char color, int moveSize) {
    int pawnsOnBoard = CountPawnsOnBoard(board, color, FIELDS),
        homePawns = CountPawnsOnBoard(board, color, HOME_FIELDS);

    if (pawnsOnBoard - homePawns > 0 || homePawns == 0)
        return 0;

    for (int i = FIELDS - 1; i >= 0; i--) {
        int fieldId = color == RED ? ReversedFieldId(i) : i;

        if (board.fields[fieldId].color != color)
            continue;

        if (moveSize - i >= 0)
            return 1;
    }

    return 0;
}

int IsAnyPossibleMove(board board, bar bar, char color, int dice[DICE_AMOUNT]) {
    if (!IsBarEmpty(bar, color))
        return IsMoveFromBarPossible(board, color, dice);

    for (int i = 0; i < DICE_AMOUNT; i++) {
        if (IsMoveWithSizeFromBoardPossible(board, color, dice[i]) ||
            IsMoveToFinishPossible(board, color, dice[i]))
            return 1;
    }

    return 0;
}

// to-do: return forced move as sth more readable
int IsThereForcedMove(game game) {
    if (!IsBarEmpty(game.bar, game.turn))
        return BAR_INIT;

    int move = 0;

    while (game.moveSize[move]) {
        int nearestAttack = GetNearestAttackFieldId(game.board, game.turn, game.moveSize[move]);

        if (nearestAttack != NO_FORCED_MOVE)
            return nearestAttack;

        move++;
    }

    int pawnsOnBoard = CountPawnsOnBoard(game.board, game.turn, FIELDS),
            homePawns = CountPawnsOnBoard(game.board, game.turn, HOME_FIELDS);

    if ()

    return 0;
}

void MovePawnOnBoard(board* board, char color, int initialField, int finalField) {
    RemovePawn(&board->fields[initialField]);
    AppendPawn(&board->fields[finalField], color, 1);
}

void MovePawnFromBar(bar* bar1, board* board, char color, int fieldId) {
    field * playerBar = color == RED ? &bar1->red_pawns : &bar1->white_pawns;

    RemovePawn(playerBar);
    AppendPawn(&board->fields[fieldId], color, 1);
}

void MovePawnToFinish(board* board, finish* finish, char color, int fieldId) {
    field* playerFinish = color == RED ? &finish->red_pawns : &finish->white_pawns;

    RemovePawn(&board->fields[fieldId]);
    AppendPawn(playerFinish, color, 1);
}

void BeatPawn(board* board, bar* bar, char color, int fieldId) {
    int beatenPawns = board->fields[fieldId].pawnsCounter;
    char opponentColor = color == RED ? WHITE : RED;
    field * opponentBar = color == RED ? &bar->white_pawns : &bar->red_pawns;

    AppendPawn(opponentBar, opponentColor, beatenPawns);
}

int CheckWinner(game game) {
    field playerBar = game.turn == RED ? game.bar.red_pawns : game.bar.white_pawns;

    if (!playerBar.pawnsCounter)
        return 0;

    for (int i = 0; i < FIELDS; i++) {
        if (game.board.fields[i].color == game.turn && !game.board.fields[i].pawnsCounter)
            return 0;
    }

    return 1;
}