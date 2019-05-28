#ifndef WALL_H_
#define WALL_H_

#include <stdbool.h>

#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

enum direction { HORIZONTAL, VERTICAL };
enum wallstyle { DASHED, SOLID };

struct wall {
    struct vector pos;
    int length;
    bool tangible;

    enum direction dir;
    enum wallstyle style;
};

#ifdef __cplusplus
}
#endif

#endif // WALL_H_
