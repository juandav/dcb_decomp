#include "psyq.h"

void _SpuDataCallback(void (*func)()) {
    DMACallback(4, func);
}

OBJECT_END(3);
