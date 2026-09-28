#include "common.h"
#include "game.h"
#include "dcb/window.h"
#include "dcb/text.h"
#include "dcb/heap.h"
#include "dcb/script.h"
#include "dcb/frame_callback.h"
#include "dcb/task.h"
#include "dcb/hacking_shell.h"
#include "dcb/vram_upload.h"
#include "dcb/scene3d.h"
#include "dcb/game_flow.h"

typedef struct {
    u8 data[0x3C];
    s16 unk3C;
    s8 unk3E;
    u8 unk3F;
} Unk801DFB6C;
typedef struct {
    u8 pad[0x3C];
    s16 unk3C;
    s8 unk3E;
    u8 pad3F;
} Unk801DFBC4;
typedef struct {
    u8 pad[0x3C];
    s16 unk3C;
    s8 unk3E;
    s8 unk3F;
} Unk801F4840;
extern Unk801F4840 D_801F4840[3];
typedef struct {
    u8 pad[0x3E];
    s8 unk3E;
    u8 pad3F;
} Unk801E0150;
typedef struct {
    u8 pad[0x78];
    s32 unk78;
    u8 pad7C[5];
    u8 unk81;
} Unk801F4588;
extern Unk801F4588 D_801F4588;
extern void (*D_801F3594[])(void);
extern s8 D_801F4609;
typedef struct {
    u8 pad[0x8];
    s32 *flags;
} Unk801F4838;
extern Unk801F4838 *D_801F4838;
extern u8 D_801F4A30;
extern s8 D_801F4A15[24];
extern void *D_801F5260[];
void func_801EBA54(void *arg0);
extern UiWindow D_801F4B00;
extern UiWindow D_801F4338;
void func_801E2120(void);
void func_801E28A0(UiWindow *window);
extern u8 D_801F469F;
void func_801EBA34(void *ptr);
typedef struct {
    u8 pad[0x8];
    s32 *unk8;
} Unk801E460C;
void func_801E390C(Unk801E460C *arg0);
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Unk801E4700;
Script *func_801E4648(u8 *scriptData);
extern UiWindow D_801F4B44;
void func_801DFA6C(void);
void func_801E0770(void);
void func_801E140C(void);
void func_801E1448(void);
void func_801E28E4(void);
void func_801E2ED4(void);
void func_801E317C(void);
void func_801E5548(void);
extern s8 D_801F4691;
void func_801E2A18(void);
void func_801E33DC(void);
void func_801E3438(void);
extern s16 D_801F4648[6];
typedef struct {
    u8 pad[0x140];
    s32 unk140;
    s32 unk144;
    s32 unk148;
    s32 unk14C;
    s16 unk150;
    s16 unk152;
    u8 pad154[6];
    u8 unk15A;
} Unk801F4804;
extern Unk801F4804 *D_801F4804;
extern char D_801DE6B0[];
extern UiWindow D_801F46B0[5];
extern void (*D_801F3670[5])();
extern u8 D_801F4834;
extern u8 D_801F4808;
extern UiWindow D_801F43D8;
extern UiWindow D_801F4478;
extern UiWindow D_801F4518;
void func_801EA9B0(void);
void func_801EAF8C(void);
void func_801EB0F8(void);
typedef struct {
    s16 type;
    s16 unk2;
    s32 size;
} PackEntry;
typedef struct {
    u8 pad[0x68];
    SVECTOR corners[4];
} Unk801EBD28;
typedef struct {
    u8 pad[0x88];
    s32 unk88;
} Unk801EBD8C;
typedef struct {
    u8 pad[0x4];
    u8 r;
    u8 g;
    u8 b;
    u8 pad7[0x21];
} Unk801EBD94;
typedef struct {
    u8 pad[0x68];
    SVECTOR corners[4];
} Unk801EBDD0;
typedef struct {
    u8 pad[0x3D4];
    s32 unk3D4;
    u8 pad3D8[0x1E];
    s16 unk3F6;
    s16 unk3F8;
} Unk801F4E48;
extern Unk801F4E48 D_801F4E48;
extern s8 D_801F5252;
void func_801ECD2C(void);
void func_801ECE60(void);
void func_801ECEB0(void);
typedef struct {
    s32 unk0;
    s16 x;
    s16 y;
    u8 unk8;
    u8 unk9;
} Unk801F5350;
extern Unk801F5350 *D_801F5350;
void func_801ED014(void);
void func_801ED2E0(void);
void func_801ED42C(void);
extern u8 D_801F524F;
typedef struct {
    UiWindow window;
    s32 slot;
} RewardWindow;
extern UiWindow D_801F5360;
void func_801F23D8(void);
extern UiWindow D_801F5410;
void func_801F2F04(void);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801DFA6C);

