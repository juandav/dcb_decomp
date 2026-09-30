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
#include "dcb/archive.h"
#include "dcb/card_render.h"
#include "dcb/prim.h"
#include "dcb/menu.h"
#include "dcb/prim_util.h"
#include "dcb/battle_hud.h"
#include "dcb/game_exit.h"
#include "dcb/sound.h"
#include "dcb/dialog.h"

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

void func_801E4AF4(s8 animate);

typedef struct {
    u8 text[0x3C];
    s16 shown;
    s8 active;
    s8 length;
} TextLine;
s32 func_801DFE70(s32 x, s32 y, TextLine *line, s32 z);
s32 func_801E002C(s16 x, s16 y, s32 arg2);
typedef struct {
    u8 pad[0x8];
    s32 playTime;
    s32 cardRate;
    s32 abilityRate;
    PlayerProfile *profile;
    char *tamerRank;
    char *collectorRank;
    s8 unk20;
} Unk801F4810;
extern Unk801F4810 D_801F4810;
extern char *STR_TAMER_RANKS[8];
extern char *STR_COLLECTOR_RANKS[8];

extern s8 D_801F460A;
extern s8 D_801F4696;
typedef struct {
    s32 unk0[0x40];
    u8 target[2];
    u8 current[2];
    u8 step[2];
    u8 pad106[7];
    s8 text[0x1B];
    s8 length;
    u8 pad129[3];
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
extern WindowDef D_801F3460[5];
void func_801E9988(UiWindow *window, WindowDef *def);
void func_801E9B20(void);
void func_801E9D50(void);
void func_801E9DB0(void);
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
typedef struct {
    DrTPage tpages[2];
    PolyF4 polys[2];
} Fade;
typedef struct {
    Fade fades[2];
    Unk801E8714Entry entries[7];
    s32 unkB8;
    s32 timer;
    u16 delay;
    u8 unkC2;
    u8 unkC3;
    u8 unkC4;
    u8 unkC5;
    u8 unkC6;
    u8 alpha;
    u8 phase;
    u8 brightness;
    u8 unkCA;
} Unk801F4AF0;
extern Unk801F4AF0 *D_801F4AF0;
extern PolyF4 *D_801F5214[2];
extern DrTPage D_801F5084[2][20];
extern u8 D_801F4AFF;
extern u8 D_801F46A5;
extern s16 D_801F4AF8[3];

extern TextLine D_801F4840[3];
typedef struct {
    s32 progress[6];
    s32 blink[6];
    s32 unkB8;
    s32 timer;
    s16 delay[6];
    u8 flags[6];
    u8 pad4A[0x12];
    s8 page;
    s8 selected;
    s8 unkE6;
    u8 pad5F;
} Unk801F4588Sub;
typedef struct {
    DrTPage tpages[2];
    PolyF4 polys[2];
    VECTOR pos;
    SVECTOR rot;
    SVECTOR corners[4];
    s32 unk78;
    s8 state[5];
    s8 unk81;
    s8 unk82;
    s8 cursor;
    s8 count;
    u8 pad85[3];
    Unk801F4588Sub saved;
    s32 openTimer;
    s32 angle;
    u32 unkF0;
    s32 unkF4;
    u8 padF8[4];
    char unkFC[0xD];
    s8 unk109;
    s8 unk10A;
    s8 unk10B;
    s8 unk10C;
    s8 unk10D;
    s8 unk10E;
    s8 unk10F;
    s8 partners[4];
    s8 unk114;
    u8 unk115;
    u8 unk116;
    u8 alpha;
    s8 opening;
    u8 pad119;
    s8 unk11A;
    s8 unk11B;
    s8 unk11C;
    u8 unk11D;
    u8 unk11E;
    u8 unk11F;
    u8 unk120;
    u8 unk121;
} Unk801F4588;
extern Unk801F4588 D_801F4588;
extern void (*D_801F3594[])(void);
extern s8 D_801F4609;
extern ScriptRunner *D_801F4838[1];
extern u8 D_801F4A30;
extern s8 D_801F4A15[24];
extern Sprite3D *D_801F5260[];
void func_801EBA54(Sprite3D *sprite);
Sprite3D *func_801EB754(s32 id);
void func_801EBDD0(Sprite3D *arg0, s16 width, s16 height);
void func_801EBE0C(Sprite3D *sprite, s32 abr);
void func_801EBD8C(Sprite3D *arg0, s32 arg1);
extern UiWindow D_801F4B00;
extern UiWindow D_801F4338;
void func_801E2120(UiWindow *win);
void func_801E28A0(UiWindow *window);
extern u8 D_801F469F;
void func_801EBA34(void *ptr);
void func_801E390C(ScriptRunner *runner);
Script *func_801E4648(u8 *scriptData);
extern UiWindow D_801F4B44;
void func_801DFA6C(UiWindow *window);
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
    POLY_FT4 quads[2][4];
    s32 unk140;
    s32 scroll;
    s32 speed;
    s32 step;
    s16 x;
    s16 y;
    u8 brightness[4];
    u8 pad158[2];
    u8 state;
    u8 loading;
} Unk801F4804;
extern Unk801F4804 *D_801F4804;
extern const char D_801DE6B0[];
extern UiWindow D_801F46B0[5];
extern void (*D_801F3670[5])();
extern u8 D_801F4834;
extern u8 D_801F4808;
extern UiWindow D_801F43D8;
extern UiWindow D_801F4478;
extern UiWindow D_801F4518;
void func_801EA9B0(UiWindow *window);
void func_801EAF8C(UiWindow *window);
void func_801EB0F8(UiWindow *window);
typedef struct {
    s16 type;
    s16 unk2;
    s32 size;
} PackEntry;
typedef struct {
    u8 pad[0x4];
    u8 r;
    u8 g;
    u8 b;
    u8 pad7[0x21];
} Unk801EBD94;
typedef struct {
    s16 x;
    s16 y;
    s8 next[4];
    s8 unlocked;
    u8 pad9;
} MapNode;
typedef struct {
    VECTOR pos;
    SVECTOR rot;
    s32 length;
} MapPath;
typedef struct {
    s8 *route;
    MapNode *node;
    MapNode *target;
    MapPath paths[20];
    DrTPage pathTpages[2][20];
    s8 pathCount;
    u8 pad37D[3];
    s8 selected;
    s8 shown;
    s16 timer;
    DrTPage tpages[2];
    u8 pad394[8];
    PolyF4 fades[2];
    u8 pad3CC[8];
    s32 unk3D4;
    s32 zoom;
    s32 blink;
    s32 unk3E0;
    s32 moving;
    s32 angle;
    s32 distance;
    s16 unk3F0;
    s16 unk3F2;
    s16 slide;
    s16 x;
    s16 y;
    u8 alpha;
    u8 unk3FB;
    u8 unk3FC;
    s8 slideDir;
    s8 unk3FE;
    s8 unk3FF;
    s8 unk400;
    s8 unk401;
    s8 unk402;
    s8 state;
    s8 lastArea;
    s8 area;
    s8 nodeIndex;
    s8 unk407;
    s8 running;
    s8 unk409;
    s8 unk40A;
    s8 unk40B;
} Unk801F4E48;
extern Unk801F4E48 D_801F4E48;
extern s8 D_801F5252;
void func_801ECD2C(void);
void func_801ECE60(void);
void func_801ECEB0(void);
typedef struct {
    s32 timer;
    s16 x;
    s16 y;
    u8 frames;
    u8 frameTime;
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
void func_801F23D8(UiWindow *win);
extern UiWindow D_801F5410;
void func_801F2F04(UiWindow *window);
extern Rect16 D_801F3CE8[];
void func_801EE40C(Sprite3D *sprite, Rect16 *rect, s8 flip);
extern u8 D_801F5244;
extern s8 D_801F524B;
void func_801EC5C4(void);
extern u8 D_801F5242;
extern u8 D_801F5253;
extern u8 D_801F525C;
void func_801EDE58(void);
extern s8 D_801F51C8;
extern MapNode D_801F3C30[16];
s32 func_801EC108(u32 id);
typedef struct {
    s32 offset;
    s8 state;
} Unk801F5258;
extern Unk801F5258 D_801F5258;
typedef struct {
    UiWindow window;
    s16 cardId;
    u16 index;
    u16 clut;
    u16 pad4A;
} CardWindow;
typedef struct {
    UiWindow main;
    CardWindow cards[3];
    RewardWindow rewards[3];
    s32 showRewards;
} RewardScreen;
extern RewardScreen *D_801F5358;
void func_801F19C4(UiWindow *window);
void func_801F1944(RewardWindow *window);
void func_801F1294(CardWindow *window);
void func_801F1C84();
typedef struct {
    u8 pad[0x1020];
    u16 armorFlags;
} SaisegSessionData;
void func_801EE5A8(Sprite3D *arg0, Rect16 *rect);
extern s8 D_801F524E;
extern u8 D_801F5245;
extern s8 D_801F5246;
typedef struct {
    Unk801F4908 unk0;
    Unk801F4588Sub unk12C;
} SaveBlock;
extern s8 D_801F469E;
extern s8 D_801F46A2;
extern u8 D_801F4692;
s32 func_801E354C(void);
void func_801E612C(void);
extern s32 D_801F4628[6];
typedef struct {
    Unk801F4908 unk0;
    Unk801F4588Sub unk12C;
    Chunk *pak;
    void *unk190;
    s32 loading;
    s32 scriptOffset;
    s32 music;
    u8 pad1A0[4];
    s8 area;
    u8 pad1A5;
    u8 unk1A6;
    s8 unk1A7;
    u8 unk1A8;
    s8 unk1A9;
    s8 unk1AA;
} SaisegSession;
#define SESSION ((SaisegSession *)((SessionData *)D_8006E054)->unk100C)
#define SESSION_SUB (((SessionData *)D_8006E054)->unk100C)
extern s8 D_801F4830;
extern s16 D_801F354C;
extern char *D_801F3564[];
extern u8 D_801F46A9;
extern u8 D_801F46A7;
typedef struct {
    s16 col;
    s16 prevCol;
    s16 row;
    s16 prevRow;
    u8 active;
    u8 pad9;
    char text[0xD];
    u8 unk17;
    u8 cursor;
    u8 mode;
    s8 sel;
    s8 prevSel;
    s8 result;
} Unk801F4568;
extern Unk801F4568 D_801F4568;
extern CursorHighlight D_801F44C8;
void func_801E0D54(void);
void func_801E55F8(void);
void func_801E0D8C(s32 *regs);
void func_801E186C(void);
extern u8 D_801F4B7C;
extern Rect16 D_801F34EC[6];
int MoveImage2(Rect16 *rect, int x, int y);
s32 func_801E460C(ScriptRunner *runner);
void func_801E0724(void);
void func_801E57C4(void);
void func_801DFB6C(TextLine *arg0);
s32 func_801DFC00(u8 *src);
void func_801E7D1C(void);
void func_801E5714(s8 index);
void func_801EBD94(Unk801EBD94 *arg0, u8 value);
extern s8 D_801F466D;
extern u8 D_801F4654[6];
s32 func_801E8980(void);
extern WindowDef D_801F351C;
extern Menu D_801F3538;
extern CursorHighlight D_801F4388;
void func_801E16B0(void);
void func_801E15EC(UiWindow *window, WindowDef *def, s32 count);
void func_801E1A54(void);
extern s8 D_801F4693;
extern s8 D_801F469C;
void func_801E550C(void);
void func_801E5BA0(void);
void func_801E108C(s32 close);
void func_801E75D4(void);
s32 func_801E2E64(void);
s32 func_801E2E90(void);
s8 func_801E6A68(void);
void func_801E6464(void);
s32 func_801E6EF0(void);
void func_801E5B1C(void);
void func_801E602C(void);
void func_801E55AC(void);
void func_801ECCA4(s8 mode);
void func_801F0A08(FrameBuffer *fb);
long ratan2(long y, long x);
int csqrt(int a);
extern u8 D_801F5251;
void func_801F0078(void);
void func_801EB628(void);
extern void (*D_801F3D00[])(void);
extern Rect16 D_801F35BC[15];
extern char *D_801F3634[];
TextLine *func_801DFBC4(TextLine *arg0);
void rollRewardCards(s32 player, s32 pack);
void func_801F1B5C(void);
void func_801F1A80(void);
void func_801F0F08(void);
void func_801F0F5C(s32 x, s32 y, s32 vramX, s32 vramY, s32 frame, u16 clut, u8 *rgb);
extern s8 D_801F5247;
extern s8 D_801F5248;
extern s32 D_801F4610[6];
void func_801E8238(void);
void func_801E7A00(void);
void func_801E9864(s32 index, s32 task);
s32 func_801E8714(void);
s32 func_801E87E8(void);
void func_801E8C48(void);
void func_801E818C(void);
extern s16 D_801F4C58;
extern s32 D_801F521C;
extern CursorHighlight D_801F4428;
extern char D_801F3684[];
s32 func_801EA230(void);
typedef struct {
    u8 cursor;
    s8 done;
} PartnerCursor;
typedef struct {
    s32 state;
    u8 count;
} PartnerList;
extern PartnerCursor D_801F5400;
extern PartnerList D_801F5408;
extern CursorHighlight D_801F53B0;
extern u8 D_801F3D34[4];
void func_801F208C(s32 x, s32 y, s32 vramX, s32 vramY, s32 frame, u8 *rgb, s32 z, s32 palette);
extern void (*D_801F3D0C[])(void);
extern s8 D_801F3CD0[3][7];
extern s8 D_801F5250;
void func_801EBFB4(s32 useMap);
void func_801EC1D8(s32 state);
void func_801EC080(void);
void func_801EBE94(void);
void func_801EFF24(void);
void func_801F0D20(s8 arg0);
void func_801EFA34(void);
void func_801EFC54(void);
void func_801EF1B4(void);
void func_801EF3F4(void);
void func_801EF750(void);
void func_801F0D84(void);
void func_801EFE84(void);
void func_801EFED4(void);
void func_801ECF94(s8 mode);
void func_801EDB90(void);
void func_801EF7C4(void);
void func_801EF25C(void);
void func_801EF4CC(void);
void func_801F0E8C(void);
void func_801F0EAC(void);
void func_801E4D80(s32 resume);
extern s8 D_801F524A;
extern s8 D_801F4695;
extern s32 D_801F4678;
ScriptRunner *func_801E4700(void);
s32 *func_801E46AC(s32 count);
void func_801E0B70(void);
void func_801E1254(void);
void func_801E1360(void);
void func_801E54C4(void);
void func_801E77F4(void);
void func_801E8DD8(s32 index);
void func_801E49A4(void);
void func_801E4878(void);
void func_801E9F08(void);
void func_801E0A74(void);
void func_801F00F4(s32 resume, s32 arg1);
typedef struct {
    char *name;
    u8 unk4[8];
} PartInfo;
extern u8 D_801F5458[];
extern PartInfo D_801F3D38[];
extern const u8 D_801DE328[][3];
s32 func_801E1ABC(Menu *menu);
typedef struct {
    u8 pad[0x30];
    s32 scriptOffset;
} ProfileSave;
extern u8 D_801F4A38[];
extern s8 D_801F4697;
extern s8 D_801F4698[4];
void func_801E1190(ScriptRunner *runner);
void func_801F2ABC(s32 task);
void func_801E84BC(void);
void func_801EB1F8(char *word);
void func_801E9F28(void);
void func_801E9F18(void);
void func_801E0174(s8 mode);
void func_801E0390(s8 id);
void func_801E0C54(s32 ch);
void func_801E09F4(void);
void func_801F0ECC(u8 value);
void func_801F32CC(s32 ability, s32 task);
void func_801E9E10(s32 mode, s32 task);
extern const char D_801DE804[];
extern char *D_801F36E0[10];
int strcmp(char *a, char *b);
void func_801EB198(void);
void func_801EE038(void);
void func_801EBD28(Sprite3D *arg0, s16 dx, s16 dy);
void func_801F082C(void);
extern const u8 D_801DEA14[];
void func_801F2A8C(void);
void func_801E0988(s32 index);

void func_801DFA6C(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;
    s32 found;
    char buf[0x48];

    x = window->originX;
    y = window->originY;
    z = window->z;
    if (D_801F4830 == 2) {
        drawText(x + 2, y, (s32)D_801F3564[D_801F354C], 7, z);
        return;
    }
    if (D_801F4588.unk120 != 0 && D_801F4588.unk11F == 0) {
        D_801F4588.unk121++;
        if (D_801F4588.unk121 & 0x10) {
            drawIcon(x + 0x11C, y + 0x1A, 0, 0x1B, z);
        }
    } else {
        D_801F46A9 = 0;
    }
    found = func_801E002C(x, y, z);
    D_801F46A7 = found;
}

void func_801DFB6C(TextLine *arg0) {
    s32 i;

    for (i = 0; i < 3; i++, arg0++) {
        arg0->active = -1;
        bzero(arg0, 0x3C);
    }
}

TextLine *func_801DFBC4(TextLine *arg0) {
    s32 i;

    for (i = 0; i < 3; i++, arg0++) {
        if (arg0->active == -1) {
            arg0->active = 0;
            arg0->shown = 0;
            return arg0;
        }
    }
    return NULL;
}

s32 func_801DFC00(u8 *src) {
    u8 *name = (u8 *)((PlayerProfile *)PLAYER_PROFILES)->name;
    TextLine *slot = func_801DFBC4(D_801F4840);
    u8 *dst;
    s32 i;
    s32 found;

    if (slot == NULL) {
        return -1;
    }
    slot->text[0] = '*';
    slot->text[1] = 'w';
    slot->text[2] = '2';
    dst = &slot->text[3];
    while (*src != 0) {
        if (*src < 0x81 || *src >= 0x99) {
            if (*src == '*') {
                if (src[1] == 'h' && src[2] == '0') {
                    src += 3;
                    for (i = 0; i < 12 && *name != 0; i++) {
                        *dst++ = *name++;
                    }
                    continue;
                }
            } else if (*src == '\\') {
                found = 0;
                for (i = 1; i < 5; i++) {
                    if (!found && src[i] == 0) {
                        found = 1;
                        break;
                    }
                }
                if (found != 1 && src[1] == '0' && src[2] == 'x' && src[3] == '2' && src[4] == '2') {
                    src += 5;
                    *dst++ = '"';
                    continue;
                }
            }
        } else {
            *dst++ = *src++;
        }
        *dst++ = *src++;
    }
    *dst = 0;
    for (dst = slot->text, i = 0; i < 60 && *dst != 0; i++, dst++) {
    }
    if (i < 60) {
        slot->active = 0;
    }
    slot->length = i;
    for (i = 0; i < 3 && slot != D_801F4840; i++, slot--) {
    }
    return i;
}

/* not referenced by any code */
const s32 D_801DDF38 = 6;

/* where the five windows of D_801F46B0 open */
WindowDef D_801F3460[5] = {
    { { 0xA0, 0x32, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0xF, 0x82, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0x1E, 0x14, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0x5A, 0xB4, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0xA0, 0x78, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
};

/* VRAM areas func_801E4AF4 moves with MoveImage2 */
Rect16 D_801F34EC[6] = {
    { 0x220, 0xE2, 0x20, 2 },
    { 0x220, 0xE5, 0x20, 1 },
    { 0x220, 0xEC, 0x20, 1 },
    { 0x220, 0xF4, 0x20, 1 },
    { 0x230, 0xEB, 0x20, 1 },
    { 0x280, 0x1FC, 0x100, 3 },
};

WindowDef D_801F351C = { { 0x88, 0x14, 0xA4, 0xD }, 0x80, 0x31, 0, 0, 8 };

/* the rows of the player's data screen */
Menu D_801F3538 = { NULL, NULL, { 10, 48, 300, 126 }, 0, -1, 0, -1, 0xa, 0x16, 72, 12, 0, 15, 0, 1, 0, 14, 0, 0, 0 };

/* the help line for each row of D_801F3538 */
char *D_801F3564[12] = {
    "*w1Player's Name.",
    "*w1Tamer Rank is based on Wins. There are 8 \nRanks. \"Beginner Tamer\" with 0 wins\nto \"Invincible Tamer\" with 500+ Wins.",
    "*w1Collector Rank is set by the number of\nCards collected. 7 Titles from \"General \n\tPublic\" to the highest, plus one more.",
    "*w1This shows how far you are in the game.\nCan you achieve 100%?",
    "*w1This is the ratio of Cards collected.",
    "*w1Collected Digi-Parts ratio, 128 total.",
    "*w1Watch your wins and losses in Digi-land.\n\tRemember the Cards you were beaten by.\nIt will make you a better Card Tamer.",
    "*w1Wins & losses in \"Battle with Friends.\"\nDo battle with all your friends.\nEverybody will love it! It's guaranteed!",
    "*w1This is the first Deck you have.",
    "*w1This is the second Deck you have.",
    "*w1This is the third Deck you have.",
    "*w1These are Partners & Digi-Eggs you own.\nVeemon alone has 3 Digi-Eggs.",
};

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
    TextLine *entry = D_801F4840;
    s32 found = 0;
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->active != -1) {
            if (func_801DFE70(x + 4, y + i * 13, entry, arg2) == 1) {
                found = 1;
                break;
            }
        }
    }
    return found;
}

void func_801E0104(void) {
    TextLine *entry = D_801F4840;
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->active != -1) {
            entry->shown = entry->length;
        }
    }
}

void func_801E0150(TextLine *arg0) {
    s32 i;

    for (i = 0; i < 3; i++, arg0++) {
        arg0->active = -1;
    }
}

void func_801E0174(s8 mode) {
    s32 i;

    D_801F4588.unk82 = mode;
    D_801F4588.unk78 = 0;
    D_801F4588.cursor = 0;
    D_801F4588.count = 0;
    D_801F4588.unk81 = 0;
    for (i = 0; i < 5; i++) {
        D_801F4604[i] = 0;
    }
    if (mode == 1) {
        D_801F5260[25] = func_801EB754(0x15);
        D_801F5260[25]->pos.vx = -0x66;
        D_801F5260[25]->pos.vy = -0x13;
        for (i = 0; i < 5; i++) {
            D_801F5260[i + 26] = func_801EB754(0x14);
            D_801F5260[i + 26]->pos.vx = -0x68;
            D_801F5260[i + 26]->pos.vy = i * 16 - 0x37;
            D_801F5260[i + 26]->corners[2].vy++;
            D_801F5260[i + 26]->corners[3].vy++;
            func_801EBD8C(D_801F5260[i + 26], 0x21);
            func_801EBE0C(D_801F5260[i + 26], 0);
        }
    } else {
        D_801F5260[25] = func_801EB754(0x16);
        D_801F5260[25]->pos.vx = -0x66;
        D_801F5260[25]->pos.vy = -0x2A;
        for (i = 0; i < 3; i++) {
            D_801F5260[i + 26] = func_801EB754(0x3D);
            D_801F5260[i + 26]->pos.vx = -0x68;
            D_801F5260[i + 26]->pos.vy = i * 16 - 0x3F;
            D_801F5260[i + 26]->corners[2].vy++;
            D_801F5260[i + 26]->corners[3].vy++;
            func_801EBD8C(D_801F5260[i + 26], 0x21);
            func_801EBE0C(D_801F5260[i + 26], 0);
        }
    }
    func_801EBD8C(D_801F5260[25], 0x20);
    func_801EBE0C(D_801F5260[25], 1);
}

void func_801E0390(s8 id) {
    Rect16 rect;

    if (((SessionData *)D_8006E054)->unk1027 == 1 && id == 15) {
        D_801F4588.state[D_801F4588.count] = 2;
    }
    if (D_801F460A != 0) {
        rect.x = 0x300;
        rect.y = id << 4;
        rect.w = 0x58;
    } else {
        rect.x = 0x318;
        rect.y = (id - 12) << 4;
        rect.w = 0x40;
    }
    rect.h = 0x10;
    func_801EE5A8(D_801F5260[D_801F4588.count + 26], &rect);
    D_801F4588.count++;
}

void func_801E0480(void) {
    D_801F4588.unk78++;
    if (D_801F4588.unk78 > 20) {
        D_801F4588.unk78 = 20;
        D_801F4588.unk81 = 1;
    }
}

void func_801E04B8(void) {
    if (PAD_STATES[0]->repeat & 0x4000) {
        D_801F4588.cursor++;
        playSoundEffect(2);
    } else if (PAD_STATES[0]->repeat & 0x1000) {
        D_801F4588.cursor--;
        playSoundEffect(2);
    } else if (PAD_STATES[0]->pressed & 0x10) {
        playSoundEffect(1);
        D_801F4609 = 2;
        D_801F4838[0]->regs[1] = -1;
    } else if (PAD_STATES[0]->pressed & 0x40) {
        if (D_801F4588.state[D_801F4588.cursor] != 2) {
            playSoundEffect(0);
            D_801F4588.unk81 = 2;
            D_801F4838[0]->regs[1] = D_801F4588.cursor + 1;
        }
    }
    if (D_801F4588.cursor < 0) {
        D_801F4588.cursor = D_801F4588.count - 1;
    }
    if (D_801F4588.cursor >= D_801F4588.count) {
        D_801F4588.cursor = 0;
    }
}

