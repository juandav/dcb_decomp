#ifndef DCB_MEMCARD_H
#define DCB_MEMCARD_H

#include "game.h"

typedef struct {
    u8 data[0x200];
} McHeader;

extern s16 D_801D813C;
extern s32 D_801D8160;
extern s32 D_801D8164;
extern s32 D_801D8168;
extern s32 D_801D816C;
extern s32 D_801D8170;
extern s32 D_801D8174;
extern s32 D_801D8178;
extern s32 D_801D817C;
extern CardDir *D_801D8190[2];
extern u8 *D_801D81A0;
extern s32 D_801D8198;
extern char D_800105E4;
extern s32 D_801D8180;
extern s32 D_801D8184;
extern s32 D_801D8188;
extern s32 D_801D818C;
extern s32 D_801D819C;
extern u8 D_8006E0B8[6];

void func_8002BC58(void);
void func_8002BB58(u32);
s32 func_8002BC2C(void);
void func_8002BC80(void);
void func_8002BF60(void);
void func_8002C094(void);
s32 func_8002BE84(s32);
s32 func_8002BFB8(s32);
s32 func_8002C0EC(s32 arg0);
s32 func_8002C1C0(s32 port);
s32 func_8002C2E4(s32 arg0);
s32 func_8002C30C(s32 slot, u8 blocks, s32 arg2, s32 arg3, McHeader *hdr);
s32 func_8002C468(void);
s32 func_8002C6EC(s32 slot, s32 arg1, s32 arg2);
s32 func_8002C784(void);
s32 func_8002C9E8(s32 arg0, void *arg1, s32 arg2);
void func_8002CAC8(s32 port);
s32 func_8002CBA0(s32 len, u8 *p);
void func_8002CC04(s32 len, u8 *p);
void func_8002CC44(s32 p);

#endif /* DCB_MEMCARD_H */
