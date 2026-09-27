#include "psyq.h"

u_short func_80067644(int x, int y) {
    return (y << 6) | ((x >> 4) & 0x3F);
}

OBJECT_END(2);
