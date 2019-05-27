#include <stdlib.h>
#include <curses.h>

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

    return 0;
}
