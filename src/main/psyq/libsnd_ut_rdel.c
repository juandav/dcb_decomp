#include "psyq.h"

extern SpuReverbAttr D_801D95E0;

void SsUtSetReverbDelay(short delay) {
    D_801D95E0.mask = SPU_REV_DELAYTIME;
    D_801D95E0.delay = delay;
    SpuSetReverbModeParam(&D_801D95E0);
}

OBJECT_END(1);
