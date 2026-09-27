#include "psyq.h"

void func_80052060(void);

void _SsContDamper(short seq, short sep, u_char data) {
    SeqStruct *score = &D_801D8618[seq][sep];

    if (data < 64) {
        func_80052050();
    } else {
        func_80052060();
    }
    score->delta = _SsReadDeltaValue(seq, sep);
}

OBJECT_END(3);
