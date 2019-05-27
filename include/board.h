#ifndef BOARD_H_
#define BOARD_H_

#include <stdbool.h>

#include "ball.h"
#include "paddle.h"
#include "wall.h"

struct board {
    struct paddle paddles[2];
    struct wall walls[4];
    struct ball ball;
};

struct board* board_init(bool is_multiplayer);
void board_destroy(struct board* b);

int board_add_player(struct board* b, struct vector pos);
int board_add_wall(struct board* b, struct vector pos, int height);

void board_update(struct board* b);

#endif // BOARD_H_
