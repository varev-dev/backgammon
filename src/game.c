#include "game.h"

void SetUpGame(game* game) {
    int rounds;
    game->score[0] = game->score[1] = rounds = game->currentRound = 0;

    while (rounds < 1 || rounds > MAX_ROUNDS) {
        printf("How many rounds (1-%d):", MAX_ROUNDS);
        scanf("%d", &rounds);
    }

    game->rounds=rounds;
}

void InitGame(game* game) {
    SetUpGame(game);
    StartRound(game);
    PrintBoard(game->board, game->bar, game->finish);
}

void RoundIntro(game game) {
    printf("Round No. %d\n", game.currentRound);
    printf("RED roll dice\nWHITE roll dice\n");
    printf("RED - %d, WHITE - %d\n%c starts\n", game.dice[0], game.dice[1], game.turn);
}

void LoadCommand(char* command) {
    while (*command != INIT_BAR_SIGN && *command != BOARD_MOVE_SIGN && *command != FINISH_SIGN) {
        printf("Choose option: (B) move on board, (I) bar init");
        scanf("%c", command);
    }
}

void ChooseValueFromArray(int arr[MAX_DICES], int* size, char* type) {
    printf("%s:", type);
    for (int i = 0; i < MAX_DICES; i++) {
        if (arr[i] == 0)
            continue;

        printf(" (%d) %d", i+1, arr[i]);
    }
    printf("\n");

    while (1) {
        printf("Choose %s by id:", type);
        scanf("%d", size);

        if (*size > 0 && *size < MAX_DICES && arr[*size] != 0)
            break;
    }
}

void ChooseField(char* name, int* fieldId) {
    printf("Choose %s field (1-24)", name);

    while (*fieldId >= 24 || *fieldId < 0) {
        printf("Enter field id");
        scanf("%d", fieldId);
        (*fieldId)--;
    }
}

pawn_move MoveMenu(game game) {
    char command;
    LoadCommand(&command);
    int initialField = 25, finalField = 25;

    switch (command) {
        case INIT_BAR_SIGN:
            ChooseValueFromArray(game.dice, &finalField, "dice");
            if (game.turn == RED)
                finalField = ReversedFieldId(finalField);
            break;
        case FINISH_SIGN:
            ChooseField("initial", &initialField);
            break;
        case BOARD_MOVE_SIGN:
            ChooseField("initial", &initialField);
            ChooseValueFromArray(game.moveSize, &finalField, "move size");
            finalField = initialField + (game.turn == RED ? -1 : 1) * finalField;
            break;
    }

    pawn_move pawnMove;
    pawnMove.color = game.turn;
    pawnMove.type = command;
    pawnMove.initial = initialField;

    return pawnMove;
}

void PlayTurn(game* game) {
    if (game->dice[0] == 0)
        RollDice(game);

    SetDicesIfDouble(game->dice);
    SortDice(game->dice);
    SetPossibleMoveSizes(game->dice, game->moveSize);

    printf("%d %d", game->dice[0], game->dice[1]);

    while (IsAnyMovePossible(game->board, game->bar, game->turn, game->dice)) {
        pawn_move forced_move = IsThereForcedMove(game->board, game->bar, game->finish, game->turn, game->moveSize);

        if (forced_move.type == BAR_INIT)
            printf("You have to init pawn\n");
        if (forced_move.type == ATTACK_SIGN)
            printf("You have to beat pawn on field %d\n", forced_move.final);

        pawn_move move = MoveMenu(*game);

        if (move.type != forced_move.type)
            continue;
        if (move.type == ATTACK_SIGN && move.final != forced_move.final)
            continue;
    }

    ChangeTurn(game);
}

void PlayRound(game* game) {
    /*while (!CheckWinner(*game)) {
        printf("%s's turn, press key to roll the dice...", game->turn == RED ? "RED" : "WHITE");
        scanf("");

        pawn_move forced = IsThereForcedMove(*game);

        if (forced.type != NOT_SET) {
            printf("There is FORCED move, press key to continue");
        }
    }*/

    printf("Winner: %c", game->turn);
}

void RollDice(game* game) {
    for (int i = 0; i < DICE_AMOUNT; i++)
        game->dice[i] = rand() % 6 + 1;
}

void SortDice(int dice[DICE_AMOUNT]) {
    if (dice[1] < dice[0]) {
        int temp = dice[0];
        dice[0] = dice[1];
        dice[1] = temp;
    }
}

int IsItDouble(const int dice[DICE_AMOUNT]) {
    for (int i = 0; i < DICE_AMOUNT - 1; i++) {
        if (dice[i] != dice[i+1])
            return 0;
    }

    return 1;
}

void SetDicesIfDouble(int dice[MAX_DICES]) {
    for (int i = DICE_AMOUNT; i < MAX_DICES; i++) {
        if (IsItDouble(dice))
            dice[i] = dice[0];
        else
            dice[i] = 0;
    }
}

void SetPossibleMoveSizes(int dice[MAX_DICES], int moveSize[MAX_DICES]) {
    for (int i = 0; i < MAX_DICES; i++) {
        if (IsItDouble(dice))
            moveSize[i] = dice[0] * (i+1);
        else if (i < DICE_AMOUNT)
            moveSize[i] = dice[1];
        else if (i == DICE_AMOUNT) {
            for (int j = 0; j < i; j++) {
                moveSize[i] += dice[j];
            }
        } else
            moveSize[i] = 0;
    }
}

void ResetMovesOrDices(int arr[MAX_DICES]) {
    for (int i = 0; i < MAX_DICES; i++) {
        arr[i] = 0;
    }
}

void SetFirstTurn(game* game) {
    ResetMovesOrDices(game->dice);

    if (game->turn != NEUTRAL)
        return;

    while (game->dice[0] == game->dice[1]) {
        RollDice(game);
    }

    game->turn = game->dice[0] > game->dice[1] ? RED : WHITE;
}

void ChangeTurn(game* game) {
    ResetMovesOrDices(game->dice);
    ResetMovesOrDices(game->moveSize);

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
    RoundIntro(*game);
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