#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/sound_play.h"
#include "dcb/pad.h"
#include "dcb/prim.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/player_rank.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
#include "dcb/model_load.h"
#include "dcb/anim_control.h"
#include "dcb/model_anim.h"
#include "dcb/frame_callback.h"
#include "dcb/nisseg.h"

/* The card trade between the two players of VS mode: each player picks up
   to three kinds of cards from those they can spare, then both offers are
   shown and the cards change hands */

extern u8 D_801E46E9;
extern TradeWindows NIS_TRADE_WINDOWS;
extern NisCursor *NIS_TRADE_CURSOR;
extern TradeView NIS_TRADE_VIEW;
extern TradeCardList *NIS_TRADE_LIST;
extern TradeMasks *NIS_TRADE_MASKS; /* two for each player, the second unused */
extern u8 NIS_TRADE_PLAYER;
extern TradeOffer NIS_TRADE_OFFERS[2];
extern DR_ENV NIS_TRADE_SCENE_ENVS[2];
extern DR_ENV NIS_TRADE_SCREEN_ENVS[2];
extern DRAWENV NIS_TRADE_SCENE_DRAWENVS[2];

void SetDrawEnv(DR_ENV *dr, DRAWENV *env);
NisCursor *KAW_createCursor(s32, s32, s32, s32, s32);
void KAW_freeCursor(NisCursor *cursor);
void NIS_runVsMode(void);
void NIS_showCard(s32 kind);

void NIS_runTradeMenu(void);
void NIS_runTradeGrid(s32 arg);
void NIS_drawTradeGrid(NisWindow *window);
void NIS_drawTradeTitle(NisWindow *window);
void NIS_drawTradeGridHelp(NisWindow *window);
void NIS_closeTradeWindow(NisWindow *window);
void NIS_drawTradeCardDetails(NisWindow *window);
void NIS_drawTradeOffers(NisWindow *window);
void NIS_drawTradeQuestion(NisWindow *window);
NisCardPicture NIS_getTradeCardPicture(s32 type, s32 index);
s32 NIS_getTradeCardId(s32 type, s32 index);
void NIS_drawTradeCardPicture(NisCardPicture picture, s32 x, s32 y, s32 z, u8 brightness);
void NIS_openTradeScene(s32 modelId);
void NIS_renderTradeScene(FrameBuffer *fb, s32 buffer);
void NIS_closeTradeScene(void);
s32 NIS_changeTradeCopies(void);
void NIS_writeTradeCopies(s32 delta);
void NIS_buildTradeList(u8 player);
void NIS_removeDeckCopies(PlayerProfile *profile, s8 *digimon, s8 *options, s8 *others);
void NIS_capReceivedCopies(PlayerProfile *profile, s8 *digimon, s8 *options, s8 *others);
void NIS_limitToNewSerials(PlayerProfile *mine, PlayerProfile *theirs, TradeMasks *masks, s8 *digimon, s8 *options, s8 *others);
void NIS_findNewSerials(PlayerProfile *mine, PlayerProfile *theirs, u8 *digimonMasks, u8 *optionMasks, u8 *otherMasks);
void NIS_findNewCardSerials(u8 type, u8 index, PlayerProfile *mine, PlayerProfile *theirs, u8 *mask);
void NIS_giveCardCopy(u8 type, u8 index, PlayerProfile *from, PlayerProfile *to, TradeMasks *masks);
s16 NIS_countTradeOffer(void);
void NIS_setTradeOffer(void);
void NIS_runTradeQuestion(void);

NisWindowDef NIS_TRADE_TITLE_WINDOW = { { 0x1A, 0x17, 0, 0xC }, { 0x1A, 0x17, 0x110, 0xC }, 0xA, 0, NIS_drawTradeTitle, NIS_closeTradeWindow };
NisWindowDef NIS_TRADE_GRID_WINDOW = { { 0x122, 0x30, 0, 0xC }, { 0x12, 0x30, 0x110, 0x94 }, 0xA, 1, NIS_drawTradeGrid, NIS_closeTradeWindow };
NisWindowDef NIS_TRADE_HELP_WINDOW = { { 0x1A, 0xD0, 0, 0xC }, { 0x1A, 0xD0, 0x110, 0xC }, 0xA, 0, NIS_drawTradeGridHelp, NIS_closeTradeWindow };
NisWindowDef NIS_TRADE_CARD_WINDOW = { { 0x122, 0x30, 0, 0xC }, { 0x1A, 0x30, 0x108, 0xB4 }, 0xA, 1, NIS_drawTradeCardDetails, NIS_closeTradeWindow };
NisWindowDef NIS_TRADE_OFFERS_WINDOW = { { 0x122, 0x30, 0, 0xC }, { 0x84, 0x30, 0x9E, 0x9C }, 0xA, 1, NIS_drawTradeOffers, NIS_closeTradeWindow };
NisWindowDef NIS_TRADE_QUESTION_WINDOW = { { 0x1A, 0xD8, 0, 0xC }, { 0x1A, 0xD8, 0x110, 0xC }, 0xA, 0, NIS_drawTradeQuestion, NIS_closeTradeWindow };

void NIS_loadTradeTimFile(char *path) {
    u32 *tims;

#if JP_DEBUG_BUILD
    spawnTask(0, -1, 4, 0x800, loadFile, path, getCurrentTaskId());
#else
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
#endif
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    if (tims != NULL) {
        uploadTimList(tims);
        freeHeapBlock(tims);
    }
}

void NIS_startTrade(void) {
#if JP_DEBUG_BUILD
    D_801DEBF0 = 0;
#endif
    NIS_loadTradeTimFile("A:\\DECK.TIM");
    openKanjiPage(0xF, 0x1E3);
    NIS_TRADE_MASKS = NIS_ALLOC_HEAP_BLOCK(sizeof(TradeMasks) * 4, 0x25A, "TRADE HEAP", 6);
    NIS_TRADE_LIST = NIS_ALLOC_HEAP_BLOCK(sizeof(TradeCardList), 0x25B, "DECK EDIT", 7);
    bzero((void *)NIS_TRADE_LIST, sizeof(TradeCardList));
    NIS_DEBUG_NAME_TASK(0, "TRADE");
    spawnTask(0, -1, 0, 0x800, NIS_runTradeMenu, 1, 0, 0, 0);
    NIS_TRADE_OFFERS[0].kinds = 0;
    NIS_TRADE_OFFERS[0].copies = 0;
    NIS_TRADE_OFFERS[1].kinds = 0;
    NIS_TRADE_OFFERS[1].copies = 0;
}