void func_801DFB6C(Unk801DFB6C *arg0) {
    s32 i;

    for (i = 0; i < 3; i++, arg0++) {
        arg0->unk3E = -1;
        bzero(arg0, 0x3C);
    }
}

Unk801DFBC4 *func_801DFBC4(Unk801DFBC4 *arg0) {
    s32 i;

    for (i = 0; i < 3; i++, arg0++) {
        if (arg0->unk3E == -1) {
            arg0->unk3E = 0;
            arg0->unk3C = 0;
            return arg0;
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801DFC00);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DDF38);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801DFE70);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E002C);

void func_801E0104(void) {
    Unk801F4840 *entry = D_801F4840;
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->unk3E != -1) {
            entry->unk3C = entry->unk3F;
        }
    }
}

void func_801E0150(Unk801E0150 *arg0) {
    s32 i;

    for (i = 0; i < 3; i++, arg0++) {
        arg0->unk3E = -1;
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0174);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0390);

void func_801E0480(void) {
    D_801F4588.unk78++;
    if (D_801F4588.unk78 > 20) {
        D_801F4588.unk78 = 20;
        D_801F4588.unk81 = 1;
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E04B8);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0650);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0684);

void func_801E0724(void) {
    if (D_801F3594[D_801F4609] != NULL) {
        D_801F3594[D_801F4609]();
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0770);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0988);

void func_801E09F4(void) {
    if (D_801F4838->flags[266] != 0) {
        ((PlayerProfile *)PLAYER_PROFILES)->unk2C |= 1;
    }
    if (D_801F4838->flags[267] != 0) {
        ((PlayerProfile *)PLAYER_PROFILES)->unk2C |= 2;
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0A74);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0B70);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0C54);

void func_801E0D54(void) {
    s32 i;

    D_801F4A30 = 0;
    for (i = 0; i < 24; i++) {
        D_801F4A15[i] = -1;
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0D8C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0F90);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE2F0);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE2FC);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE318);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E108C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E1190);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E1254);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E1360);

void func_801E140C(void) {
    func_801EBA54(D_801F5260[22]);
    func_801EBA54(D_801F5260[21]);
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E1448);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E15EC);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E16B0);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE328);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE344);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE34C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E186C);

void func_801E1A54(void) {
    drawWindow(&D_801F4B00, func_801E2120, 0x18);
    if (D_801F4838->flags[15] != 0) {
        drawWindow(&D_801F4338, func_801E28A0, 0x18);
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E1ABC);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E2120);

void func_801E28A0(UiWindow *window) {
    char buf[0x48];

    drawText(window->originX + 6, window->originY + 1, (s32)"*b0:Player's Complete Stats", 7, window->z);
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E28E4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E2A18);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E2D90);

s32 func_801E2E64(void) {
    s32 value = D_801F469F;
    s32 wasZero = value == 0;
    s32 n = value - 8;

    if (n < 0) {
        n = 0;
    }
    D_801F469F = n;
    return wasZero;
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E2E90);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E2ED4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E317C);

void func_801E33DC(void) {
    s32 i;
    s32 j;

    for (i = 10, j = 1; i < 20; i++, j++) {
        func_801EBA54(D_801F5260[j]);
    }
}

void func_801E3438(void) {
    s32 i;
    s32 j;

    for (i = 0x30, j = 11; i < 0x3A; i++, j++) {
        func_801EBA54(D_801F5260[j]);
    }
}

void func_801E3494(void) {
    s32 i;
    s32 j;

    for (i = 10, j = 1; i < 20; i++, j++) {
        func_801EBA34(D_801F5260[j]);
    }
}

void func_801E34F0(void) {
    s32 i;
    s32 j;

    for (i = 0x30, j = 11; i < 0x3A; i++, j++) {
        func_801EBA34(D_801F5260[j]);
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E354C);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE460);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE464);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE468);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE474);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE47C);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE488);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E390C);

s32 func_801E460C(Unk801E460C *arg0) {
    *arg0->unk8 = 1;
    func_801E390C(arg0);
    return *arg0->unk8;
}

Script *func_801E4648(u8 *scriptData) {
    Script *script = allocHeapBlock(sizeof(Script), 0x31);
    u8 *codeStart;

    script->base = scriptData;
    codeStart = scriptData + 0x10;
    script->start = codeStart;
    script->pc = codeStart;
    script->offset = 0;
    script->size = *(u32 *)(scriptData + 8);
    clearScriptBusy(script);
    return script;
}

s32 *func_801E46AC(s32 count) {
    s32 *block = allocHeapBlock(count * 4, 0x31);
    s32 *p = block;
    s32 i;

    for (i = 0; i < count; i++) {
        *p++ = 0;
    }
    return block;
}

