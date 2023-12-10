#include "game.h"

void SetUpFields(game* game, int current, int rounds, int score[2]) {
    game->rounds = rounds;
    game->currentRound = current;
    game->score[0] = score[0];
    game->score[1] = score[1];
}

void SetUpGame(game* game, int current, int rounds, int score[2]){
    SetUpFields(game, current, rounds, score);

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

        if (*size-1 >= 0 && *size-1 < MAX_DICES && arr[*size - 1] != 0) {
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

int TextLength(const char* text) {
    int counter = 1;

    while (text[counter++] != '\0') {
    }

    return counter;
}

void WriteToFile(FILE* file, char* text) {
    file = fopen(SAVE_NAME, "ab+");
    fwrite(text, sizeof(char), TextLength(text) - 1, file);
    fclose(file);
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

void Dices(game* game) {
    RollDice(game);
    SetDicesIfDouble(game->dice);
    DiceConf(game->dice, game->moveSize);
}

void PrintForced(pawn_move forcedMove) {
    if (forcedMove.type == INIT_BAR_SIGN)
        printf("You have to init pawn\n");
    if (forcedMove.type == ATTACK_SIGN || (forcedMove.type == INIT_BAR_SIGN && forcedMove.final != -1))
        printf("You have to beat pawn on field %d\n", forcedMove.final+1);
}

int CheckForced(pawn_move mv, pawn_move forced, game game) {
    if (mv.type == FINISH_SIGN && !CheckFinishMove(game.board, game.turn, mv.initial, game.dice)) {
        printf("YOU HAVE TO USE OTHER PAWN TO GET TO FINISH\n");
        return 0;
    } else if ((forced.type == INIT_BAR_SIGN && mv.type != INIT_BAR_SIGN) ||
            (forced.final != -1 && forced.type != NOT_SET && mv.final != forced.final)) {
        printf("YOU HAVE TO MAKE FORCED MOVE\n");
        return 0;
    }

    if (mv.type == FINISH_SIGN && !IsMoveToFinishPossible(game.board, game.turn, game.moveSize)) {
        printf("YOU HAVE TO MOVE OTHER PAWN TO FINISH");
        return 0;
    }

    return 1;
}

int TurnCondition(game game) {
    return IsAnyMovePossible(game.board, game.bar, game.turn, game.dice) &&
           !IsDiceEmpty(game.dice) && !CheckWinner(game);
}

int TurnMoveCheck(int mvRat, pawn_move move, game game) {
    if ((mvRat == NOT_POSSIBLE_MOVE) ||
        (move.type != FINISH_SIGN && (move.final < 0 || move.final >= FIELDS)) ||
        (game.board.fields[move.initial].color != game.turn && move.type != INIT_BAR_SIGN && move.type != FINISH_SIGN)) {
        printf("move is not possible\n");
        return 0;
    }

    return 1;
}

void MakeMove(game* game, int mvRat, pawn_move move) {
    if (mvRat == ATTACK_MOVE)
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

void PrintNoMoveInfo(game game) {
    if (!IsAnyMovePossible(game.board, game.bar, game.turn, game.dice))
        printf("No possible moves\n");
}

void PlayTurn(game* game) {
    Dices(game);

    while (TurnCondition(*game)) {
        PrintBoard(game->board, game->bar, game->finish);
        pawn_move forcedMove = IsThereForcedMove(game->board, game->bar,game->turn, game->moveSize, game->dice);

        PrintForced(forcedMove);

        pawn_move move = MoveMenu(*game);
        int mvRat = -1;

        if (move.type != FINISH_SIGN)
            mvRat = CheckIsMovePossible(game->board.fields[move.final], game->turn);

        if (!TurnMoveCheck(mvRat, move, *game) || !CheckForced(move, forcedMove, *game))
            continue;

        MakeMove(game, mvRat, move);
        WriteToFile(game->file, moveToString(move));
    }

    PrintNoMoveInfo(*game);
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

    if (move.type == FINISH_SIGN) {
        RemoveDiceIfFinish(board, dice, color, move);
    } else {
        RemoveDiceIfNotFinish(dice, moveSize);
    }
}

void addBoardToSave(game game, char* txt, int* ctr) {
    for (int i = 0; i < FIELDS; i++) {
        int pawns = game.board.fields[i].pawnsCounter;

        txt[(*ctr)++] = pawns == 0 ? '0' : pawns + 'A' - 1;
    }
    for (int i = 0; i < FIELDS; i++) {
        int clr = game.board.fields[i].color;
        txt[(*ctr)++] = clr == 'N' ? '0' : clr;
    }
}

char* buildGameSave(game game) {
    char* txt = malloc(SAVE_SIZE * sizeof(char));
    int ctr = 0;

    txt[ctr++] = game.currentRound + '0';
    txt[ctr++] = game.rounds + '0';
    txt[ctr++] = game.score[0] + '0';
    txt[ctr++] = game.score[1] + '0';
    addBoardToSave(game, txt, &ctr);
    txt[ctr++] = game.bar.white_pawns.pawnsCounter + '0';
    txt[ctr++] = game.bar.red_pawns.pawnsCounter + '0';
    txt[ctr++] = game.finish.white_pawns.pawnsCounter + '0';
    txt[ctr++] = game.finish.red_pawns.pawnsCounter + '0';
    txt[ctr++] = game.turn;
    txt[ctr] = '\0';

    return txt;
}

void SaveGame(game game) {
    printf("enter file name: ");
    char name[50];
    scanf("%s", name);
    FILE * file = fopen(name, "w");
    if (file == NULL) {
        printf("save error");
        exit(0);
    }
    fwrite(buildGameSave(game), sizeof(char), SAVE_SIZE, file);
    fclose(file);
}

void ChooseSaveOrPlayTurn(game* game) {
    char comm = 1;
    printf("(Q) - Quit, (S) - Save Game and Quit, (P) - Play turn\n");

    while (comm != 'Q' && comm != 'S' && comm != 'P') {
        scanf("%c", &comm);
    }

    if (comm == 'Q')
        exit(0);
    if (comm == 'S') {
        SaveGame(*game);
        exit(0);
    }
    if (comm == 'P') {
        printf("%s's turn, roll the dice...\n", colorString(game->turn));
        PlayTurn(game);
    }
}

void PlayRound(game* game) {
    while (!CheckWinner(*game)) {
        ChooseSaveOrPlayTurn(game);
        if (!CheckWinner(*game))
            ChangeTurn(game);
    }
    PrintBoard(game->board, game->bar, game->finish);
    int pts = 1;
    if (!IsBarEmpty(game->bar, ReversedColor(game->turn))) {
        pts = 3;
    } else if (PawnsOnFinish(game->finish, ReversedColor(game->turn))) {
        pts = 2;
    }

    game->score[game->turn==RED] += pts;

    printf("Round no. %d Winner: %s\n", game->currentRound,colorString(game->turn));
}

void LoadSWCFromFile(struct SECTION_WITH_COUNTER * swc, char fill[2]) {
    InitFieldWIthData(&swc->white_pawns, WHITE, fill[0] - '0');
    InitFieldWIthData(&swc->red_pawns, RED, fill[1] - '0');
}

void BindFromFileToVars(game* game, FILE* file, char round[2], char score[2], char board_pattern[25],
                        char board_color[25], char bar[2], char finish[2], char* color) {
    fread(round, sizeof(char), 2, file);
    fread(score, sizeof(char), 2, file);
    fread(board_pattern, sizeof(char), 24, file);
    board_pattern[24] = '\0';
    fread(board_color, sizeof(char), 24, file);
    board_color[24] = '\0';
    fread(bar, sizeof(char), 2, file);
    fread(finish, sizeof(char), 2, file);
    fread(color, sizeof(char), 1, file);
    InitBoard(&game->board, board_pattern, board_color);
}

void LoadGame(game* game) {
    char name[50];
    printf("Enter file name:");
    scanf("%s", name);

    FILE * file = fopen(name, "rb");

    if (file == NULL) {
        printf("File not found.");
        exit(0);
    }
    char round[2], score[2], board_pattern[25], board_color[25], bar[2], finish[2], color;
    BindFromFileToVars(game, file, round, score, board_pattern, board_color, bar, finish, &color);
    game->currentRound = round[0] - '0';
    game->rounds = round[1] - '0';
    game->score[0] = score[0] - '0';
    game->score[1] = score[1] - '0';
    game->turn = color;
    LoadSWCFromFile(&game->bar, bar);
    LoadSWCFromFile(&game->finish, finish);
    fclose(file);
}

void LoadOrNewGame(game* game) {
    char command = 0;

    printf("(N) - New Game, (L) - Load Game");

    while (command != 'N' && command != 'L') {
        scanf("%c", &command);
    }

    if (command == 'N') {
        int empty[] = {0,0};
        SetUpGame(game, 0, 0, empty);
        game->currentRound++;
        StartRound(game, empty, empty);
    } else {
        LoadGame(game);
    }
}

void InitGame(game* game) {
    game->file = fopen(SAVE_NAME, "w");
    fclose(game->file);
    int r = 0;
    int empty[2] = {0,0};
    LoadOrNewGame(game);
    while (game->currentRound <= game->rounds) {
        if (r != 0) {
            SetUpFields(game, 0, 0, empty);
            StartRound(game, empty, empty);
            RoundIntro(*game);
        }
        PlayRound(game);
        printf("Score: WHITE - %d, RED - %d\n", game->score[0], game->score[1]);
        game->currentRound++;
        r++;
        //WriteToFile(game->file, "NEXT ROUND\n");
    }

    if (game->score[0] > game->score[1])
        printf("RED WIN");
    else if (game->score[0] < game->score[1])
        printf("RED WIN");
    else
        printf("DRAW");
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
        if (dice[i] == 0 && ctr > i)
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
        moveSize[2] = dice[0] + dice[1];
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

void StartRound(game* game, int bar[2], int finish[2]) {
    InitBoard(&game->board, BOARD_PATTERN, PAWN_COLORS);
    InitFieldWIthData(&game->bar.white_pawns, WHITE, bar[0]);
    InitFieldWIthData(&game->bar.red_pawns, RED, bar[1]);
    InitFieldWIthData(&game->finish.white_pawns, WHITE, finish[0]);
    InitFieldWIthData(&game->finish.red_pawns, RED, finish[1]);

    game->turn = NEUTRAL;
    SetFirstTurn(game);
}

int CheckWinner(game game) {
    field playerBar = game.turn == RED ? game.bar.red_pawns : game.bar.white_pawns;

    if (playerBar.pawnsCounter)
        return 0;

    if (CountPawnsOnBoard(game.board, game.turn, FIELDS))
        return 0;

    return 1;
}