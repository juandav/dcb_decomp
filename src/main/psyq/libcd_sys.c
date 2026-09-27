#include "psyq.h"

typedef struct {
    u_char c[4];
} CdLBA;

typedef struct {
    long id;
    long parent;
    CdLBA lba;
    char name[32];
} CdlDIR;

extern CdlFILE D_801D98F8[CdlMAXFILE];
extern CdlDIR D_801D9EF8[CdlMAXDIR];
extern u_char D_801DB4F8[0x800];
extern long D_80070B58;
extern long D_80070B5C;
extern long D_80070C48;
extern long D_80070C54;

int func_8005759C(void);
int func_80057860(long parent, char *name);
int func_80057904(int dir);
int func_80057BA0(int sectors, int sector, u_char *buf);
int func_8005757C(char *a, char *b);

int CdPosToInt(CdlLOC *p) {
    return (btoi(p->minute) * 60 + btoi(p->second)) * 75 + btoi(p->sector) - 150;
}

CdlFILE *CdSearchFile(CdlFILE *fp, char *name) {
    char buf[32];
    int dir;
    int i;
    char *p;
    char *q;

    if (D_80070B5C != D_80070C54) {
        if (func_8005759C() == 0) {
            return NULL;
        }
        D_80070B5C = D_80070C54;
    }
    if (name[0] != '\\') {
        return NULL;
    }
    buf[0] = 0;
    dir = 1;
    p = name;
    for (i = 0; i < CdlMAXLEVEL; i++) {
        q = buf;
        while (*p != '\\') {
            if (*p == 0) {
                goto out;
            }
            *q++ = *p++;
        }
        if (*p == 0) {
            goto out;
        }
        p++;
        *q = 0;
        dir = func_80057860(dir, buf);
        if (dir == -1) {
            buf[0] = 0;
            break;
        }
    }
out:
    if (i >= CdlMAXLEVEL) {
        if (D_80070C48 > 0) {
            printf("%s: path level (%d) error\n", name, i);
        }
        return NULL;
    }
    if (buf[0] == 0) {
        if (D_80070C48 > 0) {
            printf("%s: dir was not found\n", name);
        }
        return NULL;
    }
    *q = 0;
    if (func_80057904(dir) == 0) {
        if (D_80070C48 > 0) {
            printf("CdSearchFile: disc error\n");
        }
        return NULL;
    }
    if (D_80070C48 > 1) {
        printf("CdSearchFile: searching %s...\n", buf);
    }
    for (i = 0; i < CdlMAXFILE; i++) {
        if (D_801D98F8[i].name[0] == 0) {
            break;
        }
        if (func_8005757C(D_801D98F8[i].name, buf)) {
            if (D_80070C48 > 1) {
                printf("%s:  found\n", buf);
            }
            *fp = D_801D98F8[i];
            return &D_801D98F8[i];
        }
    }
    if (D_80070C48 > 0) {
        printf("%s: not found\n", buf);
    }
    return NULL;
}

int func_8005757C(char *a, char *b) {
    return strncmp(a, b, 12) == 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005759C);

int func_80057860(long parent, char *name) {
    int i;

    for (i = 0; i < CdlMAXDIR; i++) {
        if (D_801D9EF8[i].parent == 0) {
            break;
        }
        if (D_801D9EF8[i].parent == parent && strcmp(name, D_801D9EF8[i].name) == 0) {
            return i + 1;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80057904);

int func_80057BA0(int sectors, int sector, u_char *buf) {
    CdlLOC loc;

    CdIntToPos(sector, &loc);
    CdControl(CdlSetloc, (u_char *)&loc, 0);
    CdRead(sectors, (u_long *)buf, CdlModeSpeed);
    return CdReadSync(0, 0) == 0;
}

OBJECT_END(3);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013394);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013398);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_8001339C);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_800133B8);
