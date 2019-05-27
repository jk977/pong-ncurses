#ifndef BALL_H_
#define BALL_H_

#include <curses.h>

#include "vector.h"

struct ball_state {
    chtype        chr;          // character to use as ball
    struct vector pos;          // ball position
    struct vector velocity;
    int           multiplier;   // multiplier for ball speed
};

#endif // BALL_H_
