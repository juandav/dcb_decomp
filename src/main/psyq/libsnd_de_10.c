#include "psyq.h"

void _SsSetNrpnVabAttr10(short vabId, short prog, short tone, VagAtr vag, short fn, u_char data) {
    SsADSR adsr;

    SsUtGetVagAtr(vabId, prog, tone, &vag);
    _SsUtResolveADSR(vag.adsr1, vag.adsr2, &adsr);
    adsr.rrMode = 0;
    adsr.rr = data;
    _SsUtBuildADSR(&adsr, &vag.adsr1, &vag.adsr2);
    SsUtSetVagAtr(vabId, prog, tone, &vag);
}
