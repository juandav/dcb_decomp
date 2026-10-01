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
#include "dcb/card_db.h"
#include "dcb/player_data.h"
#include "dcb/subseg.h"

/*
 * jp's SUBSEG is a card shop, a program of its own (us's SUBSEG is the deck
 * editor, deck/sub_deck_screens.c): it buys and sells cards and booster
 * packs, opens the packs bought, and shows a card's picture and its
 * Digimon's model. The executable sets it up with SUB_openBuyShop (buying)
 * or SUB_openSellShop (selling), then spawns SUB_runShop.
 */

#define CAMERA ((Graphics *)&GRAPHICS)
#define SUB_DIGIMON_CARDS ((DigimonCardData *)DIGIMON_CARDS)
#define SUB_OPTION_CARDS ((OptionCardData *)OPTION_CARDS)
#define SUB_DIGIVOLVE_CARDS ((DigivolveCardData *)DIGIVOLVE_CARDS)
/* what the shop tells the area it was opened from: [1] the outcome (1 a
   deal, 0 none, -1 copies past 8 were sold back), [12] the Bits gained
   (negative: spent) */
#define AREA_FLAGS (((SessionData *)SESSION_DATA)->areaSession->flags)

extern u8 CROSS_EFFECT_ICONS[];
extern char *CROSS_EFFECT_NAMES[];
extern BoosterPack BOOSTER_PACKS[];

void SetDrawEnv(DR_ENV *dr_env, DRAWENV *env);
void clearKanjiPage(s32);
void closeKanjiPage(s32);
void openKanjiPage(s32, s32);
void KAW_freeCursor(JpCursor *cursor);
void renderScrollingBackground();
void setBackgroundScrollMode(s32);
void stopScreenFade(void);
void drawWindowFrame(Rect16 *rect, s32, s32, s32, s32, void *, s32 z);
void KAW_drawCursor();
void drawScrollArrow(s32 x, s32 y, s32 dir, s32 palette, s32 z);
char *formatSjisNumber(s32 value, s32 digits, char *buf);
void uploadKanjiString(char *text, Rect16 *rect);
JpCursor *KAW_createCursor(s32, s32, s32, s32, s32);
void initWindowSprite(POLY_FT4 *poly, s32 clut, s32 mode, s32 u, s32 v, s32 w, s32 h);

/* sub_name_entry_jp.c */
void SUB_ignoreShopWindowClose(void);
void SUB_ignoreViewerWindowClose(void);

void SUB_runItemDetails();
void SUB_runStockGrid();
void SUB_runOpenedGrid();
void SUB_markSoldStock(void);
ShopList *SUB_buildShopList(s32 deck, u32 mode, s32 group);
void SUB_runOpenedDetails();
s32 SUB_getItemPrice(s8 type, s16 index);
s16 SUB_shopRandom(s16 seed);
void SUB_addCountsToMax(ShopList *list);
void SUB_setStockMax(ShopList *list);

void SUB_drawViewerHelp(JpWindow *window) {
    char text[64];

    sprintf(text, " Ｒ１／Ｒ２　拡大／縮小　　Ｌ１／Ｌ２　上げる／下げる");
    drawIconText(0x1A, 0x18, 7, 1, window->z, (s32)text);
}

void SUB_drawViewerMotions(JpWindow *window) {
    char text[64];

    sprintf(text, " b0b2b1　モーション　　　b3　戻る");
    drawIconText(0x1A, 0xD1, 7, 1, window->z, (s32)text);
}

void SUB_initViewerScene(void) {
    initScene3D(1);
    createWireGrid(3000, 3000, 11, 11, 1, 0);
    GRID_VISIBLE = 0;
    endTask(0x19);
    spawnTask(0x19, 0x1F, 0, 0x800, runSceneCameraTask, 1);
    endTask(0x1B);
    spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
    loadArenaStage(-1);
}

void SUB_freeViewerScene(void) {
    endTask(0x1B);
    endTask(0x19);
    removeFrameCallback((s32)renderSceneModels);
    removeFrameCallback((s32)renderWireGrid);
    unloadArenaStage();
    unloadAllModels();
    freeHeapBlocksByTag(0x7F);
}

void SUB_showViewerModel(void) {
    CAMERA->snapCamera = 1;
    CAMERA->targetDistance = 3000;
    CAMERA->targetHeight = -((Model *)SCENE_3D->models[0])->bonepos[0][1] * 3;
    SCENE_3D->modelState[0] = 1;
    showArenaStage(0);
    SCENE_3D->modelState[23] = 1;
    GRID_VISIBLE = 1;
    applyAnimationFirstFrame(0, 0);
    startModelAnimation(0, 0, -2, 0);
}

void SUB_unloadViewerModel(void) {
    freeHeapBlocksByTag(500);
    unloadModel(0);
    unloadModelAnimations(0);
    GRID_VISIBLE = 0;
    SCENE_3D->modelState[23] = 0;
}

void SUB_playViewerMotion(void) {
    s32 anim;

    CAMERA->targetModel = 0;
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
    } while (((Model *)SCENE_3D->models[0])->anim.keyTimer >= 0);
    waitFrames(30);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 1, 2, 0x20, 0);
    applyAnimationFirstFrame(0, 0);
    startModelAnimation(0, 0, -2, 0);
    CAMERA->targetModel = -1;
    waitFrames(8);
}

void SUB_runViewerControls(s32 parentTask) {
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_VIEWER_HELP_WINDOW, getCurrentTaskId());
    SUB_VIEWER_WINDOWS[0] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_VIEWER_MOTION_WINDOW, getCurrentTaskId());
    SUB_VIEWER_WINDOWS[1] = (JpWindow *)waitFrames(0x7FFFFFFF);
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
            SUB_playViewerMotion();
        }
    } while (!(PAD_STATES[0]->rawRepeat & PAD_SQUARE));
    SUB_VIEWER_WINDOWS[0]->state = 4;
    SUB_VIEWER_WINDOWS[1]->state = 4;
    waitFrames(10);
    resumeTask(parentTask);
}

void SUB_viewDigimonModel(s32 modelId) {
    s32 level;
    s32 next;

    level = SCROLLING_BACKGROUND->shownImage;
    next = level + 1;
    if (next >= 5) {
        next = 2;
    }
    setBackgroundScrollMode(next);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SCROLLING_BACKGROUND->brightness != 0);
    removeFrameCallback((s32)renderScrollingBackground);
    loadSoundEffectBank(0);
    loadDigimonModelPak(0, modelId);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 1, 2, 0x10, 0);
    SUB_showViewerModel();
    waitFrames(0x10);
    spawnTask(0, -1, 0, 0x400, SUB_runViewerControls, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 0x10, 0);
    waitFrames(0x10);
    SUB_unloadViewerModel();
    waitFrames(10);
    stopScreenFade();
    loadSoundEffectBank(1);
    setBackgroundScrollMode(level);
    addFrameCallback((s32)renderScrollingBackground);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SCROLLING_BACKGROUND->brightness != 0x80);
}

s32 SUB_getCardId(s32 type, s32 index) {
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
        total += PLAYER_DATA(0).cardCollection[i] & 0xF;
    }
    return total;
}

s32 SUB_countOwnedOptionCards(void) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < 43; i++) {
        total += PLAYER_DATA(0).optionCollection[i] & 0xF;
    }
    return total;
}

s32 SUB_countOwnedDigivolveCards(void) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < 6; i++) {
        total += PLAYER_DATA(0).digivolveCollection[i] & 0xF;
    }
    return total;
}

s32 SUB_countOwnedCards(void) {
    return SUB_countOwnedDigimonCards() + SUB_countOwnedOptionCards() + SUB_countOwnedDigivolveCards();
}

s32 SUB_countOwnedDigimonOf(s32 specialty, s32 level) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < 110; i++) {
        if ((PLAYER_DATA(0).cardCollection[i] & 0xF)
            && (specialty < 0 || SUB_DIGIMON_CARDS[i].attr >> 4 == specialty)
            && (level < 0 || (SUB_DIGIMON_CARDS[i].attr & 0xF) == level)) {
            total += PLAYER_DATA(0).cardCollection[i] & 0xF;
        }
    }
    return total;
}

s32 SUB_countDeckDigimonOf(s32 deck, s32 specialty, s32 level) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < 30; i++) {
        if (PLAYER_DATA(0).savedDecks[deck].cards[i].type == 0
            && (specialty < 0 || SUB_DIGIMON_CARDS[PLAYER_DATA(0).savedDecks[deck].cards[i].index].attr >> 4 == specialty)
            && (level < 0 || (SUB_DIGIMON_CARDS[PLAYER_DATA(0).savedDecks[deck].cards[i].index].attr & 0xF) == level)) {
            total++;
        }
    }
    return total;
}

/* the deck's slots that hold no Digimon card: the empty ones (0xFF) too */
s32 SUB_countDeckOptions(s32 deck) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < 30; i++) {
        if (PLAYER_DATA(0).savedDecks[deck].cards[i].type != 0) {
            total++;
        }
    }
    return total;
}

s32 SUB_countCards(s32 deck, s32 type, s32 specialty, s32 level) {
    s32 count = 0;

    if (type < 0) {
        if (deck == 0) {
            count = SUB_countOwnedCards();
        }
    } else if (deck == 0) {
        if (type == 0) {
            count = SUB_countOwnedDigimonOf(specialty, level);
        } else {
            count = SUB_countOwnedOptionCards() + SUB_countOwnedDigivolveCards();
        }
    } else if (type == 0) {
        count = SUB_countDeckDigimonOf(deck - 1, specialty, level);
    } else {
        count = SUB_countDeckOptions(deck - 1);
    }
    return count;
}

ItemIcon SUB_getItemIcon(s32 type, s32 index) {
    ItemIcon icon;
    s32 id = SUB_getCardId(type, index);
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

s32 SUB_countDecksInUse(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(0).savedDecks[i].inUse == 0) {
            break;
        }
    }
    return i;
}

