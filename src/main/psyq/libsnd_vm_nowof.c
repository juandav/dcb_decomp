#include "psyq.h"

extern short D_801D96F8;
extern VmVoice D_801D8EB0[];
extern u_short D_801D95D8;
extern u_short D_801D95DA;
extern u_short D_801D8EA0;
extern u_short D_801D8EA2;

void _SsVmKeyOffNow(void) {
    u_short voice = D_801D96F8;
    u_short hi;
    u_short lo;

    if (voice < 16) {
        lo = 1 << voice;
        hi = 0;
    } else {
        lo = 0;
        hi = 1 << (voice - 16);
    }
    D_801D8EB0[voice].unk1D = 0;
    D_801D8EB0[voice].unk4 = 0;
    D_801D8EB0[voice].unk0 = 0;
    D_801D95D8 |= lo;
    D_801D95DA |= hi;
    D_801D8EA0 &= ~D_801D95D8;
    D_801D8EA2 &= ~D_801D95DA;
}
