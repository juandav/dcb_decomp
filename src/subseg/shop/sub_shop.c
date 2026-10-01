#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/text.h"
#include "dcb/scene3d.h"
#include "dcb/stage.h"
#include "dcb/wire_grid.h"
#include "dcb/model_load.h"
#include "dcb/model_anim.h"
#include "dcb/anim_control.h"
#include "dcb/frame_callback.h"
#include "dcb/fade.h"
#include "dcb/sound.h"
#include "dcb/sound_play.h"
#include "dcb/pad.h"
#include "dcb/prim.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/subseg.h"

/*
 * jp's SUBSEG is a card shop, a program of its own (us's SUBSEG is the deck
 * editor, deck/sub_deck_screens.c): it buys and sells cards and booster packs, opens the packs bought, and
 * shows a card's picture and its Digimon's model. The executable sets it up
 * with SUB_openBuyShop (buying) or SUB_openSellShop (selling), then spawns
 * SUB_runShop.
 *
 * jp's cards, decks and profile have their own layouts (a Digimon card is
 * 0x122 bytes, a profile holds 110 Digimon, 43 option and 6 Digivolve
 * cards): the shop reads them through the small structs below, which only
 * place the fields it uses.
 */

#define CAMERA ((Graphics *)&GRAPHICS)
#define PROFILE ((JpProfile *)PLAYER_PROFILES)

typedef struct {
    /* 0x0000 */ u8 unk0[0x1F80];
    /* 0x1F80 */ s16 *bonepos[32];
    /* 0x2000 */ u8 unk2000[0x208];
    /* 0x2208 */ s32 loopKey;
} JpModel;

typedef struct {
    /* 0x000 */ u8 unk0[0x1C6];
    /* 0x1C6 */ s8 unk1C6;
    /* 0x1C7 */ u8 unk1C7;
    /* 0x1C8 */ u8 unk1C8;
} MusicState;

typedef struct {
    /* 0x00 */ s16 power;
    /* 0x02 */ u8 unk2[4];
    /* 0x06 */ char name[0x12];
} JpAttack;

typedef struct {
    /* 0x000 */ s16 price; /* in hundreds */
    /* 0x002 */ u8 unk2;
    /* 0x003 */ char name[0x11];
    /* 0x014 */ u8 attr; /* specialty << 4 | level */
    /* 0x015 */ s8 dpCost;
    /* 0x016 */ s8 dpBonus;
    /* 0x017 */ u8 unk17;
    /* 0x018 */ s16 hp;
    /* 0x01A */ JpAttack attacks[3];
    /* 0x062 */ u8 unk62[0xD2 - 0x62];
    /* 0x0D2 */ s8 support;
    /* 0x0D3 */ u8 unkD3;
    /* 0x0D4 */ s8 supportLevel;
    /* 0x0D5 */ char supportText[4][0x13];
    /* 0x121 */ u8 unk121;
} JpDigimonCard;

typedef struct {
    /* 0x00 */ s16 price;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ char name[0x83];
    /* 0x86 */ s8 unk86;
    /* 0x87 */ char text[4][0x13];
    /* 0xD3 */ u8 unkD3;
} JpOptionCard;

typedef struct {
    /* 0x00 */ s16 price;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ char name[0x12];
    /* 0x15 */ char text[4][0x13];
    /* 0x61 */ u8 unk61;
} JpDigivolveCard;

typedef struct {
    /* 0x00 */ s16 price;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ char name[0x60 - 3];
} JpPackData;

typedef struct {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8[0x28];
    /* 0x30 */ s32 unk30;
} JpSave;

typedef struct {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ JpSave *save;
} JpSaveSlot;

typedef struct {
    /* 0x000 */ u8 unk0[0x138];
    /* 0x138 */ JpSaveSlot *slot;
} JpGameState;

typedef struct {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 index;
    /* 0x2 */ u8 unk2[6];
} JpDeckSlot;

typedef struct {
    /* 0x000 */ u8 unk0[0x10];
    /* 0x010 */ JpDeckSlot slots[30];
    /* 0x100 */ u8 unk100[0xC];
} JpDeck;

typedef struct {
    /* 0x0 */ s16 seed;
    /* 0x2 */ s16 nextSeed;
} PackSeeds;

typedef struct {
    /* 0x00 */ s8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 soldBits;
    /* 0x03 */ u8 unk3;
    /* 0x04 */ PackSeeds packs[6];
    /* 0x1C */ s16 seed;
    /* 0x1E */ u8 unk1E[2];
} JpShopRecord;

typedef struct {
    /* 0x000 */ u8 unk0[0x14];
    /* 0x014 */ s32 unk14;
    /* 0x018 */ u8 unk18[0xA];
    /* 0x022 */ u8 unk22;
    /* 0x023 */ u8 unk23;
    /* 0x024 */ s32 money;
    /* 0x028 */ u8 unk28[0x47C - 0x28];
    /* 0x47C */ u8 digimonCards[110];
    /* 0x4EA */ u8 optionCards[43];
    /* 0x515 */ u8 digivolveCards[6];
    /* 0x51B */ u8 unk51B[0xF20 - 0x51B];
    /* 0xF20 */ s32 flags[10];
    /* 0xF48 */ JpDeck decks[3];
    /* 0x126C */ u8 unk126C[0x137C - 0x126C];
    /* 0x137C */ JpShopRecord shops[8];
} JpProfile;

extern MusicState *SCROLLING_BACKGROUND;
extern s16 CAMERA_TARGET_MODEL;
extern JpGameState *D_8007E5F8;
extern u8 *OPTION_CARDS; /* as dcb/card_db.h */
extern u8 *DIGIVOLVE_CARDS;
#define SUB_DIGIMON_CARDS ((JpDigimonCard *)DIGIMON_CARDS)
#define SUB_OPTION_CARDS ((JpOptionCard *)OPTION_CARDS)
#define SUB_DIGIVOLVE_CARDS ((JpDigivolveCard *)DIGIVOLVE_CARDS)
extern u8 CROSS_EFFECT_ICONS[];
extern char *CROSS_EFFECT_NAMES[];
extern JpPackData D_8007EAA0[];

void SetDrawEnv(DR_ENV *dr_env, DRAWENV *env);
void clearKanjiPage(s32);
void closeKanjiPage(s32);
void openKanjiPage(s32, s32);
s32 func_80043374(u8 type, u8 index, s8 count);
void func_80043740(u8 type, u8 index, s8 count);
void func_80044758(JpCursor *cursor);
void runWindowTask();
void renderScrollingBackground();
void setBackgroundScrollMode(s32);
void stopScreenFade(void);
void drawWindowFrame(Rect16 *rect, s32, s32, s32, s32, void *, s32 z);
void KAW_drawCursor();
void drawScrollArrow(s32 x, s32 y, s32 dir, s32 palette, s32 z);
char *formatSjisNumber(s32 value, s32 digits, char *buf);
void uploadKanjiString(char *text, Rect16 *rect);
JpCursor *func_80044334(s32, s32, s32, s32, s32);
void initWindowSprite(POLY_FT4 *poly, s32 clut, s32 mode, s32 u, s32 v, s32 w, s32 h);

/* sub_name_entry_jp.c */
void func_801EA908(void);
void func_801EA910(void);

void func_801F0EA0();
void func_801F127C();
void func_801F1FC4();
void func_801F0404(void);
ShopList *func_801ED598(s32 deck, u32 mode, s32 group);
void func_801F1CCC();
s32 SUB_getItemPrice(s8 type, s16 index);
s16 SUB_shopRandom(s16 seed);
void func_801ED750(ShopList *list);
void func_801ED7C0(ShopList *list);

void func_801EA918(JpWindow *window) {
    char text[64];

    /* Ｒ１／Ｒ２　拡大／縮小　　Ｌ１／Ｌ２　上げる／下げる */
    sprintf(text, " \x82q\x82P\x81^\x82q\x82Q\x81@\x8Ag\x91\xE5\x81^\x8Fk\x8F\xAC\x81@\x81@\x82k\x82P\x81^\x82k\x82Q\x81@\x8F\xE3\x82\xB0\x82\xE9\x81^\x89\xBA\x82\xB0\x82\xE9");
    drawIconText(0x1A, 0x18, 7, 1, window->z, (s32)text);
}

void func_801EA970(JpWindow *window) {
    char text[64];

    /* b0b2b1　モーション　　　b3　戻る */
    sprintf(text, " b0b2b1\x81@\x83\x82\x81[\x83V\x83\x87\x83\x93\x81@\x81@\x81@b3\x81@\x96\xDF\x82\xE9");
    drawIconText(0x1A, 0xD1, 7, 1, window->z, (s32)text);
}

void func_801EA9C8(void) {
    initScene3D(1);
    createWireGrid(3000, 3000, 11, 11, 1, 0);
    GRID_VISIBLE = 0;
    endTask(0x19);
    spawnTask(0x19, 0x1F, 0, 0x800, runSceneCameraTask, 1);
    endTask(0x1B);
    spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
    loadArenaStage(-1);
}

void func_801EAA74(void) {
    endTask(0x1B);
    endTask(0x19);
    removeFrameCallback((s32)renderSceneModels);
    removeFrameCallback((s32)renderWireGrid);
    unloadArenaStage();
    unloadAllModels();
    freeHeapBlocksByTag(0x7F);
}

void func_801EAACC(void) {
    CAMERA->snapCamera = 1;
    CAMERA->targetDistance = 3000;
    CAMERA->targetHeight = -((JpModel *)SCENE_3D->models[0])->bonepos[0][1] * 3;
    SCENE_3D->modelState[0] = 1;
    showArenaStage(0);
    SCENE_3D->modelState[23] = 1;
    GRID_VISIBLE = 1;
    applyAnimationFirstFrame(0, 0);
    startModelAnimation(0, 0, -2, 0);
}

void func_801EAB7C(void) {
    freeHeapBlocksByTag(500);
    unloadModel(0);
    unloadModelAnimations(0);
    GRID_VISIBLE = 0;
    SCENE_3D->modelState[23] = 0;
}

void func_801EABC4(void) {
    s32 anim;

    CAMERA_TARGET_MODEL = 0;
    anim = 1;
    if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
        anim = 1;
    } else if (PAD_STATES[0]->rawPressed & PAD_TRIANGLE) {
        anim = 2;
    } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
        anim = 3;
    }
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 1, 2, 0x20, 0);
    applyAnimationFirstFrame(0, anim);
    startModelAnimation(0, anim, -2, 0);
    waitFrames(8);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (((JpModel *)SCENE_3D->models[0])->loopKey >= 0);
    waitFrames(30);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 1, 2, 0x20, 0);
    applyAnimationFirstFrame(0, 0);
    startModelAnimation(0, 0, -2, 0);
    CAMERA_TARGET_MODEL = -1;
    waitFrames(8);
}

void func_801EAD68(s32 parentTask) {
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3460, getCurrentTaskId());
    D_801F3508[0] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3480, getCurrentTaskId());
    D_801F3508[1] = (JpWindow *)waitFrames(0x7FFFFFFF);
    do {
        waitFrames(FRAME_INTERVAL);
        if ((PAD_STATES[0]->rawHeld & PAD_R1) && CAMERA->targetDistance > 1000) {
            CAMERA->targetDistance -= 25;
        } else if ((PAD_STATES[0]->rawHeld & PAD_R2) && CAMERA->targetDistance < 5000) {
            CAMERA->targetDistance += 25;
        } else if (PAD_STATES[0]->rawHeld & PAD_RIGHT) {
            CAMERA->rotY -= 16;
        } else if (PAD_STATES[0]->rawHeld & PAD_LEFT) {
            CAMERA->rotY += 16;
        } else if (PAD_STATES[0]->rawHeld & PAD_UP) {
            CAMERA->targetPitch -= 16;
        } else if (PAD_STATES[0]->rawHeld & PAD_DOWN) {
            CAMERA->targetPitch += 16;
        } else if ((PAD_STATES[0]->rawHeld & PAD_L1) && CAMERA->targetHeight > -1000) {
            CAMERA->targetHeight -= 25;
        } else if ((PAD_STATES[0]->rawHeld & PAD_L2) && CAMERA->targetHeight < 1000) {
            CAMERA->targetHeight += 25;
        } else if (PAD_STATES[0]->rawPressed & (PAD_TRIANGLE | PAD_CIRCLE | PAD_CROSS)) {
            func_801EABC4();
        }
    } while (!(PAD_STATES[0]->rawRepeat & PAD_SQUARE));
    D_801F3508[0]->state = 4;
    D_801F3508[1]->state = 4;
    waitFrames(10);
    resumeTask(parentTask);
}

