#include "common.h"
#include "game.h"
#include "dcb/open_starter.h"
#include "dcb/heap.h"
#include "dcb/card_db.h"
#include "dcb/text.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/menu.h"
#include "dcb/scroll_bg.h"
#include "dcb/openseg.h"

typedef struct {
    u16 cards[30];
    char name[0x32];
} StarterDeck;

typedef struct {
    u8 unk0[0x3D04];
    StarterDeck starters[3];
} DeckFile;

extern StarterSelect OPEN_STARTER_SELECT;
extern CursorHighlight OPEN_STARTER_CURSOR;
extern UiWindow OPEN_STARTER_WINDOW;
extern u8 *CROSS_EFFECT_SHORT_NAMES[];

/* "Is this Deck OK?": the string is followed by leftover bytes in the ROM, so it stays as data */
extern const char OPEN_STR_IS_THIS_DECK_OK[];

/* the partner cards: Veemon, Hawkmon and Armadillomon */
u8 OPEN_PARTNER_CARDS[3] = { 0xAF, 0xB6, 0xBE };

/* the cards of each starter deck: two choices for each of five slots */
s16 OPEN_STARTER_BONUS_CARDS[30] = {
    0xB, 0x74, 0x19, 0x83, 0x1C, 0x89, 0x1F, 0x8A, 0xF9, 0x102,
    0x50, 0x96, 0x60, 0xA4, 0x62, 0xA6, 0x64, 0xAB, 0xFA, 0x109,
    0x2D, 0x95, 0x3C, 0xA0, 0x41, 0xA7, 0x44, 0xA9, 0xFB, 0x103,
};

void OPEN_giveStarterDeck(s32 deck) {
    u8 *file;
    DeckFile *decks;
    s32 i;
    s32 card;

    spawnTask(0, -1, 0, 0x800, loadFileTagged, "B:\\DECK2.DEK", getCurrentTaskId(), -2);
    file = (u8 *)waitFrames(0x7FFFFFFF);
    decks = (DeckFile *)(file + 8);
    obtainPartner(0, deck);
    for (i = 0; i < 30; i++) {
        card = (&decks->starters[deck])->cards[i];
        setCardSlotFromId((u8 *)&PLAYER_DATA(0).savedDecks[0].cards[i], card);
        if (findPartnerSlot(0, card) == -1) {
            addCardToCollection(0, card, 1);
        }
    }
    strcpy((char *)PLAYER_DATA(0).savedDecks[0].name, decks->starters[deck].name);
    storeSavedDeck(0, &PLAYER_DATA(0).savedDecks[0], 0);
    PLAYER_DATA(0).opponentDeckFlags[deck + 0x8E] |= 0x8000;
    for (i = 0; i < 5; i++) {
        addCardToCollection(0, OPEN_STARTER_BONUS_CARDS[deck * 10 + i * 2 + rand() % 2], 1);
    }
    PLAYER_DATA(0).activePartner = deck;
    changeScrollingBackground(PLAYER_DATA(0).activePartner, 0x380, 0, 0x380, 0x80);
    OPEN_setPartnerObtainedFlag(deck);
    freeHeapBlock(file);
}

void OPEN_drawCardPortrait(s32 x, s32 y, s32 texX, s32 texY, s32 palette, u8 *rgb, s32 otIndex) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x + 2;
        CUR_SPRT->sp.y0 = y + 5;
        CUR_SPRT->sp.u0 = (texX % 64) * 2 + 2;
        CUR_SPRT->sp.v0 = texY % 256 + 2;
        CUR_SPRT->sp.clut = getClut(0x2C0, palette + 0xF9);
        CUR_SPRT->sp.w = 36;
        CUR_SPRT->sp.h = 36;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, texX, texY));
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0x80;
            CUR_SPRT->sp.clut = getClut(0x2C0, palette + 0xB1);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0xB);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

