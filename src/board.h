#ifndef BACKGAMMON_BOARD_H
#define BACKGAMMON_BOARD_H

#include <stdio.h>
#include <malloc.h>

#include "field.h"

#define BOARD_PATTERN "200005030005500030500002"
#define PAWN_COLORS "W0000R0R000WR000W0W0000R"

#define FIELDS 24
#define HOME_FIELDS 6

typedef struct BOARD {
    field fields[24];
} board;

typedef struct SECTION_WITH_COUNTER {
    field red_pawns;
    field white_pawns;
} bar, finish;

void InitBoard(board* board);
void PrintBoard(board board);
void PrintSectionWithCounter(struct SECTION_WITH_COUNTER sectionWithCounter, char* name);
int CountPawnsOnBoard(board board, char color, int category);
int IsBarEmpty(bar bar, char color);
int ReversedFieldId(int id);

#endif //BACKGAMMON_BOARD_H