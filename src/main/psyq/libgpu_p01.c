#include "psyq.h"

u_short GetClut(int x, int y) {
    return (y << 6) | ((x >> 4) & 0x3F);
}

OBJECT_END(2);
