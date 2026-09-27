#include "psyq.h"

void _SsUtResolveADSR(u_short adsr1, u_short adsr2, SsADSR *adsr) {
    adsr->arMode = adsr1 & 0x8000;
    adsr->srMode = adsr2 & 0x8000;
    adsr->srDir = adsr2 & 0x4000;
    adsr->rrMode = adsr2 & 0x20;
    adsr->ar = (adsr1 >> 8) & 0x7F;
    adsr->dr = (adsr1 >> 4) & 0xF;
    adsr->sl = adsr1 & 0xF;
    adsr->sr = (adsr2 >> 6) & 0x7F;
    adsr->rr = adsr2 & 0x1F;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsUtBuildADSR);
