#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/prim_desc.h"
#include "dcb/heap.h"

/* jp's own: primitives described by a PrimDesc, drawn into the frame's 17th
   prim slot (the overlays' screens use them), a table of per-frame
   callbacks, and a number's decimal digits */

/* drawPrimDesc expands the description in the scratchpad */
#define SCRATCH_PRIM_DESC ((PrimDesc *)0x1F800000)

static MATRIX *buildPrimDescMatrix(VECTOR *pos, SVECTOR *rot, MATRIX *m);

/* a rectangle facing the camera's view, placed and turned in 3D */
void drawPrimDesc3D(PrimDesc3D *prim, PrimDesc3DWork *work) {
    SVECTOR *corner0;
    SVECTOR *corner1;
    SVECTOR *corner2;
    SVECTOR *corner3;
    MATRIX *matrix;
    PrimDesc *desc;
    s32 p;
    s32 flag;
    s32 otz;
    u32 *ot;
    u8 code;

    corner0 = work->corners;
    corner1 = &work->corners[1];
    corner2 = &work->corners[2];
    corner3 = &work->corners[3];
    matrix = &work->matrix;
    desc = &work->desc;
    *desc = *(PrimDesc *)prim;
    buildPrimDescMatrix(&prim->pos, &prim->rot, matrix);
    SetRotMatrix((s32)matrix);
    SetTransMatrix(matrix);
    corner0->vx = -(prim->w >> 1);
    corner0->vy = -(prim->h >> 1);
    corner0->vz = 0;
    corner1->vx = prim->w >> 1;
    corner1->vy = -(prim->h >> 1);
    corner1->vz = 0;
    corner2->vx = -(prim->w >> 1);
    corner2->vy = prim->h >> 1;
    corner2->vz = 0;
    corner3->vx = prim->w >> 1;
    corner3->vy = prim->h >> 1;
    corner3->vz = 0;
    otz = RotTransPers4((s32)corner0, (s32)corner1, (s32)corner2, (s32)corner3, (s32)&desc->x0,
                        (s32)&desc->x1, (s32)&desc->x2, (s32)&desc->x3, &p, &flag);
    ot = PRIM_DESC_OT;
    code = desc->code;
    desc->code = code & 0xFE;
    switch (code & 0xFC) {
    case 0x28:
        PRIM_DESC_PACKETS = addPrimDescF4(desc, ot, PRIM_DESC_PACKETS, otz);
        break;
    case 0x2C:
        desc->u1 = desc->u3 = desc->u0 + desc->u1;
        desc->u2 = desc->u0;
        desc->v3 = desc->v2 = desc->v0 + desc->v1;
        desc->v1 = desc->v0;
        PRIM_DESC_PACKETS = addPrimDescFT4(desc, ot, PRIM_DESC_PACKETS, otz);
        break;
    }
}

void drawPrimDesc(PrimDesc *desc) {
    PrimDesc *s;
    u32 *ot;
    s32 z;
    u8 code;

    s = SCRATCH_PRIM_DESC;
    *s = *desc;
    z = s->z;
    ot = PRIM_DESC_OT;
    code = s->code;
    s->code = code & 0xFE;
    switch (code & 0xFC) {
    case 0x64:
        PRIM_DESC_PACKETS = addPrimDescSprite(s, ot, PRIM_DESC_PACKETS, z);
        break;
    case 0x28:
        s->x1 = s->x3 = s->x0 + s->x1;
        s->x2 = s->x0;
        s->y3 = s->y2 = s->y0 + s->y1;
        s->y1 = s->y0;
        PRIM_DESC_PACKETS = addPrimDescF4(s, ot, PRIM_DESC_PACKETS, z);
        break;
    case 0x2C:
        s->x1 = s->x3 = s->x0 + s->x1;
        s->x2 = s->x0;
        s->y3 = s->y2 = s->y0 + s->y1;
        s->y1 = s->y0;
        s->u1 = s->u3 = s->u0 + s->u1;
        s->u2 = s->u0;
        s->v3 = s->v2 = s->v0 + s->v1;
        s->v1 = s->v0;
        PRIM_DESC_PACKETS = addPrimDescFT4(s, ot, PRIM_DESC_PACKETS, z);
        break;
    case 0x38:
        s->x1 = s->x3 = s->x0 + s->x1;
        s->x2 = s->x0;
        s->y3 = s->y2 = s->y0 + s->y1;
        s->y1 = s->y0;
        PRIM_DESC_PACKETS = addPrimDescG4(s, ot, PRIM_DESC_PACKETS, z);
        break;
    case 4:
        /* a line: an F4 as wide as clut, from (x0, y0) to (x1, y1) */
        s->code = (desc->code & 3) | 0x28;
        if (s->tpage != 0) {
            s->x2 = s->x0;
            s->y2 = s->y0 + s->clut;
            s->x3 = s->x1;
            s->y3 = s->y1 + s->clut;
        } else {
            s->x2 = s->x1;
            s->y2 = s->y1;
            s->x1 = s->x0 + s->clut;
            s->y1 = s->y0;
            s->x3 = s->x2 + s->clut;
            s->y3 = s->y2;
        }
        PRIM_DESC_PACKETS = addPrimDescF4(s, ot, PRIM_DESC_PACKETS, z);
        break;
    }
}

