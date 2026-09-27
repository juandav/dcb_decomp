#include "psyq.h"

extern long D_80077998;
extern long D_800779AC;
extern PadPort *D_80077994;

void _padInitDirPort(void);

void PadInitDirect(u_char *pad1, u_char *pad2) {
    int i;
    int j;
    PadPort *p;
    u_char *d;

    D_80077998 = 0;
    D_800779AC = 0;
    _padInitDirPort();
    D_80077994[0].unk30 = pad1;
    D_80077994[1].unk30 = pad2;
    p = D_80077994;
    for (i = 0; i < 2; i++, p++) {
        p->unkC = NULL;
        p->unk10 = p;
        p->unk30[0] = 0xFF;
        p->unk30[1] = 0;
        d = p->unk5D;
        for (j = 0; j < 6; j++) {
            *d++ = 0xFF;
        }
    }
    D_80077998 = 1;
}
