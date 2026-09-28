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

void buildRingEffectMesh(Obj32 *ring) {
    SVECTOR *vertex;
    s32 i;
    s16 x;
    s16 y;

    vertex = ring->unk16C;
    for (i = 0; i < ring->n; i++) {
        x = rsin((i << 12) / ring->n) * ring->unk19C[0] / 4096;
        y = rcos((i << 12) / ring->n) * ring->unk19C[0] / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[3];
        vertex++;
        x = rsin(((i + 1) << 12) / ring->n) * ring->unk19C[0] / 4096;
        y = rcos(((i + 1) << 12) / ring->n) * ring->unk19C[0] / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[3];
        vertex++;
        x = rsin((i << 12) / ring->n) * (ring->unk19C[0] + (ring->unk19C[1] - ring->unk19C[0]) * ring->unk19C[2] / 100) / 4096;
        y = rcos((i << 12) / ring->n) * (ring->unk19C[0] + (ring->unk19C[1] - ring->unk19C[0]) * ring->unk19C[2] / 100) / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[3] + (ring->unk19C[4] - ring->unk19C[3]) * ring->unk19C[2] / 100;
        vertex++;
        x = rsin(((i + 1) << 12) / ring->n) * (ring->unk19C[0] + (ring->unk19C[1] - ring->unk19C[0]) * ring->unk19C[2] / 100) / 4096;
        y = rcos(((i + 1) << 12) / ring->n) * (ring->unk19C[0] + (ring->unk19C[1] - ring->unk19C[0]) * ring->unk19C[2] / 100) / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[3] + (ring->unk19C[4] - ring->unk19C[3]) * ring->unk19C[2] / 100;
        vertex++;
        x = rsin((i << 12) / ring->n) * ring->unk19C[1] / 4096;
        y = rcos((i << 12) / ring->n) * ring->unk19C[1] / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[4];
        vertex++;
        x = rsin(((i + 1) << 12) / ring->n) * ring->unk19C[1] / 4096;
        y = rcos(((i + 1) << 12) / ring->n) * ring->unk19C[1] / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[4];
        vertex++;
    }
}

Obj32 *createRingEffect(s16 brightness, Bytes4 *innerColor, Bytes4 *midColor, Bytes4 *outerColor, Unk13C *template, s32 segments, u8 abr, u8 texDepth, s32 primType,
                     s16 innerRadius, s16 outerRadius, s16 midPercent, s16 innerZ, s16 outerZ, Bytes8 *texCoords, s32 tpage, s32 clut, s32 texAnimId, u8 u1, u8 u2,
                     s32 w, s32 x) {
    Obj32 *ring;
    u8 *prim;
    s32 i;
    s32 j;

    ring = allocTaskHeapBlock(0x1B0);
    ring->type = primType;
    ring->n = segments;
    ring->unk19C[0] = innerRadius;
    ring->unk19C[1] = outerRadius;
    ring->unk19C[2] = midPercent;
    ring->unk19C[3] = innerZ;
    ring->unk19C[4] = outerZ;
    ring->unk16C = allocTaskHeapBlock(segments * 48);
    buildRingEffectMesh(ring);
    if (primType == 13) {
        ring->unk170 = *texCoords;
        ring->unk178 = tpage;
        ring->unk17C = clut;
        if (texAnimId >= 0 && func_801E6C78(texAnimId, 1, ring, ring->unk13C, x) != 0) {
            ring->unk1AD = 1;
        } else {
            ring->unk1AD = -1;
        }
    } else {
        ring->unk1AD = -1;
    }
    ring->unk198 = texDepth;
    ring->unk1A6 = brightness;
    ring->unk1A8 = -1;
    ring->unk185 = *innerColor;
    ring->unk189 = *midColor;
    ring->unk18D = *outerColor;
    *(Unk13C *)ring = *template;
    initEffectObject(ring);
    ring->unk1AB = u2;
    ring->unk194 = w;
    ring->unk1AA = u1;
    ring->unk1AC = abr;
    for (i = 0; i < 2; i++) {
        if (ring->type < 10) {
            ring->unk15C[i] = allocTaskHeapBlock(ring->n * 16);
        } else {
            ring->unk15C[i] = 0;
        }
        prim = ring->unk164[i] = allocTaskHeapBlock(PRIM_SIZES[ring->type] * ring->n * 2);
        for (j = 0; j < ring->n * 2; j++) {
            initPrimByType(ring->type, prim, abr, 0);
            if (ring->type < 10) {
                SetDrawTPage(ring->unk15C[i] + j * 8, 0, 0, GetTPage(0, texDepth, 0, 0));
            }
            prim += PRIM_SIZES[ring->type];
        }
    }
    return ring;
}

