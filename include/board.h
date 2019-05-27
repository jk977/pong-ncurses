#ifndef BOARD_H_
#define BOARD_H_

#include "ball.h"
#include "paddle.h"
#include "wall.h"

struct board {
    struct paddle paddles[2];
    struct ball ball;

    struct wall* walls;
    unsigned int wall_count;
};

struct board* board_init(void);
void board_destroy(struct board* b);

int board_add_player(struct board* b, struct vector pos);
int board_add_wall(struct board* b, struct vector pos, int height);

void board_update(struct board* b);

#endif // BOARD_H_
