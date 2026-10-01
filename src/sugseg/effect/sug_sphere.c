#include "common.h"
#include "game.h"
#include "dcb/sug_sphere.h"
#include "dcb/heap.h"
#include "dcb/effect_object.h"
#include "dcb/scroll_bg.h"
#include "dcb/prim.h"
#include "dcb/prim_util.h"
#include "dcb/sug_tex_anim.h"

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 cd;
} Rgb;

/* called with one argument more than their definitions take (a trailing 1) */
void initLineF2Pair();

void initPolyF3Pair();
void initPolyG3Pair();
void initPolyGT3Pair();
void SUG_drawSphereLines(SphereEffect *fx, u8 cull, s32 n, s32 speed, s32 otz);
void SUG_drawSphereF(SphereEffect *fx, s32 cull, s32 count, s32 speed, s32 otz);
void SUG_drawSphereG(SphereEffect *fx, s32 cull, s32 count, s32 speed, s32 otz);
void SUG_drawSphereGT(SphereEffect *fx, s32 cull, s32 count, s32 speed, s32 otz);
void SUG_setSphereColor(SphereEffect *fx, Bytes4 *rgb, s16 pulse, s16 pulseMode);

SphereEffect *SUG_createSphereEffect(s16 brightness, u8 *color, s16 pulse, s16 pulseMode, EffectTemplate *template, s16 segments, s16 slices, s32 radius, u8 semiTrans,
                           u8 abr, u8 primKind, u8 openBottom, s16 texAnimId, Rect16 *uv, s32 tpage, s32 clut, u8 cull, s32 otz, s32 pak) {
    SphereEffect *fx;
    s32 rings;
    s32 total;
    s32 i;
    s32 n;
    s32 k;
    s32 r;
    s16 angStep;
    s16 latStep;
    s16 ringStep;
    LineF2 *line0;
    LineF2 *line1;
    POLY_F3 *tri0;
    POLY_F3 *tri1;
    POLY_F4 *quad0;
    POLY_F4 *quad1;
    POLY_G3 *gtri0;
    POLY_G3 *gtri1;
    POLY_G4 *gquad0;
    POLY_G4 *gquad1;
    POLY_GT3 *ttri0;
    POLY_GT3 *ttri1;
    POLY_GT4 *tquad0;
    POLY_GT4 *tquad1;
    DrTPage *tp0;
    DrTPage *tp1;

    fx = allocTaskHeapBlock(sizeof(SphereEffect));
    fx->segments = segments;
    fx->slices = slices;
    rings = fx->ringCount = (slices - 3) / 2 + 1;
    total = rings * segments;
    fx->vertCount = total + 2;
    fx->ringVertCount = (fx->ringCount - 1) * segments + segments * 2;
    fx->lineCount = total + (fx->ringCount - 1) * segments + segments * 2;
    if (texAnimId >= 0 && SUG_startTexAnim(texAnimId, 3, (RingEffect *)fx, &fx->texAnim, pak) != 0) {
        fx->texAnimActive = 1;
    } else {
        fx->texAnimActive = -1;
    }
    fx->verts = allocTaskHeapBlock(fx->vertCount * sizeof(SVECTOR));
    fx->kind = primKind;
    fx->semiTrans = semiTrans;
    fx->openBottom = openBottom;
    fx->abr = abr;
    fx->brightness = brightness;
    SUG_setSphereColor(fx, (Bytes4 *)color, pulse, pulseMode);
    fx->cull = cull;
    fx->otz = otz;
    *(EffectTemplate *)fx = *template;
    initEffectObject(fx);
    for (i = 0; i < 2; i++) {
        fx->tris[i] = NULL;
        fx->quads[i] = NULL;
        fx->gtris[i] = NULL;
        fx->gquads[i] = NULL;
        fx->ttris[i] = NULL;
        fx->tquads[i] = NULL;
        fx->lines[i] = NULL;
        fx->tpages[i] = NULL;
    }
    if (openBottom != 0) {
        fx->ringVertCount /= 2;
    }
    switch (primKind) {
    case 0:
        for (i = 0; i < 2; i++) {
            fx->lines[i] = allocTaskHeapBlock(fx->lineCount * sizeof(LineF2));
        }
        if (semiTrans != 0) {
            for (i = 0; i < 2; i++) {
                fx->tpages[i] = allocTaskHeapBlock(fx->lineCount * sizeof(DrTPage));
            }
        }
        line0 = fx->lines[0];
        line1 = fx->lines[1];
        tp0 = fx->tpages[0];
        tp1 = fx->tpages[1];
        for (i = 0; i < fx->lineCount; i++, line0++, line1++, tp0++, tp1++) {
            if (semiTrans == 0) {
                initLineF2Pair(line0, line1, 0, abr, 0, 0, 0, 1);
            } else {
                initLineF2Pair(line0, line1, 0, abr, tp0, tp1, 1, 1);
            }
        }
        break;
    case 8:
        if (semiTrans != 0) {
            for (i = 0; i < 2; i++) {
                fx->tpages[i] = allocTaskHeapBlock(fx->ringVertCount * sizeof(DrTPage));
            }
        }
        tp0 = fx->tpages[0];
        tp1 = fx->tpages[1];
        if (openBottom != 0) {
            n = segments;
        } else {
            n = segments * 2;
        }
        for (i = 0; i < 2; i++) {
            fx->tris[i] = allocTaskHeapBlock(n * sizeof(POLY_F3));
        }
        tri0 = fx->tris[0];
        tri1 = fx->tris[1];
        for (i = 0; i < n; i++, tri0++, tri1++, tp0++, tp1++) {
            if (semiTrans == 0) {
                initPolyF3Pair(tri0, tri1, 0, abr, 0, 0, 0, 1);
            } else {
                initPolyF3Pair(tri0, tri1, 0, abr, tp0, tp1, 1, 1);
            }
        }
        if (fx->ringVertCount - n > 0) {
            for (i = 0; i < 2; i++) {
                fx->quads[i] = allocTaskHeapBlock((fx->ringVertCount - n) * sizeof(POLY_F4));
            }
            quad0 = fx->quads[0];
            quad1 = fx->quads[1];
            for (i = 0; i < fx->ringVertCount - n; i++, quad0++, quad1++, tp0++, tp1++) {
                if (semiTrans == 0) {
                    initPolyF4Pair(quad0, quad1, 0, abr, 0, 0, 0, 0, 1);
                } else {
                    initPolyF4Pair(quad0, quad1, 0, abr, tp0, tp1, 0, 1, 1);
                }
            }
        }
        break;
    case 9:
        if (semiTrans != 0) {
            for (i = 0; i < 2; i++) {
                fx->tpages[i] = allocTaskHeapBlock(fx->ringVertCount * sizeof(DrTPage));
            }
        }
        tp0 = fx->tpages[0];
        tp1 = fx->tpages[1];
        if (openBottom != 0) {
            n = segments;
        } else {
            n = segments * 2;
        }
        for (i = 0; i < 2; i++) {
            fx->gtris[i] = allocTaskHeapBlock(n * sizeof(POLY_G3));
        }
        gtri0 = fx->gtris[0];
        gtri1 = fx->gtris[1];
        for (i = 0; i < n; i++, gtri0++, gtri1++, tp0++, tp1++) {
            if (semiTrans == 0) {
                initPolyG3Pair(gtri0, gtri1, 0, 0, 0, abr, 0, 0, 0, 1);
            } else {
                initPolyG3Pair(gtri0, gtri1, 0, 0, 0, abr, tp0, tp1, 1, 1);
            }
        }
        if (fx->ringVertCount - n > 0) {
            for (i = 0; i < 2; i++) {
                fx->gquads[i] = allocTaskHeapBlock((fx->ringVertCount - n) * sizeof(POLY_G4));
            }
            gquad0 = fx->gquads[0];
            gquad1 = fx->gquads[1];
            for (i = 0; i < fx->ringVertCount - n; i++, gquad0++, gquad1++, tp0++, tp1++) {
                if (semiTrans == 0) {
                    initPolyG4Pair(gquad0, gquad1, 0, 0, 0, 0, abr, 0, 0, 0, 0, 1);
                } else {
                    initPolyG4Pair(gquad0, gquad1, 0, 0, 0, 0, abr, tp0, tp1, 0, 1, 1);
                }
            }
        }
        break;
    case 13:
        if (openBottom != 0) {
            n = segments;
        } else {
            n = segments * 2;
        }
        for (i = 0; i < 2; i++) {
            fx->ttris[i] = allocTaskHeapBlock(n * sizeof(POLY_GT3));
        }
        ttri0 = fx->ttris[0];
        ttri1 = fx->ttris[1];
        for (i = 0; i < n; i++, ttri0++, ttri1++) {
            if (semiTrans == 0) {
                initPolyGT3Pair(ttri0, ttri1, 0, 0, 0, tpage, clut, 0, 0, 0, 1);
            } else {
                initPolyGT3Pair(ttri0, ttri1, 0, 0, 0, tpage, clut, 0, 0, 1, 1);
            }
            if (i / segments == 0) {
                ttri0->u0 = uv->x + uv->w;
                ttri0->v0 = uv->y + uv->h;
                ttri0->u1 = uv->x;
                ttri0->v1 = uv->y + uv->h;
                ttri0->u2 = uv->w / 2 + uv->x;
                ttri0->v2 = uv->y;
                ttri1->u0 = uv->x + uv->w;
                ttri1->v0 = uv->y + uv->h;
                ttri1->u1 = uv->x;
                ttri1->v1 = uv->y + uv->h;
                ttri1->u2 = uv->w / 2 + uv->x;
                ttri1->v2 = uv->y;
            } else {
                ttri0->u0 = uv->w / 2 + uv->x;
                ttri0->v0 = uv->y + uv->h;
                ttri0->u1 = uv->x;
                ttri0->v1 = uv->y;
                ttri0->u2 = uv->x + uv->w;
                ttri0->v2 = uv->y;
                ttri1->u0 = uv->w / 2 + uv->x;
                ttri1->v0 = uv->y + uv->h;
                ttri1->u1 = uv->x;
                ttri1->v1 = uv->y;
                ttri1->u2 = uv->x + uv->w;
                ttri1->v2 = uv->y;
            }
        }
        if (fx->ringVertCount - n > 0) {
            for (i = 0; i < 2; i++) {
                fx->tquads[i] = allocTaskHeapBlock((fx->ringVertCount - n) * sizeof(POLY_GT4));
            }
            tquad0 = fx->tquads[0];
            tquad1 = fx->tquads[1];
            for (i = 0; i < fx->ringVertCount - n; i++, tquad0++, tquad1++) {
                if (semiTrans == 0) {
                    initPolyGT4Pair(tquad0, tquad1, 0, 0, 0, 0, tpage, clut, uv, 0, 0, 1);
                } else {
                    initPolyGT4Pair(tquad0, tquad1, 0, 0, 0, 0, tpage, clut, uv, 0, 1, 1);
                }
            }
        }
        fx->uv = *uv;
        fx->tpage = tpage;
        fx->clut = clut;
        break;
    }
    angStep = 0x1000 / segments;
    latStep = 0x1000 / slices;
    ringStep = 0x800 / (fx->ringCount + 1);
    fx->verts[0].vx = 0;
    fx->verts[0].vy = -radius;
    fx->verts[0].vz = 0;
    fx->verts[fx->vertCount - 1].vx = 0;
    fx->verts[fx->vertCount - 1].vy = radius;
    fx->verts[fx->vertCount - 1].vz = 0;
    k = 1;
    for (n = 0; n < fx->ringCount; n++) {
        r = radius * rsin(ringStep * (n + 1)) >> 12;
        for (i = 0; i < segments; i++, k++) {
            fx->verts[k].vx = r * rcos(angStep * i) >> 12;
            fx->verts[k].vz = r * rsin(angStep * i) >> 12;
            fx->verts[k].vy = -(radius * rcos(latStep * (n + 1))) >> 12;
        }
    }
    return fx;
}

