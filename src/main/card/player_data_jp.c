#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/player_data.h"
#include "dcb/scroll_bg.h"
#include "dcb/card_db.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/window.h"
#include "dcb/text.h"
#include "dcb/sound_play.h"
#include "dcb/archive.h"
#include "dcb/vram_upload.h"
#include "dcb/prim.h"
#include "dcb/prim_util.h"
#include "dcb/frame_callback.h"

/* jp's player_data (player_data.c is us's and eu's): one object that also
   holds what us split out into game_flow, duel_launch and scroll_bg, and jp's
   own windows and menus */

/* the two sprites' texture and size: clut, colour mode, VRAM x and y, w, h */
typedef struct {
    /* 0x00 */ s32 clut;
    /* 0x04 */ s32 mode;
    /* 0x08 */ s32 x;
    /* 0x0C */ s32 y;
    /* 0x10 */ s32 w;
    /* 0x14 */ s32 h;
} WindowSpriteTexture;

WindowSpriteTexture WINDOW_SPRITE_TEXTURES[2] = {
    { 0x7FF8, 0, 0x340, 0x1E4, 0x20, 0x18 },
    { 0x7F38, 0, 0x354, 0x1BD, 0x1C, 0x18 },
};

/* the overlays' functions this module starts, while their overlay is loaded */
void SUB_openBuyShop(void); /* SUBSEG's */
void SUB_openSellShop(void);
void SUB_runShop();
void SAI_runWorldMap(); /* SAISEG's */
void NIS_enterDeckList(); /* NISSEG's deck editor */
s32 NIS_enterDeckName(char *text, s32 mode); /* NISSEG's text entry */
/* NISSEG's, the menus' tasks (0x801F117C is NIS_runMainMenuTask) */
void NIS_runMainMenuTask();
void NIS_runMenuE7D8();
void NIS_runMenuE7E4();
void NIS_returnToDeckList();
void NIS_openDeckMenu();
void NIS_runDeckKinds();
void NIS_runCardGrid();
void NIS_enterDeckListFromVs();
void NIS_enterDeckListForTrade();
void NIS_startVsDeckSelect();
void NIS_startTrade();
/* the executable's, the menus' tasks */
void runOptionScreen();
void runNewLoadMenu();
void openMemcardScreenToEditDecks();
void openMemcardScreenForVersus();
void openMemcardScreenAfterVersus();

s32 strcmp(const char *a, const char *b);

/* a menu's header icon and cancel task, and each choice's icon and task */
typedef struct {
    /* 0x0 */ s32 icon;
    /* 0x4 */ void (*cancel)();
} MenuHeader;
typedef struct {
    /* 0x0 */ s32 item;
    /* 0x4 */ void (*action)();
} MenuItem;
/* where the labels and icons of a group of menu items are in VRAM */
typedef struct {
    /* 0x0 */ u8 columns;
    /* 0x1 */ u8 width;
    /* 0x2 */ u16 x;
    /* 0x4 */ u16 y;
} MenuItemSheet;

MenuHeader MENU_HEADERS[12] = {
    { 0x35, NULL },
    { 3, NIS_runMainMenuTask },
    { 4, NIS_runMenuE7D8 },
    { 0x18, NIS_returnToDeckList },
    { 0x19, NIS_runMenuE7E4 },
    { 0x1A, NULL },
    { 0x1C, NULL },
    { 0x1D, NULL },
    { 0x1E, NULL },
    { -1, NIS_openDeckMenu },
    { 6, NULL },
    { 0x2E, NULL },
};
MenuItem MENU_ITEMS[27] = {
    { 0, runNewLoadMenu },
    { 1, openMemcardScreenForVersus },
    { 2, openMemcardScreenToEditDecks },
    { 3, runOptionScreen },
    { 6, NIS_runMenuE7E4 },
    { 7, NIS_runMenuE7E4 },
    { 8, NIS_enterDeckList },
    { 9, NIS_enterDeckList },
    { 0xA, NIS_enterDeckList },
    { 0x17, NIS_runDeckKinds },
    { 0x18, NULL },
    { 0x19, NULL },
    { 0x1A, NULL },
    { 0x12, NIS_runCardGrid },
    { 0x1C, NIS_runCardGrid },
    { 0x1D, NIS_runCardGrid },
    { 0x1E, NIS_runCardGrid },
    { 0x1F, NIS_runCardGrid },
    { 0x20, NIS_runCardGrid },
    { 0x21, NIS_runCardGrid },
    { 0x15, NIS_runCardGrid },
    { 0x22, NIS_runCardGrid },
    { 0x2F, NIS_enterDeckListFromVs },
    { 0x30, NIS_enterDeckListForTrade },
    { 0x31, NIS_startVsDeckSelect },
    { 0x32, NIS_startTrade },
    { 0x33, openMemcardScreenAfterVersus },
};
/* menus for openChoiceMenuFromList: the y, the header (MENU_HEADERS) and up
   to ten items (MENU_ITEMS), -1 after the last; NISSEG's VS mode and deck
   editor open them */
s8 MAIN_MENU[12] = { 0x32, 0, 0, 1, 2, -1 };
s8 D_8007E7D8[12] = { 0x32, 1, 4, 5, -1 };
s8 D_8007E7E4[12] = { 0x32, 2, 6, 7, 8, -1 };
s8 D_8007E7F0[12] = { 0x32, 3, 9, 0xA, 0xB, 0xC, -1 };
s8 DECK_KINDS_MENU[12] = { 0x32, 9, 0xD, 0xE, 0xF, 0x10, 0x11, 0x12, 0x13, 0x15, -1 };
s8 VS_MODE_MENU[12] = { 0x32, 0xB, 0x16, 0x17, 0x18, 0x19, 0x1A, -1 };
MenuItemSheet MENU_ITEM_SHEETS[16] = {
    { 9, 0x58, 0x300, 0x100 }, { 9, 0x68, 0x340, 0x100 }, { 9, 0x48, 0x200, 0x100 }, { 9, 0x50, 0x1C0, 0x100 },
    { 9, 0x68, 0x180, 0x100 }, { 9, 0x80, 0x140, 0x100 }, { 9, 0x48, 0x294, 0x100 }, { 9, 0x50, 0x280, 0x100 },
    { 9, 0x48, 0x19A, 0x100 }, { 9, 0x50, 0x160, 0x100 }, { 9, 0x58, 0x180, 0x1E8 }, { 9, 0x68, 0x140, 0x1EA },
    { 9, 0x54, 0x2DA, 0x16A }, { 9, 0x66, 0x2C0, 0x16A }, { 9, 0x58, 0x15A, 0x100 }, { 9, 0x68, 0x140, 0x100 },
};
s32 handleMenuInput(ChoiceMenu *menu);
s32 moveMenuCursorOut(ChoiceMenu *menu);
s32 moveMenuCursorIn(ChoiceMenu *menu);
s32 openMenuBar(ChoiceMenu *menu);
s32 dropMenuItems(ChoiceMenu *menu);
s32 gatherMenuItems(ChoiceMenu *menu);
s32 closeMenuBar(ChoiceMenu *menu);
s32 blinkMenuChoice(ChoiceMenu *menu);
/* by ChoiceMenu.state */
s32 (*MENU_STATE_HANDLERS[8])(ChoiceMenu *menu) = {
    handleMenuInput, moveMenuCursorOut, moveMenuCursorIn, openMenuBar,
    dropMenuItems, gatherMenuItems, closeMenuBar, blinkMenuChoice,
};

