#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmPBVoice);

extern short D_801D96F4;
extern char D_801D96D4;
short _SsVmPBVoice(short voice, short seq_sep, short vab, short prog, long pitch);

long _SsVmPitchBend(short seq_sep, short vab, short prog, u_short pitch) {
    short i;
    long ret = 0;

    _SsVmVSetUp(vab, prog);
    D_801D96F4 = seq_sep;
    for (i = 0; i < D_801D96D4; i++) {
        ret += _SsVmPBVoice(i, seq_sep, vab, prog, pitch);
    }
    return ret;
}

OBJECT_END(2);
