#ifndef WALL_H_
#define WALL_H_

#include <stdbool.h>

#include "vector.h"

struct wall {
    struct vector pos;
    int height;
    bool tangible;
};

#endif // WALL_H_
