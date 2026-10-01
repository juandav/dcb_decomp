#include "common.h"
#include "game.h"
#include "dcb/task.h"
#include "dcb/saiseg.h"

/* jp's event loop, which runs the area script's events, and the map window
   it opens and closes (sai_panel.c is us's and eu's) */

/* no prototype: this module passes its coordinates as ints */
void initVramSprite();

void func_801EEAD0(void);

void func_801EE8F0(void) {
    SAI_STATE->unk4B = -1;
    SAI_STATE->unk3C = 20;
    do {
        waitFrames(1);
    } while (SAI_STATE->unk4B == -1);
    SAI_STATE->unk4B = 0;
}

void SAI_showRightSprite(s32 kind) {
    SpriteDef *def;
    s8 i;

    def = &D_801F6000[kind];
    SAI_UI.unk3B4->state = 2;
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SAI_UI.unk3B4->unk2 != 1);
    for (i = 0; i < 2; i++) {
        initVramSprite(DB(i).primSlots[0] + 0x80, 0xE6, 0x4F, def->clut, def->colorMode, def->vramX, def->vramY, def->width, def->height, -1);
    }
    SAI_UI.unk3B4->state = 1;
    exitTask();
}

/* case 13 hands func_801F16E0 the state it keeps from storing the script's
   offset, and case 3 of event 11 narrows the menu value, where ours don't */
INCLUDE_RODATA("saiseg/nonmatchings/ui/sai_panel_jp", D_801EA6A0);
INCLUDE_ASM("saiseg/nonmatchings/ui/sai_panel_jp", func_801EEAD0);

s32 func_801F02B4(void) {
    *SAI_STATE->runner->regs = 1;
    func_801EEAD0();
    return *SAI_STATE->runner->regs;
}
