#include "common.h"
#include "game.h"
#include "dcb/kaw_hud.h"
#include "dcb/heap.h"
#include "dcb/text.h"
#include "dcb/card_render.h"
#include "dcb/prim_util.h"
#include "dcb/kawseg.h"

#define RING ((DuelRing *)DUEL_STATE)

typedef struct {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ CardSprite *sprite;
    /* 0x08 */ s16 offsetX;
    /* 0x0A */ s16 offsetY;
    /* 0x0C */ s16 x;
    /* 0x0E */ s16 y;
    /* 0x10 */ s32 index;
    /* 0x14 */ u8 rgb[3];
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s16 cur[64];
    /* 0x98 */ s16 points[64];
} Shape;

typedef struct {
    u8 data[0x14F0];
} Unk14F0;

typedef struct {
    /* 0x00 */ DR_MODE dm;
    /* 0x08 */ POLY_G4 prims[8];
} GradPacket;

typedef struct {
    /* 0x000 */ u8 unk0[0x828];
    /* 0x828 */ s32 mode;
    /* 0x82C */ s32 cx;
    /* 0x830 */ s32 cy;
    /* 0x834 */ s32 radius;
    /* 0x838 */ s32 width;
} DuelRing;

typedef struct HudPanelK {
    /* 0x00 */ u8 rgb[4];
    /* 0x04 */ s16 clut;
    /* 0x06 */ s16 tpage;
    /* 0x08 */ u8 uv[4]; /* u, v, w, h */
    /* 0x0C */ u8 flags;
    /* 0x0D */ u8 state;
    /* 0x0E */ u8 unkE[2];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ u8 unk14[8];
    /* 0x1C */ s32 z;
    /* 0x20 */ struct HudPanelK *parent;
} HudPanelK;

typedef struct {
    /* 0x0 */ u8 uv[4]; /* u, v, w, h */
    /* 0x4 */ s16 tpage;
    /* 0x6 */ s16 clut;
    /* 0x8 */ u8 unk8;
    /* 0x9 */ s8 parent;
    /* 0xA */ s16 z;
} HudPanelInit;

extern Unk14F0 *CARD_POLY_PACKETS;
extern GradPacket *CURSOR_PACKETS;

void KAW_drawCursor(void *cursor);
void KAW_renderCursor(void *cursor, s32 otz);
void KAW_initCursorShape(Shape *shape, s32 x, s32 y, s32 d);

const Bytes4 KAW_PORTRAIT_COLOR = { { 0x80, 0x80, 0x80, 0 } };

HudPanelInit KAW_HUD_PANEL_INITS[12] = {
    { { 0, 0, 0xFF, 0x47 }, 0x1C, 0x7E30, 0, -1, 0xA },
    { { 0, 0, 0xD0, 0x40 }, 0x1E, 0x7C31, 0, -1, 0xA },
    { { 0, 0x7E, 0x74, 0x3C }, 0x1D, 0x7EF4, 0, -1, 0x64 },
    { { 0xA0, 0x47, 0x30, 0x14 }, 0x1C, 0x7E70, 0, 2, 0x64 },
    { { 0, 0x3F, 0xFF, 0x3F }, 0x1D, 0x7E74, 0, -1, 0x64 },
    { { 0xA8, 0x4E, 0x28, 0xE }, 0x1E, 0x7DB3, 0, 4, 0x64 },
    { { 0, 0, 0xFF, 0x47 }, 0x1C, 0x7E30, 0, -1, 0xA },
    { { 0, 0, 0xD0, 0x40 }, 0x1E, 0x7E31, 0, -1, 0xA },
    { { 0x74, 0x7E, 0x74, 0x3C }, 0x1D, 0x7EB4, 0, -1, 0x64 },
    { { 0xA0, 0x5B, 0x30, 0x14 }, 0x1C, 0x7EB0, 0, 8, 0x64 },
    { { 0, 0, 0xFF, 0x3F }, 0x1D, 0x7E34, 0, -1, 0x64 },
    { { 0xA8, 0x40, 0x28, 0xE }, 0x1E, 0x7D73, 0, 10, 0x64 },
};

