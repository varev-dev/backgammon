#include "game.h"

void SetUpGame(game* game) {
    int rounds;
    game->score[0] = game->score[1] = rounds = game->currentRound = 0;

    while (rounds < 1 || rounds > MAX_ROUNDS) {
        printf("How many rounds (1-%d):", MAX_ROUNDS);
        scanf("%d", &rounds);
    }
    scanf("");

    game->rounds=rounds;
}

void RoundIntro(game game) {
    printf("Round No. %d\n", game.currentRound);
    printf("RED roll dice\nWHITE roll dice\n");
    printf("RED - %d, WHITE - %d\n%c starts\n", game.dice[0], game.dice[1], game.turn);
}

void LoadCommand(char* command) {
    printf("Choose option:\n");

    printf("(%c) init pawn from bar\n", INIT_BAR_SIGN);
    printf("(%c) move to finish\n", FINISH_SIGN);
    printf("(%c) move from board\n", BOARD_MOVE_SIGN);

    while (*command != INIT_BAR_SIGN && *command != BOARD_MOVE_SIGN && *command != FINISH_SIGN) {
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

        if (*size-1 >= 0 && *size < MAX_DICES && arr[*size - 1] != 0) {
            *size = arr[*size - 1];
            break;
        }
    }
}

void ChooseField(char* name, int* fieldId) {
    printf("Choose %s field (1-24)\n", name);

    while (*fieldId >= 24 || *fieldId <0) {
        scanf("%d", fieldId);
        (*fieldId)--;
    }
}

void Command(char command, int* init, int* final, char color, int dice[MAX_DICES], int moveSize[MAX_DICES]) {
    switch (command) {
        case INIT_BAR_SIGN:
            ChooseValueFromArray(dice, final, "dice");
            if (color == RED) {
                *final = ReversedFieldId(*final);
                (*final)++;
            } else
                (*final)--;
            break;
        case FINISH_SIGN:
            ChooseField("initial", init);
            break;
        case BOARD_MOVE_SIGN:
            ChooseField("initial", init);
            ChooseValueFromArray(moveSize, final, "move size");
            (*final) = *init + (color == RED ? -1 : 1) * (*final);
            break;
    }
}

pawn_move MoveMenu(game game) {
    char command = 1;
    LoadCommand(&command);
    int initial, final;
    initial = final =  FieldIdByColor(FIELDS, game.turn);

    Command(command, &initial, &final, game.turn, game.dice, game.moveSize);

    pawn_move pawnMove;
    pawnMove.color = game.turn;
    pawnMove.type = command;
    pawnMove.initial = initial;
    pawnMove.final = final;

    return pawnMove;
}

void DiceConf(int dice[MAX_DICES], int moveSize[MAX_DICES]) {
    SortDice(dice);
    SetPossibleMoveSizes(dice, moveSize);
    for (int i = 0; i < MAX_DICES; i++) {
        if (dice[i] == 0)
            break;
        printf("%d ", dice[i]);
    }
    printf("\n");
}

void PlayTurn(game* game) {
    RollDice(game);
    game->dice[0] = 2;
    game->dice[1] = 4;
    SetDicesIfDouble(game->dice);
    DiceConf(game->dice, game->moveSize);

    while (IsAnyMovePossible(game->board, game->bar, game->turn, game->dice) && !IsDiceEmpty(game->dice) && !CheckWinner(*game)) {
        PrintBoard(game->board, game->bar, game->finish);
        pawn_move forcedMove = IsThereForcedMove(game->board, game->bar,game->turn, game->moveSize, game->dice);

        if (forcedMove.type == INIT_BAR_SIGN)
            printf("You have to init pawn\n");
        if (forcedMove.type == ATTACK_SIGN || (forcedMove.type == INIT_BAR_SIGN && forcedMove.final != -1))
            printf("You have to beat pawn on field %d\n", forcedMove.final+1);

        pawn_move move = MoveMenu(*game);
        int mvRat = -1;

        if (move.type != FINISH_SIGN)
            mvRat = CheckIsMovePossible(game->board.fields[move.final], game->turn);

        if (mvRat == NOT_POSSIBLE_MOVE || (move.final < 0 || move.final >= FIELDS || (game->board.fields[move.initial].color != game->turn && move.type != INIT_BAR_SIGN)) && move.type != FINISH_SIGN) {
            printf("move is not possible\n");
            continue;
        }

        if (move.type == FINISH_SIGN && !CheckFinishMove(game->board, game->turn, move.initial, game->dice)) {
            printf("YOU HAVE TO USE OTHER PAWN TO GET TO FINISH\n");
        } else if ((forcedMove.type == INIT_BAR_SIGN && move.type != INIT_BAR_SIGN) || (forcedMove.final != -1 && forcedMove.type != NOT_SET && move.final != forcedMove.final)) {
            printf("YOU HAVE TO MAKE FORCED MOVE\n");
            continue;
        }

        if (move.type == FINISH_SIGN && !IsMoveToFinishPossible(game->board, game->turn, game->moveSize)) {
            printf("YOU HAVE TO MOVE OTHER PAWN TO FINISH");
            continue;
        }

        if (mvRat == ATTACK_MOVE || (move.type == INIT_BAR_SIGN && mvRat == ATTACK_MOVE))
            BeatPawn(&game->board, &game->bar, move);
        if (move.type == INIT_BAR_SIGN)
            MovePawnFromBar(&game->bar, &game->board, game->turn, move.final);
        if (CheckFinishMove(game->board, game->turn, move.initial, game->dice) && move.type == FINISH_SIGN)
            MovePawnToFinish(&game->board, &game->finish, game->turn, move.initial);
        if ((mvRat == CLEAN_MOVE || mvRat == ATTACK_MOVE) && move.type != INIT_BAR_SIGN)
            MovePawnOnBoard(&game->board, move);

        RemoveDiceFromMove(game->board, move, game->turn, game->dice);
        DiceConf(game->dice, game->moveSize);
    }

    if (!IsAnyMovePossible(game->board, game->bar, game->turn, game->dice))
        printf("No possible moves\n");
}

