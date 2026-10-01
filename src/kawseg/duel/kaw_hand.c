#include "common.h"
#include "game.h"
#include "dcb/kaw_hand.h"
#include "dcb/card_zones.h"
#include "dcb/text.h"
#include "dcb/duel_util.h"
#include "dcb/duel_launch.h"
#include "dcb/battle_hud.h"
#include "dcb/card_db.h"
#include "dcb/card_render.h"
#include "dcb/sound_play.h"
#include "dcb/libmath.h"
#include "dcb/pad.h"
#include "dcb/kaw_battle_sim.h"

void KAW_pickCardArtSlot(void) {
    s32 id;
    s32 i;

    id = PLAYER(DUEL->cursorPlayer)->cards[((CardCursor *)DUEL->cursor)->id % 30].index;
    switch (PLAYER(DUEL->cursorPlayer)->cards[((CardCursor *)DUEL->cursor)->id % 30].type) {
    case 1:
        id += 0xBF;
        break;
    case 2:
        id += 0x125;
        break;
    case 0:
        break;
    }
    for (i = 0; i < 6; i++) {
        if (DUEL->cache[i].id == id) {
            DUEL->artSlot = i;
            return;
        }
    }
    for (i = 0; i < 6; i++) {
        if (DUEL->cache[i].id == -1) {
            DUEL->artSlot = i;
            DUEL->cache[i].used = 0;
            return;
        }
    }
    id = 100;
    for (i = 0; i < 6; i++) {
        if (DUEL->cache[i].age < id) {
            id = DUEL->cache[i].age;
        }
    }
    for (i = 0; i < 6; i++) {
        if (DUEL->cache[i].age == id) {
            DUEL->artSlot = i;
            return;
        }
    }
    DUEL->artSlot = (DUEL->artSlot + 1) % 6;
}

#define DRAW_HAND_MARK(u)                                                                               \
    rect.x = (u);                                                                                        \
    rect.y = player * 24 + 0x30;                                                                         \
    rect.w = 0x1B;                                                                                       \
    rect.h = 0x18;                                                                                       \
    drawPageSprite(((CardAnim *)(CARD_ANIMS + card * 36))->spr->sx + dx,                                 \
                   ((CardAnim *)(CARD_ANIMS + card * 36))->spr->sy + dy, (s32)&rect,                     \
                   getTPage(0, 2, SYSTEM_TEX_X, SYSTEM_TEX_Y), 0xC, 0x32)