void initTexSprite(TexSprite *sprite, s16 clut, s16 x, s16 y, u16 vramX, u16 vramY, s32 w, s32 h, s32 z);
void switchScrollingBackground(s32 image);
void drawScrollingBackground(void);
void setPanelIcon(s32 icon);

void moveWindowSprite(POLY_FT4 *sprite, s32 x, s32 y);
void initWindowSprite(POLY_FT4 *sprite, s32 clut, s32 mode, s32 x, s32 y, s32 width, s32 height);
void stepWindowOpening(WindowTask *task);
void stepWindowClosing(WindowTask *task);

s32 *allocClearedWords(s32 count) {
    s32 *words;
    s32 *p;
    s32 i;

    words = allocHeapBlock(count * 4, 0x190);
    p = words;
    for (i = 0; i < count; i++) {
        *p++ = 0;
    }
    return words;
}

void allocAreaFlags(void) {
    ((SessionData *)SESSION_DATA)->areaSession->flags = allocClearedWords(0x11E);
}

void initPlayerData(void) {
    void *session;

    loadCardDatabase();
    PLAYER_PROFILES = allocPermanentHeapBlock(sizeof(PlayerProfile) * 2);
    SESSION_DATA = session = allocPermanentHeapBlock(sizeof(SessionData));
    ((SessionData *)SESSION_DATA)->areaSession = allocPermanentHeapBlock(sizeof(AreaSession));
    resetPlayerData();
}

void resetPlayerData(void) {
    PlayerProfile *profile;
    s32 player;
    s32 i;
    s32 j;
    s32 k;

    profile = (PlayerProfile *)PLAYER_PROFILES;
    PLAYER_DATA(0).unkF1E = 0;
    PLAYER_DATA(0).unkF1C = 0;
    for (i = 0; i < 10; i++) {
        PLAYER_DATA(0).eventFlags[i] = 0;
    }
    PLAYER_DATA(0).bits = 0;
    PLAYER_DATA(0).unk28_9 = 0;
    for (player = 0; player < 2; player++, profile++) {
        profile->name[0] = 0;
        profile->battleWins = 0;
        profile->battleLosses = 0;
        profile->versusWins = 0;
        profile->versusLosses = 0;
        profile->area = 0;
        profile->profileId = rand();
        profile->unkF1C = 1;
        profile->unkF1E = 0;
        profile->tradeUnlocked = 0;
        profile->unk28_13 = 0;
        profile->saveCount = 0;
        profile->hasTraded = 0;
        profile->unk28_12 = 0;
        profile->tamerRank = 0;
        profile->collectorRank = 0;
        profile->battleRank = 0;
        for (i = 0; i < 3; i++) {
            profile->attackCounts[i] = 0;
        }
        for (i = 0; i < 0x6E; i++) {
            for (j = 0; j < 3; j++) {
                profile->maxAttackPowers[i][j] = 0;
            }
            profile->cardWins[i] = 0;
            profile->cardLosses[i] = 0;
            profile->cardCollection[i] = 0;
            for (j = 0; j < 8; j++) {
                assignCardCopySerial(i, j, profile->cardSerials);
            }
        }
        for (i = 0; i < 0x2B; i++) {
            profile->optionCollection[i] = 0;
            for (j = 0; j < 8; j++) {
                assignCardCopySerial(i, j, profile->optionSerials);
            }
        }
        for (i = 0; i < 6; i++) {
            profile->digivolveCollection[i] = 0;
            for (j = 0; j < 8; j++) {
                assignCardCopySerial(i, j, profile->digivolveSerials);
            }
        }
        PLAYER_DATA(0).hallOfFameDeck.wins = 0;
        PLAYER_DATA(0).hallOfFameDeck.losses = 0;
        for (i = 0; i < 3; i++) {
            profile->savedDecks[i].inUse = 0;
            profile->savedDecks[i].wins = 0;
            profile->savedDecks[i].losses = 0;
            for (j = 0; j < 3; j++) {
                profile->savedDecks[i].attackCounts[j] = 0;
            }
        }
        profile->monoSound = 0;
        profile->skipBattleAnimation = 0;
        profile->playTime = 0;
    }
    ((SessionData *)SESSION_DATA)->areaSession->unk4F = 0;
    ((SessionData *)SESSION_DATA)->areaSession->pakState = 0;
    ((SessionData *)SESSION_DATA)->areaSession->unk44 = 0;
    ((SessionData *)SESSION_DATA)->otherPad = 0;
    for (k = 0; k < 7; k++) {
        for (j = 0; j < 2; j++) {
            PLAYER_DATA(0).shops[k].soldBits = 0;
            PLAYER_DATA(0).shops[k].timer = -1;
            PLAYER_DATA(0).shops[k].starterStock = 0;
            if (k < 2) {
                PLAYER_DATA(0).shops[k].period = 3;
            } else {
                PLAYER_DATA(0).shops[k].period = 5;
            }
            for (i = 0; i < 7; i++) {
                PLAYER_DATA(0).shops[k].seeds[i][j] = rand();
            }
        }
    }
    PLAYER_DATA(0).shops[0].starterStock = 1;
}