void SUB_viewDigimonModel(s32 modelId) {
    s32 level;
    s32 next;

    level = SCROLLING_BACKGROUND->unk1C6;
    next = level + 1;
    if (next >= 5) {
        next = 2;
    }
    setBackgroundScrollMode(next);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SCROLLING_BACKGROUND->unk1C8 != 0);
    removeFrameCallback((s32)renderScrollingBackground);
    loadSoundEffectBank(0);
    loadDigimonModelPak(0, modelId);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 1, 2, 0x10, 0);
    func_801EAACC();
    waitFrames(0x10);
    spawnTask(0, -1, 0, 0x400, func_801EAD68, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 0x10, 0);
    waitFrames(0x10);
    func_801EAB7C();
    waitFrames(10);
    stopScreenFade();
    loadSoundEffectBank(1);
    setBackgroundScrollMode(level);
    addFrameCallback((s32)renderScrollingBackground);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SCROLLING_BACKGROUND->unk1C8 != 0x80);
}

s32 func_801EB21C(s32 type, s32 index) {
    s32 id = 0;

    switch (type) {
    case 3:
        id += 6;
    case 2:
        id += 43;
    case 1:
        id += 110;
    case 0:
        id += index;
    }
    return id;
}

s32 SUB_countOwnedDigimonCards(void) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < 110; i++) {
        total += PROFILE->digimonCards[i] & 0xF;
    }
    return total;
}

s32 SUB_countOwnedOptionCards(void) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < 43; i++) {
        total += PROFILE->optionCards[i] & 0xF;
    }
    return total;
}

s32 SUB_countOwnedDigivolveCards(void) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < 6; i++) {
        total += PROFILE->digivolveCards[i] & 0xF;
    }
    return total;
}

s32 SUB_countOwnedCards(void) {
    return SUB_countOwnedDigimonCards() + SUB_countOwnedOptionCards() + SUB_countOwnedDigivolveCards();
}

s32 func_801EB370(s32 specialty, s32 level) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < 110; i++) {
        if ((PROFILE->digimonCards[i] & 0xF)
            && (specialty < 0 || SUB_DIGIMON_CARDS[i].attr >> 4 == specialty)
            && (level < 0 || (SUB_DIGIMON_CARDS[i].attr & 0xF) == level)) {
            total += PROFILE->digimonCards[i] & 0xF;
        }
    }
    return total;
}

s32 func_801EB42C(s32 deck, s32 specialty, s32 level) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < 30; i++) {
        if (PROFILE->decks[deck].slots[i].type == 0
            && (specialty < 0 || SUB_DIGIMON_CARDS[PROFILE->decks[deck].slots[i].index].attr >> 4 == specialty)
            && (level < 0 || (SUB_DIGIMON_CARDS[PROFILE->decks[deck].slots[i].index].attr & 0xF) == level)) {
            total++;
        }
    }
    return total;
}

s32 func_801EB524(s32 deck) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < 30; i++) {
        if (PROFILE->decks[deck].slots[i].type != 0) {
            total++;
        }
    }
    return total;
}

s32 func_801EB580(s32 deck, s32 type, s32 specialty, s32 level) {
    s32 count = 0;

    if (type < 0) {
        if (deck == 0) {
            count = SUB_countOwnedCards();
        }
    } else if (deck == 0) {
        if (type == 0) {
            count = func_801EB370(specialty, level);
        } else {
            count = SUB_countOwnedOptionCards() + SUB_countOwnedDigivolveCards();
        }
    } else if (type == 0) {
        count = func_801EB42C(deck - 1, specialty, level);
    } else {
        count = func_801EB524(deck - 1);
    }
    return count;
}

ItemIcon func_801EB618(s32 type, s32 index) {
    ItemIcon icon;
    s32 id = func_801EB21C(type, index);
    s32 u;

    if (type == 3) {
        icon.u = (index % 3) * 40;
        icon.v = (index / 3) * 40;
        icon.tpage = 0x97;
        icon.palette = -(index + 0x65);
    } else {
        u = 0x180 + (id % 36) / 6 * 20 + (id / 36) * 128;
        icon.u = (u * 2) & 0xFF;
        icon.v = ((id % 6) * 40) & 0xFF;
        icon.tpage = ((u & 0x380) >> 6) | 0x80;
        if (type == 0) {
            icon.palette = SUB_DIGIMON_CARDS[id].attr >> 4;
        } else {
            icon.palette = type + 4;
        }
        if (type == 0 && index >= 108) {
            s32 inv = ~icon.palette;

            icon.palette = inv;
        }
        if (type == 1 && index - 35 < 8U) {
            icon.palette++;
        }
    }
    return icon;
}

s32 func_801EB810(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (PROFILE->decks[i].unk0[0] == 0) {
            break;
        }
    }
    return i;
}

void func_801EB85C(s32 type, s32 index, s32 count) {
    JpDeckSlot *slot;
    s32 i;

    if (count == 0) {
        return;
    }
    slot = PROFILE->decks[D_801FC8C8.deck].slots;
    if (count < 0) {
        for (i = 0; i < 30; i++, slot++) {
            if (type == slot->type && index == slot->index) {
                slot->type = 0xFF;
                if (++count >= 0) {
                    return;
                }
            }
        }
    } else {
        for (i = 0; i < 30; i++, slot++) {
            if (slot->type == 0xFF) {
                slot->type = type;
                slot->index = index;
                if (--count <= 0) {
                    return;
                }
            }
        }
    }
}

void func_801EB928(void) {
}

void func_801EB930(ShopItem *items, s32 left, s32 right) {
    ShopItem swap;
    s32 pivot;
    s32 j;
    s32 i;

    pivot = func_801EB21C(items[(left + right) / 2].type, items[(left + right) / 2].index);
    i = left;
    j = right;
    while (1) {
        while (func_801EB21C(items[i].type, items[i].index) < pivot) {
            if (i >= right) {
                break;
            }
            i++;
        }
        while (pivot < func_801EB21C(items[j].type, items[j].index)) {
            if (left >= j) {
                break;
            }
            j--;
        }
        if (i >= j) {
            break;
        }
        swap = items[i];
        items[i] = items[j];
        items[j] = swap;
        i++;
        j--;
    }
    if (left < i - 1) {
        func_801EB930(items, left, i - 1);
    }
    if (j + 1 < right) {
        func_801EB930(items, j + 1, right);
    }
}

void func_801EBB3C(ShopList *list, s32 deck) {
    JpDeckSlot *slot;
    s32 i;
    s32 j;

    for (i = 0; i < list->count; i++) {
        slot = PROFILE->decks[deck].slots;
        for (j = 0; j < 30; j++, slot++) {
            if (list->items[i].type == slot->type && list->items[i].index == slot->index) {
                list->items[i].count++;
            }
        }
    }
}

void func_801EBBF4(ShopList *list) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        switch (list->items[i].type) {
        case 0:
            list->items[i].max = PROFILE->digimonCards[list->items[i].index] & 0x7F;
            break;
        case 1:
            list->items[i].max = PROFILE->optionCards[list->items[i].index] & 0x7F;
            break;
        case 2:
            list->items[i].max = PROFILE->digivolveCards[list->items[i].index] & 0x7F;
            break;
        case 3:
            list->items[i].max = 8;
            break;
        }
    }
}

s32 func_801EBD18(ShopList *list, u8 type, u8 index) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        if (list->items[i].type == type && list->items[i].index == index) {
            list->items[i].count++;
            return 1;
        }
    }
    list->items[i].count = 1;
    list->items[i].max = 0;
    list->items[i].owned = 0;
    list->items[i].brightness = 0x40;
    list->items[i].icon = func_801EB618(type, index);
    list->items[i].type = type;
    list->items[i].index = index;
    list->count++;
    return 0;
}

void func_801EBE44(ShopList *list, s32 deck) {
    JpDeckSlot *slot = PROFILE->decks[deck].slots;
    s32 i;

    for (i = 0; i < 30; i++, slot++) {
        if (slot->type != 0xFF) {
            func_801EBD18(list, slot->type, slot->index);
        }
    }
    func_801EB930(list->items, 0, list->count - 1);
}

void func_801EBEEC(ShopList *list, s32 specialty, s32 level) {
    s32 i;

    for (i = 0; i < 110; i++) {
        if ((PROFILE->digimonCards[i] & 0xF) && SUB_DIGIMON_CARDS[i].attr >> 4 == specialty
            && (level < 0 || (SUB_DIGIMON_CARDS[i].attr & 0xF) == level)) {
            func_801EBD18(list, 0, i);
        }
    }
}

void func_801EBFBC(ShopList *list) {
    s32 i;

    for (i = 0; i < 43; i++) {
        if (PROFILE->optionCards[i] & 0xF) {
            func_801EBD18(list, 1, i);
        }
    }
    for (i = 0; i < 6; i++) {
        if (PROFILE->digivolveCards[i] & 0xF) {
            func_801EBD18(list, 2, i);
        }
    }
}

void func_801EC074(ShopList *list, s8 group) {
    s32 i;

    for (i = 0; i < 43; i++) {
        if ((PROFILE->optionCards[i] & 0xF) && D_801F34A0[i] == group) {
            func_801EBD18(list, 1, i);
        }
    }
}

void func_801EC118(ShopList *list) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (PROFILE->digivolveCards[i] & 0xF) {
            func_801EBD18(list, 2, i);
        }
    }
}

void func_801EC18C(ShopList *list) {
    s32 i;

    for (i = 0; i < 110; i++) {
        if (PROFILE->digimonCards[i] & 0x80) {
            func_801EBD18(list, 0, i);
        }
    }
    for (i = 0; i < 43; i++) {
        if (PROFILE->optionCards[i] & 0x80) {
            func_801EBD18(list, 1, i);
        }
    }
    for (i = 0; i < 6; i++) {
        if (PROFILE->digivolveCards[i] & 0x80) {
            func_801EBD18(list, 2, i);
        }
    }
}

void func_801EC288(ShopList *list) {
    s32 i;

    for (i = 0; i < 110; i++) {
        if (PROFILE->digimonCards[i] & 0xF) {
            func_801EBD18(list, 0, i);
        }
    }
    func_801EBFBC(list);
}

void func_801EC304(ShopList *list) {
    s32 i;
    u8 index;

    for (i = 0; i < list->count; i++) {
        index = list->items[i].index;
        switch (list->items[i].type) {
        case 0:
            if (index < 110) {
                list->items[i].owned = PROFILE->digimonCards[index] & 0x7F;
            }
            break;
        case 1:
            if (index < 43) {
                list->items[i].owned = PROFILE->optionCards[index] & 0x7F;
            }
            break;
        case 2:
            if (index < 6) {
                list->items[i].owned = PROFILE->digivolveCards[index] & 0x7F;
            }
            break;
        case 3:
            if (index < 6) {
                list->items[i].owned = 1;
            }
            break;
        }
    }
}

void func_801EC444(ShopList *list) {
    s32 bit = 0;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (D_801F3700->packs[i].count != 0) {
            func_801EBD18(list, 3, i);
        }
    }
    for (i = 0; i < 110; i++) {
        while (D_801F3700->digimonCards[i].count != 0) {
            if (!((PROFILE->shops[D_801F3700->shopId].soldBits >> bit) & 1)) {
                func_801EBD18(list, 0, i);
            }
            D_801F3700->digimonCards[i].count--;
            bit++;
        }
    }
    for (i = 0; i < 43; i++) {
        while (D_801F3700->optionCards[i].count != 0) {
            if (!((PROFILE->shops[D_801F3700->shopId].soldBits >> bit) & 1)) {
                func_801EBD18(list, 1, i);
            }
            D_801F3700->optionCards[i].count--;
            bit++;
        }
    }
    for (i = 0; i < 6; i++) {
        while (D_801F3700->digivolveCards[i].count != 0) {
            if (!((PROFILE->shops[D_801F3700->shopId].soldBits >> bit) & 1)) {
                func_801EBD18(list, 2, i);
            }
            D_801F3700->digivolveCards[i].count--;
            bit++;
        }
    }
    func_801EC304(list);
}

