#include "dcb/card_motion.h"
#include "common.h"
#include "game.h"
#include "dcb/battle_hud.h"
#include "dcb/card_render.h"
#include "dcb/card_db.h"
#include "dcb/card_zones.h"
#include "dcb/duel.h"
#include "dcb/kaw_hand.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"

/* jp's card motion (card_motion.c is us's and eu's): jp keeps battle_hud's
   drawHudPanelContents here, before tickCardMotion and renderBoardCards */

/* jp's callees that no header declares yet */
void drawSmallDigits(s32 x, s32 y, u8 *text, s32 palette, s32 z);
void drawSmallDigitsColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z);
s32 convertSjisToTinyText(u8 *src, u8 *dst);
void drawMessageBarPanel(s32 z);

/* 1 while uploadStringGlyphs uploads its glyphs */
s32 GLYPH_UPLOAD_BUSY = 0;

/* Draws what HUD panel panelIndex shows over its quad: eleven per player
   (0-10 and 11-21), then 22, the message bar */
void drawHudPanelContents(s32 panelIndex, s32 z) {
    s32 player = panelIndex / 11;
    Panel *panel = HUD_PANEL(panelIndex);
    char text[40];
    u8 shades[2][4] = { { 0x80, 0x80, 0x80, 0 }, { 0x60, 0x60, 0x60, 0 } };
    char deckText[40];
    u8 *lineColors[8];
    DigimonCardData *card;
    OptionCardData *option;
    s32 valueColor;

    switch (panelIndex) {
    case 0:
    case 11: {
        s32 idx;
        s32 i;

        if (DUEL->turnPlayer == player) {
            drawTurnSideBadge(panel->sx + 0x47, panel->sy + 2 + player * 61, 2, 0x80, z);
        } else {
            drawTurnSideBadge(panel->sx + 0x47, panel->sy + 2 + player * 61, 2, 0x30, z);
        }
        idx = getActiveDigimonCard(player);
        if (idx >= 0) {
            valueColor = PLAYER(player)->statPenalty ? 2 : 7;
            card = (DigimonCardData *)PLAYER_CARDS(PLAYER(player))[idx % 30].card;
            convertSjisToTinyText(card->name, text);
            drawTinyText(panel->sx + 4, panel->sy + 4 + player * 63, (s32)text, 7, z);
            sprintf(text, "w-1%4d", PLAYER(player)->displayedStats[0]);
            drawText(panel->sx + 0x37, panel->sy + 0xF + player * 36, (s32)text, valueColor, z);
            sprintf(text, "w-1%4d", PLAYER(player)->displayedStats[1]);
            drawText(panel->sx + 0x37, panel->sy + 0x1C + player * 10, (s32)text, valueColor, z);
            sprintf(text, "w-1%4d", PLAYER(player)->displayedStats[2]);
            drawText(panel->sx + 0x37, panel->sy - player * 16 + 0x29, (s32)text, valueColor, z);
            sprintf(text, "w-1%4d", PLAYER(player)->displayedStats[3]);
            drawText(panel->sx + 0x37, panel->sy - player * 42 + 0x36, (s32)text, valueColor, z);
            drawTinyText(panel->sx + 0x21, panel->sy - player * 65 + 0x44, (s32)CROSS_EFFECT_SHORT_NAMES[card->crossEffect],
                         7, z);
        } else {
            for (i = 0; i < 4; i++) {
                PLAYER(player)->displayedStats[i] = 0;
            }
        }
        break;
    }
    case 1:
    case 12:
        sprintf(text, "%3d", PLAYER(player)->displayedStats[4]);
        drawSmallDigits(panel->sx + 0x14, panel->sy + 0x21 + player * -19, text, 7, z);
        break;
    case 10:
    case 21:
        drawIconText(panel->sx + 0x19, panel->sy + 2, 7, 1, z, (s32)PLAYER(player)->name);
        break;
    case 3:
    case 14:
        sprintf(text, "%sデック", PLAYER(player)->deck->name);
        drawIconText(panel->sx + 0x62 - measureText(1, text) / 2, panel->sy - player * 48 + 0x32, 7, 1, z, (s32)text);
        switch (PLAYER(player)->wins) {
        case 3:
            drawWinMarker(panel->sx + 0xC1, panel->sy - player * 46 + 0x29, 1, 0x68);
        case 2:
            drawWinMarker(panel->sx + 0xAE, panel->sy - player * 46 + 0x2F, 0, 0x68);
        case 1:
            drawWinMarker(panel->sx + 0x9B, panel->sy - player * 46 + 0x2F, 0, 0x68);
            break;
        }
        break;
    case 4:
    case 15: {
        s32 artLoaded;

        artLoaded = DUEL->cache[DUEL->artSlot].used;
        if (artLoaded == 1) {
            if (isSpritePoolFull() != 0) {
                break;
            }
            CUR_SPRT->sp.x0 = panel->sx + 0xD;
            CUR_SPRT->sp.y0 = panel->sy + 2 + player * -67;
            CUR_SPRT->sp.u0 = (DUEL->artSlot & 1) << 6;
            CUR_SPRT->sp.v0 = ((DUEL->artSlot >> 1) << 6) + 0x40;
            CUR_SPRT->sp.clut = getClut(0x2C0, DUEL->artSlot + 0xF2);
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
        } else {
            drawCardArtPlaceholder(panel->sx + 0xD, panel->sy + 2 + player * -67, z);
        }
        break;
    }
    case 5:
    case 16: {
        s32 i;

        if (SPRITE_KIND(CUR_CARD) == 0x17) {
            drawIconText(panel->sx + 0x29, panel->sy + 9 + player * 30, 7, 1, z, (s32)"h-1いちかばちか\nチャレンジ中！！");
            break;
        }
        if (DUEL->cursorSlot == 4) {
            if (DUEL->cursorMode == 4) {
                sprintf(deckText, "h-1いちかばちか\nチャレンジ！！\n残り山札枚数は\n%d枚です。",
                        countOnlineDeckCards(DUEL->cursorPlayer));
            } else {
                sprintf(deckText, "h-1残り山札枚数は\n%d枚です。", countOnlineDeckCards(DUEL->cursorPlayer));
            }
            drawIconText(panel->sx + 0x29, panel->sy + 9 + player * 30, 7, 1, z, (s32)deckText);
            break;
        }
        for (i = 0; i < 8; i++) {
            lineColors[i] = shades[0];
        }
        switch (DUEL->cursorMode) {
        case 1:
            lineColors[0] = shades[1];
            lineColors[1] = shades[1];
            lineColors[7] = shades[1];
            break;
        case 2:
            for (i = 0; i < 8; i++) {
                lineColors[i] = shades[1];
            }
            lineColors[1] = shades[0];
            break;
        case 3:
            lineColors[1] = shades[1];
            lineColors[7] = shades[1];
            break;
        case 4:
        case 5:
            for (i = 0; i < 8; i++) {
                lineColors[i] = shades[1];
            }
            lineColors[7] = shades[0];
            break;
        }
        switch (PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[(s16)(CUR_CARD % 30)].type) {
        case 0:
            card = (DigimonCardData *)PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[(s16)(CUR_CARD % 30)].card;
            sprintf(text, "%2d", card->dpCost);
            drawSmallDigitsColored(panel->sx + 0x19, panel->sy + 3 + player * 80, text, 7, lineColors[0], z);
            sprintf(text, "%2d", card->dpBonus);
            drawSmallDigitsColored(panel->sx + 0x19, panel->sy + 0xD + player * 60, text, 7, lineColors[1], z);
            sprintf(text, "%4d", card->hp);
            drawSmallDigitsColored(panel->sx + 0x10, panel->sy + 0x19 + player * 36, text, 7, lineColors[2], z);
            sprintf(text, "%4d", card->attack[0].power);
            drawSmallDigitsColored(panel->sx + 0x10, panel->sy + 0x23 + player * 16, text, 7, lineColors[3], z);
            sprintf(text, "%4d", card->attack[1].power);
            drawSmallDigitsColored(panel->sx + 0x10, panel->sy - player * 4 + 0x2D, text, 7, lineColors[4], z);
            sprintf(text, "%4d", card->attack[2].power);
            drawSmallDigitsColored(panel->sx + 0x10, panel->sy - player * 24 + 0x37, text, 7, lineColors[5], z);
            drawTinyTextColored(panel->sx + 5, panel->sy - player * 44 + 0x42, CROSS_EFFECT_SHORT_NAMES[card->crossEffect], 7,
                                lineColors[6], z);
            if (CROSS_EFFECT_ICONS[card->crossEffect] != 0) {
                drawIcon(panel->sx + 0x37, panel->sy + 0x3D + player * -39, 0, CROSS_EFFECT_ICONS[card->crossEffect] + 0xE, z);
            }
            for (i = 0; i < 4; i++) {
                drawIconTextColored(panel->sx + 0x29, panel->sy + 9 + player * 30 + i * 12, 7, 1, lineColors[7], z,
                                    card->supportText[i]);
            }
            if (DUEL->cursorSlot >= 0 && DUEL->cursorSlot != 4) {
                if (PLAYER_PANEL(player, 6)->state == 0) {
                    PLAYER_PANEL(player, 6)->state = 1;
                }
            }
            break;
        case 1:
            option = (OptionCardData *)PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[(s16)(CUR_CARD % 30)].card;
            for (i = 0; i < 4; i++) {
                drawIconTextColored(panel->sx + 0x29, panel->sy + 9 + player * 30 + i * 12, 7, 1, lineColors[7], z,
                                    option->text[i]);
            }
            if (DUEL->cursorSlot >= 0 && DUEL->cursorSlot != 4) {
                if (PLAYER_PANEL(player, 7)->state == 0) {
                    PLAYER_PANEL(player, 7)->state = 1;
                }
            }
            break;
        case 2: {
            DigivolveCardData *digivolve;

            digivolve = (DigivolveCardData *)PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[(s16)(CUR_CARD % 30)].card;
            for (i = 0; i < 4; i++) {
                drawIconTextColored(panel->sx + 0x29, panel->sy + 9 + player * 30 + i * 12, 7, 1, lineColors[7], z,
                                    digivolve->text[i]);
            }
            if (DUEL->cursorSlot >= 0 && DUEL->cursorSlot != 4) {
                if (PLAYER_PANEL(player, 7)->state == 0) {
                    PLAYER_PANEL(player, 7)->state = 1;
                }
            }
            break;
        }
        }
        break;
    }
    case 6:
    case 17:
        if (DUEL->cursorSlot == 4) {
            if (panel->state == 3) {
                panel->state = 4;
            }
            break;
        }
        if (DUEL->cursorSlot == -1) {
            break;
        }
        switch (PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[(s16)(CUR_CARD % 30)].type) {
        case 0:
            DUEL->shownDigimon = CUR_CARD;
            break;
        case 1:
        case 2:
            if (panel->state == 3) {
                panel->state = 4;
            }
            break;
        }
        card = (DigimonCardData *)PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[(u8)(DUEL->shownDigimon % 30)].card;
        drawIconText(panel->sx + 0x13, panel->sy + 2, 7, 1, z, (s32)card->name);
        drawIcon(panel->sx + 0x86, panel->sy + 2, 0, card->attr >> 4, z);
        drawIcon(panel->sx + 0xB5, panel->sy + 2, 0, (card->attr & 0xF) + 0xB, z);
        if (card->supportIcon != 0) {
            drawIcon(panel->sx + 0xD2, panel->sy + 2, 0, card->supportIcon + 0xE, z);
        }
        break;
    case 7:
    case 18: {
        s8 *shown;

        if (DUEL->cursorSlot == 4) {
            if (panel->state == 3) {
                panel->state = 4;
            }
            break;
        }
        if (DUEL->cursorSlot == -1) {
            break;
        }
        option = (OptionCardData *)PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[(u8)(DUEL->shownOption % 30)].card;
        switch (PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[(s16)(CUR_CARD % 30)].type) {
        case 0:
            if (panel->state == 3) {
                panel->state = 4;
            }
            break;
        case 1:
            if (option->supportIcon != 0) {
                drawIcon(panel->sx + 0xA0, panel->sy + 2, 0, option->supportIcon + 0xE, z);
            }
        case 2:
            DUEL->shownOption = CUR_CARD;
            break;
        }
        shown = PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[(u8)(DUEL->shownOption % 30)].card;
        drawIconText(panel->sx + 0x13, panel->sy + 2, 7, 1, z, (s32)((OptionCardData *)shown)->name);
        drawIcon(panel->sx + 0x86, panel->sy + 2, 0, CARD_BYTE(shown, type) + 4, z);
        break;
    }
    case 22:
        drawMessageBarPanel(z);
        break;
    case 8:
    case 19: {
        s32 i;
        s32 isOpponent;

        card = (DigimonCardData *)PLAYER_CARDS(PLAYER(player))[getActiveDigimonCard(player) % 30].card;
        drawIconText(panel->sx + 0x16, panel->sy + 6, 6, 1, z, (s32)card->name);
        sprintf(text, "w-1%4d", PLAYER(player)->stats[0]);
        drawText(panel->sx + 0x8F, panel->sy + 6, (s32)text, 7, z);
        drawIconColored(panel->sx + 0xB2, panel->sy + 6, 0, (card->attr & 0xF) + 0xB, shades[0], z);
        drawIconColored(panel->sx + 0xC3, panel->sy + 6, 0, PLAYER(player)->specialty, shades[0], z);
        for (i = 0; i < 3; i++) {
            drawIconText(panel->sx + 0x25, panel->sy + 0x13 + i * 12, 7, 1, z, (s32)card->attack[i].name);
            sprintf(text, "w-1%4d", PLAYER(player)->baseAttackPowers[i]);
            drawText(panel->sx + 0x8F, panel->sy + 0x13 + i * 12, (s32)text, 7, z);
        }
        drawIconText(panel->sx + 0x2A, panel->sy + 0x36, 7, 1, z, (s32)CROSS_EFFECT_NAMES[card->crossEffect]);
        if (PLAYER(player)->attackChoice != 3) {
            if (PLAYER(player)->shownAttack != PLAYER(player)->attackChoice) {
                PLAYER(player)->attackHighlightTimer = 0;
            }
            PLAYER(player)->shownAttack = PLAYER(player)->attackChoice;
            panel->clut = 0x706F;
            if (PLAYER(player)->attackHighlightTimer < 60) {
                PLAYER(player)->attackHighlightTimer++;
                if ((PLAYER(player)->attackHighlightTimer / 10) & 1) {
                    panel->clut = 0x702F;
                }
            }
        } else {
            PLAYER(player)->shownAttack = 3;
            panel->clut = 0x702F;
        }
        isOpponent = DUEL->turnPlayer != player;
        drawTurnSideBadge(panel->sx + 0x89, panel->sy + 0x3E, isOpponent, 0x80, z);
        break;
    }
    case 9: {
        s16 choice;

        choice = PLAYER(player)->attackChoice;
        if (choice != 3) {
            if (PLAYER_DATA(0).skipBattleAnimation) {
                KAW_drawSprite(panel->sx + 0xB6, panel->sy + 4, 0x2F8, choice * 32 + 0x148, 0x20, 0x20, 0x2F0,
                               PLAYER(player)->attackChoice + 0x1C8, 0, 0, 0, 0x80, z);
            } else {
                KAW_drawSprite(panel->sx + 0xB6, panel->sy + 4, 0x2EC, 0x178, 0x20, 0x20, 0x2F0, 0x1C7, 0, 0, 0, 0x80, z);
            }
            KAW_drawSprite(panel->sx + 0xB0, panel->sy, 0x2EC, 0x150, 0x2C, 0x28, 0x2F0, 0x1C3, 0, 0, 0, 0x80, z);
        }
        drawBattleLog(player, panel->sx + 7, panel->sy + 9, z);
        break;
    }
    case 20: {
        s16 choice;

        choice = PLAYER(player)->attackChoice;
        if (choice != 3) {
            if (PLAYER_DATA(0).skipBattleAnimation) {
                KAW_drawSprite(panel->sx + 0xB6, panel->sy + 0x3B, 0x2F8, choice * 32 + 0x148, 0x20, 0x20, 0x2F0,
                               PLAYER(player)->attackChoice + 0x1C8, 0, 0, 0, 0x80, z);
            } else {
                KAW_drawSprite(panel->sx + 0xB6, panel->sy + 0x3B, 0x2EC, 0x178, 0x20, 0x20, 0x2F0, 0x1C7, 0, 0, 0, 0x80,
                               z);
            }
            KAW_drawSprite(panel->sx + 0xB0, panel->sy + 0x37, 0x2EC, 0x150, 0x2C, 0x28, 0x2F0, 0x1C3, 0, 0, 0, 0x80, z);
        }
        drawBattleLog(player, panel->sx + 7, panel->sy + 9, z);
        break;
    }
    }
}

