#include "board.h"

void InitFields(board* board) {
    for (int i = 0; i < FIELDS; i++) {
        InitField(&board->fields[i]);

        int amount = (int) (BOARD_PATTERN[i] - '0');
        char color = PAWN_COLORS[i];

        if (!amount || !(color - '0'))
            continue;

        AppendPawn(&board->fields[i], color, amount);
    }
}

void InitBoard(board* board) {
    InitFields(board);
}

void PrintBoard(board board) {
    for (int i = 0; i < FIELDS; i++) {
        printf("%d. ", i + 1);
        char field_color = board.fields[i].color;
        int pawns_on_field = board.fields[i].pawnsCounter;

        for (int j = 0; j < pawns_on_field; j++) {
            printf("%c ", field_color);
        }

        printf("\n");
    }
}

void PrintBar(bar bar) {
    printf("BAR - WHITE: %d, RED: %d\n",
    bar.white_pawns.pawnsCounter, bar.red_pawns.pawnsCounter);
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