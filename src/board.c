#include "board.h"

#include "config.h"
#include "util.h"

#include <stdlib.h>
#include <time.h>

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

static int setup_singleplayer(struct board* b) {
    /*
     * only set 1 paddle, and have a wall covering the entire left side.
     * returns 0 on success, or a negative number otherwise.
     */

    struct vector bounds = get_max_bounds();

    if (bounds.x == -1) {
        return -1;
    }

    int x_center = bounds.x / 2;
    int y_center = bounds.y / 2;

    b->ball = (struct ball) {
        .pos = {x_center, y_center},
        .velocity = {1, 1},
        .multiplier = 1
    };

    b->p1_score = -1;
    b->p2_score = 0;

    int status = board_add_player(b, (struct vector) {bounds.x - 10, y_center});

    // wall acting as perfect opponent
    status |= board_add_wall(b, (struct wall) {
        .pos      = {10, PONG_OUTER_BUFFER},
        .length   = bounds.y - 2*PONG_OUTER_BUFFER + 1,
        .tangible = true,
        .dir      = VERTICAL,
        .style    = SOLID
    });

    // upper playing field boundary
    status |= board_add_wall(b, (struct wall) {
        .pos      = {PONG_OUTER_BUFFER, PONG_OUTER_BUFFER},
        .length   = bounds.x - 2*PONG_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });

    // lower playing field boundary
    status |= board_add_wall(b, (struct wall) {
        .pos      = {PONG_OUTER_BUFFER, bounds.y - PONG_OUTER_BUFFER},
        .length   = bounds.x - 2*PONG_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });

    return status;
}

static int setup_multiplayer(struct board* b) {
    /*
     * two paddles are needed for multiplayer, and no vertical
     * wall to bounce the ball off of
     */

    struct vector bounds = get_max_bounds();
    int x_center = bounds.x / 2;
    int y_center = bounds.y / 2;

    b->ball = (struct ball) {
        .pos = {x_center, y_center},
        .velocity = {-1, 1},
        .multiplier = 1
    };

    b->p1_score = 0;
    b->p2_score = 0;

    int status   = board_add_player(b, (struct vector) {10, y_center});
    status      |= board_add_player(b, (struct vector) {bounds.x - 10, y_center});

    // intangible center line dividing the players' sides
    status |= board_add_wall(b, (struct wall) {
        .pos      = {x_center, PONG_OUTER_BUFFER},
        .length   = bounds.y - 2*PONG_OUTER_BUFFER,
        .tangible = false,
        .dir      = VERTICAL,
        .style    = DASHED
    });

    // upper playing field boundary
    status |= board_add_wall(b, (struct wall) {
        .pos      = {PONG_OUTER_BUFFER, PONG_OUTER_BUFFER},
        .length   = bounds.x - 2*PONG_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });

    // lower playing field boundary
    status |= board_add_wall(b, (struct wall) {
        .pos      = {PONG_OUTER_BUFFER, bounds.y - PONG_OUTER_BUFFER},
        .length   = bounds.x - 2*PONG_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });

    return status;
}

struct board* board_init(bool is_multiplayer) {
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

    int status;

    if (is_multiplayer) {
        status = setup_multiplayer(b);
    } else {
        status = setup_singleplayer(b);
    }

    if (status != 0) {
        free(b);
        b = NULL;
    }

    return b;
}

void board_destroy(struct board* b) {
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

    // vertically center paddle around given position
    pos.y -= PONG_PADDLE_HEIGHT / 2;

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

static bool paddle_can_move(struct board* b, struct paddle* p, int y) {
    /*
     * checks if paddle can move <y> units vertically.
     */

    struct vector start = p->pos;

    if (y > 0) {
        // use bottom of paddle as start of ray since it's moving downward
        // p->height - 1 gets the edge of the paddle, and subtracting an
        // additional 1 allows the edges to overlap with walls so the ball
        // doesn't go through the paddle
        start.y += p->height - 2;
    } else {
        ++start.y;
    }

    struct vector end           = {start.x, start.y + y};
    struct line path            = line_between(start, end);
    struct intersection current = { .intersects = false, .point = {0,0} };

    // iterate through walls until an intersection is found
    for (size_t i = 0; i < b->wall_count && !current.intersects; ++i) {
        struct wall* w = b->walls[i];

        if (w == NULL || !w->tangible) {
            continue;
        }

        current = intersection_between(path, wall_to_line(w));
    }

    return !current.intersects;
}

static int random_sign(void) {
    if (rand() % 2 == 0) {
        return -1;
    } else {
        return 1;
    }
}

void board_reset_ball(struct board* b) {
    /*
     * sets the ball to a random point in the center of the board,
     * going in a random direction.
     */

    struct vector bounds = get_max_bounds();
    int x_center = bounds.x / 2;

    srand(time(NULL));

    // find a random height inside the playing field to spawn the ball at
    int const playing_field_height = bounds.y - 2*PONG_OUTER_BUFFER - 2;
    int const random = rand();

    int y_rand = random % playing_field_height + PONG_OUTER_BUFFER + 1;

    b->ball.pos      = (struct vector) {x_center, y_rand};
    b->ball.velocity = (struct vector) {random_sign(), random_sign()};
}

int board_move_paddle(struct board* b, unsigned int player, int y) {
    /*
     * moves paddle <y> units vertically, unless the paddle is unable to move.
     * y < 0 indicates upward movement, and y > 0 gives downward movement
     * returns 0 on successful move, -1 otherwise.
     */

    if (player > 1) {
        return -1;
    }

    struct paddle* p = b->players[player];

    if (p == NULL || !paddle_can_move(b, p, y)) {
        return -1;
    }

    p->pos.y += y;
    return 0;
}