void OPEN_drawStarterSelect(UiWindow *window) {
    Rect16 rect;
    char text[16];
    u8 rgb[3];
    s32 i;
    s32 card;
    s32 x;
    s32 y;
    s32 z;
    s32 j;
    s32 shade;
    s32 tx;

    z = window->z;
    y = window->originY + 1;
    for (i = 0; i < 3; i++) {
        card = OPEN_PARTNER_CARDS[i];
        shade = OPEN_STARTER_SELECT.deck;
        if (shade == i) {
            shade = 0x80;
        } else {
            shade = 0x40;
        }
        rgb[0] = shade;
        rgb[1] = shade;
        rgb[2] = shade;
        x = window->originX;
        tx = x + 4;
        OPEN_drawCardPortrait(tx, y + 4, i * 20 + 0x2C0, 0, getCardSpecialty(card), rgb, z);
        tx = x + 0x30;
        drawTextColored(tx, y + 1, ((DigimonCardData *)DIGIMON_CARDS)[card].name, rgb, 7, z);
        drawIconColored(tx, y + 13, 0, 7, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].attack[0].power);
        drawTextColored(x + 0x3E, y + 13, text, rgb, 7, z);
        drawIconColored(tx, y + 0x19, 0, 8, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].attack[1].power);
        drawTextColored(x + 0x3E, y + 0x19, text, rgb, 7, z);
        drawIconColored(tx, y + 0x25, 0, 9, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].attack[2].power);
        drawTextColored(x + 0x3E, y + 0x25, text, rgb, 7, z);
        sprintf(text, "(%s)", CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)DIGIMON_CARDS)[card].crossEffect]);
        drawLargeTextColored(tx, y + 0x31, text, 7, rgb, z);
        tx = x + 0x5C;
        drawIconColored(tx, y + 0x19, 0, 0x19, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].dpBonus);
        drawTextColored(x + 0x68, y + 0x19, text, rgb, 7, z);
        drawIconColored(tx, y + 0x25, 0, 0x1A, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].hp);
        drawTextColored(x + 0x6A, y + 0x25, text, rgb, 7, z);
        tx = x + 0x88;
        drawTextColored(tx, y, "Support Effect", rgb, 6, z);
        for (j = 0; j < 4; j++) {
            drawTextColored(tx, y + 15 + j * 12, ((DigimonCardData *)DIGIMON_CARDS)[card].supportText[j], rgb, 7, z);
        }
        rect.x = tx;
        rect.y = y + 15;
        rect.w = 0x6E;
        rect.h = 0x30;
        drawWindowFrame(&rect, 0x31, 0, 0x80, 1, z);
        y += 0x48;
    }
    if (OPEN_INTRO_TEXT.page != 5) {
        if (PAD_STATES[0]->repeat & 0x1000) {
            if (OPEN_STARTER_SELECT.deck != 0) {
                playMenuSound(2);
                OPEN_STARTER_SELECT.deck--;
                scrollWindowTo((s16 *)window, 0, OPEN_STARTER_SELECT.deck * 0x48);
            }
        }
        if (PAD_STATES[0]->repeat & 0x4000) {
            if (OPEN_STARTER_SELECT.deck < 2) {
                playMenuSound(2);
                OPEN_STARTER_SELECT.deck++;
                scrollWindowTo((s16 *)window, 0, OPEN_STARTER_SELECT.deck * 0x48);
            }
        }
        if (PAD_STATES[0]->pressed & 0x40) {
            playMenuSound(1);
            OPEN_STARTER_SELECT.chosen = 1;
        }
        OPEN_INTRO_TEXT.page = OPEN_STARTER_SELECT.deck + 6;
        rect.x = window->rect.x - window->scroll[2] + 1;
        rect.y = window->rect.y - window->scroll[3] + OPEN_STARTER_SELECT.deck * 0x48;
        rect.w = 0x104;
        rect.h = 0x48;
        moveCursorHighlight(&OPEN_STARTER_CURSOR, &rect);
        drawCursorHighlight(&OPEN_STARTER_CURSOR, z);
    }
}