void SUG_setSphereColor(SphereEffect *fx, Bytes4 *rgb, s16 pulse, s16 pulseMode) {
    *(Bytes4 *)fx->rgb = *rgb;
    fx->pulse = pulse;
    fx->pulseMode = pulseMode;
    fx->prevBrightness = -1; /* make the next tick recolour the prims */
}

void SUG_tickSphereEffect(SphereEffect *fx) {
    /* jp and eu narrow the speed before passing it */
#if VERSION_JP || VERSION_EU
    s16 speed;
#elif VERSION_US
    s32 speed;
#endif

    if (fx->suspended != 0) {
        TICK_START_DELAY(fx);
        return;
    }
    PushMatrix();
    tickEffectMotion((s32)fx, 0);
    if ((fx->brightness = updateEffectBrightness(fx, fx->brightness)) == 0) {
        PopMatrix();
        return;
    }
    switch (fx->kind) {
    case 0:
        if (fx->pulseMode == 0) {
            speed = 0x800 / fx->ringCount;
        } else {
            speed = 0x400 / fx->ringCount;
        }
        SUG_drawSphereLines(fx, fx->cull, fx->segments, speed, fx->otz);
        break;
    case 8:
        if (fx->pulseMode == 0) {
            speed = 0x800 / fx->ringCount;
        } else {
            speed = 0x400 / fx->ringCount;
        }
        SUG_drawSphereF(fx, fx->cull, fx->segments, speed, fx->otz);
        break;
    case 9:
        if (fx->pulseMode == 0) {
            speed = 0x800 / (fx->ringCount + 1);
        } else {
            speed = 0x400 / (fx->ringCount + 1);
        }
        SUG_drawSphereG(fx, fx->cull, fx->segments, speed, fx->otz);
        break;
    case 13:
        if (fx->pulseMode == 0) {
            speed = 0x800 / (fx->ringCount + 1);
        } else {
            speed = 0x400 / (fx->ringCount + 1);
        }
        SUG_drawSphereGT(fx, fx->cull, fx->segments, speed, fx->otz);
        break;
    }
    PopMatrix();
}