s32 KAW_drawHandHints(s32 player) {
    Rect16 rect;
    s32 dx;
    s32 dy;
    s32 i;
    s32 card;
    s32 level;
    s32 cardLevel;

    player %= 2;
    if (player == 0) {
        dx = 0x16;
        dy = -12;
    } else {
        dx = -12;
        dy = 0x24;
    }
    getActiveDigimonCard(player);
    for (i = 0; i < 4; i++) {
        card = PLAYER(player)->hand[i];
        if (card == -1) {
            continue;
        }
        switch (KAW_DUEL->cursorMode) {
        case 1:
            if (PLAYER(player)->cards[card % 30].type != 0) {
                break;
            }
            cardLevel = (u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF;
            level = cardLevel - 1;
            if (level < 0) {
                level = 0;
            }
            DRAW_HAND_MARK(level * 28 + 0x80);
            break;
        case 2:
            if (PLAYER(player)->cards[card % 30].type != 0) {
                break;
            }
            DRAW_HAND_MARK(0x80);
            break;
        case 3:
        case 6:
            if (PLAYER(player)->cards[card % 30].type != 0) {
                break;
            }
            if (KAW_checkDigivolveTarget(card, player) != 0) {
                break;
            }
            DRAW_HAND_MARK(0x80);
            break;
        case 4:
            if (PLAYER(player)->cards[card % 30].type == 2) {
                break;
            }
            DRAW_HAND_MARK(0x80);
            break;
        case 5:
            if (PLAYER(player)->cards[card % 30].type != 2) {
                break;
            }
            DRAW_HAND_MARK(0x80);
            break;
        }
    }
    if (KAW_DUEL->cursorMode == 4) {
        card = peekOnlineDeckTop(player);
        if (card != -1) {
            if (player == 0) {
                dx = 8;
                dy = -36;
            } else {
                dx = -10;
                dy = -6;
            }
            DRAW_HAND_MARK(0x80);
        }
    }
}

s32 KAW_tickCardCursor(s32 player, s32 mode) {
    s32 i;
    s32 p;
    s32 card;
    s32 x;
    s32 y;
    s32 cx;
    s32 cy;
    s32 dist;
    s32 best;
    s32 cur;
    CardSprite *spr;

    KAW_DUEL->cursorMode = mode;
    for (i = 0; i < 2; i++) {
        PLAYER(i)->topCards[0] = peekOnlineDeckTop(i);
        PLAYER(i)->topCards[1] = peekOfflineDeckTop(i);
        PLAYER(i)->topCards[2] = getActiveDigimonCard(i);
        PLAYER(i)->topCards[3] = getPlayedCard(i);
        PLAYER(i)->topCards[4] = peekDpSlotTop(i);
    }
    if (DUEL->cursorSlot == -1) {
        DUEL->cursorPlayer = player;
        for (i = 0; i < 9; i++) {
            if (PLAYER(player)->hand[i] != -1) {
                ((CardCursor *)DUEL->cursor)->id = PLAYER(player)->hand[i];
                DUEL->cursorSlot = i;
                break;
            }
        }
    }
    cur = ((CardCursor *)DUEL->cursor)->id;
    best = 0x280;
    if (PAD_STATES[player]->repeat & PAD_UP) {
        cx = CARD_SPR(cur)->pos.vx;
        spr = CARD_SPR(cur);
        cy = spr->pos.vy - spr->scale * 24 / 4096;
        for (p = 0; p < 2; p++) {
            for (i = 0; i < 9; i++) {
                card = PLAYER(p)->hand[i];
                if (card != -1) {
                    x = CARD_SPR(card)->pos.vx;
                    y = CARD_SPR(card)->pos.vy;
                    if (y < cy) {
                        dist = sqrtDouble((double)((abs(x - cx) ^ 2) + (abs(y - cy) ^ 2)));
                        if (dist <= best) {
                            best = dist;
                            ((CardCursor *)DUEL->cursor)->id = card;
                            DUEL->cursorSlot = i;
                            DUEL->cursorPlayer = p;
                        }
                    }
                }
            }
        }
    } else if (PAD_STATES[player]->repeat & PAD_DOWN) {
        cx = CARD_SPR(cur)->pos.vx;
        spr = CARD_SPR(cur);
        cy = spr->pos.vy + spr->scale * 24 / 4096;
        for (p = 0; p < 2; p++) {
            for (i = 0; i < 9; i++) {
                card = PLAYER(p)->hand[i];
                if (card != -1) {
                    x = CARD_SPR(card)->pos.vx;
                    y = CARD_SPR(card)->pos.vy;
                    if (cy < y) {
                        dist = sqrtDouble((double)((abs(x - cx) ^ 2) + (abs(y - cy) ^ 2)));
                        if (dist <= best) {
                            best = dist;
                            ((CardCursor *)DUEL->cursor)->id = card;
                            DUEL->cursorSlot = i;
                            DUEL->cursorPlayer = p;
                        }
                    }
                }
            }
        }
    } else if ((u16)PAD_STATES[player]->repeat & PAD_LEFT) {
        spr = CARD_SPR(cur);
        cx = spr->pos.vx - spr->scale * 20 / 4096;
        cy = CARD_SPR(cur)->pos.vy;
        for (p = 0; p < 2; p++) {
            for (i = 0; i < 9; i++) {
                card = PLAYER(p)->hand[i];
                if (card != -1) {
                    x = CARD_SPR(card)->pos.vx;
                    y = CARD_SPR(card)->pos.vy;
                    if (x < cx) {
                        dist = sqrtDouble((double)((abs(x - cx) ^ 2) + ((abs(y - cy) / 48 * 480) ^ 2)));
                        if (dist <= best) {
                            best = dist;
                            ((CardCursor *)DUEL->cursor)->id = card;
                            DUEL->cursorSlot = i;
                            DUEL->cursorPlayer = p;
                        }
                    }
                }
            }
        }
    } else if (PAD_STATES[player]->repeat & PAD_RIGHT) {
        spr = CARD_SPR(cur);
        cx = spr->pos.vx + spr->scale * 20 / 4096;
        cy = CARD_SPR(cur)->pos.vy;
        for (p = 0; p < 2; p++) {
            for (i = 0; i < 9; i++) {
                card = PLAYER(p)->hand[i];
                if (card != -1) {
                    x = CARD_SPR(card)->pos.vx;
                    y = CARD_SPR(card)->pos.vy;
                    if (cx < x) {
                        dist = sqrtDouble((double)((abs(x - cx) ^ 2) + ((abs(y - cy) / 48 * 480) ^ 2)));
                        if (dist <= best) {
                            best = dist;
                            ((CardCursor *)DUEL->cursor)->id = card;
                            DUEL->cursorSlot = i;
                            DUEL->cursorPlayer = p;
                        }
                    }
                }
            }
        }
    }
    if (cur != ((CardCursor *)DUEL->cursor)->id) {
        playSoundEffect(0xA2);
    }
    if (DUEL->cursorSlot < 6) {
        if (DUEL->cursorPlayer == 0) {
            if (HUD_PANELS[player * 0xD8 + 0xD] == 4) {
                HUD_PANELS[player * 0xD8 + 0xD] = 1;
            }
        } else {
            if (HUD_PANELS[player * 0xD8 + 0xD] == 4) {
                HUD_PANELS[player * 0xD8 + 0xD] = 2;
            }
        }
    }
    KAW_pickCardArtSlot();
    if (DUEL->cursorPlayer != player) {
        return -1;
    }
    if (mode == 4) {
        if (DUEL->cursorSlot >= 5) {
            return -1;
        }
    } else if (DUEL->cursorSlot >= 4) {
        return -1;
    }
    if (PAD_STATES[player]->pressed & PAD_CROSS) {
        return 0;
    }
    return -1;
}

s32 KAW_openCardSelect(s32 player) {
    MSG_BAR_PLAYER_LABEL = (*(u32 *)(DUEL_PLAYERS[player] + 0x178) >> 17) & 3;
    HUD_PANELS[player * 0xD8 + 0xD] = player + 1;
}

s32 KAW_closeCardSelect(s32 index) {
    DUEL->cursorSlot = -1;
    KAW_DUEL->cursorMode = -1;
    HUD_PANELS[index * 0xD8 + 0xD] = 5;
}

s32 KAW_drawCardToHand(s32 player) {
    s32 card;
    s32 slot;

    if (countOnlineDeckCards(player) == 0) {
        return -1;
    }
    if (countEmptyHandSlots(player) == 0) {
        return -1;
    }
    card = drawOnlineDeckCard(player);
    slot = addCardToHand(card, player);
    SPRITE_KIND(card) = 3;
    CARD_ANIM(card)->handSlot = slot;
    return card;
}

s32 KAW_discardCard(s32 card, s32 player) {
    if (removeCardFromHand(card, player) != -1) {
        SPRITE_KIND(card) = 8;
    } else if (removeCardFromDigimonStack(card, player) != -1) {
        SPRITE_KIND(card) = 8;
    }
    CARD_SPR(card)->pal = (u8)((Player *)DUEL_PLAYERS[player])->cards[card % 30].card[0x1A] >> 4;
    discardCardToOfflineDeck(card, player);
}

s32 KAW_discardHand(s32 player) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (((Player *)DUEL_PLAYERS[player])->hand[i] != -1) {
            SPRITE_KIND(((Player *)DUEL_PLAYERS[player])->hand[i]) = 8;
            discardCardToOfflineDeck(((Player *)DUEL_PLAYERS[player])->hand[i], player);
            removeCardFromHand(((Player *)DUEL_PLAYERS[player])->hand[i], player);
        }
    }
}

