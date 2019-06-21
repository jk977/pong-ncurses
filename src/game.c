#include "game.h"

#include "input.h"
#include "render.h"
#include "screens.h"
#include "update.h"

#include "config.h"
#include "util.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h>
#include <locale.h>
#include <signal.h>
#include <unistd.h>

unsigned int PONG_REFRESH_RATE = PONG_EASY_HZ;
struct board* main_board = NULL;

static int setup_curses(void) {
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

static void cleanup(void) {
    endwin();
    board_destroy(main_board);
    main_board = NULL;
}

static useconds_t period_from_freq(unsigned int hz) {
    /*
     * convert frequency (Hz) to period (μs)
     *
     * note: math is based on the relation f=1/T, where the frequency f is in Hz
     *       and the period T is in seconds.
     */

    int const usec_per_sec = 1000000;
    return 1.0/hz * usec_per_sec;
}

static int handle_win_condition(struct board* b) {
    bool p1_wins = b->p1_score >= PONG_WIN_SCORE;
    bool p2_wins = b->p2_score >= PONG_WIN_SCORE;

    if (!p1_wins && !p2_wins) {
        return OK;
    }
    
    int status;

    if (p1_wins) {
        status = screen_win("P1");
    } else if (p2_wins) {
        if (b->p1_score < 0) {
            // single-player game
            status = screen_win("The wall");
        } else {
            // multiplayer game
            status = screen_win("P2");
        }
    }

    // game restarts on SIGUSR1
    kill(0, SIGUSR1);
    return status;
}

int game_run(void) {
    /*
     * runs the starting menu and main game loop
     */

    TRY_FN( setup_curses() );

    // when SIGUSR1 received, restart game
    struct sigaction act = {0};
    act.sa_handler = game_restart;

    if (sigaction(SIGUSR1, &act, NULL) == -1) {
        ERROR("Failed to install signal handler.");
        return ERR;
    }

    bool is_multiplayer;
    TRY_FN( screen_start(&PONG_REFRESH_RATE, &is_multiplayer) );

    if (main_board != NULL) {
        // reset board if it already exists
        board_destroy(main_board);
    } 

    main_board = board_init(is_multiplayer);
    atexit(cleanup);

    if (main_board == NULL) {
        ERROR("Failed to initialize board.");
        return ERR;
    }

    while (true) {
        board_handle_input(main_board);

        TRY_FN( update_board(main_board) );
        TRY_FN( render_board(main_board) );
        TRY_FN( refresh() );
        TRY_FN( handle_win_condition(main_board) );

        usleep(period_from_freq(PONG_REFRESH_RATE));
    }

    return OK;
}

void game_restart(int signal) {
    (void) signal;
    bool is_multiplayer = false;

    screen_start(&PONG_REFRESH_RATE, &is_multiplayer);
    main_board = board_init(is_multiplayer);
}

