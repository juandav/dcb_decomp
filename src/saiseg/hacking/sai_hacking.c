#include "common.h"
#include "game.h"
#include "dcb/sai_hacking.h"
#include "dcb/window.h"
#include "dcb/text.h"
#include "dcb/frame_callback.h"
#include "dcb/task.h"
#include "dcb/hacking_shell.h"
#include "dcb/sound_play.h"
#include "dcb/scroll_bg.h"
#include "dcb/memcard.h"
#include "dcb/prim.h"
#include "dcb/prim_util.h"
#include "dcb/saiseg.h"
#include "dcb/sai_area.h"
#include "dcb/sai_data.h"

extern UiWindow SAI_ERROR_WINDOWS[5];
extern u8 SAI_HACK_OVERLAY_FLICKER;
extern u8 SAI_HACK_OVERLAY_STATE;

const char SAI_STR_SYSTEM_ERROR[] = "               *c6SYSTEM ERROR\n*c2 Illegal Sharing: \n*c7 Unauthorized command was used.";

void SAI_openErrorWindow(UiWindow *window, WindowDef *def) {
    measureText((u8 *)SAI_STR_SYSTEM_ERROR);
    def->rect.w = (TEXT_WIDTH + 1) / 2 * 2;
    def->rect.h = (TEXT_HEIGHT + 1) / 2 * 2;
    openWindow(window, def, -1, (s16 *)-1, def->flags, def->style, def->brightness, 6);
    if (def->label != 0) {
        window->label = def->label;
    }
    window->labelPalette = def->labelPalette;
    animateWindowTo(window, (Rect16 *)-1);
}

void SAI_drawErrorText(UiWindow *window) {
    char buf[0x48]; /* unused, but it is in the original stack frame */

    drawText(window->originX, window->originY, (s32)SAI_STR_SYSTEM_ERROR, 0, window->z);
}

void (*SAI_ERROR_WINDOW_DRAW_FUNCS[5])() = {
    SAI_drawErrorText,
    SAI_drawErrorText,
    SAI_drawErrorText,
    SAI_drawErrorText,
    SAI_drawErrorText,
};

void SAI_drawErrorWindows(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        drawWindow(&SAI_ERROR_WINDOWS[i], SAI_ERROR_WINDOW_DRAW_FUNCS[i], 9);
    }
}

void SAI_runSystemErrorHack(void) {
    stopMusic();
    addFrameCallback((s32)SAI_drawErrorWindows);
    setBackgroundScrollMode(1);
    spawnTask(0, -1, 0, 0x800, runHackingSequence, 0, getCurrentTaskId(), 0, 0);
    waitFrames(360);
    playMenuSound(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[0], &SAI_ERROR_WINDOW_DEFS[0].rect);
    waitFrames(7);
    playMenuSound(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[1], &SAI_ERROR_WINDOW_DEFS[1].rect);
    waitFrames(8);
    playMenuSound(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[2], &SAI_ERROR_WINDOW_DEFS[2].rect);
    waitFrames(3);
    playMenuSound(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[3], &SAI_ERROR_WINDOW_DEFS[3].rect);
    waitFrames(2);
    playMenuSound(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[4], &SAI_ERROR_WINDOW_DEFS[4].rect);
    waitFrames(110);
    playSoundEffect(0x18);
    waitFrames(480);
    playSoundEffect(0x19);
    waitFrames(120);
    playSoundEffect(0x19);
    waitFrames(240);
    animateWindowTo(&SAI_ERROR_WINDOWS[0], (Rect16 *)-1);
    playSoundEffect(0x19);
    waitFrames(4);
    animateWindowTo(&SAI_ERROR_WINDOWS[1], (Rect16 *)-1);
    playSoundEffect(0x19);
    waitFrames(2);
    animateWindowTo(&SAI_ERROR_WINDOWS[2], (Rect16 *)-1);
    playSoundEffect(0x19);
    waitFrames(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[3], (Rect16 *)-1);
    playSoundEffect(0x19);
    waitFrames(1);
    animateWindowTo(&SAI_ERROR_WINDOWS[4], (Rect16 *)-1);
    playSoundEffect(0x19);
    waitFrames(120);
    SAI_glitchVram(1);
    removeFrameCallback((s32)SAI_drawErrorWindows);
    waitFrames(170);
    playSoundEffect(0x18);
    waitFrames(30);
}

void SAI_runHackingScene1(void) {
    spawnTask(0, -1, 0, 0x800, runHackingSequence, 1, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
}

void SAI_runHackingScene3(void) {
    spawnTask(0, -1, 0, 0x800, runHackingSequence, 3, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
}

void SAI_runHackingEvent(s32 mode, s32 task) {
    s32 i;

    for (i = 0; i < 5; i++) {
        SAI_openErrorWindow(&SAI_ERROR_WINDOWS[i], &SAI_ERROR_WINDOW_DEFS[i]);
    }
    switch (mode) {
    case 0:
        SAI_runSystemErrorHack();
        break;
    case 1:
        SAI_runHackingScene1();
        break;
    case 2:
        SAI_runHackingScene3();
        break;
    }
    SAI_AREA_MODE = AREA_MODE_SCRIPT;
    resumeTask(task);
}

void SAI_flickerHackOverlay(void) {
    SAI_HACK_OVERLAY_FLICKER = 6;
}

void SAI_closeHackOverlay(void) {
    SAI_HACK_OVERLAY_STATE = 2;
}

void SAI_runHackOverlay(void) {
    Rect16 uv = { 0, 0, 0x80, 0x80 };
    DrTPage tpages[2];
    PolyF4 polys[2];
    u8 brightness = 0;
    s32 i;

    SAI_HACK_OVERLAY_STATE = 1;
    for (i = 0; i < 2; i++) {
        SetDrawTPage(&tpages[i], 0, 0, 0x40);
        SetPolyF4(&polys[i]);
        SetSemiTrans(&polys[i], 1);
        setPrimQuadRect(&polys[i], 0, 0, 0x140, 0xF0);
        setPrimRgb0(&polys[i], brightness, brightness, brightness);
    }
    do {
        waitFrames(1);
        if (SAI_HACK_OVERLAY_STATE == 1) {
            if (brightness < 0xF7) {
                brightness += 8;
            } else {
                brightness = 0xFF;
            }
        } else if (SAI_HACK_OVERLAY_STATE == 2) {
            if (brightness > 8) {
                brightness -= 8;
            } else {
                brightness = 0;
                SAI_HACK_OVERLAY_STATE = 0;
            }
        }
        if (SAI_HACK_OVERLAY_FLICKER != 0) {
            SAI_HACK_OVERLAY_FLICKER--;
        }
        if (SAI_HACK_OVERLAY_STATE != 0) {
            if ((SAI_HACK_OVERLAY_FLICKER & 1) && brightness == 0xFF) {
                drawTexturedSprite(0x60, 0x1A, &uv, 0x95, 0x7C00, 0x18, 0x80, -1);
            }
            setPrimRgb0(&polys[FRAME_BUFFER_INDEX], brightness, brightness, brightness);
            addPrim(&CURRENT_FRAME_BUFFER->ot[24], &polys[FRAME_BUFFER_INDEX]);
            addPrim(&CURRENT_FRAME_BUFFER->ot[24], &tpages[FRAME_BUFFER_INDEX]);
        }
    } while (SAI_HACK_OVERLAY_STATE != 0);
    waitFrames(1);
}
