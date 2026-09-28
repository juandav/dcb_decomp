#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/model_load.h"
#include "dcb/effect_object.h"
#include "dcb/archive.h"
#include "dcb/loader.h"
#include "dcb/task.h"
#include "dcb/vram_upload.h"
#include "dcb/transform.h"
#include "dcb/script.h"
#include "dcb/vblank.h"
#include "dcb/frame_callback.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
#include "dcb/anim_control.h"
#include "dcb/player_data.h"

typedef struct {
    u8 unk0[0x26D4];
    s32 unk26D4;
} Unk801E3C2C_13C;
typedef struct {
    u8 unk0[0x139];
    u8 unk139;
    u8 unk13A[2];
    Unk801E3C2C_13C *unk13C;
    u8 unk140[0x20];
    u8 unk160[0x40A];
    s16 brightness;
    s16 lastBrightness;
    s8 modelSlot;
    u8 unk56F;
    s8 unk570;
    s8 unk571;
} Unk801E3C2C;
extern MATRIX D_801EF25C;
extern MATRIX D_801EF27C;
typedef struct {
    u8 unk0[0x130];
    u32 animId;
    u8 unk134[0x1C];
    s32 pixelX;
    s32 pixelY;
    u8 unk158[8];
    s32 clutX;
    s32 clutY;
} Unk801E8168;
typedef struct {
    u8 unk0[0x98];
    void *parent;
    u8 unk9C[0x10];
    s32 unkAC[3];
    u8 unkB8[4];
    s32 unkBC[3];
    u8 unkC8[4];
    s16 unkCC[3];
    u8 unkD2[2];
    s16 unkD4[3];
    u8 unkDA[2];
    s16 unkDC[3];
    u8 unkE2[2];
    s16 unkE4[3];
    u8 unkEA[2];
    s16 unkEC[3];
    u8 unkF2[2];
    s16 unkF4[3];
    u8 unkFA[0x32];
    s16 unk12C;
    s16 unk12E;
    s16 unk130;
    u8 unk132[5];
    u8 unk137;
    u8 unk138[4];
} CameraEffect;
extern CameraEffect D_801EF808;
typedef struct {
    u8 unk0[0x65C];
    s32 unk65C[3];
    s32 unk668[3];
    u8 unk674[8];
    s32 unk67C;
    s32 unk680;
    s32 unk684;
    u8 unk688[0x18];
    s32 unk6A0[3];
    s32 unk6AC[3];
} Unk801E9FE0;
void func_801E76A8(VECTOR *a, VECTOR *b, VECTOR *c, VECTOR *d, s32 a4, s32 a5, s32 a6);