s32 KAW_placeDigimonFromHand(s32 card, s32 player) {
    s32 result;

    result = -1;
    if (countEmptyDigimonStackSlots(player)) {
        result = removeCardFromHand(card, player);
        if (result != -1) {
            SPRITE_KIND(card) = 11;
            placeActiveDigimon(card, player);
        }
    }
    return result;
}

s32 KAW_returnDigimonToHand(s32 card, s32 player, s32 slot) {
    if (removeCardFromDigimonStack(card, player) != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        CARD_ANIM(card)->handSlot = slot;
    }
}

void KAW_returnPlayedCard(s32 player, s32 slot) {
    s32 card;

    card = takePlayedCard(player);
    if (card != -1) {
        if (slot != 4) {
            SPRITE_KIND(card) = 3;
            ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
            CARD_ANIM(card)->handSlot = slot;
        } else {
            SPRITE_KIND(card) = 1;
            returnCardToOnlineDeck(card, player);
        }
    }
}

s32 KAW_returnDiscardToHand(s32 player, s32 slot) {
    s32 card;

    card = takeOfflineDeckTopCard(player);
    if (card != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        CARD_ANIM(card)->handSlot = slot;
    }
}

s32 KAW_returnDpCardToHand(s32 card, s32 player, s32 slot) {
    if (removeCardFromDpSlots(card, player) != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        CARD_ANIM(card)->handSlot = slot;
    }
}

