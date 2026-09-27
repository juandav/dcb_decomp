#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", __fixsfsi);

INCLUDE_ASM("asm/main/nonmatchings/psyq", __floatsisf);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _err_math);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuClearReverbWorkArea);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80012FBC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_init);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004AC20);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_FiDMA);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_Fr_);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_t);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_Fw);

extern u_long *D_8006EF34;

















u_long _spu_Fr(char *addr, u_long size) {
    _spu_t(2, D_8006EF3C << D_8006EF4C);
    _spu_t(0);
    _spu_t(3, addr, size);
    return size;
}

extern u_long *D_8006EF34;

















void _spu_FsetRXX(int reg, u_long value, int mode) {
    if (mode == 0) {
        D_8006EF24[reg] = value;
    } else {
        D_8006EF24[reg] = value >> D_8006EF4C;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_FsetRXXa);

extern u_long *D_8006EF34;

















u_long _spu_FgetRXXa(int reg, int mode) {
    u_short v = D_8006EF24[reg];

    if (mode == -1) {
        return v;
    }
    return v << D_8006EF4C;
}

extern u_long *D_8006EF34;

















void _spu_FsetPCR(int flag) {
    *D_8006EF34 &= 0xFFF8FFFF;
    if (flag) {
        *D_8006EF34 |= 0x30000;
    } else {
        *D_8006EF34 |= 0x50000;
    }
}

extern u_long *D_8006EF34;

















void func_8004B428(void) {
    *D_8006EF38 = (*D_8006EF38 & 0xF0FFFFFF) | 0x20000000;
}

extern u_long *D_8006EF34;

















void func_8004B450(void) {
    *D_8006EF38 = (*D_8006EF38 & 0xF0FFFFFF) | 0x22000000;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_Fw1ts);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SpuInit);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuStart);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SpuDataCallback);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SpuIsInAllocateArea);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SpuIsInAllocateArea_);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuSetVoiceAttr);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_note2pitch);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_pitch2note);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuSetCommonAttr);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004C300);

void SsSeqClose(short seq) {
    func_8004C300(seq);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSepClose);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsInit);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsInit);

void func_8004C5B0(void) {
    _SpuInit(0);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSeqOpen);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContBankChange);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContDataEntry);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContMainVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContPanpot);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContExpression);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContDamper);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContExternal);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContNrpn1);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContNrpn2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContRpn1);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContRpn2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsContResetAll);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr1);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr4);

void _SsUtResolveADSR(u_short adsr1, u_short adsr2, SsADSR *adsr) {
    adsr->arMode = adsr1 & 0x8000;
    adsr->srMode = adsr2 & 0x8000;
    adsr->srDir = adsr2 & 0x4000;
    adsr->rrMode = adsr2 & 0x20;
    adsr->ar = (adsr1 >> 8) & 0x7F;
    adsr->dr = (adsr1 >> 4) & 0xF;
    adsr->sl = adsr1 & 0xF;
    adsr->sr = (adsr2 >> 6) & 0x7F;
    adsr->rr = adsr2 & 0x1F;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsUtBuildADSR);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr5);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr6);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr7);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr8);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr9);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr10);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr11);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr12);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr13);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr14);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004DF40);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetNrpnVabAttr16);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004DFA0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004DFD0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004E000);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetPitchBend);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetControlChange);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsGetMetaEvent);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsNoteOn);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSetProgramChange);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsReadDeltaValue);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsInitSoundSeq);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSeqPlay);

INCLUDE_ASM("asm/main/nonmatchings/psyq", Snd_SetPlayMode);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSetSerialAttr);

void SsSetMVol(short left, short right) {
    SpuCommonAttr attr;

    attr.mask = 3;
    attr.mvol.left = left * 129;
    attr.mvol.right = right * 129;
    SpuSetCommonAttr(&attr);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004ECA0);

extern long D_8005B85C;
extern void (*D_8005B850[2])(void);







void SsStart(void) {
    func_8004ECA0(1);
}

extern long D_8005B85C;
extern void (*D_8005B850[2])(void);







void SsStart2(void) {
    func_8004ECA0(0);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004EF10);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004EF5C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSeqCalledTbyT);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSndCrescendo);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSndPause);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSndPlay);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSeqPlay);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSeqGetEof);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsGetSeqData);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSndNextSep);

