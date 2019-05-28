#ifndef VECTOR_H_
#define VECTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef int distance_t;

struct vector {
    distance_t x;
    distance_t y;
};

#ifdef __cplusplus
}
#endif

#endif // VECTOR_H_
