#include "common.h"
#include "game.h"
#include "dcb/sai_choice.h"
#include "dcb/sound_play.h"
#include "dcb/pad.h"
#include "dcb/saiseg.h"
#include "dcb/sai_sprite.h"
#include "dcb/sai_world_map.h"

extern s8 SAI_CHOICE_MODE;
extern s8 SAI_CHOICE_PHASE;

void SAI_openChoiceMenu(s8 mode) {
    s32 i;

    SAI_AREA.choiceMode = mode;
    SAI_AREA.choiceSlide = 0;
    SAI_AREA.choiceCursor = 0;
    SAI_AREA.choiceCount = 0;
    SAI_AREA.choicePhase = 0;
    for (i = 0; i < 5; i++) {
        SAI_AREA.choiceStates[i] = 0;
    }
    if (mode == 1) {
        SAI_SPRITES[25] = SAI_createSprite(0x15);
        SAI_SPRITES[25]->pos.vx = -0x66;
        SAI_SPRITES[25]->pos.vy = -0x13;
        for (i = 0; i < 5; i++) {
            SAI_SPRITES[i + 26] = SAI_createSprite(0x14);
            SAI_SPRITES[i + 26]->pos.vx = -0x68;
            SAI_SPRITES[i + 26]->pos.vy = i * 16 - 0x37;
            SAI_SPRITES[i + 26]->corners[2].vy++;
            SAI_SPRITES[i + 26]->corners[3].vy++;
            SAI_setSpriteDepth(SAI_SPRITES[i + 26], 0x21);
            SAI_setSpriteBlendMode(SAI_SPRITES[i + 26], 0);
        }
    } else {
        SAI_SPRITES[25] = SAI_createSprite(0x16);
        SAI_SPRITES[25]->pos.vx = -0x66;
        SAI_SPRITES[25]->pos.vy = -0x2A;
        for (i = 0; i < 3; i++) {
            SAI_SPRITES[i + 26] = SAI_createSprite(0x3D);
            SAI_SPRITES[i + 26]->pos.vx = -0x68;
            SAI_SPRITES[i + 26]->pos.vy = i * 16 - 0x3F;
            SAI_SPRITES[i + 26]->corners[2].vy++;
            SAI_SPRITES[i + 26]->corners[3].vy++;
            SAI_setSpriteDepth(SAI_SPRITES[i + 26], 0x21);
            SAI_setSpriteBlendMode(SAI_SPRITES[i + 26], 0);
        }
    }
    SAI_setSpriteDepth(SAI_SPRITES[25], 0x20);
    SAI_setSpriteBlendMode(SAI_SPRITES[25], 1);
}

void SAI_addChoice(s8 id) {
    Rect16 rect;

    if (((SessionData *)SESSION_DATA)->playWithoutSaving == 1 && id == 15) {
        SAI_AREA.choiceStates[SAI_AREA.choiceCount] = 2;
    }
    if (SAI_CHOICE_MODE != 0) {
        rect.x = 0x300;
        rect.y = id << 4;
        rect.w = 0x58;
    } else {
        rect.x = 0x318;
        rect.y = (id - 12) << 4;
        rect.w = 0x40;
    }
    rect.h = 0x10;
    SAI_setSpriteImage4Bit(SAI_SPRITES[SAI_AREA.choiceCount + 26], &rect);
    SAI_AREA.choiceCount++;
}

void SAI_slideInChoiceMenu(void) {
    SAI_AREA.choiceSlide++;
    if (SAI_AREA.choiceSlide > 20) {
        SAI_AREA.choiceSlide = 20;
        SAI_AREA.choicePhase = 1;
    }
}

