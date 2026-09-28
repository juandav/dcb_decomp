#include "common.h"
#include "game.h"
#include "dcb/scene3d.h"
#include "dcb/model_anim.h"
#include "dcb/wire_grid.h"
#include "dcb/frame_callback.h"
#include "dcb/heap.h"
#include "dcb/model_load.h"
#include "dcb/anim_control.h"
#include "dcb/task.h"
#include "dcb/stage.h"
#include "dcb/player_data.h"
#include "dcb/card_db.h"
#include "dcb/window.h"
#include "dcb/prim_util.h"
#include "dcb/text.h"
#include "dcb/fade.h"
#include "dcb/decompress.h"
#include "dcb/effect_object.h"
#include "dcb/battle_hud.h"
#include "dcb/transform.h"

typedef struct {
    s16 *win;
    u8 pad4[0x14 - 0x4];
    s16 unk14;
    u8 pad16[0x26 - 0x16];
    u8 unk26;
} EvoList;
extern EvoList D_801F0030;
void func_801EE2DC();
void func_801EB2A0(void);
extern UiWindow D_801F51B8;

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
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
} PolyG3;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
    u8 r3, g3, b3, pad3;
    s16 x3, y3;
} PolyG4;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
} PolyF3;
extern void (*D_801F02A8[])(s32);

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
typedef struct {
    s16 r;
    s16 g;
    s16 b;
} EvoColor;
extern TmdPacketFT3 *D_801F0540;
extern SVECTOR *D_801F0538;
extern EvoColor D_801F0530;
extern MATRIX D_801DBEA0;
long func_80062C44(void);
void func_80062C34(long base);
typedef struct {
    s16 first;
    s16 last;
} EvoRange;
typedef struct {
    u8 pad0[0x18];
    u8 unk18;
    u8 level;
} EvoCardInfo;
extern EvoRange D_801EFF00[];
extern EvoCardInfo *D_801F4980[];
typedef struct {
    Rect16 rect;
    u16 src[0x100];
    u16 dst[0x100];
    s16 brighten;
    s16 level;
} EvoClut;

void func_801E0618(s32 arg);
void D_801E2E30();
void D_801E00F4();
typedef struct {
    s16 x;
    s16 y;
} EvoPose;
typedef struct {
    u8 pad0[0x10];
} EvoPart;
typedef struct {
    u8 pad0[4];
    s16 partCount;
    u8 pad6[0xB80 - 0x6];
    EvoPart parts[320];
    EvoPose *pose;
} EvoModel;
void func_801DF7EC(s16 slot);
void func_801E08D4(EvoPart *part, s32 arg);
extern s16 D_80079584;
void D_801DFC18();
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
} FlatLight;
s32 GsSetFlatLight(s32 id, FlatLight *light);
extern u8 *D_801F4E34;
extern u8 *D_801F4E40[3];
typedef struct {
    u8 pad0[0xAC];
    s16 unkAC;
    s16 unkAE;
    u8 padB0[0xBB - 0xB0];
    u8 unkBB;
    u8 padBC[0xC1 - 0xBC];
    u8 unkC1;
    u8 unkC2;
    u8 unkC3;
    u8 unkC4;
    u8 padC5;
    u8 unkC6;
    u8 padC7[0xCD - 0xC7];
    u8 unkCD;
    u8 unkCE;
} EvoMenu;
typedef struct {
    u8 pad0[0x124];
    s16 unk124;
    u8 pad126[0x12C - 0x126];
} EvoScene;
extern EvoMenu D_801F5478;
extern EvoScene D_801F5548[2];
extern u8 D_801F0056;
extern UiWindow D_801F5008;
extern UiWindow D_801F5050;
extern Rect16 D_801F0168;
extern Rect16 D_801F0184;
typedef struct {
    u32 tag;
    u32 code[1];
} DrTPage;
typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} PolyF4;
typedef struct {
    DrTPage tpage[2];
    PolyF4 poly[2];
    s16 unk40;
    u8 unk42;
} EvoFade;
extern EvoFade D_801F53C8;
void D_801E7F64();
typedef struct {
    s32 unk0;
    s32 *unk4;
    s32 *state;
} EvoScript;
s32 func_801E81C4(EvoScript *script);
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 *vars;
} EvoScriptData;
extern EvoScriptData *D_801F5250;
typedef struct {
    u8 pad0[0x3E];
    s8 active;
    u8 pad3F;
} EvoSlot;
typedef struct {
    u8 text[0x3C];
    s16 pos;
    u8 pad3E;
    s8 len;
} EvoText;
extern UiWindow D_801F5128;
extern Rect16 D_801F01D8;
extern UiWindow D_801F43D0;
extern Rect16 D_801F0038;
void func_801ECC6C(void);
extern u8 D_801F553F;
extern u8 D_801F59A4;
extern u8 D_801F59A5;
extern u8 D_801F59A6;
extern s32 D_801F59A8;
void D_801EF7DC();
typedef struct {
    u8 pad0[0x118];
    s32 unk118;
    s32 unk11C;
} EvoFx;
typedef struct {
    s16 kind;
    s16 active;
    EvoFx *handle;
} EvoEntry;
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 *vars;
    EvoEntry entries[16];
    s32 count;
    void *buffer;
} EvoLoader;
extern u8 D_801F5868[];
extern void (*D_801F0280[])(EvoFx *);
void func_801EE69C(EvoLoader *loader);
typedef struct {
    u8 pad[0x260];
    s32 x;
    s32 y;
    s32 z;
} EvoObject;
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
} Color;
typedef struct {
    u8 pad0[0x98];
    s32 r;
    s32 g;
    s32 b;
} EvoLight;
void func_801F893C(void *sprite, Color *color);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801DF594);

