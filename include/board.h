#ifndef BOARD_H_
#define BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "ball.h"
#include "paddle.h"
#include "wall.h"

#include "config.h"

struct board {
    struct paddle* players[PONG_PLAYER_MAX];
    struct ball ball;

    struct wall** walls;    // used as a dynamic array of pointers
    size_t wall_count;
};

struct board* board_init(bool is_multiplayer);
void board_destroy(struct board* b);

int board_add_player(struct board* b, struct vector pos);
int board_add_wall(struct board* b, struct wall w);

void board_update(struct board* b);

#ifdef __cplusplus
}
#endif

#endif // BOARD_H_
