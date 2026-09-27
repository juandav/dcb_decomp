#include "psyq.h"

/* a 4-byte little-endian sector number, unaligned in the path table */
typedef union {
    long addr;
    struct {
        u_char c[4];
    } b;
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

int func_8005759C(void) {
    CdLBA lba;
    u_char *p;
    int i;

    if (func_80057BA0(1, 16, D_801DB4F8) != 1) {
        if (D_80070C48 > 0) {
            printf("CD_newmedia: Read error in cd_read(PVD)\n");
        }
        return 0;
    }
    if (strncmp(&D_801DB4F8[1], "CD001", 5) != 0) {
        if (D_80070C48 > 0) {
            printf("CD_newmedia: Disc format error in cd_read(PVD)\n");
        }
        return 0;
    }
    (&lba)->b = ((CdLBA *)&D_801DB4F8[0x8C])->b;
    if (func_80057BA0(1, lba.addr, D_801DB4F8) != 1) {
        if (D_80070C48 > 0) {
            printf("CD_newmedia: Read error (PT:%08x)\n", lba.addr);
        }
        return 0;
    }
    if (D_80070C48 > 1) {
        printf("CD_newmedia: sarching dir..\n");
    }
    i = 0;
    p = D_801DB4F8;
    while (p < D_801DB4F8 + 0x800) {
        if (p[0] == 0) {
            break;
        }
        D_801D9EF8[i].lba.b = ((CdLBA *)&p[2])->b;
        D_801D9EF8[i].parent = p[6];
        D_801D9EF8[i].id = i + 1;
        memcpy(D_801D9EF8[i].name, &p[8], p[0]);
        D_801D9EF8[i].name[p[0]] = 0;
        p += 8 + p[0] + p[0] % 2;
        if (D_80070C48 > 1) {
            printf("\t%08x,%04x,%04x,%s\n", D_801D9EF8[i].lba.addr, D_801D9EF8[i].id, D_801D9EF8[i].parent,
                   D_801D9EF8[i].name);
        }
        if (++i >= CdlMAXDIR) {
            break;
        }
    }
    if (i < CdlMAXDIR) {
        D_801D9EF8[i].parent = 0;
    }
    D_80070B58 = 0;
    if (D_80070C48 > 1) {
        printf("CD_newmedia: %d dir entries found\n", i);
    }
    return 1;
}

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

int func_80057904(int dir) {
    CdLBA lba;
    u_short *name; /* halfword view: "." and ".." are copied with lhu/sh */
    u_char *p;
    int i;

    if (dir == D_80070B58) {
        return 1;
    }
    if (func_80057BA0(1, (D_801D9EF8 - 1)[dir].lba.addr, D_801DB4F8) != 1) {
        if (D_80070C48 > 0) {
            printf("CD_cachefile: dir not found\n");
        }
        return -1;
    }
    if (D_80070C48 > 1) {
        printf("CD_cachefile: searching...\n");
    }
    i = 0;
    p = D_801DB4F8;
    while (p < D_801DB4F8 + 0x800) {
        if (p[0] == 0) {
            break;
        }
        (&lba)->b = ((CdLBA *)&p[2])->b;
        CdIntToPos(lba.addr, &D_801D98F8[i].pos);
        ((CdLBA *)&D_801D98F8[i].size)->b = ((CdLBA *)&p[10])->b;
        switch (i) {
        case 0:
            name = (u_short *)D_801D98F8[i].name;
            __builtin_strcpy((char *)name, ".");
            break;
        case 1:
            name = (u_short *)D_801D98F8[i].name;
            __builtin_strcpy((char *)name, "..");
            break;
        default:
            memcpy(D_801D98F8[i].name, &p[0x21], p[0x20]);
            D_801D98F8[i].name[p[0x20]] = 0;
            break;
        }
        if (D_80070C48 > 1) {
            printf("\t(%02x:%02x:%02x) %8d %s\n", D_801D98F8[i].pos.minute, D_801D98F8[i].pos.second,
                   D_801D98F8[i].pos.sector, D_801D98F8[i].size, D_801D98F8[i].name);
        }
        p += p[0];
        if (++i >= CdlMAXFILE) {
            break;
        }
    }
    D_80070B58 = dir;
    if (i < CdlMAXFILE) {
        D_801D98F8[i].name[0] = 0;
    }
    if (D_80070C48 > 1) {
        printf("CD_cachefile: %d files found\n", i);
    }
    return 1;
}

int func_80057BA0(int sectors, int sector, u_char *buf) {
    CdlLOC loc;

    CdIntToPos(sector, &loc);
    CdControl(CdlSetloc, (u_char *)&loc, 0);
    CdRead(sectors, (u_long *)buf, CdlModeSpeed);
    return CdReadSync(0, 0) == 0;
}

OBJECT_END(3);


/* the object's rodata ends with 4 bytes of padding */
__asm__(".section .rodata\n\t.space 4\n\t.section .text\n");
