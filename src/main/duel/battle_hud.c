#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/battle_hud.h"
#include "dcb/card_render.h"
#include "dcb/duel.h"
#include "dcb/duel_rules.h"
#include "dcb/heap.h"
#include "dcb/main.h"
#include "dcb/scene3d.h"
#include "dcb/sound.h"
#include "dcb/stage.h"
#include "dcb/text.h"

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

void tickCardMotion(s32 cardIndex, s32 player) {
    CardAnim *anim;

    anim = (CardAnim *)(D_801D833C + cardIndex * 36);
    anim->spr->flags |= 0x80;
    switch (SPRITE_KIND(cardIndex)) {
    case 0:
        anim->spr->pos.vx = SLOT(player, 0x90)->x - 0x80 + player * 0xBE;
        anim->spr->pos.vy = SLOT(player, 0x90)->y - 0x54 + player * 0xE;
        anim->spr->pos.vz = 0;
        UNK7F8(cardIndex).rx = 0x2000;
        UNK7F8(cardIndex).ry = 0x2800;
        UNK7F8(cardIndex).rz = 0x1C00;
        anim->spr->scale = 0x800;
        anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x90)->unkC;
        anim->count = 0;
        break;
    case 1:
    case 21:
    case 26:
        ANIM_SAVE(anim);
        anim->total = 0x10;
        anim->count = 0x10;
        playSoundEffect(0xA5);
        anim->state++;
        break;
    case 2:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x90)->x - 0x80 + player * 0xBE);
            targetY = (s16)(SLOT(player, 0x90)->y - 0x54 + player * 0xE);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x1C00;
            scale = 0x800;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state = 0;
        }
        break;
    case 3:
        ANIM_SAVE(anim);
        anim->total = 0x10;
        anim->count = 0x10;
        playSoundEffect(0xA5);
        anim->state++;
    case 4:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x90)->x - 0x5C + player * -10 + anim->unk23 * 0x2B);
            targetY = (s16)(SLOT(player, 0x90)->y - 0x69 + player * 0x21);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
            if (anim->count != 0) {
                break;
            }
        }
        anim->total = 4;
        anim->count = 4;
        anim->state++;
        playSoundEffect(0xA7);
        break;
    case 5:
    case 13:
        if (--anim->count == 0) {
            ANIM_SAVE(anim);
            anim->total = 0xE;
            anim->count = 0xC;
            anim->state++;
        }
        break;
    case 6:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x90)->x - 0x5C + player * -10 + anim->unk23 * 0x2B);
            targetY = (s16)(SLOT(player, 0x90)->y - 0x61 + player * 0x11);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 7:
        anim->spr->pos.vx = SLOT(player, 0x90)->x - 0x5C + player * -10 + anim->unk23 * 0x2B;
        anim->spr->pos.vy = SLOT(player, 0x90)->y - 0x61 + player * 0x11;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2000;
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x1000;
        anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x90)->unkC;
        break;
    case 8:
        ANIM_SAVE(anim);
        anim->total = 0x10;
        anim->count = 0x10;
        playSoundEffect(0xA5);
        anim->state++;
    case 9:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x90)->x - 0x80 + player * 0xBE);
            targetY = (s16)(SLOT(player, 0x90)->y - 0x6C + player * 0xE);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2400;
            scale = 0x800;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 10:
        anim->spr->pos.vx = SLOT(player, 0x90)->x - 0x80 + player * 0xBE;
        anim->spr->pos.vy = SLOT(player, 0x90)->y - 0x6C + player * 0xE;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2000;
        anim->spr->rot.vz = 0x2400;
        anim->spr->scale = 0x800;
        anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x90)->unkC;
        break;
    case 11:
        ANIM_SAVE(anim);
        anim->total = 0x10;
        anim->count = 0x10;
        playSoundEffect(0xA5);
        anim->state++;
    case 12: {
        s32 stackDepth;
        s32 i;

            stackDepth = 0;
            if (anim->count != 0) {
                s32 targetX;
                s32 targetY;
                s16 rx;
                s16 ry;
                s16 rz;
                s16 scale;

                for (i = 2; i >= 0; i--) {
                    if (cardIndex == ((Player *)DUEL_PLAYERS[player])->unk1CA[i]) {
                        break;
                    }
                    stackDepth++;
                }
                targetX = (s16)(SLOT(player, 0x48)->x + (s16)(stackDepth * 2 - 0x46) + (s16)((-0x40 - (stackDepth * 2 + 8) * 2) * player + 8));
                targetY = (s16)(SLOT(player, 0x48)->y - 0x54);
                rx = 0x2000;
                ry = 0x2000;
                rz = 0x2000;
                scale = 0x1000;
                ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
            } else {
                playSoundEffect(0xA7);
                anim->total = 4;
                anim->count = 4;
                anim->state++;
            }
            break;
    }
    case 14: {
        s32 stackDepth;
        s32 i;

            stackDepth = 0;
            if (anim->count != 0) {
                s32 targetX;
                s32 targetY;
                s16 rx;
                s16 ry;
                s16 rz;
                s16 scale;

                for (i = 2; i >= 0; i--) {
                    if (cardIndex == ((Player *)DUEL_PLAYERS[player])->unk1CA[i]) {
                        break;
                    }
                    stackDepth++;
                }
                targetX = (s16)(SLOT(player, 0x48)->x + (s16)(stackDepth * 2 - 0x46) + (s16)((-0x40 - stackDepth * 4) * player));
                targetY = (s16)(SLOT(player, 0x48)->y - 0x54);
                rx = 0x2000;
                ry = 0x2000;
                rz = 0x2000;
                scale = 0x1000;
                ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
            } else {
                anim->state++;
                playSoundEffect(0xA7);
            }
            break;
    }
    case 15: {
        s32 stackDepth;
        s32 i;

            stackDepth = 0;
            for (i = 2; i >= 0; i--) {
                if (cardIndex == ((Player *)DUEL_PLAYERS[player])->unk1CA[i]) {
                    break;
                }
                stackDepth++;
            }
            anim->spr->pos.vx = SLOT(player, 0x48)->x - 0x46 + stackDepth * 2 + (-0x40 - stackDepth * 4) * player;
            anim->spr->pos.vy = SLOT(player, 0x48)->y - 0x54;
            anim->spr->pos.vz = 0;
            anim->spr->rot.vx = 0x2000;
            anim->spr->rot.vy = 0x2000;
            anim->spr->rot.vz = 0x2000;
            anim->spr->scale = 0x1000;
            anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x48)->unkC;
            break;
    }
    case 16:
        ANIM_SAVE(anim);
        anim->total = 0x10;
        anim->count = 0x10;
        if (anim->spr->rot.vy == 0x2000) {
            playSoundEffect(0xA5);
        } else {
            playSoundEffect(0xA6);
        }
        anim->state++;
        break;
    case 17:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x6C)->x - 0x89 + player * -1);
            targetY = (s16)(SLOT(player, 0x6C)->y - 0x50 + player * -0x3E);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            playSoundEffect(0xA7);
            anim->total = 4;
            anim->count = 4;
            anim->state++;
        }
        break;
    case 18:
        if (--anim->count == 0) {
            ANIM_SAVE(anim);
            anim->total = 4;
            anim->count = 4;
            anim->state++;
        }
        break;
    case 19:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x6C)->x - 0x89 + player * -1);
            targetY = (s16)(SLOT(player, 0x6C)->y - 0x58 + player * -0x2E);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 20:
        anim->spr->pos.vx = SLOT(player, 0x6C)->x - player - 0x89;
        anim->spr->pos.vy = SLOT(player, 0x6C)->y - player * 0x2E - 0x58;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2000;
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x1000;
        anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x6C)->unkC;
        break;
    case 22:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x6C)->x - 0x89 + player * -1);
            targetY = (s16)(SLOT(player, 0x6C)->y - 0x50 + player * -0x3E);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->total = 0x20;
            anim->count = 0x20;
            anim->state++;
        }
        break;
    case 23:
        if (anim->count != 0) {
            anim->count--;
            ANIM_SAVE(anim);
            anim->total = 4;
            anim->count = 4;
            anim->state++;
        }
        break;
    case 24:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x6C)->x - 0x89 + player * -1);
            targetY = (s16)(SLOT(player, 0x6C)->y - 0x58 + player * -0x2E);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 25:
        anim->spr->pos.vx = SLOT(player, 0x6C)->x - player - 0x89;
        anim->spr->pos.vy = SLOT(player, 0x6C)->y - player * 0x2E - 0x58;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2800;
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x1000;
        break;
    case 27:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x48)->x - 0x94 + player * 0x5D);
            targetY = (s16)(SLOT(player, 0x48)->y - 0x54);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x800;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state++;
            showDpGainPopup(player);
            playSoundEffect(0xA7);
        }
        break;
    case 28:
        anim->spr->pos.vx = SLOT(player, 0x48)->x - 0x94 + player * 0x5D;
        anim->spr->pos.vy = SLOT(player, 0x48)->y - 0x54;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2000;
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x800;
        anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x48)->unkC;
        break;
    case 29:
        ANIM_SAVE(anim);
        anim->total = 0x20;
        anim->count = 0x20;
        anim->state++;
        break;
    case 30:
        if (anim->count != 0) {
            s32 targetY;
            s16 r;

            targetY = (s16)(0x3C - player * 0x78);
            r = 0x2000;
            ANIM_STEP(anim, 0, targetY, r, r, r, r);
        } else {
            anim->state++;
        }
        break;
    case 31:
        anim->spr->pos.vx = 0;
        anim->spr->pos.vy = 0x3C - player * 0x78;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2000;
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x2000;
        break;
    case 32:
        ANIM_SAVE(anim);
        anim->total = 0x20;
        anim->count = 0x20;
        playSoundEffect(0xA6);
        anim->state++;
        break;
    case 33:
        if (anim->count != 0) {
            s32 targetY;
            s16 ry;

            s16 r;

            targetY = (s16)(0xA0 - player * 0x140);
            r = 0x2000;
            ry = 0x2800 - (player << 12);
            ANIM_STEP(anim, 0, targetY, r, ry, r, r);
        } else {
            anim->state++;
        }
        break;
    case 34:
        anim->spr->pos.vx = 0;
        anim->spr->pos.vy = 0xA0 - player * 0x140;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2800 - (player << 12);
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x2000;
        break;
    }
}

