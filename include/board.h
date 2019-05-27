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

#endif // BOARD_H_