void SUG_drawSphereLines(SphereEffect *fx, u8 cull, s32 n, s32 speed, s32 otz) {
    u8 rgb[3];
    s32 i;
    s32 j;
    s32 k;
    s32 v;

    if (fx->brightness != fx->prevBrightness) {
        rgb[0] = fx->rgb[0] * fx->brightness / 256;
        rgb[1] = fx->rgb[1] * fx->brightness / 256;
        rgb[2] = fx->rgb[2] * fx->brightness / 256;
    }
    for (i = 0; i < n; i++) {
        transformAndAddLineF2((s32)&fx->lines[FRAME_BUFFER_INDEX][i], (s32)&fx->tpages[FRAME_BUFFER_INDEX][i],
                              (s32)&fx->verts[0], (s32)&fx->verts[i + 1], fx->semiTrans, otz);
        transformAndAddLineF2((s32)&fx->lines[FRAME_BUFFER_INDEX][fx->lineCount - n + i],
                              (s32)&fx->tpages[FRAME_BUFFER_INDEX][fx->lineCount - n + i], (s32)&fx->verts[fx->vertCount - 1],
                              (s32)&fx->verts[fx->vertCount - n - 1 + i], fx->semiTrans, otz);
        if (fx->brightness != fx->prevBrightness) {
            for (k = 0; k < 2; k++) {
                setRGB0(&fx->lines[k][i], rgb[0], rgb[1], rgb[2]);
                setRGB0(&fx->lines[k][fx->lineCount - n + i], rgb[0], rgb[1], rgb[2]);
            }
        }
    }
    for (j = 0; j < fx->ringCount; j++) {
        for (i = 0, v = 1; i < n; i++, v++) {
            transformAndAddLineF2((s32)&fx->lines[FRAME_BUFFER_INDEX][i + (j + 1) * n],
                                  (s32)&fx->tpages[FRAME_BUFFER_INDEX][i + (j + 1) * n], (s32)&fx->verts[v + j * n],
                                  (s32)&fx->verts[v % n + 1 + j * n], fx->semiTrans, otz);
            if (fx->brightness != fx->prevBrightness) {
                for (k = 0; k < 2; k++) {
                    setRGB0(&fx->lines[k][i + (j + 1) * n], rgb[0], rgb[1], rgb[2]);
                }
            }
        }
    }
    for (i = 0, v = 1; i < (fx->ringCount - 1) * n; i++, v++) {
        transformAndAddLineF2((s32)&fx->lines[FRAME_BUFFER_INDEX][i + (fx->ringCount + 1) * n],
                              (s32)&fx->tpages[FRAME_BUFFER_INDEX][i + (fx->ringCount + 1) * n], (s32)&fx->verts[v],
                              (s32)&fx->verts[v + n], fx->semiTrans, otz);
        if (fx->brightness != fx->prevBrightness) {
            for (k = 0; k < 2; k++) {
                setRGB0(&fx->lines[k][i + (fx->ringCount + 1) * n], rgb[0], rgb[1], rgb[2]);
            }
        }
    }
    fx->prevBrightness = fx->brightness;
}

