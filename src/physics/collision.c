#include "physics/collision.h"
#include "physics/line.h"
#include "physics/ratio.h"

#include <assert.h>
#include <stdbool.h>

struct intersection {
    bool intersects;     // whether or not there's an intersection
    struct vector point; // the point of intersection, if any
};

static inline struct intersection get_collinear_intersect(struct line l1, struct line l2) {
    /*
     * finds intersection of two collinear line segments.
     *
     * note: because the lines are formed with line_between, there is only 1
     *       representation of a line in the coordinate system. this reduces
     *       the number of checks needed to tell if two lines overlap or
     *       intersect.
     */

    struct intersection result = { .intersects = true, .point = {0,0} };

    if (vector_equals(l1.start, l2.end)) {
        result.point = l1.start;
    } else if (vector_equals(l1.end, l2.start)) {
        result.point = l1.end;
    } else {
        // lines don't intersect, or they overlap,
        // which isn't counted as an intersection
        result.intersects = false;
    }

    return result;
}

static inline struct intersection get_angled_intersect(
        struct vector p, struct vector r, struct ratio t,
        struct vector q, struct vector s, struct ratio u) {
    /*
     * get point of intersection of two lines described as (p,p+r) and (q,q+s) with
     * solution at p+tr = q+us, where t and u are rational numbers.
     *
     * note: function signature is longer than necessary because of the debug
     *       section of code below.
     */

    struct intersection result = { .intersects = true, .point = {0,0} };

    ratio_simplify(&t);
    ratio_simplify(&u);

    // at this point, denominators are guaranteed to not be 0, so
    // approximate values of t and u are calculated to check if
    // they're in the range [0,1] since if they are, l1 and l2 intersect.
    float t_approx = (float) t.num / t.denom;
    float u_approx = (float) u.num / u.denom;

    if (t_approx < 0 || t_approx > 1 || u_approx < 0 || u_approx > 1) {
        // line segments would intersect if they were infinite, but they end
        // before the point of intersection.
        result.intersects = false;
    } else {
        struct vector rt = vector_multiply_ratio(r, t);
        result.point = vector_add(p, rt);

#ifdef DEBUG
        // because intersections can only happen on points in ℤ²,
        // the result should be the same using p+tr or q+us.
        // this test is to verify that it's the same, though.

        // if the ball's velocity is something that doesn't play nice with
        // the gridlike nature of the terminal, the test might fail.
        // the default {1,1} should work fine.

        struct vector su = vector_multiply_ratio(s, u);
        assert( vector_equals(result.point, vector_add(q, su)) );
#else
        (void) q;
        (void) s;
#endif
    }

    return result;
}

static inline struct intersection intersection_between(struct line l1, struct line l2)  {
    /*
     * find point of intersection between two line segments, if any exists.
     *
     * based on the following mathematical description, with adjustments made
     * due to the fact that the coordinate system is ℤ² instead of ℝ²:
     * https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect#565282
     */

    struct intersection result = { .intersects = true, .point = {0,0} };

    // this function is primarily just running calculations for the solution.
    // see the linked SO answer for diagrams showing p, q, etc.
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
        struct ratio t = { q_sub_p_x_s, r_x_s };
        struct ratio u = { q_sub_p_x_r, r_x_s };
        result = get_angled_intersect(p, r, t, q, s, u);
    }

    return result;
}

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

    switch (w->dir) {
    case HORIZONTAL:
        result.type = COLLISION_TB;
        break;
    case VERTICAL:
        result.type = COLLISION_LR;
        break;
    }

    // TODO handle corner collisions

    return result;
}

