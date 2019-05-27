#ifndef VECTOR_H_
#define VECTOR_H_

typedef int distance_t;

struct vector {
    distance_t x;
    distance_t y;
};

extern struct vector const zero_vector;

#endif // VECTOR_H_
