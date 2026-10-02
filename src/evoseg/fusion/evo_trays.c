#include "common.h"
#include "game.h"
#include "dcb/evo_trays.h"
#include "dcb/window.h"
#include "dcb/text.h"
#include "dcb/dialog.h"
#include "dcb/prim.h"
#include "dcb/evo_card_list.h"
#include "dcb/evo_data.h"

void EVO_drawFusionTypeTitle(EvoWindow *w) {
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;
    u8 unused[0x48]; /* unused, but it is in the original stack frame */

    if (w->isPartner == 0) {
        drawTextColored(x + 0x4B, y, "Card Fusion", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 6, z);
    } else {
        drawTextColored(x + 0x41, y, "Partner Fusion", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 6, z);
    }
}

void EVO_drawFusionTypeHelp(EvoWindow *w) {
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;
    u8 unused[0x48]; /* unused, but it is in the original stack frame */

    if (w->isPartner == 0) {
        x += 2;
        drawTextColored(x, y, "*w1Create a New Card", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 8, z);
        drawTextColored(x, y + 0xC, "*w1by Fusing 2 Cards.", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 8, z);
        drawTextColored(x, y + 0x18, "*w1Partner Cards can't be used.", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 8, z);
    } else {
        x += 2;
        drawTextColored(x, y, "*w1Increase Experience Points by", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 8, z);
        drawTextColored(x, y + 0xC, "*w1Fusing a Card to a Partner Card.", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 8, z);
        drawTextColored(x, y + 0x18, "*w1Also,2 Partner Cards can't be Fused.", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 8, z);
    }
}

void EVO_drawTray(EvoTray *tray) {
    char text[40];
    Rect16 uv;
    POLY_FT4 *poly;
    u8 player;

    poly = tray->polys[FRAME_BUFFER_INDEX];
    player = 2;
    if (tray == &EVO_TRAYS[0]) {
        player = 1;
    }
#if VERSION_US
    /* the match depends on adding the index before the field offset, as the
       original does */
    if (((EvoFusion *)((u8 *)&EVO_FUSION + player))->busy[0] == 0) {
#elif VERSION_EU
    if (EVO_FUSION.busy[player] == 0) {
#else
#error "evoseg/fusion/evo_trays: version not checked"
#endif
        bzero((Scene3D *)text, 0x21);
        sprintf(text, "TRAY%d", player);
        drawLargeText(tray->x + 16, 0x5C, (s32)text, 7, 0x1D);
        uv.x = 0;
        uv.y = 0x74;
        uv.w = 0x50;
        uv.h = 0x74;
        drawTexturedSprite(tray->x, tray->y, &uv, 0x18, 0x7BDF, 0x1E, 0x80, 0);
        return;
    }
    setlen(poly, 9);
    setcode(poly, 0x2C);
    poly->clut = 0x7BD8;
    poly->tpage = 0x18;
    poly->r0 = 0x80;
    poly->g0 = 0x80;
    poly->b0 = 0x80;
    poly->x0 = tray->x;
    poly->y0 = tray->y;
    poly->x1 = tray->x + 0x50;
    poly->y1 = tray->y;
    poly->x2 = tray->x;
    poly->y2 = tray->y + 0x74;
    poly->x3 = tray->x + 0x50;
    poly->y3 = tray->y + 0x74;
    poly->u0 = 0;
    poly->v0 = 0;
    poly->u1 = 0x50;
    poly->v1 = 0;
    poly->u2 = 0;
    poly->v2 = 0x74;
    poly->u3 = 0x50;
    poly->v3 = 0x74;
    addPrim(&CURRENT_FRAME_BUFFER->ot[29], poly);
    poly++;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    if (player == 1 && EVO_FUSION.hideResult == 0) {
        poly->clut = 0x7A98;
        poly->u0 = 0;
        poly->v0 = 0x40;
        poly->u1 = 0x3F;
        poly->v1 = 0x40;
        poly->u2 = 0;
        poly->v2 = 0x7F;
        poly->u3 = 0x3F;
        poly->v3 = 0x7F;
    } else {
        poly->clut = getClut(0x180, player + 0x1E7);
        poly->u0 = (player - 1) * 64;
        poly->v0 = 0;
        poly->u1 = (player - 1) * 64 + 0x3F;
        poly->v1 = 0;
        poly->u2 = (player - 1) * 64;
        poly->v2 = 0x3F;
        poly->u3 = (player - 1) * 64 + 0x3F;
        poly->v3 = 0x3F;
    }
    poly->tpage = 0x99;
    poly->r0 = 0x80;
    poly->g0 = 0x80;
    poly->b0 = 0x80;
    poly->x0 = tray->x + 8;
    poly->y0 = tray->y + 0x14;
    poly->x1 = tray->x + 0x48;
    poly->y1 = tray->y + 0x14;
    poly->x2 = tray->x + 8;
    poly->y2 = tray->y + 0x54;
    poly->x3 = tray->x + 0x48;
    poly->y3 = tray->y + 0x54;
    addPrim(&CURRENT_FRAME_BUFFER->ot[28], poly);
}

EvoWindowDef EVO_WINDOW_DEFS[14] = {
    { { 0x10, 0x24, 0x40, 0x40 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5C, 0x2A, 0xDE, 0xC }, 0x80, 0x56, 8, 0, 8 },
    { { 0x62, 0x3E, 0xD8, 0x24 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x10, 0x4C, 0x40, 0x40 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5C, 0x52, 0xDE, 0xC }, 0x80, 0x56, 8, 0, 8 },
    { { 0x62, 0x66, 0xD8, 0x24 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x10, 0xAC, 0x3E, 0x38 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5A, 0xB4, 0xE0, 0x30 }, 0x80, 0x56, 8, (s32)"MESSAGE", 8 },
    { { 0x10, 0x96, 0x3E, 0x38 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5A, 0x9C, 0xDE, 0x30 }, 0x80, 0x56, 8, 0, 8 },
    { { 0x76, 0x26, 0xC4, 0x7E }, 0x80, 0x66, 8, 0, 8 },
    { { 0xA, 0x2C, 0x52, 0x78 }, 0x80, 0x56, 8, 0, 8 },
    { { 0x78, 0x51, 0x88, 0x34 }, 0x80, 0x56, 8, 0, 8 },
    { { 0x42, 0x78, 0x48, 0x9 }, 0x80, 0x36, 0, 0, 8 },
};
const char D_801DF260[] = "";

void EVO_slideInFirstTray(void) {
    EVO_TRAYS[0].x += 10;
    if (EVO_TRAYS[0].x >= 15) {
        EVO_TRAYS[0].x = 14;
        EVO_FUSION.step = 3;
        EVO_CARD_LIST_MENU.active = 1;
        EVO_FUSION.scriptState = 0;
    }
}

void EVO_slideOutFirstTray(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x58) {
        EVO_TRAYS[0].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = -1;
    }
}

