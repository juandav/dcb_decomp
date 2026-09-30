#include "common.h"
#include "game.h"
#include "dcb/evo_screen_flash.h"
#include "dcb/frame_callback.h"
#include "dcb/prim_util.h"
#include "dcb/evoseg.h"

void EVO_drawScreenFlash(void) {
    if (EVO_SCREEN_FLASH.on == 0) {
        EVO_SCREEN_FLASH.brightness -= 8;
        if (EVO_SCREEN_FLASH.brightness < 0) {
            EVO_SCREEN_FLASH.brightness = 0;
        }
    } else {
        EVO_SCREEN_FLASH.brightness += 8;
        if (EVO_SCREEN_FLASH.brightness >= 0x100) {
            EVO_SCREEN_FLASH.brightness = 0xFF;
        }
    }
    if (EVO_SCREEN_FLASH.brightness != 0) {
        EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX].r0 = EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX].g0 =
            EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX].b0 = EVO_SCREEN_FLASH.brightness;
        addPrim(&CURRENT_FRAME_BUFFER->ot[25], &EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[25], &EVO_SCREEN_FLASH.tpage[FRAME_BUFFER_INDEX]);
    }
}

void EVO_initScreenFlash(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        SetPolyF4(&EVO_SCREEN_FLASH.poly[i]);
        SetSemiTrans(&EVO_SCREEN_FLASH.poly[i], 1);
        setPrimQuadRect(&EVO_SCREEN_FLASH.poly[i], 0, 0, 320, 240);
        SetDrawTPage(&EVO_SCREEN_FLASH.tpage[i], 0, 0, 0x20);
        EVO_SCREEN_FLASH.poly[i].b0 = 0;
        EVO_SCREEN_FLASH.poly[i].g0 = 0;
        EVO_SCREEN_FLASH.poly[i].r0 = 0;
    }
    EVO_SCREEN_FLASH.on = 0;
    EVO_SCREEN_FLASH.brightness = 0;
    addFrameCallback((s32)EVO_drawScreenFlash);
}
