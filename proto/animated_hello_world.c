#include <stdlib.h>

#include <locale.h>
#include <curses.h>
#include <unistd.h>

#include "config.h"

void setup_curses(void) {
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

    return 0;
}
