#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/pad.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/battle_hud.h"
#include "dcb/card_db.h"
#include "dcb/card_render.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_hand.h"

/* jp's KAWSEG gives a card of the CPU's deck as the prize (kaw_prize.c is
   us's and eu's, which give a prize pack) */

/* jp's executable: the copies of a card the player owns, giving one, and
   the cursor that points at a window */
void *KAW_createCursor(s32 kind, s32 w, s32 h, s32 frames, s32 count);
void KAW_freeCursor(void *cursor);
void KAW_drawCursorAt(void *cursor, s32 x, s32 y);

/* libgpu's */
#define setRECT(r, _x, _y, _w, _h) (r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h)

extern void *KAW_HAND_CURSOR;

/* the frame colours of KAWSEG's text windows (kaw_hand_jp.c's too) */
CVECTOR KAW_WINDOW_FRAME_COLORS[3] = { { 0, 0, 0x30, 0 }, { 0xC0, 0xC0, 0xC0, 0 }, { 0xC0, 0xC0, 0xC0, 0 } };
extern char KAW_FMT_FOUR_DIGITS[]; /* kaw_match_intro's "w-1%4d" */

/* Draws prize window index: the card's number, name and stats, and how many
   the player owns; the chosen one is bright */
void KAW_drawPrizeWindow(s32 index, s32 z) {
    char text[72];
    u8 rgb[4];
    PlayerDeck *deck;
    s32 x;
    s32 y;
    s32 card;
    s32 i;

    if (KAW_CARD_PRIZE->choice == index) {
        rgb[0] = 0x80;
        rgb[1] = 0x80;
        rgb[2] = 0x80;
    } else {
        rgb[0] = 0x40;
        rgb[1] = 0x40;
        rgb[2] = 0x40;
    }
    x = KAW_CARD_PRIZE->windows[index].x;
    y = KAW_CARD_PRIZE->windows[index].y;
    card = KAW_CARD_PRIZE->cards[index];
    deck = PLAYER(1)->deck;
    if (getOwnedCardCount(deck->cards[card % 30].type, deck->cards[card % 30].index) == 0) {
        KAW_drawSprite(x + 0xE, y + 0x2E, 0x360, 0x1F0, 0x18, 9, 0x360, 0x1F9, 0, 1, 0, rgb[0], 0);
    }
    CARD_SPR(card)->pos.vx = x - 0x8C;
    CARD_SPR(card)->pos.vy = y - 0x54;
    CARD_SPR(card)->pos.vz = 0;
    CARD_SPR(card)->rot.vx = 0x2000;
    CARD_SPR(card)->rot.vy = 0x2000;
    CARD_SPR(card)->rot.vz = 0x2000;
    CARD_SPR(card)->scale = 0x1000;
    CARD_SPR(card)->flags = 0x80;
    CARD_SPR(card)->fade[0] = rgb[0];
    CARD_SPR(card)->fade[1] = rgb[1];
    CARD_SPR(card)->fade[2] = rgb[2];
    switch (deck->cards[card % 30].type) {
    case 0: {
        DigimonCardData *data;

        data = (DigimonCardData *)deck->cards[card % 30].card;
        sprintf(text, "Ｎｏ．%d", deck->cards[card % 30].index + 1);
        drawIconTextColored(x + 5, y, 7, 1, rgb, z, text);
        drawIconTextColored(x + 0x32, y, 7, 1, rgb, z, data->name);
        drawIconTextColored(x + 0x2C, y + 0xE, 7, 1, rgb, z, "ＨＰ");
        sprintf(text, KAW_FMT_FOUR_DIGITS, data->hp);
        drawTextColored(x + 0x3C, y + 0xE, text, rgb, 7, z);
        drawIconTextColored(x + 0x2C, y + 0x1A, 7, 1, rgb, z, "b0");
        sprintf(text, KAW_FMT_FOUR_DIGITS, data->attack[0].power);
        drawTextColored(x + 0x3C, y + 0x1A, text, rgb, 7, z);
        drawIconTextColored(x + 0x2C, y + 0x26, 7, 1, rgb, z, "b1");
        sprintf(text, KAW_FMT_FOUR_DIGITS, data->attack[1].power);
        drawTextColored(x + 0x3C, y + 0x26, text, rgb, 7, z);
        drawIconTextColored(x + 0x2C, y + 0x32, 7, 1, rgb, z, "b2");
        sprintf(text, KAW_FMT_FOUR_DIGITS, data->attack[2].power);
        drawTextColored(x + 0x3C, y + 0x32, text, rgb, 7, z);
        sprintf(text, "(%s)", CROSS_EFFECT_SHORT_NAMES[data->crossEffect]);
        drawTinyTextColored(x + 0x5A, y + 0x34, text, 7, rgb, z);
        drawIconTextColored(x + 0x64, y + 0x1A, 7, 1, rgb, z, "必進");
        sprintf(text, "w-1%2d", data->dpCost);
        drawTextColored(x + 0x86, y + 0x1A, text, rgb, 7, z);
        drawIconTextColored(x + 0x64, y + 0x26, 7, 1, rgb, z, "ＰＯＷ");
        sprintf(text, "w-1%2d", data->dpBonus);
        drawTextColored(x + 0x86, y + 0x26, text, rgb, 7, z);
        drawIconTextColored(x + 0xA4, y, 6, 1, rgb, z, "援護能力");
        for (i = 0; i < 4; i++) {
            drawIconTextColored(x + 0xA4, y + 0xE + i * 12, 7, 1, rgb, z, data->supportText[i]);
        }
        CARD_SPR(card)->pal = data->attr >> 4;
        break;
    }
    case 1: {
        OptionCardData *data;

        data = (OptionCardData *)deck->cards[card % 30].card;
        sprintf(text, "Ｎｏ．%d", deck->cards[card % 30].index + 0x6F);
        drawIconTextColored(x + 5, y, 7, 1, rgb, z, text);
        drawIconTextColored(x + 0x32, y, 7, 1, rgb, z, data->name);
        drawIconTextColored(x + 0xA4, y, 6, 1, rgb, z, "オプション内容");
        for (i = 0; i < 4; i++) {
            drawIconTextColored(x + 0xA4, y + 0xE + i * 12, 7, 1, rgb, z, data->text[i]);
        }
        CARD_SPR(card)->pal = 5;
        break;
    }
    case 2: {
        DigivolveCardData *data;

        data = (DigivolveCardData *)deck->cards[card % 30].card;
        sprintf(text, "Ｎｏ．%d", deck->cards[card % 30].index + 0x9A);
        drawIconTextColored(x + 5, y, 7, 1, rgb, z, text);
        drawIconTextColored(x + 0x32, y, 7, 1, rgb, z, data->name);
        drawIconTextColored(x + 0xA4, y, 6, 1, rgb, z, "オプション内容");
        for (i = 0; i < 4; i++) {
            drawIconTextColored(x + 0xA4, y + 0xE + i * 12, 7, 1, rgb, z, data->text[i]);
        }
        CARD_SPR(card)->pal = 5;
        break;
    }
    }
    renderCardSprite(CARD_SPR(card), 0);
    sprintf(text, "%d", getOwnedCardCount(deck->cards[card % 30].type, deck->cards[card % 30].index));
    drawTextColored(x + 0x7F, y + 0xE, text, rgb, 7, z);
    drawIconTextColored(x + 0x64, y + 0xE, 6, 1, rgb, z, "所持  c7枚");
}

