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
#include "dcb/loader.h"
#include "dcb/duel_launch.h"
#include "dcb/card_db.h"
#include "dcb/sound_play.h"
#include "dcb/scroll_bg.h"
#include "dcb/memcard.h"

typedef struct {
    POLY_FT4 quads[2];
    VECTOR pos;
    SVECTOR rot;
    SVECTOR corners[4];
    s32 otz;
    s16 w;
    s16 h;
} Sprite3D;
typedef struct {
    s32 x;
    s32 y;
    s32 clutX;
    u16 clutY;
    u16 padE;
    u8 w;
    u8 h;
    u8 mode;
    u8 pad13;
} SpriteTemplate;
extern SpriteTemplate D_801F3708[];

void func_801E4AF4(s32 arg0);

typedef struct {
    u8 text[0x3C];
    s16 shown;
    s8 active;
    s8 length;
} TextLine;
s32 func_801DFE70(s32 x, s32 y, TextLine *line, s32 z);
typedef struct {
    u8 pad[0x8];
    s32 playTime;
    s32 cardRate;
    s32 abilityRate;
    PlayerProfile *profile;
    char *tamerRank;
    char *collectorRank;
    u8 unk20;
} Unk801F4810;
extern Unk801F4810 D_801F4810;
extern char *STR_TAMER_RANKS[8];
extern char *STR_COLLECTOR_RANKS[8];

extern s8 D_801F460A;
extern u8 D_801F4696;
typedef struct {
    u8 pad[0x10D];
    s8 text[0x1B];
    s8 length;
} Unk801F4908;
extern Unk801F4908 D_801F4908;
typedef struct {
    s32 unk0;
    Script *script;
    s32 *regs;
} ScriptRunner;
extern u8 D_801F4604[];
typedef struct {
    Rect16 rect;
    s32 brightness;
    s32 style;
    s32 flags;
    s32 label;
    u8 labelPalette;
} WindowDef;
typedef struct {
    s32 delay;
    s32 count;
} Unk801E8714Entry;
typedef struct {
    u8 pad[0x80];
    Unk801E8714Entry entries[7];
    s32 unkB8;
    u8 padBC[7];
    u8 unkC3;
} Unk801F4AF0;
extern Unk801F4AF0 *D_801F4AF0;
extern WindowDef D_801F3460[5];
void func_801E9988(UiWindow *window, WindowDef *def);
void func_801E9B20(void);
void func_801E9D50(void);
void func_801E9DB0(void);
typedef struct {
    u8 pad[0x16];
    u16 tpage;
    u8 pad18[0x10];
} Quad;
typedef struct {
    Quad quad[2];
} QuadPair;
typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
} PolyF4;
typedef struct {
    u32 tag;
    u32 code[1];
} DrTPage;
extern PolyF4 *D_801F5214[2];
extern DrTPage D_801F5084[2][20];
extern u8 D_801F4AFF;
extern u8 D_801F46A5;
extern s16 D_801F4AF8[3];

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
    u8 pad82[2];
    u8 unk84;
    u8 pad85[0x10E - 0x85];
    u8 unk10E;
    u8 pad10F[0x115 - 0x10F];
    u8 unk115;
    u8 pad116;
    u8 unk117;
    u8 pad118[0x11C - 0x118];
    u8 unk11C;
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

s32 func_801DFE70(s32 x, s32 y, TextLine *line, s32 z) {
    u8 buf[0x40];
    u8 *dst = buf;
    u8 *src;
    s8 i;
    u8 c;

    if (line->length == line->shown) {
        drawText(x, y, (s32)line, 7, z);
        return -1;
    }
    src = line->text;
    for (i = 0; i < line->shown; i++) {
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
                    line->shown += 2;
                }
                break;
            }
        }
        dst[0] = *src;
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        line->shown += 1;
    } else {
        *dst++ = src[0];
        dst[0] = src[1];
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        line->shown += 2;
    }
    return 1;
}

s32 func_801E002C(s16 x, s16 y, s32 arg2) {
    Unk801F4840 *entry = D_801F4840;
    s32 found = 0;
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->unk3E != -1) {
            if (func_801DFE70(x + 4, y + i * 13, entry, arg2) == 1) {
                found = 1;
                break;
            }
        }
    }
    return found;
}

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

void func_801E0650(void) {
    if (--D_801F4588.unk78 < 0) {
        D_801F4588.unk78 = 0;
        D_801F4588.unk81 = 3;
        D_801F4588.unk84 = 0;
    }
}

void func_801E0684(void) {
    s32 i;

    if (D_801F460A == 0) {
        for (i = 0; i < 4; i++) {
            func_801EBA34(D_801F5260[i + 25]);
        }
    } else {
        for (i = 0; i < 6; i++) {
            func_801EBA34(D_801F5260[i + 25]);
        }
    }
    D_801F4696 = 0;
}

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

