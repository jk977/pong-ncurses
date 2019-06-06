#ifndef BOARD_H_
#define BOARD_H_

#include <stdbool.h>

#include "objects.h"

#ifdef __cplusplus
extern "C" {
#endif

struct board {
    struct paddle* players[PONG_PLAYER_MAX];
    struct ball ball;

    struct wall** walls; // used as a dynamic array of pointers
    size_t wall_count;
};

struct board* board_init(bool is_multiplayer);
void board_destroy(struct board* b);

int board_add_player(struct board* b, struct vector pos);
int board_add_wall(struct board* b, struct wall w);

int board_move_paddle(struct board* b, unsigned int player, int y);

#ifdef __cplusplus
}
#endif

#endif // BOARD_H_