void _SsSndReplay(short seq, short sep) {
    SeqStruct *score = &D_801D8618[seq][sep];

    score->unk14 = 1;
    D_801D8618[seq][sep].flags &= ~8;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSndStop);

void SsSeqStop(short seq) {
    _SsSndStop(seq, 0);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSepStop);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSetSerialVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSetTableSize);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSetTickMode);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSndSetVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSeqSetVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSepSetVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSeqGetVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSndTempo);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtAllKeyOff);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtGetProgAtr);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtGetVagAtr);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtKeyOnV);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtKeyOffV);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtSetReverbDelay);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuSetReverbModeParam);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_setReverbAttr);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtSetReverbDepth);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtSetReverbType);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuSetReverb);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtSetReverbFeedback);

void func_80051C70(void) {
    SpuSetReverb(0);
}

void func_80051C90(void) {
    SpuSetReverb(1);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtSetVagAtr);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmDoAllocate);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80052050);

void func_80052060(void) {
    D_801D9678 = 2;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmFlush);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuSetNoiseVoice);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SpuSetAnyVoice);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuGetNoiseVoice);

u_long _SpuGetAnyVoice(int lo, int hi) {
    u_long h = D_8006EF24[hi] & 0xFF;

    return D_8006EF24[lo] | (h << 16);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuSetReverbVoice);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuGetReverbVoice);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuSetKey);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80052AB0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmInit);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuInitMalloc);

void _spu_setInTransfer(int mode) {
    if (mode == 1) {
        D_8006EF58 = 0;
    } else {
        D_8006EF58 = 1;
    }
}

int _spu_getInTransfer(void) {
    return D_8006EF58 != 1;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmKeyOn);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmKeyOff);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmSeKeyOn);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmSeKeyOff);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmAlloc);

INCLUDE_ASM("asm/main/nonmatchings/psyq", note2pitch);

INCLUDE_ASM("asm/main/nonmatchings/psyq", note2pitch2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsPitchFromNote);

INCLUDE_ASM("asm/main/nonmatchings/psyq", vmNoiseOn);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuSetNoiseClock);

INCLUDE_ASM("asm/main/nonmatchings/psyq", vmNoiseOff);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmKeyOffNow);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmKeyOnNow);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmPBVoice);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmPitchBend);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmSetProgVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmSetSeqVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmGetSeqVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmSeqKeyOff);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmSelectToneAndVag);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmSetVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmVSetUp);

extern short D_801D96C0;

void func_80055730(void) {
    D_801D96C0 = 1;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80055740);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsVabClose);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuFree);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_gcSPU);

short SsVabOpenHeadSticky(unsigned char *addr, short vabId, unsigned long sbaddr) {
    return _SsVabOpenHeadWithMode(addr, vabId, func_80055BE8, sbaddr);
}

short SsVabFakeHead(unsigned char *addr, short vabId, unsigned long sbaddr) {
    return _SsVabOpenHeadWithMode(addr, vabId, func_80055BE8, sbaddr);
}

int func_80055BE8(int arg0, int arg1) {
    return arg1;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVabOpenHeadWithMode);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsVabTransBody);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuRead);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuSetTransferStartAddr);

long SpuSetTransferMode(long mode) {
    long m;

    switch (mode) {
    case 0:
        m = 0;
        break;
    case 1:
        m = 1;
        break;
    default:
        m = 0;
        break;
    }
    D_8006EF94 = mode;
    D_8006EF40 = m;
    return m;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsVabTransCompleted);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuIsTransferCompleted);

INCLUDE_ASM("asm/main/nonmatchings/psyq", __SN_ENTRY_POINT);

INCLUDE_ASM("asm/main/nonmatchings/psyq", __main);

INCLUDE_ASM("asm/main/nonmatchings/psyq", __sn_cpp_structors);

INCLUDE_ASM("asm/main/nonmatchings/psyq", VSync);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005655C);

int ResetCallback(void) {
    return D_80070AA8->resetCallback();
}

void *InterruptCallback(int irq, void (*func)()) {
    return D_80070AA8->interruptCallback(irq, func);
}

void *DMACallback(int dma, void (*func)()) {
    return D_80070AA8->dmaCallback(dma, func);
}

int VSyncCallback(void (*func)()) {
    return (int)D_80070AA8->vsyncCallbacks(4, func);
}

