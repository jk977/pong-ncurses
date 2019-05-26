#include <stdlib.h>

#include <locale.h>
#include <curses.h>

#include "config.h"

void setup_curses() {
    /*
     * initializes and configures curses window.
     */

    setlocale(LC_ALL, "en_US.utf-8");
    initscr();

    if (has_colors()) {
        start_color();
        init_pair(PONG_COLOR_PAIR, PONG_FG, PONG_BG);
        bkgd(COLOR_PAIR(PONG_COLOR_PAIR));
    }

    cbreak();
    noecho();

    scrollok(stdscr, FALSE);
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
}

void cleanup(void) {
    /*
     * tidying function invoked via atexit()
     */

    endwin();
}

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    setup_curses(argc, argv);
    atexit(cleanup);

    addstr("hello, world!");

    refresh();
    getch();
    return 0;
}
