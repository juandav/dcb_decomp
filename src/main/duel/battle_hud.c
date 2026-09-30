#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/battle_hud.h"
#include "dcb/card_db.h"
#include "dcb/duel_launch.h"
#include "dcb/card_render.h"
#include "dcb/duel.h"
#include "dcb/cpu_decision.h"
#include "dcb/duel_setup.h"
#include "dcb/card_zones.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
#include "dcb/camera.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
#include "dcb/scroll_bg.h"
#include "dcb/text.h"
#include "dcb/str_util.h"

void waitForStatCountersToSettle(void) {
    s32 unsettled;
    s32 player;
    s32 i;

    do {
        waitFrames(FRAME_INTERVAL);
        unsettled = 0;
        for (player = 0; player < 2; player++) {
            for (i = 0; i < 5; i++) {
                if (PLAYER(player)->stats[i] != PLAYER(player)->displayedStats[i]) {
                    unsettled = 1;
                }
            }
        }
    } while (unsettled);
}

void showDpGainPopup(s32 player) {
    s32 dpCard;
    Panel *statusPanel;

    dpCard = peekDpSlotTop(player);
    if (dpCard != -1) {
        statusPanel = PLAYER_PANEL(player, HUD_STATUS);
        PLAYER(player)->statPopups[4].value = ((DigimonCardData *)PLAYER(player)->cards[dpCard % 30].card)->dpBonus;
        PLAYER(player)->statPopups[4].type = 5;
        PLAYER(player)->statPopups[4].timer = 0x30;
        PLAYER(player)->statPopups[4].x = statusPanel->x + (s16)(player * 93 + 0x10);
        PLAYER(player)->statPopups[4].y = statusPanel->y + 2;
    } else {
        PLAYER(player)->statPopups[4].timer = 0;
    }
}

void showStatChangePopup(s32 player, s32 newValue, s32 stat) {
    s32 activeCard;
    Panel *statusPanel;

    PLAYER(player)->statPopups[stat].value = newValue - PLAYER(player)->stats[stat];
    if (PLAYER(player)->statPopups[stat].value == 0) {
        PLAYER(player)->statPopups[stat].type = 7;
    } else if (PLAYER(player)->statPopups[stat].value > 0) {
        PLAYER(player)->statPopups[stat].type = 5;
    } else {
        PLAYER(player)->statPopups[stat].type = 2;
    }
    PLAYER(player)->statPopups[stat].value = abs(PLAYER(player)->statPopups[stat].value);
    PLAYER(player)->statPopups[stat].timer = 0x30;
    if (stat == 0) {
        activeCard = getActiveDigimonCard(player);
        /* the HP popup sits on the Digimon's card, the others on the status panel */
        projectCardSprite(SPRITE(activeCard), activeCard);
        PLAYER(player)->statPopups[0].x = SPRITE(activeCard)->sx + 0x19;
        PLAYER(player)->statPopups[0].y = SPRITE(activeCard)->sy + 0x15;
    } else {
        statusPanel = PLAYER_PANEL(player, HUD_STATUS);
        PLAYER(player)->statPopups[stat].x = statusPanel->x + player * 25 + 0x1C;
        PLAYER(player)->statPopups[stat].y = statusPanel->y + (s16)((stat - 1) * 13 + 3);
    }
}

void renderStatPopups(void) {
    char text[24];
    s32 player;
    s32 stat;
    char *sign;
    s32 age;

    for (player = 0; player < 2; player++) {
        for (stat = 4; stat >= 0; stat--) {
            if (PLAYER(player)->statPopups[stat].timer != 0) {
                PLAYER(player)->statPopups[stat].timer--;
                if (PLAYER(player)->statPopups[stat].type == 7) {
                    sign = "=";
                } else if (PLAYER(player)->statPopups[stat].type == 5) {
                    sign = "+";
                } else {
                    sign = "-";
                }
                age = PLAYER(player)->statPopups[stat].timer;
                if (age < 0x2C) {
                    age = 0x2C;
                }
                sprintf(text, "%s%d", sign, PLAYER(player)->statPopups[stat].value);
                drawBigDigits(PLAYER(player)->statPopups[stat].x + (0x30 - age),
                              PLAYER(player)->statPopups[stat].y - (0x30 - age) * 2, (u8 *)text, (u8 *)&STAT_POPUP_RGB,
                              PLAYER(player)->statPopups[stat].type, 0);
            }
        }
    }
}