void *VSyncCallbacks(int ch, void (*func)()) {
    return D_80070AA8->vsyncCallbacks(ch, func);
}

int StopCallback(void) {
    return D_80070AA8->stopCallback();
}

int RestartCallback(void) {
    return D_80070AA8->restartCallback();
}

int CheckCallback(void) {
    return D_8006FA22;
}

u_short GetIntrMask(void) {
    return *D_80070AB0;
}

u_short SetIntrMask(u_short mask) {
    u_short old = *D_80070AB0;

    *D_80070AB0 = mask;
    return old;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056788);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056860);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056A30);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056B78);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056C18);

void func_80056C90(long *p, int n) {
    int i = n - 1;

    if (n != 0) {
        do {
            *p++ = 0;
        } while (i-- != 0);
    }
}

void *startIntrVSync(void) {
    *D_80070AEC = 0x100;
    D_80070AE8 = 0;
    func_80056DA4((long *)D_80070AC8, 8);
    InterruptCallback(0, func_80056D0C);
    return func_80056D78;
}

void func_80056D0C(void) {
    int i;

    D_80070AE8++;
    for (i = 0; i < 8; i++) {
        if (D_80070AC8[i] != NULL) {
            D_80070AC8[i]();
        }
    }
}

void *func_80056D78(int index, void (*func)()) {
    void (*old)() = D_80070AC8[index];

    if (func != old) {
        D_80070AC8[index] = func;
    }
    return old;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056DA4);

void *startIntrDMA(void) {
    func_8005704C((long *)D_80070AFC, 8);
    *D_80070AF8 = 0;
    InterruptCallback(3, func_80056E20);
    return func_80056FA0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056E20);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056FA0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005704C);

extern long D_80070B28;

long SetVideoMode(long value) {
    long old = D_80070B28;

    D_80070B28 = value;
    return old;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", GetVideoMode);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StSetRing);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdInit);

int func_80057164(void) {
    if (CD_init() != 0) {
        return 0;
    }
    return CD_initvol() == 0;
}

void func_800571A0(void) {
    func_8006A784(0xF0000003, 0x20);
}

void func_800571C8(void) {
    func_8006A784(0xF0000003, 0x40);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800571F0);

int CdPosToInt(CdlLOC *p) {
    return (btoi(p->minute) * 60 + btoi(p->second)) * 75 + btoi(p->sector) - 150;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdSearchFile);

int func_8005757C(char *a, char *b) {
    return strncmp(a, b, 12) == 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005759C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80057860);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80057904);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80057BA0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdRead2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80057C98);

void StClearRing(void) {
    D_801D98DC = 0;
    D_801D98D8 = 0;
    D_801D98D4 = 0;
    D_801D98CC = 0;
    init_ring_status(0, D_801D98F4);
    D_801D98BC = 0;
    D_801D98B4 = 0;
    D_801D98B0 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", StUnSetRing);

INCLUDE_ASM("asm/main/nonmatchings/psyq", data_ready_callback);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StGetBackloc);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StSetStream);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StFreeRing);

INCLUDE_ASM("asm/main/nonmatchings/psyq", init_ring_status);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StGetNext);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800580D4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StCdInterrupt);

void func_80058A10(long *dst, long *src, u_long n) {
    u_long i = 0;

    if (n != 0) {
        do {
            *dst++ = *src++;
            i++;
        } while (i < n);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013394);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013398);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_8001339C);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_800133B8);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80058A3C);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013538);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013548);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80058BE4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_sync);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_ready);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_cw);

int CD_vol(CdlATV *vol) {
    *D_80070F04 = 2;
    *D_80070F14 = vol->val0;
    *D_80070F08 = vol->val1;
    *D_80070F04 = 3;
    *D_80070F10 = vol->val2;
    *D_80070F14 = vol->val3;
    *D_80070F08 = 0x20;
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_flush);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_initvol);

extern long D_80070C40;

void CD_initintr(void) {
    D_80070C44 = 0;
    D_80070C40 = 0;
    D_80070C50 = 0;
    D_80070C4C = 0;
    ResetCallback();
    InterruptCallback(2, func_8005A088);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_init);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_datasync);

extern int D_80070EE8;

void CD_set_test_parmnum(int num) {
    D_80070EE8 = num;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A088);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StRingStatus);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdIntToPos);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A344);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A364);

