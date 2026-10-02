#include "common.h"
#include "game.h"
#include "dcb/sai_area.h"
#include "dcb/window.h"
#include "dcb/heap.h"
#include "dcb/frame_callback.h"
#include "dcb/task.h"
#include "dcb/vram_upload.h"
#include "dcb/game_flow.h"
#include "dcb/loader.h"
#include "dcb/duel_launch.h"
#include "dcb/sound_play.h"
#include "dcb/scroll_bg.h"
#include "dcb/game_exit.h"
#include "dcb/sound.h"
#include "dcb/pad.h"
#include "dcb/saiseg.h"
#include "dcb/sai_text.h"
#include "dcb/sai_choice.h"
#include "dcb/sai_flags.h"
#include "dcb/sai_labels.h"
#include "dcb/sai_player_data.h"
#include "dcb/sai_panel.h"
#include "dcb/sai_area_script.h"
#include "dcb/sai_opponent_select.h"
#include "dcb/sai_opponent_info.h"
#include "dcb/sai_hacking.h"
#include "dcb/sai_sprite.h"
#include "dcb/sai_world_map.h"
#include "dcb/sai_data.h"

extern u8 SAI_MESSAGE_WINDOW_PALETTE;
extern s8 SAI_ICON_MOTION;
extern s32 SAI_EXIT_ARG;

void SAI_drawPanel(void);
void SAI_drawAreaHud(void);

void SAI_loadAreaTextures(void) {
    char path[0x18];
    u32 *pack;

    if (((PlayerProfile *)PLAYER_PROFILES)->areaId < 10) {
        sprintf(path, "C:\\DEBUG\\area0%d.TIS", ((PlayerProfile *)PLAYER_PROFILES)->areaId);
    } else {
        sprintf(path, "C:\\DEBUG\\area%d.TIS", ((PlayerProfile *)PLAYER_PROFILES)->areaId);
    }
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    sprintf(path, "C:\\OBJECT\\world.TIS");
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
}

void SAI_tickArea(void) {
    char buf[0x20];

    switch (SAI_AREA_MODE) {
    case AREA_MODE_SCRIPT:
        SAI_stepAreaScript(SAI_SCRIPT[0]);
        break;
    case AREA_MODE_CHOICES:
        SAI_tickChoiceMenu();
        break;
    case AREA_MODE_SELECT_OPPONENT:
        SAI_tickOpponentSelectInput();
        break;
    case AREA_MODE_TEXT_FULL:
    case AREA_MODE_WAIT_CROSS:
        if (SAI_AREA.typing == 0 && (PAD_STATES[0]->pressed & PAD_CROSS)) {
            playSoundEffect(0);
            SAI_clearTextLines(SAI_TEXT_LINES);
            if (SAI_AREA.mode == AREA_MODE_TEXT_FULL) {
                SAI_addTextLine((u8 *)SAI_SCRIPT[0]->regs[4]);
            }
            SAI_AREA.mode = AREA_MODE_SCRIPT;
            SAI_AREA.waitingForCross = 0;
        }
        break;
    case AREA_MODE_BUSY:
        break;
    default:
        sprintf(buf, "(No = %d)", SAI_AREA_MODE);
        break;
        /* unreachable, but it leaves the "" the ROM has after the format */
        printf("");
    }
}

void SAI_reopenPlayerData(void) {
    SESSION->resumeMode = 0;
    SAI_SCRIPT[0]->script->pc = SAI_SCRIPT[0]->script->start + SESSION->scriptOffset;
    playMusic(0, SESSION->music, 100);
    SAI_AREA_MODE = AREA_MODE_BUSY;
    SAI_clearOpponents();
    spawnTask(0, -1, 0, 0x800, SAI_runTalkPanel, 0, getCurrentTaskId(), 0, 0);
    do {
        waitFrames(1);
    } while (SAI_AREA.mode == AREA_MODE_BUSY);
    SAI_AREA.location = 0;
    SAI_unlockArmorsFromFlags(SAI_SCRIPT[0]->regs);
    SAI_PLAYER_STATS_STATE = 1;
    spawnTask(0, -1, 0, 0x400, SAI_runPlayerData, 0, getCurrentTaskId(), 0, 0);
    SAI_AREA.mode = AREA_MODE_BUSY;
}

