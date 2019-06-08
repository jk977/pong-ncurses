#include "render.h"
#include "config.h"
#include "util.h"

#include <stddef.h>
#include <curses.h>
#include <string.h>

#define BALL_CHAR        '*'
#define WALL_CHAR_HDASH  '-'
#define WALL_CHAR_HSOLID '_'
#define WALL_CHAR_VDASH  ':'
#define WALL_CHAR_VSOLID '|'
#define WALL_CHAR_EMPTY  ' '

#define SCORE_WALL_PREFIX   "Wall: "
#define SCORE_P1_PREFIX     "P1: "
#define SCORE_P2_PREFIX     "P2: "

static chtype get_wall_char(struct wall* w) {
    switch (w->style) {
    case DASHED:
        if (w->dir == HORIZONTAL) {
            return WALL_CHAR_HDASH;
        } else {
            return WALL_CHAR_VDASH;
        }
    case SOLID:
        if (w->dir == HORIZONTAL) {
            return WALL_CHAR_HSOLID;
        } else {
            return WALL_CHAR_VSOLID;
        }
    default:
        return WALL_CHAR_EMPTY;
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

    struct wall equiv = paddle_to_wall(p);
    render_wall(&equiv);
}

void render_ball(struct board* b) {
    mvaddch(b->ball.pos.y, b->ball.pos.x, BALL_CHAR);
}

static void render_paddles(struct board* b) {
    for (size_t i = 0; i < PONG_PLAYER_MAX; ++i) {
        render_paddle(b->players[i]);
    }
}

static void render_walls(struct board* b) {
    for (size_t i = 0; i < b->wall_count; ++i) {
        render_wall(b->walls[i]);
    }
}

static void print_score(char const* prefix, int score, int x_center) {
    if (score < 0) {
        return;
    }

    char msg[strlen(prefix) + 5];
    sprintf(msg, "%s%d", prefix, score);

    int const x      = x_center - strlen(msg) / 2;
    int const y_text = PONG_OUTER_BUFFER / 2;

    mvprintw(y_text, x, msg);
}

void render_score(struct board* b) {
    struct vector bounds = get_max_bounds();

    if (b->p1_score < 0) {
        int const center = bounds.x / 2;
        print_score(SCORE_WALL_PREFIX, b->p2_score, center);
    } else {
        int const p1_center  = bounds.x / 4;
        int const p2_center = 3*bounds.x / 4;
        print_score(SCORE_P1_PREFIX, b->p1_score, p1_center);
        print_score(SCORE_P2_PREFIX, b->p2_score, p2_center);
    }
}

void render_board(struct board* b) {
    /*
     * order of drawing is important here.
     * ball is done last to ensure it's always on top.
     */

    clear();
    render_walls(b);
    render_paddles(b);
    render_score(b);
    render_ball(b);
}