void openWindowTaskWindow(WindowTask *task) {
    Rect16 rect;
    Rect16 from;
    s16 view[4];
    WindowSpec *spec;

    spec = task->spec;
    rect.x = spec->to.x;
    rect.y = spec->to.y;
    rect.w = spec->to.w;
    rect.h = spec->to.h;
    from.x = 0;
    from.y = 0;
    from.w = 0;
    from.h = 0;
    view[0] = 0;
    view[1] = 0;
    view[2] = spec->to.w;
    view[3] = spec->to.h;
    openWindow(&task->window, &rect, (s32)&from, view, 0x10, 0xFF, spec->frames);
    from.x = spec->from.x;
    from.y = spec->from.y;
    from.w = spec->from.w;
    from.h = spec->from.h;
    animateWindowTo(&task->window, &from, -1, -1);
    task->phase = 1;
    task->state = 1;
}

void initWindowTaskSprites(WindowTask *task) {
    s8 i;
    s8 j;

    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[1] = (s32)task->sprites[i];
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            initWindowSprite((POLY_FT4 *)((Graphics *)&GRAPHICS)->buffers[i].primSlots[1] + j,
                             WINDOW_SPRITE_TEXTURES[j].clut, WINDOW_SPRITE_TEXTURES[j].mode,
                             WINDOW_SPRITE_TEXTURES[j].x, WINDOW_SPRITE_TEXTURES[j].y,
                             WINDOW_SPRITE_TEXTURES[j].w, WINDOW_SPRITE_TEXTURES[j].h);
        }
    }
    switch (task->spec->side) {
    case 0:
        task->x = -50;
        break;
    case 1:
        /* on the right: the sprites face the other way */
        for (j = 0; j < 2; j++) {
            u8 left;
            u8 right;

            left = task->sprites[0][j].u0;
            right = task->sprites[0][j].u1;
            for (i = 0; i < 2; i++) {
                task->sprites[i][j].u0 = task->sprites[i][j].u2 = right;
                task->sprites[i][j].u1 = task->sprites[i][j].u3 = left;
            }
            task->sprites[j][0].x1 += 4;
            task->sprites[j][0].x3 += 4;
        }
        task->x = 0x142;
        break;
    default:
        goto move;
    }
    task->y = task->spec->from.y - 6;
move:
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            moveWindowSprite(&task->sprites[i][j], task->x, task->y);
        }
    }
}

void moveWindowSprite(POLY_FT4 *sprite, s32 x, s32 y) {
    x -= sprite->x0;
    y -= sprite->y0;
    sprite->x0 += x;
    sprite->y0 += y;
    sprite->x1 += x;
    sprite->y1 += y;
    sprite->x2 += x;
    sprite->y2 += y;
    sprite->x3 += x;
    sprite->y3 += y;
}

void initWindowSprite(POLY_FT4 *sprite, s32 clut, s32 mode, s32 x, s32 y, s32 width, s32 height) {
    s32 uv[2];
    s32 u;
    s32 v;
    s32 w;
    s32 h;

    setlen(sprite, 9);
    setcode(sprite, 0x2C);
    sprite->clut = clut;
    sprite->tpage = ((mode & 3) << 7) | ((y & 0x100) >> 4) | ((x & (-64 << mode) & 0x3FF) >> 6) | ((y & 0x200) << 2);
    w = (u8)width;
    h = (u8)height;
    /* the texture's u in its page: 4 texels a VRAM word in 4-bit mode */
    u = (mode != 0 ? x * 2 : x * 4) & 0xFF;
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    uv[0] = u;
    uv[1] = u + w;
    v = (u8)y;
    if (uv[1] >= 0x100) {
        uv[1] = 0xFF;
    }
    sprite->u0 = u;
    sprite->v0 = v;
    sprite->u1 = uv[1];
    sprite->v1 = v;
    sprite->u2 = u;
    sprite->v2 = v + h;
    sprite->u3 = uv[1];
    sprite->v3 = v + h;
    sprite->x0 = 0;
    sprite->y0 = 0;
    sprite->x1 = w;
    sprite->y1 = 0;
    sprite->x2 = 0;
    sprite->y2 = h;
    sprite->x3 = w;
    sprite->y3 = h;
}

/* slides the window's two sprites out to the side; false once they are out */
s32 slideWindowSpritesOut(WindowTask *task) {
    s32 speed;
    s32 moving;
    s32 dy;
    s32 dx;
    s32 right;
    s8 i;

    speed = task->spec->frames;
    moving = 1;
    dy = 0;
    dx = 0;
    switch (task->spec->side) {
    case 0:
        dx = 17;
        dy = 2;
        task->x -= speed;
        if (task->x < -49) {
            task->x = -50;
            moving = 0;
        }
        break;
    case 1:
        dx = -11;
        dy = 2;
        right = task->spec->to.x + task->spec->to.w;
        task->x += speed;
        if (task->x >= right + 28) {
            task->x = right + 28;
            moving = 0;
        }
        break;
    }
    for (i = 0; i < 2; i++) {
        moveWindowSprite(&task->sprites[i][0], task->x, task->y);
        moveWindowSprite(&task->sprites[i][1], task->x + dx, task->y + dy);
    }
    return moving;
}

/* slides them back in, to the window's corner; false once they are there */
s32 slideWindowSpritesIn(WindowTask *task) {
    s32 speed;
    s32 moving;
    s32 dy;
    s32 dx;
    s32 limit;
    s8 i;

    speed = task->spec->frames;
    moving = 1;
    dy = 0;
    dx = 0;
    switch (task->spec->side) {
    case 0:
        dx = 17;
        dy = 2;
        task->x += speed;
        limit = task->spec->from.x - 27;
        if (task->x >= limit) {
            task->x = limit;
            moving = 0;
        }
        break;
    case 1:
        dx = -11;
        dy = 2;
        limit = task->spec->to.x + task->spec->to.w;
        task->x -= speed;
        if (task->x <= limit - 5) {
            task->x = limit - 5;
            moving = 0;
        }
        break;
    }
    for (i = 0; i < 2; i++) {
        moveWindowSprite(&task->sprites[i][0], task->x, task->y);
        moveWindowSprite(&task->sprites[i][1], task->x + dx, task->y + dy);
    }
    return moving;
}

/* the task that runs a WindowSpec's window, spawned with the parent task to
   resume with it */
