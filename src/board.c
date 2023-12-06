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
        printf("%c%d ", field_color, pawns_on_field);

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