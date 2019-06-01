#include <assert.h>

#include "intersection.h"
#include "ratio.h"

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

struct intersection intersection_between(struct line l1, struct line l2)  {
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