Unk801E4700 *func_801E4700(void) {
    u8 unused[0x18];
    Unk801E4700 *obj = allocHeapBlock(sizeof(Unk801E4700), 0x31);

    obj->unk0 = *(s32 *)&((SessionData *)D_8006E054)->unk100C->unk0[0x190];
    obj->unk4 = func_801E4648(obj->unk0);
    return obj;
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E4754);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E4878);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E49A4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E4AF4);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE5C8);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E4D80);

void func_801E54C4(void) {
    drawWindow(&D_801F4B44, func_801DFA6C, 0x17);
    func_801E0770();
    func_801E140C();
    func_801E1448();
}

void func_801E550C(void) {
    func_801E28E4();
    func_801E2ED4();
    func_801E317C();
    addFrameCallback((s32)func_801E5548);
}

void func_801E5548(void) {
    if (D_801F469F != 0) {
        func_801E2A18();
    }
    func_801E33DC();
    func_801E3438();
    if (D_801F4691 != -1) {
        func_801EBA54(D_801F5260[0]);
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E55AC);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E55F8);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E5714);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E57C4);

void func_801E5B1C(void) {
    s32 i = 0;
    s16 *values = D_801F4648;

loop:
    values[i] = rand() % 30;
    i++;
    if (i < 6) {
        goto loop;
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E5BA0);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E602C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E612C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E6464);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E6A68);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E6EF0);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E7240);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E75D4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E77F4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E7A00);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE638);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE644);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE64C);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE654);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE664);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E7D1C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E818C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E8238);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E84BC);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E8714);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E87E8);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E8980);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E8C48);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E8DD8);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E9264);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E931C);

void func_801E9420(void) {
    D_801F4804->unk15A = 0;
    D_801F4804->unk148 = 40;
    D_801F4804->unk150 = 32;
    D_801F4804->unk152 = 44;
    D_801F4804->unk14C = 0;
    D_801F4804->unk144 = 0;
    D_801F4804->unk140 = 60;
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E9460);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E96DC);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E972C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E9784);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E9864);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E9988);

void func_801E9A6C(UiWindow *window) {
    char buf[0x48];

    drawText(window->originX, window->originY, (s32)D_801DE6B0, 0, window->z);
}

void func_801E9AAC(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        drawWindow(&D_801F46B0[i], D_801F3670[i], 9);
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E9B20);

void func_801E9D50(void) {
    func_800149B8(0, -1, 0, 0x800, runHackingSequence, 1, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
}

void func_801E9DB0(void) {
    func_800149B8(0, -1, 0, 0x800, runHackingSequence, 3, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E9E10);

void func_801E9F08(void) {
    D_801F4834 = 6;
}

void func_801E9F18(void) {
    D_801F4808 = 2;
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E9F28);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE6B0);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE708);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EA230);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EA9B0);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EAF8C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EB0F8);

void func_801EB198(void) {
    drawWindow(&D_801F43D8, func_801EA9B0, 0x1E);
    drawWindow(&D_801F4478, func_801EAF8C, 0x1E);
    drawWindow(&D_801F4518, func_801EB0F8, 0x1E);
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EB1F8);

void func_801EB5C0(u8 *pack) {
    PackEntry *entry;

    if (pack == NULL) {
        return;
    }
    do {
        entry = (PackEntry *)pack;
        pack += sizeof(PackEntry);
        if (entry->type < 0) {
            break;
        }
        if (entry->type == 5) {
            uploadTexturePack((u32 *)pack);
        }
        pack += entry->size;
    } while (1);
}

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE804);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EB628);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EB754);

void func_801EBA34(void *ptr) {
    freeHeapBlock(ptr);
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EBA54);

void func_801EBD28(Unk801EBD28 *arg0, s16 dx, s16 dy) {
    arg0->corners[2].vx += dx;
    arg0->corners[0].vx = arg0->corners[2].vx;
    arg0->corners[3].vx += dx;
    arg0->corners[1].vx = arg0->corners[3].vx;
    arg0->corners[1].vy += dy;
    arg0->corners[0].vy = arg0->corners[1].vy;
    arg0->corners[3].vy += dy;
    arg0->corners[2].vy = arg0->corners[3].vy;
}

void func_801EBD8C(Unk801EBD8C *arg0, s32 arg1) {
    arg0->unk88 = arg1;
}

void func_801EBD94(Unk801EBD94 *arg0, u8 value) {
    s32 i;

    for (i = 0; i < 2; i++) {
        arg0[i].r = value;
        arg0[i].g = value;
        arg0[i].b = value;
    }
}

