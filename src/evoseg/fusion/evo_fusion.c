#include "common.h"
#include "game.h"
#include "dcb/evo_fusion.h"
#include "dcb/scene3d.h"
#include "dcb/frame_callback.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/window.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/sound_play.h"
#include "dcb/scroll_bg.h"
#include "dcb/menu.h"
#include "dcb/game_flow.h"
#include "dcb/evoseg.h"
#include "dcb/evo_cutscene.h"
#include "dcb/evo_card_list.h"
#include "dcb/evo_trays.h"
#include "dcb/evo_screen_flash.h"
#include "dcb/evo_fusion_script.h"
#include "dcb/evo_text.h"
#include "dcb/evo_banners.h"
#include "dcb/evo_type_choice.h"
#include "dcb/evo_lists.h"
#include "dcb/evo_rewards.h"
#include "dcb/evo_partner_status.h"
#include "dcb/evo_effect.h"

extern CursorHighlight EVO_CARD_LIST_CURSOR;
extern CursorHighlight EVO_SORT_CURSOR;

void EVO_openWindows(void) {
    EvoWindowDef *def;
    s32 i;

    for (def = EVO_WINDOW_DEFS, i = 0; i < 14; i++, def++) {
        EVO_WINDOWS[i].z = 30;
        openWindow(&EVO_WINDOWS[i].win, def, -1, (s16 *)-1, def->flags, def->style, def->brightness, 12);
        if (def->label != 0) {
            EVO_WINDOWS[i].win.label = def->label;
        }
        EVO_WINDOWS[i].win.labelPalette = def->labelPalette;
        animateWindowTo(&EVO_WINDOWS[i].win, (Rect16 *)-1);
    }
    for (i = 0; i < 3; i++) {
        EVO_WINDOWS[i].isPartner = 0;
        EVO_WINDOWS[i + 3].isPartner = 1;
    }
    openMenu(&EVO_CARD_LIST_MENU, &EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_CURSOR, (Bytes4 *)-1);
    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
    EVO_CARD_LIST_WINDOW.labelPalette = 8;
    EVO_CARD_LIST_WINDOW.label = (s32)"CARD LIST";
    EVO_CARD_LIST_MENU.active = 0;
    openMenu(&EVO_SORT_MENU, &EVO_SORT_WINDOW, &EVO_SORT_CURSOR, (Bytes4 *)-1);
    animateWindowTo(&EVO_SORT_WINDOW, (Rect16 *)-1);
    EVO_SORT_WINDOW.label = (s32)"SORT MENU";
    EVO_SORT_WINDOW.labelPalette = 8;
    EVO_WINDOWS[12].z = 0x1B;
    EVO_WINDOWS[13].z = 5;
    EVO_WINDOWS[13].win.palette = 2;
    EVO_RANK_UP_STATE = 0;
    openWindow(&EVO_RANK_UP_WINDOW, &EVO_RANK_UP_RECT, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    animateWindowTo(&EVO_RANK_UP_WINDOW, (Rect16 *)-1);
    EVO_RANK_UP_WINDOW.palette = 2;
}

void EVO_drawEmptyWindow(void) {
}

void (*EVO_WINDOW_DRAW_FUNCS[14])() = {
    EVO_drawFusionTypeIcon, EVO_drawFusionTypeTitle, EVO_drawFusionTypeHelp, EVO_drawFusionTypeIcon, EVO_drawFusionTypeTitle, EVO_drawFusionTypeHelp, EVO_drawUnitPortrait,
    EVO_drawMessageWindow, EVO_drawEmptyWindow, EVO_drawEmptyWindow, EVO_drawPartnerList, EVO_drawPartnerStatus, EVO_drawCardInfo, EVO_drawReceivedBanner,
};

void EVO_renderFusion(void) {
    s32 i;
    s32 t;

    if (EVO_FUSION.resultStep == 1 && EVO_TRAYS[1].merge == 1 && EVO_TRAYS[0].merge < 30) {
        EVO_TRAYS[0].merge++;
        if (EVO_TRAYS[0].merge >= 30) {
            EVO_TRAYS[0].merge = 30;
            EVO_FUSION.hideResult = 0;
        }
        t = EVO_TRAYS[0].merge;
        EVO_TRAYS[0].x = (t * 58 + (30 - t) * 14) / 30;
        EVO_TRAYS[1].x = (t * 58 + (30 - t) * 102) / 30;
    }
    for (i = 0; i < 14; i++) {
        drawWindow(&EVO_WINDOWS[i].win, EVO_WINDOW_DRAW_FUNCS[i], EVO_WINDOWS[i].z);
    }
    drawWindow(&EVO_RANK_UP_WINDOW, EVO_drawRankUpBanner, 5);
    drawWindow(&EVO_CARD_LIST_WINDOW, EVO_drawCardList, 0x1D);
    drawWindow(&EVO_SORT_WINDOW, EVO_drawSortMenu, 0x1C);
    for (i = 0; i < 2; i++) {
        EVO_drawTray(&EVO_TRAYS[i]);
    }
}

void EVO_advanceText(void) {
    if (EVO_FUSION.textTyping == 0 && (PAD_STATES[0]->pressed & 0x40)) {
        EVO_clearTextLines(EVO_TEXT_LINES);
        if (EVO_FUSION.scriptState == 1) {
            EVO_addTextLine((u8 *)EVO_SCRIPT->vars[4]);
        }
        EVO_FUSION.scriptState = 0;
        playSoundEffect(0);
    }
}

void EVO_loadCardImages(void) {
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    EVO_FUSION.cardArchive = (s32 *)waitFrames(0x7FFFFFFF);
}

void EVO_loadCardImage(s32 id, s32 slot) {
    char path[64];
    u32 *tim;

    EVO_FUSION.busy[0] = 1;
    sprintf(path, "B:\\CARD\\LC%3.3d.TIM", id);
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTim(tim, (slot & 1) * 32 + 0x240, (slot >> 1) * 64 + 0x100, 0x180, slot + 0x1E8);
    DrawSync(0);
    freeHeapBlock(tim);
    EVO_FUSION.busy[0] = 0;
}

void EVO_loadScriptFlags(void) {
    s32 i;
    s32 bit;
    PlayerProfile *profile;

    for (i = 20, bit = 0, profile = (PlayerProfile *)PLAYER_PROFILES; i < 30; i++, bit++) {
        if ((1 << bit) & profile->scriptFlags) {
            EVO_SCRIPT->vars[i] = 1;
        }
    }
}

void EVO_saveScriptFlags(void) {
    s32 i;
    s32 bit;

    i = 20;
    bit = 0;
    while (1) {
        if (bit >= 32) {
            break;
        }
        if (EVO_SCRIPT->vars[i] != 0) {
            ((PlayerProfile *)PLAYER_PROFILES)->scriptFlags |= 1 << bit;
        }
        i++;
        bit++;
        if (i >= 30) {
            break;
        }
    }
}

void EVO_runFusion(s32 unit) {
    s32 running = 1;
    s32 i;

    EVO_loadUnitTextures();
    EVO_loadCardImages();
    EVO_initFusionScene();
    EVO_loadEffectArchive();
    for (i = 0; i < 3; i++) {
        EVO_DECK_CARD_COUNTS[i] = allocTaskHeapBlock(0x12D);
    }
    EVO_SPARE_CARD_COUNTS = allocTaskHeapBlock(0x12D);
    EVO_countSpareCards();
    EVO_openWindows();
    EVO_clearTextLines(EVO_TEXT_LINES);
    if (unit >= 0) {
        EVO_FUSION.unit = unit;
        EVO_initScreenFlash();
    }
    EVO_SCRIPT = EVO_loadUnitScript(EVO_FUSION.unit);
    EVO_SCRIPT->vars = EVO_allocScriptRegisters(30);
    if (unit < 0) {
        playMusic(0, EVO_FUSION.unit + 0x85, 100);
        EVO_FUSION.step = 17;
        EVO_FUSION.scriptState = 0;
        EVO_FUSION.hideResult = 0;
        EVO_SCRIPT->script->pc = EVO_SCRIPT->script->start + EVO_FUSION.resumeOffset;
        EVO_toggleMessageWindows(0);
    } else {
        EVO_initCardList();
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_FUSION.hideResult = 1;
    }
    EVO_FUSION.swapTimer = 0;
    EVO_FUSION.typeChoiceOpen = 0;
    EVO_FUSION.unkBD = 0;
    EVO_FUSION.partnerListOpen = 0;
    EVO_FUSION.fusionType = 0;
    EVO_FUSION.sortMenuOpen = 0;
    EVO_FUSION.resultStep = 0;
    for (i = 0; i < 2; i++) {
        EVO_TRAYS[i].x = -0x78;
        EVO_TRAYS[i].y = 0x29;
    }
    addFrameCallback((s32)EVO_renderFusion);
    EVO_loadScriptFlags();
    do {
        waitFrames(1);
        switch (EVO_FUSION.scriptState) {
        case 0:
            running = EVO_tickFusionScript(EVO_SCRIPT);
            break;
        case 1:
        case 2:
            EVO_advanceText();
            break;
        }
        switch (EVO_FUSION.step) {
        case 0:
            break;
        case 1:
            EVO_tickFusionTypeChoice();
            break;
        case 2:
            EVO_tickPartnerList();
            break;
        case 3:
            EVO_tickCardList();
            break;
        case 4:
            EVO_tickPartnerReward();
            break;
        case 5:
            EVO_slideInFirstTray();
            break;
        case 6:
            EVO_swapToFirstTray();
            break;
        case 7:
            EVO_swapToSecondTray();
            break;
        case 8:
            EVO_slideOutFirstTray();
            break;
        case 10:
            EVO_tickFusionResult();
            break;
        case 11:
            EVO_showBothTrays();
            break;
        case 12:
            EVO_repickSecondCard();
            break;
        case 13:
            EVO_slideOutFirstTrayWithResult();
            break;
        case 14:
            EVO_slideTrayOut((s16)EVO_SCRIPT->script->params[0]);
            break;
        case 15:
            EVO_slideTrayIn((s16)EVO_SCRIPT->script->params[0]);
            break;
        case 16:
            EVO_cancelSecondCard((s16)EVO_SCRIPT->script->params[0]);
            break;
        case 17:
            EVO_showCutsceneResult();
            break;
        case 18:
            EVO_closeFusionResult();
            break;
        }
    } while (running != 0 && EVO_FUSION.cutscene == 0);
    if (running == 0) {
        animateWindowTo(&EVO_WINDOWS[6].win, (Rect16 *)-1);
        animateWindowTo(&EVO_WINDOWS[7].win, (Rect16 *)-1);
        waitFrames(20);
    }
    EVO_saveScriptFlags();
    removeFrameCallback((s32)EVO_renderFusion);
    removeFrameCallback((s32)renderSceneModels);
    waitFrames(1);
    endTask(0x1B);
    freeHeapBlock(EVO_FUSION.cardArchive);
    freeHeapBlock(EVO_SCRIPT->data);
    freeHeapBlocksByTag(0x2C);
    freeHeapBlock(EVO_EFFECT_ARCHIVE);
    freeHeapBlocksByTag(0x7F);
    if (EVO_FUSION.cutscene != 0) {
        waitFrames(60);
        hideScrollingBackground();
        spawnTask(0, -1, 0, 0x400, EVO_runFusionCutscene, 0, getCurrentTaskId(), 0, 0);
    } else {
        removeFrameCallback((s32)EVO_drawScreenFlash);
        spawnTask(0, -1, 0, 0x400, returnToWorldMap, 0, 0, 0, 0);
    }
}