void func_8005A384(void) {
    CD_ready();
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A3A4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdControl);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdControlF);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdControlB);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A784);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A7A4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A7D4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A808);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005AA7C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005AB4C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdReadBreak);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdRead);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdReadSync);

int func_8005B174(void) {
    return *(u_char *)&D_80070C4C;
}

extern u_char D_80070C5C;

int func_8005B184(void) {
    return D_80070C5C;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005B194);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005B1A4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005B1C4);

int func_8005B1E4(void *madr, int size) {
    return CD_getsector(madr, size) == 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_getsector);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005B304);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_getsector2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005B414);

int rsin(int a) {
    if (a < 0) {
        return -sin_1(-a & 0xFFF);
    }
    return sin_1(a & 0xFFF);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", sin_1);

INCLUDE_ASM("asm/main/nonmatchings/psyq", rcos);

INCLUDE_ASM("asm/main/nonmatchings/psyq", csqrt_1);

INCLUDE_ASM("asm/main/nonmatchings/psyq", csqrt);

INCLUDE_ASM("asm/main/nonmatchings/psyq", catan);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005B864);

INCLUDE_ASM("asm/main/nonmatchings/psyq", InitGeom);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SquareRoot0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", InvSquareRoot);

INCLUDE_ASM("asm/main/nonmatchings/psyq", VectorNormalS);

INCLUDE_ASM("asm/main/nonmatchings/psyq", VectorNormal);

INCLUDE_ASM("asm/main/nonmatchings/psyq", VectorNormalSS);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005BA84);

INCLUDE_ASM("asm/main/nonmatchings/psyq", MatrixNormal);

INCLUDE_ASM("asm/main/nonmatchings/psyq", MulMatrix0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CompMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", PushMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", PopMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", MulMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", MulMatrix2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", ApplyMatrixSV);

INCLUDE_ASM("asm/main/nonmatchings/psyq", TransMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", ScaleMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetRotMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetLightMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005C444);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005C464);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005C484);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005C4A4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotTransPers);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotTransPers3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotTrans);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotTransPers4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotAverage4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotNclip3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotNclip4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotAverageNclip3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotAverageNclip4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", TransposeMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotMatrixYXZ);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTF3NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005D104);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotAverage3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTNF3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTG3NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTNG3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTF4L);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005DB44);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTF4NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTNF4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTG4NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTNG4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyFT3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyFT3A);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005E920);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyGT3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyGT3A);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005EDB0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyFT4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyFT4A);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005F2D4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyGT4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyGT4A);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005F8D0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", ratan2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _patch_gte);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005FBB0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005FBE4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", NormalColorCol3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005FC54);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastF3L);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastNF4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastF4NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastF4L);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastG3L);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastG4L);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTNF3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTF3NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTF3L);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTNF4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTF4NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTF4L);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTNG3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTG3NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTG3L);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTNG4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTG4NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDfastTG4L);

void GsInitGraph(u_short x, u_short y, u_short intmode, u_short dith, u_short vrammode) {
    func_80061958(x, y, intmode, dith, vrammode);
    gte_init();
    D_801DBE24 = 0;
    func_80061ADC(x, y);
    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80061958);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsInitGraph2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80061ADC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSortClear);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetDrawBuffOffset);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetDrawBuffClip);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSwapDispBuff);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsInitCoordinate2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetLsMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetLightMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsMulCoord3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", ApplyMatrixLV);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsInit3D);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsMapModelingData);

void func_80062484(void) {
    func_8005C4A4();
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetFlatLight);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006295C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800629C0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetColorMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetLightMode);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetAmbient);

INCLUDE_ASM("asm/main/nonmatchings/psyq", gte_init);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80062B44);

INCLUDE_ASM("asm/main/nonmatchings/psyq", Gssub_make_matrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80062C34);

extern long D_801DBF98;

long func_80062C44(void) {
    return D_801DBF98;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetRefView2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80063024);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80063110);

long func_800631D8(long value) {
    long bits = 0;

    while (value > 0) {
        value >>= 1;
        bits++;
    }
    return bits;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsMulCoord2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", gte_rotate_z_matrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsGetLw);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsGetLs);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsGetLws);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsLinkObject4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSortObject4);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_8001389C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", ResetGraph);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetGraphDebug);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetGraphQueue);

