#ifndef PADDLE_H_
#define PADDLE_H_

#include "physics/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

struct paddle {
    struct vector pos;
    int height;
};

#ifdef __cplusplus
}
#endif

#endif // PADDLE_H_
