#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "objects/board.h"

#ifdef __cplusplus
extern "C" {
#endif

// which player scored
enum score_status { P1_SCORE, P2_SCORE, NO_SCORE };

enum score_status get_score_status(struct board* b);

#ifdef __cplusplus
}
#endif

#endif // PHYSICS_H_
