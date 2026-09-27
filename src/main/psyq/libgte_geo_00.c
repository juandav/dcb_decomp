#include "psyq.h"

int rsin(int a) {
    if (a < 0) {
        return -sin_1(-a & 0xFFF);
    }
    return sin_1(a & 0xFFF);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", sin_1);
