#ifndef PSYQ_H
#define PSYQ_H

#include "common.h"
#include <sys/types.h>
#include <libapi.h>
#include <libetc.h>
#include <libgpu.h>
#include <libcd.h>
#include <libspu.h>
#include <libsnd.h>
#include <libmcrd.h>
#include <libpad.h>
#include <libgs.h>

/* libgpu driver entry points */
typedef struct GpuDriver {
    /* 0x00 */ void *unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ int (*addque)(void *func, void *param, int size, long arg);
    /* 0x0C */ void *unkC;
    /* 0x10 */ int (*unk10)(u_long cmd);
    /* 0x14 */ int (*unk14)(u_long *p, int len);
    /* 0x18 */ int (*unk18)();
    /* 0x1C */ int (*unk1C)();
    /* 0x20 */ int (*unk20)();
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ void (*unk2C)(u_long *ot, int n);
    /* 0x30 */ void *unk30;
    /* 0x34 */ int (*unk34)(int mode);
    /* 0x38 */ u_long (*status)(void);
    /* 0x3C */ int (*sync)(int mode);
} GpuDriver;

/* libsnd voice state (_svm_voice), 0x38 bytes per voice */
typedef struct VmVoice {
    /* 0x00 */ short unk0;
    /* 0x02 */ short unk2;
    /* 0x04 */ short unk4;
    /* 0x06 */ u_short envx;
    /* 0x08 */ short unk8;
    /* 0x0A */ u_char unkA;
    /* 0x0B */ u8 unkB;
    /* 0x0C */ short unkC;
    /* 0x0E */ short note;
    /* 0x10 */ short unk10;
    /* 0x12 */ short unk12;
    /* 0x14 */ short prog;
    /* 0x16 */ short tone;
    /* 0x18 */ short vabId;
    /* 0x1A */ short prior;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ char unk1D;
    /* 0x1E */ short autoVol;
    /* 0x20 */ short unk20;
    /* 0x22 */ short unk22;
    /* 0x24 */ short unk24;
    /* 0x26 */ short startVol;
    /* 0x28 */ short endVol;
    /* 0x2A */ short autoPan;
    /* 0x2C */ short unk2C;
    /* 0x2E */ short unk2E;
    /* 0x30 */ short unk30;
    /* 0x32 */ short startPan;
    /* 0x34 */ short endPan;
    /* 0x36 */ short unk36;
} VmVoice;
extern VmVoice D_800815D0[];

/* libpad per-port command state */
/* actuator info table entry */
typedef struct PadActInfo {
    /* 0x0 */ u_char unk0;
    /* 0x1 */ u_char unk1;
    /* 0x2 */ u_char unk2;
    /* 0x3 */ u_char power;
    /* 0x4 */ u_char unk4;
} PadActInfo;

/* one received block: its length and where its data went */
typedef struct PadRecvBlock {
    /* 0x0 */ u_char len;
    /* 0x1 */ u8 unk1[3];
    /* 0x4 */ u_char *data;
} PadRecvBlock;

typedef struct PadPort {
    /* 0x00 */ u_short *unk0;
    /* 0x04 */ PadActInfo *unk4;
    /* 0x08 */ PadRecvBlock *unk8;
    /* 0x0C */ struct PadPort *unkC;
    /* 0x10 */ struct PadPort *unk10;
    /* 0x14 */ void (*unk14)();
    /* 0x18 */ int (*unk18)(struct PadPort *p);
    /* 0x1C */ u8 unk1C[4];
    /* 0x20 */ u_char *unk20;
    /* 0x24 */ u_char param;
    /* 0x25 */ u8 unk25[3];
    /* 0x28 */ u_char *actTable;
    /* 0x2C */ u_char *data;
    /* 0x30 */ u_char *unk30;
    /* 0x34 */ u_char actLen;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u_char len;
    /* 0x37 */ u_char cmd;
    /* 0x38 */ u_char prevCmd;
    /* 0x39 */ u_char unk39;
    /* 0x3A */ u8 unk3A[2];
    /* 0x3C */ u_char *unk3C;
    /* 0x40 */ u_char *unk40;
    /* 0x44 */ u_char unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u_char unk46;
    /* 0x47 */ u_char unk47[2];
    /* 0x49 */ u_char unk49;
    /* 0x4A */ u_char unk4A;
    /* 0x4B */ u8 unk4B;
    /* 0x4C */ long unk4C;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u_char unk51[2];
    /* 0x53 */ u_char unk53;
    /* 0x54 */ u8 unk54[3];
    /* 0x57 */ u_char unk57[6];
    /* 0x5D */ u_char unk5D[6];
    /* 0x63 */ u8 unk63[0x80];
    /* 0xE3 */ u_char unkE3;
    /* 0xE4 */ u_char unkE4;
    /* 0xE5 */ u8 unkE5;
    /* 0xE6 */ u_short unkE6;
    /* 0xE8 */ u8 unkE8;
    /* 0xE9 */ u_char unkE9;
    /* 0xEA */ u_char unkEA;
    /* 0xEB */ u8 unkEB;
    /* 0xEC */ u_short unkEC;
    /* 0xEE */ u_short unkEE;
} PadPort;