typedef struct {
    s32 v[4];
} Quad;
typedef struct {
    s16 v[4];
} Short4;
typedef struct {
    Quad *quads;
    Short4 *shorts;
    s32 unk8;
    s32 unkC;
    s32 count;
} Table;
typedef struct {
    u8 unk0[0x26D4];
    s32 unk26D4;
} Unk801E3D2C_13C;
typedef struct {
    u8 unk0[0x13C];
    Unk801E3D2C_13C *unk13C;
    u8 unk140[0x20];
    u8 unk160[0x40A];
    s16 unk56A;
    u8 unk56C[2];
    s8 modelSlot;
} Unk801E3D2C;
void func_801E3668(void *a0, s32 a1);
void func_801E3D94(s32 a0);
typedef struct {
    u8 unk0[0x139];
    u8 unk139;
    u8 unk13A[0x8C];
    u8 unk1C6;
    u8 unk1C7;
    s32 brightness;
    s32 unk1CC;
} Effect;
extern MATRIX D_801DBEC0;
void func_801EB874(void *a0, s16 brightness);
typedef struct {
    u8 unk0[0x13C];
    u8 unk13C[0x20];
    void *unk15C;
    void *unk160[2];
    void *unk168[2];
    void *unk170[2];
    void *unk178[2];
    void *unk180[2];
    u8 unk188[0x98];
    void *unk220[2];
    u8 unk228[0xA8];
    s32 unk2D0;
} Unk801E5144;
void func_801DE40C(void **obj);
void func_801E72D4(u8 *obj);
void func_801E1EE8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8);
extern s32 D_801EF7A0[];
extern s32 D_801EF7A4[];
typedef struct {
    Rect16 rect;
    u8 unk8[0x14];
    u8 *pixels;
} Image;
s32 LoadImage2(Image *image, u8 *pixels);
typedef struct {
    s16 kind;
    u8 unk2[2];
    u8 *source;
} Unk801E7BEC;
typedef struct {
    u8 unk0[0x13C];
    s32 value;
} Unk801E7BEC_Dst;
typedef struct {
    s16 kind;
    u8 unk2[2];
    u8 *target;
} Unk801E7C94;
typedef union {
    s32 w;
    u8 b;
    s16 h;
} Value;
typedef struct {
    u8 unk0[0x13C];
    Value value;
} Unk801E7C94_Src;
typedef struct {
    s16 id;
    s16 unk2;
    s32 unk4;
} Slot;
typedef struct {
    Slot slots[150];
    u8 unk4B0[0x58];
    s32 unk508;
    s32 unk50C;
} Slots;
typedef struct {
    u8 unk0[0xC];
    Slots *slots;
    s32 unk10;
    s32 unk14;
} Unk801E8470;
void func_801EA174(void *obj);
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 cd;
} Color;
void func_801E6424(SVECTOR *pos, Color *from, Color *to, u8 a3, s16 a4, u8 a5);
void func_801E66D0(SVECTOR *pos, s32 a1, s32 a2, s16 a3);
void func_801E9494(EffectTemplate *template, s32 a1, s32 a2);
void func_801E864C(void *obj);
typedef struct {
    u8 unk0[0x508];
    Chunk *pak;
} Unk801EA410_C;
typedef struct {
    u8 unk0[0xC];
    Unk801EA410_C *unkC;
} Unk801EA410;
extern s32 D_801EF7F8;
extern u8 D_801EF800[3];
extern u8 D_801EF804[3];
void func_801EA574(void);
extern s32 D_801EF7E4;
extern s16 D_801EF2A0;
extern s32 D_801EF7FC;
extern s32 D_801EF29C;
extern s32 D_801EF7F0;
extern s32 D_801EF7F4;
void func_801EA7BC(u8 r, u8 g, u8 b, s32 a3, s32 a4);
typedef struct {
    s32 key;
    s32 used;
    s32 subKey;
} Entry;
extern Entry *D_801EF950;
void func_801EBBFC(s32 a0, s32 a1, s32 a2, s32 a3);
typedef struct {
    float unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8[6];
    s16 flags;
    s16 unk16;
} Unk801EC160;
void func_801EBFA0(s16 *dst, s32 count, s16 value);
extern s16 D_801EF37C;
extern s32 D_801EF978[2];
void func_801EB3EC(void);
void func_801E6B40(void);
void func_801EDC4C(void);
void func_801E8358(void);
void func_801EBE40(void);
void func_801E723C(void);
void func_801E40B0(void);
extern s16 D_80079584;
void func_801EBE8C(void);
void func_801ECF20(s32 a0);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DE0C0);

void func_801DE1AC(Table *table, Quad *quad, Short4 *s4) {
    s32 i;

    for (i = 0; i < table->count; i++) {
        table->quads[i] = *quad;
        table->shorts[i] = *s4;
    }
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DE244);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DE2F8);

void func_801DE40C(void **obj) {
    obj[0] = (void *)freeHeapBlock(obj[0]);
    obj[1] = (void *)freeHeapBlock(obj[1]);
    freeHeapBlock(obj);
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DE454);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DE504);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DE55C);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DE5D8);

void func_801DE69C(void **obj) {
    obj[0] = (void *)freeHeapBlock(obj[0]);
    obj[1] = (void *)freeHeapBlock(obj[1]);
    freeHeapBlock(obj);
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DE6E4);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DF570);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DF598);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DF7E8);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801DFDA4);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E05F4);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E0F98);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E1960);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E1D80);