/* Draws the prize's message window: what the state asks or says */
void KAW_drawPrizeMessage(void) {
    char text[72];
    s32 x;
    s32 y;

    x = KAW_CARD_PRIZE->message.x;
    y = KAW_CARD_PRIZE->message.y;
    switch (KAW_CARD_PRIZE->state) {
    case 0:
        strcpy(text, "カードを１枚もらえます。どれか１枚選んでね。");
        break;
    case 1:
        sprintf(text, "c6%sカードc7に決定！",
                ((DigimonCardData *)PLAYER(1)->deck->cards[KAW_CARD_PRIZE->cards[KAW_CARD_PRIZE->choice] % 30].card)->name);
        break;
    case 2:
        strcpy(text, "カードをもらわずに終了します。 b0はい b2いいえ");
        break;
    case 3:
        strcpy(text, "すでに８枚持っています。");
        break;
    case 4:
        strcpy(text, "カードをもらわずに終了します。");
        break;
    case 5: {
        s32 index;
        s32 type;

        type = PLAYER(1)->deck->cards[KAW_CARD_PRIZE->cards[KAW_CARD_PRIZE->choice] % 30].type;
        index = PLAYER(1)->deck->cards[KAW_CARD_PRIZE->cards[KAW_CARD_PRIZE->choice] % 30].index;
        if (getOwnedCardCount(type, index) != 0) {
            sprintf(text, "このカードは%d枚持っています。 b0決定 b2戻る", getOwnedCardCount(type, index));
        } else {
            strcpy(text, "このカードは持っていません。 b0決定 b2戻る");
        }
        break;
    }
    }
    drawIconText(x + 5 + (0x118 - measureText(1, text)) / 2, y, 7, 1, 0, (s32)text);
}

