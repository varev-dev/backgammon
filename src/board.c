#include "board.h"

#define TOP 'T'
#define BOTTOM 'B'
#define BAR_TEXT "BAR"
#define END_TEXT "END"
#define BAR_TEXT_SIZE 3
#define BAR_INDENT 3

void InitFields(board* board, const char* pawns, const char* colors) {
    for (int i = 0; i < FIELDS; i++) {
        InitField(&board->fields[i]);

        int amount = (int) (pawns[i] - 'A' + 1);
        char color = colors[i];

        if (amount <= 0 || !(color - '0'))
            continue;

        AppendPawn(&board->fields[i], color, amount);
    }
}

void InitBoard(board* board) {
    InitFields(board, BOARD_PATTERN, PAWN_COLORS);
}

void PrintAdditional(struct SECTION_WITH_COUNTER swc, char color) {
    printf("%c %d", color, color == RED ? swc.red_pawns.pawnsCounter : swc.white_pawns.pawnsCounter);
}

void PrintBoardTags(char mode) {
    for (int i = mode == TOP ? 0 : 12; i < (mode == TOP ? 12 : 24); i++) {
        int clockWiseIndex = i + 1;

        if (i < 12)
            clockWiseIndex = 12 - i;

        if (i % 12 == 6)
            printf("%*s%*s", BAR_INDENT + BAR_TEXT_SIZE, mode == TOP ? BAR_TEXT : "", BAR_INDENT + 1, "");

        printf("%02d ", clockWiseIndex);
    }

    if (mode == TOP)
        printf("%*s", BAR_INDENT+BAR_TEXT_SIZE, END_TEXT);

    printf("\n");
}

void PrintBoard(board board, bar bar, finish finish) {
    PrintBoardTags(TOP);
    for (int i = 0; i < FIELDS; i++) {
        int clockWiseIndex = i;

        if (i < 12)
            clockWiseIndex = 11 - i;

        char field_color = board.fields[clockWiseIndex].color;
        int pawns_on_field = board.fields[clockWiseIndex].pawnsCounter;
        printf("%c%X ", field_color, pawns_on_field);

        if ((i + 1) % 12 == 0 || (i + 1) % 12 == 6) {
            printf("|| ");
            PrintAdditional((i + 1) % 12 == 0 ? finish : bar, i < 12 ? RED : WHITE);
            printf("%s", (i + 1) % 12 == 0 ? "\n" : " || ");
        }
    }
    PrintBoardTags(BOTTOM);
}

int ReversedFieldId(int id) {
    return FIELDS - id - 1;
}

char ReversedColor(char color) {
    return color == RED ? WHITE : RED;
}

int FieldIdByColor(int fieldId, char color) {
    return color == RED ? ReversedFieldId(fieldId) : fieldId;
}

int CountPawnsNotInHome(board board, char color) {
    return CountPawnsOnBoard(board, color, FIELDS) - CountPawnsOnBoard(board, color, HOME_FIELDS);
}

int CountPawnsOnBoard(board board, char color, int category) {
    int counter = 0;

    for (int i = 0; i < FIELDS; i++) {
        if (color != board.fields[i].color)
            continue;

        int limit = category == HOME_FIELDS ? HOME_FIELDS : FIELDS;

        if (color == RED ? (i < limit) : (ReversedFieldId(i) < limit))
            counter += board.fields[i].pawnsCounter;
    }

    return counter;
}

int IsBarEmpty(bar bar, char color) {
    return color == RED ? !bar.red_pawns.pawnsCounter : !bar.white_pawns.pawnsCounter;
}

