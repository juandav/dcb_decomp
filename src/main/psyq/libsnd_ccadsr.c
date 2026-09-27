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

void _SsUtBuildADSR(SsADSR *adsr, u_short *adsr1, u_short *adsr2) {
    u_short arMode;
    u_short srMode;
    u_short mode2;
    u_short a1;
    u_short a2;

    srMode = adsr->srMode ? 0x8000 : 0;
    arMode = adsr->arMode ? 0x8000 : 0;
    mode2 = srMode;
    if (adsr->srDir) {
        mode2 = srMode | 0x4000;
    }
    if (adsr->rrMode) {
        mode2 |= 0x20;
    }
    a1 = arMode | ((adsr->ar << 8) & 0x7F00) | ((adsr->dr << 4) & 0xF0) | (adsr->sl & 0xF);
    a2 = mode2 | ((adsr->sr << 6) & 0x1FC0) | (adsr->rr & 0x1F);
    *adsr1 = a1;
    *adsr2 = a2;
}
