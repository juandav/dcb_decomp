#include "common.h"
#include "game.h"
#include "dcb/card_zones.h"
#include "dcb/text.h"
#include "dcb/duel_util.h"
#include "dcb/battle_hud.h"
#include "dcb/sound_play.h"
#include "dcb/task.h"
#include "dcb/pad.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_battle_sim.h"
#include "dcb/window.h"

/* jp's hand and card cursor (kaw_hand.c is us's and eu's): the cursor
   steps through a table of the neighbouring slots instead of measuring the
   distances between the cards, select opens four pages of help on the
   special attacks and start a Give Up prompt (both kept here, where us has
   the card select's HUD panels), and a knocked out Digimon goes to the
   Offline Deck without the armor or the level 3 bonus checks */

/* the window frame drawn around a text box */
extern CVECTOR D_801FF6D8[];
/* frames before the Give Up prompt takes a button */
extern s32 D_801FFB1C;

void KAW_pickCardArtSlot(void) {
    s32 id;
    s32 i;

    id = PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[((CardCursor *)DUEL->cursor)->id % 30].index;
    switch (PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[((CardCursor *)DUEL->cursor)->id % 30].type) {
    case 1:
        id += 0x6E;
        break;
    case 2:
        id += 0x99;
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

/* marks a hand card: the u of its mark in the system texture */
#define DRAW_HAND_MARK(u)                                                                               \
    rect.x = (u);                                                                                        \
    rect.y = 0xB8;                                                                                       \
    rect.w = 0x1C;                                                                                       \
    rect.h = 0x18;                                                                                       \
    drawPageSprite(CARD_SPR(card)->sx + dx, CARD_SPR(card)->sy + dy, (s32)&rect,                         \
                   getTPage(0, 2, SYSTEM_TEX_X, SYSTEM_TEX_Y), 0xC, 0)

/* marks the hand cards that the card cursor's mode offers: the level of a
   Digimon card to place (mode 1), else a plain mark */
void KAW_drawHandHints(s32 player, s32 mode) {
    Rect16 rect;
    s32 dx;
    s32 dy;
    s32 i;
    s32 card;

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
        switch (mode) {
        case 1:
            if (PLAYER_CARDS(PLAYER(player))[card % 30].type != 0) {
                break;
            }
            DRAW_HAND_MARK(((CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF) + player * 3) * 28);
            break;
        case 2:
            if (PLAYER_CARDS(PLAYER(player))[card % 30].type != 0) {
                break;
            }
            DRAW_HAND_MARK(player * 3 * 28);
            break;
        case 3:
            if (PLAYER_CARDS(PLAYER(player))[card % 30].type != 0) {
                break;
            }
            if (KAW_checkDigivolveTarget(card, player) != 0) {
                break;
            }
            DRAW_HAND_MARK(player * 3 * 28);
            break;
        case 4:
            if (PLAYER_CARDS(PLAYER(player))[card % 30].type == 2) {
                break;
            }
            DRAW_HAND_MARK(player * 3 * 28);
            break;
        case 5:
            if (PLAYER_CARDS(PLAYER(player))[card % 30].type != 2) {
                break;
            }
            DRAW_HAND_MARK(player * 3 * 28);
            break;
        }
    }
}

/* for each player's nine card slots (the hand, then the Online Deck, the
   Offline Deck, the Digimon, the played card and the DP slots): the slot
   up, down, left and right; -1: the nearest card in that direction, -2: the
   same slot on the other player's side */
s8 KAW_CURSOR_NEIGHBOURS[2][9][4] = {
    {
        { -2, 7, 4, 1 },
        { -2, 7, 0, 2 },
        { -2, 7, 1, 3 },
        { -2, 7, 2, 7 },
        { 5, -1, -1, 0 },
        { -2, 4, -1, 0 },
        { -2, 8, 7, 8 },
        { 3, -1, 3, 6 },
        { 6, -1, 6, -1 },
    },
    {
        { 7, -2, 4, 1 },
        { 7, -2, 0, 2 },
        { 7, -2, 1, 3 },
        { 7, -2, 2, 7 },
        { -1, 5, -1, 0 },
        { 4, -2, -1, 0 },
        { 8, -2, 7, 8 },
        { -1, 3, 3, 6 },
        { -1, 6, 6, -1 },
    },
};

/* the help pages on the special attacks, the last first */
char *KAW_HELP_PAGES[4] = {
    /* zc6      b2特殊攻撃の説明\nh-6\nzc7h0c5d3「ぼうがい」\nc7 相手の援護効果を無効化する。\n
       オプションの効果は無効化できない。\nc5d2「先制（せんせい）」\nc7 自分が後攻なら、先攻になる。\n
       自分が先攻なら、相手の先制を無効化する。\nc5d2「すいとる」\nc7 相手に与えたダメージと同じ数だけ、自分の\n
       ＨＰを回復する。\nc5d1「自爆（じばく）」\nc7 自分の攻撃力は自分のＨＰと同じになり、\n
       自分のＨＰは１０になる。\n 自分の攻撃寸前まで効果は現れない。 */
    "zc6      b2\x93\xC1\x8E\xEA\x8DU\x8C\x82\x82\xCC\x90\xE0\x96\xBE\nh-6\nzc7h0c5d3\x81u\x82\xDA\x82\xA4\x82\xAA"
    "\x82\xA2\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC\x89\x87\x8C\xEC\x8C\xF8\x89\xCA\x82\xF0\x96\xB3\x8C\xF8\x89\xBB"
    "\x82\xB7\x82\xE9\x81"
    "B\n \x83I\x83v\x83V\x83\x87\x83\x93\x82\xCC\x8C\xF8\x89\xCA\x82\xCD\x96\xB3\x8C\xF8\x89\xBB\x82\xC5\x82\xAB"
    "\x82\xC8\x82\xA2\x81"
    "B\nc5d2\x81u\x90\xE6\x90\xA7\x81i\x82\xB9\x82\xF1\x82\xB9\x82\xA2\x81j\x81v\nc7 \x8E\xA9\x95\xAA\x82\xAA\x8C"
    "\xE3\x8DU\x82\xC8\x82\xE7\x81"
    "A\x90\xE6\x8DU\x82\xC9\x82\xC8\x82\xE9\x81"
    "B\n \x8E\xA9\x95\xAA\x82\xAA\x90\xE6\x8DU\x82\xC8\x82\xE7\x81"
    "A\x91\x8A\x8E\xE8\x82\xCC\x90\xE6\x90\xA7\x82\xF0\x96\xB3\x8C\xF8\x89\xBB\x82\xB7\x82\xE9\x81"
    "B\nc5d2\x81u\x82\xB7\x82\xA2\x82\xC6\x82\xE9\x81v\nc7 \x91\x8A\x8E\xE8\x82\xC9\x97^\x82\xA6\x82\xBD\x83_\x83"
    "\x81\x81[\x83W\x82\xC6\x93\xAF\x82\xB6\x90\x94\x82\xBE\x82\xAF\x81"
    "A\x8E\xA9\x95\xAA\x82\xCC\n \x82g\x82o\x82\xF0\x89\xF1\x95\x9C\x82\xB7\x82\xE9\x81"
    "B\nc5d1\x81u\x8E\xA9\x94\x9A\x81i\x82\xB6\x82\xCE\x82\xAD\x81j\x81v\nc7 \x8E\xA9\x95\xAA\x82\xCC\x8DU\x8C\x82"
    "\x97\xCD\x82\xCD\x8E\xA9\x95\xAA\x82\xCC\x82g\x82o\x82\xC6\x93\xAF\x82\xB6\x82\xC9\x82\xC8\x82\xE8\x81"
    "A\n \x8E\xA9\x95\xAA\x82\xCC\x82g\x82o\x82\xCD\x82P\x82O\x82\xC9\x82\xC8\x82\xE9\x81"
    "B\n \x8E\xA9\x95\xAA\x82\xCC\x8DU\x8C\x82\x90\xA1\x91O\x82\xDC\x82\xC5\x8C\xF8\x89\xCA\x82\xCD\x8C\xBB\x82\xEA"
    "\x82\xC8\x82\xA2\x81"
    "B",
    /* c5d1「対a0×３」\nc7 相手の属性がa0なら、自分のb2攻撃力は\n ３倍になる。\n... (a1 to a4 alike) */
    "c5d1\x81u\x91\xCE"
    "a0\x81~\x82R\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC\x91\xAE\x90\xAB\x82\xAA"
    "a0\x82\xC8\x82\xE7\x81"
    "A\x8E\xA9\x95\xAA\x82\xCC"
    "b2\x8DU\x8C\x82\x97\xCD\x82\xCD\n \x82R\x94{\x82\xC9\x82\xC8\x82\xE9\x81"
    "B\nc5d1\x81u\x91\xCE"
    "a1\x81~\x82R\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC\x91\xAE\x90\xAB\x82\xAA"
    "a1\x82\xC8\x82\xE7\x81"
    "A\x8E\xA9\x95\xAA\x82\xCC"
    "b2\x8DU\x8C\x82\x97\xCD\x82\xCD\n \x82R\x94{\x82\xC9\x82\xC8\x82\xE9\x81"
    "B\nc5d1\x81u\x91\xCE"
    "a2\x81~\x82R\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC\x91\xAE\x90\xAB\x82\xAA"
    "a2\x82\xC8\x82\xE7\x81"
    "A\x8E\xA9\x95\xAA\x82\xCC"
    "b2\x8DU\x8C\x82\x97\xCD\x82\xCD\n \x82R\x94{\x82\xC9\x82\xC8\x82\xE9\x81"
    "B\nc5d1\x81u\x91\xCE"
    "a3\x81~\x82R\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC\x91\xAE\x90\xAB\x82\xAA"
    "a3\x82\xC8\x82\xE7\x81"
    "A\x8E\xA9\x95\xAA\x82\xCC"
    "b2\x8DU\x8C\x82\x97\xCD\x82\xCD\n \x82R\x94{\x82\xC9\x82\xC8\x82\xE9\x81"
    "B\nc5d1\x81u\x91\xCE"
    "a4\x81~\x82R\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC\x91\xAE\x90\xAB\x82\xAA"
    "a4\x82\xC8\x82\xE7\x81"
    "A\x8E\xA9\x95\xAA\x82\xCC"
    "b2\x8DU\x8C\x82\x97\xCD\x82\xCD\n \x82R\x94{\x82\xC9\x82\xC8\x82\xE9\x81"
    "B",
    /* c5d1「b0カウンター」\nc7 相手の攻撃ボタンがb0なら、自分は後攻に\n なり、相手のb0攻撃はミスになり、相手の\n
       b0攻撃力で反撃する。\n... (b1 and b2 alike) */
    "c5d1\x81ub0\x83J\x83"
    "E\x83\x93\x83^\x81[\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC\x8DU\x8C\x82\x83{\x83^\x83\x93\x82\xAA"
    "b0\x82\xC8\x82\xE7\x81"
    "A\x8E\xA9\x95\xAA\x82\xCD\x8C\xE3\x8DU\x82\xC9\n \x82\xC8\x82\xE8\x81"
    "A\x91\x8A\x8E\xE8\x82\xCC"
    "b0\x8DU\x8C\x82\x82\xCD\x83~\x83X\x82\xC9\x82\xC8\x82\xE8\x81"
    "A\x91\x8A\x8E\xE8\x82\xCC\n b0\x8DU\x8C\x82\x97\xCD\x82\xC5\x94\xBD\x8C\x82\x82\xB7\x82\xE9\x81"
    "B\nc5d1\x81ub1\x83J\x83"
    "E\x83\x93\x83^\x81[\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC\x8DU\x8C\x82\x83{\x83^\x83\x93\x82\xAA"
    "b1\x82\xC8\x82\xE7\x81"
    "A\x8E\xA9\x95\xAA\x82\xCD\x8C\xE3\x8DU\x82\xC9\n \x82\xC8\x82\xE8\x81"
    "A\x91\x8A\x8E\xE8\x82\xCC"
    "b1\x8DU\x8C\x82\x82\xCD\x83~\x83X\x82\xC9\x82\xC8\x82\xE8\x81"
    "A\x91\x8A\x8E\xE8\x82\xCC\n b1\x8DU\x8C\x82\x97\xCD\x82\xC5\x94\xBD\x8C\x82\x82\xB7\x82\xE9\x81"
    "B\nc5d1\x81ub2\x83J\x83"
    "E\x83\x93\x83^\x81[\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC\x8DU\x8C\x82\x83{\x83^\x83\x93\x82\xAA"
    "b2\x82\xC8\x82\xE7\x81"
    "A\x8E\xA9\x95\xAA\x82\xCD\x8C\xE3\x8DU\x82\xC9\n \x82\xC8\x82\xE8\x81"
    "A\x91\x8A\x8E\xE8\x82\xCC"
    "b2\x8DU\x8C\x82\x82\xCD\x83~\x83X\x82\xC9\x82\xC8\x82\xE8\x81"
    "A\x91\x8A\x8E\xE8\x82\xCC\n b2\x8DU\x8C\x82\x97\xCD\x82\xC5\x94\xBD\x8C\x82\x82\xB7\x82\xE9\x81"
    "B",
    /* c5d1「b0を０に」\nc7 相手のb0攻撃力を０に変える。\n... (b1 and b2 alike) */
    "c5d1\x81ub0\x82\xF0\x82O\x82\xC9\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC"
    "b0\x8DU\x8C\x82\x97\xCD\x82\xF0\x82O\x82\xC9\x95\xCF\x82\xA6\x82\xE9\x81"
    "B\nc5d1\x81ub1\x82\xF0\x82O\x82\xC9\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC"
    "b1\x8DU\x8C\x82\x97\xCD\x82\xF0\x82O\x82\xC9\x95\xCF\x82\xA6\x82\xE9\x81"
    "B\nc5d1\x81ub2\x82\xF0\x82O\x82\xC9\x81v\nc7 \x91\x8A\x8E\xE8\x82\xCC"
    "b2\x8DU\x8C\x82\x97\xCD\x82\xF0\x82O\x82\xC9\x95\xCF\x82\xA6\x82\xE9\x81"
    "B",
};

s32 KAW_tickCardCursor(s32 player, s32 mode) {
    s32 i;
    s32 prevId;
    s32 slot;
    s32 side;
    s32 card;
    s32 below;
    s32 above;
    s32 slotCount;

    DUEL->cursorMode = mode;
    KAW_drawHandHints(player, mode);
    for (i = 0; i < 2; i++) {
        PLAYER(i)->topCards[0] = peekOnlineDeckTop(i);
        PLAYER(i)->topCards[1] = peekOfflineDeckTop(i);
        PLAYER(i)->topCards[2] = getActiveDigimonCard(i);
        PLAYER(i)->topCards[3] = getPlayedCard(i);
        PLAYER(i)->topCards[4] = peekDpSlotTop(i);
    }
    slotCount = 9;
    if (DUEL->cursorSlot == -1) {
        DUEL->cursorPlayer = player;
        for (i = 0; i < slotCount; i++) {
            if (PLAYER(player)->hand[i] != -1) {
                ((CardCursor *)DUEL->cursor)->id = PLAYER(player)->hand[i];
                DUEL->cursorSlot = i;
                break;
            }
        }
    }
    prevId = ((CardCursor *)DUEL->cursor)->id;
    slot = DUEL->cursorSlot;
    side = DUEL->cursorPlayer;
next:
    if (PAD_STATES[player]->rawRepeat & PAD_UP) {
        slot = KAW_CURSOR_NEIGHBOURS[side][slot][0];
    } else if (PAD_STATES[player]->rawRepeat & PAD_DOWN) {
        slot = KAW_CURSOR_NEIGHBOURS[side][slot][1];
    } else if (PAD_STATES[player]->rawRepeat & PAD_LEFT) {
        slot = KAW_CURSOR_NEIGHBOURS[side][slot][2];
    } else if (PAD_STATES[player]->rawRepeat & PAD_RIGHT) {
        slot = KAW_CURSOR_NEIGHBOURS[side][slot][3];
    }
    switch (slot) {
    case -1:
        /* up or down onto the other side: the nearest card there */
        if (DUEL->cursorPlayer != side && (PAD_STATES[player]->rawRepeat & (PAD_UP | PAD_DOWN))) {
            below = DUEL->cursorSlot - 1;
            above = DUEL->cursorSlot + 1;
            for (i = 0; i < 9; i++) {
                if (below > 0 && (slot = PLAYER(side)->hand[below]) != -1) {
                    ((CardCursor *)DUEL->cursor)->id = slot;
                    DUEL->cursorSlot = below;
                    DUEL->cursorPlayer = side;
                    break;
                }
                if (above < 9 && (slot = PLAYER(side)->hand[above]) != -1) {
                    ((CardCursor *)DUEL->cursor)->id = slot;
                    DUEL->cursorSlot = above;
                    DUEL->cursorPlayer = side;
                    break;
                }
                below--;
                above++;
            }
        }
        goto moved;
    case -2:
        side ^= 1;
        slot = DUEL->cursorSlot;
        break;
    }
    card = PLAYER(side)->hand[slot];
    if (card == -1) {
        goto next;
    }
    ((CardCursor *)DUEL->cursor)->id = card;
    DUEL->cursorSlot = slot;
    DUEL->cursorPlayer = side;
moved:
    if (prevId != ((CardCursor *)DUEL->cursor)->id) {
        playSoundEffect(0xA2);
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
    if (PAD_STATES[player]->rawPressed & PAD_CIRCLE) {
        return 0;
    }
    return -1;
}

/* draws help page `page` at x, y */
void func_801F8D04(s32 x, s32 y, s32 page) {
    Rect16 rect;

    rect.x = x - 2;
    rect.y = y - 2;
    rect.w = 0xE4;
    rect.h = 0xC4;
    drawIconText(x, y, 7, 1, 0, (s32)KAW_HELP_PAGES[page]);
    drawWindowFrame(&rect, 0, 0, 0, 0xFF, D_801FF6D8, 0);
    x += 0xF0;
    y += 0xA8;
    rect.x = x - 2;
    rect.y = y - 2;
    rect.w = 0x36;
    rect.h = 0x1C;
    /* b0次の頁\nb2前の頁\n */
    drawIconText(x, y, 7, 1, 0, (s32) "b0\x8E\x9F\x82\xCC\x95\xC5\nb2\x91O\x82\xCC\x95\xC5\n");
    drawWindowFrame(&rect, 0, 0, 0, 0xFF, D_801FF6D8, 0);
}

void func_801F8E24(s32 player) {
    HUD_PANEL(22)->state = 4;
    HUD_PANEL(3)->state = player * 3 + 5;
    HUD_PANEL(14)->state = player * 3 + 5;
}

void func_801F8E6C(s32 player) {
    DUEL->cursorSlot = -1;
    HUD_PANEL(3)->state = 2;
    HUD_PANEL(14)->state = 2;
    PLAYER_PANEL(player, 4)->state = 4;
    PLAYER_PANEL(player, 5)->state = 4;
    if (PLAYER_PANEL(player, 6)->state < 4) {
        PLAYER_PANEL(player, 6)->state = 4;
    }
    if (PLAYER_PANEL(player, 7)->state < 4) {
        PLAYER_PANEL(player, 7)->state = 4;
    }
}

/* select opens and closes the help, circle and cross turn its pages */
void func_801F8F68(void) {
    s32 pad;

    if (DUEL->helpOpen != 0) {
        pad = DUEL->menuPlayer;
        func_801F8D04(0x10, 0x14, DUEL->helpPage);
        if (PAD_STATES[pad]->rawPressed & PAD_CIRCLE) {
            playSoundEffect(0xA2);
            DUEL->helpPage++;
        } else if (PAD_STATES[pad]->rawPressed & PAD_CROSS) {
            playSoundEffect(0xA2);
            DUEL->helpPage--;
        }
        DUEL->helpPage = (DUEL->helpPage + 4) % 4;
        if (PAD_STATES[pad]->rawPressed & PAD_SELECT) {
            playSoundEffect(0xA1);
            DUEL->helpOpen = 0;
        }
    } else if (DUEL->quit == 0 && DUEL->unk480 < 2) {
        pad = (s8)DUEL->unk480;
        if (PAD_STATES[pad]->rawPressed & PAD_SELECT) {
            playSoundEffect(0xA0);
            DUEL->helpOpen = 1;
            DUEL->helpPage = 0;
            DUEL->menuPlayer = pad;
        }
    }
}

/* "  降参しますか？\nc7b0する  b2しない\n": the last three bytes are leftovers
   in the original, not zero padding */
const char KAW_STR_GIVE_UP[40] = "  \x8D~\x8EQ\x82\xB5\x82\xDC\x82\xB7\x82\xA9\x81H\nc7b0\x82\xB7\x82\xE9  b2"
                                 "\x82\xB5\x82\xC8\x82\xA2\n\0\xA2\xF0\xA3";

/* start opens the Give Up prompt: circle gives up, cross or start closes it */
void func_801F9144(void) {
    Rect16 rect;
    s32 pad;

    if (DUEL->quit != 0) {
        pad = DUEL->menuPlayer;
        rect.x = 0x7A;
        rect.y = 0x6A;
        rect.w = 0x5E;
        rect.h = 0x1C;
        drawIconText(0x7C, 0x6C, 6, 1, 0, (s32)KAW_STR_GIVE_UP);
        drawWindowFrame(&rect, 0, 0, 0, 0xFF, D_801FF6D8, 0);
        if (D_801FFB1C == 0) {
            if (PAD_STATES[pad]->rawPressed & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                DUEL->quit = (DUEL->unk480 + 2) ^ 1;
                return;
            }
            if (PAD_STATES[pad]->rawPressed & (PAD_CROSS | PAD_START)) {
                playSoundEffect(0xA1);
                DUEL->quit = 0;
            }
        } else {
            D_801FFB1C--;
        }
    } else if (DUEL->helpOpen == 0 && DUEL->unk480 < 2) {
        pad = (s8)DUEL->unk480;
        if (PAD_STATES[pad]->rawPressed & PAD_START) {
            playSoundEffect(0xA0);
            DUEL->quit = 1;
            DUEL->menuPlayer = pad;
            D_801FFB1C = 16;
        }
    }
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
    SPRITE_KIND(card) = CARD_MOVE_TO_HAND;
    CARD_ANIM(card)->handSlot = slot;
    return card;
}

s32 KAW_discardCard(s32 card, s32 player) {
    if (removeCardFromHand(card, player) != -1) {
        SPRITE_KIND(card) = CARD_MOVE_TO_OFFLINE_DECK;
    } else if (removeCardFromDigimonStack(card, player) != -1) {
        SPRITE_KIND(card) = CARD_MOVE_TO_OFFLINE_DECK;
    }
    discardCardToOfflineDeck(card, player);
}

s32 KAW_discardHand(s32 player) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (((Player *)DUEL_PLAYERS[player])->hand[i] != -1) {
            SPRITE_KIND(((Player *)DUEL_PLAYERS[player])->hand[i]) = CARD_MOVE_TO_OFFLINE_DECK;
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
            SPRITE_KIND(card) = CARD_MOVE_TO_DIGIMON_STACK;
            placeActiveDigimon(card, player);
        }
    }
    return result;
}

