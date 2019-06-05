#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdbool.h>

#include "physics/ratio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int distance_t;

struct vector {
    distance_t x;
    distance_t y;
};

bool vector_equals(struct vector v1, struct vector v2);

struct vector vector_add(struct vector v1, struct vector v2);
struct vector vector_subtract(struct vector v1, struct vector v2);
struct vector vector_multiply(struct vector v, distance_t n);
struct vector vector_multiply_ratio(struct vector v, struct ratio r);

distance_t vector_dot(struct vector v1, struct vector v2);
distance_t vector_cross(struct vector v1, struct vector v2);

#ifdef __cplusplus
}
#endif

#endif // VECTOR_H_
