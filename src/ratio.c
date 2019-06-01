#include "ratio.h"

static int gcd(int a, int b) {
    /*
     * euclidean algorithm for greatest common denominator.
     */

    if (b == 0) {
        return a;
    } else {
        return gcd(b, a % b);
    }
}

void ratio_simplify(struct ratio* r) {
    if (r->num == 0) {
        r->denom = 1;
    } else if (r->denom == 0) {
        r->num = 1;
    } else {
        int common = gcd(r->num, r->denom);
        r->num /= common;
        r->denom /= common;
    }
}

bool ratio_equals(struct ratio r1, struct ratio r2) {
    ratio_simplify(&r1);
    ratio_simplify(&r2);

    return r1.num == r2.num
        && r1.denom == r2.denom;
}