s32 KAW_returnDigimonToHand(s32 card, s32 player, s32 slot) {
    if (removeCardFromDigimonStack(card, player) != -1) {
        SPRITE_KIND(card) = CARD_MOVE_TO_HAND;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        CARD_ANIM(card)->handSlot = slot;
    }
}

void KAW_returnPlayedCard(s32 player, s32 slot) {
    s32 card;

    card = takePlayedCard(player);
    if (card != -1) {
        if (slot != 4) {
            SPRITE_KIND(card) = CARD_MOVE_TO_HAND;
            ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
            CARD_ANIM(card)->handSlot = slot;
        } else {
            SPRITE_KIND(card) = CARD_MOVE_TO_ONLINE_DECK;
            returnCardToOnlineDeck(card, player);
        }
    }
}

s32 KAW_returnDiscardToHand(s32 player, s32 slot) {
    s32 card;

    card = takeOfflineDeckTopCard(player);
    if (card != -1) {
        SPRITE_KIND(card) = CARD_MOVE_TO_HAND;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        CARD_ANIM(card)->handSlot = slot;
    }
}

s32 KAW_returnDpCardToHand(s32 card, s32 player, s32 slot) {
    if (removeCardFromDpSlots(card, player) != -1) {
        SPRITE_KIND(card) = CARD_MOVE_TO_HAND;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        CARD_ANIM(card)->handSlot = slot;
    }
}

