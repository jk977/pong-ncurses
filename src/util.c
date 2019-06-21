#include "util.h"

#include <stdlib.h>
#include <curses.h>

struct vector get_max_bounds() {
    /*
     * gets x and y bounds for the screen.
     * if curses screen isn't found, returns {-1,-1}.
     */

    if (stdscr == NULL) {
        return (struct vector) {-1, -1};
    }

    int x;
    int y;
    getmaxyx(stdscr, y, x);

    return (struct vector) { x, y };
}

void* find_first_null(void* ptr, size_t count) {
    /*
     * get first null pointer in array of pointers.
     *
     * note: ptr must be an array of pointers (void**) or the function will
     *       cause a segmentation fault.
     *
     * returns the address of the first null pointer (pointer to pointer),
     * or null if all pointers are nonnull.
     */

    void** ary_ptrs = (void**) ptr;

    for (size_t i = 0; i < count; ++i) {
        if (ary_ptrs[i] == NULL) {
            return &ary_ptrs[i];
        }
    }

    return NULL;
}

void* malloc_first_null(void* ptr, size_t count, size_t type_size) {
    /*
     * finds first null pointer in list of pointers <ptr>, then
     * mallocs a pointer in its place and returns the malloc'd pointer.
     * since pointers of type void* are opaque, the size of the base type
     * <type_size> must be provided for the malloc
     */

    void** p_next = find_first_null(ptr, count);

    if (p_next == NULL) {
        return NULL;
    }

    *p_next = malloc(type_size);

    return *p_next;
}

int center_horiz(int x_middle, int length) {
    /*
     * calculates starting x-coordinate needed to center an object around x_middle
     */

    return x_middle - length / 2;
}

int center_in_window(int length) {
    int const x_center = get_max_bounds().x / 2;
    return center_horiz(x_center, length);
}
