#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "ball.h"
#include "wall.h"
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

enum collision {
    TB,     // collision on top/bottom face
    LR,     // collision on left/right face
    CORNER, // collision on both horizontal and vertical faces
    NO_COLLISION
};

// which player scored
enum score_status { P1_SCORE, P2_SCORE, NO_SCORE };

enum collision get_collision(struct ball* ball, struct wall* w);
enum score_status get_score_status(struct board* b);

#ifdef __cplusplus
}
#endif

#endif // PHYSICS_H_
