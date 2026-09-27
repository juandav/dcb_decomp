#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void _SsSndNextSep(short seq, short sep) {
    SeqStruct *score = &D_801D8618[seq][sep];

    score->unk20 = 1;
    score->unk21 = 0;
    D_801D8618[seq][sep].flags &= ~0x100;
    D_801D8618[seq][sep].flags &= ~8;
    D_801D8618[seq][sep].flags &= ~2;
    D_801D8618[seq][sep].flags &= ~4;
    D_801D8618[seq][sep].flags &= ~0x200;
    score->unk14 = 1;
    score->unk0 = score->unk4;
    D_801D8618[seq][sep].flags |= 1;
}

OBJECT_END(1);
