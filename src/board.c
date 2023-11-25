#include "board.h"

void AddPawnsToField(field* field, pawn pawn[15], int amount) {
    int i = 0;
    while (i < amount) {
        AppendPawn(field, &pawn[i]);
        i++;
    }
}

void InitFields(board* board, pawn pawns[PLAYERS][PAWNS_PER_TEAM]) {
    for (int i = 1; i <= FIELDS; i++) {
        InitField(&board->fields[i-1]);

        int amount = (int) (BOARD_PATTERN[i-1] - '0');
        char color = NEUTRAL_FIELD;

        if (i == 1 || i == 12 || i == 17 || i == 19)
            color = WHITE_PAWN;
        if (i == 24 || i == 13 || i == 8 || i == 6)
            color = RED_PAWN;

        if (amount && color != NEUTRAL_FIELD)
            AddPawnsToField(&board->fields[i-1], pawns[color == RED_PAWN], amount);
    }
}

void InitBoard(board* board, pawn pawns[PLAYERS][PAWNS_PER_TEAM]) {
    InitFields(board, pawns);
}

void PrintBoard(board* board) {
    for (int i = 0; i < FIELDS; ++i) {
        printf("%d. ", i + 1);
        char field_color = FieldColor(board->fields[i]);
        int pawns_on_field = GetNumberOfPawnsOnField(board->fields[i]);

        for (int j = 0; j < pawns_on_field; j++) {
            printf("%c ", field_color);
        }
        printf("\n");
    }
}