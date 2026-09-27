#ifndef DCB_TMD_SORT_H
#define DCB_TMD_SORT_H

#include "game.h"

#define SORT_WORK ((SortWork *)0x1F800000)
#define STRIP_DRAW(draw)                                                                                            \
    {                                                                                                                \
        gte_avsz3();                                                                                                 \
        pkt = draw(func_800206B0(pkt, SORT_WORK->ot, gouraud, SORT_WORK->code), SORT_WORK->ot, gouraud,             \
                   SORT_WORK->code);                                                                                 \
    }

typedef struct {
    /* 0x00 */ u32 *data;
    /* 0x04 */ u32 *work;
    /* 0x08 */ u32 *ot;
    /* 0x0C */ u32 packet;
    /* 0x10 */ s32 pass;
    /* 0x14 */ u32 code;
    /* 0x18 */ u32 textured;
    /* 0x1C */ u32 quad;
    /* 0x20 */ void *unk20;
    /* 0x24 */ u32 count[2];
    /* 0x2C */ u32 unk2C;
    /* 0x30 */ u32 tpage;
    /* 0x34 */ u32 clut;
    /* 0x38 */ u32 unk38;
} SortWork;

extern Unk8006DF60 D_8006DF60[];

void func_8001F518(u32 i0, u32 *idx, u8 *base);
void func_8001F580(s32 flag, u32 i, u8 *base);
void func_8001F5A4(s32 flag, u32 i, u8 *base);
void func_8001F5CC(s32 flag, u32 i, u8 *base);
void func_8001F5FC(s32 flag, u32 i, u8 *base);
void func_8001F630(s32 flag, u32 i, u8 *base);
void func_8001F660(s32 flag, u32 i, u8 *base);
void func_8001F694(s32 flag, u32 i, u8 *base);
u32 *func_8001F6C4(u32 *p, u32 *ot, s32 gouraud, u32 code);
u32 *func_8001F768(u32 *p, u32 *ot, s32 gouraud, u32 code);
u32 *func_8001F824(u32 *p, u32 *ot, s32 gouraud, u32 code);
u32 *func_8001F8B0(u32 *p, u32 *ot, s32 gouraud, u32 code);
u32 *func_8001F3C0(u32 *, u32 *);
void func_8001F94C(SortWork *);
u32 *func_80020440(u32 *, u32 *);
void func_80020778(SortWork *);
u32 func_800202D8(u32 *data, u32 *ot, u32 packet, void *arg3);
u32 func_80020370(u32 *data, u32 *ot, u32 packet, void *arg3);
void func_8002060C(s32 flag, u32 i, u8 *base);
void func_80020638(s32 flag, u32 i, u8 *base);
void func_80020674(s32 flag, u32 i, u8 *base);
u32 *func_800206B0(u32 *p, u32 *ot, s32 gouraud, u32 code);

#endif /* DCB_TMD_SORT_H */
