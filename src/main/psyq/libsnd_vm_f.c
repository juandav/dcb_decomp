#include "psyq.h"

extern long D_801D9594;
extern u_long D_801D9598[];
extern char D_801D96D4;
extern char D_801D9710;
extern VmVoice D_801D8EB0[];
extern u_short D_801D95D8;
extern u_short D_801D95DA;
extern u_short D_801D8EA0;
extern u_short D_801D8EA2;
extern u_short D_801D8EA4;
extern u_short D_801D8EA6;
extern u_short D_801D8EA8;
extern u_short D_801D8EAA;
extern void (*D_801D9590)(int voice);
extern void (*D_801D9408)(int voice);
extern u_char D_801D93F0[];
extern u_short D_801D9410[];
void func_80052AB0(int voice, u_short *envx);

void _SsVmFlush(void) {
    SpuVoiceAttr attr;
    u_long mask;
    long vmask;
    u_long bits;
    int i;
    short lo;
    short hi;

    D_801D9594 = (D_801D9594 + 1) & 0xF;
    D_801D9598[D_801D9594] = 0;
    i = 0;
    if (D_801D96D4 > 0) do {
        func_80052AB0(i, &D_801D8EB0[i].envx);
        if (D_801D8EB0[i].envx == 0) {
            D_801D9598[D_801D9594] |= 1 << i;
        }
    } while (++i < D_801D96D4);
    if (D_801D9710 == 0) {
        mask = 0xFFFFFFFF;
        for (i = 0; i < 15; i++) {
            mask &= D_801D9598[i];
        }
        i = 0;
        if (D_801D96D4 > 0) do {
            if (mask & (1 << i)) {
                if (D_801D8EB0[i].unk1D == 2) {
                    if (i < 16) {
                        lo = 1 << i;
                        hi = 0;
                    } else {
                        lo = 0;
                        hi = 1 << (i - 16);
                    }
                    SpuSetNoiseVoice(SPU_OFF, ((hi & 0xFF) << 16) | lo);
                }
                D_801D8EB0[i].unk1D = 0;
            }
        } while (++i < D_801D96D4);
    }
    D_801D8EA0 &= ~D_801D95D8;
    D_801D8EA2 &= ~D_801D95DA;
    for (i = 0; i < 24; i++) {
        if (D_801D8EB0[i].autoVol != 0) {
            D_801D9590(i);
        }
        if (D_801D8EB0[i].autoPan != 0) {
            D_801D9408(i);
        }
    }
    for (i = 0; i < 24; i++) {
        attr.mask = 0;
        attr.voice = 1 << i;
        if (D_801D93F0[i] & 1) {
            attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR;
            attr.volume.left = D_801D9410[i * 8 + 0];
            attr.volume.right = D_801D9410[i * 8 + 1];
        }
        if (D_801D93F0[i] & 4) {
            attr.mask |= SPU_VOICE_PITCH;
            attr.pitch = D_801D9410[i * 8 + 2];
        }
        if (D_801D93F0[i] & 8) {
            attr.mask |= SPU_VOICE_WDSA;
            attr.addr = D_801D9410[i * 8 + 3] << 3;
        }
        if (D_801D93F0[i] & 0x10) {
            attr.mask |= SPU_VOICE_ADSR_ADSR1 | SPU_VOICE_ADSR_ADSR2;
            attr.adsr1 = D_801D9410[i * 8 + 4];
            attr.adsr2 = D_801D9410[i * 8 + 5];
        }
        if (attr.mask != 0) {
            SpuSetVoiceAttr(&attr);
        }
        D_801D93F0[i] = 0;
    }
    SpuSetKey(SPU_OFF, ((D_801D95DA & 0xFF) << 16) | D_801D95D8);
    SpuSetKey(SPU_ON, ((D_801D8EA2 & 0xFF) << 16) | D_801D8EA0);
    vmask = 0xFFFFFF >> (24 - D_801D96D4);
    bits = ((D_801D8EA6 << 16) | D_801D8EA4) & vmask;
    SpuSetReverbVoice(SPU_BIT, bits | (SpuGetReverbVoice() & ~vmask));
    bits = ((D_801D8EAA << 16) | D_801D8EA8) & vmask;
    SpuSetNoiseVoice(SPU_BIT, bits | (SpuGetNoiseVoice() & ~vmask));
    D_801D95D8 = 0;
    D_801D95DA = 0;
    D_801D8EA0 = 0;
    D_801D8EA2 = 0;
    D_801D8EA8 = 0;
    D_801D8EAA = 0;
}

OBJECT_END(3);
