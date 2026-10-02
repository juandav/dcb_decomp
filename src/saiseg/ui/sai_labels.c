#include "common.h"
#include "game.h"
#include "dcb/sai_labels.h"
#include "dcb/window.h"
#include "dcb/script.h"
#include "dcb/sai_sprite.h"
#include "dcb/sai_world_map.h"

void SAI_openCenteredWindow(UiWindow *window, Rect16 pos, s32 label, s32 style, s32 brightness) {
    Rect16 rect;
    Rect16 unused; /* unused, but it is in the original stack frame */
    Rect16 view;

    rect.x = pos.x - pos.w / 2;
    rect.y = pos.y - pos.h / 2;
    rect.w = pos.w & ~1;
    rect.h = pos.h & ~1;
    view.x = 0;
    view.y = 0;
    view.w = (pos.w + 10) & ~1;
    view.h = 0x2000;
    openWindow(window, &rect, -1, (s16 *)&view, style, brightness, 0x80, 0x10);
    window->label = label;
    window->labelPalette = 8;
}

void SAI_toggleMessageWindow(s32 close) {
    Rect16 rect;
    Rect16 pos = { 0xA1, 0xCF, 0x12A, 0x28 };
    char *title = "MESSAGE";

    if (close == 0) {
        SAI_openCenteredWindow(&SAI_MESSAGE_WINDOW, pos, (s32)title, 8, 0x51);
        animateWindowTo(&SAI_MESSAGE_WINDOW, (Rect16 *)-1);
    } else {
        rect.x = pos.x - pos.w / 2;
        rect.y = pos.y - pos.h / 2;
        rect.w = pos.w & ~1;
        rect.h = pos.h & ~1;
        animateWindowTo(&SAI_MESSAGE_WINDOW, &rect);
    }
}

void SAI_runChoiceScript(ScriptRunner *runner) {
    s32 result;

    do {
        result = runScriptToNextEvent(runner->script, runner->regs);
        if (result == 1) {
            if (runner->script->eventOp == 10) {
                if (runner->script->eventArg == 14) {
                    SAI_AREA.choiceStates[runner->regs[1] - 1] = result;
                }
            }
            return;
        }
        clearScriptBusy(runner->script);
    } while (result != 0);
}

void SAI_createAreaName(void) {
    Rect16 rect;

    SAI_SPRITES[21] = SAI_createSprite(0x1E);
    SAI_SPRITES[21]->pos.vx = 0x33;
    SAI_SPRITES[21]->pos.vy = -0xBC;
    SAI_setSpriteDepth(SAI_SPRITES[21], 0x21);
    SAI_SPRITES[22] = SAI_createSprite(0x1F);
    SAI_SPRITES[22]->pos.vx = 0x34;
    SAI_SPRITES[22]->pos.vy = -0xBB;
    SAI_setSpriteDepth(SAI_SPRITES[22], 0x21);
    rect.x = ((PlayerProfile *)PLAYER_PROFILES)->areaId / 6 * 24 + 0x340;
    rect.y = ((PlayerProfile *)PLAYER_PROFILES)->areaId % 6 * 20;
    rect.w = 0x60;
    rect.h = 0x14;
    SAI_setSpriteImage4Bit(SAI_SPRITES[22], &rect);
}

void SAI_createLocationLabel(void) {
    s32 unused[2]; /* unused, but it is in the original stack frame */

    SAI_AREA.location = -1;
    SAI_AREA.shownLocation = -1;
    SAI_SPRITES[23] = SAI_createSprite(6);
    SAI_SPRITES[23]->pos.vx = -0x5F;
    SAI_SPRITES[23]->pos.vy = -0x98;
    SAI_setSpriteDepth(SAI_SPRITES[23], 0x21);
    SAI_SPRITES[24] = SAI_createSprite(7);
    SAI_SPRITES[24]->pos.vx = -0x5F;
    SAI_SPRITES[24]->pos.vy = -0x60;
    SAI_SPRITES[24]->pos.vy = -0x97;
    SAI_setSpriteDepth(SAI_SPRITES[24], 0x22);
    SAI_AREA.labelSlide = 0;
}

void SAI_drawAreaName(void) {
    SAI_drawSprite(SAI_SPRITES[22]);
    SAI_drawSprite(SAI_SPRITES[21]);
}

void SAI_drawLocationLabel(void) {
    Rect16 rect;

    if (SAI_LOCATION == -1) {
        rect.x = 0x2E0;
        rect.y = 0x90;
    } else {
        rect.x = 0x2E0;
        rect.y = SAI_LOCATION * 24;
    }
    rect.w = 0x74;
    rect.h = 0x18;
    if (SAI_AREA.shownLocation != SAI_AREA.location) {
        SAI_AREA.labelSlide--;
        if (SAI_AREA.labelSlide < 0) {
            SAI_AREA.labelSlide = 0;
            SAI_AREA.shownLocation = SAI_AREA.location;
            SAI_setSpriteImage4Bit(SAI_SPRITES[24], &rect);
        }
    } else if (SAI_AREA.location != -1) {
        SAI_AREA.labelSlide++;
        if (SAI_AREA.labelSlide > 30) {
            SAI_AREA.labelSlide = 30;
        }
    } else {
        SAI_AREA.labelSlide = 0;
    }
    SAI_SPRITES[23]->pos.vy = (SAI_AREA.labelSlide * -97 + (30 - SAI_AREA.labelSlide) * -152) / 30;
    SAI_SPRITES[24]->pos.vy = SAI_SPRITES[23]->pos.vy + 2;
    SAI_drawSprite(SAI_SPRITES[24]);
    SAI_drawSprite(SAI_SPRITES[23]);
}

void SAI_openWindows(UiWindow *window, WindowDef *def, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        openWindow(window, def, -1, (s16 *)-1, def->flags, def->style, def->brightness, 12);
        if (def->label != 0) {
            window->label = def->label;
        }
        window->labelPalette = def->labelPalette;
        window++;
        def++;
    }
}
