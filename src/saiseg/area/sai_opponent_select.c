#include "common.h"
#include "game.h"
#include "dcb/sai_opponent_select.h"
#include "dcb/frame_callback.h"
#include "dcb/sound_play.h"
#include "dcb/pad.h"
#include "dcb/saiseg.h"
#include "dcb/sai_flags.h"
#include "dcb/sai_labels.h"
#include "dcb/sai_panel.h"
#include "dcb/sai_area.h"
#include "dcb/sai_sprite.h"
#include "dcb/sai_world_map.h"

typedef struct {
    /* 0x000 */ OpponentList opponents;
    /* 0x12C */ OpponentSelect select;
} SaveBlock;

extern s16 SAI_SELECT_DELAYS[6];
extern s32 SAI_SELECT_BLINK[6];
extern s8 SAI_SELECT_SELECTED;
extern u8 SAI_SELECT_EMPTY_BLINK[6];
extern s8 SAI_SELECT_STATE;
extern s32 SAI_SELECT_PROGRESS[6];

void SAI_tickOpponentSelectInput(void) {
    s8 card;
    s32 i;
    s32 *timers;

    if (PAD_STATES[0]->pressed & (PAD_UP | PAD_DOWN)) {
        SAI_AREA.select.selected += 3;
        if (SAI_AREA.select.selected >= 6) {
            SAI_AREA.select.selected -= 6;
        }
    } else if ((u16)PAD_STATES[0]->pressed & PAD_LEFT) {
        SAI_AREA.select.selected--;
        if (SAI_AREA.select.selected < 0) {
            SAI_AREA.select.selected = 5;
        }
    } else if (PAD_STATES[0]->pressed & PAD_RIGHT) {
        SAI_AREA.select.selected++;
        if (SAI_AREA.select.selected >= 6) {
            SAI_AREA.select.selected = 0;
        }
    } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
        playSoundEffect(1);
        SAI_AREA.mode = AREA_MODE_SCRIPT;
        SAI_AREA.select.unk5E = 0;
        SAI_SCRIPT[0]->regs[2] = -1;
    } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
        card = SAI_OPPONENTS.ids[SAI_AREA.select.page * 6 + SAI_AREA.select.selected];
        if (SAI_SCRIPT[0]->regs[SAI_AREA.select.selected + 268] == 0) {
            if (card == 16 || card == 17) {
                SAI_AREA.mode = AREA_MODE_BUSY;
                SAI_AREA.selectState = 20;
                if (card == 16) {
                    SAI_AREA.select.page--;
                } else {
                    SAI_AREA.select.page++;
                }
                i = 0;
                timers = SAI_SELECT_BLINK;
            loop:
                timers[i] = rand() % 30 + 30;
                i++;
                if (i < 6) {
                    goto loop;
                }
                playSoundEffect(0);
            } else if (card >= 0 && card < 16) {
                SAI_AREA.select.zoom = 0;
                SAI_AREA.selectState = 6;
                SAI_AREA.mode = AREA_MODE_BUSY;
                SAI_SCRIPT[0]->regs[2] = card;
                SAI_AREA.opponentPicked = 1;
                playSoundEffect(0);
            }
        }
    }
    SAI_SPRITES[43]->pos.vx = SAI_SPRITES[SAI_AREA.select.selected + 31]->pos.vx;
    SAI_SPRITES[43]->pos.vy = SAI_SPRITES[SAI_AREA.select.selected + 31]->pos.vy;
    SAI_drawSprite(SAI_SPRITES[43]);
}

void SAI_randomizeOpponentDelays(void) {
    s32 i = 0;
    s16 *values = SAI_SELECT_DELAYS;

loop:
    values[i] = rand() % 30;
    i++;
    if (i < 6) {
        goto loop;
    }
}

