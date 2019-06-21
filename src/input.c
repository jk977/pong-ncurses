#include "input.h"
#include "screens.h"

#include <signal.h>

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
    case '\n':
    case '\r':
        // enter key is pressed
        return screen_pause();
    case ERR:
        return ERR;
    default:
        return OK;
    }
}
