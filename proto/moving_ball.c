#define _DEFAULT_SOURCE

#ifdef DEBUG
#   include <stdio.h>
#endif

#include <stdlib.h>
#include <string.h>

#include <locale.h>
#include <curses.h>
#include <unistd.h>

#include "config.h"
#include "ball.h"
#include "util.h"

#define REFRESH_RATE 30 /* Hz */

static struct ball_state ball = {
    .x_pos = 0,
    .y_pos = 0,
    .velocity = {1,1}
};

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

    int x_next = ball.x_pos;
    int y_next = ball.y_pos;

    int x_max;
    int y_max;
    getmaxyx(stdscr, y_max, x_max);

    mvdelch(ball.y_pos, ball.x_pos);

    x_next += ball.velocity.x;
    y_next += ball.velocity.y;

    // wrap around if edge is reached
    x_next %= x_max;
    y_next %= y_max;

    if (mvaddch(y_next, x_next, ball_ch) == ERR
            && (x_next >= x_max)
            && (y_next >= y_max)) {
        // note: mvaddch returns ERR if char is written to bottom-right,
        //       even if write was successful.
        TRACE("update_screen: Failed to move ball to (%d,%d)\n", x_next, y_next);
    } else {
        // no error, so update ball position
        ball.x_pos = x_next;
        ball.y_pos = y_next;
    }

#ifdef DEBUG
    // display useful info for debugging
    static char current[25];
    static char max[25];

    sprintf(current, "Current: (%d,%d)", ball.x_pos, ball.y_pos);
    sprintf(max, "Bound: (%d,%d)", x_max, y_max);

    mvaddstr(0, 0, current);
    mvaddstr(1, 0, max);

    move(ball.y_pos, ball.x_pos);
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

    return 0;
}
