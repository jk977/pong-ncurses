#ifndef WALL_H_
#define WALL_H_

#include <stdbool.h>

#include "vector.h"

enum direction { HORIZONTAL, VERTICAL };
enum wallstyle { DASHED, SOLID };

struct wall {
    struct vector pos;
    int length;
    bool tangible;

    enum direction dir;
    enum wallstyle style;
};

#endif // WALL_H_
