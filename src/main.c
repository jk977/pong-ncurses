#include <stdlib.h>
#include <curses.h>

#include "board.h"
#include "render.h"
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
    render_board(b);
    refresh();
    getch();

    return 0;
}
