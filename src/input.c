#include "input.h"

void handle_input(struct board* b) {
    switch(getch()) {
    case INPUT_P1_UP:
        board_move_paddle(b, 0, -1);
        break;
    case INPUT_P1_DOWN:
        board_move_paddle(b, 0, 1);
        break;
    case INPUT_P2_UP:
        board_move_paddle(b, 1, -1);
        break;
    case INPUT_P2_DOWN:
        board_move_paddle(b, 1, 1);
        break;
    default:
        break;
    }
}