void func_801E0C54(s32 ch) {
    s8 last;

    if (D_801F4908.length % 6 == 0 && D_801F4908.length != 0) {
        last = D_801F4908.text[D_801F4908.length - 1];
        D_801F4908.text[D_801F4908.length - 1] = 0x11;
        D_801F4908.text[D_801F4908.length++] = 0x10;
        D_801F4908.text[D_801F4908.length++] = last;
        D_801F4908.text[D_801F4908.length++] = ch;
    } else {
        D_801F4908.text[D_801F4908.length++] = ch;
    }
}

void func_801E0D54(void) {
    s32 i;

    D_801F4A30 = 0;
    for (i = 0; i < 24; i++) {
        D_801F4A15[i] = -1;
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E0D8C);

void func_801E0F90(UiWindow *window, Rect16 pos, s32 label, s32 style, s32 brightness) {
    Rect16 rect;
    Rect16 unused;
    Rect16 view;

    rect.x = pos.x - pos.w / 2;
    rect.y = pos.y - pos.h / 2;
    rect.w = pos.w & ~1;
    rect.h = pos.h & ~1;
    view.x = 0;
    view.y = 0;
    view.w = (pos.w + 10) & ~1;
    view.h = 0x2000;
    openWindow(window, &rect, -1, (s16 *)&view, style, brightness, 0x80, 0x10);
    window->label = label;
    window->labelPalette = 8;
}

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE2F0);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE2FC);

INCLUDE_RODATA("asm/saiseg/nonmatchings/saiseg", D_801DE318);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E108C);

void func_801E1190(ScriptRunner *runner) {
    s32 result;

    do {
        result = runScriptToNextEvent(runner->script, runner->regs);
        if (result == 1) {
            if (runner->script->eventOp == 10) {
                if (runner->script->eventArg == 14) {
                    D_801F4604[runner->regs[1] - 1] = result;
                }
            }
            return;
        }
        clearScriptBusy(runner->script);
    } while (result != 0);
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E1254);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E1360);

void func_801E140C(void) {
    func_801EBA54(D_801F5260[22]);
    func_801EBA54(D_801F5260[21]);
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E1448);

void func_801E15EC(UiWindow *window, WindowDef *def, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        openWindow(window, def, -1, (s16 *)-1, def->flags, def->style, def->brightness, 12);
        if (def->label != 0) {
            window->label = def->label;
        }
        window->labelPalette = def->labelPalette;
        window++;
        def++;
    }
}

void func_801E16B0(void) {
    s32 i;

    D_801F4810.profile = (PlayerProfile *)PLAYER_PROFILES;
    D_801F4810.tamerRank = STR_TAMER_RANKS[D_801F4810.profile->tamerRank];
    D_801F4810.collectorRank = STR_COLLECTOR_RANKS[D_801F4810.profile->collectorRank];
    D_801F4810.cardRate = 0;
    D_801F4810.abilityRate = 0;
    D_801F4810.unk20 = 2;
    D_801F4810.playTime = (u16)D_801F4810.profile->unk14 * 1000 / 166;
    for (i = 0; i < 301; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->cardCollection[i] & 0x40) {
            D_801F4810.cardRate += 1000;
        }
    }
    D_801F4810.cardRate /= 301;
    for (i = 0; i < 128; i++) {
        if (getPartnerAbilityState(0, i)) {
            D_801F4810.abilityRate += 1000;
        }
    }
    D_801F4810.abilityRate /= 128;
}

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

s32 func_801E2E90(void) {
    s32 done;
    s32 level;

    D_801F4588.unk115 = 1;
    level = D_801F4588.unk117;
    done = level == 0xFF;
    level += 8;
    if (level > 0xFF) {
        level = 0xFF;
    }
    D_801F469F = level;
    return done;
}

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

void func_801E4754(void) {
    char path[0x18];
    u32 *pack;

    if (((PlayerProfile *)PLAYER_PROFILES)->unkE < 10) {
        sprintf(path, "C:\\DEBUG\\area0%d.TIS", ((PlayerProfile *)PLAYER_PROFILES)->unkE);
    } else {
        sprintf(path, "C:\\DEBUG\\area%d.TIS", ((PlayerProfile *)PLAYER_PROFILES)->unkE);
    }
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    sprintf(path, "C:\\OBJECT\\world.TIS");
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
}

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

void func_801E55AC(void) {
    removeFrameCallback(func_801E5548);
    func_80014C08(1);
    func_801EBA34(D_801F5260[0]);
    func_801E3494();
    func_801E34F0();
}

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

