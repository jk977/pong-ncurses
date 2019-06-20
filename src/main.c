#define _DEFAULT_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h>
#include <locale.h>
#include <signal.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include "board.h"
#include "menu.h"

#include "input.h"
#include "render.h"
#include "update.h"

#include "config.h"
#include "util.h"

unsigned int PONG_REFRESH_RATE = PONG_EASY_HZ;
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

bool get_confirmation(void) {
    chtype c = getch();
    return c == 'y' || c == 'Y';
}

int print_centered(char* msg, int y) {
    /*
     * print message at specified y-coordinate, centered horizontally
     */

    int x_start  = center_in_window(strlen(msg));
    return mvaddstr(y, x_start, msg);
}

int run_menu(unsigned int* refresh_rate, bool* is_multiplayer) {
    /*
     * ask user for some game settings.
     *
     * note: this could definitely be accomplished more elegantly,
     *       but it's a little thrown together due to time constraints.
     *
     * note 2: function takes a pointer to refresh_rate instead of
     *         explicitly manipulating PONG_REFRESH_RATE intentionally,
     *         to minimize side effects.
     */

    struct vector bounds = get_max_bounds();

    if (bounds.x == -1) {
        return ERR;
    }

    int y_center = bounds.y / 2;

    // let getch() block
    TRY_FN( nodelay(stdscr, FALSE) );

    // first user prompt (mode)
    TRY_FN( clear() );
    TRY_FN( print_centered("Enable multiplayer? [y/N]", y_center-2) );

    *is_multiplayer = get_confirmation();

    // second user prompt (difficulty)
    TRY_FN( clear() );
    TRY_FN( print_centered("Enable hard mode? [y/N]", y_center-2) );

    if (get_confirmation()) {
        *refresh_rate = PONG_HARD_HZ;
    } else {
        *refresh_rate = PONG_EASY_HZ;
    }

    // third user prompt (controls)
    TRY_FN( clear() );
    TRY_FN( print_centered("Controls:", y_center-2) );

    if (*is_multiplayer) {
        TRY_FN( print_centered("Player 1 (left) moves up/down with W/S.", y_center-1) );
        TRY_FN( print_centered("Player 2 (right) moves up/down with I/K.", y_center) );
    } else {
        TRY_FN( print_centered("Move the paddle up/down with W/S.", y_center-1) );
    }

    TRY_FN( print_centered("Don't let the ball get past your paddle!", y_center+2) );
    TRY_FN( getch() );
    TRY_FN( clear() );

    TRY_FN( nodelay(stdscr, TRUE) );

    return OK;
}

int run_game(void) {
    /*
     * runs the starting menu and main game loop
     */

    bool is_multiplayer;
    TRY_FN( run_menu(&PONG_REFRESH_RATE, &is_multiplayer) );

    if (main_board != NULL) {
        // reset board if it already exists
        board_destroy(main_board);
    } 

    main_board = board_init(is_multiplayer);

    if (main_board == NULL) {
        ERROR("Failed to initialize board.");
        return ERR;
    }

    while (true) {
        board_handle_input(main_board);

        TRY_FN( update_board(main_board) );
        TRY_FN( render_board(main_board) );
        TRY_FN( refresh() );

        usleep(period_from_freq(PONG_REFRESH_RATE));
    }

    return OK;
}

void restart_game(int signal) {
    (void) signal;
    bool is_multiplayer = false;

    run_menu(&PONG_REFRESH_RATE, &is_multiplayer);
    main_board = board_init(is_multiplayer);
}

int main(void) {
    if (sanity_check() == ERR) {
        return EXIT_FAILURE;
    }

    setup_curses();
    atexit(cleanup);

    // when SIGUSR1 received, block SIGINTs and restart game
    struct sigaction act = {0};
    act.sa_handler = restart_game;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);

    if (sigaction(SIGUSR1, &act, NULL) == -1) {
        perror("main");
        return EXIT_FAILURE;
    }

    if (run_game() == ERR) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}
