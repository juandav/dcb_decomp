#include "psyq.h"

void _SsSetNrpnVabAttr12(short vabId, short prog, short tone, VagAtr vag, short fn, u_char data) {
    SsADSR adsr;

    SsUtGetVagAtr(vabId, prog, tone, &vag);
    if (data >= 1 && data < 64) {
        adsr.srDir = 0;
    } else if (data >= 64 && data < 128) {
        adsr.srDir = 1;
    }
    _SsUtBuildADSR(&adsr, &vag.adsr1, &vag.adsr2);
    SsUtSetVagAtr(vabId, prog, tone, &vag);
}

OBJECT_END(2);
