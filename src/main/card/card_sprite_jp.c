#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/card_render.h"
#include "dcb/duel_launch.h"
#include "dcb/heap.h"
#include "dcb/prim_util.h"

/* jp's own: the duel's card polys and the card sprite renderer (card_render's
   in us), then the blinking cursor the duel, KAWSEG, SAISEG and NISSEG draw
   (KAWSEG's in us) */

#define setRGB2(p, _r2, _g2, _b2) (p)->r2 = _r2, (p)->g2 = _g2, (p)->b2 = _b2
#define setRGB3(p, _r3, _g3, _b3) (p)->r3 = _r3, (p)->g3 = _g3, (p)->b3 = _b3

/* a cursor: eight gradient quads around a point, or around a card */
typedef struct {
    /* 0x00 */ s16 mode; /* 0: around shape->sprite, in 3D; 1: on screen */
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
    /* 0x00 */ DR_MODE dm;
    /* 0x08 */ POLY_G4 prims[8];
} GradPacket;

extern RawPolyFT4 *CARD_POLY_PACKETS;
extern s32 CARD_POLY_COUNT;
extern GradPacket *CURSOR_PACKETS;

void KAW_initCursorShape(Shape *shape, s32 x, s32 y, s32 d);
void KAW_drawCursor(void *cursor);
void KAW_renderCursor(void *cursor, s32 otz);

/* COUNT cards' polys (two each, per frame buffer) and their sprites */
CardSprite *KAW_allocCardPolys(s32 count) {
    s32 i;

    CARD_POLY_PACKETS = allocTaskHeapBlock(count * sizeof(RawPolyFT4) * 2 * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[10] = (s32)&CARD_POLY_PACKETS[(i << 1) * count];
    }
    return allocTaskHeapBlock(count * sizeof(CardSprite));
}

void KAW_freeCardPolys(void) {
    freeHeapBlock(CARD_POLY_PACKETS);
}

void resetCardPolyCount(void) {
    CARD_POLY_COUNT = 0;
}