void renderBoardCards(void) {
    char text[8];
    Rect16 hpLabelRect;
    u8 labelRgb[4] = "@@@";
    s32 i;
    s32 j;
    s32 hpLabelDrawn;
    s8 card;
    s32 hpColor;
    s32 z;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 30; j++) {
            tickCardMotion(i * 30 + j, i);
        }
    }
    resetCardPolyCount();
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            card = PLAYER(i)->unk1C2[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), card);
                if (SPRITE_KIND(card) == 0x1C) {
                    break;
                }
            }
        }
        card = PLAYER(i)->unk1CD;
        if (card >= 0) {
            renderCardSprite(SPRITE(card), card);
        }
        hpLabelDrawn = 0;
        for (j = 0; j < 3; j++) {
            card = PLAYER(i)->unk1CA[j];
            if (card >= 0) {
                if (!hpLabelDrawn) {
                    ((u8 *)SPRITE(card))[0x14] = PLAYER(i)->unk178_19;
                    hpLabelDrawn = 1;
                    if (SPRITE_KIND(card) < 0x1D) {
                        hpColor = PLAYER(i)->unk178_15 ? 3 : 7;
                        projectCardSprite(SPRITE(card), card);
                        z = *(s32 *)((u8 *)SPRITE(card) + 0x38);
                        drawIcon(*(s16 *)((u8 *)SPRITE(card) + 0x34) + 2, *(s16 *)((u8 *)SPRITE(card) + 0x36) + 30, 0,
                                      0x1A, z);
                        sprintf(text, "%4d", PLAYER(i)->unk126[0]);
                        drawText(*(s16 *)((u8 *)SPRITE(card) + 0x34) + 15, *(s16 *)((u8 *)SPRITE(card) + 0x36) + 30,
                                      (s32)text, hpColor, z);
                        hpLabelRect.x = 0x60;
                        hpLabelRect.y = 0xDB;
                        hpLabelRect.w = 0x26;
                        hpLabelRect.h = 0xC;
                        drawPageSpriteColored(*(s16 *)((u8 *)SPRITE(card) + 0x34) + 1, *(s16 *)((u8 *)SPRITE(card) + 0x36) + 30,
                                      &hpLabelRect, labelRgb, getTPage(0, 2, SYSTEM_TEX_X, SYSTEM_TEX_Y), 0xC, z);
                        hpLabelDrawn = 1;
                    }
                }
                renderCardSprite(SPRITE(card), card);
            }
        }
        for (j = 0; j < 30; j++) {
            card = PLAYER(i)->unk19B[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), card);
                if (SPRITE_KIND(card) == 10) {
                    break;
                }
            }
        }
        for (j = 3; j >= 0; j--) {
            card = PLAYER(i)->unk1B9[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), card);
            }
        }
        for (j = 0; j < 30; j++) {
            card = PLAYER(i)->unk17D[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), card);
                if (SPRITE_KIND(card) == 0) {
                    break;
                }
            }
        }
    }
}