void runWindowTask(WindowSpec *spec, s32 parent) {
    WindowTask *task;
    s32 moving;
    Rect16 unused; /* unused, but it is in the original stack frame */

    task = allocTaskHeapBlock(sizeof(WindowTask));
    task->spec = spec;
    openWindowTaskWindow(task);
    initWindowTaskSprites(task);
    resumeTask(parent, task);
    do {
        waitFrames(1);
        moving = slideWindowSpritesIn(task);
        AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)&task->sprites[FRAME_BUFFER_INDEX][0]);
        if (task->phase == 1) {
            AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)&task->sprites[FRAME_BUFFER_INDEX][1]);
        }
        task->spec->update(&task->state);
        if (moving == 0) {
            switch (task->state) {
            case 1:
                stepWindowOpening(task);
                break;
            case 2:
            case 4:
                stepWindowClosing(task);
                break;
            }
        }
    } while (task->state != 3 && task->state != 4);
    if (task->state == 4) {
        do {
            waitFrames(1);
            AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)&task->sprites[FRAME_BUFFER_INDEX][0]);
            if (task->phase == 1) {
                AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)&task->sprites[FRAME_BUFFER_INDEX][1]);
            }
            stepWindowClosing(task);
            if (task->phase == 1 && slideWindowSpritesOut(task) == 0) {
                task->state = 3;
            }
        } while (task->state != 3);
    }
    waitFrames(2);
    exitTask();
}

/* the window's opening: wide first, then tall */
void stepWindowOpening(WindowTask *task) {
    Rect16 rect;

    if (task->window.animDone != 0) {
        switch (task->phase) {
        case 6:
            playSoundEffectAtVolume(3, 30);
        case 1:
            playSoundEffectAtVolume(3, 30);
            rect.x = task->spec->to.x;
            rect.y = task->spec->to.y;
            rect.w = task->spec->to.w;
            rect.h = task->spec->from.h;
            task->phase = 2;
            animateWindowTo(&task->window, &rect, 0, 0);
            break;
        case 2:
        case 5:
            rect.x = task->spec->to.x;
            rect.y = task->spec->to.y;
            rect.w = task->spec->to.w;
            rect.h = task->spec->to.h;
            task->phase = 3;
            animateWindowTo(&task->window, &rect, 0, 0);
            break;
        }
    }
    drawWindow(&task->window, task->spec->draw, 0);
}

/* and its closing, the other way round */
void stepWindowClosing(WindowTask *task) {
    Rect16 rect;

    if (task->window.animDone != 0) {
        switch (task->phase) {
        case 3:
            playSoundEffectAtVolume(4, 30);
            rect.x = task->spec->to.x;
            rect.y = task->spec->to.y;
            rect.w = task->spec->to.w;
            rect.h = task->spec->from.h;
            task->phase = 5;
            animateWindowTo(&task->window, &rect, 0, 0);
            break;
        case 2:
        case 5:
            rect.x = task->spec->from.x;
            rect.y = task->spec->from.y;
            rect.w = task->spec->from.w;
            rect.h = task->spec->from.h;
            task->phase = 6;
            animateWindowTo(&task->window, &rect, 0, 0);
            break;
        case 6:
            task->phase = 1;
            break;
        }
    }
    if (task->phase != 1) {
        drawWindow(&task->window, task->spec->draw, 0);
    }
}

/* loads the area's model PAK (C:\mNN.pak) into the area state */
void loadAreaPakTask(void) {
    char path[0x18];
    s32 area;
    void *reserve;

    area = ((SessionData *)SESSION_DATA)->areaSession->area + 1;
    if (((SessionData *)SESSION_DATA)->areaSession->unk44 == 1) {
        ((SessionData *)SESSION_DATA)->areaSession->unk44 = 0;
        loadScrollingBackground(0xE, 1);
    }
    if (area < 10) {
        sprintf(path, "C:\\m0%d.pak", area);
    } else {
        sprintf(path, "C:\\m%d.pak", area);
    }
    /* keep 0x77002 bytes of the largest free block for the PAK */
    reserve = allocTaskHeapBlock(getLargestFreeHeapBlock() - 0x77002);
    ((SessionData *)SESSION_DATA)->areaSession->pakState = 1;
    ((SessionData *)SESSION_DATA)->areaSession->pak = loadFileTagged(path, getCurrentTaskId(), 0x190);
    ((SessionData *)SESSION_DATA)->areaSession->pakState = 2;
    freeHeapBlock(reserve);
}

void startAreaPakLoad(void) {
    if (((SessionData *)SESSION_DATA)->areaSession->pakState == 0) {
        spawnTask(0, -1, 0, 0x1000, loadAreaPakTask, 1, 0, 0, 0);
    }
}