s32 KAW_initHudPanels(void) {
    s32 i;

    KAW_DUEL->hudPrims = allocTaskHeapBlock(sizeof(HudPrims) * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[11] = (s32)&KAW_DUEL->hudPrims[i];
    }
    HUD_PANELS = allocTaskHeapBlock(sizeof(HudPanelK) * 12);
    for (i = 0; i < 12; i++) {
        ((HudPanelK *)HUD_PANELS)[i].rgb[0] = 0x80;
        ((HudPanelK *)HUD_PANELS)[i].rgb[1] = 0x80;
        ((HudPanelK *)HUD_PANELS)[i].rgb[2] = 0x80;
        ((HudPanelK *)HUD_PANELS)[i].x = 0;
        ((HudPanelK *)HUD_PANELS)[i].y = 0;
        ((HudPanelK *)HUD_PANELS)[i].uv[0] = KAW_HUD_PANEL_INITS[i].uv[0];
        ((HudPanelK *)HUD_PANELS)[i].uv[1] = KAW_HUD_PANEL_INITS[i].uv[1];
        ((HudPanelK *)HUD_PANELS)[i].uv[2] = KAW_HUD_PANEL_INITS[i].uv[2];
        ((HudPanelK *)HUD_PANELS)[i].uv[3] = KAW_HUD_PANEL_INITS[i].uv[3];
        ((HudPanelK *)HUD_PANELS)[i].tpage = KAW_HUD_PANEL_INITS[i].tpage;
        ((HudPanelK *)HUD_PANELS)[i].clut = KAW_HUD_PANEL_INITS[i].clut;
        ((HudPanelK *)HUD_PANELS)[i].flags = 0;
        ((HudPanelK *)HUD_PANELS)[i].state = 0;
        if (KAW_HUD_PANEL_INITS[i].parent != -1) {
            ((HudPanelK *)HUD_PANELS)[i].parent = &((HudPanelK *)HUD_PANELS)[KAW_HUD_PANEL_INITS[i].parent];
        } else {
            ((HudPanelK *)HUD_PANELS)[i].parent = NULL;
        }
        ((HudPanelK *)HUD_PANELS)[i].z = KAW_HUD_PANEL_INITS[i].z;
    }
    DUEL_MSG_BAR.bannerState = 0;
    DUEL_MSG_BAR.playerLabel = 0;
    DUEL_MSG_BAR.phase = -1;
    DUEL_MSG_BAR.next = 0;
    DUEL_MSG_BAR.cur = -1;
    DUEL_MSG_BAR.y = 0;
    DUEL_MSG_BAR.next2 = 0;
    DUEL_MSG_BAR.cur2 = -1;
    DUEL_MSG_BAR.y2 = 0;
    DUEL_MSG_BAR.bannerLabel = -1;
    DUEL_MSG_BAR.bannerStep = -1;
    DUEL_MSG_BAR.bannerPhase = -1;
    DUEL_MSG_BAR.echoAge = 0;
    DUEL_MSG_BAR.px = 0;
    DUEL_MSG_BAR.py = 0;
    DUEL_MSG_BAR.tx = 0;
    DUEL_MSG_BAR.ty = 0;
}

s32 KAW_freeHudPanels(void) {
    freeHeapBlock(KAW_DUEL->hudPrims);
    freeHeapBlock(HUD_PANELS);
}

void KAW_drawPortrait(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, u16 a5) {
    Bytes4 color;

    color = KAW_PORTRAIT_COLOR;
    KAW_drawPortraitColored(a0, a1, a2, a3, a4, a5, &color);
}