/* the original file padded its strings with an empty word here */
__asm__(".section .rodata\n\t.word 0\n\t.section .text\n");

void startPanelMove(Panel *panel, s16 targetX, s16 targetY, s32 frames) {
    if (frames == 0) {
        frames = 1;
    }
    panel->unkC |= 0x80;
    if (panel->parent != 0) {
        panel->unkC = panel->parent->unkC;
        panel->unk18 = panel->unk10 - panel->parent->unk10;
        panel->unk1A = panel->unk12 - panel->parent->unk12;
    } else {
        panel->unk18 = panel->unk10;
        panel->unk1A = panel->unk12;
    }
    panel->unk14 = targetX;
    panel->unk16 = targetY;
    panel->unkE = frames;
    panel->unkF = frames;
    panel->unkD++;
}

s32 stepPanelMove(Panel *panel) {
    s16 parentX;
    s16 parentY;

    parentX = 0;
    parentY = 0;
    panel->unkF--;
    if (panel->parent != 0) {
        panel->unkC = panel->parent->unkC;
        parentX = panel->parent->unk10;
        parentY = panel->parent->unk12;
    }
    panel->unk10 = parentX + (panel->unk14 - (panel->unk14 - panel->unk18) * panel->unkF / panel->unkE);
    panel->unk12 = parentY + (panel->unk16 - (panel->unk16 - panel->unk1A) * panel->unkF / panel->unkE);
    if (panel->unkF == 0) {
        panel->unkD++;
    }
    return panel->unkF;
}

