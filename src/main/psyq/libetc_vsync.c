#include "psyq.h"

/* VSync() and its wait loop read the counter as volatile, trapIntrVSync() does not */
extern volatile long D_80070AE8;
extern volatile long *D_8006F9B0;
extern volatile long *D_8006F9B4;
extern volatile long D_8006F9B8;
extern long D_8006F9BC;
void func_8005655C(long count, long timeout);

int VSync(int mode) {
    volatile long count;
    long status = *D_8006F9B0;
    long delta;
    long target;
    volatile long *gpu;

    do {
        count = *D_8006F9B4;
    } while (count != *D_8006F9B4);
    delta = (count - D_8006F9B8) & 0xFFFF;
    if (mode < 0) {
        return D_80070AE8;
    }
    if (mode == 1) {
        return delta;
    }
    if (mode > 0) {
        target = D_8006F9BC + (mode - 1);
    } else {
        target = D_8006F9BC;
    }
    func_8005655C(target, mode > 0 ? mode - 1 : 0);
    status = *D_8006F9B0;
    func_8005655C(D_80070AE8 + 1, 1);
    if (status & 0x400000) {
        gpu = D_8006F9B0;
        while (!((status ^ *gpu) & 0x80000000)) {
        }
    }
    D_8006F9BC = D_80070AE8;
    do {
        D_8006F9B8 = *D_8006F9B4;
    } while (D_8006F9B8 != *D_8006F9B4);
    return delta;
}

void func_8005655C(long count, long timeout) {
    volatile long t = timeout << 15;

    while (D_80070AE8 < count) {
        if (--t == -1) {
            puts("VSync: timeout\n");
            ChangeClearPad(0);
            ChangeClearRCnt(3, 0);
            return;
        }
    }
}
