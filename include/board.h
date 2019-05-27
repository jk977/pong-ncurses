#ifndef BOARD_H_
#define BOARD_H_

#include <stdbool.h>

#include "ball.h"
#include "paddle.h"
#include "wall.h"

#define BOARD_PLAYER_MAX    2
#define BOARD_OUTER_BUFFER  5 /* rows/cols */

struct board {
    struct paddle* players[BOARD_PLAYER_MAX];
    struct ball ball;

    struct wall** walls;     // used as a dynamic array of pointers
    unsigned int wall_count;
};

struct board* board_init(bool is_multiplayer);
void board_destroy(struct board* b);

int board_add_player(struct board* b, struct vector pos);
int board_add_wall(struct board* b, struct wall w);

void board_update(struct board* b);

#endif // BOARD_H_