/* the sprite of card c's animation and the panel P of the player */
#define ANIM CARD_ANIM(cardIndex)
#define PANEL_X(slot) PLAYER_PANEL(player, slot)->pos.vx
#define PANEL_Y(slot) PLAYER_PANEL(player, slot)->pos.vy

/* starts a move of frames from where the sprite is */
#define ANIM_START(frames)                                                                              \
    ANIM_SAVE(ANIM);                                                                                     \
    ANIM->total = (frames);                                                                              \
    ANIM->count = (frames)

/* places the sprite at x, y, turned rx, ry, rz and at scale sc */
#define ANIM_PLACE(x, y, rx, ry, rz, sc)                                                                \
    ANIM->spr->pos.vx = (x);                                                                             \
    ANIM->spr->pos.vy = (y);                                                                             \
    ANIM->spr->pos.vz = 0;                                                                               \
    ANIM->spr->rot.vx = (rx);                                                                            \
    ANIM->spr->rot.vy = (ry);                                                                            \
    ANIM->spr->rot.vz = (rz);                                                                            \
    ANIM->spr->scale = (sc)

/* Moves card sprite cardIndex to where its SPRITE_KIND state puts it, as
   us's does: 0-2 the Online Deck, 3-5 the hand, 6-8 the Offline Deck,
   9-13 the Digimon, 14-18 the played card, 19-23 the played card drawn
   from the Online Deck, 24-26 the DP slots, 27-32 the screen's centre */
