#include "common.h"
#include "game.h"
#include "dcb/evo_type_choice.h"
#include "dcb/window.h"
#include "dcb/sound_play.h"
#include "dcb/prim.h"
#include "dcb/evo_trays.h"

void EVO_tickFusionTypeChoice(void) {
    Rect16 rects[6];
    s32 i;
    s32 dx;
    s32 dy;

    if (EVO_FUSION.swapState == 0) {
        if (PAD_STATES[0]->pressed & 0x5000) {
            playSoundEffect(2);
            if (EVO_FUSION.fusionType == 0) {
                EVO_FUSION.swapState = 1;
            } else if (EVO_FUSION.fusionType == 1) {
                EVO_FUSION.swapState = 2;
            }
        } else if (PAD_STATES[0]->pressed & 0x40) {
            playSoundEffect(0);
            EVO_SCRIPT->vars[8] = 1;
            EVO_SCRIPT->vars[1] = EVO_TYPE_CHOICE.side;
            EVO_FUSION.scriptState = 0;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            playSoundEffect(1);
            EVO_SCRIPT->vars[8] = 2;
            EVO_FUSION.scriptState = 0;
        }
    } else if (EVO_FUSION.swapState == 1) {
        EVO_FUSION.swapTimer++;
        if (EVO_FUSION.swapTimer >= 21) {
            EVO_FUSION.swapTimer = 20;
            EVO_FUSION.swapState = 0;
            EVO_FUSION.fusionType = 1;
        }
    } else if (EVO_FUSION.swapState == 2) {
        EVO_FUSION.swapTimer++;
        if (EVO_FUSION.swapTimer >= 41) {
            EVO_FUSION.swapTimer = 0;
            EVO_FUSION.swapState = 0;
            EVO_FUSION.fusionType = 0;
        }
    }
    EVO_TYPE_CHOICE.side = EVO_FUSION.fusionType;
    for (i = 0; i < 3; i++) {
        EVO_WINDOWS[i].win.brightness = 0x80 - EVO_TYPE_CHOICE.side * 64;
        EVO_WINDOWS[i + 3].win.brightness = EVO_TYPE_CHOICE.side * 64 + 64;
    }
    dy = rsin(EVO_FUSION.swapTimer * 1024 / 20) * 40 / 4096;
    dx = rsin(EVO_FUSION.swapTimer * 2048 / 20) * 60 / 4096;
    for (i = 0; i < 3; i++) {
        rects[i] = EVO_WINDOW_DEFS[i].rect;
        rects[i + 3] = EVO_WINDOW_DEFS[i + 3].rect;
        rects[i].x += dx;
        rects[i].y += dy;
        rects[i + 3].x -= dx;
        rects[i + 3].y -= dy;
        EVO_WINDOWS[i].win.animFrame = EVO_WINDOWS[i].win.animFrames;
        EVO_WINDOWS[i + 3].win.animFrame = EVO_WINDOWS[i + 3].win.animFrames;
        animateWindowTo(&EVO_WINDOWS[i].win, &rects[i]);
        animateWindowTo(&EVO_WINDOWS[i + 3].win, &rects[i + 3]);
    }
    for (i = 0; i < 6; i++) {
        if (i < 3) {
            EVO_WINDOWS[i].z = EVO_TYPE_CHOICE.side + 30;
        } else {
            EVO_WINDOWS[i].z = (EVO_TYPE_CHOICE.side + 30) ^ 1;
        }
    }
}

void EVO_openFusionTypeChoice(void) {
    s32 i;

    EVO_MAX_CARD_LEVEL = EVO_SCRIPT->vars[12];
    for (i = 0; i < 3; i++) {
        EVO_WINDOWS[i].win.brightness = 0x80 - EVO_TYPE_CHOICE.side * 64;
        EVO_WINDOWS[i + 3].win.brightness = EVO_TYPE_CHOICE.side * 64 + 64;
    }
    if (EVO_FUSION.typeChoiceOpen == 0) {
        EVO_FUSION.typeChoiceOpen = 1;
        EVO_FUSION.swapState = 0;
        EVO_FUSION.step = 1;
        EVO_SCRIPT->vars[8] = -1;
        for (i = 0; i < 6; i++) {
            if (EVO_FUSION.fusionType == 0) {
                animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i].rect);
            } else if (i < 3) {
                animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i + 3].rect);
            } else {
                animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i - 3].rect);
            }
        }
        waitFrames(30);
    }
}

void EVO_closeFusionTypeChoice(void) {
    s32 i;

    if (EVO_FUSION.typeChoiceOpen == 1) {
        EVO_FUSION.typeChoiceOpen = 0;
        EVO_FUSION.step = 0;
        for (i = 0; i < 6; i++) {
            EVO_WINDOWS[i].win.animFrame = EVO_WINDOWS[i].win.animFrames - 1;
            animateWindowTo(&EVO_WINDOWS[i].win, (Rect16 *)-1);
        }
    }
}

void EVO_drawFusionTypeIcon(EvoWindow *w) {
    Rect16 uv;
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;

    if (w->isPartner == 0) {
        uv.x = 0;
        uv.y = 0;
        uv.w = 0x40;
        uv.h = 0x40;
        drawTexturedSprite(x, y, &uv, 0x97, 0x7C58, z, w->win.brightness, -1);
    } else {
        uv.x = 0x40;
        uv.y = 0;
        uv.w = 0x40;
        uv.h = 0x40;
        drawTexturedSprite(x, y, &uv, 0x97, 0x7C58, z, w->win.brightness, -1);
    }
}
