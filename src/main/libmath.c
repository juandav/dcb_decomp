/*
 * The end of the soft-float library: the trap handler the exception hooks
 * call, left empty, and a double square root by Newton's method, both
 * compiled. The arithmetic before them is hand-written assembly,
 * in libmath.s.
 */
#include "common.h"
#include "game.h"
#include "dcb/libmath.h"
#include "dcb/main.h"
#include "dcb/task.h"

void handleSoftFloatTrap(void) {
}

double sqrtDouble(double x) {
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