int GetGraphDebug(void) {
    return D_80076758.level;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", DrawSyncCallback);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetDispMask);

int DrawSync(int mode) {
    if (D_80076758.level >= 2) {
        D_80076754("DrawSync(%d)...\n", mode);
    }
    return D_80076750->sync(mode);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800649E8);

int ClearImage(RECT *rect, u_char r, u_char g, u_char b) {
    func_800649E8("ClearImage", rect);
    return D_80076750->addque(D_80076750->unkC, rect, 8, (b << 16) | (g << 8) | r);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", ClearImage2);

int LoadImage(RECT *rect, u_long *p) {
    func_800649E8("LoadImage", rect);
    return D_80076750->addque(D_80076750->unk20, rect, 8, (long)p);
}

int StoreImage(RECT *rect, u_long *p) {
    func_800649E8(D_800139D4, rect);
    return D_80076750->addque(D_80076750->unk1C, rect, 8, (long)p);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", MoveImage);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_800139D4);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_800139E0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", ClearOTag);

INCLUDE_ASM("asm/main/nonmatchings/psyq", ClearOTagR);

void DrawPrim(void *p) {
    int len = getlen(p);

    D_80076750->sync(0);
    D_80076750->unk14((u_long *)p + 1, len);
}

void DrawOTag(u_long *p) {
    if (D_80076758.level >= 2) {
        D_80076754(D_80013A1C, p);
    }
    D_80076750->addque(D_80076750->unk18, p, 0, 0);
}

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013A1C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", PutDrawEnv);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DrawOTagEnv);

DRAWENV *GetDrawEnv(DRAWENV *env) {
    memcpy((u_char *)env, (u_char *)&D_80076768, sizeof(DRAWENV));
    return env;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", PutDispEnv);

DISPENV *GetDispEnv(DISPENV *env) {
    memcpy((u_char *)env, (u_char *)&D_800767C4, sizeof(DISPENV));
    return env;
}

int GetODE(void) {
    return D_80076750->status() >> 31;
}

void SetDrawArea(DR_AREA *p, RECT *r) {
    setlen(p, 2);
    p->code[0] = func_80065C54(r->x, r->y);
    p->code[1] = func_80065CEC(r->x + r->w - 1, r->y + r->h - 1);
}

void SetDrawOffset(DR_OFFSET *p, u_short *ofs) {
    setlen(p, 2);
    p->code[0] = func_80065D84(ofs[0], ofs[1]);
    p->code[1] = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetDrawEnv);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800659C4);

u_long func_80065C34(int dfe, int dtd, int tpage) {
    return (dtd ? 0xE1000200 : 0xE1000000) | (dfe ? 0x400 : 0) | (tpage & 0x9FF);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80065C54);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80065CEC);

u_long func_80065D84(short x, short y) {
    return 0xE5000000 | ((y & 0x7FF) << 11) | (x & 0x7FF);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80065DA0);

extern u_long *D_80076860;

u_long func_80065E20(void) {
    return *D_80076860;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80065E38);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80065F18);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066148);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066384);

void func_80066604(u_long value) {
    *D_80076860 = value;
}

void func_80066618(void) {
}

int func_80066620(u_long *p, int n) {
    int i = n - 1;

    *D_80076860 = 0x04000000;
    if (n != 0) {
        do {
            *D_8007685C = *p++;
        } while (i-- != 0);
    }
    return 0;
}

void func_80066660(u_long addr) {
    *D_80076860 = 0x04000002;
    *D_80076864 = addr;
    *D_80076868 = 0;
    *D_8007686C = 0x01000401;
}

u_long func_800666A8(u_long cmd) {
    *D_80076860 = cmd | 0x10000000;
    return *D_8007685C & 0xFFFFFF;
}

int func_800666D8(int arg0, int arg1, int arg2) {
    return func_800666FC(arg0, arg1, 0, arg2);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800666FC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800669AC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066C0C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066D48);

void func_80066E84(void) {
    D_80076894 = VSync(-1) + 240;
    D_80076898 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066EB8);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066FFC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", LoadImage2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StoreImage2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", MoveImage2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DrawOTag2);

void _GPU_ResetCallback(void) {
    DMACallback(2, func_800669AC);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800674DC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetDefDrawEnv);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetDefDispEnv);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GetTPage);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067644);

