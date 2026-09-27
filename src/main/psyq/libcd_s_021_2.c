#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

int func_8005B304(void) {
    return CD_getsector2() == 0;
}

extern volatile u_char *D_80071038;
extern volatile u_char *D_8007103C;
extern volatile u_long *D_80071040;
extern volatile u_long *D_80071044;
extern volatile u_long *D_80071048;
extern volatile u_long *D_8007104C;
extern volatile u_long *D_80071050;
extern volatile u_long *D_80071054;

int CD_getsector2(void *madr, int size) {
    volatile u_long dummy;

    *D_80071038 = 0;
    *D_8007103C = 0x80;
    *D_80071044 = 0x21020843;
    *D_80071040 = 0x1325;
    *D_80071048 |= 0x8000;
    *D_80071050 = (u_long)madr;
    *D_80071054 = size | 0x10000;
    while (!(*D_80071038 & 0x40)) {
    }
    *D_8007104C = 0x11400100;
    dummy = *D_8007104C;
    return 0;
}

OBJECT_END(1);

void func_8005B414(void) {
    CD_datasync();
}
