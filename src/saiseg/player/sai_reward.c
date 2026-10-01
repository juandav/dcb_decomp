#include "common.h"
#include "game.h"
#include "dcb/sai_reward.h"
#include "dcb/window.h"
#include "dcb/text.h"
#include "dcb/heap.h"
#include "dcb/frame_callback.h"
#include "dcb/task.h"
#include "dcb/vram_upload.h"
#include "dcb/loader.h"
#include "dcb/duel_launch.h"
#include "dcb/card_db.h"
#include "dcb/sound_play.h"
#include "dcb/battle_hud.h"
#include "dcb/pad.h"
#include "dcb/saiseg.h"

extern u8 SAI_REWARD_FROM_SCRIPT;
extern u8 SAI_PRIZE_PACK;
extern RewardScreen *SAI_REWARD_SCREEN;

void rollRewardCards(s32 player, s32 pack);
void SAI_showRewardCards();

void SAI_runRewardTask(u8 value) {
    SAI_REWARD_FROM_SCRIPT = value;
    SAI_showRewardCards();
    waitFrames(5);
    SAI_AREA.mode = AREA_MODE_SCRIPT;
    SAI_AREA.rewardBusy = 0;
}

void SAI_waitForCross(void) {
    do {
        waitFrames(FRAME_INTERVAL);
    } while (!(PAD_STATES[0]->pressed & PAD_CROSS));
}

void SAI_drawRewardCardArt(s32 x, s32 y, s32 vramX, s32 vramY, s32 frame, u16 clut, u8 *rgb) {
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

void SAI_drawRewardCard(CardWindow *win) {
    char buf[0x48];
    u8 rgb[4];
    s32 x;
    s32 y;
    s32 z;
    s32 specialty;
    s32 i;
    DigimonCardData *card;
    u8 *data;

    if (SAI_REWARD_SCREEN->showRewards != 0) {
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
        SAI_drawRewardCardArt(x + 1, y + 0xC, win->index * 20 + 0x200, 0x78, specialty, win->clut, rgb);
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

void SAI_drawRewardResult(RewardWindow *win) {
    s32 x = win->window.originX;
    s32 y = win->window.originY;
    s32 z = win->window.z;

    if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[win->slot] < 0) {
        drawLargeText(x + 1, y + 1, (s32)"FULL SET!", 7, z);
    } else {
        drawLargeText(x + 1, y + 1, (s32)"RECEIVED!", 7, z);
    }
}

void SAI_drawRewardTitle(UiWindow *window) {
    if (SAI_REWARD_FROM_SCRIPT == 0) {
        drawText(window->originX + 2, window->originY + 1, (s32)"Earned a Prize Pack", 7, 0);
        drawText(window->originX + 0x92, window->originY + 1, (s32)CARD_PACK_NAMES[SAI_PRIZE_PACK], 6, 0);
    } else {
        drawText(window->originX + 2, window->originY + 1, (s32)"Received", 7, 0);
    }
}

void SAI_drawRewardWindows(void) {
    s32 i;

    drawWindow(&SAI_REWARD_SCREEN->main, SAI_drawRewardTitle, 0);
    for (i = 0; i < 3; i++) {
        if (SAI_REWARD_SCREEN->showRewards != 0 && ((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] != 100) {
            drawWindow(&SAI_REWARD_SCREEN->rewards[i].window, SAI_drawRewardResult, 0);
        }
        drawWindow(&SAI_REWARD_SCREEN->cards[i].window, SAI_drawRewardCard, 0);
    }
}

void SAI_addScriptRewardCards(void) {
    s32 i;
    u8 clearNew;
    u8 flags;

    for (i = 0; i < 3; i++) {
        ((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i] = SAI_SCRIPT_REWARD_CARDS[i];
        if (SAI_SCRIPT_REWARD_CARDS[i] >= 0) {
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

void SAI_showRewardCards(s32 fromScript) {
#if VERSION_US
    /* the ROM keeps two stray bytes after the terminator */
    static const char path[16] = "B:\\M_CARD.ARC\0\xBB\xBB";
#elif VERSION_EU
    static const char path[16] = "B:\\M_CARD.ARC";
#else
#error "saiseg/player/sai_reward: version not checked"
#endif
    Rect16 rect;
    u8 *tims;
    s32 i;
    s32 card;

    SAI_REWARD_SCREEN = allocPermanentHeapBlock(sizeof(RewardScreen));
    SAI_REWARD_SCREEN->showRewards = 0;
    if (fromScript == 0) {
        rollRewardCards(0, SAI_PRIZE_PACK);
        addRewardCardsToCollection(0);
    } else {
        SAI_addScriptRewardCards();
    }
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tims = (u8 *)waitFrames(0x7FFFFFFF);
    playSoundEffect(3);
    rect.x = 0x10;
    rect.y = 0x10;
    rect.w = 0x120;
    rect.h = 0xE;
    openWindow(&SAI_REWARD_SCREEN->main, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    for (i = 0; i < 3; i++) {
        card = ((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i];
        if (card >= 0) {
            uploadTim((u32 *)(tims + ((s32 *)tims)[card]), i * 20 + 0x200, 0x78, 0x200, i + 0xFB);
        }
        SAI_REWARD_SCREEN->cards[i].index = i;
        SAI_REWARD_SCREEN->cards[i].cardId = card;
        SAI_REWARD_SCREEN->cards[i].clut = getClut(0x200, i + 0xFB);
        rect.x = 0x10;
        rect.y = i * 65 + 0x26;
        rect.w = 0x120;
        rect.h = 0x3C;
        openWindow(&SAI_REWARD_SCREEN->cards[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    }
    DrawSync(0);
    waitFrames(FRAME_INTERVAL);
    freeHeapBlock(tims);
    addFrameCallback((s32)SAI_drawRewardWindows);
    waitFrames(20);
    SAI_waitForCross();
    playSoundEffect(0);
    SAI_REWARD_SCREEN->showRewards = 1;
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
            openWindow(&SAI_REWARD_SCREEN->rewards[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
            SAI_REWARD_SCREEN->rewards[i].slot = i;
            SAI_REWARD_SCREEN->rewards[i].window.palette = 2;
        }
    }
    SAI_waitForCross();
    playSoundEffect(4);
    animateWindowTo(&SAI_REWARD_SCREEN->main, (Rect16 *)-1);
    for (i = 0; i < 3; i++) {
        animateWindowTo(&SAI_REWARD_SCREEN->cards[i].window, (Rect16 *)-1);
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] != 100) {
            animateWindowTo(&SAI_REWARD_SCREEN->rewards[i].window, (Rect16 *)-1);
        }
    }
    waitFrames(20);
    freeHeapBlock(SAI_REWARD_SCREEN);
    waitFrames(10);
    removeFrameCallback((s32)SAI_drawRewardWindows);
}
