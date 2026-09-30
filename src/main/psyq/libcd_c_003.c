#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_80070C60;

extern volatile u_char *D_80070B78;

extern volatile u_char *D_80070B84;

void StUnSetRing(void) {
    EnterCriticalSection();
    if (D_80070C60 == 1) {
        func_8006B0D4(NULL);
        func_8006B0B4(0);
    } else {
        func_8005A7A4(NULL);
        func_8005A3A4(0);
    }
    *D_80070B78 = 0;
    *D_80070B84 = 0;
    ExitCriticalSection();
}