void tickCardMotion(s32 cardIndex, s32 player) {

    switch (ANIM->state) {
    case 0:
        ANIM->spr->pos.vx = PANEL_X(3) + 0x1A;
        ANIM->spr->pos.vy = PANEL_Y(3) + 0x2F + player * -0x1F;
        ANIM->spr->pos.vz = 0;
        DUEL->sprites[cardIndex].rot.vx = 0x2000;
        DUEL->sprites[cardIndex].rot.vy = 0x2800;
        DUEL->sprites[cardIndex].rot.vz = 0x1C00;
        ANIM->spr->scale = 0x800;
        ANIM->spr->flags = PLAYER_PANEL(player, 3)->flags;
        break;
    case 1:
        ANIM_START(0x10);
        playSoundEffect(0xA5);
        ANIM->state++;
        break;
    case 2:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = PANEL_X(3) + 0x1A;
            targetY = PANEL_Y(3) + 0x2F + player * -0x1F;
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x1C00;
            scale = 0x800;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            ANIM->state = 0;
        }
        break;
    case 3:
        ANIM_START(0x10);
        playSoundEffect(0xA5);
        ANIM->state++;
    case 4:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;


            targetX = PANEL_X(3) + 0x41 + ANIM->handSlot * 0x2C;
            targetY = PANEL_Y(3) + 0x18 + player * 0xF;
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            ANIM->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 5:
        ANIM_PLACE(PANEL_X(3) + 0x41 + ANIM->handSlot * 0x2C, PANEL_Y(3) + 0x18 + player * 0xF, 0x2000, 0x2000, 0x2000,
                   0x1000);
        ANIM->spr->flags = PLAYER_PANEL(player, 3)->flags;
        break;
    case 6:
        ANIM_START(0x10);
        playSoundEffect(0xA5);
        ANIM->state++;
    case 7:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = PANEL_X(3) + 0x18;
            targetY = PANEL_Y(3) + 0x10 + player * 0x1E;
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2400;
            scale = 0x800;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            ANIM->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 8:
        ANIM_PLACE(PANEL_X(3) + 0x18, PANEL_Y(3) + 0x10 + player * 0x1E, 0x2000, 0x2000, 0x2400, 0x800);
        ANIM->spr->flags = PLAYER_PANEL(player, 3)->flags;
        break;
    case 9:
        ANIM_START(0x10);
        playSoundEffect(0xA5);
        ANIM->state++;
    case 10:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = PANEL_X(0) + 8;
            targetY = PANEL_Y(0) + 0x26 + player;
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            playSoundEffect(0xA7);
            ANIM->total = 4;
            ANIM->count = 4;
            ANIM->state++;
        }
        break;
    case 11:
        if (--ANIM->count == 0) {
            ANIM_START(4);
            ANIM->state++;
        }
        break;
    case 12:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = PANEL_X(0) + 0x14;
            targetY = PANEL_Y(0) + 0x26 + player;
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            ANIM->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 13:
        ANIM_PLACE(PANEL_X(0) + 0x14, PANEL_Y(0) + 0x26 + player, 0x2000, 0x2000, 0x2000, 0x1000);
        ANIM->spr->flags = PLAYER_PANEL(player, 0)->flags;
        break;
    case 14:
        ANIM_START(0x10);
        if (ANIM->spr->rot.vy == 0x2000) {
            playSoundEffect(0xA5);
        } else {
            playSoundEffect(0xA6);
        }
        ANIM->state++;
        break;
    case 15:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = PANEL_X(2) - 0x1D;
            targetY = PANEL_Y(2) + 0x1F + player * 5;
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x1C00;
            scale = 0x1000;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            playSoundEffect(0xA7);
            ANIM->total = 4;
            ANIM->count = 4;
            ANIM->state++;
        }
        break;
    case 16:
        if (--ANIM->count == 0) {
            ANIM_START(4);
            ANIM->state++;
        }
        break;
    case 17:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = PANEL_X(2) - 0x11;
            targetY = PANEL_Y(2) + 0x1F + player * 5;
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x1C00;
            scale = 0x1000;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            ANIM->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 18:
        ANIM_PLACE(PANEL_X(2) - 0x11, PANEL_Y(2) + 0x1F + player * 5, 0x2000, 0x2000, 0x1C00, 0x1000);
        ANIM->spr->flags = PLAYER_PANEL(player, 2)->flags;
        break;
    case 19:
        ANIM_START(0x10);
        playSoundEffect(0xA5);
        ANIM->state++;
        break;
    case 20:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = PANEL_X(2) - 0x1D;
            targetY = PANEL_Y(2) + 0x1F + player * 5;
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2400;
            scale = 0x1000;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            ANIM->total = 0x20;
            ANIM->count = 0x20;
            ANIM->state++;
        }
        break;
    case 21:
        if (ANIM->count != 0) {
            ANIM->count--;
            ANIM_START(4);
            ANIM->state++;
        }
        break;
    case 22:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = PANEL_X(2) - 0x11;
            targetY = PANEL_Y(2) + 0x1F + player * 5;
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2400;
            scale = 0x1000;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            ANIM->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 23:
        ANIM_PLACE(PANEL_X(2) - 0x11, PANEL_Y(2) + 0x1F + player * 5, 0x2000, 0x2800, 0x2400, 0x1000);
        ANIM->spr->flags = PLAYER_PANEL(player, 2)->flags;
        break;
    case 24:
        ANIM_START(0x10);
        playSoundEffect(0xA5);
        ANIM->state++;
        break;
    case 25:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = PANEL_X(1) + 0x35;
            targetY = PANEL_Y(1) + 0x29 + player * -0x1C;
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x1C00;
            scale = 0x800;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            ANIM->state++;
            PLAYER(player)->dpGainTimer = 100;
            playSoundEffect(0xA7);
        }
        break;
    case 26:
        ANIM_PLACE(PANEL_X(1) + 0x35, PANEL_Y(1) + 0x29 + player * -0x1C, 0x2000, 0x2000, 0x1C00, 0x800);
        ANIM->spr->flags = PLAYER_PANEL(player, 1)->flags;
        break;
    case 27:
        ANIM_START(0x20);
        ANIM->state++;
        break;
    case 28:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = 0;
            targetY = 0x3C - player * 0x78;
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x2000;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            ANIM->state++;
        }
        break;
    case 29:
        ANIM_PLACE(0, 0x3C - player * 0x78, 0x2000, 0x2000, 0x2000, 0x2000);
        break;
    case 30:
        ANIM_START(0x20);
        playSoundEffect(0xA6);
        ANIM->state++;
        break;
    case 31:
        if (ANIM->count != 0) {
            s16 targetX;
            s16 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = 0;
            targetY = 0xA0 - player * 0x140;
            rx = 0x2000;
            ry = 0x2800 - (player << 12);
            rz = 0x2000;
            scale = 0x2000;
            ANIM_STEP(ANIM, targetX, targetY, rx, ry, rz, scale);
        } else {
            ANIM->state++;
        }
        break;
    case 32:
        ANIM_PLACE(0, 0xA0 - player * 0x140, 0x2000, 0x2800 - (player << 12), 0x2000, 0x2000);
        break;
    }
}

