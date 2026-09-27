#ifndef DCB_ARCHIVE_H
#define DCB_ARCHIVE_H

#include "game.h"

extern s32 D_801D4868;
extern u32 D_801D486C;
extern u8 *D_801D4870;
extern s32 D_801D4878;
extern s32 D_801D4888;
extern s32 D_801D5108;
extern s32 D_801D487C;
extern u8 *D_801D4874;
extern u8 D_801D5988[0x1000];

void *func_8001BB44(Chunk *p, s32 id, s32 sub);
void func_8001BB94(Chunk *p, s32 id, s32 sub);
void func_8001BC14(Chunk *arg0);
s32 func_8001BC38(void);
u32 func_8001BCA4(s32);
s32 func_8001BD60(void);
s32 func_8001C078(s32);
s32 func_8001BFCC(s32 arg0, s32 arg1);
void func_8001BDEC(u32);
s32 func_8001BFF8(s32, s32);
void func_8001C1E0(s8 *, s8 *, s32);
void func_8001C0A8(s8 *base, u32 n, s32 size, s32 (*cmp)(s8 *, s8 *));

#endif /* DCB_ARCHIVE_H */
