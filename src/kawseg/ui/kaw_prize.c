#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/duel_launch.h"
#include "dcb/battle_hud.h"
#include "dcb/card_db.h"
#include "dcb/vram_upload.h"
#include "dcb/sound_play.h"
#include "dcb/frame_callback.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_hand.h"
#include "dcb/kaw_hud.h"

extern PrizeScreen *KAW_PRIZE_SCREEN;

void rollRewardCards(s32 player, s32 level);

void KAW_drawPrizeCard(PrizeWindow *win) {
    /* arrays, not literals: GCC would share KAW_drawPartnerExp's "*s0%4d", and the
       strings before it have to be arrays too to keep their order in .rodata */
    static const char numberLabel[] = "No.";
    static const char idFormat[] = "*s0%3d";
    static const char valueFormat[] = "*s0%4d";
    char buf[72];
    Bytes4 rgb;
    s32 x;
    s32 y;
    s32 z;
    s32 specialty;
    s32 i;
    DigimonCardData *card;
    u8 *data;

    if (KAW_PRIZE_SCREEN->showRewards != 0) {
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[win->index] < 0) {
            rgb.b[0] = 0x40;
            rgb.b[1] = 0x40;
            rgb.b[2] = 0x40;
            win->window.brightness = 0x40;
        } else {
            rgb.b[0] = 0x80;
            rgb.b[1] = 0x80;
            rgb.b[2] = 0x80;
            win->window.brightness = 0x80;
        }
    } else {
        rgb.b[0] = 0x80;
        rgb.b[1] = 0x80;
        rgb.b[2] = 0x80;
        win->window.brightness = 0x80;
    }
    x = win->window.originX;
    y = win->window.originY;
    z = win->window.z;
    specialty = getCardSpecialty(win->cardId);
    if (((PlayerProfile *)PLAYER_PROFILES)->cardCollection[win->cardId] & 0x20) {
        drawIcon(x + 0xF, y + 0x2A, 2, 9, 0);
    }
    KAW_drawPortraitColored(x + 1, y + 0xC, win->index * 20 + 0x2C0, 0x100, specialty, win->clut, &rgb);
    drawTextColored(x + 3, y, numberLabel, rgb.b, 7, z);
    sprintf(buf, idFormat, win->cardId);
    drawTextColored(x + 0x17, y, buf, rgb.b, 7, z);
    if (specialty < 5) {
        card = &((DigimonCardData *)DIGIMON_CARDS)[win->cardId];
        drawTextColored(x + 0x32, y, card->name, rgb.b, 7, z);
        drawIconColored(x + 0x2C, y + 0xE, 0, 0x1A, rgb.b, z);
        sprintf(buf, valueFormat, card->hp);
        drawTextColored(x + 0x3C, y + 0xD, buf, rgb.b, 7, z);
        drawIconColored(x + 0x2C, y + 0x1A, 0, 7, rgb.b, z);
        sprintf(buf, valueFormat, card->attack[0].power);
        drawTextColored(x + 0x3C, y + 0x19, buf, rgb.b, 7, z);
        drawIconColored(x + 0x2C, y + 0x26, 0, 8, rgb.b, z);
        sprintf(buf, valueFormat, card->attack[1].power);
        drawTextColored(x + 0x3C, y + 0x25, buf, rgb.b, 7, z);
        drawIconColored(x + 0x2C, y + 0x32, 0, 9, rgb.b, z);
        sprintf(buf, valueFormat, card->attack[2].power);
        drawTextColored(x + 0x3C, y + 0x31, buf, rgb.b, 7, z);
        sprintf(buf, "(%s)", CROSS_EFFECT_SHORT_NAMES[card->crossEffect]);
        drawSmallTextColored(x + 0x56, y + 0x33, buf, 7, rgb.b, z);
        if (CROSS_EFFECT_ICONS[card->crossEffect] != 0) {
            drawIconColored(x + 0x95, y + 0x31, 0, CROSS_EFFECT_ICONS[card->crossEffect] + 0x14, rgb.b, z);
        }
        drawIconColored(x + 0x74, y + 0x1A, 0, 0x18, rgb.b, z);
        sprintf(buf, "*s0%2d", card->dpCost);
        drawTextColored(x + 0x86, y + 0x19, buf, rgb.b, 7, z);
        drawIconColored(x + 0x74, y + 0x26, 0, 0x19, rgb.b, z);
        sprintf(buf, "*s0%2d", card->dpBonus);
        drawTextColored(x + 0x86, y + 0x25, buf, rgb.b, 7, z);
        drawTextColored(x + 0xA8, y, "Support Effect", rgb.b, 6, z);
        if (card->supportIcon != 0) {
            drawIconColored(x + 0x114, y, 0, card->supportIcon + 0x14, rgb.b, z);
        }
        for (i = 0; i < 4; i++) {
            drawTextColored(x + 0xA8, y + 0xD + i * 12, card->supportText[i], rgb.b, 7, z);
        }
    } else if (specialty == 5) {
        data = (u8 *)&((OptionCardData *)OPTION_CARDS)[win->cardId - 0xBF];
        drawTextColored(x + 0x32, y, data + 3, rgb.b, 7, z);
        drawTextColored(x + 0xA8, y, "Option Description", rgb.b, 6, z);
        if (*(s8 *)(data + 0x8C) != 0) {
            drawIconColored(x + 0x114, y, 0, *(s8 *)(data + 0x8C) + 0x14, rgb.b, z);
        }
        for (i = 0; i < 4; i++) {
            drawTextColored(x + 0xA8, y + 0xD + i * 12, data + (i * 0x15 + 0x8D), rgb.b, 7, z);
        }
    } else {
        data = (u8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[win->cardId - 0x125];
        drawTextColored(x + 0x32, y, data + 3, rgb.b, 7, z);
        drawTextColored(x + 0xA8, y, "Option Description", rgb.b, 6, z);
        for (i = 0; i < 4; i++) {
            drawTextColored(x + 0xA8, y + 0xD + i * 12, data + (i * 0x15 + 0x1B), rgb.b, 7, z);
        }
    }
}