void RemoveDiceIfFinish(board board, int dice[MAX_DICES], char color, pawn_move move) {
    int mult = Mlp(color);
    for (int i = 0; i < MAX_DICES; i++) {
        int final = move.initial + mult * dice[i];
        if (final == -1 || final == FIELDS) {
            RemoveDice(dice, dice[i]);
            return;
        }
    }
    if (move.initial == ForcedFinishFieldId(board, color, dice)) {
        RemoveDice(dice, MaxDiceValue(dice));
    }
}

void RemoveDiceIfNotFinish(int dice[MAX_DICES], int moveSize) {
    if (IsItDouble(dice)) {
        int tmp = dice[0];
        for (int i = 0; i < (moveSize / tmp); i++) {
            RemoveDice(dice, tmp);
        }
        return;
    } else {
        if (moveSize == dice[0] + dice[1]) {
            RemoveDice(dice, dice[0]);
            RemoveDice(dice, dice[1]);
        } else {
            RemoveDice(dice, moveSize);
        }
    }
}

int Abs(int value) {
    return value >= 0 ? value : value * (-1);
}

void RemoveDiceFromMove(board board, pawn_move move, char color, int dice[MAX_DICES]) {
    int mult = Mlp(color);
    int moveSize;

    if (move.type == INIT_BAR_SIGN)
        moveSize = Abs(FieldIdByColor(-1,  color)) + move.final * mult;
    else if (color == RED)
        moveSize = move.initial - move.final;
    else
        moveSize = move.final - move.initial;
    moveSize = Abs(moveSize);

    printf("\n\n%d %d %d\n\n", moveSize, FieldIdByColor(-1,  color) , move.final);

    if (move.type == FINISH_SIGN) {
        RemoveDiceIfFinish(board, dice, color, move);
    } else {
        RemoveDiceIfNotFinish(dice, moveSize);
    }
}

void PlayRound(game* game) {
    while (!CheckWinner(*game)) {
        printf("%s's turn, roll the dice...\n", colorString(game->turn));
        PlayTurn(game);
        if (!CheckWinner(*game))
            ChangeTurn(game);
    }
    PrintBoard(game->board, game->bar, game->finish);

    printf("Winner: %s", colorString(game->turn));
}

void InitGame(game* game) {
    SetUpGame(game);
    StartRound(game);
    PlayRound(game);
}

void RollDice(game* game) {
    for (int i = 0; i < DICE_AMOUNT; i++)
        game->dice[i] = rand() % 6 + 1;
}

int IsDiceEmpty(const int dice[MAX_DICES]) {
    for (int i = 0; i < MAX_DICES; i++) {
        if (dice[i] != 0)
            return 0;
    }
    return 1;
}

void SortDice(int dice[MAX_DICES]) {
    for (int i = 0; i < MAX_DICES; i++) {
        for (int j = 0; j < MAX_DICES-i-1; j++) {
            if ((dice[j] > dice[j+1] && dice[j+1] != 0) || dice[j] == 0) {
                int temp = dice[j];
                dice[j] = dice[j+1];
                dice[j+1] = temp;
            }
        }
    }
}

int IsItDouble(const int dice[DICE_AMOUNT]) {
    for (int i = 0; i < DICE_AMOUNT - 1; i++) {
        if (dice[i] != dice[i+1])
            return 0;
    }

    return 1;
}

void RemoveDice(int dice[MAX_DICES], int value) {
    for (int i = 0; i < MAX_DICES; i++) {
        if (dice[i] == value) {
            dice[i] = 0;
            break;
        }
    }
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
    int ctr = MAX_DICES;
    for (int i = 0; i < MAX_DICES; i++) {
        if (dice[i] == 0)
            ctr = i;
        if (IsItDouble(dice))
            moveSize[i] = dice[0] * (i + 1);
        else
            moveSize[i] = dice[i];
    }
    for (int i = ctr; i < MAX_DICES; i++) {
        moveSize[i] = 0;
    }
    if (dice[0] && dice[1] && dice[0] != dice[1])
        moveSize[2] = dice[0] + dice[1] ;
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
    /*InitFieldWIthData(&game->bar.white_pawns, WHITE, 1);
    InitFieldWIthData(&game->bar.red_pawns, RED, 1);*/
    InitField(&game->finish.white_pawns);
    InitField(&game->finish.red_pawns);

    game->turn = NEUTRAL;
    SetFirstTurn(game);
    RoundIntro(*game);
}

int CheckWinner(game game) {
    field playerBar = game.turn == RED ? game.bar.red_pawns : game.bar.white_pawns;

    if (playerBar.pawnsCounter)
        return 0;

    if (CountPawnsOnBoard(game.board, game.turn, FIELDS))
        return 0;

    return 1;
}