/* The hacking glitch: moves blocks of VRAM around, with a random pause and a
   sound between steps when animate is 1. */
void SAI_glitchVram(s8 animate) {
    Rect16 rect = { 0x220, 0xE1, 0x20, 1 };
    Rect16 unused[3]; /* unused, but it is in the original stack frame */
    s32 delay;
    s8 i;

    SAI_MESSAGE_WINDOW_PALETTE = 2;
    for (i = 0; i < 6; i++) {
        delay = abs(rand() % 30);
        if (animate == 1) {
            while (delay > 0) {
                waitFrames(1);
                delay--;
            }
            playSoundEffect(0x18);
        }
        if (i == 5) {
            MoveImage2(&SAI_GLITCH_VRAM_RECTS[i], 0x280, 0x1F8);
        } else {
            MoveImage2(&SAI_GLITCH_VRAM_RECTS[i], SAI_GLITCH_VRAM_RECTS[i].x - 0x20, SAI_GLITCH_VRAM_RECTS[i].y);
        }
    }
    delay = abs(rand() % 30);
    if (animate == 1) {
        while (delay > 0) {
            waitFrames(1);
            delay--;
        }
        playSoundEffect(0x18);
    }
    for (i = 0; i < 6; i++) {
        MoveImage2(&SAI_GLITCH_VRAM_RECTS[4], SAI_GLITCH_VRAM_RECTS[4].x - 0x20, i + 0xEC);
    }
    delay = abs(rand() % 30);
    if (animate == 1) {
        while (delay > 0) {
            waitFrames(1);
            delay--;
        }
        playSoundEffect(0x18);
    }
    MoveImage2(&rect, 0x380, 0x80);
}

const char D_801DE5D0[] = "";

/*
 * The area task: loads the area's pak (unless resuming), runs its script with
 * the message window and panels until the script stops, then leaves for what
 * exitAction says. resumeMode brings it back into a panel after a duel or the
 * complete stats.
 */
