#ifndef LINE_H_
#define LINE_H_

#include <stdbool.h>

#include "vector.h"
#include "ratio.h"

#ifdef __cplusplus
extern "C" {
#endif

struct line {
    struct vector start;
    struct vector end;
};

struct line line_between(struct vector v1, struct vector v2);
bool line_is_vertical(struct line l);
bool line_is_horizontal(struct line l);
struct ratio line_slope(struct line l);

#ifdef __cplusplus
}
#endif

#endif // LINE_H_
