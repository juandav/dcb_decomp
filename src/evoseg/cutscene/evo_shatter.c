#include "common.h"
#include "game.h"
#include "dcb/evo_shatter.h"
#include "dcb/frame_callback.h"
#include "dcb/task.h"
#include "dcb/sound_play.h"
#include "dcb/evoseg.h"
#include "dcb/evo_cutscene.h"

typedef struct {
    u8 olen;
    u8 ilen;
    u8 flag;
    u8 mode;
    u8 u0, v0;
    u16 cba;
    u8 u1, v1;
    u16 tsb;
    u8 u2, v2;
    u16 pad;
    u8 u3, v3;
    u16 pad2;
    u16 idx[8];
} TmdPacketFT4;

typedef struct {
    u8 olen;
    u8 ilen;
    u8 flag;
    u8 mode;
    u8 u0, v0;
    u16 cba;
    u8 u1, v1;
    u16 tsb;
    u8 u2, v2;
    u16 pad;
    u16 idx[8];
} TmdPacketFT3;

extern s8 *EVO_SHARD_PRIM;
extern SVECTOR *EVO_SHARD_VERTS;
extern EvoColor EVO_SHARD_COLOR;
extern SVECTOR *EVO_SHARD_VERTEX_CURSOR;
extern EvoShatter EVO_SHATTER;
extern s16 EVO_SHATTER_ORDER[40];
extern s16 EVO_SHATTER_TIMERS[42];
extern u16 EVO_SHATTER_DELAY;
extern u32 EVO_RAND_SEED_LO;
extern EvoSpark EVO_SPARKS[16];