void func_801DF658(s8 evolved) {
    if (evolved == 0) {
        func_801E0618(1);
        createWireGrid(3000, 3000, 11, 11, 1, 0);
    } else {
        initScene3D(1);
        addFrameCallback((s32)renderWireGrid);
    }
    GRID_VISIBLE = 0;
    func_80014A00(0x19);
    func_800149B8(0x19, 0x1F, 0, 0x800, &runSceneCameraTask, 1);
    func_80014A00(0x1B);
    func_800149B8(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
}

void func_801DF724(s8 evolved) {
    func_80014A00(0x1B);
    func_80014A00(0x19);
    removeFrameCallback((s32)renderWireGrid);
    removeFrameCallback((s32)D_801E2E30);
    if (evolved == 0) {
        removeFrameCallback((s32)D_801E00F4);
    } else {
        removeFrameCallback((s32)renderSceneModels);
    }
    func_80014C08(1);
    unloadAllModels();
    GRID_VISIBLE = 0;
    freeHeapBlocksByTag(0x7F);
    if (evolved == 0) {
        freeHeapBlocksByTag(0x1F5);
        freeHeapBlocksByTag(0x41);
    } else {
        freeHeapBlocksByTag(0x84);
    }
    freeHeapBlocksByTag(0x1F4);
    freeHeapBlocksByTag(0x40);
}

void func_801DF7EC(s16 slot) {
    applyAnimationFirstFrame(slot, 0);
    startModelAnimation(slot, 0, -2, 0);
}

void func_801DF830(void) {
    Graphics *camera;
    EvoModel *model;
    EvoPart *part;
    s32 i;

    camera = (Graphics *)&GRAPHICS;
    camera->snapCamera = 1;
    camera->unk90 = 3000;
    camera->unk92 = -((EvoModel *)SCENE_3D->models[0])->pose->y * 3;
    camera->targetModel = 0;
    SCENE_3D->modelState[0] = 1;
    SCENE_3D->modelState[1] = -1;
    GRID_VISIBLE = 1;
    func_801DF7EC(0);
    model = SCENE_3D->models[1];
    for (i = 0, part = model->parts; i < model->partCount; i++, part++) {
        func_801E08D4(part, 1);
    }
}

void func_801DF914(s32 id) {
    loadDigimonModelPak(0, id, 0, 0);
    func_80014C08(2);
    func_80014C08(20);
    setModelAnimationPose(0, 0);
    D_80079584 = 0;
    func_80014C08(1);
    SCENE_3D->modelState[0] = 1;
    playModelAnimation(0, 0);
    GRID_VISIBLE = 1;
    func_800149B8(0, -1, 0, 0x400, D_801DFC18, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(10);
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801DF9D0);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801DFEC0);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E00A4);

