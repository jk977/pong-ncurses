#include <stdlib.h>

#include <locale.h>
#include <curses.h>
#include <signal.h>

void setup_curses(int argc, char** argv) {
    /*
     * initializes and configures curses window
     */

    (void) argc;
    (void) argv;

    initscr();
    cbreak();
    noecho();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
}

void cleanup(void) {
    /*
     * tidying function invoked via atexit()
     */

    endwin();
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "en_US.utf-8");
    setup_curses(argc, argv);
    atexit(cleanup);

    addstr("hello, world!");

    refresh();
    getch();
    return 0;
}