INCLUDE_RODATA("asm/sugseg/nonmatchings/sugseg", D_801DDF38);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E1EE8);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E2F70);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E31E4);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E3428);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E3668);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E38A0);

void func_801E3C2C(Unk801E3C2C *obj) {
    if (obj->unk571 != 0) {
        if (obj->unk139 != 0) {
            tickEffectStartDelay(obj);
            SCENE_3D->modelState[obj->modelSlot] = -1;
            return;
        }
        if (obj->unk13C->unk26D4 != -1) {
            obj->brightness = updateEffectBrightness(obj, obj->brightness);
            if (obj->brightness != obj->lastBrightness) {
                obj->lastBrightness = obj->brightness;
                func_801E3668(obj->unk160, 0x8000);
            }
            if (obj->brightness == 0) {
                SCENE_3D->modelState[obj->modelSlot] = -1;
                return;
            }
        }
        SCENE_3D->modelState[obj->modelSlot] = 3;
    }
    if (obj->unk570 >= 0) {
        func_801E7020(obj->unk140);
    }
}

void func_801E3D2C(Unk801E3D2C *obj) {
    unloadModel(obj->modelSlot);
    if (obj->unk13C->unk26D4 != -1 && obj->unk56A != 0xFF) {
        obj->unk56A = 0xFF;
        func_801E3668(obj->unk160, 0x8000);
    }
    freeHeapBlock(obj);
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E3D94);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E3EB8);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E3FA8);

void func_801E40B0(void) {
    func_801E3D94(0xFF);
    DB(1).draw.r0 = 0;
    DB(0).draw.r0 = 0;
    DB(1).draw.g0 = 0;
    DB(0).draw.g0 = 0;
    DB(1).draw.b0 = 0;
    DB(0).draw.b0 = 0;
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E40F0);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E41AC);

void func_801E42F4(Effect *fx) {
    PushMatrix();
    if (fx->unk1CC != 0) {
        if (fx->unk139 != 0 || (fx->brightness = updateEffectBrightness(fx, fx->brightness)) == 0) {
            PopMatrix();
            tickEffectStartDelay(fx);
            return;
        }
        tickEffectMotion((s32)fx, fx->unk1C6);
    } else {
        SetRotMatrix((s32)&D_801DBEC0);
        func_8005C444(&D_801DBEC0);
    }
    func_801EB874(&fx->unk0[0x13C], fx->brightness);
    PopMatrix();
}

void func_801E43A4(void *ptr) {
    freeHeapBlock(ptr);
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E43C4);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E4728);

void func_801E5144(Unk801E5144 *obj) {
    s32 i;

    for (i = 0; i < 2; i++) {
        freeHeapBlock(obj->unk168[i]);
        freeHeapBlock(obj->unk170[i]);
        freeHeapBlock(obj->unk178[i]);
        freeHeapBlock(obj->unk180[i]);
        freeHeapBlock(obj->unk160[i]);
        func_801DE40C(obj->unk220[i]);
    }
    if (obj->unk2D0 >= 0) {
        func_801E72D4(obj->unk13C);
    }
    freeHeapBlock(obj->unk15C);
    freeHeapBlock(obj);
}

void func_801E521C(u8 *obj, u8 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    obj[0x2DB] = a1;
    func_801E1EE8(*(s32 *)(obj + 0x15C), a1, 1, *(s32 *)(obj + 0x2C8), a2, a3, a4, a5, 0);
    *(s16 *)(obj + 0x2D6) = -1;
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E5278);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E57E0);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E5AB0);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E6424);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E651C);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E66D0);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E6814);

void func_801E6AF8(void **obj) {
    obj[3] = (void *)freeHeapBlock(obj[3]);
    obj[2] = (void *)freeHeapBlock(obj[2]);
    freeHeapBlock(obj);
}

void func_801E6B40(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        D_801EF7A0[i * 2] = -1;
        D_801EF7A4[i * 2] = 0;
    }
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E6B84);

