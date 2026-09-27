#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_801D98E0;

extern long D_801D98B8;

extern long D_801D98C8;

extern long D_801D98C0;

extern void (*D_801DBD08)();

extern void (*D_801DBD0C)();

void StSetStream(u_long mode, u_long start_frame, u_long end_frame, void (*func1)(), void (*func2)()) {
    func_800580D4(1, start_frame, end_frame);
    D_801D98E0 = 0;
    D_801DBD08 = func1;
    D_801D98B8 = mode & 1;
    D_801D98C8 = 0;
    D_801D98C0 = 0;
    D_801D98B4 = 0;
    D_801D98B0 = 0;
    D_801DBD0C = func2;
}

OBJECT_END(3);