void SAI_tickChoiceInput(void) {
    if (PAD_STATES[0]->repeat & PAD_DOWN) {
        SAI_AREA.choiceCursor++;
        playSoundEffect(2);
    } else if (PAD_STATES[0]->repeat & PAD_UP) {
        SAI_AREA.choiceCursor--;
        playSoundEffect(2);
    } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
        playSoundEffect(1);
        SAI_CHOICE_PHASE = 2;
        SAI_SCRIPT[0]->regs[1] = -1;
    } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
        if (SAI_AREA.choiceStates[SAI_AREA.choiceCursor] != 2) {
            playSoundEffect(0);
            SAI_AREA.choicePhase = 2;
            SAI_SCRIPT[0]->regs[1] = SAI_AREA.choiceCursor + 1;
        }
    }
    if (SAI_AREA.choiceCursor < 0) {
        SAI_AREA.choiceCursor = SAI_AREA.choiceCount - 1;
    }
    if (SAI_AREA.choiceCursor >= SAI_AREA.choiceCount) {
        SAI_AREA.choiceCursor = 0;
    }
}

void SAI_slideOutChoiceMenu(void) {
    if (--SAI_AREA.choiceSlide < 0) {
        SAI_AREA.choiceSlide = 0;
        SAI_AREA.choicePhase = 3;
        SAI_AREA.choiceCount = 0;
    }
}

void SAI_freeChoiceMenu(void) {
    s32 i;

    if (SAI_CHOICE_MODE == 0) {
        for (i = 0; i < 4; i++) {
            SAI_freeSprite(SAI_SPRITES[i + 25]);
        }
    } else {
        for (i = 0; i < 6; i++) {
            SAI_freeSprite(SAI_SPRITES[i + 25]);
        }
    }
    SAI_AREA_MODE = AREA_MODE_SCRIPT;
}

void (*SAI_CHOICE_MENU_FUNCS[4])(void) = {
    SAI_slideInChoiceMenu,
    SAI_tickChoiceInput,
    SAI_slideOutChoiceMenu,
    SAI_freeChoiceMenu,
};

void SAI_tickChoiceMenu(void) {
    if (SAI_CHOICE_MENU_FUNCS[SAI_CHOICE_PHASE] != NULL) {
        SAI_CHOICE_MENU_FUNCS[SAI_CHOICE_PHASE]();
    }
}

void SAI_drawChoiceMenu(void) {
    s32 count;
    s32 x;
    s32 i;

    count = 5;
    if (SAI_CHOICE_MODE == 0) {
        count = 3;
    }
    if (SAI_AREA.choiceCount != 0) {
        x = (SAI_AREA.choiceSlide * -102 + (20 - SAI_AREA.choiceSlide) * -212) / 20;
        for (i = 0; i < count; i++) {
            if (SAI_AREA.choiceMode == 0) {
                SAI_SPRITES[i + 26]->pos.vx = x + 3;
            } else {
                SAI_SPRITES[i + 26]->pos.vx = x;
            }
            if (SAI_AREA.choicePhase == 1 && SAI_AREA.choiceCursor == i) {
                if (SAI_AREA.choiceStates[i] != 0) {
                    SAI_SPRITES[i + 26]->quads[0].clut = getClut(0x200, 0xF1);
                    SAI_SPRITES[i + 26]->quads[1].clut = getClut(0x200, 0xF1);
                } else {
                    SAI_SPRITES[i + 26]->quads[0].clut = getClut(0x200, 0xF0);
                    SAI_SPRITES[i + 26]->quads[1].clut = getClut(0x200, 0xF0);
                }
            } else if (SAI_AREA.choiceStates[i] != 0) {
                SAI_SPRITES[i + 26]->quads[0].clut = getClut(0x200, 0xF2);
                SAI_SPRITES[i + 26]->quads[1].clut = getClut(0x200, 0xF2);
            } else {
                SAI_SPRITES[i + 26]->quads[0].clut = getClut(0x200, 0xEF);
                SAI_SPRITES[i + 26]->quads[1].clut = getClut(0x200, 0xEF);
            }
            SAI_drawSprite(SAI_SPRITES[i + 26]);
        }
        SAI_SPRITES[25]->pos.vx = x;
        SAI_drawSprite(SAI_SPRITES[25]);
    }
}