#define XY(x, y) (((y) << 16) | (u16)(x))
#define UV(u, v, hi) (((hi) << 16) | ((v) << 8) | (u))

/* a sprite with its texture page: a DR_MODE and a SPRT in one packet */
u32 *addPrimDescSprite(PrimDesc *desc, u32 *ot, u32 *packet, s32 z) {
    packet[0] = 0x05000000;
    packet[1] = desc->tpage | 0xE1000000;
    packet[2] = *(u32 *)&desc->r0;
    packet[3] = XY(desc->x0, desc->y0);
    packet[4] = UV(desc->u0, desc->v0, desc->clut);
    packet[5] = (desc->y1 << 16) | desc->x1;
    AddPrim((s32 *)&ot[z], (s32)packet);
    return packet + 6;
}

u32 *addPrimDescF4(PrimDesc *desc, u32 *ot, u32 *packet, s32 z) {
    packet[0] = 0x05000000;
    packet[1] = *(u32 *)&desc->r0;
    packet[2] = XY(desc->x0, desc->y0);
    packet[3] = XY(desc->x1, desc->y1);
    packet[4] = XY(desc->x2, desc->y2);
    packet[5] = XY(desc->x3, desc->y3);
    AddPrim((s32 *)&ot[z], (s32)packet);
    return packet + 6;
}

u32 *addPrimDescFT4(PrimDesc *desc, u32 *ot, u32 *packet, s32 z) {
    packet[0] = 0x09000000;
    packet[1] = *(u32 *)&desc->r0;
    packet[2] = XY(desc->x0, desc->y0);
    packet[3] = UV(desc->u0, desc->v0, desc->clut);
    packet[4] = XY(desc->x1, desc->y1);
    packet[5] = UV(desc->u1, desc->v1, desc->tpage);
    packet[6] = XY(desc->x2, desc->y2);
    packet[7] = (desc->v2 << 8) | desc->u2;
    packet[8] = XY(desc->x3, desc->y3);
    packet[9] = (desc->v3 << 8) | desc->u3;
    AddPrim((s32 *)&ot[z], (s32)packet);
    return packet + 10;
}

u32 *addPrimDescG4(PrimDesc *desc, u32 *ot, u32 *packet, s32 z) {
    packet[0] = 0x08000000;
    packet[1] = *(u32 *)&desc->r0;
    packet[2] = XY(desc->x0, desc->y0);
    packet[3] = desc->rgb1;
    packet[4] = XY(desc->x1, desc->y1);
    packet[5] = desc->rgb2;
    packet[6] = XY(desc->x2, desc->y2);
    packet[7] = desc->rgb3;
    packet[8] = XY(desc->x3, desc->y3);
    AddPrim((s32 *)&ot[z], (s32)packet);
    return packet + 9;
}

