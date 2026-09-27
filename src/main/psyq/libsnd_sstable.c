#include "psyq.h"

extern short D_801D8E98;
extern short D_801D8E9A;
extern long D_801D8610;

void SsSetTableSize(char *table, short s_max, short t_max) {
    int i;
    int j;

    D_801D8E98 = s_max;
    D_801D8E9A = t_max;
    for (i = 0; i < s_max; i++) {
        D_801D8618[i] = &((SeqStruct *)table)[i * t_max];
    }
    for (i = s_max; i < 32; i++) {
        D_801D8610 |= 1 << i;
    }
    for (i = 0; i < D_801D8E98; i++) {
        for (j = 0; j < D_801D8E9A; j++) {
            D_801D8618[i][j].flags = 0;
            D_801D8618[i][j].unk22 = -1;
            D_801D8618[i][j].unk23 = 0;
            D_801D8618[i][j].unk48 = 0;
            D_801D8618[i][j].unk4A = 0;
            D_801D8618[i][j].unk9C = 0;
            D_801D8618[i][j].unkA0 = 0;
            D_801D8618[i][j].unk4C = 0;
            D_801D8618[i][j].unkAC = 0;
            D_801D8618[i][j].unkA8 = 0;
            D_801D8618[i][j].unkA4 = 0;
            D_801D8618[i][j].unk4E = 0;
            D_801D8618[i][j].voll = 0x7F;
            D_801D8618[i][j].volr = 0x7F;
            D_801D8618[i][j].unk5C = 0x7F;
            D_801D8618[i][j].unk5E = 0x7F;
        }
    }
}

OBJECT_END(2);
