/*
 * line.h: Definition and functions for line segment used in physics calculations.
 */

#ifndef LINE_H_
#define LINE_H_

#include <stdbool.h>

#include "physics/vector.h"
#include "physics/ratio.h"

#ifdef __cplusplus
extern "C" {
#endif

struct line {
    struct vector start;
    struct vector end;
};

struct line line_between(struct vector v1, struct vector v2);
struct ratio line_slope(struct line l);

#ifdef __cplusplus
}
#endif

#endif // LINE_H_