void SAI_initOpponentSelect(void) {
    Rect16 rect;
    AreaState *ctx;
    s32 i;

    for (i = 0; i < 6; i++) {
        SAI_SPRITES[i + 31] = SAI_createSprite(8);
        SAI_setSpriteDepth(SAI_SPRITES[i + 31], 0x21);
        SAI_setSpriteSize(SAI_SPRITES[i + 31], 0x18, 0x16);
        SAI_SPRITES[i + 31]->pos.vx = 0x86;
        SAI_SPRITES[i + 31]->pos.vy = -0x57;
        SAI_setSpriteBlendMode(SAI_SPRITES[i + 31], 1);
    }
    for (i = 0, ctx = &SAI_AREA; i < 6; i++) {
        ctx->select.delay[i] = rand() % 30;
        ctx->select.progress[i] = 0;
        ctx->select.emptyBlink[i] = 0;
        SAI_SELECT_BLINK[i] = rand() % 30 + 50;
    }
    for (i = 0; i < 6; i++) {
        SAI_SPRITES[i + 37] = SAI_createSprite(0x3A);
        SAI_setSpriteDepth(SAI_SPRITES[i + 37], 0x21);
        SAI_setSpriteSize(SAI_SPRITES[i + 37], 0x12, 0x10);
        SAI_SPRITES[i + 37]->pos.vx = SAI_SPRITES[i + 31]->pos.vx;
        SAI_SPRITES[i + 37]->pos.vy = SAI_SPRITES[i + 31]->pos.vy - 1;
    }
    SAI_SPRITES[43] = SAI_createSprite(0x3C);
    SAI_setSpriteDepth(SAI_SPRITES[43], 0x20);
    SAI_setSpriteBlendMode(SAI_SPRITES[43], 0);
    SAI_SPRITES[44] = SAI_createSprite(8);
    SAI_setSpriteDepth(SAI_SPRITES[44], 0x21);
    SAI_setSpriteBlendMode(SAI_SPRITES[44], 1);
    SAI_SPRITES[45] = SAI_createSprite(0x3B);
    SAI_setSpriteDepth(SAI_SPRITES[45], 0x21);
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx = -0xC8;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy = 0x15;
    if (SESSION->unk1A7 == 0) {
        rect.x = 0x200;
        rect.y = 0x100;
        rect.w = 0xFF;
        rect.h = 0x80;
    } else {
        rect.x = 0x180;
        rect.y = 0x180;
        rect.w = 0xFF;
        rect.h = 0x7F;
    }
    if (SESSION->resumeMode == 1) {
        SAI_SCRIPT[0]->script->pc = SAI_SCRIPT[0]->script->start + SESSION->scriptOffset;
        SAI_OPPONENTS = SESSION->opponents;
        SAI_AREA.select = SESSION->select;
        SAI_SCRIPT[0]->regs[1] = (s8)(SESSION->duelResult ^ 1);
        SAI_AREA.location = SESSION->location;
        SESSION->resumeMode = 0;
        SAI_AREA.selectState = 9;
        SAI_AREA.select.timer = 20;
        SAI_AREA.opponentPicked = 1;
    } else {
        SAI_AREA.select.page = 0;
        SAI_AREA.select.selected = 0;
        SAI_AREA.select.unk5E = 0;
        SAI_AREA.select.timer = 0;
        SAI_AREA.select.zoom = 0;
        SAI_AREA.opponentPicked = 0;
        SAI_AREA.selectState = 4;
    }
    if ((u8)SAI_AREA.select.page >= 3) {
        SAI_AREA.select.page = 0;
    }
}

void SAI_freeOpponentSelect(void) {
    s32 i;

    for (i = 0x1F; i < 0x2E; i++) {
        SAI_freeSprite(SAI_SPRITES[i]);
    }
    ((SaveBlock *)((SessionData *)SESSION_DATA)->unk100C)->opponents = SAI_OPPONENTS;
    ((SaveBlock *)((SessionData *)SESSION_DATA)->unk100C)->select = SAI_AREA.select;
    SAI_clearOpponents();
}

