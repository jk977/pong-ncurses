#ifndef GAME_H_
#define GAME_H_

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

int game_run(void);
void game_restart(int signal);

extern struct board* main_board;

#ifdef __cplusplus
}
#endif

#endif // GAME_H_