void SUB_addCardsToDeck(s32 type, s32 index, s32 count) {
    CardSlot *slot;
    s32 i;

    if (count == 0) {
        return;
    }
    slot = PLAYER_DATA(0).savedDecks[SUB_SHOP_ITEM.deck].cards;
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

void SUB_sortItems(ShopItem *items, s32 left, s32 right) {
    ShopItem swap;
    s32 pivot;
    s32 j;
    s32 i;

    pivot = SUB_getCardId(items[(left + right) / 2].type, items[(left + right) / 2].index);
    i = left;
    j = right;
    while (1) {
        while (SUB_getCardId(items[i].type, items[i].index) < pivot) {
            if (i >= right) {
                break;
            }
            i++;
        }
        while (pivot < SUB_getCardId(items[j].type, items[j].index)) {
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
        SUB_sortItems(items, left, i - 1);
    }
    if (j + 1 < right) {
        SUB_sortItems(items, j + 1, right);
    }
}

void SUB_countListInDeck(ShopList *list, s32 deck) {
    CardSlot *slot;
    s32 i;
    s32 j;

    for (i = 0; i < list->count; i++) {
        slot = PLAYER_DATA(0).savedDecks[deck].cards;
        for (j = 0; j < 30; j++, slot++) {
            if (list->items[i].type == slot->type && list->items[i].index == slot->index) {
                list->items[i].count++;
            }
        }
    }
}

void SUB_setListMaxToOwned(ShopList *list) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        switch (list->items[i].type) {
        case 0:
            list->items[i].max = PLAYER_DATA(0).cardCollection[list->items[i].index] & 0x7F;
            break;
        case 1:
            list->items[i].max = PLAYER_DATA(0).optionCollection[list->items[i].index] & 0x7F;
            break;
        case 2:
            list->items[i].max = PLAYER_DATA(0).digivolveCollection[list->items[i].index] & 0x7F;
            break;
        case 3:
            list->items[i].max = 8;
            break;
        }
    }
}

s32 SUB_addItemToList(ShopList *list, u8 type, u8 index) {
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
    list->items[i].icon = SUB_getItemIcon(type, index);
    list->items[i].type = type;
    list->items[i].index = index;
    list->count++;
    return 0;
}

void SUB_listDeckCards(ShopList *list, s32 deck) {
    CardSlot *slot = PLAYER_DATA(0).savedDecks[deck].cards;
    s32 i;

    for (i = 0; i < 30; i++, slot++) {
        if (slot->type != 0xFF) {
            SUB_addItemToList(list, slot->type, slot->index);
        }
    }
    SUB_sortItems(list->items, 0, list->count - 1);
}

void SUB_listOwnedDigimon(ShopList *list, s32 specialty, s32 level) {
    s32 i;

    for (i = 0; i < 110; i++) {
        if ((PLAYER_DATA(0).cardCollection[i] & 0xF) && SUB_DIGIMON_CARDS[i].attr >> 4 == specialty
            && (level < 0 || (SUB_DIGIMON_CARDS[i].attr & 0xF) == level)) {
            SUB_addItemToList(list, 0, i);
        }
    }
}

void SUB_listOwnedOptionAndDigivolveCards(ShopList *list) {
    s32 i;

    for (i = 0; i < 43; i++) {
        if (PLAYER_DATA(0).optionCollection[i] & 0xF) {
            SUB_addItemToList(list, 1, i);
        }
    }
    for (i = 0; i < 6; i++) {
        if (PLAYER_DATA(0).digivolveCollection[i] & 0xF) {
            SUB_addItemToList(list, 2, i);
        }
    }
}

void SUB_listOwnedOptionsOfGroup(ShopList *list, s8 group) {
    s32 i;

    for (i = 0; i < 43; i++) {
        if ((PLAYER_DATA(0).optionCollection[i] & 0xF) && SUB_OPTION_GROUPS[i] == group) {
            SUB_addItemToList(list, 1, i);
        }
    }
}

void SUB_listOwnedDigivolveCards(ShopList *list) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (PLAYER_DATA(0).digivolveCollection[i] & 0xF) {
            SUB_addItemToList(list, 2, i);
        }
    }
}

void SUB_listNewCards(ShopList *list) {
    s32 i;

    for (i = 0; i < 110; i++) {
        if (PLAYER_DATA(0).cardCollection[i] & 0x80) {
            SUB_addItemToList(list, 0, i);
        }
    }
    for (i = 0; i < 43; i++) {
        if (PLAYER_DATA(0).optionCollection[i] & 0x80) {
            SUB_addItemToList(list, 1, i);
        }
    }
    for (i = 0; i < 6; i++) {
        if (PLAYER_DATA(0).digivolveCollection[i] & 0x80) {
            SUB_addItemToList(list, 2, i);
        }
    }
}

void SUB_listOwnedCards(ShopList *list) {
    s32 i;

    for (i = 0; i < 110; i++) {
        if (PLAYER_DATA(0).cardCollection[i] & 0xF) {
            SUB_addItemToList(list, 0, i);
        }
    }
    SUB_listOwnedOptionAndDigivolveCards(list);
}

void SUB_setListOwned(ShopList *list) {
    s32 i;
    u8 index;

    for (i = 0; i < list->count; i++) {
        index = list->items[i].index;
        switch (list->items[i].type) {
        case 0:
            if (index < 110) {
                list->items[i].owned = PLAYER_DATA(0).cardCollection[index] & 0x7F;
            }
            break;
        case 1:
            if (index < 43) {
                list->items[i].owned = PLAYER_DATA(0).optionCollection[index] & 0x7F;
            }
            break;
        case 2:
            if (index < 6) {
                list->items[i].owned = PLAYER_DATA(0).digivolveCollection[index] & 0x7F;
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

void SUB_listStock(ShopList *list) {
    s32 bit = 0;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (SUB_SHOP->packs[i].count != 0) {
            SUB_addItemToList(list, 3, i);
        }
    }
    for (i = 0; i < 110; i++) {
        while (SUB_SHOP->digimonCards[i].count != 0) {
            if (!((PLAYER_DATA(0).shops[SUB_SHOP->shopId].soldBits >> bit) & 1)) {
                SUB_addItemToList(list, 0, i);
            }
            SUB_SHOP->digimonCards[i].count--;
            bit++;
        }
    }
    for (i = 0; i < 43; i++) {
        while (SUB_SHOP->optionCards[i].count != 0) {
            if (!((PLAYER_DATA(0).shops[SUB_SHOP->shopId].soldBits >> bit) & 1)) {
                SUB_addItemToList(list, 1, i);
            }
            SUB_SHOP->optionCards[i].count--;
            bit++;
        }
    }
    for (i = 0; i < 6; i++) {
        while (SUB_SHOP->digivolveCards[i].count != 0) {
            if (!((PLAYER_DATA(0).shops[SUB_SHOP->shopId].soldBits >> bit) & 1)) {
                SUB_addItemToList(list, 2, i);
            }
            SUB_SHOP->digivolveCards[i].count--;
            bit++;
        }
    }
    SUB_setListOwned(list);
}

void SUB_sortList(ShopList *list) {
    ShopItem swap;
    s32 gap;
    s32 i;
    s32 j;

    for (gap = 1; gap < list->count / 3; gap = gap * 3 + 1) {
    }
    for (; gap > 0; gap /= 3) {
        for (i = gap; i < list->count; i++) {
            for (j = i - gap; j >= 0; j -= gap) {
                if (SUB_getCardId(list->items[j].type, list->items[j].index)
                    <= SUB_getCardId(list->items[j + gap].type, list->items[j + gap].index)) {
                    break;
                }
                swap = list->items[j];
                list->items[j] = list->items[j + gap];
                list->items[j + gap] = swap;
            }
        }
    }
}

void SUB_addLevelCardIds(u8 *unused, u8 pack, u8 level) {
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
        *SUB_PACK_CARD_END = id;
        id++;
        SUB_PACK_CARD_END++;
    }
    *SUB_PACK_CARD_END = 0xFF;
}

/* Returns an int: SUB_listOpenedPacks's code needs the wide return value, and
   SUB_rollStock narrows it to u8 itself. */
s32 SUB_rollPackCard(u8 pack, u8 level) {
    u8 *ids;
    u8 swap;
    s32 count;
    s32 i;
    s32 j;
    s16 k;

    ids = SUB_PACK_CARD_END = allocTaskHeapBlock(200);
    if (pack == 0xFF) {
        if (level >= 4) {
            level = 3;
        }
        if (level == 3) {
            for (j = 0; j < 7; j++) {
                for (i = 0; i < 3; i++) {
                    SUB_addLevelCardIds(SUB_PACK_CARD_END, j, i);
                }
            }
        } else {
            for (i = 0; i < 7; i++) {
                SUB_addLevelCardIds(SUB_PACK_CARD_END, i, level);
            }
        }
    } else {
        if (level >= 4 || pack >= 7) {
            level = 3;
            pack = 0;
        }
        if (level == 3) {
            for (i = 0; i < 3; i++) {
                SUB_addLevelCardIds(SUB_PACK_CARD_END, pack, i);
            }
        } else {
            SUB_addLevelCardIds(SUB_PACK_CARD_END, pack, level);
        }
    }
    count = SUB_PACK_CARD_END - ids;
    for (i = 0; i < count; i++) {
        k = SUB_shopRandom(-1) % count;
        swap = ids[i];
        ids[i] = ids[k];
        ids[k] = swap;
    }
    freeHeapBlock(ids);
    return ids[0];
}

void SUB_listOpenedPacks(ShopList *list) {
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

    for (i = 0; i < SUB_SHOP->list.count; i++) {
        if (SUB_SHOP->list.items[i].type != 3) {
            while (SUB_SHOP->list.items[i].count != 0) {
                SUB_addItemToList(list, SUB_SHOP->list.items[i].type, SUB_SHOP->list.items[i].index);
                SUB_SHOP->list.items[i].count--;
            }
        }
    }
    for (i = 0; i < count; i++) {
        if (SUB_SHOP->list.items[i].type == 3) {
            SUB_shopRandom(PLAYER_DATA(0).shops[SUB_SHOP->shopId].seeds[SUB_SHOP->list.items[i].index][0]);
            while (SUB_SHOP->list.items[i].count != 0) {
                for (j = 0; j < 4; j++) {
                    swap = order[j];
                    r = (s16)(SUB_shopRandom(-1) % 8);
                    order[j] = order[r];
                    order[r] = swap;
                }
                for (k = 0; k < 8; k++) {
                    SUB_shopRandom(-1);
                    for (j = 0; j < 4; j++) {
                        if (order[j] == k && SUB_SHOP->list.items[i].index < 5) {
                            pack = SUB_SHOP->list.items[i].index;
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
                    id = SUB_rollPackCard(pack, level);
                    if (id < 110) {
                        SUB_addItemToList(list, 0, id);
                    } else {
                        id -= 110;
                        if (id < 43) {
                            SUB_addItemToList(list, 1, id);
                        } else {
                            SUB_addItemToList(list, 2, id - 43);
                        }
                    }
                    if (!(k & 1)) {
                        waitFrames(1);
                    }
                }
                SUB_SHOP->list.items[i].count--;
                PLAYER_DATA(0).shops[SUB_SHOP->shopId].seeds[SUB_SHOP->list.items[i].index][0] =
                    PLAYER_DATA(0).shops[SUB_SHOP->shopId].seeds[SUB_SHOP->list.items[i].index][1];
                PLAYER_DATA(0).shops[SUB_SHOP->shopId].seeds[SUB_SHOP->list.items[i].index][1] = SUB_shopRandom(-1);
            }
        }
    }
    SUB_sortList(list);
    SUB_setListOwned(list);
}

void SUB_listSellableCards(ShopList *list) {
    u8 most;
    u8 copies;
    s32 i;
    s32 deck;
    s32 slot;

    for (i = 0; i < 110; i++) {
        most = 0;
        for (deck = 0; deck < 3; deck++) {
            copies = 0;
            if (PLAYER_DATA(0).savedDecks[deck].inUse != 0) {
                for (slot = 0; slot < 30; slot++) {
                    if (PLAYER_DATA(0).savedDecks[deck].cards[slot].type == 0
                        && PLAYER_DATA(0).savedDecks[deck].cards[slot].index == i) {
                        copies++;
                    }
                }
                if (copies > most) {
                    most = copies;
                }
            }
        }
        SUB_SHOP->digimonCards[i].stock -= most;
        if (SUB_DIGIMON_CARDS[i].price > 0) {
            while (--SUB_SHOP->digimonCards[i].stock != 0xFF) {
                SUB_addItemToList(list, 0, i);
            }
        }
    }
    for (i = 0; i < 43; i++) {
        most = 0;
        for (deck = 0; deck < 3; deck++) {
            copies = 0;
            if (PLAYER_DATA(0).savedDecks[deck].inUse != 0) {
                for (slot = 0; slot < 30; slot++) {
                    if (PLAYER_DATA(0).savedDecks[deck].cards[slot].type == 1
                        && PLAYER_DATA(0).savedDecks[deck].cards[slot].index == i) {
                        copies++;
                    }
                }
                if (copies > most) {
                    most = copies;
                }
            }
        }
        SUB_SHOP->optionCards[i].stock -= most;
        if (SUB_OPTION_CARDS[i].price > 0) {
            while (--SUB_SHOP->optionCards[i].stock != 0xFF) {
                SUB_addItemToList(list, 1, i);
            }
        }
    }
    for (i = 0; i < 6; i++) {
        most = 0;
        for (deck = 0; deck < 3; deck++) {
            copies = 0;
            if (PLAYER_DATA(0).savedDecks[deck].inUse != 0) {
                for (slot = 0; slot < 30; slot++) {
                    if (PLAYER_DATA(0).savedDecks[deck].cards[slot].type == 2
                        && PLAYER_DATA(0).savedDecks[deck].cards[slot].index == i) {
                        copies++;
                    }
                }
                if (copies > most) {
                    most = copies;
                }
            }
        }
        SUB_SHOP->digivolveCards[i].stock -= most;
        if (SUB_DIGIVOLVE_CARDS[i].price > 0) {
            while (--SUB_SHOP->digivolveCards[i].stock != 0xFF) {
                SUB_addItemToList(list, 2, i);
            }
        }
    }
    for (slot = 0; slot < list->count; slot++) {
        list->items[slot].max = list->items[slot].count;
        list->items[slot].count = 0;
        list->items[slot].owned = 1;
    }
}

void SUB_setListLeft(ShopList *list) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        list->items[i].owned = list->items[i].max - list->items[i].count;
        list->items[i].count = 0;
    }
}

ShopList *SUB_buildShopList(s32 deck, u32 mode, s32 group) {
    ShopList *list = allocHeapBlock(0x920, 0x195);

    bzero((Scene3D *)list, 0x920);
    switch (mode) {
    case 0:
        SUB_listDeckCards(list, deck);
        break;
    case 1:
        SUB_listOwnedDigimon(list, 0, group);
        break;
    case 2:
        SUB_listOwnedDigimon(list, 1, group);
        break;
    case 3:
        SUB_listOwnedDigimon(list, 2, group);
        break;
    case 4:
        SUB_listOwnedDigimon(list, 3, group);
        break;
    case 5:
        SUB_listOwnedDigimon(list, 4, group);
        break;
    case 6:
        SUB_listOwnedOptionAndDigivolveCards(list);
        break;
    case 7:
        SUB_listNewCards(list);
        break;
    case 8:
        SUB_listOwnedCards(list);
        break;
    case 9:
        SUB_listOwnedOptionsOfGroup(list, group);
        break;
    case 10:
        SUB_listOwnedDigivolveCards(list);
        break;
    case 11:
        SUB_listStock(list);
        break;
    case 12:
        SUB_listOpenedPacks(list);
        break;
    case 13:
        SUB_listSellableCards(list);
        break;
    }
    if (mode != 12) {
        if (mode == 11) {
            SUB_setStockMax(list);
        } else if (mode != 13) {
            SUB_setListMaxToOwned(list);
        }
    } else {
        SUB_setListMaxToOwned(list);
        SUB_addCountsToMax(list);
    }
    SUB_SHOP_LIST = list;
    return list;
}

void SUB_addCountsToMax(ShopList *list) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        list->items[i].max += list->items[i].count;
        if (list->items[i].max >= 9) {
            list->items[i].max = 8;
        }
    }
}