/* libmcrd global state, returned by McrdGetGlobalStructure */
typedef struct McrdGlobal {
    /* 0x00 */ long unk0;
    /* 0x04 */ long unk4;
    /* 0x08 */ long unk8;
    /* 0x0C */ long unkC;
    /* 0x10 */ long unk10;
    /* 0x14 */ long fd;
    /* 0x18 */ u8 unk18[0x2C];
    /* 0x44 */ MemCB callback;
} McrdGlobal;

/* serial port registers */
typedef struct SioRegs {
    /* 0x0 */ u_char data;
    /* 0x1 */ u8 unk1[3];
    /* 0x4 */ u_short stat;
    /* 0x6 */ u_short unk6;
    /* 0x8 */ u_short mode;
    /* 0xA */ u_short ctrl;
    /* 0xC */ u_short unkC;
    /* 0xE */ u_short baud;
} SioRegs;

/* libetc interrupt handlers, reached through D_80070AA8 */
typedef struct IntrFuncs {
    /* 0x00 */ void *unk0;
    /* 0x04 */ void *(*dmaCallback)(int dma, void (*func)());
    /* 0x08 */ void *(*interruptCallback)(int irq, void (*func)());
    /* 0x0C */ int (*resetCallback)(void);
    /* 0x10 */ int (*stopCallback)(void);
    /* 0x14 */ void *(*vsyncCallbacks)(int ch, void (*func)());
    /* 0x18 */ int (*restartCallback)(void);
} IntrFuncs;

/* libspu reverb register block, as passed to _spu_setReverbAttr */
typedef struct SpuReverbRegs {
    /* 0x00 */ long mask;
    /* 0x04 */ u_short param[32];
} SpuReverbRegs;

/* libsnd per-sequence state, D_801D8618[seq][sep] */
typedef struct SeqStruct {
    /* 0x00 */ u_char *unk0;
    /* 0x04 */ u_char *unk4;
    /* 0x08 */ u_char *unk8;
    /* 0x0C */ u_char *unkC;
    /* 0x10 */ u_char *unk10;
    /* 0x14 */ char unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u_char status;
    /* 0x17 */ u_char channel;
    /* 0x18 */ u_char rpn1;
    /* 0x19 */ u_char rpn2;
    /* 0x1A */ u8 unk1A[4];
    /* 0x1E */ u_char unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ char unk20;
    /* 0x21 */ char unk21;
    /* 0x22 */ char unk22;
    /* 0x23 */ char unk23;
    /* 0x24 */ u8 unk24[2];
    /* 0x26 */ char vabId;
    /* 0x27 */ u_char panpot[16];
    /* 0x37 */ u_char programs[16];
    /* 0x47 */ u8 unk47;
    /* 0x48 */ short unk48;
    /* 0x4A */ short unk4A;
    /* 0x4C */ short unk4C;
    /* 0x4E */ short unk4E;
    /* 0x50 */ short unk50;
    /* 0x52 */ short unk52;
    /* 0x54 */ short unk54;
    /* 0x56 */ u_short unk56;
    /* 0x58 */ u_short voll;
    /* 0x5A */ u_short volr;
    /* 0x5C */ u_short unk5C;
    /* 0x5E */ u_short unk5E;
    /* 0x60 */ short vol[16];
    /* 0x80 */ short unk80;
    /* 0x82 */ u8 unk82[2];
    /* 0x84 */ long unk84;
    /* 0x88 */ long unk88;
    /* 0x8C */ long unk8C;
    /* 0x90 */ long delta;
    /* 0x94 */ u_long unk94;
    /* 0x98 */ long flags;
    /* 0x9C */ long unk9C;
    /* 0xA0 */ long unkA0;
    /* 0xA4 */ long unkA4;
    /* 0xA8 */ long unkA8;
    /* 0xAC */ u_long unkAC;
} SeqStruct;

