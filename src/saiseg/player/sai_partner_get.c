#include "common.h"
#include "game.h"
#include "dcb/sai_partner_get.h"
#include "dcb/window.h"
#include "dcb/text.h"
#include "dcb/heap.h"
#include "dcb/frame_callback.h"
#include "dcb/task.h"
#include "dcb/vram_upload.h"
#include "dcb/loader.h"
#include "dcb/card_db.h"
#include "dcb/sound_play.h"
#include "dcb/menu.h"
#include "dcb/battle_hud.h"
#include "dcb/dialog.h"
#include "dcb/pad.h"
#include "dcb/saiseg.h"
#include "dcb/sai_flags.h"

extern UiWindow SAI_PARTNER_GET_WINDOW;
extern PartnerCursor SAI_PARTNER_CURSOR;
extern PartnerList SAI_PARTNER_LIST;
extern CursorHighlight SAI_PARTNER_GET_CURSOR;

const u8 SAI_PARTNER_CARD_IDS[6] = { 0xAF, 0xB6, 0xBE, 0xB8, 0xB7, 0xBB };

u8 SAI_PARTNER_CHOICE_CARDS[4] = { 0xB7, 0xB8, 0xBB, 0x0 };

void SAI_drawPartnerCardArt(s32 x, s32 y, s32 vramX, s32 vramY, s32 frame, u8 *rgb, s32 z, s32 palette) {
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

void SAI_drawPartnerChoices(UiWindow *win) {
    /* not literals: GCC would share SAI_drawRewardCard's identical strings */
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
    for (i = 0; i < SAI_PARTNER_LIST.count; i++) {
        cardId = SAI_PARTNER_CHOICE_CARDS[i];
        if (SAI_PARTNER_CURSOR.cursor == i) {
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
        SAI_drawPartnerCardArt(tx, y + 4, i * 20 + 0x200, 0x180, getCardSpecialty(cardId), rgb, z, i);
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
    if (SAI_PARTNER_LIST.state != 5) {
        if (PAD_STATES[0]->repeat & PAD_UP) {
            if (SAI_PARTNER_CURSOR.cursor != 0) {
                playSoundEffect(2);
                SAI_PARTNER_CURSOR.cursor--;
                scrollWindowTo(&win->originX, 0, SAI_PARTNER_CURSOR.cursor * 72);
            }
        } else if (PAD_STATES[0]->repeat & PAD_DOWN) {
            if (SAI_PARTNER_CURSOR.cursor < SAI_PARTNER_LIST.count - 1) {
                playSoundEffect(2);
                SAI_PARTNER_CURSOR.cursor++;
                scrollWindowTo(&win->originX, 0, SAI_PARTNER_CURSOR.cursor * 72);
            }
        } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
            playSoundEffect(0);
            SAI_PARTNER_CURSOR.done = 1;
        }
        SAI_PARTNER_LIST.state = SAI_PARTNER_CURSOR.cursor + 6;
        rect.x = win->rect.x - win->scroll[2] + 1;
        rect.y = win->rect.y - win->scroll[3] + SAI_PARTNER_CURSOR.cursor * 72;
        rect.w = 0x104;
        rect.h = 0x48;
        moveCursorHighlight(&SAI_PARTNER_GET_CURSOR, &rect);
        drawCursorHighlight(&SAI_PARTNER_GET_CURSOR, z);
    }
}

void SAI_drawPartnerGetWindow(void) {
    drawWindow(&SAI_PARTNER_GET_WINDOW, SAI_drawPartnerChoices, 1);
}

void SAI_runPartnerGet(s32 task) {
    Rect16 cursorRect;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];
    u8 *file;
    s32 i;

    SAI_PARTNER_LIST.count = 0;
    for (i = 0; i < 4; i++) {
        if (SAI_AREA.partners[i] != -1) {
            SAI_PARTNER_CHOICE_CARDS[i] = SAI_PARTNER_CARD_IDS[SAI_AREA.partners[i]];
            SAI_PARTNER_LIST.count++;
        }
    }
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\CARD_F.TIM", getCurrentTaskId());
    file = (u8 *)waitFrames(0x7FFFFFFF);
    uploadTim((u32 *)file, 0x200, 0x1A8, 0x290, 0x1F2);
    DrawSync(0);
    waitFrames(FRAME_INTERVAL);
    freeHeapBlock(file);
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\P_CARD.ARC", getCurrentTaskId());
    file = (u8 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < SAI_PARTNER_LIST.count; i++) {
        uploadTim((u32 *)(file + ((s32 *)file)[getPartnerIndex(SAI_PARTNER_CHOICE_CARDS[i])]), i * 20 + 0x200, 0x180, 0x280, i + 0x1EE);
    }
    SAI_PARTNER_CURSOR.done = 0;
    SAI_PARTNER_CURSOR.cursor = 0;
    rect.x = 0x1E;
    rect.y = 0x44;
    rect.w = 0x104;
    rect.h = 0x5A;
    view.x = 0;
    view.y = 0;
    view.w = 0x10E;
    view.h = SAI_PARTNER_LIST.count * 72;
    openWindow(&SAI_PARTNER_GET_WINDOW, &rect, -1, (s16 *)&view, 10, 0x86, 0x80, 12);
    SAI_PARTNER_GET_WINDOW.label = (s32)"PARTNER GET";
    SAI_PARTNER_GET_WINDOW.labelPalette = 7;
    cursorRect.x = SAI_PARTNER_GET_WINDOW.originX + 4;
    cursorRect.y = SAI_PARTNER_GET_WINDOW.originY + 1;
    cursorRect.w = 12;
    cursorRect.h = 12;
    initCursorHighlight(&SAI_PARTNER_GET_CURSOR, &cursorRect, (Bytes4 *)-1);
    waitFrames(FRAME_INTERVAL);
    playSoundEffect(3);
    addFrameCallback((s32)SAI_drawPartnerGetWindow);
    do {
#if VERSION_US
    wait:
#elif VERSION_EU
        /* eu goes back with continue: even unused, the label changes its code */
#else
#error "saiseg/player/sai_partner_get: version not checked"
#endif
        waitFrames(FRAME_INTERVAL);
        if (SAI_PARTNER_LIST.state == 5 && (PAD_STATES[0]->pressed & PAD_CROSS)) {
            SAI_PARTNER_LIST.state = 6;
        }
        if (SAI_PARTNER_CURSOR.done == 0) {
            /* both go back to the wait; the match depends on the form: each
               version's compiler needs its own to lay the loop out as the
               original does */
#if VERSION_US
            goto wait;
#elif VERSION_EU
            continue;
#else
#error "saiseg/player/sai_partner_get: version not checked"
#endif
        }
        initDialog(dialog, "Is this Partner OK?", 1);
        runDialog(dialog);
        switch ((s8)dialog[0xA5]) {
        case 1:
            break;
        case 0:
        case 2:
            SAI_PARTNER_CURSOR.done = 0;
            break;
        }
    } while (SAI_PARTNER_CURSOR.done == 0);
    animateWindowTo(&SAI_PARTNER_GET_WINDOW, (Rect16 *)-1);
    playSoundEffect(4);
    freeHeapBlock(file);
    waitFrames(20);
    removeFrameCallback((s32)SAI_drawPartnerGetWindow);
    obtainPartner(0, SAI_AREA.partners[SAI_PARTNER_CURSOR.cursor]);
    SAI_AREA.mode = AREA_MODE_SCRIPT;
    SAI_setPartnerObtainedFlag(SAI_AREA.partners[SAI_PARTNER_CURSOR.cursor]);
    resumeTask(task);
}
