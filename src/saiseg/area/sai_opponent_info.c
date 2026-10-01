#include "common.h"
#include "game.h"
#include "dcb/sai_opponent_info.h"
#include "dcb/text.h"
#include "dcb/heap.h"
#include "dcb/frame_callback.h"
#include "dcb/task.h"
#include "dcb/sound_play.h"
#include "dcb/prim.h"
#include "dcb/prim_util.h"
#include "dcb/pad.h"
#include "dcb/saiseg.h"
#include "dcb/sai_labels.h"
#include "dcb/sai_panel.h"
#include "dcb/sai_area.h"
#include "dcb/sai_splash.h"
#include "dcb/sai_sprite.h"
#include "dcb/sai_world_map.h"

extern OpponentInfo *SAI_OPPONENT_INFO;

void SAI_drawOpponentStats(void);
s32 SAI_moveOpponentPortraits(void);

void SAI_drawOpponentInfo(void) {
    s32 i;

    if (SAI_OPPONENT_INFO->showWipe == 1) {
        SAI_drawOpponentStats();
        SAI_OPPONENT_INFO->fades[0].polys[FRAME_BUFFER_INDEX].x0 = SAI_OPPONENT_INFO->fades[0].polys[FRAME_BUFFER_INDEX].x2 = SAI_OPPONENT_INFO->wipeX;
        addPrim(&CURRENT_FRAME_BUFFER->ot[35], &SAI_OPPONENT_INFO->fades[0].polys[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[35], &SAI_OPPONENT_INFO->fades[0].tpages[FRAME_BUFFER_INDEX]);
    }
    if (SAI_OPPONENT_INFO->phase < 2) {
        setRGB0(&SAI_OPPONENT_INFO->fades[1].polys[FRAME_BUFFER_INDEX], SAI_OPPONENT_INFO->alpha, SAI_OPPONENT_INFO->alpha, SAI_OPPONENT_INFO->alpha);
        addPrim(&CURRENT_FRAME_BUFFER->ot[32], &SAI_OPPONENT_INFO->fades[1].polys[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[32], &SAI_OPPONENT_INFO->fades[1].tpages[FRAME_BUFFER_INDEX]);
    }
    for (i = 0; i < 2; i++) {
        SAI_stepBrightness(i);
    }
    SAI_setSpriteBrightness(SAI_SPRITES[43], SAI_OPPONENTS.current[1]);
    SAI_setSpriteBrightness(SAI_SPRITES[45], SAI_OPPONENTS.current[0]);
    for (i = 0; i < 4; i++) {
        SAI_drawSprite(SAI_SPRITES[i + 42]);
    }
}

void SAI_drawOpponentStats(void) {
    Rect16 uv;
    char *labels[4] = { "Deck Color", "Attack", "Defense", "Digivolve Speed" };
    char buf[0x19];
    s32 x;
    s32 i;
    s32 j;

    x = (SAI_OPPONENT_INFO->entries[0].progress * 47 + (20 - SAI_OPPONENT_INFO->entries[0].progress) * 330) / 20;
    drawText(x + 0x56, 0x4B, SAI_SCRIPT[0]->regs[9], 7, 0x22);
    uv.x = 0x60;
    uv.y = SAI_SCRIPT[0]->regs[11] * 18;
    uv.w = 0x20;
    uv.h = 0x12;
    drawTexturedSprite(x, 0x45, &uv, 0x2B, 0x3A61, 0x22, SAI_OPPONENT_INFO->brightness, 0);
    uv.x = 0xC4;
    uv.y = 0;
    uv.w = 0x34;
    uv.h = 0x12;
    drawTexturedSprite(x + 0x20, 0x45, &uv, 0x2A, 0x3AA1, 0x22, SAI_OPPONENT_INFO->brightness, 0);
    x = (SAI_OPPONENT_INFO->entries[1].progress * 42 + (20 - SAI_OPPONENT_INFO->entries[1].progress) * 330) / 20;
    drawLargeText(x, 0x58, (s32)"ABILITIES", 7, 0x22);
    for (i = 0; i < 4; i++) {
        x = (SAI_OPPONENT_INFO->entries[i + 2].progress * 54 + (20 - SAI_OPPONENT_INFO->entries[i + 2].progress) * 330) / 20;
        drawText(x, i * 12 + 0x64, (s32)labels[i], 7, 0x22);
        bzero((Scene3D *)buf, 0x19);
        switch (i) {
        case 0:
            for (j = 0; j < 5; j++) {
                if (SAI_SCRIPT[0]->regs[5] & (0x10 >> j)) {
                    s32 dx;

                    uv.x = (j + 0x13) * 4;
                    uv.y = 0;
                    uv.w = 4;
                    uv.h = 10;
                    dx = j * 5 + 0x64;
                    drawTexturedSprite(x + dx, i * 12 + 0x64, &uv, 0xA, 0x3A21, 0x22, 0x80, -1);
                }
            }
            break;
        case 1:
            sprintf(buf, "%d", SAI_SCRIPT[0]->regs[6]);
            break;
        case 2:
            sprintf(buf, "%d", SAI_SCRIPT[0]->regs[7]);
            break;
        case 3:
            sprintf(buf, "%d", SAI_SCRIPT[0]->regs[8]);
            break;
        }
        if (i != 0) {
            drawText(x + 0x64, i * 12 + 0x64, (s32)buf, 7, 0x22);
        }
    }
}

void SAI_freeOpponentInfo(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        SAI_freeSprite(SAI_SPRITES[i + 42]);
    }
    *(OpponentList *)((SessionData *)SESSION_DATA)->areaSession = SAI_OPPONENTS;
}

void SAI_initOpponentInfo(void) {
    Fade *fade = SAI_OPPONENT_INFO->fades;
    s32 k;
    s32 i;

    for (k = 0; k < 2; k++, fade++) {
        for (i = 0; i < 2; i++) {
            SetPolyF4(&fade->polys[i]);
            SetSemiTrans(&fade->polys[i], 1);
            if (k == 0) {
                SetDrawTPage(&fade->tpages[i], 0, 0, 0x40);
                fade->polys[i].r0 = fade->polys[i].g0 = fade->polys[i].b0 = 0x60;
                setPrimQuadRect(&fade->polys[i], 0x140, 0x42, 1, 0x5A);
            } else {
                setPrimQuadRect(&fade->polys[i], 0, 0x42, 0x140, 0x5A);
                SetDrawTPage(&fade->tpages[i], 0, 0, 0x20);
                fade->polys[i].r0 = fade->polys[i].g0 = fade->polys[i].b0 = 0;
            }
        }
    }
    SAI_SPRITES[42] = SAI_createSprite(8);
    SAI_setSpriteDepth(SAI_SPRITES[42], 0x21);
    SAI_setSpriteBlendMode(SAI_SPRITES[42], 1);
    SAI_SPRITES[43] = SAI_createSprite(0x3B);
    SAI_setSpriteDepth(SAI_SPRITES[43], 0x21);
    SAI_SPRITES[42]->pos.vx = 0xD2;
    SAI_SPRITES[42]->pos.vy = 0x15;
    SAI_SPRITES[43]->pos.vx = SAI_SPRITES[42]->pos.vx = 0xD1;
    SAI_SPRITES[43]->pos.vy = SAI_SPRITES[42]->pos.vy = 0x14;
    SAI_SPRITES[44] = SAI_createSprite(8);
    SAI_setSpriteDepth(SAI_SPRITES[44], 0x21);
    SAI_setSpriteBlendMode(SAI_SPRITES[44], 1);
    SAI_SPRITES[45] = SAI_createSprite(0x3B);
    SAI_setSpriteDepth(SAI_SPRITES[45], 0x21);
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx = -0xC8;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy = 0x15;
    SAI_OPPONENTS.target[0] = SAI_OPPONENTS.current[0] = 0x80;
    SAI_OPPONENTS.target[1] = SAI_OPPONENTS.current[1] = 0x80;
    SAI_OPPONENT_INFO->wipeX = 0x140;
    SAI_OPPONENT_INFO->unkC2 = 0;
    SAI_OPPONENT_INFO->state = 0;
    SAI_OPPONENT_INFO->timer = 0;
    SAI_OPPONENT_INFO->statsShown = 0;
    SAI_OPPONENT_INFO->phase = 2;
}

void SAI_showOpponentInfo(void) {
    Rect16 rect;
    s32 i;

    if (SESSION->shownOpponent != (s8)SAI_SCRIPT[0]->regs[10]) {
        SAI_AREA_MODE = AREA_MODE_BUSY;
        if (SAI_OPPONENT_INFO->statsShown == 1) {
            do {
                waitFrames(1);
                SAI_OPPONENT_INFO->state = 4;
            } while (SAI_moveOpponentPortraits() != 0);
        }
        for (i = 0; i < 7; i++) {
            SAI_OPPONENT_INFO->entries[i].delay = rand() % 30;
            SAI_OPPONENT_INFO->entries[i].progress = 0;
        }
        SAI_OPPONENT_INFO->statsShown = 0;
        SESSION->shownOpponent = SAI_SCRIPT[0]->regs[10];
        SAI_OPPONENT_INFO->timer = 0;
        SAI_OPPONENT_INFO->unkC5 = 1;
        SAI_OPPONENT_INFO->state = 1;
        SAI_OPPONENT_INFO->showWipe = 1;
        SAI_OPPONENT_INFO->alpha = 0;
        SAI_OPPONENT_INFO->phase = 0;
        SAI_OPPONENT_INFO->delay = 360;
        SAI_OPPONENT_INFO->brightness = 0x80;
        SAI_OPPONENT_INFO->pulsePhase = 0;
        rect.x = (SAI_SCRIPT[0]->regs[10] & 3) * 32 + 0x280;
        rect.y = ((u32)SAI_SCRIPT[0]->regs[10] >> 2) * 56 + 0x100;
        rect.w = 0x3F;
        rect.h = 0x38;
        SAI_setSpriteImage8Bit(SAI_SPRITES[43], &rect, 0);
        SAI_SPRITES[43]->quads[0].clut = getClut(0x280, 0x1FB);
        SAI_SPRITES[43]->quads[1].clut = getClut(0x280, 0x1FB);
    } else {
        SAI_OPPONENT_INFO->state = 0;
        SAI_AREA_MODE = AREA_MODE_SCRIPT;
    }
}

s32 SAI_slideInOpponentStats(void) {
    s32 result = 1;
    s32 finished = -1;
    s32 i;

    SAI_OPPONENT_INFO->wipeX -= 15;
    if (SAI_OPPONENT_INFO->wipeX <= 0) {
        SAI_OPPONENT_INFO->wipeX = 0;
        for (i = 0; i < 7; i++) {
            if (SAI_OPPONENT_INFO->entries[i].delay <= 0) {
                SAI_OPPONENT_INFO->entries[i].progress++;
                if (SAI_OPPONENT_INFO->entries[i].progress >= 20) {
                    SAI_OPPONENT_INFO->entries[i].progress = 20;
                    finished++;
                }
            } else {
                SAI_OPPONENT_INFO->entries[i].delay--;
            }
        }
    }
    if (finished >= 6) {
        SAI_OPPONENT_INFO->state = 2;
        result = 0;
    }
    return result;
}

s32 SAI_slideInOpponentPortrait(void) {
    s16 alpha = SAI_OPPONENT_INFO->brightness;
    s32 result = 1;

    if (SAI_OPPONENT_INFO->timer == 19) {
        playSoundEffect(0x10);
    }
    SAI_OPPONENT_INFO->timer++;
    if (SAI_OPPONENT_INFO->timer > 20) {
        SAI_OPPONENT_INFO->timer = 20;
    }
    SAI_SPRITES[43]->pos.vx = (SAI_OPPONENT_INFO->timer * 60 + (20 - SAI_OPPONENT_INFO->timer) * 210) / 20;
    SAI_SPRITES[43]->pos.vy = -1;
    if (SAI_OPPONENT_INFO->timer == 20) {
        if (SAI_OPPONENT_INFO->pulsePhase == 0) {
            alpha += 8;
            if (alpha > 0xFF) {
                alpha = 0xFF;
                SAI_OPPONENT_INFO->pulsePhase = 1;
            }
        } else {
            alpha -= 8;
            if (alpha < 0x80) {
                alpha = 0x80;
                SAI_OPPONENT_INFO->pulsePhase = 2;
                SAI_OPPONENT_INFO->state = 5;
                result = 0;
            }
        }
        SAI_OPPONENT_INFO->brightness = alpha;
    }
    if (result == 0) {
        SAI_OPPONENT_INFO->timer = 0;
    }
    return result;
}

s32 SAI_moveOpponentPortraits(void) {
    s32 result = 1;

    if (SAI_OPPONENT_INFO->state == 4) {
        SAI_OPPONENT_INFO->timer--;
        if (SAI_OPPONENT_INFO->timer < 0) {
            SAI_OPPONENT_INFO->timer = 0;
            result = 0;
        }
    } else {
        SAI_OPPONENT_INFO->timer++;
        if (SAI_OPPONENT_INFO->timer > 20) {
            SAI_OPPONENT_INFO->statsShown = 1;
            SAI_OPPONENT_INFO->timer = 20;
            SAI_OPPONENT_INFO->state = 0;
            SAI_AREA_MODE = AREA_MODE_SCRIPT;
            result = 0;
        }
    }
    SAI_SPRITES[44]->pos.vx = (SAI_OPPONENT_INFO->timer * -103 + (20 - SAI_OPPONENT_INFO->timer) * -200) / 20;
    SAI_SPRITES[44]->pos.vy = 0x15;
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy;
    if (SAI_OPPONENT_INFO->unkC5 == 1) {
        SAI_SPRITES[42]->pos.vx = (SAI_OPPONENT_INFO->timer * 108 + (20 - SAI_OPPONENT_INFO->timer) * 60) / 20;
        SAI_SPRITES[42]->pos.vy = (SAI_OPPONENT_INFO->timer * 21 + (20 - SAI_OPPONENT_INFO->timer) * -1) / 20;
        SAI_SPRITES[43]->pos.vx = SAI_SPRITES[42]->pos.vx - 1;
        SAI_SPRITES[43]->pos.vy = SAI_SPRITES[42]->pos.vy - 1;
    } else {
        SAI_SPRITES[42]->pos.vx = (SAI_OPPONENT_INFO->timer * 108 + (20 - SAI_OPPONENT_INFO->timer) * 210) / 20;
        SAI_SPRITES[42]->pos.vy = 0x15;
        SAI_SPRITES[43]->pos.vx = SAI_SPRITES[42]->pos.vx - 1;
        SAI_SPRITES[43]->pos.vy = SAI_SPRITES[42]->pos.vy - 1;
    }
    if (result == 0) {
        SAI_OPPONENT_INFO->unkC5 = 0;
    }
    return result;
}

void SAI_tickOpponentBanner(void) {
    s16 alpha = SAI_OPPONENT_INFO->alpha;

    if (SAI_OPPONENT_INFO->delay == 0) {
        if (SAI_OPPONENT_INFO->phase == 0) {
            if (alpha == 0) {
                playSoundEffect(0x11);
            }
            alpha += 8;
            if (alpha > 0xFF) {
                alpha = 0xFF;
                SAI_OPPONENT_INFO->phase = 1;
                SAI_OPPONENT_INFO->showWipe = 0;
                SAI_SPRITES[42]->pos.vx = SAI_SPRITES[43]->pos.vx;
                SAI_SPRITES[42]->pos.vy = SAI_SPRITES[43]->pos.vy;
                SAI_SPRITES[43]->pos.vx--;
                SAI_SPRITES[43]->pos.vy--;
            }
        } else {
            alpha -= 8;
            if (alpha < 0) {
                alpha = 0;
                SAI_OPPONENT_INFO->phase = 2;
                SAI_OPPONENT_INFO->state = 3;
            }
        }
        SAI_OPPONENT_INFO->alpha = alpha;
    } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
        SAI_OPPONENT_INFO->delay = 0;
    } else if (SAI_OPPONENT_INFO->delay != 0) {
        SAI_OPPONENT_INFO->delay--;
    }
}