/* libsnd decoded ADSR */
typedef struct SsADSR {
    /* 0x00 */ short ar;
    /* 0x02 */ short dr;
    /* 0x04 */ short sl;
    /* 0x06 */ short sr;
    /* 0x08 */ short rr;
    /* 0x0A */ short arMode;
    /* 0x0C */ short srMode;
    /* 0x0E */ short rrMode;
    /* 0x10 */ short srDir;
} SsADSR;

int CD_getsector(void *madr, int size);
u_long func_80065C54(short x, short y);
u_long func_80065CEC(short x, short y);
u_long func_80065D84(short x, short y);
void func_8003B578(void);
void func_8003B6A8(long val);
void func_800689B4(void);
long func_8003D698(long chan, long block, u_char *buf);
void func_8003D6A8(void);
long ReadInitPadFlag(void);
void _ExitCard(void);
void _copy_memcard_patch(void);
void _patch_card(void);
void _patch_card2(void);
void _patch_card_info(void);

extern GpuDriver *D_80076750;
extern int (*D_80076754)(char *fmt, ...);
typedef struct GpuDebug {
    /* 0x0 */ u_char type;
    /* 0x1 */ u_char queue;
    /* 0x2 */ u_char level;
    /* 0x3 */ u_char reverse;
    /* 0x4 */ short w;
    /* 0x6 */ short h;
    /* 0x8 */ volatile long unk8;
    /* 0x0C */ void (*drawSyncCallback)();
    /* 0x10 */ DRAWENV draw;
    /* 0x6C */ DISPENV disp;
} GpuDebug;
extern GpuDebug D_80076758;
extern u_long *D_800557A8;
extern volatile u_long *D_80076864;
extern volatile u_long *D_80076868;
extern volatile u_long *D_8007686C;
extern u_short D_8006FA22;
extern volatile u_short *D_80070AB0;
extern long D_8005B800;
extern volatile u_short *D_8006EF24;
extern long D_8006EF94;
extern long D_8006EF40;
extern volatile u_short *D_8007790C;
extern long D_8006EF58;
extern long D_8005C2B8;
extern long D_8005C2E8;
extern McrdGlobal D_80082068;

void *DMACallback(int dma, void (*func)());
void *InterruptCallback(int irq, void (*func)());
void *VSyncCallbacks(int ch, void (*func)());
int CD_ready();
void init_ring_status(int start, u_int count);
long sin_1(long a);
void _card_stop(void);
long _spu_FsetRXXa(long reg, u_long addr);
void _putchar(char c);
void _putchar_flash(void);
void _SsInit(void);
void _SsSeqPlay(short, short);
void _SsSndStop(short, short);
int _SsVmKeyOff(short, short, short, u_short);
void Snd_SetPlayMode(short, short, char, short);
void _SpuInit(int);
u_long _SpuGetAnyVoice(int, int);
void func_8006CA48(PadPort *port, u_char cmd, u_char *data, u_char len);
void func_80024CE8(long fd);
int CD_init(void);
int CD_initvol(void);
long func_80055BE8(long size, long sbaddr);
short _SsVabOpenHeadWithMode(unsigned char *addr, short vabId, long (*alloc)(), unsigned long sbaddr);
void SysDeqIntRP(int, u_char *);
void ChangeClearRCnt(int, int);
void func_8002B018(void);
void func_80056D0C();
void *func_80056D78();
void func_80056DA4(long *p, int n);
void func_80056E20();
void (*func_80056FA0(int index, void (*callback)(void)))(void);
void func_8005704C(long *p, int n);
void func_8004C300(short);
void func_8004ECA0(int);
void func_8004C5B0(void);
void func_8003B1C8(void);
int func_800669AC();