void func_801E0488(s32 projection) {
    func_8005C484(160, 120);
    func_8005C4A4(projection);
    func_80062484(projection);
    SCENE_3D->view.vpx = 0;
    SCENE_3D->view.vpy = 0;
    SCENE_3D->view.vpz = 0;
    SCENE_3D->view.vrx = 0;
    SCENE_3D->view.vry = 0;
    SCENE_3D->view.vrz = 0;
    SCENE_3D->view.rz = 0;
    SCENE_3D->view.super = NULL;
    GsSetRefView2(&SCENE_3D->view);
}

void func_801E04F8(void) {
    ((FlatLight *)SCENE_3D->unkE4)[0].vx = 0;
    ((FlatLight *)SCENE_3D->unkE4)[0].vy = -100;
    ((FlatLight *)SCENE_3D->unkE4)[0].vz = 100;
    ((FlatLight *)SCENE_3D->unkE4)[0].r = 0xFF;
    ((FlatLight *)SCENE_3D->unkE4)[0].g = 0xFF;
    ((FlatLight *)SCENE_3D->unkE4)[0].b = 0xFF;
    GsSetFlatLight(0, &((FlatLight *)SCENE_3D->unkE4)[0]);
    ((FlatLight *)SCENE_3D->unkE4)[1].vx = 0;
    ((FlatLight *)SCENE_3D->unkE4)[1].vy = 100;
    ((FlatLight *)SCENE_3D->unkE4)[1].vz = 100;
    ((FlatLight *)SCENE_3D->unkE4)[1].r = 0x80;
    ((FlatLight *)SCENE_3D->unkE4)[1].g = 0x80;
    ((FlatLight *)SCENE_3D->unkE4)[1].b = 0x80;
    GsSetFlatLight(1, &((FlatLight *)SCENE_3D->unkE4)[1]);
    ((FlatLight *)SCENE_3D->unkE4)[2].vx = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].vy = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].vz = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].r = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].g = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].b = 0;
    GsSetFlatLight(2, &((FlatLight *)SCENE_3D->unkE4)[2]);
    GsSetAmbient(0x40, 0x40, 0x40);
    func_8005C464(0x30, 0x30, 0x40);
    GsSetLightMode(0);
}

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DDF38);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DDF3C);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DDF80);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DDFC0);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E0618);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E08D4);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E0AFC);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E285C);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E29AC);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E2C0C);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E2FC8);

