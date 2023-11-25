#include <ncurses.h>

#include "stack.h"
#include "board.h"
#include "game.h"
#include "pawn.h"

int main() {
    pawn pawns[PLAYERS][PAWNS_PER_TEAM];
    board board;

    InitField(&board.fields[0]);

    InitManyPawns(pawns[WHITE_PLAYER], WHITE_PAWN, PAWNS_PER_TEAM);
    InitManyPawns(pawns[RED_PLAYER], RED_PAWN, PAWNS_PER_TEAM);

    InitBoard(&board, pawns);
    PrintBoard(&board);

    return 0;
}