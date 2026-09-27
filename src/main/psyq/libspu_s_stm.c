#include "psyq.h"

long SpuSetTransferMode(long mode) {
    long m;

    switch (mode) {
    case 0:
        m = 0;
        break;
    case 1:
        m = 1;
        break;
    default:
        m = 0;
        break;
    }
    D_8006EF94 = mode;
    D_8006EF40 = m;
    return m;
}