void holdPanelAtTarget(Panel *panel) {
    s16 x;
    s16 y;

    x = panel->unk14;
    y = panel->unk16;
    if (panel->parent != 0) {
        panel->unkC = panel->parent->unkC;
        x += panel->parent->unk10;
        y += panel->parent->unk12;
    }
    panel->unk10 = x;
    panel->unk12 = y;
}

void tickDeckPanel(s32 player) {
    void *panel;

    panel = D_801D83EC + (player * 0xD8 + 0x90);
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)panel + 0x10)) = 0x20;
        (*(s16 *)((s8 *)panel + 0x12)) = player * -0x12F + 0xF0;
        break;
    case 1:
        (D_801D83EC + player * 0xD8)[0x55] = 1;
        (D_801D83EC + player * 0xD8)[0xD] = 5;
        (*(u8 *)((s8 *)panel + 0xD)) += 1;
        break;
    case 2:
        startPanelMove(panel, 0x20, -(player * 0x7D) + 0x99, 0x10);
        break;
    case 3:
        stepPanelMove(panel);
        break;
    case 4:
        holdPanelAtTarget(panel);
        break;
    case 7:
        (D_801D83EC + player * 0xD8)[0x55] = 6;
        (D_801D83EC + player * 0xD8)[0xD] = 5;
        (*(u8 *)((s8 *)panel + 0xD)) = 2;
        break;
    case 11:
        startPanelMove(panel, 0xE8, -(player * 0x7D) + 0x99, 0x10);
        break;
    case 12:
        if (stepPanelMove(panel) == 0) {
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
        }
        break;
    }
}

