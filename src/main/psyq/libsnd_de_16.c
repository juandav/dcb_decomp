#include "psyq.h"

void _SsSetNrpnVabAttr16(short vabId, short prog, short tone, VagAtr vag, short fn, unsigned char data) {
    SsUtSetReverbDepth(data, data);
}

OBJECT_END(2);

void func_8004DFA0(short vabId, short prog, short tone, VagAtr vag, short fn, unsigned char data) {
    SsUtSetReverbFeedback(data);
}

OBJECT_END(3);

void func_8004DFD0(short vabId, short prog, short tone, VagAtr vag, short fn, unsigned char data) {
    SsUtSetReverbDelay(data);
}

OBJECT_END(3);

void func_8004E000(short vabId, short prog, short tone, VagAtr vag, short fn, unsigned char data) {
    SsUtSetReverbDelay(data);
}

OBJECT_END(3);