/* old-style definition: the callers pass ints, cull and speed are narrowed here */
void SUG_drawSphereF(fx, cull, count, speed, otz)
    SphereEffect *fx;
    u8 cull;
    s32 count;
    s16 speed;
    s32 otz;
{
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    s32 next;
    u8 dimR;
    u8 dimG;
    u8 dimB;
    u8 r;
    u8 g;
    u8 bl;
    s32 i;
    s32 scale;
    s32 sine;
    s32 cosine;
    s32 angle;
    s32 prev;
    POLY_F4 *quad;
    DrTPage *tpage;

    if (fx->pulse == 0) {
        r = fx->rgb[0];
        g = fx->rgb[1];
        bl = fx->rgb[2];
        dimR = r;
        dimG = g;
        dimB = bl;
    } else {
        r = 0;
        if (fx->pulseMode == 0) {
            g = 0;
            bl = 0;
            dimR = 0;
            dimG = 0;
            dimB = 0;
        } else {
            angle = speed * fx->ringCount * fx->pulse;
            r = fx->rgb[0];
            g = fx->rgb[1];
            bl = fx->rgb[2];
            scale = abs(rcos(angle & 0x7FF));
            dimR = ((fx->rgb[0] * scale) >> 12) & 0xFF;
            dimG = ((fx->rgb[1] * scale) >> 12) & 0xFF;
            dimB = ((fx->rgb[2] * scale) >> 12) & 0xFF;
        }
    }
    next = 1;
    for (i = 0; i < count; i++, next++) {
        fx->tris[FRAME_BUFFER_INDEX][i].r0 = r;
        fx->tris[FRAME_BUFFER_INDEX][i].g0 = g;
        fx->tris[FRAME_BUFFER_INDEX][i].b0 = bl;
        a.vx = fx->verts[0].vx;
        a.vy = fx->verts[0].vy;
        a.vz = fx->verts[0].vz;
        b.vx = fx->verts[i + 1].vx;
        b.vy = fx->verts[i + 1].vy;
        b.vz = fx->verts[i + 1].vz;
        c.vx = fx->verts[next % count + 1].vx;
        c.vy = fx->verts[next % count + 1].vy;
        c.vz = fx->verts[next % count + 1].vz;
        transformAndAddPolyF3((s32)&fx->tris[FRAME_BUFFER_INDEX][i], (s32)&fx->tpages[FRAME_BUFFER_INDEX][i],
                              (s32)&c, (s32)&b, (s32)&a, fx->semiTrans, cull, otz);
        if (fx->openBottom == 0) {
            fx->tris[FRAME_BUFFER_INDEX][count + i].r0 = dimR;
            fx->tris[FRAME_BUFFER_INDEX][count + i].g0 = dimG;
            fx->tris[FRAME_BUFFER_INDEX][count + i].b0 = dimB;
            /* unused: tpage is set again after the loop */
            tpage = fx->tpages[FRAME_BUFFER_INDEX];
            a.vx = fx->verts[fx->vertCount - 1].vx;
            a.vy = fx->verts[fx->vertCount - 1].vy;
            a.vz = fx->verts[fx->vertCount - 1].vz;
            b.vx = fx->verts[fx->vertCount - count - 1 + i].vx;
            b.vy = fx->verts[fx->vertCount - count - 1 + i].vy;
            b.vz = fx->verts[fx->vertCount - count - 1 + i].vz;
            c.vx = fx->verts[fx->vertCount - count - 1 + next % count].vx;
            c.vy = fx->verts[fx->vertCount - count - 1 + next % count].vy;
            c.vz = fx->verts[fx->vertCount - count - 1 + next % count].vz;
            transformAndAddPolyF3((s32)&fx->tris[FRAME_BUFFER_INDEX][count + i],
                                  (s32)&fx->tpages[FRAME_BUFFER_INDEX][count + i], (s32)&a, (s32)&b, (s32)&c,
                                  fx->semiTrans, cull, otz);
        }
    }
    quad = fx->quads[FRAME_BUFFER_INDEX];
    tpage = fx->tpages[FRAME_BUFFER_INDEX] + count * 2;
    next = 1;
    for (i = 0; i < fx->ringVertCount - count * 2; i++, next++, quad++, tpage++) {
        angle = speed * (i / count + 1) * fx->pulse;
        angle &= 0x7FF;
        if (fx->pulse == 0) {
            r = fx->rgb[0];
            g = fx->rgb[1];
            bl = fx->rgb[2];
        } else if (fx->pulseMode == 0) {
            sine = rsin(angle);
            r = ((fx->rgb[0] * sine) >> 12) & 0xFF;
            g = ((fx->rgb[1] * sine) >> 12) & 0xFF;
            bl = ((fx->rgb[2] * sine) >> 12) & 0xFF;
        } else {
            scale = abs(rcos(angle & 0x7FF));
            r = ((fx->rgb[0] * scale) >> 12) & 0xFF;
            g = ((fx->rgb[1] * scale) >> 12) & 0xFF;
            bl = ((fx->rgb[2] * scale) >> 12) & 0xFF;
        }
        fx->quads[FRAME_BUFFER_INDEX][i].r0 = r;
        fx->quads[FRAME_BUFFER_INDEX][i].g0 = g;
        fx->quads[FRAME_BUFFER_INDEX][i].b0 = bl;
        if (next % count == 0) {
            prev = next - count;
        } else {
            prev = next;
        }
        a.vx = fx->verts[i + 1].vx;
        a.vy = fx->verts[i + 1].vy;
        a.vz = fx->verts[i + 1].vz;
        b.vx = fx->verts[prev + 1].vx;
        b.vy = fx->verts[prev + 1].vy;
        b.vz = fx->verts[prev + 1].vz;
        c.vx = fx->verts[count + (i + 1)].vx;
        c.vy = fx->verts[count + (i + 1)].vy;
        c.vz = fx->verts[count + (i + 1)].vz;
        d.vx = fx->verts[count + (prev + 1)].vx;
        d.vy = fx->verts[count + (prev + 1)].vy;
        d.vz = fx->verts[count + (prev + 1)].vz;
        transformAndAddPolyF4((s32)quad, (s32)tpage, (s32)&a, (s32)&b, (s32)&c, (s32)&d, fx->semiTrans, cull, otz);
    }
}

