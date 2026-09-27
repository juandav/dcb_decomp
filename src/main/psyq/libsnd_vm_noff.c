#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern VmVoice D_801D8EB0[];

void vmNoiseOff(u8 voice) {
    D_801D8EB0[voice].unk1D = 0;
    D_801D8EB0[voice].unk0 = 0;
    D_801D8EB0[voice].unk4 = 0;
}

OBJECT_END(2);
