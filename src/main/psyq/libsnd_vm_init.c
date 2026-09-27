#include "psyq.h"

extern short D_801D9678;
extern char D_801D97A8[];
extern short D_801D9410[0xC0];
extern char D_801D93F0[0x18];
extern short D_801D9758;
extern u_char D_801D9700[16];
extern char D_801D96D4;
extern VmVoice D_801D8EB0[];
extern short D_801D96F8;
extern SpuReverbAttr D_801D95E0;
extern u_short D_801D8EA0;
extern u_short D_801D8EA2;
extern u_short D_801D95D8;
extern u_short D_801D8EA4;
extern u_short D_801D8EA6;
extern u_short D_801D8EA8;
extern u_short D_801D8EAA;
extern char D_801D9710;
extern short D_801D96C0;
extern long D_801D96D8;
extern short D_801D96C2;

void _spu_setInTransfer(int);
void _SsVmKeyOffNow(int);
void _SsVmFlush(void);

void _SsVmInit(char voiceCount) {
    SpuVoiceAttr attr;
    u_short i;
    u_int n;

    _spu_setInTransfer(0);
    D_801D9678 = 0;
    SpuInitMalloc(32, D_801D97A8);
    for (i = 0; i < 0xC0; i++) {
        *(D_801D9410 + i) = 0;
    }
    for (i = 0; i < 0x18; i++) {
        D_801D93F0[i] = 0;
    }
    D_801D9758 = 0;
    for (i = 0; i < 16; i++) {
        D_801D9700[i] = 0;
    }
    n = voiceCount;
    if (n >= 24) {
        D_801D96D4 = 24;
    } else {
        D_801D96D4 = n;
    }
    attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR | SPU_VOICE_PITCH | SPU_VOICE_WDSA | SPU_VOICE_ADSR_ADSR1 | SPU_VOICE_ADSR_ADSR2;
    attr.pitch = 0x1000;
    attr.addr = 0x1000;
    attr.adsr1 = 0x80FF;
    attr.volume.left = 0;
    attr.volume.right = 0;
    attr.adsr2 = 0x4000;
    for (i = 0; i < D_801D96D4; i++) {
        D_801D8EB0[i].unk2 = 24;
        D_801D8EB0[i].unk0 = 0xFF;
        D_801D8EB0[i].unk1D = 0;
        D_801D8EB0[i].unk4 = 0;
        D_801D8EB0[i].envx = 0;
        D_801D8EB0[i].unk10 = -1;
        D_801D8EB0[i].unk12 = 0;
        D_801D8EB0[i].prog = 0;
        D_801D8EB0[i].tone = 0xFF;
        D_801D8EB0[i].unk8 = 0;
        D_801D8EB0[i].unkC = 0;
        D_801D8EB0[i].unkA = 0x40;
        D_801D8EB0[i].unk36 = 0;
        D_801D8EB0[i].autoVol = 0;
        D_801D8EB0[i].unk20 = 0;
        D_801D8EB0[i].unk22 = 0;
        D_801D8EB0[i].unk24 = 0;
        D_801D8EB0[i].autoPan = 0;
        D_801D8EB0[i].unk2C = 0;
        D_801D8EB0[i].unk2E = 0;
        D_801D8EB0[i].unk30 = 0;
        D_801D8EB0[i].startPan = 0;
        D_801D8EB0[i].startVol = 0;
        attr.voice = 1 << i;
        SpuSetVoiceAttr(&attr);
        D_801D96F8 = i;
        _SsVmKeyOffNow(1);
    }
    D_801D95E0.mask = 0;
    D_801D95E0.depth.left = 0x3FFF;
    D_801D95E0.depth.right = 0x3FFF;
    D_801D95E0.mode = 0;
    D_801D8EA0 = 0;
    D_801D8EA2 = 0;
    D_801D95D8 = 0;
    D_801D8EA4 = 0;
    D_801D8EA6 = 0;
    D_801D8EA8 = 0;
    D_801D8EAA = 0;
    D_801D9710 = 0;
    D_801D96C0 = 0;
    D_801D96D8 = 0;
    D_801D96C2 = 0x80;
    _SsVmFlush();
}
