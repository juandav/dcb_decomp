#ifndef DCB_CARD_DB_H
#define DCB_CARD_DB_H

#include "game.h"

typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 unk2[0xE0];
} Unk801D8400;
typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 unk2[0x6E];
} Unk801D8404;

extern u8 D_8006E520[6][3];
extern u8 *D_801D840C;
extern u8 *D_801D8400;
extern u8 *D_801D8404;
extern u8 D_8006E50C[];
extern u8 D_8006E518[];
extern u8 D_8006EEFC[];
extern CardRec20 D_8006E534[];
extern CardRec10 D_8006E774[];
extern CardEffect D_8006E9B4[];
extern u8 *D_8006EDB4[];

s32 func_80048230(s32, s32);
void func_800457FC();
void func_80045968(s32 a, s32 row, s32 n);
s32 func_80047B84(s32 a, s32 id);
s32 func_80047A58(s32 arg0);
s32 func_80048014(s32 a, s32 b);
s32 func_80048150(s32 a, s32 id);
void func_80047620(s32, s32, s32);
void func_80047C38(s32, s32, s32);
void func_80047248(s32);
void func_80045A58(s32 arg0);
void func_80045AB8(s32 arg0);
void *func_80046088(s32);
s8 func_80045E1C(s32 p, s32 id, s32 n);
s32 func_80045F5C(s32 arg0, s32 arg1);
s32 func_80045F94(s32 arg0, s32 arg1);
s32 func_80045FE8(s32 id);
s32 func_80046038(s32 id);
void func_80046118(s32 p);
void func_800461C0(s32 a);
s8 func_80045B18(s32, s32, s32);
void func_80046864(s32 a);
void func_80046908(s32 i);
void func_80046A38(s32, Unk110 *);
void func_800469A4(s32 a);
void func_80046BAC(u8 *out, s32 id);
s32 func_80046C0C(s32 unused, Unk110 *deck, s32 mask);
s32 func_80046D68(s32 a, Unk110 *src, s32 slot);
s32 func_80046FB8(s32 a, Unk110 *out, s32 i);
s32 func_8004707C(s32 a, s32 b);
s32 func_800471F4(s32 arg0);
void func_80047364(s32 a);
void func_80047438(s32 a);
void func_8004950C(s32, s32);
void func_80047A38(s32 arg0, s32 arg1);
s32 func_80047A98(s32 a, s32 b);
void func_80047E64(s32 a, s32 b, s32 c);
s32 func_80047D5C(s32 a, s32 b);
s32 func_800496E4(s32, s32);
void func_800493EC(s32 a, s32 b, s32 c, s32 v);
void func_8004949C(s32 arg0, s32 arg1, s32 arg2);
s32 func_800495B4(s32 a, s32 b, s32 skip, s32 card);

#endif /* DCB_CARD_DB_H */
