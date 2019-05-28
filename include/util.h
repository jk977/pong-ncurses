/*
 * util.h: Various utility functions and macros, including the logic used in
 *         displaying information gathered by sttyl to the user.
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>
#include <curses.h>

#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

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

void sanity_check(void);
void setup_curses(void);

struct vector get_max_bounds();
void* find_first_null(void* ptr, size_t count);
void* malloc_first_null(void* ptr, size_t count, size_t type_size);

#ifdef __cplusplus
}
#endif

#endif // UTIL_H_
