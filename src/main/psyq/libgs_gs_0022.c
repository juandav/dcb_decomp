#include "psyq.h"

typedef struct GsPosition {
    /* 0x0 */ short offx;
    /* 0x2 */ short offy;
} GsPosition;

extern short D_801DBE26;
extern short D_801DBE16;
extern short D_801DBE14;
extern GsPosition D_801DBD98;
extern short D_801DBD88[2];
extern short D_801DBD8C[2];
extern DRAWENV D_801DBDA0;

void GsSetDrawBuffOffset(void) {
    if (D_801DBE26 != 0) {
        D_801DBDA0.ofs[0] = D_801DBD98.offx + D_801DBD88[D_801DBE24];
        D_801DBE16 = 0;
        D_801DBE14 = 0;
        D_801DBDA0.ofs[1] = D_801DBD98.offy + D_801DBD8C[D_801DBE24];
        PutDrawEnv(&D_801DBDA0);
    } else {
        int x, y;

        x = D_801DBD98.offx + D_801DBD88[D_801DBE24 == 0];
        y = D_801DBD98.offy + D_801DBD8C[D_801DBE24 == 0];
        SetGeomOffset(x, y);
        D_801DBE14 = x;
        D_801DBE16 = y;
    }
}

OBJECT_END(2);
