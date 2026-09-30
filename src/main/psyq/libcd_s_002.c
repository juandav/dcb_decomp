#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_80070C40;

#define itob(i) ((i) / 10 * 16 + (i) % 10)

CdlLOC *CdIntToPos(int i, CdlLOC *p) {
    int min;
    int sec;
    int sector;

    i += 150;
    sec = i / 75;
    sector = i % 75;
    min = sec / 60;
    sec = sec % 60;
    p->sector = itob(sector);
    p->second = itob(sec);
    p->minute = itob(min);
    return p;
}

OBJECT_END(3);

extern long D_80070C48, D_80070C44, D_80070C40;

int CdSetDebug(int level) {
    long old = D_80070C48;

    D_80070C48 = level;
    return old;
}

OBJECT_END(3);

int CdSync(int mode, u_char *result) {
    return CD_sync(mode, result);
}

void func_8005A384(void) {
    CD_ready();
}

long func_8005A3A4(long v) {
    long old = D_80070C44;

    D_80070C44 = v;
    return old;
}

OBJECT_END(3);
