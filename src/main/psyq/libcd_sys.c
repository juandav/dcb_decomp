#include "psyq.h"

int CdPosToInt(CdlLOC *p) {
    return (btoi(p->minute) * 60 + btoi(p->second)) * 75 + btoi(p->sector) - 150;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdSearchFile);

int func_8005757C(char *a, char *b) {
    return strncmp(a, b, 12) == 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005759C);

typedef struct {
    long key;
    long unk4;
    char name[0x24];
} CdSysEntry;
extern CdSysEntry D_801D9EFC[128];

int func_80057860(long key, char *name) {
    int i;

    for (i = 0; i < 128; i++) {
        if (D_801D9EFC[i].key == 0) {
            break;
        }
        if (D_801D9EFC[i].key == key && strcmp(name, D_801D9EFC[i].name) == 0) {
            return i + 1;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80057904);

int func_80057BA0(u_long *buf, int sector, int mode) {
    CdlLOC loc;

    CdIntToPos(sector, &loc);
    CdControl(CdlSetloc, (u_char *)&loc, 0);
    CdRead(buf, mode, 0x80);
    return CdReadSync(0, 0) == 0;
}

OBJECT_END(3);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013394);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013398);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_8001339C);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_800133B8);