s32 KAW_playOnlineDeckTop(s32 player) {
    s32 card;

    if (countOnlineDeckCards(player) && isPlayedCardSlotEmpty(player)) {
        card = drawOnlineDeckCard(player);
        setPlayedCard(card, player);
        SPRITE_KIND(card) = 21;
    }
}

s32 KAW_playCardFromHand(s32 card, s32 player) {
    s32 result;

    result = -1;
    if (isPlayedCardSlotEmpty(player)) {
        result = removeCardFromHand(card, player);
        if (result != -1) {
            SPRITE_KIND(card) = 16;
            setPlayedCard(card, player);
        }
    }
    return result;
}

s32 KAW_chargeDpCard(s32 card, s32 player) {
    s32 result;

    result = -1;
    if (countEmptyDpSlots(player)) {
        result = removeCardFromHand(card, player);
        if (result != -1) {
            SPRITE_KIND(card) = 26;
            addCardToDpSlots(card, player);
        }
    }
    return result;
}

s32 KAW_redrawHand(s32 player) {
    s32 i;

    while (waitFrames(20), countEmptyHandSlots(player) != 4) {
        for (i = 0; i < 4; i++) {
            if (((Player *)DUEL_PLAYERS[player])->hand[i] != -1) {
                SPRITE_KIND(((Player *)DUEL_PLAYERS[player])->hand[i]) = 8;
                discardCardToOfflineDeck(((Player *)DUEL_PLAYERS[player])->hand[i], player);
                removeCardFromHand(((Player *)DUEL_PLAYERS[player])->hand[i], player);
                break;
            }
        }
    }
    while (KAW_drawCardToHand(player) != -1) {
        waitFrames(20);
        KAW_checkHandBonuses(player);
    }
}

s32 KAW_undoDigivolve(void) {
    if (DUEL->discardedFromSlot >= 0) {
        KAW_returnDiscardToHand(DUEL->turnPlayer, DUEL->discardedFromSlot);
        DUEL->discardedFromSlot = -1;
    }
    waitDuelFrames(30);
    if (DUEL->dpFromSlot >= 0) {
        KAW_returnDpCardToHand(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer, DUEL->dpFromSlot);
        DUEL->dpFromSlot = -1;
    }
    waitDuelFrames(30);
    DUEL->step = 11;
}

s32 KAW_discardDpSlots(void) {
    while (peekDpSlotTop(DUEL->turnPlayer) != -1) {
        discardCardToOfflineDeck(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer);
        SPRITE_KIND(peekDpSlotTop(DUEL->turnPlayer)) = 8;
        removeCardFromDpSlots(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer);
        waitDuelFrames(20);
    }
    waitDuelFrames(30);
    DUEL->step = 23;
}

s32 KAW_checkKnockout(s32 player) {
    s32 opponent;
    s32 card;
    s32 slot;
    u8 *data;

    opponent = player ^ 1;
    if (PLAYER(player)->stats[0] == 0) {
        if (((u8)PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card[0x1A] & 0xF) == 3) {
            KAW_showBonusBanner(opponent, 0xF);
        }
        DUEL->winner = opponent;
        if (((*(u32 *)((u8 *)PLAYER(player) + 0x178) >> 14) & 1) && PLAYER(opponent)->wins != 2) {
            KAW_playEffect(0x1D, player);
            showStatChangePopup(player, PLAYER(player)->reviveHp, 0);
            PLAYER(player)->stats[0] = PLAYER(player)->reviveHp;
            waitForStatCountersToSettle();
        } else {
            data = DUEL_PLAYERS[opponent];
            data += (getActiveDigimonCard(opponent) % 30) * sizeof(CardSlot);
            card = ((Player *)data)->cards[0].index;
            if (++PLAYER_DATA(opponent).cardWins[card] >= 1000) {
                PLAYER_DATA(opponent).cardWins[card] = 999;
            }
            data = DUEL_PLAYERS[player];
            data += (getActiveDigimonCard(player) % 30) * sizeof(CardSlot);
            card = ((Player *)data)->cards[0].index;
            if (++PLAYER_DATA(player).cardLosses[card] >= 1000) {
                PLAYER_DATA(player).cardLosses[card] = 999;
            }
            data = DUEL_PLAYERS[player];
            data += (getActiveDigimonCard(player) % 30) * sizeof(CardSlot);
            slot = findArmorPartnerSlot(player, ((Player *)data)->cards[0].id);
            if (slot != -1) {
                KAW_playEffect(0x1E, player);
                armorDevolvePartner(player, slot);
            }
            while (getActiveDigimonCard(player) != -1) {
                card = getActiveDigimonCard(player);
                CARD_SPR(card)->pal = (u8)PLAYER(player)->cards[card % 30].card[0x1A] >> 4;
                KAW_discardCard(card, player);
                waitDuelFrames(20);
            }
        }
        PLAYER(opponent)->wins++;
        return 1;
    }
    return 0;
}

