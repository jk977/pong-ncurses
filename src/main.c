#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdbool.h>

#include <curses.h>
#include <locale.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include "board.h"

#include "render.h"
#include "update.h"
#include "input.h"

#include "config.h"
#include "util.h"

unsigned int PONG_REFRESH_RATE = 15;
struct board* main_board = NULL;

int sanity_check(void) {
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
        return ERR;
    }

    x = ws.ws_col;
    y = ws.ws_row;

    TRACE("Found window dimensions: %dx%d\n", x, y);

    if (x < PONG_REQUIRED_X || y < PONG_REQUIRED_Y) {
        ERROR("Window size is too small "
              "(minimum required dimensions: %dx%d).\n",
              PONG_REQUIRED_X,
              PONG_REQUIRED_Y);
        return ERR;
    }

    return OK;
}

int setup_curses(void) {
    setlocale(LC_ALL, "en_US.utf-8");

    if (initscr() == NULL) {
        return ERR;
    }

    if (has_colors()) {
        TRY_FN( start_color() );
        TRY_FN( init_pair(PONG_MAIN_COLOR, PONG_MAIN_FG, PONG_MAIN_BG) );
        TRY_FN( bkgd(COLOR_PAIR(PONG_MAIN_COLOR)) );
    }

    TRY_FN( cbreak() );       // game must have inputs immediately available
    TRY_FN( noecho() );       // games shouldn't display inputs (in most cases)
    TRY_FN( curs_set(0) );    // make cursor invisible when moving it around screen

    TRY_FN( nodelay(stdscr, TRUE) );    // don't block on getch()
    TRY_FN( scrollok(stdscr, FALSE) );
    TRY_FN( intrflush(stdscr, FALSE) );
    TRY_FN( keypad(stdscr, TRUE) );

    return OK;
}

void cleanup(void) {
    endwin();
    board_destroy(main_board);
}

useconds_t period_from_freq(unsigned int hz) {
    /*
     * convert frequency (Hz) to period (μs)
     *
     * note: math is based on the relation f=1/T, where the frequency f is in Hz
     *       and the period T is in seconds.
     */

    int const usec_per_sec = 1000000;
    return 1.0/hz * usec_per_sec;
}

int run_game(bool is_multiplayer) {
    main_board = board_init(is_multiplayer);

    if (main_board == NULL) {
        ERROR("Failed to initialize board.");
        return ERR;
    }

    useconds_t const sleep_length = period_from_freq(PONG_REFRESH_RATE);

    while (true) {
        handle_input(main_board);
        update_board(main_board);

        TRY_FN( render_board(main_board) );
        TRY_FN( refresh() );

        usleep(sleep_length);
    }

    return OK;
}

int main(void) {
    if (sanity_check() == ERR) {
        return EXIT_FAILURE;
    }

    setup_curses();
    atexit(cleanup);

    return run_game(true);
}