void func_801E335C(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = D_801F0540;
    poly = (POLY_FT3 *)func_80062C44();
    func_80067724(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
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
    vert = &D_801F0538[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[5]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E3694(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = D_801F0540;
    poly = (POLY_FT3 *)func_80062C44();
    func_80067724(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
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
    vert = &D_801F0538[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[2]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E39CC(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
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

    prim = D_801F0540;
    poly = (POLY_FT4 *)func_80062C44();
    func_800677A4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
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
    vert = &D_801F0538[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[5]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[7]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E3D6C(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
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

    prim = D_801F0540;
    poly = (POLY_FT4 *)func_80062C44();
    func_800677A4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
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
    vert = &D_801F0538[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[2]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[4]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E410C(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = D_801F0540;
    poly = (POLY_FT3 *)func_80062C44();
    func_80067724(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
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
    vert = &D_801F0538[prim->idx[2]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[4]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E4444(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
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

    prim = D_801F0540;
    poly = (POLY_FT4 *)func_80062C44();
    func_800677A4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
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
    vert = &D_801F0538[prim->idx[2]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[4]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim->idx[5]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E47E4);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E5024);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E5244);

void func_801E55FC(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
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

    prim = D_801F0540;
    poly = (PolyF4 *)func_80062C44();
    setlen(poly, 5);
    setcode(poly, 0x28);
    SetSemiTrans(poly, 1);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[6]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[7]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[8]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 5);
    setcode(poly, 0x28);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E5934(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
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
    PolyG3 *poly;

    prim = D_801F0540;
    poly = (PolyG3 *)func_80062C44();
    setlen(poly, 6);
    setcode(poly, 0x30);
    SetSemiTrans(poly, 1);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[7]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[9]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 6);
    setcode(poly, 0x30);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E5C1C(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
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

    prim = D_801F0540;
    poly = (PolyG4 *)func_80062C44();
    setlen(poly, 8);
    setcode(poly, 0x38);
    SetSemiTrans(poly, 1);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[7]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[9]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[11]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 8);
    setcode(poly, 0x38);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void func_801E5F54(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
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
    PolyF3 *poly;

    prim = D_801F0540;
    poly = (PolyF3 *)func_80062C44();
    setlen(poly, 4);
    setcode(poly, 0x20);
    SetSemiTrans(poly, 1);
    poly->r0 = D_801F0530.r;
    poly->g0 = D_801F0530.g;
    poly->b0 = D_801F0530.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &D_801F0538[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[6]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &D_801F0538[prim[7]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 4);
    setcode(poly, 0x20);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DE084);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF03C);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E623C);

void func_801E62D0(void) {
    Graphics *camera;

    initScene3D(1);
    func_800149B8(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    func_80014A00(0x1B);
    func_800149B8(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
    func_80014C08(2);
    camera = (Graphics *)&GRAPHICS;
    camera->rotX = 0;
    camera->rotY = 0;
    camera->rotZ = 0;
    camera->posX = 0;
    camera->posY = 0;
    camera->posZ = 0;
    camera->unk8E = 0;
    camera->unk90 = 20;
    camera->unk92 = 0;
    camera->unk94 = 0;
    camera->targetModel = -1;
    camera->snapCamera = 1;
    func_80014C08(2);
}

void func_801E6398(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 0x12D; i++) {
        D_801F4E34[i] = getOwnedCardCount(0, i);
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 0x12D; j++) {
            D_801F4E40[i][j] = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].inUse != 0) {
            for (j = 0; j < 30; j++) {
                D_801F4E40[i][getCardId(((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].type,
                                        ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].index)]++;
            }
        }
    }
    for (i = 0; i < 0x12D; i++) {
        for (j = 1; j < 3; j++) {
            if (D_801F4E40[0][i] < D_801F4E40[j][i]) {
                D_801F4E40[0][i] = D_801F4E40[j][i];
            }
        }
    }
    for (i = 0; i < 0x12D; i++) {
        D_801F4E34[i] -= D_801F4E40[0][i];
    }
}

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF13C);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E65A8);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF15C);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF168);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF16C);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF174);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF180);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF190);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF1A8);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF1C0);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF1E0);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF204);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF228);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E780C);

void func_801E7B8C(void) {
    D_801F5548[0].unk124 += 10;
    if (D_801F5548[0].unk124 >= 15) {
        D_801F5548[0].unk124 = 14;
        D_801F5478.unkC1 = 3;
        D_801F0056 = 1;
        D_801F5478.unkBB = 0;
    }
}

void func_801E7BE4(void) {
    D_801F5548[0].unk124 -= 10;
    if (D_801F5548[0].unk124 < -0x58) {
        D_801F5548[0].unk124 = -0x58;
        D_801F5478.unkC1 = 0;
        D_801F5250->vars[8] = -1;
    }
}

