#ifndef BALL_H_
#define BALL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "vector.h"

struct ball {
    struct vector pos;          // ball position
    struct vector velocity;
    int           multiplier;   // multiplier for ball speed
};

#ifdef __cplusplus
}
#endif

#endif // BALL_H_