void func_801EC6F4(ShopList *list) {
    ShopItem swap;
    s32 gap;
    s32 i;
    s32 j;

    for (gap = 1; gap < list->count / 3; gap = gap * 3 + 1) {
    }
    for (; gap > 0; gap /= 3) {
        for (i = gap; i < list->count; i++) {
            for (j = i - gap; j >= 0; j -= gap) {
                if (func_801EB21C(list->items[j].type, list->items[j].index)
                    <= func_801EB21C(list->items[j + gap].type, list->items[j + gap].index)) {
                    break;
                }
                swap = list->items[j];
                list->items[j] = list->items[j + gap];
                list->items[j + gap] = swap;
            }
        }
    }
}

void func_801EC904(u8 *unused, u8 pack, u8 level) {
    u8 ranges[7][3][2] = {
        { { 0x00, 0x02 }, { 0x03, 0x0A }, { 0x0B, 0x13 } },
        { { 0x14, 0x16 }, { 0x17, 0x1E }, { 0x1F, 0x29 } },
        { { 0x2A, 0x2C }, { 0x2D, 0x38 }, { 0x39, 0x44 } },
        { { 0x45, 0x47 }, { 0x48, 0x50 }, { 0x51, 0x5A } },
        { { 0x5B, 0x5D }, { 0x5E, 0x63 }, { 0x64, 0x6B } },
        { { 0x6E, 0x80 }, { 0x81, 0x87 }, { 0x88, 0x90 } },
        { { 0x99, 0x99 }, { 0x9A, 0x9C }, { 0x9D, 0x9D } },
    };
    u8 id;

    for (id = ranges[pack][level][0]; id <= ranges[pack][level][1];) {
        *D_801F3784 = id;
        id++;
        D_801F3784++;
    }
    *D_801F3784 = 0xFF;
}

/* Returns an int: func_801ECC68's code needs the wide return value, and
   func_801F2618 narrows it to u8 itself. */
s32 func_801ECA7C(u8 pack, u8 level) {
    u8 *ids;
    u8 swap;
    s32 count;
    s32 i;
    s32 j;
    s16 k;

    ids = D_801F3784 = allocTaskHeapBlock(200);
    if (pack == 0xFF) {
        if (level >= 4) {
            level = 3;
        }
        if (level == 3) {
            for (j = 0; j < 7; j++) {
                for (i = 0; i < 3; i++) {
                    func_801EC904(D_801F3784, j, i);
                }
            }
        } else {
            for (i = 0; i < 7; i++) {
                func_801EC904(D_801F3784, i, level);
            }
        }
    } else {
        if (level >= 4 || pack >= 7) {
            level = 3;
            pack = 0;
        }
        if (level == 3) {
            for (i = 0; i < 3; i++) {
                func_801EC904(D_801F3784, pack, i);
            }
        } else {
            func_801EC904(D_801F3784, pack, level);
        }
    }
    count = D_801F3784 - ids;
    for (i = 0; i < count; i++) {
        k = SUB_shopRandom(-1) % count;
        swap = ids[i];
        ids[i] = ids[k];
        ids[k] = swap;
    }
    freeHeapBlock(ids);
    return ids[0];
}

void func_801ECC68(ShopList *list) {
    s32 count = 165;
    u8 order[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    s32 i;
    s32 j;
    s32 k;
    s32 r;
    u8 swap;
    u8 pack;
    u8 level;
    u8 id;

    for (i = 0; i < D_801F3700->list.count; i++) {
        if (D_801F3700->list.items[i].type != 3) {
            while (D_801F3700->list.items[i].count != 0) {
                func_801EBD18(list, D_801F3700->list.items[i].type, D_801F3700->list.items[i].index);
                D_801F3700->list.items[i].count--;
            }
        }
    }
    for (i = 0; i < count; i++) {
        if (D_801F3700->list.items[i].type == 3) {
            SUB_shopRandom(PROFILE->shops[D_801F3700->shopId].packs[D_801F3700->list.items[i].index].seed);
            while (D_801F3700->list.items[i].count != 0) {
                for (j = 0; j < 4; j++) {
                    swap = order[j];
                    r = (s16)(SUB_shopRandom(-1) % 8);
                    order[j] = order[r];
                    order[r] = swap;
                }
                for (k = 0; k < 8; k++) {
                    SUB_shopRandom(-1);
                    for (j = 0; j < 4; j++) {
                        if (order[j] == k && D_801F3700->list.items[i].index < 5) {
                            pack = D_801F3700->list.items[i].index;
                            break;
                        }
                        pack = 0xFF;
                    }
                    if (k == 0) {
                        level = 0;
                    } else if (k < 3) {
                        level = 1;
                    } else {
                        level = 2;
                    }
                    id = func_801ECA7C(pack, level);
                    if (id < 110) {
                        func_801EBD18(list, 0, id);
                    } else {
                        id -= 110;
                        if (id < 43) {
                            func_801EBD18(list, 1, id);
                        } else {
                            func_801EBD18(list, 2, id - 43);
                        }
                    }
                    if (!(k & 1)) {
                        waitFrames(1);
                    }
                }
                D_801F3700->list.items[i].count--;
                PROFILE->shops[D_801F3700->shopId].packs[D_801F3700->list.items[i].index].seed =
                    PROFILE->shops[D_801F3700->shopId].packs[D_801F3700->list.items[i].index].nextSeed;
                PROFILE->shops[D_801F3700->shopId].packs[D_801F3700->list.items[i].index].nextSeed = SUB_shopRandom(-1);
            }
        }
    }
    func_801EC6F4(list);
    func_801EC304(list);
}

void func_801ED080(ShopList *list) {
    u8 most;
    u8 copies;
    s32 i;
    s32 deck;
    s32 slot;

    for (i = 0; i < 110; i++) {
        most = 0;
        for (deck = 0; deck < 3; deck++) {
            copies = 0;
            if (PROFILE->decks[deck].unk0[0] != 0) {
                for (slot = 0; slot < 30; slot++) {
                    if (PROFILE->decks[deck].slots[slot].type == 0 && PROFILE->decks[deck].slots[slot].index == i) {
                        copies++;
                    }
                }
                if (copies > most) {
                    most = copies;
                }
            }
        }
        D_801F3700->digimonCards[i].stock -= most;
        if (SUB_DIGIMON_CARDS[i].price > 0) {
            while (--D_801F3700->digimonCards[i].stock != 0xFF) {
                func_801EBD18(list, 0, i);
            }
        }
    }
    for (i = 0; i < 43; i++) {
        most = 0;
        for (deck = 0; deck < 3; deck++) {
            copies = 0;
            if (PROFILE->decks[deck].unk0[0] != 0) {
                for (slot = 0; slot < 30; slot++) {
                    if (PROFILE->decks[deck].slots[slot].type == 1 && PROFILE->decks[deck].slots[slot].index == i) {
                        copies++;
                    }
                }
                if (copies > most) {
                    most = copies;
                }
            }
        }
        D_801F3700->optionCards[i].stock -= most;
        if (SUB_OPTION_CARDS[i].price > 0) {
            while (--D_801F3700->optionCards[i].stock != 0xFF) {
                func_801EBD18(list, 1, i);
            }
        }
    }
    for (i = 0; i < 6; i++) {
        most = 0;
        for (deck = 0; deck < 3; deck++) {
            copies = 0;
            if (PROFILE->decks[deck].unk0[0] != 0) {
                for (slot = 0; slot < 30; slot++) {
                    if (PROFILE->decks[deck].slots[slot].type == 2 && PROFILE->decks[deck].slots[slot].index == i) {
                        copies++;
                    }
                }
                if (copies > most) {
                    most = copies;
                }
            }
        }
        D_801F3700->digivolveCards[i].stock -= most;
        if (SUB_DIGIVOLVE_CARDS[i].price > 0) {
            while (--D_801F3700->digivolveCards[i].stock != 0xFF) {
                func_801EBD18(list, 2, i);
            }
        }
    }
    for (slot = 0; slot < list->count; slot++) {
        list->items[slot].max = list->items[slot].count;
        list->items[slot].count = 0;
        list->items[slot].owned = 1;
    }
}

void func_801ED53C(ShopList *list) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        list->items[i].owned = list->items[i].max - list->items[i].count;
        list->items[i].count = 0;
    }
}

ShopList *func_801ED598(s32 deck, u32 mode, s32 group) {
    ShopList *list = allocHeapBlock(0x920, 0x195);

    bzero((Scene3D *)list, 0x920);
    switch (mode) {
    case 0:
        func_801EBE44(list, deck);
        break;
    case 1:
        func_801EBEEC(list, 0, group);
        break;
    case 2:
        func_801EBEEC(list, 1, group);
        break;
    case 3:
        func_801EBEEC(list, 2, group);
        break;
    case 4:
        func_801EBEEC(list, 3, group);
        break;
    case 5:
        func_801EBEEC(list, 4, group);
        break;
    case 6:
        func_801EBFBC(list);
        break;
    case 7:
        func_801EC18C(list);
        break;
    case 8:
        func_801EC288(list);
        break;
    case 9:
        func_801EC074(list, group);
        break;
    case 10:
        func_801EC118(list);
        break;
    case 11:
        func_801EC444(list);
        break;
    case 12:
        func_801ECC68(list);
        break;
    case 13:
        func_801ED080(list);
        break;
    }
    if (mode != 12) {
        if (mode == 11) {
            func_801ED7C0(list);
        } else if (mode != 13) {
            func_801EBBF4(list);
        }
    } else {
        func_801EBBF4(list);
        func_801ED750(list);
    }
    D_801FC8E8 = list;
    return list;
}

void func_801ED750(ShopList *list) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        list->items[i].max += list->items[i].count;
        if (list->items[i].max >= 9) {
            list->items[i].max = 8;
        }
    }
}

void func_801ED7C0(ShopList *list) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        if (list->items[i].type == 3) {
            list->items[i].max = 8;
        } else {
            list->items[i].max = list->items[i].count;
        }
        list->items[i].count = 0;
    }
}

void func_801ED84C(JpWindow *window) {
    char text[152];
    POLY_FT4 *poly;
    s32 y;
    s16 flip;
    s16 dx;
    s16 dy;

    poly = D_801F3700->polys[FRAME_BUFFER_INDEX];
    y = 0xB2;
    if (D_801F3700->paid == 0) {
        /* b0枚数決定 */
        sprintf(text, "b0\x96\x87\x90\x94\x8C\x88\x92\xE8 ");
        drawIconText(0x1C, y, 7, 1, window->z, (s32)text);
    } else {
        y -= 12;
    }
    if (D_801FC8C8.type == 0) {
        y += 12;
        /* b1ビューモード */
        sprintf(text, "b1\x83r\x83\x85\x81[\x83\x82\x81[\x83h ");
        drawIconText(0x1C, y, 7, 1, window->z, (s32)text);
    }
    y += 12;
    /* b2キャンセル */
    sprintf(text, "b2\x83L\x83\x83\x83\x93\x83Z\x83\x8B ");
    drawIconText(0x1C, y, 7, 1, window->z, (s32)text);
    if (D_801F3700->loading == 0 && D_801F3700->flipTimer-- <= 0) {
        D_801F3700->flipTimer = 0;
        if (D_801F3700->flipBack == 0) {
            D_801F3700->flip++;
        } else {
            D_801F3700->flip--;
        }
    }
    if (D_801F3700->flip <= 0) {
        D_801F3700->flip = 0;
    }
    if (D_801F3700->flip == 10) {
        D_801F3700->flipBack = 1;
    }
    if (D_801F3700->flipBack == 1) {
        flip = D_801F3700->flip;
        initWindowSprite(poly, (D_801F3700->imageSlot->clutY << 6) | ((D_801F3700->imageSlot->clutX >> 4) & 0x3F), 1,
                      D_801F3700->imageSlot->x, D_801F3700->imageSlot->y, 0x40, 0x40);
        dx = flip * 32 / 10;
        dy = D_801F3700->flip * 6 / 10;
        poly->x0 = dx + 36;
        poly->y0 = dy + 74;
        poly->x1 = 100 - dx;
        poly->y1 = 74 - dy;
        poly->x2 = dx + 36;
        poly->y2 = 138 - dy;
        poly->x3 = 100 - dx;
        poly->y3 = dy + 138;
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[window->z], (s32)D_801F3700->polys[FRAME_BUFFER_INDEX]);
        poly++;
        if (D_801FC8C8.type == 3) {
            initWindowSprite(poly, 0x7F01, 0, 0x220, 0x180, 0x50, 0x74);
        } else {
            initWindowSprite(poly, 0x7F00, 0, 0x1A8, 0x100, 0x50, 0x74);
        }
        dx = flip * 4;
        dy = D_801F3700->flip * 6 / 10;
        poly->x0 = dx + 28;
        poly->y3 = dy + 170;
        poly->x1 = 108 - dx;
        poly->y2 = 170 - dy;
        poly->x2 = dx + 28;
        poly->y1 = 54 - dy;
        poly->x3 = 108 - dx;
        poly->y0 = dy + 54;
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[window->z], (s32)(D_801F3700->polys[FRAME_BUFFER_INDEX] + 1));
    } else {
        poly += 2;
        initWindowSprite(poly, 0x3DD8, 1, 0x180, 0x100, 0x50, 0x74);
        dx = D_801F3700->flip * 4;
        dy = D_801F3700->flip * 6 / 10;
        poly->x0 = dx + 28;
        poly->y0 = 54 - dy;
        poly->x1 = 108 - dx;
        poly->y1 = dy + 54;
        poly->x2 = dx + 28;
        poly->y2 = dy + 170;
        poly->x3 = 108 - dx;
        poly->y3 = 170 - dy;
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[window->z], (s32)(D_801F3700->polys[FRAME_BUFFER_INDEX] + 2));
    }
}

