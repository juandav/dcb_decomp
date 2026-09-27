#include "psyq.h"

short SsVabTransCompleted(short immediateFlag) {
    return SpuIsTransferCompleted(immediateFlag);
}

OBJECT_END(2);