void OPEN_drawStarterSelectWindow(void) {
    drawWindow(&OPEN_STARTER_WINDOW, OPEN_drawStarterSelect, 1);
}

void OPEN_runStarterSelect(s32 parentTask) {
    Rect16 cursor;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];
    u32 *arc;
    s32 i;

    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\BCARD.ARC", getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\P_CARD.ARC", getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < 3; i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), i * 20 + 0x2C0, 0, -1, -1);
        DrawSync(0);
    }
    OPEN_STARTER_SELECT.chosen = 0;
    OPEN_STARTER_SELECT.deck = 0;
    rect.x = 0x1E;
    rect.y = 0x44;
    rect.w = 0x104;
    rect.h = 0x5A;
    view.x = 0;
    view.y = 0;
    view.w = 0x10E;
    view.h = 0xD8;
    openWindow(&OPEN_STARTER_WINDOW, &rect, -1, (s16 *)&view, 10, 0x81, 0x80, 0xC);
    OPEN_STARTER_WINDOW.label = (s32)"STARTER SELECT";
    OPEN_STARTER_WINDOW.labelPalette = 7;
    cursor.x = OPEN_STARTER_WINDOW.originX + 4;
    cursor.y = OPEN_STARTER_WINDOW.originY + 1;
    cursor.w = 12;
    cursor.h = 12;
    initCursorHighlight(&OPEN_STARTER_CURSOR, &cursor, (Bytes4 *)-1);
    waitFrames(FRAME_INTERVAL);
    playMenuSound(3);
    addFrameCallback((s32)OPEN_drawStarterSelectWindow);
    OPEN_INTRO_TEXT.waitInput = 1;
    do {
#if VERSION_US
    wait:
#elif VERSION_EU
        /* eu goes back with continue: even unused, the label changes its code */
#else
#error "openseg/registration/open_starter: version not checked"
#endif
        waitFrames(FRAME_INTERVAL);
        if (OPEN_INTRO_TEXT.page == 5 && (PAD_STATES[0]->pressed & 0x40)) {
            playMenuSound(1);
            OPEN_INTRO_TEXT.waitInput = 0;
            OPEN_INTRO_TEXT.page = 6;
        }
        if (OPEN_STARTER_SELECT.chosen == 0) {
            /* both go back to the wait; the match depends on the form: each
               version's compiler needs its own to lay the loop out as the
               original does */
#if VERSION_US
            goto wait;
#elif VERSION_EU
            continue;
#else
#error "openseg/registration/open_starter: version not checked"
#endif
        }
        initDialog(dialog, OPEN_STR_IS_THIS_DECK_OK, 1);
        runDialog(dialog);
        switch ((s8)dialog[0xA5]) {
        case 1:
            break;
        case 0:
        case 2:
            OPEN_STARTER_SELECT.chosen = 0;
            break;
        }
    } while (OPEN_STARTER_SELECT.chosen == 0);
    animateWindowTo(&OPEN_STARTER_WINDOW, (Rect16 *)-1);
    playMenuSound(4);
    freeHeapBlock(arc);
    waitFrames(20);
    removeFrameCallback((s32)OPEN_drawStarterSelectWindow);
    OPEN_giveStarterDeck(OPEN_STARTER_SELECT.deck);
    resumeTask(parentTask);
}

/* the last three bytes are leftovers in the original, not zero padding, and
   not the same in every version */
#if VERSION_US
const char OPEN_STR_IS_THIS_DECK_OK[20] = "Is this Deck OK?\0\x6D\x01\x0C";
#elif VERSION_EU
const char OPEN_STR_IS_THIS_DECK_OK[20] = "Is this Deck OK?\0\0\x02\x62";
#else
#error "openseg/registration/open_starter: version not checked"
#endif
