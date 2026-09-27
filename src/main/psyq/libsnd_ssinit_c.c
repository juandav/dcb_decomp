#include "psyq.h"

void SsInit(void) {
    ResetCallback();
    func_8004C5B0();
    SpuClearReverbWorkArea(7);
    _SsInit();
}

OBJECT_END(2);

void func_8004C5B0(void) {
    _SpuInit(0);
}

extern long D_801D8610;
extern _SsFCALL D_801D8578;

short _SsInitSoundSeq(short seq, short vabId, u_char *addr);
void func_8004DF40();
void func_8004DFA0();
void func_8004DFD0();
void func_8004E000();

short SsSeqOpen(u_long *addr, short vabId) {
    short seq = 0;
    u_char *data = (u_char *)addr;
    int i;
    short ret;

    if (D_801D8610 == -1) {
        printf("Can't Open Sequence data any more\n\n");
        return -1;
    }
    for (i = 0; i < 32; i++) {
        if (!(D_801D8610 & (1 << i))) {
            seq = i;
            break;
        }
    }
    D_801D8610 |= 1 << seq;
    ret = _SsInitSoundSeq(seq, vabId, data);
    D_801D8578.noteon = (void (*)())_SsNoteOn;
    D_801D8578.programchange = (void (*)())_SsSetProgramChange;
    D_801D8578.metaevent = (void (*)())_SsGetMetaEvent;
    D_801D8578.pitchbend = (void (*)())_SsSetPitchBend;
    D_801D8578.control[CC_NUMBER] = (void (*)())_SsSetControlChange;
    D_801D8578.control[CC_BANKCHANGE] = (void (*)())_SsContBankChange;
    D_801D8578.control[CC_MAINVOL] = (void (*)())_SsContMainVol;
    D_801D8578.control[CC_PANPOT] = (void (*)())_SsContPanpot;
    D_801D8578.control[CC_EXPRESSION] = (void (*)())_SsContExpression;
    D_801D8578.control[CC_DAMPER] = (void (*)())_SsContDamper;
    D_801D8578.control[CC_NRPN1] = (void (*)())_SsContNrpn1;
    D_801D8578.control[CC_NRPN2] = (void (*)())_SsContNrpn2;
    D_801D8578.control[CC_RPN1] = (void (*)())_SsContRpn1;
    D_801D8578.control[CC_RPN2] = (void (*)())_SsContRpn2;
    D_801D8578.control[CC_EXTERNAL] = (void (*)())_SsContExternal;
    D_801D8578.control[CC_RESETALL] = (void (*)())_SsContResetAll;
    D_801D8578.control[CC_DATAENTRY] = (void (*)())_SsContDataEntry;
    D_801D8578.ccentry[DE_PRIORITY] = (void (*)())_SsSetNrpnVabAttr0;
    D_801D8578.ccentry[DE_MODE] = (void (*)())_SsSetNrpnVabAttr1;
    D_801D8578.ccentry[DE_LIMITL] = (void (*)())_SsSetNrpnVabAttr2;
    D_801D8578.ccentry[DE_LIMITH] = (void (*)())_SsSetNrpnVabAttr3;
    D_801D8578.ccentry[DE_ADSR_AR_L] = (void (*)())_SsSetNrpnVabAttr4;
    D_801D8578.ccentry[DE_ADSR_AR_E] = (void (*)())_SsSetNrpnVabAttr5;
    D_801D8578.ccentry[DE_ADSR_DR] = (void (*)())_SsSetNrpnVabAttr6;
    D_801D8578.ccentry[DE_ADSR_SL] = (void (*)())_SsSetNrpnVabAttr7;
    D_801D8578.ccentry[DE_ADSR_SR_L] = (void (*)())_SsSetNrpnVabAttr8;
    D_801D8578.ccentry[DE_ADSR_SR_E] = (void (*)())_SsSetNrpnVabAttr9;
    D_801D8578.ccentry[DE_ADSR_RR_L] = (void (*)())_SsSetNrpnVabAttr10;
    D_801D8578.ccentry[DE_ADSR_RR_E] = (void (*)())_SsSetNrpnVabAttr11;
    D_801D8578.ccentry[DE_ADSR_SR] = (void (*)())_SsSetNrpnVabAttr12;
    D_801D8578.ccentry[DE_VIB_TIME] = (void (*)())_SsSetNrpnVabAttr13;
    D_801D8578.ccentry[DE_PORTA_DEPTH] = (void (*)())_SsSetNrpnVabAttr14;
    D_801D8578.ccentry[DE_REV_TYPE] = func_8004DF40;
    D_801D8578.ccentry[DE_REV_DEPTH] = (void (*)())_SsSetNrpnVabAttr16;
    D_801D8578.ccentry[DE_ECHO_FB] = func_8004DFA0;
    D_801D8578.ccentry[DE_ECHO_DELAY] = func_8004DFD0;
    D_801D8578.ccentry[DE_DELAY] = func_8004E000;
    if (ret == -1) {
        return -1;
    }
    return seq;
}

/* ASPSX padded the string table of the object as well */
__asm__(".section .rodata\n\t.space 12\n");

OBJECT_END(1);
