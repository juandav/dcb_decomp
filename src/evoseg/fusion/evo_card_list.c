#include "common.h"
#include "game.h"
#include "dcb/evo_card_list.h"
#include "dcb/scene3d.h"
#include "dcb/model_anim.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/card_db.h"
#include "dcb/text.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/memcard.h"
#include "dcb/menu.h"
#include "dcb/sort.h"
#include "dcb/evoseg.h"
#include "dcb/evo_data.h"

typedef s32 (*EvoCardCompare)(s8 *, s8 *);

extern const char EVO_FMT_CARD_COUNT[];
extern const char EVO_STR_CARDS[];

void EVO_loadUnitTextures(void) {
    char path[24];
    u32 *pack;

    sprintf(path, "C:\\OBJECT\\unit.TIS", (s8)((SessionData *)SESSION_DATA)->areaSession->area);
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
}

char *EVO_SORT_LABELS[12] = {
    (char *)EVO_STR_NUMBER,
    "*a0 Fire",
    "*a1 Ice",
    "*a2 Nature",
    "*a3 Darkness",
    "*a4 Rare",
    "*a5 Option",
    "*a6 Option",
    "Level *e3",
    "Level *e4",
    "Level *e5",
    "Number of Cards that can be Fused.",
};

/* also drawCardInfo's heading */
const char EVO_STR_NUMBER[] = "Number";

Menu EVO_CARD_LIST_MENU = { 0, 0, { 0x6A, 0x2C, 0xD0, 0x6C }, 0, -1, 0, -1, 0xA, 0x61, 0x74, 0xC, 0, 301, 0x36, 1, 0, 0xC };
Menu EVO_SORT_MENU = { 0, 0, { 0x28, 0x3C, 0xCA, 0x70 }, 0, -1, 0, -1, 0xA, 0x56, 0xC0, 0xC, 0, 12, 0, 1, 0, 0xE };

void EVO_initFusionScene(void) {
    Graphics *camera;

    initScene3D(1);
    spawnTask(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    endTask(0x1B);
    spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
    waitFrames(2);
    camera = (Graphics *)&GRAPHICS;
    camera->rotX = 0;
    camera->rotY = 0;
    camera->rotZ = 0;
    camera->posX = 0;
    camera->posY = 0;
    camera->posZ = 0;
    camera->targetPitch = 0;
    camera->targetDistance = 20;
    camera->targetHeight = 0;
    camera->targetYaw = 0;
    camera->targetModel = -1;
    camera->snapCamera = 1;
    waitFrames(2);
}

void EVO_countSpareCards(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 0x12D; i++) {
        EVO_SPARE_CARD_COUNTS[i] = getOwnedCardCount(0, i);
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 0x12D; j++) {
            EVO_DECK_CARD_COUNTS[i][j] = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].inUse != 0) {
            for (j = 0; j < 30; j++) {
                EVO_DECK_CARD_COUNTS[i][getCardId(((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].type,
                                        ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].index)]++;
            }
        }
    }
    for (i = 0; i < 0x12D; i++) {
        for (j = 1; j < 3; j++) {
            if (EVO_DECK_CARD_COUNTS[0][i] < EVO_DECK_CARD_COUNTS[j][i]) {
                EVO_DECK_CARD_COUNTS[0][i] = EVO_DECK_CARD_COUNTS[j][i];
            }
        }
    }
    for (i = 0; i < 0x12D; i++) {
        EVO_SPARE_CARD_COUNTS[i] -= EVO_DECK_CARD_COUNTS[0][i];
    }
}

void EVO_initCardList(void) {
    s32 i;
    s32 j;

    for (i = 0, j = 0; j < 0xBF; j++, i++) {
        EVO_CARD_LIST[i] = (EvoCardInfo *)(DIGIMON_CARDS + j * 0x13C);
        EVO_CARDS_BY_ID[i] = EVO_CARD_LIST[i];
    }
    for (j = 0; j < 0x66; j++, i++) {
        EVO_CARD_LIST[i] = (EvoCardInfo *)(OPTION_CARDS + j * 0xE2);
        EVO_CARDS_BY_ID[i] = EVO_CARD_LIST[i];
    }
    for (j = 0; j < 8; j++, i++) {
        EVO_CARD_LIST[i] = (EvoCardInfo *)(DIGIVOLVE_CARDS + j * 0x70);
        EVO_CARDS_BY_ID[i] = EVO_CARD_LIST[i];
    }
    for (j = 0; j < 3; j++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->partners[j].cardId != 0) {
            EVO_CARD_LIST[((PlayerProfile *)PLAYER_PROFILES)->partners[j].cardId] =
                (EvoCardInfo *)&((PlayerProfile *)PLAYER_PROFILES)->partners[j].card[0];
        }
    }
}