void KAW_drawPortraitColored(s32 x, s32 y, s32 u, s32 v, s32 frame, u16 clut, Bytes4 *rgb) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x + 2;
        CUR_SPRT->sp.y0 = y + 5;
        CUR_SPRT->sp.u0 = (u % 64) * 2 + 2;
        CUR_SPRT->sp.v0 = v % 256 + 2;
        CUR_SPRT->sp.clut = clut;
        CUR_SPRT->sp.w = 36;
        CUR_SPRT->sp.h = 36;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = rgb->b[0];
        CUR_SPRT->sp.g0 = rgb->b[1];
        CUR_SPRT->sp.b0 = rgb->b[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, u, v));
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (frame >= 6) {
            frame = 5;
        }
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0xCC;
            CUR_SPRT->sp.v0 = 0x6F;
            CUR_SPRT->sp.clut = getClut(0x320, frame + 0x1F1);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = rgb->b[0];
            CUR_SPRT->sp.g0 = rgb->b[1];
            CUR_SPRT->sp.b0 = rgb->b[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, 0x300, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

s32 KAW_allocCardPolys(void) {
    s32 i;

    CARD_POLY_PACKETS = allocTaskHeapBlock(sizeof(Unk14F0) * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[10] = (s32)&CARD_POLY_PACKETS[i];
    }
    allocTaskHeapBlock(0xE10);
}

s32 KAW_freeCardPolys(void) {
    freeHeapBlock(CARD_POLY_PACKETS);
}

void KAW_showCardLabel(CardSprite *sprite, s32 num) {
    sprite->flags |= 0x20;
    sprite->num = num;
}

void KAW_hideCardLabel(CardSprite *sprite) {
    sprite->flags &= ~0x20;
}

void KAW_fadeCardSprite(CardSprite *sprite, u8 *to) {
    sprite->flags |= 0x40;
    sprite->t = 0;
    sprite->from[0] = sprite->fade[0];
    sprite->from[1] = sprite->fade[1];
    sprite->from[2] = sprite->fade[2];
    sprite->to[0] = to[0];
    sprite->to[1] = to[1];
    sprite->to[2] = to[2];
}

s32 KAW_createCursor(s32 mode, s32 x, s32 y, s32 d, s32 count) {
    s32 k;
    s32 i;
    s32 j;
    Shape *shapes;
    u8 *rgb;

    CURSOR_PACKETS = allocTaskHeapBlock(count * sizeof(GradPacket) * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[12] = (s32)&CURSOR_PACKETS[i * count];
        for (j = 0; j < count; j++) {
            setDrawMode(&CURSOR_PACKETS[i * count + j].dm, 0, 0, GetTPage(0, 1, 0, 0));
            for (k = 0; k < 8; k++) {
                initPrimByType(9, &CURSOR_PACKETS[i * count + j].prims[k], 1, 0);
                (CURSOR_PACKETS[i * count + j].prims + k)->r0 = 0;
                (CURSOR_PACKETS[i * count + j].prims + k)->g0 = 0;
                (CURSOR_PACKETS[i * count + j].prims + k)->b0 = 0;
                (CURSOR_PACKETS[i * count + j].prims + k)->r1 = 0;
                (CURSOR_PACKETS[i * count + j].prims + k)->g1 = 0;
                (CURSOR_PACKETS[i * count + j].prims + k)->b1 = 0;
            }
        }
    }
    shapes = allocTaskHeapBlock(count * sizeof(Shape));
    for (i = 0; i < count; i++) {
        shapes[i].unk2 = 0;
        shapes[i].mode = mode;
        shapes[i].index = i;
        rgb = shapes[i].rgb;
        rgb[0] = 0xFF;
        rgb[1] = 0xFF;
        rgb[2] = 0;
        KAW_initCursorShape(&shapes[i], x, y, d);
    }
    return (s32)shapes;
}

void KAW_initCursorShape(Shape *shape, s32 x, s32 y, s32 d) {
    s32 k;

    k = d * 14 / 10;
    shape->x = x;
    shape->y = y;
    shape->points[0] = -x;
    shape->points[1] = -y - k;
    shape->points[2] = x;
    shape->points[3] = -y - k;
    shape->points[4] = -x;
    shape->points[5] = -y;
    shape->points[6] = x;
    shape->points[7] = -y;
    shape->points[8] = x + k;
    shape->points[9] = -y;
    shape->points[10] = x + k;
    shape->points[11] = y;
    shape->points[12] = x;
    shape->points[13] = -y;
    shape->points[14] = x;
    shape->points[15] = y;
    shape->points[16] = -x - k;
    shape->points[17] = -y;
    shape->points[18] = -x - k;
    shape->points[19] = y;
    shape->points[20] = -x;
    shape->points[21] = -y;
    shape->points[22] = -x;
    shape->points[23] = y;
    shape->points[24] = -x;
    shape->points[25] = y + k;
    shape->points[26] = x;
    shape->points[27] = y + k;
    shape->points[28] = -x;
    shape->points[29] = y;
    shape->points[30] = x;
    shape->points[31] = y;
    shape->points[32] = -x - k;
    shape->points[33] = -y;
    shape->points[34] = -x - d;
    shape->points[35] = -y - d;
    shape->points[36] = -x;
    shape->points[37] = -y;
    shape->points[38] = -x;
    shape->points[39] = -y - k;
    shape->points[40] = x + k;
    shape->points[41] = -y;
    shape->points[42] = x + d;
    shape->points[43] = -y - d;
    shape->points[44] = x;
    shape->points[45] = -y;
    shape->points[46] = x;
    shape->points[47] = -y - k;
    shape->points[48] = x + k;
    shape->points[49] = y;
    shape->points[50] = x + d;
    shape->points[51] = y + d;
    shape->points[52] = x;
    shape->points[53] = y;
    shape->points[54] = x;
    shape->points[55] = y + k;
    shape->points[56] = -x - k;
    shape->points[57] = y;
    shape->points[58] = -x - d;
    shape->points[59] = y + d;
    shape->points[60] = -x;
    shape->points[61] = y;
    shape->points[62] = -x;
    shape->points[63] = y + k;
}

void KAW_freeCursor(void *ptr) {
    if (ptr != NULL) {
        freeHeapBlock(CURSOR_PACKETS);
        freeHeapBlock(ptr);
    }
}

void KAW_drawCursorAt(cursor, x, y)
    s16 *cursor;
    s16 x;
    s16 y;
{
    cursor[4] = x;
    cursor[5] = y;
    KAW_drawCursor(cursor);
}

void KAW_drawCursor(void *cursor) {
    KAW_renderCursor(cursor, 1);
}

#define setXY4(p, _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3)                                  \
    (p)->x0 = _x0, (p)->y0 = _y0, (p)->x1 = _x1, (p)->y1 = _y1, (p)->x2 = _x2, (p)->y2 = _y2, \
    (p)->x3 = _x3, (p)->y3 = _y3

#define SCALE_COORD(dst, src, scale) \
    do {                                \
        (dst) = (src) * (scale) / 8192; \
    } while (0)

#define SET_SCALED_VERTEX(v, px, py, scale) \
    do {                                    \
        SCALE_COORD((v).vx, px, scale);     \
        SCALE_COORD((v).vy, py, scale);     \
        (v).vz = 0;                         \
    } while (0)

#define SET_SPRITE_MATRIX(sprite, m)                                  \
    do {                                                              \
        buildRotTransMatrix(&(sprite)->pos, &(sprite)->rot, m);       \
        CompMatrix((MATRIX *)((u8 *)SCENE_3D + 0x78), m, m);          \
        SetRotMatrix((s32)(m));                                       \
        SetTransMatrix(m);                                             \
    } while (0)

void KAW_renderCursor(void *cursor, s32 otz) {
    Shape *shape;
    GradPacket *pk;
    MATRIX matrix;
    SVECTOR v[4];
    s32 sxy[4];
    s32 depthCue;
    s32 flag;
#if VERSION_US
    s32 level;
#elif VERSION_EU
    u8 level;
#else
#error "kawseg/ui/kaw_hud: version not checked"
#endif
    s32 i;
    CardSprite *sprite;

    shape = cursor;
    pk = (GradPacket *)CURRENT_FRAME_BUFFER->primSlots[12] + shape->index;
    i = (((PlayerProfile *)PLAYER_PROFILES)->playTime * 8) % 200;
    level = (u8)(i >= 100 ? 300 - i : i + 100);
    for (i = 0; i < 64; i += 2) {
        if (abs(shape->points[i]) == shape->x) {
            shape->cur[i] = shape->points[i];
        } else {
            shape->cur[i] = shape->x + (abs(shape->points[i]) - shape->x) * level / 100;
            if (shape->points[i] < 0) {
                shape->cur[i] *= -1;
            }
        }
        if (abs(shape->points[i + 1]) == shape->y) {
            shape->cur[i + 1] = shape->points[i + 1];
        } else {
            shape->cur[i + 1] = shape->y + (abs(shape->points[i + 1]) - shape->y) * level / 100;
            if (shape->points[i + 1] < 0) {
                shape->cur[i + 1] *= -1;
            }
        }
    }
    switch (shape->mode) {
    case 0:
        sprite = shape->sprite;
        SET_SPRITE_MATRIX(sprite, &matrix);
        for (i = 0; i < 8; i++) {
            SET_SCALED_VERTEX(v[0], shape->cur[i * 8 + 0], shape->cur[i * 8 + 1], sprite->scale);
            SET_SCALED_VERTEX(v[1], shape->cur[i * 8 + 2], shape->cur[i * 8 + 3], sprite->scale);
            SET_SCALED_VERTEX(v[2], shape->cur[i * 8 + 4], shape->cur[i * 8 + 5], sprite->scale);
            SET_SCALED_VERTEX(v[3], shape->cur[i * 8 + 6], shape->cur[i * 8 + 7], sprite->scale);
            RotAverage4(&v[0], &v[1], &v[2], &v[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
            setXY4(&pk->prims[i], sxy[0], sxy[0] >> 16, sxy[1], sxy[1] >> 16, sxy[2], sxy[2] >> 16, sxy[3],
                   sxy[3] >> 16);
            setRGB2(&pk->prims[i], shape->rgb[0], shape->rgb[1], shape->rgb[2]);
            if (i / 4 != 0) {
                setRGB3(&pk->prims[i], 0, 0, 0);
            } else {
                setRGB3(&pk->prims[i], shape->rgb[0], shape->rgb[1], shape->rgb[2]);
            }
            addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->prims[i]);
        }
        break;
    case 1:
        for (i = 0; i < 8; i++) {
            pk->prims[i].x0 = shape->cur[i * 8 + 0] + shape->offsetX;
            pk->prims[i].y0 = shape->cur[i * 8 + 1] + shape->offsetY;
            pk->prims[i].x1 = shape->cur[i * 8 + 2] + shape->offsetX;
            pk->prims[i].y1 = shape->cur[i * 8 + 3] + shape->offsetY;
            pk->prims[i].x2 = shape->cur[i * 8 + 4] + shape->offsetX;
            pk->prims[i].y2 = shape->cur[i * 8 + 5] + shape->offsetY;
            pk->prims[i].x3 = shape->cur[i * 8 + 6] + shape->offsetX;
            pk->prims[i].y3 = shape->cur[i * 8 + 7] + shape->offsetY;
            setRGB2(&pk->prims[i], shape->rgb[0], shape->rgb[1], shape->rgb[2]);
            if (i / 4 != 0) {
                setRGB3(&pk->prims[i], 0, 0, 0);
            } else {
                setRGB3(&pk->prims[i], shape->rgb[0], shape->rgb[1], shape->rgb[2]);
            }
            addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->prims[i]);
        }
        break;
    }
    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->dm);
}

