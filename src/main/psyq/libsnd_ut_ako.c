#include "psyq.h"

extern char D_801D96D4;
extern long D_8006F564;
extern short D_801D96F8;
extern VmVoice D_801D8EB0[];
void _SsVmKeyOffNow(int);

void SsUtAllKeyOff(short mode) {
    SpuVoiceAttr attr;
    short i;

    attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR | SPU_VOICE_PITCH | SPU_VOICE_WDSA | SPU_VOICE_ADSR_ADSR1 |
                SPU_VOICE_ADSR_ADSR2;
    attr.pitch = 0x1000;
    attr.addr = 0x1000;
    attr.adsr1 = 0x80FF;
    attr.volume.left = 0;
    attr.volume.right = 0;
    attr.adsr2 = 0x4000;
    for (i = 0; i < D_801D96D4; i++) {
        if (!(D_8006F564 & (1 << i))) {
            D_801D8EB0[i].unk2 = 24;
            D_801D8EB0[i].envx = 0;
            D_801D8EB0[i].unk10 = 255;
            D_801D8EB0[i].unk12 = 0;
            D_801D8EB0[i].prog = 0;
            D_801D8EB0[i].tone = 255;
            D_801D8EB0[i].unk36 = 0;
            attr.voice = 1 << i;
            SpuSetVoiceAttr(&attr);
            D_801D96F8 = i;
            _SsVmKeyOffNow(1);
        }
    }
}

OBJECT_END(3);
