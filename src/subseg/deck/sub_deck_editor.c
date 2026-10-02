#include "common.h"
#include "game.h"
#include "dcb/sub_deck_editor.h"
#include "dcb/window.h"
#include "dcb/card_db.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/heap.h"
#include "dcb/vram_upload.h"
#include "dcb/subseg.h"
#include "dcb/sub_deck_screens.h"

extern SprtPacket *SUB_SPRITE_CURSOR;
extern u8 SUB_CARD_LIST_MENU_PAD;
extern u8 SUB_CARD_SORT_MENU_PAD;
extern u8 SUB_DECK_SORT_MENU_PAD;

void SUB_loadCardMapTim(void) {
    char path[64];
    u32 *tim;

    sprintf(path, "C:\\OBJECT\\c_map.tim");
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTim(tim, 0x140, 0, 0, 0x1F4);
    freeHeapBlock(tim);
    printf("aaa\n");
}

void SUB_runCardImageCache(void) {
    char path[64];
    s32 i;
    u32 *tim;
    s32 slot;
    s8 found;

    for (i = 0; i < 8; i++) {
        SUB_CARD_IMAGE_CACHE.ids[i] = -1;
        SUB_CARD_IMAGE_CACHE.ages[i] = 0;
    }
    SUB_CARD_IMAGE_CACHE.running = 1;
    SUB_CARD_IMAGE_CACHE.request = -1;
    SUB_CARD_IMAGE_CACHE.busy = 0;
    do {
        waitFrames(1);
        if (SUB_CARD_IMAGE_CACHE.busy == 0) {
            found = -1;
            for (i = 0; i < 8; i++) {
                if (SUB_CARD_IMAGE_CACHE.request == SUB_CARD_IMAGE_CACHE.ids[i]) {
                    found = i;
                }
            }
            if (found == -1) {
                slot = 0;
                SUB_CARD_IMAGE_CACHE.busy = 1;
                for (i = 1; i < 8; i++) {
                    if (i == 1) {
                        slot = 0;
                    }
                    if (SUB_CARD_IMAGE_CACHE.ages[slot] > SUB_CARD_IMAGE_CACHE.ages[i]) {
                        slot = i;
                    }
                }
                for (i = 0; i < 8; i++) {
                    if (i != slot) {
                        if (--SUB_CARD_IMAGE_CACHE.ages[i] < 0) {
                            SUB_CARD_IMAGE_CACHE.ages[i] = 0;
                        }
                    } else {
                        SUB_CARD_IMAGE_CACHE.ids[i] = SUB_CARD_IMAGE_CACHE.request;
                        SUB_CARD_IMAGE_CACHE.ages[i] = 0;
                    }
                }
                sprintf(path, "B:\\Card\\LC%3.3d.TIM", SUB_CARD_IMAGE_CACHE.ids[slot]);
                spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
                tim = (u32 *)waitFrames(0x7FFFFFFF);
                uploadTim(tim, (slot / 4) * 32 + 0x140, (slot % 4) * 64 + 0x100, 0, slot + 0x1F4);
                freeHeapBlock(tim);
                SUB_CARD_IMAGE_CACHE.ages[slot] = 100;
                SUB_CARD_IMAGE_CACHE.busy = 0;
            }
        }
    } while (SUB_CARD_IMAGE_CACHE.running != 0);
    SUB_CARD_IMAGE_CACHE.busy = -1;
}

s32 SUB_findCachedCardImage(s16 id) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (id == SUB_CARD_IMAGE_CACHE.ids[i] && SUB_CARD_IMAGE_CACHE.ages[i] != 0) {
            return i;
        }
    }
    return -1;
}

void SUB_loadEditorAssets(void) {
    char path[24];
    u32 *pack;

    sprintf(path, "C:\\OBJECT\\deck.TIS");
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    SUB_CARD_ARCHIVE = (u8 *)waitFrames(0x7FFFFFFF);
}

void SUB_initPrimBuffers(s8 keepBuffers) {
    s32 i;
    s16 *p;

    if (!keepBuffers) {
        for (i = 0; i < 2; i++) {
            DB(i).primSlots[0] = (s32)(SUB_EDITOR.primBuffers[i] = allocHeapBlock(0xAF0, 0x3C));
        }
        p = allocHeapBlock(0x320, 0x3C);
        SUB_EDITOR.unk8 = p;
    }
    p = SUB_EDITOR.unk8;
    for (i = 0; i < 100; i++) {
        p[1] = 0;
        p[0] = 0;
        p[3] = 0;
        p[2] = 0;
        p += 4;
    }
}

