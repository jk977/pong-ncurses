#include "input.h"
#include "pause.h"

#include <signal.h>

#define INPUT_ESCAPE 27

int board_handle_input(struct board* b) {
    switch (getch()) {
    case INPUT_P1_UP:
        return board_move_paddle(b, 0, -1);
    case INPUT_P1_DOWN:
        return board_move_paddle(b, 0, 1);
    case INPUT_P2_UP:
        return board_move_paddle(b, 1, -1);
    case INPUT_P2_DOWN:
        return board_move_paddle(b, 1, 1);
    case KEY_ENTER:
        return pause_menu();
    case ERR:
        return ERR;
    default:
        return OK;
    }
}