void SUB_setStockMax(ShopList *list) {
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

void SUB_drawCardPicture(JpWindow *window) {
    char text[152];
    POLY_FT4 *poly;
    s32 y;
    s16 flip;
    s16 dx;
    s16 dy;

    poly = SUB_SHOP->polys[FRAME_BUFFER_INDEX];
    y = 0xB2;
    if (SUB_SHOP->paid == 0) {
        sprintf(text, "b0枚数決定 ");
        drawIconText(0x1C, y, 7, 1, window->z, (s32)text);
    } else {
        y -= 12;
    }
    if (SUB_SHOP_ITEM.type == 0) {
        y += 12;
        sprintf(text, "b1ビューモード ");
        drawIconText(0x1C, y, 7, 1, window->z, (s32)text);
    }
    y += 12;
    sprintf(text, "b2キャンセル ");
    drawIconText(0x1C, y, 7, 1, window->z, (s32)text);
    if (SUB_SHOP->loading == 0 && SUB_SHOP->flipTimer-- <= 0) {
        SUB_SHOP->flipTimer = 0;
        if (SUB_SHOP->flipBack == 0) {
            SUB_SHOP->flip++;
        } else {
            SUB_SHOP->flip--;
        }
    }
    if (SUB_SHOP->flip <= 0) {
        SUB_SHOP->flip = 0;
    }
    if (SUB_SHOP->flip == 10) {
        SUB_SHOP->flipBack = 1;
    }
    if (SUB_SHOP->flipBack == 1) {
        flip = SUB_SHOP->flip;
        initWindowSprite(poly, (SUB_SHOP->imageSlot->clutY << 6) | ((SUB_SHOP->imageSlot->clutX >> 4) & 0x3F), 1,
                      SUB_SHOP->imageSlot->x, SUB_SHOP->imageSlot->y, 0x40, 0x40);
        dx = flip * 32 / 10;
        dy = SUB_SHOP->flip * 6 / 10;
        poly->x0 = dx + 36;
        poly->y0 = dy + 74;
        poly->x1 = 100 - dx;
        poly->y1 = 74 - dy;
        poly->x2 = dx + 36;
        poly->y2 = 138 - dy;
        poly->x3 = 100 - dx;
        poly->y3 = dy + 138;
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[window->z], (s32)SUB_SHOP->polys[FRAME_BUFFER_INDEX]);
        poly++;
        if (SUB_SHOP_ITEM.type == 3) {
            initWindowSprite(poly, 0x7F01, 0, 0x220, 0x180, 0x50, 0x74);
        } else {
            initWindowSprite(poly, 0x7F00, 0, 0x1A8, 0x100, 0x50, 0x74);
        }
        dx = flip * 4;
        dy = SUB_SHOP->flip * 6 / 10;
        poly->x0 = dx + 28;
        poly->y3 = dy + 170;
        poly->x1 = 108 - dx;
        poly->y2 = 170 - dy;
        poly->x2 = dx + 28;
        poly->y1 = 54 - dy;
        poly->x3 = 108 - dx;
        poly->y0 = dy + 54;
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[window->z], (s32)(SUB_SHOP->polys[FRAME_BUFFER_INDEX] + 1));
    } else {
        poly += 2;
        initWindowSprite(poly, 0x3DD8, 1, 0x180, 0x100, 0x50, 0x74);
        dx = SUB_SHOP->flip * 4;
        dy = SUB_SHOP->flip * 6 / 10;
        poly->x0 = dx + 28;
        poly->y0 = 54 - dy;
        poly->x1 = 108 - dx;
        poly->y1 = dy + 54;
        poly->x2 = dx + 28;
        poly->y2 = dy + 170;
        poly->x3 = 108 - dx;
        poly->y3 = 170 - dy;
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[window->z], (s32)(SUB_SHOP->polys[FRAME_BUFFER_INDEX] + 2));
    }
}

void SUB_drawItemIcon(ItemIcon icon, s32 x, s32 y, s32 z, u8 brightness) {
    s32 palette;
    s32 tpage;
    s32 clutX;
    s32 clut;

    if (icon.palette < 0) {
        /* SUB_getItemIcon stored ~palette */
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

void SUB_drawStockGrid(JpWindow *window) {
    char text[32];
    char number[24];
    Rect16 rect = { 0, 0xF0, 0x18, 9 };
    DigimonCardData *card;
    BoosterPack *pack;
    char *name;
    s32 id;
    u8 index;
    s32 price;
    s32 i;
    s32 y;
    s32 x;
    s32 n;
    s32 type;

    if (SUB_SHOP_LIST->count == 0) {
        return;
    }
    for (n = 0, i = SUB_SHOP_LIST->top; n < 10; n++, i++) {
        if (i >= SUB_SHOP_LIST->count) {
            break;
        }
        x = (n % 5) * 50 + 0x1A;
        y = (n / 5) * 64 + 0x38;
        if (SUB_SHOP->mode != 11) {
            price = 0;
        } else {
            price = SUB_getItemPrice(SUB_SHOP_LIST->items[i].type, SUB_SHOP_LIST->items[i].index) + SUB_SHOP->total;
            if (PLAYER_DATA(0).bits < price) {
                price = -1;
            }
        }
        if (SUB_SHOP_LIST->cursor == i) {
            SUB_SHOP_LIST->items[i].brightness = 0x80;
            SUB_GRID_CURSOR->x = x + 20;
            SUB_GRID_CURSOR->y = y + 24;
        } else if (SUB_SHOP_LIST->items[i].brightness > 0x40) {
            if ((SUB_SHOP_LIST->items[i].brightness -= 0x10) < 0x40) {
                SUB_SHOP_LIST->items[i].brightness = 0x40;
            }
        }
        if (SUB_SHOP_LIST->items[i].owned == 0) {
            drawTexturedSprite(x + 14, y + 32, &rect, 0x17, 0x7E5C, window->z, SUB_SHOP_LIST->items[i].brightness, -1);
        }
        SUB_drawItemIcon(SUB_SHOP_LIST->items[i].icon, x, y, window->z, SUB_SHOP_LIST->items[i].brightness);
        sprintf(text, "%3dc7(%d)", SUB_SHOP_LIST->items[i].count, SUB_SHOP_LIST->items[i].max);
        drawText(x - 8, y + 50, (s32)text,
                 SUB_SHOP_LIST->items[i].count == SUB_SHOP_LIST->items[i].max || price == -1 ? 2 : 7, window->z);
    }
    KAW_drawCursor(SUB_GRID_CURSOR);
    if (SUB_SHOP_LIST->top != 0) {
        drawScrollArrow(0x10E, 0x39, 4, 5, window->z);
    }
    if (SUB_SHOP_LIST->top + 10 < SUB_SHOP_LIST->count) {
        drawScrollArrow(0x10E, 0xAA, 6, 5, window->z);
    }
    type = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].type;
    if (type == 0) {
        card = &SUB_DIGIMON_CARDS[SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index];
        sprintf(text, "Ｎｏ %d", SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index + 1);
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
        sprintf(text, "a%d%s", card->attr >> 4, card->name);
        drawIconText(0x4E, 0xB8, 7, 1, window->z, (s32)text);
        sprintf(text, "Ｌｖe%d", (card->attr & 0xF) + 3);
        drawIconText(0xBA, 0xB8, 7, 1, window->z, (s32)text);
        sprintf(text, "ＨＰ%s", formatSjisNumber(card->hp, 4, number));
        drawIconText(0xEA, 0xB8, 7, 1, window->z, (s32)text);
    } else {
        if (type == 3) {
            index = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index;
            pack = &BOOSTER_PACKS[index];
            SUB_getCardId(SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].type, index);
            sprintf(text, "%sブースターパック", pack->name);
        } else {
            id = SUB_getCardId(SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].type, SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index) + 1;
            if (type == 1) {
                name = SUB_OPTION_CARDS[SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index].name;
            } else {
                name = SUB_DIGIVOLVE_CARDS[SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index].name;
            }
            sprintf(text, "Ｎｏ %ds0　s1a5オプションカードs0　s1%s", id, name);
        }
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
    }
}