void SUG_drawSphereG(fx, cull, count, speed, otz)
    SphereEffect *fx;
    u8 cull;
    s32 count;
    s16 speed;
    s32 otz;
{
    Rgb c0;
    Rgb c1;
    Rgb c2;
    Rgb c3;
    Rgb base;
    s32 inner;
    s32 i;
    s32 j;
    s32 next;
    s32 prev;
    s32 s;
    s32 t;
    s32 u;
    s32 angle0;
    s32 angle1;
    s32 angle2;
    POLY_G4 *quad;
    DrTPage *tpage;

    inner = 0;
    if (fx->brightness != fx->prevBrightness) {
        base.r = fx->rgb[0] * fx->brightness / 256;
        base.g = fx->rgb[1] * fx->brightness / 256;
        base.b = fx->rgb[2] * fx->brightness / 256;
        if (fx->pulse == 0) {
            c0.r = base.r;
            c0.g = base.g;
            c0.b = base.b;
            c1 = c0;
            c2 = c1;
            c3 = c2;
        } else {
            angle0 = speed * fx->pulse;
            angle1 = speed * fx->ringCount * fx->pulse;
            angle2 = speed * (fx->ringCount + 1) * fx->pulse;
            if (fx->pulseMode == 0) {
                c0.r = 0;
                c0.g = 0;
                c0.b = 0;
                s = rsin(angle0 & 0x7FF);
                c1.r = base.r * s >> 12;
                c1.g = base.g * s >> 12;
                c1.b = base.b * s >> 12;
                s = rsin(angle1 & 0x7FF);
                c2.r = base.r * s >> 12;
                c2.g = base.g * s >> 12;
                c2.b = base.b * s >> 12;
                s = rsin(angle2 & 0x7FF);
                c3.r = base.r * s >> 12;
                c3.g = base.g * s >> 12;
                c3.b = base.b * s >> 12;
            } else {
                c0.r = base.r;
                c0.g = base.g;
                c0.b = base.b;
                s = abs(rcos(angle0 & 0x7FF));
                c1.r = base.r * s >> 12;
                c1.g = base.g * s >> 12;
                c1.b = base.b * s >> 12;
                s = abs(rcos(angle1 & 0x7FF));
                c2.r = base.r * s >> 12;
                c2.g = base.g * s >> 12;
                c2.b = base.b * s >> 12;
                s = abs(rcos(angle2 & 0x7FF));
                c3.r = base.r * s >> 12;
                c3.g = base.g * s >> 12;
                c3.b = base.b * s >> 12;
            }
        }
    }
    next = 1;
    for (i = 0; i < count; i++, next++) {
        if (fx->brightness != fx->prevBrightness) {
            for (j = 0; j < 2; j++) {
                setPrimRgb0(&fx->gtris[j][i], c1.r, c1.g, c1.b);
                setPrimRgb1(&fx->gtris[j][i], c1.r, c1.g, c1.b);
                setPrimRgb2(&fx->gtris[j][i], c0.r, c0.g, c0.b);
            }
        }
        transformAndAddPolyG3((s32)&fx->gtris[FRAME_BUFFER_INDEX][i], (s32)&fx->tpages[FRAME_BUFFER_INDEX][i],
                              (s32)&fx->verts[next % count + 1], (s32)&fx->verts[i + 1], (s32)fx->verts, fx->semiTrans,
                              cull, otz);
        if (fx->openBottom == 0) {
            if (fx->brightness != fx->prevBrightness) {
                for (j = 0; j < 2; j++) {
                    setPrimRgb0(&fx->gtris[j][count + i], c3.r, c3.g, c3.b);
                    setPrimRgb1(&fx->gtris[j][count + i], c2.r, c2.g, c2.b);
                    setPrimRgb2(&fx->gtris[j][count + i], c2.r, c2.g, c2.b);
                }
            }
            transformAndAddPolyG3((s32)&fx->gtris[FRAME_BUFFER_INDEX][count + i],
                                  (s32)&fx->tpages[FRAME_BUFFER_INDEX][count + i],
                                  (s32)&fx->verts[fx->vertCount - 1],
                                  (s32)&fx->verts[fx->vertCount - count - 1 + i],
                                  (s32)&fx->verts[fx->vertCount - count - 1 + next % count], fx->semiTrans, cull,
                                  otz);
            inner = count * 2;
        } else {
            inner = count;
        }
    }
    quad = fx->gquads[FRAME_BUFFER_INDEX];
    tpage = &fx->tpages[FRAME_BUFFER_INDEX][inner];
    next = 1;
    for (i = 0; i < fx->ringVertCount - inner; i++, next++, quad++, tpage++) {
        if (fx->brightness != fx->prevBrightness) {
            angle0 = speed * (i / count + 1) * fx->pulse;
            angle1 = speed * (i / count + 2) * fx->pulse;
            angle0 &= 0x7FF;
            angle1 &= 0x7FF;
            if (fx->pulse == 0) {
                c0.r = base.r;
                c0.g = base.g;
                c0.b = base.b;
                c1 = c0;
            } else {
                if (fx->pulseMode == 0) {
                    s = rsin(angle0);
                    t = rsin(angle1);
                    c0.r = base.r * s >> 12;
                    c0.g = base.g * s >> 12;
                    c0.b = base.b * s >> 12;
                    c1.r = base.r * t >> 12;
                    c1.g = base.g * t >> 12;
                    c1.b = base.b * t >> 12;
                } else {
                    s = abs(rcos(angle0));
                    u = abs(rcos(angle1));
                    c0.r = base.r * s >> 12;
                    c0.g = base.g * s >> 12;
                    c0.b = base.b * s >> 12;
                    c1.r = base.r * u >> 12;
                    c1.g = base.g * u >> 12;
                    c1.b = base.b * u >> 12;
                }
            }
            for (j = 0; j < 2; j++) {
                setPrimRgb0(&fx->gquads[j][i], c0.r, c0.g, c0.b);
                setPrimRgb1(&fx->gquads[j][i], c0.r, c0.g, c0.b);
                setPrimRgb2(&fx->gquads[j][i], c1.r, c1.g, c1.b);
                setPrimRgb3(&fx->gquads[j][i], c1.r, c1.g, c1.b);
            }
        }
        if (next % count == 0) {
            prev = next - count;
        } else {
            prev = next;
        }
        transformAndAddPolyG4((s32)quad, (s32)tpage, (s32)&fx->verts[i + 1], (s32)&fx->verts[prev + 1],
                              (s32)&fx->verts[i + 1 + count], (s32)&fx->verts[prev + 1 + count], fx->semiTrans,
                              cull, otz);
    }
    fx->prevBrightness = fx->brightness;
}

