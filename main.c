#include <ncurses.h>

int main() {
    initscr();
    printw("123");
    refresh();
    getch();
    endwin();
    return 0;
}
