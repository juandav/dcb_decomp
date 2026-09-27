#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/battle_hud.h"
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
#include "dcb/scroll_bg.h"
#include "dcb/text.h"
#include "dcb/str_util.h"

void waitForStatCountersToSettle(void) {
    s32 unsettled;
    s32 player;
    s32 i;

    do {
        func_80014C08(FRAME_INTERVAL);
        unsettled = 0;
        for (player = 0; player < 2; player++) {
            for (i = 0; i < 5; i++) {
                if (((Player *)DUEL_PLAYERS[player])->unk11C[i] !=
                    ((Player *)DUEL_PLAYERS[player])->unk126[i]) {
                    unsettled = 1;
                }
            }
        }
    } while (unsettled);
}

void showDpGainPopup(s32 player) {
    s32 dpCard;
    u8 *statusPanel;

    dpCard = peekDpSlotTop(player);
    if (dpCard != -1) {
        statusPanel = D_801D83EC + (player * 0xD8 + 0x48);
        PLAYER(player)->unk130[4].value = (s8)PLAYER(player)->cards[dpCard % 30].card[0x1C];
        PLAYER(player)->unk130[4].type = 5;
        PLAYER(player)->unk130[4].timer = 0x30;
        PLAYER(player)->unk130[4].x = *(s16 *)(statusPanel + 0x10) + (s16)(player * 93 + 0x10);
        PLAYER(player)->unk130[4].y = *(u16 *)(statusPanel + 0x12) + 2;
    } else {
        PLAYER(player)->unk130[4].timer = 0;
    }
}

void showStatChangePopup(s32 player, s32 newValue, s32 stat) {
    s32 activeCard;
    u8 *statusPanel;

    PLAYER(player)->unk130[stat].value = newValue - PLAYER(player)->unk11C[stat];
    if (PLAYER(player)->unk130[stat].value == 0) {
        PLAYER(player)->unk130[stat].type = 7;
    } else if (PLAYER(player)->unk130[stat].value > 0) {
        PLAYER(player)->unk130[stat].type = 5;
    } else {
        PLAYER(player)->unk130[stat].type = 2;
    }
    PLAYER(player)->unk130[stat].value = abs(PLAYER(player)->unk130[stat].value);
    PLAYER(player)->unk130[stat].timer = 0x30;
    if (stat == 0) {
        activeCard = getActiveDigimonCard(player);
        projectCardSprite(*(void **)(D_801D833C + activeCard * 36), activeCard);
        PLAYER(player)->unk130[0].x = *(u16 *)(*(u8 **)(D_801D833C + activeCard * 36) + 0x34) + 0x19;
        PLAYER(player)->unk130[0].y = *(u16 *)(*(u8 **)(D_801D833C + activeCard * 36) + 0x36) + 0x15;
    } else {
        statusPanel = D_801D83EC + (player * 0xD8 + 0x48);
        PLAYER(player)->unk130[stat].x = *(u16 *)(statusPanel + 0x10) + player * 25 + 0x1C;
        PLAYER(player)->unk130[stat].y = *(s16 *)(statusPanel + 0x12) + (s16)((stat - 1) * 13 + 3);
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
            if (((Player *)DUEL_PLAYERS[player])->unk130[stat].timer != 0) {
                ((Player *)DUEL_PLAYERS[player])->unk130[stat].timer--;
                if (((Player *)DUEL_PLAYERS[player])->unk130[stat].type == 7) {
                    sign = "=";
                } else if (((Player *)DUEL_PLAYERS[player])->unk130[stat].type == 5) {
                    sign = "+";
                } else {
                    sign = "-";
                }
                age = ((Player *)DUEL_PLAYERS[player])->unk130[stat].timer;
                if (age < 0x2C) {
                    age = 0x2C;
                }
                sprintf(text, "%s%d", sign, ((Player *)DUEL_PLAYERS[player])->unk130[stat].value);
                drawBigDigits(((Player *)DUEL_PLAYERS[player])->unk130[stat].x + (0x30 - age),
                              ((Player *)DUEL_PLAYERS[player])->unk130[stat].y - (0x30 - age) * 2, (u8 *)text, (u8 *)&D_8006E298,
                              ((Player *)DUEL_PLAYERS[player])->unk130[stat].type, 0);
            }
        }
    }
}

