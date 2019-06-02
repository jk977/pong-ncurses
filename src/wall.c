#include "wall.h"

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