void SAI_runArea(s32 resume) {
    s32 timer = 0;

    if (resume == 0) {
        spawnTask(0, -1, 0, 0x400, SAI_loadAreaPak, 0, getCurrentTaskId, 0, 0);
        do {
            waitFrames(1);
        } while (SESSION->loading != 0);
    }
    loadSoundEffectBank(1);
    SAI_initCamera();
    SAI_AREA.waitingForCross = 0;
    SAI_SCRIPT[0] = SAI_createAreaScript();
    SAI_SCRIPT[0]->regs = SAI_allocScriptRegisters(0x174);
    SAI_loadScriptFlags();
    SAI_SCRIPT[0]->regs[0] = 1;
    ((SessionData *)SESSION_DATA)->deckAllowed[0] = ((SessionData *)SESSION_DATA)->deckAllowed[1] = ((SessionData *)SESSION_DATA)->deckAllowed[2] = 0;
    ((SessionData *)SESSION_DATA)->deckRuleActive = 0;
    SAI_clearTextLines(SAI_TEXT_LINES);
    SAI_toggleMessageWindow(0);
    SAI_AREA.choiceCount = 0;
    SAI_AREA.openPanel = 0;
    SAI_AREA.exitAction = AREA_EXIT_MAP;
    SAI_AREA.closePanel = 0;
    SAI_AREA.mode = AREA_MODE_SCRIPT;
    ((SessionData *)SESSION_DATA)->npcDeckIndex[0] = -1;
    SAI_createAreaName();
    SAI_createLocationLabel();
    addFrameCallback((s32)SAI_drawAreaHud);
    if (SAI_ICON_RUNNING != 1) {
        if (SAI_SCRIPT[0]->regs[0xB8] != 0) {
            if (SESSION->resumeMode == 1) {
                spawnTask(0, -1, 0, 0x400, SAI_runCornerIcon, 4, getCurrentTaskId(), 0, 0);
            } else {
                spawnTask(0, -1, 0, 0x400, SAI_runCornerIcon, 3, getCurrentTaskId(), 0, 0);
            }
        } else {
            spawnTask(0, -1, 0, 0x400, SAI_runCornerIcon, 2, getCurrentTaskId(), 0, 0);
        }
    }
    SAI_ICON_MOTION = 2;
    if (SESSION->resumeMode == 1) {
        SAI_AREA_MODE = AREA_MODE_BUSY;
        if ((s8)SESSION->location == 1) {
            spawnTask(0, -1, 0, 0x800, SAI_runOpponentSelectPanel, 0, getCurrentTaskId(), 0, 0);
        } else {
            spawnTask(0, -1, 0, 0x800, SAI_runOpponentInfoPanel, 0, getCurrentTaskId(), 0, 0);
        }
    } else if (SESSION->resumeMode == 2) {
        SAI_reopenPlayerData();
    }
    if (SAI_SCRIPT[0]->regs[0xB8] != 0) {
        SAI_glitchVram(0);
        setBackgroundScrollMode(1);
    }
    do {
        waitFrames(1);
        timer++;
        SAI_tickArea();
        if (SAI_SCRIPT[0]->regs[0xB8] != 0 && timer >= 150) {
            timer -= 150;
            SAI_flickerHackOverlay();
            playSoundEffect(0x1A);
        }
    } while (SAI_SCRIPT[0]->regs[0] != 0);
    playSoundEffect(4);
    animateWindowTo(&SAI_MESSAGE_WINDOW, (Rect16 *)-1);
    SAI_CLOSE_PANEL = 1;
    do {
        waitFrames(1);
    } while (SAI_AREA.openPanel != 0);
    removeFrameCallback((s32)SAI_drawAreaHud);
    SAI_saveScriptFlags();
    SAI_unlockArmorsFromFlags(SAI_SCRIPT[0]->regs);
    if (SAI_SCRIPT[0]->regs[0xB8] != 0 && (SAI_EXIT_ACTION == AREA_EXIT_EQUIPMENT || SAI_EXIT_ACTION == AREA_EXIT_DECK_EDITOR)) {
        fadeOutScrollingBackground();
        do {
            waitFrames(1);
        } while (SCROLL_BACKGROUND.shownImage != -1);
        changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->activePartner, 0x380, 0, 0x380, 0x80);
    }
    waitFrames(1);
    endTask(0x19);
    freeHeapBlocksByTag(0x7F);
    freeHeapBlocksByTag(0x2E);
    freeHeapBlocksByTag(0x31);
    waitFrames(1);
    switch (SAI_EXIT_ACTION) {
    default:
        spawnTask(0, -1, 0, 0x400, SAI_loadMapTextures, 1, getCurrentTaskId, 0, 0);
        do {
            waitFrames(1);
        } while (SESSION->loading != 0);
        playMusic(0, 0x6F, 0x64);
        spawnTask(0, -1, 0, 0x400, SAI_runWorldMap, 1, 0, getCurrentTaskId(), 0);
        break;
    case AREA_EXIT_DUEL:
        SAI_ICON_RUNNING = 0;
        fadeOutScrollingBackground();
        waitFrames(10);
        spawnTask(0, -1, 0, 0x200, startCpuDuel, SAI_EXIT_ARG, 0, 0, 0);
        break;
    case AREA_EXIT_DECK_EDITOR:
        SAI_ICON_RUNNING = 0;
        waitFrames(1);
        openDeckEditor(1);
        break;
    case AREA_EXIT_SAVE:
        SAI_ICON_RUNNING = 0;
        waitFrames(1);
        spawnTask(0, -1, 0, 0x400, openSaveScreenFromMap, 4, getCurrentTaskId(), 0, 0);
        break;
    case AREA_EXIT_FUSION:
        SAI_ICON_RUNNING = 0;
        waitFrames(1);
        if (SAI_AREA.exitArg >= 3) {
            SAI_AREA.exitArg = 0;
        }
        spawnTask(0, -1, 0, 0x400, openPartnerFusion, SAI_EXIT_ARG, 0, 0, 0);
        break;
    case AREA_EXIT_EQUIPMENT:
        SAI_ICON_RUNNING = 0;
        waitFrames(1);
        openPartnerEquipment(1);
        break;
    case AREA_EXIT_TITLE_OR_ENDING:
        SAI_ICON_RUNNING = 0;
        waitFrames(10);
        spawnTask(0, -1, 0, 0x800, quitToTitleOrPlayEnding, SAI_EXIT_ARG, 0, 0, 0);
        break;
    }
}

