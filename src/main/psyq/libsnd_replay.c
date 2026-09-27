#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void _SsSndReplay(short seq, short sep) {
    SeqStruct *score = &D_801D8618[seq][sep];

    score->unk14 = 1;
    D_801D8618[seq][sep].flags &= ~8;
}