void NIS_runTradeMenu(void) {
    NisMenu menu;

    bzero((void *)NIS_TRADE_LIST, sizeof(TradeCardList));
    openChoiceMenu(&menu, 0x32, 0x32, NULL, NULL);
    addChoiceMenuItem(&menu, 0x2F, NIS_runTradeGrid);
    addChoiceMenuItem(&menu, 0x30, NIS_runTradeGrid);
    addChoiceMenuItem(&menu, 0x32, NIS_runTradeQuestion);
    addChoiceMenuItem(&menu, 0x33, NIS_runVsMode);
    NIS_DEBUG_NAME_TASK(0, "WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &NIS_TRADE_OFFERS_WINDOW, getCurrentTaskId(), 0, 0);
    NIS_TRADE_WINDOWS.offers = waitFrames(0x7FFFFFFF);
    waitFrames(0x28);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
#if JP_DEBUG_BUILD
        /* each player's offer, and the first masks */
        sprintf(DEBUG_TEXT_LINES[0], "KIND_NUM.%d TOTAL_NUM.%d\n", NIS_TRADE_OFFERS[0].kinds, NIS_TRADE_OFFERS[0].copies);
        sprintf(DEBUG_TEXT_LINES[1], "KIND_NUM.%d TOTAL_NUM.%d\n", NIS_TRADE_OFFERS[1].kinds, NIS_TRADE_OFFERS[1].copies);
        sprintf(DEBUG_TEXT_LINES[2], "FLG.%d FLG.%d\n", NIS_TRADE_MASKS[0].digimon[0], NIS_TRADE_MASKS[2].digimon[0]);
#endif
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        switch (menu.choice) {
        case 0:
            D_801E46E9 = 0;
            NIS_WINDOW(NIS_TRADE_WINDOWS.offers)->state = 4;
            SCROLLING_BACKGROUND->unk1C0 = -1;
            waitFrames(0x3C);
            NIS_TRADE_PLAYER = 0;
            NIS_buildTradeList(0);
            break;
        case 1:
            D_801E46E9 = 1;
            NIS_WINDOW(NIS_TRADE_WINDOWS.offers)->state = 4;
            SCROLLING_BACKGROUND->unk1C0 = -1;
            waitFrames(0x3C);
            NIS_TRADE_PLAYER = 1;
            NIS_buildTradeList(1);
            break;
        case 2:
            break;
        case 3:
            updatePlayerRanks(0);
            updatePlayerRanks(1);
            NIS_WINDOW(NIS_TRADE_WINDOWS.offers)->state = 4;
            waitFrames(0x3C);
            freeHeapBlocksByTag(0x25B);
            freeHeapBlocksByTag(0x25A);
            closeKanjiPage(0xF);
            break;
        }
        startChoiceMenuAction(&menu);
    }
}

#define TRADE_PRESSED(m) ((PAD_STATES[0]->rawPressed & (m)) || (PAD_STATES[1]->rawPressed & (m)))
#define TRADE_REPEATED(m) ((PAD_STATES[0]->rawRepeat & (m)) || (PAD_STATES[1]->rawRepeat & (m)))

/* the grid of the cards the player can trade: L1 and R1 choose the copies,
   Circle shows a card, Triangle keeps the offer, Cross drops it */
void NIS_runTradeGrid(s32 arg) {
    NisMenu unused; /* unused, but it is in the original stack frame */
    s32 kind;
    s32 cursor;
    s32 rowStart;
    s32 rowEnd;

    kind = 0;
    NIS_TRADE_CURSOR = KAW_createCursor(1, 0x14, 0x18, 5, 1);
    NIS_DEBUG_NAME_TASK(0, "WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &NIS_TRADE_TITLE_WINDOW, getCurrentTaskId(), 0, 0);
    NIS_TRADE_SCREENS.titleWindow = waitFrames(0x7FFFFFFF);
    NIS_DEBUG_NAME_TASK(0, "WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &NIS_TRADE_GRID_WINDOW, getCurrentTaskId(), 0, 0);
    NIS_TRADE_WINDOWS.grid = waitFrames(0x7FFFFFFF);
    NIS_DEBUG_NAME_TASK(0, "WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &NIS_TRADE_HELP_WINDOW, getCurrentTaskId(), 0, 0);
    NIS_TRADE_WINDOWS.help = waitFrames(0x7FFFFFFF);
    waitFrames(0x28);
    while (1) {
        cursor = NIS_TRADE_LIST->cursor;
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (TRADE_PRESSED(PAD_CIRCLE)) {
            if (NIS_TRADE_LIST->count != 0) {
                playSoundEffect(0);
                KAW_freeCursor(NIS_TRADE_CURSOR);
                NIS_WINDOW(NIS_TRADE_SCREENS.titleWindow)->state = 4;
                NIS_WINDOW(NIS_TRADE_WINDOWS.grid)->state = 4;
                NIS_WINDOW(NIS_TRADE_WINDOWS.help)->state = 4;
                waitFrames(0x3C);
                D_801E46E8 = 1;
                NIS_DECK_EDIT.cardType = NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].type;
                NIS_DECK_EDIT.cardIndex = NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].index;
                NIS_DECK_EDIT.copies = NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].chosen;
                NIS_DECK_EDIT.unkA = NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].owned;
                NIS_DECK_EDIT.maxCopies = NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].max;
                NIS_DEBUG_NAME_TASK(0, "DETAIL");
                spawnTask(0, -1, 0, 0x800, NIS_showCard, kind, 0, 0, 0);
                exitTask();
            } else {
                playSoundEffect(1);
                KAW_freeCursor(NIS_TRADE_CURSOR);
                NIS_WINDOW(NIS_TRADE_SCREENS.titleWindow)->state = 4;
                NIS_WINDOW(NIS_TRADE_WINDOWS.grid)->state = 4;
                NIS_WINDOW(NIS_TRADE_WINDOWS.help)->state = 4;
                waitFrames(0x3C);
                NIS_TRADE_OFFERS[NIS_TRADE_PLAYER].kinds = 0;
                NIS_TRADE_OFFERS[NIS_TRADE_PLAYER].copies = 0;
                NIS_DEBUG_NAME_TASK(0, "TEST TRADE");
                spawnTask(0, -1, 0, 0x800, NIS_runTradeMenu, 0, 0, 0, 0);
                exitTask();
            }
        } else if (TRADE_PRESSED(PAD_CROSS)) {
            playSoundEffect(1);
            KAW_freeCursor(NIS_TRADE_CURSOR);
            NIS_WINDOW(NIS_TRADE_SCREENS.titleWindow)->state = 4;
            NIS_WINDOW(NIS_TRADE_WINDOWS.grid)->state = 4;
            NIS_WINDOW(NIS_TRADE_WINDOWS.help)->state = 4;
            waitFrames(0x3C);
            NIS_TRADE_OFFERS[NIS_TRADE_PLAYER].kinds = 0;
            NIS_TRADE_OFFERS[NIS_TRADE_PLAYER].copies = 0;
            NIS_DEBUG_NAME_TASK(0, "TEST TRADE");
            spawnTask(0, -1, 0, 0x800, NIS_runTradeMenu, 0, 0, 0, 0);
            exitTask();
        } else if (TRADE_PRESSED(PAD_TRIANGLE)) {
            playSoundEffect(0);
            KAW_freeCursor(NIS_TRADE_CURSOR);
            NIS_WINDOW(NIS_TRADE_SCREENS.titleWindow)->state = 4;
            NIS_WINDOW(NIS_TRADE_WINDOWS.grid)->state = 4;
            NIS_WINDOW(NIS_TRADE_WINDOWS.help)->state = 4;
            waitFrames(0x3C);
            NIS_setTradeOffer();
            NIS_DEBUG_NAME_TASK(0, "TEST TRADE");
            spawnTask(0, -1, 0, 0x800, NIS_runTradeMenu, 0, 0, 0, 0);
            exitTask();
        }
        if (NIS_TRADE_LIST->count == 0) {
            continue;
        }
        rowStart = NIS_TRADE_LIST->cursor / 5 * 5;
        rowEnd = rowStart + 5;
        if (NIS_TRADE_LIST->count < rowEnd) {
            rowEnd = NIS_TRADE_LIST->count;
        }
        if (TRADE_REPEATED(PAD_R1)) {
            if (NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].chosen < NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].max) {
                NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].chosen++;
                if (NIS_countTradeOffer() == 0) {
                    playSoundEffect(0);
                }
            }
        } else if (TRADE_REPEATED(PAD_L1)) {
            if (NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].chosen > 0) {
                NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].chosen--;
                if (NIS_countTradeOffer() == 0) {
                    playSoundEffect(1);
                }
            }
        } else if (TRADE_REPEATED(PAD_RIGHT)) {
            if (++NIS_TRADE_LIST->cursor >= rowEnd) {
                NIS_TRADE_LIST->cursor = rowStart;
            }
            if (cursor != NIS_TRADE_LIST->cursor) {
                playSoundEffect(2);
            }
        } else if (TRADE_REPEATED(PAD_LEFT)) {
            if (--NIS_TRADE_LIST->cursor < rowStart) {
                NIS_TRADE_LIST->cursor = rowEnd - 1;
            }
            if (cursor != NIS_TRADE_LIST->cursor) {
                playSoundEffect(2);
            }
        } else if (TRADE_REPEATED(PAD_DOWN)) {
            NIS_TRADE_LIST->cursor += 5;
            if (NIS_TRADE_LIST->cursor >= NIS_TRADE_LIST->count) {
                if (NIS_TRADE_LIST->cursor < (NIS_TRADE_LIST->count - 1) / 5 * 5 + 5) {
                    playSoundEffect(2);
                    NIS_TRADE_LIST->cursor = NIS_TRADE_LIST->count - 1;
                } else {
                    NIS_TRADE_LIST->cursor -= 5;
                }
            } else {
                playSoundEffect(2);
            }
        } else if (TRADE_REPEATED(PAD_UP)) {
            if (NIS_TRADE_LIST->cursor - 5 >= 0) {
                NIS_TRADE_LIST->cursor -= 5;
                playSoundEffect(2);
            }
        }
        if (NIS_TRADE_LIST->scroll > NIS_TRADE_LIST->cursor) {
            NIS_TRADE_LIST->scroll -= 5;
        }
        if (NIS_TRADE_LIST->scroll + 9 < NIS_TRADE_LIST->cursor) {
            NIS_TRADE_LIST->scroll += 5;
        }
    }
}

