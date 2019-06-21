/*
 * game.h: The core logic of the game. All other files except main.c are for
 *         use by this unit.
 */

#ifndef GAME_H_
#define GAME_H_

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

int game_run(void);

extern struct board* main_board;

#ifdef __cplusplus
}
#endif

#endif // GAME_H_