INCLUDE_RODATA("asm/sugseg/nonmatchings/sugseg", D_801DDF50);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E6C78);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E7020);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E723C);

void func_801E72D4(u8 *obj) {
    void *ptr = *(void **)(obj + 0x1C);

    if (ptr != NULL) {
        freeHeapBlock(ptr);
    }
}

void func_801E7304(Image *image) {
    Image *self;
    u8 *row;
    u8 *p;
    s8 first;
    s32 x;
    s32 y;

    self = image;
    row = image->pixels;
    for (y = 0; y < image->rect.h; y++, row += image->rect.w * 2) {
        first = row[0];
        p = row;
        for (x = 0; x < image->rect.w * 2 - 1; x++) {
            p[0] = p[1];
            p++;
        }
        *p = first;
    }
    LoadImage2(image, self->pixels);
    DrawSync(0);
}

void func_801E73C0(Image *image) {
    Image *self;
    u8 *row;
    u8 *p;
    s8 first;
    s32 x;
    s32 y;

    self = image;
    y = 0;
    row = image->pixels;
    for (; y < image->rect.h; y++, row += image->rect.w * 2) {
        p = row + image->rect.w * 2 - 1;
        first = *p;
        for (x = 0; x < image->rect.w * 2 - 1; x++) {
            p[0] = p[-1];
            p--;
        }
        *p = first;
    }
    LoadImage2(image, self->pixels);
    DrawSync(0);
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E7480);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E7598);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E76A8);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E7880);

void func_801E7AF4(void *obj) {
    SCENE_LIGHT_MATRIX = D_801EF25C;
    SCENE_LIGHT_COLORS = D_801EF27C;
    freeHeapBlock(obj);
}

void func_801E7BEC(Unk801E7BEC *cmd, Unk801E7BEC_Dst *dst) {
    switch (cmd->kind) {
    case 10:
        dst->value = *(s32 *)(cmd->source + 0x1B4);
        break;
    case 12:
        dst->value = *(s32 *)(cmd->source + 0x2C4);
        break;
    case 13:
        dst->value = *(s32 *)(cmd->source + 0x194);
        break;
    case 16:
        dst->value = cmd->source[0x1BD];
        break;
    case 17:
        dst->value = *(s16 *)(cmd->source + 0x154);
        break;
    }
}

void func_801E7C94(Unk801E7C94 *cmd, Unk801E7C94_Src *src) {
    switch (cmd->kind) {
    case 10:
        *(s32 *)(cmd->target + 0x1B4) = src->value.w;
        break;
    case 12:
        *(s32 *)(cmd->target + 0x2C4) = src->value.w;
        break;
    case 13:
        *(s32 *)(cmd->target + 0x194) = src->value.w;
        break;
    case 16:
        cmd->target[0x1BD] = src->value.b;
        break;
    case 17:
        *(s16 *)(cmd->target + 0x154) = src->value.h;
        break;
    }
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E7D28);

void func_801E8168(Unk801E8168 *obj, s32 dy, Chunk *pak) {
    char path[32];
    u32 *tim;
    s32 loaded = 0;

    tim = findPakChunk(pak, 5, obj->animId);
    if (tim == NULL) {
        sprintf(path, "E:\\ANM\\%d_%d.TIM", obj->animId / 10, obj->animId % 10);
        tim = (u32 *)loadFile(path, getCurrentTaskId());
        loaded = 1;
        if (tim == NULL) {
            return;
        }
    }
    uploadTim(tim, obj->pixelX, obj->pixelY + dy, obj->clutX, obj->clutY + dy);
    DrawSync(0);
    if (loaded) {
        freeHeapBlock(tim);
    }
}

void func_801E826C(s32 id, Chunk *pak) {
    char path[32];
    u32 *tims;
    s32 loaded = 0;

    tims = findPakChunk(pak, 5, id);
    if (tims == NULL) {
        sprintf(path, "E:\\TIM\\%04d.TIM", id);
        tims = (u32 *)loadFile(path, getCurrentTaskId());
        loaded = 1;
        if (tims == NULL) {
            return;
        }
    }
    uploadTimList(tims);
    if (loaded) {
        freeHeapBlock(tims);
    }
}

