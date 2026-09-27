#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80063024);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80063110);

long func_800631D8(long value) {
    long bits = 0;

    while (value > 0) {
        value >>= 1;
        bits++;
    }
    return bits;
}
