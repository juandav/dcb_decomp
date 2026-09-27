#include "psyq.h"

extern ProgAtr *D_801D96C4;

long _SsVmSetProgVol(short vab, short prog, u_char vol) {
    if (_SsVmVSetUp(vab, prog) != 0) {
        return -1;
    }
    D_801D96C4[prog].mvol = vol;
    return D_801D96C4[prog].mvol;
}