void func_801E7C3C(void) {
    D_801F5548[0].unk124 -= 10;
    if (D_801F5548[0].unk124 < -0x78) {
        D_801F5548[0].unk124 = -0x78;
        D_801F5548[1].unk124 += 10;
        if (D_801F5548[1].unk124 >= 15) {
            D_801F5548[1].unk124 = 14;
            D_801F5478.unkC1 = 3;
            D_801F0056 = 1;
        }
    }
}

void func_801E7CB4(void) {
    D_801F5548[1].unk124 -= 10;
    if (D_801F5548[1].unk124 < -0x78) {
        D_801F5548[1].unk124 = -0x78;
        D_801F5548[0].unk124 += 10;
        if (D_801F5548[0].unk124 >= 15) {
            D_801F5548[0].unk124 = 14;
            D_801F5478.unkC1 = 3;
            D_801F0056 = 1;
        }
    }
}

void func_801E7D2C(s32 active) {
    if (active != 0) {
        D_801F5548[0].unk124 -= 10;
        D_801F5548[1].unk124 -= 10;
        if (D_801F5548[0].unk124 < -0x58) {
            D_801F5548[0].unk124 = -0x58;
        }
        if (D_801F5548[1].unk124 < 14) {
            D_801F5548[1].unk124 = 14;
        }
        if (D_801F5548[0].unk124 == -0x58 && D_801F5548[1].unk124 == 14) {
            D_801F5478.unkC4 = 2;
            D_801F5250->vars[8] = -1;
            D_801F5478.unkBB = 0;
            D_801F5478.unkC1 = 0;
            D_801F4E34[D_801F5478.unkAE]++;
        }
    }
}

void func_801E7E10(s8 mode) {
    if (mode == 0) {
        animateWindowTo(&D_801F5008, &D_801F0168);
        animateWindowTo(&D_801F5050, &D_801F0184);
    } else if (mode == 1) {
        animateWindowTo(&D_801F5008, (Rect16 *)-1);
        animateWindowTo(&D_801F5050, (Rect16 *)-1);
    }
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E7E8C);

void func_801E80E4(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_80067784(&D_801F53C8.poly[i]);
        SetSemiTrans(&D_801F53C8.poly[i], 1);
        setPrimQuadRect(&D_801F53C8.poly[i], 0, 0, 320, 240);
        SetDrawTPage(&D_801F53C8.tpage[i], 0, 0, 0x20);
        D_801F53C8.poly[i].b0 = 0;
        D_801F53C8.poly[i].g0 = 0;
        D_801F53C8.poly[i].r0 = 0;
    }
    D_801F53C8.unk42 = 0;
    D_801F53C8.unk40 = 0;
    addFrameCallback((s32)D_801E7F64);
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E81C4);

s32 func_801E8614(EvoScript *script) {
    *script->state = 1;
    func_801E81C4(script);
    return *script->state;
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E8650);

s32 *func_801E86B4(s32 count) {
    s32 *flags;
    s32 *p;
    s32 i;

    flags = allocHeapBlock(count * 4, 0x2C);
    p = flags;
    for (i = 0; i < count; i++) {
        *p++ = 0;
    }
    return flags;
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E8708);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E87A8);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E8BD8);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E8C74);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E8CD8);

void func_801E8DC0(void) {
    s32 i;
    s32 bit;
    PlayerProfile *profile;

    for (i = 20, bit = 0, profile = (PlayerProfile *)PLAYER_PROFILES; i < 30; i++, bit++) {
        if ((1 << bit) & profile->unk2C) {
            D_801F5250->vars[i] = 1;
        }
    }
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E8E1C);

void func_801E9390(EvoSlot *slot) {
    s32 i;

    for (i = 0; i < 4; i++) {
        slot->active = 0;
        slot++;
    }
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E93B0);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E943C);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF388);