/* old-style definition: the callers pass ints, cull and speed are narrowed here */
void SUG_drawSphereGT(fx, cull, count, speed, otz)
    SphereEffect *fx;
    u8 cull;
    s32 count;
    s16 speed;
    s32 otz;
{
    Rgb c0;
    Rgb c1;
    Rgb c2;
    Rgb c3;
    Rgb base;
    s32 inner;
    s32 i;
    s32 j;
    s32 next;
    s32 prev;
    s32 s;
    s32 t;
    s32 u;
    s32 angle0;
    s32 angle1;
    s32 angle2;
    POLY_GT4 *quad;

    inner = 0;
    if (fx->texAnimActive >= 0) {
        SUG_updateSphereUvs(fx);
    }
    if (fx->brightness != fx->prevBrightness) {
        base.r = fx->rgb[0] * fx->brightness / 256;
        base.g = fx->rgb[1] * fx->brightness / 256;
        base.b = fx->rgb[2] * fx->brightness / 256;
        if (fx->pulse == 0) {
            c0.r = base.r;
            c0.g = base.g;
            c0.b = base.b;
            c1 = c0;
            c2 = c1;
            c3 = c2;
        } else {
            angle0 = speed * fx->pulse;
            angle1 = speed * fx->ringCount * fx->pulse;
            angle2 = speed * (fx->ringCount + 1) * fx->pulse;
            if (fx->pulseMode == 0) {
                c0.r = 0;
                c0.g = 0;
                c0.b = 0;
                s = rsin(angle0 & 0x7FF);
                c1.r = base.r * s >> 12;
                c1.g = base.g * s >> 12;
                c1.b = base.b * s >> 12;
                s = rsin(angle1 & 0x7FF);
                c2.r = base.r * s >> 12;
                c2.g = base.g * s >> 12;
                c2.b = base.b * s >> 12;
                s = rsin(angle2 & 0x7FF);
                c3.r = base.r * s >> 12;
                c3.g = base.g * s >> 12;
                c3.b = base.b * s >> 12;
            } else {
                c0.r = base.r;
                c0.g = base.g;
                c0.b = base.b;
                s = abs(rcos(angle0 & 0x7FF));
                c1.r = base.r * s >> 12;
                c1.g = base.g * s >> 12;
                c1.b = base.b * s >> 12;
                s = abs(rcos(angle1 & 0x7FF));
                c2.r = base.r * s >> 12;
                c2.g = base.g * s >> 12;
                c2.b = base.b * s >> 12;
                s = abs(rcos(angle2 & 0x7FF));
                c3.r = base.r * s >> 12;
                c3.g = base.g * s >> 12;
                c3.b = base.b * s >> 12;
            }
        }
    }
    next = 1;
    for (i = 0; i < count; i++, next++) {
        if (fx->brightness != fx->prevBrightness) {
            for (j = 0; j < 2; j++) {
                setPrimRgb0(&fx->ttris[j][i], c1.r, c1.g, c1.b);
                setPrimRgb1(&fx->ttris[j][i], c1.r, c1.g, c1.b);
                setPrimRgb2(&fx->ttris[j][i], c0.r, c0.g, c0.b);
            }
        }
        transformAndAddPolyGT3((s32)&fx->ttris[FRAME_BUFFER_INDEX][i], (s32)&fx->verts[next % count + 1],
                               (s32)&fx->verts[i + 1], (s32)fx->verts, cull, otz);
        if (fx->openBottom == 0) {
            if (fx->brightness != fx->prevBrightness) {
                for (j = 0; j < 2; j++) {
                    setPrimRgb0(&fx->ttris[j][count + i], c3.r, c3.g, c3.b);
                    setPrimRgb1(&fx->ttris[j][count + i], c2.r, c2.g, c2.b);
                    setPrimRgb2(&fx->ttris[j][count + i], c2.r, c2.g, c2.b);
                }
            }
            transformAndAddPolyGT3((s32)&fx->ttris[FRAME_BUFFER_INDEX][count + i],
                                  (s32)&fx->verts[fx->vertCount - 1],
                                  (s32)&fx->verts[fx->vertCount - count - 1 + i],
                                  (s32)&fx->verts[fx->vertCount - count - 1 + next % count], cull, otz);
            inner = count * 2;
        } else {
            inner = count;
        }
    }
    quad = fx->tquads[FRAME_BUFFER_INDEX];
    next = 1;
    for (i = 0; i < fx->ringVertCount - inner; i++, next++, quad++) {
        if (fx->brightness != fx->prevBrightness) {
            angle0 = speed * (i / count + 1) * fx->pulse;
            angle1 = speed * (i / count + 2) * fx->pulse;
            angle0 &= 0x7FF;
            angle1 &= 0x7FF;
            if (fx->pulse == 0) {
                c0.r = base.r;
                c0.g = base.g;
                c0.b = base.b;
                c1 = c0;
            } else {
                if (fx->pulseMode == 0) {
                    s = rsin(angle0);
                    t = rsin(angle1);
                    c0.r = base.r * s >> 12;
                    c0.g = base.g * s >> 12;
                    c0.b = base.b * s >> 12;
                    c1.r = base.r * t >> 12;
                    c1.g = base.g * t >> 12;
                    c1.b = base.b * t >> 12;
                } else {
                    s = rcos(angle0);
                    if (s < 0) {
                        s = -s;
                    }
                    u = rcos(angle1);
                    if (u < 0) {
                        u = -u;
                    }
                    c0.r = base.r * s >> 12;
                    c0.g = base.g * s >> 12;
                    c0.b = base.b * s >> 12;
                    c1.r = base.r * u >> 12;
                    c1.g = base.g * u >> 12;
                    c1.b = base.b * u >> 12;
                }
            }
            for (j = 0; j < 2; j++) {
                setPrimRgb0(&fx->tquads[j][i], c0.r, c0.g, c0.b);
                setPrimRgb1(&fx->tquads[j][i], c0.r, c0.g, c0.b);
                setPrimRgb2(&fx->tquads[j][i], c1.r, c1.g, c1.b);
                setPrimRgb3(&fx->tquads[j][i], c1.r, c1.g, c1.b);
            }
        }
        if (next % count == 0) {
            prev = next - count;
        } else {
            prev = next;
        }
        transformAndAddPolyGT4((s32)quad, (s32)&fx->verts[i + 1], (s32)&fx->verts[prev + 1],
                               (s32)&fx->verts[i + 1 + count], (s32)&fx->verts[prev + 1 + count], cull, otz);
    }
    fx->prevBrightness = fx->brightness;
}

