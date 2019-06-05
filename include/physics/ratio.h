/*
 * ratio.h: Ratio of two whole numbers used in physics calculations.
 */

#ifndef RATIO_H_
#define RATIO_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ratio {
    int num;
    int denom;
};

void ratio_simplify(struct ratio* r);
bool ratio_equals(struct ratio r1, struct ratio r2);

#ifdef __cplusplus
}
#endif

#endif // RATIO_H_
