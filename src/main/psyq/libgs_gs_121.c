#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern short D_801DBE16;

extern short D_801DBE14;

void gte_init(void) {
    InitGeom();
    func_80062B44(0, 0, 0);
    SetGeomOffset(0, 0);
    D_801DBE16 = 0;
    D_801DBE14 = 0;
}

OBJECT_END(1);

INCLUDE_ASM("main/nonmatchings/psyq", func_80062B44);
