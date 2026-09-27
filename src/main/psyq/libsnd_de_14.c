#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr14);

void func_8004DF40(short vabId, short prog, short tone, VagAtr vag, short fn, unsigned char data) {
    SsUtSetReverbType(data);
}

OBJECT_END(3);
