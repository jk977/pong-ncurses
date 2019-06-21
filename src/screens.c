#include "screens.h"
#include "util.h"
#include "config.h"

#include <string.h>
#include <curses.h>

static bool get_confirmation(void) {
    chtype c = getch();
    return c == 'y' || c == 'Y';
}

static int print_centered(char* msg, int y) {
    /*
     * print message at specified y-coordinate, centered horizontally
     */

    int x_start  = center_in_window(strlen(msg));
    return mvaddstr(y, x_start, msg);
}

int screen_start(unsigned int* refresh_rate, bool* is_multiplayer) {
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
int screen_pause(void) {
    return OK;
}
