#include "board.h"

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

void PrintBoard(board board) {
    for (int i = 0; i < FIELDS; i++) {
        int clockWiseBoardPrintCurrentIndex = i;

        if (i < 12)
            clockWiseBoardPrintCurrentIndex = 11 - i;

        char field_color = board.fields[clockWiseBoardPrintCurrentIndex].color;
        int pawns_on_field = board.fields[clockWiseBoardPrintCurrentIndex].pawnsCounter;
        printf("%c%d ", field_color, pawns_on_field);

        if ((i + 1) % 12 == 0)
            printf("\n");
    }
}

void PrintSectionWithCounter(struct SECTION_WITH_COUNTER swc, char* name) {
    printf("%s - WHITE: %d, RED: %d\n", name, swc.white_pawns.pawnsCounter, swc.red_pawns.pawnsCounter);
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