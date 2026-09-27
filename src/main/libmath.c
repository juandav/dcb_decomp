/*
 * Soft-float library linked in with the game code: double and float
 * arithmetic, conversions and compares (the libgcc entry points), their
 * exception hooks and a double square root. All but the last two are
 * hand-written assembly.
 */
#include "common.h"
#include "game.h"
#include "dcb/libmath.h"
#include "dcb/main.h"
#include "dcb/task.h"

INCLUDE_ASM("asm/main/nonmatchings/libmath", __negdf2);

INCLUDE_ASM("asm/main/nonmatchings/libmath", __subdf3);

INCLUDE_ASM("asm/main/nonmatchings/libmath", __adddf3);

INCLUDE_ASM("asm/main/nonmatchings/libmath", func_80025874);

INCLUDE_ASM("asm/main/nonmatchings/libmath", __divdf3);

INCLUDE_ASM("asm/main/nonmatchings/libmath", func_80025C00);

INCLUDE_ASM("asm/main/nonmatchings/libmath", __muldf3);

INCLUDE_ASM("asm/main/nonmatchings/libmath", func_80025F08);

INCLUDE_ASM("asm/main/nonmatchings/libmath", __cmpdf2);

INCLUDE_ASM("asm/main/nonmatchings/libmath", __floatsidf);

INCLUDE_ASM("asm/main/nonmatchings/libmath", __fixdfsi);

INCLUDE_ASM("asm/main/nonmatchings/libmath", __subsf3);

INCLUDE_ASM("asm/main/nonmatchings/libmath", __mulsf3);

INCLUDE_ASM("asm/main/nonmatchings/libmath", func_80026C70);

INCLUDE_ASM("asm/main/nonmatchings/libmath", func_80026D30);

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