/* the card's frame (or its back) and, face up, its art, at depth OTZ */
void renderCardSprite(CardSprite *sprite, s32 otz) {
    MATRIX matrix;
    SVECTOR vertices[4];
    s32 sxy[4];
    s32 depthCue;
    s32 z;
    s32 flag;
    s32 art0, art1, art2, art3;
    u32 *col;
    RawPolyFT4 *buf;
    RawPolyFT4 *pk;
    u16 clut;
    u16 tpage;
    u8 u0, v0, u1, v1, u2, v2, u3, v3;

    if (!(sprite->flags & 0x80)) {
        return;
    }
    buildRotTransMatrix(&sprite->pos, &sprite->rot, &matrix);
    CompMatrix((MATRIX *)SCENE_3D->viewMatrix, &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    SetTransMatrix(&matrix);
    vertices[0].vx = -(sprite->scale * 40) / 8192;
    vertices[0].vy = -(sprite->scale * 48) / 8192;
    vertices[0].vz = 0;
    vertices[1].vx = (sprite->scale * 40) / 8192;
    vertices[1].vy = -(sprite->scale * 48) / 8192;
    vertices[1].vz = 0;
    vertices[2].vx = -(sprite->scale * 40) / 8192;
    vertices[2].vy = (sprite->scale * 48) / 8192;
    vertices[2].vz = 0;
    vertices[3].vx = (sprite->scale * 40) / 8192;
    vertices[3].vy = (sprite->scale * 48) / 8192;
    vertices[3].vz = 0;
    col = (u32 *)sprite->fade;
    buf = (RawPolyFT4 *)CURRENT_FRAME_BUFFER->primSlots[10];
    if (RotAverageNclip4((s32)&vertices[0], (s32)&vertices[1], (s32)&vertices[2], (s32)&vertices[3], (s32)&sxy[0],
                         (s32)&sxy[1], (s32)&sxy[2], (s32)&sxy[3], &depthCue, &z, &flag) <= 0) {
        /* the back */
        z = RotAverage4(&vertices[1], &vertices[0], &vertices[3], &vertices[2], &sxy[0], &sxy[1], &sxy[2], &sxy[3],
                        &depthCue, &flag);
        tpage = 0x9D;
        clut = 0x3E2C;
        u0 = 0;
        v0 = 0xC0;
        u1 = 0x27;
        v1 = 0xC0;
        u2 = 0;
        v2 = 0xEF;
        u3 = 0x27;
        v3 = 0xEF;
    } else {
        /* the art, then the frame over it */
        vertices[0].vx = -(sprite->scale * 40) / 8192;
        vertices[0].vy = -(sprite->scale * 42) / 8192;
        vertices[0].vz = 0;
        vertices[1].vx = (sprite->scale * 40) / 8192;
        vertices[1].vy = -(sprite->scale * 42) / 8192;
        vertices[1].vz = 0;
        vertices[2].vx = -(sprite->scale * 40) / 8192;
        vertices[2].vy = (sprite->scale * 38) / 8192;
        vertices[2].vz = 0;
        vertices[3].vx = (sprite->scale * 40) / 8192;
        vertices[3].vy = (sprite->scale * 38) / 8192;
        vertices[3].vz = 0;
        u0 = sprite->u;
        v0 = sprite->v;
        u1 = sprite->u + 40;
        v1 = v0;
        u2 = u0;
        v2 = sprite->v + 39;
        u3 = u1;
        v3 = v2;
        z = RotAverage4(&vertices[0], &vertices[1], &vertices[2], &vertices[3], &art0, &art1, &art2, &art3,
                        &depthCue, &flag);
        pk = &buf[CARD_POLY_COUNT++];
        pk->tag = 0x09000000;
        pk->rgbc = *col;
        pk->xy0 = art0;
        pk->uv0 = (sprite->clut << 16) | (v0 << 8) | u0;
        pk->xy1 = art1;
        pk->uv1 = (sprite->tpage << 16) | (v1 << 8) | u1;
        pk->xy2 = art2;
        pk->uv2 = (v2 << 8) | u2;
        pk->xy3 = art3;
        pk->uv3 = (v3 << 8) | u3;
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], pk);
        tpage = 0x1A;
        clut = getClut(0x300, 0x1F9 + sprite->pal);
        u0 = 0;
        v0 = 0;
        u1 = 0x28;
        v1 = 0;
        u2 = 0;
        v2 = 0x30;
        u3 = 0x28;
        v3 = 0x30;
    }
    sprite->sx = sxy[0];
    sprite->sy = sxy[0] >> 16;
    pk = &buf[CARD_POLY_COUNT++];
    pk->tag = 0x09000000;
    pk->rgbc = *col;
    pk->xy0 = sxy[0];
    pk->uv0 = (clut << 16) | (v0 << 8) | u0;
    pk->xy1 = sxy[1];
    pk->uv1 = (tpage << 16) | (v1 << 8) | u1;
    pk->xy2 = sxy[2];
    pk->uv2 = (v2 << 8) | u2;
    pk->xy3 = sxy[3];
    pk->uv3 = (v3 << 8) | u3;
    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], pk);
}

MATRIX *buildRotTransMatrix(VECTOR *pos, SVECTOR *rot, MATRIX *m) {
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
    freeHeapBlock(CURSOR_PACKETS);
    freeHeapBlock(ptr);
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
    KAW_renderCursor(cursor, 0);
}

#define setXY4(p, _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3)                                  \
    (p)->x0 = _x0, (p)->y0 = _y0, (p)->x1 = _x1, (p)->y1 = _y1, (p)->x2 = _x2, (p)->y2 = _y2, \
    (p)->x3 = _x3, (p)->y3 = _y3

#define SCALE_COORD(dst, src, scale) ((dst) = (src) * (scale) / 8192)

#define SET_SCALED_VERTEX(v, px, py, scale) \
    (SCALE_COORD((v).vx, px, scale), SCALE_COORD((v).vy, py, scale), (v).vz = 0)

#define SET_SPRITE_MATRIX(sprite, m)                                \
    (buildRotTransMatrix(&(sprite)->pos, &(sprite)->rot, m),         \
     CompMatrix((MATRIX *)SCENE_3D->viewMatrix, m, m), SetRotMatrix((s32)(m)), SetTransMatrix(m))

void KAW_renderCursor(void *cursor, s32 otz) {
    Shape *shape;
    GradPacket *pk;
    MATRIX matrix;
    SVECTOR v[4];
    s32 sxy[4];
    s32 depthCue;
    s32 flag;
    u8 level;
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
