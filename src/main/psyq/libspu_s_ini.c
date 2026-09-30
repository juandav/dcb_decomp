#include "psyq.h"

extern long D_8006EF98;
extern long D_8006EF9C;
extern long D_8006EFA0;
typedef struct {
    long mode;
    short left;
    short right;
    long delay;
    long feedback;
} SpuRevAttr8006EFA8;
extern SpuRevAttr8006EFA8 D_8006EFA8;
extern long D_8006EFB8;
extern long D_8006EFBC;
extern u_short D_8006EFC0[24];
extern long D_8006EFF0;
extern long D_8006F004;
extern long D_8006F008;
extern long D_8006F00C;
extern long D_8006EF94;
extern long D_8006EF40;
extern long D_8006EF90;
extern long D_8006F534;

void _SpuInit(int mode) {
    int i;

    ResetCallback();
    _spu_init(mode);
    if (mode == 0) {
        u_short n = 0xC000;

        for (i = 23; i >= 0; i--) {
            D_8006EFC0[i] = n;
        }
    }
    SpuStart();
    D_8006EF98 = 0;
    D_8006EF9C = 0;
    D_8006EFA8.mode = 0;
    D_8006EFA8.left = 0;
    D_8006EFA8.right = 0;
    D_8006EFA8.delay = 0;
    D_8006EFA8.feedback = 0;
    D_8006EFA0 = D_8006F534;
    _spu_FsetRXX(0xD1, D_8006F534, 0);
    D_8006F004 = 0;
    D_8006F008 = 0;
    D_8006F00C = 0;
    D_8006EF94 = 0;
    D_8006EF40 = 0;
    D_8006EF90 = 0;
    D_8006EFBC = 0;
    D_8006EFB8 = 0;
    D_8006EFF0 = 0;
}

extern long D_8006EFF4;

extern long D_8006EF8C;

void SpuStart(void) {
    long event;

    if (D_8006EFF4 == 0) {
        D_8006EFF4 = 1;
        EnterCriticalSection();
        _SpuDataCallback(_spu_FiDMA);
        event = OpenEvent(0xF0000009, 0x20, 0x2000, NULL);
        D_8006EF8C = event;
        EnableEvent(event);
        ExitCriticalSection();
    }
}

OBJECT_END(2);
