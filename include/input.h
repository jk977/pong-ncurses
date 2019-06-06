/*
 * input.h: Input handler for game.
 */

#ifndef INPUT_H_
#define INPUT_H_

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INPUT_P1_UP     'w'
#define INPUT_P1_DOWN   's'

#define INPUT_P2_UP     'i'
#define INPUT_P2_DOWN   'k'

void handle_input(struct board* b);

#ifdef __cplusplus
}
#endif

#endif // INPUT_H_