void func_801EBDD0(Unk801EBDD0 *arg0, s16 width, s16 height) {
    s32 halfWidth;
    s32 halfHeight;

    halfWidth = width >> 1;
    arg0->corners[2].vx = -halfWidth;
    arg0->corners[0].vx = -halfWidth;
    arg0->corners[3].vx = halfWidth;
    arg0->corners[1].vx = halfWidth;
    halfHeight = height >> 1;
    arg0->corners[1].vy = -halfHeight;
    arg0->corners[0].vy = -halfHeight;
    arg0->corners[3].vy = halfHeight;
    arg0->corners[2].vy = halfHeight;
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EBE0C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EBE94);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EBFB4);

void func_801EC080(void) {
    Graphics *camera;

    initScene3D(0);
    func_800149B8(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    camera = (Graphics *)&GRAPHICS;
    camera->rotX = 0;
    camera->rotY = 0;
    camera->rotZ = 0;
    camera->posX = 0;
    camera->posY = 0;
    camera->posZ = 0;
    camera->unk8E = 0;
    camera->unk90 = 0x1C0;
    camera->unk92 = 0;
    camera->unk94 = 0;
    camera->targetModel = -1;
    camera->snapCamera = 1;
}

s32 func_801EC108(u32 id) {
    u32 word;
    s32 mask;

    id -= 12;
    word = id >> 5;
    id &= 31;
    return (((PlayerProfile *)PLAYER_PROFILES)->unk23FC[word] & (mask = 1 << id)) != 0;
}

void func_801EC140(void) {
    s32 t = D_801F4E48.unk3D4;

    D_801F4E48.unk3F6 = (t * 270 + (15 - t) * 136) / 15;
    D_801F4E48.unk3F8 = (t * 9 + (15 - t) * 96) / 15;
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EC1D8);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EC41C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EC51C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EC5C4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EC844);

void func_801ECCA4(s8 mode) {
    D_801F5252 = mode;
    switch (mode) {
    case 0:
        func_801ECD2C();
        break;
    case 1:
        func_801ECE60();
        break;
    case 2:
        func_801ECEB0();
        break;
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801ECD2C);

void func_801ECE60(void) {
    s32 x = 0xAB;
    s32 y = 0x28;

    D_801F5350->x = x;
    D_801F5350->y = y;
    D_801F5350->unk0 = 0;
    D_801F5350->unk8 = 4;
    D_801F5350->unk9 = 6;
    D_801F5350->unk9 *= 2;
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801ECEB0);

void func_801ECF94(s8 mode) {
    switch (mode) {
    case 0:
        func_801ED014();
        break;
    case 1:
        func_801ED2E0();
        break;
    case 2:
        func_801ED42C();
        break;
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801ED014);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801ED2E0);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801ED42C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801ED5D4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EDB90);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EDE58);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EDFB8);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EE038);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EE13C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EE40C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EE5A8);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EE690);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EE9A0);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EEBD0);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EEC78);

void func_801EF1A8(void) {
    D_801F524F = 0;
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EF1B4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EF25C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EF3F4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EF4CC);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EF750);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EF7C4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EFA34);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EFC54);

void func_801EFE84(void) {
    s32 i;

    for (i = 0x28; i < 0x30; i++) {
        func_801EBA54(D_801F5260[i]);
    }
}

void func_801EFED4(void) {
    s32 i;

    for (i = 0x30; i < 0x38; i++) {
        func_801EBA54(D_801F5260[i]);
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801EFF24);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F0078);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F00F4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F082C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F0A08);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F0D20);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F0D84);

void func_801F0E84(void) {
}

void func_801F0E8C(void) {
    openDeckEditor(0);
}

void func_801F0EAC(void) {
    openPartnerEquipment(0);
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F0ECC);

void func_801F0F08(void) {
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (!(PAD_STATES[0]->pressed & 0x40));
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F0F5C);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE864);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE86C);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE878);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE884);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE894);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE8A4);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE8E4);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE904);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE944);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE94C);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE954);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE95C);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE964);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F1294);

void func_801F1944(RewardWindow *arg0) {
    s32 x = arg0->window.originX;
    s32 y = arg0->window.originY;
    s32 z = arg0->window.z;

    if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[arg0->slot] < 0) {
        drawLargeText(x + 1, y + 1, (s32)"FULL SET!", 7, z);
    } else {
        drawLargeText(x + 1, y + 1, (s32)"RECEIVED!", 7, z);
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F19C4);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F1A80);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F1B5C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F1C84);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F208C);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DEA04);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DEA14);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F23D8);

void func_801F2A8C(void) {
    drawWindow(&D_801F5360, func_801F23D8, 1);
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F2ABC);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F2F04);

void func_801F329C(void) {
    drawWindow(&D_801F5410, func_801F2F04, 0);
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F32CC);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DFA58);
