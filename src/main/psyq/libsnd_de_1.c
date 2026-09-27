#include "psyq.h"

void func_80051C90(void);

void _SsSetNrpnVabAttr1(short vabId, short prog, short tone, VagAtr vag, short fn, u_char data) {
    SsUtGetVagAtr(vabId, prog, tone, &vag);
    vag.mode = data;
    SsUtSetVagAtr(vabId, prog, tone, &vag);
    if (data == 0) {
        func_80051C70();
    } else if (data == 4) {
        func_80051C90();
    }
}

OBJECT_END(2);
