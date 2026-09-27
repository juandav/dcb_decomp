#include "psyq.h"

extern u_long D_801D8614;

short _SsInitSoundSeq(short seq, short vabId, u_char *addr) {
    SeqStruct *score;
    long i;
    long tempo;
    long hi;
    long lo;
    long t0;
    long t1;
    long t2;

    score = *(D_801D8618 + seq);
    score->vabId = vabId;
    score->unk50 = 0;
    score->rpn1 = 0;
    score->rpn2 = 0;
    score->unk1E = 0;
    score->unk1A[0] = 0;
    score->unk1A[1] = 0;
    score->unk1F = 0;
    score->channel = 0;
    score->unk84 = 0;
    score->unk88 = 0;
    score->unk8C = 0;
    score->unk56 = 0;
    score->unk21 = 0;
    score->unk20 = 1;
    score->unk14 = 0;
    score->delta = 0;
    score->unk1A[2] = 0;
    score->unk1A[3] = 0;
    score->unk15 = 0;
    score->status = 0;
    score->unk80 = 0;
    score->unk24[0] = 0;
    score->unk24[1] = 0;
    for (i = 0; i < 16; i++) {
        score->programs[i] = i;
        score->panpot[i] = 64;
        score->vol[i] = 127;
    }
    score->unk52 = 1;
    score->unk0 = addr;
    if (*score->unk0 == 'S' || *score->unk0 == 'p') {
        score->unk0 += 7;
        if (*score->unk0++ != 1) {
            printf("This is not SEQ Data.\n");
            return -1;
        }
    } else {
        printf("This is an old SEQ Data Format.\n");
        return 0;
    }
    hi = *score->unk0++;
    lo = *score->unk0++;
    score->unk50 = lo | hi << 8;
    t0 = *score->unk0++;
    t1 = *score->unk0++;
    t2 = *score->unk0++;
    tempo = t2 | (t0 << 16 | t1 << 8);
    score->unk8C = tempo;
    if (tempo / 2 < 60000000 % tempo) {
        score->unk8C = 60000000 / tempo + 1;
    } else {
        score->unk8C = 60000000 / tempo;
    }
    score->unk94 = score->unk8C;
    score->unk24[0] = *score->unk0++;
    score->unk24[1] = *score->unk0++;
    score->delta = score->unk84 = _SsReadDeltaValue(seq, 0);
    score->unk4 = score->unk0;
    score->unk8 = score->unk0;
    score->unkC = score->unk0;
    score->unk10 = NULL;
    if (score->unk50 * score->unk8C * 10 < D_801D8614 * 60) {
        score->unk54 = score->unk52 = D_801D8614 * 600 / (score->unk50 * score->unk8C);
    } else {
        score->unk52 = -1;
        score->unk54 = score->unk50 * score->unk8C * 10 / (D_801D8614 * 60);
        if (D_801D8614 * 30 < score->unk50 * score->unk8C * 10 % (D_801D8614 * 60)) {
            score->unk54++;
        }
    }
    score->unk56 = score->unk54;
    return 0;
}

__asm__(".section .rodata\n\t.space 4\n");

OBJECT_END(3);