NisCardPicture NIS_getTradeCardPicture(s32 type, s32 index) {
    NisCardPicture picture;
    s32 id;
    s32 x;

    id = NIS_getTradeCardId(type, index);
    x = 384 + (id % 36) / 6 * 20 + id / 36 * 128;
    picture.u = (x * 2) & 0xFF;
    picture.v = ((id % 6) * 40) & 0xFF;
    picture.tpage = ((x & 0x380) >> 6) | 0x80;
    if (type == 0) {
        picture.frame = NIS_CARD_ELEMENT(id);
    } else {
        picture.frame = type + 4;
    }
    if (type == 0 && index >= 0x6C) {
        picture.frame = picture.frame * -1 - 1;
    }
    if (type == 1 && (u32)(index - 0x23) < 8) {
        picture.frame++;
    }
    return picture;
}

s32 NIS_getTradeCardId(s32 type, s32 index) {
    s32 id = 0;

    switch (type) {
    case 2:
        id += NIS_OPTION_COUNT;
    case 1:
        id += NIS_DIGIMON_COUNT;
    case 0:
        id += index;
    }
    return id;
}

/* the grid of ten cards, the arrows, and the line of the card under the
   cursor */
void NIS_drawTradeGrid(NisWindow *window) {
    char text[0x20];
    char number[0x18];
    NisCardData *card;
    s32 type;
    s32 id;
    s32 x;
    s32 y;
    s32 slot;
    s32 i;

    if (NIS_TRADE_LIST->count == 0) {
        return;
    }
    for (slot = 0, i = NIS_TRADE_LIST->scroll; slot < 10; slot++, i++) {
        if (i >= NIS_TRADE_LIST->count) {
            break;
        }
        x = (slot % 5) * 50 + 0x1A;
        y = (slot / 5) * 64 + 0x38;
        if (NIS_TRADE_LIST->cursor == i) {
            NIS_TRADE_LIST->cards[i].brightness = 0x80;
            NIS_TRADE_CURSOR->x = (slot % 5) * 50 + 0x2E;
            NIS_TRADE_CURSOR->y = (slot / 5) * 64 + 0x50;
        } else if (NIS_TRADE_LIST->cards[i].brightness > 0x40) {
            if ((u8)(NIS_TRADE_LIST->cards[i].brightness -= 0x10) < 0x40) {
                NIS_TRADE_LIST->cards[i].brightness = 0x40;
            }
        }
        NIS_drawTradeCardPicture(NIS_TRADE_LIST->cards[i].picture, x, y, window->z, NIS_TRADE_LIST->cards[i].brightness);
        sprintf(text, "%dc7(%d)", NIS_TRADE_LIST->cards[i].chosen, NIS_TRADE_LIST->cards[i].owned);
        drawText(x + 8, y + 0x32, (s32)text, (NIS_TRADE_LIST->cards[i].chosen == NIS_TRADE_LIST->cards[i].max) ? 2 : 7, window->z);
    }
    KAW_drawCursor(NIS_TRADE_CURSOR);
    if (NIS_TRADE_LIST->scroll != 0) {
        drawScrollArrow(0x10E, 0x39, 4, 5, window->z);
    }
    if (NIS_TRADE_LIST->scroll + 10 < NIS_TRADE_LIST->count) {
        drawScrollArrow(0x10E, 0xAA, 6, 5, window->z);
    }
    type = NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].type;
    if (type == 0) {
        card = &NIS_DIGIMON_CARDS[NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].index];
        /* "No %d" */
        sprintf(text, "Ｎｏ %d", NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].index + 1);
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
        sprintf(text, "a%d%s", card->elementLevel >> 4, card->name);
        drawIconText(0x4E, 0xB8, 7, 1, window->z, (s32)text);
        /* "Lv %d", "HP %s" */
        sprintf(text, "Ｌｖe%d", (card->elementLevel & 0xF) + 3);
        drawIconText(0xBA, 0xB8, 7, 1, window->z, (s32)text);
        sprintf(text, "ＨＰ%s", formatSjisNumber(card->hp, 4, number));
        drawIconText(0xEA, 0xB8, 7, 1, window->z, (s32)text);
    } else {
        id = NIS_getTradeCardId(NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].type, NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].index) + 1;
        sprintf(text, "Ｎｏ %ds0　s1a5オプションカードs0　s1%s", id, (type == 1) ? NIS_OPTION_CARDS[NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].index].name : NIS_DIGIVOLVE_CARDS[NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].index].name);
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
    }
}

