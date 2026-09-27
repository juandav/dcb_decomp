#ifndef DCB_SOUND_H
#define DCB_SOUND_H

#include "game.h"

typedef struct {
    /* 0x0 */ s16 id;
    /* 0x2 */ s16 vab;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ u8 *buf;
} SndSlot;
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 cur;
    /* 0x04 */ s16 seq[2];
    /* 0x08 */ u8 vol[2];
    /* 0x0A */ u8 unkA[2];
    /* 0x0C */ u8 *data[2];
    /* 0x14 */ SndSlot unk14;
    /* 0x20 */ SndSlot slot[2];
} SndState;

extern s32 D_801D6B28;
extern s32 D_801D8128;
extern s32 D_8006DFFC;
extern s8 *D_8006E000[];
extern s16 D_8006E048;
extern s32 D_80010598;
extern s32 D_800105A8;
extern s32 D_8006E044;
extern u16 D_8006E04C;
extern s16 D_801D813E;
extern s16 D_801D812A;

void func_8002ADEC();
void func_8002B3EC(s32 arg0, s32 arg1);
void func_8002AEA4(s32);
void func_8002B688(void);
void func_8002B258(s32 arg0);
void func_8002B668(void);
s32 func_8002B300(void *, s16, s32);
void func_8002B38C(void *, s32, s32);
void func_8002B2C0(void);
void func_8002B3DC(void);
void func_8002B3E4(void);
void func_8002B498(s32 arg0);
void func_8002B530(s32 arg0, s32 vol);
void func_8002B5D0(s32 arg0, s32 arg1);
void func_8002B644(s16 arg0);
void func_8002B6E4(s32 idx, s32 step);
void func_8002B7DC(s32 arg0);
void func_8002B850(void);
void func_8002B858(s32 arg0);
void func_8002B024(s32, s32, u8);
void func_8002B900(s32 seq, s32 arg1, s32 arg2, s32 load);
void func_8002BA24(void);
void func_8002BA6C(s32 arg0, s32 arg1, s32 arg2);

#endif /* DCB_SOUND_H */