void drawHudPanelContents(s32 panelIndex, s32 z) {
    s32 player = panelIndex / 6;
    InfoPanel *panel = (InfoPanel *)D_801D83EC + panelIndex;
    char text[72];
    u8 shades[2][4] = { { 0x80, 0x80, 0x80, 0 }, { 0x40, 0x40, 0x40, 0 } };
    char deckText[40];
    u8 *lineColors[10];
    CardInfo *card;
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
            valueColor = PLAYER(player)->unk178_15 ? 3 : 7;
            card = (CardInfo *)PLAYER(player)->cards[idx % 30].card;
            drawSmallText(panel->x - player * 14 + 17, panel->y + 2, (s32)card->name, 7, z);
            sprintf(text, "*s0%4d", PLAYER(player)->unk126[1]);
            drawText(panel->x + 42 + player * 25, panel->y + 11, (s32)text, valueColor, z);
            sprintf(text, "*s0%4d", PLAYER(player)->unk126[2]);
            drawText(panel->x + 42 + player * 25, panel->y + 24, (s32)text, valueColor, z);
            sprintf(text, "*s0%4d", PLAYER(player)->unk126[3]);
            drawText(panel->x + 42 + player * 25, panel->y + 37, (s32)text, valueColor, z);
            drawSmallText(panel->x + 24 + player * 24, panel->y + 51, (s32)CROSS_EFFECT_SHORT_NAMES[card->unkE4], 7, z);
        }
        sprintf(text, "*s0%2d", PLAYER(player)->unk126[4]);
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
        sprintf(text, "%s Deck", PLAYER(player)->unk0 + 1);
        drawText(panel->x + 1 + (0x82 - measureText((u8 *)text)) / 2, panel->y + 0x33 + player * -50, (s32)text, 7, z);
        drawText(panel->x + 0x85 + (0x78 - measureText((u8 *)PLAYER(player)->unk1CE)) / 2, panel->y + 0x33 + player * -50,
                      (s32)PLAYER(player)->unk1CE, 7, z);
        k = countOnlineDeckCards(player) >= 8 ? 7 : 2;
        sprintf(text, "*s0%2d", countOnlineDeckCards(player));
        drawText(panel->x + 4 + player * 0xEC, panel->y + 0x1E + player * 14, (s32)text, k, z);
        sprintf(text, "*s0%2d", countOfflineDeckCards(player));
        drawText(panel->x + 4 + player * 0xEC, panel->y + 6 + player * 14, (s32)text, 7, z);
        for (k = 0; k < PLAYER(player)->unk17C; k++) {
            drawWinMarker(panel->x + 0xDF + player * -0xDD, panel->y + 4 + player * 13 + k * 15, 0x4A);
        }
        break;
    case 0:
    case 6: {
        s32 artLoaded;
        s32 powerShift;

        if (*(s16 *)(DUEL->unk58 + 2) == -1) {
            break;
        }
        artLoaded = DUEL->cache[DUEL->unk826].used;
        if (artLoaded == 1) {
            if (isSpritePoolFull() != 0) {
                break;
            }
            CUR_SPRT->sp.x0 = panel->x;
            CUR_SPRT->sp.y0 = panel->y + 7;
            CUR_SPRT->sp.u0 = (DUEL->unk826 & 1) << 6;
            CUR_SPRT->sp.v0 = ((DUEL->unk826 >> 1) << 6) + 0x40;
            CUR_SPRT->sp.clut = (0x1FF - DUEL->unk826) << 6;
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
        } else if (SPRITE_KIND(*(s16 *)(DUEL->unk58 + 2)) == 0x19 || DUEL->unk81C == 4) {
            drawCardArtPlaceholder(panel->x, panel->y + 7, z, player, 0);
        } else {
            drawCardArtPlaceholder(panel->x, panel->y + 7, z, player, SPRITE(*(s16 *)(DUEL->unk58 + 2)));
        }
        if (SPRITE_KIND(*(s16 *)(DUEL->unk58 + 2)) == 0x19) {
            drawText(panel->x + 0x8E, panel->y + 0x10, (s32)"*h-1All-or-Nothing\nGamble!", 7, z);
            break;
        }
        if (DUEL->unk81C == 4) {
            if (DUEL->unk81D == 4) {
                sprintf(deckText, "*h-1All-or-Nothing\nGamble!\nCards left in the\nOnline Deck are %d.",
                        countOnlineDeckCards(DUEL->unk81B));
            } else {
                sprintf(deckText, "*h-1Cards left in the\nOnline Deck is %d.", countOnlineDeckCards(DUEL->unk81B));
            }
            drawText(panel->x + 0x8E, panel->y + 0x10, (s32)deckText, 7, z);
            break;
        }
        for (i = 0; i < 10; i++) {
            lineColors[i] = shades[0];
        }
        switch (DUEL->unk81D) {
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
        switch (PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].state) {
        case 0:
            card = (CardInfo *)PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            sprintf(text, "*s0%2d", card->unk1B);
            drawTextColored(panel->x + 0x7A, panel->y + 0x17, (s32)text, (s32 *)lineColors[0], 7, z);
            sprintf(text, "*s0%2d", card->level);
            drawTextColored(panel->x + 0x7C, panel->y + 0x2D, (s32)text, (s32 *)lineColors[1], 7, z);
            if (DUEL->unk81D == 1 || DUEL->unk81D == 3) {
                if (DUEL->unk81C < 4 && DUEL->unk81B == DUEL->unk817) {
                    if (DUEL->unk81D == 1) {
                        powerShift = card->attr & 0xF;
                    } else {
                        powerShift = PLAYER(player)->unk178_15;
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
                } else if (DUEL->unk81C == 6) {
                    valueColor = PLAYER(DUEL->unk81B)->unk178_15 ? 3 : 7;
                    for (i = 0; i < 4; i++) {
                        sprintf(text, "*s0%4d", PLAYER(DUEL->unk81B)->unk11C[i]);
                        x = panel->x;
                        y = panel->y;
                        drawTextColored(x + 0x56, i * 12 + y + 0xE, (s32)text, (s32 *)lineColors[i + 2], valueColor, z);
                    }
                    drawIcon(panel->x + 0xC3, panel->y + 1, 0, PLAYER(DUEL->unk81B)->unk178_19, z);
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
            } else if (DUEL->unk81C == 6) {
                valueColor = PLAYER(DUEL->unk81B)->unk178_15 ? 3 : 7;
                for (i = 0; i < 4; i++) {
                    sprintf(text, "*s0%4d", PLAYER(DUEL->unk81B)->unk11C[i]);
                    x = panel->x;
                    y = panel->y;
                    drawTextColored(x + 0x56, i * 12 + y + 0xE, (s32)text, (s32 *)lineColors[i + 2], valueColor, z);
                }
                drawIcon(panel->x + 0xC3, panel->y + 1, 0, PLAYER(DUEL->unk81B)->unk178_19, z);
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
            drawSmallTextColored(panel->x + 0x44, panel->y + 0x40, CROSS_EFFECT_SHORT_NAMES[card->unkE4], 7, lineColors[6], z);
            if (D_8006E4FC[card->unkE4] != 0) {
                drawIcon(panel->x + 0x75, panel->y + 0x3B, 0, D_8006E4FC[card->unkE4] + 0x14, z);
            }
            drawText(panel->x + 0x44, panel->y + 1, (s32)card->name, 7, z);
            drawIcon(panel->x + 0xD4, panel->y + 1, 0, (card->attr & 0xF) + 0x10, z);
            if (card->unkE6 != 0) {
                drawIcon(panel->x + 0xE3, panel->y + 2, 0, card->unkE6 + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                drawTextColored(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)card->text[i], (s32 *)lineColors[7], 7, z);
            }
            break;
        case 1: {
            s8 *optionCard;

            optionCard = PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            drawText(panel->x + 0x44, panel->y + 1, (s32)(optionCard + 3), 7, z);
            drawIcon(panel->x + 0xC3, panel->y + 1, 0, 5, z);
            if (optionCard[0x8C] != 0) {
                drawIcon(panel->x + 0xE3, panel->y + 2, 0, optionCard[0x8C] + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                drawTextColored(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)(optionCard + 0x8D + i * 21), (s32 *)lineColors[8], 7,
                              z);
            }
            break;
        }
        case 2: {
            s8 *optionCard;

            optionCard = PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            drawText(panel->x + 0x44, panel->y + 1, (s32)(optionCard + 3), 7, z);
            drawIcon(panel->x + 0xC3, panel->y + 1, 0, 6, z);
            for (i = 0; i < 4; i++) {
                drawTextColored(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)(optionCard + 0x1B + i * 21), (s32 *)lineColors[9], 7,
                              z);
            }
            break;
        }
        }
        break;
    }
    case 1:
    case 7:
        card = (CardInfo *)PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card;
        valueColor = PLAYER(player)->unk178_15 ? 3 : 7;
        drawText(panel->x + 4, panel->y + 1, (s32)card->name, 6, z);
        sprintf(text, "*s0%4d", PLAYER(player)->unk11C[0]);
        drawText(panel->x + 0x82, panel->y + 1, (s32)text, valueColor, z);
        drawIconColored(panel->x + 0xA4, panel->y + 2, 0, (card->attr & 0xF) + 0x10, shades[0], z);
        drawIconColored(panel->x + 0xB6, panel->y + 2, 0, PLAYER(player)->unk178_19, shades[0], z);
        for (k = 0; k < 3; k++) {
            drawText(panel->x + 0x25, panel->y + 13 + k * 12, (s32)card->attack[k].name, 7, z);
            sprintf(text, "*s0%4d", PLAYER(player)->unk15C[k]);
            drawText(panel->x + 0xA2, panel->y + 13 + k * 12, (s32)text, valueColor, z);
        }
        drawText(panel->x + 0x47, panel->y + 0x32, (s32)CROSS_EFFECT_NAMES[card->unkE4], 7, z);
        if (D_8006E4FC[card->unkE4] != 0) {
            drawIcon(panel->x + 0x95, panel->y + 0x32, 0, D_8006E4FC[card->unkE4] + 0x14, z);
        }
        if (PLAYER(player)->unk178_2 != 3) {
            if (PLAYER(player)->unk178_4 != PLAYER(player)->unk178_2) {
                PLAYER(player)->unk16E = 0;
            }
            PLAYER(player)->unk178_4 = PLAYER(player)->unk178_2;
            if (PLAYER(player)->unk16E < 28) {
                PLAYER(player)->unk16E++;
                panel->clut = getClut(784, player * 8 + 0x1F0 + PLAYER(player)->unk16E / 4);
            } else {
                panel->clut = getClut(784, player * 8 + 0x1F7);
            }
        } else {
            PLAYER(player)->unk178_4 = 3;
            panel->clut = getClut(784, player * 8 + 0x1F0);
        }
        isOpponent = DUEL->unk817 != player;
        func_80044504(panel->x + 0xA7, panel->y + 0x32, isOpponent, 0x80, z);
        break;
    }
}
