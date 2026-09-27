#include "psyq.h"

extern volatile u_char *D_80071018;
extern volatile u_char *D_8007101C;
extern volatile u_long *D_80071020;
extern volatile u_long *D_80071024;
extern volatile u_long *D_80071028;
extern volatile u_long *D_8007102C;
extern volatile u_long *D_80071030;
extern volatile u_long *D_80071034;

int CD_getsector(void *madr, int size) {
    *D_80071018 = 0;
    *D_8007101C = 0x80;
    *D_80071024 = 0x20943;
    *D_80071020 = 0x1323;
    *D_80071028 |= 0x8000;
    *D_80071030 = (u_long)madr;
    *D_80071034 = size | 0x10000;
    while (!(*D_80071018 & 0x40)) {
    }
    *D_8007102C = 0x11000000;
    while (*D_8007102C & 0x1000000) {
    }
    *D_80071020 = 0x1325;
    return 0;
}