s32 func_801E97E4(s32 x, s32 y, EvoText *t, s32 z) {
    u8 buf[64];
    u8 *dst;
    u8 *src;
    s8 i;
    u8 c;

    dst = buf;
    if (t->len == t->pos) {
        drawText(x, y, (s32)t, 7, z);
        return -1;
    }
    src = t->text;
    for (i = 0; i < t->pos; i++) {
        *dst++ = *src++;
    }
    c = *src;
    if (c == 0) {
        return 1;
    }
    if (*src < 0x81 || *src > 0x98) {
        if (c == '*') {
            switch (src[1]) {
            case 'a':
            case 'b':
            case 'c':
            case 'e':
            case 's':
            case 'w':
                if (src[2] >= '0' && src[2] <= '9') {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    t->pos += 2;
                }
                break;
            }
        }
        *dst = *src;
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        t->pos += 1;
    } else {
        *dst++ = src[0];
        *dst = src[1];
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        t->pos += 2;
    }
    return 1;
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E99A0);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801E9C88);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EA110);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EA2D4);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EA414);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EA530);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EA598);

void func_801EA790(void) {
    animateWindowTo(&D_801F5128, (Rect16 *)-1);
    animateWindowTo(&D_801F43D0, &D_801F0038);
    D_801F0030.unk26 = 1;
    D_801F5478.unkC2 = 0;
    D_801F5478.unkC1 = 3;
    D_801F5478.unkC3 = 0;
    D_801F0030.unk14 = 0;
    scrollWindowTo(D_801F0030.win, 0, 0);
    D_801F5250->vars[8] = -1;
}

void func_801EA820(void) {
    animateWindowTo(&D_801F5128, &D_801F01D8);
    animateWindowTo(&D_801F43D0, (Rect16 *)-1);
    D_801F0056 = 0;
}

void func_801EA864(s32 index) {
    EvoScene *scenes = D_801F5548;

    scenes[index].unk124 -= 10;
    if (scenes[index].unk124 < -0x58) {
        scenes[index].unk124 = -0x58;
        D_801F5478.unkC1 = 0;
        D_801F5478.unkBB = 0;
        D_801F5250->vars[8] = -1;
        if (index == 0) {
            D_801F4E34[D_801F5478.unkAC]--;
        } else {
            D_801F4E34[D_801F5478.unkAC]++;
        }
    }
}

void func_801EA934(s32 index) {
    EvoScene *scenes = D_801F5548;

    scenes[index].unk124 += 10;
    if (scenes[index].unk124 >= 15) {
        scenes[index].unk124 = 14;
        D_801F5478.unkC1 = 0;
        D_801F5478.unkBB = 0;
        D_801F5250->vars[8] = -1;
    }
}

void func_801EA9AC(void) {
    D_801F5548[0].unk124 += 10;
    D_801F5548[1].unk124 += 10;
    if (D_801F5548[0].unk124 >= 15) {
        D_801F5548[0].unk124 = 14;
    }
    if (D_801F5548[1].unk124 >= 0x67) {
        D_801F5548[1].unk124 = 0x66;
    }
    if (D_801F5548[0].unk124 == 14 && D_801F5548[1].unk124 == 0x66) {
        D_801F5478.unkC1 = 0;
        D_801F5250->vars[8] = 1;
    }
}

void func_801EAA5C(void) {
    D_801F5548[0].unk124 -= 10;
    D_801F5548[1].unk124 -= 10;
    if (D_801F5548[1].unk124 < 14) {
        D_801F5548[0].unk124 = -0x58;
        D_801F5548[1].unk124 = 14;
        D_801F5478.unkC1 = 0;
        D_801F5478.unkBB = 0;
        animateWindowTo(&D_801F43D0, &D_801F0038);
        func_801ECC6C();
    }
}

void func_801EAAE8(s16 cardId) {
    if (cardId < 0) {
        cardId = -1;
    }
    if (D_801F5250->vars[13] != 0) {
        D_801F5250->vars[11] = 0;
        return;
    }
    D_801F5250->vars[11] = getOwnedCardCount(0, cardId);
    if ((((PlayerProfile *)PLAYER_PROFILES)->cardCollection[cardId] & 7) + 1 >= 7) {
        D_801F5250->vars[11] = -1;
    } else {
        D_801F5250->vars[11] = 0;
    }
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EABB0);