void renderBoardCards(void) {
    s32 i;
    s32 j;
    s32 card;
    s32 labelled;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 30; j++) {
            tickCardMotion(i * 30 + j, i);
        }
    }
    resetCardPolyCount();
    for (i = 0; i < 2; i++) {
        for (j = 0; j < DP_SLOT_COUNT; j++) {
            card = PLAYER(i)->dpSlots[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), 0x64);
                if (SPRITE_KIND(card) == 0x1A) {
                    break;
                }
            }
        }
        card = PLAYER(i)->playedCard;
        if (card >= 0) {
            renderCardSprite(SPRITE(card), 0x65);
        }
        labelled = 0;
        for (j = 0; j < 3; j++) {
            card = PLAYER(i)->digimonStack[j];
            if (card >= 0) {
                if (!labelled) {
                    SPRITE(card)->pal = PLAYER(i)->specialty;
                    labelled = 1;
                }
                renderCardSprite(SPRITE(card), 0x66);
                if (SPRITE_KIND(card) == 0xD) {
                    break;
                }
            }
        }
        for (j = 0; j < 30; j++) {
            card = PLAYER(i)->offlineDeck[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), 0x67);
                if (SPRITE_KIND(card) == 8) {
                    break;
                }
            }
        }
        for (j = 3; j >= 0; j--) {
            card = PLAYER(i)->hand[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), 0x68);
            }
        }
        for (j = 0; j < 30; j++) {
            card = PLAYER(i)->onlineDeck[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), 0x69);
                if (SPRITE_KIND(card) == 0) {
                    break;
                }
            }
        }
    }
}
