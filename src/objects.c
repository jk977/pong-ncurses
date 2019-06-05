#include "objects.h"
#include "config.h"
#include "util.h"

#include <stdlib.h>

struct vector ball_project(struct ball* ball) {
    return (struct vector) {
        ball->pos.x + ball->velocity.x * ball->multiplier,
        ball->pos.y + ball->velocity.y * ball->multiplier
    };
}

struct line wall_to_line(struct wall* w) {
    struct vector wall_end = w->pos;

    switch (w->dir) {
    case HORIZONTAL:
        wall_end.x += w->length;
        break;
    case VERTICAL:
        wall_end.y += w->length;
        break;
    }

    return line_between(w->pos, wall_end);
}

static void zero_pointers(void* ptr, size_t const count) {
    /*
     * sets all pointers in array of pointers to NULL.
     * ptr MUST be of type void**, otherwise there will likely be a segfault.
     */

    void** ary_ptrs = (void**) ptr;

    for (size_t i = 0; i < count; ++i) {
        ary_ptrs[i] = NULL;
    }
}

static void setup_singleplayer(struct board* b) {
    /*
     * only set 1 paddle, and have a wall covering the entire left side.
     */

    struct vector bounds = get_max_bounds();
    int x_center = bounds.x / 2;
    int y_center = bounds.y / 2;

    b->ball = (struct ball) {
        .pos = {x_center, y_center},
        .velocity = {1, 1},
        .multiplier = 1
    };

    board_add_player(b, (struct vector) {bounds.x - 10, y_center - 1});

    // wall acting as perfect opponent
    board_add_wall(b, (struct wall) {
        .pos      = {10, PONG_OUTER_BUFFER},
        .length   = bounds.y - 2*PONG_OUTER_BUFFER,
        .tangible = false,
        .dir      = VERTICAL,
        .style    = SOLID
    });

    // upper playing field boundary
    board_add_wall(b, (struct wall) {
        .pos      = {PONG_OUTER_BUFFER, PONG_OUTER_BUFFER},
        .length   = bounds.x - 2*PONG_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });

    // lower playing field boundary
    board_add_wall(b, (struct wall) {
        .pos      = {PONG_OUTER_BUFFER, bounds.y - PONG_OUTER_BUFFER},
        .length   = bounds.x - 2*PONG_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });
}

static void setup_multiplayer(struct board* b) {
    /*
     * set 2 paddles, and only horizontal boundaries.
     */

    struct vector bounds = get_max_bounds();
    int x_center = bounds.x / 2;
    int y_center = bounds.y / 2;

    board_add_player(b, (struct vector) {10, y_center - 1});
    board_add_player(b, (struct vector) {bounds.x - 10, y_center - 1});

    b->ball = (struct ball) {
        .pos = {x_center, y_center},
        .velocity = {-1, 1},
        .multiplier = 1
    };

    // intangible center line dividing the players' sides
    board_add_wall(b, (struct wall) {
        .pos      = {x_center, PONG_OUTER_BUFFER},
        .length   = bounds.y - 2*PONG_OUTER_BUFFER,
        .tangible = false,
        .dir      = VERTICAL,
        .style    = DASHED
    });

    // upper playing field boundary
    board_add_wall(b, (struct wall) {
        .pos      = {PONG_OUTER_BUFFER, PONG_OUTER_BUFFER},
        .length   = bounds.x - 2*PONG_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });

    // lower playing field boundary
    board_add_wall(b, (struct wall) {
        .pos      = {PONG_OUTER_BUFFER, bounds.y - PONG_OUTER_BUFFER},
        .length   = bounds.x - 2*PONG_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });
}

struct board* board_init(bool is_multiplayer) {
    /*
     * constructor for board.
     */

    struct board* b = malloc(sizeof *b);

    if (b == NULL) {
        return NULL;
    }

    size_t wall_count = 3;
    struct wall** ws = malloc(sizeof(*ws) * wall_count);

    if (ws == NULL) {
        free(b);
        return NULL;
    }

    b->walls = ws;
    b->wall_count = wall_count;

    zero_pointers(&b->players[0], PONG_PLAYER_MAX);
    zero_pointers(b->walls, wall_count);

    if (is_multiplayer) {
        setup_multiplayer(b);
    } else {
        setup_singleplayer(b);
    }

    return b;
}

void board_destroy(struct board* b) {
    /*
     * destructor for board.
     */

    for (size_t i = 0; i < PONG_PLAYER_MAX; ++i) {
        free(b->players[i]);
    }

    for (size_t i = 0; i < b->wall_count; ++i) {
        free(b->walls[i]);
    }

    free(b);
}

int board_add_player(struct board* b, struct vector pos) {
    /*
     * add new player to board. order is important; the first to be added
     * is player 1, second is player 2 (and if more players were supported,
     * the pattern would continue).
     *
     * returns 0 on success, -1 otherwise.
     */

    struct paddle* next = malloc_first_null(b->players, PONG_PLAYER_MAX, sizeof *next);

    if (next == NULL) {
        // max number of players or failed to allocate memory
        return -1;
    }

    *next = (struct paddle) {
        .height = PONG_PADDLE_HEIGHT,
        .pos    = pos
    };

    return 0;
}

int board_add_wall(struct board* b, struct wall w) {
    /*
     * adds wall to board, essentially the same way as board_add_player().
     * returns 0 on success, -1 otherwise.
     */

    struct wall* next = malloc_first_null(b->walls, b->wall_count, sizeof *next);

    if (next == NULL) {
        return -1;
    }

    *next = w;

    return 0;
}
