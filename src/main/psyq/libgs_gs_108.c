#include "psyq.h"

extern long D_801DBE34;

void GsSetLightMode(int mode) {
    switch (mode) {
    case 0:
        D_801DBE34 = 0;
        break;
    case 1:
        D_801DBE34 = 1;
        break;
    case 2:
        D_801DBE34 = 2;
        break;
    case 3:
        D_801DBE34 = 3;
        break;
    default:
        printf("not supported light mode %d\n", mode);
        break;
    }
}

OBJECT_END(1);