s32 KAW_playOnlineDeckTop(s32 player) {
    s32 card;

    if (countOnlineDeckCards(player) && isPlayedCardSlotEmpty(player)) {
        card = drawOnlineDeckCard(player);
        setPlayedCard(card, player);
        SPRITE_KIND(card) = CARD_MOVE_DRAWN_TO_PLAYED;
    }
}

s32 KAW_playCardFromHand(s32 card, s32 player) {
    s32 result;

    result = -1;
    if (isPlayedCardSlotEmpty(player)) {
        result = removeCardFromHand(card, player);
        if (result != -1) {
            SPRITE_KIND(card) = CARD_MOVE_TO_PLAYED;
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
            SPRITE_KIND(card) = CARD_MOVE_TO_DP_SLOTS;
            addCardToDpSlots(card, player);
        }
    }
    return result;
}

s32 KAW_redrawHand(s32 player) {
    s32 i;

    while (countEmptyHandSlots(player) != 4) {
        waitFrames(20);
        for (i = 0; i < 4; i++) {
            if (((Player *)DUEL_PLAYERS[player])->hand[i] != -1) {
                SPRITE_KIND(((Player *)DUEL_PLAYERS[player])->hand[i]) = CARD_MOVE_TO_OFFLINE_DECK;
                discardCardToOfflineDeck(((Player *)DUEL_PLAYERS[player])->hand[i], player);
                removeCardFromHand(((Player *)DUEL_PLAYERS[player])->hand[i], player);
                break;
            }
        }
    }
    do {
        waitFrames(20);
    } while (KAW_drawCardToHand(player) != -1);
}

