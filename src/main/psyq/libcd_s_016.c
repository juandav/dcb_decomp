#include "psyq.h"

extern long D_80070C40;
extern long D_80070F58[];
int CD_cw(u_char com, u_char *param, u_char *result, int async);
int CD_sync(int mode, u_char *result);

static __inline__ int cd_cw(u_char com, u_char *param, u_char *result, int async) {
    long old = D_80070C40;
    int count = 4;

    while (count--) {
        D_80070C40 = 0;
        if (com != CdlNop && (*(u_char *)&D_80070C4C & CdlStatShellOpen)) {
            CD_cw(CdlNop, NULL, NULL, 0);
        }
        if (param == NULL || D_80070F58[com] == 0 || CD_cw(CdlSetloc, param, result, 0) == 0) {
            D_80070C40 = old;
            if (CD_cw(com, param, result, async) == 0) {
                return 0;
            }
        }
    }
    D_80070C40 = old;
    return -1;
}

int CdControl(u_char com, u_char *param, u_char *result) {
    return cd_cw(com, param, result, 0) == 0;
}

int CdControlF(u_char com, u_char *param) {
    return cd_cw(com, param, NULL, 1) == 0;
}

int CdControlB(u_char com, u_char *param, u_char *result) {
    if (cd_cw(com, param, result, 0)) {
        return 0;
    }
    return CD_sync(0, result) == CdlComplete;
}

OBJECT_END(1);

int CdMix(CdlATV *vol) {
    CD_vol(vol);
    return 1;
}

int func_8005A7A4(void (*func)()) {
    return DMACallback(3, func);
}

OBJECT_END(3);