void func_801E0650(void) {
    if (--D_801F4588.unk78 < 0) {
        D_801F4588.unk78 = 0;
        D_801F4588.unk81 = 3;
        D_801F4588.count = 0;
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

void func_801E0770(void) {
    s32 count;
    s32 x;
    s32 i;

    count = 5;
    if (D_801F460A == 0) {
        count = 3;
    }
    if (D_801F4588.count != 0) {
        x = (D_801F4588.unk78 * -102 + (20 - D_801F4588.unk78) * -212) / 20;
        for (i = 0; i < count; i++) {
            if (D_801F4588.unk82 == 0) {
                D_801F5260[i + 26]->pos.vx = x + 3;
            } else {
                D_801F5260[i + 26]->pos.vx = x;
            }
            if (D_801F4588.unk81 == 1 && D_801F4588.cursor == i) {
                if (D_801F4588.state[i] != 0) {
                    D_801F5260[i + 26]->quads[0].clut = getClut(0x200, 0xF1);
                    D_801F5260[i + 26]->quads[1].clut = getClut(0x200, 0xF1);
                } else {
                    D_801F5260[i + 26]->quads[0].clut = getClut(0x200, 0xF0);
                    D_801F5260[i + 26]->quads[1].clut = getClut(0x200, 0xF0);
                }
            } else if (D_801F4588.state[i] != 0) {
                D_801F5260[i + 26]->quads[0].clut = getClut(0x200, 0xF2);
                D_801F5260[i + 26]->quads[1].clut = getClut(0x200, 0xF2);
            } else {
                D_801F5260[i + 26]->quads[0].clut = getClut(0x200, 0xEF);
                D_801F5260[i + 26]->quads[1].clut = getClut(0x200, 0xEF);
            }
            func_801EBA54(D_801F5260[i + 26]);
        }
        D_801F5260[25]->pos.vx = x;
        func_801EBA54(D_801F5260[25]);
    }
}

void func_801E0988(s32 index) {
    s16 flagIds[6] = { 0x126, 0x12A, 0x12D, 0x133, 0x130, 0x136 };

    D_801F4838[0]->regs[flagIds[index]] = 1;
}

void func_801E09F4(void) {
    if (D_801F4838[0]->regs[266] != 0) {
        ((PlayerProfile *)PLAYER_PROFILES)->scriptFlags |= 1;
    }
    if (D_801F4838[0]->regs[267] != 0) {
        ((PlayerProfile *)PLAYER_PROFILES)->scriptFlags |= 2;
    }
}

void func_801E0A74(void) {
    s32 flag;
    s32 i;
    s32 bit;

    flag = 12;
    for (i = 0; i < 12; i++) {
        for (bit = 0; bit < 32; bit++) {
            if (D_801F4838[0]->regs[flag++] != 0) {
                ((PlayerProfile *)PLAYER_PROFILES)->unk23FC[i] |= 1 << bit;
            } else {
                ((PlayerProfile *)PLAYER_PROFILES)->unk23FC[i] &= ~(1 << bit);
            }
            if (flag >= 0x16B) {
                break;
            }
        }
    }
    for (flag = 0x16B, bit = 0; bit < 9 && flag < 0x175; bit++, flag++) {
        ((PlayerProfile *)PLAYER_PROFILES)->unk242C[bit] = D_801F4838[0]->regs[flag];
    }
}

void func_801E0B70(void) {
    s32 i;
    s32 bit;
    s32 flag;

    flag = 12;
    for (i = 0; i < 12; i++) {
        for (bit = 0; bit < 32 && flag < 0x16B; bit++, flag++) {
            D_801F4838[0]->regs[flag] = ((u32)((PlayerProfile *)PLAYER_PROFILES)->unk23FC[i] >> bit) & 1;
        }
    }
    for (flag = 0x16B, bit = 0; bit < 9 && flag < 0x174; bit++, flag++) {
        D_801F4838[0]->regs[flag] = ((PlayerProfile *)PLAYER_PROFILES)->unk242C[bit];
    }
}

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

void func_801E0D8C(s32 *regs) {
    s16 ids[13] = { 0x127, 0x128, 0x129, 0x12B, 0x12C, 0x12E, 0x12F, 0x134, 0x135, 0x131, 0x132, 0x137, 0x138 };
    s32 i;
    s32 partner;
    s32 slot;
    s32 first;

    ((SaisegSessionData *)D_8006E054)->armorFlags = 0;
    for (slot = 0; slot < 13; slot++) {
        if (regs[ids[slot]] != 0) {
            ((SaisegSessionData *)D_8006E054)->armorFlags |= 1 << slot;
        }
    }
    for (slot = 0; slot < 3; slot++) {
        partner = getSlotPartnerIndex(0, slot);
        if (partner == -1) {
            continue;
        }
        if (partner == 0) {
            for (i = 0; i < 3; i++) {
                if (regs[ids[i]] != 0) {
                    unlockPartnerArmor(0, partner, i);
                }
            }
        } else {
            for (i = 0, first = partner * 2 + 1; i < 2; i++) {
                if (regs[ids[i + first]] != 0) {
                    unlockPartnerArmor(0, partner, i);
                }
            }
        }
    }
}

void func_801E0F90(UiWindow *window, Rect16 pos, s32 label, s32 style, s32 brightness) {
    Rect16 rect;
    Rect16 unused; /* never used, but the original frame has room for it */
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

void func_801E108C(s32 close) {
    Rect16 rect;
    Rect16 pos = { 0xA1, 0xCF, 0x12A, 0x28 };
    char *title = "MESSAGE";

    if (close == 0) {
        func_801E0F90(&D_801F4B44, pos, (s32)title, 8, 0x51);
        animateWindowTo(&D_801F4B44, (Rect16 *)-1);
    } else {
        rect.x = pos.x - pos.w / 2;
        rect.y = pos.y - pos.h / 2;
        rect.w = pos.w & ~1;
        rect.h = pos.h & ~1;
        animateWindowTo(&D_801F4B44, &rect);
    }
}

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

void func_801E1254(void) {
    Rect16 rect;

    D_801F5260[21] = func_801EB754(0x1E);
    D_801F5260[21]->pos.vx = 0x33;
    D_801F5260[21]->pos.vy = -0xBC;
    func_801EBD8C(D_801F5260[21], 0x21);
    D_801F5260[22] = func_801EB754(0x1F);
    D_801F5260[22]->pos.vx = 0x34;
    D_801F5260[22]->pos.vy = -0xBB;
    func_801EBD8C(D_801F5260[22], 0x21);
    rect.x = ((PlayerProfile *)PLAYER_PROFILES)->unkE / 6 * 24 + 0x340;
    rect.y = ((PlayerProfile *)PLAYER_PROFILES)->unkE % 6 * 20;
    rect.w = 0x60;
    rect.h = 0x14;
    func_801EE5A8(D_801F5260[22], &rect);
}

void func_801E1360(void) {
    s32 unused[2];

    D_801F4588.unk11A = -1;
    D_801F4588.unk11B = -1;
    D_801F5260[23] = func_801EB754(6);
    D_801F5260[23]->pos.vx = -0x5F;
    D_801F5260[23]->pos.vy = -0x98;
    func_801EBD8C(D_801F5260[23], 0x21);
    D_801F5260[24] = func_801EB754(7);
    D_801F5260[24]->pos.vx = -0x5F;
    D_801F5260[24]->pos.vy = -0x60;
    D_801F5260[24]->pos.vy = -0x97;
    func_801EBD8C(D_801F5260[24], 0x22);
    D_801F4588.unkF4 = 0;
}

void func_801E140C(void) {
    func_801EBA54(D_801F5260[22]);
    func_801EBA54(D_801F5260[21]);
}

void func_801E1448(void) {
    Rect16 rect;

    if (D_801F46A2 == -1) {
        rect.x = 0x2E0;
        rect.y = 0x90;
    } else {
        rect.x = 0x2E0;
        rect.y = D_801F46A2 * 24;
    }
    rect.w = 0x74;
    rect.h = 0x18;
    if (D_801F4588.unk11B != D_801F4588.unk11A) {
        D_801F4588.unkF4--;
        if (D_801F4588.unkF4 < 0) {
            D_801F4588.unkF4 = 0;
            D_801F4588.unk11B = D_801F4588.unk11A;
            func_801EE5A8(D_801F5260[24], &rect);
        }
    } else if (D_801F4588.unk11A != -1) {
        D_801F4588.unkF4++;
        if (D_801F4588.unkF4 > 30) {
            D_801F4588.unkF4 = 30;
        }
    } else {
        D_801F4588.unkF4 = 0;
    }
    D_801F5260[23]->pos.vy = (D_801F4588.unkF4 * -97 + (30 - D_801F4588.unkF4) * -152) / 30;
    D_801F5260[24]->pos.vy = D_801F5260[23]->pos.vy + 2;
    func_801EBA54(D_801F5260[24]);
    func_801EBA54(D_801F5260[23]);
}

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

/* the three icons drawn for each partner */
const u8 D_801DE328[6][3] = {
    { 1, 2, 9 },
    { 3, 5, 0 },
    { 4, 6, 0 },
    { 7, 6, 0 },
    { 8, 1, 0 },
    { 1, 7, 0 },
};

void (*D_801F3594[4])(void) = {
    func_801E0480,
    func_801E04B8,
    func_801E0650,
    func_801E0684,
};

/* not referenced by any code: "後藤豪太" in Shift-JIS */
char D_801F35A4[24] = "\x8C\xE3\x93\xA1\x8D\x8B\x91\xBE";

/* the cursor of each row of D_801F3538 */
Rect16 D_801F35BC[15] = {
    { 0x64, 0, 0x18, 0xE },
    { 0x64, 0xE, 0x3C, 0xE },
    { 0x64, 0, 0x48, 0xE },
    { 0x64, 0xE, 0x30, 0xE },
    { 0x64, 0, 0x60, 0xE },
    { 0x64, 0xE, 0x60, 0xE },
    { 0x15, 0, 0x60, 0xE },
    { 0x15, 0xE, 0x48, 0xE },
    { 0x15, 0, 0x30, 0xE },
    { 0x15, 0xE, 0x30, 0xE },
    { 0x15, 0, 0x30, 0xE },
    { 0x15, 0xE, 0x90, 0xE },
    { 0x15, 0, 0x18, 0xE },
    { 0x15, 0xE, 0x3C, 0xE },
    { 0x64, 0, 0x18, 0xE },
};

/* the labels of the player's data screen */
char *D_801F3634[15] = {
    "Name",
    "Battle Title",
    "Collector Title",
    "Game Completion",
    "Card Collection",
    "Digi-Parts Stock",
    "COM Battle Stats",
    "2P Battle Stats",
    "Deck 1",
    "Deck 2",
    "Deck 3",
    "Partner Cards & Digi-Eggs",
    "Wins",
    "Losses",
    "Deck",
};

void func_801E186C(void) {
    func_801E16B0();
    if (D_801F4838[0]->regs[15] != 0) {
        func_801E15EC(&D_801F4338, &D_801F351C, 1);
    }
    openMenu(&D_801F3538, &D_801F4B00, &D_801F4388, (Bytes4 *)-1);
    D_801F4B00.label = (s32)"PLAYER'S DATA";
    func_80014C08(1);
    playSoundEffect(3);
    addFrameCallback((s32)func_801E1A54);
    while (1) {
        func_80014C08(1);
        if (PAD_STATES[0]->pressed & 0x10) {
            break;
        }
        if ((PAD_STATES[0]->pressed & 0x20) && D_801F4838[0]->regs[15] != 0) {
            D_801F4838[0]->regs[0] = 0;
            SESSION->scriptOffset = D_801F4838[0]->script->pc - D_801F4838[0]->script->start;
            SESSION->unk1A9 = 2;
            D_801F4588.unkF0 = 2;
            D_801F4588.unk10D = 6;
            break;
        }
    }
    playSoundEffect(4);
    animateWindowTo(&D_801F4B00, (Rect16 *)-1);
    if (D_801F4838[0]->regs[15] != 0) {
        animateWindowTo(&D_801F4338, (Rect16 *)-1);
    }
    func_80014C08(15);
    removeFrameCallback((s32)func_801E1A54);
    D_801F4588.unk10E = 0;
    D_801F4588.unk11C = 0;
    D_801F4830 = 0;
}

void func_801E1A54(void) {
    drawWindow(&D_801F4B00, func_801E2120, 0x18);
    if (D_801F4838[0]->regs[15] != 0) {
        drawWindow(&D_801F4338, func_801E28A0, 0x18);
    }
}

s32 func_801E1ABC(Menu *menu) {
    UiWindow *win;
    CursorHighlight *highlight;
    s16 target[4];

    win = menu->win;
    highlight = menu->cursor;
    menu->moved = 0;
    if (menu->active != 0) {
        highlight->brightness = 0x80;
        if (menu->nrows | menu->rowH) {
            if (PAD_STATES[menu->pad]->repeat & 0x5003) {
                playMenuSound(2);
            }
            if (PAD_STATES[menu->pad]->repeat & 0x1000) {
                menu->moved = 1;
                if (--menu->row < 0) {
                    scrollWindowTo(&win->originX, 0, win->view.h - win->rect.h);
                    menu->row = 11;
                } else {
                    if (menu->row == 0) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    if (menu->row * menu->rowH < win->scroll[3]) {
                        scrollWindowTo(&win->originX, 0, menu->row * menu->rowH);
                    }
                }
            } else if (PAD_STATES[menu->pad]->repeat & 0x4000) {
                menu->moved = 1;
                if (++menu->row >= 12) {
                    scrollWindowTo(&win->originX, 0, 0);
                    menu->row = 0;
                } else if (menu->row == 11) {
                    PAD_STATES[menu->pad]->repeatEnabled = 0;
                    scrollWindowTo(&win->originX, 0, (menu->row + 13) * menu->rowH - win->rect.h);
                } else if (menu->row * menu->rowH >= win->scroll[3] + win->rect.h) {
                    scrollWindowTo(&win->originX, 0, (menu->row + 1) * menu->rowH - win->rect.h);
                }
            } else if (PAD_STATES[menu->pad]->repeat & 0x1) {
                menu->moved = 1;
                menu->row -= (win->rect.h + menu->rowH - 1) / menu->rowH;
                if (menu->row < 0) {
                    PAD_STATES[menu->pad]->repeatEnabled = 0;
                    scrollWindowTo(&win->originX, 0, 0);
                    menu->row = 0;
                } else {
                    if (menu->row == 0) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    scrollWindowTo(&win->originX, 0, win->scroll[3] - (win->rect.h + menu->rowH - 1) / menu->rowH * menu->rowH);
                }
            } else if (PAD_STATES[menu->pad]->repeat & 0x2) {
                menu->moved = 1;
                menu->row += (win->rect.h + menu->rowH - 1) / menu->rowH;
                if (menu->row >= menu->nrows) {
                    PAD_STATES[menu->pad]->repeatEnabled = 0;
                    scrollWindowTo(&win->originX, 0, win->view.h - win->rect.h);
                    menu->row = 11;
                } else {
                    if (menu->row == menu->nrows - 1) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    scrollWindowTo(&win->originX, 0, win->scroll[3] + (win->rect.h + menu->rowH - 1) / menu->rowH * menu->rowH);
                    if (menu->row >= 12) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                        menu->row = 11;
                    }
                }
            }
        }
    } else {
        highlight->brightness = 0x40;
    }
    if (menu->row != menu->prevRow || menu->col != menu->prevCol) {
        menu->col = D_801F35BC[menu->row].x - 10;
        menu->colW = 1;
        menu->prevCol = menu->col;
        menu->prevRow = menu->row;
        target[0] = (win->rect.x - win->scroll[2]) + menu->ox + menu->col * menu->colW - 10;
        target[1] = (win->rect.y - win->scroll[3]) + menu->oy + menu->row * menu->rowH;
        target[2] = measureText(D_801F3634[menu->row]);
        target[3] = menu->ch;
        moveCursorHighlight(menu->cursor, (Rect16 *)target);
    }
    drawCursorHighlight(highlight, win->z);
    return menu->col + menu->row * menu->ncols;
}

void func_801E2120(UiWindow *win) {
    /* not literals: GCC would share func_801F1294's identical string */
    static const char rateFormat[] = "*s0%3d.%1d%%";
    static const char countFormat[] = "*s0%3d";
    Rect16 uv;
    char buf[0x48];
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 j;
    s32 partner;

    x = win->originX - 0x14;
    y = win->originY + 2;
    z = win->z;
    drawText(x + 0x62, y, (s32)D_801F3634[0], 6, z);
    drawText(x + 0x132 - measureText(D_801F4810.profile->name), y, (s32)D_801F4810.profile->name, 7, z);
    drawText(x + 0x62, y + 0xE, (s32)D_801F3634[1], 6, z);
    drawText(x + 0x132 - measureText(D_801F4810.tamerRank), y + 0xE, (s32)D_801F4810.tamerRank, 7, z);
    drawText(x + 0x62, y + 0x1C, (s32)D_801F3634[2], 6, z);
    drawText(x + 0x132 - measureText(D_801F4810.collectorRank), y + 0x1C, (s32)D_801F4810.collectorRank, 7, z);
    drawText(x + 0x62, y + 0x2A, (s32)D_801F3634[3], 6, z);
    sprintf(buf, rateFormat, D_801F4810.playTime / 10, D_801F4810.playTime % 10);
    drawText(x + 0x105, y + 0x2A, (s32)buf, 7, z);
    drawText(x + 0x62, y + 0x38, (s32)D_801F3634[4], 6, z);
    sprintf(buf, rateFormat, D_801F4810.cardRate / 10, D_801F4810.cardRate % 10);
    drawText(x + 0x105, y + 0x38, (s32)buf, 7, z);
    drawText(x + 0x62, y + 0x46, (s32)D_801F3634[5], 6, z);
    sprintf(buf, rateFormat, D_801F4810.abilityRate / 10, D_801F4810.abilityRate % 10);
    drawText(x + 0x105, y + 0x46, (s32)buf, 7, z);
    drawText(x + 0x15, y + 0x54, (s32)D_801F3634[6], 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->battleWins);
    drawText(x + 0x89, y + 0x54, (s32)buf, 7, z);
    drawText(x + 0xA3, y + 0x54, (s32)"Wins", 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->battleLosses);
    drawText(x + 0xC0, y + 0x54, (s32)buf, 7, z);
    drawText(x + 0xDB, y + 0x54, (s32)"Losses", 6, z);
    drawText(x + 0x15, y + 0x62, (s32)D_801F3634[7], 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->versusWins);
    drawText(x + 0x89, y + 0x62, (s32)buf, 7, z);
    drawText(x + 0xA3, y + 0x62, (s32)"Wins", 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->versusLosses);
    drawText(x + 0xC0, y + 0x62, (s32)buf, 7, z);
    drawText(x + 0xDB, y + 0x62, (s32)"Losses", 6, z);
    for (i = 0; i < 3; i++) {
        drawText(x + 0x15, y + (i + 8) * 14, (s32)D_801F3634[i + 8], 6, z);
        if (((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].inUse != 0) {
            sprintf(buf, "%s %s", ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].unk1, D_801F3634[14]);
            drawText(x + 0x46, y + (i + 8) * 14, (s32)buf, 7, z);
            sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].unk108[1]);
            drawText(x + 0xCA, y + (i + 8) * 14, (s32)buf, 7, z);
            drawText(x + 0xE2, y + (i + 8) * 14, (s32)D_801F3634[12], 6, z);
            sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].unk108[2]);
            drawText(x + 0xFE, y + (i + 8) * 14, (s32)buf, 7, z);
            drawText(x + 0x116, y + (i + 8) * 14, (s32)D_801F3634[13], 6, z);
        }
    }
    drawText(x + 0x15, y + 0x9A, (s32)D_801F3634[11], 6, z);
    x += 0xC;
    for (i = 0; i < 3; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId != 0) {
            drawText(x + 0x15 + i * 84, y + 0xA8, (s32)((PlayerProfile *)PLAYER_PROFILES)->partners[i].card[0].name, 7, z);
            drawLargeText(x + 0x15 + i * 84, y + 0xB9, (s32)"RANK", 6, z);
            sprintf(buf, "%2d", (s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level);
            drawText(x + 0x3D + i * 84, y + 0xB6, (s32)buf, 7, z);
            for (j = 0; j < 3; j++) {
                if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].unlockedArmors[j] != 0) {
                    partner = getSlotPartnerIndex(0, i);
                    drawIcon(x + 0x1D + i * 84 + j * 13, y + 0xC4, 0, D_801DE328[partner][j] + 0x1B, z);
                }
            }
        }
    }
    uv.x = 0;
    uv.y = 0x82;
    uv.w = 0x40;
    uv.h = 0x38;
    drawTexturedSprite(x + 0xC, y + 0xC, &uv, 0x8B, 0x3EA0, z, 0x80, -1);
    func_801E1ABC(&D_801F3538);
}

void func_801E28A0(UiWindow *window) {
    char buf[0x48];

    drawText(window->originX + 6, window->originY + 1, (s32)"*b0:Player's Complete Stats", 7, window->z);
}

void func_801E28E4(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_80067784(&D_801F4588.polys[i]);
        SetSemiTrans(&D_801F4588.polys[i], 1);
        SetDrawTPage(&D_801F4588.tpages[i], 0, 0, 0x40);
        D_801F4588.polys[i].b0 = 0xFF;
        D_801F4588.polys[i].g0 = 0xFF;
        D_801F4588.polys[i].r0 = 0xFF;
    }
    D_801F4588.alpha = 0xFF;
    D_801F4588.pos.vz = 0;
    D_801F4588.pos.vy = 0;
    D_801F4588.pos.vx = 0;
    D_801F4588.rot.vz = 0;
    D_801F4588.rot.vy = 0;
    D_801F4588.rot.vx = 0;
    D_801F4588.corners[0].vx = D_801F4588.corners[2].vx = -0x87;
    D_801F4588.corners[1].vx = D_801F4588.corners[3].vx = 0x8A;
    D_801F4588.corners[0].vy = D_801F4588.corners[1].vy = -0x4D;
    D_801F4588.corners[2].vy = D_801F4588.corners[3].vy = 0x48;
    for (i = 0; i < 4; i++) {
        D_801F4588.corners[i].vz = 0;
    }
}

void func_801E2A18(void) {
    MATRIX matrix;
    SVECTOR corners[4];
    s16 xs[2] = { -0x8E, 0xB2 };
    s16 ys[2] = { -0x4D, 0x48 };
    s32 sxy[4];
    s32 depthCue;
    s32 flag;

    buildRotTransMatrix(&D_801F4588.pos, &D_801F4588.rot, &matrix);
    CompMatrix((MATRIX *)SCENE_3D->unk78, &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    func_8005C444(&matrix);
    corners[0] = D_801F4588.corners[0];
    corners[1] = D_801F4588.corners[1];
    corners[2] = D_801F4588.corners[2];
    corners[3] = D_801F4588.corners[3];
    RotAverage4(&corners[0], &corners[1], &corners[2], &corners[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
    D_801F4588.polys[FRAME_BUFFER_INDEX].r0 = D_801F4588.polys[FRAME_BUFFER_INDEX].g0 = D_801F4588.polys[FRAME_BUFFER_INDEX].b0 = D_801F4588.alpha;
    D_801F4588.polys[FRAME_BUFFER_INDEX].x0 = sxy[0];
    D_801F4588.polys[FRAME_BUFFER_INDEX].y0 = sxy[0] >> 16;
    D_801F4588.polys[FRAME_BUFFER_INDEX].x1 = sxy[1];
    D_801F4588.polys[FRAME_BUFFER_INDEX].y1 = sxy[1] >> 16;
    D_801F4588.polys[FRAME_BUFFER_INDEX].x2 = sxy[2];
    D_801F4588.polys[FRAME_BUFFER_INDEX].y2 = sxy[2] >> 16;
    D_801F4588.polys[FRAME_BUFFER_INDEX].x3 = sxy[3];
    D_801F4588.polys[FRAME_BUFFER_INDEX].y3 = sxy[3] >> 16;
    addPrim(&CURRENT_FRAME_BUFFER->ot[36], &D_801F4588.polys[FRAME_BUFFER_INDEX]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[36], &D_801F4588.tpages[FRAME_BUFFER_INDEX]);
}

void func_801E2D90(void) {
    Rect16 rect;

    rect.x = 0x180;
    rect.y = 0x100;
    rect.w = 0xFF;
    rect.h = 0x80;
    if (D_801F4588.unk11E != 0) {
        D_801F5260[0] = func_801EB754(4);
        D_801F4588.unk116 = 1;
        func_80014C08(20);
        func_801E108C(1);
    } else {
        if (D_801F469E == 0) {
            D_801F5260[0] = func_801EB754(3);
        } else {
            func_801EE40C(D_801F5260[0], &rect, 0);
            D_801F5260[0]->quads[0].clut = D_801F5260[0]->quads[1].clut = getClut(0x280, 0x1F8);
        }
        D_801F469E = 1;
    }
}

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
    level = D_801F4588.alpha;
    done = level == 0xFF;
    level += 8;
    if (level > 0xFF) {
        level = 0xFF;
    }
    D_801F469F = level;
    return done;
}

void func_801E2ED4(void) {
    s16 xs[5] = { -0xA2, -0x7A, -2, 0x76, 0x9E };
    s16 ys[4] = { -0x60, -0x38, 0x38, 0x60 };
    s32 i;
    s32 j;
    s32 palette;
    s32 k;

    if ((s8)SESSION->unk1A8 < 0) {
        palette = 0;
    } else {
        palette = (s8)SESSION->unk1A8;
    }
    for (j = 10, i = 1; j < 20; j++, i++) {
        D_801F5260[i] = func_801EB754(j);
        k = palette + 0xEB;
        D_801F5260[i]->quads[0].clut = getClut(0x210, k);
        D_801F5260[i]->quads[1].clut = getClut(0x210, palette + 0xEB);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            k = i * 2 + j;
            D_801F5260[k + 1]->corners[0].vx = D_801F5260[k + 1]->corners[2].vx = xs[j + 1];
            D_801F5260[k + 1]->corners[1].vx = D_801F5260[k + 1]->corners[3].vx = xs[j + 2];
            D_801F5260[k + 1]->corners[0].vy = D_801F5260[k + 1]->corners[1].vy = ys[i * 2];
            D_801F5260[k + 1]->corners[2].vy = D_801F5260[k + 1]->corners[3].vy = ys[i * 2 + 1];
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            k = i * 3 + j;
            D_801F5260[k + 5]->corners[0].vx = D_801F5260[k + 5]->corners[2].vx = xs[i * 3];
            D_801F5260[k + 5]->corners[1].vx = D_801F5260[k + 5]->corners[3].vx = xs[i * 3 + 1];
            D_801F5260[k + 5]->corners[0].vy = D_801F5260[k + 5]->corners[1].vy = ys[j];
            D_801F5260[k + 5]->corners[2].vy = D_801F5260[k + 5]->corners[3].vy = ys[j + 1];
        }
    }
}

void func_801E317C(void) {
    s16 xs[5] = { -0x9D, -0x75, 3, 0x7B, 0x9F };
    s16 ys[4] = { -0x63, -0x3B, 0x3D, 0x65 };
    s32 i;
    s32 j;
    s32 k;

    for (j = 0x30, i = 11; j < 0x3A; j++, i++) {
        D_801F5260[i] = func_801EB754(j);
        D_801F5260[i]->quads[0].clut = getClut(0x200, 0xE2);
        D_801F5260[i]->quads[1].clut = getClut(0x200, 0xE2);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            k = i * 2 + j + 11;
            D_801F5260[k]->corners[0].vx = D_801F5260[k]->corners[2].vx = xs[j + 1];
            D_801F5260[k]->corners[1].vx = D_801F5260[k]->corners[3].vx = xs[j + 2];
            D_801F5260[k]->corners[0].vy = D_801F5260[k]->corners[1].vy = ys[i * 2];
            D_801F5260[k]->corners[2].vy = D_801F5260[k]->corners[3].vy = ys[i * 2 + 1];
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            k = i * 3 + j + 15;
            D_801F5260[k]->corners[0].vx = D_801F5260[k]->corners[2].vx = xs[i * 3];
            D_801F5260[k]->corners[1].vx = D_801F5260[k]->corners[3].vx = xs[i * 3 + 1];
            D_801F5260[k]->corners[0].vy = D_801F5260[k]->corners[1].vy = ys[j];
            D_801F5260[k]->corners[2].vy = D_801F5260[k]->corners[3].vy = ys[j + 1];
        }
    }
}

const char D_801DE490[] = "";

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

s32 func_801E354C(void) {
    s32 done = 0;
    s32 i;
    s32 y;

    if (D_801F4588.opening == 1) {
        if (D_801F4588.angle == 0x400 && D_801F4588.openTimer == 35) {
            done = 1;
            D_801F4588.opening = 0;
        }
        D_801F4588.openTimer++;
        if (D_801F4588.openTimer > 35) {
            D_801F4588.openTimer = 35;
        }
        D_801F4E48.unk3D4 = D_801F4588.openTimer - 20;
        if (D_801F4E48.unk3D4 < 0) {
            D_801F4E48.unk3D4 = 0;
        } else if (D_801F4E48.unk3D4 > 15) {
            D_801F4E48.unk3D4 = 15;
        }
    } else {
        D_801F4588.openTimer--;
        if (D_801F4588.openTimer < 0) {
            D_801F4588.openTimer = 0;
            D_801F4588.opening = 1;
            done = 1;
        }
        D_801F4E48.unk3D4 = D_801F4588.openTimer - 20;
        if (D_801F4E48.unk3D4 < 0) {
            D_801F4E48.unk3D4 = 0;
        } else if (D_801F4E48.unk3D4 > 15) {
            D_801F4E48.unk3D4 = 15;
        }
    }
    D_801F4588.angle = (D_801F4588.openTimer << 10) / 35;
    if (D_801F4588.angle > 0x400) {
        D_801F4588.angle = 0x400;
    }
    for (i = 1; i < 11; i++) {
        D_801F5260[i]->rot.vz = -(D_801F4588.openTimer << 12) / 35;
    }
    for (i = 11; i < 21; i++) {
        D_801F5260[i]->rot.vz = (D_801F4588.openTimer << 12) / 35;
    }
    D_801F4588.rot.vz = (D_801F4588.openTimer << 12) / 35;
    D_801F4588.pos.vz = rcos(D_801F4588.angle) * 2 / 3;
    y = -(D_801F4E48.unk3D4 * 14) / 15;
    for (i = 1; i < 21; i++) {
        D_801F5260[i]->pos.vy = y;
        D_801F5260[i]->pos.vz = D_801F4588.pos.vz;
    }
    D_801F5260[0]->pos.vy = y;
    D_801F5260[21]->pos.vy = (D_801F4E48.unk3D4 * -92 + (15 - D_801F4E48.unk3D4) * -188) / 15;
    D_801F5260[22]->pos.vy = D_801F5260[21]->pos.vy + 1;
    return done;
}

