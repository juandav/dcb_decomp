#include "common.h"
#include "game.h"
#include "dcb/sug_trail.h"
#include "dcb/heap.h"
#include "dcb/effect_object.h"
#include "dcb/transform.h"
#include "dcb/scroll_bg.h"
#include "dcb/prim.h"
#include "dcb/prim_util.h"
#include "dcb/sug_history.h"
#include "dcb/sug_gradient.h"
#include "dcb/sug_tex_anim.h"

void SUG_setTrailColors(TrailEffect *obj, u8 kind, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3);
void SUG_initTrailPrims(TrailEffect *obj, u8 semiTrans, u8 blend, u8 kind, u8 texAnimId, Rect16 *uv, s32 tpage, s32 clut);
void SUG_shadeTrailPrims(TrailEffect *obj);

/* jp's trails never start a texture animation, so they take no pak */
#if VERSION_JP
TrailEffect *SUG_createTrailEffect(s16 brightness, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3, EffectTemplate *template, s16 x0, s16 x1,
                           s32 count, s16 rows, u8 followMode, u8 colorMode, u8 semiTrans, u8 blend, u8 primKind, u8 texAnimId, Rect16 *uv, s32 tpage, s32 clut,
                           s32 otz) {
#elif VERSION_US || VERSION_EU
TrailEffect *SUG_createTrailEffect(s16 brightness, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3, EffectTemplate *template, s16 x0, s16 x1,
                           s32 count, s16 rows, u8 followMode, u8 colorMode, u8 semiTrans, u8 blend, u8 primKind, s32 texAnimId, Rect16 *uv, s32 tpage, s32 clut,
                           s32 otz, s32 pak) {
#endif
    TrailEffect *obj;
    s32 i;

    obj = allocTaskHeapBlock(sizeof(TrailEffect));
    for (i = 0; i < 2; i++) {
        obj->histories[i] = SUG_createPosHistory(count + 1, rows);
    }
#if VERSION_JP
    obj->texAnimId = -1;
#elif VERSION_US || VERSION_EU
    obj->clut = clut;
    obj->tpage = tpage;
#endif
    obj->semiTrans = semiTrans;
    obj->primKind = primKind;
    obj->colorMode = colorMode;
    obj->count = count;
    obj->blend = blend;
    obj->level = brightness;
    obj->primeCount = 0;
    obj->followMode = followMode;
    initTransform(obj->xform, 0, 0, 0, 0, 0, 0, 0);
    *(EffectTemplate *)obj = *template;
    initEffectObject(obj);
    obj->otz = otz;
    obj->prevPos[0] = obj->pos;
    obj->prevPos[1] = obj->pos;
    obj->prevRot[0] = obj->rot;
    obj->prevRot[1] = obj->rot;
    initTransform(obj->edges[0], (s32)obj, x0, 0, 0, 0, 0, 0);
    initTransform(obj->edges[1], (s32)obj, x1, 0, 0, 0, 0, 0);
    obj->colors = allocTaskHeapBlock(colorMode ? count * 16 : 16);
    SUG_initTrailPrims(obj, semiTrans, blend, primKind, texAnimId, uv, tpage, clut);
    SUG_setTrailColors(obj, colorMode, c0, c1, c2, c3);
#if VERSION_US || VERSION_EU
    obj->texAnimId = texAnimId;
    if (texAnimId < 100 || SUG_startTexAnim(texAnimId, 4, (RingEffect *)obj, &obj->texAnim, pak) == 0) {
        obj->texAnimId = -1;
    }
#endif
    return obj;
}

void SUG_tickTrailEffect(TrailEffect *obj) {
    Short4 pos;
    Short4 b;
    Short4 c;
    Short4 d;
    Short4 delta;
    s32 moved;
    s32 i;
    s32 j;
    s32 k;
    DrTPage *tp;
    LineG2 *line;
    POLY_G4 *g4;
    POLY_FT4 *ft4;
    POLY_GT4 *gt4;

    moved = 0;
    if (obj->suspended != 0) {
        TICK_START_DELAY(obj);
        return;
    }
    PushMatrix();
    tickEffectMotion((s32)obj, 0);
    PopMatrix();
    switch (obj->fadeMode) {
    case 1:
        obj->level = obj->sx / 16;
        if (obj->sx > 0x1000) {
            obj->level = 0x100 - (obj->sx - 0x1000) / 16;
        }
        break;
    case 2:
        obj->level += obj->speed;
        break;
    case 3:
        if (obj->fadeState == 2) {
            break;
        }
        if (obj->fadeState == 0) {
            goto grow;
        }
        obj->level -= obj->speed;
        if (obj->level < 0) {
            /* the match depends on the extra block, for the register allocation */
            do {
                obj->level = 0;
            } while (0);
            obj->fadeState = 2;
        }
        break;
    case 4:
        if (obj->fadeState == 0) {
        grow:
            obj->level += obj->speed;
            if (obj->level > 0x100) {
                obj->level = 0x100;
                obj->fadeState = 1;
            }
        } else {
            obj->level -= obj->speed;
            if (obj->level < 0) {
                obj->level = 0;
                obj->fadeState = 0;
            }
        }
        break;
    }
    if (obj->level < 0) {
        obj->level = 0;
    }
    if (obj->level > 0x100) {
        obj->level = 0x100;
    }
    if (obj->mode == 10) {
        obj->level = obj->brightness;
    } else {
        obj->brightness = obj->level;
    }
    if (obj->level == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        PushMatrix();
        updateTransformMatrix(obj->edges[i], 0);
        PopMatrix();
        getTransformWorldPos(obj->edges[i], &pos);
        if (obj->followMode == 0) {
            moved = 1;
        } else if (obj->followMode == 1) {
            if (obj->lastPos[i].v[0] != pos.v[0] || obj->lastPos[i].v[1] != pos.v[1] || obj->lastPos[i].v[2] != pos.v[2]) {
                moved = 1;
            }
        } else {
            moved = 1;
            /* the match depends on the extra block, for the delay slots */
            do {
                if (obj->prevPos[i].vx != obj->pos.vx || obj->prevPos[i].vy != obj->pos.vy || obj->prevPos[i].vz != obj->pos.vz ||
                    obj->prevRot[i].vx != obj->rot.vx || obj->prevRot[i].vy != obj->rot.vy || obj->prevRot[i].vz != obj->rot.vz) {
                    goto skip;
                }
            } while (0);
            moved = 2;
            delta.v[0] = obj->lastPos[i].v[0] - pos.v[0];
            delta.v[1] = obj->lastPos[i].v[1] - pos.v[1];
            delta.v[2] = obj->lastPos[i].v[2] - pos.v[2];
            for (j = 0; j < obj->histories[i]->count; j++) {
                obj->histories[i]->shorts[j].v[0] -= delta.v[0];
                obj->histories[i]->shorts[j].v[1] -= delta.v[1];
                obj->histories[i]->shorts[j].v[2] -= delta.v[2];
            }
        skip:
            obj->prevPos[i] = obj->pos;
            obj->prevRot[i] = obj->rot;
        }
        if (obj->primeCount != 2) {
            for (k = 0; k < obj->histories[i]->count; k++) {
                obj->histories[i]->shorts[k] = pos;
            }
            obj->lastPos[i] = pos;
            obj->primeCount++;
        }
        if (moved == 1) {
            SUG_pushPosHistory(obj->histories[i], NULL, &pos);
        }
        obj->lastPos[i] = pos;
    }
    PushMatrix();
    updateTransformMatrix(obj->xform, 0);
    SUG_shadeTrailPrims(obj);
    switch (obj->primKind) {
    case 1:
        tp = obj->tpages[FRAME_BUFFER_INDEX];
        line = obj->lines[FRAME_BUFFER_INDEX];
        for (i = 0; i < obj->count; i++, line++, tp++) {
            SUG_getPosHistory(obj->histories[0], i, NULL, &pos);
            SUG_getPosHistory(obj->histories[0], i + 1, NULL, &d);
            transformAndAddLineG2((s32)line, (s32)tp, (s32)&pos, (s32)&d, obj->semiTrans, obj->otz);
        }
        break;
    case 9:
        tp = obj->tpages[FRAME_BUFFER_INDEX];
        g4 = obj->g4s[FRAME_BUFFER_INDEX];
        for (i = 0; i < obj->count; i++, g4++, tp++) {
            SUG_getPosHistory(obj->histories[0], i, NULL, &pos);
            SUG_getPosHistory(obj->histories[0], i + 1, NULL, &c);
            SUG_getPosHistory(obj->histories[1], i, NULL, &b);
            SUG_getPosHistory(obj->histories[1], i + 1, NULL, &d);
            transformAndAddPolyG4((s32)g4, (s32)tp, (s32)&pos, (s32)&b, (s32)&c, (s32)&d, obj->semiTrans, 0, obj->otz);
        }
        break;
    /* jp's trails are never textured */
#if VERSION_US || VERSION_EU
    case 12:
        ft4 = obj->ft4s[FRAME_BUFFER_INDEX];
        if (obj->texAnimId != -1) {
            SUG_tickTexAnim(&obj->texAnim);
        }
        for (i = 0; i < obj->count; i++, ft4++) {
            SUG_getPosHistory(obj->histories[0], i, NULL, &pos);
            SUG_getPosHistory(obj->histories[0], i + 1, NULL, &c);
            SUG_getPosHistory(obj->histories[1], i, NULL, &b);
            SUG_getPosHistory(obj->histories[1], i + 1, NULL, &d);
            setPrimQuadUvRect((u8 *)ft4, obj->uv.x, obj->uv.y, obj->uv.w, obj->uv.h);
            transformAndAddPolyFT4((s32)ft4, (s32)&pos, (s32)&b, (s32)&c, (s32)&d, 0, obj->otz);
        }
        break;
    case 13:
        gt4 = obj->gt4s[FRAME_BUFFER_INDEX];
        if (obj->texAnimId != -1) {
            SUG_tickTexAnim(&obj->texAnim);
        }
        for (i = 0; i < obj->count; i++, gt4++) {
            SUG_getPosHistory(obj->histories[0], i, NULL, &pos);
            SUG_getPosHistory(obj->histories[0], i + 1, NULL, &c);
            SUG_getPosHistory(obj->histories[1], i, NULL, &b);
            SUG_getPosHistory(obj->histories[1], i + 1, NULL, &d);
            setPrimQuadUvRect((u8 *)gt4, obj->uv.x, obj->uv.y, obj->uv.w, obj->uv.h);
            transformAndAddPolyGT4((s32)gt4, (s32)&pos, (s32)&b, (s32)&c, (s32)&d, 0, obj->otz);
        }
        break;
#endif
    }
    PopMatrix();
}

void SUG_freeTrailEffect(TrailEffect *obj) {
    s32 i;

    for (i = 0; i < 2; i++) {
        freeHeapBlock(obj->lines[i]);
        freeHeapBlock(obj->g4s[i]);
        freeHeapBlock(obj->ft4s[i]);
        freeHeapBlock(obj->gt4s[i]);
        freeHeapBlock(obj->tpages[i]);
        SUG_freePosHistory((void **)obj->histories[i]);
    }
#if VERSION_US || VERSION_EU
    if (obj->texAnimId >= 0) {
        SUG_freeTexAnim(&obj->texAnim);
    }
#endif
    freeHeapBlock(obj->colors);
    freeHeapBlock(obj);
}

void SUG_setTrailColors(TrailEffect *obj, u8 kind, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3) {
    obj->colorMode = kind;
    SUG_fillGradientColors(obj->colors, kind, 1, obj->count, c0, c1, c2, c3, 0);
    obj->prevLevel = -1;
}

void SUG_initTrailPrims(TrailEffect *obj, u8 semiTrans, u8 blend, u8 kind, u8 texAnimId, Rect16 *uv, s32 tpage, s32 clut) {
    s32 i;
    LineG2 *line0;
    LineG2 *line1;
    POLY_G4 *g4a;
    POLY_G4 *g4b;
    POLY_FT4 *ft4a;
    POLY_FT4 *ft4b;
    POLY_GT4 *gt4a;
    POLY_GT4 *gt4b;
    DrTPage *tp0;
    DrTPage *tp1;

    for (i = 0; i < 2; i++) {
        obj->tpages[i] = NULL;
        obj->lines[i] = NULL;
        obj->g4s[i] = NULL;
        obj->ft4s[i] = NULL;
        obj->gt4s[i] = NULL;
    }
    switch (kind) {
    case 1:
        if (semiTrans) {
            for (i = 0; i < 2; i++) {
                obj->tpages[i] = allocTaskHeapBlock(obj->count * sizeof(DrTPage));
            }
        }
        for (i = 0; i < 2; i++) {
            obj->lines[i] = allocTaskHeapBlock(obj->count * sizeof(LineG2));
        }
        line0 = obj->lines[0];
        line1 = obj->lines[1];
        tp0 = obj->tpages[0];
        tp1 = obj->tpages[1];
        for (i = 0; i < obj->count; i++, line0++, line1++, tp0++, tp1++) {
            if (!semiTrans) {
                initLineG2Pair((s32 *)line0, (s32 *)line1, NULL, NULL, blend, NULL, NULL, 0, 1);
            } else {
                initLineG2Pair((s32 *)line0, (s32 *)line1, NULL, NULL, blend, tp0, tp1, 1, 1);
            }
        }
        break;
    case 9:
        if (semiTrans) {
            for (i = 0; i < 2; i++) {
                obj->tpages[i] = allocTaskHeapBlock(obj->count * sizeof(DrTPage));
            }
        }
        for (i = 0; i < 2; i++) {
            obj->g4s[i] = allocTaskHeapBlock(obj->count * sizeof(POLY_G4));
        }
        g4a = obj->g4s[0];
        g4b = obj->g4s[1];
        tp0 = obj->tpages[0];
        tp1 = obj->tpages[1];
        for (i = 0; i < obj->count; i++, g4a++, g4b++, tp0++, tp1++) {
            if (!semiTrans) {
                initPolyG4Pair(g4a, g4b, NULL, NULL, NULL, NULL, blend, NULL, NULL, NULL, 0, 1);
            } else {
                initPolyG4Pair(g4a, g4b, NULL, NULL, NULL, NULL, blend, tp0, tp1, NULL, 1, 1);
            }
        }
        break;
    /* jp's trails are only lines and gouraud quads, never textured */
#if VERSION_US || VERSION_EU
    case 12:
        for (i = 0; i < 2; i++) {
            obj->ft4s[i] = allocTaskHeapBlock(obj->count * sizeof(POLY_FT4));
        }
        ft4a = obj->ft4s[0];
        ft4b = obj->ft4s[1];
        for (i = 0; i < obj->count; i++, ft4a++, ft4b++) {
            if (!semiTrans) {
                initPolyFT4Pair(ft4a, ft4b, NULL, tpage, clut, NULL, NULL, 0, 1);
            } else {
                initPolyFT4Pair(ft4a, ft4b, NULL, tpage, clut, NULL, NULL, 1, 1);
            }
        }
        obj->uv = *uv;
        break;
    case 13:
        for (i = 0; i < 2; i++) {
            obj->gt4s[i] = allocTaskHeapBlock(obj->count * sizeof(POLY_GT4));
        }
        gt4a = obj->gt4s[0];
        gt4b = obj->gt4s[1];
        for (i = 0; i < obj->count; i++, gt4a++, gt4b++) {
            if (!semiTrans) {
                initPolyGT4Pair(gt4a, gt4b, NULL, NULL, NULL, NULL, tpage, clut, uv, NULL, 0, 1);
            } else {
                initPolyGT4Pair(gt4a, gt4b, NULL, NULL, NULL, NULL, tpage, clut, uv, NULL, 1, 1);
            }
        }
        obj->uv = *uv;
        break;
#endif
    }
}

/* jp's trails are never textured */
#if VERSION_US || VERSION_EU
void SUG_updateTrailUvs(TrailEffect *obj) {
    Rect16 uv;
    POLY_FT4 *ft4;
    POLY_GT4 *gt4;
    s32 i;

    SUG_tickTexAnim(&obj->texAnim);
    uv = obj->uv;
    if (obj->texAnimId != 3) {
        switch (obj->primKind) {
        case 12:
            ft4 = obj->ft4s[FRAME_BUFFER_INDEX];
            SUG_setGridUvsFT4(ft4, &uv, obj->count, 1, obj->count, 0, 0, 0);
            for (i = 0; i < obj->count; i++, ft4++) {
                ft4->tpage = obj->tpage;
                ft4->clut = obj->clut;
            }
            break;
        case 13:
            gt4 = obj->gt4s[FRAME_BUFFER_INDEX];
            SUG_setGridUvsGT4(gt4, &uv, obj->count, 1, obj->count, 0, 0, 0);
            for (i = 0; i < obj->count; i++, gt4++) {
                gt4->tpage = obj->tpage;
                gt4->clut = obj->clut;
            }
            break;
        }
    } else {
        switch (obj->primKind) {
        case 12:
            ft4 = obj->ft4s[FRAME_BUFFER_INDEX];
            for (i = 0; i < obj->count; i++, ft4++) {
                ft4->tpage = obj->tpage;
                ft4->clut = obj->clut;
                setPrimQuadUvRect((u8 *)ft4, uv.x, uv.y, uv.w - 1, uv.h - 1);
            }
            break;
        case 13:
            gt4 = obj->gt4s[FRAME_BUFFER_INDEX];
            for (i = 0; i < obj->count; i++, gt4++) {
                gt4->tpage = obj->tpage;
                gt4->clut = obj->clut;
                setPrimQuadUvRect((u8 *)gt4, uv.x, uv.y, uv.w - 1, uv.h - 1);
            }
            break;
        }
    }
}
#endif

void SUG_shadeTrailPrims(TrailEffect *obj) {
    u8 c0[3];
    u8 c1[3];
    u8 c2[3];
    u8 c3[3];
    u8 *col;
    LineG2 *line;
    LineG2 *line2;
    POLY_G4 *g4;
    POLY_G4 *g42;
    POLY_FT4 *ft4;
    POLY_FT4 *ft42;
    POLY_GT4 *gt4;
    POLY_GT4 *gt42;
    s32 i;

    if (obj->level != obj->prevLevel) {
        switch (obj->primKind) {
        case 1:
            line = obj->lines[FRAME_BUFFER_INDEX];
            line2 = obj->lines[FRAME_BUFFER_INDEX ^ 1];
            col = obj->colors;
            for (i = 0; i < obj->count; i++, line++, line2++) {
                c0[0] = col[0] * obj->level / 256;
                c0[1] = col[1] * obj->level / 256;
                c0[2] = col[2] * obj->level / 256;
                c1[0] = col[4] * obj->level / 256;
                c1[1] = col[5] * obj->level / 256;
                c1[2] = col[6] * obj->level / 256;
                setPrimRgb0(line, c0[0], c0[1], c0[2]);
                setPrimRgb1(line, c0[0], c0[1], c0[2]);
                setPrimRgb0(line2, c1[0], c1[1], c1[2]);
                setPrimRgb1(line2, c1[0], c1[1], c1[2]);
                if (obj->colorMode != 0) {
                    col += 16;
                }
            }
            break;
        case 9:
            g4 = obj->g4s[FRAME_BUFFER_INDEX];
            g42 = obj->g4s[FRAME_BUFFER_INDEX ^ 1];
            col = obj->colors;
            for (i = 0; i < obj->count; i++, g4++, g42++) {
                c0[0] = col[0] * obj->level / 256;
                c0[1] = col[1] * obj->level / 256;
                c0[2] = col[2] * obj->level / 256;
                c1[0] = col[4] * obj->level / 256;
                c1[1] = col[5] * obj->level / 256;
                c1[2] = col[6] * obj->level / 256;
                c2[0] = col[8] * obj->level / 256;
                c2[1] = col[9] * obj->level / 256;
                c2[2] = col[10] * obj->level / 256;
                c3[0] = col[12] * obj->level / 256;
                c3[1] = col[13] * obj->level / 256;
                c3[2] = col[14] * obj->level / 256;
                setPrimRgb0(g4, c0[0], c0[1], c0[2]);
                setPrimRgb1(g4, c1[0], c1[1], c1[2]);
                setPrimRgb2(g4, c2[0], c2[1], c2[2]);
                setPrimRgb3(g4, c3[0], c3[1], c3[2]);
                setPrimRgb0(g42, c0[0], c0[1], c0[2]);
                setPrimRgb1(g42, c1[0], c1[1], c1[2]);
                setPrimRgb2(g42, c2[0], c2[1], c2[2]);
                setPrimRgb3(g42, c3[0], c3[1], c3[2]);
                if (obj->colorMode != 0) {
                    col += 16;
                }
            }
            break;
        /* jp's trails are never textured */
#if VERSION_US || VERSION_EU
        case 12:
            ft4 = obj->ft4s[FRAME_BUFFER_INDEX];
            ft42 = obj->ft4s[FRAME_BUFFER_INDEX ^ 1];
            col = obj->colors;
            for (i = 0; i < obj->count; i++, ft4++, ft42++) {
                c0[0] = col[0] * obj->level / 256;
                c0[1] = col[1] * obj->level / 256;
                c0[2] = col[2] * obj->level / 256;
                setPrimRgb0(ft4, c0[0], c0[1], c0[2]);
                setPrimRgb0(ft42, c0[0], c0[1], c0[2]);
                if (obj->colorMode != 0) {
                    col += 16;
                }
            }
            break;
        case 13:
            gt4 = obj->gt4s[FRAME_BUFFER_INDEX];
            gt42 = obj->gt4s[FRAME_BUFFER_INDEX ^ 1];
            col = obj->colors;
            for (i = 0; i < obj->count; i++, gt4++, gt42++) {
                c0[0] = col[0] * obj->level / 256;
                c0[1] = col[1] * obj->level / 256;
                c0[2] = col[2] * obj->level / 256;
                c1[0] = col[4] * obj->level / 256;
                c1[1] = col[5] * obj->level / 256;
                c1[2] = col[6] * obj->level / 256;
                c2[0] = col[8] * obj->level / 256;
                c2[1] = col[9] * obj->level / 256;
                c2[2] = col[10] * obj->level / 256;
                c3[0] = col[12] * obj->level / 256;
                c3[1] = col[13] * obj->level / 256;
                c3[2] = col[14] * obj->level / 256;
                setPrimRgb0(gt4, c0[0], c0[1], c0[2]);
                setPrimRgb1(gt4, c1[0], c1[1], c1[2]);
                setPrimRgb2(gt4, c2[0], c2[1], c2[2]);
                setPrimRgb3(gt4, c3[0], c3[1], c3[2]);
                setPrimRgb0(gt42, c0[0], c0[1], c0[2]);
                setPrimRgb1(gt42, c1[0], c1[1], c1[2]);
                setPrimRgb2(gt42, c2[0], c2[1], c2[2]);
                setPrimRgb3(gt42, c3[0], c3[1], c3[2]);
                if (obj->colorMode != 0) {
                    col += 16;
                }
            }
            break;
#endif
        }
        obj->prevLevel = obj->level;
    }
}
