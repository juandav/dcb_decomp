#ifndef DCB_PRIM_DESC_H
#define DCB_PRIM_DESC_H

#include "game.h"

/* jp's own (src/main/gfx/prim_desc.c): a primitive described by its corner,
   its size and its GPU code, which drawPrimDesc turns into a packet in the
   frame's 17th prim slot. A rectangle (F4, FT4, G4) gives its size in
   (x1, y1) and its texture's in (u1, v1); a sprite its size in (x1, y1); a
   line (code 4) its end in (x1, y1), its width in clut and whether it is
   horizontal in tpage. */
typedef struct {
    /* 0x00 */ s32 z; /* the OT entry */
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ u8 u0;
    /* 0x09 */ u8 v0;
    /* 0x0A */ u16 clut;
    /* 0x0C */ u8 u1;
    /* 0x0D */ u8 v1;
    /* 0x0E */ u16 tpage;
    /* 0x10 */ s16 x0;
    /* 0x12 */ s16 y0;
    /* 0x14 */ s16 x1;
    /* 0x16 */ s16 y1;
    /* 0x18 */ s16 x2;
    /* 0x1A */ s16 y2;
    /* 0x1C */ s16 x3;
    /* 0x1E */ s16 y3;
    /* 0x20 */ u32 rgb1; /* G4's other corners */
    /* 0x24 */ u32 rgb2;
    /* 0x28 */ u32 rgb3;
    /* 0x2C */ u8 u2;
    /* 0x2D */ u8 v2;
    /* 0x2E */ u8 u3;
    /* 0x2F */ u8 v3;
} PrimDesc;

/* a sprite: what a PrimDesc of code 0x64 uses of it */
typedef struct {
    /* 0x00 */ s32 z;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ u8 u0;
    /* 0x09 */ u8 v0;
    /* 0x0A */ u16 clut;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u16 tpage;
    /* 0x10 */ s16 x0;
    /* 0x12 */ s16 y0;
    /* 0x14 */ s16 w;
    /* 0x16 */ s16 h;
} SpriteDesc;

/* a rectangle (F4 or FT4) placed in 3D: the head of a PrimDesc, then where */
typedef struct {
    /* 0x00 */ s32 z;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ u8 u0;
    /* 0x09 */ u8 v0;
    /* 0x0A */ u16 clut;
    /* 0x0C */ u8 u1;
    /* 0x0D */ u8 v1;
    /* 0x0E */ u16 tpage;
    /* 0x10 */ s32 unk10[2];
    /* 0x18 */ VECTOR pos;
    /* 0x28 */ SVECTOR rot;
    /* 0x30 */ s32 w;
    /* 0x34 */ s32 h;
} PrimDesc3D;

/* drawPrimDesc3D's work area */
typedef struct {
    /* 0x00 */ SVECTOR corners[4];
    /* 0x20 */ MATRIX matrix;
    /* 0x40 */ PrimDesc desc;
} PrimDesc3DWork;

extern u32 *PRIM_DESC_OT;
extern u32 *PRIM_DESC_PACKETS;
/* the callbacks the overlays' screens run each frame, and the one running */
extern void (*CALLBACK_SLOTS[10])(void *);
extern void (**CURRENT_CALLBACK_SLOT)(void *);

void drawPrimDesc3D(PrimDesc3D *prim, PrimDesc3DWork *work);
void drawPrimDesc(PrimDesc *desc);
u32 *addPrimDescSprite(PrimDesc *desc, u32 *ot, u32 *packet, s32 z);
u32 *addPrimDescF4(PrimDesc *desc, u32 *ot, u32 *packet, s32 z);
u32 *addPrimDescFT4(PrimDesc *desc, u32 *ot, u32 *packet, s32 z);
u32 *addPrimDescG4(PrimDesc *desc, u32 *ot, u32 *packet, s32 z);
void clearCallbackSlots(s16 first, s16 count);
void runCallbackSlots(void *arg, s16 first, s16 count);
s16 splitDigits(s32 value, u8 *digits);
s32 intPow(s32 base, s32 exponent);
void allocPrimDescPackets(s16 count);
void freePrimDescPackets(void);
void renderFullscreenBackground(void);

extern SpriteDesc FULLSCREEN_BG_LEFT;
extern SpriteDesc FULLSCREEN_BG_RIGHT;

#endif /* DCB_PRIM_DESC_H */
