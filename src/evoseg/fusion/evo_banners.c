#include "common.h"
#include "game.h"
#include "dcb/evo_banners.h"
#include "dcb/text.h"
#include "dcb/prim.h"
#include "dcb/evoseg.h"

void EVO_drawUnitPortrait(UiWindow *w) {
    Rect16 uv;
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    uv.x = 0x28;
    uv.y = EVO_FUSION.unit * 56;
    uv.w = 0x40;
    uv.h = 0x38;
    drawTexturedSprite(x, y, &uv, 0x98, 0x7C18, z, w->brightness, -1);
}

void EVO_drawRankUpBanner(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    if (EVO_RANK_UP_STATE == 2) {
        drawLargeText(x + 1, y + 1, (s32)"RANK MAX!", 7, z);
    } else if (EVO_RANK_UP_STATE == 1) {
        drawLargeText(x + 1, y + 1, (s32)"RANK UP!", 7, z);
    }
}

void EVO_drawReceivedBanner(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    if (EVO_CARD_RECEIVED == 0) {
        drawLargeText(x + 1, y + 1, (s32)"FULL SET!", 7, z);
    } else {
        drawLargeText(x + 1, y + 1, (s32)"RECEIVED!", 7, z);
    }
}
