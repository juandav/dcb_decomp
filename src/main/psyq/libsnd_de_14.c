#include "psyq.h"

void _SsSetNrpnVabAttr14(short vabId, short prog, short tone, VagAtr vag, short fn, u_char data) {
    SsADSR adsr;

    SsUtGetVagAtr(vabId, prog, tone, &vag);
    vag.porW = data;
    _SsUtBuildADSR(&adsr, &vag.adsr1, &vag.adsr2);
    SsUtSetVagAtr(vabId, prog, tone, &vag);
}

OBJECT_END(1);

void func_8004DF40(short vabId, short prog, short tone, VagAtr vag, short fn, unsigned char data) {
    SsUtSetReverbType(data);
}

OBJECT_END(3);