void func_801E390C(ScriptRunner *runner) {
    s32 result;
    s32 i;
    s32 offset;

    do {
        result = runScriptToNextEvent(runner->script, runner->regs);
        if (result == 1) {
            switch (runner->script->eventOp) {
            case 10:
                switch (runner->script->eventArg) {
                case 0:
                    if (D_801F4588.unk10A == 0) {
                        D_801F4588.unk10E = 10;
                        func_801E0D54();
                        func_800149B8(0, -1, 0, 0x800, func_801E55F8, 0, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    if (D_801F4588.unk10A == 1) {
                        return;
                    }
                    D_801F4588.unk10B = 1;
                    do {
                        func_80014C08(1);
                    } while (D_801F4588.unk10A != 0);
                    D_801F4696 = 10;
                    func_801E0D54();
                    func_800149B8(0, -1, 0, 0x800, func_801E55F8, 0, getCurrentTaskId(), 0, 0);
                    return;
                case 1:
                    offset = runner->script->pc - runner->script->start;
                    func_801E0A74();
                    for (i = 0; i < D_801F4588.count;) {
                        runner->regs[1] = ++i;
                        func_801E1190(runner);
                        runner->script->pc = runner->script->start + offset;
                        func_801E0B70();
                    }
                    runner->regs[1] = 0;
                    D_801F4696 = 1;
                    return;
                case 2:
                    if (D_801F4588.unk10A == 0) {
                        D_801F4588.unk10E = 10;
                        D_801F4908.current[0] = 0x80;
                        D_801F4908.target[0] = 0x80;
                        D_801F4908.current[1] = 0x80;
                        D_801F4908.target[1] = 0x80;
                        func_800149B8(0, -1, 0, 0x800, func_801E77F4, 0, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    if (D_801F4588.unk10A == 2) {
                        return;
                    }
                    D_801F4588.unk10B = 1;
                    do {
                        func_80014C08(1);
                    } while (D_801F4588.unk10A != 0);
                    D_801F4696 = 10;
                    D_801F4908.current[0] = 0x80;
                    D_801F4908.target[0] = 0x80;
                    D_801F4908.current[1] = 0x80;
                    D_801F4908.target[1] = 0x80;
                    func_800149B8(0, -1, 0, 0x800, func_801E77F4, 0, getCurrentTaskId(), 0, 0);
                    return;
                case 3:
                    if (D_801F4588.unk10C != 0) {
                        D_801F4588.unk10C = 0;
                        D_801F4588.unk10E = 10;
                        D_801F4588.unk114 = 7;
                        return;
                    }
                    D_801F4696 = 2;
                    return;
                case 4:
                    if (func_801DFC00((u8 *)runner->regs[4]) == -1) {
                        D_801F4696 = 3;
                        return;
                    }
                    break;
                case 5:
                    D_801F4588.unk10E = 4;
                    D_801F4588.unk120 = 1;
                    return;
                case 6:
                    func_801DFB6C(D_801F4840);
                    break;
                case 7:
                    if (D_801F4810.unk20 == 0) {
                        func_801E0D8C(D_801F4838[0]->regs);
                        D_801F4810.unk20 = 1;
                        func_800149B8(0, -1, 0, 0x400, func_801E186C, 0, getCurrentTaskId(), 0, 0);
                        D_801F4696 = 10;
                    }
                    return;
                case 9:
                    runner->regs[0] = 0;
                    D_801F4695 = 2;
                    return;
                case 10:
                    D_801F4696 = 10;
                    func_800149B8(0, -1, 0, 0x400, func_801F2ABC, getCurrentTaskId(), 0, 0, 0);
                    return;
                case 11:
                    D_801F4697 = 0;
                    for (i = 0; i < 4; i++) {
                        D_801F4698[i] = -1;
                    }
                    break;
                case 12:
                    if (D_801F4588.unk10A == 0) {
                        D_801F4588.unk10E = 10;
                        func_800149B8(0, -1, 0, 0x800, func_801E8DD8, D_801F4588.unk11A, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    if (D_801F4588.unk10A == D_801F4588.unk11A + 1) {
                        return;
                    }
                    D_801F4588.unk10B = 1;
                    do {
                        func_80014C08(1);
                    } while (D_801F4588.unk10A != 0);
                    D_801F4588.unk10E = 10;
                    func_800149B8(0, -1, 0, 0x800, func_801E8DD8, D_801F4588.unk11A, getCurrentTaskId(), 0, 0);
                    return;
                case 13:
                    func_801E84BC();
                    return;
                case 14:
                    break;
                case 15:
                    animateWindowTo(&D_801F4B44, (Rect16 *)-1);
                    playSoundEffect(4);
                    D_801F4588.unk10E = 10;
                    bzero((Scene3D *)D_801F4588.unkFC, 0xD);
                    func_800149B8(0, -1, 0, 0x800, func_801EB1F8, D_801F4588.unkFC, getCurrentTaskId(), 0, 0);
                    return;
                case 16:
                    runner->regs[1] = ((PlayerProfile *)PLAYER_PROFILES)->battleWins;
                    break;
                case 17:
                    runner->regs[0] = 0;
                    D_801F4695 = 5;
                    return;
                case 18:
                    ((PlayerProfile *)PLAYER_PROFILES)->unk28_10 = 1;
                    break;
                case 19:
                    ((PlayerProfile *)PLAYER_PROFILES)->unk14 = 0;
                    break;
                case 20:
                    func_800149B8(0, -1, 0, 0x400, func_801E9F28, 0, 0, 0, 0);
                    break;
                case 21:
                    func_801E9F18();
                    break;
                case 22:
                    initDialog(D_801F4A38, (u8 *)D_801F4838[0]->regs[9], 0);
                    runDialog(D_801F4A38);
                    break;
                default:
                    runner->regs[0] = 0;
                    return;
                }
                break;
            case 11:
                switch (runner->script->eventArg) {
                case 0:
                    func_801E0174((s16)runner->script->params[0] == 0x78);
                    break;
                case 1:
                    func_801E0390(runner->script->params[0]);
                    break;
                case 2:
                    SESSION->unk1A9 = 1;
                    D_801F4588.unkF0 = (s16)runner->script->params[0];
                    runner->regs[0] = 0;
                    D_801F4588.unk10D = 1;
                    SESSION->scriptOffset = D_801F4838[0]->script->pc - D_801F4838[0]->script->start;
                    return;
                case 3:
                    func_801E0C54((s16)runner->script->params[0]);
                    break;
                case 4:
                    func_801E09F4();
                    runner->regs[0] = 0;
                    D_801F4588.unk10D = 4;
                    D_801F4588.unkF0 = (s16)runner->script->params[0];
                    return;
                case 6:
                    func_801E0A74();
                    ((ProfileSave *)PLAYER_PROFILES)->scriptOffset = D_801F4838[0]->script->pc - D_801F4838[0]->script->start;
                    ((PlayerProfile *)PLAYER_PROFILES)->unkF = runner->script->params[0];
                    runner->regs[0] = 0;
                    D_801F4695 = 3;
                    return;
                case 7:
                    SESSION->unk1A7 = runner->script->params[0];
                    break;
                case 8:
                    D_801F46A2 = SESSION->unk1A8 = runner->script->params[0];
                    break;
                case 9:
                    D_801F4588.unk11D = runner->script->params[0];
                    if (D_801F4588.unk11C == 0) {
                        D_801F4588.unk11C = 1;
                        D_801F4588.unk10E = 10;
                        func_800149B8(0, -1, 0, 0x400, func_801F0ECC, 0, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    break;
                case 10:
                    D_801F4588.partners[D_801F4588.unk10F] = runner->script->params[0];
                    D_801F4588.unk10F++;
                    break;
                case 11:
                case 12:
                    break;
                case 13:
                    playSoundEffect((s16)runner->script->params[0]);
                    break;
                case 14:
                    func_80014C08((s16)runner->script->params[0]);
                    break;
                case 15:
                    do {
                        func_80014C08(1);
                    } while (isMusicIdle() != 1);
                    if (D_801F4838[0]->regs[0xB8] == 0 && (s16)runner->script->params[0] != 0x6F) {
                        SESSION->music = (s16)runner->script->params[0];
                        playMusic(0, (s16)runner->script->params[0], 100);
                    }
                    break;
                case 16:
                    D_801F4696 = 10;
                    func_800149B8(0, -1, 0, 0x400, func_801F32CC, (s16)runner->script->params[0], getCurrentTaskId(), 0, 0);
                    return;
                case 17:
                    ((SessionData *)D_8006E054)->npcDeckIndex[0] = runner->script->params[0];
                    break;
                case 18:
                    ((PlayerProfile *)PLAYER_PROFILES)->unk14 += runner->script->params[0];
                    break;
                case 19:
                    SESSION->unk1A9 = 1;
                    D_801F4588.unkF0 = (s16)runner->script->params[0];
                    runner->regs[0] = 0;
                    D_801F4588.unk10D = 6;
                    SESSION->scriptOffset = D_801F4838[0]->script->pc - D_801F4838[0]->script->start;
                    return;
                case 20:
                    D_801F4696 = 10;
                    func_800149B8(0, -1, 0, 0x400, func_801E9E10, (s16)runner->script->params[0], getCurrentTaskId(), 0, 0);
                    return;
                case 21:
                    for (i = 0; i < 3; i++) {
                        if (((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].inUse == 0) {
                            ((SessionData *)((u8 *)D_8006E054 + i))->unk1010[0x13] = 0;
                        } else if (countDeckCardsByFilter(0, &((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i], (s16)runner->script->params[0]) != 0) {
                            ((SessionData *)((u8 *)D_8006E054 + i))->unk1010[0x13] = 0;
                        } else {
                            ((SessionData *)((u8 *)D_8006E054 + i))->unk1010[0x13] = 1;
                        }
                    }
                    if (((SessionData *)D_8006E054)->unk1010[0x13] == 1 || ((SessionData *)D_8006E054)->unk1010[0x14] == 1 ||
                        ((SessionData *)D_8006E054)->unk1010[0x15] == 1) {
                        ((SessionData *)D_8006E054)->unk1010[0x12] = 1;
                        D_801F4838[0]->regs[1] = 0;
                    } else {
                        ((SessionData *)D_8006E054)->unk1010[0x12] = 0;
                        D_801F4838[0]->regs[1] = 1;
                    }
                    break;
                default:
                    runner->regs[0] = 0;
                    return;
                }
                break;
            case 13:
                switch (runner->script->eventArg) {
                case 0:
                    if ((s16)runner->script->params[0] == 0) {
                        D_801F4908.target[0] = runner->script->params[1];
                        D_801F4908.step[0] = runner->script->params[2];
                    } else {
                        D_801F4908.target[1] = runner->script->params[1];
                        D_801F4908.step[1] = runner->script->params[2];
                    }
                    break;
                case 1:
                    D_801F4696 = 10;
                    D_801F4AF8[0] = runner->script->params[0];
                    D_801F4AF8[1] = runner->script->params[1];
                    D_801F4AF8[2] = runner->script->params[2];
                    func_800149B8(0, -1, 0, 0x400, func_801F0ECC, 1, getCurrentTaskId(), 0, 0);
                    return;
                default:
                    runner->regs[0] = 0;
                    return;
                }
                break;
            case 12:
            default:
                runner->regs[0] = 0;
                return;
            }
        }
        clearScriptBusy(runner->script);
    } while (result != 0);
}

s32 func_801E460C(ScriptRunner *runner) {
    *runner->regs = 1;
    func_801E390C(runner);
    return *runner->regs;
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

ScriptRunner *func_801E4700(void) {
    u8 unused[0x18];
    ScriptRunner *obj = allocHeapBlock(sizeof(ScriptRunner), 0x31);

    obj->unk0 = *(s32 *)&((SessionData *)D_8006E054)->unk100C->unk0[0x190];
    obj->script = func_801E4648((u8 *)obj->unk0);
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

void func_801E4878(void) {
    char buf[0x20];

    switch (D_801F4696) {
    case 0:
        func_801E460C(D_801F4838[0]);
        break;
    case 1:
        func_801E0724();
        break;
    case 2:
        func_801E57C4();
        break;
    case 3:
    case 4:
        if (D_801F4588.unk11F == 0 && (PAD_STATES[0]->pressed & 0x40)) {
            playSoundEffect(0);
            func_801DFB6C(D_801F4840);
            if (D_801F4588.unk10E == 3) {
                func_801DFC00((u8 *)D_801F4838[0]->regs[4]);
            }
            D_801F4588.unk10E = 0;
            D_801F4588.unk120 = 0;
        }
        break;
    case 10:
        break;
    default:
        sprintf(buf, "(No = %d)", D_801F4696);
        break;
        /* unreachable, but it leaves the "" the ROM has after the format */
        printf("");
    }
}

void func_801E49A4(void) {
    SESSION->unk1A9 = 0;
    D_801F4838[0]->script->pc = D_801F4838[0]->script->start + SESSION->scriptOffset;
    playMusic(0, SESSION->music, 100);
    D_801F4696 = 10;
    func_801E0D54();
    func_800149B8(0, -1, 0, 0x800, func_801E55F8, 0, getCurrentTaskId(), 0, 0);
    do {
        func_80014C08(1);
    } while (D_801F4588.unk10E == 10);
    D_801F4588.unk11A = 0;
    func_801E0D8C(D_801F4838[0]->regs);
    D_801F4830 = 1;
    func_800149B8(0, -1, 0, 0x400, func_801E186C, 0, getCurrentTaskId(), 0, 0);
    D_801F4588.unk10E = 10;
}

void func_801E4AF4(s8 animate) {
    Rect16 rect = { 0x220, 0xE1, 0x20, 1 };
    Rect16 unused[3];
    s32 delay;
    s8 i;

    D_801F4B7C = 2;
    for (i = 0; i < 6; i++) {
        delay = abs(rand() % 30);
        if (animate == 1) {
            while (delay > 0) {
                func_80014C08(1);
                delay--;
            }
            playSoundEffect(0x18);
        }
        if (i == 5) {
            MoveImage2(&D_801F34EC[i], 0x280, 0x1F8);
        } else {
            MoveImage2(&D_801F34EC[i], D_801F34EC[i].x - 0x20, D_801F34EC[i].y);
        }
    }
    delay = abs(rand() % 30);
    if (animate == 1) {
        while (delay > 0) {
            func_80014C08(1);
            delay--;
        }
        playSoundEffect(0x18);
    }
    for (i = 0; i < 6; i++) {
        MoveImage2(&D_801F34EC[4], D_801F34EC[4].x - 0x20, i + 0xEC);
    }
    delay = abs(rand() % 30);
    if (animate == 1) {
        while (delay > 0) {
            func_80014C08(1);
            delay--;
        }
        playSoundEffect(0x18);
    }
    MoveImage2(&rect, 0x380, 0x80);
}

const char D_801DE5D0[] = "";

void func_801E4D80(s32 resume) {
    s32 timer = 0;

    if (resume == 0) {
        func_800149B8(0, -1, 0, 0x400, func_801EB628, 0, getCurrentTaskId, 0, 0);
        do {
            func_80014C08(1);
        } while (SESSION->loading != 0);
    }
    loadSoundEffectBank(1);
    func_801EC080();
    D_801F4588.unk120 = 0;
    D_801F4838[0] = func_801E4700();
    D_801F4838[0]->regs = func_801E46AC(0x174);
    func_801E0B70();
    D_801F4838[0]->regs[0] = 1;
    ((SessionData *)D_8006E054)->unk1010[0x13] = ((SessionData *)D_8006E054)->unk1010[0x14] = ((SessionData *)D_8006E054)->unk1010[0x15] = 0;
    ((SessionData *)D_8006E054)->unk1010[0x12] = 0;
    func_801DFB6C(D_801F4840);
    func_801E108C(0);
    D_801F4588.count = 0;
    D_801F4588.unk10A = 0;
    D_801F4588.unk10D = 0;
    D_801F4588.unk10B = 0;
    D_801F4588.unk10E = 0;
    ((SessionData *)D_8006E054)->npcDeckIndex[0] = -1;
    func_801E1254();
    func_801E1360();
    addFrameCallback((s32)func_801E54C4);
    if (D_801F5250 != 1) {
        if (D_801F4838[0]->regs[0xB8] != 0) {
            if (SESSION->unk1A9 == 1) {
                func_800149B8(0, -1, 0, 0x400, func_801EC1D8, 4, getCurrentTaskId(), 0, 0);
            } else {
                func_800149B8(0, -1, 0, 0x400, func_801EC1D8, 3, getCurrentTaskId(), 0, 0);
            }
        } else {
            func_800149B8(0, -1, 0, 0x400, func_801EC1D8, 2, getCurrentTaskId(), 0, 0);
        }
    }
    D_801F524A = 2;
    if (SESSION->unk1A9 == 1) {
        D_801F4696 = 10;
        if ((s8)SESSION->unk1A8 == 1) {
            func_800149B8(0, -1, 0, 0x800, func_801E77F4, 0, getCurrentTaskId(), 0, 0);
        } else {
            func_800149B8(0, -1, 0, 0x800, func_801E8DD8, 0, getCurrentTaskId(), 0, 0);
        }
    } else if (SESSION->unk1A9 == 2) {
        func_801E49A4();
    }
    if (D_801F4838[0]->regs[0xB8] != 0) {
        func_801E4AF4(0);
        setBackgroundScrollMode(1);
    }
    do {
        func_80014C08(1);
        timer++;
        func_801E4878();
        if (D_801F4838[0]->regs[0xB8] != 0 && timer >= 150) {
            timer -= 150;
            func_801E9F08();
            playSoundEffect(0x1A);
        }
    } while (D_801F4838[0]->regs[0] != 0);
    playSoundEffect(4);
    animateWindowTo(&D_801F4B44, (Rect16 *)-1);
    D_801F4693 = 1;
    do {
        func_80014C08(1);
    } while (D_801F4588.unk10A != 0);
    removeFrameCallback((s32)func_801E54C4);
    func_801E0A74();
    func_801E0D8C(D_801F4838[0]->regs);
    if (D_801F4838[0]->regs[0xB8] != 0 && (D_801F4695 == 5 || D_801F4695 == 2)) {
        fadeOutScrollingBackground();
        do {
            func_80014C08(1);
        } while (SCROLL_BACKGROUND.shownImage != -1);
        changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->activePartner, 0x380, 0, 0x380, 0x80);
    }
    func_80014C08(1);
    func_80014A00(0x19);
    freeHeapBlocksByTag(0x7F);
    freeHeapBlocksByTag(0x2E);
    freeHeapBlocksByTag(0x31);
    func_80014C08(1);
    switch (D_801F4695) {
    default:
        func_800149B8(0, -1, 0, 0x400, func_801EBFB4, 1, getCurrentTaskId, 0, 0);
        do {
            func_80014C08(1);
        } while (SESSION->loading != 0);
        playMusic(0, 0x6F, 0x64);
        func_800149B8(0, -1, 0, 0x400, func_801F00F4, 1, 0, getCurrentTaskId(), 0);
        break;
    case 1:
        D_801F5250 = 0;
        fadeOutScrollingBackground();
        func_80014C08(10);
        func_800149B8(0, -1, 0, 0x200, startCpuDuel, D_801F4678, 0, 0, 0);
        break;
    case 2:
        D_801F5250 = 0;
        func_80014C08(1);
        openDeckEditor(1);
        break;
    case 3:
        D_801F5250 = 0;
        func_80014C08(1);
        func_800149B8(0, -1, 0, 0x400, openSaveScreenFromMap, 4, getCurrentTaskId(), 0, 0);
        break;
    case 4:
        D_801F5250 = 0;
        func_80014C08(1);
        if (D_801F4588.unkF0 >= 3) {
            D_801F4588.unkF0 = 0;
        }
        func_800149B8(0, -1, 0, 0x400, openPartnerFusion, D_801F4678, 0, 0, 0);
        break;
    case 5:
        D_801F5250 = 0;
        func_80014C08(1);
        openPartnerEquipment(1);
        break;
    case 6:
        D_801F5250 = 0;
        func_80014C08(10);
        func_800149B8(0, -1, 0, 0x800, quitToTitleOrPlayEnding, D_801F4678, 0, 0, 0);
        break;
    }
}

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

void func_801E55F8(void) {
    D_801F4588.unk10A = 1;
    D_801F4588.unk109 = -1;
    D_801F4588.opening = 1;
    D_801F4588.unk10B = 0;
    func_801E550C();
    D_801F5260[0] = func_801EB754(3);
    func_801EBD8C(D_801F5260[0], 0x24);
    func_801E354C();
    playSoundEffect(10);
    do {
        func_80014C08(1);
    } while (func_801E354C() == 0);
    D_801F4691 = 0;
    func_801E108C(1);
    do {
        func_80014C08(1);
    } while (func_801E2E64() == 0);
    D_801F4696 = 0;
    do {
        func_80014C08(1);
    } while (D_801F4588.unk10B == 0);
    do {
        func_80014C08(1);
    } while (func_801E2E90() == 0);
    D_801F4691 = -1;
    playSoundEffect(11);
    do {
        func_80014C08(1);
    } while (func_801E354C() == 0);
    func_801E55AC();
    D_801F4692 = 0;
}

void func_801E5714(s8 index) {
    s16 value = D_801F4908.current[index];

    if (D_801F4908.current[index] < D_801F4908.target[index]) {
        value += D_801F4908.step[index];
        if (D_801F4908.target[index] < D_801F4908.current[index]) {
            value = D_801F4908.target[index];
        }
    } else if (D_801F4908.target[index] < D_801F4908.current[index]) {
        value -= D_801F4908.step[index];
        if (D_801F4908.current[index] < D_801F4908.target[index]) {
            value = D_801F4908.target[index];
        }
    }
    D_801F4908.current[index] = value;
}

void func_801E57C4(void) {
    s8 card;
    s32 i;
    s32 *timers;

    if (PAD_STATES[0]->pressed & 0x5000) {
        D_801F4588.saved.selected += 3;
        if (D_801F4588.saved.selected >= 6) {
            D_801F4588.saved.selected -= 6;
        }
    } else if ((u16)PAD_STATES[0]->pressed & 0x8000) {
        D_801F4588.saved.selected--;
        if (D_801F4588.saved.selected < 0) {
            D_801F4588.saved.selected = 5;
        }
    } else if (PAD_STATES[0]->pressed & 0x2000) {
        D_801F4588.saved.selected++;
        if (D_801F4588.saved.selected >= 6) {
            D_801F4588.saved.selected = 0;
        }
    } else if (PAD_STATES[0]->pressed & 0x10) {
        playSoundEffect(1);
        D_801F4588.unk10E = 0;
        D_801F4588.saved.unkE6 = 0;
        D_801F4838[0]->regs[2] = -1;
    } else if (PAD_STATES[0]->pressed & 0x40) {
        card = D_801F4908.text[D_801F4588.saved.page * 6 + D_801F4588.saved.selected];
        if (D_801F4838[0]->regs[D_801F4588.saved.selected + 268] == 0) {
            if (card == 16 || card == 17) {
                D_801F4588.unk10E = 10;
                D_801F4588.unk114 = 20;
                if (card == 16) {
                    D_801F4588.saved.page--;
                } else {
                    D_801F4588.saved.page++;
                }
                i = 0;
                timers = D_801F4628;
            loop:
                timers[i] = rand() % 30 + 30;
                i++;
                if (i < 6) {
                    goto loop;
                }
                playSoundEffect(0);
            } else if (card >= 0 && card < 16) {
                D_801F4588.saved.unkB8 = 0;
                D_801F4588.unk114 = 6;
                D_801F4588.unk10E = 10;
                D_801F4838[0]->regs[2] = card;
                D_801F4588.unk10C = 1;
                playSoundEffect(0);
            }
        }
    }
    D_801F5260[43]->pos.vx = D_801F5260[D_801F4588.saved.selected + 31]->pos.vx;
    D_801F5260[43]->pos.vy = D_801F5260[D_801F4588.saved.selected + 31]->pos.vy;
    func_801EBA54(D_801F5260[43]);
}

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

void func_801E5BA0(void) {
    Rect16 rect;
    Unk801F4588 *ctx;
    s32 i;

    for (i = 0; i < 6; i++) {
        D_801F5260[i + 31] = func_801EB754(8);
        func_801EBD8C(D_801F5260[i + 31], 0x21);
        func_801EBDD0(D_801F5260[i + 31], 0x18, 0x16);
        D_801F5260[i + 31]->pos.vx = 0x86;
        D_801F5260[i + 31]->pos.vy = -0x57;
        func_801EBE0C(D_801F5260[i + 31], 1);
    }
    for (i = 0, ctx = &D_801F4588; i < 6; i++) {
        ctx->saved.delay[i] = rand() % 30;
        ctx->saved.progress[i] = 0;
        ctx->saved.flags[i] = 0;
        D_801F4628[i] = rand() % 30 + 50;
    }
    for (i = 0; i < 6; i++) {
        D_801F5260[i + 37] = func_801EB754(0x3A);
        func_801EBD8C(D_801F5260[i + 37], 0x21);
        func_801EBDD0(D_801F5260[i + 37], 0x12, 0x10);
        D_801F5260[i + 37]->pos.vx = D_801F5260[i + 31]->pos.vx;
        D_801F5260[i + 37]->pos.vy = D_801F5260[i + 31]->pos.vy - 1;
    }
    D_801F5260[43] = func_801EB754(0x3C);
    func_801EBD8C(D_801F5260[43], 0x20);
    func_801EBE0C(D_801F5260[43], 0);
    D_801F5260[44] = func_801EB754(8);
    func_801EBD8C(D_801F5260[44], 0x21);
    func_801EBE0C(D_801F5260[44], 1);
    D_801F5260[45] = func_801EB754(0x3B);
    func_801EBD8C(D_801F5260[45], 0x21);
    D_801F5260[45]->pos.vx = D_801F5260[44]->pos.vx = -0xC8;
    D_801F5260[45]->pos.vy = D_801F5260[44]->pos.vy = 0x15;
    if (SESSION->unk1A7 == 0) {
        rect.x = 0x200;
        rect.y = 0x100;
        rect.w = 0xFF;
        rect.h = 0x80;
    } else {
        rect.x = 0x180;
        rect.y = 0x180;
        rect.w = 0xFF;
        rect.h = 0x7F;
    }
    if (SESSION->unk1A9 == 1) {
        D_801F4838[0]->script->pc = D_801F4838[0]->script->start + SESSION->scriptOffset;
        D_801F4908 = SESSION->unk0;
        D_801F4588.saved = SESSION->unk12C;
        D_801F4838[0]->regs[1] = (s8)(SESSION->unk1A6 ^ 1);
        D_801F4588.unk11A = SESSION->unk1A8;
        SESSION->unk1A9 = 0;
        D_801F4588.unk114 = 9;
        D_801F4588.saved.timer = 20;
        D_801F4588.unk10C = 1;
    } else {
        D_801F4588.saved.page = 0;
        D_801F4588.saved.selected = 0;
        D_801F4588.saved.unkE6 = 0;
        D_801F4588.saved.timer = 0;
        D_801F4588.saved.unkB8 = 0;
        D_801F4588.unk10C = 0;
        D_801F4588.unk114 = 4;
    }
    if ((u8)D_801F4588.saved.page >= 3) {
        D_801F4588.saved.page = 0;
    }
}

void func_801E602C(void) {
    s32 i;

    for (i = 0x1F; i < 0x2E; i++) {
        func_801EBA34(D_801F5260[i]);
    }
    ((SaveBlock *)((SessionData *)D_8006E054)->unk100C)->unk0 = D_801F4908;
    ((SaveBlock *)((SessionData *)D_8006E054)->unk100C)->unk12C = D_801F4588.saved;
    func_801E0D54();
}

void func_801E612C(void) {
    Rect16 rect;
    s32 i;
    s8 *cards;
    s8 page;
    s8 card;
    s16 w;
    s16 h;

    for (i = 0; i < 6; i++) {
        page = D_801F4588.saved.page;
        cards = D_801F4A15;
        card = cards[page * 6 + i];
        if (card != -1) {
            if (D_801F4628[i] > 0) {
                if (D_801F4628[i] & 4) {
                    rect.x = 0x318;
                } else {
                    rect.x = 0x323;
                }
                rect.y = 0xA4;
                rect.w = 0x2C;
                rect.h = 0x24;
                func_801EE5A8(D_801F5260[i + 37], &rect);
                D_801F5260[i + 37]->quads[0].clut = getClut(0x200, 0xEB);
                D_801F5260[i + 37]->quads[1].clut = getClut(0x200, 0xEB);
            } else if (card == 16) {
                rect.x = 0x2EB;
                rect.y = 0x90;
                rect.w = 0x2A;
                rect.h = 0x24;
                func_801EE5A8(D_801F5260[i + 37], &rect);
                D_801F5260[i + 37]->quads[0].clut = getClut(0x200, 0xEE);
                D_801F5260[i + 37]->quads[1].clut = getClut(0x200, 0xEE);
            } else if (card == 17) {
                rect.x = 0x2E0;
                rect.y = 0x90;
                rect.w = 0x2A;
                rect.h = 0x24;
                func_801EE5A8(D_801F5260[i + 37], &rect);
                D_801F5260[i + 37]->quads[0].clut = getClut(0x200, 0xEE);
                D_801F5260[i + 37]->quads[1].clut = getClut(0x200, 0xEE);
            } else {
                rect.x = card % 4 * 32 + 0x280;
                rect.y = card / 4 * 56 + 0x100;
                rect.w = 0x3F;
                rect.h = 0x38;
                func_801EE40C(D_801F5260[i + 37], &rect, 0);
                D_801F4628[i] = 0;
                D_801F5260[i + 37]->quads[0].clut = getClut(0x280, 0x1FB);
                D_801F5260[i + 37]->quads[1].clut = getClut(0x280, 0x1FB);
            }
        }
        w = (D_801F4588.saved.progress[i] * 44 + (20 - D_801F4588.saved.progress[i]) * 18) / 20;
        h = (D_801F4588.saved.progress[i] * 38 + (20 - D_801F4588.saved.progress[i]) * 16) / 20;
        func_801EBDD0(D_801F5260[i + 37], w, h);
        D_801F5260[i + 37]->pos.vx = D_801F5260[i + 31]->pos.vx;
        D_801F5260[i + 37]->pos.vy = D_801F5260[i + 31]->pos.vy - 1;
    }
}


void func_801E6464(void) {
    Rect16 rect;
    s32 i;
    s16 w;
    s16 h;

    if (D_801F4588.unk114 == 6) {
        if (D_801F4588.saved.unkB8 == 20) {
            D_801F4588.unk10E = 0;
            D_801F4588.unk114 = 2;
        }
        D_801F4588.saved.unkB8++;
        if (D_801F4588.saved.unkB8 > 20) {
            D_801F4588.saved.unkB8 = 20;
        }
    } else {
        if (D_801F4588.saved.unkB8 == 0) {
            D_801F4588.unk10E = 2;
            D_801F4588.unk114 = 2;
        }
        if (--D_801F4588.saved.unkB8 < 0) {
            D_801F4588.saved.unkB8 = 0;
        }
    }
    for (i = 0; i < 6; i++) {
        if (i == D_801F4588.saved.selected) {
            D_801F5260[i + 31]->pos.vx = (D_801F4588.saved.unkB8 * 108 + ((i % 3) * 55 - 46) * (20 - D_801F4588.saved.unkB8)) / 20;
            D_801F5260[i + 31]->pos.vy = (D_801F4588.saved.unkB8 * 21 + ((i / 3) * 50 - 43) * (20 - D_801F4588.saved.unkB8)) / 20;
            func_801EBDD0(D_801F5260[i + 31], (D_801F4588.saved.unkB8 * 80 + (20 - D_801F4588.saved.unkB8) * 56) / 20, (D_801F4588.saved.unkB8 * 70 + (20 - D_801F4588.saved.unkB8) * 48) / 20);
            w = (D_801F4588.saved.unkB8 * 64 + (20 - D_801F4588.saved.unkB8) * 44) / 20;
            h = (D_801F4588.saved.unkB8 * 56 + (20 - D_801F4588.saved.unkB8) * 38) / 20;
            func_801EBDD0(D_801F5260[i + 37], w, h);
            D_801F5260[i + 37]->pos.vx = D_801F5260[i + 31]->pos.vx;
            D_801F5260[i + 37]->pos.vy = D_801F5260[i + 31]->pos.vy - 1;
        } else {
            D_801F5260[i + 31]->pos.vx = (D_801F4588.saved.unkB8 * 134 + ((i % 3) * 55 - 46) * (20 - D_801F4588.saved.unkB8)) / 20;
            D_801F5260[i + 31]->pos.vy = (D_801F4588.saved.unkB8 * -87 + ((i / 3) * 50 - 43) * (20 - D_801F4588.saved.unkB8)) / 20;
            func_801EBDD0(D_801F5260[i + 31], (D_801F4588.saved.unkB8 * 24 + (20 - D_801F4588.saved.unkB8) * 56) / 20, (D_801F4588.saved.unkB8 * 22 + (20 - D_801F4588.saved.unkB8) * 48) / 20);
            w = (D_801F4588.saved.unkB8 * 18 + (20 - D_801F4588.saved.unkB8) * 44) / 20;
            h = (D_801F4588.saved.unkB8 * 16 + (20 - D_801F4588.saved.unkB8) * 38) / 20;
            func_801EBDD0(D_801F5260[i + 37], w, h);
            D_801F5260[i + 37]->pos.vx = D_801F5260[i + 31]->pos.vx;
            D_801F5260[i + 37]->pos.vy = D_801F5260[i + 31]->pos.vy - 1;
        }
    }
    D_801F5260[44]->pos.vx = (D_801F4588.saved.unkB8 * -103 - (20 - D_801F4588.saved.unkB8) * 200) / 20;
    D_801F5260[44]->pos.vy = 21;
    D_801F5260[45]->pos.vx = D_801F5260[44]->pos.vx;
    D_801F5260[45]->pos.vy = D_801F5260[44]->pos.vy;
}

s8 func_801E6A68(void) {
    s32 i;
    s8 busy;

    for (i = 0; i < 6; i++) {
        if (D_801F4588.unk114 == 4 || D_801F4588.unk114 == 20) {
            if (D_801F4648[i] <= 0) {
                D_801F4610[i]++;
            } else {
                D_801F4648[i]--;
                if (D_801F4648[i] < 0) {
                    D_801F4648[i] = 0;
                }
            }
            if (D_801F4610[i] > 20) {
                D_801F4610[i] = 20;
            }
        } else if (D_801F4588.unk114 == 5) {
            if (D_801F4648[i] <= 0) {
                D_801F4588.saved.blink[i] = (D_801F4588.saved.blink[i] + 1) & 0xFFF;
                if (D_801F4588.saved.blink[i] > 20 || D_801F4610[i] != 20) {
                    D_801F4610[i]--;
                }
            } else {
                D_801F4648[i]--;
                if (D_801F4648[i] < 0) {
                    D_801F4648[i] = 0;
                }
            }
            if (D_801F4610[i] < 0) {
                D_801F4610[i] = 0;
            }
        }
        D_801F5260[i + 31]->pos.vx = ((i % 3 * 55 - 46) * D_801F4610[i] + (20 - D_801F4610[i]) * 134) / 20;
        D_801F5260[i + 31]->pos.vy = ((i / 3 * 50 - 43) * D_801F4610[i] + (20 - D_801F4610[i]) * -87) / 20;
        func_801EBDD0(D_801F5260[i + 31], (D_801F4610[i] * 56 + (20 - D_801F4610[i]) * 24) / 20,
                      (D_801F4610[i] * 48 + (20 - D_801F4610[i]) * 22) / 20);
    }
    busy = 0;
    if (D_801F469C == 4 || D_801F469C == 20) {
        for (i = 0; i < 6; i++) {
            if (D_801F4628[i] > 0) {
                D_801F4628[i]--;
            }
            if (D_801F4628[i] < 0) {
                D_801F4628[i] = 0;
            }
            if (D_801F4628[i] != 0) {
                busy++;
            }
        }
        if (busy == 0) {
            if (D_801F4588.unk114 == 20) {
                D_801F4588.unk114 = 2;
                D_801F4588.unk10E = 2;
            } else {
                D_801F4588.unk114 = 2;
                D_801F4588.unk10E = 0;
            }
        }
    } else if (D_801F469C == 5) {
        for (i = 0; i < 6; i++) {
            if (D_801F4610[i] != 0) {
                busy++;
            }
        }
        if (busy == 0) {
            D_801F4588.unk10E = 0;
            D_801F4588.unk114 = 2;
        }
    }
    func_801E612C();
    return busy;
}

s32 func_801E6EF0(void) {
    s32 result = 1;
    s32 i;

    if (D_801F4588.unk114 != 9) {
        D_801F4588.saved.timer++;
        if (D_801F4588.saved.timer > 20) {
            D_801F4588.saved.timer = 20;
            result = 0;
            D_801F4588.unk10E = 0;
            D_801F4588.unk114 = 2;
        }
    } else {
        D_801F4588.saved.timer--;
        func_801E612C();
        for (i = 0; i < 6; i++) {
            if (D_801F4588.saved.selected == i) {
                func_801EBDD0(D_801F5260[i + 31], 0x50, 0x46);
                func_801EBDD0(D_801F5260[i + 37], 0x40, 0x38);
            } else {
                func_801EBDD0(D_801F5260[i + 31], 0x18, 0x16);
                func_801EBDD0(D_801F5260[i + 37], 0x12, 0x10);
            }
        }
        if (D_801F4588.saved.timer < 0) {
            D_801F4588.saved.timer = 0;
            result = 0;
            D_801F4588.unk10E = 0;
            D_801F4588.unk114 = 2;
        }
    }
    D_801F5260[44]->pos.vx = (D_801F4588.saved.timer * -200 + (20 - D_801F4588.saved.timer) * -103) / 20;
    D_801F5260[44]->pos.vy = 0x15;
    D_801F5260[45]->pos.vx = D_801F5260[44]->pos.vx;
    D_801F5260[45]->pos.vy = D_801F5260[44]->pos.vy;
    for (i = 0; i < 6; i++) {
        if (i == D_801F4588.saved.selected) {
            D_801F5260[i + 31]->pos.vx = (D_801F4588.saved.timer * 210 + (20 - D_801F4588.saved.timer) * 108) / 20;
            D_801F5260[i + 37]->pos.vx = D_801F5260[i + 31]->pos.vx;
            D_801F5260[i + 31]->pos.vy = D_801F5260[i + 37]->pos.vy = 0x15;
            D_801F5260[i + 37]->pos.vy--;
        } else {
            D_801F5260[i + 31]->pos.vx = D_801F5260[i + 37]->pos.vx = 0x86;
            D_801F5260[i + 31]->pos.vy = D_801F5260[i + 37]->pos.vy = -0x57;
            D_801F5260[i + 37]->pos.vy--;
        }
    }
    return result;
}

void func_801E7240(void) {
    Sprite3D *card;
    s16 w;
    s16 h;

    D_801F4588.saved.timer++;
    if (D_801F4588.saved.timer > 30) {
        D_801F4588.saved.timer = 30;
        D_801F4588.unk116 = 3;
        D_801F4588.unk114 = 3;
        D_801F4588.unk10E = 0;
    }
    card = D_801F5260[D_801F4588.saved.selected + 31];
    w = (D_801F4588.saved.timer * 24 + (30 - D_801F4588.saved.timer) * 80) / 30;
    h = (D_801F4588.saved.timer * 22 + (30 - D_801F4588.saved.timer) * 70) / 30;
    func_801EBDD0(D_801F5260[44], w, h);
    func_801EBDD0(card, w, h);
    w = (D_801F4588.saved.timer * 18 + (30 - D_801F4588.saved.timer) * 64) / 30;
    h = (D_801F4588.saved.timer * 16 + (30 - D_801F4588.saved.timer) * 56) / 30;
    func_801EBDD0(D_801F5260[45], w, h);
    func_801EBDD0(card + 6, w, h);
    D_801F5260[44]->pos.vx = (D_801F4588.saved.timer * 134 + (30 - D_801F4588.saved.timer) * -103) / 30;
    D_801F5260[44]->pos.vy = (D_801F4588.saved.timer * -87 + (30 - D_801F4588.saved.timer) * 21) / 30;
    D_801F5260[45]->pos.vx = D_801F5260[44]->pos.vx;
    D_801F5260[45]->pos.vy = D_801F5260[44]->pos.vy;
    card->pos.vx = card[6].pos.vx = (D_801F4588.saved.timer * 134 + (30 - D_801F4588.saved.timer) * 108) / 30;
    card->pos.vy = card[6].pos.vy = (D_801F4588.saved.timer * -87 + (30 - D_801F4588.saved.timer) * 21) / 30;
}

void func_801E75D4(void) {
    Rect16 rect;
    s32 i;

    for (i = 0; i < 2; i++) {
        func_801E5714(i);
    }
    if (D_801F4696 == 2) {
        D_801F4908.target[0] = D_801F4908.target[1] = 0x80;
    }
    func_801EBD94(D_801F5260[45], D_801F4908.current[0]);
    func_801EBD94(D_801F5260[D_801F466D + 37], D_801F4908.current[1]);
    func_801EBA54(D_801F5260[44]);
    func_801EBA54(D_801F5260[45]);
    for (i = 0; i < 6; i++) {
        if ((D_801F4A15 + D_801F4588.saved.page * 6)[i] == -1) {
            D_801F4654[i]++;
            if (D_801F4654[i] & 4) {
                rect.x = 0x318;
            } else {
                rect.x = 0x323;
            }
            rect.y = 0xA4;
            rect.w = 0x2C;
            rect.h = 0x24;
            func_801EE5A8(D_801F5260[i + 37], &rect);
            D_801F5260[i + 37]->quads[0].clut = getClut(0x200, 0xEB);
            D_801F5260[i + 37]->quads[1].clut = getClut(0x200, 0xEB);
        } else {
            if (D_801F4838[0]->regs[i + 268] != 0) {
                func_801EBD94(D_801F5260[i + 37], 0x30);
            }
            D_801F4654[i] = 0;
        }
        func_801EBA54(D_801F5260[i + 31]);
        func_801EBA54(D_801F5260[i + 37]);
    }
}

void func_801E77F4(void) {
    D_801F4588.unk10A = 2;
    D_801F4588.unk10B = 0;
    D_801F4588.unk109 = -1;
    D_801F4588.opening = 1;
    func_801E550C();
    D_801F5260[0] = func_801EB754(5);
    func_801EBD8C(D_801F5260[0], 0x24);
    func_801E5BA0();
    func_801E354C();
    playSoundEffect(10);
    do {
        func_80014C08(1);
    } while (func_801E354C() == 0);
    D_801F4691 = 0;
    func_801E108C(1);
    addFrameCallback((s32)func_801E75D4);
    do {
        func_80014C08(1);
    } while (func_801E2E64() == 0);
    do {
        func_80014C08(1);
        switch (D_801F469C) {
        case 2:
            break;
        case 4:
        case 5:
        case 20:
            func_801E6A68();
            break;
        case 6:
        case 7:
            func_801E6464();
            break;
        case 9:
            func_801E6EF0();
            break;
        }
    } while (D_801F4693 == 0);
    D_801F469C = 5;
    if (SESSION->unk1A9 == 1) {
        do {
            func_80014C08(1);
        } while (func_801E6EF0() != 0);
    } else {
        func_801E5B1C();
        do {
            func_80014C08(1);
        } while (func_801E6A68() != 0);
    }
    removeFrameCallback((s32)func_801E75D4);
    do {
        func_80014C08(1);
    } while (func_801E2E90() == 0);
    D_801F4691 = -1;
    playSoundEffect(11);
    do {
        func_80014C08(1);
    } while (func_801E354C() == 0);
    func_801E602C();
    func_801E55AC();
    D_801F4692 = 0;
}

void func_801E7A00(void) {
    s32 i;

    if (D_801F4AF0->unkC4 == 1) {
        func_801E7D1C();
        D_801F4AF0->fades[0].polys[FRAME_BUFFER_INDEX].x0 = D_801F4AF0->fades[0].polys[FRAME_BUFFER_INDEX].x2 = D_801F4AF0->unkB8;
        addPrim(&CURRENT_FRAME_BUFFER->ot[35], &D_801F4AF0->fades[0].polys[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[35], &D_801F4AF0->fades[0].tpages[FRAME_BUFFER_INDEX]);
    }
    if (D_801F4AF0->phase < 2) {
        setRGB0(&D_801F4AF0->fades[1].polys[FRAME_BUFFER_INDEX], D_801F4AF0->alpha, D_801F4AF0->alpha, D_801F4AF0->alpha);
        addPrim(&CURRENT_FRAME_BUFFER->ot[32], &D_801F4AF0->fades[1].polys[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[32], &D_801F4AF0->fades[1].tpages[FRAME_BUFFER_INDEX]);
    }
    for (i = 0; i < 2; i++) {
        func_801E5714(i);
    }
    func_801EBD94(D_801F5260[43], D_801F4908.current[1]);
    func_801EBD94(D_801F5260[45], D_801F4908.current[0]);
    for (i = 0; i < 4; i++) {
        func_801EBA54(D_801F5260[i + 42]);
    }
}

void func_801E7D1C(void) {
    Rect16 uv;
    char *labels[4] = { "Deck Color", "Attack", "Defense", "Digivolve Speed" };
    char buf[0x19];
    s32 x;
    s32 i;
    s32 j;

    x = (D_801F4AF0->entries[0].count * 47 + (20 - D_801F4AF0->entries[0].count) * 330) / 20;
    drawText(x + 0x56, 0x4B, D_801F4838[0]->regs[9], 7, 0x22);
    uv.x = 0x60;
    uv.y = D_801F4838[0]->regs[11] * 18;
    uv.w = 0x20;
    uv.h = 0x12;
    drawTexturedSprite(x, 0x45, &uv, 0x2B, 0x3A61, 0x22, D_801F4AF0->brightness, 0);
    uv.x = 0xC4;
    uv.y = 0;
    uv.w = 0x34;
    uv.h = 0x12;
    drawTexturedSprite(x + 0x20, 0x45, &uv, 0x2A, 0x3AA1, 0x22, D_801F4AF0->brightness, 0);
    x = (D_801F4AF0->entries[1].count * 42 + (20 - D_801F4AF0->entries[1].count) * 330) / 20;
    drawLargeText(x, 0x58, (s32)"ABILITIES", 7, 0x22);
    for (i = 0; i < 4; i++) {
        x = (D_801F4AF0->entries[i + 2].count * 54 + (20 - D_801F4AF0->entries[i + 2].count) * 330) / 20;
        drawText(x, i * 12 + 0x64, (s32)labels[i], 7, 0x22);
        bzero((Scene3D *)buf, 0x19);
        switch (i) {
        case 0:
            for (j = 0; j < 5; j++) {
                if (D_801F4838[0]->regs[5] & (0x10 >> j)) {
                    s32 dx;

                    uv.x = (j + 0x13) * 4;
                    uv.y = 0;
                    uv.w = 4;
                    uv.h = 10;
                    dx = j * 5 + 0x64;
                    drawTexturedSprite(x + dx, i * 12 + 0x64, &uv, 0xA, 0x3A21, 0x22, 0x80, -1);
                }
            }
            break;
        case 1:
            sprintf(buf, "%d", D_801F4838[0]->regs[6]);
            break;
        case 2:
            sprintf(buf, "%d", D_801F4838[0]->regs[7]);
            break;
        case 3:
            sprintf(buf, "%d", D_801F4838[0]->regs[8]);
            break;
        }
        if (i != 0) {
            drawText(x + 0x64, i * 12 + 0x64, (s32)buf, 7, 0x22);
        }
    }
}

void func_801E818C(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        func_801EBA34(D_801F5260[i + 42]);
    }
    *(Unk801F4908 *)((SessionData *)D_8006E054)->unk100C = D_801F4908;
}

void func_801E8238(void) {
    Fade *fade = D_801F4AF0->fades;
    s32 k;
    s32 i;

    for (k = 0; k < 2; k++, fade++) {
        for (i = 0; i < 2; i++) {
            func_80067784(&fade->polys[i]);
            SetSemiTrans(&fade->polys[i], 1);
            if (k == 0) {
                SetDrawTPage(&fade->tpages[i], 0, 0, 0x40);
                fade->polys[i].r0 = fade->polys[i].g0 = fade->polys[i].b0 = 0x60;
                setPrimQuadRect(&fade->polys[i], 0x140, 0x42, 1, 0x5A);
            } else {
                setPrimQuadRect(&fade->polys[i], 0, 0x42, 0x140, 0x5A);
                SetDrawTPage(&fade->tpages[i], 0, 0, 0x20);
                fade->polys[i].r0 = fade->polys[i].g0 = fade->polys[i].b0 = 0;
            }
        }
    }
    D_801F5260[42] = func_801EB754(8);
    func_801EBD8C(D_801F5260[42], 0x21);
    func_801EBE0C(D_801F5260[42], 1);
    D_801F5260[43] = func_801EB754(0x3B);
    func_801EBD8C(D_801F5260[43], 0x21);
    D_801F5260[42]->pos.vx = 0xD2;
    D_801F5260[42]->pos.vy = 0x15;
    D_801F5260[43]->pos.vx = D_801F5260[42]->pos.vx = 0xD1;
    D_801F5260[43]->pos.vy = D_801F5260[42]->pos.vy = 0x14;
    D_801F5260[44] = func_801EB754(8);
    func_801EBD8C(D_801F5260[44], 0x21);
    func_801EBE0C(D_801F5260[44], 1);
    D_801F5260[45] = func_801EB754(0x3B);
    func_801EBD8C(D_801F5260[45], 0x21);
    D_801F5260[45]->pos.vx = D_801F5260[44]->pos.vx = -0xC8;
    D_801F5260[45]->pos.vy = D_801F5260[44]->pos.vy = 0x15;
    D_801F4908.target[0] = D_801F4908.current[0] = 0x80;
    D_801F4908.target[1] = D_801F4908.current[1] = 0x80;
    D_801F4AF0->unkB8 = 0x140;
    D_801F4AF0->unkC2 = 0;
    D_801F4AF0->unkC3 = 0;
    D_801F4AF0->timer = 0;
    D_801F4AF0->unkC6 = 0;
    D_801F4AF0->phase = 2;
}

void func_801E84BC(void) {
    Rect16 rect;
    s32 i;

    if (SESSION->unk1AA != (s8)D_801F4838[0]->regs[10]) {
        D_801F4696 = 10;
        if (D_801F4AF0->unkC6 == 1) {
            do {
                func_80014C08(1);
                D_801F4AF0->unkC3 = 4;
            } while (func_801E8980() != 0);
        }
        for (i = 0; i < 7; i++) {
            D_801F4AF0->entries[i].delay = rand() % 30;
            D_801F4AF0->entries[i].count = 0;
        }
        D_801F4AF0->unkC6 = 0;
        SESSION->unk1AA = D_801F4838[0]->regs[10];
        D_801F4AF0->timer = 0;
        D_801F4AF0->unkC5 = 1;
        D_801F4AF0->unkC3 = 1;
        D_801F4AF0->unkC4 = 1;
        D_801F4AF0->alpha = 0;
        D_801F4AF0->phase = 0;
        D_801F4AF0->delay = 360;
        D_801F4AF0->brightness = 0x80;
        D_801F4AF0->unkCA = 0;
        rect.x = (D_801F4838[0]->regs[10] & 3) * 32 + 0x280;
        rect.y = ((u32)D_801F4838[0]->regs[10] >> 2) * 56 + 0x100;
        rect.w = 0x3F;
        rect.h = 0x38;
        func_801EE40C(D_801F5260[43], &rect, 0);
        D_801F5260[43]->quads[0].clut = getClut(0x280, 0x1FB);
        D_801F5260[43]->quads[1].clut = getClut(0x280, 0x1FB);
    } else {
        D_801F4AF0->unkC3 = 0;
        D_801F4696 = 0;
    }
}

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

s32 func_801E87E8(void) {
    s16 alpha = D_801F4AF0->brightness;
    s32 result = 1;

    if (D_801F4AF0->timer == 19) {
        playSoundEffect(0x10);
    }
    D_801F4AF0->timer++;
    if (D_801F4AF0->timer > 20) {
        D_801F4AF0->timer = 20;
    }
    D_801F5260[43]->pos.vx = (D_801F4AF0->timer * 60 + (20 - D_801F4AF0->timer) * 210) / 20;
    D_801F5260[43]->pos.vy = -1;
    if (D_801F4AF0->timer == 20) {
        if (D_801F4AF0->unkCA == 0) {
            alpha += 8;
            if (alpha > 0xFF) {
                alpha = 0xFF;
                D_801F4AF0->unkCA = 1;
            }
        } else {
            alpha -= 8;
            if (alpha < 0x80) {
                alpha = 0x80;
                D_801F4AF0->unkCA = 2;
                D_801F4AF0->unkC3 = 5;
                result = 0;
            }
        }
        D_801F4AF0->brightness = alpha;
    }
    if (result == 0) {
        D_801F4AF0->timer = 0;
    }
    return result;
}

s32 func_801E8980(void) {
    s32 result = 1;

    if (D_801F4AF0->unkC3 == 4) {
        D_801F4AF0->timer--;
        if (D_801F4AF0->timer < 0) {
            D_801F4AF0->timer = 0;
            result = 0;
        }
    } else {
        D_801F4AF0->timer++;
        if (D_801F4AF0->timer > 20) {
            D_801F4AF0->unkC6 = 1;
            D_801F4AF0->timer = 20;
            D_801F4AF0->unkC3 = 0;
            D_801F4696 = 0;
            result = 0;
        }
    }
    D_801F5260[44]->pos.vx = (D_801F4AF0->timer * -103 + (20 - D_801F4AF0->timer) * -200) / 20;
    D_801F5260[44]->pos.vy = 0x15;
    D_801F5260[45]->pos.vx = D_801F5260[44]->pos.vx;
    D_801F5260[45]->pos.vy = D_801F5260[44]->pos.vy;
    if (D_801F4AF0->unkC5 == 1) {
        D_801F5260[42]->pos.vx = (D_801F4AF0->timer * 108 + (20 - D_801F4AF0->timer) * 60) / 20;
        D_801F5260[42]->pos.vy = (D_801F4AF0->timer * 21 + (20 - D_801F4AF0->timer) * -1) / 20;
        D_801F5260[43]->pos.vx = D_801F5260[42]->pos.vx - 1;
        D_801F5260[43]->pos.vy = D_801F5260[42]->pos.vy - 1;
    } else {
        D_801F5260[42]->pos.vx = (D_801F4AF0->timer * 108 + (20 - D_801F4AF0->timer) * 210) / 20;
        D_801F5260[42]->pos.vy = 0x15;
        D_801F5260[43]->pos.vx = D_801F5260[42]->pos.vx - 1;
        D_801F5260[43]->pos.vy = D_801F5260[42]->pos.vy - 1;
    }
    if (result == 0) {
        D_801F4AF0->unkC5 = 0;
    }
    return result;
}

void func_801E8C48(void) {
    s16 alpha = D_801F4AF0->alpha;

    if (D_801F4AF0->delay == 0) {
        if (D_801F4AF0->phase == 0) {
            if (alpha == 0) {
                playSoundEffect(0x11);
            }
            alpha += 8;
            if (alpha > 0xFF) {
                alpha = 0xFF;
                D_801F4AF0->phase = 1;
                D_801F4AF0->unkC4 = 0;
                D_801F5260[42]->pos.vx = D_801F5260[43]->pos.vx;
                D_801F5260[42]->pos.vy = D_801F5260[43]->pos.vy;
                D_801F5260[43]->pos.vx--;
                D_801F5260[43]->pos.vy--;
            }
        } else {
            alpha -= 8;
            if (alpha < 0) {
                alpha = 0;
                D_801F4AF0->phase = 2;
                D_801F4AF0->unkC3 = 3;
            }
        }
        D_801F4AF0->alpha = alpha;
    } else if (PAD_STATES[0]->pressed & 0x40) {
        D_801F4AF0->delay = 0;
    } else if (D_801F4AF0->delay != 0) {
        D_801F4AF0->delay--;
    }
}

void func_801E8DD8(s32 index) {
    Rect16 rect;

    D_801F4AF0 = allocTaskHeapBlock(0xCC);
    func_801E8238();
    addFrameCallback((s32)func_801E7A00);
    if (SESSION->unk1A9 != 0) {
        D_801F4908 = SESSION->unk0;
        D_801F46A2 = SESSION->unk1A8;
        D_801F4838[0]->script->pc = D_801F4838[0]->script->start + SESSION->scriptOffset;
        D_801F4838[0]->regs[1] = (s8)(SESSION->unk1A6 ^ 1);
        rect.x = SESSION->unk1AA % 4 * 32 + 0x280;
        rect.y = SESSION->unk1AA / 4 * 56 + 0x100;
        rect.w = 0x3F;
        rect.h = 0x38;
        func_801EE40C(D_801F5260[43], &rect, 0);
        D_801F5260[43]->quads[0].clut = getClut(0x280, 0x1FB);
        D_801F5260[43]->quads[1].clut = getClut(0x280, 0x1FB);
        D_801F4AF0->timer = 0;
        D_801F4AF0->unkC5 = 0;
        D_801F4AF0->unkC3 = 3;
        D_801F4696 = 10;
        D_801F4AF0->unkC4 = 0;
    } else {
        SESSION->unk1AA = -1;
    }
    D_801F4588.unk10A = D_801F4588.unk11A + 1;
    D_801F4588.unk10B = 0;
    D_801F4588.unk109 = -1;
    D_801F4588.opening = 1;
    func_801E550C();
    D_801F5260[0] = func_801EB754(4);
    func_801EBD8C(D_801F5260[0], 0x24);
    func_801E354C();
    playSoundEffect(10);
    do {
        func_80014C08(1);
    } while (func_801E354C() == 0);
    D_801F4691 = 0;
    func_801E108C(1);
    if (SESSION->unk1A9 == 0) {
        func_800149B8(0, -1, 0, 0x800, func_801E9864, index - 1, getCurrentTaskId(), 0, 0);
        func_80014C08(0x7FFFFFFF);
    }
    do {
        func_80014C08(1);
    } while (func_801E2E64() == 0);
    if (SESSION->unk1A9 == 0) {
        D_801F4696 = 0;
    } else {
        SESSION->unk1A9 = 0;
    }
    do {
        func_80014C08(1);
        switch (D_801F4AF0->unkC3) {
        case 0:
            break;
        case 1:
            func_801E8714();
            break;
        case 2:
            func_801E87E8();
            break;
        case 3:
        case 4:
            func_801E8980();
            break;
        case 5:
            func_801E8C48();
            break;
        }
    } while (D_801F4693 == 0);
    do {
        func_80014C08(1);
        D_801F4AF0->unkC3 = 4;
    } while (func_801E8980() != 0);
    removeFrameCallback((s32)func_801E7A00);
    do {
        func_80014C08(1);
    } while (func_801E2E90() == 0);
    D_801F4691 = -1;
    playSoundEffect(11);
    do {
        func_80014C08(1);
    } while (func_801E354C() == 0);
    func_801E55AC();
    func_801E818C();
    D_801F4692 = 0;
}

void func_801E9264(u8 index) {
    char path[24];
    u32 *tim;

    if (index < 1 || index > 4) {
        index = 0;
    }
    sprintf(path, "C:\\OBJECT\\a_%d.TIM", index);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTim(tim, 0x140, 0, 0x140, 0x81);
    freeHeapBlock(tim);
    D_801F4804->loading = 0;
}

void func_801E931C(void) {
    POLY_FT4 *quad;
    s32 buf;
    s32 i;

    for (buf = 0; buf < 2; buf++) {
        quad = D_801F4804->quads[buf];
        for (i = 0; i < 4; i++, quad++) {
            setlen(quad, 9);
            setcode(quad, 0x2C);
            SetSemiTrans(quad, 0);
            quad->clut = getClut(0x140, 0x81);
            quad->tpage = getTPage(0, 0, 0x140, 0);
            quad->r0 = 0x80;
            quad->g0 = 0x80;
            quad->b0 = 0x80;
            quad->u0 = quad->u2 = 0;
            quad->u1 = quad->u3 = 0xFF;
            quad->x0 = quad->x2 = D_801F4804->x;
            quad->x1 = quad->x3 = D_801F4804->x + 0xFF;
        }
    }
}

void func_801E9420(void) {
    D_801F4804->state = 0;
    D_801F4804->speed = 40;
    D_801F4804->x = 32;
    D_801F4804->y = 44;
    D_801F4804->step = 0;
    D_801F4804->scroll = 0;
    D_801F4804->unk140 = 60;
}

void func_801E9460(void) {
    POLY_FT4 *quad = D_801F4804->quads[FRAME_BUFFER_INDEX];
    u8 brightness;
    s32 limit = 62;
    s32 i;

    D_801F4804->scroll += D_801F4804->speed;
    if (limit - D_801F4804->step < D_801F4804->scroll) {
        D_801F4804->scroll = 0;
        D_801F4804->step++;
        if (D_801F4804->step > 62) {
            D_801F4804->step = 62;
        }
    }
    quad->v0 = quad->v1 = 62 - D_801F4804->step;
    quad->v2 = quad->v3 = 63 - D_801F4804->step;
    quad->y0 = quad->y1 = 0;
    quad->y2 = quad->y3 = D_801F4804->y - (s16)(D_801F4804->step - 63);
    quad++;
    quad->y0 = quad->y1 = D_801F4804->y - (s16)(D_801F4804->step - 63);
    quad->y2 = quad->y3 = D_801F4804->y + 63;
    quad->v0 = quad->v1 = 63 - D_801F4804->step;
    quad->v2 = quad->v3 = 63;
    quad++;
    quad->v0 = quad->v1 = D_801F4804->step + 64;
    quad->v2 = quad->v3 = D_801F4804->step + 65;
    quad->y0 = quad->y1 = D_801F4804->y + (s16)(D_801F4804->step + 63);
    quad->y2 = quad->y3 = 0xF0;
    quad++;
    quad->y0 = quad->y1 = D_801F4804->y + 63;
    quad->y2 = quad->y3 = D_801F4804->y + (s16)(D_801F4804->step + 63);
    quad->v0 = quad->v1 = 64;
    quad->v2 = quad->v3 = D_801F4804->step + 64;
    brightness = (D_801F4804->step * 128 + (limit - D_801F4804->step) * 64) / limit;
    for (i = 0; i < 4; i++) {
        D_801F4804->brightness[i] = brightness;
    }
    if (brightness == 0x80) {
        D_801F4804->state = 1;
    }
}

void func_801E96DC(void) {
    D_801F4804->unk140--;
    if (D_801F4804->unk140 < 0) {
        playSoundEffect(0x13);
        D_801F4804->state = 2;
    }
}

void func_801E972C(void) {
    s32 value = D_801F4804->brightness[0] - 8;
    s32 i;

    if (value < 0) {
        D_801F4804->state = 3;
        value = 0;
    }
    for (i = 0; i < 4; i++) {
        D_801F4804->brightness[i] = value;
    }
}

void func_801E9784(void) {
    POLY_FT4 *quad = D_801F4804->quads[FRAME_BUFFER_INDEX];
    s32 i;

    for (i = 0; i < 4; i++, quad++) {
        quad->r0 = D_801F4804->brightness[i];
        quad->g0 = D_801F4804->brightness[i];
        quad->b0 = D_801F4804->brightness[i];
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], quad);
    }
}

void func_801E9864(s32 index, s32 task) {
    D_801F4804 = allocTaskHeapBlock(0x15C);
    D_801F4804->loading = 1;
    func_801E9264(index);
    do {
        func_80014C08(1);
    } while (D_801F4804->loading != 0);
    func_801E9420();
    func_801E931C();
    do {
        func_80014C08(1);
        switch (D_801F4804->state) {
        case 0:
            func_801E9460();
            break;
        case 1:
            func_801E96DC();
            break;
        case 2:
            func_801E972C();
            break;
        }
        func_801E9784();
    } while (D_801F4804->state != 3);
    func_80014C08(30);
    func_80014A48(task);
}

void func_801E9988(UiWindow *window, WindowDef *def) {
    measureText((u8 *)D_801DE6B0);
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

const char D_801DE6B0[] = "               *c6SYSTEM ERROR\n*c2 Illegal Sharing: \n*c7 Unauthorized command was used.";

void func_801E9F28(void) {
    Rect16 uv = { 0, 0, 0x80, 0x80 };
    DrTPage tpages[2];
    PolyF4 polys[2];
    u8 brightness = 0;
    s32 i;

    D_801F4808 = 1;
    for (i = 0; i < 2; i++) {
        SetDrawTPage(&tpages[i], 0, 0, 0x40);
        func_80067784(&polys[i]);
        SetSemiTrans(&polys[i], 1);
        setPrimQuadRect(&polys[i], 0, 0, 0x140, 0xF0);
        setPrimRgb0(&polys[i], brightness, brightness, brightness);
    }
    do {
        func_80014C08(1);
        if (D_801F4808 == 1) {
            if (brightness < 0xF7) {
                brightness += 8;
            } else {
                brightness = 0xFF;
            }
        } else if (D_801F4808 == 2) {
            if (brightness > 8) {
                brightness -= 8;
            } else {
                brightness = 0;
                D_801F4808 = 0;
            }
        }
        if (D_801F4834 != 0) {
            D_801F4834--;
        }
        if (D_801F4808 != 0) {
            if ((D_801F4834 & 1) && brightness == 0xFF) {
                drawTexturedSprite(0x60, 0x1A, &uv, 0x95, 0x7C00, 0x18, 0x80, -1);
            }
            setPrimRgb0(&polys[FRAME_BUFFER_INDEX], brightness, brightness, brightness);
            addPrim(&CURRENT_FRAME_BUFFER->ot[24], &polys[FRAME_BUFFER_INDEX]);
            addPrim(&CURRENT_FRAME_BUFFER->ot[24], &tpages[FRAME_BUFFER_INDEX]);
        }
    } while (D_801F4808 != 0);
    func_80014C08(1);
}

void (*D_801F3670[5])() = {
    func_801E9A6C,
    func_801E9A6C,
    func_801E9A6C,
    func_801E9A6C,
    func_801E9A6C,
};

/* the characters of the name entry grid, ten to a row */
char D_801F3684[] =
    "ABCDEabcde"
    "FGHIJfghij"
    "KLMNOklmno"
    "PQRSTpqrst"
    "UVWXYuvwxy"
    "Z-   z    "
    "          "
    "          "
    "0123456789";

/* the names func_801EB1F8 recognises when one is typed in */
char *D_801F36E0[10] = {
    "OMNIMON-1",
    "WARGREYMON",
    "OMNIMON-2",
    "MTLGARURUMON",
    "A-VEEDRAMON",
    "H-KBUTERIMON",
    "VENOMMYOTIS",
    "PIEDMON",
    "MTLETEMON",
    "JIJIMON",
};

const char D_801DE788[] = "";

s32 func_801EA230(void) {
    Rect16 rect;
    UiWindow *win = &D_801F43D8;
    s16 r;

    if (D_801F4568.active != 0) {
        if (D_801F4568.mode == 0) {
            if ((u16)PAD_STATES[0]->repeat & 0xF000) {
                playSoundEffect(2);
            }
            do {
                if (PAD_STATES[0]->repeat & 0x1000) {
                    D_801F4568.row = ((s16)(D_801F4568.row - 1) + 1) / 9 * 9 + ((s16)(D_801F4568.row - 1) + 9) % 9;
                    if (D_801F4568.row % 9 == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & 0x4000) {
                    D_801F4568.row = ((s16)(D_801F4568.row + 1) - 1) / 9 * 9 + ((s16)(D_801F4568.row + 1) + 9) % 9;
                    if (D_801F4568.row % 9 == 8) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[0]->repeat & 0x8000) {
                    if (--D_801F4568.col < 0) {
                        D_801F4568.col = 9;
                        D_801F4568.mode = 1;
                        D_801F4568.sel = 7;
                    } else if (D_801F4568.col == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & 0x2000) {
                    if (++D_801F4568.col >= 10) {
                        D_801F4568.col = 0;
                        D_801F4568.mode = 1;
                        D_801F4568.sel = 7;
                    } else if (D_801F4568.col == 9) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                }
            } while (D_801F3684[D_801F4568.row * 10 + D_801F4568.col] == ' ');
        } else {
            if ((u16)PAD_STATES[0]->repeat & 0xF000) {
                playSoundEffect(2);
            }
            if ((u16)PAD_STATES[0]->pressed & 0xA000) {
                playSoundEffect(2);
            }
            if (PAD_STATES[0]->repeat & 0x1000) {
                if (--D_801F4568.sel < 7) {
                    D_801F4568.sel = 8;
                } else if (D_801F4568.sel == 7) {
                    PAD_STATES[0]->repeatEnabled = 0;
                }
            } else if (PAD_STATES[0]->repeat & 0x4000) {
                if (++D_801F4568.sel >= 9) {
                    D_801F4568.sel = 7;
                } else if (D_801F4568.sel == 8) {
                    PAD_STATES[0]->repeatEnabled = 0;
                }
            } else if ((u16)PAD_STATES[0]->pressed & 0x8000) {
                D_801F4568.mode = 0;
                D_801F4568.prevSel = -1;
                D_801F4568.prevRow = D_801F4568.prevCol = -1;
                D_801F4568.col = 9;
                while (D_801F3684[D_801F4568.row * 10 + D_801F4568.col] == ' ') {
                    D_801F4568.col--;
                }
            } else if (PAD_STATES[0]->pressed & 0x2000) {
                D_801F4568.mode = 0;
                D_801F4568.prevSel = -1;
                D_801F4568.prevRow = D_801F4568.prevCol = -1;
                D_801F4568.col = 0;
                while (D_801F3684[D_801F4568.row * 10 + D_801F4568.col] == ' ') {
                    D_801F4568.col++;
                }
            }
        }
    }
    if (D_801F4568.mode == 0) {
        if (D_801F4568.row != D_801F4568.prevRow || D_801F4568.col != D_801F4568.prevCol) {
            D_801F4568.prevCol = D_801F4568.col;
            D_801F4568.prevRow = D_801F4568.row;
            rect.x = win->rect.x - win->scroll[2] + D_801F4568.col * 17 + 4;
            rect.y = win->rect.y - win->scroll[3] + D_801F4568.row * 14 + 1;
            rect.w = 0xC;
            rect.h = 0xC;
            if (D_801F4568.col >= 5) {
                rect.x += 11;
            }
            moveCursorHighlight(&D_801F4428, &rect);
        }
    } else if (D_801F4568.sel != D_801F4568.prevSel) {
        D_801F4568.prevSel = D_801F4568.sel;
        switch (D_801F4568.sel) {
        case 0:
        case 1:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + D_801F4568.sel * 14 + 1;
            rect.w = 0x30;
            rect.h = 0xC;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + D_801F4568.sel * 14 + 1;
            rect.w = 0x24;
            rect.h = 0xC;
            break;
        case 7:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + 0x63;
            rect.w = 0x18;
            rect.h = 0xC;
            break;
        case 8:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + 0x71;
            rect.w = 0x30;
            rect.h = 0xC;
            break;
        }
        moveCursorHighlight(&D_801F4428, &rect);
    }
}

void func_801EA9B0(UiWindow *window) {
    char text[0x40];
    u8 marks[7];
    s32 i;
    s32 x = window->originX + 4;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 gap;
    s32 dx;
    s32 sel;

    for (i = 0; i < 90; i++) {
        sprintf(text, "%c", D_801F3684[i]);
        gap = 0;
        if (i % 10 >= 5) {
            gap = 11;
        }
        dx = (i % 10) * 17 + 4;
        drawText(x + dx + gap, y + (i / 10) * 14, (s32)text, 7, z);
    }
    for (i = 0; i < 7; i++) {
        marks[i] = 4;
    }
    marks[window->view.y / window->rect.h] = 5;
    drawText(window->rect.x + 0xCC, window->rect.y + 0x63, (s32)"OK", 6, z);
    drawText(window->rect.x + 0xCC, window->rect.y + 0x71, (s32)"Cancel", 6, z);
    func_801EA230();
    if (D_801F4568.mode == 0) {
        if (PAD_STATES[0]->repeat & 0x40) {
            D_801F4568.text[D_801F4568.cursor] = D_801F3684[D_801F4568.row * 10 + D_801F4568.col];
            playSoundEffect(0);
            if (D_801F4568.cursor < 11) {
                D_801F4568.cursor++;
            } else {
                D_801F4568.mode = 1;
                D_801F4568.sel = 7;
            }
        } else if (PAD_STATES[0]->repeat & 0x20) {
            for (i = 11; i >= D_801F4568.cursor + 1; i--) {
                D_801F4568.text[i] = D_801F4568.text[i - 1];
            }
            D_801F4568.text[D_801F4568.cursor] = D_801F3684[D_801F4568.row * 10 + D_801F4568.col];
            playSoundEffect(0);
            if (D_801F4568.cursor < 11) {
                D_801F4568.cursor++;
            } else {
                D_801F4568.mode = 1;
                D_801F4568.sel = 7;
            }
        } else if (PAD_STATES[0]->repeat & 0x10) {
            if (D_801F4568.text[0] != 0) {
                playSoundEffect(0);
            }
            if (D_801F4568.cursor == 0) {
                D_801F4568.cursor++;
            }
            for (i = D_801F4568.cursor; i < 13; i++) {
                D_801F4568.text[i - 1] = D_801F4568.text[i];
            }
            D_801F4568.cursor--;
        }
    } else if (PAD_STATES[0]->pressed & 0x40) {
        playSoundEffect(0);
        sel = D_801F4568.sel;
        if (sel >= 0) {
            if (sel < 7) {
                D_801F4568.row = sel * 9;
                D_801F4568.col = 0;
                scrollWindowTo(&window->originX, 0, D_801F4568.row * 14);
            } else if (sel < 9) {
                D_801F4568.result = D_801F4568.sel;
            }
        }
    } else if (PAD_STATES[0]->repeat & 0x10) {
        if (D_801F4568.text[0] != 0) {
            playSoundEffect(0);
        }
        if (D_801F4568.cursor == 0) {
            D_801F4568.cursor++;
        }
        for (i = D_801F4568.cursor; i < 13; i++) {
            D_801F4568.text[i - 1] = D_801F4568.text[i];
        }
        D_801F4568.cursor--;
    }
    if (PAD_STATES[0]->pressed & 0x800) {
        if (D_801F4568.mode != 1 || D_801F4568.sel != 7) {
            playSoundEffect(0);
            D_801F4568.mode = 1;
            D_801F4568.sel = 7;
        }
    }
    drawCursorHighlight(&D_801F4428, z);
}

void func_801EAF8C(UiWindow *window) {
    char text[0x40];
    Rect16 cursor;
    s32 x = window->originX + 1;
    s32 y = window->originY;
    s32 z = window->z;

    sprintf(text, "*s0%s", D_801F4568.text);
    drawText(x, y, (s32)text, 7, z);
    if (PAD_STATES[0]->repeat & 4) {
        if (D_801F4568.cursor != 0) {
            playSoundEffect(2);
            D_801F4568.cursor--;
        }
    } else if (PAD_STATES[0]->repeat & 8) {
        if (D_801F4568.cursor != 11 && D_801F4568.text[D_801F4568.cursor] != 0) {
            playSoundEffect(2);
            D_801F4568.cursor++;
        }
    }
    cursor.x = x + D_801F4568.cursor * 6;
    cursor.y = y + 13;
    cursor.w = 6;
    cursor.h = 0;
    moveCursorHighlight(&D_801F44C8, &cursor);
    drawCursorHighlight(&D_801F44C8, z);
}

void func_801EB0F8(UiWindow *window) {
    s32 x = window->originX + 1;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 unused[2];

    drawText(x, y, (s32)"*b0 Insert", 7, z);
    drawText(x + 8, y + 13, (s32)"*b2 OK", 7, z);
    drawText(x, y + 26, (s32)"*b1 Delete", 7, z);
}

void func_801EB198(void) {
    drawWindow(&D_801F43D8, func_801EA9B0, 0x1E);
    drawWindow(&D_801F4478, func_801EAF8C, 0x1E);
    drawWindow(&D_801F4518, func_801EB0F8, 0x1E);
}

void func_801EB1F8(char *word) {
    Rect16 cursorRect;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];
    s32 i;

    bzero((Scene3D *)D_801F4568.text, 13);
    strcpy(D_801F4568.text, word);
    D_801F4568.col = 0;
    D_801F4568.row = 0;
    D_801F4568.prevCol = -1;
    D_801F4568.prevRow = -1;
    D_801F4568.active = 1;
    D_801F4568.cursor = 0;
    D_801F4568.mode = 0;
    D_801F4568.sel = 0;
    D_801F4568.prevSel = -1;
    D_801F4568.result = 0;
    rect.x = 0x10;
    rect.y = 0x5C;
    rect.w = 0x10E;
    rect.h = 0x7E;
    D_801F4568.unk17 = 9;
    view.x = 0;
    view.y = 0;
    view.w = rect.w;
    view.h = D_801F4568.unk17 * 14;
    openWindow(&D_801F43D8, &rect, -1, (s16 *)&view, 10, 0x26, 0x80, 12);
    D_801F43D8.label = (s32)"WORD INPUT";
    cursorRect.x = D_801F43D8.originX + 4;
    cursorRect.y = D_801F43D8.originY + 1;
    cursorRect.w = 12;
    cursorRect.h = 12;
    initCursorHighlight(&D_801F4428, &cursorRect, (Bytes4 *)-1);
    rect.x = 0x1E;
    rect.y = 0x1C;
    rect.w = 0x34;
    rect.h = 0x28;
    openWindow(&D_801F4518, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    D_801F4518.label = (s32)"HELP";
    rect.x = 0x78;
    rect.y = 0x1C;
    rect.w = 0x48;
    rect.h = 0xE;
    openWindow(&D_801F4478, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    D_801F4478.label = (s32)"KEYWORD";
    cursorRect.x = D_801F4478.originX;
    cursorRect.y = D_801F4478.originY + 13;
    cursorRect.w = 12;
    cursorRect.h = 0;
    initCursorHighlight(&D_801F44C8, &rect, (Bytes4 *)-1);
    playSoundEffect(3);
    addFrameCallback((s32)func_801EB198);
    while (1) {
        func_80014C08(FRAME_INTERVAL);
        if (D_801F4568.result == 0) {
            continue;
        }
        if (D_801F4568.result == 7 && D_801F4568.text[0] == 0) {
            initDialog(dialog, D_801DE804, 0);
            runDialog(dialog);
            D_801F4568.result = 0;
            continue;
        }
        break;
    }
    D_801F4838[0]->regs[1] = -2;
    if (D_801F4568.result == 7) {
        strcpy(word, D_801F4568.text);
        D_801F4838[0]->regs[1] = -1;
    }
    playSoundEffect(4);
    animateWindowTo(&D_801F4518, (Rect16 *)-1);
    animateWindowTo(&D_801F43D8, (Rect16 *)-1);
    animateWindowTo(&D_801F4478, (Rect16 *)-1);
    func_80014C08(20);
    removeFrameCallback((s32)func_801EB198);
    for (i = 0; i < 10; i++) {
        if (strcmp(word, D_801F36E0[i]) == 0) {
            D_801F4838[0]->regs[1] = i;
            break;
        }
    }
    D_801F4696 = 0;
    func_801E108C(1);
}

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

/* the last three bytes are leftovers in the original, not zero padding */
const char D_801DE804[36] = "A Key Word has not been entered!\0" "333";

void func_801EB628(void) {
    char path[0x48];
    s32 area = ((PlayerProfile *)PLAYER_PROFILES)->unkE;

    SESSION->loading = 1;
    sprintf(path, "C:\\area%2.2d.pak", area);
    func_800149B8(0, -1, 0, 0x400, loadFileTagged, path, getCurrentTaskId(), 0x31);
    SESSION->pak = (Chunk *)func_80014C08(0x7FFFFFFF);
    func_801EB5C0((u8 *)SESSION->pak);
    truncatePakTextures(SESSION->pak);
    SESSION->unk190 = findPakChunk(SESSION->pak, 2, area + 200);
    SESSION->loading = 0;
}

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

void func_801EBA54(Sprite3D *sprite) {
    MATRIX matrix;
    SVECTOR corners[4];
    s32 sxy[4];
    s32 depthCue;
    s32 flag;

    buildRotTransMatrix(&sprite->pos, &sprite->rot, &matrix);
    CompMatrix((MATRIX *)SCENE_3D->unk78, &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    func_8005C444(&matrix);
    corners[0] = sprite->corners[0];
    corners[1] = sprite->corners[1];
    corners[2] = sprite->corners[2];
    corners[3] = sprite->corners[3];
    RotAverage4(&corners[0], &corners[1], &corners[2], &corners[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
    sprite->quads[FRAME_BUFFER_INDEX].x0 = sxy[0];
    sprite->quads[FRAME_BUFFER_INDEX].y0 = sxy[0] >> 16;
    sprite->quads[FRAME_BUFFER_INDEX].x1 = sxy[1];
    sprite->quads[FRAME_BUFFER_INDEX].y1 = sxy[1] >> 16;
    sprite->quads[FRAME_BUFFER_INDEX].x2 = sxy[2];
    sprite->quads[FRAME_BUFFER_INDEX].y2 = sxy[2] >> 16;
    sprite->quads[FRAME_BUFFER_INDEX].x3 = sxy[3];
    sprite->quads[FRAME_BUFFER_INDEX].y3 = sxy[3] >> 16;
    addPrim(&CURRENT_FRAME_BUFFER->ot[sprite->otz], &sprite->quads[FRAME_BUFFER_INDEX]);
}

void func_801EBD28(Sprite3D *arg0, s16 dx, s16 dy) {
    arg0->corners[2].vx += dx;
    arg0->corners[0].vx = arg0->corners[2].vx;
    arg0->corners[3].vx += dx;
    arg0->corners[1].vx = arg0->corners[3].vx;
    arg0->corners[1].vy += dy;
    arg0->corners[0].vy = arg0->corners[1].vy;
    arg0->corners[3].vy += dy;
    arg0->corners[2].vy = arg0->corners[3].vy;
}

void func_801EBD8C(Sprite3D *arg0, s32 arg1) {
    arg0->otz = arg1;
}

void func_801EBD94(Unk801EBD94 *arg0, u8 value) {
    s32 i;

    for (i = 0; i < 2; i++) {
        arg0[i].r = value;
        arg0[i].g = value;
        arg0[i].b = value;
    }
}

void func_801EBDD0(Sprite3D *arg0, s16 width, s16 height) {
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

void func_801EBE0C(Sprite3D *sprite, s32 abr) {
    s16 tpage;

    if (abr >= 0) {
        SetSemiTrans(&sprite->quads[0], 1);
        SetSemiTrans(&sprite->quads[1], 1);
        tpage = sprite->quads[0].tpage & ~0x60;
        tpage |= (abr & 3) << 5;
        sprite->quads[1].tpage = tpage;
        sprite->quads[0].tpage = tpage;
    } else {
        SetSemiTrans(&sprite->quads[0], 0);
        SetSemiTrans(&sprite->quads[1], 0);
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

void func_801EBFB4(s32 useMap) {
    /* not a literal: GCC would share func_801E4754's identical string */
    static const char worldPath[] = "C:\\OBJECT\\world.TIS";
    char path[0x48];
    u32 *pack;

    *(s32 *)&((SessionData *)D_8006E054)->unk100C->unk0[0x194] = 1;
    if (useMap == 0) {
        sprintf(path, worldPath);
    } else {
        sprintf(path, "C:\\OBJECT\\map.TIS");
    }
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    *(s32 *)&((SessionData *)D_8006E054)->unk100C->unk0[0x194] = 0;
}

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

    D_801F4E48.x = (t * 270 + (15 - t) * 136) / 15;
    D_801F4E48.y = (t * 9 + (15 - t) * 96) / 15;
}

void func_801EC1D8(s32 state) {
    Rect16 uv[2];
    u8 brightness;
    s32 dir;

    if (state == 3) {
        state = 2;
    } else if (state == 4) {
        /* not a Rect16: GCC would share func_801E4AF4's identical constant */
        s16 rect[4] = { 0x220, 0xE1, 0x20, 1 };

        changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->activePartner, 0x380, 0, 0x380, 0x80);
        func_80014C08(2);
        MoveImage2((Rect16 *)rect, 0x380, 0x80);
        setBackgroundScrollMode(1);
        state = 2;
    } else {
        changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->activePartner, 0x380, 0, 0x380, 0x80);
    }
    uv[0].x = 0;
    uv[0].y = 0;
    uv[0].w = 0x30;
    uv[0].h = 0x30;
    uv[1].x = 0x30;
    uv[1].y = 0;
    uv[1].w = 0x30;
    uv[1].h = 0x30;
    brightness = 0x80;
    dir = 1;
    D_801F4E48.x = 0x88;
    D_801F4E48.y = 0x60;
    D_801F4E48.running = 1;
    D_801F4E48.unk402 = state;
    do {
        func_80014C08(1);
        if (D_801F3D00[D_801F4E48.unk402] != NULL) {
            D_801F3D00[D_801F4E48.unk402]();
        }
        if (dir == 1) {
            brightness += 4;
        } else if (dir == -1) {
            brightness -= 4;
        }
        if (brightness >= 0xD8) {
            brightness = 0xD8;
            dir = -1;
        } else if (brightness < 0x70) {
            brightness = 0x70;
            dir = 1;
        }
        drawTexturedSprite(D_801F4E48.x, D_801F4E48.y, &uv[1], 0x28, 0x39E0, 0x19, brightness, 1);
        drawTexturedSprite(D_801F4E48.x, D_801F4E48.y, &uv[0], 8, 0x3960, 0x19, 0x80, -1);
    } while (D_801F4E48.running != 0);
}

void func_801EC41C(void) {
    D_801F4E48.unk3D4++;
    if (D_801F4E48.unk3D4 > 15) {
        D_801F4E48.unk3D4 = 15;
    }
    D_801F4E48.unk3E0 = (D_801F4E48.zoom << 10) / 35;
    D_801F5260[0]->pos.vx = D_801F5260[1]->pos.vx = D_801F4E48.unk3D4 * 134 / 15;
    D_801F5260[0]->pos.vy = D_801F5260[1]->pos.vy = D_801F4E48.unk3D4 * 87 / 15;
}

void func_801EC51C(void) {
    s32 t = D_801F4E48.unk3D4;

    D_801F4E48.x = (t * 270 + (15 - t) * 136) / 15;
    D_801F4E48.y = (t * 183 + (15 - t) * 96) / 15;
}

void func_801EC5C4(void) {
    s32 i;
    s8 node;

    D_801F4E48.selected = D_801F4E48.area;
    D_801F4E48.route = D_801F3CD0[D_801F4E48.area];
    D_801F4E48.node = &D_801F3C30[D_801F4E48.nodeIndex];
    D_801F5260[0] = func_801EB754(0);
    D_801F5260[0]->pos.vx = D_801F3C30[D_801F4E48.nodeIndex].x;
    D_801F5260[0]->pos.vy = D_801F3C30[D_801F4E48.nodeIndex].y - 15;
    func_801EBD8C(D_801F5260[0], 0x1E);
    D_801F5260[1] = func_801EB754(1);
    func_801EBD8C(D_801F5260[1], 0x1F);
    for (i = 0; i < 2; i++) {
        SetSemiTrans(&D_801F5260[1]->quads[i], 1);
        D_801F5260[1]->quads[i].tpage = 0xC7;
    }
    D_801F4E48.alpha = 0xFF;
    D_801F4E48.lastArea = D_801F4E48.area;
    D_801F4E48.unk3FB = 0;
    for (i = 0; i < 7; i++) {
        node = D_801F3CD0[D_801F4E48.area][i];
        if (node < 0) {
            continue;
        }
        if (node >= 12) {
            D_801F5260[i + 21] = func_801EB754(0x40);
        } else {
            D_801F5260[i + 21] = func_801EB754(9);
        }
        func_801EBD28(D_801F5260[i + 21], D_801F3C30[node].x, D_801F3C30[node].y);
        func_801EBD8C(D_801F5260[i + 21], 0x20);
        D_801F4E48.unk3FB++;
    }
    func_801F082C();
}

void func_801EC844(void) {
    s32 x;
    s32 i;

    if (D_801F5258.offset != 0) {
        return;
    }
    x = ((20 - D_801F4E48.unk3F0) * -214 + D_801F4E48.unk3F0 * -117) / 20;
    if (D_801F4E48.unk400 == 0) {
        D_801F4E48.unk3F0++;
        if (D_801F4E48.unk3F0 > 20) {
            D_801F4E48.unk3F0 = 20;
            D_801F4E48.unk400 = 1;
            D_801F4E48.unk3FF = 0;
        }
    } else if (D_801F4E48.unk400 == 1) {
        if (PAD_STATES[0]->repeat & 0x4000) {
            playSoundEffect(2);
            D_801F4E48.unk3FF++;
            if (D_801F4E48.unk3FF >= 3) {
                D_801F4E48.unk3FF = 0;
            }
        } else if (PAD_STATES[0]->repeat & 0x1000) {
            playSoundEffect(2);
            D_801F4E48.unk3FF--;
            if (D_801F4E48.unk3FF < 0) {
                D_801F4E48.unk3FF = 2;
            }
        } else if (PAD_STATES[0]->pressed & 0x30) {
            playSoundEffect(1);
            D_801F4E48.unk400 = 2;
            D_801F4E48.unk401 = 0;
        } else if (PAD_STATES[0]->pressed & 0x40) {
            if (((SessionData *)D_8006E054)->unk1027 != 1 || D_801F5247 != 2) {
                playSoundEffect(0);
                D_801F4E48.unk400 = 2;
                D_801F4E48.unk401 = 1;
                D_801F525C = 3;
            }
        }
    } else if (D_801F4E48.unk400 == 2) {
        D_801F4E48.unk3F0--;
        if (D_801F4E48.unk3F0 < 0) {
            D_801F4E48.unk3F0 = 0;
            D_801F4E48.unk400 = 3;
        }
    } else if (D_801F4E48.unk400 == 3) {
        if (D_801F4E48.unk401 == 0) {
            D_801F4E48.state = 1;
        } else {
            D_801F524B = 6;
        }
        D_801F5248 = 4;
    }
    for (i = 30; i < 34; i++) {
        if (i == 30) {
            D_801F5260[i]->pos.vx = x + 1;
        } else {
            D_801F5260[i]->pos.vx = x + 3;
        }
        if (i != 30) {
            if (((SessionData *)D_8006E054)->unk1027 == 1) {
                if (D_801F4E48.unk3FF == i - 31) {
                    if (i == 33) {
                        D_801F5260[i]->quads[0].clut = getClut(0x200, 0xF1);
                        D_801F5260[i]->quads[1].clut = getClut(0x200, 0xF1);
                    } else {
                        D_801F5260[i]->quads[0].clut = getClut(0x200, 0xF0);
                        D_801F5260[i]->quads[1].clut = getClut(0x200, 0xF0);
                    }
                } else if (i == 33) {
                    D_801F5260[i]->quads[0].clut = getClut(0x200, 0xF2);
                    D_801F5260[i]->quads[1].clut = getClut(0x200, 0xF2);
                } else {
                    D_801F5260[i]->quads[0].clut = getClut(0x200, 0xEF);
                    D_801F5260[i]->quads[1].clut = getClut(0x200, 0xEF);
                }
            } else if (D_801F4E48.unk3FF == i - 31) {
                D_801F5260[i]->quads[0].clut = getClut(0x200, 0xF0);
                D_801F5260[i]->quads[1].clut = getClut(0x200, 0xF0);
            } else {
                D_801F5260[i]->quads[0].clut = getClut(0x200, 0xEF);
                D_801F5260[i]->quads[1].clut = getClut(0x200, 0xEF);
            }
        }
        if (D_801F4E48.unk400 < 3) {
            func_801EBA54(D_801F5260[i]);
        } else if (D_801F4E48.unk400 == 4) {
            if (D_801F5258.state != 3) {
                D_801F5258.state = 1;
            }
            func_801EBA34(D_801F5260[i]);
        }
    }
}

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

void func_801ECD2C(void) {
    s16 xs[6] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xCF, 0x8E };
    s16 ys[6] = { 0x54, 0x4B, 0x4C, 0x4C, 0x30, 0x4B };
    s32 i;

    for (i = 0; i < 6; i++) {
        D_801F5350[i].x = xs[i];
        D_801F5350[i].y = ys[i];
        D_801F5350[i].timer = 0;
        D_801F5350[i].frames = 6;
        D_801F5350[i].frameTime = 4;
    }
    D_801F5350[4].frameTime = 8;
    for (i = 0; i < 6; i++) {
        D_801F5350[i].frameTime *= 2;
    }
}

void func_801ECE60(void) {
    s32 x = 0xAB;
    s32 y = 0x28;

    D_801F5350->x = x;
    D_801F5350->y = y;
    D_801F5350->timer = 0;
    D_801F5350->frames = 4;
    D_801F5350->frameTime = 6;
    D_801F5350->frameTime *= 2;
}

void func_801ECEB0(void) {
    s16 xs[7] = { 0x6B, 0x9E, 0x90, 0xA8, 0xCA, 0xDC, 0xD7 };
    s16 ys[7] = { 0x30, 0x60, 0x75, 0x8B, 0x8F, 0x7A, 0x62 };

    D_801F5350->x = xs[0];
    D_801F5350->y = ys[0];
    D_801F5350->timer = 0;
    D_801F5350->frames = 6;
    D_801F5350->frameTime = 4;
    D_801F5350->frameTime *= 2;
}

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

void func_801ED014(void) {
    Rect16 uv[8] = {
        { 0x00, 0x00, 0x20, 0x10 },
        { 0x20, 0x00, 0x20, 0x10 },
        { 0x40, 0x00, 0x20, 0x10 },
        { 0x60, 0x00, 0x20, 0x10 },
        { 0x80, 0x00, 0x28, 0x20 },
        { 0xD0, 0x00, 0x20, 0x10 },
        { 0x50, 0x00, 0x20, 0x1A },
        { 0x50, 0x1A, 0x20, 0x20 },
    };
    s32 i;
    u8 frame;

    for (i = 0; i < 6; i++) {
        D_801F5350[i].timer++;
        if (D_801F5350[i].timer >= D_801F5350[i].frames * D_801F5350[i].frameTime) {
            D_801F5350[i].timer = 0;
        }
        frame = D_801F5350[i].timer / D_801F5350[i].frameTime;
        if (i == 4) {
            uv[4].x += uv[4].w * (frame / 3);
            uv[4].y = (frame % 3) * uv[4].h;
            drawTexturedSprite(D_801F5350[i].x, D_801F5350[i].y, &uv[4], 0x3E, 0x7C3B, 0x23, 0x80, 1);
        } else {
            uv[i].y = frame * uv[i].h;
            drawTexturedSprite(D_801F5350[i].x, D_801F5350[i].y, &uv[i], 0x3E, 0x7C3B, 0x21, 0x80, 1);
        }
    }
    drawTexturedSprite(0xCC, 0x74, &uv[6], 0x9B, 0x6A18, 0x21, 0x80, 1);
    drawTexturedSprite(0x8F, 0x46, &uv[7], 0x9B, 0x6A18, 0x21, 0x80, 1);
}

void func_801ED2E0(void) {
    Rect16 uv[4] = {
        { 0x40, 0x00, 0x48, 0x48 },
        { 0x20, 0x60, 0x48, 0x48 },
        { 0x20, 0xA8, 0x48, 0x48 },
        { 0x20, 0x60, 0x48, 0x48 },
    };
    u8 frame;

    D_801F5350->timer++;
    if (D_801F5350->timer >= D_801F5350->frames * D_801F5350->frameTime) {
        D_801F5350->timer = 0;
    }
    frame = D_801F5350->timer / D_801F5350->frameTime;
    if (frame != 0) {
        drawTexturedSprite(D_801F5350->x, D_801F5350->y, &uv[frame], 0x9E, 0x6A58, 0x23, 0x80, -1);
    }
}

void func_801ED42C(void) {
    Rect16 uv[8] = {
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
    };
    u8 frame;

    drawTexturedSprite(0xBE, 0x28, &uv[1], 0x9B, 0x6A98, 0x1D, 0x80, 1);
    D_801F5350->timer++;
    if (D_801F5350->timer >= D_801F5350->frames * D_801F5350->frameTime) {
        D_801F5350->timer = 0;
    }
    frame = D_801F5350->timer / D_801F5350->frameTime;
    if (frame != 0) {
        uv[0].y = (frame - 1) * uv[0].h + 0x60;
        drawTexturedSprite(D_801F5350->x, D_801F5350->y, &uv[0], 0x9E, 0x6A98, 0x23, 0x80, -1);
    }
}

void func_801ED5D4(void) {
    s32 i;

    D_801F4E48.zoom++;
    if (D_801F4E48.zoom >= 36) {
        D_801F4E48.zoom = 35;
    }
    if (D_801F521C == 5) {
        playSoundEffect(10);
    }
    if (D_801F4E48.zoom > 20) {
        D_801F4E48.unk402 = 1;
        D_801F4E48.unk3D4++;
        if (D_801F4E48.unk3D4 >= 16) {
            D_801F4E48.unk3D4 = 15;
            if (D_801F4C58 < 0x80) {
                D_801F4C58 += 8;
            } else {
                D_801F4E48.state = 3;
                func_801EC5C4();
                D_801F4E48.unk3FE = 2;
                D_801F4E48.slideDir = 1;
            }
            if (D_801F4C58 >= 0x80) {
                D_801F4C58 = 0x80;
            }
        }
    }
    D_801F4E48.unk3E0 = (D_801F4E48.zoom << 10) / 35;
    if (D_801F4E48.unk3E0 > 0x400) {
        D_801F4E48.unk3E0 = 0x400;
    }
    for (i = 40; i < 48; i++) {
        setRGB0(&D_801F5260[i]->quads[FRAME_BUFFER_INDEX], D_801F4C58, D_801F4C58, D_801F4C58);
        D_801F5260[i]->pos.vx = D_801F4E48.unk3D4 * 38 / 15;
        D_801F5260[i]->pos.vy = D_801F4E48.unk3D4 / 15;
        D_801F5260[i]->pos.vz = rcos(D_801F4E48.unk3E0) * 2 / 3;
        D_801F5260[i]->rot.vz = -(D_801F4E48.zoom << 12) / 35;
    }
    for (i = 48; i < 56; i++) {
        setRGB0(&D_801F5260[i]->quads[FRAME_BUFFER_INDEX], D_801F4C58, D_801F4C58, D_801F4C58);
        D_801F5260[i]->pos.vx = D_801F4E48.unk3D4 * 38 / 15;
        D_801F5260[i]->pos.vy = D_801F4E48.unk3D4 / 15;
        D_801F5260[i]->pos.vz = rcos(D_801F4E48.unk3E0) * 2 / 3;
        D_801F5260[i]->rot.vz = (D_801F4E48.zoom << 12) / 35;
    }
    setRGB0(&D_801F5260[2]->quads[FRAME_BUFFER_INDEX], D_801F4C58, D_801F4C58, D_801F4C58);
    D_801F5260[2]->pos.vx = D_801F4E48.unk3D4 * 38 / 15;
    D_801F5260[2]->pos.vy = D_801F4E48.unk3D4 / 15;
    D_801F5260[2]->pos.vz = rcos(D_801F4E48.unk3E0) * 2 / 3;
    D_801F5260[2]->rot.vz = (D_801F4E48.zoom << 12) / 35;
}

void func_801EDB90(void) {
    if (D_801F524B == 3 || D_801F524B == 0) {
        D_801F4E48.fades[FRAME_BUFFER_INDEX].r0 = D_801F4E48.fades[FRAME_BUFFER_INDEX].g0 = D_801F4E48.fades[FRAME_BUFFER_INDEX].b0 = D_801F4E48.alpha;
        D_801F4E48.fades[FRAME_BUFFER_INDEX].x0 = D_801F5260[2]->quads[FRAME_BUFFER_INDEX].x0;
        D_801F4E48.fades[FRAME_BUFFER_INDEX].y0 = D_801F5260[2]->quads[FRAME_BUFFER_INDEX].y0;
        D_801F4E48.fades[FRAME_BUFFER_INDEX].x1 = D_801F5260[2]->quads[FRAME_BUFFER_INDEX].x1;
        D_801F4E48.fades[FRAME_BUFFER_INDEX].y1 = D_801F5260[2]->quads[FRAME_BUFFER_INDEX].y1;
        D_801F4E48.fades[FRAME_BUFFER_INDEX].x2 = D_801F5260[2]->quads[FRAME_BUFFER_INDEX].x2;
        D_801F4E48.fades[FRAME_BUFFER_INDEX].y2 = D_801F5260[2]->quads[FRAME_BUFFER_INDEX].y2;
        D_801F4E48.fades[FRAME_BUFFER_INDEX].x3 = D_801F5260[2]->quads[FRAME_BUFFER_INDEX].x3;
        D_801F4E48.fades[FRAME_BUFFER_INDEX].y3 = D_801F5260[2]->quads[FRAME_BUFFER_INDEX].y3;
        addPrim(&CURRENT_FRAME_BUFFER->ot[28], &D_801F4E48.fades[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[28], &D_801F4E48.tpages[FRAME_BUFFER_INDEX]);
    }
}

void func_801EDE58(void) {
    s32 i;

    D_801F525C = 2;
    D_801F4E48.state = 9;
    D_801F4E48.unk400 = 0;
    D_801F4E48.unk3F0 = 0;
    D_801F4E48.unk3FF = 100;
    D_801F5260[30] = func_801EB754(0x16);
    D_801F5260[31] = func_801EB754(0x18);
    D_801F5260[32] = func_801EB754(0x19);
    D_801F5260[33] = func_801EB754(0x17);
    if (((SessionData *)D_8006E054)->unk1027 == 1) {
        D_801F5260[33]->quads[0].clut = getClut(0x200, 0xF1);
        D_801F5260[33]->quads[1].clut = getClut(0x200, 0xF1);
    }
    D_801F5260[30]->pos.vy = -0x24;
    func_801EBD8C(D_801F5260[30], 0x1A);
    D_801F5260[31]->pos.vy = -0x39;
    func_801EBD8C(D_801F5260[31], 0x1B);
    D_801F5260[32]->pos.vy = -0x29;
    func_801EBD8C(D_801F5260[32], 0x1B);
    D_801F5260[33]->pos.vy = -0x19;
    func_801EBD8C(D_801F5260[33], 0x1B);
    func_801EBE0C(D_801F5260[30], 1);
    for (i = 0; i < 3; i++) {
        func_801EBE0C(D_801F5260[i + 31], 0);
    }
}

void func_801EDFB8(void) {
    s32 value;

    if (D_801F4E48.state == 3) {
        value = D_801F4E48.alpha - 8;
        if (value < 0) {
            value = 0;
            if (D_801F4E48.unk40B == 0) {
                D_801F4E48.state = 1;
                D_801F525C = 1;
            } else {
                D_801F5253 = 0;
                func_801EDE58();
            }
        }
        D_801F5242 = value;
    }
}

void func_801EE038(void) {
    if (D_801F4E48.nodeIndex >= 0 && D_801F4E48.nodeIndex < 12) {
        D_801F4E48.state = 6;
    } else if (D_801F4E48.nodeIndex == 12) {
        D_801F4E48.nodeIndex = 13;
        D_801F4E48.area = 1;
        D_801F4E48.state = 4;
        playSoundEffect(5);
    } else if (D_801F4E48.nodeIndex == 13) {
        D_801F4E48.nodeIndex = 12;
        D_801F4E48.area = 0;
        D_801F4E48.state = 4;
        playSoundEffect(5);
    } else if (D_801F4E48.nodeIndex == 14) {
        D_801F4E48.nodeIndex = 15;
        D_801F4E48.area = 2;
        D_801F4E48.state = 4;
        playSoundEffect(5);
    } else if (D_801F4E48.nodeIndex == 15) {
        D_801F4E48.nodeIndex = 14;
        D_801F4E48.area = 1;
        D_801F4E48.state = 4;
        playSoundEffect(5);
    }
    D_801F51C8 = -1;
}

void func_801EE13C(void) {
    s32 dir = -1;
    s32 dx;
    s32 dy;

    if (D_801F4E48.moving == 0) {
        stopSoundVoice(0x17);
        if (PAD_STATES[0]->held & 0x1000) {
            dir = 0;
        } else if (PAD_STATES[0]->held & 0x2000) {
            dir = 1;
        } else if (PAD_STATES[0]->held & 0x4000) {
            dir = 2;
        } else if ((u16)PAD_STATES[0]->held & 0x8000) {
            dir = 3;
        } else if (PAD_STATES[0]->pressed & 0x40) {
            dir = 4;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            dir = 5;
        }
        if (dir == -1) {
            return;
        }
        if (dir == 4) {
            func_801EE038();
        } else if (dir == 5) {
            playSoundEffect(1);
            func_801EDE58();
        } else {
            if (D_801F4E48.node->next[dir] != -1) {
                D_801F4E48.target = &D_801F3C30[D_801F4E48.node->next[dir]];
            } else {
                if (PAD_STATES[0]->pressed & 0x40) {
                    func_801EE038();
                } else if (PAD_STATES[0]->pressed & 0x10) {
                    playSoundEffect(1);
                    func_801EDE58();
                }
                return;
            }
            if (D_801F4E48.target->unlocked != 1) {
                if (PAD_STATES[0]->pressed & 0x40) {
                    func_801EE038();
                } else if (PAD_STATES[0]->pressed & 0x10) {
                    playSoundEffect(1);
                    func_801EDE58();
                }
                return;
            }
            playSoundEffectOnVoice(0x17, 7);
            D_801F4E48.nodeIndex = D_801F4E48.node->next[dir];
            D_801F4E48.moving++;
            dx = D_801F4E48.target->x - D_801F4E48.node->x;
            dy = D_801F4E48.target->y - D_801F4E48.node->y;
            D_801F4E48.angle = ratan2(dy, dx);
            D_801F4E48.distance = csqrt(dx * dx + dy * dy);
            D_801F4E48.distance /= 64;
            D_801F524B = 2;
        }
    }
}

void func_801EE40C(Sprite3D *sprite, Rect16 *rect, s8 flip) {
    s32 i;

    for (i = 0; i < 2; i++) {
        sprite->quads[i].tpage = 0x80 | ((rect->y & 0x100) >> 4) | ((rect->x & 0x3C0) >> 6) | ((rect->y & 0x200) << 2);
        if (flip == -1) {
            sprite->quads[i].u0 = sprite->quads[i].u2 = ((rect->x % 64) << 1) + rect->w;
            sprite->quads[i].u1 = sprite->quads[i].u3 = (rect->x % 64) << 1;
        } else {
            sprite->quads[i].u0 = sprite->quads[i].u2 = (rect->x % 64) << 1;
            sprite->quads[i].u1 = sprite->quads[i].u3 = sprite->quads[i].u2 + rect->w;
        }
        sprite->quads[i].v0 = sprite->quads[i].v1 = rect->y;
        sprite->quads[i].v2 = sprite->quads[i].v3 = rect->y + rect->h;
    }
}

void func_801EE5A8(Sprite3D *sprite, Rect16 *rect) {
    s32 i;

    for (i = 0; i < 2; i++) {
        sprite->quads[i].tpage = ((rect->y & 0x100) >> 4) | ((rect->x & 0x3C0) >> 6) | ((rect->y & 0x200) << 2);
        sprite->quads[i].u0 = sprite->quads[i].u2 = (rect->x % 64) << 2;
        sprite->quads[i].u1 = sprite->quads[i].u3 = sprite->quads[i].u2 + rect->w;
        sprite->quads[i].v0 = sprite->quads[i].v1 = rect->y;
        sprite->quads[i].v2 = sprite->quads[i].v3 = rect->y + rect->h;
    }
}

void func_801EE690(void) {
    Rect16 rect;
    s32 angle = D_801F4E48.angle & 0xFFF;
    s8 flip = 0;

    D_801F4E48.moving++;
    if (D_801F4E48.moving >= D_801F4E48.distance) {
        D_801F4E48.moving = 0;
        D_801F5260[0]->pos.vx = D_801F4E48.target->x;
        D_801F5260[0]->pos.vy = D_801F4E48.target->y - 15;
        D_801F4E48.node = D_801F4E48.target;
        D_801F4E48.state = 1;
    } else {
        D_801F5260[0]->pos.vx = rcos(D_801F4E48.angle) * D_801F4E48.moving / 4096;
        D_801F5260[0]->pos.vy = rsin(D_801F4E48.angle) * D_801F4E48.moving / 4096;
        D_801F5260[0]->pos.vx += D_801F4E48.node->x;
        D_801F5260[0]->pos.vy += D_801F4E48.node->y - 15;
    }
    if (angle < 0x400) {
        flip = -1;
        rect.x = D_801F4E48.moving / 6 % 6 * 13 + 0x19A;
        rect.y = 0xA8;
    } else if (angle < 0x800) {
        rect.x = D_801F4E48.moving / 6 % 6 * 13 + 0x19A;
        rect.y = 0xA8;
    } else {
        if (angle >= 0xC00) {
            flip = -1;
        }
        rect.x = D_801F4E48.moving / 6 % 6 * 13 + 0x19A;
        rect.y = 0xD0;
    }
    rect.w = 0x1A;
    rect.h = 0x28;
    if (D_801F4E48.moving == 0) {
        rect.x = 0x180;
    }
    func_801EE40C(D_801F5260[0], &rect, flip);
}

void func_801EE9A0(void) {
    s32 alpha = D_801F4E48.alpha + 8;

    if (alpha > 0xFF) {
        alpha = 0xFF;
        D_801F4E48.unk3FC = D_801F4E48.unk3FB;
        D_801F4E48.unk3FB = 0;
        if (D_801F4E48.state == 4) {
            D_801F4E48.state = 5;
            func_801F0078();
        } else {
            if (D_801F4E48.unk401 != 1) {
                ((PlayerProfile *)PLAYER_PROFILES)->unkE = SESSION->area = D_801F4E48.nodeIndex;
                func_800149B8(0, -1, 0, 0x400, func_801EB628, 0, getCurrentTaskId, 0, 0);
            }
            D_801F525C = 3;
            D_801F4E48.selected = -1;
            D_801F4E48.slideDir = 2;
            D_801F4E48.unk3FE = 4;
            D_801F4E48.state = 7;
            playSoundEffect(11);
        }
        removeFrameCallback((s32)func_801F0A08);
        D_801F5251 = 0;
    }
    D_801F4E48.alpha = alpha;
    D_801F4E48.fades[FRAME_BUFFER_INDEX].r0 = D_801F4E48.fades[FRAME_BUFFER_INDEX].g0 = D_801F4E48.fades[FRAME_BUFFER_INDEX].b0 = alpha;
    addPrim(&CURRENT_FRAME_BUFFER->ot[29], &D_801F4E48.fades[FRAME_BUFFER_INDEX]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[29], &D_801F4E48.tpages[FRAME_BUFFER_INDEX]);
}

void func_801EEBD0(void) {
    s32 i = 0;

    if (D_801F5244 != 0) {
        do {
            func_801EBA34(D_801F5260[i + 21]);
            i++;
        } while (i < D_801F4E48.unk3FC);
    }
    func_801EBA34(D_801F5260[0]);
    func_801EBA34(D_801F5260[1]);
    D_801F524B = 3;
    func_801EC5C4();
}

void func_801EEC78(void) {
    s32 i;

    if (D_801F4E48.unk3D4 == 0) {
        D_801F4E48.zoom--;
        if (D_801F4E48.zoom < 0) {
            D_801F4E48.zoom = 0;
        }
        D_801F4E48.unk3E0 = (D_801F4E48.zoom << 10) / 35;
        if (D_801F4E48.unk3E0 <= 0) {
            D_801F4E48.unk3E0 = 0;
        }
    } else {
        D_801F4E48.unk3D4--;
        if (D_801F4E48.unk3D4 < 0) {
            D_801F4E48.unk3D4 = 0;
        }
    }
    for (i = 40; i < 48; i++) {
        setRGB0(&D_801F5260[i]->quads[FRAME_BUFFER_INDEX], D_801F4C58, D_801F4C58, D_801F4C58);
        D_801F5260[i]->pos.vx = D_801F4E48.zoom * 38 / 35;
        D_801F5260[i]->pos.vy = D_801F4E48.zoom / 35;
        D_801F5260[i]->pos.vz = rcos(D_801F4E48.unk3E0) * 2 / 3;
        D_801F5260[i]->rot.vz = -(D_801F4E48.zoom << 12) / 35;
    }
    for (i = 48; i < 56; i++) {
        setRGB0(&D_801F5260[i]->quads[FRAME_BUFFER_INDEX], D_801F4C58, D_801F4C58, D_801F4C58);
        D_801F5260[i]->pos.vx = D_801F4E48.zoom * 38 / 35;
        D_801F5260[i]->pos.vy = D_801F4E48.zoom / 35;
        D_801F5260[i]->pos.vz = rcos(D_801F4E48.unk3E0) * 2 / 3;
        D_801F5260[i]->rot.vz = (D_801F4E48.zoom << 12) / 35;
    }
    setRGB0(&D_801F5260[2]->quads[FRAME_BUFFER_INDEX], 0, 0, 0);
    D_801F5260[2]->pos.vx = D_801F4E48.zoom * 38 / 35;
    D_801F5260[2]->pos.vy = D_801F4E48.zoom / 35;
    D_801F5260[2]->pos.vz = rcos(D_801F4E48.unk3E0) * 2 / 3;
    D_801F5260[2]->rot.vz = (D_801F4E48.zoom << 12) / 35;
    D_801F4C58 -= 8;
    if (D_801F4C58 < 0x40) {
        D_801F4C58 = 0x40;
    }
    if (D_801F4E48.unk3E0 == 0 && D_801F4E48.zoom == 0) {
        D_801F4E48.state = 8;
    }
}

void func_801EF1A8(void) {
    D_801F524F = 0;
}

void func_801EF1B4(void) {
    D_801F4E48.shown = -1;
    D_801F4E48.selected = -1;
    D_801F4E48.timer = 0;
    D_801F5260[4] = func_801EB754(6);
    D_801F5260[4]->pos.vx = -0x5F;
    D_801F5260[4]->pos.vy = -0x98;
    func_801EBD8C(D_801F5260[4], 0x18);
    D_801F5260[3] = func_801EB754(7);
    D_801F5260[3]->pos.vx = -0x5F;
    D_801F5260[3]->pos.vy = -0x97;
    func_801EBD8C(D_801F5260[3], 0x19);
    D_801F5260[3]->quads[0].clut = D_801F5260[3]->quads[1].clut = getClut(0x200, 0xEA);
}

void func_801EF25C(void) {
    Rect16 rect;

    rect.x = 0x200;
    rect.y = D_801F4E48.selected * 24 + 0x30;
    rect.w = 0x74;
    rect.h = 0x18;
    if (D_801F4E48.selected != D_801F4E48.shown || D_801F4E48.selected == -1) {
        D_801F4E48.timer--;
        if (D_801F4E48.timer < 0) {
            D_801F4E48.timer = 0;
            D_801F4E48.shown = D_801F4E48.selected;
            if (D_801F4E48.shown != -1) {
                func_801EE5A8(D_801F5260[3], &rect);
            }
        }
    } else {
        D_801F4E48.timer++;
        if (D_801F4E48.timer > 20) {
            D_801F4E48.timer = 20;
        }
    }
    D_801F5260[4]->pos.vy = (D_801F4E48.timer * -97 + (20 - D_801F4E48.timer) * -152) / 20;
    D_801F5260[3]->pos.vy = D_801F5260[4]->pos.vy + 2;
    func_801EBA54(D_801F5260[3]);
    func_801EBA54(D_801F5260[4]);
}

void func_801EF3F4(void) {
    s32 i;

    if (D_801F5246 == 0) {
        D_801F5260[35] = func_801EB754(0x1A);
        D_801F5260[36] = func_801EB754(0x1B);
        D_801F5260[37] = func_801EB754(0x1C);
        for (i = 35; i < 38; i++) {
            func_801EBD8C(D_801F5260[i], 0x1B);
        }
        D_801F5260[35]->pos.vy = 0x3B;
        D_801F5260[36]->pos.vy = D_801F5260[37]->pos.vy = 0x32;
        D_801F5246 = 1;
    }
    D_801F4E48.unk3F2 = 0;
    D_801F4E48.blink = 0;
}

void func_801EF4CC(void) {
    Rect16 rect;

    D_801F5260[35]->pos.vx = (D_801F4E48.unk3F2 * -108 + (20 - D_801F4E48.unk3F2) * -200) / 20;
    D_801F5260[36]->pos.vx = D_801F5260[37]->pos.vx = D_801F5260[35]->pos.vx - 7;
    D_801F4E48.blink = (D_801F4E48.blink + 1) & 0xFFF;
    if (D_801F4E48.unk3FE == 2) {
        D_801F4E48.unk3F2 += 2;
        if (D_801F4E48.unk3F2 > 20) {
            D_801F4E48.unk3F2 = 20;
            D_801F4E48.unk3FE = 3;
        }
    } else if (D_801F4E48.unk3FE == 4) {
        D_801F4E48.unk3F2 -= 2;
        if (D_801F4E48.unk3F2 < 0) {
            D_801F4E48.unk3F2 = 0;
            D_801F4E48.unk3FE = 1;
        }
    }
    func_801EBA54(D_801F5260[35]);
    if (D_801F4E48.nodeIndex >= 12 || D_801F4E48.moving != 0 || D_801F4E48.unk3F2 != 20) {
        if (D_801F4E48.blink & 4) {
            rect.x = 0x318;
        } else {
            rect.x = 0x328;
        }
        rect.y = 0xC8;
        rect.w = 0x40;
        rect.h = 0x37;
        func_801EE5A8(D_801F5260[36], &rect);
        func_801EBA54(D_801F5260[36]);
    } else {
        rect.x = D_801F524E % 4 * 32 + 0x180;
        rect.y = D_801F524E / 4 * 56;
        rect.w = 0x40;
        rect.h = 0x37;
        func_801EE40C(D_801F5260[37], &rect, 0);
        func_801EBA54(D_801F5260[37]);
    }
}

void func_801EF750(void) {
    D_801F5245 = 0;
    D_801F5260[38] = func_801EB754(0x1F);
    D_801F5260[39] = func_801EB754(0x1E);
    D_801F5260[38]->pos.vx = 0x26;
    D_801F5260[39]->pos.vx = 0x25;
    func_801EBD8C(D_801F5260[38], 0x19);
    func_801EBD8C(D_801F5260[39], 0x19);
}

void func_801EF7C4(void) {
    Rect16 rect;
    s32 i;

    D_801F5260[39]->pos.vy = (D_801F4E48.slide * 89 + (20 - D_801F4E48.slide) * 134) / 20;
    D_801F5260[38]->pos.vy = D_801F5260[39]->pos.vy + 1;
    if (D_801F4E48.slideDir == 1) {
        D_801F4E48.slide += 2;
        if (D_801F4E48.slide > 20) {
            D_801F4E48.slide = 20;
            D_801F4E48.slideDir = 0;
        }
    } else if (D_801F4E48.slideDir == 2) {
        D_801F4E48.slide -= 2;
        if (D_801F4E48.slide < 0) {
            D_801F4E48.slide = 0;
            D_801F4E48.slideDir = 0;
        }
    }
    if (D_801F524E >= 12) {
        switch (D_801F524E) {
        case 12:
            rect.x = 0x2C0;
            rect.y = 0x5A;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        case 13:
            rect.x = 0x2C0;
            rect.y = 0x46;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        case 14:
            rect.x = 0x2C0;
            rect.y = 0x6E;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        case 15:
            rect.x = 0x2C0;
            rect.y = 0x5A;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        }
    } else {
        rect.x = D_801F524E / 6 * 24 + 0x340;
        rect.y = D_801F524E % 6 * 20;
        rect.w = 0x60;
        rect.h = 0x14;
    }
    func_801EE5A8(D_801F5260[38], &rect);
    for (i = 38; i < 40; i++) {
        func_801EBA54(D_801F5260[i]);
    }
}

void func_801EFA34(void) {
    s16 xs[4] = { -0x80, -0x58, 0x58, 0x7C };
    s16 ys[4] = { -0x78, -0x48, 0x48, 0x78 };
    s32 i;
    s32 col;
    s32 row;

    for (i = 40; i < 48; i++) {
        D_801F5260[i] = func_801EB754(i - 8);
        func_801EBD8C(D_801F5260[i], 0x1A);
    }
    D_801F5260[40]->corners[0].vx = D_801F5260[40]->corners[2].vx = xs[1];
    D_801F5260[40]->corners[1].vx = D_801F5260[40]->corners[3].vx = xs[2];
    D_801F5260[40]->corners[0].vy = D_801F5260[40]->corners[1].vy = ys[0];
    D_801F5260[40]->corners[2].vy = D_801F5260[40]->corners[3].vy = ys[1];
    D_801F5260[41]->corners[0].vx = D_801F5260[41]->corners[2].vx = xs[1];
    D_801F5260[41]->corners[1].vx = D_801F5260[41]->corners[3].vx = xs[2];
    D_801F5260[41]->corners[0].vy = D_801F5260[41]->corners[1].vy = ys[2];
    D_801F5260[41]->corners[2].vy = D_801F5260[41]->corners[3].vy = ys[3];
    for (i = 0; i < 6; i++) {
        col = i / 3;
        row = i % 3;
        D_801F5260[i + 42]->corners[0].vx = D_801F5260[i + 42]->corners[2].vx = xs[col * 2];
        D_801F5260[i + 42]->corners[1].vx = D_801F5260[i + 42]->corners[3].vx = xs[col * 2 + 1];
        D_801F5260[i + 42]->corners[0].vy = D_801F5260[i + 42]->corners[1].vy = ys[row];
        D_801F5260[i + 42]->corners[2].vy = D_801F5260[i + 42]->corners[3].vy = ys[row + 1];
    }
}

void func_801EFC54(void) {
    s16 xs[4] = { -0x8B, -0x53, 0x5D, 0x89 };
    s16 ys[4] = { -0x75, -0x45, 0x43, 0x77 };
    s32 i;
    s32 col;
    s32 row;
    s32 id;

    for (i = 48, id = 40; i < 56; i++, id++) {
        D_801F5260[i] = func_801EB754(id);
        func_801EBD8C(D_801F5260[i], 0x1C);
    }
    D_801F5260[48]->corners[0].vx = D_801F5260[48]->corners[2].vx = xs[1];
    D_801F5260[48]->corners[1].vx = D_801F5260[48]->corners[3].vx = xs[2];
    D_801F5260[48]->corners[0].vy = D_801F5260[48]->corners[1].vy = ys[0];
    D_801F5260[48]->corners[2].vy = D_801F5260[48]->corners[3].vy = ys[1];
    D_801F5260[49]->corners[0].vx = D_801F5260[49]->corners[2].vx = xs[1];
    D_801F5260[49]->corners[1].vx = D_801F5260[49]->corners[3].vx = xs[2];
    D_801F5260[49]->corners[0].vy = D_801F5260[49]->corners[1].vy = ys[2];
    D_801F5260[49]->corners[2].vy = D_801F5260[49]->corners[3].vy = ys[3];
    for (i = 0; i < 6; i++) {
        col = i / 3;
        row = i % 3;
        D_801F5260[i + 50]->corners[0].vx = D_801F5260[i + 50]->corners[2].vx = xs[col * 2];
        D_801F5260[i + 50]->corners[1].vx = D_801F5260[i + 50]->corners[3].vx = xs[col * 2 + 1];
        D_801F5260[i + 50]->corners[0].vy = D_801F5260[i + 50]->corners[1].vy = ys[row];
        D_801F5260[i + 50]->corners[2].vy = D_801F5260[i + 50]->corners[3].vy = ys[row + 1];
    }
}

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

void func_801EFF24(void) {
    u16 ids[12] = { 0x11A, 0x11B, 0x11C, 0x11D, 0x11E, 0x11F, 0x120, 0x121, 0x122, 0x123, 0x124, 0x125 };
    s32 i;

    for (i = 0; i < 12; i++) {
        D_801F3C30[i].unlocked = func_801EC108(ids[i]);
    }
    for (i = 12; i < 16; i++) {
        D_801F3C30[i].unlocked = 0;
    }
    if (func_801EC108(ids[5])) {
        D_801F3C30[12].unlocked = 1;
        D_801F3C30[13].unlocked = 1;
    }
    if (func_801EC108(ids[9])) {
        D_801F3C30[14].unlocked = 1;
        D_801F3C30[15].unlocked = 1;
    }
}

void func_801F0078(void) {
    func_801EE40C(D_801F5260[2], &D_801F3CE8[D_801F4E48.area], 0);
    D_801F5260[2]->quads[0].clut = D_801F5260[2]->quads[1].clut = getClut(0x180, D_801F4E48.area + 0x1A8);
}

void func_801F00F4(s32 resume, s32 arg1) {
    s32 i;
    s32 j;
    s8 node;

    if (resume == 0) {
        func_800149B8(0, -1, 0, 0x400, func_801EBFB4, 0, getCurrentTaskId, 0, 0);
        do {
            func_80014C08(1);
        } while (SESSION->loading != 0);
    }
    loadSoundEffectBank(1);
    func_801EC080();
    func_801EBE94();
    func_801EFF24();
    func_801F0D20(0);
    if (D_801F5250 != 1) {
        func_800149B8(0, -1, 0, 0x400, func_801EC1D8, 0, getCurrentTaskId(), 0, 0);
    }
    D_801F5350 = allocTaskHeapBlock(sizeof(Unk801F5350) * 6);
    func_801ECCA4(D_801F4E48.area);
    func_801EFA34();
    func_801EFC54();
    SESSION_SUB->unk1A4 = D_801F4E48.nodeIndex = ((PlayerProfile *)PLAYER_PROFILES)->unkE;
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 7; i++) {
            if (((PlayerProfile *)PLAYER_PROFILES)->unkE == D_801F3CD0[j][i]) {
                D_801F4E48.area = j;
            }
        }
    }
    D_801F4C58 = 0x40;
    D_801F4E48.unk407 = 1;
    D_801F4E48.unk3D4 = 0;
    D_801F4E48.zoom = 0;
    D_801F4E48.state = 0;
    D_801F4E48.unk3FE = 0;
    D_801F4E48.alpha = 0xFF;
    D_801F4E48.unk401 = 0;
    D_801F4E48.unk3FB = 0;
    D_801F4E48.unk409 = 0;
    D_801F4E48.lastArea = D_801F4E48.area;
    D_801F4E48.unk40B = arg1;
    func_801EF1B4();
    func_801EF3F4();
    func_801EF750();
    D_801F5260[2] = func_801EB754(2);
    func_801F0078();
    for (i = 0; i < 2; i++) {
        setRGB0(&D_801F5260[2]->quads[i], 0, 0, 0);
    }
    func_801EBD8C(D_801F5260[2], 0x23);
    for (i = 0; i < 2; i++) {
        func_80067784(&D_801F4E48.fades[i]);
        SetSemiTrans(&D_801F4E48.fades[i], 1);
        SetDrawTPage(&D_801F4E48.tpages[i], 0, 0, 0x40);
        D_801F4E48.fades[i].r0 = D_801F4E48.fades[i].g0 = D_801F4E48.fades[i].b0 = 0xFF;
        D_801F4E48.fades[i].x0 = D_801F4E48.fades[i].x2 = 100;
        D_801F4E48.fades[i].x1 = D_801F4E48.fades[i].x3 = 0x128;
        D_801F4E48.fades[i].y0 = D_801F4E48.fades[i].y1 = 0x28;
        D_801F4E48.fades[i].y2 = D_801F4E48.fades[i].y3 = 0xC8;
    }
    do {
        func_80014C08(1);
        if (D_801F3D0C[D_801F4E48.state] != NULL) {
            D_801F3D0C[D_801F4E48.state]();
        }
        for (i = 0; i < D_801F4E48.unk3FB; i++) {
            node = D_801F3CD0[D_801F4E48.lastArea][i];
            if (D_801F3C30[node].unlocked == 1) {
                func_801EBA54(D_801F5260[i + 21]);
            }
        }
        if (D_801F4E48.unk3FB != 0) {
            func_801EBA54(D_801F5260[0]);
            D_801F5260[1]->pos.vx = D_801F5260[0]->pos.vx;
            D_801F5260[1]->pos.vy = D_801F5260[0]->pos.vy + 15;
            func_801EBA54(D_801F5260[1]);
        }
        func_801F0D84();
        func_801EFE84();
        func_801EFED4();
        if (D_801F4E48.unk409 != 0) {
            func_801ECF94(D_801F4E48.unk40A);
        }
        func_801EBA54(D_801F5260[2]);
        func_801EDB90();
        func_801EF7C4();
        func_801EF25C();
        if (D_801F4E48.unk3FE >= 2) {
            func_801EF4CC();
        }
    } while (D_801F4E48.state != 8);
    func_80014C08(1);
    freeHeapBlocksByTag(0x2E);
    D_801F4E48.unk407 = -1;
    func_80014A00(0x19);
    freeHeapBlocksByTag(0x28);
    freeHeapBlocksByTag(0x7F);
    freeHeapBlocksByTag(0x29);
    func_80014C08(0x1E);
    SESSION_SUB->unk1A2 = 0;
    SESSION_SUB->unk1A5[0] = 0;
    ((PlayerProfile *)PLAYER_PROFILES)->unkE = SESSION_SUB->unk1A4 = (u8)D_801F4E48.nodeIndex;
    if (D_801F4E48.unk401 == 1) {
        D_801F4E48.running = 0;
        switch (D_801F4E48.unk3FF) {
        case 0:
            func_800149B8(0, -1, 0, 0x1600, func_801F0E8C, 0, getCurrentTaskId(), 0, 0);
            break;
        case 1:
            func_800149B8(0, -1, 0, 0x1600, func_801F0EAC, 0, getCurrentTaskId(), 0, 0);
            break;
        case 2:
            ((PlayerProfile *)PLAYER_PROFILES)->unkF = 0;
            func_800149B8(0, -1, 0, 0x400, openSaveScreenFromMap, 2, getCurrentTaskId(), 0, 0);
            break;
        }
    } else {
        do {
            func_80014C08(1);
        } while (SESSION->loading != 0);
        func_80014C08(1);
        func_800149B8(0, -1, 0, 0x1600, func_801E4D80, 1, getCurrentTaskId(), 0, 0);
    }
    func_80014A90();
}

void func_801F082C(void) {
    MapPath *path;
    MapNode *node;
    s32 dx;
    s32 dy;
    s32 i;
    s32 j;
    s8 next;
    s16 angle;

    func_801ECCA4(D_801F4E48.area);
    path = D_801F4E48.paths;
    D_801F4E48.unk409 = 1;
    D_801F4E48.pathCount = 0;
    for (i = 0; i < 7; i++) {
        if (D_801F4E48.route[i] == -1) {
            continue;
        }
        node = &D_801F3C30[D_801F4E48.route[i]];
        if (node->unlocked != 1) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            next = node->next[j];
            if (i < next && next >= 0 && D_801F3C30[next].unlocked != 0) {
                dx = D_801F3C30[next].x - node->x;
                dy = D_801F3C30[next].y - node->y;
                angle = ratan2(dy, dx);
                path->rot.vz = -angle;
                path->pos.vx = node->x;
                path->pos.vy = node->y;
                path->length = csqrt(dx * dx + dy * dy);
                path->length /= 64;
                D_801F4E48.pathCount++;
                path++;
            }
        }
    }
    addFrameCallback((s32)func_801F0A08);
}

void func_801F0A08(FrameBuffer *fb) {
    MATRIX matrix;
    SVECTOR corners[4];
    s32 sxy[4];
    s32 depthCue;
    s32 flag;
    s8 i;

    for (i = 0; i < D_801F4E48.pathCount; i++) {
        buildRotTransMatrix(&D_801F4E48.paths[i].pos, &D_801F4E48.paths[i].rot, &matrix);
        CompMatrix((MATRIX *)SCENE_3D->unk78, &matrix, &matrix);
        SetRotMatrix((s32)&matrix);
        func_8005C444(&matrix);
        corners[0].vx = 0;
        corners[0].vy = 2;
        corners[0].vz = 0;
        corners[1].vx = D_801F4E48.paths[i].length;
        corners[1].vy = 2;
        corners[1].vz = 0;
        corners[2].vx = 0;
        corners[2].vy = -2;
        corners[2].vz = 0;
        corners[3].vx = D_801F4E48.paths[i].length;
        corners[3].vy = -2;
        corners[3].vz = 0;
        RotAverage4(&corners[0], &corners[1], &corners[2], &corners[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
        ((PolyF4 *)fb->primSlots[1])[i].x0 = sxy[0];
        ((PolyF4 *)fb->primSlots[1])[i].y0 = sxy[0] >> 16;
        ((PolyF4 *)fb->primSlots[1])[i].x1 = sxy[1];
        ((PolyF4 *)fb->primSlots[1])[i].y1 = sxy[1] >> 16;
        ((PolyF4 *)fb->primSlots[1])[i].x2 = sxy[2];
        ((PolyF4 *)fb->primSlots[1])[i].y2 = sxy[2] >> 16;
        ((PolyF4 *)fb->primSlots[1])[i].x3 = sxy[3];
        ((PolyF4 *)fb->primSlots[1])[i].y3 = sxy[3] >> 16;
        addPrim(&fb->ot[34], &((PolyF4 *)fb->primSlots[1])[i]);
        addPrim(&fb->ot[34], &D_801F4E48.pathTpages[FRAME_BUFFER_INDEX][i]);
    }
}

void func_801F0D20(s8 arg0) {
    if (arg0 == 0) {
        D_801F525C = 0;
    }
    D_801F5258.offset = 0;
    D_801F5260[5] = func_801EB754(0x41);
    D_801F5260[5]->pos.vx = -0xD5;
    D_801F5260[5]->pos.vy = -0x24;
    func_801EBD8C(D_801F5260[5], 0x1A);
}

void func_801F0D84(void) {
    char buf[0x48];
    s32 x;
    s32 state;

    state = D_801F5258.state;
    if (state == 1) {
        D_801F5258.offset++;
        if (D_801F5258.offset > 20) {
            D_801F5258.offset = 20;
        }
    } else if (state > 0) {
        if (state < 4) {
            D_801F5258.offset--;
            if (D_801F5258.offset < 0) {
                D_801F5258.offset = 0;
            }
        }
    }
    x = ((20 - D_801F5258.offset) * -213 - D_801F5258.offset * 116) / 20;
    D_801F5260[5]->pos.vx = x;
    func_801EBA54(D_801F5260[5]);
}

void func_801F0E84(void) {
}

void func_801F0E8C(void) {
    openDeckEditor(0);
}

void func_801F0EAC(void) {
    openPartnerEquipment(0);
}

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

void func_801F0F5C(s32 x, s32 y, s32 vramX, s32 vramY, s32 frame, u16 clut, u8 *rgb) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x + 2;
        CUR_SPRT->sp.y0 = y + 5;
        CUR_SPRT->sp.u0 = vramX % 64 * 2 + 2;
        CUR_SPRT->sp.v0 = vramY % 256 + 2;
        CUR_SPRT->sp.clut = clut;
        CUR_SPRT->sp.w = 36;
        CUR_SPRT->sp.h = 36;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, vramX, vramY));
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (frame > 5) {
            frame = 5;
        }
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0x80;
            CUR_SPRT->sp.v0 = 0x30;
            CUR_SPRT->sp.clut = getClut(0x210, frame + 0xE2);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, 8);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

void func_801F1294(CardWindow *win) {
    char buf[0x48];
    u8 rgb[4];
    s32 x;
    s32 y;
    s32 z;
    s32 specialty;
    s32 i;
    DigimonCardData *card;
    u8 *data;

    if (D_801F5358->showRewards != 0) {
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[win->index] < 0) {
            rgb[0] = 0x40;
            rgb[1] = 0x40;
            rgb[2] = 0x40;
            win->window.brightness = 0x40;
        } else {
            rgb[0] = 0x80;
            rgb[1] = 0x80;
            rgb[2] = 0x80;
            win->window.brightness = 0x80;
        }
    } else {
        rgb[0] = 0x80;
        rgb[1] = 0x80;
        rgb[2] = 0x80;
        win->window.brightness = 0x80;
    }
    x = win->window.originX;
    y = win->window.originY;
    z = win->window.z;
    if (win->cardId >= 0) {
        specialty = getCardSpecialty(win->cardId);
        if (((PlayerProfile *)PLAYER_PROFILES)->cardCollection[win->cardId] & 0x20) {
            drawIcon(x + 0xF, y + 0x2A, 2, 9, 0);
        }
        func_801F0F5C(x + 1, y + 0xC, win->index * 20 + 0x200, 0x78, specialty, win->clut, rgb);
        drawTextColored(x + 3, y, "No.", rgb, 7, z);
        sprintf(buf, "*s0%3d", win->cardId);
        drawTextColored(x + 0x17, y, buf, rgb, 7, z);
        if (specialty < 5) {
            card = &((DigimonCardData *)DIGIMON_CARDS)[win->cardId];
            drawTextColored(x + 0x32, y, card->name, rgb, 7, z);
            drawIconColored(x + 0x2C, y + 0xE, 0, 0x1A, rgb, z);
            sprintf(buf, "*s0%4d", card->hp);
            drawTextColored(x + 0x3C, y + 0xD, buf, rgb, 7, z);
            drawIconColored(x + 0x2C, y + 0x1A, 0, 7, rgb, z);
            sprintf(buf, "*s0%4d", card->attack[0].power);
            drawTextColored(x + 0x3C, y + 0x19, buf, rgb, 7, z);
            drawIconColored(x + 0x2C, y + 0x26, 0, 8, rgb, z);
            sprintf(buf, "*s0%4d", card->attack[1].power);
            drawTextColored(x + 0x3C, y + 0x25, buf, rgb, 7, z);
            drawIconColored(x + 0x2C, y + 0x32, 0, 9, rgb, z);
            sprintf(buf, "*s0%4d", card->attack[2].power);
            drawTextColored(x + 0x3C, y + 0x31, buf, rgb, 7, z);
            sprintf(buf, "(%s)", CROSS_EFFECT_SHORT_NAMES[card->crossEffect]);
            drawSmallTextColored(x + 0x5A, y + 0x33, buf, 7, rgb, z);
            drawIconColored(x + 0x74, y + 0x1A, 0, 0x18, rgb, z);
            sprintf(buf, "*s0%2d", card->dpCost);
            drawTextColored(x + 0x86, y + 0x19, buf, rgb, 7, z);
            drawIconColored(x + 0x74, y + 0x26, 0, 0x19, rgb, z);
            sprintf(buf, "*s0%2d", card->dpBonus);
            drawTextColored(x + 0x86, y + 0x25, buf, rgb, 7, z);
            drawTextColored(x + 0xB4, y, "Support Effect", rgb, 6, z);
            for (i = 0; i < 4; i++) {

            drawTextColored(x + 0xB4, y + 0xD + i * 12, card->supportText[i], rgb, 7, z);
            }
        } else if (specialty == 5) {
            data = (u8 *)&((OptionCardData *)OPTION_CARDS)[win->cardId - 0xBF];
            drawTextColored(x + 0x32, y, data + 3, rgb, 7, z);
            drawTextColored(x + 0xB4, y, "Option Description", rgb, 6, z);
            for (i = 0; i < 4; i++) {

            drawTextColored(x + 0xB4, y + 0xD + i * 12, data + (i * 0x15 + 0x8D), rgb, 7, z);
            }
        } else {
            data = (u8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[win->cardId - 0x125];
            drawTextColored(x + 0x32, y, data + 3, rgb, 7, z);
            drawTextColored(x + 0xB4, y, "Option Description", rgb, 6, z);
            for (i = 0; i < 4; i++) {

            drawTextColored(x + 0xB4, y + 0xD + i * 12, data + (i * 0x15 + 0x1B), rgb, 7, z);
            }
        }
    } else {
        drawTextColored(x + 0x74, y + 0x1A, "NO DATA", rgb, 6, z);
    }
}

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

void func_801F1A80(void) {
    s32 i;

    drawWindow(&D_801F5358->main, func_801F19C4, 0);
    for (i = 0; i < 3; i++) {
        if (D_801F5358->showRewards != 0 && ((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] != 100) {
            drawWindow(&D_801F5358->rewards[i].window, func_801F1944, 0);
        }
        drawWindow(&D_801F5358->cards[i].window, func_801F1294, 0);
    }
}

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

void func_801F1C84(s32 fromScript) {
    /* the ROM keeps two stray bytes after the terminator */
    static const char path[16] = "B:\\M_CARD.ARC\0\xBB\xBB";
    Rect16 rect;
    u8 *tims;
    s32 i;
    s16 card;

    D_801F5358 = allocPermanentHeapBlock(sizeof(RewardScreen));
    D_801F5358->showRewards = 0;
    if (fromScript == 0) {
        rollRewardCards(0, D_801F46A5);
        addRewardCardsToCollection(0);
    } else {
        func_801F1B5C();
    }
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tims = (u8 *)func_80014C08(0x7FFFFFFF);
    playSoundEffect(3);
    rect.x = 0x10;
    rect.y = 0x10;
    rect.w = 0x120;
    rect.h = 0xE;
    openWindow(&D_801F5358->main, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    for (i = 0; i < 3; i++) {
        card = ((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i];
        if (card >= 0) {
            uploadTim((u32 *)(tims + ((s32 *)tims)[card]), i * 20 + 0x200, 0x78, 0x200, i + 0xFB);
        }
        D_801F5358->cards[i].index = i;
        D_801F5358->cards[i].cardId = card;
        D_801F5358->cards[i].clut = getClut(0x200, i + 0xFB);
        rect.x = 0x10;
        rect.y = i * 65 + 0x26;
        rect.w = 0x120;
        rect.h = 0x3C;
        openWindow(&D_801F5358->cards[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    }
    DrawSync(0);
    func_80014C08(FRAME_INTERVAL);
    freeHeapBlock(tims);
    addFrameCallback((s32)func_801F1A80);
    func_80014C08(20);
    func_801F0F08();
    playSoundEffect(0);
    D_801F5358->showRewards = 1;
    for (i = 0; i < 3; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i] >= 0) {
            rect.x = 0xC8;
            rect.y = i * 65 + 0x40;
            rect.w = 0x48;
            rect.h = 9;
            if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] < 0) {
                rect.x = 0xC4;
                rect.w = 0x50;
            }
            openWindow(&D_801F5358->rewards[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
            D_801F5358->rewards[i].slot = i;
            D_801F5358->rewards[i].window.palette = 2;
        }
    }
    func_801F0F08();
    playSoundEffect(4);
    animateWindowTo(&D_801F5358->main, (Rect16 *)-1);
    for (i = 0; i < 3; i++) {
        animateWindowTo(&D_801F5358->cards[i].window, (Rect16 *)-1);
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] != 100) {
            animateWindowTo(&D_801F5358->rewards[i].window, (Rect16 *)-1);
        }
    }
    func_80014C08(20);
    freeHeapBlock(D_801F5358);
    func_80014C08(10);
    removeFrameCallback((s32)func_801F1A80);
}

void func_801F208C(s32 x, s32 y, s32 vramX, s32 vramY, s32 frame, u8 *rgb, s32 z, s32 palette) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x + 2;
        CUR_SPRT->sp.y0 = y + 5;
        CUR_SPRT->sp.u0 = vramX % 64 * 2 + 2;
        CUR_SPRT->sp.v0 = vramY % 256 + 2;
        CUR_SPRT->sp.clut = getClut(0x280, palette + 0x1EE);
        CUR_SPRT->sp.w = 36;
        CUR_SPRT->sp.h = 36;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, vramX, vramY));
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0xA8;
            CUR_SPRT->sp.clut = getClut(0x290, frame + 0x1F2);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

const u8 D_801DEA14[6] = { 0xAF, 0xB6, 0xBE, 0xB8, 0xB7, 0xBB };

void func_801F23D8(UiWindow *win) {
    /* not literals: GCC would share func_801F1294's identical strings */
    static const char valueFormat[] = "%4d";
    static const char effectFormat[] = "(%s)";
    static const char supportEffect[] = "Support Effect";
    Rect16 rect;
    char buf[0x10];
    u8 rgb[4];
    s32 i;
    s32 cardId;
    s32 y;
    s32 z;
    s32 x;
    s32 j;
    s32 tx;

    z = win->z;
    y = win->originY + 1;
    for (i = 0; i < D_801F5408.count; i++) {
        cardId = D_801F3D34[i];
        if (D_801F5400.cursor == i) {
            rgb[0] = 0x80;
            rgb[1] = 0x80;
            rgb[2] = 0x80;
        } else {
            rgb[0] = 0x40;
            rgb[1] = 0x40;
            rgb[2] = 0x40;
        }
        x = win->originX;
        tx = x + 4;
        func_801F208C(tx, y + 4, i * 20 + 0x200, 0x180, getCardSpecialty(cardId), rgb, z, i);
        tx = x + 0x30;
        drawTextColored(tx, y + 1, ((DigimonCardData *)DIGIMON_CARDS)[cardId].name, rgb, 7, z);
        drawIconColored(tx, y + 0xD, 0, 7, rgb, z);
        sprintf(buf, valueFormat, ((DigimonCardData *)DIGIMON_CARDS)[cardId].attack[0].power);
        drawTextColored(x + 0x3E, y + 0xD, buf, rgb, 7, z);
        drawIconColored(tx, y + 0x19, 0, 8, rgb, z);
        sprintf(buf, valueFormat, ((DigimonCardData *)DIGIMON_CARDS)[cardId].attack[1].power);
        drawTextColored(x + 0x3E, y + 0x19, buf, rgb, 7, z);
        drawIconColored(tx, y + 0x25, 0, 9, rgb, z);
        sprintf(buf, valueFormat, ((DigimonCardData *)DIGIMON_CARDS)[cardId].attack[2].power);
        drawTextColored(x + 0x3E, y + 0x25, buf, rgb, 7, z);
        sprintf(buf, effectFormat, CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)DIGIMON_CARDS)[cardId].crossEffect]);
        drawLargeTextColored(tx, y + 0x31, buf, 7, rgb, z);
        tx = x + 0x5C;
        drawIconColored(tx, y + 0x19, 0, 0x19, rgb, z);
        sprintf(buf, valueFormat, ((DigimonCardData *)DIGIMON_CARDS)[cardId].dpBonus);
        drawTextColored(x + 0x68, y + 0x19, buf, rgb, 7, z);
        drawIconColored(tx, y + 0x25, 0, 0x1A, rgb, z);
        sprintf(buf, valueFormat, ((DigimonCardData *)DIGIMON_CARDS)[cardId].hp);
        drawTextColored(x + 0x6A, y + 0x25, buf, rgb, 7, z);
        tx = x + 0x88;
        drawTextColored(x + 0x98, y, supportEffect, rgb, 6, z);
        for (j = 0; j < 4; j++) {
            drawTextColored(tx, y + 0xF + j * 12, ((DigimonCardData *)DIGIMON_CARDS)[cardId].supportText[j], rgb, 7, z);
        }
        rect.x = tx;
        rect.y = y + 0xF;
        rect.w = 0x6E;
        rect.h = 0x30;
        drawWindowFrame(&rect, 0x31, 0, 0x80, 1, z);
        y += 0x48;
    }
    if (D_801F5408.state != 5) {
        if (PAD_STATES[0]->repeat & 0x1000) {
            if (D_801F5400.cursor != 0) {
                playSoundEffect(2);
                D_801F5400.cursor--;
                scrollWindowTo(&win->originX, 0, D_801F5400.cursor * 72);
            }
        } else if (PAD_STATES[0]->repeat & 0x4000) {
            if (D_801F5400.cursor < D_801F5408.count - 1) {
                playSoundEffect(2);
                D_801F5400.cursor++;
                scrollWindowTo(&win->originX, 0, D_801F5400.cursor * 72);
            }
        } else if (PAD_STATES[0]->pressed & 0x40) {
            playSoundEffect(0);
            D_801F5400.done = 1;
        }
        D_801F5408.state = D_801F5400.cursor + 6;
        rect.x = win->rect.x - win->scroll[2] + 1;
        rect.y = win->rect.y - win->scroll[3] + D_801F5400.cursor * 72;
        rect.w = 0x104;
        rect.h = 0x48;
        moveCursorHighlight(&D_801F53B0, &rect);
        drawCursorHighlight(&D_801F53B0, z);
    }
}

void func_801F2A8C(void) {
    drawWindow(&D_801F5360, func_801F23D8, 1);
}

void func_801F2ABC(s32 task) {
    Rect16 cursorRect;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];
    u8 *file;
    s32 i;

    D_801F5408.count = 0;
    for (i = 0; i < 4; i++) {
        if (D_801F4698[i] != -1) {
            D_801F3D34[i] = D_801DEA14[D_801F4698[i]];
            D_801F5408.count++;
        }
    }
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\CARD_F.TIM", getCurrentTaskId());
    file = (u8 *)func_80014C08(0x7FFFFFFF);
    uploadTim((u32 *)file, 0x200, 0x1A8, 0x290, 0x1F2);
    DrawSync(0);
    func_80014C08(FRAME_INTERVAL);
    freeHeapBlock(file);
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\P_CARD.ARC", getCurrentTaskId());
    file = (u8 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < D_801F5408.count; i++) {
        uploadTim((u32 *)(file + ((s32 *)file)[getPartnerIndex(D_801F3D34[i])]), i * 20 + 0x200, 0x180, 0x280, i + 0x1EE);
    }
    D_801F5400.done = 0;
    D_801F5400.cursor = 0;
    rect.x = 0x1E;
    rect.y = 0x44;
    rect.w = 0x104;
    rect.h = 0x5A;
    view.x = 0;
    view.y = 0;
    view.w = 0x10E;
    view.h = D_801F5408.count * 72;
    openWindow(&D_801F5360, &rect, -1, (s16 *)&view, 10, 0x86, 0x80, 12);
    D_801F5360.label = (s32)"PARTNER GET";
    D_801F5360.labelPalette = 7;
    cursorRect.x = D_801F5360.originX + 4;
    cursorRect.y = D_801F5360.originY + 1;
    cursorRect.w = 12;
    cursorRect.h = 12;
    initCursorHighlight(&D_801F53B0, &cursorRect, (Bytes4 *)-1);
    func_80014C08(FRAME_INTERVAL);
    playSoundEffect(3);
    addFrameCallback((s32)func_801F2A8C);
    do {
    wait:
        func_80014C08(FRAME_INTERVAL);
        if (D_801F5408.state == 5 && (PAD_STATES[0]->pressed & 0x40)) {
            D_801F5408.state = 6;
        }
        if (D_801F5400.done == 0) {
            goto wait;
        }
        initDialog(dialog, "Is this Partner OK?", 1);
        runDialog(dialog);
        switch ((s8)dialog[0xA5]) {
        case 1:
            break;
        case 0:
        case 2:
            D_801F5400.done = 0;
            break;
        }
    } while (D_801F5400.done == 0);
    animateWindowTo(&D_801F5360, (Rect16 *)-1);
    playSoundEffect(4);
    freeHeapBlock(file);
    func_80014C08(20);
    removeFrameCallback((s32)func_801F2A8C);
    obtainPartner(0, D_801F4588.partners[D_801F5400.cursor]);
    D_801F4588.unk10E = 0;
    func_801E0988(D_801F4588.partners[D_801F5400.cursor]);
    func_80014A48(task);
}

SpriteTemplate D_801F3708[66] = {
    { 0x180, 0xA8, 0x200, 0xF8, 0, 0x1A, 0x28, 1, 0 },
    { 0x1E8, 0xA8, 0x200, 0xF8, 0, 0x18, 0x10, 1, 0 },
    { 0x180, 0x100, 0x180, 0x1A8, 0, 0xC6, 0xA2, 1, 0 },
    { 0x180, 0x100, 0x280, 0x1F8, 0, 0xFF, 0x80, 1, 0 },
    { 0x180, 0x180, 0x280, 0x1F9, 0, 0xFF, 0x7F, 1, 0 },
    { 0x200, 0x100, 0x280, 0x1FA, 0, 0xFF, 0x80, 1, 0 },
    { 0x2C0, 0xC0, 0x200, 0xF3, 0, 0x80, 0x20, 0, 0 },
    { 0x2E0, 0, 0x200, 0xED, 0, 0x74, 0x18, 0, 0 },
    { 0x2C0, 0, 0x200, 0xF4, 0, 0x50, 0x46, 0, 0 },
    { 0x229, 0, 0x200, 0xE8, 0, 0x10, 0xD, 0, 0 },
    { 0x320, 0x100, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x320, 0x128, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x320, 0x150, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x320, 0x178, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x340, 0x100, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x340, 0x128, 0x280, 0x1F7, 0, 0x27, 0x70, 0, 0 },
    { 0x340, 0x198, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x34A, 0x100, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x34A, 0x128, 0x280, 0x1F7, 0, 0x27, 0x70, 0, 0 },
    { 0x34A, 0x198, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x300, 0xA0, 0x200, 0xEF, 0, 0x58, 0x10, 0, 0 },
    { 0x340, 0x78, 0x200, 0xE3, 0, 0x70, 0x6C, 0, 0 },
    { 0x300, 0xB0, 0x200, 0xE3, 0, 0x54, 0x4C, 0, 0 },
    { 0x318, 0x30, 0x200, 0xF1, 0, 0x44, 0x10, 0, 0 },
    { 0x318, 0x60, 0x200, 0xF1, 0, 0x44, 0x10, 0, 0 },
    { 0x318, 0x70, 0x200, 0xF1, 0, 0x44, 0x10, 0, 0 },
    { 0x360, 0x78, 0x200, 0xE6, 0, 0x6C, 0x7A, 0, 0 },
    { 0x318, 0xC8, 0x200, 0xEB, 0, 0x40, 0x37, 0, 0 },
    { 0x180, 0, 0x200, 0xF9, 0, 0x40, 0x37, 1, 0 },
    { 0x200, 0x30, 0x200, 0xEA, 0, 0x84, 0x30, 0, 0 },
    { 0x2E0, 0xB4, 0x200, 0xE5, 0, 0x68, 0x1C, 0, 0 },
    { 0x340, 0, 0x200, 0xE4, 0, 0x60, 0x14, 0, 0 },
    { 0x240, 0, 0x200, 0xE5, 0, 0xAF, 0x30, 0, 0 },
    { 0x240, 0x30, 0x200, 0xE5, 0, 0xAF, 0x30, 0, 0 },
    { 0x280, 0, 0x200, 0xE5, 0, 0x27, 0x30, 0, 0 },
    { 0x280, 0x30, 0x200, 0xE5, 0, 0x27, 0x90, 0, 0 },
    { 0x280, 0xC0, 0x200, 0xE5, 0, 0x27, 0x30, 0, 0 },
    { 0x28A, 0, 0x200, 0xE5, 0, 0x23, 0x30, 0, 0 },
    { 0x28A, 0x30, 0x200, 0xE5, 0, 0x23, 0x90, 0, 0 },
    { 0x28A, 0xC0, 0x200, 0xE5, 0, 0x23, 0x30, 0, 0 },
    { 0x240, 0x60, 0x200, 0xE2, 0, 0xAF, 0x30, 0, 0 },
    { 0x240, 0x90, 0x200, 0xE2, 0, 0xAF, 0x34, 0, 0 },
    { 0x298, 0, 0x200, 0xE2, 0, 0x37, 0x30, 0, 0 },
    { 0x298, 0x30, 0x200, 0xE2, 0, 0x37, 0x88, 0, 0 },
    { 0x298, 0xB8, 0x200, 0xE2, 0, 0x37, 0x34, 0, 0 },
    { 0x2A6, 0, 0x200, 0xE2, 0, 0x2B, 0x30, 0, 0 },
    { 0x2A6, 0x30, 0x200, 0xE2, 0, 0x2B, 0x88, 0, 0 },
    { 0x2A6, 0xB8, 0x200, 0xE2, 0, 0x2B, 0x34, 0, 0 },
    { 0x300, 0x100, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x300, 0x128, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x300, 0x150, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x300, 0x178, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x358, 0x100, 0x280, 0x1F6, 0, 0x27, 0x28, 0, 0 },
    { 0x358, 0x128, 0x280, 0x1F6, 0, 0x27, 0x78, 0, 0 },
    { 0x358, 0x1A0, 0x280, 0x1F6, 0, 0x27, 0x28, 0, 0 },
    { 0x362, 0x100, 0x280, 0x1F6, 0, 0x23, 0x28, 0, 0 },
    { 0x362, 0x128, 0x280, 0x1F6, 0, 0x23, 0x78, 0, 0 },
    { 0x362, 0x1A0, 0x280, 0x1F6, 0, 0x23, 0x28, 0, 0 },
    { 0x318, 0xA4, 0x200, 0xEB, 0, 0x2A, 0x24, 0, 0 },
    { 0x2C0, 0x82, 0x200, 0xFA, 0, 0x40, 0x38, 1, 0 },
    { 0x218, 0, 0x200, 0xEC, 0, 0x34, 0x30, 0, 0 },
    { 0x318, 0x80, 0x200, 0xF1, 0, 0x40, 0x10, 0, 0 },
    { 0x2E0, 0x80, 0x200, 0xEE, 0, 0x2A, 0x24, 0, 0 },
    { 0x2EB, 0x80, 0x200, 0xEE, 0, 0x2A, 0x24, 0, 0 },
    { 0x22D, 0, 0x200, 0xE8, 0, 0x10, 0xD, 0, 0 },
    { 0x328, 0, 0x210, 0xF2, 0, 0x58, 0x50, 0, 0 },
};

/* the map's nodes: position, the node in each direction, unlocked */
MapNode D_801F3C30[16] = {
    { 73, 26, { -1, 12, 4, 1 }, 1, 0 },
    { 16, -17, { 3, 0, 2, 3 }, 1, 0 },
    { 14, 34, { 1, 1, -1, -1 }, 1, 0 },
    { -27, -50, { -1, 1, 1, -1 }, 1, 0 },
    { 81, 53, { 0, -1, -1, 0 }, 1, 0 },
    { -22, 25, { -1, 7, 13, 13 }, 1, 0 },
    { 61, 67, { 7, 7, -1, -1 }, 1, 0 },
    { 67, 18, { 8, -1, 6, 5 }, 1, 0 },
    { 47, -37, { 14, 7, 7, 14 }, 1, 0 },
    { 70, -36, { -1, 15, -1, 10 }, 1, 0 },
    { -31, -16, { 9, 9, 11, -1 }, 1, 0 },
    { 44, 21, { 10, -1, -1, 10 }, 1, 0 },
    { 107, -11, { -1, -1, 0, 0 }, 1, 0 },
    { -46, 33, { 5, 5, -1, -1 }, 1, 0 },
    { -16, -44, { -1, 8, 8, -1 }, 1, 0 },
    { 120, -18, { 9, -1, -1, 9 }, 1, 0 },
};

/* the nodes of each of the three areas, ended by -1 */
s8 D_801F3CD0[3][7] = {
    { 0, 1, 2, 3, 4, 12, -1 },
    { 5, 6, 7, 8, 13, 14, -1 },
    { 9, 10, 11, 15, -1, -1, -1 },
};

Rect16 D_801F3CE8[3] = {
    { 0x180, 0x100, 0xC6, 0xA2 },
    { 0x200, 0x100, 0xC6, 0xA2 },
    { 0x280, 0x100, 0xC6, 0xA2 },
};

void (*D_801F3D00[3])(void) = {
    NULL,
    func_801EC51C,
    func_801EC140,
};

void (*D_801F3D0C[10])(void) = {
    func_801ED5D4,
    func_801EE13C,
    func_801EE690,
    func_801EDFB8,
    func_801EE9A0,
    func_801EEBD0,
    func_801EE9A0,
    func_801EEC78,
    func_801EF1A8,
    func_801EC844,
};

u8 D_801F3D34[4] = { 0xB7, 0xB8, 0xBB, 0x0 };

/* the partner abilities; the code here only reads their texts */
PartInfo D_801F3D38[128] = {
    { "HP+50.", { 3, 5, 1, 99, 3, 7, 0, 0 } },
    { "HP+100.", { 17, 16, 8, 12, 14, 19, 0, 0 } },
    { "HP+150.", { 29, 32, 19, 25, 32, 33, 0, 0 } },
    { "HP+200.", { 45, 48, 31, 40, 52, 59, 0, 0 } },
    { "HP+300.", { 61, 67, 51, 57, 68, 78, 0, 0 } },
    { "HP+400.", { 96, 89, 71, 81, 91, 88, 0, 0 } },
    { "HP+500.", { 99, 0xFF, 75, 91, 0xFF, 95, 0, 0 } },
    { "All Attack Powers +50.", { 18, 39, 54, 30, 57, 28, 0, 0 } },
    { "All Attack Powers +100.", { 39, 86, 72, 50, 89, 51, 0, 0 } },
    { "All Attack Powers +200.", { 75, 0xFF, 90, 93, 0xFF, 84, 0, 0 } },
    { "*b0 Attack Power +100.", { 1, 9, 15, 5, 4, 2, 0, 0 } },
    { "*b0 Attack Power +150.", { 10, 21, 38, 22, 16, 15, 0, 0 } },
    { "*b0 Attack Power +200.", { 27, 54, 62, 41, 39, 30, 0, 0 } },
    { "*b0 Attack Power +250.", { 48, 0xFF, 78, 61, 62, 61, 0, 0 } },
    { "*b0 Attack Power +300.", { 67, 0xFF, 0xFF, 85, 82, 79, 0, 0 } },
    { "*b1 Attack Power +50.", { 4, 1, 12, 2, 6, 13, 0, 0 } },
    { "*b1 Attack Power +100.", { 12, 7, 22, 16, 18, 29, 0, 0 } },
    { "*b1 Attack Power +150.", { 32, 28, 45, 36, 45, 43, 0, 0 } },
    { "*b1 Attack Power +200.", { 51, 44, 0xFF, 56, 67, 66, 0, 0 } },
    { "*b1 Attack Power +250.", { 77, 0xFF, 0xFF, 76, 86, 96, 0, 0 } },
    { "*b2 Attack Power +50.", { 6, 6, 2, 4, 10, 1, 0, 0 } },
    { "*b2 Attack Power +100.", { 19, 36, 20, 19, 27, 9, 0, 0 } },
    { "*b2 Attack Power +150.", { 35, 69, 42, 38, 53, 24, 0, 0 } },
    { "*b2 Attack Power +200.", { 82, 0xFF, 63, 67, 0xFF, 48, 0, 0 } },
    { "*b0 to 0, *b2 Attack Power -100.", { 24, 12, 4, 14, 0xFF, 34, 0, 0 } },
    { "*b1 to 0, *b2 Attack Power -100.", { 46, 23, 9, 35, 0xFF, 17, 0, 0 } },
    { "*b2 to 0, *b2 Attack Power -100.", { 58, 71, 36, 10, 0xFF, 71, 0, 0 } },
    { "Counter *b0,*b2 Attack Power to 0.", { 11, 26, 0xFF, 0xFF, 40, 62, 0, 0 } },
    { "Counter *b1,*b2 Attack Power to 0.", { 36, 14, 0xFF, 0xFF, 24, 49, 0, 0 } },
    { "Counter *b2,*b2 Attack Power to 0.", { 40, 57, 0xFF, 0xFF, 11, 21, 0, 0 } },
    { "Opponent *a0 X3, *b2 Attack Power -200.", { 87, 92, 39, 69, 31, 50, 0, 0 } },
    { "Opponent *a1 X3, *b2 Attack Power -200.", { 25, 97, 0xFF, 42, 77, 73, 0, 0 } },
    { "Opponent *a2 X3, *b2 Attack Power -200.", { 37, 0xFF, 66, 0xFF, 83, 4, 0, 0 } },
    { "Opponent *a3 X3, *b2 Attack Power -200.", { 68, 33, 46, 21, 5, 97, 0, 0 } },
    { "Opponent *a4 X3, *b2 Attack Power -200.", { 52, 41, 6, 0xFF, 0xFF, 25, 0, 0 } },
    { "1st Attack, *b2 Attack Power -200.", { 76, 50, 95, 62, 0xFF, 0xFF, 0, 0 } },
    { "Jamming Support, *b2 Attack Power -100.", { 0xFF, 74, 27, 0xFF, 54, 10, 0, 0 } },
    { "Eat-up HP, *b2 Attack Power -200.", { 0xFF, 87, 50, 0xFF, 73, 0xFF, 0, 0 } },
    { "Add + 10 DP.", { 42, 4, 61, 1, 9, 0xFF, 0, 0 } },
    { "Add + 20 DP.", { 91, 29, 79, 27, 21, 0xFF, 0, 0 } },
    { "Add + 30 DP.", { 0xFF, 98, 97, 60, 78, 85, 0, 0 } },
    { "Boost Attack Power +50.", { 2, 10, 13, 3, 7, 11, 0, 0 } },
    { "Boost Attack Power +100.", { 21, 42, 56, 33, 36, 35, 0, 0 } },
    { "Boost Attack Power +200.", { 26, 81, 73, 46, 84, 44, 0, 0 } },
    { "Boost Attack Power +300.", { 69, 0xFF, 84, 82, 0xFF, 80, 0, 0 } },
    { "Attack Power is Doubled.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Boost *b0 Attack Power +300.", { 8, 30, 47, 17, 26, 12, 0, 0 } },
    { "Boost *b0 Attack Power +400.", { 34, 77, 74, 47, 46, 39, 0, 0 } },
    { "Boost *b0 Attack Power +500.", { 55, 0xFF, 88, 88, 66, 74, 0, 0 } },
    { "*b0 Attack Power is Doubled.", { 13, 64, 67, 58, 33, 63, 0, 0 } },
    { "*b0 Attack Power is Tripled.", { 59, 0xFF, 94, 74, 0xFF, 86, 0, 0 } },
    { "Boost *b1 Attack Power +200.", { 15, 2, 0xFF, 7, 22, 18, 0, 0 } },
    { "Boost *b1 Attack Power +300.", { 28, 19, 0xFF, 23, 41, 36, 0, 0 } },
    { "Boost *b1 Attack Power +400.", { 43, 70, 0xFF, 92, 59, 53, 0, 0 } },
    { "*b1 Attack Power is Doubled.", { 22, 13, 59, 34, 0xFF, 67, 0, 0 } },
    { "*b1 Attack Power is Tripled.", { 83, 0xFF, 91, 94, 0xFF, 89, 0, 0 } },
    { "Boost *b2 Attack Power +100.", { 23, 40, 10, 18, 0xFF, 8, 0, 0 } },
    { "Boost *b2 Attack Power +200.", { 38, 58, 40, 43, 0xFF, 20, 0, 0 } },
    { "Boost *b2 Attack Power +300.", { 71, 0xFF, 60, 63, 0xFF, 90, 0, 0 } },
    { "*b2 Attack Power is Doubled.", { 44, 72, 48, 51, 0xFF, 40, 0, 0 } },
    { "*b2 Attack Power is Tripled.", { 88, 0xFF, 85, 95, 0xFF, 65, 0, 0 } },
    { "Attack Power becomes same as HP.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Get 1st Attack.", { 53, 55, 98, 71, 85, 0xFF, 0, 0 } },
    { "Attack becomes Eat-up HP.", { 0xFF, 93, 80, 0xFF, 61, 55, 0, 0 } },
    { "Lower Opponent's *b0 Attack Power to 0.", { 54, 0xFF, 17, 0xFF, 28, 75, 0, 0 } },
    { "Lower Opponent's *b1 Attack Power to 0.", { 84, 24, 28, 0xFF, 15, 0xFF, 0, 0 } },
    { "Lower Opponent's *b2 Attack Power to 0.", { 0xFF, 66, 24, 0xFF, 13, 45, 0, 0 } },
    { "*b0 Counterattack (Attack 2nd).", { 7, 0xFF, 33, 0xFF, 47, 14, 0, 0 } },
    { "*b1 Counterattack (Attack 2nd).", { 33, 17, 0xFF, 0xFF, 37, 26, 0, 0 } },
    { "*b2 Counterattack (Attack 2nd).", { 47, 43, 5, 0xFF, 0xFF, 37, 0, 0 } },
    { "If *a0 Opponent, X2 own Attack Power.", { 74, 79, 0xFF, 77, 25, 38, 0, 0 } },
    { "If *a0 Opponent, X3 own Attack Power.", { 0xFF, 91, 0xFF, 65, 74, 82, 0, 0 } },
    { "If *a1 Opponent, X2 own Attack Power.", { 5, 47, 64, 26, 80, 76, 0, 0 } },
    { "If *a1 Opponent, X3 own Attack Power.", { 60, 0xFF, 89, 83, 98, 91, 0, 0 } },
    { "If *a2 Opponent, X2 own Attack Power.", { 50, 27, 58, 59, 42, 6, 0, 0 } },
    { "If *a2 Opponent, X3 own Attack Power.", { 79, 0xFF, 93, 0xFF, 93, 70, 0, 0 } },
    { "If *a3 Opponent, X2 own Attack Power.", { 56, 37, 44, 6, 19, 93, 0, 0 } },
    { "If *a3 Opponent, X3 own Attack Power.", { 92, 60, 87, 79, 63, 0xFF, 0, 0 } },
    { "If *a4 Opponent, X2 own Attack Power.", { 78, 76, 26, 72, 69, 46, 0, 0 } },
    { "If *a4 Opponent, X3 own Attack Power.", { 89, 96, 55, 0xFF, 0xFF, 68, 0, 0 } },
    { "Change own Specialty to *a0.", { 9, 35, 0xFF, 48, 96, 0xFF, 0, 0 } },
    { "Change own Specialty to *a1.", { 0xFF, 99, 30, 87, 58, 0xFF, 0, 0 } },
    { "Change own Specialty to *a2.", { 97, 3, 41, 8, 12, 94, 0, 0 } },
    { "Change own Specialty to *a3.", { 30, 0xFF, 68, 98, 81, 3, 0, 0 } },
    { "Change own Specialty to *a4.", { 95, 82, 14, 52, 76, 99, 0, 0 } },
    { "Switch Opponent's Specialty to own.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Swap Specialty with Opponent's.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *a0 Opponent, lower its AP to 0.", { 66, 52, 0xFF, 97, 43, 0xFF, 0, 0 } },
    { "If *a1 Opponent, lower its AP to 0.", { 41, 61, 34, 84, 8, 0xFF, 0, 0 } },
    { "If *a2 Opponent, lower its AP to 0.", { 0xFF, 11, 52, 0xFF, 64, 22, 0, 0 } },
    { "If *a3 Opponent, lower its AP to 0.", { 0xFF, 25, 37, 13, 2, 72, 0, 0 } },
    { "If *a4 Opponent, lower its AP to 0.", { 0xFF, 75, 16, 0xFF, 34, 52, 0, 0 } },
    { "Reduce both Players' Atk Pwr to 0.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *e3, boost Attack Power +200.", { 14, 31, 0xFF, 9, 48, 23, 0, 0 } },
    { "If *e4, boost Attack Power +300.", { 31, 53, 0xFF, 20, 38, 57, 0, 0 } },
    { "If *e5, boost Attack Power +400.", { 57, 0xFF, 81, 70, 95, 77, 0, 0 } },
    { "Opponent uses *b0 Attack.", { 16, 62, 0xFF, 28, 97, 31, 0, 0 } },
    { "Opponent uses *b1 Attack.", { 62, 18, 11, 39, 0xFF, 47, 0, 0 } },
    { "Opponent uses *b2 Attack.", { 94, 8, 21, 0xFF, 44, 58, 0, 0 } },
    { "Opponent uses same Attack.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Recover HP +200.", { 72, 15, 32, 11, 1, 81, 0, 0 } },
    { "Recover HP +300.", { 90, 38, 53, 31, 17, 0xFF, 0, 0 } },
    { "Recover HP +400.", { 0xFF, 68, 92, 73, 55, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +400.", { 93, 20, 43, 15, 23, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +600.", { 0xFF, 46, 86, 44, 60, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +500.", { 98, 59, 23, 37, 29, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +700.", { 0xFF, 83, 69, 53, 56, 0xFF, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 300.", { 49, 34, 0xFF, 24, 30, 42, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 600.", { 0xFF, 63, 0xFF, 49, 49, 64, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 1000.", { 0xFF, 94, 0xFF, 64, 65, 98, 0, 0 } },
    { "Drop 1 Card in Opponent's Hand.", { 63, 22, 3, 45, 75, 27, 0, 0 } },
    { "Drop 2 Cards in Opponent's Hand.", { 0xFF, 78, 25, 0xFF, 92, 56, 0, 0 } },
    { "Drop Opponent's Top 2 DP Cards shown.", { 0xFF, 56, 18, 86, 35, 5, 0, 0 } },
    { "Drop Opponent's Top 3 DP Cards shown.", { 0xFF, 88, 49, 0xFF, 90, 32, 0, 0 } },
    { "Drop Opponent's Top 4 DP Cards shown.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Drop 2 Cards in Opponent's Online Deck.", { 0xFF, 51, 7, 29, 0xFF, 16, 0, 0 } },
    { "Drop 3 Cards in Opponent's Online Deck.", { 0xFF, 95, 35, 89, 0xFF, 54, 0, 0 } },
    { "Move Offline Top Card to Online Deck.", { 86, 0xFF, 70, 78, 50, 0xFF, 0, 0 } },
    { "Void Opponent's Support Effect.", { 0xFF, 84, 65, 0xFF, 87, 69, 0, 0 } },
    { "Draw until there are 4 Cards.", { 64, 45, 29, 54, 20, 0xFF, 0, 0 } },
    { "Draw Online Partner Card, then Shuffle.", { 65, 80, 82, 68, 72, 0xFF, 0, 0 } },
    { "If *e3, HP + 200 & all Attack Powers +100.", { 73, 73, 0xFF, 55, 94, 83, 0, 0 } },
    { "If *ea, HP + 200 & all Attack Powers +100.", { 81, 0xFF, 76, 66, 79, 87, 0, 0 } },
    { "Boost Battle Experience by 10%.", { 20, 49, 57, 32, 71, 41, 0, 0 } },
    { "Boost Battle Experience by 20%.", { 85, 65, 77, 75, 88, 92, 0, 0 } },
    { "Boost Battle Experience by 30%.", { 80, 0xFF, 96, 90, 99, 60, 0, 0 } },
    { "Rare Card might appear after battle.", { 70, 85, 83, 80, 51, 0xFF, 0, 0 } },
    { "Rare Card even more likely to appear.", { 0xFF, 90, 99, 96, 70, 0xFF, 0, 0 } },
};

/* the u8 arrays among these are not referenced by any code */
UiWindow D_801F4338 = { 0 };
u8 D_801F437C[12] = { 0 };
CursorHighlight D_801F4388 = { { { 0 } } };
UiWindow D_801F43D8 = { 0 };
u8 D_801F441C[12] = { 0 };
CursorHighlight D_801F4428 = { { { 0 } } };
UiWindow D_801F4478 = { 0 };
u8 D_801F44BC[12] = { 0 };
CursorHighlight D_801F44C8 = { { { 0 } } };
UiWindow D_801F4518 = { 0 };
u8 D_801F455C[12] = { 0 };
Unk801F4568 D_801F4568 = { 0 };
Unk801F4588 D_801F4588 = { { { 0 } } };
u8 D_801F46AC[4] = { 0 };
UiWindow D_801F46B0[5] = { { 0 } };
Unk801F4804 *D_801F4804 = NULL;
u8 D_801F4808 = 0;
u8 D_801F480C[4] = { 0 };
Unk801F4810 D_801F4810 = { { 0 } };
u8 D_801F4834 = 0;
ScriptRunner *D_801F4838[1] = { NULL };
u8 D_801F483C[4] = { 0 };
TextLine D_801F4840[3] = { { { 0 } } };
u8 D_801F4900[8] = { 0 };
Unk801F4908 D_801F4908 = { { 0 } };
u8 D_801F4A34[4] = { 0 };
u8 D_801F4A38[0xB8] = { 0 };
Unk801F4AF0 *D_801F4AF0 = NULL;
u8 D_801F4AF4[4] = { 0 };
s16 D_801F4AF8[3] = { 0 };
u8 D_801F4AFE = 0;
u8 D_801F4AFF = 0;
UiWindow D_801F4B00 = { 0 };
UiWindow D_801F4B44 = { 0 };
u8 D_801F4B88[0xD0] = { 0 };
s16 D_801F4C58 = 0;
u8 D_801F4C5C[0x1EC] = { 0 };
Unk801F4E48 D_801F4E48 = { 0 };
u8 D_801F5254[4] = { 0 };
Unk801F5258 D_801F5258 = { 0 };
Sprite3D *D_801F5260[60] = { NULL };
Unk801F5350 *D_801F5350 = NULL;
u8 D_801F5354[4] = { 0 };
RewardScreen *D_801F5358 = NULL;
u8 D_801F535C[4] = { 0 };
UiWindow D_801F5360 = { 0 };
u8 D_801F53A4[12] = { 0 };
CursorHighlight D_801F53B0 = { { { 0 } } };
PartnerCursor D_801F5400 = { 0 };
u8 D_801F5404[4] = { 0 };
PartnerList D_801F5408 = { 0 };
UiWindow D_801F5410 = { 0 };
u8 D_801F5454[4] = { 0 };
u8 D_801F5458[16] = { 0 };

void func_801F2F04(UiWindow *window) {
    char buf[0x18];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 count;
    s32 i;
    s32 icon;
    s32 palette;

    drawText(x + 0x40, y + 1, (s32)"Earned Digi-Parts List", 6, 0);
    count = 0;
    for (i = 0; i < 128; i++) {
        if ((D_801F5458[i / 8] >> (i % 8)) & 1) {
            count++;
            if (count >= (window->view.y - 15) / 13 && (window->view.y + window->rect.h - 15) / 13 >= count) {
                sprintf(buf, "%3.3d", i);
                drawText(x + 2, y + 15 + (count - 1) * 13, (s32)buf, 5, z);
                if (i < 7) {
                    icon = 0;
                } else if (i < 10) {
                    icon = 1;
                } else if (i < 15) {
                    icon = 2;
                } else if (i < 20) {
                    icon = 3;
                } else if (i < 24) {
                    icon = 4;
                } else if (i < 38) {
                    icon = 5;
                } else if (i < 41) {
                    icon = 6;
                } else if (i < 121) {
                    icon = 7;
                } else {
                    icon = 8;
                }
                drawIcon(x + 0x16, y + 15 + (count - 1) * 13, 2, icon, z);
                palette = 7;
                if (i >= 24 && i < 38) {
                    palette = 4;
                }
                if (i >= 41 && i < 121) {
                    palette = 5;
                }
                drawText(x + 0x30, y + 15 + (count - 1) * 13, (s32)D_801F3D38[i].name, palette, z);
            }
        }
    }
    window->view.h = count * 13 + 15;
    if (window->view.h - window->rect.h >= 0) {
        if (PAD_STATES[0]->repeat & 1) {
            scrollWindowTo(&window->originX, 0, window->view.y - window->rect.h);
        }
        if (PAD_STATES[0]->repeat & 2) {
            scrollWindowTo(&window->originX, 0, window->view.y + window->rect.h);
        }
        if (PAD_STATES[0]->repeat & 0x1000) {
            scrollWindowTo(&window->originX, 0, window->view.y - 13);
        }
        if (PAD_STATES[0]->repeat & 0x4000) {
            scrollWindowTo(&window->originX, 0, window->view.y + 13);
        }
    }
    if (count == 0) {
        drawText(x + 0x1A, y + 0xE, (s32)"None", 7, 0);
    }
}

void func_801F329C(void) {
    drawWindow(&D_801F5410, func_801F2F04, 0);
}

extern const char D_801DFA58[];

/*
 * Task that grants a partner ability (a Digi-Part) and shows the parts list
 * until Cross is pressed.
 */
void func_801F32CC(s32 ability, s32 task) {
    Rect16 unused; /* never used, but the original frame has room for it */
    Rect16 rect;
    s32 i;
    UiWindow *win;

    rect.x = 0x28;
    rect.y = 0x2C;
    rect.w = 0xF0;
    rect.h = 0xA8;
    openWindow(&D_801F5410, &rect, -1, (s16 *)-1, 10, 0x16, 0x80, 12);
    D_801F5410.label = (s32)D_801DFA58;
    /*
     * Dead code in the original: this loop's result is never used. GCC deletes
     * the store only after register allocation, which is why the ROM's loop
     * counter sits in a0; without it the code differs.
     */
    for (i = 0; i < 16; i++) {
        if ((D_801F5458[i] >> (ability % 8)) & 1) {
            win = &D_801F5410;
        }
    }
    /* mark the ability as owned */
    D_801F5458[ability / 8] |= 1 << (ability % 8);
    grantPartnerAbility(0, ability);
    playSoundEffect(3);
    addFrameCallback((s32)func_801F329C);
    do {
        func_80014C08(1);
    } while (!(PAD_STATES[0]->pressed & 0x40)); /* Cross */
    playSoundEffect(4);
    win = &D_801F5410;
    animateWindowTo(win, (Rect16 *)-1);
    func_80014C08(20);
    removeFrameCallback((s32)func_801F329C);
    func_80014C08(1);
    D_801F4696 = 0;
    func_80014A48(task);
}

/* the last byte is a leftover in the original, not zero padding */
const char D_801DFA58[20] = "GET DIGIPARTS LIST\0\x99";
