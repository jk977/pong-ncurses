#ifndef PHYSICS_H_
#define PHYSICS_H_

#ifdef __cplusplus
extern "C" {
#endif

enum collision {
    VERTICAL,   // collision on top/bottom face
    HORIZONTAL, // collision on left/right face
    CORNER,     // collision on both horizontal and vertical faces
    NONE        // no collision
};

// which player scored
enum score_status { P1, P2, NONE };

enum collision get_collision(struct ball b, struct wall w);
enum score_status get_score_status(struct board* b);

#ifdef __cplusplus
}
#endif

#endif // PHYSICS_H_
