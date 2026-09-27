#include "psyq.h"

extern u_char D_801D9700[];
extern ProgAtr *D_801D96C4;

short SsUtGetProgAtr(short vabId, short progNum, ProgAtr *progatrptr) {
    if (D_801D9700[vabId] == 1) {
    _SsVmVSetUp(vabId, progNum);
    progatrptr->tones = D_801D96C4[progNum].tones;
    progatrptr->mvol = D_801D96C4[progNum].mvol;
    progatrptr->prior = D_801D96C4[progNum].prior;
    progatrptr->mode = D_801D96C4[progNum].mode;
    progatrptr->mpan = D_801D96C4[progNum].mpan;
    progatrptr->attr = D_801D96C4[progNum].attr;
        return 0;
    }
    return -1;
}

OBJECT_END(3);