void KAW_initRing(void) {
    s32 i;

    KAW_DUEL->ringPrims = allocTaskHeapBlock(sizeof(RingPrims) * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[13] = (s32)&KAW_DUEL->ringPrims[i];
    }
    DUEL->ringMode = -1;
    DUEL->ringRadius = 320;
}

void KAW_freeRing(void) {
    freeHeapBlock(KAW_DUEL->ringPrims);
}

void KAW_closeRing(s32 x, s32 y, s32 width) {
    DUEL->ringMode = 1;
    DUEL->ringX = x;
    DUEL->ringY = y;
    DUEL->ringWidth = width;
}

void KAW_openRing(void) {
    DUEL->ringMode = 0;
}

s32 KAW_renderRing(void) {
    PolyF4 *f4;
    POLY_G4 *g4;
    DR_MODE *dm;
    s32 cx;
    s32 cy;
    s32 r;
    s32 i;

    switch (RING->mode) {
    case 0:
        if ((RING->radius += 16) > 0x140) {
            RING->mode = -1;
            RING->radius = 0x140;
            return;
        }
        break;
    case 1:
        if ((RING->radius -= 16) < 0) {
            RING->radius = 0;
        }
        break;
    }
    cx = RING->cx;
    cy = RING->cy;
    r = RING->radius;
    f4 = ((RingPrims *)CURRENT_FRAME_BUFFER->primSlots[13])->edges;
    for (i = 0; i < 32; i++, f4++) {
        initPrimByType(8, f4, 1, 0);
        setRGB0(f4, 0xA0, 0xA0, 0xA0);
        f4->x0 = cx + (r + RING->width + 32 + r) * rsin(i << 7) / 4096;
        f4->y0 = cy + (r + RING->width + 32 + r) * rcos(i << 7) / 4096;
        f4->x1 = cx + (r + RING->width + 32 + r) * rsin((i + 1) << 7) / 4096;
        f4->y1 = cy + (r + RING->width + 32 + r) * rcos((i + 1) << 7) / 4096;
        f4->x2 = cx + rsin(i << 7) * 400 / 4096;
        f4->y2 = cy + rcos(i << 7) * 400 / 4096;
        f4->x3 = cx + rsin((i + 1) << 7) * 400 / 4096;
        f4->y3 = cy + rcos((i + 1) << 7) * 400 / 4096;
        addPrim(&CURRENT_FRAME_BUFFER->ot[1], f4);
    }
    g4 = ((RingPrims *)CURRENT_FRAME_BUFFER->primSlots[13])->fades;
    for (i = 0; i < 32; i++, g4++) {
        initPrimByType(9, g4, 1, 0);
        setRGB0(g4, 0, 0, 0);
        setRGB1(g4, 0, 0, 0);
        setRGB2(g4, 0xA0, 0xA0, 0xA0);
        setRGB3(g4, 0xA0, 0xA0, 0xA0);
        g4->x0 = cx + (r + RING->width) * rsin(i << 7) / 4096;
        g4->y0 = cy + (r + RING->width) * rcos(i << 7) / 4096;
        g4->x1 = cx + (r + RING->width) * rsin((i + 1) << 7) / 4096;
        g4->y1 = cy + (r + RING->width) * rcos((i + 1) << 7) / 4096;
        g4->x2 = cx + (r + RING->width + 32 + r) * rsin(i << 7) / 4096;
        g4->y2 = cy + (r + RING->width + 32 + r) * rcos(i << 7) / 4096;
        g4->x3 = cx + (r + RING->width + 32 + r) * rsin((i + 1) << 7) / 4096;
        g4->y3 = cy + (r + RING->width + 32 + r) * rcos((i + 1) << 7) / 4096;
        addPrim(&CURRENT_FRAME_BUFFER->ot[1], g4);
    }
    dm = &((RingPrims *)CURRENT_FRAME_BUFFER->primSlots[13])->dm;
    SetDrawTPage(dm, 0, 0, GetTPage(0, 2, 0, 0));
    addPrim(&CURRENT_FRAME_BUFFER->ot[1], dm);
}