void SAI_runOpponentInfoPanel(s32 index) {
    Rect16 rect;

    SAI_OPPONENT_INFO = allocTaskHeapBlock(0xCC);
    SAI_initOpponentInfo();
    addFrameCallback((s32)SAI_drawOpponentInfo);
    if (SESSION->resumeMode != 0) {
        SAI_OPPONENTS = SESSION->opponents;
        SAI_LOCATION = SESSION->location;
        SAI_SCRIPT[0]->script->pc = SAI_SCRIPT[0]->script->start + SESSION->scriptOffset;
        SAI_SCRIPT[0]->regs[1] = (s8)(SESSION->duelResult ^ 1);
        rect.x = SESSION->shownOpponent % 4 * 32 + 0x280;
        rect.y = SESSION->shownOpponent / 4 * 56 + 0x100;
        rect.w = 0x3F;
        rect.h = 0x38;
        SAI_setSpriteImage8Bit(SAI_SPRITES[43], &rect, 0);
        SAI_SPRITES[43]->quads[0].clut = getClut(0x280, 0x1FB);
        SAI_SPRITES[43]->quads[1].clut = getClut(0x280, 0x1FB);
        SAI_OPPONENT_INFO->timer = 0;
        SAI_OPPONENT_INFO->unkC5 = 0;
        SAI_OPPONENT_INFO->state = 3;
        SAI_AREA_MODE = AREA_MODE_BUSY;
        SAI_OPPONENT_INFO->showWipe = 0;
    } else {
        SESSION->shownOpponent = -1;
    }
    SAI_AREA.openPanel = SAI_AREA.location + 1;
    SAI_AREA.closePanel = 0;
    SAI_AREA.imageHidden = -1;
    SAI_AREA.opening = 1;
    SAI_createPanel();
    SAI_SPRITES[0] = SAI_createSprite(4);
    SAI_setSpriteDepth(SAI_SPRITES[0], 0x24);
    SAI_spinPanel();
    playSoundEffect(10);
    do {
        waitFrames(1);
    } while (SAI_spinPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = 0;
    SAI_toggleMessageWindow(1);
    if (SESSION->resumeMode == 0) {
        spawnTask(0, -1, 0, 0x800, SAI_runSplash, index - 1, getCurrentTaskId(), 0, 0);
        waitFrames(0x7FFFFFFF);
    }
    do {
        waitFrames(1);
    } while (SAI_uncoverPanel() == 0);
    if (SESSION->resumeMode == 0) {
        SAI_AREA_MODE = AREA_MODE_SCRIPT;
    } else {
        SESSION->resumeMode = 0;
    }
    do {
        waitFrames(1);
        switch (SAI_OPPONENT_INFO->state) {
        case 0:
            break;
        case 1:
            SAI_slideInOpponentStats();
            break;
        case 2:
            SAI_slideInOpponentPortrait();
            break;
        case 3:
        case 4:
            SAI_moveOpponentPortraits();
            break;
        case 5:
            SAI_tickOpponentBanner();
            break;
        }
    } while (SAI_AREA.closePanel == 0);
    do {
        waitFrames(1);
        SAI_OPPONENT_INFO->state = 4;
    } while (SAI_moveOpponentPortraits() != 0);
    removeFrameCallback((s32)SAI_drawOpponentInfo);
    do {
        waitFrames(1);
    } while (SAI_coverPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = -1;
    playSoundEffect(11);
    do {
        waitFrames(1);
    } while (SAI_spinPanel() == 0);
    SAI_freePanel();
    SAI_freeOpponentInfo();
    SAI_OPEN_PANEL = 0;
}
