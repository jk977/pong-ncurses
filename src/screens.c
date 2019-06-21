#include "screens.h"
#include "util.h"
#include "config.h"

#include <curses.h>
#include <signal.h>
#include <string.h>

static bool get_confirmation(void) {
    chtype c = getch();
    return c == 'y' || c == 'Y';
}

static int print_centered(char* msg, int y_offset) {
    /*
     * print message at specified y-coordinate, centered horizontally
     */

    int y_center = get_max_bounds().y / 2;
    int x_start  = center_in_window(strlen(msg));
    return mvaddstr(y_center + y_offset, x_start, msg);
}

static int screen_mode(bool* is_multiplayer) {
    // first user prompt (mode)
    TRY_FN( clear() );
    TRY_FN( print_centered("Enable multiplayer? [y/N]", -2) );

    *is_multiplayer = get_confirmation();
    return OK;
}

static int screen_difficulty(unsigned int* refresh_rate) {
    TRY_FN( clear() );
    TRY_FN( print_centered("Enable hard mode? [y/N]", -2) );

    if (get_confirmation()) {
        *refresh_rate = PONG_HARD_HZ;
    } else {
        *refresh_rate = PONG_EASY_HZ;
    }

    return OK;
}

static int screen_controls(void) {
    TRY_FN( clear() );
    TRY_FN( print_centered("Controls:", -2) );

    TRY_FN( print_centered("Player 1 moves up/down with w/s keys.", -1) );
    TRY_FN( print_centered("Player 2 moves up/down with the i/k keys.", 0) );
    TRY_FN( print_centered("Pause with Enter.", 1) );

    TRY_FN( print_centered("Don't let the ball get past your paddle!", 3) );
    TRY_FN( getch() );

    return OK;
}

int screen_start(unsigned int* refresh_rate, bool* is_multiplayer) {
    /*
     * ask user for some game settings, then display general controls
     *
     * note: function takes a pointer to refresh_rate instead of
     *       explicitly manipulating PONG_REFRESH_RATE intentionally,
     *       to minimize side effects.
     */

    TRY_FN( nodelay(stdscr, FALSE) );

    TRY_FN( screen_mode(is_multiplayer) );
    TRY_FN( screen_difficulty(refresh_rate) );
    TRY_FN( screen_controls() );

    TRY_FN( nodelay(stdscr, TRUE) );

    return OK;
}

int screen_pause(void) {
    /*
     * simple pause menu with several options
     */

    TRY_FN( nodelay(stdscr, FALSE) );

    TRY_FN( clear() );

    TRY_FN( print_centered("Make a selection:", -3) );
    TRY_FN( print_centered("c - controls", -2) );
    TRY_FN( print_centered("r - restart", -1) );
    TRY_FN( print_centered("enter - continue", 0) );

    bool invalid_option = true;

    while (invalid_option) {
        switch (getch()) {
        case 'c':
            TRY_FN( screen_controls() );
            invalid_option = false;
            break;
        case 'r':
            kill(0, PONG_RESTART_SIG);
            invalid_option = false;
            break;
        case '\n':
        case '\r':
            invalid_option = false;
            break;
        default:
            break;
        }
    }

    TRY_FN( nodelay(stdscr, TRUE) );
    return OK;
}

int screen_win(char const* player) {
    /*
     * displays win screen for specified player
     */

    char* suffix = " wins!";

    char msg[strlen(player) + strlen(suffix) + 1];
    sprintf(msg, "%s%s", player, suffix);

    TRY_FN( clear() );
    TRY_FN( print_centered(msg, 0) );

    TRY_FN( nodelay(stdscr, FALSE) );
    TRY_FN( getch() );
    TRY_FN( nodelay(stdscr, TRUE) );

    return OK;
}