void SAI_drawAreaHud(void) {
    drawWindow(&SAI_MESSAGE_WINDOW, SAI_drawMessageWindow, 0x17);
    SAI_drawChoiceMenu();
    SAI_drawAreaName();
    SAI_drawLocationLabel();
}

void SAI_createPanel(void) {
    SAI_initPanelCover();
    SAI_createPanelFrame();
    SAI_createPanelFrameShadow();
    addFrameCallback((s32)SAI_drawPanel);
}

void SAI_drawPanel(void) {
    if (SAI_PANEL_COVER_ALPHA != 0) {
        SAI_drawPanelCover();
    }
    SAI_drawPanelFrame();
    SAI_drawPanelFrameShadow();
    if (SAI_PANEL_IMAGE_HIDDEN != -1) {
        SAI_drawSprite(SAI_SPRITES[0]);
    }
}

void SAI_freePanel(void) {
    removeFrameCallback(SAI_drawPanel);
    waitFrames(1);
    SAI_freeSprite(SAI_SPRITES[0]);
    SAI_freePanelFrame();
    SAI_freePanelFrameShadow();
}

void SAI_runTalkPanel(void) {
    SAI_AREA.openPanel = 1;
    SAI_AREA.imageHidden = -1;
    SAI_AREA.opening = 1;
    SAI_AREA.closePanel = 0;
    SAI_createPanel();
    SAI_SPRITES[0] = SAI_createSprite(3);
    SAI_setSpriteDepth(SAI_SPRITES[0], 0x24);
    SAI_spinPanel();
    playSoundEffect(10);
    do {
        waitFrames(1);
    } while (SAI_spinPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = 0;
    SAI_toggleMessageWindow(1);
    do {
        waitFrames(1);
    } while (SAI_uncoverPanel() == 0);
    SAI_AREA_MODE = AREA_MODE_SCRIPT;
    do {
        waitFrames(1);
    } while (SAI_AREA.closePanel == 0);
    do {
        waitFrames(1);
    } while (SAI_coverPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = -1;
    playSoundEffect(11);
    do {
        waitFrames(1);
    } while (SAI_spinPanel() == 0);
    SAI_freePanel();
    SAI_OPEN_PANEL = 0;
}

void SAI_stepBrightness(s8 index) {
    s16 value = SAI_OPPONENTS.current[index];

    if (SAI_OPPONENTS.target[index] > SAI_OPPONENTS.current[index]) {
        value += SAI_OPPONENTS.step[index];
        if (SAI_OPPONENTS.target[index] < SAI_OPPONENTS.current[index]) {
            value = SAI_OPPONENTS.target[index];
        }
    } else if (SAI_OPPONENTS.target[index] < SAI_OPPONENTS.current[index]) {
        value -= SAI_OPPONENTS.step[index];
        if (SAI_OPPONENTS.current[index] < SAI_OPPONENTS.target[index]) {
            value = SAI_OPPONENTS.target[index];
        }
    }
    SAI_OPPONENTS.current[index] = value;
}
