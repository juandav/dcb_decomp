#include "psyq.h"

void _SsUtResolveADSR(u_short adsr1, u_short adsr2, SsADSR *adsr);
void _SsUtBuildADSR(SsADSR *adsr, u_short *adsr1, u_short *adsr2);

void _SsSetNrpnVabAttr0(short vabId, short prog, short tone, VagAtr vag, short fn, u_char data) {
    SsUtGetVagAtr(vabId, prog, tone, &vag);
    vag.prior = data;
    SsUtSetVagAtr(vabId, prog, tone, &vag);
}

OBJECT_END(1);
