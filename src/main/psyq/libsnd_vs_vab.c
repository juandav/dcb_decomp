#include "psyq.h"

extern u_char D_801D9700[];
extern u_long D_801D9760[];
extern u_short D_801D9758;

void SsVabClose(short vabId) {
    int status;

    if ((u_short)vabId < 16) {
        status = D_801D9700[vabId];
        if (status < 3 && status != 0) {
            SpuFree(D_801D9760[vabId]);
            D_801D9700[vabId] = 0;
            D_801D9758--;
            if (_spu_getInTransfer() == 1) {
                _spu_setInTransfer(0);
            }
        }
    }
}

OBJECT_END(3);
