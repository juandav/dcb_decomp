#include "psyq.h"

extern SpuReverbAttr D_801D95E0;

short SsUtSetReverbType(short type) {
    int echo = 0;
    short t = type;

    if (t < 0) {
        echo = 1;
        t = -t;
    }
    if ((u_short)t < 10) {
        D_801D95E0.mask = SPU_REV_MODE;
        if (echo) {
            D_801D95E0.mode = t | SPU_REV_MODE_CLEAR_WA;
        } else {
            D_801D95E0.mode = t;
        }
        if (t == 0) {
            SpuSetReverb(SPU_OFF);
        }
        SpuSetReverbModeParam(&D_801D95E0);
        return t;
    }
    return -1;
}

OBJECT_END(1);