SVECTOR *ApplyMatrixSV(MATRIX *m, SVECTOR *v0, SVECTOR *v1);
void EVO_tickShatter();
void EVO_drawSpark(EvoSpark *spark);
void EVO_drawShardTG3(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardTF3(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardTG4(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardTF4(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardTNF3(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardTNF4(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardF4(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardG3(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardG4(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardF3(SVECTOR *pos, s32 unused, s16 div, s16 mul);
s32 EVO_addShard(s32 part, s32 arg);
void EVO_drawShard(s32 index);
void EVO_runSparkTask(EvoSpark *spark);

u32 EVO_RAND_SEED_HI = 0x13CC25;

void EVO_startShatter(s32 model) {
    s32 i;
    s32 j;
    s32 tmp;
    u8 unused[0x50]; /* unused, but it is in the original stack frame */

    EVO_SHATTER.total = -1;
    EVO_SHATTER.next = 0;
    EVO_SHATTER.count = 0;
    EVO_SHATTER.model = model;
    EVO_SHARD_VERTEX_CURSOR = EVO_SHARD_VERTEX_POOL;
    for (i = 0; i < 60; i++) {
        EVO_SHARDS[i].timer = -1;
    }
    for (i = 0; i < 40; i++) {
        EVO_PART_DRAW_MODES[i] = 0x10;
    }
    EVO_SHATTER.total = ((EvoModel *)SCENE_3D->models[EVO_SHATTER.model])->partCount;
    EVO_SHATTER.queue = EVO_SHATTER_ORDER;
    for (i = 0; i < EVO_SHATTER.total; i++) {
        EVO_SHATTER_TIMERS[i] = 1;
    }
    for (i = 0; i < EVO_SHATTER.total; i++) {
        EVO_SHATTER_ORDER[i] = i;
    }
    for (i = 0; i < EVO_SHATTER.total; i++) {
        j = rand() % EVO_SHATTER.total;
        tmp = EVO_SHATTER_ORDER[i];
        EVO_SHATTER_ORDER[i] = EVO_SHATTER_ORDER[j];
        EVO_SHATTER_ORDER[j] = tmp;
    }
    if (EVO_SHATTER_STARTED == 0) {
        addFrameCallback((s32)EVO_tickShatter);
        EVO_SHATTER_STARTED = 1;
    }
    EVO_SHATTER_DELAY = 120;
}

void EVO_tickShatter(void) {
    s32 part;
    s32 i;

    if (EVO_SHATTER.next < EVO_SHATTER.total) {
        if (EVO_SHATTER_DELAY != 0) {
            EVO_SHATTER_DELAY--;
        } else {
            EVO_SHATTER_TIMERS[EVO_SHATTER.next]--;
        }
        if (EVO_SHATTER_TIMERS[EVO_SHATTER.next] < 0) {
            part = *EVO_SHATTER.queue++;
            EVO_SHATTER.next++;
            EVO_addShard(part, part);
            EVO_PART_DRAW_MODES[part] = 0;
            playSoundEffect(0x58);
        }
    }
    if (EVO_SHATTER.next > EVO_SHATTER.total - 1) {
        if (EVO_CUTSCENE_STEP == 0) {
            EVO_CUTSCENE_STEP = 1;
        } else if (EVO_CUTSCENE_STEP == 3) {
            EVO_CUTSCENE_STEP = 4;
        }
    }
    for (i = 0; i < EVO_SHATTER.count; i++) {
        EVO_drawShard(i);
    }
}

void EVO_drawShard(s32 index) {
    EvoShard *shard;
    SVECTOR *offset;
    s32 count;

    shard = &EVO_SHARDS[index];
    offset = shard->offsets;
    count = shard->primCount;
    EVO_SHARD_PRIM = shard->prims;
    EVO_SHARD_VERTS = shard->verts;
    if (shard->timer < 120) {
        if (shard->timer >= 0) {
            shard->timer++;
            EVO_SHARD_COLOR.r = (61 - shard->timer) * 74 / 60 + 54;
            EVO_SHARD_COLOR.g = EVO_SHARD_COLOR.r;
            EVO_SHARD_COLOR.b = EVO_SHARD_COLOR.r;
            while (count-- > 0) {
                if (EVO_SHARD_PRIM[3] == 0x34 || EVO_SHARD_PRIM[3] == 0x36) {
                    EVO_drawShardTG3(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x1C;
                } else if (EVO_SHARD_PRIM[3] == 0x24) {
                    EVO_drawShardTF3(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x18;
                } else if (EVO_SHARD_PRIM[3] == 0x3C || EVO_SHARD_PRIM[3] == 0x3E) {
                    EVO_drawShardTG4(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x24;
                } else if (EVO_SHARD_PRIM[3] == 0x2C) {
                    EVO_drawShardTF4(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x20;
                } else if (EVO_SHARD_PRIM[3] == 0x25) {
                    EVO_drawShardTNF3(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x1C;
                } else if (EVO_SHARD_PRIM[3] == 0x2D || EVO_SHARD_PRIM[3] == 0x2F) {
                    EVO_drawShardTNF4(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x20;
                } else if (EVO_SHARD_PRIM[3] == 0x28) {
                    EVO_drawShardF4(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x14;
                } else if (EVO_SHARD_PRIM[3] == 0x30) {
                    EVO_drawShardG3(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x14;
                } else if (EVO_SHARD_PRIM[3] == 0x38) {
                    EVO_drawShardG4(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x18;
                } else if (EVO_SHARD_PRIM[3] == 0x20) {
                    EVO_drawShardF3(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x10;
                }
            }
        }
    }
}

void EVO_drawShardTG3(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3; /* unused, but it is in the original stack frame */
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = (TmdPacketFT3 *)EVO_SHARD_PRIM;
    poly = (POLY_FT3 *)GsGetWorkBase();
    SetPolyFT3(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[5]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        GsSetWorkBase((long)(poly + 1));
    }
}

void EVO_drawShardTF3(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3; /* unused, but it is in the original stack frame */
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = (TmdPacketFT3 *)EVO_SHARD_PRIM;
    poly = (POLY_FT3 *)GsGetWorkBase();
    SetPolyFT3(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[2]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        GsSetWorkBase((long)(poly + 1));
    }
}

void EVO_drawShardTG4(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT4 *prim;
    POLY_FT4 *poly;

    prim = (TmdPacketFT4 *)EVO_SHARD_PRIM;
    poly = (POLY_FT4 *)GsGetWorkBase();
    SetPolyFT4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    poly->u3 = prim->u3;
    poly->v3 = prim->v3;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[5]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[7]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        GsSetWorkBase((long)(poly + 1));
    }
}

void EVO_drawShardTF4(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT4 *prim;
    POLY_FT4 *poly;

    prim = (TmdPacketFT4 *)EVO_SHARD_PRIM;
    poly = (POLY_FT4 *)GsGetWorkBase();
    SetPolyFT4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    poly->u3 = prim->u3;
    poly->v3 = prim->v3;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[2]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[4]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        GsSetWorkBase((long)(poly + 1));
    }
}

void EVO_drawShardTNF3(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3; /* unused, but it is in the original stack frame */
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = (TmdPacketFT3 *)EVO_SHARD_PRIM;
    poly = (POLY_FT3 *)GsGetWorkBase();
    SetPolyFT3(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[2]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[4]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        GsSetWorkBase((long)(poly + 1));
    }
}

void EVO_drawShardTNF4(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT4 *prim;
    POLY_FT4 *poly;

    prim = (TmdPacketFT4 *)EVO_SHARD_PRIM;
    poly = (POLY_FT4 *)GsGetWorkBase();
    SetPolyFT4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    poly->u3 = prim->u3;
    poly->v3 = prim->v3;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[2]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[4]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[5]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        GsSetWorkBase((long)(poly + 1));
    }
}

s32 EVO_addShard(s32 part, s32 arg) {
    SVECTOR out;
    MATRIX rot;
    MATRIX world;
    SVECTOR n0;
    SVECTOR n1;
    SVECTOR n2;
    SVECTOR n3;
    SVECTOR center;
    MATRIX unused; /* unused, but it is in the original stack frame */
    TmdObject *obj;
    SVECTOR *vert;
    SVECTOR *verts;
    SVECTOR *a;
    SVECTOR *b;
    SVECTOR *c;
    SVECTOR *d;
    u8 *prim;
    EvoShard *shard;
    s32 slot;
    u32 i;
    u32 k;

    obj = ((EvoModel *)SCENE_3D->models[EVO_SHATTER.model])->parts[part].tmd;
    if (obj == NULL) {
        return -1;
    }
    for (slot = 0; slot < 30 && EVO_SHARDS[slot].timer >= 0; slot++) {
    }
    if (slot == 30) {
        return -1;
    }
    PushMatrix();
    rot = ((EvoModel *)SCENE_3D->models[EVO_SHATTER.model])->matrices[part];
    verts = EVO_SHARD_VERTEX_CURSOR;
#if VERSION_US
    vert = obj->vertTop;
#elif VERSION_EU
    /* eu walks the vertices with prim too */
    prim = (u8 *)obj->vertTop;
#else
#error "evoseg/cutscene/evo_shatter: version not checked"
#endif
    world = ((EvoModel *)SCENE_3D->models[EVO_SHATTER.model])->matrices[part];
    for (i = 0; i < obj->nvert; i++) {
#if VERSION_US
        ApplyMatrixSV(&world, vert++, &out);
#elif VERSION_EU
        vert = (SVECTOR *)prim;
        prim += sizeof(SVECTOR);
        ApplyMatrixSV(&world, vert, &out);
#else
#error "evoseg/cutscene/evo_shatter: version not checked"
#endif
        EVO_SHARD_VERTEX_CURSOR->vx = out.vx + world.t[0];
        EVO_SHARD_VERTEX_CURSOR->vy = out.vy + world.t[1];
        EVO_SHARD_VERTEX_CURSOR->vz = out.vz + world.t[2];
        EVO_SHARD_VERTEX_CURSOR++;
    }
    shard = &EVO_SHARDS[slot];
    shard->timer = 0;
    shard->count = obj->nprim;
    shard->offsets = EVO_SHARD_VERTEX_CURSOR;
    shard->part = arg;
    shard->verts = verts;
    shard->prims = (s8 *)obj->primTop;
    shard->primCount = obj->nprim;
    prim = obj->primTop;
    for (k = 0; k < obj->nprim; k++) {
        switch ((s8)(prim[3] - 0x20)) {
        case 0x14:
        case 0x16:
            a = &obj->normTop[*(u16 *)(prim + 0x10)];
            b = &obj->normTop[*(u16 *)(prim + 0x14)];
            c = &obj->normTop[*(u16 *)(prim + 0x18)];
            ApplyMatrixSV(&rot, a, &n0);
            ApplyMatrixSV(&rot, b, &n1);
            ApplyMatrixSV(&rot, c, &n2);
            center.vx = (n0.vx + n1.vx + n2.vx) / 3;
            center.vy = (n0.vy + n1.vy + n2.vy) / 3;
            center.vz = (n0.vz + n1.vz + n2.vz) / 3;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x1C;
            break;
        case 0x1C:
        case 0x1E:
            a = &obj->normTop[*(u16 *)(prim + 0x14)];
            b = &obj->normTop[*(u16 *)(prim + 0x18)];
            c = &obj->normTop[*(u16 *)(prim + 0x1C)];
            d = &obj->normTop[*(u16 *)(prim + 0x20)];
            ApplyMatrixSV(&rot, a, &n0);
            ApplyMatrixSV(&rot, b, &n1);
            ApplyMatrixSV(&rot, c, &n2);
            ApplyMatrixSV(&rot, d, &n3);
            center.vx = (n0.vx + n1.vx + n2.vx) / 3;
            center.vy = (n0.vy + n1.vy + n2.vy) / 3;
            center.vz = (n0.vz + n1.vz + n2.vz) / 3;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x24;
            break;
        case 0x0C:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x14)], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x20;
            break;
        case 0x04:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x10)], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x18;
            break;
        case 0x0D:
        case 0x0F:
            ApplyMatrixSV(&rot, &obj->normTop[1], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x20;
            break;
        case 0x05:
            ApplyMatrixSV(&rot, &obj->normTop[1], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x1C;
            break;
        case 0x08:
        case 0x10:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x8)], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x14;
            break;
        case 0x18:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x8)], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x18;
            break;
        case 0x00:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x8)], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x10;
            break;
        }
    }
    PopMatrix();
    EVO_SHATTER.count = slot + 1;
    return slot;
}

s32 EVO_randomRange(s32 min, s32 max) {
    s32 tmp;

    if (max == min) {
        return min;
    }
    if (max < min) {
        tmp = min;
        min = max;
        max = tmp;
    }
    EVO_RAND_SEED_LO = EVO_RAND_SEED_LO * 0x41C650AD + 0x3039;
    EVO_RAND_SEED_HI = EVO_RAND_SEED_HI * 0x41C650AD + 0x3039;
    return min + ((EVO_RAND_SEED_LO >> 16) | (EVO_RAND_SEED_HI << 16)) % (max - min + 1);
}

void EVO_clearSparks(void) {
    EvoSpark *spark;
    s32 i;

    for (i = 0; i < 16; i++) {
        spark = &EVO_SPARKS[i];
        spark->joint = -1;
    }
}

s32 EVO_addSpark(s16 joint, s16 timer) {
    EvoSpark *spark;
    EvoSpark *slot;
    s32 i;

    for (i = 0; i < 16; i++) {
        slot = &EVO_SPARKS[i];
        if (slot->joint == -1) {
            break;
        }
    }
    if (i == 16) {
        return -1;
    }
    spark = &EVO_SPARKS[i];
    spark->joint = joint;
    spark->timer = timer;
    spawnTask(0, -1, 0, 0x1000, EVO_runSparkTask, spark, getCurrentTaskId(), 0, 0);
    return i;
}

void EVO_runSparkTask(EvoSpark *spark) {
    do {
        waitFrames(1);
        EVO_drawSpark(spark);
    } while (--spark->timer >= 0);
    waitFrames(10);
    spark->joint = -1;
    exitTask();
}

void EVO_drawSpark(EvoSpark *spark) {
    MATRIX mat;
    SVECTOR from;
    SVECTOR to;
    SVECTOR tip;
    s32 depthCue;
    s32 flag;
    s32 dist;
    s32 len;
    PolyF3 *poly;
    s32 shade;

    PushMatrix();
    mat = ((EvoModel *)SCENE_3D->models[0])->matrices[spark->joint];
    from.vx = mat.t[0];
    from.vy = mat.t[1];
    from.vz = mat.t[2];
    mat = ((EvoModel *)SCENE_3D->models[0])->matrices[1];
    to.vx = from.vx - mat.t[0];
    to.vy = from.vy - mat.t[1];
    to.vz = from.vz - mat.t[2];
    from.vx = mat.t[0];
    from.vy = mat.t[1];
    from.vz = mat.t[2];
    dist = to.vx * to.vx + to.vy * to.vy + to.vz * to.vz;
    len = EVO_randomRange(400, 500);
    len *= len;
    if (dist == 0) {
        dist = 1;
    }
    to.vx = from.vx + to.vx * len / dist;
    to.vy = from.vy + to.vy * len / dist;
    to.vz = from.vz + to.vz * len / dist;
    tip.vx = to.vx + EVO_randomRange(-80, 80);
    tip.vy = to.vy + EVO_randomRange(-80, 80);
    tip.vz = to.vz + EVO_randomRange(-80, 80);
    poly = (PolyF3 *)GsGetWorkBase();
    SetPolyF3(poly);
    SetSemiTrans(poly, 1);
    shade = rand() % 128 + 10;
    poly->r0 = shade;
    poly->g0 = shade;
    poly->b0 = shade;
    SetRotMatrix((s32)&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    if ((u32)((RotTransPers3((s32)&tip, (s32)&from, (s32)&to, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag) >> 2) - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)poly);
        GsSetWorkBase((long)(poly + 1));
    }
    PopMatrix();
}

void EVO_drawShardF4(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyF4 *poly;

    prim = (u16 *)EVO_SHARD_PRIM;
    poly = (PolyF4 *)GsGetWorkBase();
    setlen(poly, 5);
    setcode(poly, 0x28);
    SetSemiTrans(poly, 1);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[6]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[7]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[8]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 5);
    setcode(poly, 0x28);
    SetRotMatrix((s32)&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        GsSetWorkBase((long)(poly + 1));
    }
}

void EVO_drawShardG3(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3; /* unused, but it is in the original stack frame */
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyG3 *poly;

    prim = (u16 *)EVO_SHARD_PRIM;
    poly = (PolyG3 *)GsGetWorkBase();
    setlen(poly, 6);
    setcode(poly, 0x30);
    SetSemiTrans(poly, 1);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[7]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[9]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 6);
    setcode(poly, 0x30);
    SetRotMatrix((s32)&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        GsSetWorkBase((long)(poly + 1));
    }
}

void EVO_drawShardG4(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyG4 *poly;

    prim = (u16 *)EVO_SHARD_PRIM;
    poly = (PolyG4 *)GsGetWorkBase();
    setlen(poly, 8);
    setcode(poly, 0x38);
    SetSemiTrans(poly, 1);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[7]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[9]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[11]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 8);
    setcode(poly, 0x38);
    SetRotMatrix((s32)&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        GsSetWorkBase((long)(poly + 1));
    }
}

void EVO_drawShardF3(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3; /* unused, but it is in the original stack frame */
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyF3 *poly;

    prim = (u16 *)EVO_SHARD_PRIM;
    poly = (PolyF3 *)GsGetWorkBase();
    setlen(poly, 4);
    setcode(poly, 0x20);
    SetSemiTrans(poly, 1);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[6]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[7]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 4);
    setcode(poly, 0x20);
    SetRotMatrix((s32)&GsWSMATRIX);
    SetTransMatrix(&GsWSMATRIX);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        GsSetWorkBase((long)(poly + 1));
    }
}