extern IntrFuncs *D_80070AA8;
extern void (*D_8005551C)(void);
extern u_char D_80055578[];
extern volatile u_long *D_8007685C;
extern long D_80076894;
extern long D_80076898;
extern long D_800557F4;
extern volatile u_char *D_80070F04;
extern volatile u_char *D_80070F08;
extern volatile u_char *D_80070F10;
extern volatile u_char *D_80070F14;
extern u_long *D_80070AEC;
extern void (*D_80070AC8[8])();
extern int D_801DDF28;
extern int D_801DDF24;
extern volatile u_long *D_80070AF8;
extern void (*D_80070AFC[])(void);
extern u_long *D_8006EF38;
extern short D_80080A64;
extern short D_80080A66;
extern char D_80080998[];
extern long D_801D98B0;
extern short D_801D98B4;
extern long StCdIntrFlag;
extern long D_801D98CC;
extern long D_801D98D4;
extern long D_801D98D8;
extern long D_801D98DC;
extern long D_801D98F4;
extern MemCB D_800820C0;
extern long D_80082168;
extern long D_8008216C;
extern long D_80082170;
extern long D_80082174;
extern long D_80082178;
extern long D_8008217C;
extern long D_80082180;
extern long D_80082184;

void func_800649E8(char *name, RECT *rect);
void func_8003B444(void);
void func_8003B568(char *bufA, long lenA, char *bufB, long lenB);
void func_8003B588(char *bufA, long lenA, char *bufB, long lenB);
void func_8003B6B8(void);
void func_80061958(u_short, u_short, u_short, u_short, u_short);
void func_80061ADC(u_short, u_short);
void gte_init(void);
void GsSetDrawBuffClip(void);
void GsSetDrawBuffOffset(void);
void _remove_ChgclrPAD(void);
void _patch_pad(void);

extern short D_801DBE24;
extern long D_80082148;
extern long D_8008214C;
extern long D_80082150;
extern long D_80082154;
extern long D_80082158;
extern long D_8008215C;
extern long D_80082160;
extern long D_80082164;

extern PadPort *(*D_8005552C)(int port);
extern long D_80055588;
extern long D_8005A2C8;
extern long D_80070C44;
extern long D_8005A2D0;
extern long D_8005A570;
extern PadPort D_801DDCB0[2];
extern short D_801D9678;

long _SsReadDeltaValue(short seq, short sep);
long _SsVmVSetUp(short vab, short prog);
long _SsVmSetProgVol(short vab, short prog, u_char vol);
long _SsVmSetVol(short seq_sep_no, short vabId, short prog, u_short vol, u_short pan);
void _SsSndSetVolData();
u_long _SpuSetAnyVoice(long on_off, u_long bits, int addr1, int addr2);
int func_800666FC(int (*func)(), u_long *param, int size, u_long value);

extern SeqStruct *D_801D8618[];

void *memcpy(u_char *dst, u_char *src, int n);
void func_8006D318(PadPort *port);
void func_8006D32C(PadPort *port, u_char param);
void func_8006D34C(PadPort *port, u_char param);
void func_8006D36C(PadPort *port, u_char param);
void func_8006D38C(PadPort *port);

extern DRAWENV D_80076768;
extern DISPENV D_800767C4;

u_long _spu_Fw(u_char *addr, u_long size);
void func_8002E388(void (*func)());
void func_8002E3D8(void (*func)());
long func_8002DE88(long value);
long func_8002E3B8(long value);

extern volatile SioRegs *D_800779D8;
extern volatile u_char *D_8005A200;
extern volatile u_char *D_8005A20C;
extern long D_8005A2E8;
extern u_long *D_80077908;
extern u_long D_80077910[];
extern long D_8006EF4C;
extern volatile u_short *D_8007792C;
long _spu_t(long mode, ...);
extern void (*D_80077988)(void);
extern volatile u_long *D_800779D4;
extern long D_8005BA60;
extern long D_80080BE8;
extern long D_80080BF0;
extern long D_80080BF4;
extern long D_80080BF8;
extern long D_80080C10;
extern long D_80080C14;
extern long D_80080C18;
extern void (*D_80080C38)();
extern void (*D_80080C3C)();
extern long D_80080C8C[];

void _clr_card_event(void);
void _SpuDataCallback(void (*func)());
long funcEvSpIOE(void);
long funcEvSpError(void);
long funcEvSpTimeout(void);
long funcEvSpNewcard(void);
long funcEvSpIOEx(void);
long funcEvSpErrorx(void);
long funcEvSpTimeoutx(void);
long funcEvSpNewcardx(void);
void _spu_FiDMA();

extern long D_80070C4C;
extern long D_80070C50;
extern long D_8005B9B0;
extern long D_8005BA18;
extern long D_80080C20;

void SsUtReverbOff(void);
void func_80034BE8(void);
extern u_char D_800555C1[];

#endif /* PSYQ_H */
