/*
 * Soft-float library linked in with the game code: double and float
 * arithmetic, conversions and compares (the libgcc entry points), their
 * exception hooks and a double square root.
 *
 * The arithmetic routines are hand-written assembly, not compiler output:
 * they use $at as an ordinary temporary (`addiu $at,$zero,0x0` then
 * `beq $x,$at`, `slti $at,$x,0` then `bnez $at`), which no compiler does
 * since $at belongs to the assembler; they pass results to each other in
 * $a0/$a1 and keep the caller's $ra in $t8 (`jr $t8`); __subdf3 has no
 * return and runs into __adddf3, and more entry points sit unlabelled
 * after __cmpdf2 and __fixdfsi; they open a private 0xC-byte frame in the
 * middle of the code around each call; and __divdf3 and __muldf3 save $ra
 * at 4($sp) of a 0x18 frame, where GCC puts it at the top.
 *
 * The two exception hooks, func_80026C70 and func_80026D30, are compiled,
 * but not by any GCC the game or PsyQ were built with (2.7.2, 2.8.x and
 * 2.95.2 at any -O): $fp points at the caller's $sp, the saves go upwards
 * from 8($sp) with $ra lowest, moves are `addu $s0,$zero,$a0`, and the
 * stack drops 8 bytes around each call instead of keeping the argument
 * area in the frame.
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