void SAI_updateOpponentPortraits(void) {
    Rect16 rect;
    s32 i;
    s8 *cards;
    s8 page;
    s8 card;
    s16 w;
    s16 h;

    for (i = 0; i < 6; i++) {
        page = SAI_AREA.select.page;
        cards = SAI_OPPONENT_IDS;
        card = cards[page * 6 + i];
        if (card != -1) {
            if (SAI_SELECT_BLINK[i] > 0) {
                if (SAI_SELECT_BLINK[i] & 4) {
                    rect.x = 0x318;
                } else {
                    rect.x = 0x323;
                }
                rect.y = 0xA4;
                rect.w = 0x2C;
                rect.h = 0x24;
                SAI_setSpriteImage4Bit(SAI_SPRITES[i + 37], &rect);
                SAI_SPRITES[i + 37]->quads[0].clut = getClut(0x200, 0xEB);
                SAI_SPRITES[i + 37]->quads[1].clut = getClut(0x200, 0xEB);
            } else if (card == 16) {
                rect.x = 0x2EB;
                rect.y = 0x90;
                rect.w = 0x2A;
                rect.h = 0x24;
                SAI_setSpriteImage4Bit(SAI_SPRITES[i + 37], &rect);
                SAI_SPRITES[i + 37]->quads[0].clut = getClut(0x200, 0xEE);
                SAI_SPRITES[i + 37]->quads[1].clut = getClut(0x200, 0xEE);
            } else if (card == 17) {
                rect.x = 0x2E0;
                rect.y = 0x90;
                rect.w = 0x2A;
                rect.h = 0x24;
                SAI_setSpriteImage4Bit(SAI_SPRITES[i + 37], &rect);
                SAI_SPRITES[i + 37]->quads[0].clut = getClut(0x200, 0xEE);
                SAI_SPRITES[i + 37]->quads[1].clut = getClut(0x200, 0xEE);
            } else {
                rect.x = card % 4 * 32 + 0x280;
                rect.y = card / 4 * 56 + 0x100;
                rect.w = 0x3F;
                rect.h = 0x38;
                SAI_setSpriteImage8Bit(SAI_SPRITES[i + 37], &rect, 0);
                SAI_SELECT_BLINK[i] = 0;
                SAI_SPRITES[i + 37]->quads[0].clut = getClut(0x280, 0x1FB);
                SAI_SPRITES[i + 37]->quads[1].clut = getClut(0x280, 0x1FB);
            }
        }
        w = (SAI_AREA.select.progress[i] * 44 + (20 - SAI_AREA.select.progress[i]) * 18) / 20;
        h = (SAI_AREA.select.progress[i] * 38 + (20 - SAI_AREA.select.progress[i]) * 16) / 20;
        SAI_setSpriteSize(SAI_SPRITES[i + 37], w, h);
        SAI_SPRITES[i + 37]->pos.vx = SAI_SPRITES[i + 31]->pos.vx;
        SAI_SPRITES[i + 37]->pos.vy = SAI_SPRITES[i + 31]->pos.vy - 1;
    }
}

void SAI_zoomOpponentPortrait(void) {
    Rect16 rect;
    s32 i;
    s16 w;
    s16 h;

    if (SAI_AREA.selectState == 6) {
        if (SAI_AREA.select.zoom == 20) {
            SAI_AREA.mode = AREA_MODE_SCRIPT;
            SAI_AREA.selectState = 2;
        }
        SAI_AREA.select.zoom++;
        if (SAI_AREA.select.zoom > 20) {
            SAI_AREA.select.zoom = 20;
        }
    } else {
        if (SAI_AREA.select.zoom == 0) {
            SAI_AREA.mode = AREA_MODE_SELECT_OPPONENT;
            SAI_AREA.selectState = 2;
        }
        if (--SAI_AREA.select.zoom < 0) {
            SAI_AREA.select.zoom = 0;
        }
    }
    for (i = 0; i < 6; i++) {
        if (i == SAI_AREA.select.selected) {
            SAI_SPRITES[i + 31]->pos.vx = (SAI_AREA.select.zoom * 108 + ((i % 3) * 55 - 46) * (20 - SAI_AREA.select.zoom)) / 20;
            SAI_SPRITES[i + 31]->pos.vy = (SAI_AREA.select.zoom * 21 + ((i / 3) * 50 - 43) * (20 - SAI_AREA.select.zoom)) / 20;
            SAI_setSpriteSize(SAI_SPRITES[i + 31], (SAI_AREA.select.zoom * 80 + (20 - SAI_AREA.select.zoom) * 56) / 20, (SAI_AREA.select.zoom * 70 + (20 - SAI_AREA.select.zoom) * 48) / 20);
            w = (SAI_AREA.select.zoom * 64 + (20 - SAI_AREA.select.zoom) * 44) / 20;
            h = (SAI_AREA.select.zoom * 56 + (20 - SAI_AREA.select.zoom) * 38) / 20;
            SAI_setSpriteSize(SAI_SPRITES[i + 37], w, h);
            SAI_SPRITES[i + 37]->pos.vx = SAI_SPRITES[i + 31]->pos.vx;
            SAI_SPRITES[i + 37]->pos.vy = SAI_SPRITES[i + 31]->pos.vy - 1;
        } else {
            SAI_SPRITES[i + 31]->pos.vx = (SAI_AREA.select.zoom * 134 + ((i % 3) * 55 - 46) * (20 - SAI_AREA.select.zoom)) / 20;
            SAI_SPRITES[i + 31]->pos.vy = (SAI_AREA.select.zoom * -87 + ((i / 3) * 50 - 43) * (20 - SAI_AREA.select.zoom)) / 20;
            SAI_setSpriteSize(SAI_SPRITES[i + 31], (SAI_AREA.select.zoom * 24 + (20 - SAI_AREA.select.zoom) * 56) / 20, (SAI_AREA.select.zoom * 22 + (20 - SAI_AREA.select.zoom) * 48) / 20);
            w = (SAI_AREA.select.zoom * 18 + (20 - SAI_AREA.select.zoom) * 44) / 20;
            h = (SAI_AREA.select.zoom * 16 + (20 - SAI_AREA.select.zoom) * 38) / 20;
            SAI_setSpriteSize(SAI_SPRITES[i + 37], w, h);
            SAI_SPRITES[i + 37]->pos.vx = SAI_SPRITES[i + 31]->pos.vx;
            SAI_SPRITES[i + 37]->pos.vy = SAI_SPRITES[i + 31]->pos.vy - 1;
        }
    }
    SAI_SPRITES[44]->pos.vx = (SAI_AREA.select.zoom * -103 - (20 - SAI_AREA.select.zoom) * 200) / 20;
    SAI_SPRITES[44]->pos.vy = 21;
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy;
}

