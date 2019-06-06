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
     * horizontally and/or vertically because it isn't guaranteed to take the
     * collision of the closest wall.
     */

    struct vector projected_pos = ball_project(&b->ball);
    struct collision coll       = { COLLISION_NONE, {0,0} };
    coltype_t coltype           = COLLISION_NONE;

    // iterate through each wall and find type of collision
    for (size_t i = 0; i < b->wall_count; ++i) {
        struct wall* w = b->walls[i];

        if (w == NULL) {
            continue;
        }

        // accumulate the overall collision type in coltype using bitfields
        coll = collision_between(&b->ball, w);
        coltype |= coll.type;
    }

    // do the same for each paddle
    for (size_t i = 0; i < PONG_PLAYER_MAX; ++i) {
        struct paddle* p = b->players[i];

        if (p == NULL) {
            continue;
        }

        struct wall equiv = paddle_to_wall(p);
        coll = collision_between(&b->ball, &equiv);
        coltype |= coll.type;
    }

    struct vector actual_pos = projected_pos;

    // difference between projected position and point of collision is
    // used to find where the ball will actually go
    distance_t x_delta = projected_pos.x - coll.point.x;
    distance_t y_delta = projected_pos.y - coll.point.y;

    switch (coltype) {
    case COLLISION_NONE:
        break;

    case COLLISION_TB:
        b->ball.velocity.y *= -1;
        actual_pos.x = coll.point.x + x_delta;
        break;

    case COLLISION_LR:
        b->ball.velocity.x *= -1;
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