s32 func_801E8304(s32 id) {
    char path[32];
    s32 file;

    sprintf(path, "E:\\%d.PAK", id);
    file = loadFileTagged((s32 *)path, getCurrentTaskId(), 0x12C);
    if (file != 0) {
        return file;
    }
    return 0;
}

void func_801E8358(void) {
    CameraEffect fx;

    fx.unkD4[0] = 100;
    fx.unkD4[1] = -150;
    fx.unkD4[2] = 9000;
    fx.unkDC[0] = 100;
    fx.unkDC[1] = -150;
    fx.unkDC[2] = 9000;
    fx.unkE4[0] = 160;
    fx.unkE4[1] = 5800;
    fx.unkE4[2] = 0;
    fx.unkEC[0] = 0;
    fx.unkEC[1] = 0;
    fx.unkEC[2] = 0;
    fx.unkF4[0] = 0;
    fx.unkF4[1] = 0;
    fx.unkF4[2] = 0;
    fx.unkAC[0] = 0x1000;
    fx.unkAC[1] = 0x1000;
    fx.unkAC[2] = 0x1000;
    fx.unkBC[0] = 0x1000;
    fx.unkBC[1] = 0x1000;
    fx.unkBC[2] = 0x1000;
    fx.unkCC[0] = 0;
    fx.unkCC[1] = 0;
    fx.unkCC[2] = 0;
    fx.unk137 = 0;
    fx.unk130 = 0;
    fx.unk12C = 0x80;
    fx.parent = SCENE_3D->unk78;
    fx.unk12E = 0;
    D_801EF808 = fx;
    initEffectObject(&D_801EF808);
}

void func_801E8470(Unk801E8470 *obj) {
    s32 i;

    obj->slots = allocTaskHeapBlock(sizeof(Slots));
    for (i = 0; i < 150; i++) {
        obj->slots->slots[i].id = -1;
        obj->slots->slots[i].unk2 = 0;
        obj->slots->slots[i].unk4 = 0;
    }
    obj->slots->unk50C = obj->unk10 = 0;
    obj->unk14 = 0;
    obj->slots->unk508 = 0;
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E8500);

void func_801E864C(void *obj) {
    func_801EA174(obj);
    freeHeapBlock(obj);
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E8678);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E908C);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E92B0);

void func_801E9448(void *xform, u8 *obj) {
    SVECTOR pos;

    getTransformWorldPos(xform, &pos);
    *(s32 *)(obj + 0x65C) = pos.vx;
    *(s32 *)(obj + 0x660) = pos.vy;
    *(s32 *)(obj + 0x664) = pos.vz;
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E9494);

void func_801E9598(u8 *obj) {
    SVECTOR pos;
    Color from;
    Color to;

    pos.vx = *(s32 *)(obj + 0x140);
    pos.vy = *(s32 *)(obj + 0x144);
    pos.vz = *(s32 *)(obj + 0x148);
    pos.pad = *(s32 *)(obj + 0x14C);
    from.r = *(s32 *)(obj + 0xA4);
    from.g = *(s32 *)(obj + 0xA8);
    from.b = *(s32 *)(obj + 0xAC);
    to.r = *(s32 *)(obj + 0xB0);
    to.g = *(s32 *)(obj + 0xB4);
    to.b = *(s32 *)(obj + 0xB8);
    func_801E6424(&pos, &from, &to, *(s32 *)(obj + 0x78), *(s32 *)(obj + 0x80), *(s32 *)(obj + 0x120));
}

void func_801E9654(u8 *a, u8 *b) {
    SVECTOR pos;

    pos.vx = *(s32 *)(a + 0x150);
    pos.vy = *(s32 *)(a + 0x154) + *(s32 *)(b + 0x4B0) * 256;
    pos.vz = *(s32 *)(a + 0x158);
    pos.pad = *(s32 *)(a + 0x15C);
    func_801E66D0(&pos, *(s32 *)(a + 0x70), *(s32 *)(a + 0x120), *(s16 *)(a + 0x80));
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E96C4);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E9890);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E9ADC);

