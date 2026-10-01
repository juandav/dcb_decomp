#include "dcb/effect_prims.h"
#include "dcb/effect_object.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/scroll_bg.h"
#include "dcb/archive.h"
#include "dcb/decompress.h"
#include "dcb/sort.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/menu.h"
#include "dcb/dialog.h"
#include "dcb/prim3d.h"
#include "dcb/prim_util.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/transform.h"
#include "dcb/overlay_calls.h"

/* jp links the rings and the streak particles into SUGSEG as two units: its
   wrappers src/sugseg/model/effect_prims.c and streak_particles.c each build
   one part of this file. us builds both here. */
#if VERSION_JP
#define BUILD_RINGS JP_BUILD_RINGS
#define BUILD_STREAKS JP_BUILD_STREAKS
#elif VERSION_US || VERSION_EU
#define BUILD_RINGS 1
#define BUILD_STREAKS 1
#endif

#if BUILD_RINGS
/* per segment, the vertices at its two edge angles on the inner ring, the
   middle ring (midPercent of the way out, in radius and z) and the outer ring */
void buildRingEffectMesh(RingEffect *ring) {
    SVECTOR *vertex;
    s32 i;
    s16 x;
    s16 y;

    vertex = ring->vertices;
    for (i = 0; i < ring->n; i++) {
        x = rsin((i << 12) / ring->n) * ring->innerRadius / 4096;
        y = rcos((i << 12) / ring->n) * ring->innerRadius / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->innerZ;
        vertex++;
        x = rsin(((i + 1) << 12) / ring->n) * ring->innerRadius / 4096;
        y = rcos(((i + 1) << 12) / ring->n) * ring->innerRadius / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->innerZ;
        vertex++;
        x = rsin((i << 12) / ring->n) * (ring->innerRadius + (ring->outerRadius - ring->innerRadius) * ring->midPercent / 100) / 4096;
        y = rcos((i << 12) / ring->n) * (ring->innerRadius + (ring->outerRadius - ring->innerRadius) * ring->midPercent / 100) / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->innerZ + (ring->outerZ - ring->innerZ) * ring->midPercent / 100;
        vertex++;
        x = rsin(((i + 1) << 12) / ring->n) * (ring->innerRadius + (ring->outerRadius - ring->innerRadius) * ring->midPercent / 100) / 4096;
        y = rcos(((i + 1) << 12) / ring->n) * (ring->innerRadius + (ring->outerRadius - ring->innerRadius) * ring->midPercent / 100) / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->innerZ + (ring->outerZ - ring->innerZ) * ring->midPercent / 100;
        vertex++;
        x = rsin((i << 12) / ring->n) * ring->outerRadius / 4096;
        y = rcos((i << 12) / ring->n) * ring->outerRadius / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->outerZ;
        vertex++;
        x = rsin(((i + 1) << 12) / ring->n) * ring->outerRadius / 4096;
        y = rcos(((i + 1) << 12) / ring->n) * ring->outerRadius / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->outerZ;
        vertex++;
    }
}

RingEffect *createRingEffect(s16 brightness, Bytes4 *innerColor, Bytes4 *midColor, Bytes4 *outerColor, EffectTemplate *template, s32 segments, u8 abr, u8 texDepth, s32 primType,
                     s16 innerRadius, s16 outerRadius, s16 midPercent, s16 innerZ, s16 outerZ, Bytes8 *texCoords, s32 tpage, s32 clut, s32 texAnimId, u8 u1, u8 u2,
                     s32 w, s32 x) {
    RingEffect *ring;
    u8 *prim;
    s32 i;
    s32 j;

    ring = allocTaskHeapBlock(sizeof(RingEffect));
    ring->type = primType;
    ring->n = segments;
    ring->innerRadius = innerRadius;
    ring->outerRadius = outerRadius;
    ring->midPercent = midPercent;
    ring->innerZ = innerZ;
    ring->outerZ = outerZ;
    /* 6 vertices per segment: inner, middle and outer ring, at both edges */
    ring->vertices = allocTaskHeapBlock(ring->n * 48);
    buildRingEffectMesh(ring);
    if (primType == 13) {
        ring->texCoords = *texCoords;
        ring->tpage = tpage;
        ring->clut = clut;
        if (texAnimId >= 0 && SUG_startTexAnim(texAnimId, 1, ring, ring->texAnim, x) != 0) {
            ring->texAnimActive = 1;
        } else {
            ring->texAnimActive = -1;
        }
    } else {
        ring->texAnimActive = -1;
    }
    ring->texDepth = texDepth;
    ring->brightness = brightness;
    ring->prevBrightness = -1;
    ring->innerColor = *innerColor;
    ring->midColor = *midColor;
    ring->outerColor = *outerColor;
    *(EffectTemplate *)ring = *template;
    initEffectObject(ring);
    ring->cullBackface = u2;
    ring->fixedOtz = w;
    ring->axisMode = u1;
    ring->abr = abr;
    for (i = 0; i < 2; i++) {
        if (ring->type < 10) {
            ring->tpagePrims[i] = allocTaskHeapBlock(ring->n * 16);
        } else {
            ring->tpagePrims[i] = 0;
        }
        prim = ring->prims[i] = allocTaskHeapBlock(PRIM_SIZES[ring->type] * ring->n * 2);
        for (j = 0; j < ring->n * 2; j++) {
            initPrimByType(ring->type, prim, abr, 0);
            if (ring->type < 10) {
                SetDrawTPage(ring->tpagePrims[i] + j * 8, 0, 0, GetTPage(0, texDepth, 0, 0));
            }
            prim += PRIM_SIZES[ring->type];
        }
    }
    return ring;
}

