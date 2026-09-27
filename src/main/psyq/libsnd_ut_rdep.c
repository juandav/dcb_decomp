#include "psyq.h"

extern SpuReverbAttr D_801D95E0;

void SsUtSetReverbDepth(short ldepth, short rdepth) {
    D_801D95E0.mask = SPU_REV_DEPTHL | SPU_REV_DEPTHR;
    D_801D95E0.depth.left = ldepth * 0x7FFF / 127;
    D_801D95E0.depth.right = rdepth * 0x7FFF / 127;
    SpuSetReverbModeParam(&D_801D95E0);
}

OBJECT_END(1);
