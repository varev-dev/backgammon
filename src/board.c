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
    printf("%c %d", color == RED ? 'R' : 'W',
           color == RED ? swc.red_pawns.pawnsCounter : swc.white_pawns.pawnsCounter);
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

int IsAnyMovePossible(board board, bar bar, char color, int dice[DICE_AMOUNT]) {
    if (!IsBarEmpty(bar, color))
        return IsMoveFromBarPossible(board, color, dice);

    for (int i = 0; i < DICE_AMOUNT; i++) {
        if (IsMoveWithSizeFromBoardPossible(board, color, dice[i]) ||
            IsMoveToFinishPossible(board, color, dice[i]))
            return 1;
    }

    return 0;
}

void ClosestPossibleAttack(board board, char color, int moveSize[MAX_DICES], pawn_move* move) {
    int fieldId, init, mult = color == RED ? -1 : 1;

    init = fieldId = color == RED ? FIELDS - 1 : 0;

    while (fieldId < FIELDS && fieldId >= 0) {
        if (board.fields[fieldId].color == color)
            continue;

        for (int i = 0; i < MAX_DICES; i++) {
            int finalField = fieldId + moveSize[i];

            if (finalField >= FIELDS || finalField < 0)
                continue;

            if (CheckIsMovePossible(board.fields[finalField], color) == ATTACK &&
                init + mult * (finalField) < init + mult * (move->final)) {
                move->initial = fieldId;
                move->final = finalField;
            }
        }

        moveSize > 0 ? fieldId++ : fieldId--;
    }
}

pawn_move ForcedAttack(board board, char color, int moveSize[MAX_DICES]) {
    pawn_move move;
    move.type = NOT_SET;
    move.final = FIELDS;
    int mult = color == RED ? -1 : 1;

    for (int i = 0; i < MAX_DICES; i++) {
        moveSize[i] *= mult;
    }

    ClosestPossibleAttack(board, color, moveSize, &move);

    if (move.final != FIELDS)
        move.type = ATTACK_SIGN;

    return move;
}

pawn_move IsThereForcedMove(board board, bar bar, finish finish, char color, int moveSize[MAX_DICES]) {
    pawn_move forcedMove;
    forcedMove.type = NOT_SET;

    if (!IsBarEmpty(bar, color)) {
        forcedMove.type = INIT_BAR_SIGN;
        return forcedMove;
    }

    forcedMove = ForcedAttack(board, color, moveSize);
    if (forcedMove.type == ATTACK_SIGN)
        return forcedMove;

    return forcedMove;
}

void MovePawnOnBoard(board* board, char color, int initialField, int finalField) {
    RemovePawn(&board->fields[initialField]);
    AppendPawn(&board->fields[finalField], color, 1);
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

void BeatPawn(board* board, bar* bar, char color, int fieldId) {
    int beatenPawns = board->fields[fieldId].pawnsCounter;
    char opponentColor = color == RED ? WHITE : RED;
    field * opponentBar = color == RED ? &bar->white_pawns : &bar->red_pawns;

    AppendPawn(opponentBar, opponentColor, beatenPawns);
}