#include "game.h"
#include "util.h"

#include <stdlib.h>

#include <curses.h>
#include <locale.h>
#include <unistd.h>

#include <sys/ioctl.h>

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

int main(void) {
    if (sanity_check() == ERR) {
        return EXIT_FAILURE;
    }

    if (game_run() == ERR) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}
