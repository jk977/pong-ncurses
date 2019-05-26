#ifndef BALL_H_
#define BALL_H_

#include <curses.h>

#include "vector.h"

chtype const ball_ch = '*';

struct ball_state {
    struct vector velocity;
    int x_pos;
    int y_pos;
};

#endif // BALL_H_
