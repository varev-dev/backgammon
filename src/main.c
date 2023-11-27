#include <ncurses.h>

#include "board.h"

int main() {
    board board1;
    InitBoard(&board1);
//    printf("%d %c", board1.fields[23].pawns_counter, board1.fields[23].color);
    PrintBoard(board1);
    return 0;
}