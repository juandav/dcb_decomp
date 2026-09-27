#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtGetVagAtr);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtKeyOnV);

extern int D_801D860C;
extern short D_801D96F8;
void _SsVmKeyOffNow(int);

short SsUtKeyOffV(short voice) {
    if (D_801D860C == 1) {
        return -1;
    }
    D_801D860C = 1;
    if ((u_short)voice < 24) {
        D_801D96F8 = voice;
        _SsVmKeyOffNow(0);
        D_801D860C = 0;
        return 0;
    }
    D_801D860C = 0;
    return -1;
}
