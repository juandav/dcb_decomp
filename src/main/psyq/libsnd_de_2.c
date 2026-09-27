#include "psyq.h"

void _SsSetNrpnVabAttr2(short vabId, short prog, short tone, VagAtr vag, short fn, u_char data) {
    SsUtGetVagAtr(vabId, prog, tone, &vag);
    vag.min = data;
    SsUtSetVagAtr(vabId, prog, tone, &vag);
}

OBJECT_END(1);