void func_801EDDA0(ItemIcon icon, s32 x, s32 y, s32 z, u8 brightness) {
    s32 palette;
    s32 tpage;
    s32 clutX;
    s32 clut;

    if (icon.palette < 0) {
        /* func_801EB618 stored ~palette */
        clutX = 0xFD;
        clut = (s16)(icon.palette + 1);
        icon.palette = -clut;
    } else {
        clutX = 0;
    }
    tpage = icon.tpage;
    palette = icon.palette;
    icon.palette = 40;
    icon.tpage = 40;
    if (clutX == 0) {
        clut = ((palette + 0xF9) << 6) | 0x18;
    } else if (palette >= 100) {
        palette -= 100;
        clut = 0x3E18;
    } else {
        clut = (clutX << 6) | 0x18;
    }
    drawTexturedSprite(x, y + 3, (Rect16 *)&icon, tpage, clut, z, brightness, -1);
    if (palette >= 6) {
        palette = 5;
    }
    icon.u = 0;
    icon.v = 0xBD;
    icon.tpage = 40;
    icon.palette = 48;
    drawTexturedSprite(x, y, (Rect16 *)&icon, 0x17, ((palette + 0x1C0) << 6) | 0x1D, z, brightness, -1);
}

void func_801EDEF4(JpWindow *window) {
    char text[32];
    char number[24];
    Rect16 rect = { 0, 0xF0, 0x18, 9 };
    JpDigimonCard *card;
    JpPackData *pack;
    char *name;
    s32 id;
    u8 index;
    s32 price;
    s32 i;
    s32 y;
    s32 x;
    s32 n;
    s32 type;

    if (D_801FC8E8->count == 0) {
        return;
    }
    for (n = 0, i = D_801FC8E8->top; n < 10; n++, i++) {
        if (i >= D_801FC8E8->count) {
            break;
        }
        x = (n % 5) * 50 + 0x1A;
        y = (n / 5) * 64 + 0x38;
        if (D_801F3700->mode != 11) {
            price = 0;
        } else {
            price = SUB_getItemPrice(D_801FC8E8->items[i].type, D_801FC8E8->items[i].index) + D_801F3700->total;
            if (PROFILE->money < price) {
                price = -1;
            }
        }
        if (D_801FC8E8->cursor == i) {
            D_801FC8E8->items[i].brightness = 0x80;
            D_801F353C->x = x + 20;
            D_801F353C->y = y + 24;
        } else if (D_801FC8E8->items[i].brightness > 0x40) {
            if ((D_801FC8E8->items[i].brightness -= 0x10) < 0x40) {
                D_801FC8E8->items[i].brightness = 0x40;
            }
        }
        if (D_801FC8E8->items[i].owned == 0) {
            drawTexturedSprite(x + 14, y + 32, &rect, 0x17, 0x7E5C, window->z, D_801FC8E8->items[i].brightness, -1);
        }
        func_801EDDA0(D_801FC8E8->items[i].icon, x, y, window->z, D_801FC8E8->items[i].brightness);
        sprintf(text, "%3dc7(%d)", D_801FC8E8->items[i].count, D_801FC8E8->items[i].max);
        drawText(x - 8, y + 50, (s32)text,
                 D_801FC8E8->items[i].count == D_801FC8E8->items[i].max || price == -1 ? 2 : 7, window->z);
    }
    KAW_drawCursor(D_801F353C);
    if (D_801FC8E8->top != 0) {
        drawScrollArrow(0x10E, 0x39, 4, 5, window->z);
    }
    if (D_801FC8E8->top + 10 < D_801FC8E8->count) {
        drawScrollArrow(0x10E, 0xAA, 6, 5, window->z);
    }
    type = D_801FC8E8->items[D_801FC8E8->cursor].type;
    if (type == 0) {
        card = &SUB_DIGIMON_CARDS[D_801FC8E8->items[D_801FC8E8->cursor].index];
        /* Ｎｏ %d */
        sprintf(text, "\x82m\x82\x8F %d", D_801FC8E8->items[D_801FC8E8->cursor].index + 1);
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
        sprintf(text, "a%d%s", card->attr >> 4, card->name);
        drawIconText(0x4E, 0xB8, 7, 1, window->z, (s32)text);
        /* Ｌｖe%d */
        sprintf(text, "\x82k\x82\x96" "e%d", (card->attr & 0xF) + 3);
        drawIconText(0xBA, 0xB8, 7, 1, window->z, (s32)text);
        /* ＨＰ%s */
        sprintf(text, "\x82g\x82o%s", formatSjisNumber(card->hp, 4, number));
        drawIconText(0xEA, 0xB8, 7, 1, window->z, (s32)text);
    } else {
        if (type == 3) {
            index = D_801FC8E8->items[D_801FC8E8->cursor].index;
            pack = &D_8007EAA0[index];
            func_801EB21C(D_801FC8E8->items[D_801FC8E8->cursor].type, index);
            /* %sブースターパック */
            sprintf(text, "%s\x83u\x81[\x83X\x83^\x81[\x83p\x83" "b\x83N", pack->name);
        } else {
            id = func_801EB21C(D_801FC8E8->items[D_801FC8E8->cursor].type, D_801FC8E8->items[D_801FC8E8->cursor].index) + 1;
            if (type == 1) {
                name = SUB_OPTION_CARDS[D_801FC8E8->items[D_801FC8E8->cursor].index].name;
            } else {
                name = SUB_DIGIVOLVE_CARDS[D_801FC8E8->items[D_801FC8E8->cursor].index].name;
            }
            /* Ｎｏ %ds0　s1a5オプションカードs0　s1%s */
            sprintf(text, "\x82m\x82\x8F %ds0\x81@s1a5\x83I\x83v\x83V\x83\x87\x83\x93\x83J\x81[\x83hs0\x81@s1%s", id, name);
        }
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
    }
}

