/*
 * The end of the soft-float library: an empty function and a double square
 * root, both compiled. The arithmetic before them is hand-written assembly,
 * in libmath.s.
 */
#include "common.h"
#include "game.h"
#include "dcb/libmath.h"
#include "dcb/main.h"
#include "dcb/task.h"

void func_80026D84(void) {
}

double func_80026D8C(double x) {
    double r;
    double s;

    if (x <= 0) {
        return 0;
    }
    if (x > 1.0) {
        r = x;
    } else {
        r = 1.0;
    }
    do {
        s = r;
        r = (x / s + s) * 0.5;
    } while (r < s);
    return s;
}
