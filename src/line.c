#include "line.h"

struct line line_between(struct vector v1, struct vector v2) {
    /*
     * create line segment between v1 and v2, ordered by x-coordinate,
     * then by y-coordinate. in other words, the result is guaranteed
     * that .start.x <= .end.x and .start.y <= .end.y
     */

    bool v1_first = (v1.x < v2.x) || (v1.x == v2.x && v1.y < v2.y);

    if (v1_first) {
        return (struct line) { v1, v2 };
    } else {
        return (struct line) { v2, v1 };
    }
}

struct ratio line_slope(struct line l) {
    struct ratio slope = {
        l.end.y - l.start.y,
        l.end.x - l.start.x
    };

    ratio_simplify(&slope);
    return slope;
}
