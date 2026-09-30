#include "psyq.h"

void GsSetAmbient(long r, long g, long b) {
    SetBackColor(r >> 4, g >> 4, b >> 4);
}

OBJECT_END(2);
