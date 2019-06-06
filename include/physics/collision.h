/*
 * collision.h: Interface for collision detection between game objects.
 */

#ifndef COLLISION_H_
#define COLLISION_H_

#include "physics/vector.h"
#include "objects.h"

#ifdef __cplusplus
extern "C" {
#endif

// bitfields for types of collisions
#define COLLISION_NONE      0
#define COLLISION_TB        (1 << 0) /* top/bottom face collision */
#define COLLISION_LR        (1 << 1) /* left/right face collision */
#define COLLISION_CORNER    (COLLISION_TB | COLLISION_LR)

typedef unsigned char coltype_t;

struct collision {
    coltype_t type;
    struct vector point; // point of collision
};

struct collision collision_between(struct ball* ball, struct wall* w);

#ifdef __cplusplus
}
#endif

#endif // COLLISION_H_