void func_801EE5CC(JpWindow *window) {
    Rect16 pos;
    char text[72];
    char title[72];
    char number[32];
    JpDigimonCard *card;
    JpPackData *pack;
    JpOptionCard *option;
    s32 n;
    s32 price;
    s32 i;
    s32 id;
    u8 level;
    s32 color;
    s32 palette;

    if (D_801F3700->mode != 11) {
        price = 0;
    } else {
        price = SUB_getItemPrice(D_801FC8C8.type, D_801FC8C8.index) + D_801F3700->total;
        if (PROFILE->money < price) {
            price = -1;
        }
    }
    if (D_801FC8C8.type == 0) {
        card = &SUB_DIGIMON_CARDS[D_801FC8C8.index];
        pos.x = 0x79;
        pos.y = 0x32;
        pos.w = 0;
        pos.h = 0;
        /* a%dデジモンカード　Ｌｖ　e%d */
        sprintf(title, "  a%d\x83" "f\x83W\x83\x82\x83\x93\x83J\x81[\x83h\x81@\x82k\x82\x96\x81@e%d", card->attr >> 4,
                (card->attr & 0xF) + 3);
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)title);
        pos.y += 16;
        /* 属性　%s　Ｎｏ%s */
        sprintf(title, "  \x91\xAE\x90\xAB\x81@%s\x81@\x82m\x82\x8F%s", D_801F34D0[SUB_DIGIMON_CARDS[D_801FC8C8.index].attr >> 4],
                formatSjisNumber(D_801FC8C8.index + 1, 3, number));
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)title);
        pos.y += 12;
        sprintf(text, "  %s", SUB_DIGIMON_CARDS[D_801FC8C8.index].name);
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
        /* ＨＰ */
        drawIconText(pos.x + 100, pos.y, 7, 1, window->z, (s32)" \x82g\x82o");
        sprintf(text, " w-1%4d", SUB_DIGIMON_CARDS[D_801FC8C8.index].hp);
        drawText(pos.x + 120, pos.y, (s32)text, 7, window->z);
        pos.y += 12;
        /* 必進Ｐ */
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)" \x95K\x90i\x82o");
        sprintf(text, " w-1%2d", SUB_DIGIMON_CARDS[D_801FC8C8.index].dpCost);
        drawText(pos.x + 48, pos.y, (s32)text, 7, window->z);
        /* ＰＯＷ */
        drawIconText(pos.x + 100, pos.y, 7, 1, window->z, (s32)" \x82o\x82n\x82v");
        sprintf(text, " w-1%4d", SUB_DIGIMON_CARDS[D_801FC8C8.index].dpBonus);
        drawText(pos.x + 120, pos.y, (s32)text, 7, window->z);
        pos.y += 12;
        for (i = 0; i < 3; i++, pos.y += 12) {
            /* b%d%s */
            sprintf(text, " \x81@b%d%s", i, SUB_DIGIMON_CARDS[D_801FC8C8.index].attacks[i].name);
            drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
            sprintf(text, " w-1%4d", SUB_DIGIMON_CARDS[D_801FC8C8.index].attacks[i].power);
            drawText(pos.x + 120, pos.y, (s32)text, 7, window->z);
        }
        level = CROSS_EFFECT_ICONS[card->support];
        if (level != 0) {
            /* %s d%d */
            sprintf(text, "  \x81@%s d%d", CROSS_EFFECT_NAMES[SUB_DIGIMON_CARDS[D_801FC8C8.index].support], level);
        } else {
            /* %s */
            sprintf(text, "  \x81@%s", CROSS_EFFECT_NAMES[SUB_DIGIMON_CARDS[D_801FC8C8.index].support]);
        }
        drawIconText(pos.x + 1, pos.y, 7, 1, window->z, (s32)text);
        pos.y += 16;
        if (card->supportLevel == 0) {
            /* 援護能力 */
            drawIconText(pos.x + 2, pos.y, 6, 1, window->z, (s32)" \x89\x87\x8C\xEC\x94\\\x97\xCD");
        } else {
            /* 援護能力 d%d */
            sprintf(text, "\x89\x87\x8C\xEC\x94\\\x97\xCD d%d", card->supportLevel);
            drawIconText(pos.x + 2, pos.y, 6, 1, window->z, (s32)text);
        }
        pos.y += 12;
        for (i = 0; i < 4; i++, pos.y += 12) {
            sprintf(text, " %s", SUB_DIGIMON_CARDS[D_801FC8C8.index].supportText[i]);
            drawIconText(pos.x - 2, pos.y, 7, 1, window->z, (s32)text);
        }
    } else if (D_801FC8C8.type == 3) {
        func_801EB21C(3, D_801FC8C8.index);
        pack = &D_8007EAA0[D_801FC8C8.index];
        pos.x = 0x7D;
        pos.y = 0x36;
        pos.w = 0;
        pos.h = 0;
        /* %sブースターパック */
        sprintf(text, " %s\x83u\x81[\x83X\x83^\x81[\x83p\x83" "b\x83N", pack->name);
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
        pos.y += 16;
        /* ８枚入り */
        drawIconText(pos.x + 12, pos.y, 7, 1, window->z, (s32)" \x82W\x96\x87\x93\xFC\x82\xE8");
        pos.y += 16;
        switch (D_801FC8C8.index) {
        case 0:
            drawIconText(pos.x, pos.y, 7, 1, window->z,
                         /* 必ず４枚以上の火炎デジモン */
                         (s32)" \x95K\x82\xB8\x82S\x96\x87\x88\xC8\x8F\xE3\x82\xCC\x89\xCE\x89\x8A\x83" "f\x83W\x83\x82\x83\x93");
            break;
        case 1:
            drawIconText(pos.x, pos.y, 7, 1, window->z,
                         /* 必ず４枚以上の氷水デジモン */
                         (s32)" \x95K\x82\xB8\x82S\x96\x87\x88\xC8\x8F\xE3\x82\xCC\x95X\x90\x85\x83" "f\x83W\x83\x82\x83\x93");
            break;
        case 2:
            drawIconText(pos.x, pos.y, 7, 1, window->z,
                         /* 必ず４枚以上の自然デジモン */
                         (s32)" \x95K\x82\xB8\x82S\x96\x87\x88\xC8\x8F\xE3\x82\xCC\x8E\xA9\x91R\x83" "f\x83W\x83\x82\x83\x93");
            break;
        case 3:
            drawIconText(pos.x, pos.y, 7, 1, window->z,
                         /* 必ず４枚以上の暗黒デジモン */
                         (s32)" \x95K\x82\xB8\x82S\x96\x87\x88\xC8\x8F\xE3\x82\xCC\x88\xC3\x8D\x95\x83" "f\x83W\x83\x82\x83\x93");
            break;
        case 4:
            drawIconText(pos.x, pos.y, 7, 1, window->z,
                         /* 必ず４枚以上の珍種デジモン */
                         (s32)" \x95K\x82\xB8\x82S\x96\x87\x88\xC8\x8F\xE3\x82\xCC\x92\xBF\x8E\xED\x83" "f\x83W\x83\x82\x83\x93");
            break;
        case 5:
            drawIconText(pos.x, pos.y, 7, 1, window->z,
                         /* 属性とオプションの割合は */
                         (s32)" \x91\xAE\x90\xAB\x82\xC6\x83I\x83v\x83V\x83\x87\x83\x93\x82\xCC\x8A\x84\x8D\x87\x82\xCD");
            drawIconText(pos.x, pos.y + 12, 7, 1, window->z,
                         /* 完全にランダムです。 */
                         (s32)" \x8A\xAE\x91S\x82\xC9\x83\x89\x83\x93\x83_\x83\x80\x82\xC5\x82\xB7\x81" "B");
            break;
        }
        if (D_801FC8C8.index != 5) {
            drawIconText(pos.x, pos.y + 12, 7, 1, window->z,
                         /* が入っています。 */
                         (s32)" \x82\xAA\x93\xFC\x82\xC1\x82\xC4\x82\xA2\x82\xDC\x82\xB7\x81" "B");
        }
    } else {
        id = func_801EB21C(D_801FC8C8.type, D_801FC8C8.index);
        palette = D_801FC8C8.type + 4;
        pos.x = 0x7D;
        pos.y = 0x36;
        pos.w = 0;
        pos.h = 0;
        if (D_801FC8C8.type == 1 && (option = &SUB_OPTION_CARDS[D_801FC8C8.index])->unk86 != 0) {
            /* a%dオプションカード d%d */
            sprintf(text, " a%d\x83I\x83v\x83V\x83\x87\x83\x93\x83J\x81[\x83h d%d", palette, option->unk86);
        } else {
            /* a%dオプションカード */
            sprintf(text, " a%d\x83I\x83v\x83V\x83\x87\x83\x93\x83J\x81[\x83h", palette);
        }
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
        pos.y += 16;
        /* %s　　Ｎｏ%s */
        sprintf(text, "   %s\x81@\x81@\x82m\x82\x8F%s", D_801F34FC[D_801FC8C8.type - 1], formatSjisNumber(id + 1, 3, number));
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
        pos.y += 16;
        drawIconText(pos.x, pos.y, 7, 1, window->z,
                     D_801FC8C8.type == 1 ? (s32)SUB_OPTION_CARDS[D_801FC8C8.index].name : (s32)SUB_DIGIVOLVE_CARDS[D_801FC8C8.index].name);
        pos.y += 24;
        for (i = 0; i < 4; i++, pos.y += 12) {
            drawIconText(pos.x + 24, pos.y, 7, 1, window->z,
                         D_801FC8C8.type == 1 ? (s32)SUB_OPTION_CARDS[D_801FC8C8.index].text[i] : (s32)SUB_DIGIVOLVE_CARDS[D_801FC8C8.index].text[i]);
        }
    }
    if (D_801F3700->paid == 0) {
        if (D_801F3700->mode == 11) {
            /* 購入枚数 */
            drawIconText(0xEC, 0xA6, 7, 1, window->z, (s32)"\x8Dw\x93\xFC\x96\x87\x90\x94");
            sprintf(text, "b%d", D_801FC8C8.count);
            drawText(0xF1, 0xBA, (s32)text, D_801FC8C8.count < D_801FC8C8.max ? price == -1 ? 2 : 8 : 2, window->z);
            sprintf(text, "c7(%d)", D_801FC8C8.max);
            drawText(0x104, 0xC6, (s32)text, D_801FC8C8.count >= D_801FC8C8.max || price == -1 ? 2 : 8, window->z);
        } else {
            /* 売却枚数 */
            drawIconText(0xEC, 0xA6, 7, 1, window->z, (s32)"\x94\x84\x8Bp\x96\x87\x90\x94");
            sprintf(text, "b%d", D_801FC8C8.count);
            drawText(0xF1, 0xBA, (s32)text, D_801FC8C8.count < D_801FC8C8.max ? price == -1 ? 2 : 8 : 2, window->z);
            sprintf(text, "c7(%d)", D_801FC8C8.max);
            drawText(0x104, 0xC6, (s32)text, D_801FC8C8.count >= D_801FC8C8.max || price == -1 ? 2 : 8, window->z);
        }
        pos.x = 0xEC;
        pos.y = 0xA4;
        pos.w = 0x30;
        pos.h = 0x31;
        drawWindowFrame(&pos, 0, 0, 1, 0xFF, window->unk2 + 0x36, window->z);
    }
}

void func_801EF51C(JpWindow *window) {
    char text[136];
    char number[16];
    s32 total;

    total = D_801F3700->total;
    if (D_801F3700->mode == 13) {
        total /= 5;
    }
    formatSjisNumber(total, 6, number);
    /* 合計 s0w-4%s d0 */
    sprintf(text, " \x8D\x87\x8Cv s0w-4%s d0", number);
    drawIconText(0x1A, 0x17, 7, 1, window->z, (s32)text);
}

void func_801EF5C8(JpWindow *window) {
    char text[136];
    char number[16];
    s8 i;

    for (i = 0; i < 64; i++) {
        text[i] = 0;
    }
    for (i = 0; i < 13; i++) {
        number[i] = 0;
    }
    formatSjisNumber(PROFILE->money, 6, number);
    /* 所持金 s0w-4%s d0 */
    sprintf(text, " \x8F\x8A\x8E\x9D\x8B\xE0 s0w-4%s d0", number);
    drawIconText(0xAA, 0x17, 7, 1, window->z, (s32)text);
}

void func_801EF6B0(JpWindow *window) {
    char text[136];
    char number[16];
    ShopItem *items = D_801F3700->list.items;
    s32 cur = D_801F3700->list.cursor;
    s32 price = PROFILE->unk14;

    if (D_801FC8E8->count > 0) {
        switch (items[cur].type) {
        case 0:
            price = SUB_DIGIMON_CARDS[items[cur].index].price;
            break;
        case 1:
            price = SUB_OPTION_CARDS[items[cur].index].price;
            break;
        case 2:
            price = SUB_DIGIVOLVE_CARDS[items[cur].index].price;
            break;
        case 3:
            price = D_8007EAA0[items[cur].index].price;
            break;
        }
        price *= 100;
    } else {
        price = 0;
    }
    if (D_801F3700->mode == 13) {
        formatSjisNumber(price / 5, 4, number);
        /* b0見る b2戻る b1売る Ｌ１ − Ｒ１ ＋ 売値 s0w-4%s d0 */
        sprintf(text, "b0\x8C\xA9\x82\xE9 b2\x96\xDF\x82\xE9 b1\x94\x84\x82\xE9 \x82k\x82P \x81| \x82q\x82P \x81{ \x94\x84\x92l s0w-4%s d0", number);
    } else {
        formatSjisNumber(price, 4, number);
        /* b0見る b2戻る b1買う Ｌ１ − Ｒ１ ＋ 買値 s0w-4%s d0 */
        sprintf(text, "b0\x8C\xA9\x82\xE9 b2\x96\xDF\x82\xE9 b1\x94\x83\x82\xA4 \x82k\x82P \x81| \x82q\x82P \x81{ \x94\x83\x92l s0w-4%s d0", number);
    }
    drawIconText(0x1A, 0xD1, 7, 1, window->z, (s32)text);
}

void func_801EF8F8(JpWindow *window) {
    char text[152];

    /* b0見る b2戻る */
    sprintf(text, " b0\x8C\xA9\x82\xE9 b2\x96\xDF\x82\xE9 ");
    drawIconText(0x1A, 0xD1, 7, 1, window->z, (s32)text);
}

void func_801EF950(JpWindow *window) {
    char text[32];
    char number[24];
    Rect16 rect = { 0, 0xF0, 0x18, 9 };
    JpDigimonCard *card;
    char *name;
    s32 id;
    s32 i;
    s32 y;
    s32 x;
    s32 n;
    s32 type;

    for (n = 0, i = D_801FC8E8->top; n < 10; n++, i++) {
        if (i >= D_801FC8E8->count) {
            break;
        }
        x = (n % 5) * 50 + 0x1A;
        y = (n / 5) * 64 + 0x38;
        if (D_801FC8E8->cursor == i) {
            D_801FC8E8->items[i].brightness = 0x80;
            D_801F353C->x = x + 20;
            D_801F353C->y = y + 24;
        } else if (D_801FC8E8->items[i].brightness > 0x40) {
            if ((D_801FC8E8->items[i].brightness -= 0x10) < 0x40) {
                D_801FC8E8->items[i].brightness = 0x40;
            }
        }
        if (D_801FC8E8->items[i].owned == 0) {
            drawTexturedSprite(x + 14, y + 32, &rect, 0x17, 0x7E5C, window->z, D_801FC8E8->items[i].brightness, -1);
        }
        func_801EDDA0(D_801FC8E8->items[i].icon, x, y, window->z, D_801FC8E8->items[i].brightness);
        sprintf(text, "%3dc7(%d)", D_801FC8E8->items[i].count, D_801FC8E8->items[i].max);
        drawText(x - 8, y + 50, (s32)text, D_801FC8E8->items[i].max >= 8 ? 2 : 7, window->z);
    }
    KAW_drawCursor(D_801F353C);
    if (D_801FC8E8->top != 0) {
        drawScrollArrow(0x10E, 0x39, 4, 5, window->z);
    }
    if (D_801FC8E8->top + 10 < D_801FC8E8->count) {
        drawScrollArrow(0x10E, 0xAA, 6, 5, window->z);
    }
    type = D_801FC8E8->items[D_801FC8E8->cursor].type;
    if (type == 0) {
        card = &SUB_DIGIMON_CARDS[D_801FC8E8->items[D_801FC8E8->cursor].index];
        /* Ｎｏ %d */
        sprintf(text, "\x82m\x82\x8F %d", D_801FC8E8->items[D_801FC8E8->cursor].index + 1);
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
        sprintf(text, "a%d%s", card->attr >> 4, card->name);
        drawIconText(0x4E, 0xB8, 7, 1, window->z, (s32)text);
        /* Ｌｖe%d */
        sprintf(text, "\x82k\x82\x96" "e%d", (card->attr & 0xF) + 3);
        drawIconText(0xBA, 0xB8, 7, 1, window->z, (s32)text);
        /* ＨＰ%s */
        sprintf(text, "\x82g\x82o%s", formatSjisNumber(card->hp, 4, number));
        drawIconText(0xEA, 0xB8, 7, 1, window->z, (s32)text);
    } else {
        id = func_801EB21C(D_801FC8E8->items[D_801FC8E8->cursor].type, D_801FC8E8->items[D_801FC8E8->cursor].index) + 1;
        if (type == 1) {
            name = SUB_OPTION_CARDS[D_801FC8E8->items[D_801FC8E8->cursor].index].name;
        } else {
            name = SUB_DIGIVOLVE_CARDS[D_801FC8E8->items[D_801FC8E8->cursor].index].name;
        }
        /* Ｎｏ %ds0　s1a5オプションカードs0　s1%s */
        sprintf(text, "\x82m\x82\x8F %ds0\x81@s1a5\x83I\x83v\x83V\x83\x87\x83\x93\x83J\x81[\x83hs0\x81@s1%s", id, name);
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
    }
}