int IsMoveFromBarPossible(board board, char color, int dice[DICE_AMOUNT]) {
    for (int i = 0; i < DICE_AMOUNT; i++) {
        int fieldId = FieldIdByColor(dice[i] - 1, color);
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

int IsAnyMoveOnBoardPossible(board board, char color, const int moveSize[MAX_DICES]) {
    for (int i = 0; i < FIELDS; i++) {
        if (board.fields[i].color != color)
            continue;

        for (int j = 0; j < MAX_DICES; j++) {
            if (moveSize[j] == 0)
                return 0;
            int final = i + (color == RED ? -1 : 1) * moveSize[j];
            if (final >= 0 && final < FIELDS && CheckIsMovePossible(board.fields[final], color))
                return 1;
        }
    }

    return 0;
}

int IsMoveToFinishPossible(board board, char color, int moveSize[MAX_DICES]) {
    int homePawns = CountPawnsOnBoard(board, color, HOME_FIELDS);

    if (CountPawnsNotInHome(board, color) > 0 || homePawns == 0)
        return 0;

    for (int i = 0; i < MAX_DICES; i++) {
        if (moveSize[i] == 0)
            return 0;

        if (CheckMoveToFinishWithSize(board, color, moveSize[i]))
            return 1;
    }

    return 0;
}

int MaxDiceValue(const int dice[MAX_DICES]) {
    int temp = 0;
    for (int i = 0; i < MAX_DICES; i++) {
        if (temp < dice[i])
            temp = dice[i];
    }
    return temp;
}

int ForcedFinishFieldId(board board, char color, int dice[MAX_DICES]) {
    int mult = color == RED ? -1 : 1, max = MaxDiceValue(dice);

    for (int i = HOME_FIELDS - 1; i >= 0; i--) {
        int fClr = FieldIdByColor(i, ReversedColor(color));
        int final = fClr + mult * max;
        if (board.fields[fClr].color != color)
            continue;
        if (final <= -1 || final >= FIELDS)
            return fClr;
    }
    return -1;
}

int CheckFinishMove(board board, char color, int fieldId, int dice[MAX_DICES]) {
    int homePawns = CountPawnsOnBoard(board, color, HOME_FIELDS);

    if (CountPawnsNotInHome(board, color) > 0 || homePawns == 0)
        return 0;

    if (CountPawnsNotInHome(board, color) || board.fields[fieldId].color != color)
        return 0;

    int mult = color == RED ? -1 : 1;

    for (int i = 0; i < MAX_DICES; i++) {
        int final = fieldId + mult * dice[i];
        if (final == -1 || final == FIELDS)
            return 1;
    }

    if (ForcedFinishFieldId(board, color, dice) == fieldId)
        return 1;

    return 0;
}

int CheckMoveToFinishWithSize(board board, char color, int moveSize) {
    int homePawns = CountPawnsOnBoard(board, color, HOME_FIELDS);

    if (CountPawnsNotInHome(board, color) > 0 || homePawns == 0)
        return 0;

    int multiplier = color == RED ? -1 : 1;

    for (int i = FIELDS - 1; i >= 0; i--) {
        int fieldId = FieldIdByColor(i, color);

        if (board.fields[fieldId].color != color)
            continue;

        if (fieldId + multiplier * moveSize < 0 || fieldId + multiplier * moveSize >= FIELDS)
            return 1;
    }

    return 0;
}

int IsAnyMovePossible(board board, bar bar, char color, int dice[DICE_AMOUNT]) {
    if (!IsBarEmpty(bar, color))
        return IsMoveFromBarPossible(board, color, dice);

    for (int i = 0; i < DICE_AMOUNT; i++) {
        if (dice[i] == 0)
            break;
        if (IsMoveWithSizeFromBoardPossible(board, color, dice[i]) ||
                CheckMoveToFinishWithSize(board, color, dice[i]))
            return 1;
    }

    return 0;
}

int IsBarInitAttackPossible(bar bar, board board, char color, int dice[MAX_DICES]) {
    if (IsBarEmpty(bar, color))
        return -1;

    for (int i = 0; i < MAX_DICES; i++) {
        if (dice[i] == 0)
            return -1;

        int fieldId = FieldIdByColor(dice[i] - 1, color);

        if (CheckIsMovePossible(board.fields[fieldId], color) == ATTACK_MOVE)
            return fieldId;
    }

    return -1;
}

void ClosestPossibleAttack(board board, char color, const int moveSize[MAX_DICES], pawn_move* move) {
    int id, init, mult = color == RED ? -1 : 1, inc = color == WHITE;

    init = id = FieldIdByColor(0, color);

    while (id < FIELDS && id >= 0) {
        if (board.fields[id].color != color) {
            inc ? id++ : id--;
            continue;
        }

        for (int i = 0; i < MAX_DICES; i++) {
            int final = id + moveSize[i] * mult;

            if (final >= FIELDS || final < 0)
                continue;

            if (CheckIsMovePossible(board.fields[final], color) == ATTACK_MOVE &&
                init + mult * final < init + mult * (move->final)) {
                move->initial = id;
                move->final = final;
            }
        }

        inc ? id++ : id--;
    }
}

void ForcedAttack(board board, char color, int moveSize[MAX_DICES], pawn_move* move) {
    move->final = FieldIdByColor(24, color);

    ClosestPossibleAttack(board, color, moveSize, move);

    if (move->final != FieldIdByColor(24, color))
        move->type = ATTACK_SIGN;
}

// to-do finish forced to last pawn on board if movesize > fields to finish
pawn_move IsThereForcedMove(board board, bar bar, char color, int moveSize[MAX_DICES], int dice[MAX_DICES]) {
    pawn_move forcedMove;
    forcedMove.type = NOT_SET;
    forcedMove.final = -1;

    int barInitAttack = IsBarInitAttackPossible(bar, board, color, dice);

    if (!IsBarEmpty(bar, color)) {
        forcedMove.type = INIT_BAR_SIGN;
        if (barInitAttack != -1)
            forcedMove.final = barInitAttack;
        return forcedMove;
    }

    ForcedAttack(board, color, moveSize, &forcedMove);

    if (forcedMove.type == ATTACK_SIGN)
        return forcedMove;

    return forcedMove;
}

void MovePawnOnBoard(board* board, pawn_move move) {
    RemovePawn(&board->fields[move.initial]);
    AppendPawn(&board->fields[move.final], move.color, 1);
}

void MovePawnFromBar(bar* bar, board* board, char color, int fieldId) {
    field * playerBar = color == RED ? &bar->red_pawns : &bar->white_pawns;

    RemovePawn(playerBar);
    AppendPawn(&board->fields[fieldId], color, 1);
}

void MovePawnToFinish(board* board, finish* finish, char color, int fieldId) {
    field* playerFinish = color == RED ? &finish->red_pawns : &finish->white_pawns;

    RemovePawn(&board->fields[fieldId]);
    AppendPawn(playerFinish, color, 1);
}

void BeatPawn(board* board, bar* bar, pawn_move move) {
    int beatenPawns = board->fields[move.final].pawnsCounter;
    board->fields[move.final].pawnsCounter = 0;
    char opponentColor = move.color == RED ? WHITE : RED;
    field * opponentBar = move.color == RED ? &bar->white_pawns : &bar->red_pawns;

    AppendPawn(opponentBar, opponentColor, beatenPawns);
}