void drawHudPanelContents(s32 panelIndex, s32 z) {
    s32 player = panelIndex / 6;
    HudPanel *panel = (HudPanel *)HUD_PANELS + panelIndex;
    char text[72];
    u8 shades[2][4] = { { 0x80, 0x80, 0x80, 0 }, { 0x40, 0x40, 0x40, 0 } };
    char deckText[40];
    u8 *lineColors[10];
    DigimonCardData *card;
    s32 valueColor;
    s32 i;
    s32 k;
    s32 isOpponent;
    s32 x;
    s32 y;

    switch (panelIndex) {
    case 2:
    case 8: {
        s32 idx;

        idx = getActiveDigimonCard(player);
        if (idx >= 0) {
            valueColor = PLAYER(player)->statPenalty ? 3 : 7;
            card = (DigimonCardData *)PLAYER(player)->cards[idx % 30].card;
            drawSmallText(panel->x - player * 14 + 17, panel->y + 2, (s32)card->name, 7, z);
            sprintf(text, "*s0%4d", PLAYER(player)->displayedStats[1]);
            drawText(panel->x + 42 + player * 25, panel->y + 11, (s32)text, valueColor, z);
            sprintf(text, "*s0%4d", PLAYER(player)->displayedStats[2]);
            drawText(panel->x + 42 + player * 25, panel->y + 24, (s32)text, valueColor, z);
            sprintf(text, "*s0%4d", PLAYER(player)->displayedStats[3]);
            drawText(panel->x + 42 + player * 25, panel->y + 37, (s32)text, valueColor, z);
            drawSmallText(panel->x + 24 + player * 24, panel->y + 51, (s32)CROSS_EFFECT_SHORT_NAMES[card->crossEffect], 7, z);
        }
        sprintf(text, "*s0%2d", PLAYER(player)->displayedStats[4]);
        drawText(panel->x + 6 + player * 93, panel->y + 9, (s32)text, 7, z);
        k = 8 - countEmptyDpSlots(player);
        if (k != 0) {
            CUR_SPRT->sp.x0 = panel->x + 3 + player * 94;
            CUR_SPRT->sp.y0 = panel->y + 50;
            CUR_SPRT->sp.u0 = 0xF0;
            CUR_SPRT->sp.v0 = 0x47;
            CUR_SPRT->sp.clut = getClut(800, k + 0x1F7);
            CUR_SPRT->sp.w = 16;
            CUR_SPRT->sp.h = 8;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
        break;
    }
    case 5:
    case 11:
        if (panel->state == 5) {
            CUR_SPRT->sp.x0 = panel->x + 5;
            CUR_SPRT->sp.y0 = panel->y - 56 + player * 64;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0xBA;
            CUR_SPRT->sp.clut = 0x7CF3;
            CUR_SPRT->sp.w = 32;
            CUR_SPRT->sp.h = 62;
            setSemiTrans(&CUR_SPRT->sp, 1);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x3D);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
        break;
    case 4:
    case 10:
        sprintf(text, "%s Deck", PLAYER(player)->deckName);
        drawText(panel->x + 1 + (0x82 - measureText((u8 *)text)) / 2, panel->y + 0x33 + player * -50, (s32)text, 7, z);
        drawText(panel->x + 0x85 + (0x78 - measureText((u8 *)PLAYER(player)->name)) / 2, panel->y + 0x33 + player * -50,
                      (s32)PLAYER(player)->name, 7, z);
        k = countOnlineDeckCards(player) >= 8 ? 7 : 2;
        sprintf(text, "*s0%2d", countOnlineDeckCards(player));
        drawText(panel->x + 4 + player * 0xEC, panel->y + 0x1E + player * 14, (s32)text, k, z);
        sprintf(text, "*s0%2d", countOfflineDeckCards(player));
        drawText(panel->x + 4 + player * 0xEC, panel->y + 6 + player * 14, (s32)text, 7, z);
        for (k = 0; k < PLAYER(player)->wins; k++) {
            drawWinMarker(panel->x + 0xDF + player * -0xDD, panel->y + 4 + player * 13 + k * 15, 0x4A);
        }
        break;
    case 0:
    case 6: {
        s32 artLoaded;
        s32 powerShift;

        if (CUR_CARD == -1) {
            break;
        }
        artLoaded = DUEL->cache[DUEL->artSlot].used;
        if (artLoaded == 1) {
            if (isSpritePoolFull() != 0) {
                break;
            }
            CUR_SPRT->sp.x0 = panel->x;
            CUR_SPRT->sp.y0 = panel->y + 7;
            CUR_SPRT->sp.u0 = (DUEL->artSlot & 1) << 6;
            CUR_SPRT->sp.v0 = ((DUEL->artSlot >> 1) << 6) + 0x40;
            CUR_SPRT->sp.clut = (0x1FF - DUEL->artSlot) << 6;
            CUR_SPRT->sp.w = 64;
            CUR_SPRT->sp.h = 64;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x9A);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        } else if (SPRITE_KIND(CUR_CARD) == 0x19 || DUEL->cursorSlot == 4) {
            drawCardArtPlaceholder(panel->x, panel->y + 7, z, player, 0);
        } else {
            drawCardArtPlaceholder(panel->x, panel->y + 7, z, player, SPRITE(CUR_CARD));
        }
        if (SPRITE_KIND(CUR_CARD) == 0x19) {
            drawText(panel->x + 0x8E, panel->y + 0x10, (s32)"*h-1All-or-Nothing\nGamble!", 7, z);
            break;
        }
        if (DUEL->cursorSlot == 4) {
            if (DUEL->cursorMode == 4) {
                sprintf(deckText, "*h-1All-or-Nothing\nGamble!\nCards left in the\nOnline Deck are %d.",
                        countOnlineDeckCards(DUEL->cursorPlayer));
            } else {
                sprintf(deckText, "*h-1Cards left in the\nOnline Deck is %d.", countOnlineDeckCards(DUEL->cursorPlayer));
            }
            drawText(panel->x + 0x8E, panel->y + 0x10, (s32)deckText, 7, z);
            break;
        }
        for (i = 0; i < 10; i++) {
            lineColors[i] = shades[0];
        }
        switch (DUEL->cursorMode) {
        case 1:
            lineColors[0] = shades[1];
            lineColors[1] = shades[1];
            lineColors[7] = shades[1];
            lineColors[8] = shades[1];
            lineColors[9] = shades[1];
            break;
        case 2:
            for (i = 0; i < 10; i++) {
                lineColors[i] = shades[1];
            }
            lineColors[1] = shades[0];
            break;
        case 3:
        case 6:
            lineColors[1] = shades[1];
            lineColors[7] = shades[1];
            lineColors[8] = shades[1];
            lineColors[9] = shades[1];
            break;
        case 4:
            for (i = 0; i < 10; i++) {
                lineColors[i] = shades[1];
            }
            lineColors[7] = shades[0];
            lineColors[8] = shades[0];
            break;
        case 5:
            for (i = 0; i < 10; i++) {
                lineColors[i] = shades[1];
            }
            lineColors[9] = shades[0];
            break;
        }
        switch (PLAYER(DUEL->cursorPlayer)->cards[(s16)(CUR_CARD % 30)].type) {
        case 0:
            card = (DigimonCardData *)PLAYER(DUEL->cursorPlayer)->cards[(s16)(CUR_CARD % 30)].card;
            sprintf(text, "*s0%2d", card->dpCost);
            drawTextColored(panel->x + 0x7A, panel->y + 0x17, (s32)text, (s32 *)lineColors[0], 7, z);
            sprintf(text, "*s0%2d", card->dpBonus);
            drawTextColored(panel->x + 0x7C, panel->y + 0x2D, (s32)text, (s32 *)lineColors[1], 7, z);
            if (DUEL->cursorMode == 1 || DUEL->cursorMode == 3) {
                if (DUEL->cursorSlot < 4 && DUEL->cursorPlayer == DUEL->turnPlayer) {
                    if (DUEL->cursorMode == 1) {
                        powerShift = card->attr & 0xF;
                    } else {
                        powerShift = PLAYER(player)->statPenalty;
                    }
                    powerShift--;
                    valueColor = 3;
                    if (powerShift <= 0) {
                        valueColor = 7;
                        powerShift = 0;
                    }
                    sprintf(text, "*s0%4d", (card->hp >> powerShift) / 10 * 10);
                    drawTextColored(panel->x + 0x56, panel->y + 0xE, (s32)text, (s32 *)lineColors[2], valueColor, z);
                    for (i = 0; i < 3; i++) {
                        sprintf(text, "*s0%4d", (card->attack[i].power >> powerShift) / 10 * 10);
                        x = panel->x;
                        y = panel->y;
                        drawTextColored(x + 0x56, i * 12 + y + 0x1A, (s32)text, (s32 *)lineColors[i + 3], valueColor, z);
                    }
                    drawIcon(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
                } else if (DUEL->cursorSlot == 6) {
                    valueColor = PLAYER(DUEL->cursorPlayer)->statPenalty ? 3 : 7;
                    for (i = 0; i < 4; i++) {
                        sprintf(text, "*s0%4d", PLAYER(DUEL->cursorPlayer)->stats[i]);
                        x = panel->x;
                        y = panel->y;
                        drawTextColored(x + 0x56, i * 12 + y + 0xE, (s32)text, (s32 *)lineColors[i + 2], valueColor, z);
                    }
                    drawIcon(panel->x + 0xC3, panel->y + 1, 0, PLAYER(DUEL->cursorPlayer)->specialty, z);
                } else {
                    sprintf(text, "*s0%4d", card->hp);
                    drawTextColored(panel->x + 0x56, panel->y + 0xE, (s32)text, (s32 *)lineColors[2], 7, z);
                    for (i = 0; i < 3; i++) {
                        sprintf(text, "*s0%4d", card->attack[i].power);
                        x = panel->x;
                        y = panel->y;
                        drawTextColored(x + 0x56, i * 12 + y + 0x1A, (s32)text, (s32 *)lineColors[i + 3], 7, z);
                    }
                    drawIcon(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
                }
            } else if (DUEL->cursorSlot == 6) {
                valueColor = PLAYER(DUEL->cursorPlayer)->statPenalty ? 3 : 7;
                for (i = 0; i < 4; i++) {
                    sprintf(text, "*s0%4d", PLAYER(DUEL->cursorPlayer)->stats[i]);
                    x = panel->x;
                    y = panel->y;
                    drawTextColored(x + 0x56, i * 12 + y + 0xE, (s32)text, (s32 *)lineColors[i + 2], valueColor, z);
                }
                drawIcon(panel->x + 0xC3, panel->y + 1, 0, PLAYER(DUEL->cursorPlayer)->specialty, z);
            } else {
                sprintf(text, "*s0%4d", card->hp);
                drawTextColored(panel->x + 0x56, panel->y + 0xE, (s32)text, (s32 *)lineColors[2], 7, z);
                for (i = 0; i < 3; i++) {
                    sprintf(text, "*s0%4d", card->attack[i].power);
                    x = panel->x;
                    y = panel->y;
                    drawTextColored(x + 0x56, i * 12 + y + 0x1A, (s32)text, (s32 *)lineColors[i + 3], 7, z);
                }
                drawIcon(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
            }
            drawSmallTextColored(panel->x + 0x44, panel->y + 0x40, CROSS_EFFECT_SHORT_NAMES[card->crossEffect], 7, lineColors[6], z);
            if (CROSS_EFFECT_ICONS[card->crossEffect] != 0) {
                drawIcon(panel->x + 0x75, panel->y + 0x3B, 0, CROSS_EFFECT_ICONS[card->crossEffect] + 0x14, z);
            }
            drawText(panel->x + 0x44, panel->y + 1, (s32)card->name, 7, z);
            drawIcon(panel->x + 0xD4, panel->y + 1, 0, (card->attr & 0xF) + 0x10, z);
            if (card->supportIcon != 0) {
                drawIcon(panel->x + 0xE3, panel->y + 2, 0, card->supportIcon + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                drawTextColored(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)card->supportText[i], (s32 *)lineColors[7], 7, z);
            }
            break;
        case 1: {
            OptionCardData *optionCard;

            optionCard = (OptionCardData *)PLAYER(DUEL->cursorPlayer)->cards[(s16)(CUR_CARD % 30)].card;
            drawText(panel->x + 0x44, panel->y + 1, (s32)optionCard->name, 7, z);
            drawIcon(panel->x + 0xC3, panel->y + 1, 0, 5, z);
            if (optionCard->supportIcon != 0) {
                drawIcon(panel->x + 0xE3, panel->y + 2, 0, optionCard->supportIcon + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                drawTextColored(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)optionCard->text[i], (s32 *)lineColors[8], 7, z);
            }
            break;
        }
        case 2: {
            DigivolveCardData *digivolveCard;

            digivolveCard = (DigivolveCardData *)PLAYER(DUEL->cursorPlayer)->cards[(s16)(CUR_CARD % 30)].card;
            drawText(panel->x + 0x44, panel->y + 1, (s32)digivolveCard->name, 7, z);
            drawIcon(panel->x + 0xC3, panel->y + 1, 0, 6, z);
            for (i = 0; i < 4; i++) {
                drawTextColored(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)digivolveCard->text[i], (s32 *)lineColors[9], 7, z);
            }
            break;
        }
        }
        break;
    }
    case 1:
    case 7:
        card = (DigimonCardData *)PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card;
        valueColor = PLAYER(player)->statPenalty ? 3 : 7;
        drawText(panel->x + 4, panel->y + 1, (s32)card->name, 6, z);
        sprintf(text, "*s0%4d", PLAYER(player)->stats[0]);
        drawText(panel->x + 0x82, panel->y + 1, (s32)text, valueColor, z);
        drawIconColored(panel->x + 0xA4, panel->y + 2, 0, (card->attr & 0xF) + 0x10, shades[0], z);
        drawIconColored(panel->x + 0xB6, panel->y + 2, 0, PLAYER(player)->specialty, shades[0], z);
        for (k = 0; k < 3; k++) {
            drawText(panel->x + 0x25, panel->y + 13 + k * 12, (s32)card->attack[k].name, 7, z);
            sprintf(text, "*s0%4d", PLAYER(player)->baseAttackPowers[k]);
            drawText(panel->x + 0xA2, panel->y + 13 + k * 12, (s32)text, valueColor, z);
        }
        drawText(panel->x + 0x47, panel->y + 0x32, (s32)CROSS_EFFECT_NAMES[card->crossEffect], 7, z);
        if (CROSS_EFFECT_ICONS[card->crossEffect] != 0) {
            drawIcon(panel->x + 0x95, panel->y + 0x32, 0, CROSS_EFFECT_ICONS[card->crossEffect] + 0x14, z);
        }
        if (PLAYER(player)->attackChoice != 3) {
            if (PLAYER(player)->shownAttack != PLAYER(player)->attackChoice) {
                PLAYER(player)->attackHighlightTimer = 0;
            }
            PLAYER(player)->shownAttack = PLAYER(player)->attackChoice;
            if (PLAYER(player)->attackHighlightTimer < 28) {
                PLAYER(player)->attackHighlightTimer++;
                panel->clut = getClut(784, player * 8 + 0x1F0 + PLAYER(player)->attackHighlightTimer / 4);
            } else {
                panel->clut = getClut(784, player * 8 + 0x1F7);
            }
        } else {
            PLAYER(player)->shownAttack = 3;
            panel->clut = getClut(784, player * 8 + 0x1F0);
        }
        isOpponent = DUEL->turnPlayer != player;
        drawTurnSideBadge(panel->x + 0xA7, panel->y + 0x32, isOpponent, 0x80, z);
        break;
    }
}
