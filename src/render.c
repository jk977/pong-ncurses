#include <stddef.h>
#include <curses.h>

#include "render.h"
#include "config.h"

static chtype get_wall_char(struct wall* w) {
    switch (w->style) {
    case DASHED:
        if (w->dir == HORIZONTAL) {
            return '-';
        } else {
            return ':';
        }
    case SOLID:
        if (w->dir == HORIZONTAL) {
            return '_';
        } else {
            return '|';
        }
    default:
        return ' ';
    }
}

void render_wall(struct wall* w) {
    if (w == NULL) {
        return;
    }

    chtype chr = get_wall_char(w);

    for (int i = 0; i < w->length; ++i) {
        int x = w->pos.x;
        int y = w->pos.y;

        if (w->dir == HORIZONTAL) {
            x += i;
        } else {
            y += i;
        }

        mvaddch(y, x, chr);
    }
}

void render_paddle(struct paddle* p) {
    if (p == NULL) {
        return;
    }

    // treat paddles as a special type of wall
    struct wall equiv = {
        .pos      = p->pos,
        .length   = p->height,
        .tangible = true,
        .dir      = VERTICAL,
        .style    = SOLID
    };

    render_wall(&equiv);
}

void render_ball(struct ball* ball) {
    if (ball == NULL) {
        return;
    }

    mvaddch(ball->pos.y, ball->pos.x, ball->chr);
}

static void render_paddles(struct board* b) {
    for (size_t i = 0; i < PONG_PLAYER_MAX; ++i) {
        render_paddle(b->players[i]);
    }
}

static void render_walls(struct board* b) {
    for (size_t i = 0; i < b->wall_count; i++) {
        render_wall(b->walls[i]);
    }
}

void render_board(struct board* b) {
    /*
     * order of drawing is actually important here.
     * ball is done last to ensure it's always on top.
     */

    render_walls(b);
    render_paddles(b);
    render_ball(&b->ball);
}
