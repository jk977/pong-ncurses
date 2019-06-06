#include "physics/collision.h"
#include "physics/line.h"
#include "physics/ratio.h"

#include <stdbool.h>

struct collision collision_between(struct ball* ball, struct wall* w) {
    /*
     * find point and type of collision between ball's projected
     * position after the next update and the specified wall.
     */

    struct collision const no_collision = { COLLISION_NONE, {0,0} };

    if (!w->tangible) {
        return no_collision;
    }

    struct vector next_coord = ball_project(ball);
    struct line trajectory   = line_between(ball->pos, next_coord);
    struct line wall         = wall_to_line(w);
    struct intersection i    = intersection_between(trajectory, wall);

    if (!i.intersects || vector_equals(i.point, ball->pos)) {
        // in addition to not colliding if the projection doesn't intersect,
        // no collision is counted if the point of collision is the ball's
        // current position. this prevents the ball from zigzagging into a
        // wall without bouncing away.
        return no_collision;
    }

    struct collision result = { COLLISION_NONE, i.point };
    bool at_corner;

    switch (w->dir) {
    case HORIZONTAL:
        // corner collisions occur if the ball collides with the corner and
        // approaches from the same side as the corner. 
        at_corner = (vector_equals(i.point, wall.start) && i.point.x > ball->pos.x)
            || (vector_equals(i.point, wall.end) && i.point.x < ball->pos.x);

        if (at_corner) {
            result.type = COLLISION_CORNER;
        } else {
            result.type = COLLISION_TB;
        }

        break;

    case VERTICAL:
        // same logic as the horizontal corner, but using y-coordinates
        at_corner = (vector_equals(i.point, wall.start) && i.point.y > ball->pos.y)
            || (vector_equals(i.point, wall.end) && i.point.y < ball->pos.y);

        if (at_corner) {
            result.type = COLLISION_CORNER;
        } else {
            result.type = COLLISION_LR;
        }

        break;
    }

    return result;
}