void KAW_drawPrizeResult(RewardWindow *w) {
    s32 x;
    s32 y;
    s32 z;

    x = w->window.originX;
    y = w->window.originY;
    z = w->window.z;
    if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[w->index] < 0) {
        drawLargeText(x + 1, y + 1, (s32)"FULL SET!", 7, z);
    } else {
        drawLargeText(x + 1, y + 1, (s32)"RECEIVED!", 7, z);
    }
}

void KAW_drawPrizeTitle(UiWindow *window) {
    drawText(window->originX + 2, window->originY + 1, (s32)"Earned a Prize Pack", 7, 0);
    drawText(window->originX + 0x92, window->originY + 1, (s32)CARD_PACK_NAMES[((u8 *)SESSION_DATA)[0x73]], 6, 0);
}

void KAW_renderPrizeScreen(void) {
    s32 i;

    drawWindow(&KAW_PRIZE_SCREEN->window, KAW_drawPrizeTitle, 0);
    for (i = 0; i < 3; i++) {
        if (KAW_PRIZE_SCREEN->showRewards) {
            drawWindow(&KAW_PRIZE_SCREEN->rewards[i].window, KAW_drawPrizeResult, 0);
        }
        drawWindow(&KAW_PRIZE_SCREEN->prizes[i].window, KAW_drawPrizeCard, 0);
    }
}

void KAW_rollPrizeCards(u8 *archive) {
    s32 i;

    rollRewardCards(0, ((u8 *)SESSION_DATA)[0x73]);
    for (i = 0; i < 3; i++) {
        uploadTim((u32 *)(archive + ((s32 *)archive)[((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i]]), i * 20 + 0x2C0, 0x100, -1, -1);
        KAW_DUEL->rewardCluts[i] = getClut(LOADED_TIM.crect->x, LOADED_TIM.crect->y);
    }
}

void KAW_runPrizeScreen(void) {
    Rect16 rect;
    s32 i;

    KAW_PRIZE_SCREEN = allocPermanentHeapBlock(sizeof(PrizeScreen));
    addRewardCardsToCollection(0);
    KAW_PRIZE_SCREEN->showRewards = 0;
    rect.x = 16;
    rect.y = 16;
    rect.w = 0x120;
    rect.h = 14;
    openWindow(&KAW_PRIZE_SCREEN->window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    for (i = 0; i < 3; i++) {
        KAW_PRIZE_SCREEN->prizes[i].index = i;
        KAW_PRIZE_SCREEN->prizes[i].cardId = ((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i];
        KAW_PRIZE_SCREEN->prizes[i].clut = KAW_DUEL->rewardCluts[i];
        rect.x = 16;
        rect.y = i * 65 + 0x26;
        rect.w = 0x120;
        rect.h = 0x3C;
        openWindow(&KAW_PRIZE_SCREEN->prizes[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    }
    playSoundEffect(0xA3);
    addFrameCallback((s32)KAW_renderPrizeScreen);
    waitFrames(20);
    KAW_waitForCross();
    playSoundEffect(0xA0);
    KAW_PRIZE_SCREEN->showRewards = 1;
    for (i = 0; i < 3; i++) {
        rect.x = 200;
        rect.y = i * 65 + 0x40;
        rect.w = 0x48;
        rect.h = 9;
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] < 0) {
            rect.x = 0xC4;
            rect.w = 0x50;
        }
        openWindow(&KAW_PRIZE_SCREEN->rewards[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
        KAW_PRIZE_SCREEN->rewards[i].index = i;
        KAW_PRIZE_SCREEN->rewards[i].window.palette = 2;
    }
    KAW_waitForCross();
    playSoundEffect(0xA4);
    animateWindowTo(&KAW_PRIZE_SCREEN->window, (Rect16 *)-1);
    for (i = 0; i < 3; i++) {
        animateWindowTo(&KAW_PRIZE_SCREEN->rewards[i].window, (Rect16 *)-1);
        animateWindowTo(&KAW_PRIZE_SCREEN->prizes[i].window, (Rect16 *)-1);
    }
    waitFrames(30);
    removeFrameCallback((s32)KAW_renderPrizeScreen);
    waitFrames(2);
    freeHeapBlock(KAW_PRIZE_SCREEN);
    waitFrames(2);
}
