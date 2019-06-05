#include "update.h"
#include "physics/collision.h"

void update_board(struct board* b) {
    /*
     * calculates the next actual position of ball on the board using its
     * projected position and whether or not it collides with a wall.
     *
     * note that the current algorithm is fine for a ball that can only
     * move to adjacent coordinates, or in a traditional board that doesn't
     * have layered walls, but less reliable for balls that move 2+ units
     * horizontally and/or vertically because it doesn't take the collision
     * of the closest wall.
     */

    struct vector projected_pos = ball_project(&b->ball);
    struct collision coll = { COLLISION_NONE, {0,0} };
    size_t i = 0;

    // iterate through each wall and stop if a collision is found
    while (i < b->wall_count && coll.type == COLLISION_NONE) {
        struct wall* w = b->walls[i];
        ++i;

        if (w == NULL) {
            continue;
        }

        coll = collision_between(&b->ball, w);
    }

    struct vector actual_pos = projected_pos;

    distance_t x_delta = projected_pos.x - coll.point.x;
    distance_t y_delta = projected_pos.y - coll.point.y;

    switch (coll.type) {
    case COLLISION_NONE:
        actual_pos = projected_pos;
        break;

    case COLLISION_TB:
        b->ball.velocity.y *= -1;
        actual_pos.x = coll.point.x + x_delta;
        actual_pos.y = projected_pos.y;
        break;

    case COLLISION_LR:
        b->ball.velocity.x *= -1;
        actual_pos.x = projected_pos.x;
        actual_pos.y = coll.point.y + y_delta;
        break;

    case COLLISION_CORNER:
        b->ball.velocity.x *= -1;
        b->ball.velocity.y *= -1;
        actual_pos.x = coll.point.x + x_delta;
        actual_pos.y = coll.point.y + y_delta;
        break;
    }

    b->ball.pos = actual_pos;
}
