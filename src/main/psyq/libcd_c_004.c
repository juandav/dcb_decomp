#include "psyq.h"

extern CdlLOC D_801DBCF8;
extern long D_801DBCFC;
extern void (*D_801DBD08)(void);
extern u_long *D_801D98F0;

void data_ready_callback(void) {
    u_long *h = D_801D98F0 + D_801D98D8 * 8;

    *(u_short *)h = 2;
    D_801DBCF8 = *(CdlLOC *)&h[7];
    D_801DBCFC = h[2];
    D_801D98D8 = D_801D98D4;
    if (D_801DBD08 != NULL) {
        D_801DBD08();
    }
    D_801D98CC = 0;
}

extern long D_801DBD50;

extern CdlLOC D_801DBCF8;

extern long D_801DBCFC;

int StGetBackloc(CdlLOC *loc) {
    if (D_801DBD50 != 0) {
        return -1;
    }
    CdIntToPos(CdPosToInt(&D_801DBCF8) + 1, loc);
    return D_801DBCFC;
}

OBJECT_END(3);
