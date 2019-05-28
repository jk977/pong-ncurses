#ifndef RENDER_H_
#define RENDER_H_

#include "ball.h"
#include "board.h"
#include "paddle.h"
#include "wall.h"

#ifdef __cplusplus
extern "C" {
#endif

void render_wall(struct wall* w);
void render_paddle(struct paddle* p);
void render_ball(struct ball* ball);
void render_board(struct board* b);

#ifdef __cplusplus
}
#endif

#endif // RENDER_H_