void SUB_drawItemDetails(JpWindow *window) {
    Rect16 pos;
    char text[72];
    char title[72];
    char number[32];
    DigimonCardData *card;
    BoosterPack *pack;
    OptionCardData *option;
    s32 n;
    s32 price;
    s32 i;
    s32 id;
    u8 level;
    s32 color;
    s32 palette;

    if (SUB_SHOP->mode != 11) {
        price = 0;
    } else {
        price = SUB_getItemPrice(SUB_SHOP_ITEM.type, SUB_SHOP_ITEM.index) + SUB_SHOP->total;
        if (PLAYER_DATA(0).bits < price) {
            price = -1;
        }
    }
    if (SUB_SHOP_ITEM.type == 0) {
        card = &SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index];
        pos.x = 0x79;
        pos.y = 0x32;
        pos.w = 0;
        pos.h = 0;
        sprintf(title, "  a%dデジモンカード　Ｌｖ　e%d", card->attr >> 4,
                (card->attr & 0xF) + 3);
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)title);
        pos.y += 16;
        sprintf(title, "  属性　%s　Ｎｏ%s", SUB_SPECIALTY_NAMES[SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index].attr >> 4],
                formatSjisNumber(SUB_SHOP_ITEM.index + 1, 3, number));
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)title);
        pos.y += 12;
        sprintf(text, "  %s", SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index].name);
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
        drawIconText(pos.x + 100, pos.y, 7, 1, window->z, (s32)" ＨＰ");
        sprintf(text, " w-1%4d", SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index].hp);
        drawText(pos.x + 120, pos.y, (s32)text, 7, window->z);
        pos.y += 12;
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)" 必進Ｐ");
        sprintf(text, " w-1%2d", SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index].dpCost);
        drawText(pos.x + 48, pos.y, (s32)text, 7, window->z);
        drawIconText(pos.x + 100, pos.y, 7, 1, window->z, (s32)" ＰＯＷ");
        sprintf(text, " w-1%4d", SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index].dpBonus);
        drawText(pos.x + 120, pos.y, (s32)text, 7, window->z);
        pos.y += 12;
        for (i = 0; i < 3; i++, pos.y += 12) {
            /* b%d%s */
            sprintf(text, " 　b%d%s", i, SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index].attack[i].name);
            drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
            sprintf(text, " w-1%4d", SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index].attack[i].power);
            drawText(pos.x + 120, pos.y, (s32)text, 7, window->z);
        }
        level = CROSS_EFFECT_ICONS[card->crossEffect];
        if (level != 0) {
            /* %s d%d */
            sprintf(text, "  　%s d%d", CROSS_EFFECT_NAMES[SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index].crossEffect], level);
        } else {
            /* %s */
            sprintf(text, "  　%s", CROSS_EFFECT_NAMES[SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index].crossEffect]);
        }
        drawIconText(pos.x + 1, pos.y, 7, 1, window->z, (s32)text);
        pos.y += 16;
        if (card->supportIcon == 0) {
            drawIconText(pos.x + 2, pos.y, 6, 1, window->z, (s32)" 援護能力");
        } else {
            sprintf(text, "援護能力 d%d", card->supportIcon);
            drawIconText(pos.x + 2, pos.y, 6, 1, window->z, (s32)text);
        }
        pos.y += 12;
        for (i = 0; i < 4; i++, pos.y += 12) {
            sprintf(text, " %s", SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index].supportText[i]);
            drawIconText(pos.x - 2, pos.y, 7, 1, window->z, (s32)text);
        }
    } else if (SUB_SHOP_ITEM.type == 3) {
        SUB_getCardId(3, SUB_SHOP_ITEM.index);
        pack = &BOOSTER_PACKS[SUB_SHOP_ITEM.index];
        pos.x = 0x7D;
        pos.y = 0x36;
        pos.w = 0;
        pos.h = 0;
        sprintf(text, " %sブースターパック", pack->name);
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
        pos.y += 16;
        drawIconText(pos.x + 12, pos.y, 7, 1, window->z, (s32)" ８枚入り");
        pos.y += 16;
        switch (SUB_SHOP_ITEM.index) {
        case 0:
            drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)" 必ず４枚以上の火炎デジモン");
            break;
        case 1:
            drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)" 必ず４枚以上の氷水デジモン");
            break;
        case 2:
            drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)" 必ず４枚以上の自然デジモン");
            break;
        case 3:
            drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)" 必ず４枚以上の暗黒デジモン");
            break;
        case 4:
            drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)" 必ず４枚以上の珍種デジモン");
            break;
        case 5:
            drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)" 属性とオプションの割合は");
            drawIconText(pos.x, pos.y + 12, 7, 1, window->z, (s32)" 完全にランダムです。");
            break;
        }
        if (SUB_SHOP_ITEM.index != 5) {
            drawIconText(pos.x, pos.y + 12, 7, 1, window->z, (s32)" が入っています。");
        }
    } else {
        id = SUB_getCardId(SUB_SHOP_ITEM.type, SUB_SHOP_ITEM.index);
        palette = SUB_SHOP_ITEM.type + 4;
        pos.x = 0x7D;
        pos.y = 0x36;
        pos.w = 0;
        pos.h = 0;
        if (SUB_SHOP_ITEM.type == 1 && (option = &SUB_OPTION_CARDS[SUB_SHOP_ITEM.index])->supportIcon != 0) {
            sprintf(text, " a%dオプションカード d%d", palette, option->supportIcon);
        } else {
            sprintf(text, " a%dオプションカード", palette);
        }
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
        pos.y += 16;
        sprintf(text, "   %s　　Ｎｏ%s", SUB_OPTION_KINDS[SUB_SHOP_ITEM.type - 1], formatSjisNumber(id + 1, 3, number));
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
        pos.y += 16;
        drawIconText(pos.x, pos.y, 7, 1, window->z,
                     SUB_SHOP_ITEM.type == 1 ? (s32)SUB_OPTION_CARDS[SUB_SHOP_ITEM.index].name : (s32)SUB_DIGIVOLVE_CARDS[SUB_SHOP_ITEM.index].name);
        pos.y += 24;
        for (i = 0; i < 4; i++, pos.y += 12) {
            drawIconText(pos.x + 24, pos.y, 7, 1, window->z,
                         SUB_SHOP_ITEM.type == 1 ? (s32)SUB_OPTION_CARDS[SUB_SHOP_ITEM.index].text[i] : (s32)SUB_DIGIVOLVE_CARDS[SUB_SHOP_ITEM.index].text[i]);
        }
    }
    if (SUB_SHOP->paid == 0) {
        if (SUB_SHOP->mode == 11) {
            drawIconText(0xEC, 0xA6, 7, 1, window->z, (s32)"購入枚数");
            sprintf(text, "b%d", SUB_SHOP_ITEM.count);
            drawText(0xF1, 0xBA, (s32)text, SUB_SHOP_ITEM.count < SUB_SHOP_ITEM.max ? price == -1 ? 2 : 8 : 2, window->z);
            sprintf(text, "c7(%d)", SUB_SHOP_ITEM.max);
            drawText(0x104, 0xC6, (s32)text, SUB_SHOP_ITEM.count >= SUB_SHOP_ITEM.max || price == -1 ? 2 : 8, window->z);
        } else {
            drawIconText(0xEC, 0xA6, 7, 1, window->z, (s32)"売却枚数");
            sprintf(text, "b%d", SUB_SHOP_ITEM.count);
            drawText(0xF1, 0xBA, (s32)text, SUB_SHOP_ITEM.count < SUB_SHOP_ITEM.max ? price == -1 ? 2 : 8 : 2, window->z);
            sprintf(text, "c7(%d)", SUB_SHOP_ITEM.max);
            drawText(0x104, 0xC6, (s32)text, SUB_SHOP_ITEM.count >= SUB_SHOP_ITEM.max || price == -1 ? 2 : 8, window->z);
        }
        pos.x = 0xEC;
        pos.y = 0xA4;
        pos.w = 0x30;
        pos.h = 0x31;
        drawWindowFrame(&pos, 0, 0, 1, 0xFF, window->colors, window->z);
    }
}

void SUB_drawTotal(JpWindow *window) {
    char text[136];
    char number[16];
    s32 total;

    total = SUB_SHOP->total;
    if (SUB_SHOP->mode == 13) {
        total /= 5;
    }
    formatSjisNumber(total, 6, number);
    sprintf(text, " 合計 s0w-4%s d0", number);
    drawIconText(0x1A, 0x17, 7, 1, window->z, (s32)text);
}

void SUB_drawBits(JpWindow *window) {
    char text[136];
    char number[16];
    s8 i;

    for (i = 0; i < 64; i++) {
        text[i] = 0;
    }
    for (i = 0; i < 13; i++) {
        number[i] = 0;
    }
    formatSjisNumber(PLAYER_DATA(0).bits, 6, number);
    sprintf(text, " 所持金 s0w-4%s d0", number);
    drawIconText(0xAA, 0x17, 7, 1, window->z, (s32)text);
}