s8 SAI_slideOpponentPortraits(void) {
    s32 i;
    s8 busy;

    for (i = 0; i < 6; i++) {
        if (SAI_AREA.selectState == 4 || SAI_AREA.selectState == 20) {
            if (SAI_SELECT_DELAYS[i] <= 0) {
                SAI_SELECT_PROGRESS[i]++;
            } else {
                SAI_SELECT_DELAYS[i]--;
                if (SAI_SELECT_DELAYS[i] < 0) {
                    SAI_SELECT_DELAYS[i] = 0;
                }
            }
            if (SAI_SELECT_PROGRESS[i] > 20) {
                SAI_SELECT_PROGRESS[i] = 20;
            }
        } else if (SAI_AREA.selectState == 5) {
            if (SAI_SELECT_DELAYS[i] <= 0) {
                SAI_AREA.select.blink[i] = (SAI_AREA.select.blink[i] + 1) & 0xFFF;
                if (SAI_AREA.select.blink[i] > 20 || SAI_SELECT_PROGRESS[i] != 20) {
                    SAI_SELECT_PROGRESS[i]--;
                }
            } else {
                SAI_SELECT_DELAYS[i]--;
                if (SAI_SELECT_DELAYS[i] < 0) {
                    SAI_SELECT_DELAYS[i] = 0;
                }
            }
            if (SAI_SELECT_PROGRESS[i] < 0) {
                SAI_SELECT_PROGRESS[i] = 0;
            }
        }
        SAI_SPRITES[i + 31]->pos.vx = ((i % 3 * 55 - 46) * SAI_SELECT_PROGRESS[i] + (20 - SAI_SELECT_PROGRESS[i]) * 134) / 20;
        SAI_SPRITES[i + 31]->pos.vy = ((i / 3 * 50 - 43) * SAI_SELECT_PROGRESS[i] + (20 - SAI_SELECT_PROGRESS[i]) * -87) / 20;
        SAI_setSpriteSize(SAI_SPRITES[i + 31], (SAI_SELECT_PROGRESS[i] * 56 + (20 - SAI_SELECT_PROGRESS[i]) * 24) / 20,
                      (SAI_SELECT_PROGRESS[i] * 48 + (20 - SAI_SELECT_PROGRESS[i]) * 22) / 20);
    }
    busy = 0;
    if (SAI_SELECT_STATE == 4 || SAI_SELECT_STATE == 20) {
        for (i = 0; i < 6; i++) {
            if (SAI_SELECT_BLINK[i] > 0) {
                SAI_SELECT_BLINK[i]--;
            }
            if (SAI_SELECT_BLINK[i] < 0) {
                SAI_SELECT_BLINK[i] = 0;
            }
            if (SAI_SELECT_BLINK[i] != 0) {
                busy++;
            }
        }
        if (busy == 0) {
            if (SAI_AREA.selectState == 20) {
                SAI_AREA.selectState = 2;
                SAI_AREA.mode = AREA_MODE_SELECT_OPPONENT;
            } else {
                SAI_AREA.selectState = 2;
                SAI_AREA.mode = AREA_MODE_SCRIPT;
            }
        }
    } else if (SAI_SELECT_STATE == 5) {
        for (i = 0; i < 6; i++) {
            if (SAI_SELECT_PROGRESS[i] != 0) {
                busy++;
            }
        }
        if (busy == 0) {
            SAI_AREA.mode = AREA_MODE_SCRIPT;
            SAI_AREA.selectState = 2;
        }
    }
    SAI_updateOpponentPortraits();
    return busy;
}

