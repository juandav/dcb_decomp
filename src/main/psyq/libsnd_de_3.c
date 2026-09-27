#include "psyq.h"

void _SsSetNrpnVabAttr3(short vabId, short prog, short tone, VagAtr vag, short fn, u_char data) {
    SsUtGetVagAtr(vabId, prog, tone, &vag);
    vag.max = data;
    SsUtSetVagAtr(vabId, prog, tone, &vag);
}

OBJECT_END(1);