/* the player's name, and the kinds and copies they give */
void NIS_drawTradeTitle(NisWindow *window) {
    char text[0x80];
    char kinds[0x10];
    char copies[0x10];
    u8 kindCount;
    u8 copyCount;

    kindCount = NIS_TRADE_OFFERS[NIS_TRADE_PLAYER].kinds;
    copyCount = NIS_TRADE_OFFERS[NIS_TRADE_PLAYER].copies;
    drawIconText(0x1E, 0x17, 7, 1, window->z, (s32)PLAYER_DATA(NIS_TRADE_PLAYER).name);
    formatSjisNumber(kindCount, 1, kinds);
    formatSjisNumber(copyCount, 2, copies);
    /* " Cards to trade %s/3 kinds %s cards" */
    sprintf(text, "　交換カード　%s／３種　%s枚", kinds, copies);
    drawIconText(0x66, 0x17, 7, 0, window->z, (s32)text);
}

void NIS_drawTradeGridHelp(NisWindow *window) {
    char text[0x40];

    /* "<Circle> view <Cross> back <Triangle> done L1 - R1 +" */
    sprintf(text, "　b0見る　b2戻る　b1決定　Ｌ１－　Ｒ１＋");
    drawIconText(0x1A, 0xD1, 7, 0, window->z, (s32)text);
}

void NIS_closeTradeWindow(NisWindow *window) {
}

void NIS_doNothingInTrade(void) {
}

/* a card's picture with its frame */
void NIS_drawTradeCardPicture(NisCardPicture picture, s32 x, s32 y, s32 z, u8 brightness) {
    s32 tpage;
    s32 frame;

    tpage = picture.tpage;
    frame = picture.frame;
    picture.frame = 0x28;
    picture.tpage = 0x28;
    drawTexturedSprite(x, y + 3, (Rect16 *)&picture, tpage, ((frame + 0xF9) << 6) | 0x18, z, brightness, -1);
    if (frame >= 6) {
        frame = 5;
    }
    picture.u = 0;
    picture.v = 0xBD;
    picture.tpage = 0x28;
    picture.frame = 0x30;
    drawTexturedSprite(x, y, (Rect16 *)&picture, 0x17, ((frame + 0x1C0) << 6) | 0x1D, z, brightness, -1);
}

s32 D_801FBE30 = 0xFF00; /* unused */

/* the names of the elements, of the Sevens' effects and of the option
   cards' kinds (defined here, so that their text follows the functions
   above in .rodata) */
char *NIS_TRADE_ELEMENT_NAMES[5] = {
    "火炎",
    "氷水",
    "自然",
    "暗黒",
    "珍種",
};
char *NIS_TRADE_SEVENS_EFFECT_NAMES[5] = {
    "色、必要進化ポイント無視",
    "２段階進化",
    "進化ポイント＋５０",
    "同世代の交換",
    "１段階退化",
};
char *NIS_TRADE_KIND_NAMES[2] = {
    "戦闘用",
    "進化用",
};

/* keeps the copies chosen on the card screen */
void NIS_keepTradeCopies(void) {
    NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].chosen = NIS_DECK_EDIT.copies;
    NIS_countTradeOffer();
}

/* the trade's own card screen: Circle keeps the copies, Cross puts them
   back */
void NIS_showTradeCard(s32 arg) {
    if (NIS_TRADE_VIEW.type == 0) {
        NIS_openTradeScene(NIS_DIGIMON_CARDS[NIS_TRADE_VIEW.index].unkD3);
    }
    NIS_DEBUG_NAME_TASK(0, "WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &NIS_TRADE_CARD_WINDOW, getCurrentTaskId(), 0, 0);
    NIS_TRADE_WINDOWS.card = waitFrames(0x7FFFFFFF);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (PAD_STATES[D_801E46E9]->rawRepeat & PAD_CROSS) {
            break;
        }
        if (PAD_STATES[D_801E46E9]->rawRepeat & PAD_CIRCLE) {
            NIS_keepTradeCopies();
            break;
        }
        NIS_changeTradeCopies();
    }
    NIS_WINDOW(NIS_TRADE_WINDOWS.card)->state = 4;
    waitFrames(0x1E);
    NIS_closeTradeScene();
    NIS_DEBUG_NAME_TASK(0, "SELECT");
    spawnTask(0, -1, 0, 0x800, NIS_runTradeGrid, arg, 0, 0, 0);
    exitTask();
}

/* Left takes a copy out, Right puts one in; returns 1 if it did */
s32 NIS_changeTradeCopies(void) {
    TradeView *view = &NIS_TRADE_VIEW;

    if ((PAD_STATES[D_801E46E9]->rawRepeat & PAD_LEFT) && view->copies > 0) {
        view->copies--;
        NIS_writeTradeCopies(-1);
        return 1;
    }
    if ((PAD_STATES[D_801E46E9]->rawRepeat & PAD_RIGHT) && view->copies < view->max) {
        view->copies++;
        NIS_writeTradeCopies(1);
        return 1;
    }
    return 0;
}

/* adds to the copies shown and writes them into VRAM */
void NIS_writeTradeCopies(s32 delta) {
    Rect16 rect = { 0x3D5, 0xF4 };
    char text[8];

    NIS_TRADE_VIEW.shown += delta;
    formatSjisNumber(NIS_TRADE_VIEW.shown, 2, text);
    uploadKanjiString(text, &rect);
}

/* the Digimon's model on its wire grid, drawn into a corner of VRAM */
void NIS_openTradeScene(s32 modelId) {
    SetDefDrawEnv(&NIS_TRADE_SCENE_DRAWENVS[0], 0x140, 0x100, 0x60, 0x80);
    SetDefDrawEnv(&NIS_TRADE_SCENE_DRAWENVS[1], 0x140, 0x180, 0x60, 0x80);
    NIS_TRADE_SCENE_DRAWENVS[0].ofs[0] -= 0x70;
    NIS_TRADE_SCENE_DRAWENVS[1].ofs[0] -= 0x70;
    NIS_TRADE_SCENE_DRAWENVS[0].ofs[1] -= 0x14;
    NIS_TRADE_SCENE_DRAWENVS[1].ofs[1] -= 0x14;
    SetDrawEnv(&NIS_TRADE_SCENE_ENVS[0], &NIS_TRADE_SCENE_DRAWENVS[0]);
    SetDrawEnv(&NIS_TRADE_SCENE_ENVS[1], &NIS_TRADE_SCENE_DRAWENVS[1]);
    initScene3D(1);
    addFrameCallback((s32)NIS_renderTradeScene);
    createWireGrid(0x1F4, 0x1F4, 0xB, 0xB, 0, 0);
    loadOmdModelFromDisc(0, modelId, -1);
    SCENE_3D->modelState[0] = 1;
    spawnTask(0x19, -1, 0, 0x800, runSceneCameraTask, 2);
    spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
}

