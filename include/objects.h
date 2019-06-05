/*
 * objects.h: Definitions for pong game objects and related functions.
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <stdbool.h>

#include "physics/vector.h"
#include "physics/line.h"

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ball {
    struct vector pos;          // ball position
    struct vector velocity;
    int           multiplier;   // multiplier for ball speed
};

struct paddle {
    struct vector pos;
    int height;
};

struct wall {
    struct vector pos;
    int length;
    bool tangible;

    enum { HORIZONTAL, VERTICAL }
    dir;

    enum { INVISIBLE, DASHED, SOLID }
    style;
};

struct board {
    struct paddle* players[PONG_PLAYER_MAX];
    struct ball ball;

    struct wall** walls;    // used as a dynamic array of pointers
    size_t wall_count;
};

struct vector ball_project(struct ball* ball);

struct line wall_to_line(struct wall* w);

struct board* board_init(bool is_multiplayer);
void board_destroy(struct board* b);

int board_add_player(struct board* b, struct vector pos);
int board_add_wall(struct board* b, struct wall w);

#ifdef __cplusplus
}
#endif

#endif // OBJECTS_H_
