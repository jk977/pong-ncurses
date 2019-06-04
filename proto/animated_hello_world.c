#include <stdlib.h>

#include <locale.h>
#include <curses.h>
#include <unistd.h>

#include "util.h"
#include "config.h"

void setup_curses(void) {
    /*
     * initializes and configures curses window.
     */

    setlocale(LC_ALL, "en_US.utf-8");
    initscr();

    if (has_colors()) {
        start_color();
        init_pair(PONG_MAIN_COLOR, PONG_MAIN_FG, PONG_MAIN_BG);
        bkgd(COLOR_PAIR(PONG_MAIN_COLOR));
    }

    cbreak();       // game must have inputs immediately available
    noecho();       // games shouldn't display inputs (in most cases)
    curs_set(0);    // make cursor invisible when moving it around screen

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

void update_screen(void) {
    /*
     * function to test stateful displaying of text
     */

    static unsigned int i = 0;
    clear();

    if (i % 2 == 0) {
        addstr("hello, world!");
    } else {
        addstr("¡hola, mundo!");
    }

    ++i;
    refresh();
}

int main(void) {
    setup_curses();
    atexit(cleanup);

    while (1) {
        update_screen();
        sleep(1);
    }

    return EXIT_SUCCESS;
}
