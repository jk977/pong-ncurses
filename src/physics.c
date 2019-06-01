#include <stddef.h>
#include <assert.h>

#include "physics.h"
#include "vector.h"
#include "ratio.h"
#include "line.h"

struct intersection {
    bool intersects;     // whether or not there's an intersection
    struct vector point; // the point of intersection, if any
};

static inline struct intersection get_collinear_intersect(struct line l1, struct line l2) {
    struct intersection result = { .intersects = true, .point = {0,0} };

    if (vector_equals(l1.start, l2.start) || vector_equals(l1.start, l2.end)) {
        result.point = l1.start;
    } else if (vector_equals(l1.end, l2.start) || vector_equals(l1.end, l2.end)) {
        result.point = l1.end;
    } else {
        result.intersects = false;
    }

    return result;
}

static inline struct intersection get_intersect(struct line l1, struct line l2)  {
    /*
     * find point of intersection between two line segments, if any exists.
     *
     * based on the following mathematical description:
     * https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect#565282
     */

    struct intersection result = { .intersects = true, .point = {0,0} };

    struct vector p = l1.start;
    struct vector q = l2.start;
    struct vector r = vector_subtract(l1.end, l1.start);
    struct vector s = vector_subtract(l2.end, l2.start);

    struct vector q_sub_p = vector_subtract(q, p);

    distance_t r_x_s       = vector_cross(r, s);
    distance_t q_sub_p_x_s = vector_cross(q_sub_p, s);
    distance_t q_sub_p_x_r = vector_cross(q_sub_p, r);

    if (r_x_s == 0) {
        if (q_sub_p_x_r == 0) {
            result = get_collinear_intersect(l1, l2);
        } else {
            result.intersects = false;
        }
    } else {
        struct ratio t = {
            q_sub_p_x_s,
            r_x_s
        };

        struct ratio u = {
            q_sub_p_x_r,
            r_x_s
        };

        ratio_simplify(&t);
        ratio_simplify(&u);

        double t_approx = (double) t.num / t.denom;
        double u_approx = (double) u.num / u.denom;

        if (t_approx < 0 || t_approx > 1 || u_approx < 0 || u_approx > 1) {
            result.intersects = false;
        } else {
            struct vector rt = vector_multiply_ratio(r, t);
            result.point = vector_add(p, rt);

#ifdef DEBUG
            struct vector su = vector_multiply_ratio(s, u);
            assert( vector_equals(result.point, vector_add(q, su)) );
#endif
        }
    }

    return result;
}

enum collision get_collision(struct ball* ball, struct wall* w) {
    struct vector next_coord = {
        ball->pos.x + ball->velocity.x * ball->multiplier,
        ball->pos.y + ball->velocity.y * ball->multiplier
    };

    struct vector wall_end = w->pos;

    switch (w->dir) {
    case HORIZONTAL:
        wall_end.x += w->length;
        break;
    case VERTICAL:
        wall_end.y += w->length;
        break;
    }

    struct line trajectory = line_between(ball->pos, next_coord);
    struct line wall = line_between(w->pos, wall_end);
    struct intersection result = get_intersect(trajectory, wall);

    if (!result.intersects) {
        return NO_COLLISION;
    }

    // TODO finish this
    return TB;
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