void func_8003DD9C(s32 player) {
    void *panel;
    s32 y;

    panel = D_801D83EC + (player * 0xD8 + 0xB4);
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)panel + 0x10)) = 0x164;
        y = 0x31 - player * 0x31;
        (*(s16 *)((s8 *)panel + 0x12)) = y;
        startPanelMove(panel, 0x164, y, 0);
        stepPanelMove(panel);
        (*(u8 *)((s8 *)panel + 0xD)) = 0;
        break;
    case 1:
        startPanelMove(panel, 0x100, 0x31 - player * 0x31, 8);
        break;
    case 2:
        if (stepPanelMove(panel) == 0) {
            playSoundEffect(0xA7);
        }
        break;
    case 3:
        startPanelMove(panel, 0xF9, 0x31 - player * 0x31, 8);
        break;
    case 4:
        stepPanelMove(panel);
        break;
    case 5:
        holdPanelAtTarget(panel);
        break;
    case 6:
        startPanelMove(panel, 0x164, 0x31 - player * 0x31, 8);
        break;
    case 7:
        if (stepPanelMove(panel) == 0) {
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
        }
        break;
    }
}

void tickStatusPanel(s32 player) {
    void *panel;

    panel = D_801D83EC + (player * 0xD8 + 0x48);
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)panel + 0x10)) = player * 0x2A0 - 0xEC;
        (*(s16 *)((s8 *)panel + 0x12)) = 0x5C;
        break;
    case 1:
        startPanelMove(panel, player * 0x7C + 0x28, 0x5C, 0x10);
        (D_801D83EC + player * 0xD8)[0x79] = 4;
        break;
    case 2:
        stepPanelMove(panel);
        break;
    case 3:
        holdPanelAtTarget(panel);
        break;
    case 4:
        startPanelMove(panel, player * 0x2A0 - 0xEC, 0x5C, 0x10);
        break;
    case 5:
        if (stepPanelMove(panel) == 0) {
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
        }
        break;
    case 6:
        startPanelMove(panel, player * 0xCC, 0x5C, 0x10);
        break;
    case 7:
        if (stepPanelMove(panel) == 0) {
            (D_801D83EC + player * 0xD8)[0x79] = 1;
        }
        break;
    case 8:
        holdPanelAtTarget(panel);
        break;
    }
}

void func_8003E11C(s32 player) {
    void *panel;
    s32 x;
    s32 y;

    panel = D_801D83EC + (player * 0xD8 + 0x6C);
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        x = -(player * 0x41) + 0x44;
        (*(s16 *)((s8 *)panel + 0x10)) = x;
        y = player * 0x1E + 0xA;
        (*(s16 *)((s8 *)panel + 0x12)) = y;
        startPanelMove(panel, x, y, 0);
        stepPanelMove(panel);
        (*(u8 *)((s8 *)panel + 0xD)) = 0;
        break;
    case 1:
        startPanelMove(panel, -(player * 0xA1) + 0x74, player * 0x1E + 0xA, 8);
        break;
    case 2:
        if (stepPanelMove(panel) == 0) {
            playSoundEffect(0xA7);
        }
        break;
    case 3:
        holdPanelAtTarget(panel);
        break;
    case 4:
        startPanelMove(panel, -(player * 0x41) + 0x44, player * 0x1E + 0xA, 8);
        break;
    case 5:
        if (stepPanelMove(panel) == 0) {
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
        }
        break;
    }
}