s32 SAI_slideChosenOpponentBack(void) {
    s32 result = 1;
    s32 i;

    if (SAI_AREA.selectState != 9) {
        SAI_AREA.select.timer++;
        if (SAI_AREA.select.timer > 20) {
            SAI_AREA.select.timer = 20;
            result = 0;
            SAI_AREA.mode = AREA_MODE_SCRIPT;
            SAI_AREA.selectState = 2;
        }
    } else {
        SAI_AREA.select.timer--;
        SAI_updateOpponentPortraits();
        for (i = 0; i < 6; i++) {
            if (SAI_AREA.select.selected == i) {
                SAI_setSpriteSize(SAI_SPRITES[i + 31], 0x50, 0x46);
                SAI_setSpriteSize(SAI_SPRITES[i + 37], 0x40, 0x38);
            } else {
                SAI_setSpriteSize(SAI_SPRITES[i + 31], 0x18, 0x16);
                SAI_setSpriteSize(SAI_SPRITES[i + 37], 0x12, 0x10);
            }
        }
        if (SAI_AREA.select.timer < 0) {
            SAI_AREA.select.timer = 0;
            result = 0;
            SAI_AREA.mode = AREA_MODE_SCRIPT;
            SAI_AREA.selectState = 2;
        }
    }
    SAI_SPRITES[44]->pos.vx = (SAI_AREA.select.timer * -200 + (20 - SAI_AREA.select.timer) * -103) / 20;
    SAI_SPRITES[44]->pos.vy = 0x15;
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy;
    for (i = 0; i < 6; i++) {
        if (i == SAI_AREA.select.selected) {
            SAI_SPRITES[i + 31]->pos.vx = (SAI_AREA.select.timer * 210 + (20 - SAI_AREA.select.timer) * 108) / 20;
            SAI_SPRITES[i + 37]->pos.vx = SAI_SPRITES[i + 31]->pos.vx;
            SAI_SPRITES[i + 31]->pos.vy = SAI_SPRITES[i + 37]->pos.vy = 0x15;
            SAI_SPRITES[i + 37]->pos.vy--;
        } else {
            SAI_SPRITES[i + 31]->pos.vx = SAI_SPRITES[i + 37]->pos.vx = 0x86;
            SAI_SPRITES[i + 31]->pos.vy = SAI_SPRITES[i + 37]->pos.vy = -0x57;
            SAI_SPRITES[i + 37]->pos.vy--;
        }
    }
    return result;
}

void func_801E7240(void) {
    Sprite3D *card;
    s16 w;
    s16 h;

    SAI_AREA.select.timer++;
    if (SAI_AREA.select.timer > 30) {
        SAI_AREA.select.timer = 30;
        SAI_AREA.unk116 = 3;
        SAI_AREA.selectState = 3;
        SAI_AREA.mode = AREA_MODE_SCRIPT;
    }
    card = SAI_SPRITES[SAI_AREA.select.selected + 31];
    w = (SAI_AREA.select.timer * 24 + (30 - SAI_AREA.select.timer) * 80) / 30;
    h = (SAI_AREA.select.timer * 22 + (30 - SAI_AREA.select.timer) * 70) / 30;
    SAI_setSpriteSize(SAI_SPRITES[44], w, h);
    SAI_setSpriteSize(card, w, h);
    w = (SAI_AREA.select.timer * 18 + (30 - SAI_AREA.select.timer) * 64) / 30;
    h = (SAI_AREA.select.timer * 16 + (30 - SAI_AREA.select.timer) * 56) / 30;
    SAI_setSpriteSize(SAI_SPRITES[45], w, h);
    SAI_setSpriteSize(card + 6, w, h);
    SAI_SPRITES[44]->pos.vx = (SAI_AREA.select.timer * 134 + (30 - SAI_AREA.select.timer) * -103) / 30;
    SAI_SPRITES[44]->pos.vy = (SAI_AREA.select.timer * -87 + (30 - SAI_AREA.select.timer) * 21) / 30;
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy;
    card->pos.vx = card[6].pos.vx = (SAI_AREA.select.timer * 134 + (30 - SAI_AREA.select.timer) * 108) / 30;
    card->pos.vy = card[6].pos.vy = (SAI_AREA.select.timer * -87 + (30 - SAI_AREA.select.timer) * 21) / 30;
}

