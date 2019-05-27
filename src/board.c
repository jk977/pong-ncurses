#include <stdlib.h>
#include <curses.h>

#include "board.h"
#include "config.h"
#include "util.h"

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

static void setup_multiplayer(struct board* b) {
    struct vector bounds = get_max_bounds(stdscr);
    int x_center = bounds.x / 2;
    int y_center = bounds.y / 2;

    board_add_player(b, (struct vector) {10, y_center - 1});
    board_add_player(b, (struct vector) {bounds.x - 10, y_center - 1});

    // center line dividing the players' sides
    board_add_wall(b, (struct wall) {
        .pos      = {x_center, BOARD_OUTER_BUFFER},
        .length   = bounds.y - 2*BOARD_OUTER_BUFFER,
        .tangible = false,
        .dir      = VERTICAL,
        .style    = DASHED
    });

    // upper playing field boundary
    board_add_wall(b, (struct wall) {
        .pos      = {BOARD_OUTER_BUFFER, 0},
        .length   = bounds.x - 2*BOARD_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });

    // lower playing field boundary
    board_add_wall(b, (struct wall) {
        .pos      = {BOARD_OUTER_BUFFER, bounds.y - BOARD_OUTER_BUFFER},
        .length   = bounds.x - 2*BOARD_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });
}

static void setup_singleplayer(struct board* b) {
    struct vector bounds = get_max_bounds(stdscr);
    int y_center = bounds.y / 2;

    board_add_player(b, (struct vector) {bounds.x - 10, y_center - 1});

    // wall acting as perfect opponent
    board_add_wall(b, (struct wall) {
        .pos      = {10, y_center - 1},
        .length   = bounds.y - 2*BOARD_OUTER_BUFFER,
        .tangible = false,
        .dir      = VERTICAL,
        .style    = SOLID
    });

    // upper playing field boundary
    board_add_wall(b, (struct wall) {
        .pos      = {BOARD_OUTER_BUFFER, 0},
        .length   = bounds.x - 2*BOARD_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });

    // lower playing field boundary
    board_add_wall(b, (struct wall) {
        .pos      = {BOARD_OUTER_BUFFER, bounds.y - BOARD_OUTER_BUFFER},
        .length   = bounds.x - 2*BOARD_OUTER_BUFFER,
        .tangible = true,
        .dir      = HORIZONTAL,
        .style    = DASHED
    });
}

struct board* board_init(bool is_multiplayer) {
    if (stdscr == NULL) {
        // curses TUI must be running
        ERROR("board_init: No curses screen found.");
        return NULL;
    }

    struct board* b = malloc(sizeof *b);

    if (b == NULL) {
        return NULL;
    }

    unsigned int wall_count = 3;
    struct wall** ws = malloc(sizeof(*ws) * wall_count);

    if (ws == NULL) {
        free(b);
        return NULL;
    }

    b->walls = ws;
    b->wall_count = wall_count;

    zero_pointers(&b->players[0], BOARD_PLAYER_MAX);
    zero_pointers(b->walls, wall_count);

    if (is_multiplayer) {
        setup_multiplayer(b);
    } else {
        setup_singleplayer(b);
    }

    return b;
}

void board_destroy(struct board* b) {
    free(b);
}

int board_add_player(struct board* b, struct vector pos) {
    (void) b;
    (void) pos;
    return 0;
}

int board_add_wall(struct board* b, struct wall w) {
    (void) b;
    (void) w;
    return 0;
}