void SUB_drawStockHelp(JpWindow *window) {
    char text[136];
    char number[16];
    ShopItem *items = SUB_SHOP->list.items;
    s32 cur = SUB_SHOP->list.cursor;
    s32 price = PLAYER_DATA(0).playTime; /* jp starts from the play time, which only an unknown item type keeps */

    if (SUB_SHOP_LIST->count > 0) {
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
            price = BOOSTER_PACKS[items[cur].index].price;
            break;
        }
        price *= 100;
    } else {
        price = 0;
    }
    if (SUB_SHOP->mode == 13) {
        formatSjisNumber(price / 5, 4, number);
        sprintf(text, "b0見る b2戻る b1売る Ｌ１ － Ｒ１ ＋ 売値 s0w-4%s d0", number);
    } else {
        formatSjisNumber(price, 4, number);
        sprintf(text, "b0見る b2戻る b1買う Ｌ１ － Ｒ１ ＋ 買値 s0w-4%s d0", number);
    }
    drawIconText(0x1A, 0xD1, 7, 1, window->z, (s32)text);
}

void SUB_drawOpenedHelp(JpWindow *window) {
    char text[152];

    sprintf(text, " b0見る b2戻る ");
    drawIconText(0x1A, 0xD1, 7, 1, window->z, (s32)text);
}

void SUB_drawOpenedGrid(JpWindow *window) {
    char text[32];
    char number[24];
    Rect16 rect = { 0, 0xF0, 0x18, 9 };
    DigimonCardData *card;
    char *name;
    s32 id;
    s32 i;
    s32 y;
    s32 x;
    s32 n;
    s32 type;

    for (n = 0, i = SUB_SHOP_LIST->top; n < 10; n++, i++) {
        if (i >= SUB_SHOP_LIST->count) {
            break;
        }
        x = (n % 5) * 50 + 0x1A;
        y = (n / 5) * 64 + 0x38;
        if (SUB_SHOP_LIST->cursor == i) {
            SUB_SHOP_LIST->items[i].brightness = 0x80;
            SUB_GRID_CURSOR->x = x + 20;
            SUB_GRID_CURSOR->y = y + 24;
        } else if (SUB_SHOP_LIST->items[i].brightness > 0x40) {
            if ((SUB_SHOP_LIST->items[i].brightness -= 0x10) < 0x40) {
                SUB_SHOP_LIST->items[i].brightness = 0x40;
            }
        }
        if (SUB_SHOP_LIST->items[i].owned == 0) {
            drawTexturedSprite(x + 14, y + 32, &rect, 0x17, 0x7E5C, window->z, SUB_SHOP_LIST->items[i].brightness, -1);
        }
        SUB_drawItemIcon(SUB_SHOP_LIST->items[i].icon, x, y, window->z, SUB_SHOP_LIST->items[i].brightness);
        sprintf(text, "%3dc7(%d)", SUB_SHOP_LIST->items[i].count, SUB_SHOP_LIST->items[i].max);
        drawText(x - 8, y + 50, (s32)text, SUB_SHOP_LIST->items[i].max >= 8 ? 2 : 7, window->z);
    }
    KAW_drawCursor(SUB_GRID_CURSOR);
    if (SUB_SHOP_LIST->top != 0) {
        drawScrollArrow(0x10E, 0x39, 4, 5, window->z);
    }
    if (SUB_SHOP_LIST->top + 10 < SUB_SHOP_LIST->count) {
        drawScrollArrow(0x10E, 0xAA, 6, 5, window->z);
    }
    type = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].type;
    if (type == 0) {
        card = &SUB_DIGIMON_CARDS[SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index];
        sprintf(text, "Ｎｏ %d", SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index + 1);
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
        sprintf(text, "a%d%s", card->attr >> 4, card->name);
        drawIconText(0x4E, 0xB8, 7, 1, window->z, (s32)text);
        sprintf(text, "Ｌｖe%d", (card->attr & 0xF) + 3);
        drawIconText(0xBA, 0xB8, 7, 1, window->z, (s32)text);
        sprintf(text, "ＨＰ%s", formatSjisNumber(card->hp, 4, number));
        drawIconText(0xEA, 0xB8, 7, 1, window->z, (s32)text);
    } else {
        id = SUB_getCardId(SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].type, SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index) + 1;
        if (type == 1) {
            name = SUB_OPTION_CARDS[SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index].name;
        } else {
            name = SUB_DIGIVOLVE_CARDS[SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index].name;
        }
        sprintf(text, "Ｎｏ %ds0　s1a5オプションカードs0　s1%s", id, name);
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
    }
}

void SUB_drawOpenedTitle(JpWindow *window) {
    char text[13] = " 購入カード ";

    drawIconText(0x38, 0x17, 7, 1, window->z, (s32)text);
}

void SUB_loadShopTextures(void) {
    u32 *tims;

    spawnTask(0, -1, 0, 0x800, loadFile, "C:\\OBJECT\\shop.TIM", getCurrentTaskId());
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    if (tims != NULL) {
        uploadTimList(tims);
        freeHeapBlock(tims);
    }
}

void SUB_updateDeckCount(s32 delta) {
    Rect16 rect = { 0x3D5, 0xF4, 0, 0 };
    char text[8];

    formatSjisNumber(SUB_SHOP_ITEM.deckCount += delta, 2, text);
    uploadKanjiString(text, &rect);
}

void SUB_uploadDeckName(s32 deck) {
    char text[48];
    Rect16 rect;
    PlayerDeck *saved = &PLAYER_DATA(0).savedDecks[deck];

    fillVramRect(0x3C0, 0xF4, 0x9C, 0xC, 0);
    rect.x = 0x3C0;
    rect.y = 0xF4;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString(saved->name, &rect);
    rect.x = 0x3D5;
    sprintf(text, "　　／３０枚");
    uploadKanjiString(text, &rect);
    SUB_updateDeckCount(0);
}

/* the card numbers each booster pack holds: count of them from base + start
   (SUB_addPackCardIds); packs 5 and 6 hold the option cards */
PackRange SUB_PACK_RANGES[7] = {
    { 0, 3, 8, 9 },  { 20, 3, 8, 11 }, { 42, 3, 12, 12 }, { 69, 3, 9, 10 },
    { 91, 3, 6, 8 }, { 0, 19, 7, 9 },  { 0, 1, 3, 1 },
};

JpWindowDesc SUB_STOCK_GRID_WINDOW = { 290, 48, 0, 12, 18, 48, 272, 148, 10, 1, SUB_drawStockGrid, SUB_ignoreShopWindowClose };
JpWindowDesc SUB_DETAILS_WINDOW = { 290, 48, 0, 12, 122, 48, 168, 170, 10, 1, SUB_drawItemDetails, SUB_ignoreShopWindowClose };
JpWindowDesc SUB_TOTAL_WINDOW = { 26, 23, 0, 12, 26, 23, 116, 12, 10, 0, SUB_drawTotal, SUB_ignoreShopWindowClose };
JpWindowDesc SUB_BITS_WINDOW = { 290, 23, 0, 12, 170, 23, 120, 12, 10, 1, SUB_drawBits, SUB_ignoreShopWindowClose };
JpWindowDesc SUB_STOCK_HELP_WINDOW = { 26, 208, 0, 12, 26, 208, 272, 12, 10, 0, SUB_drawStockHelp, SUB_ignoreShopWindowClose };
JpWindowDesc SUB_OPENED_HELP_WINDOW = { 26, 208, 0, 12, 26, 208, 116, 12, 10, 0, SUB_drawOpenedHelp, SUB_ignoreShopWindowClose };
JpWindowDesc SUB_OPENED_GRID_WINDOW = { 290, 48, 0, 12, 18, 48, 272, 148, 10, 1, SUB_drawOpenedGrid, SUB_ignoreShopWindowClose };
JpWindowDesc SUB_OPENED_TITLE_WINDOW = { 26, 23, 0, 12, 26, 23, 116, 12, 10, 0, SUB_drawOpenedTitle, SUB_ignoreShopWindowClose };
JpWindowDesc SUB_PICTURE_WINDOW = { 26, 48, 0, 12, 26, 48, 84, 170, 10, 0, SUB_drawCardPicture, SUB_ignoreShopWindowClose };
JpWindowDesc SUB_VIEWER_HELP_WINDOW = { 26, 23, 0, 12, 26, 23, 272, 12, 10, 0, SUB_drawViewerHelp, SUB_ignoreViewerWindowClose };
JpWindowDesc SUB_VIEWER_MOTION_WINDOW = { 26, 208, 0, 12, 26, 208, 272, 12, 10, 0, SUB_drawViewerMotions, SUB_ignoreViewerWindowClose };

/* per option card: the group SUB_listOwnedOptionsOfGroup lists it in */
s8 SUB_OPTION_GROUPS[46] = {
    0, 1, 1, 2, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 2,
    1, 1, 2, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 2, 1, 0, 0, -1,
};

/* the specialties */
char *SUB_SPECIALTY_NAMES[6] = {
    "火炎",
    "氷水",
    "自然",
    "暗黒",
    "珍種",
    "普通",
};

/* not referenced by any code */
char *SUB_DIGIVOLVE_EFFECT_NAMES[5] = {
    "色、必要進化ポイント無視",
    "２段階進化",
    "進化ポイント＋３０",
    "同世代の交換",
    "１段階退化",
};

/* the option cards' kinds */
char *SUB_OPTION_KINDS[2] = {
    "戦闘用",
    "進化用",
};

/* SUB_shopRandom's seed */
s16 SUB_RANDOM_SEED = 31416;

void SUB_renderModelPreview(FrameBuffer *fb, s32 index) {
    s32 other = index ^ 1;

    fillVramRect(SUB_PREVIEW_DRAW_ENVS[other].clip[0], SUB_PREVIEW_DRAW_ENVS[other].clip[1], 0x180, 0x80, 0);
    AddPrim((s32 *)&fb->ot[0xFFF], (s32)&SUB_PREVIEW_DR_ENVS[other]);
    SetDrawEnv(&SUB_SCREEN_DR_ENVS[other], &fb->draw);
    AddPrim((s32 *)&fb->ot[300], (s32)&SUB_SCREEN_DR_ENVS[other]);
}

