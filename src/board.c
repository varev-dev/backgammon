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
        int pawns_on_field = board.fields[i].pawns_counter;

        for (int j = 0; j < pawns_on_field; j++) {
            printf("%c ", field_color);
        }

        printf("\n");
    }
}

int ReversedFieldId(int id) {
    return 25 - id;
}

int CheckIfAllPawnsInHome(board board, bar bar, char color) {
    if (color == RED_FIELD ? bar.red_pawns.pawns_counter > 0 : bar.white_pawns.pawns_counter > 0)
        return PIECES_NOT_IN_HOME;

    for (int i = 0; i < FIELDS; i++) {
        if (board.fields[i].color == color &&
            color == RED_FIELD ? i >= HOME_FIELDS : ReversedFieldId(i) >= HOME_FIELDS) {
            return PIECES_NOT_IN_HOME;
        }
    }

    return PIECES_IN_HOME;
}