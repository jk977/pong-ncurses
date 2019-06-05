#ifndef COLLISION_H_
#define COLLISION_H_

#include "physics/vector.h"
#include "objects.h"

#ifdef __cplusplus
extern "C" {
#endif

struct collision {
    // no collision, top/bottom collision, left/right, or corner collision
    enum { COLLISION_NONE, COLLISION_TB, COLLISION_LR, COLLISION_CORNER }
    type;

    struct vector point; // point of collision
};

struct collision collision_between(struct ball* ball, struct wall* w);

#ifdef __cplusplus
}
#endif

#endif // COLLISION_H_