void NIS_renderTradeScene(FrameBuffer *fb, s32 buffer) {
    buffer ^= 1;
    fillVramRect(NIS_TRADE_SCENE_DRAWENVS[buffer].clip[0], NIS_TRADE_SCENE_DRAWENVS[buffer].clip[1], 0x180, 0x80, 0);
    AddPrim((s32 *)&fb->ot[0xFFF], (s32)&NIS_TRADE_SCENE_ENVS[buffer]);
    SetDrawEnv(&NIS_TRADE_SCREEN_ENVS[buffer], &fb->draw);
    AddPrim((s32 *)&fb->ot[0x12C], (s32)&NIS_TRADE_SCREEN_ENVS[buffer]);
}

void NIS_closeTradeScene(void) {
    if (NIS_TRADE_VIEW.type == 0) {
        endTask(0x19);
        endTask(0x1B);
        removeFrameCallback((s32)NIS_renderTradeScene);
        removeFrameCallback((s32)renderSceneModels);
        removeFrameCallback((s32)renderWireGrid);
        waitFrames(4);
        unloadAllModels();
        freeHeapBlocksByTag(0x7F);
    }
}

/* a card's details: a Digimon's level, element, HP, DP, attacks and
   support effect with its model turning, or an option card's text; and the
   copies to trade */
void NIS_drawTradeCardDetails(NisWindow *window) {
    Rect16 rect;
    char text[8];
    char title[0x48];
    char number[0x10];
    NisCardPicture picture;
    s32 icon;
    s32 id;
    s32 i;

    if (NIS_TRADE_VIEW.type == 0) {
        rect.x = 0x1A;
        rect.y = 0x36;
        rect.w = 0;
        rect.h = 0;
        /* " Digimon card  Element %s  No%s" */
        sprintf(title, " a%dデジモンカード　　属性　%s　Ｎｏ%s", NIS_DIGIMON_CARDS[NIS_TRADE_VIEW.index].elementLevel >> 4, NIS_TRADE_ELEMENT_NAMES[NIS_DIGIMON_CARDS[NIS_TRADE_VIEW.index].elementLevel >> 4], formatSjisNumber(NIS_TRADE_VIEW.index + 1, 3, number));
        drawIconText(rect.x, rect.y, 4, 1, window->z, (s32)title);
        rect.y += 0x10;
        drawIconText(rect.x + 0x18, rect.y, 4, 1, window->z, (s32)NIS_DIGIMON_CARDS[NIS_TRADE_VIEW.index].name);
        sprintf(text, "Ｌｖ　e%d", (NIS_DIGIMON_CARDS[NIS_TRADE_VIEW.index].elementLevel & 0xF) + 3);
        drawIconText(rect.x + 0x78, rect.y, 4, 1, window->z, (s32)text);
        rect.y += 0xC;
        sprintf(text, "ＨＰ　　%s", formatSjisNumber(NIS_DIGIMON_CARDS[NIS_TRADE_VIEW.index].hp, 4, number));
        drawIconText(rect.x + 0x18, rect.y, 4, 1, window->z, (s32)text);
        rect.y += 0xC;
        sprintf(text, "進化Ｐ　%s", formatSjisNumber(NIS_DIGIMON_CARDS[NIS_TRADE_VIEW.index].dpCost, 2, number));
        drawIconText(rect.x + 0x18, rect.y, 4, 1, window->z, (s32)text);
        sprintf(text, "ＰＷ　%s", formatSjisNumber(NIS_DIGIMON_CARDS[NIS_TRADE_VIEW.index].dpBonus, 2, number));
        drawIconText(rect.x + 0x78, rect.y, 4, 1, window->z, (s32)text);
        rect.y += 0xC;
        for (i = 0; i < 3; i++, rect.y += 0xC) {
            sprintf(text, "　b%d%s", i, NIS_DIGIMON_CARDS[NIS_TRADE_VIEW.index].attacks[i].name);
            drawIconText(rect.x + 0x18, rect.y, 4, 1, window->z, (s32)text);
            drawIconText(rect.x + 0x90, rect.y, 4, 1, window->z, (s32)formatSjisNumber(NIS_DIGIMON_CARDS[NIS_TRADE_VIEW.index].attacks[i].power, 3, number));
        }
        rect.y += 0x12;
        drawIconText(rect.x + 0x48, rect.y, 4, 1, window->z, (s32)"援護能力");
        rect.y += 0xC;
        for (i = 0; i < 4; i++, rect.y += 0xC) {
            drawIconText(rect.x + 0x48, rect.y, 4, 1, window->z, (s32)NIS_DIGIMON_CARDS[NIS_TRADE_VIEW.index].supportText[i]);
        }
        /* the model, drawn into VRAM by the scene */
        rect.x = 0;
        rect.y = FRAME_BUFFER_INDEX << 7;
        rect.w = 0x60;
        rect.h = 0x80;
        drawPageSprite(0x20, 0x60, (s32)&rect, 0x115, 0, window->z);
        ((Graphics *)&GRAPHICS)->rotY += 8;
    } else {
        id = NIS_getTradeCardId(NIS_TRADE_VIEW.type, NIS_TRADE_VIEW.index);
        icon = NIS_TRADE_VIEW.type + 4;
        rect.x = 0x1A;
        rect.y = 0x36;
        rect.w = 0;
        rect.h = 0;
        /* "Option card  %s  No%s" */
        sprintf(text, "a%dオプションカード　　%s　　Ｎｏ%s", icon, NIS_TRADE_KIND_NAMES[NIS_TRADE_VIEW.type - 1], formatSjisNumber(id + 1, 3, number));
        drawIconText(rect.x, rect.y, 4, 1, window->z, (s32)text);
        rect.y += 0x10;
        drawIconText(rect.x + 0x18, rect.y, 4, 1, window->z, (s32)((NIS_TRADE_VIEW.type == 1) ? NIS_OPTION_CARDS[NIS_TRADE_VIEW.index].name : NIS_DIGIVOLVE_CARDS[NIS_TRADE_VIEW.index].name));
        rect.y += 0x18;
        for (i = 0; i < 4; i++, rect.y += 0xC) {
            /* every line shows the first one */
            drawIconText(rect.x + 0x18, rect.y, 4, 1, window->z, (s32)((NIS_TRADE_VIEW.type == 1) ? NIS_OPTION_CARDS[NIS_TRADE_VIEW.index].text[0] : NIS_DIGIVOLVE_CARDS[NIS_TRADE_VIEW.index].text[0]));
        }
    }
    picture = NIS_getTradeCardPicture(NIS_TRADE_VIEW.type, NIS_TRADE_VIEW.index);
    NIS_drawTradeCardPicture(picture, 0xEA, 0x52, window->z, 0x80);
    /* "Copies to trade" */
    drawIconText(0xE6, 0x99, 7, 1, window->z, (s32)"交換数");
    sprintf(text, "b%d", NIS_TRADE_VIEW.copies);
    drawText(0xF2, 0xAE, (s32)text, (NIS_TRADE_VIEW.copies == NIS_TRADE_VIEW.max) ? 2 : 7, window->z);
    sprintf(text, "(%d)", NIS_TRADE_VIEW.owned);
    drawText(0x109, 0xC0, (s32)text, 7, window->z);
    if (NIS_TRADE_VIEW.copies != 0) {
        drawScrollArrow(0xE6, 0xB1, 0, 5, window->z);
    }
    if (NIS_TRADE_VIEW.copies < NIS_TRADE_VIEW.max) {
        drawScrollArrow(0x106, 0xB1, 2, 5, window->z);
    }
    rect.x = 0xE0;
    rect.y = 0x96;
    rect.w = 0x3C;
    rect.h = 0x3C;
    drawWindowFrame(&rect, 0, 0, 1, 0xFF, (CVECTOR *)window->frame, window->z);
}