EffectTemplate *func_801E9D14(s32 a0, s32 a1) {
    EffectTemplate template;

    func_801E9494(&template, a0, a1);
    return cloneEffectObject(&template);
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E9D48);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E9E04);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801E9EAC);

void func_801E9FE0(Unk801E9FE0 *obj) {
    VECTOR a;
    VECTOR b;
    VECTOR c;
    VECTOR d;

    a.vx = obj->unk65C[0];
    a.vy = obj->unk65C[1];
    a.vz = obj->unk65C[2];
    b.vx = obj->unk668[0];
    b.vy = obj->unk668[1];
    b.vz = obj->unk668[2];
    c.vx = obj->unk6A0[0];
    c.vy = obj->unk6A0[1];
    c.vz = obj->unk6A0[2];
    d.vx = obj->unk6AC[0];
    d.vy = obj->unk6AC[1];
    d.vz = obj->unk6AC[2];
    func_801E76A8(&a, &b, &c, &d, obj->unk67C, obj->unk680, obj->unk684);
}

void func_801EA0C0(void *ptr) {
    freeHeapBlock(ptr);
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EA0E0);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EA174);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EA258);

void func_801EA3AC(void **obj) {
    func_801E864C(obj[3]);
    freeScriptContext(obj[1], obj[2]);
    if (*(void **)((u8 *)obj[3] + 0x508) != NULL) {
        freeHeapBlock(*(void **)((u8 *)obj[3] + 0x508));
    }
    freeHeapBlock(obj);
}

s32 func_801EA410(Unk801EA410 *obj, s32 *state) {
    if (*state != 3) {
        *state = 1;
        truncatePakTextures(obj->unkC->pak);
        do {
            func_80014C08(FRAME_INTERVAL);
        } while (*state == 1);
    }
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EA48C);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EA574);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EA7BC);

void func_801EA9FC(void) {
    SCREEN_COPY_EFFECT.r = D_801EF800[0] + D_801EF804[0] * D_801EF7F8;
    SCREEN_COPY_EFFECT.g = D_801EF800[1] + D_801EF804[1] * D_801EF7F8;
    SCREEN_COPY_EFFECT.b = D_801EF800[2] + D_801EF804[2] * D_801EF7F8;
    func_801EA574();
    D_801EF7F8++;
}

void func_801EAAAC(u8 *obj, s32 clearColor) {
    D_801EF7F8 = 0;
    D_801EF2A0 = D_801EF7E4 = 0;
    D_801EF7FC = *(s32 *)(obj + 0x6D4);
    D_801EF29C = 1;
    D_801EF7F0 = *(s32 *)(obj + 0xE4);
    D_801EF7F4 = *(s32 *)(obj + 0x118);
    SCREEN_COPY_EFFECT.mode = 1;
    if (clearColor) {
        SCREEN_COPY_EFFECT.r = 0;
        SCREEN_COPY_EFFECT.g = 0;
        SCREEN_COPY_EFFECT.b = 0;
    }
    func_801EA7BC(*(s32 *)(obj + 0xA4), *(s32 *)(obj + 0xA8), *(s32 *)(obj + 0xAC), *(s32 *)(obj + 0x84), *(s32 *)(obj + 0x88));
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EAB5C);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EAD04);

Entry *func_801EB380(s32 key, s32 subKey) {
    Entry *entry;
    Entry *free;
    s32 i;

    entry = D_801EF950;
    free = NULL;
    for (i = 0; i < 128; i++, entry++) {
        if (key == entry->key && subKey == entry->subKey) {
            return entry;
        }
        if (free == NULL && entry->used == 0) {
            free = entry;
        }
    }
    return free;
}

void func_801EB3EC(void) {
    s32 i;

    D_801EF950 = allocHeapBlock(0x600, 0x80);
    for (i = 0; i < 128; i++) {
        D_801EF950[i].key = -1;
        D_801EF950[i].used = 0;
        D_801EF950[i].subKey = 0;
    }
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EB458);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EB5CC);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EB798);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EB874);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EBBFC);

