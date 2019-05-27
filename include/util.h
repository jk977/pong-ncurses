/*
 * util.h: Various utility functions and macros, including the logic used in
 *         displaying information gathered by sttyl to the user.
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>
#include <curses.h>

#include "vector.h"

#define ERROR(...)                      \
    do {                                \
        fprintf(stderr, "pong: ");      \
        fprintf(stderr, __VA_ARGS__);   \
        fprintf(stderr, "\n");          \
    } while (0)

#ifdef DEBUG
#   define TRACE(...) ERROR(__VA_ARGS__)
#else
#   define TRACE(...)
#endif

void setup_curses(void);
void sanity_check(void);

struct vector get_max_bounds(WINDOW* win);
void* find_first_null(void* ptr, size_t count);

#endif // UTIL_H_