void tickAttackPanel(s32 player) {
    void *panel;

    panel = D_801D83EC + (player * 0xD8 + 0x24);
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)panel + 0x10)) = 0x38;
        (*(s16 *)((s8 *)panel + 0x12)) = player * -0x12F + 0xF0;
        break;
    case 1:
        startPanelMove(panel, 0x38, player * -0x7F + 0x99, 0x10);
        break;
    case 2:
        stepPanelMove(panel);
        break;
    case 3:
        holdPanelAtTarget(panel);
        break;
    case 4:
        startPanelMove(panel, 0x38, player * -0x12F + 0xF0, 0x10);
        break;
    case 5:
        if (stepPanelMove(panel) == 0) {
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
        }
        break;
    }
}

void tickCardInfoPanel(s32 player) {
    void *panel;

    panel = D_801D83EC + player * 0xD8;
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)panel + 0x10)) = -0xFF;
        (*(s16 *)((s8 *)panel + 0x12)) = player * 0x7E + 0x16;
        break;
    case 1:
        startPanelMove(panel, 0x22, 0x16, 0xC);
        (*(u8 *)((s8 *)panel + 0xD)) = 3;
        break;
    case 2:
        startPanelMove(panel, 0x22, 0x94, 0xA);
        (*(u8 *)((s8 *)panel + 0xD)) = 3;
        break;
    case 3:
        stepPanelMove(panel);
        break;
    case 4:
        holdPanelAtTarget(panel);
        break;
    case 5:
        startPanelMove(panel, -0xFF, (*(s16 *)((s8 *)panel + 0x12)), 0xC);
        break;
    case 6:
        if (stepPanelMove(panel) == 0) {
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
        }
        break;
    }
}

void tickBattleHud(void) {
    s32 player;
    s32 i;
    s32 delta;
    s32 step;
    s32 rollingCount;

    for (i = 0; i < 2; i++) {
        tickDeckPanel(i);
        func_8003DD9C(i);
        tickAttackPanel(i);
        tickStatusPanel(i);
        func_8003E11C(i);
        tickCardInfoPanel(i);
    }
    for (player = 0; player < 2; player++) {
        PLAYER(player)->unk11C[4] = sumDigivolvePoints(player);
        if (getActiveDigimonCard(player) == -1) {
            for (i = 0; i < 4; i++) {
                PLAYER(player)->unk126[i] = 0;
                PLAYER(player)->unk11C[i] = 0;
            }
        }
        for (i = 0; i < 5; i++) {
            delta = PLAYER(player)->unk126[i] - PLAYER(player)->unk11C[i];
            step = (delta < 0 ? -delta : delta) / 16 + 1;
            if (PLAYER(player)->unk126[i] < PLAYER(player)->unk11C[i]) {
                PLAYER(player)->unk126[i] += step;
                if (PLAYER(player)->unk126[i] > PLAYER(player)->unk11C[i]) {
                    PLAYER(player)->unk126[i] = PLAYER(player)->unk11C[i];
                }
            } else if (PLAYER(player)->unk126[i] > PLAYER(player)->unk11C[i]) {
                PLAYER(player)->unk126[i] -= step;
                if (PLAYER(player)->unk126[i] < PLAYER(player)->unk11C[i]) {
                    PLAYER(player)->unk126[i] = PLAYER(player)->unk11C[i];
                }
            }
        }
    }
    rollingCount = 0;
    for (player = 0; player < 2; player++) {
        for (i = 0; i < 5; i++) {
            if (PLAYER(player)->unk126[i] != PLAYER(player)->unk11C[i]) {
                rollingCount++;
            }
        }
    }
    if (rollingCount != 0 && !(((Unk8006E050 *)PLAYER_PROFILES)->unk24 & 3)) {
        playSoundEffect(0xAA);
    }
    renderStatPopups();
    for (i = 0; i < 12; i++) {
        if (PANEL(i).flags & 0x80) {
            drawHudSprite((SprtInfo *)&PANEL(i), i, i * 2 + PANEL(i).z + 1);
            drawHudPanelContents(i, i * 2 + PANEL(i).z);
        }
    }
}