void SUG_updateSphereUvs(SphereEffect *obj) {
    Rect16 uv;
    POLY_GT4 *gt4;
    s32 n;
    s32 i;
    s32 skip;

    n = obj->segments;
    SUG_tickTexAnim(&obj->texAnim);
    uv = obj->uv;
    for (i = 0; i < n; i++) {
        obj->ttris[FRAME_BUFFER_INDEX][i].u0 = uv.x + uv.w;
        obj->ttris[FRAME_BUFFER_INDEX][i].v0 = uv.y + uv.h;
        obj->ttris[FRAME_BUFFER_INDEX][i].u1 = uv.x;
        obj->ttris[FRAME_BUFFER_INDEX][i].v1 = uv.y + uv.h;
        obj->ttris[FRAME_BUFFER_INDEX][i].u2 = uv.w / 2 + uv.x;
        obj->ttris[FRAME_BUFFER_INDEX][i].v2 = uv.y;
        obj->ttris[FRAME_BUFFER_INDEX][i].tpage = obj->tpage;
        obj->ttris[FRAME_BUFFER_INDEX][i].clut = obj->clut;
        if (obj->openBottom == 0) {
            obj->ttris[FRAME_BUFFER_INDEX][n + i].u0 = uv.w / 2 + uv.x;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].v0 = uv.y + uv.h;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].u1 = uv.x;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].v1 = uv.y;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].u2 = uv.x + uv.w;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].v2 = uv.y;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].tpage = obj->tpage;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].clut = obj->clut;
        }
    }
    if (obj->openBottom != 0) {
        skip = n;
    } else {
        skip = n * 2;
    }
    gt4 = obj->tquads[FRAME_BUFFER_INDEX];
    for (i = 0; i < obj->ringVertCount - skip; i++, gt4++) {
        gt4->tpage = obj->tpage;
        gt4->clut = obj->clut;
        setPrimQuadUvRect((u8 *)gt4, uv.x, uv.y, uv.w, uv.h);
    }
}

void SUG_freeSphereEffect(SphereEffect *obj) {
    s32 i;

    freeHeapBlock(obj->verts);
    if (obj->texAnimActive >= 0) {
        SUG_freeTexAnim(&obj->texAnim);
    }
    for (i = 0; i < 2; i++) {
        if (obj->tris[i] != NULL) {
            freeHeapBlock(obj->tris[i]);
        }
        if (obj->quads[i] != NULL) {
            freeHeapBlock(obj->quads[i]);
        }
        if (obj->gtris[i] != NULL) {
            freeHeapBlock(obj->gtris[i]);
        }
        if (obj->gquads[i] != NULL) {
            freeHeapBlock(obj->gquads[i]);
        }
        if (obj->ttris[i] != NULL) {
            freeHeapBlock(obj->ttris[i]);
        }
        if (obj->tquads[i] != NULL) {
            freeHeapBlock(obj->tquads[i]);
        }
        if (obj->lines[i] != NULL) {
            freeHeapBlock(obj->lines[i]);
        }
        if (obj->tpages[i] != NULL) {
            freeHeapBlock(obj->tpages[i]);
        }
    }
    freeHeapBlock(obj);
}