void SUB_openModelPreview(s32 modelId) {
    SetDefDrawEnv(&SUB_PREVIEW_DRAW_ENVS[0], 0x140, 0x100, 0x60, 0x80);
    SetDefDrawEnv(&SUB_PREVIEW_DRAW_ENVS[1], 0x140, 0x180, 0x60, 0x80);
    SUB_PREVIEW_DRAW_ENVS[0].ofs[0] -= 0x70;
    SUB_PREVIEW_DRAW_ENVS[1].ofs[0] -= 0x70;
    SUB_PREVIEW_DRAW_ENVS[0].ofs[1] -= 0x14;
    SUB_PREVIEW_DRAW_ENVS[1].ofs[1] -= 0x14;
    SetDrawEnv(&SUB_PREVIEW_DR_ENVS[0], &SUB_PREVIEW_DRAW_ENVS[0]);
    SetDrawEnv(&SUB_PREVIEW_DR_ENVS[1], &SUB_PREVIEW_DRAW_ENVS[1]);
    initScene3D(1);
    addFrameCallback((s32)SUB_renderModelPreview);
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

void SUB_closeModelPreview(void) {
    if (SUB_SHOP_ITEM.type == 0) {
        endTask(0x19);
        endTask(0x1B);
        removeFrameCallback((s32)SUB_renderModelPreview);
        removeFrameCallback((s32)renderSceneModels);
        removeFrameCallback((s32)renderWireGrid);
        waitFrames(4);
        unloadAllModels();
        freeHeapBlocksByTag(0x7F);
    }
}

void SUB_markSoldStock(void) {
    ShopList list = *SUB_SHOP_LIST;
    s32 i = 0;
    /* redundant (the loop sets it), but it puts i = 0 before the count load, as in jp */
    s32 bit = 0;

    for (; i < list.count; i++) {
        if (list.items[i].type != 3) {
            break;
        }
    }
    for (bit = 0; bit < 8; bit++) {
        if (!((PLAYER_DATA(0).shops[SUB_SHOP->shopId].soldBits >> bit) & 1)) {
            if (list.items[i].count != 0) {
                PLAYER_DATA(0).shops[SUB_SHOP->shopId].soldBits |= 1 << bit;
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
        return PLAYER_DATA(0).cardCollection[index] & 0x7F;
    case 1:
        return PLAYER_DATA(0).optionCollection[index] & 0x7F;
    case 2:
        return PLAYER_DATA(0).digivolveCollection[index] & 0x7F;
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

void SUB_commitDeal(s8 selling) {
    s32 i;
    u8 excess;

    for (i = 0; i < SUB_SHOP_LIST->count; i++) {
        if (selling == 0) {
            excess = SUB_getCopiesOverLimit(SUB_SHOP_LIST->items[i].type, SUB_SHOP_LIST->items[i].index, SUB_SHOP_LIST->items[i].count);
            if (excess != 0) {
                AREA_FLAGS[12] += SUB_getItemPrice(SUB_SHOP_LIST->items[i].type, SUB_SHOP_LIST->items[i].index) * excess / 5;
            }
            if (addCardToCollection(SUB_SHOP_LIST->items[i].type, SUB_SHOP_LIST->items[i].index, SUB_SHOP_LIST->items[i].count) < 0) {
                AREA_FLAGS[1] = -1;
            }
        } else {
            removeCardFromCollection(SUB_SHOP_LIST->items[i].type, SUB_SHOP_LIST->items[i].index, SUB_SHOP_LIST->items[i].count);
        }
        if ((i & 3) == 0) {
            waitFrames(1);
        }
    }
}

void SUB_closeShop(s8 mode) {
    setBackgroundScrollMode(1);
    switch (mode) {
    case 0:
        SUB_SHOP_WINDOWS[0]->state = 4;
        SUB_SHOP_WINDOWS[1]->state = 4;
        SUB_SHOP_WINDOWS[3]->state = 4;
        SUB_SHOP_WINDOWS[4]->state = 4;
        break;
    case 1:
        SUB_SHOP_WINDOWS[5]->state = 4;
        SUB_SHOP_WINDOWS[6]->state = 4;
        SUB_SHOP_WINDOWS[7]->state = 4;
        SUB_SHOP_WINDOWS[4]->state = 4;
        break;
    }
    waitFrames(90);
    KAW_freeCursor(SUB_GRID_CURSOR);
    SUB_SHOP_RUNNING = 0;
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
        price = BOOSTER_PACKS[index].price;
        break;
    default:
        price = -1;
        break;
    }
    return price * 100;
}

s32 SUB_adjustItemCount(s32 more, s32 less, s8 *count, s8 max) {
    s32 price = SUB_getItemPrice(SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].type, SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index);
    s32 sound;

    if (PAD_STATES[0]->rawRepeat & more) {
        if (SUB_SHOP->mode == 13) {
            if (++*count > max) {
                *count = max;
                return 1;
            }
        } else {
            if (PLAYER_DATA(0).bits < price + SUB_SHOP->total) {
                return 1;
            }
            if (++*count > max) {
                *count = max;
                return 1;
            }
        }
        SUB_SHOP->total += price;
        sound = 0;
    } else if (PAD_STATES[0]->rawRepeat & less) {
        if (--*count < 0) {
            *count = 0;
            return 1;
        }
        SUB_SHOP->total -= price;
        sound = 1;
    } else {
        return 0;
    }
    playSoundEffect(sound);
    return 1;
}

void SUB_loadCardPicture(void) {
    char path[64];
    u32 *tim;
    s32 id = SUB_getCardId(SUB_SHOP_ITEM.type, SUB_SHOP_ITEM.index);
    s32 i;

    SUB_SHOP->flip = 0;
    SUB_SHOP->flipBack = 0;
    SUB_SHOP->flipTimer = 60;
    for (i = 0; i < 3; i++) {
        if (SUB_SHOP->imageSlot->id != id) {
            if (SUB_SHOP->imageSlot != &SUB_SHOP->imageSlots[3]) {
                SUB_SHOP->imageSlot++;
            } else {
                SUB_SHOP->imageSlot = SUB_SHOP->imageSlots;
            }
        } else {
            SUB_SHOP->loading = 1;
        }
    }
    if (SUB_SHOP->loading == 0) {
        SUB_SHOP->imageSlot->id = id;
        SUB_SHOP->loading = 1;
        sprintf(path, "B:\\L_CARD\\LC%3.3d.TIM", id);
        spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
        tim = (u32 *)waitFrames(0x7FFFFFFF);
        uploadTim(tim, SUB_SHOP->imageSlot->x, SUB_SHOP->imageSlot->y, SUB_SHOP->imageSlot->clutX,
                  SUB_SHOP->imageSlot->clutY);
        DrawSync(0);
        freeHeapBlock(tim);
    }
    SUB_SHOP->loading = 0;
}

void SUB_viewItemModel(s32 arg) {
    clearKanjiPage(0xF);
    closeKanjiPage(0xF);
    openKanjiPage(0x15, 0x1B9);
    clearKanjiPage(0x15);
    SUB_initViewerScene();
    SUB_viewDigimonModel(SUB_DIGIMON_CARDS[SUB_SHOP_ITEM.index].modelId);
    SUB_freeViewerScene();
    SUB_SHOP_WINDOWS[1] = NULL;
    SUB_SHOP_WINDOWS[7] = NULL;
    SUB_SHOP_WINDOWS[4] = NULL;
    SUB_SHOP_WINDOWS[3] = NULL;
    clearKanjiPage(0x15);
    closeKanjiPage(0x15);
    openKanjiPage(0xF, 0x1B9);
    clearKanjiPage(0xF);
    spawnTask(0, -1, 0, 0x1000, SUB_SHOP->fromOpened == 0 ? SUB_runItemDetails : SUB_runOpenedDetails, arg);
    exitTask();
}

void SUB_runItemDetails(s32 parent) {
    s32 count = SUB_SHOP_ITEM.count;
    s32 viewing;

    spawnTask(0, -1, 0, 0x1000, SUB_loadCardPicture, parent);
    SUB_SHOP->fromOpened = 0;
    if (SUB_SHOP_WINDOWS[3] == NULL) {
        spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_TOTAL_WINDOW, getCurrentTaskId());
        SUB_SHOP_WINDOWS[3] = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
    if (SUB_SHOP_WINDOWS[4] == NULL) {
        spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_BITS_WINDOW, getCurrentTaskId());
        SUB_SHOP_WINDOWS[4] = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_DETAILS_WINDOW, getCurrentTaskId());
    SUB_SHOP_WINDOWS[2] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_PICTURE_WINDOW, getCurrentTaskId());
    SUB_SHOP_WINDOWS[0] = (JpWindow *)waitFrames(0x7FFFFFFF);
    waitFrames(90);
    viewing = 0;
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (SUB_SHOP->loading != 0) {
            continue;
        }
        if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
            SUB_updateDeckCount(count - SUB_SHOP_ITEM.count);
            SUB_SHOP->total = SUB_SHOP->savedTotal;
            goto close;
        }
        if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
            SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].count = SUB_SHOP_ITEM.count;
            goto close;
        }
        if ((PAD_STATES[0]->rawPressed & PAD_TRIANGLE) && SUB_SHOP_ITEM.type == 0) {
            viewing = 1;
            spawnTask(0, -1, 0, 0x1000, SUB_viewItemModel, count - SUB_SHOP_ITEM.count, getCurrentTaskId(), 0, 0);
            goto close;
        }
        if (SUB_SHOP->mode == 11 || SUB_SHOP->mode == 13) {
            SUB_adjustItemCount(PAD_RIGHT | PAD_R1, PAD_LEFT | PAD_L1, &SUB_SHOP_ITEM.count, SUB_SHOP_ITEM.max);
        }
    }
close:
    SUB_SHOP_WINDOWS[2]->state = 4;
    SUB_SHOP_WINDOWS[0]->state = 4;
    if (viewing == 1) {
        SUB_SHOP_WINDOWS[3]->state = 4;
        SUB_SHOP_WINDOWS[4]->state = 4;
    }
    waitFrames(30);
    waitFrames(30);
    if (viewing == 1) {
        exitTask();
    } else {
        spawnTask(0, -1, 0, 0x1000, SUB_runStockGrid, parent);
        exitTask();
    }
}

