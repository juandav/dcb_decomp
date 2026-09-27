#include "psyq.h"

int CdPosToInt(CdlLOC *p) {
    return (btoi(p->minute) * 60 + btoi(p->second)) * 75 + btoi(p->sector) - 150;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdSearchFile);

int func_8005757C(char *a, char *b) {
    return strncmp(a, b, 12) == 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005759C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80057860);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80057904);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80057BA0);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013394);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013398);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_8001339C);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_800133B8);
