#include "psyq.h"

typedef struct SvmCur {
    /* 0x00 */ u8 unk0[0xF];
    /* 0x0F */ char prior;
} SvmCur;

extern SvmCur D_801D96E0;
extern char D_801D96D4;
extern long D_8006F564;
extern VmVoice D_801D8EB0[];

u_char _SsVmAlloc(void) {
    u_char count;
    u_short age;
    u_short envx;
    u_char i;
    u_char channel;
    u_char best;
    u_short prior;

    channel = 99;
    envx = 0xFFFF;
    count = 0;
    age = 0;
    best = 99;
    prior = D_801D96E0.prior;
    for (i = 0; i < D_801D96D4; i++) {
        if (!(D_8006F564 & (1 << i))) {
            if (D_801D8EB0[i].unk1D == 0 && D_801D8EB0[i].envx == 0) {
                channel = i;
                break;
            }
            if (D_801D8EB0[i].prior < prior) {
                prior = D_801D8EB0[i].prior;
                best = i;
                envx = D_801D8EB0[i].envx;
                age = D_801D8EB0[i].unk2;
                count = 1;
            } else if (D_801D8EB0[i].prior == prior) {
                count++;
                if (D_801D8EB0[i].envx < envx) {
                    age = D_801D8EB0[i].unk2;
                    envx = D_801D8EB0[i].envx;
                    best = i;
                } else if (D_801D8EB0[i].envx == envx) {
                    if (age < D_801D8EB0[i].unk2) {
                        age = D_801D8EB0[i].unk2;
                        best = i;
                    }
                }
            }
        }
    }
    if (channel == 99) {
        if (count == 0) {
            channel = D_801D96D4;
        } else {
            channel = best;
        }
    }
    if (channel < D_801D96D4) {
        for (i = 0; i < D_801D96D4; i++) {
            if (!(D_8006F564 & (1 << i))) {
                D_801D8EB0[i].unk2++;
            }
        }
        D_801D8EB0[channel].unk2 = 0;
        D_801D8EB0[channel].prior = D_801D96E0.prior;
        D_801D8EB0[channel].autoPan = 0;
        D_801D8EB0[channel].autoVol = 0;
    }
    return channel;
}
