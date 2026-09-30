#include "psyq.h"

void SsUtReverbOn(void);

void _SsSetNrpnVabAttr1(short vabId, short prog, short tone, VagAtr vag, short fn, u_char data) {
    SsUtGetVagAtr(vabId, prog, tone, &vag);
    vag.mode = data;
    SsUtSetVagAtr(vabId, prog, tone, &vag);
    if (data == 0) {
        SsUtReverbOff();
    } else if (data == 4) {
        SsUtReverbOn();
    }
}

OBJECT_END(2);