s32 KAW_undoDigivolve(void) {
    if (DUEL->discardedFromSlot >= 0) {
        KAW_returnDiscardToHand(DUEL->turnPlayer, DUEL->discardedFromSlot);
        DUEL->discardedFromSlot = -1;
    }
    if (DUEL->dpFromSlot >= 0) {
        KAW_returnDpCardToHand(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer, DUEL->dpFromSlot);
        DUEL->dpFromSlot = -1;
    }
    DUEL->step = 15;
}

s32 KAW_discardDpSlots(void) {
    while (peekDpSlotTop(DUEL->turnPlayer) != -1) {
        discardCardToOfflineDeck(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer);
        SPRITE_KIND(peekDpSlotTop(DUEL->turnPlayer)) = CARD_MOVE_TO_OFFLINE_DECK;
        removeCardFromDpSlots(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer);
    }
    waitDuelFrames(120);
    DUEL->step = 32;
}

s32 KAW_checkKnockout(s32 player) {
    s32 opponent;
    s32 card;

    opponent = player ^ 1;
    if (PLAYER(player)->stats[0] == 0) {
        PLAYER(opponent)->wins++;
        DUEL->winner = opponent;
        if ((PLAYER(player)->unk178_13 >> 1) && PLAYER(opponent)->wins != 3) {
            PLAYER(player)->stats[0] = PLAYER(player)->reviveHp;
        } else {
            card = PLAYER_CARDS(PLAYER(opponent))[getActiveDigimonCard(opponent) % 30].index;
            if (((ProfileK *)PLAYER_PROFILES)[opponent].cardWins[card] != 999) {
                ((ProfileK *)PLAYER_PROFILES)[opponent].cardWins[card]++;
            }
            card = PLAYER_CARDS(PLAYER(player))[getActiveDigimonCard(player) % 30].index;
            if (((ProfileK *)PLAYER_PROFILES)[player].cardLosses[card] != 999) {
                ((ProfileK *)PLAYER_PROFILES)[player].cardLosses[card]++;
            }
            while (getActiveDigimonCard(player) != -1) {
                KAW_discardCard(getActiveDigimonCard(player), player);
            }
        }
        return 1;
    }
    return 0;
}