void KAW_drawCard3D(Icon3D *icon, s32 z, RawPolyFT4 *pk) {
    MATRIX matrix;
    SVECTOR vertices[4];
    s32 sxy[4];
    s32 depthCue;
    s32 otz;
    s32 flag;
    u32 *col;
    u16 clut;
    u16 tpage;
    u8 u0, v0, u1, v1, u2, v2, u3, v3;

    PushMatrix();
    buildRotTransMatrix(&icon->pos, &icon->rot, &matrix);
    CompMatrix((MATRIX *)((u8 *)SCENE_3D + 0x78), &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    SetTransMatrix(&matrix);
    vertices[0].vx = -20;
    vertices[0].vy = -24;
    vertices[0].vz = 0;
    vertices[1].vx = 20;
    vertices[1].vy = -24;
    vertices[1].vz = 0;
    vertices[2].vx = -20;
    vertices[2].vy = 24;
    vertices[2].vz = 0;
    vertices[3].vx = 20;
    vertices[3].vy = 24;
    vertices[3].vz = 0;
    col = (u32 *)&icon->r0;
    if (RotAverageNclip4((s32)&vertices[0], (s32)&vertices[1], (s32)&vertices[2], (s32)&vertices[3], (s32)&sxy[0], (s32)&sxy[1],
                         (s32)&sxy[2], (s32)&sxy[3], &depthCue, &otz, &flag) <= 0) {
        otz = RotAverage4(&vertices[1], &vertices[0], &vertices[3], &vertices[2], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
        tpage = 0x1C;
        clut = 0x7C32;
        u0 = 0xA0;
        v0 = 0x6F;
        u1 = 0xC8;
        v1 = 0x6F;
        u2 = 0xA0;
        v2 = 0x9F;
        u3 = 0xC8;
        v3 = 0x9F;
    } else {
        tpage = icon->tpage;
        clut = icon->clut;
        u0 = icon->u;
        v0 = icon->v;
        u1 = icon->u + 40;
        v1 = icon->v;
        u2 = icon->u;
        v2 = icon->v + 48;
        u3 = u1;
        v3 = v2;
    }
    pk->tag = 0x09000000;
    pk->rgbc = *col;
    pk->xy0 = sxy[0];
    pk->uv0 = (clut << 16) | (v0 << 8) | u0;
    pk->xy1 = sxy[1];
    pk->uv1 = (tpage << 16) | (v1 << 8) | u1;
    pk->xy2 = sxy[2];
    pk->uv2 = (v2 << 8) | u2;
    pk->xy3 = sxy[3];
    pk->uv3 = (v3 << 8) | u3;
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], pk);
    PopMatrix();
}

void KAW_waitForCross(void) {
    do {
        waitFrames(FRAME_INTERVAL);
    } while (!(PAD_STATES[0]->pressed & PAD_CROSS));
}

void KAW_drawSprite(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 clutX, s32 clutY, s32 tp, s32 semi, s32 abr, s32 brightness,
                   s32 otz) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = (u % 64) * (4 >> tp);
        CUR_SPRT->sp.v0 = v % 256;
        CUR_SPRT->sp.clut = getClut(clutX, clutY);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, semi);
        CUR_SPRT->sp.r0 = brightness;
        CUR_SPRT->sp.g0 = brightness;
        CUR_SPRT->sp.b0 = brightness;
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(tp, abr, u, v));
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}