void func_801EFF14(JpWindow *window) {
    /* 購入カード */
    char text[13] = " \x8Dw\x93\xFC\x83J\x81[\x83h ";

    drawIconText(0x38, 0x17, 7, 1, window->z, (s32)text);
}

void func_801EFF98(void) {
    u32 *tims;

    spawnTask(0, -1, 0, 0x800, loadFile, "C:\\OBJECT\\shop.TIM", getCurrentTaskId());
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    if (tims != NULL) {
        uploadTimList(tims);
        freeHeapBlock(tims);
    }
}

void func_801F0010(s32 delta) {
    Rect16 rect = { 0x3D5, 0xF4, 0, 0 };
    char text[8];

    formatSjisNumber(D_801FC8C8.deckCount += delta, 2, text);
    uploadKanjiString(text, &rect);
}

void func_801F009C(s32 deck) {
    char text[48];
    Rect16 rect;
    JpDeck *jpDeck = &PROFILE->decks[deck];

    fillVramRect(0x3C0, 0xF4, 0x9C, 0xC, 0);
    rect.x = 0x3C0;
    rect.y = 0xF4;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString(jpDeck->unk0 + 1, &rect);
    rect.x = 0x3D5;
    /* ／３０枚 */
    sprintf(text, "\x81@\x81@\x81^\x82R\x82O\x96\x87");
    uploadKanjiString(text, &rect);
    func_801F0010(0);
}

/* the card numbers each booster pack holds: count of them from base + start
   (func_801F2570); packs 5 and 6 hold the option cards */
PackRange D_801F32D0[7] = {
    { 0, 3, 8, 9 },  { 20, 3, 8, 11 }, { 42, 3, 12, 12 }, { 69, 3, 9, 10 },
    { 91, 3, 6, 8 }, { 0, 19, 7, 9 },  { 0, 1, 3, 1 },
};

JpWindowDesc D_801F3340 = { 290, 48, 0, 12, 18, 48, 272, 148, 10, 1, func_801EDEF4, func_801EA908 };
JpWindowDesc D_801F3360 = { 290, 48, 0, 12, 122, 48, 168, 170, 10, 1, func_801EE5CC, func_801EA908 };
JpWindowDesc D_801F3380 = { 26, 23, 0, 12, 26, 23, 116, 12, 10, 0, func_801EF51C, func_801EA908 };
JpWindowDesc D_801F33A0 = { 290, 23, 0, 12, 170, 23, 120, 12, 10, 1, func_801EF5C8, func_801EA908 };
JpWindowDesc D_801F33C0 = { 26, 208, 0, 12, 26, 208, 272, 12, 10, 0, func_801EF6B0, func_801EA908 };
JpWindowDesc D_801F33E0 = { 26, 208, 0, 12, 26, 208, 116, 12, 10, 0, func_801EF8F8, func_801EA908 };
JpWindowDesc D_801F3400 = { 290, 48, 0, 12, 18, 48, 272, 148, 10, 1, func_801EF950, func_801EA908 };
JpWindowDesc D_801F3420 = { 26, 23, 0, 12, 26, 23, 116, 12, 10, 0, func_801EFF14, func_801EA908 };
JpWindowDesc D_801F3440 = { 26, 48, 0, 12, 26, 48, 84, 170, 10, 0, func_801ED84C, func_801EA908 };
JpWindowDesc D_801F3460 = { 26, 23, 0, 12, 26, 23, 272, 12, 10, 0, func_801EA918, func_801EA910 };
JpWindowDesc D_801F3480 = { 26, 208, 0, 12, 26, 208, 272, 12, 10, 0, func_801EA970, func_801EA910 };

/* per option card: the group func_801EC074 lists it in */
s8 D_801F34A0[46] = {
    0, 1, 1, 2, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 2,
    1, 1, 2, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 2, 1, 0, 0, -1,
};

/* the specialties */
char *D_801F34D0[6] = {
    "\x89\xCE\x89\x8A", /* 火炎 */
    "\x95X\x90\x85",    /* 氷水 */
    "\x8E\xA9\x91R",    /* 自然 */
    "\x88\xC3\x8D\x95", /* 暗黒 */
    "\x92\xBF\x8E\xED", /* 珍種 */
    "\x95\x81\x92\xCA", /* 普通 */
};

/* not referenced by any code */
char *D_801F34E8[5] = {
    "\x90" "F\x81" "A\x95K\x97v\x90i\x89\xBB\x83|\x83" "C\x83\x93\x83g\x96\xB3\x8E\x8B", /* 色、必要進化ポイント無視 */
    "\x82Q\x92i\x8AK\x90i\x89\xBB",                                                  /* ２段階進化 */
    "\x90i\x89\xBB\x83|\x83" "C\x83\x93\x83g\x81{\x82R\x82O",                         /* 進化ポイント＋３０ */
    "\x93\xAF\x90\xA2\x91\xE3\x82\xCC\x8C\xF0\x8A\xB7",                               /* 同世代の交換 */
    "\x82P\x92i\x8AK\x91\xDE\x89\xBB",                                                /* １段階退化 */
};

/* the option cards' kinds */
char *D_801F34FC[2] = {
    "\x90\xED\x93\xAC\x97p", /* 戦闘用 */
    "\x90i\x89\xBB\x97p",    /* 進化用 */
};

/* SUB_shopRandom's seed */
s16 D_801F3504 = 31416;

void func_801F0154(FrameBuffer *fb, s32 index) {
    s32 other = index ^ 1;

    fillVramRect(D_801F3648[other].clip[0], D_801F3648[other].clip[1], 0x180, 0x80, 0);
    AddPrim((s32 *)&fb->ot[0xFFF], (s32)&D_801F3548[other]);
    SetDrawEnv(&D_801F35C8[other], &fb->draw);
    AddPrim((s32 *)&fb->ot[300], (s32)&D_801F35C8[other]);
}

void func_801F01F4(s32 modelId) {
    SetDefDrawEnv(&D_801F3648[0], 0x140, 0x100, 0x60, 0x80);
    SetDefDrawEnv(&D_801F3648[1], 0x140, 0x180, 0x60, 0x80);
    D_801F3648[0].ofs[0] -= 0x70;
    D_801F3648[1].ofs[0] -= 0x70;
    D_801F3648[0].ofs[1] -= 0x14;
    D_801F3648[1].ofs[1] -= 0x14;
    SetDrawEnv(&D_801F3548[0], &D_801F3648[0]);
    SetDrawEnv(&D_801F3548[1], &D_801F3648[1]);
    initScene3D(1);
    addFrameCallback((s32)func_801F0154);
    createWireGrid(500, 500, 11, 11, 0, 0);
    loadOmdModelFromDisc(0, modelId, -1);
    /* jp reads the result loadModelAnimation leaves in v0 */
    if (((s32 (*)(s32, s32, s32))loadModelAnimationFile)(0, 0, 0) != 0) {
        startModelAnimation(0, 0, -2, 0);
    }
    SCENE_3D->modelState[0] = 1;
    spawnTask(0x19, -1, 0, 0x800, runSceneCameraTask, 2);
    spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
}

void func_801F038C(void) {
    if (D_801FC8C8.type == 0) {
        endTask(0x19);
        endTask(0x1B);
        removeFrameCallback((s32)func_801F0154);
        removeFrameCallback((s32)renderSceneModels);
        removeFrameCallback((s32)renderWireGrid);
        waitFrames(4);
        unloadAllModels();
        freeHeapBlocksByTag(0x7F);
    }
}

void func_801F0404(void) {
    ShopList list = *D_801FC8E8;
    s32 i = 0;
    s32 bit = 0;

    for (; i < list.count; i++) {
        if (list.items[i].type != 3) {
            break;
        }
    }
    for (bit = 0; bit < 8; bit++) {
        if (!((PROFILE->shops[D_801F3700->shopId].soldBits >> bit) & 1)) {
            if (list.items[i].count != 0) {
                PROFILE->shops[D_801F3700->shopId].soldBits |= 1 << bit;
                list.items[i].count--;
                list.items[i].max--;
                if (list.items[i].max == 0) {
                    if (++i >= list.count) {
                        return;
                    }
                }
            } else if (--list.items[i].max == 0) {
                if (++i >= list.count) {
                    return;
                }
            }
        }
    }
}

s32 SUB_getOwnedCount(s8 type, s8 index) {
    switch (type) {
    case 0:
        return PROFILE->digimonCards[index] & 0x7F;
    case 1:
        return PROFILE->optionCards[index] & 0x7F;
    case 2:
        return PROFILE->digivolveCards[index] & 0x7F;
    }
    return 100;
}

u8 SUB_getCopiesOverLimit(s8 type, s8 index, s32 count) {
    u8 excess;

    count += SUB_getOwnedCount(type, index);
    excess = count;
    if ((u8)count >= 9) {
        return excess - 8;
    }
    return 0;
}

void func_801F0670(s8 selling) {
    s32 i;
    u8 excess;

    for (i = 0; i < D_801FC8E8->count; i++) {
        if (selling == 0) {
            excess = SUB_getCopiesOverLimit(D_801FC8E8->items[i].type, D_801FC8E8->items[i].index, D_801FC8E8->items[i].count);
            if (excess != 0) {
                D_8007E5F8->slot->save->unk30 += SUB_getItemPrice(D_801FC8E8->items[i].type, D_801FC8E8->items[i].index) * excess / 5;
            }
            if (func_80043374(D_801FC8E8->items[i].type, D_801FC8E8->items[i].index, D_801FC8E8->items[i].count) < 0) {
                D_8007E5F8->slot->save->unk4 = -1;
            }
        } else {
            func_80043740(D_801FC8E8->items[i].type, D_801FC8E8->items[i].index, D_801FC8E8->items[i].count);
        }
        if ((i & 3) == 0) {
            waitFrames(1);
        }
    }
}

void func_801F0824(s8 mode) {
    setBackgroundScrollMode(1);
    switch (mode) {
    case 0:
        D_801F3518[0]->state = 4;
        D_801F3518[1]->state = 4;
        D_801F3518[3]->state = 4;
        D_801F3518[4]->state = 4;
        break;
    case 1:
        D_801F3518[5]->state = 4;
        D_801F3518[6]->state = 4;
        D_801F3518[7]->state = 4;
        D_801F3518[4]->state = 4;
        break;
    }
    waitFrames(90);
    func_80044758(D_801F353C);
    D_801F3780 = 0;
    closeKanjiPage(0xF);
    freeHeapBlocksByTag(0x195);
    freeHeapBlocksByTag(0x193);
    exitTask();
}

s32 SUB_getItemPrice(s8 type, s16 index) {
    s32 price;

    switch (type) {
    case 0:
        price = SUB_DIGIMON_CARDS[index].price;
        break;
    case 1:
        price = SUB_OPTION_CARDS[index].price;
        break;
    case 2:
        price = SUB_DIGIVOLVE_CARDS[index].price;
        break;
    case 3:
        price = D_8007EAA0[index].price;
        break;
    default:
        price = -1;
        break;
    }
    return price * 100;
}

s32 SUB_adjustItemCount(s32 more, s32 less, s8 *count, s8 max) {
    s32 price = SUB_getItemPrice(D_801FC8E8->items[D_801FC8E8->cursor].type, D_801FC8E8->items[D_801FC8E8->cursor].index);
    s32 sound;

    if (PAD_STATES[0]->rawRepeat & more) {
        if (D_801F3700->mode == 13) {
            if (++*count > max) {
                *count = max;
                return 1;
            }
        } else {
            if (PROFILE->money < price + D_801F3700->total) {
                return 1;
            }
            if (++*count > max) {
                *count = max;
                return 1;
            }
        }
        D_801F3700->total += price;
        sound = 0;
    } else if (PAD_STATES[0]->rawRepeat & less) {
        if (--*count < 0) {
            *count = 0;
            return 1;
        }
        D_801F3700->total -= price;
        sound = 1;
    } else {
        return 0;
    }
    playSoundEffect(sound);
    return 1;
}