s32 EVO_compareFireCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 0) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 0) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareIceCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 1) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 1) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareNatureCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 2) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 2) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareDarknessCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 3) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 3) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareRareCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 4) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 4) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareOptionCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = (*a)->type == 1;
    s32 kb = (*b)->type == 1;

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareDigivolveCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = (*a)->type == 2;
    s32 kb = (*b)->type == 2;

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareLevel0Cards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 0) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 0) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareLevel2Cards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 2) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 2) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareLevel3Cards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 3) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 3) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareSpareCounts(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = EVO_SPARE_CARD_COUNTS[(*a)->id];
    s32 kb = EVO_SPARE_CARD_COUNTS[(*b)->id];

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 (*EVO_SORT_COMPARES[12])(s8 *, s8 *) = {
    0,
    (EvoCardCompare)EVO_compareFireCards,
    (EvoCardCompare)EVO_compareIceCards,
    (EvoCardCompare)EVO_compareNatureCards,
    (EvoCardCompare)EVO_compareDarknessCards,
    (EvoCardCompare)EVO_compareRareCards,
    (EvoCardCompare)EVO_compareOptionCards,
    (EvoCardCompare)EVO_compareDigivolveCards,
    (EvoCardCompare)EVO_compareLevel0Cards,
    (EvoCardCompare)EVO_compareLevel2Cards,
    (EvoCardCompare)EVO_compareLevel3Cards,
    (EvoCardCompare)EVO_compareSpareCounts,
};

Rect16 EVO_RANK_UP_RECT = { 0xD, 0x4A, 0x48, 0x9 };

void EVO_drawSortMenu(UiWindow *w) {
    s32 x = w->originX;
    s32 z = w->z;
    s32 i;
    u8 unused[0x48]; /* unused, but it is in the original stack frame */

    for (i = 0; i < EVO_SORT_MENU.nrows; i++) {
        if (i < w->view.y / EVO_SORT_MENU.rowH) {
            continue;
        }
        if ((w->view.y + w->rect.h) / EVO_SORT_MENU.rowH < i) {
            break;
        }
        drawText(x, w->originY + i * EVO_SORT_MENU.rowH + 1, (s32)EVO_SORT_LABELS[i], 7, z);
    }
    updateMenuCursor(&EVO_SORT_MENU);
    if (EVO_SORT_MENU.active != 0 && (PAD_STATES[0]->pressed & 0x40)) {
        playMenuSound(1);
        EVO_CARD_LIST_MENU.row = 0;
        centerMenuOnCursor(&EVO_CARD_LIST_MENU);
        if (EVO_SORT_COMPARES[EVO_SORT_MENU.row] != NULL) {
            sortArray((s8 *)EVO_CARD_LIST, 0x12D, 4, EVO_SORT_COMPARES[EVO_SORT_MENU.row]);
        } else {
            EVO_initCardList();
        }
    }
}

void EVO_drawCardList(UiWindow *w) {
    char text[72];
    u8 *color;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 type;
    u8 palette;

    x = w->originX;
    z = w->z;
    x -= 10;
    if (EVO_TYPE_CHOICE.unkE == 0) {
        w->brightness = 0x80;
    } else {
        w->brightness = 0x40;
    }
    for (i = 0; i < EVO_CARD_LIST_MENU.nrows; i++) {
        if (i < w->view.y / EVO_CARD_LIST_MENU.rowH) {
            continue;
        }
        if ((w->view.y + w->rect.h) / EVO_CARD_LIST_MENU.rowH < i) {
            break;
        }
        y = w->originY + i * EVO_CARD_LIST_MENU.rowH + 1;
        type = EVO_CARD_LIST[i]->type;
        color = EVO_TEXT_COLORS[0].b;
        palette = 7;
        if (EVO_CARD_LIST[i]->fusionPoints == 0) {
            palette = 3;
        }
        if (type == 0 && (EVO_CARD_LIST[i]->attr & 0xF) > EVO_MAX_CARD_LEVEL) {
            palette = 3;
        }
        if (((PlayerProfile *)PLAYER_PROFILES)->cardCollection[EVO_CARD_LIST[i]->id] & 0x40) {
            if (EVO_SPARE_CARD_COUNTS[EVO_CARD_LIST[i]->id] == 0) {
                color = EVO_TEXT_COLOR_GREY.b;
            }
            drawTextColored(x + 0x3C, y, EVO_CARD_LIST[i]->name, color, palette, z);
            switch (type) {
            case 0:
                drawIconColored(x + 0x20, y, 0, EVO_CARD_LIST[i]->attr >> 4, color, z);
                if (palette == 3) {
                    drawIconColored(x + 0x2C, y, 0, (EVO_CARD_LIST[i]->attr & 0xF) + 0x10, EVO_TEXT_COLOR_RED.b, z);
                } else {
                    drawIconColored(x + 0x2C, y, 0, (EVO_CARD_LIST[i]->attr & 0xF) + 0x10, color, z);
                }
                break;
            case 1:
                drawIconColored(x + 0x20, y, 0, 5, color, z);
                break;
            case 2:
                drawIconColored(x + 0x20, y, 0, 6, color, z);
                break;
            }
        } else {
            palette = 9;
            drawTextColored(x + 0x20, y, "??", color, palette, z);
            drawTextColored(x + 0x3C, y, "------------------", color, palette, z);
        }
        sprintf(text, EVO_FMT_CARD_NUMBER, EVO_CARD_LIST[i]->id);
        drawTextColored(x + 0xA, y, text, color, palette, z);
        sprintf(text, EVO_FMT_CARD_COUNT, EVO_SPARE_CARD_COUNTS[EVO_CARD_LIST[i]->id]);
        drawTextColored(x + 0xB5, y, text, color, palette, z);
        drawTinyTextColored(x + 0xBD, y + 6, (u8 *)EVO_STR_CARDS, palette, color, z);
    }
    updateMenuCursor(&EVO_CARD_LIST_MENU);
}

const char EVO_FMT_CARD_NUMBER[] = "*s0%3.3d";

const char EVO_FMT_CARD_COUNT[] = "%d";

const char EVO_STR_CARDS[] = "Cards";
