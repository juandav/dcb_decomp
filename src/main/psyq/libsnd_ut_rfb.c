#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern SpuReverbAttr D_801D95E0;

void SsUtSetReverbFeedback(short feedback) {
    D_801D95E0.mask = SPU_REV_FEEDBACK;
    D_801D95E0.feedback = feedback;
    SpuSetReverbModeParam(&D_801D95E0);
}
OBJECT_END(1);

void SsUtReverbOff(void) {
    SpuSetReverb(0);
}

void SsUtReverbOn(void) {
    SpuSetReverb(1);
}
