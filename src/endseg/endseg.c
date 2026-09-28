#include "common.h"
#include "game.h"
#include "dcb/text.h"

INCLUDE_RODATA("asm/endseg/nonmatchings/endseg", D_801DDF38);

INCLUDE_ASM("asm/endseg/nonmatchings/endseg", func_801DE80C);

INCLUDE_ASM("asm/endseg/nonmatchings/endseg", func_801DEF8C);

INCLUDE_ASM("asm/endseg/nonmatchings/endseg", func_801DF2BC);

void func_801DF408(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    x += 40;
    drawText(x, y + 1, (s32)"*s0*b4: Scroll L1, R1: Fast Scroll", 7, z);
    drawText(x, y + 15, (s32)"*s0L2: Previous R2: Next *b6: Quit", 7, z);
}

INCLUDE_ASM("asm/endseg/nonmatchings/endseg", func_801DF47C);