void initDuelState(s32 isCpuDuel) {
    void *block;

    D_801D833C = block = allocTaskHeapBlock(0x870);
    D_801D8340 = block = allocTaskHeapBlock(0x86C);
    (*(s32 *)((s8 *)D_801D8340 + 0x7F8)) = func_801F8854();
    (*(s8 *)((s8 *)D_801D8340 + 0x817)) = (s8) (rand() % 2);
    (*(s8 *)((s8 *)D_801D8340 + 0x818)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81B)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81C)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x810)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x81F)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x825)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x823)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x822)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x824)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x820)) = 0;
    func_801F8200();
    initDuelPlayers(isCpuDuel);
    (*(s8 *)((s8 *)D_801D8340 + 0x81D)) = -1;
}

void startDuelScene(void) {
    Unk800794F8 *camera;

    initScene3D(0);
    func_800149B8(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    func_80014C08(2);
    camera = (Unk800794F8 *)&GRAPHICS;
    camera->unk54 = 0;
    camera->unk56 = 0;
    camera->unk58 = 0;
    camera->unk7C = 0;
    camera->unk80 = 0;
    camera->unk84 = 0;
    camera->unk8E = 0;
    camera->unk90 = 0x1C0;
    camera->unk92 = 0;
    camera->unk94 = 0;
    camera->unk8C = -1;
    camera->unk74 = 1;
    (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 0;
    func_80014C08(2);
}

void spawnDuelTasks(s32 isCpuDuel) {
    s32 stageId;
    s32 stageArg;

    (*(s32 *)((s8 *)D_801D8340 + 0x58)) = func_801F8998(0, 0x26, 0x2E, 0xA, 1);
    func_800149B8(0x1E, -1, 0, 0x800, &runDuelTurnLoop, 0, 0, 0, 0);
    if ((isCpuDuel != 0) && ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) == 0)) {
        func_800149B8(0, -1, 0, 0x800, runCpuDecisionTask, 0, 0, 0, 0);
    }
    func_800149B8(0, -1, 0, 0x800, &runCardArtLoader, 0, 0, 0, 0);
    if (isCpuDuel != 0) {
        stageId = (*(u8 *)((s8 *)D_8006E054 + 0x72));
        stageArg = (*(u8 *)((s8 *)D_8006E054 + 0x71));
    } else {
        stageId = -1;
        stageArg = -1;
    }
    func_800149B8(0, -1, 0, 0x1000, &runDuelStageTask, stageId, stageArg, 0, 0);
}

void teardownDuelScene(void) {
    func_80014A00(0x19);
    func_801F848C();
    func_801F88E8();
    freeHeapBlocksByTag(0x7F);
}

void renderDuelFrame(void) {
    s16 fadeLevel;

    fadeLevel = (*(s16 *)((s8 *)D_801D8340 + 0x808));
    if (fadeLevel != 0) {
        renderDuelBackground(fadeLevel);
        renderStatusMessage((*(s16 *)((s8 *)D_801D8340 + 0x808)));
        renderHelpBar((*(s16 *)((s8 *)D_801D8340 + 0x808)));
    }
    renderPhaseBanner();
    tickBattleHud();
    renderBoardCards();
    if ((*(s32 *)((s8 *)D_801D8340 + 0x83C)) == 0) {
        if ((*(s32 *)((s8 *)D_801D8340 + 0x828)) != -1) {
            func_801F97F4();
        }
        func_801EB53C(D_801D83D1);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/duel/battle_hud", DUEL_VRAM_CLEAR_RECT);

INCLUDE_ASM("asm/main/nonmatchings/duel/battle_hud", runDuel);