void SUB_runStockGrid(s32 parent) {
    ShopList *list;
    s16 action;
    s32 i;
    u16 pressed;
    s32 count;
    s32 prev;
    s32 rowStart;
    s32 rowEnd;

    if (SUB_SHOP_LIST == NULL) {
        list = SUB_buildShopList(SUB_SHOP_ITEM.deck, SUB_SHOP->mode, -1);
        if (SUB_SHOP->itemChosen == 0) {
            SUB_SHOP->itemChosen = 1;
            SUB_SHOP_ITEM.type = list->items[0].type;
            SUB_SHOP_ITEM.index = list->items[0].index;
        }
    } else {
        list = SUB_SHOP_LIST;
    }
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_STOCK_GRID_WINDOW, getCurrentTaskId());
    SUB_SHOP_WINDOWS[0] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_STOCK_HELP_WINDOW, getCurrentTaskId());
    SUB_SHOP_WINDOWS[1] = (JpWindow *)waitFrames(0x7FFFFFFF);
    if (list->count >= SUB_SAVED_CURSOR) {
        list->cursor = SUB_SAVED_CURSOR;
        list->top = SUB_SAVED_TOP;
    }
    SUB_SHOP->list = *SUB_SHOP_LIST;
    waitFrames(60);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
            AREA_FLAGS[1] = 0;
            SUB_closeShop(0);
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
            for (i = 0; i < SUB_SHOP_LIST->count; i++) {
                if (SUB_SHOP_LIST->items[i].count != 0) {
                    action = 2;
                    goto found;
                }
            }
        found:
        }
        if (SUB_SHOP_LIST->count == 0) {
            action = 0;
        }
        SUB_SHOP->list = *SUB_SHOP_LIST;
        if (action != 0) {
            if (action != 1 && SUB_SHOP->mode != 13) {
                SUB_SHOP_WINDOWS[3]->state = 4;
                SUB_SHOP_WINDOWS[0]->state = 4;
                SUB_SHOP_WINDOWS[1]->state = 4;
            } else if (action == 1) {
                SUB_SHOP_WINDOWS[0]->state = 4;
                SUB_SHOP_WINDOWS[1]->state = 4;
            }
            waitFrames(30);
            waitFrames(30);
            SUB_SHOP_ITEM.type = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].type;
            SUB_SHOP_ITEM.index = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index;
            SUB_SHOP_ITEM.count = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].count;
            SUB_SHOP_ITEM.max = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].max;
            SUB_SHOP_ITEM.owned = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].owned;
            SUB_SAVED_CURSOR = list->cursor;
            SUB_SAVED_TOP = list->top;
            if (action == 1) {
                SUB_SHOP->savedTotal = SUB_SHOP->total;
                spawnTask(0, -1, 0, 0x1000, SUB_SHOP_LIST->count != 0 ? SUB_runItemDetails : NULL, parent);
            } else if (SUB_SHOP->mode == 13) {
                AREA_FLAGS[1] = 1;
                AREA_FLAGS[12] = SUB_SHOP->total / 5;
                SUB_commitDeal(1);
                SUB_closeShop(0);
            } else {
                SUB_markSoldStock();
                SUB_SHOP->list = *SUB_SHOP_LIST;
                SUB_SHOP->paid = 1;
                AREA_FLAGS[1] = 1;
                AREA_FLAGS[12] = -SUB_SHOP->total;
                spawnTask(0, -1, 0, 0x1000, SUB_SHOP_LIST->count != 0 ? SUB_runOpenedGrid : NULL, parent, getCurrentTaskId(), 0, 0);
                freeHeapBlocksByTag(0x195);
                SUB_SHOP_LIST = NULL;
                SUB_SAVED_CURSOR = SUB_SAVED_TOP = 0;
            }
            clearKanjiPage(0xF);
            exitTask();
        }
        count = SUB_SHOP_LIST->count;
        if (count != 0) {
            prev = SUB_SHOP_LIST->cursor;
            rowStart = (prev / 5) * 5;
            rowEnd = rowStart + 5;
            if (count < rowEnd) {
                rowEnd = count;
            }
            if ((PAD_STATES[0]->rawRepeat & PAD_RIGHT) && ++SUB_SHOP_LIST->cursor >= rowEnd) {
                SUB_SHOP_LIST->cursor = rowStart;
            } else if ((PAD_STATES[0]->rawRepeat & PAD_LEFT) && --SUB_SHOP_LIST->cursor < rowStart) {
                SUB_SHOP_LIST->cursor = rowEnd - 1;
            } else if ((PAD_STATES[0]->rawRepeat & PAD_DOWN) && (SUB_SHOP_LIST->cursor += 5) >= SUB_SHOP_LIST->count) {
                if (SUB_SHOP_LIST->cursor < (SUB_SHOP_LIST->count - 1) / 5 * 5 + 5) {
                    SUB_SHOP_LIST->cursor = SUB_SHOP_LIST->count - 1;
                } else {
                    SUB_SHOP_LIST->cursor -= 5;
                }
            } else if (PAD_STATES[0]->rawRepeat & PAD_UP) {
                if (SUB_SHOP_LIST->cursor - 5 >= 0) {
                    SUB_SHOP_LIST->cursor -= 5;
                }
            }
            if (SUB_SHOP_LIST->top > SUB_SHOP_LIST->cursor) {
                SUB_SHOP_LIST->top -= 5;
            }
            if (SUB_SHOP_LIST->top + 9 < SUB_SHOP_LIST->cursor) {
                SUB_SHOP_LIST->top += 5;
            }
            if (SUB_SHOP_LIST->cursor != prev) {
                playSoundEffect(2);
            }
        }
        /* Dead store: action is recomputed at the top of the loop. gcc drops the
           store but keeps the start of the test (the original's field is unknown). */
        if (SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].count == 0) {
            action = 0;
        }
        SUB_adjustItemCount(PAD_R1, PAD_L1, &SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].count, SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].max);
    }
}

void SUB_runShop(s32 unused, s32 parentTask) {
    SUB_SHOP_RUNNING = 1;
    SUB_SHOP->paid = 0;
    SUB_SHOP->total = 0;
    SUB_SHOP_LIST = NULL;
    SUB_SHOP_ITEM.deckCount = 30;
    SUB_loadShopTextures();
    SUB_SHOP_WINDOWS[7] = NULL;
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_TOTAL_WINDOW, getCurrentTaskId());
    SUB_SHOP_WINDOWS[3] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_BITS_WINDOW, getCurrentTaskId());
    SUB_SHOP_WINDOWS[4] = (JpWindow *)waitFrames(0x7FFFFFFF);
    openKanjiPage(0xF, 0x1E3);
    SUB_SAVED_CURSOR = SUB_SAVED_TOP = 0;
    spawnTask(0, -1, 0, 0x1000, SUB_runStockGrid, 0, getCurrentTaskId(), 0, 0);
    do {
        waitFrames(1);
    } while (SUB_SHOP_RUNNING != 0);
    resumeTask(parentTask);
}

void SUB_runOpenedDetails(s32 parent) {
    u16 pressed;
    s32 viewing;

    spawnTask(0, -1, 0, 0x1000, SUB_loadCardPicture, parent);
    SUB_SHOP->fromOpened = 1;
    clearKanjiPage(0xF);
    if (SUB_SHOP_WINDOWS[7] == NULL) {
        spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_OPENED_TITLE_WINDOW, getCurrentTaskId());
        SUB_SHOP_WINDOWS[7] = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
    if (SUB_SHOP_WINDOWS[4] == NULL) {
        spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_BITS_WINDOW, getCurrentTaskId());
        SUB_SHOP_WINDOWS[4] = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_DETAILS_WINDOW, getCurrentTaskId());
    SUB_SHOP_WINDOWS[2] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_PICTURE_WINDOW, getCurrentTaskId());
    SUB_SHOP_WINDOWS[0] = (JpWindow *)waitFrames(0x7FFFFFFF);
    waitFrames(90);
    while (1) {
        viewing = 0;
        waitFrames(FRAME_INTERVAL);
        if (SUB_SHOP->loading != 0) {
            continue;
        }
        pressed = PAD_STATES[0]->rawPressed;
        if (pressed & (PAD_CIRCLE | PAD_CROSS)) {
            break;
        }
        if ((pressed & PAD_TRIANGLE) && SUB_SHOP_ITEM.type == 0) {
            viewing = 1;
            spawnTask(0, -1, 0, 0x1000, SUB_viewItemModel, SUB_SHOP_ITEM.count, getCurrentTaskId(), 0, 0);
            break;
        }
    }
    SUB_SHOP_WINDOWS[2]->state = 4;
    SUB_SHOP_WINDOWS[0]->state = 4;
    if (viewing == 1) {
        SUB_SHOP_WINDOWS[7]->state = 4;
        SUB_SHOP_WINDOWS[4]->state = 4;
    }
    waitFrames(30);
    waitFrames(30);
    if (viewing != 1) {
        spawnTask(0, -1, 0, 0x1000, SUB_runOpenedGrid, parent);
    }
    exitTask();
}

void SUB_runOpenedGrid(s32 parent) {
    u8 unused[0x250]; /* unused, but it is in the original stack frame */
    ShopList *list = NULL;
    s32 count;
    s32 prev;
    s32 rowStart;
    s32 rowEnd;

    if (SUB_SHOP_LIST == NULL) {
        list = SUB_buildShopList(SUB_SHOP_ITEM.deck, 12, -1);
        list->cursor = 0;
    }
    if (SUB_SHOP_WINDOWS[7] == NULL) {
        spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_OPENED_TITLE_WINDOW, getCurrentTaskId());
        SUB_SHOP_WINDOWS[7] = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_OPENED_HELP_WINDOW, getCurrentTaskId());
    SUB_SHOP_WINDOWS[5] = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SUB_OPENED_GRID_WINDOW, getCurrentTaskId());
    SUB_SHOP_WINDOWS[6] = (JpWindow *)waitFrames(0x7FFFFFFF);
    if (list->count >= SUB_SAVED_CURSOR) {
        list->cursor = SUB_SAVED_CURSOR;
        list->top = SUB_SAVED_TOP;
    }
    waitFrames(60);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
            SUB_SHOP_WINDOWS[5]->state = 4;
            SUB_SHOP_WINDOWS[6]->state = 4;
            waitFrames(30);
            waitFrames(30);
            SUB_SHOP_ITEM.type = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].type;
            SUB_SHOP_ITEM.index = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].index;
            SUB_SHOP_ITEM.count = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].count;
            SUB_SHOP_ITEM.max = SUB_SHOP_LIST->items[SUB_SHOP_LIST->cursor].max;
            SUB_SAVED_CURSOR = list->cursor;
            SUB_SAVED_TOP = list->top;
            spawnTask(0, -1, 0, 0x1000, SUB_SHOP_LIST->count != 0 ? SUB_runOpenedDetails : NULL, parent);
            exitTask();
        } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
            SUB_commitDeal(0);
            SUB_closeShop(1);
        }
        count = SUB_SHOP_LIST->count;
        if (count == 0) {
            continue;
        }
        prev = SUB_SHOP_LIST->cursor;
        rowStart = (prev / 5) * 5;
        rowEnd = rowStart + 5;
        if (count < rowEnd) {
            rowEnd = count;
        }
        if (PAD_STATES[0]->rawRepeat & PAD_RIGHT) {
            if (++SUB_SHOP_LIST->cursor >= rowEnd) {
                SUB_SHOP_LIST->cursor = rowStart;
            }
        } else if (PAD_STATES[0]->rawRepeat & PAD_LEFT) {
            if (--SUB_SHOP_LIST->cursor < rowStart) {
                SUB_SHOP_LIST->cursor = rowEnd - 1;
            }
        } else if (PAD_STATES[0]->rawRepeat & PAD_DOWN) {
            if ((SUB_SHOP_LIST->cursor += 5) >= SUB_SHOP_LIST->count) {
                if (SUB_SHOP_LIST->cursor < (SUB_SHOP_LIST->count - 1) / 5 * 5 + 5) {
                    SUB_SHOP_LIST->cursor = SUB_SHOP_LIST->count - 1;
                } else {
                    SUB_SHOP_LIST->cursor -= 5;
                }
            }
        } else if (PAD_STATES[0]->rawRepeat & PAD_UP) {
            if (SUB_SHOP_LIST->cursor - 5 >= 0) {
                SUB_SHOP_LIST->cursor -= 5;
            }
        }
        if (prev != SUB_SHOP_LIST->cursor) {
            playSoundEffect(2);
        }
        if (SUB_SHOP_LIST->top > SUB_SHOP_LIST->cursor) {
            SUB_SHOP_LIST->top -= 5;
        }
        if (SUB_SHOP_LIST->top + 9 < SUB_SHOP_LIST->cursor) {
            SUB_SHOP_LIST->top += 5;
        }
    }
}