INCLUDE_ASM("asm/main/nonmatchings/psyq", AddPrim);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetSemiTrans);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetShadeTex);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067704);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067724);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067744);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067764);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067784);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800677A4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800677C4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800677E4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067804);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067824);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067844);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067864);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067884);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800678A4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800678C4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800678E4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067904);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067924);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetLineG3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067974);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetLineG4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetDrawTPage);

INCLUDE_ASM("asm/main/nonmatchings/psyq", MargePrim);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetTexWindow);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetDrawStp);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetDrawMode);

extern u_long *D_801DD930;

int OpenTIM(u_long *addr) {
    D_801DD930 = addr;
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", ReadTIM);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067BE8);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTReset);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTGetEnv);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTPutEnv);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTin);

void DecDCTout(void) {
    func_80068144();
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTinSync);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCToutSync);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTinCallback);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCToutCallback);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067FC4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800680B4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068144);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800681D0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068264);

extern u_long *D_800769EC;

u_long func_800682F8(void) {
    return *D_800769EC;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068310);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTvlcSize2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTvlc2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTvlcBuild);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068804);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068814);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068824);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _card_clear);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068874);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068884);

INCLUDE_ASM("asm/main/nonmatchings/psyq", InitCARD);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StartCARD);

long StopCARD(void) {
    func_800689B4();
    _ExitCard();
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068994);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800689A4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800689B4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _patch_card_info);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068A08);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068A34);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068A78);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _patch_card);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _patch_card2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _copy_memcard_patch);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _ExitCard);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068C64);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _card_format);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80069024);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80069034);

INCLUDE_ASM("asm/main/nonmatchings/psyq", bcopy);

void *bzero(unsigned char *p, int n) {
    unsigned char *s;

    if (p == NULL) {
        return NULL;
    }
    if (n <= 0) {
        return NULL;
    }
    s = p;
    while (n > 0) {
        *p++ = 0;
        n--;
    }
    return s;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", memcpy);

INCLUDE_ASM("asm/main/nonmatchings/psyq", memset);

extern u_long D_801DDC10;

int rand(void) {
    D_801DDC10 = D_801DDC10 * 0x41C64E6D + 12345;
    return (D_801DDC10 >> 16) & 0x7FFF;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", srand);

INCLUDE_ASM("asm/main/nonmatchings/psyq", strcat);

INCLUDE_ASM("asm/main/nonmatchings/psyq", strcmp);

INCLUDE_ASM("asm/main/nonmatchings/psyq", strcpy);

INCLUDE_ASM("asm/main/nonmatchings/psyq", strlen);

INCLUDE_ASM("asm/main/nonmatchings/psyq", strncmp);

INCLUDE_ASM("asm/main/nonmatchings/psyq", printf);

INCLUDE_ASM("asm/main/nonmatchings/psyq", prnt);

void *memchr(unsigned char *s, int c, int n) {
    if (s == NULL || n <= 0) {
        return NULL;
    }
    while (--n >= 0) {
        if (*s++ == (u_char)c) {
            return s - 1;
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _putchar);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _putchar_flash);

INCLUDE_ASM("asm/main/nonmatchings/psyq", putchar);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013D5C);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013D70);

INCLUDE_ASM("asm/main/nonmatchings/psyq", sprintf);

INCLUDE_ASM("asm/main/nonmatchings/psyq", memmove);

INCLUDE_ASM("asm/main/nonmatchings/psyq", puts);

INCLUDE_ASM("asm/main/nonmatchings/psyq", setjmp);

INCLUDE_ASM("asm/main/nonmatchings/psyq", longjmp);

INCLUDE_ASM("asm/main/nonmatchings/psyq", toupper);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A734);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A744);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A754);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A76C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A784);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A794);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7A4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7B4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7C4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7D4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7E4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7F4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A804);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A814);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A824);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A834);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A844);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A854);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A864);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A874);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A884);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A894);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetRCnt);

long GetRCnt(unsigned long spec) {
    int c = spec & 0xFFFF;

    if (c >= 3) {
        return 0;
    }
    return D_8007790C[c * 8];
}

long StartRCnt(u_long spec) {
    int timer = spec & 0xFFFF;

    D_80077908[1] |= D_80077910[timer];
    return timer < 3;
}