void func_801EBE20(s32 a0, s32 a1, s32 a2) {
    func_801EBBFC(a0, a1, a2, 0);
}

void func_801EBE40(void) {
    freeHeapBlocksByTag(0x80);
}

void func_801EBE60(s8 *obj) {
    obj[0x82] = -1;
}

void func_801EBE6C(void) {
    ((Graphics *)&GRAPHICS)->posZ = 0;
    ((Graphics *)&GRAPHICS)->posX = 0;
    ((Graphics *)&GRAPHICS)->unk92 = 0;
    ((Graphics *)&GRAPHICS)->posY = -150;
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EBE8C);

void func_801EBFA0(s16 *dst, s32 count, s16 value) {
    s32 i;

    for (i = 0; i < count; i++) {
        dst[i] = value;
    }
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EBFD4);

void func_801EC160(Unk801EC160 *obj, s16 a1, s16 a2, s16 a3, s8 flags) {
    obj->unk0 = a1;
    obj->unk4 = a2;
    obj->unk6 = a3;
    obj->flags = flags;
    if (flags & 2) {
        func_801EBFA0(obj->unk8, 6, a1);
    }
    obj->unk16 = 0x80;
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EC1F4);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EC494);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EC6E0);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EC8A4);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801ECC70);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801ECF20);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801ED6F0);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801ED97C);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EDB64);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EDC4C);

void func_801EDD9C(void) {
    s32 unused[8];

    D_801EF37C = 0;
    addFrameCallback((s32)renderSceneModels);
    func_801EB3EC();
    func_801E6B40();
    func_801EDC4C();
    func_801E8358();
    D_801EF978[1] = 0;
    D_801EF978[0] = 0;
}

void func_801EDDF8(void) {
    createWireGrid(2000, 3000, 9, 13, 1, 0);
    GRID_VISIBLE = 1;
    SCENE_3D->modelState[23] = 1;
}

void func_801EDE4C(void) {
    SCENE_3D->modelState[0] = -1;
    SCENE_3D->modelState[1] = -1;
    SCENE_3D->modelState[23] = -1;
    func_80014A00(0x1B);
    func_80014A00(0x1A);
    removeFrameCallback((s32)renderSceneModels);
    removeFrameCallback((s32)renderWireGrid);
    freeHeapBlock(DB(0).scenePackets);
    freeHeapBlock(DB(1).scenePackets);
    func_801EBE40();
    func_801E723C();
    func_80022E58();
    func_801E40B0();
}

void func_801EDF00(s32 frames, s32 resetCamera) {
    Graphics *camera;

    if (resetCamera) {
        camera = (Graphics *)&GRAPHICS;
        *(s32 *)&camera->pad5A[0xE] = 0x9C4000;
        camera->unk90 = 4000;
        camera->snapCamera = 1;
        camera->unk8E = 400;
        *(s32 *)&camera->pad5A[0x16] = 0x190000;
    }
    D_80079584 = -1;
    SCENE_3D->modelState[0] = 1;
    SCENE_3D->modelState[1] = 1;
    do {
        frames--;
        func_801EBE8C();
        func_80014C08(FRAME_INTERVAL);
    } while (frames >= 0);
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EDFA8);

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EE3D4);

void func_801EEBEC(s32 model, s32 anim) {
    SCENE_3D->modelState[model] = 1;
    SCENE_3D->modelState[model ^ 1] = -1;
    D_80079584 = model;
    playModelAnimation(model, anim);
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EEC3C);

void func_801EEE24(s32 model) {
    SCENE_3D->modelState[model] = 1;
    playModelAnimation(model, 0);
    D_80079584 = model;
    SCENE_3D->modelState[model ^ 1] = -1;
    func_801ECF20(~model);
}

INCLUDE_ASM("asm/sugseg/nonmatchings/sugseg", func_801EEE90);

INCLUDE_RODATA("asm/sugseg/nonmatchings/sugseg", D_801DE0B0);