/* the cards the player can trade: those not in a deck, as long as the
   other player can take them */
void NIS_buildTradeList(u8 player) {
    s8 digimon[NIS_DIGIMON_COUNT];
    s8 options[NIS_OPTION_COUNT];
    s8 others[NIS_OTHER_COUNT];
    s8 ownedDigimon[NIS_DIGIMON_COUNT];
    s8 ownedOptions[NIS_OPTION_COUNT];
    s8 ownedOthers[NIS_OTHER_COUNT];
    NisCardPicture picture;
    TradeCardList *list;
    TradeOffer *offer;
    PlayerProfile *mine;
    PlayerProfile *theirs;
    s16 count;
    s16 i;
    s16 j;

    list = NIS_TRADE_LIST;
    offer = &NIS_TRADE_OFFERS[player];
    if (player == 0) {
        mine = &PLAYER_DATA(0);
        theirs = mine + 1;
    } else {
        mine = &PLAYER_DATA(1);
        theirs = &PLAYER_DATA(0);
    }
    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        ownedDigimon[i] = digimon[i] = mine->cardCollection[i] & 0xF;
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        ownedOptions[i] = options[i] = mine->optionCollection[i] & 0xF;
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        ownedOthers[i] = others[i] = mine->digivolveCollection[i] & 0xF;
    }
    NIS_removeDeckCopies(mine, digimon, options, others);
    NIS_capReceivedCopies(theirs, digimon, options, others);
    NIS_limitToNewSerials(mine, theirs, &NIS_TRADE_MASKS[player * 2], digimon, options, others);
    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        if (NIS_DIGIMON_CARDS[i].id == 0) {
            digimon[i] = 0;
        }
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        if (NIS_OPTION_CARDS[i].id == 0) {
            options[i] = 0;
        }
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        if (NIS_DIGIVOLVE_CARDS[i].id == 0) {
            others[i] = 0;
        }
    }
    count = 0;
    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        if (digimon[i] > 0) {
            list->cards[count].type = 0;
            list->cards[count].index = i;
            picture = NIS_getTradeCardPicture(0, i);
            list->cards[count].picture = picture;
            list->cards[count].brightness = 0x40;
            list->cards[count].owned = digimon[i];
            list->cards[count].max = digimon[i];
            list->cards[count].chosen = 0;
            if (offer->kinds != 0) {
                for (j = 0; j < offer->kinds; j++) {
                    if (offer->cards[j].type == 0 && offer->cards[j].index == i) {
                        list->cards[count].chosen = offer->cards[j].chosen;
                    }
                }
            }
            count++;
        }
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        if (options[i] > 0) {
            list->cards[count].type = 1;
            list->cards[count].index = i;
            picture = NIS_getTradeCardPicture(1, i);
            list->cards[count].picture = picture;
            list->cards[count].brightness = 0x40;
            list->cards[count].owned = options[i];
            list->cards[count].max = options[i];
            list->cards[count].chosen = 0;
            if (offer->kinds != 0) {
                for (j = 0; j < offer->kinds; j++) {
                    if (offer->cards[j].type == 1 && offer->cards[j].index == i) {
                        list->cards[count].chosen = offer->cards[j].chosen;
                    }
                }
            }
            count++;
        }
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        if (others[i] > 0) {
            list->cards[count].type = 2;
            list->cards[count].index = i;
            picture = NIS_getTradeCardPicture(2, i);
            list->cards[count].picture = picture;
            list->cards[count].brightness = 0x40;
            list->cards[count].owned = others[i];
            list->cards[count].max = others[i];
            list->cards[count].chosen = 0;
            if (offer->kinds != 0) {
                for (j = 0; j < offer->kinds; j++) {
                    if (offer->cards[j].type == 2 && offer->cards[j].index == i) {
                        list->cards[count].chosen = offer->cards[j].chosen;
                    }
                }
            }
            count++;
        }
    }
    list->count = count;
}

/* takes out of the counts the copies the player's decks use: for each card,
   as many as the deck that uses it most */
void NIS_removeDeckCopies(PlayerProfile *profile, s8 *digimon, s8 *options, s8 *others) {
    u8 inDecks[3][3][150];
    u8 most;
    s16 i;
    s16 j;
    s16 k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 150; k++) {
                inDecks[i][j][k] = 0;
            }
        }
    }
    for (j = 0; j < 3; j++) {
        if (profile->savedDecks[j].inUse) {
            for (i = 0; i < 30; i++) {
                inDecks[j][profile->savedDecks[j].cards[i].type][profile->savedDecks[j].cards[i].index]++;
            }
        }
    }
    for (j = 0; j < 150; j++) {
        for (i = 0; i < 3; i++) {
            most = inDecks[0][i][j];
            for (k = 1; k < 3; k++) {
                if (most < inDecks[k][i][j]) {
                    most = inDecks[k][i][j];
                }
            }
            inDecks[0][i][j] = most;
        }
    }
    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        digimon[i] -= inDecks[0][0][i];
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        options[i] -= inDecks[0][1][i];
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        others[i] -= inDecks[0][2][i];
    }
}

/* lowers the counts so that the other player ends with at most 8 copies */
void NIS_capReceivedCopies(PlayerProfile *profile, s8 *digimon, s8 *options, s8 *others) {
    u8 ownedDigimon[NIS_DIGIMON_COUNT];
    u8 ownedOptions[NIS_OPTION_COUNT];
    u8 ownedOthers[NIS_OTHER_COUNT];
    s16 i;

    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        ownedDigimon[i] = profile->cardCollection[i] & 0xF;
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        ownedOptions[i] = profile->optionCollection[i] & 0xF;
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        ownedOthers[i] = profile->digivolveCollection[i] & 0xF;
    }
    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        if (digimon[i] > 0 && ownedDigimon[i] + digimon[i] >= 9) {
            digimon[i] = 8 - ownedDigimon[i];
        }
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        if (options[i] > 0 && ownedOptions[i] + options[i] >= 9) {
            options[i] = 8 - ownedOptions[i];
        }
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        if (others[i] > 0 && ownedOthers[i] + others[i] >= 9) {
            others[i] = 8 - ownedOthers[i];
        }
    }
}

/* lowers the counts to the copies whose serial the other player doesn't
   have, for both players' masks */
