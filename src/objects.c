#include "objects.h"

struct vector ball_project(struct ball* ball) {
    /*
     * get where the ball would be next tick without any collisions.
     */

    return (struct vector) {
        ball->pos.x + ball->velocity.x * ball->multiplier,
        ball->pos.y + ball->velocity.y * ball->multiplier
    };
}

struct wall paddle_to_wall(struct paddle* p) {
    return (struct wall) {
        .pos      = p->pos,
        .length   = p->height,
        .tangible = true,
        .dir      = VERTICAL,
        .style    = SOLID
    };
}

struct line wall_to_line(struct wall* w) {
    /*
     * get line equivalent of specified wall.
     */

    struct vector wall_end = w->pos;

    switch (w->dir) {
    case HORIZONTAL:
        wall_end.x += w->length - 1;
        break;
    case VERTICAL:
        wall_end.y += w->length - 1;
        break;
    }

    return line_between(w->pos, wall_end);
}
