/*
 * render.h: Interface for rendering game objects.
 */

#ifndef RENDER_H_
#define RENDER_H_

#include "objects.h"
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

int render_wall(struct wall* w);
int render_paddle(struct paddle* p);
int render_ball(struct board* b);
int render_score(struct board* b);
int render_board(struct board* b);

#ifdef __cplusplus
}
#endif

#endif // RENDER_H_
