#ifndef BACKGAMMON_BOARD_H
#define BACKGAMMON_BOARD_H

#include <stdio.h>

#include "stack.h"
#include "field.h"

#define BOARD_PATTERN "200005030005500030500002"

#define PLAYERS 2
#define PAWNS_PER_TEAM 15
#define FIELDS 24
#define HOME_FIELDS 6
#define OUTER_FIELDS 12;

typedef struct SECTION_WITH_COUNTER {
    stack red_pawns;
    stack white_pawns;
} counted_section;

typedef struct BOARD {
    field fields[FIELDS];
} board;

void InitBoard(board* board, pawn pawns[PLAYERS][PAWNS_PER_TEAM]);
void PrintBoard(board* board);

#endif //BACKGAMMON_BOARD_H