s32 func_801E8714(void) {
    s32 result = 1;
    s32 finished = -1;
    s32 i;

    D_801F4AF0->unkB8 -= 15;
    if (D_801F4AF0->unkB8 <= 0) {
        D_801F4AF0->unkB8 = 0;
        for (i = 0; i < 7; i++) {
            if (D_801F4AF0->entries[i].delay <= 0) {
                D_801F4AF0->entries[i].count++;
                if (D_801F4AF0->entries[i].count >= 20) {
                    D_801F4AF0->entries[i].count = 20;
                    finished++;
                }
            } else {
                D_801F4AF0->entries[i].delay--;
            }
        }
    }
    if (finished >= 6) {
        D_801F4AF0->unkC3 = 2;
        result = 0;
    }
    return result;
}

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

void func_801E96DC(void) {
    D_801F4804->unk140--;
    if (D_801F4804->unk140 < 0) {
        playSoundEffect(0x13);
        D_801F4804->unk15A = 2;
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E972C);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E9784);

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801E9864);

void func_801E9988(UiWindow *window, WindowDef *def) {
    measureText(D_801DE6B0);
    def->rect.w = (TEXT_WIDTH + 1) / 2 * 2;
    def->rect.h = (TEXT_HEIGHT + 1) / 2 * 2;
    openWindow(window, def, -1, (s16 *)-1, def->flags, def->style, def->brightness, 6);
    if (def->label != 0) {
        window->label = def->label;
    }
    window->labelPalette = def->labelPalette;
    animateWindowTo(window, (Rect16 *)-1);
}

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

void func_801E9B20(void) {
    stopMusic();
    addFrameCallback((s32)func_801E9AAC);
    setBackgroundScrollMode(1);
    func_800149B8(0, -1, 0, 0x800, runHackingSequence, 0, getCurrentTaskId(), 0, 0);
    func_80014C08(360);
    playMenuSound(3);
    animateWindowTo(&D_801F46B0[0], &D_801F3460[0].rect);
    func_80014C08(7);
    playMenuSound(3);
    animateWindowTo(&D_801F46B0[1], &D_801F3460[1].rect);
    func_80014C08(8);
    playMenuSound(3);
    animateWindowTo(&D_801F46B0[2], &D_801F3460[2].rect);
    func_80014C08(3);
    playMenuSound(3);
    animateWindowTo(&D_801F46B0[3], &D_801F3460[3].rect);
    func_80014C08(2);
    playMenuSound(3);
    animateWindowTo(&D_801F46B0[4], &D_801F3460[4].rect);
    func_80014C08(110);
    playSoundEffect(0x18);
    func_80014C08(480);
    playSoundEffect(0x19);
    func_80014C08(120);
    playSoundEffect(0x19);
    func_80014C08(240);
    animateWindowTo(&D_801F46B0[0], (Rect16 *)-1);
    playSoundEffect(0x19);
    func_80014C08(4);
    animateWindowTo(&D_801F46B0[1], (Rect16 *)-1);
    playSoundEffect(0x19);
    func_80014C08(2);
    animateWindowTo(&D_801F46B0[2], (Rect16 *)-1);
    playSoundEffect(0x19);
    func_80014C08(3);
    animateWindowTo(&D_801F46B0[3], (Rect16 *)-1);
    playSoundEffect(0x19);
    func_80014C08(1);
    animateWindowTo(&D_801F46B0[4], (Rect16 *)-1);
    playSoundEffect(0x19);
    func_80014C08(120);
    func_801E4AF4(1);
    removeFrameCallback((s32)func_801E9AAC);
    func_80014C08(170);
    playSoundEffect(0x18);
    func_80014C08(30);
}

