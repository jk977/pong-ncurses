#ifndef COLLISION_H_
#define COLLISION_H_

#include "vector.h"
#include "ball.h"
#include "wall.h"

#ifdef __cplusplus
extern "C" {
#endif

// bitfield flags for types of collision.
// NONE:    no collision occurred
// TB:      collision occurred on top/bottom face
// LR:      collision occurred on left/right face
// CORNER:  collision occurred on a corner (i.e., both TB and LR)
#define COLLISION_NONE      0
#define COLLISION_TB        (1 << 1)
#define COLLISION_LR        (1 << 2)
#define COLLISION_CORNER    (COLLISION_TB | COLLISION_LR)

// type for collision bitfield
typedef unsigned char coltype_t;

struct collision {
    coltype_t type;      // bitwise OR of all collision flags describing its type
    struct vector point; // point of collision
};

struct collision collision_between(struct ball* ball, struct wall* w);

#ifdef __cplusplus
}
#endif

#endif // COLLISION_H_