void SAI_drawOpponentSelect(void) {
    Rect16 rect;
    s32 i;

    for (i = 0; i < 2; i++) {
        SAI_stepBrightness(i);
    }
    if (SAI_AREA_MODE == AREA_MODE_SELECT_OPPONENT) {
        SAI_OPPONENTS.target[0] = SAI_OPPONENTS.target[1] = 0x80;
    }
    SAI_setSpriteBrightness(SAI_SPRITES[45], SAI_OPPONENTS.current[0]);
    SAI_setSpriteBrightness(SAI_SPRITES[SAI_SELECT_SELECTED + 37], SAI_OPPONENTS.current[1]);
    SAI_drawSprite(SAI_SPRITES[44]);
    SAI_drawSprite(SAI_SPRITES[45]);
    for (i = 0; i < 6; i++) {
        if ((SAI_OPPONENT_IDS + SAI_AREA.select.page * 6)[i] == -1) {
            SAI_SELECT_EMPTY_BLINK[i]++;
            if (SAI_SELECT_EMPTY_BLINK[i] & 4) {
                rect.x = 0x318;
            } else {
                rect.x = 0x323;
            }
            rect.y = 0xA4;
            rect.w = 0x2C;
            rect.h = 0x24;
            SAI_setSpriteImage4Bit(SAI_SPRITES[i + 37], &rect);
            SAI_SPRITES[i + 37]->quads[0].clut = getClut(0x200, 0xEB);
            SAI_SPRITES[i + 37]->quads[1].clut = getClut(0x200, 0xEB);
        } else {
            if (SAI_SCRIPT[0]->regs[i + 268] != 0) {
                SAI_setSpriteBrightness(SAI_SPRITES[i + 37], 0x30);
            }
            SAI_SELECT_EMPTY_BLINK[i] = 0;
        }
        SAI_drawSprite(SAI_SPRITES[i + 31]);
        SAI_drawSprite(SAI_SPRITES[i + 37]);
    }
}

void SAI_runOpponentSelectPanel(void) {
    SAI_AREA.openPanel = 2;
    SAI_AREA.closePanel = 0;
    SAI_AREA.imageHidden = -1;
    SAI_AREA.opening = 1;
    SAI_createPanel();
    SAI_SPRITES[0] = SAI_createSprite(5);
    SAI_setSpriteDepth(SAI_SPRITES[0], 0x24);
    SAI_initOpponentSelect();
    SAI_spinPanel();
    playSoundEffect(10);
    do {
        waitFrames(1);
    } while (SAI_spinPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = 0;
    SAI_toggleMessageWindow(1);
    addFrameCallback((s32)SAI_drawOpponentSelect);
    do {
        waitFrames(1);
    } while (SAI_uncoverPanel() == 0);
    do {
        waitFrames(1);
        switch (SAI_SELECT_STATE) {
        case 2:
            break;
        case 4:
        case 5:
        case 20:
            SAI_slideOpponentPortraits();
            break;
        case 6:
        case 7:
            SAI_zoomOpponentPortrait();
            break;
        case 9:
            SAI_slideChosenOpponentBack();
            break;
        }
    } while (SAI_CLOSE_PANEL == 0);
    SAI_SELECT_STATE = 5;
    if (SESSION->resumeMode == 1) {
        do {
            waitFrames(1);
        } while (SAI_slideChosenOpponentBack() != 0);
    } else {
        SAI_randomizeOpponentDelays();
        do {
            waitFrames(1);
        } while (SAI_slideOpponentPortraits() != 0);
    }
    removeFrameCallback((s32)SAI_drawOpponentSelect);
    do {
        waitFrames(1);
    } while (SAI_coverPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = -1;
    playSoundEffect(11);
    do {
        waitFrames(1);
    } while (SAI_spinPanel() == 0);
    SAI_freeOpponentSelect();
    SAI_freePanel();
    SAI_OPEN_PANEL = 0;
}
