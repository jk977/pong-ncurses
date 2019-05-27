#include <stdlib.h>
#include <curses.h>

#include "board.h"
#include "util.h"

void cleanup(void) {
    /*
     * tidying function invoked via atexit()
     */

    endwin();
}

int main(void) {
    sanity_check();
    setup_curses();
    atexit(cleanup);

    struct board* b = board_init(true);
    (void) b;

    return 0;
}