void NIS_limitToNewSerials(PlayerProfile *mine, PlayerProfile *theirs, TradeMasks *masks, s8 *digimon, s8 *options, s8 *others) {
    s16 i;
    s16 bit;
    u8 count;

    NIS_findNewSerials(mine, theirs, masks->digimon, masks->options, masks->others);
    if (NIS_TRADE_PLAYER == 0) {
        NIS_findNewSerials(theirs, mine, masks[2].digimon, masks[2].options, masks[2].others);
    } else {
        NIS_findNewSerials(theirs, mine, masks[-2].digimon, masks[-2].options, masks[-2].others);
    }
    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        count = 0;
        for (bit = 0; bit < 8; bit++) {
            if ((masks->digimon[i] >> bit) & 1) {
                count++;
            }
        }
        if (count < digimon[i]) {
            digimon[i] = count;
        }
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        count = 0;
        for (bit = 0; bit < 8; bit++) {
            if ((masks->options[i] >> bit) & 1) {
                count++;
            }
        }
        if (count < options[i]) {
            options[i] = count;
        }
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        count = 0;
        for (bit = 0; bit < 8; bit++) {
            if ((masks->others[i] >> bit) & 1) {
                count++;
            }
        }
        if (count < others[i]) {
            others[i] = count;
        }
    }
}

void NIS_findNewSerials(PlayerProfile *mine, PlayerProfile *theirs, u8 *digimonMasks, u8 *optionMasks, u8 *otherMasks) {
    s16 i;

    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        NIS_findNewCardSerials(0, i, mine, theirs, &digimonMasks[i]);
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        NIS_findNewCardSerials(1, i, mine, theirs, &optionMasks[i]);
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        NIS_findNewCardSerials(2, i, mine, theirs, &otherMasks[i]);
    }
}

/* bit n of the mask: the player's copy n of the card has a serial the other
   player doesn't have */
void NIS_findNewCardSerials(u8 type, u8 index, PlayerProfile *mine, PlayerProfile *theirs, u8 *mask) {
    u16 *mySerials = NULL;
    u16 *theirSerials = NULL;
    u8 myCount = 0;
    u8 theirCount = 0;
    u8 same;
    s16 i;
    s16 j;

    *mask = 0;
    switch (type) {
    case 0:
        mySerials = mine->cardSerials[index];
        theirSerials = theirs->cardSerials[index];
        myCount = mine->cardCollection[index] & 0xF;
        theirCount = theirs->cardCollection[index] & 0xF;
        break;
    case 1:
        mySerials = mine->optionSerials[index];
        theirSerials = theirs->optionSerials[index];
        myCount = mine->optionCollection[index] & 0xF;
        theirCount = theirs->optionCollection[index] & 0xF;
        break;
    case 2:
        mySerials = mine->digivolveSerials[index];
        theirSerials = theirs->digivolveSerials[index];
        myCount = mine->digivolveCollection[index] & 0xF;
        theirCount = theirs->digivolveCollection[index] & 0xF;
        break;
    }
    for (i = 0; i < myCount; i++) {
        same = 0;
        for (j = 0; j < theirCount; j++) {
            if (mySerials[i] == theirSerials[j]) {
                same++;
            }
        }
        if (same == 0) {
            *mask |= 1 << i;
        }
    }
}

/* both offers change hands */
void NIS_exchangeOffers(void) {
    s16 i;
    s16 j;

    for (i = 0; i < NIS_TRADE_OFFERS[0].kinds; i++) {
        for (j = 0; j < NIS_TRADE_OFFERS[0].cards[i].chosen; j++) {
            NIS_giveCardCopy(NIS_TRADE_OFFERS[0].cards[i].type, NIS_TRADE_OFFERS[0].cards[i].index, &PLAYER_DATA(0), &PLAYER_DATA(1), NIS_TRADE_MASKS);
            PLAYER_DATA(1).hasTraded = 1;
        }
    }
    for (i = 0; i < NIS_TRADE_OFFERS[1].kinds; i++) {
        for (j = 0; j < NIS_TRADE_OFFERS[1].cards[i].chosen; j++) {
            NIS_giveCardCopy(NIS_TRADE_OFFERS[1].cards[i].type, NIS_TRADE_OFFERS[1].cards[i].index, &PLAYER_DATA(1), &PLAYER_DATA(0), &NIS_TRADE_MASKS[2]);
            PLAYER_DATA(0).hasTraded = 1;
        }
    }
    for (j = 0; j < 2; j++) {
        NIS_TRADE_OFFERS[j].kinds = 0;
        NIS_TRADE_OFFERS[j].copies = 0;
    }
}

/* gives the first tradeable copy of a card, with its serial number */
void NIS_giveCardCopy(u8 type, u8 index, PlayerProfile *from, PlayerProfile *to, TradeMasks *masks) {
    s16 i;
    s16 j;
    u8 n;
    u8 bits;

    switch (type) {
    case 0:
        for (i = 0; i < 8; i++) {
            if ((masks->digimon[index] >> i) & 1) {
                to->cardSerials[0][index * 8 + (to->cardCollection[index] & 0xF)] = from->cardSerials[0][index * 8 + i];
                from->cardCollection[index]--;
                to->cardCollection[index] = (to->cardCollection[index] + 1) | 0x80;
                for (j = i; j < 7; j++) {
                    from->cardSerials[0][index * 8 + j] = from->cardSerials[0][index * 8 + (j + 1)];
                }
                bits = 0;
                n = 0;
                for (j = 0; j < 8; j++) {
                    if (j != i) {
                        if ((masks->digimon[index] >> j) & 1) {
                            bits |= 1 << n;
                        }
                        n++;
                    }
                }
                masks->digimon[index] = bits;
                return;
            }
        }
        break;
    case 1:
        for (i = 0; i < 8; i++) {
            if ((masks->options[index] >> i) & 1) {
                to->optionSerials[0][index * 8 + (to->optionCollection[index] & 0xF)] = from->optionSerials[0][index * 8 + i];
                from->optionCollection[index]--;
                to->optionCollection[index] = (to->optionCollection[index] + 1) | 0x80;
                for (j = i; j < 7; j++) {
                    from->optionSerials[0][index * 8 + j] = from->optionSerials[0][index * 8 + (j + 1)];
                }
                bits = 0;
                n = 0;
                for (j = 0; j < 8; j++) {
                    if (j != i) {
                        if ((masks->options[index] >> j) & 1) {
                            bits |= 1 << n;
                        }
                        n++;
                    }
                }
                masks->options[index] = bits;
                return;
            }
        }
        break;
    case 2:
        for (i = 0; i < 8; i++) {
            if ((masks->others[index] >> i) & 1) {
                to->digivolveSerials[0][index * 8 + (to->digivolveCollection[index] & 0xF)] = from->digivolveSerials[0][index * 8 + i];
                from->digivolveCollection[index]--;
                to->digivolveCollection[index] = (to->digivolveCollection[index] + 1) | 0x80;
                for (j = i; j < 7; j++) {
                    from->digivolveSerials[0][index * 8 + j] = from->digivolveSerials[0][index * 8 + (j + 1)];
                }
                bits = 0;
                n = 0;
                for (j = 0; j < 8; j++) {
                    if (j != i) {
                        if ((masks->others[index] >> j) & 1) {
                            bits |= 1 << n;
                        }
                        n++;
                    }
                }
                masks->others[index] = bits;
                return;
            }
        }
        break;
    }
}

/* counts the kinds chosen and keeps the copies in the offer; more than
   three kinds takes back the copy just chosen and returns 1 */