void func_801EB0F4(void) {
    PlayerProfile *profile;

    D_801F5250->vars[8] = -2;
    D_801F5250->vars[18] = 0;
    D_801F5250->vars[19] = 0;
    D_801F5250->vars[7] = 1;
    func_801EE2DC(0, 0);
    func_801EB2A0();
    D_801F5478.unkC1 = 4;
    removeCardFromCollection(0, D_801F5478.unkAE, 1);
    D_801F4E34[D_801F5478.unkAE]--;
    profile = (PlayerProfile *)PLAYER_PROFILES;
    profile->unk52++;
    if ((u16)profile->unk52 >= 10000) {
        profile->unk52 = 9999;
    }
}

void func_801EB1D0(void) {
    D_801F5250->vars[8] = -1;
    D_801F0056 = 1;
    D_801F5478.unkC3 = 0;
    animateWindowTo(&D_801F51B8, (Rect16 *)-1);
    D_801F5478.unkC1 = 3;
}

void func_801EB234(void) {
    setScreenFadeParams(0, 2, 6);
    func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 1, 6, 0);
    D_801F553F = 1;
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EB2A0);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EB440);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EB670);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EBA08);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EBD04);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EBD64);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EBE08);

void func_801ECBE8(void) {
    D_801F5478.unkAE = -1;
    D_801F5478.unkAC = -1;
    D_801F5478.unkC6 = 1;
    D_801F5478.unkBB = 4;
    D_801F5478.unkC2 = 0;
    D_801F5478.unkC3 = 0;
    D_801F5478.unkC4 = 1;
    D_801F5478.unkCD = 0;
    D_801F5478.unkCE = 0;
}

void func_801ECC24(void) {
    D_801F5478.unkC4 = 1;
    D_801F5478.unkC1 = 8;
    animateWindowTo(&D_801F43D0, (Rect16 *)-1);
    D_801F0056 = 0;
}

void func_801ECC6C(void) {
    D_801F5478.unkC4 = 2;
    D_801F5250->vars[8] = -1;
    D_801F5478.unkC1 = 7;
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801ECCA0);

void func_801ED5F4(u8 type, u8 level, s16 *out, s16 excludeA, s16 excludeB) {
    s16 candidates[10];
    s16 i;
    s16 j;
    s16 tmp;
    s8 count;

    *out = -1;
    if (level < 2) {
        level = 2;
    }
    if (level >= 36) {
        level = 35;
    }
    count = 0;
    for (i = 0; i < 10; i++) {
        candidates[i] = -1;
    }
    if (type == 5) {
        for (i = D_801EFF00[5].first; i <= D_801EFF00[5].last; i++) {
            if (D_801F4980[i]->level == level && i != excludeA && i != excludeB) {
                candidates[count] = i;
                count++;
            }
        }
        for (i = D_801EFF00[6].first; i <= D_801EFF00[6].last; i++) {
            if (D_801F4980[i]->level == level && i != excludeA && i != excludeB) {
                candidates[count] = i;
                count++;
            }
        }
        for (i = 0; i < count; i++) {
            j = rand() % count;
            tmp = candidates[i];
            candidates[i] = candidates[j];
            candidates[j] = tmp;
        }
        *out = candidates[0];
    } else {
        for (i = D_801EFF00[type].first; i <= D_801EFF00[type].last; i++) {
            if (D_801F4980[i]->level == level) {
                *out = i;
                return;
            }
        }
    }
}

