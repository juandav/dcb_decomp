#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_801DBD50;
void func_8005A7A4(void (*func)());
void func_8005A3A4(void (*func)());
void data_ready_callback(void);
void func_80057C98(void);

int CdRead2(long mode) {
    u_char param[4];

    param[0] = mode;
    CdControl(CdlSetmode, param, 0);
    if (mode & 0x100) {
        if (mode & 0x20) {
            D_801DBD50 = 0;
        } else {
            D_801DBD50 = 1;
        }
        func_8005A7A4(data_ready_callback);
        func_8005A3A4(func_80057C98);
    }
    return CdControl(CdlReadS, NULL, NULL);
}

void func_80057C98(void) {
    StCdInterrupt();
}

OBJECT_END(3);