s16 NIS_countTradeOffer(void) {
    u8 kinds = 0;
    s16 i;

    for (i = 0; i < NIS_TRADE_LIST->count; i++) {
        if (NIS_TRADE_LIST->cards[i].chosen != 0) {
            kinds++;
        }
    }
    if (kinds >= 4) {
        NIS_TRADE_LIST->cards[NIS_TRADE_LIST->cursor].chosen = 0;
        return 1;
    }
    NIS_TRADE_OFFERS[NIS_TRADE_PLAYER].kinds = kinds;
    NIS_TRADE_OFFERS[NIS_TRADE_PLAYER].copies = 0;
    for (i = 0; i < NIS_TRADE_LIST->count; i++) {
        if (NIS_TRADE_LIST->cards[i].chosen != 0) {
            NIS_TRADE_OFFERS[NIS_TRADE_PLAYER].copies += NIS_TRADE_LIST->cards[i].chosen;
        }
    }
    return 0;
}

/* puts the cards chosen in the player's offer */
void NIS_setTradeOffer(void) {
    u8 n = 0;
    s16 i;

    for (i = 0; i < NIS_TRADE_LIST->count; i++) {
        if (NIS_TRADE_LIST->cards[i].chosen != 0) {
            NIS_TRADE_OFFERS[NIS_TRADE_PLAYER].cards[n] = NIS_TRADE_LIST->cards[i];
            NIS_TRADE_OFFERS[NIS_TRADE_PLAYER].cards[n].brightness = 0xFF;
            n++;
        }
    }
}

/* both offers and "Trade?" (or "No card chosen!"): yes trades them */
void NIS_runTradeQuestion(void) {
    NisMenu menu;

    openChoiceMenu(&menu, 0x32, 0x32, NULL, NULL);
    if (NIS_TRADE_OFFERS[0].kinds == 0 && NIS_TRADE_OFFERS[1].kinds == 0) {
        addChoiceMenuItem(&menu, 0x35, NIS_runTradeMenu);
    } else {
        addChoiceMenuItem(&menu, 0x35, NIS_runTradeMenu);
        addChoiceMenuItem(&menu, 0x36, NIS_runTradeMenu);
    }
    NIS_DEBUG_NAME_TASK(0, "WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &NIS_TRADE_QUESTION_WINDOW, getCurrentTaskId(), 0, 0);
    NIS_TRADE_WINDOWS.question = waitFrames(0x7FFFFFFF);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        NIS_WINDOW(NIS_TRADE_WINDOWS.question)->state = 4;
        NIS_WINDOW(NIS_TRADE_WINDOWS.offers)->state = 4;
        waitFrames(0x1E);
        switch (menu.choice) {
        case 0:
            NIS_exchangeOffers();
            break;
        case 1: /* no */
            break;
        }
        startChoiceMenuAction(&menu);
    }
}

/* both players' names and offers */
void NIS_drawTradeOffers(NisWindow *window) {
    char lines[2][0x80];
    char kinds[2][0xC];
    char copies[2][0xC];
    char text[0x20];
    TradeOffer *offer;
    s32 x;
    s32 y;
    s32 rowH;
    s16 i;
    s16 k;

    offer = NIS_TRADE_OFFERS;
    x = 0x93;
    y = 0x32;
    rowH = 0x4E;
    for (i = 0; i < 2; i++) {
        drawIconText(x - 4, y + i * rowH, 7, 1, window->z, (s32)PLAYER_DATA(i).name);
        formatSjisNumber(NIS_TRADE_OFFERS[i].kinds, 1, kinds[i]);
        formatSjisNumber(NIS_TRADE_OFFERS[i].copies, 2, copies[i]);
        /* "%s kinds %s cards" */
        sprintf(lines[i], "%s種%s枚", kinds[i], copies[i]);
        drawIconText(x + 0x44, y + i * rowH, 7, 0, window->z, (s32)lines[i]);
    }
    y += 0xD;
    for (k = 0; k < 2; k++, y += rowH) {
        x = 0x8C;
        for (i = 0; i < offer->kinds; i++, x += 0x32) {
            NIS_drawTradeCardPicture(offer->cards[i].picture, x, y, window->z, 0x80);
            sprintf(text, "%dc7(%d)", offer->cards[i].chosen, offer->cards[i].owned);
            drawText(x + 8, y + 0x32, (s32)text, (offer->cards[i].chosen == offer->cards[i].max) ? 2 : 7, window->z);
        }
        offer++;
    }
}

/* "Trade with these?", or "No card is chosen!" */
void NIS_drawTradeQuestion(NisWindow *window) {
    char text[0x40];

    if (NIS_TRADE_OFFERS[0].kinds == 0 && NIS_TRADE_OFFERS[1].kinds == 0) {
        sprintf(text, " カードが１枚も選択されていません！");
    } else {
        sprintf(text, " トレードしますがよろしいですか？");
    }
    drawIconText(0x1A, 0xD9, 7, 0, window->z, (s32)text);
}

#if JP_DEBUG_BUILD
/* the debug build's test profile for a player (unused): its name, the
   copies of every card, each with a random serial number the card's other
   copies don't have, and a first deck of 30 random Digimon */
void NIS_makeDebugProfile(u8 player, u8 copies) {
    PlayerProfile *profile;
    u16 serial;
    s16 i;
    s16 j;
    s16 k;

    profile = &PLAYER_DATA(player);
    if (player == 0) {
        strcpy(profile->name, "後藤豪太");
    } else {
        strcpy(profile->name, "菅野刺激");
    }
    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        profile->cardCollection[i] = copies;
    digimon:
        for (j = 0; j < 8; j++) {
            serial = rand();
            for (k = 0; k < j; k++) {
                if (profile->cardSerials[0][i * 8 + k] == serial) {
                    goto digimon;
                }
            }
            profile->cardSerials[0][i * 8 + j] = serial;
        }
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        profile->optionCollection[i] = copies;
    option:
        for (j = 0; j < 8; j++) {
            serial = rand();
            for (k = 0; k < j; k++) {
                if (profile->optionSerials[0][i * 8 + k] == serial) {
                    goto option;
                }
            }
            profile->optionSerials[0][i * 8 + j] = serial;
        }
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        profile->digivolveCollection[i] = copies;
    other:
        for (j = 0; j < 8; j++) {
            serial = rand();
            for (k = 0; k < j; k++) {
                if (profile->digivolveSerials[0][i * 8 + k] == serial) {
                    goto other;
                }
            }
            profile->digivolveSerials[0][i * 8 + j] = serial;
        }
    }
    profile->savedDecks[0].inUse = 1;
    /* NIS_fillRandomDeck's loop with every card a Digimon: the switch on the
       type it has just set stays, and its case labels make the slot's
       address be worked out again after rand() */
    for (i = 0; i < 30; i++) {
        profile->savedDecks[0].cards[i].type = 0;
        switch (profile->savedDecks[0].cards[i].type) {
        case 0:
            profile->savedDecks[0].cards[i].index = rand() % NIS_DIGIMON_COUNT;
            break;
        case 1:
            profile->savedDecks[0].cards[i].index = rand() % NIS_OPTION_COUNT;
            break;
        case 2:
            profile->savedDecks[0].cards[i].index = rand() % NIS_OTHER_COUNT;
            break;
        }
    }
}
#endif