void renderRingEffect(Obj32 *ring) {
    u8 innerRgb[8];
    u8 midRgb[8];
    u8 outerRgb[8];
    SVECTOR *vertex;
    s32 i;

    if (ring->unk0[0x139] != 0) {
        tickEffectStartDelay(ring);
        return;
    }
    PushMatrix();
    tickEffectMotion((s32)ring, ring->unk1AA);
    ring->unk1A6 = updateEffectBrightness(ring, ring->unk1A6);
    if (ring->unk1A6 == 0) {
        PopMatrix();
        return;
    }
    vertex = ring->unk16C;
    if (ring->unk1A6 != ring->unk1A8) {
        innerRgb[0] = ring->unk185.b[0] * ring->unk1A6 / 256;
        innerRgb[1] = ring->unk185.b[1] * ring->unk1A6 / 256;
        innerRgb[2] = ring->unk185.b[2] * ring->unk1A6 / 256;
        midRgb[0] = ring->unk189.b[0] * ring->unk1A6 / 256;
        midRgb[1] = ring->unk189.b[1] * ring->unk1A6 / 256;
        midRgb[2] = ring->unk189.b[2] * ring->unk1A6 / 256;
        outerRgb[0] = ring->unk18D.b[0] * ring->unk1A6 / 256;
        outerRgb[1] = ring->unk18D.b[1] * ring->unk1A6 / 256;
        outerRgb[2] = ring->unk18D.b[2] * ring->unk1A6 / 256;
    }
    switch (ring->type) {
    case 9: {
        u8 *prim;
        u8 *otherPrim;
        u8 *tpagePrim;

        prim = ring->unk164[FRAME_BUFFER_INDEX];
        otherPrim = ring->unk164[FRAME_BUFFER_INDEX ^ 1];
        tpagePrim = ring->unk15C[FRAME_BUFFER_INDEX];
        for (i = 0; i < ring->n; i++) {
            if (ring->unk1A6 != ring->unk1A8) {
                setPrimRgb0(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb0(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
            }
            transformAndAddPolyG4((s32)prim, (s32)tpagePrim, (s32)&vertex[0], (s32)&vertex[1], (s32)&vertex[2], (s32)&vertex[3], ring->unk1AC, ring->unk1AB, ring->unk194);
            prim += 0x24;
            otherPrim += 0x24;
            tpagePrim += 8;
            if (ring->unk1A6 != ring->unk1A8) {
                setPrimRgb0(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb0(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
            }
            transformAndAddPolyG4((s32)prim, (s32)tpagePrim, (s32)&vertex[2], (s32)&vertex[3], (s32)&vertex[4], (s32)&vertex[5], ring->unk1AC, ring->unk1AB, ring->unk194);
            prim += 0x24;
            otherPrim += 0x24;
            tpagePrim += 8;
            vertex += 6;
        }
        break;
    }
    case 13: {
        u8 *prim;
        u8 *otherPrim;

        if (ring->unk1AD >= 0) {
            func_801E7020(ring->unk13C);
        }
        prim = ring->unk164[FRAME_BUFFER_INDEX];
        otherPrim = ring->unk164[FRAME_BUFFER_INDEX ^ 1];
        for (i = 0; i < ring->n; i++) {
            setPrimQuadUvRect(prim, ring->unk170.b[0], ring->unk170.b[2], ring->unk170.b[4], ring->unk170.b[6]);
            *(u16 *)(prim + 0x1A) = ring->unk178;
            *(u16 *)(prim + 0xE) = ring->unk17C;
            if (ring->unk1A6 != ring->unk1A8) {
                setPrimRgb0(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb0(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
            }
            transformAndAddPolyGT4((s32)prim, (s32)&vertex[0], (s32)&vertex[1], (s32)&vertex[2], (s32)&vertex[3], ring->unk1AB, ring->unk194);
            prim += 0x34;
            otherPrim += 0x34;
            setPrimQuadUvRect(prim, ring->unk170.b[0], ring->unk170.b[2], ring->unk170.b[4], ring->unk170.b[6]);
            *(u16 *)(prim + 0x1A) = ring->unk178;
            *(u16 *)(prim + 0xE) = ring->unk17C;
            if (ring->unk1A6 != ring->unk1A8) {
                setPrimRgb0(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb0(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
            }
            transformAndAddPolyGT4((s32)prim, (s32)&vertex[2], (s32)&vertex[3], (s32)&vertex[4], (s32)&vertex[5], ring->unk1AB, ring->unk194);
            prim += 0x34;
            otherPrim += 0x34;
            vertex += 6;
        }
        break;
    }
    }
    PopMatrix();
    ring->unk1A8 = ring->unk1A6;
}

void freeRingEffect(Obj32 *ring) {
    s32 i;

    for (i = 0; i < 2; i++) {
        freeHeapBlock(ring->unk15C[i]);
        freeHeapBlock(ring->unk164[i]);
    }
    if (ring->unk1AD >= 0) {
        func_801E72D4(ring->unk13C);
    }
    freeHeapBlock(ring->unk16C);
    freeHeapBlock(ring);
}

Particles *createStreakParticles(u8 *startColor, u8 *endColor, Unk13C *template, s16 spreadX, s16 spreadY, s16 length, s16 endLength, s16 frames, s16 speedRange, s16 reverse,
                         s16 count, s16 zOffset, s16 spin, s16 pattern, s16 kind, s16 semi, s32 flags, s32 fixedOtz) {
    Particles *fx;
    Particle *particle;
    LINE_G2 *line;
    s32 i;
    s32 spinAngle;
    s32 angle;

    fx = allocTaskHeapBlock(0x15C);
    fx->p = particle = allocTaskHeapBlock(count * 0x88);
    spinAngle = 0;
    if (template == 0) {
        fx->parent = (u8 *)SCENE_3D + 0x78;
        fx->own = 0;
    } else {
        fx->base = *template;
        initEffectObject(fx);
        fx->parent = fx;
        fx->own = 1;
    }
    fx->unk14E = zOffset;
    fx->unk154 = fixedOtz;
    fx->unk15A = flags & 1;
    fx->count = count;
    fx->unk150 = 0;
    fx->frames = frames;
    fx->rgb[0] = startColor[0];
    fx->rgb[1] = startColor[1];
    fx->rgb[2] = startColor[2];
    fx->drgb[0] = (endColor[0] - fx->rgb[0]) / fx->frames;
    fx->drgb[1] = (endColor[1] - fx->rgb[1]) / fx->frames;
    fx->drgb[2] = (endColor[2] - fx->rgb[2]) / fx->frames;
    fx->unk158 = reverse == 0 ? 1 : -1;
    fx->kind = kind;
    for (i = 0; i < fx->count; i++, particle++) {
        if (fx->kind == 0) {
            line = &particle->line[0];
            func_800678E4(line);
            setSemiTrans(line, semi);
            line = &particle->line[1];
            func_800678E4(line);
            setSemiTrans(line, semi);
        } else {
            line = &particle->line[0];
            func_80067904(line);
            setSemiTrans(line, semi);
            line->r0 = startColor[0];
            line->g0 = startColor[1];
            line->b0 = startColor[2];
            line->r1 = endColor[0];
            line->g1 = endColor[1];
            line->b1 = endColor[2];
            line++;
            func_80067904(line);
            setSemiTrans(line, semi);
            line->r0 = startColor[0];
            line->g0 = startColor[1];
            line->b0 = startColor[2];
            line->r1 = endColor[0];
            line->g1 = endColor[1];
            line->b1 = endColor[2];
        }
        initTransform(particle, (s32)fx->parent, 0, 0, 0, 0, 0, 0);
        particle->unk7C = length * 8;
        particle->unk7E = rand() % speedRange + 1;
        if (spreadY == 0) {
            spreadY = 1;
        }
        if (spreadX == 0) {
            spreadX = 1;
        }
        if (pattern < 3) {
            particle->unk32 = rand() % spreadX - spreadX / 2;
            particle->unk30 = rand() % spreadY - spreadY / 2;
            particle->unk34 = 0;
            particle->unk7A = 0;
        } else {
            particle->unk32 = 0;
            particle->unk30 = 0;
            particle->unk34 = 0;
            particle->unk7A = spreadX - 0xB4;
        }
        particle->unk80 = particle->unk7E * frames;
        particle->unk78 = 0;
        particle->unk76 = 0;
        particle->unk74 = 0;
        if (spin != 0) {
            switch ((s16)(pattern % 3)) {
            case 0:
                angle = i << 12;
                spinAngle = angle / fx->count;
                particle->unk76 = spin;
                break;
            case 1:
                spinAngle = rand() % 4096;
                particle->unk76 = spin;
                break;
            case 2:
                spinAngle = rand() % 4096;
                particle->unk76 = rand() % spin;
                break;
            }
            particle->unk34 = spinAngle;
        }
        particle->unk82 = rand() & 0xFFF;
        particle->unk84 = rand() & 0x1FF;
        if (i & 1) {
            particle->unk84 = -particle->unk84;
        }
    }
    fx->unk147 = flags & 2;
    if (endLength == 0) {
        fx->unk156 = 0;
    } else {
        fx->unk156 = (endLength - length) * 8 / fx->frames;
    }
    return fx;
}

void renderStreakParticles(Particles *fx) {
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
        if (((u8 *)fx)[0x139] != 0) {
            tickEffectStartDelay(fx);
            return;
        }
        PushMatrix();
        tickEffectMotion((s32)fx, fx->unk15A);
        PopMatrix();
        limit = (*(s16 *)((u8 *)fx + 0x124) + fx->frames - 1) / fx->frames * fx->frames;
    }
    PushMatrix();
    if (fx->kind == 0) {
        if (fx->unk147 == 0) {
        for (i = 0; i < fx->count; i++) {
            frame = fx->unk150 + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->unk74;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->unk158 < 0) {
                    vertex->vz = particle->unk80 - particle->unk7E * frame;
                } else {
                    vertex->vz = particle->unk7E * frame;
                }
                vertex->vz += fx->unk14E;
                length = (particle->unk7C + fx->unk156 * frame) * fx->unk158 / 8;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vz += length;
                    otz = RotTransPers((s32)vertex, (s32)&line->r1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->unk154 != 0) {
                            otz = fx->unk154;
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
            frame = fx->unk150 + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->unk74;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->unk158 < 0) {
                    vertex->vz = particle->unk80 - particle->unk7E * frame;
                } else {
                    vertex->vz = particle->unk7E * frame;
                }
                vertex->vz += fx->unk14E;
                length = (particle->unk7C + fx->unk156 * frame) * fx->unk158 / 16;
                vertex->vx += dx = length * rsin(particle->unk82) / 4096;
                vertex->vz += dz = length * rcos(particle->unk82) / 4096;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vx -= dx;
                    vertex->vz -= dz;
                    otz = RotTransPers((s32)vertex, (s32)&line->r1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->unk154 != 0) {
                            otz = fx->unk154;
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
            particle->unk82 += particle->unk84;
            particle++;
        }
        }
    } else {
        if (fx->unk147 == 0) {
        for (i = 0; i < fx->count; i++) {
            frame = fx->unk150 + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->unk74;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->unk158 < 0) {
                    vertex->vz = particle->unk80 - particle->unk7E * frame;
                } else {
                    vertex->vz = particle->unk7E * frame;
                }
                vertex->vz += fx->unk14E;
                length = (particle->unk7C + fx->unk156 * frame) * fx->unk158 / 8;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vz += length;
                    otz = RotTransPers((s32)vertex, (s32)&line->x1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->unk154 != 0) {
                            otz = fx->unk154;
                        }
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle++;
        }
        } else {
        for (i = 0; i < fx->count; i++) {
            frame = fx->unk150 + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->unk74;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->unk158 < 0) {
                    vertex->vz = particle->unk80 - particle->unk7E * frame;
                } else {
                    vertex->vz = particle->unk7E * frame;
                }
                vertex->vz += fx->unk14E;
                length = (particle->unk7C + fx->unk156 * frame) * fx->unk158 / 16;
                vertex->vx += dx = length * rsin(particle->unk82) / 4096;
                vertex->vz += dz = length * rcos(particle->unk82) / 4096;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vx -= dx;
                    vertex->vz -= dz;
                    otz = RotTransPers((s32)vertex, (s32)&line->x1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->unk154 != 0) {
                            otz = fx->unk154;
                        }
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle->unk82 += particle->unk84;
            particle++;
        }
        }
    }
    fx->unk150++;
    PopMatrix();
}

void freeStreakParticles(void *fx) {
    freeHeapBlock((*(void **)((s8 *)fx + 0x140)));
    freeHeapBlock(fx);
}
