#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void _SsVmSeqKeyOff(short seq_sep_no);
void func_80052050(void);

void _SsSndStop(short seq, short sep) {
    SeqStruct *score = &D_801D8618[seq][sep];
    int i;

    score->flags &= ~1;
    D_801D8618[seq][sep].flags &= ~2;
    D_801D8618[seq][sep].flags &= ~8;
    D_801D8618[seq][sep].flags &= ~0x400;
    D_801D8618[seq][sep].flags |= 4;
    _SsVmSeqKeyOff(seq | (sep << 8));
    func_80052050();
    score->unk14 = 0;
    score->unk88 = 0;
    score->unk1A[2] = 0;
    score->rpn1 = 0;
    score->rpn2 = 0;
    score->unk1E = 0;
    score->unk1A[0] = 0;
    score->unk1A[1] = 0;
    score->unk1F = 0;
    score->channel = 0;
    score->unk21 = 0;
    score->unk1A[2] = 0;
    score->unk1A[3] = 0;
    score->unk15 = 0;
    score->status = 0;
    score->delta = score->unk84;
    score->unk94 = score->unk8C;
    score->unk54 = score->unk56;
    score->unk0 = score->unk4;
    score->unk8 = score->unk4;
    for (i = 0; i < 16; i++) {
        score->programs[i] = i;
        score->panpot[i] = 0x40;
        score->vol[i] = 0x7F;
    }
    score->unk5C = 0x7F;
    score->unk5E = 0x7F;
}

void SsSeqStop(short seq) {
    _SsSndStop(seq, 0);
}

void SsSepStop(short a, short b) {
    _SsSndStop(a, b);
}

OBJECT_END(3);
