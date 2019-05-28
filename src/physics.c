#include <stddef.h>

#include "physics.h"
#include "vector.h"

enum collision get_collision(struct ball* ball, struct wall* w) {
    struct vector next_coord = {
        ball->pos.x + ball->velocity.x * ball->multiplier,
        ball->pos.y + ball->velocity.y * ball->multiplier
    };

    (void) next_coord;
    (void) w;

    return NONE;
}

enum score_status get_score_status(struct board* b) {
    enum score_status status = NO_SCORE;

    distance_t x_ball = b->ball.pos.x;
    distance_t x_p1   = b->players[0]->pos.x;

    if (x_ball <= x_p1) {
        // ball is behind P1; P2 scores
        status = P2_SCORE;
    } else if (b->players[1] != NULL) {
        distance_t x_p2 = b->players[1]->pos.x;

        if (x_ball >= x_p2) {
            // ball is behind P2; P1 scores
            status = P1_SCORE;
        }
    }

    return status;
}