/* The card prize: deals three of the CPU's cards (a Digimon below 0x6C or
   an Option below 0x23), slides their windows in, lets the player take one
   or none, then slides the others out */
void KAW_runCardPrize(void) {
    s32 i;
    s32 ok;
    s32 card;
    s32 timer;

    ((SessionData *)SESSION_DATA)->unkC = allocPermanentHeapBlock(0x40);
    KAW_HAND_CURSOR = KAW_createCursor(1, 0x90, 0x22, 4, 1);
    for (i = 0; i < 3; i++) {
        for (;;) {
            card = rand() % 30 + 30;
            KAW_CARD_PRIZE->cards[i] = card;
            if (PLAYER(1)->deck->cards[card % 30].type == 0) {
                ok = PLAYER(1)->deck->cards[card % 30].index < 0x6C;
            } else if (PLAYER(1)->deck->cards[card % 30].type != 1) {
                PLAYER(1)->deck->cards[card % 30].index = 5;
                continue;
            } else {
                ok = PLAYER(1)->deck->cards[card % 30].index < 0x23;
            }
            if (ok) {
                break;
            }
        }
        setRECT(&KAW_CARD_PRIZE->windows[i], 0x154 - (i % 2) * 0x280, i * 0x41 + 0x12, 0x118, 0x3C);
        KAW_CARD_PRIZE->brightness[i] = 0xFF;
    }
    KAW_CARD_PRIZE->choice = 0;
    KAW_CARD_PRIZE->state = 0;
    KAW_CARD_PRIZE->message.x = 0x14;
    KAW_CARD_PRIZE->message.y = 0xDA;
    KAW_CARD_PRIZE->message.w = 0x118;
    KAW_CARD_PRIZE->message.h = 0xC;
    while (KAW_CARD_PRIZE->windows[0].x != 0x14) {
        waitFrames(FRAME_INTERVAL);
        resetCardPolyCount();
        for (i = 0; i < 3; i++) {
            if (KAW_CARD_PRIZE->windows[i].x > 0x14) {
                KAW_CARD_PRIZE->windows[i].x -= 0x20;
                if (KAW_CARD_PRIZE->windows[i].x < 0x14) {
                    KAW_CARD_PRIZE->windows[i].x = 0x14;
                }
            } else if (KAW_CARD_PRIZE->windows[i].x < 0x14) {
                KAW_CARD_PRIZE->windows[i].x += 0x20;
                if (KAW_CARD_PRIZE->windows[i].x > 0x14) {
                    KAW_CARD_PRIZE->windows[i].x = 0x14;
                }
            }
            KAW_drawPrizeWindow(i, 1);
            drawWindowFrame(&KAW_CARD_PRIZE->windows[i], 0, 0, 0, KAW_CARD_PRIZE->brightness[i], KAW_WINDOW_FRAME_COLORS, 1);
        }
        KAW_drawPrizeMessage();
        drawWindowFrame(&KAW_CARD_PRIZE->message, 0, 0, 0, 0xFF, KAW_WINDOW_FRAME_COLORS, 1);
    }
    do {
        waitFrames(FRAME_INTERVAL);
        resetCardPolyCount();
        switch (KAW_CARD_PRIZE->state) {
        case 0:
            if (PAD_STATES[0]->rawRepeat & PAD_UP) {
                playSoundEffect(0xA2);
                KAW_CARD_PRIZE->choice--;
            } else if (PAD_STATES[0]->rawRepeat & PAD_DOWN) {
                playSoundEffect(0xA2);
                KAW_CARD_PRIZE->choice++;
            }
            KAW_CARD_PRIZE->choice = (KAW_CARD_PRIZE->choice + 3) % 3;
            if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                KAW_CARD_PRIZE->state = 5;
            } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
                playSoundEffect(0xA1);
                KAW_CARD_PRIZE->state = 2;
            }
            break;
        case 2:
            if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                KAW_CARD_PRIZE->state = 4;
            } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
                playSoundEffect(0xA1);
                KAW_CARD_PRIZE->state = 0;
            }
            break;
        case 1:
        case 4:
            break;
        case 3:
            if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                KAW_CARD_PRIZE->state = 0;
            }
            break;
        case 5:
            if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                if (getOwnedCardCount(PLAYER(1)->deck->cards[KAW_CARD_PRIZE->cards[KAW_CARD_PRIZE->choice] % 30].type,
                                  PLAYER(1)->deck->cards[KAW_CARD_PRIZE->cards[KAW_CARD_PRIZE->choice] % 30].index) == 8) {
                    KAW_CARD_PRIZE->state = 3;
                } else {
                    KAW_CARD_PRIZE->state = 1;
                }
            } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
                playSoundEffect(0xA1);
                KAW_CARD_PRIZE->state = 0;
            }
            break;
        }
        KAW_drawCursorAt(KAW_HAND_CURSOR, 0xA0, KAW_CARD_PRIZE->choice * 0x41 + 0x30);
        KAW_drawPrizeWindow(KAW_CARD_PRIZE->choice, 1);
        drawWindowFrame(&KAW_CARD_PRIZE->windows[KAW_CARD_PRIZE->choice], 0, 0, 0, 0xFF, KAW_WINDOW_FRAME_COLORS, 1);
        for (i = 0; i < 3; i++) {
            if (KAW_CARD_PRIZE->choice != i) {
                KAW_drawPrizeWindow(i, 1);
                drawWindowFrame(&KAW_CARD_PRIZE->windows[i], 0, 0, 0, KAW_CARD_PRIZE->brightness[i], KAW_WINDOW_FRAME_COLORS, 1);
            }
        }
        KAW_drawPrizeMessage();
        drawWindowFrame(&KAW_CARD_PRIZE->message, 0, 0, 0, 0xFF, KAW_WINDOW_FRAME_COLORS, 1);
    } while (KAW_CARD_PRIZE->state != 1 && KAW_CARD_PRIZE->state != 4);
    for (timer = 0; timer < 180; timer++) {
        waitFrames(FRAME_INTERVAL);
        resetCardPolyCount();
        if (KAW_CARD_PRIZE->state == 4) {
            KAW_CARD_PRIZE->choice = -1;
        } else {
            i = KAW_CARD_PRIZE->choice;
            if (KAW_CARD_PRIZE->windows[i].y > 0x53) {
                KAW_CARD_PRIZE->windows[i].y -= 8;
                if (KAW_CARD_PRIZE->windows[i].y < 0x53) {
                    KAW_CARD_PRIZE->windows[i].y = 0x53;
                }
            } else if (KAW_CARD_PRIZE->windows[i].y < 0x53) {
                KAW_CARD_PRIZE->windows[i].y += 8;
                if (KAW_CARD_PRIZE->windows[i].y > 0x53) {
                    KAW_CARD_PRIZE->windows[i].y = 0x53;
                }
            }
            KAW_drawPrizeWindow(i, 1);
            drawWindowFrame(&KAW_CARD_PRIZE->windows[i], 0, 0, 0, 0xFF, KAW_WINDOW_FRAME_COLORS, 1);
        }
        for (i = 0; i < 3; i++) {
            if (KAW_CARD_PRIZE->choice != i) {
                if (i & 1) {
                    KAW_CARD_PRIZE->windows[i].x -= 0x10;
                    if (KAW_CARD_PRIZE->windows[i].x < -0x12C) {
                        KAW_CARD_PRIZE->windows[i].x = -0x12C;
                    }
                } else {
                    KAW_CARD_PRIZE->windows[i].x += 0x10;
                    if (KAW_CARD_PRIZE->windows[i].x > 0x154) {
                        KAW_CARD_PRIZE->windows[i].x = 0x154;
                    }
                }
                KAW_drawPrizeWindow(i, 1);
                drawWindowFrame(&KAW_CARD_PRIZE->windows[i], 0, 0, 0, KAW_CARD_PRIZE->brightness[i], KAW_WINDOW_FRAME_COLORS, 1);
            }
        }
        if (KAW_CARD_PRIZE->choice != -1) {
            KAW_drawCursorAt(KAW_HAND_CURSOR, 0xA0, KAW_CARD_PRIZE->windows[KAW_CARD_PRIZE->choice].y + 0x1E);
        }
        KAW_drawPrizeMessage();
        drawWindowFrame(&KAW_CARD_PRIZE->message, 0, 0, 0, 0xFF, KAW_WINDOW_FRAME_COLORS, 1);
        if (timer > 60 && (PAD_STATES[0]->rawPressed & PAD_CIRCLE)) {
            playSoundEffect(0xA0);
            break;
        }
    }
    if (KAW_CARD_PRIZE->state == 1) {
        addCardToCollection(PLAYER(1)->deck->cards[KAW_CARD_PRIZE->cards[KAW_CARD_PRIZE->choice] % 30].type,
                      PLAYER(1)->deck->cards[KAW_CARD_PRIZE->cards[KAW_CARD_PRIZE->choice] % 30].index, 1);
    }
    KAW_freeCursor(KAW_HAND_CURSOR);
    freeHeapBlock(KAW_CARD_PRIZE);
    waitFrames(10);
}