void EVO_swapToSecondTray(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x78) {
        EVO_TRAYS[0].x = -0x78;
        EVO_TRAYS[1].x += 10;
        if (EVO_TRAYS[1].x >= 15) {
            EVO_TRAYS[1].x = 14;
            EVO_FUSION.step = 3;
            EVO_CARD_LIST_MENU.active = 1;
        }
    }
}

void EVO_swapToFirstTray(void) {
    EVO_TRAYS[1].x -= 10;
    if (EVO_TRAYS[1].x < -0x78) {
        EVO_TRAYS[1].x = -0x78;
        EVO_TRAYS[0].x += 10;
        if (EVO_TRAYS[0].x >= 15) {
            EVO_TRAYS[0].x = 14;
            EVO_FUSION.step = 3;
            EVO_CARD_LIST_MENU.active = 1;
        }
    }
}

void EVO_cancelSecondCard(s32 active) {
    if (active != 0) {
        EVO_TRAYS[0].x -= 10;
        EVO_TRAYS[1].x -= 10;
        if (EVO_TRAYS[0].x < -0x58) {
            EVO_TRAYS[0].x = -0x58;
        }
        if (EVO_TRAYS[1].x < 14) {
            EVO_TRAYS[1].x = 14;
        }
        if (EVO_TRAYS[0].x == -0x58 && EVO_TRAYS[1].x == 14) {
            EVO_FUSION.pickSlot = 2;
            EVO_SCRIPT->vars[8] = -1;
            EVO_FUSION.scriptState = 0;
            EVO_FUSION.step = 0;
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.secondCard]++;
        }
    }
}

void EVO_toggleMessageWindows(s8 mode) {
    if (mode == 0) {
        animateWindowTo(&EVO_WINDOWS[6].win, &EVO_WINDOW_DEFS[6].rect);
        animateWindowTo(&EVO_WINDOWS[7].win, &EVO_WINDOW_DEFS[7].rect);
    } else if (mode == 1) {
        animateWindowTo(&EVO_WINDOWS[6].win, (Rect16 *)-1);
        animateWindowTo(&EVO_WINDOWS[7].win, (Rect16 *)-1);
    }
}

void EVO_runChoiceDialog(s32 mode) {
    initDialog((u8 *)&EVO_DIALOG, NULL, 1);
    if (mode != 1) {
        EVO_DIALOG.choice = 1;
    }
    runDialog(&EVO_DIALOG);
    switch (EVO_DIALOG.choice) {
    case 0:
        EVO_SCRIPT->vars[1] = 0;
        break;
    case 1:
        EVO_SCRIPT->vars[1] = 1;
        break;
    case 2:
        EVO_SCRIPT->vars[1] = 2;
        break;
    }
}