void func_801ED8B0(EvoClut *clut, u16 flags) {
    u16 *src;
    u16 *dst;
    s32 brighten;
    s32 level;
    s32 i;
    s32 color;
    s32 r;
    s32 g;
    s32 b;

    brighten = clut->brighten;
    level = clut->level;
    src = clut->src;
    dst = clut->dst;
    for (i = 0; i < clut->rect.w * clut->rect.h; i++) {
        color = *src++;
        if (color != 0) {
            r = color & 0x1F;
            g = (color >> 5) & 0x1F;
            b = (color >> 10) & 0x1F;
            if (brighten == 0) {
                r = r * level / 255;
                g = g * level / 255;
                b = b * level / 255;
            } else {
                r += (31 - r) * level / 255;
                g += (31 - g) * level / 255;
                b += (31 - b) * level / 255;
            }
            if (r < 0) {
                r = 0;
            } else if (r >= 32) {
                r = 31;
            }
            if (g < 0) {
                g = 0;
            } else if (g >= 32) {
                g = 31;
            }
            if (b < 0) {
                b = 0;
            } else if (b >= 32) {
                b = 31;
            }
            color = flags | (color & 0x8000) | (b << 10) | (g << 5) | r;
        }
        *dst++ = color;
    }
    LoadImage((s16 *)&clut->rect, (s32)clut->dst);
    DrawSync(0);
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EDAE8);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EDF48);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF534);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF53C);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF540);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF548);

INCLUDE_RODATA("asm/evoseg/nonmatchings/evoseg", D_801DF550);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EE1F4);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EE248);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EE2DC);

void func_801EE330(s32 index) {
    s32 data;

    D_801F59A4 = 0;
    D_801F59A5 = 0;
    D_801F59A6 = 0;
    data = decompressArchiveEntry(D_801F59A8, index);
    func_800149B8(0, 0x1F, 0, 0x800, D_801EF7DC, data, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    freeHeapBlock((void *)data);
}

s32 func_801EE3BC(EvoLoader *loader) {
    s32 i;

    PushMatrix();
    tickEffectMotion((s32)D_801F5868, 0);
    PopMatrix();
    loader->vars[0] = 1;
    func_801EE69C(loader);
    for (i = 0; i < 16; i++) {
        if (loader->entries[i].active != 0 && D_801F0280[loader->entries[i].kind] != NULL) {
            D_801F0280[loader->entries[i].kind](loader->entries[i].handle);
            if (loader->entries[i].kind > 0) {
                loader->vars[i + 88] = loader->entries[i].handle->unk118;
                loader->vars[i + 120] = loader->entries[i].handle->unk11C;
            }
        }
    }
    return loader->vars[0];
}

void func_801EE4E8(EvoLoader *loader) {
    s32 i;

    func_80014C08(FRAME_INTERVAL);
    for (i = 0; i < 16; i++) {
        if (loader->entries[i].kind != -1) {
            D_801F02A8[loader->entries[i].kind](loader->entries[i].handle);
        }
    }
    func_80014C08(FRAME_INTERVAL);
    if (loader->buffer != NULL) {
        freeHeapBlock(loader->buffer);
    }
}

void func_801EE5B0(s32 kind, EvoObject *obj) {
    switch (kind) {
    case 0:
        obj->x = 28;
        obj->y = -13;
        obj->z = 0;
        break;
    case 1:
        obj->x = -106;
        obj->y = -13;
        obj->z = 0;
        break;
    case 2:
        obj->x = -12;
        obj->y = -21;
        obj->z = 0;
        break;
    case 3:
        obj->x = -90;
        obj->y = -21;
        obj->z = 0;
        break;
    case 4:
        obj->x = -59;
        obj->y = -21;
        obj->z = 0;
        break;
    default:
        obj->x = 0;
        obj->y = 0;
        obj->z = 0;
        break;
    }
}

void func_801EE638(s32 index, EvoLight *light) {
    Color color;

    if (index >= 0) {
        color.r = light->r;
        color.g = light->g;
        color.b = light->b;
        func_801F893C(SPRITE(index), &color);
    }
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EE69C);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EED18);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EEF24);

void func_801EF0BC(void *xform, EvoObject *obj) {
    SVECTOR pos;

    getTransformWorldPos(xform, &pos);
    obj->x = pos.vx;
    obj->y = pos.vy;
    obj->z = pos.vz;
}

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EF108);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EF5C8);

INCLUDE_ASM("asm/evoseg/nonmatchings/evoseg", func_801EF65C);
