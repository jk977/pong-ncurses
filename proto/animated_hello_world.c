#include <stdlib.h>

#include <curses.h>
#include <unistd.h>

#include "util.h"

void cleanup(void) {
    /*
     * tidying function invoked via atexit()
     */

    endwin();
}

void update_screen(void) {
    /*
     * function to test stateful displaying of text
     */

    static unsigned int i = 0;
    clear();

    if (i % 2 == 0) {
        addstr("hello, world!");
    } else {
        addstr("¡hola, mundo!");
    }

    ++i;
    refresh();
}

int main(void) {
    setup_curses();
    atexit(cleanup);

    while (1) {
        update_screen();
        sleep(1);
    }

    return EXIT_SUCCESS;
}