/* from SAISEG: runs SUBSEG's screens, then goes back to SAISEG */
s32 runSubsegFromArea(s8 mode) {
    waitFrames(0x28);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\subseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    if (mode == 0) {
        SUB_openBuyShop();
    } else {
        SUB_openSellShop();
    }
    waitFrames(0x1E);
    spawnTask(0, -1, 0, 0x1000, SUB_runShop, 0, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    return 1;
}

/* a frame callback: the prims of slot 0 go in at OT entry 30 */
void addPrimSlot0(FrameBuffer *fb) {
    addPrim(&fb->ot[30], fb->primSlots[0]);
}

void uploadAreaPakTims(void) {
    uploadTimList(findPakChunk(SCROLLING_BACKGROUND->pak, 5, 6));
}

/* from SAISEG: the deck editor in NISSEG */
void openDeckEditorFromArea(void) {
    ((SessionData *)SESSION_DATA)->areaSession->unk4F = 1;
    waitFrames(2);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\nisseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x800, NIS_enterDeckList, 1, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
    exitTask();
}

/* and back from it to the area */
void returnToAreaFromDeckEditor(void) {
    ((SessionData *)SESSION_DATA)->areaSession->unk4F = 0;
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    uploadAreaPakTims();
    setBackgroundScrollMode(0);
    spawnTask(0, -1, 0, 0x800, SAI_runWorldMap, 1, 0, 0, 0);
    exitTask();
}

/* asks for a password with NISSEG's text entry: the index of the one given,
   -1 for none of them, -2 if cancelled */
s8 enterPassword(void) {
    char text[0x10];
    char passwords[12][14] = {
        "ＳＨＩＳＹＯ", "ＪＩ２ＭＯＮ", "ＭＧＮＤＲＡ", "ＧＩＧＡＤＲ", "ＰＡＮＪＹＡ", "ＢＵＲＡＫＩ",
        "ＶＡＮＤＥＭ", "ＭＴＲＥＴＥ", "ＨＯＵＯＵＭ", "ＨーＫＡＢＵ", "ＶーＤＯＲＡ", "ＭＴＲＧＲＹ",
    };
    s8 i = 0;

    waitFrames(2);
    openKanjiPage(0xF, 0x1E3);
    clearKanjiPage(0xF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\nisseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    sprintf(text, "");
    if (NIS_enterDeckName(text, 1) > 0) {
        for (; i < 12; i++) {
            if (strcmp(text, passwords[i]) == 0) {
                break;
            }
        }
        if (i == 12) {
            i = -1;
        }
    } else {
        i = -2;
    }
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    clearKanjiPage(0xF);
    closeKanjiPage(0xF);
    return i;
}

/* starts the task of the menu's choice (the last one if cancelled) with its
   argument, and ends the current task */
void startChoiceMenuAction(ChoiceMenu *menu) {
    s32 choice;
    s32 arg;

    choice = 10;
    if (menu->result >= 0) {
        choice = menu->choice;
    }
    if (menu->actions[choice] != NULL) {
        arg = 0;
        if (menu->arg != NULL) {
            arg = *menu->arg;
        }
        spawnTask(0, -1, 0, 0x1000, menu->actions[choice], arg);
        exitTask();
    }
}

void setMenuBarTexture(ChoiceMenu *menu, TexSprite *sprite) {
    POLY_FT4 *poly;
    s32 i;

    for (i = 0; i < 2; i++) {
        poly = &menu->bars[i];
        poly->clut = sprite->clut;
        poly->tpage = sprite->tpage;
        setPrimQuadUvRect((u8 *)poly, sprite->uv.x, sprite->uv.y, sprite->uv.w, sprite->uv.h);
    }
}

void addChoiceMenuItem(ChoiceMenu *menu, s32 item, void (*action)()) {
    s32 groupStarts[8] = { 0, 0x12, 0x27, 0x2F, 0x39, 0x3C, 0x3D, 0x3F };
    TexSprite *sprite;
    MenuItemSheet *sheet;
    s32 index;

    menu->actions[menu->count] = action;
    sprite = &menu->items[menu->count * 2];
    index = ((item >= 0x12) + (item >= 0x27) + (item >= 0x2F) + (item >= 0x39) + (item >= 0x3C) + (item >= 0x3D) + (item >= 0x3F)) * 2;
    item -= groupStarts[index / 2];
    sheet = &MENU_ITEM_SHEETS[index];
    initTexSprite(sprite++, 0x7FB8, -sheet->width, menu->cursor.y + 1, sheet->x + item / sheet->columns * (sheet->width >> 2),
                  sheet->y + item % sheet->columns * 19, sheet->width, 19, menu->count + 1);
    sheet++;
    initTexSprite(sprite, 0x7F38, -sheet->width, menu->cursor.y, sheet->x + item / sheet->columns * (sheet->width >> 2),
                  sheet->y + item % sheet->columns * 21, sheet->width, 21, menu->count + 1);
    if (menu->count == 0) {
        menu->barFullWidth = sprite->uv.w;
        setMenuBarTexture(menu, sprite);
    }
    menu->count++;
}

void openChoiceMenu(ChoiceMenu *menu, s32 icon, s32 y, void (*cancel)(), s32 *arg) {
    TexSprite *sprite;
    POLY_FT4 *poly;
    s32 i;

    sprite = &menu->cursor;
    menu->count = 0;
    menu->choice = 0;
    menu->result = 0;
    menu->barWidth = 20;
    menu->state = 3;
    menu->actions[10] = cancel;
    menu->arg = arg;
    for (i = 0; i < 2; i++) {
        poly = &menu->bars[i];
        SetPolyFT4(poly);
        poly->r0 = 0x80;
        poly->g0 = 0x80;
        poly->b0 = 0x80;
    }
    initTexSprite(sprite++, 0x7FF8, -51, y, 0x340, 0x1E6, 0x1F, 21, 0);
    initTexSprite(sprite, 0x7F79, -20, y, 0x350, 0x1BD, 20, 21, 0);
    /* bit 15 set: the panel keeps showing the icon it has */
    SCROLLING_BACKGROUND->requestedIcon = icon;
    if ((s16)icon >= 0) {
        SCROLLING_BACKGROUND->icon = -1;
    }
}

void openChoiceMenuFromList(ChoiceMenu *menu, s8 *list, s32 *arg) {
    s32 i;

    openChoiceMenu(menu, MENU_HEADERS[list[1]].icon, list[0], MENU_HEADERS[list[1]].cancel, arg);
    for (i = 1; i < 11 && *(list + i + 1) >= 0; i++) {
        addChoiceMenuItem(menu, MENU_ITEMS[*(list + i + 1)].item, MENU_ITEMS[*(list + i + 1)].action);
    }
}

/* MENU_STATE_HANDLERS: each returns what runChoiceMenu draws, 0 to 3, or -1 once
   the menu is closed */

s32 handleMenuInput(ChoiceMenu *menu) {
    s32 choice;

    choice = menu->choice;
    if (menu->actions[10] != NULL && (PAD_STATES[0]->rawPressed | PAD_STATES[((SessionData *)SESSION_DATA)->otherPad]->rawPressed) & 0x40) {
        menu->result = -1;
        menu->state = 5;
        playSoundEffectAtVolume(1, 0x50);
    } else if ((PAD_STATES[0]->rawPressed | PAD_STATES[((SessionData *)SESSION_DATA)->otherPad]->rawPressed) & 0x20 &&
               menu->actions[menu->choice] != NULL) {
        playSoundEffectAtVolume(0, 0x50);
        menu->result = 1;
        menu->state = 7;
        menu->timer = 0x20;
    } else {
        if ((PAD_STATES[0]->rawRepeat | PAD_STATES[((SessionData *)SESSION_DATA)->otherPad]->rawRepeat) & 0x4000 &&
            ++menu->choice >= menu->count) {
            menu->choice = 0;
        } else if ((PAD_STATES[0]->rawRepeat | PAD_STATES[((SessionData *)SESSION_DATA)->otherPad]->rawRepeat) & 0x1000) {
            if (--menu->choice < 0) {
                menu->choice = menu->count - 1;
            }
        }
        if (menu->choice != choice) {
            playSoundEffectAtVolume(2, 0x50);
            menu->state = 1;
        }
    }
    return menu->state == 1 ? 3 : 2;
}

s32 moveMenuCursorOut(ChoiceMenu *menu) {
    menu->cursor.x -= 16;
    if (menu->cursor.x < -51) {
        menu->cursor.x = -51;
        menu->cursor.y = menu->bar.y + menu->choice * 21;
        menu->state = 2;
    }
    return 3;
}

s32 moveMenuCursorIn(ChoiceMenu *menu) {
    menu->cursor.x += 16;
    if (menu->cursor.x > 0) {
        menu->cursor.x = 0;
        menu->state = 0;
    }
    return 2;
}

s32 blinkMenuChoice(ChoiceMenu *menu) {
    if (--menu->timer <= 0) {
        menu->state = 5;
    }
    menu->blink += 0x18;
    return 2;
}

s32 openMenuBar(ChoiceMenu *menu) {
    if (menu->cursor.x < 0) {
        menu->cursor.x += 8;
        if (menu->cursor.x > 0) {
            menu->cursor.x = 0;
        }
        return 0;
    }
    if (menu->barWidth < menu->barFullWidth) {
        menu->barWidth += 8;
    } else {
        menu->barWidth = menu->barFullWidth;
        menu->state++;
    }
    return 1;
}

s32 dropMenuItems(ChoiceMenu *menu) {
    TexSprite *bar;
    TexSprite *sprite;
    s32 i;
    s32 done;

    bar = &menu->bar;
    sprite = menu->items;
    done = 0;
    for (i = 0; i < menu->count * 2; i++, sprite++) {
        sprite->x = bar->x;
        if (sprite->y < bar->y + i / 2 * 21) {
            sprite->y += 8;
            if (sprite->y > bar->y + i / 2 * 21) {
                sprite->y = bar->y + i / 2 * 21;
            }
        } else {
            done++;
        }
    }
    if (done == menu->count * 2) {
        menu->state = 0;
    }
    return 2;
}

s32 gatherMenuItems(ChoiceMenu *menu) {
    TexSprite *cursor;
    TexSprite *sprite;
    s32 i;
    s32 done;

    cursor = &menu->cursor;
    sprite = menu->items;
    done = 0;
    for (i = 0; i < menu->count * 2; i++, sprite++) {
        sprite->z = menu->choice - i / 2 >= 0 ? menu->choice - i / 2 + 1 : i / 2 - menu->choice + 1;
        /* each item moves 8 a frame to the cursor's row */
        if (sprite->y == cursor->y) {
            done++;
        } else if (sprite->y > cursor->y && (sprite->y -= 8) < cursor->y) {
            sprite->y = cursor->y;
        } else if (sprite->y < cursor->y && (sprite->y += 8) > cursor->y) {
            sprite->y = cursor->y;
        }
    }
    if (done == menu->count * 2) {
        menu->state = 6;
        cursor[1].y = cursor->y; /* the bar, right after the cursor */
        setMenuBarTexture(menu, &menu->items[menu->choice * 2 + 1]);
        if (SCROLLING_BACKGROUND->icon >= -1) {
            SCROLLING_BACKGROUND->state = 2;
        }
    }
    return 2;
}

s32 closeMenuBar(ChoiceMenu *menu) {
    if (menu->barWidth >= 21) {
        menu->barWidth -= 20;
        return 1;
    }
    menu->cursor.x -= 8;
    if (menu->cursor.x < -51) {
        return -1;
    }
    return 0;
}

/* a frame of the menu: its result once it closes (-1 cancelled), else 0 */
s8 runChoiceMenu(ChoiceMenu *menu) {
    TexSprite *sprite;
    s32 view;
    s32 brightness;
    s32 itemBrightness;
    s32 blink;
    s32 i;
    POLY_FT4 *poly;

    sprite = &menu->cursor;
    view = MENU_STATE_HANDLERS[menu->state](menu);
    if (view < 0) {
        return menu->result;
    }
    /* the choice blinks between 0x48 and 0xC0 */
    blink = menu->blink;
    menu->blink = blink + 8;
    if ((s8)menu->blink >= 0) {
        brightness = blink + 0x48;
    } else {
        brightness = 0xC0 - (menu->blink & 0x7F);
    }
    drawTexturedSprite(sprite->x, sprite->y, &sprite->uv, sprite->tpage, sprite->clut, sprite->z, 0x80, -1);
    sprite++;
    switch (view) {
    case 0:
        /* the bar follows the cursor */
        sprite->x = sprite[-1].x + 24;
        drawTexturedSprite(sprite->x, sprite->y, &sprite->uv, sprite->tpage, sprite->clut, sprite->z, 0x80, -1);
        break;
    case 1:
        poly = &menu->bars[FRAME_BUFFER_INDEX];
        setPrimQuadRect(poly, sprite->x, sprite->y, menu->barWidth, 21);
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[sprite->z], (s32)poly);
        break;
    case 2:
    case 3:
        /* the labels, or the choice's icon instead of its label */
        for (i = 0; i < menu->count; i++) {
            if (view != 3 && menu->choice == i) {
                sprite = &menu->items[i * 2 + 1];
                itemBrightness = brightness;
            } else {
                sprite = &menu->items[i * 2];
                itemBrightness = 0x80;
            }
            drawTexturedSprite(sprite->x, sprite->y, &sprite->uv, sprite->tpage, sprite->clut, sprite->z, itemBrightness, -1);
        }
        break;
    default:
        return -1;
    }
    return 0;
}

void initTexSprite(TexSprite *sprite, s16 clut, s16 x, s16 y, u16 vramX, u16 vramY, s32 w, s32 h, s32 z) {
    sprite->x = x;
    sprite->y = y;
    sprite->uv.x = (vramX * 4) & 0xFC;
    sprite->uv.y = vramY & 0xFF;
    sprite->uv.w = w;
    sprite->uv.h = h;
    sprite->tpage = getTPage(0, 0, vramX, vramY);
    sprite->clut = clut;
    sprite->z = z;
}

/* the panel's icon, from where each one is in VRAM */
void setPanelIcon(s32 icon) {
    TexSprite *sprite;
    s32 n;
    s32 x;
    s32 y;
    s32 clut;
    POLY_FT4 *poly;

    sprite = &SCROLLING_BACKGROUND->panel[2];
    if (icon < 0x18) {
        x = icon / 12 * 32 + 0x380;
        y = icon % 12 * 21 + 0x100;
    } else if (icon < 0x1F) {
        n = icon - 0x18;
        x = n / 4 * 32 + 0x200;
        y = n % 4 * 21 + 0x1AB;
    } else if (icon < 0x20) {
        x = 0x340;
        y = 0x13F;
    } else if (icon < 0x2E) {
        n = icon - 0x20;
        x = n / 8 * 32 + 0x140;
        y = n % 8 * 21 + 0x100;
    } else if (icon < 0x35) {
        n = icon - 0x2E;
        x = n / 9 * 32 + 0x240;
        y = n % 9 * 21 + 0x100;
    } else if (icon < 0x39) {
        n = icon - 0x35;
        x = n / 2 * 32 + 0x2C0;
        y = n % 2 * 21 + 0x140;
    } else if (icon < 0x3C) {
        n = icon - 0x39;
        x = n / 5 * 32 + 0x140;
        y = n % 5 * 21 + 0x1AC;
    } else if (icon < 0x3D) {
        n = icon - 0x3C;
        x = n / 5 * 32 + 0x140;
        y = n % 5 * 21 + 0x1EA;
    } else if (icon < 0x3E) {
        n = icon - 0x3D;
        x = n / 5 * 32 + 0x140;
        y = n % 5 * 21 + 0x17E;
    } else {
        n = icon - 0x3E;
        x = n / 5 * 32 + 0x3E0;
        y = n % 5 * 21 + 0x160;
    }
    SCROLLING_BACKGROUND->icon = icon;
    clut = 0x7F38;
    if (icon == 0xB) {
        clut = 0x7F78;
    }
    /* at (20, 21); icons 0x1F and 0x3C are narrower */
    initTexSprite(sprite, clut, 20, 21, x, y, (icon == 0x3C || icon == 0x1F) ? 0x70 : 0x80, 21, 0);
    if (sprite->uv.x + sprite->uv.w >= 0x100) {
        sprite->uv.x -= sprite->uv.x + sprite->uv.w - 0xFF;
    }
    for (n = 0; n < 2; n++) {
        poly = &SCROLLING_BACKGROUND->bars[n];
        poly->clut = sprite->clut;
        poly->tpage = sprite->tpage;
        setPrimQuadUvRect((u8 *)poly, sprite->uv.x, sprite->uv.y, sprite->uv.w, sprite->uv.h);
    }
}

/* the panel's icon slides out; true once it is out */
s32 slidePanelIconOut(ScrollingBackground *bg) {
    TexSprite *icon;

    icon = &bg->panel[2];
    icon->x -= 8;
    if (icon->x < 20) {
        icon->x = 20;
        return 1;
    }
    return 0;
}

void closePanel(ScrollingBackground *bg) {
    TexSprite *panel;

    panel = bg->panel;
    if (slidePanelIconOut(bg) != 0) {
        if (bg->panel[0].x < -0xCC) {
            bg->panel[0].x = -0xCC;
            bg->state = 0;
        } else {
            panel->x -= 8;
        }
    }
}

void openPanel(ScrollingBackground *bg) {
    TexSprite *sprite;
    s16 state;

    sprite = bg->panel;
    if (bg->icon != bg->requestedIcon) {
        if (bg->requestedIcon == -1) {
            bg->icon = bg->requestedIcon;
        }
        state = 2;
        if (bg->requestedIcon == -1) {
            state = 3;
        }
        bg->state = state;
        return;
    }
    if (sprite->x < 0) {
        sprite->x += 8;
        if (sprite->x > 0) {
            sprite->x = 0;
        }
    } else {
        sprite += 2;
        if (sprite->x < 0x80) {
            sprite->x += 8;
        } else {
            sprite->x = 0x80;
        }
    }
}

/* the panel's frame: how many of its sprites to draw */
s32 updatePanel(void) {
    ScrollingBackground *bg;
    TexSprite *panel;

    bg = SCROLLING_BACKGROUND;
    panel = bg->panel;
    switch (bg->state) {
    case 0:
        if (bg->icon == bg->requestedIcon) {
            break;
        }
        if (bg->requestedIcon == -1) {
            bg->icon = bg->requestedIcon;
            bg->state = 3;
            break;
        }
        if (bg->requestedIcon <= 0) {
            setPanelIcon(-bg->requestedIcon);
            SCROLLING_BACKGROUND->icon *= -1;
        } else {
            setPanelIcon(bg->requestedIcon);
        }
        bg->state = 1;
    case 1:
        openPanel(bg);
        break;
    case 2:
        if (slidePanelIconOut(bg) != 0) {
            bg->state = 0;
        }
        break;
    case 3:
        closePanel(bg);
        break;
    }
    panel[1].x = panel[0].x + 0xAC;
    return panel[2].x != 20 ? 1 : 2;
}

/* a frame callback: the panel, then the background */
void renderScrollingBackground(FrameBuffer *fb, s32 index) {
    TexSprite *sprite;
    POLY_FT4 *poly;
    s32 count;
    s32 i;

    sprite = SCROLLING_BACKGROUND->panel;
    count = updatePanel();
    for (i = 0; i < count; i++, sprite++) {
        drawTexturedSprite(sprite->x, sprite->y, &sprite->uv, sprite->tpage, sprite->clut, sprite->z + 1, 0x80, -1);
    }
    if (count == 1) {
        poly = &SCROLLING_BACKGROUND->bars[index];
        setPrimQuadRect(poly, sprite->x, sprite->y, sprite[1].x, 21);
        AddPrim((s32 *)(&fb->ot[sprite->z] + 1), (s32)poly);
    }
    drawScrollingBackground();
}

void loadScrollingBackground(s32 panelY, s8 image) {
    void *reserve;
    TexSprite *sprite;
    POLY_FT4 *poly;
    s32 i;

    freeHeapBlocksByTag(0x1EB);
    SCROLLING_BACKGROUND = allocHeapBlock(sizeof(ScrollingBackground), 0x1EB);
    SCROLLING_BACKGROUND->ready = 0;
    for (i = 0; i < 2; i++) {
        poly = &SCROLLING_BACKGROUND->bars[i];
        SetPolyFT4(poly);
        poly->r0 = 0x80;
        poly->g0 = 0x80;
        poly->b0 = 0x80;
    }
    /* keep 0x4FE7C bytes of the largest free block for the PAK */
    reserve = allocTaskHeapBlock(getLargestFreeHeapBlock() - 0x4FE7C);
    SCROLLING_BACKGROUND->pak = loadFileTagged("A:\\ADVBG.PAK", getCurrentTaskId(), 0x1EA);
    freeHeapBlock(reserve);
    SCROLLING_BACKGROUND->brightness = 0;
    SCROLLING_BACKGROUND->image = image;
    sprite = SCROLLING_BACKGROUND->panel;
    initTexSprite(sprite++, 0x7FF8, -0xCC, panelY, 0x300, 0x1B0, 0xB8, 0x24, 0);
    initTexSprite(sprite, 0x7F79, -20, panelY, 0x350, 0x1BD, 20, 21, 0);
    setPanelIcon(0x35);
    SCROLLING_BACKGROUND->requestedIcon = -1;
    SCROLLING_BACKGROUND->icon = -1;
    SCROLLING_BACKGROUND->state = 0;
    SCROLLING_BACKGROUND->ready = 1;
}

void showScrollingBackground(void) {
    while (SCROLLING_BACKGROUND->ready != 1) {
        waitFrames(FRAME_INTERVAL);
    }
    uploadTimList(findPakChunk(SCROLLING_BACKGROUND->pak, 5, 7));
    uploadTimList(findPakChunk(SCROLLING_BACKGROUND->pak, 5, 6));
    truncatePakAtChunk(SCROLLING_BACKGROUND->pak, 5, 7);
    switchScrollingBackground(SCROLLING_BACKGROUND->image);
    addFrameCallback((s32)renderScrollingBackground);
}

void setBackgroundScrollMode(s8 image) {
    SCROLLING_BACKGROUND->image = image;
}

/* once the old picture has faded out: the new one's TIM and sprites */
void switchScrollingBackground(s32 image) {
    TexSprite *sprite;
    s32 i;

    sprite = SCROLLING_BACKGROUND->tiles;
    if (SCROLLING_BACKGROUND->brightness > 0x80) {
        SCROLLING_BACKGROUND->brightness = 0;
        return;
    }
    if (SCROLLING_BACKGROUND->brightness == 0) {
        uploadTimList(findPakChunk(SCROLLING_BACKGROUND->pak, 5, image));
        SCROLLING_BACKGROUND->shownImage = image;
        if ((s8)image < 2) {
            initTexSprite(sprite++, 0x403C, 0, 0, 0x2C0, 0x100, 0x100, 0x100, 0xC8);
            initTexSprite(sprite, 0x403C, 0x100, 0, 0x330, 0x100, 0x40, 0x100, 0xC8);
        } else {
            for (i = 0; i < 15; i++, sprite++) {
                initTexSprite(sprite, 0x403C, i % 3 << 8, i / 3 << 6, 0x2C0, 0x100, 0x100, 0x40, 0xC8);
            }
        }
        SCROLLING_BACKGROUND->scrollY = 0;
        SCROLLING_BACKGROUND->scrollX = 0;
    }
}

void drawScrollingBackground(void) {
    TexSprite *sprite;
    u8 brightness;
    s8 blend;
    s32 i;

    sprite = SCROLLING_BACKGROUND->tiles;
    brightness = SCROLLING_BACKGROUND->brightness;
    blend = brightness != 0x80 ? 1 : -1;
    if (SCROLLING_BACKGROUND->shownImage >= 2) {
        if (--SCROLLING_BACKGROUND->scrollX < -0x1FF) {
            SCROLLING_BACKGROUND->scrollX = 0;
        }
        if (--SCROLLING_BACKGROUND->scrollY < -0x7F) {
            SCROLLING_BACKGROUND->scrollY = 0;
        }
        for (i = 0; i < 15; i++, sprite++) {
            sprite->x = (i % 3 << 8) + SCROLLING_BACKGROUND->scrollX / 2;
            sprite->y = (i / 3 << 6) + SCROLLING_BACKGROUND->scrollY / 2;
            drawTexturedSprite(sprite->x, sprite->y, &sprite->uv, sprite->tpage, sprite->clut, sprite->z, brightness, blend);
        }
    } else {
        /* the still pictures cycle through the six CLUTs from (0x3C0, 0x100)
           on, a new one every 8 frames */
        s32 clut = getClut(0x3C0, 0x100 + (u16)(((u16)++SCROLLING_BACKGROUND->scrollX >> 3) % 6));

        for (i = 0; i < 2; i++, sprite++) {
            drawTexturedSprite(sprite->x, sprite->y, &sprite->uv, sprite->tpage, clut, sprite->z, brightness, blend);
        }
    }
    if (SCROLLING_BACKGROUND->shownImage != SCROLLING_BACKGROUND->image) {
        if (SCROLLING_BACKGROUND->brightness != 0) {
            SCROLLING_BACKGROUND->brightness -= 4;
        }
        switchScrollingBackground(SCROLLING_BACKGROUND->image);
    } else if ((s8)brightness >= 0) {
        if ((SCROLLING_BACKGROUND->brightness += 4) > 0x80) {
            SCROLLING_BACKGROUND->brightness = 0x80;
        }
    }
}

void freeScrollingBackground(void) {
    removeFrameCallback((s32)renderScrollingBackground);
    waitFrames(4);
    freeHeapBlocksByTag(0x1EA);
    freeHeapBlocksByTag(0x1EB);
}

/* the first of the 0x6E Digimon cards that uses modelId (the last one if none does) */
void *findDigimonCardByModelId(s32 modelId) {
    DigimonCardData *card;
    s32 i;

    card = (DigimonCardData *)DIGIMON_CARDS;
    for (i = 0; i < 0x6E; i++, card++) {
        if (card->modelId == modelId) {
            break;
        }
    }
    return card;
}

s32 loadSkill(s32 skillId, s32 pak) {
    char path[32];
    s32 skill;

    skill = (s32)findPakChunk((Chunk *)pak, 2, skillId);
    if (skill == 0) {
        sprintf(path, "E:\\SKILL\\SKILL%03d.MSD", skillId);
        skill = loadFileTagged(path, getCurrentTaskId(), 0x81);
    }
    return skill;
}

void loadSkillFromDisc(s32 skillId) {
    loadSkill(skillId, 0);
}
