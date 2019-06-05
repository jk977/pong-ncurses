#include "physics/vector.h"

bool vector_equals(struct vector v1, struct vector v2) {
    return v1.x == v2.x
        && v1.y == v2.y;
}

struct vector vector_add(struct vector v1, struct vector v2) {
    return (struct vector) { v1.x + v2.x, v1.y + v2.y };
}

struct vector vector_subtract(struct vector v1, struct vector v2) {
    return vector_add(v1, vector_multiply(v2, -1));
}

struct vector vector_multiply(struct vector v, distance_t n) {
    /*
     * scalar multiplication of a vector by a whole number
     */

    return (struct vector) { n * v.x, n * v.y };
}

struct vector vector_multiply_ratio(struct vector v, struct ratio r) {
    /*
     * scalar multiplication of a vector by a rational number.
     *
     * the resulting vector is truncated if the vector would have a decimal
     * component, and no checks for division by zero occur.
     */

    return (struct vector) { r.num * v.x / r.denom, r.num * v.y / r.denom };
}

distance_t vector_cross(struct vector v1, struct vector v2) {
    /*
     * 2D cross product of vectors. mathematically, this returns the
     * determinant of the 2x2 matrix formed by using v1 and v2 as row vectors.
     */

    return v1.x * v2.y - v1.y * v2.x;
}