s16 SUB_shopRandom(s16 seed) {
    if (seed >= 0) {
        SUB_RANDOM_SEED = seed;
    }
    return SUB_RANDOM_SEED = SUB_RANDOM_SEED * 3022 % 25763;
}

s16 *SUB_addPackCardIds(s16 *ids, s8 pack) {
    s32 i;

    for (i = SUB_PACK_RANGES[pack].start; i < SUB_PACK_RANGES[pack].start + SUB_PACK_RANGES[pack].count; i++) {
        if ((u8)(pack - 5) < 2) {
            if (pack == 5) {
                *ids++ = i + 110;
            } else {
                *ids++ = i + 153;
            }
        } else {
            *ids++ = i + SUB_PACK_RANGES[pack].base;
        }
    }
    return ids;
}

void SUB_rollStock(void) {
    s8 shop = SUB_SHOP->shopId;
    s32 i = 0;
    s32 count;

    switch (shop) {
    case 0:
        for (i = 0; i < 5; i++) {
            SUB_STOCK_IDS[i] = (u8)SUB_rollPackCard(0xFF, 1);
        }
        for (i = 5; i < 7; i++) {
            SUB_STOCK_IDS[i] = (u8)SUB_rollPackCard(0xFF, 2);
        }
        SUB_SHOP->packs[5].count = 1;
        break;
    case 1:
        for (i = 0; i < 6; i++) {
            SUB_SHOP->packs[i].count = 1;
        }
        for (i = 0; i < 4; i++) {
            SUB_STOCK_IDS[i] = (u8)SUB_rollPackCard(0xFF, 1);
        }
        for (i = 4; i < 6; i++) {
            SUB_STOCK_IDS[i] = (u8)SUB_rollPackCard(0xFF, 2);
        }
        SUB_STOCK_IDS[i++] = (u8)SUB_rollPackCard(0xFF, 0);
        break;
    case 2 ... 6:
        SUB_SHOP->packs[shop - 2].count = 1;
        SUB_SHOP->packs[5].count = 1;
        for (i = 0; i < 3; i++) {
            SUB_STOCK_IDS[i] = (u8)SUB_rollPackCard(shop - 2, 1);
        }
        for (i = 3; i < 5; i++) {
            SUB_STOCK_IDS[i] = (u8)SUB_rollPackCard(shop - 2, 2);
        }
        break;
    }
    count = i;
    if (shop == 0 && PLAYER_DATA(0).shops[0].starterStock == 1) {
        SUB_SHOP->digimonCards[16].count = 1;
        SUB_SHOP->digimonCards[36].count = 1;
        SUB_SHOP->digimonCards[65].count = 1;
        SUB_SHOP->optionCards[26].count = 1;
        SUB_SHOP->optionCards[34].count = 1;
        return;
    }
    for (i = 0; i < count; i++) {
        if (SUB_STOCK_IDS[i] < 110) {
            SUB_SHOP->digimonCards[SUB_STOCK_IDS[i]].count++;
        } else if ((SUB_STOCK_IDS[i] -= 110) < 43) {
            SUB_SHOP->optionCards[SUB_STOCK_IDS[i]].count++;
        } else if ((SUB_STOCK_IDS[i] -= 43) < 6) {
            SUB_SHOP->digivolveCards[SUB_STOCK_IDS[i]].count++;
        }
    }
}

void SUB_rollStockFromRanges(void) {
    s16 *ids = SUB_STOCK_IDS;
    u8 shop = SUB_SHOP->shopId;
    s32 i;
    s16 j;
    s16 swap;
    s16 pack;

    SUB_SHOP->packs[5].count = 1;
    switch ((s8)shop) {
    case 0:
        for (i = 0; i < 7; i++) {
            ids = SUB_addPackCardIds(ids, i);
        }
        *ids = -1;
        break;
    case 1:
        for (i = 0; i < 7; i++) {
            ids = SUB_addPackCardIds(ids, i);
        }
        for (i = 0; i < 6; i++) {
            SUB_SHOP->packs[i].count = 1;
        }
        *ids = -1;
        break;
    default:
        ids = SUB_addPackCardIds(ids, shop - 2);
        SUB_SHOP->packs[(s8)shop - 2].count = 1;
        *ids = -1;
        break;
    }
    SUB_STOCK_ID_COUNT = ids - SUB_STOCK_IDS;
    for (i = 0; i < SUB_STOCK_ID_COUNT; i++) {
        j = SUB_shopRandom(-1) % SUB_STOCK_ID_COUNT;
        swap = SUB_STOCK_IDS[i];
        SUB_STOCK_IDS[i] = SUB_STOCK_IDS[j];
        SUB_STOCK_IDS[j] = swap;
    }
    if ((s8)shop == 1) {
        pack = SUB_shopRandom(-1) % 6;
        SUB_STOCK_IDS[0] = SUB_PACK_RANGES[pack].base + SUB_shopRandom(-1) % SUB_PACK_RANGES[pack].start;
    }
    if (SUB_SHOP->shopId == 0 && PLAYER_DATA(0).shops[0].starterStock == 1) {
        SUB_SHOP->digimonCards[16].count = 1;
        SUB_SHOP->digimonCards[36].count = 1;
        SUB_SHOP->digimonCards[65].count = 1;
        SUB_SHOP->optionCards[26].count = 1;
        SUB_SHOP->optionCards[34].count = 1;
        return;
    }
    for (i = 0; i < 5; i++) {
        if (SUB_STOCK_IDS[i] - 110 < 0) {
            SUB_SHOP->digimonCards[SUB_STOCK_IDS[i]].count = 1;
        } else if (SUB_STOCK_IDS[i] - 43 < 0) {
            SUB_SHOP->optionCards[SUB_STOCK_IDS[i] - 110].count = 1;
        } else {
            SUB_SHOP->digivolveCards[SUB_STOCK_IDS[i] - 153].count = 1;
        }
    }
}

void SUB_initShop(void) {
    s32 i;
    s32 j;
    POLY_FT4 *poly;

    SUB_SHOP->itemChosen = 0;
    SUB_SHOP->loading = 0;
    for (i = 0; i < 4; i++) {
        SUB_SHOP->imageSlots[i].id = -1;
        SUB_SHOP->imageSlots[i].x = ((i & 1) << 5) + 0x180;
        SUB_SHOP->imageSlots[i].y = ((i & 2) << 5) + 0x175;
        SUB_SHOP->imageSlots[i].clutX = 0x180;
        SUB_SHOP->imageSlots[i].clutY = i + 0xF2;
    }
    for (i = 0; i < 2; i++) {
        DB(i).primSlots[2] = (s32)(SUB_SHOP->polys[i] = allocHeapBlock(0x78, 0x193));
    }
    for (j = 0; j < 2; j++) {
        poly = SUB_SHOP->polys[j];
        for (i = 0; i < 3; i++, poly++) {
            SetPolyFT4(poly);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
        }
    }
    SUB_SHOP->imageSlot = SUB_SHOP->imageSlots;
    SUB_GRID_CURSOR = KAW_createCursor(1, 20, 24, 5, 1);
}

void SUB_openSellShop(void) {
    s32 i;

    SUB_SHOP = allocHeapBlock(0xC00, 0x193);
    SUB_SHOP->mode = 13;
    SUB_initShop();
    for (i = 0; i < 110; i++) {
        SUB_SHOP->digimonCards[i].stock = PLAYER_DATA(0).cardCollection[i] & 0x7F;
    }
    for (i = 0; i < 43; i++) {
        SUB_SHOP->optionCards[i].stock = PLAYER_DATA(0).optionCollection[i] & 0x7F;
    }
    for (i = 0; i < 6; i++) {
        SUB_SHOP->digivolveCards[i].stock = PLAYER_DATA(0).digivolveCollection[i] & 0x7F;
    }
}

void SUB_openBuyShop(void) {
    s8 places[6] = { 0, 5, 9, 4, 10, 8 };
    s32 i;
    s32 flag;
    s32 word;
    s32 shift;
    s32 bit;

    SUB_SHOP = allocHeapBlock(0xC00, 0x193);
    SUB_SHOP->mode = 11;
    SUB_initShop();
    for (i = 0; i < 6; i++) {
        if (places[i] == PLAYER_DATA(0).area) {
            SUB_SHOP->shopId = i;
            if (SUB_SHOP->shopId != 0) {
                SUB_SHOP->shopId++;
            }
            i = 6;
        }
    }
    if (SUB_SHOP->shopId == 0) {
        flag = 13;
        word = flag / 32;
        shift = flag % 32;
        if (PLAYER_DATA(0).eventFlags[word] & (bit = 1 << shift)) {
            SUB_SHOP->shopId = 1;
        }
    }
    if (PLAYER_DATA(0).shops[SUB_SHOP->shopId].timer == -1) {
        PLAYER_DATA(0).shops[SUB_SHOP->shopId].timer = 0;
    }
    SUB_shopRandom(PLAYER_DATA(0).shops[SUB_SHOP->shopId].seeds[6][0]);
    for (i = 0; i < 110; i++) {
        SUB_SHOP->digimonCards[i].stock = PLAYER_DATA(0).cardCollection[i];
        SUB_SHOP->digimonCards[i].count = 0;
    }
    for (i = 0; i < 43; i++) {
        SUB_SHOP->optionCards[i].stock = PLAYER_DATA(0).optionCollection[i];
        SUB_SHOP->optionCards[i].count = 0;
    }
    for (i = 0; i < 6; i++) {
        SUB_SHOP->digivolveCards[i].stock = PLAYER_DATA(0).digivolveCollection[i];
        SUB_SHOP->digivolveCards[i].count = 0;
    }
    for (i = 0; i < 6; i++) {
        SUB_SHOP->packs[i].stock = 0;
        SUB_SHOP->packs[i].count = 0;
    }
    SUB_rollStock();
}
