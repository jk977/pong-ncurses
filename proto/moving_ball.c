#define _DEFAULT_SOURCE

#ifdef DEBUG
#   include <stdio.h>
#   include <string.h>
#endif

#include <stdlib.h>

#include <curses.h>
#include <unistd.h>

#include "ball.h"
#include "config.h"
#include "util.h"

#define REFRESH_RATE 30 /* Hz */

static struct ball ball = {
    .chr = '*',
    .pos = {0,0},
    .velocity = {1,1},
    .multiplier = 1
};

void cleanup(void) {
    /*
     * tidying function invoked via atexit()
     */

    endwin();
}

void update_screen(void) {
    /*
     * function to test animations
     */

    int x_next = ball.pos.x;
    int y_next = ball.pos.y;

    int x_max;
    int y_max;
    getmaxyx(stdscr, y_max, x_max);

    // remove old ball location
    mvdelch(ball.pos.y, ball.pos.x);

    // velocity treated as position delta for each unit of time
    x_next += ball.velocity.x;
    y_next += ball.velocity.y;

    // wrap around if edge is reached
    x_next %= x_max;
    y_next %= y_max;

    if (mvaddch(y_next, x_next, ball.chr) == ERR
            && (x_next >= x_max)
            && (y_next >= y_max)) {
        // note: mvaddch returns ERR if char is written to bottom-right,
        //       even if write was successful.
        TRACE("update_screen: Failed to move ball to (%d,%d)\n", x_next, y_next);
    } else {
        // no error, so update ball position
        ball.pos.x = x_next;
        ball.pos.y = y_next;
    }

#ifdef DEBUG
    // display useful info for debugging
    static char current[25];
    static char max[25];

    sprintf(current, "Current: (%d,%d)", ball.pos.x, ball.pos.y);
    sprintf(max, "Bound: (%d,%d)", x_max, y_max);

    mvaddstr(0, 0, current);
    mvaddstr(1, 0, max);

    move(ball.pos.y, ball.pos.x);
#endif

    refresh();
}

void sleep_from_freq(unsigned int hz) {
    /*
     * find the period of a <hz> Hz refresh rate, then sleep for that period.
     *
     * note: math is based on the relation f=1/T, where the frequency f is in Hz
     *       and the period T is in seconds.
     */

    int const usec_per_sec = 1000000;
    useconds_t period = 1.0/hz * usec_per_sec;
    usleep(period);
}

void loop_sleep(void) {
    /*
     * step frame-by-frame if STEP is defined, otherwise sleep for the amount of
     * time specified by REFRESH_RATE
     */

#ifdef STEP
    getch();
#else
    sleep_from_freq(REFRESH_RATE);
#endif
}

int main(void) {
    setup_curses();
    atexit(cleanup);

    while (1) {
        update_screen();
        loop_sleep();
    }

    return EXIT_SUCCESS;
}
