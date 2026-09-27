#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

u_long SpuGetReverbVoice(void) {
    return _SpuGetAnyVoice(0xCC, 0xCD);
}

OBJECT_END(3);