void SUB_drawCardIcon(s16 cardId, s16 x, s16 y, u8 brightness, s32 otIndex) {
    s16 specialty = ((DigimonCardData *)DIGIMON_CARDS)[cardId].attr >> 4;
    s32 u;
    s32 v;
    s32 clutY;
    s32 column;
    s16 row;
    s32 pageU;
    s32 columnU;

    if (cardId == 300) {
        u = 0x230;
        v = 0x1E0;
    } else {
        /* u holds the card's page (50 icons each) before its column is
           added: the European version's match depends on it, for its
           registers */
        u = cardId / 50;
        pageU = u << 6;
        column = cardId % 5;
        columnU = column * 12 + 0x240;
        u = pageU + columnU;
        row = cardId % 50;
        row = row / 5;
        v = row * 24 + 0x100;
    }
    if (cardId >= 0x11D || cardId == 200) {
        clutY = 0x1FE;
    } else if (cardId >= 0xBF) {
        clutY = 0x1FD;
    } else {
        clutY = specialty + 0x1F8;
    }
    SUB_drawSprite(x, y, getClut(0x240, clutY), u, v, 24, 24, 1, brightness, -1, otIndex);
}

void SUB_drawSprite(s16 x, s16 y, s16 clut, s32 u, s32 v, s16 w, s16 h, s8 tp, u8 brightness, s8 abr, s32 otIndex) {
    u16 tpage = ((tp & 3) << 7) | ((abr & 3) << 5) | ((v & 0x100) >> 4) | ((u & 0x3C0) >> 6) | ((v & 0x200) << 2);

    setlen(&SUB_SPRITE_CURSOR->sp, 4);
    setcode(&SUB_SPRITE_CURSOR->sp, 0x64);
    SUB_SPRITE_CURSOR->sp.clut = clut;
    SUB_SPRITE_CURSOR->sp.w = w;
    SUB_SPRITE_CURSOR->sp.h = h;
    SUB_SPRITE_CURSOR->sp.x0 = x;
    SUB_SPRITE_CURSOR->sp.y0 = y;
    if (tp != 0) {
        SUB_SPRITE_CURSOR->sp.u0 = (u % 64) << 1;
    } else {
        SUB_SPRITE_CURSOR->sp.u0 = (u % 64) << 2;
    }
    SUB_SPRITE_CURSOR->sp.v0 = v;
    SUB_SPRITE_CURSOR->sp.r0 = brightness;
    SUB_SPRITE_CURSOR->sp.g0 = brightness;
    SUB_SPRITE_CURSOR->sp.b0 = brightness;
    if (abr >= 0) {
        tpage |= (abr & 3) << 5;
        setSemiTrans(&SUB_SPRITE_CURSOR->sp, 1);
    } else {
        setSemiTrans(&SUB_SPRITE_CURSOR->sp, 0);
    }
    setDrawMode(&SUB_SPRITE_CURSOR->dm, 0, 0, tpage);
    addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &SUB_SPRITE_CURSOR->sp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &SUB_SPRITE_CURSOR->dm);
    SUB_SPRITE_CURSOR++;
}

void SUB_runDeckEditor(s32 player, s32 parentTask) {
    SUB_loadEditorAssets();
    SUB_initPrimBuffers(0);
    SUB_EDITOR.running = 1;
    SUB_EDITOR.player = player;
    SUB_CARD_LIST_MENU_PAD = SUB_EDITOR.player;
    SUB_CARD_SORT_MENU_PAD = SUB_EDITOR.player;
    SUB_DECK_SORT_MENU_PAD = SUB_EDITOR.player;
    spawnTask(0, -1, 0, 0x1000, SUB_EDITOR.task = SUB_runCardList, 0, 0, 0, 0);
    spawnTask(0, -1, 0, 0x1000, SUB_runCardImageCache, 0, getCurrentTaskId(), 0, 0);
    do {
        waitFrames(1);
        SUB_SPRITE_CURSOR = SUB_EDITOR.primBuffers[FRAME_BUFFER_INDEX];
    } while (SUB_EDITOR.running == 1);
    SUB_CARD_IMAGE_CACHE.running = 0;
    do {
        waitFrames(1);
    } while (SUB_CARD_IMAGE_CACHE.busy != -1);
    clearCollectionNewFlags(player);
    waitFrames(2);
    freeHeapBlocksByTag(0x3C);
    freeHeapBlock(SUB_CARD_ARCHIVE);
    resumeTask(parentTask);
}

void SUB_openCenteredWindow(UiWindow *window, Rect16 area, s32 label, s32 flags, s32 style) {
    Rect16 rect;
    s32 unused[2]; /* unused, but it is in the original stack frame */
    Rect16 view;

    rect.x = area.x - area.w / 2;
    rect.y = area.y - area.h / 2;
    rect.w = area.w & ~1;
    rect.h = area.h & ~1;
    view.x = 0;
    view.y = 0;
    view.w = (area.w + 10) & ~1;
    view.h = 0x2000;
    openWindow(window, &rect, -1, (s16 *)&view, flags, style, 0x80, 12);
    window->label = label;
}
