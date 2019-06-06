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
    struct vector pos; // top of paddle
    int height;
};

struct wall {
    struct vector pos; // top (or left, if horizontal) of wall
    int length;
    bool tangible;

    enum { HORIZONTAL, VERTICAL }
    dir;

    enum { INVISIBLE, DASHED, SOLID }
    style;
};

struct vector ball_project(struct ball* ball);
struct wall paddle_to_wall(struct paddle* p);
struct line wall_to_line(struct wall* w);

#ifdef __cplusplus
}
#endif

#endif // OBJECTS_H_
