#include <stdio.h>
#include <stdlib.h>

#include <curses.h>
#include <locale.h>
#include <unistd.h>

#include <sys/ioctl.h>

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
        init_pair(PONG_COLOR_PAIR, PONG_FG, PONG_BG);
        bkgd(COLOR_PAIR(PONG_COLOR_PAIR));
    }

    cbreak();       // game must have inputs immediately available
    noecho();       // games shouldn't display inputs (in most cases)
    curs_set(0);    // make cursor invisible when moving it around screen

    scrollok(stdscr, FALSE);
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
}

void sanity_check(void) {
    /*
     * make sure everything is ok before starting main program.
     * mainly for ensuring window isn't too small for game.
     */

    int x;
    int y;
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        // if ioctl fails, something's seriously wrong, so don't continue
        perror("sanity_check");
        exit(EXIT_FAILURE);
    }

    x = ws.ws_col;
    y = ws.ws_row;

    TRACE("Found window dimensions: %dx%d\n", x, y);

    if (x < PONG_REQUIRED_X || y < PONG_REQUIRED_Y) {
        ERROR("sanity_check: Window size is too small "
              "(minimum required dimensions: %dx%d).\n",
              PONG_REQUIRED_X,
              PONG_REQUIRED_Y);
        exit(EXIT_FAILURE);
    }
}

struct vector get_max_bounds(WINDOW* win) {
    int x;
    int y;
    getmaxyx(win, y, x);

    return (struct vector) { x, y };
}

void* find_first_null(void* ptr, size_t count) {
    /*
     * get first null pointer in array of pointers.
     *
     * note: like with zero_pointers(), ptr must be an array
     *       of pointers (void**).
     *
     * returns the address of the first null pointer, or null if all pointers
     * are nonnull.
     */

    void** ary_ptrs = (void**) ptr;

    for (size_t i = 0; i < count; ++i) {
        if (ary_ptrs[i] == NULL) {
            return &ary_ptrs[i];
        }
    }

    return NULL;
}