/* card_render's, a copy of its own */
static MATRIX *buildPrimDescMatrix(VECTOR *pos, SVECTOR *rot, MATRIX *m) {
    MATRIX tmp;
    SVECTOR r;

    r.vx = 0;
    r.vy = rot->vy;
    r.vz = 0;
    RotMatrix(&r, m);
    r.vx = rot->vx;
    r.vy = 0;
    r.vz = 0;
    RotMatrix(&r, &tmp);
    MulMatrix(m, &tmp);
    r.vx = 0;
    r.vy = 0;
    r.vz = rot->vz;
    RotMatrix(&r, &tmp);
    MulMatrix2(&tmp, m);
    MatrixNormal(m, &tmp);
    TransposeMatrix(&tmp, m);
    m->t[0] = pos->vx;
    m->t[1] = pos->vy;
    m->t[2] = pos->vz;
    return m;
}

void clearCallbackSlots(s16 first, s16 count) {
    s16 i;

    CURRENT_CALLBACK_SLOT = NULL;
    for (i = first; i < first + count; i++) {
        CALLBACK_SLOTS[i] = NULL;
    }
}

void runCallbackSlots(void *arg, s16 first, s16 count) {
    s16 i;

    for (i = first; i < first + count; i++) {
        if (CALLBACK_SLOTS[i] != NULL) {
            CURRENT_CALLBACK_SLOT = &CALLBACK_SLOTS[i];
            CALLBACK_SLOTS[i](arg);
        }
    }
}

/* value's decimal digits into digits, the units first; returns how many */
s16 splitDigits(s32 value, u8 *digits) {
    s16 count;
    s16 i;
    s32 digit = 0;

    if (value == 0) {
        return digit;
    }
    for (i = 1; value / intPow(10, i) != 0; i++) {
    }
    count = i;
    for (i = 1; i < count + 1; i++) {
        switch ((s16)(i - 1)) {
        case 0:
            digit = value % 10;
            break;
        case 1:
            digit = value % 100;
            digit /= 10;
            break;
        case 2:
            digit = value % 1000;
            digit /= 100;
            break;
        case 3:
            digit = value % 10000;
            digit /= 1000;
            break;
        case 4:
            digit = value % 100000;
            digit /= 10000;
            break;
        case 5:
            digit = value % 1000000;
            digit /= 100000;
            break;
        case 6:
            digit = value % 10000000;
            digit /= 1000000;
            break;
        case 7:
            digit = value % 100000000;
            digit /= 10000000;
            break;
        case 8:
            digit = value % 1000000000;
            digit /= 100000000;
            break;
        }
        *digits++ = digit;
    }
    return count;
}

s32 intPow(s32 base, s32 exponent) {
    s32 result = 1;

    while (exponent-- > 0) {
        result *= base;
    }
    return result;
}

void allocPrimDescPackets(s16 count) {
    s32 size = count * 0x28;

    ((Graphics *)&GRAPHICS)->buffers[0].primSlots[16] = (s32)allocHeapBlock(size, 0x258);
    ((Graphics *)&GRAPHICS)->buffers[1].primSlots[16] = (s32)allocHeapBlock(size, 0x259);
}

void freePrimDescPackets(void) {
    freeHeapBlocksByTag(0x259);
    freeHeapBlocksByTag(0x258);
    ((Graphics *)&GRAPHICS)->buffers[0].primSlots[16] = 0;
    ((Graphics *)&GRAPHICS)->buffers[1].primSlots[16] = 0;
}

/* the 320x240 picture at texture pages 0x1B and 0x1C, as two sprites */
SpriteDesc FULLSCREEN_BG_LEFT = { 0xF, 0x80, 0x80, 0x80, 0x64, 0, 0, 0x7F39, 0, 0x1B, 0, 0, 0x100, 0xF0 };
SpriteDesc FULLSCREEN_BG_RIGHT = { 0xF, 0x80, 0x80, 0x80, 0x64, 0xC0, 0, 0x7F39, 0, 0x1C, 0x100, 0, 0x40, 0xF0 };

void renderFullscreenBackground(void) {
    drawPrimDesc((PrimDesc *)&FULLSCREEN_BG_LEFT);
    drawPrimDesc((PrimDesc *)&FULLSCREEN_BG_RIGHT);
}