void func_801F0BF0(void) {
    char path[64];
    u32 *tim;
    s32 id = func_801EB21C(D_801FC8C8.type, D_801FC8C8.index);
    s32 i;

    D_801F3700->flip = 0;
    D_801F3700->flipBack = 0;
    D_801F3700->flipTimer = 60;
    for (i = 0; i < 3; i++) {
        if (D_801F3700->imageSlot->id != id) {
            if (D_801F3700->imageSlot != &D_801F3700->imageSlots[3]) {
                D_801F3700->imageSlot++;
            } else {
                D_801F3700->imageSlot = D_801F3700->imageSlots;
            }
        } else {
            D_801F3700->loading = 1;
        }
    }
    if (D_801F3700->loading == 0) {
        D_801F3700->imageSlot->id = id;
        D_801F3700->loading = 1;
        sprintf(path, "B:\\L_CARD\\LC%3.3d.TIM", id);
        spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
        tim = (u32 *)waitFrames(0x7FFFFFFF);
        uploadTim(tim, D_801F3700->imageSlot->x, D_801F3700->imageSlot->y, D_801F3700->imageSlot->clutX,
                  D_801F3700->imageSlot->clutY);
        DrawSync(0);
        freeHeapBlock(tim);
    }
    D_801F3700->loading = 0;
}

void func_801F0D8C(s32 arg) {
    clearKanjiPage(0xF);
    closeKanjiPage(0xF);
    openKanjiPage(0x15, 0x1B9);
    clearKanjiPage(0x15);
    func_801EA9C8();
    SUB_viewDigimonModel(SUB_DIGIMON_CARDS[D_801FC8C8.index].unkD3);
    func_801EAA74();
    D_801F3518[1] = NULL;
    D_801F3518[7] = NULL;
    D_801F3518[4] = NULL;
    D_801F3518[3] = NULL;
    clearKanjiPage(0x15);
    closeKanjiPage(0x15);
    openKanjiPage(0xF, 0x1B9);
    clearKanjiPage(0xF);
    spawnTask(0, -1, 0, 0x1000, D_801F3700->fromOpened == 0 ? func_801F0EA0 : func_801F1CCC, arg);
    exitTask();
}

void func_801F0EA0(s32 parent) {
    s32 count = D_801FC8C8.count;
    s32 viewing;

    spawnTask(0, -1, 0, 0x1000, func_801F0BF0, parent);
    D_801F3700->fromOpened = 0;
    if (D_801F3518[3] == NULL) {
        spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3380, getCurrentTaskId());
        D_801F3518[3] = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
    if (D_801F3518[4] == NULL) {
        spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F33A0, getCurrentTaskId());
        D_801F3518[4] = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3360, getCurrentTaskId());
    D_801F3518[2] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3440, getCurrentTaskId());
    D_801F3518[0] = (JpWindow *)waitFrames(0x7FFFFFFF);
    waitFrames(90);
    viewing = 0;
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (D_801F3700->loading != 0) {
            continue;
        }
        if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
            func_801F0010(count - D_801FC8C8.count);
            D_801F3700->total = D_801F3700->savedTotal;
            goto close;
        }
        if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
            D_801FC8E8->items[D_801FC8E8->cursor].count = D_801FC8C8.count;
            goto close;
        }
        if ((PAD_STATES[0]->rawPressed & PAD_TRIANGLE) && D_801FC8C8.type == 0) {
            viewing = 1;
            spawnTask(0, -1, 0, 0x1000, func_801F0D8C, count - D_801FC8C8.count, getCurrentTaskId(), 0, 0);
            goto close;
        }
        if (D_801F3700->mode == 11 || D_801F3700->mode == 13) {
            SUB_adjustItemCount(PAD_RIGHT | PAD_R1, PAD_LEFT | PAD_L1, &D_801FC8C8.count, D_801FC8C8.max);
        }
    }
close:
    D_801F3518[2]->state = 4;
    D_801F3518[0]->state = 4;
    if (viewing == 1) {
        D_801F3518[3]->state = 4;
        D_801F3518[4]->state = 4;
    }
    waitFrames(30);
    waitFrames(30);
    if (viewing == 1) {
        exitTask();
    } else {
        spawnTask(0, -1, 0, 0x1000, func_801F127C, parent);
        exitTask();
    }
}

void func_801F127C(s32 parent) {
    ShopList *list;
    s16 action;
    s32 i;
    u16 pressed;
    s32 count;
    s32 prev;
    s32 rowStart;
    s32 rowEnd;

    if (D_801FC8E8 == NULL) {
        list = func_801ED598(D_801FC8C8.deck, D_801F3700->mode, -1);
        if (D_801F3700->itemChosen == 0) {
            D_801F3700->itemChosen = 1;
            D_801FC8C8.type = list->items[0].type;
            D_801FC8C8.index = list->items[0].index;
        }
    } else {
        list = D_801FC8E8;
    }
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3340, getCurrentTaskId());
    D_801F3518[0] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F33C0, getCurrentTaskId());
    D_801F3518[1] = (JpWindow *)waitFrames(0x7FFFFFFF);
    if (list->count >= D_801F3538) {
        list->cursor = D_801F3538;
        list->top = D_801F3539;
    }
    D_801F3700->list = *D_801FC8E8;
    waitFrames(60);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
            D_8007E5F8->slot->save->unk4 = 0;
            func_801F0824(0);
        }
        pressed = PAD_STATES[0]->rawPressed;
        if (pressed & (PAD_CIRCLE | PAD_TRIANGLE)) {
            if (pressed & PAD_CIRCLE) {
                action = 1;
            } else {
                action = 2;
            }
        } else {
            action = 0;
        }
        if (action == 2) {
            action = 0;
            for (i = 0; i < D_801FC8E8->count; i++) {
                if (D_801FC8E8->items[i].count != 0) {
                    action = 2;
                    goto found;
                }
            }
        found:
        }
        if (D_801FC8E8->count == 0) {
            action = 0;
        }
        D_801F3700->list = *D_801FC8E8;
        if (action != 0) {
            if (action != 1 && D_801F3700->mode != 13) {
                D_801F3518[3]->state = 4;
                D_801F3518[0]->state = 4;
                D_801F3518[1]->state = 4;
            } else if (action == 1) {
                D_801F3518[0]->state = 4;
                D_801F3518[1]->state = 4;
            }
            waitFrames(30);
            waitFrames(30);
            D_801FC8C8.type = D_801FC8E8->items[D_801FC8E8->cursor].type;
            D_801FC8C8.index = D_801FC8E8->items[D_801FC8E8->cursor].index;
            D_801FC8C8.count = D_801FC8E8->items[D_801FC8E8->cursor].count;
            D_801FC8C8.max = D_801FC8E8->items[D_801FC8E8->cursor].max;
            D_801FC8C8.owned = D_801FC8E8->items[D_801FC8E8->cursor].owned;
            D_801F3538 = list->cursor;
            D_801F3539 = list->top;
            if (action == 1) {
                D_801F3700->savedTotal = D_801F3700->total;
                spawnTask(0, -1, 0, 0x1000, D_801FC8E8->count != 0 ? func_801F0EA0 : NULL, parent);
            } else if (D_801F3700->mode == 13) {
                D_8007E5F8->slot->save->unk4 = 1;
                D_8007E5F8->slot->save->unk30 = D_801F3700->total / 5;
                func_801F0670(1);
                func_801F0824(0);
            } else {
                func_801F0404();
                D_801F3700->list = *D_801FC8E8;
                D_801F3700->paid = 1;
                D_8007E5F8->slot->save->unk4 = 1;
                D_8007E5F8->slot->save->unk30 = -D_801F3700->total;
                spawnTask(0, -1, 0, 0x1000, D_801FC8E8->count != 0 ? func_801F1FC4 : NULL, parent, getCurrentTaskId(), 0, 0);
                freeHeapBlocksByTag(0x195);
                D_801FC8E8 = NULL;
                D_801F3538 = D_801F3539 = 0;
            }
            clearKanjiPage(0xF);
            exitTask();
        }
        count = D_801FC8E8->count;
        if (count != 0) {
            prev = D_801FC8E8->cursor;
            rowStart = (prev / 5) * 5;
            rowEnd = rowStart + 5;
            if (count < rowEnd) {
                rowEnd = count;
            }
            if ((PAD_STATES[0]->rawRepeat & PAD_RIGHT) && ++D_801FC8E8->cursor >= rowEnd) {
                D_801FC8E8->cursor = rowStart;
            } else if ((PAD_STATES[0]->rawRepeat & PAD_LEFT) && --D_801FC8E8->cursor < rowStart) {
                D_801FC8E8->cursor = rowEnd - 1;
            } else if ((PAD_STATES[0]->rawRepeat & PAD_DOWN) && (D_801FC8E8->cursor += 5) >= D_801FC8E8->count) {
                if (D_801FC8E8->cursor < (D_801FC8E8->count - 1) / 5 * 5 + 5) {
                    D_801FC8E8->cursor = D_801FC8E8->count - 1;
                } else {
                    D_801FC8E8->cursor -= 5;
                }
            } else if (PAD_STATES[0]->rawRepeat & PAD_UP) {
                if (D_801FC8E8->cursor - 5 >= 0) {
                    D_801FC8E8->cursor -= 5;
                }
            }
            if (D_801FC8E8->top > D_801FC8E8->cursor) {
                D_801FC8E8->top -= 5;
            }
            if (D_801FC8E8->top + 9 < D_801FC8E8->cursor) {
                D_801FC8E8->top += 5;
            }
            if (D_801FC8E8->cursor != prev) {
                playSoundEffect(2);
            }
        }
        /* Dead store: action is recomputed at the top of the loop. gcc drops the
           store but keeps the start of the test (the original's field is unknown). */
        if (D_801FC8E8->items[D_801FC8E8->cursor].count == 0) {
            action = 0;
        }
        SUB_adjustItemCount(PAD_R1, PAD_L1, &D_801FC8E8->items[D_801FC8E8->cursor].count, D_801FC8E8->items[D_801FC8E8->cursor].max);
    }
}

void SUB_runShop(s32 unused, s32 parentTask) {
    D_801F3780 = 1;
    D_801F3700->paid = 0;
    D_801F3700->total = 0;
    D_801FC8E8 = NULL;
    D_801FC8C8.deckCount = 30;
    func_801EFF98();
    D_801F3518[7] = NULL;
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3380, getCurrentTaskId());
    D_801F3518[3] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F33A0, getCurrentTaskId());
    D_801F3518[4] = (JpWindow *)waitFrames(0x7FFFFFFF);
    openKanjiPage(0xF, 0x1E3);
    D_801F3538 = D_801F3539 = 0;
    spawnTask(0, -1, 0, 0x1000, func_801F127C, 0, getCurrentTaskId(), 0, 0);
    do {
        waitFrames(1);
    } while (D_801F3780 != 0);
    resumeTask(parentTask);
}

void func_801F1CCC(s32 parent) {
    u16 pressed;
    s32 viewing;

    spawnTask(0, -1, 0, 0x1000, func_801F0BF0, parent);
    D_801F3700->fromOpened = 1;
    clearKanjiPage(0xF);
    if (D_801F3518[7] == NULL) {
        spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3420, getCurrentTaskId());
        D_801F3518[7] = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
    if (D_801F3518[4] == NULL) {
        spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F33A0, getCurrentTaskId());
        D_801F3518[4] = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3360, getCurrentTaskId());
    D_801F3518[2] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3440, getCurrentTaskId());
    D_801F3518[0] = (JpWindow *)waitFrames(0x7FFFFFFF);
    waitFrames(90);
    while (1) {
        viewing = 0;
        waitFrames(FRAME_INTERVAL);
        if (D_801F3700->loading != 0) {
            continue;
        }
        pressed = PAD_STATES[0]->rawPressed;
        if (pressed & (PAD_CIRCLE | PAD_CROSS)) {
            break;
        }
        if ((pressed & PAD_TRIANGLE) && D_801FC8C8.type == 0) {
            viewing = 1;
            spawnTask(0, -1, 0, 0x1000, func_801F0D8C, D_801FC8C8.count, getCurrentTaskId(), 0, 0);
            break;
        }
    }
    D_801F3518[2]->state = 4;
    D_801F3518[0]->state = 4;
    if (viewing == 1) {
        D_801F3518[7]->state = 4;
        D_801F3518[4]->state = 4;
    }
    waitFrames(30);
    waitFrames(30);
    if (viewing != 1) {
        spawnTask(0, -1, 0, 0x1000, func_801F1FC4, parent);
    }
    exitTask();
}

