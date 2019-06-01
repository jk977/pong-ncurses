#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include <stdbool.h>

#include "vector.h"
#include "line.h"

#ifdef __cplusplus
extern "C" {
#endif

struct intersection {
    bool intersects;     // whether or not there's an intersection
    struct vector point; // the point of intersection, if any
};

struct intersection intersection_between(struct line l1, struct line l2);

#ifdef __cplusplus
}
#endif

#endif // INTERSECTION_H_