long StopRCnt(u_long spec) {
    D_80077908[1] &= ~D_80077910[spec & 0xFFFF];
    return 1;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", ResetRCnt);

INCLUDE_ASM("asm/main/nonmatchings/psyq", firstfile);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006ABB0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006ACB4);

extern long D_80077928;

void SetInitPadFlag(int num) {
    D_80077928 = num;
}

long ReadInitPadFlag(void) {
    return D_80077928;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", PAD_init);

INCLUDE_ASM("asm/main/nonmatchings/psyq", InitPAD);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StartPAD);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AE30);

int func_8006AEA8(void) {
    volatile int i, j, k;

    D_8007792C[5] = 0;
    i = 10;
    while (--i != -1) {
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AF10);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AF54);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AF64);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AF74);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AF84);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AF94);

INCLUDE_ASM("asm/main/nonmatchings/psyq", EnablePAD);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DisablePAD);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _patch_pad);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _remove_ChgclrPAD);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006B0B4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006B0D4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", PadGetState);

INCLUDE_ASM("asm/main/nonmatchings/psyq", PadInfoMode);

INCLUDE_ASM("asm/main/nonmatchings/psyq", PadStartCom);

INCLUDE_ASM("asm/main/nonmatchings/psyq", PadStopCom);

INCLUDE_ASM("asm/main/nonmatchings/psyq", PadInitDirect);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _padInitDirPort);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006B584);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006B5EC);

void func_8006B6E0(PadPort *port) {
    u_char cmd = port->cmd;

    port->cmd = 0;
    port->prevCmd = cmd;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006B6F0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006B7AC);

PadPort *func_8006BA28(int port) {
    PadPort *p = D_801DDCB0;

    if (port & 0xF0) {
        p = &D_801DDCB0[1];
    }
    return p;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006BA48);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006BB58);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _dirFailAuto);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006BEA4);

int func_8006BED4(void) {
    if (!(D_800779D4[1] & 1)) {
        return 0;
    }
    if (!(D_800779D4[0] & 1)) {
        return 0;
    }
    if (D_80077988 != NULL) {
        D_80077988();
    }
    return 1;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006BF3C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C0CC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C400);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C4F0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C714);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C990);

void func_8006CA20(void) {
    while (!(D_800779D8->stat & 2)) {
    }
}

void func_8006CA48(PadPort *port, u_char cmd, u_char *data, u_char len) {
    port->cmd = cmd;
    port->data = data;
    port->len = len;
}

void func_8006CA58(PadPort *port) {
    switch (port->unk46) {
    case 2:
        func_8006D318(port);
        return;
    case 3:
        func_8006D32C(port, port->unkE4);
        return;
    case 4:
        func_8006D36C(port, port->unk47[0]);
        return;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006CADC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006CD4C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006CD84);

void func_8006CE58(PadPort *port) {
    switch (port->unk46) {
    case 2:
        func_8006D32C(port, port->unk47[0]);
        return;
    case 3:
        func_8006D34C(port, port->unk47[0]);
        return;
    case 4:
        if (port->unk47[1] == 0) {
            func_8006D36C(port, port->unk47[0]);
            return;
        }
        func_8006D38C(port);
        return;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006CF00);

void func_8006D2F8(PadPort *port, u_char param) {
    port->cmd = 0x43;
    port->data = &port->param;
    port->param = param;
    port->len = 1;
}

void func_8006D318(PadPort *port) {
    port->cmd = 0x45;
    port->data = NULL;
    port->len = 0;
}

void func_8006D32C(PadPort *port, u_char param) {
    port->cmd = 0x4C;
    port->data = &port->param;
    port->param = param;
    port->len = 1;
}

void func_8006D34C(PadPort *port, u_char param) {
    port->cmd = 0x46;
    port->data = &port->param;
    port->param = param;
    port->len = 1;
}

void func_8006D36C(PadPort *port, u_char param) {
    port->cmd = 0x47;
    port->data = &port->param;
    port->param = param;
    port->len = 1;
}

void func_8006D38C(PadPort *port) {
    port->cmd = 0x4B;
    port->data = NULL;
    port->len = 0;
}

void func_8006D3A0(int wait) {
    D_801DDF28 = wait;
    D_801DDF24 = *(volatile u_short *)0x1F801120;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D3C0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D460);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D4A8);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D580);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D62C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D748);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013E3C);
