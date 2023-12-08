//#include <ncurses.h>
#include <time.h>

#include "game.h"

int main() {
    srand(time(NULL));
    game game1;
    InitGame(&game1);
    return 0;
}