void func_801E9D50(void) {
    func_800149B8(0, -1, 0, 0x800, runHackingSequence, 1, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
}

void func_801E9DB0(void) {
    func_800149B8(0, -1, 0, 0x800, runHackingSequence, 3, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
}

void func_801E9E10(s32 mode, s32 task) {
    s32 i;

    for (i = 0; i < 5; i++) {
        func_801E9988(&D_801F46B0[i], &D_801F3460[i]);
    }
    switch (mode) {
    case 0:
        func_801E9B20();
        break;
    case 1:
        func_801E9D50();
        break;
    case 2:
        func_801E9DB0();
        break;
    }
    D_801F4696 = 0;
    func_80014A48(task);
}

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

Sprite3D *func_801EB754(s32 id) {
    SpriteTemplate *tmpl = &D_801F3708[id];
    s32 abr = 0;
    Sprite3D *sprite = allocHeapBlock(sizeof(Sprite3D), 0x2E);
    POLY_FT4 *quad;
    s32 i;

    sprite->corners[0].vx = sprite->corners[2].vx = -(tmpl->w >> 1);
    sprite->corners[1].vx = sprite->corners[3].vx = tmpl->w >> 1;
    sprite->corners[0].vy = sprite->corners[1].vy = -(tmpl->h >> 1);
    sprite->corners[2].vy = sprite->corners[3].vy = (tmpl->h >> 1) - 1;
    for (i = 0; i < 4; i++) {
        sprite->corners[i].vz = 0;
    }
    sprite->rot.vx = sprite->rot.vy = sprite->rot.vz = 0;
    sprite->pos.vx = sprite->pos.vy = sprite->pos.vz = 0;
    sprite->w = tmpl->w;
    sprite->h = tmpl->h - 1;
    sprite->otz = 0x23;
    for (i = 0; i < 2; i++) {
        quad = &sprite->quads[i];
        func_800677A4(quad);
        quad->r0 = 0x80;
        quad->g0 = 0x80;
        quad->b0 = 0x80;
        SetSemiTrans(quad, 0);
        quad->clut = getClut(tmpl->clutX, tmpl->clutY);
        quad->tpage = ((tmpl->mode & 3) << 7) | ((abr & 3) << 5) | ((tmpl->y & 0x100) >> 4) | ((tmpl->x & 0x3C0) >> 6) | ((tmpl->y & 0x200) << 2);
        if (tmpl->mode != 0) {
            quad->u2 = quad->u0 = (tmpl->x % 64) << 1;
            quad->u1 = quad->u3 = ((tmpl->x % 64) << 1) + tmpl->w;
        } else {
            quad->u2 = quad->u0 = (tmpl->x % 64) << 2;
            quad->u1 = quad->u3 = ((tmpl->x % 64) << 2) + tmpl->w;
        }
        quad->v0 = quad->v1 = tmpl->y;
        quad->v2 = quad->v3 = tmpl->y + tmpl->h - 1;
    }
    return sprite;
}

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

void func_801EBE0C(QuadPair *pair, s32 abr) {
    s16 tpage;

    if (abr >= 0) {
        SetSemiTrans(&pair->quad[0], 1);
        SetSemiTrans(&pair->quad[1], 1);
        tpage = pair->quad[0].tpage & ~0x60;
        tpage |= (abr & 3) << 5;
        pair->quad[1].tpage = tpage;
        pair->quad[0].tpage = tpage;
    } else {
        SetSemiTrans(&pair->quad[0], 0);
        SetSemiTrans(&pair->quad[1], 0);
    }
}

void func_801EBE94(void) {
    PolyF4 *poly;
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        DB(i).primSlots[1] = (s32)(D_801F5214[i] = allocHeapBlock(20 * sizeof(PolyF4), 0x28));
        poly = D_801F5214[i];
        for (j = 0; j < 20; j++, poly++) {
            func_80067784(poly);
            poly->r0 = 0x7F;
            poly->g0 = 0x44;
            poly->b0 = 0xC;
            SetSemiTrans(poly, 1);
            SetDrawTPage(&D_801F5084[i][j], 0, 0, 0x20);
        }
    }
}

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

void func_801EC51C(void) {
    s32 t = D_801F4E48.unk3D4;

    D_801F4E48.unk3F6 = (t * 270 + (15 - t) * 136) / 15;
    D_801F4E48.unk3F8 = (t * 183 + (15 - t) * 96) / 15;
}

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

void func_801F1C84(void);

void func_801F0ECC(u8 value) {
    D_801F4AFF = value;
    func_801F1C84();
    func_80014C08(5);
    D_801F4588.unk10E = 0;
    D_801F4588.unk11C = 0;
}

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

void func_801F19C4(UiWindow *window) {
    if (D_801F4AFF == 0) {
        drawText(window->originX + 2, window->originY + 1, (s32)"Earned a Prize Pack", 7, 0);
        drawText(window->originX + 0x92, window->originY + 1, (s32)D_8006E31C[D_801F46A5], 6, 0);
    } else {
        drawText(window->originX + 2, window->originY + 1, (s32)"Received", 7, 0);
    }
}

INCLUDE_ASM("asm/saiseg/nonmatchings/saiseg", func_801F1A80);

void func_801F1B5C(void) {
    s32 i;
    s32 clearNew;
    u8 flags;

    for (i = 0; i < 3; i++) {
        ((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i] = D_801F4AF8[i];
        if (D_801F4AF8[i] >= 0) {
            flags = ((PlayerProfile *)PLAYER_PROFILES)->cardCollection[((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i]];
            clearNew = 0;
            if (flags & 0x40) {
                flags &= 0x20;
                clearNew = flags != 0;
            }
            ((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] = addCardToCollection(0, ((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i], 1);
            if (clearNew) {
                ((PlayerProfile *)PLAYER_PROFILES)->cardCollection[((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i]] &= ~0x20;
            }
        } else {
            ((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] = 100;
        }
    }
}

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