void func_801F1FC4(s32 parent) {
    u8 unused[0x250]; /* unused, but it is in the original stack frame */
    ShopList *list = NULL;
    s32 count;
    s32 prev;
    s32 rowStart;
    s32 rowEnd;

    if (D_801FC8E8 == NULL) {
        list = func_801ED598(D_801FC8C8.deck, 12, -1);
        list->cursor = 0;
    }
    if (D_801F3518[7] == NULL) {
        spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3420, getCurrentTaskId());
        D_801F3518[7] = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F33E0, getCurrentTaskId());
    D_801F3518[5] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &D_801F3400, getCurrentTaskId());
    D_801F3518[6] = (JpWindow *)waitFrames(0x7FFFFFFF);
    if (list->count >= D_801F3538) {
        list->cursor = D_801F3538;
        list->top = D_801F3539;
    }
    waitFrames(60);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
            D_801F3518[5]->state = 4;
            D_801F3518[6]->state = 4;
            waitFrames(30);
            waitFrames(30);
            D_801FC8C8.type = D_801FC8E8->items[D_801FC8E8->cursor].type;
            D_801FC8C8.index = D_801FC8E8->items[D_801FC8E8->cursor].index;
            D_801FC8C8.count = D_801FC8E8->items[D_801FC8E8->cursor].count;
            D_801FC8C8.max = D_801FC8E8->items[D_801FC8E8->cursor].max;
            D_801F3538 = list->cursor;
            D_801F3539 = list->top;
            spawnTask(0, -1, 0, 0x1000, D_801FC8E8->count != 0 ? func_801F1CCC : NULL, parent);
            exitTask();
        } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
            func_801F0670(0);
            func_801F0824(1);
        }
        count = D_801FC8E8->count;
        if (count == 0) {
            continue;
        }
        prev = D_801FC8E8->cursor;
        rowStart = (prev / 5) * 5;
        rowEnd = rowStart + 5;
        if (count < rowEnd) {
            rowEnd = count;
        }
        if (PAD_STATES[0]->rawRepeat & PAD_RIGHT) {
            if (++D_801FC8E8->cursor >= rowEnd) {
                D_801FC8E8->cursor = rowStart;
            }
        } else if (PAD_STATES[0]->rawRepeat & PAD_LEFT) {
            if (--D_801FC8E8->cursor < rowStart) {
                D_801FC8E8->cursor = rowEnd - 1;
            }
        } else if (PAD_STATES[0]->rawRepeat & PAD_DOWN) {
            if ((D_801FC8E8->cursor += 5) >= D_801FC8E8->count) {
                if (D_801FC8E8->cursor < (D_801FC8E8->count - 1) / 5 * 5 + 5) {
                    D_801FC8E8->cursor = D_801FC8E8->count - 1;
                } else {
                    D_801FC8E8->cursor -= 5;
                }
            }
        } else if (PAD_STATES[0]->rawRepeat & PAD_UP) {
            if (D_801FC8E8->cursor - 5 >= 0) {
                D_801FC8E8->cursor -= 5;
            }
        }
        if (prev != D_801FC8E8->cursor) {
            playSoundEffect(2);
        }
        if (D_801FC8E8->top > D_801FC8E8->cursor) {
            D_801FC8E8->top -= 5;
        }
        if (D_801FC8E8->top + 9 < D_801FC8E8->cursor) {
            D_801FC8E8->top += 5;
        }
    }
}

s16 SUB_shopRandom(s16 seed) {
    if (seed >= 0) {
        D_801F3504 = seed;
    }
    return D_801F3504 = D_801F3504 * 3022 % 25763;
}

s16 *func_801F2570(s16 *ids, s8 pack) {
    s32 i;

    for (i = D_801F32D0[pack].start; i < D_801F32D0[pack].start + D_801F32D0[pack].count; i++) {
        if ((u8)(pack - 5) < 2) {
            if (pack == 5) {
                *ids++ = i + 110;
            } else {
                *ids++ = i + 153;
            }
        } else {
            *ids++ = i + D_801F32D0[pack].base;
        }
    }
    return ids;
}

void func_801F2618(void) {
    s8 shop = D_801F3700->shopId;
    s32 i = 0;
    s32 count;

    switch (shop) {
    case 0:
        for (i = 0; i < 5; i++) {
            D_801F3708[i] = (u8)func_801ECA7C(0xFF, 1);
        }
        for (i = 5; i < 7; i++) {
            D_801F3708[i] = (u8)func_801ECA7C(0xFF, 2);
        }
        D_801F3700->packs[5].count = 1;
        break;
    case 1:
        for (i = 0; i < 6; i++) {
            D_801F3700->packs[i].count = 1;
        }
        for (i = 0; i < 4; i++) {
            D_801F3708[i] = (u8)func_801ECA7C(0xFF, 1);
        }
        for (i = 4; i < 6; i++) {
            D_801F3708[i] = (u8)func_801ECA7C(0xFF, 2);
        }
        D_801F3708[i++] = (u8)func_801ECA7C(0xFF, 0);
        break;
    case 2 ... 6:
        D_801F3700->packs[shop - 2].count = 1;
        D_801F3700->packs[5].count = 1;
        for (i = 0; i < 3; i++) {
            D_801F3708[i] = (u8)func_801ECA7C(shop - 2, 1);
        }
        for (i = 3; i < 5; i++) {
            D_801F3708[i] = (u8)func_801ECA7C(shop - 2, 2);
        }
        break;
    }
    count = i;
    if (shop == 0 && PROFILE->shops[0].unk3 == 1) {
        D_801F3700->digimonCards[16].count = 1;
        D_801F3700->digimonCards[36].count = 1;
        D_801F3700->digimonCards[65].count = 1;
        D_801F3700->optionCards[26].count = 1;
        D_801F3700->optionCards[34].count = 1;
        return;
    }
    for (i = 0; i < count; i++) {
        if (D_801F3708[i] < 110) {
            D_801F3700->digimonCards[D_801F3708[i]].count++;
        } else if ((D_801F3708[i] -= 110) < 43) {
            D_801F3700->optionCards[D_801F3708[i]].count++;
        } else if ((D_801F3708[i] -= 43) < 6) {
            D_801F3700->digivolveCards[D_801F3708[i]].count++;
        }
    }
}

void func_801F29BC(void) {
    s16 *ids = D_801F3708;
    u8 shop = D_801F3700->shopId;
    s32 i;
    s16 j;
    s16 swap;
    s16 pack;

    D_801F3700->packs[5].count = 1;
    switch ((s8)shop) {
    case 0:
        for (i = 0; i < 7; i++) {
            ids = func_801F2570(ids, i);
        }
        *ids = -1;
        break;
    case 1:
        for (i = 0; i < 7; i++) {
            ids = func_801F2570(ids, i);
        }
        for (i = 0; i < 6; i++) {
            D_801F3700->packs[i].count = 1;
        }
        *ids = -1;
        break;
    default:
        ids = func_801F2570(ids, shop - 2);
        D_801F3700->packs[(s8)shop - 2].count = 1;
        *ids = -1;
        break;
    }
    D_801F3782 = ids - D_801F3708;
    for (i = 0; i < D_801F3782; i++) {
        j = SUB_shopRandom(-1) % D_801F3782;
        swap = D_801F3708[i];
        D_801F3708[i] = D_801F3708[j];
        D_801F3708[j] = swap;
    }
    if ((s8)shop == 1) {
        pack = SUB_shopRandom(-1) % 6;
        D_801F3708[0] = D_801F32D0[pack].base + SUB_shopRandom(-1) % D_801F32D0[pack].start;
    }
    if (D_801F3700->shopId == 0 && PROFILE->shops[0].unk3 == 1) {
        D_801F3700->digimonCards[16].count = 1;
        D_801F3700->digimonCards[36].count = 1;
        D_801F3700->digimonCards[65].count = 1;
        D_801F3700->optionCards[26].count = 1;
        D_801F3700->optionCards[34].count = 1;
        return;
    }
    for (i = 0; i < 5; i++) {
        if (D_801F3708[i] - 110 < 0) {
            D_801F3700->digimonCards[D_801F3708[i]].count = 1;
        } else if (D_801F3708[i] - 43 < 0) {
            D_801F3700->optionCards[D_801F3708[i] - 110].count = 1;
        } else {
            D_801F3700->digivolveCards[D_801F3708[i] - 153].count = 1;
        }
    }
}

void func_801F2D50(void) {
    s32 i;
    s32 j;
    POLY_FT4 *poly;

    D_801F3700->itemChosen = 0;
    D_801F3700->loading = 0;
    for (i = 0; i < 4; i++) {
        D_801F3700->imageSlots[i].id = -1;
        D_801F3700->imageSlots[i].x = ((i & 1) << 5) + 0x180;
        D_801F3700->imageSlots[i].y = ((i & 2) << 5) + 0x175;
        D_801F3700->imageSlots[i].clutX = 0x180;
        D_801F3700->imageSlots[i].clutY = i + 0xF2;
    }
    for (i = 0; i < 2; i++) {
        DB(i).primSlots[2] = (s32)(D_801F3700->polys[i] = allocHeapBlock(0x78, 0x193));
    }
    for (j = 0; j < 2; j++) {
        poly = D_801F3700->polys[j];
        for (i = 0; i < 3; i++, poly++) {
            SetPolyFT4(poly);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
        }
    }
    D_801F3700->imageSlot = D_801F3700->imageSlots;
    D_801F353C = func_80044334(1, 20, 24, 5, 1);
}

void SUB_openSellShop(void) {
    s32 i;

    D_801F3700 = allocHeapBlock(0xC00, 0x193);
    D_801F3700->mode = 13;
    func_801F2D50();
    for (i = 0; i < 110; i++) {
        D_801F3700->digimonCards[i].stock = PROFILE->digimonCards[i] & 0x7F;
    }
    for (i = 0; i < 43; i++) {
        D_801F3700->optionCards[i].stock = PROFILE->optionCards[i] & 0x7F;
    }
    for (i = 0; i < 6; i++) {
        D_801F3700->digivolveCards[i].stock = PROFILE->digivolveCards[i] & 0x7F;
    }
}

void SUB_openBuyShop(void) {
    s8 places[6] = { 0, 5, 9, 4, 10, 8 };
    s32 i;
    s32 flag;
    s32 word;
    s32 shift;
    s32 bit;

    D_801F3700 = allocHeapBlock(0xC00, 0x193);
    D_801F3700->mode = 11;
    func_801F2D50();
    for (i = 0; i < 6; i++) {
        if (places[i] == PROFILE->unk22) {
            D_801F3700->shopId = i;
            if (D_801F3700->shopId != 0) {
                D_801F3700->shopId++;
            }
            i = 6;
        }
    }
    if (D_801F3700->shopId == 0) {
        flag = 13;
        word = flag / 32;
        shift = flag % 32;
        if (PROFILE->flags[word] & (bit = 1 << shift)) {
            D_801F3700->shopId = 1;
        }
    }
    if (PROFILE->shops[D_801F3700->shopId].unk0 == -1) {
        PROFILE->shops[D_801F3700->shopId].unk0 = 0;
    }
    SUB_shopRandom(PROFILE->shops[D_801F3700->shopId].seed);
    for (i = 0; i < 110; i++) {
        D_801F3700->digimonCards[i].stock = PROFILE->digimonCards[i];
        D_801F3700->digimonCards[i].count = 0;
    }
    for (i = 0; i < 43; i++) {
        D_801F3700->optionCards[i].stock = PROFILE->optionCards[i];
        D_801F3700->optionCards[i].count = 0;
    }
    for (i = 0; i < 6; i++) {
        D_801F3700->digivolveCards[i].stock = PROFILE->digivolveCards[i];
        D_801F3700->digivolveCards[i].count = 0;
    }
    for (i = 0; i < 6; i++) {
        D_801F3700->packs[i].stock = 0;
        D_801F3700->packs[i].count = 0;
    }
    func_801F2618();
}
