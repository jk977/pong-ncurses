/*
 * util.h: Various utility functions and macros, including the logic used in
 *         displaying information gathered by sttyl to the user.
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "physics/vector.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ERROR(...)                          \
    do {                                    \
        fprintf(stderr, "%s: ", __func__);  \
        fprintf(stderr, __VA_ARGS__);       \
        fprintf(stderr, "\n");              \
    } while (0)

#ifdef DEBUG
#   define TRACE(...) ERROR(__VA_ARGS__)
#else
#   define TRACE(...)
#endif

// macro that returns ERR if an OK/ERR-returning function fails
#define TRY_FN(ret)                                                     \
    do {                                                                \
        if (ret == ERR) {                                               \
            ERROR("TRY_FN failed (%s: line %d).", __FILE__, __LINE__);   \
            return ERR;                                                 \
        }                                                               \
    } while (0)

struct vector get_max_bounds(void);
void* find_first_null(void* ptr, size_t count);
void* malloc_first_null(void* ptr, size_t count, size_t type_size);

int center_horiz(int x_middle, int length);
int center_in_window(int length);

#ifdef __cplusplus
}
#endif

#endif // UTIL_H_
