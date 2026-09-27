#include "psyq.h"

void _SsSetNrpnVabAttr5(short vabId, short prog, short tone, VagAtr vag, short fn, u_char data) {
    SsADSR adsr;

    SsUtGetVagAtr(vabId, prog, tone, &vag);
    _SsUtResolveADSR(vag.adsr1, vag.adsr2, &adsr);
    adsr.arMode = 1;
    adsr.ar = data;
    _SsUtBuildADSR(&adsr, &vag.adsr1, &vag.adsr2);
    SsUtSetVagAtr(vabId, prog, tone, &vag);
}

OBJECT_END(3);