/* each segment draws two quads, inner-middle and middle-outer: POLY_G4 for
   type 9, POLY_GT4 for type 13; the colours of both frame buffers' prims are
   only rewritten when the brightness changed */
void renderRingEffect(RingEffect *ring) {
    u8 innerRgb[8];
    u8 midRgb[8];
    u8 outerRgb[8];
    SVECTOR *vertex;
    s32 i;

    if (((EffectObject *)ring)->suspended != 0) {
        TICK_START_DELAY(ring);
        return;
    }
    PushMatrix();
    tickEffectMotion((s32)ring, ring->axisMode);
    ring->brightness = updateEffectBrightness(ring, ring->brightness);
    if (ring->brightness == 0) {
        PopMatrix();
        return;
    }
    vertex = ring->vertices;
    if (ring->brightness != ring->prevBrightness) {
        innerRgb[0] = ring->innerColor.b[0] * ring->brightness / 256;
        innerRgb[1] = ring->innerColor.b[1] * ring->brightness / 256;
        innerRgb[2] = ring->innerColor.b[2] * ring->brightness / 256;
        midRgb[0] = ring->midColor.b[0] * ring->brightness / 256;
        midRgb[1] = ring->midColor.b[1] * ring->brightness / 256;
        midRgb[2] = ring->midColor.b[2] * ring->brightness / 256;
        outerRgb[0] = ring->outerColor.b[0] * ring->brightness / 256;
        outerRgb[1] = ring->outerColor.b[1] * ring->brightness / 256;
        outerRgb[2] = ring->outerColor.b[2] * ring->brightness / 256;
    }
    switch (ring->type) {
    case 9: {
        u8 *prim;
        u8 *otherPrim;
        u8 *tpagePrim;

        prim = ring->prims[FRAME_BUFFER_INDEX];
        otherPrim = ring->prims[FRAME_BUFFER_INDEX ^ 1];
        tpagePrim = ring->tpagePrims[FRAME_BUFFER_INDEX];
        for (i = 0; i < ring->n; i++) {
            if (ring->brightness != ring->prevBrightness) {
                setPrimRgb0(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb0(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
            }
            transformAndAddPolyG4((s32)prim, (s32)tpagePrim, (s32)&vertex[0], (s32)&vertex[1], (s32)&vertex[2], (s32)&vertex[3], ring->abr, ring->cullBackface, ring->fixedOtz);
            prim += sizeof(POLY_G4);
            otherPrim += sizeof(POLY_G4);
            tpagePrim += 8;
            if (ring->brightness != ring->prevBrightness) {
                setPrimRgb0(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb0(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
            }
            transformAndAddPolyG4((s32)prim, (s32)tpagePrim, (s32)&vertex[2], (s32)&vertex[3], (s32)&vertex[4], (s32)&vertex[5], ring->abr, ring->cullBackface, ring->fixedOtz);
            prim += sizeof(POLY_G4);
            otherPrim += sizeof(POLY_G4);
            tpagePrim += 8;
            vertex += 6;
        }
        break;
    }
    case 13: {
        u8 *prim;
        u8 *otherPrim;

        if (ring->texAnimActive >= 0) {
            SUG_tickTexAnim(ring->texAnim);
        }
        prim = ring->prims[FRAME_BUFFER_INDEX];
        otherPrim = ring->prims[FRAME_BUFFER_INDEX ^ 1];
        for (i = 0; i < ring->n; i++) {
            setPrimQuadUvRect(prim, ring->texCoords.b[0], ring->texCoords.b[2], ring->texCoords.b[4], ring->texCoords.b[6]);
            ((POLY_GT4 *)prim)->tpage = ring->tpage;
            ((POLY_GT4 *)prim)->clut = ring->clut;
            if (ring->brightness != ring->prevBrightness) {
                setPrimRgb0(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb0(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
            }
            transformAndAddPolyGT4((s32)prim, (s32)&vertex[0], (s32)&vertex[1], (s32)&vertex[2], (s32)&vertex[3], ring->cullBackface, ring->fixedOtz);
            prim += sizeof(POLY_GT4);
            otherPrim += sizeof(POLY_GT4);
            setPrimQuadUvRect(prim, ring->texCoords.b[0], ring->texCoords.b[2], ring->texCoords.b[4], ring->texCoords.b[6]);
            ((POLY_GT4 *)prim)->tpage = ring->tpage;
            ((POLY_GT4 *)prim)->clut = ring->clut;
            if (ring->brightness != ring->prevBrightness) {
                setPrimRgb0(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb0(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
            }
            transformAndAddPolyGT4((s32)prim, (s32)&vertex[2], (s32)&vertex[3], (s32)&vertex[4], (s32)&vertex[5], ring->cullBackface, ring->fixedOtz);
            prim += sizeof(POLY_GT4);
            otherPrim += sizeof(POLY_GT4);
            vertex += 6;
        }
        break;
    }
    }
    PopMatrix();
    ring->prevBrightness = ring->brightness;
}

void freeRingEffect(RingEffect *ring) {
    s32 i;

    for (i = 0; i < 2; i++) {
        freeHeapBlock(ring->tpagePrims[i]);
        freeHeapBlock(ring->prims[i]);
    }
    if (ring->texAnimActive >= 0) {
        SUG_freeTexAnim(ring->texAnim);
    }
    freeHeapBlock(ring->vertices);
    freeHeapBlock(ring);
}
#endif

#if BUILD_STREAKS
/* jp's streak particles have no swirl and no length step */
#if VERSION_JP
StreakParticles *createStreakParticles(u8 *startColor, u8 *endColor, EffectTemplate *template, s16 spreadX, s16 spreadY, s16 length, s16 frames, s16 speedRange, s16 reverse,
                         s16 count, s16 zOffset, s16 spin, s16 pattern, s16 kind, s16 semi, s32 axisMode, s32 fixedOtz) {
    StreakParticles *fx;
    Particle *particle;
    LINE_G2 *line;
    s32 i;
    s32 spinAngle;
    s32 angle;

    fx = allocTaskHeapBlock(sizeof(StreakParticles));
    particle = allocTaskHeapBlock(count * sizeof(Particle));
    spinAngle = 0;
    fx->p = particle;
    if (template == 0) {
        fx->parent = SCENE_3D->viewMatrix;
        fx->own = 0;
    } else {
        fx->base = *template;
        initEffectObject(fx);
        fx->parent = fx;
        fx->own = 1;
    }
    fx->zOffset = zOffset;
    fx->fixedOtz = fixedOtz;
    fx->axisMode = axisMode;
    fx->count = count;
    fx->frame = 0;
    fx->frames = frames;
    fx->rgb[0] = startColor[0];
    fx->rgb[1] = startColor[1];
    fx->rgb[2] = startColor[2];
    fx->drgb[0] = (endColor[0] - fx->rgb[0]) / fx->frames;
    fx->drgb[1] = (endColor[1] - fx->rgb[1]) / fx->frames;
    fx->drgb[2] = (endColor[2] - fx->rgb[2]) / fx->frames;
    fx->direction = reverse == 0 ? 1 : -1;
    fx->kind = kind;
    for (i = 0; i < fx->count; i++, particle++) {
        if (fx->kind == 0) {
            line = &particle->line[0];
            SetLineF2(line);
            setSemiTrans(line, semi);
            line = &particle->line[1];
            SetLineF2(line);
            setSemiTrans(line, semi);
        } else {
            line = &particle->line[0];
            SetLineG2(line);
            setSemiTrans(line, semi);
            line->r0 = startColor[0];
            line->g0 = startColor[1];
            line->b0 = startColor[2];
            line->r1 = endColor[0];
            line->g1 = endColor[1];
            line->b1 = endColor[2];
            line++;
            SetLineG2(line);
            setSemiTrans(line, semi);
            line->r0 = startColor[0];
            line->g0 = startColor[1];
            line->b0 = startColor[2];
            line->r1 = endColor[0];
            line->g1 = endColor[1];
            line->b1 = endColor[2];
        }
        initTransform(particle, (s32)fx->parent, 0, 0, 0, 0, 0, 0);
        particle->length = length;
        particle->speed = rand() % speedRange + 1;
        if (pattern < 3) {
            particle->rotY = rand() % spreadX - spreadX / 2;
            particle->rotX = rand() % spreadY - spreadY / 2;
            particle->rotZ = 0;
            particle->unk7A = 0;
        } else {
            particle->rotY = 0;
            particle->rotX = 0;
            particle->rotZ = 0;
            particle->unk7A = spreadX - 0xB4;
        }
        particle->distance = particle->speed * frames;
        particle->posZ = 0;
        particle->posY = 0;
        particle->posX = 0;
        if (spin != 0) {
            switch ((s16)(pattern % 3)) {
            case 0:
                angle = i << 12;
                spinAngle = angle / fx->count;
                particle->posY = spin;
                break;
            case 1:
                spinAngle = rand() % 4096;
                particle->posY = spin;
                break;
            case 2:
                spinAngle = rand() % 4096;
                particle->posY = rand() % spin;
                break;
            }
            particle->rotZ = spinAngle;
        }
    }
    return fx;
}
#elif VERSION_US || VERSION_EU
StreakParticles *createStreakParticles(u8 *startColor, u8 *endColor, EffectTemplate *template, s16 spreadX, s16 spreadY, s16 length, s16 endLength, s16 frames, s16 speedRange, s16 reverse,
                         s16 count, s16 zOffset, s16 spin, s16 pattern, s16 kind, s16 semi, s32 flags, s32 fixedOtz) {
    StreakParticles *fx;
    Particle *particle;
    LINE_G2 *line;
    s32 i;
    s32 spinAngle;
    s32 angle;

    fx = allocTaskHeapBlock(sizeof(StreakParticles));
    fx->p = particle = allocTaskHeapBlock(count * sizeof(Particle));
    spinAngle = 0;
    if (template == 0) {
        fx->parent = SCENE_3D->viewMatrix;
        fx->own = 0;
    } else {
        fx->base = *template;
        initEffectObject(fx);
        fx->parent = fx;
        fx->own = 1;
    }
    fx->zOffset = zOffset;
    fx->fixedOtz = fixedOtz;
    fx->axisMode = flags & 1;
    fx->count = count;
    fx->frame = 0;
    fx->frames = frames;
    fx->rgb[0] = startColor[0];
    fx->rgb[1] = startColor[1];
    fx->rgb[2] = startColor[2];
    fx->drgb[0] = (endColor[0] - fx->rgb[0]) / fx->frames;
    fx->drgb[1] = (endColor[1] - fx->rgb[1]) / fx->frames;
    fx->drgb[2] = (endColor[2] - fx->rgb[2]) / fx->frames;
    fx->direction = reverse == 0 ? 1 : -1;
    fx->kind = kind;
    for (i = 0; i < fx->count; i++, particle++) {
        if (fx->kind == 0) {
            line = &particle->line[0];
            SetLineF2(line);
            setSemiTrans(line, semi);
            line = &particle->line[1];
            SetLineF2(line);
            setSemiTrans(line, semi);
        } else {
            line = &particle->line[0];
            SetLineG2(line);
            setSemiTrans(line, semi);
            line->r0 = startColor[0];
            line->g0 = startColor[1];
            line->b0 = startColor[2];
            line->r1 = endColor[0];
            line->g1 = endColor[1];
            line->b1 = endColor[2];
            line++;
            SetLineG2(line);
            setSemiTrans(line, semi);
            line->r0 = startColor[0];
            line->g0 = startColor[1];
            line->b0 = startColor[2];
            line->r1 = endColor[0];
            line->g1 = endColor[1];
            line->b1 = endColor[2];
        }
        initTransform(particle, (s32)fx->parent, 0, 0, 0, 0, 0, 0);
        particle->length = length * 8;
        particle->speed = rand() % speedRange + 1;
        if (spreadY == 0) {
            spreadY = 1;
        }
        if (spreadX == 0) {
            spreadX = 1;
        }
        if (pattern < 3) {
            particle->rotY = rand() % spreadX - spreadX / 2;
            particle->rotX = rand() % spreadY - spreadY / 2;
            particle->rotZ = 0;
            particle->unk7A = 0;
        } else {
            particle->rotY = 0;
            particle->rotX = 0;
            particle->rotZ = 0;
            particle->unk7A = spreadX - 0xB4;
        }
        particle->distance = particle->speed * frames;
        particle->posZ = 0;
        particle->posY = 0;
        particle->posX = 0;
        if (spin != 0) {
            switch ((s16)(pattern % 3)) {
            case 0:
                angle = i << 12;
                spinAngle = angle / fx->count;
                particle->posY = spin;
                break;
            case 1:
                spinAngle = rand() % 4096;
                particle->posY = spin;
                break;
            case 2:
                spinAngle = rand() % 4096;
                particle->posY = rand() % spin;
                break;
            }
            particle->rotZ = spinAngle;
        }
        particle->angle = rand() & 0xFFF;
        particle->angleSpeed = rand() & 0x1FF;
        if (i & 1) {
            particle->angleSpeed = -particle->angleSpeed;
        }
    }
    fx->swirl = flags & 2;
    if (endLength == 0) {
        fx->lengthStep = 0;
    } else {
        fx->lengthStep = (endLength - length) * 8 / fx->frames;
    }
    return fx;
}
#endif

#if VERSION_JP
void renderStreakParticles(StreakParticles *fx) {
    Particle *particle;
    LINE_G2 *line;
    SVECTOR *vertex;
    s32 limit;
    s32 i;
    s32 frame;
    s32 length;
    u32 otz;
    s32 interp;
    u8 r;
    u8 g;
    u8 b;
    s32 flag;

    particle = fx->p;
    /* every particle streaks as long as the first one */
    length = particle->length * fx->direction;
    limit = 10000;
    if (fx->own != 0) {
        if (((EffectObject *)fx)->suspended != 0) {
            TICK_START_DELAY(fx);
            return;
        }
        PushMatrix();
        tickEffectMotion((s32)fx, fx->axisMode);
        PopMatrix();
        /* the effect's period rounded up to whole particle cycles */
        limit = (((EffectObject *)fx)->period + fx->frames - 1) / fx->frames * fx->frames;
    }
    PushMatrix();
    if (fx->kind == 0) {
        for (i = 0; i < fx->count; i++) {
            frame = fx->frame + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->posX;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->direction < 0) {
                    vertex->vz = particle->distance - particle->speed * frame;
                } else {
                    vertex->vz = particle->speed * frame;
                }
                vertex->vz += fx->zOffset;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vz += length;
                    otz = RotTransPers((s32)vertex, (s32)&line->r1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->fixedOtz != 0) {
                            otz = fx->fixedOtz;
                        }
                        r = fx->rgb[0] + fx->drgb[0] * frame;
                        g = fx->rgb[1] + fx->drgb[1] * frame;
                        b = fx->rgb[2] + fx->drgb[2] * frame;
                        line->r0 = r;
                        line->g0 = g;
                        line->b0 = b;
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle++;
        }
    } else {
        for (i = 0; i < fx->count; i++) {
            frame = fx->frame + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->posX;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->direction < 0) {
                    vertex->vz = particle->distance - particle->speed * frame;
                } else {
                    vertex->vz = particle->speed * frame;
                }
                vertex->vz += fx->zOffset;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vz += length;
                    otz = RotTransPers((s32)vertex, (s32)&line->x1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->fixedOtz != 0) {
                            otz = fx->fixedOtz;
                        }
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle++;
        }
    }
    fx->frame++;
    PopMatrix();
}
#elif VERSION_US || VERSION_EU
void renderStreakParticles(StreakParticles *fx) {
    Particle *particle;
    LINE_G2 *line;
    SVECTOR *vertex;
    s32 limit;
    s32 i;
    s32 frame;
    s32 length;
    u32 otz;
    s16 dx;
    s16 dz;
    s32 interp;
    u8 r;
    u8 g;
    u8 b;
    s32 flag;

    particle = fx->p;
    limit = 10000;
    if (fx->own != 0) {
        if (((EffectObject *)fx)->suspended != 0) {
            TICK_START_DELAY(fx);
            return;
        }
        PushMatrix();
        tickEffectMotion((s32)fx, fx->axisMode);
        PopMatrix();
        /* the effect's period rounded up to whole particle cycles */
        limit = (((EffectObject *)fx)->period + fx->frames - 1) / fx->frames * fx->frames;
    }
    PushMatrix();
    if (fx->kind == 0) {
        if (fx->swirl == 0) {
        for (i = 0; i < fx->count; i++) {
            frame = fx->frame + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->posX;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->direction < 0) {
                    vertex->vz = particle->distance - particle->speed * frame;
                } else {
                    vertex->vz = particle->speed * frame;
                }
                vertex->vz += fx->zOffset;
                /* eu computes the signed length in a temporary of its own */
#if VERSION_US
                length = (particle->length + fx->lengthStep * frame) * fx->direction / 8;
#elif VERSION_EU
                {
                    s32 scaled = (particle->length + fx->lengthStep * frame) * fx->direction;

                    length = scaled / 8;
                }
#endif
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vz += length;
                    otz = RotTransPers((s32)vertex, (s32)&line->r1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->fixedOtz != 0) {
                            otz = fx->fixedOtz;
                        }
                        r = fx->rgb[0] + fx->drgb[0] * frame;
                        g = fx->rgb[1] + fx->drgb[1] * frame;
                        b = fx->rgb[2] + fx->drgb[2] * frame;
                        line->r0 = r;
                        line->g0 = g;
                        line->b0 = b;
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle++;
        }
        } else {
        for (i = 0; i < fx->count; i++) {
            frame = fx->frame + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->posX;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->direction < 0) {
                    vertex->vz = particle->distance - particle->speed * frame;
                } else {
                    vertex->vz = particle->speed * frame;
                }
                vertex->vz += fx->zOffset;
#if VERSION_US
                length = (particle->length + fx->lengthStep * frame) * fx->direction / 16;
#elif VERSION_EU
                {
                    s32 scaled = (particle->length + fx->lengthStep * frame) * fx->direction;

                    length = scaled / 16;
                }
#endif
                vertex->vx += dx = length * rsin(particle->angle) / 4096;
                vertex->vz += dz = length * rcos(particle->angle) / 4096;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vx -= dx;
                    vertex->vz -= dz;
                    otz = RotTransPers((s32)vertex, (s32)&line->r1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->fixedOtz != 0) {
                            otz = fx->fixedOtz;
                        }
                        r = fx->rgb[0] + fx->drgb[0] * frame;
                        g = fx->rgb[1] + fx->drgb[1] * frame;
                        b = fx->rgb[2] + fx->drgb[2] * frame;
                        line->r0 = r;
                        line->g0 = g;
                        line->b0 = b;
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle->angle += particle->angleSpeed;
            particle++;
        }
        }
    } else {
        if (fx->swirl == 0) {
        for (i = 0; i < fx->count; i++) {
            frame = fx->frame + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->posX;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->direction < 0) {
                    vertex->vz = particle->distance - particle->speed * frame;
                } else {
                    vertex->vz = particle->speed * frame;
                }
                vertex->vz += fx->zOffset;
#if VERSION_US
                length = (particle->length + fx->lengthStep * frame) * fx->direction / 8;
#elif VERSION_EU
                {
                    s32 scaled = (particle->length + fx->lengthStep * frame) * fx->direction;

                    length = scaled / 8;
                }
#endif
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vz += length;
                    otz = RotTransPers((s32)vertex, (s32)&line->x1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->fixedOtz != 0) {
                            otz = fx->fixedOtz;
                        }
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle++;
        }
        } else {
        for (i = 0; i < fx->count; i++) {
            frame = fx->frame + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->posX;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->direction < 0) {
                    vertex->vz = particle->distance - particle->speed * frame;
                } else {
                    vertex->vz = particle->speed * frame;
                }
                vertex->vz += fx->zOffset;
#if VERSION_US
                length = (particle->length + fx->lengthStep * frame) * fx->direction / 16;
#elif VERSION_EU
                {
                    s32 scaled = (particle->length + fx->lengthStep * frame) * fx->direction;

                    length = scaled / 16;
                }
#endif
                vertex->vx += dx = length * rsin(particle->angle) / 4096;
                vertex->vz += dz = length * rcos(particle->angle) / 4096;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vx -= dx;
                    vertex->vz -= dz;
                    otz = RotTransPers((s32)vertex, (s32)&line->x1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->fixedOtz != 0) {
                            otz = fx->fixedOtz;
                        }
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle->angle += particle->angleSpeed;
            particle++;
        }
        }
    }
    fx->frame++;
    PopMatrix();
}
#endif

void freeStreakParticles(StreakParticles *fx) {
    freeHeapBlock(fx->p);
    freeHeapBlock(fx);
}
#endif
