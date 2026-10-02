#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/prim.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/nisseg.h"

/* What the deck screens' windows draw: the card grid, a card's details,
   the deck's name and counts, the questions of the auto deck and the name
   entry */

extern u8 CROSS_EFFECT_ICONS[];
extern char *CROSS_EFFECT_NAMES[];

void initWindowSprite(POLY_FT4 *poly, s32 clut, s32 mode, s32 u, s32 v, s32 w, s32 h);

extern char *NIS_ELEMENT_NAMES[];
extern char *NIS_CARD_KIND_NAMES[];
void KAW_initCursorShape(NisCursor *cursor, s32 w, s32 h, s32 d);
void func_8006689C(char *dst, char *src, s32 count);

void NIS_drawDeckSummary(s32 deck, s32 z);

/* a card of the grid: its picture with its frame, or the back of a card
   the player doesn't have */
void NIS_drawCardPicture(NisCardPicture picture, s32 x, s32 y, s32 z, u8 brightness, s8 owned) {
    s32 palette;
    s32 tint;
    s32 tpage;
    s32 frame;

    if (owned) {
        tint = 0;
        if (picture.frame < 0) {
            tint = 0xFD;
            palette = (s16)(picture.frame + 1);
            picture.frame = -palette;
        }
        tpage = picture.tpage;
        frame = picture.frame;
        picture.frame = 0x28;
        picture.tpage = 0x28;
        /* the frame's colour, or the tint of a picture turned over */
        palette = (tint == 0) ? (frame + 0xF9) << 6 : tint << 6;
        drawTexturedSprite(x, y + 3, (Rect16 *)&picture, tpage, palette | 0x18, z, brightness, -1);
        if (frame >= 6) {
            frame = 5;
        }
        picture.u = 0;
        picture.v = 0xBD;
        picture.tpage = 0x28;
        picture.frame = 0x30;
        drawTexturedSprite(x, y, (Rect16 *)&picture, 0x17, ((frame + 0x1C0) << 6) | 0x1D, z, brightness, -1);
    } else {
        picture.u = 0;
        picture.v = 0xC0;
        picture.tpage = 0x28;
        picture.frame = 0x30;
        drawTexturedSprite(x, y, (Rect16 *)&picture, 0x96, 0x7E40, z, brightness, -1);
    }
}

void NIS_drawDeckSummaryWindow(NisWindow *window) {
    Rect16 unused = { 0, 0, 0x90, 0x9E }; /* unused, but it is in the original stack frame */

    NIS_drawDeckSummary(NIS_DECK_EDIT.deck, window->z);
}

void NIS_drawDeckQuestion(NisWindow *window) {
    Rect16 unused; /* unused, but it is in the original stack frame */
    char lines[2][0x30];
    char unused2[0x28]; /* unused, but it is in the original stack frame */
    s32 i;

    switch (NIS_DECK_EDIT.unk10) {
    case 1:
        /* "Copy of the "%s" deck" / "Make the "%s" deck?" */
        sprintf(lines[0], "「%s」デックのコピー", NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].name);
        drawIconText(0x23, 0x9E, 7, 1, window->z, (s32)lines[0]);
        for (i = 0; i < 3; i++) {
            if (!NIS_PROFILE(0)->savedDecks[i].inUse) {
                break;
            }
        }
        sprintf(lines[1], "「%s」デックを作りますか？", NIS_PROFILE(0)->savedDecks[i].name);
        break;
    case 2:
        /* "Make a new "%s" deck?" */
        sprintf(lines[0], "新たに「%s」デックを", NIS_TYPED_NAME);
        drawIconText(0x23, 0x9E, 7, 1, window->z, (s32)lines[0]);
        sprintf(lines[0], "作りますか？");
        drawIconText(0x23, 0xAD, 7, 1, window->z, (s32)lines[0]);
        break;
    case 3:
        /* "Use the auto deck?" */
        drawIconText(0x23, 0x9E, 7, 1, window->z, (s32)"オートデックを使用しますか？");
        break;
    default:
        /* "Delete the "%s" deck?" */
        sprintf(lines[0], "「%s」デックを", NIS_TYPED_NAME);
        drawIconText(0x23, 0x9E, 7, 1, window->z, (s32)lines[0]);
        drawIconText(0x23, 0xAD, 7, 1, window->z, (s32)"削除しますか？");
        break;
    }
}

/* the grid of ten cards, the arrows, and the line of the card under the
   cursor */
void NIS_drawCardGrid(NisWindow *window) {
    char text[0x20];
    char number[0x18];
    NisCardData *card;
    NisCursor *cursor;
    char *name;
    s8 owned;
    s32 type;
    s32 id;
    s32 x;
    s32 y;
    s32 slot;
    s32 i;

    if (NIS_CARD_LIST->count == 0) {
        return;
    }
    for (slot = 0, i = NIS_CARD_LIST->scroll; slot < 10; slot++, i++) {
        if (i >= NIS_CARD_LIST->count) {
            break;
        }
        x = (slot % 5) * 50 + 0x1A;
        y = (slot / 5) * 64 + 0x38;
        if (NIS_CARD_LIST->cursor == i) {
            NIS_CARD_LIST->entries[i].brightness = 0x80;
            cursor = NIS_GRID_POINTER;
            cursor->x = (slot % 5) * 50 + 0x2E;
            cursor->y = (slot / 5) * 64 + 0x50;
        } else if (NIS_CARD_LIST->entries[i].brightness > 0x40) {
            if ((u8)(NIS_CARD_LIST->entries[i].brightness -= 0x10) < 0x40) {
                NIS_CARD_LIST->entries[i].brightness = 0x40;
            }
        }
        NIS_drawCardPicture(NIS_CARD_LIST->entries[i].picture, x, y, window->z, NIS_CARD_LIST->entries[i].brightness, NIS_CARD_LIST->entries[i].inDeck + NIS_CARD_LIST->entries[i].count);
        sprintf(text, "%3dc7(%d)", (s8)NIS_CARD_LIST->entries[i].inDeck, NIS_CARD_LIST->entries[i].count);
        drawText(x - 8, y + 0x32, (s32)text, ((s8)NIS_CARD_LIST->entries[i].inDeck == 4) ? 2 : 7, window->z);
    }
    KAW_drawCursor(NIS_GRID_POINTER);
    if (NIS_CARD_LIST->scroll != 0) {
        drawScrollArrow(0x10E, 0x39, 4, 5, window->z);
    }
    if (NIS_CARD_LIST->scroll + 10 < NIS_CARD_LIST->count) {
        drawScrollArrow(0x10E, 0xAA, 6, 5, window->z);
    }
    type = NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].type;
    if (type == 0) {
        card = &NIS_DIGIMON_CARDS[NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].index];
        /* "No %d" */
        sprintf(text, "Ｎｏ %d", NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].index + 1);
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
        owned = NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].inDeck + NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].count;
        if (owned == 0) {
            drawIconText(0x4E, 0xB8, 7, 1, window->z, (s32)"？？？？？？？");
        } else {
            sprintf(text, "a%d%s", card->elementLevel >> 4, card->name);
            drawIconText(0x4E, 0xB8, 7, 1, window->z, (s32)text);
            /* "Lv %d", "HP %s" */
            sprintf(text, "Ｌｖe%d", (card->elementLevel & 0xF) + 3);
            drawIconText(0xBA, 0xB8, 7, 1, window->z, (s32)text);
            sprintf(text, "ＨＰ%s", formatSjisNumber(card->hp, 4, number));
            drawIconText(0xEA, 0xB8, 7, 1, window->z, (s32)text);
        }
    } else {
        id = NIS_getCardId(NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].type, NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].index) + 1;
        owned = NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].inDeck + NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].count;
        if (owned == 0) {
            sprintf(text, "Ｎｏ %ds0　s1？？？？？？？", id);
        } else {
            if (type == 1) {
                name = NIS_OPTION_CARDS[NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].index].name;
            } else {
                name = NIS_DIGIVOLVE_CARDS[NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].index].name;
            }
            sprintf(text, "Ｎｏ %ds0　s1a5オプションカードs0　s1%s", id, name);
        }
        drawIconText(0x1E, 0xB8, 7, 1, window->z, (s32)text);
    }
}

void NIS_drawCardGridHelp(NisWindow *window) {
    char text[0x88];
    char unused[0x10];

    /* "<Circle> view <Cross> back L1 - R1 +" */
    sprintf(text, "b0見る b2戻る Ｌ１ － Ｒ１ ＋", unused);
    drawIconText(0x1A, 0xD1, 7, 1, window->z, (s32)text);
}

/* the card in view: a Digimon's level, element, number, HP, DP, attacks
   and support effect, or an option card's text, and the copies */
void NIS_drawCardDetails(NisWindow *window) {
    Rect16 pos;
    char text[0x48];
    char line[0x48];
    char number[0x20];
    NisCardData *card;
    NisOptionData *option;
    s32 id;
    s32 kind;
    s32 level;
    s32 i;

    if (NIS_DECK_EDIT.cardType == 0) {
        card = &NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex];
        pos.x = 0x79;
        pos.y = 0x32;
        pos.w = 0;
        pos.h = 0;
        /* "Digimon card Lv %d", "element %s No %s" */
        sprintf(line, "  a%dデジモンカード　Ｌｖ　e%d", card->elementLevel >> 4, (card->elementLevel & 0xF) + 3);
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)line);
        pos.y += 0x10;
        sprintf(line, "  属性　%s　Ｎｏ%s", NIS_ELEMENT_NAMES[NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex].elementLevel >> 4], formatSjisNumber(NIS_DECK_EDIT.cardIndex + 1, 3, number));
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)line);
        pos.y += 0xC;
        sprintf(text, "  %s", NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex].name);
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
        drawIconText(pos.x + 0x64, pos.y, 7, 1, window->z, (s32)"ＨＰ");
        sprintf(text, " w-1%4d", NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex].hp);
        drawText(pos.x + 0x78, pos.y, (s32)text, 7, window->z);
        pos.y += 0xC;
        /* "DP needed", "POW" */
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)"  必進Ｐ");
        sprintf(text, " w-1%2d", NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex].dpCost);
        drawText(pos.x + 0x30, pos.y, (s32)text, 7, window->z);
        drawIconText(pos.x + 0x64, pos.y, 7, 1, window->z, (s32)"ＰＯＷ");
        sprintf(text, " w-1%4d", NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex].dpBonus);
        drawText(pos.x + 0x78, pos.y, (s32)text, 7, window->z);
        pos.y += 0xC;
        for (i = 0; i < 3; i++, pos.y += 0xC) {
            sprintf(text, " 　b%d%s", i, NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex].attacks[i].name);
            drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
            sprintf(text, " w-1%4d", NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex].attacks[i].power);
            drawText(pos.x + 0x78, pos.y, (s32)text, 7, window->z);
        }
        if (CROSS_EFFECT_ICONS[card->supportIcon] != 0) {
            sprintf(text, "  　%s d%d", CROSS_EFFECT_NAMES[NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex].supportIcon], CROSS_EFFECT_ICONS[card->supportIcon]);
        } else {
            sprintf(text, "  　%s", CROSS_EFFECT_NAMES[NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex].supportIcon]);
        }
        drawIconText(pos.x + 1, pos.y, 7, 1, window->z, (s32)text);
        pos.y += 0x10;
        /* "support effect" */
        if (card->supportLevel == 0) {
            drawIconText(pos.x + 2, pos.y, 6, 1, window->z, (s32)" 援護能力");
        } else {
            sprintf(text, "援護能力 d%d", card->supportLevel);
            drawIconText(pos.x + 2, pos.y, 6, 1, window->z, (s32)text);
        }
        pos.y += 0xC;
        for (i = 0; i < 4; i++, pos.y += 0xC) {
            sprintf(text, " %s", NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex].supportText[i]);
            drawIconText(pos.x - 2, pos.y, 7, 1, window->z, (s32)text);
        }
    } else {
        id = NIS_getCardId(NIS_DECK_EDIT.cardType, NIS_DECK_EDIT.cardIndex);
        kind = NIS_DECK_EDIT.cardType + 4;
        pos.x = 0x7D;
        pos.y = 0x36;
        pos.w = 0;
        pos.h = 0;
        /* "option card" */
        if (NIS_DECK_EDIT.cardType == 1 && (level = (option = &NIS_OPTION_CARDS[NIS_DECK_EDIT.cardIndex])->level) != 0) {
            sprintf(text, " a%dオプションカード d%d", kind, level);
        } else {
            sprintf(text, " a%dオプションカード", kind);
        }
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
        pos.y += 0x10;
        sprintf(text, "   %s　　Ｎｏ%s", NIS_CARD_KIND_NAMES[NIS_DECK_EDIT.cardType - 1], formatSjisNumber(id + 1, 3, number));
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)text);
        pos.y += 0x10;
        drawIconText(pos.x, pos.y, 7, 1, window->z, (s32)((NIS_DECK_EDIT.cardType == 1) ? NIS_OPTION_CARDS[NIS_DECK_EDIT.cardIndex].name : NIS_DIGIVOLVE_CARDS[NIS_DECK_EDIT.cardIndex].name));
        pos.y += 0x18;
        for (i = 0; i < 4; i++, pos.y += 0xC) {
            drawIconText(pos.x + 0x18, pos.y, 7, 1, window->z, (s32)((NIS_DECK_EDIT.cardType == 1) ? NIS_OPTION_CARDS[NIS_DECK_EDIT.cardIndex].text[i] : NIS_DIGIVOLVE_CARDS[NIS_DECK_EDIT.cardIndex].text[i]));
        }
    }
    /* "copies used" in the deck, or "copies traded" */
    if (D_801E46E8 != 1) {
        drawIconText(0xEC, 0xA6, 7, 1, window->z, (s32)"使用枚数");
    } else {
        drawIconText(0xEC, 0xA6, 7, 1, window->z, (s32)"交換枚数");
    }
    sprintf(text, "b%d", NIS_DECK_EDIT.copies);
    /* colour 2 once the deck can take no more copies */
    if (D_801E46E8 != 1) {
        drawText(0xF1, 0xBA, (s32)text, (NIS_DECK_EDIT.copies >= NIS_DECK_EDIT.maxCopies || NIS_DECK_EDIT.copies >= 4) ? 2 : 8, window->z);
    } else {
        drawText(0xF1, 0xBA, (s32)text, (NIS_DECK_EDIT.copies >= NIS_DECK_EDIT.maxCopies) ? 2 : 8, window->z);
    }
    sprintf(text, "c7(%d)", NIS_DECK_EDIT.unkA);
    drawText(0x104, 0xC6, (s32)text, 8, window->z);
    pos.x = 0xEC;
    pos.y = 0xA4;
    pos.w = 0x30;
    pos.h = 0x31;
    drawWindowFrame(&pos, 0, 0, 1, 0xFF, (CVECTOR *)window->frame, window->z);
}
/* the menu of the card details: "decide the copies", "view mode", "back";
   and the card's big picture, turning over */
void NIS_drawCardCountMenu(NisWindow *window) {
    char text[0x90];
    Rect16 unused; /* unused, but it is in the original stack frame */
    POLY_FT4 *poly;
    s16 turn;
    s16 dx;
    s16 dy;
    s32 y;

    poly = NIS_CARD_IMAGE.polys[FRAME_BUFFER_INDEX];
    y = 0xB2;
    sprintf(text, "b0枚数決定 ");
    drawIconText(0x1C, y, 7, 1, window->z, (s32)text);
    if (NIS_DECK_EDIT.cardType == 0) {
        y += 0xC;
        sprintf(text, "b1ビューモード ");
        drawIconText(0x1C, y, 7, 1, window->z, (s32)text);
    }
    y += 0xC;
    sprintf(text, "b2戻る ");
    drawIconText(0x1C, y, 7, 1, window->z, (s32)text);
    if (NIS_CARD_IMAGE.loaded == 0) {
        if (NIS_CARD_IMAGE.delay-- <= 0) {
            NIS_CARD_IMAGE.delay = 0;
            if (NIS_CARD_IMAGE.back == 0) {
                NIS_CARD_IMAGE.turn++;
            } else {
                NIS_CARD_IMAGE.turn--;
            }
        }
    }
    if (NIS_CARD_IMAGE.turn <= 0) {
        NIS_CARD_IMAGE.turn = 0;
    }
    if (NIS_CARD_IMAGE.turn == 10) {
        NIS_CARD_IMAGE.back = 1;
    }
    if (NIS_CARD_IMAGE.back == 1) {
        turn = NIS_CARD_IMAGE.turn;
        initWindowSprite(poly, (NIS_CARD_IMAGE.current->clutY << 6) | ((NIS_CARD_IMAGE.current->clutX >> 4) & 0x3F), 1, NIS_CARD_IMAGE.current->x, NIS_CARD_IMAGE.current->y, 0x40, 0x40);
        dx = (turn << 5) / 10;
        dy = (NIS_CARD_IMAGE.turn * 6) / 10;
        poly->x0 = dx + 0x24;
        poly->y0 = dy + 0x4A;
        poly->x1 = 0x64 - dx;
        poly->y1 = 0x4A - dy;
        poly->x2 = dx + 0x24;
        poly->y2 = 0x8A - dy;
        poly->x3 = 0x64 - dx;
        poly->y3 = dy + 0x8A;
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[window->z], (s32)NIS_CARD_IMAGE.polys[FRAME_BUFFER_INDEX]);
        poly++;
        initWindowSprite(poly, 0x7F00, 0, 0x1A8, 0x100, 0x50, 0x74);
        dx = turn * 4;
        dy = (NIS_CARD_IMAGE.turn * 6) / 10;
        poly->x0 = dx + 0x1C;
        poly->y3 = dy + 0xAA;
        poly->x1 = 0x6C - dx;
        poly->y2 = 0xAA - dy;
        poly->x2 = dx + 0x1C;
        poly->y1 = 0x36 - dy;
        poly->x3 = 0x6C - dx;
        poly->y0 = dy + 0x36;
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[window->z], (s32)(NIS_CARD_IMAGE.polys[FRAME_BUFFER_INDEX] + 1));
    } else {
        poly += 2;
        initWindowSprite(poly, 0x7EC0, 1, 0x180, 0x100, 0x50, 0x74);
        dx = NIS_CARD_IMAGE.turn * 4;
        dy = (NIS_CARD_IMAGE.turn * 6) / 10;
        poly->x0 = dx + 0x1C;
        poly->y0 = 0x36 - dy;
        poly->x1 = 0x6C - dx;
        poly->y1 = dy + 0x36;
        poly->x2 = dx + 0x1C;
        poly->y2 = dy + 0xAA;
        poly->x3 = 0x6C - dx;
        poly->y3 = 0xAA - dy;
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[window->z], (s32)(NIS_CARD_IMAGE.polys[FRAME_BUFFER_INDEX] + 2));
    }
}

/* the wins and losses of the card in view, or of the deck */
void NIS_drawDeckRecord(NisWindow *window) {
    char text[8];

    if (NIS_CARD_IN_VIEW == 1) {
        sprintf(text, "w-1%3d", NIS_PROFILE(0)->cardWins[NIS_DECK_EDIT.cardIndex]);
        drawText(0x1E, 0x18, (s32)text, 7, window->z);
        drawIconText(0x38, 0x18, 7, 1, window->z, (s32)"勝");
        sprintf(text, "w-1%3d", NIS_PROFILE(0)->cardLosses[NIS_DECK_EDIT.cardIndex]);
    } else {
        sprintf(text, "w-1%3d", NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].wins);
        drawText(0x1E, 0x18, (s32)text, 7, window->z);
        drawIconText(0x38, 0x18, 7, 1, window->z, (s32)"勝");
        sprintf(text, "w-1%3d", NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].losses);
    }
    drawText(0x49, 0x18, (s32)text, 7, window->z);
    drawIconText(0x64, 0x18, 7, 1, window->z, (s32)"敗");
}

s32 NIS_countDeckCards(s8 deck) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < 30; i++) {
        if (NIS_PROFILE(0)->savedDecks[deck].cards[i].type < 3) {
            count++;
        }
    }
    return count;
}

/* "%s deck", "%s/30 cards" */
const char NIS_FMT_DECK_NAME[] = "%sデック";

void NIS_drawDeckName(NisWindow *window) {
    Rect16 unused;
    char count[0x20];
    char number[0x10];
    char name[0x20];
    NisDeck *deck;

    deck = &NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck];
    formatSjisNumber(NIS_countDeckCards(NIS_DECK_EDIT.deck), 2, number);
    unused.x = 0;
    unused.y = 0xF4;
    unused.w = 0xA8;
    unused.h = 0xC;
    sprintf(name, NIS_FMT_DECK_NAME, deck->name);
    drawIconText(0x7B, 0x17, 7, 1, window->z, (s32)name);
    sprintf(count, "%s／３０枚", number);
    drawIconText(0xE7, 0x17, 7, 1, window->z, (s32)count);
}

/* "A deck must have 30 cards" */
void NIS_drawDeckNotFull(NisWindow *window) {
    Rect16 unused; /* unused, but it is in the original stack frame */

    drawIconText(0x2D, 0x9E, 7, 1, window->z, (s32)"s0デックには、必ずカードを３０枚");
    drawIconText(0x2D, 0xAC, 7, 1, window->z, (s32)"s0入れてください。");
}

/* Betamon's questions for the auto deck */
void NIS_drawAutoDeckQuestion(NisWindow *window) {
    Rect16 unused; /* unused, but it is in the original stack frame */
    char lines[2][0x30];
    char elements[5][5] = { "火炎", "氷水", "自然", "暗黒", "珍種" };
    char styles[2][5] = { "攻撃", "防御" };
    char amounts[2][7] = { "多め", "少なめ" };

#if JP_DEBUG_BUILD
    /* the debug text shows the answers so far */
    sprintf(DEBUG_TEXT_LINES[0], "ATR  = %d \n", NIS_DECK_EDIT.cardType);
    sprintf(DEBUG_TEXT_LINES[1], "TYPE = %d \n", NIS_DECK_EDIT.cardIndex);
    sprintf(DEBUG_TEXT_LINES[2], "OPT  = %d \n", NIS_DECK_EDIT.unkA);
#endif
    /* "Betamon:" */
    drawIconText(0x26, 0xA2, 7, 1, window->z, (s32)"c4ベタモンc7s0w4：");
    switch (NIS_AUTO_DECK_QUESTION) {
    case 0:
        /* "Which element's Digimon should the deck be built around?" */
        drawIconText(0x26, 0xB0, 7, 1, window->z, (s32)"どの属性のデジモンを中心にデックを作る？");
        break;
    case 1:
        /* "Attack type or defense type?" */
        drawIconText(0x26, 0xB0, 7, 1, window->z, (s32)"攻撃タイプ、防御タイプどっちにする？");
        break;
    case 2:
        /* "Many option cards, or few?" */
        drawIconText(0x26, 0xB0, 7, 1, window->z, (s32)"オプションカードは多め、少なめ？");
        break;
    case 3:
        /* "%s Digimon, %s type." / "I'll put %s option cards in." / "Is this OK?" */
        sprintf(lines[0], "%s系デジモン中心で%sタイプ。", elements[NIS_DECK_EDIT.cardType], styles[NIS_DECK_EDIT.cardIndex]);
        drawIconText(0x26, 0xB0, 7, 1, window->z, (s32)lines[0]);
        sprintf(lines[1], "オプションカードは%sに入れるよ。", amounts[NIS_DECK_EDIT.unkA]);
        drawIconText(0x26, 0xBE, 7, 1, window->z, (s32)lines[1]);
        drawIconText(0x26, 0xCC, 7, 1, window->z, (s32)"これでいい？");
        break;
    case 10:
        /* "That color doesn't have enough cards yet, / so you can't pick it!" */
        drawIconText(0x26, 0xB0, 7, 1, window->z, (s32)"その色は、まだカードが足りないから");
        drawIconText(0x26, 0xBE, 7, 1, window->z, (s32)"選べないよ！");
        break;
    case 11:
        /* "There aren't enough option cards yet, / so you can't pick it!" */
        drawIconText(0x26, 0xB0, 7, 1, window->z, (s32)"オプションカードがまだ足りないから");
        drawIconText(0x26, 0xBE, 7, 1, window->z, (s32)"選べないよ！");
        break;
    }
}

void NIS_drawAutoDeckPortrait(NisWindow *window) {
    Rect16 uv;

    uv.x = 0x40;
    uv.y = 0xC0;
    uv.w = 0x40;
    uv.h = 0x38;
    drawTexturedSprite(0xE7, 0x4F, &uv, 0x96, 0x3C18, window->z, 0x80, -1);
}

/* the name being typed, with a mark under the next letter */
void NIS_drawNameField(NisWindow *window) {
    Rect16 unused; /* unused, but it is in the original stack frame */
    char text[0x30];

    sprintf(text, "s0%s", NIS_TYPED_NAME);
    drawIconText(0x20, 0x36, 7, 1, window->z, (s32)text);
    if (NIS_DECK_EDIT.cardType < 6) {
        drawIconText(NIS_DECK_EDIT.cardType * 12 + 0x20, 0x37, 7, 1, window->z, (s32)"＿");
    }
}

/* the kana table: rows of five letters, the cursor moving over them */
void NIS_drawNameEntry(NisWindow *window) {
    Rect16 unused; /* unused, but it is in the original stack frame */
    u8 rgb[4] = { 0 };

    rgb[1] = 0x40;
    {
        s32 blockX[3] = { 0, 0x18, 0x3C };
        s32 commandY[7] = { 0, 0xF, 0x1E, 0x2D, 0x3C, 0x5A, 0x69 };
        char text[0x30];
        char letter[0x30];
        char (*kana)[11];
        s8 rowOffset;
        s32 i;
        s32 y;

        kana = NIS_KANA[NIS_KANA_PAGE];
        /* i is first the block of columns the cursor is in: two of five
           kana, then the commands */
        i = (NIS_DECK_EDIT.copies >= 5) + (NIS_DECK_EDIT.copies >= 10);
        if (NIS_DECK_EDIT.copies != 0) {
            rowOffset = (s8)(NIS_DECK_EDIT.copies / 5) * 9;
        } else {
            rowOffset = 0;
        }
        if (NIS_DECK_EDIT.copies == 10) {
            if ((u8)(NIS_DECK_EDIT.unkA - 3) < 2) {
                KAW_initCursorShape(NIS_NAME_CURSOR, 0x12, 6, 4);
                NIS_NAME_CURSOR->x = NIS_DECK_EDIT.copies * 12 + blockX[i] + 0x44;
            } else {
                KAW_initCursorShape(NIS_NAME_CURSOR, 0xC, 6, 4);
                NIS_NAME_CURSOR->x = NIS_DECK_EDIT.copies * 12 + blockX[i] + 0x3E;
            }
            NIS_NAME_CURSOR->y = ((i != 2) ? NIS_DECK_EDIT.unkA * 15 : commandY[NIS_DECK_EDIT.unkA]) + 0x58;
        } else {
            func_8006689C(letter, (char *)kana + ((s8)(NIS_DECK_EDIT.copies % 5) * 2 + NIS_DECK_EDIT.unkA * 11 + rowOffset * 11), 2);
            {
                static const char fmt[] = "s0%s";

                sprintf(text, fmt, letter);
            }
            KAW_initCursorShape(NIS_NAME_CURSOR, 6, 6, 4);
            NIS_NAME_CURSOR->h = 6;
            NIS_NAME_CURSOR->w = 6;
            NIS_NAME_CURSOR->x = NIS_DECK_EDIT.copies * 12 + blockX[i] + 0x37;

            NIS_NAME_CURSOR->y = ((i != 2) ? NIS_DECK_EDIT.unkA * 15 : commandY[NIS_DECK_EDIT.unkA]) + 0x58;
        }
        KAW_drawCursor(NIS_NAME_CURSOR);
#if JP_DEBUG_BUILD
        /* and the cursor's box */
        sprintf(DEBUG_TEXT_LINES[0], "CURSOL_X %d", NIS_NAME_CURSOR->x);
        sprintf(DEBUG_TEXT_LINES[1], "CURSOL_Y %d", NIS_NAME_CURSOR->y);
        sprintf(DEBUG_TEXT_LINES[2], "CUSIZE_X %d", NIS_NAME_CURSOR->w);
        sprintf(DEBUG_TEXT_LINES[3], "CUXIZE_Y %d", NIS_NAME_CURSOR->h);
#endif
        for (i = 0, y = 0; i < 9; i++, y += 0xF) {
            sprintf(text, "s0%s", kana[i]);
            drawIconText(0x32, y + 0x52, 7, 1, window->z, (s32)text);
        }
        for (i = 9, y = 0; i < 19; i++, y += 0xF) {
            sprintf(text, "s0%s", kana[i]);
            drawIconText(0x86, y + 0x52, 7, 1, window->z, (s32)text);
        }
        /* "hiragana", "katakana", "letters", "kanji 1", "kanji 2", "back",
           "done" */
        drawIconText(0xE6, 0x52, 7, 1, window->z, (s32)"かな");
        drawIconText(0xE6, 0x61, 7, 1, window->z, (s32)"カナ");
        drawIconText(0xE6, 0x70, 7, 1, window->z, (s32)"英数");
        drawIconText(0xE6, 0x7F, 7, 1, window->z, (s32)"漢字１");
        drawIconText(0xE6, 0x8E, 7, 1, window->z, (s32)"漢字２");
        drawIconText(0xE6, 0xAC, 7, 1, window->z, (s32)"戻る");
        drawIconText(0xE6, 0xBC, 7, 1, window->z, (s32)"決定");
    }
}
/* "Copy of the "%s" deck" / "Make the "%s" deck?" */
void NIS_drawDeckCopyQuestion(NisWindow *window) {
    char lines[2][0x30];
    char unused2[0x28]; /* unused, but it is in the original stack frame */
    s32 i;

    sprintf(lines[0], "「%s」デックのコピー", NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].name);
    drawIconText(0x23, 0x9E, 7, 1, window->z, (s32)lines[0]);
    for (i = 0; i < 3; i++) {
        if (!NIS_PROFILE(0)->savedDecks[i].inUse) {
            break;
        }
    }
    sprintf(lines[1], "「%s」デックを作りますか？", NIS_TYPED_NAME);
    drawIconText(0x23, 0xAD, 7, 1, window->z, (s32)lines[1]);
}

/* returns 1 if A:\DECK.TIM couldn't be loaded */
s32 NIS_loadDeckTims(void) {
    u32 *tims;

#if JP_DEBUG_BUILD
    spawnTask(0, -1, 4, 0x800, loadFile, "A:\\DECK.TIM", getCurrentTaskId());
#else
    spawnTask(0, -1, 0, 0x800, loadFile, "A:\\DECK.TIM", getCurrentTaskId());
#endif
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    if (tims == NULL) {
#if JP_DEBUG_BUILD
        /* the debug build stops on an error task */
        NIS_DEBUG_NAME_TASK(0, NIS_STR_ERROR_TASK);
        spawnTask(0, -1, 0, 0x200, func_80019CE8, "DECK.TIM LOAD ERROR !!\n", 0, getCurrentTaskId(), 0);
        waitFrames(0x7FFFFFFF);
#endif
        return 1;
    }
    uploadTimList(tims);
    freeHeapBlock(tims);
    return 0;
}

/* adds to the deck's card count and writes it into VRAM */
void NIS_writeDeckCardCount(s32 delta) {
    Rect16 rect = { 0x3D5, 0xF4 };
    char text[8];

    NIS_DECK_EDIT.cardCount += delta;
    formatSjisNumber(NIS_DECK_EDIT.cardCount, 2, text);
    uploadKanjiString(text, &rect);
}

/* writes the deck's name and "  /30 cards" into VRAM */
void NIS_writeDeckName(s32 deck) {
    char text[0x30];
    Rect16 rect;
    NisDeck *saved;

    saved = &NIS_PROFILE(0)->savedDecks[deck];
    fillVramRect(0x3C0, 0xF4, 0x9C, 0xC, 0);
    rect.x = 0x3C0;
    rect.y = 0xF4;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString(saved->name, &rect);
    rect.x = 0x3D5;
    sprintf(text, "　　／３０枚");
    uploadKanjiString(text, &rect);
    NIS_writeDeckCardCount(0);
}

/* the copies owned of the seven "Sevens" option cards */
s32 NIS_countOwnedSevens(void) {
    s32 count = 0;
    s32 i;

    for (i = 0x23; i < 0x2A; i++) {
        count += NIS_PROFILE(0)->optionCards[i] & 0xF;
    }
    return count;
}

s32 NIS_countDeckOption(s8 deck, s32 index) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < 30; i++) {
        if (NIS_PROFILE(0)->savedDecks[deck].cards[i].type == 1 && NIS_PROFILE(0)->savedDecks[deck].cards[i].index == index) {
            count++;
        }
    }
    return count;
}

/* what NIS_drawDeckSummary draws */
const Rect16 NIS_SEVENS_ICON_UV = { 0, 0xED, 0x10, 0x10 };
const char NIS_FMT_TEXT[] = "%s";
const char NIS_STR_CARDS_BY_KIND[] = "種類別カード枚数";
const char NIS_FMT_FIRE_CARDS[] = "a0s0w-4%sw0／s0w-4%s";
const char NIS_FMT_ICE_CARDS[] = "a1s0w-4%sw0／s0w-4%s";
const char NIS_FMT_NATURE_CARDS[] = "a2s0w-4%sw0／s0w-4%s";
const char NIS_FMT_DARKNESS_CARDS[] = "a3s0w-4%sw0／s0w-4%s";
const char NIS_FMT_RARE_CARDS[] = "a4s0w-4%sw0／s0w-4%s";
const char NIS_FMT_OPTION_CARDS[] = "a5s0w-4%sw0／s0w-4%s";
const char NIS_STR_CARDS_BY_LEVEL[] = "レベル別カード枚数";
const char NIS_FMT_LEVEL_R_CARDS[] = "e3s0w-4%sw0／s0w-4%s";
const char NIS_FMT_LEVEL_C_CARDS[] = "e4s0w-4%sw0／s0w-4%s";
const char NIS_FMT_LEVEL_U_CARDS[] = "e5s0w-4%sw0／s0w-4%s";
const char NIS_FMT_ALL_CARDS[] = "総カード枚数　s0w-4%s枚";
const char NIS_FMT_SEVENS_CARDS[] = "セブンズカード　%s枚";

/* a deck's cards by kind and by level against the cards owned, the cards
   in all, the Sevens cards and which of the Sevens the deck has */
void NIS_drawDeckSummary(s32 deck, s32 z) {
    Rect16 uv = NIS_SEVENS_ICON_UV;
    char text[0x20];
    char deckCount[0x10];
    char ownedCount[8];
    char deckCount2[8];
    char ownedCount2[8];
    s32 tpage;
    NisDeck *saved;
    s32 group;
    s32 x;
    s32 i;

    tpage = (u16)GetTPage(1, 0, 0x1C0, 0x100);
    if (deck < 0) {
        return;
    }
    saved = &NIS_PROFILE(0)->savedDecks[deck];
    sprintf(text, NIS_FMT_DECK_NAME, saved->name);
    drawIconText(0x8C, 0x36, 7, 1, z, (s32)text);
    sprintf(text, NIS_FMT_TEXT, NIS_STR_CARDS_BY_KIND);
    drawIconText(0x8C, 0x46, 7, 1, z, (s32)text);
    group = deck + 1;
    formatSjisNumber(NIS_countCards(group, 0, 0, -1), 2, deckCount);
    sprintf(text, NIS_FMT_FIRE_CARDS, deckCount, formatSjisNumber(NIS_countCards(0, 0, 0, -1), 3, ownedCount));
    drawIconText(0x8D, 0x52, 7, 1, z, (s32)text);
    formatSjisNumber(NIS_countCards(group, 0, 1, -1), 2, deckCount2);
    sprintf(text, NIS_FMT_ICE_CARDS, deckCount2, formatSjisNumber(NIS_countCards(0, 0, 1, -1), 3, ownedCount2));
    drawIconText(0xDA, 0x52, 7, 1, z, (s32)text);
    formatSjisNumber(NIS_countCards(group, 0, 2, -1), 2, deckCount);
    sprintf(text, NIS_FMT_NATURE_CARDS, deckCount, formatSjisNumber(NIS_countCards(0, 0, 2, -1), 3, ownedCount));
    drawIconText(0x8D, 0x5E, 7, 1, z, (s32)text);
    formatSjisNumber(NIS_countCards(group, 0, 3, -1), 2, deckCount2);
    sprintf(text, NIS_FMT_DARKNESS_CARDS, deckCount2, formatSjisNumber(NIS_countCards(0, 0, 3, -1), 3, ownedCount2));
    drawIconText(0xDA, 0x5E, 7, 1, z, (s32)text);
    formatSjisNumber(NIS_countCards(group, 0, 4, -1), 2, deckCount);
    sprintf(text, NIS_FMT_RARE_CARDS, deckCount, formatSjisNumber(NIS_countCards(0, 0, 4, -1), 3, ownedCount));
    drawIconText(0x8D, 0x6A, 7, 1, z, (s32)text);
    formatSjisNumber(NIS_countCards(group, 1, -1, -1), 2, deckCount2);
    sprintf(text, NIS_FMT_OPTION_CARDS, deckCount2, formatSjisNumber(NIS_countCards(0, 1, -1, -1), 3, ownedCount2));
    drawIconText(0xDA, 0x6A, 7, 1, z, (s32)text);
    sprintf(text, NIS_FMT_TEXT, NIS_STR_CARDS_BY_LEVEL);
    drawIconText(0x8C, 0x7A, 7, 1, z, (s32)text);
    formatSjisNumber(NIS_countCards(group, 0, -1, 0), 2, deckCount);
    sprintf(text, NIS_FMT_LEVEL_R_CARDS, deckCount, formatSjisNumber(NIS_countCards(0, 0, -1, 0), 3, ownedCount));
    drawIconText(0x8D, 0x86, 7, 1, z, (s32)text);
    formatSjisNumber(NIS_countCards(group, 0, -1, 1), 2, deckCount2);
    sprintf(text, NIS_FMT_LEVEL_C_CARDS, deckCount2, formatSjisNumber(NIS_countCards(0, 0, -1, 1), 3, ownedCount2));
    drawIconText(0xDA, 0x86, 7, 1, z, (s32)text);
    formatSjisNumber(NIS_countCards(group, 0, -1, 2), 2, deckCount);
    sprintf(text, NIS_FMT_LEVEL_U_CARDS, deckCount, formatSjisNumber(NIS_countCards(0, 0, -1, 2), 3, ownedCount));
    drawIconText(0x8D, 0x92, 7, 1, z, (s32)text);
    formatSjisNumber(NIS_countCards(0, -1, -1, -1), 4, deckCount);
    sprintf(text, NIS_FMT_ALL_CARDS, deckCount);
    drawIconText(0x90, 0xA2, 7, 1, z, (s32)text);
    formatSjisNumber(NIS_countOwnedSevens(), 3, deckCount);
    sprintf(text, NIS_FMT_SEVENS_CARDS, deckCount);
    drawIconText(0x8C, 0xB2, 7, 1, z, (s32)text);
    /* the icons of the Sevens cards in the deck */
    for (x = 0x8D, i = 0x23; i < 0x2A; i++, x += 0x11) {
        if (NIS_countDeckOption(deck, i) != 0) {
            drawTexturedSprite(x, 0xC2, &uv, tpage, 0x7E80, z, 0x80, -1);
        }
        uv.x += 0x10;
    }
}
/* the names of the elements, of the Sevens' effects and of the option
   cards' kinds */
char *NIS_ELEMENT_NAMES[5] = {
    "火炎",
    "氷水",
    "自然",
    "暗黒",
    "珍種",
};
char *NIS_SEVENS_EFFECT_NAMES[5] = {
    "色、必要進化ポイント無視",
    "２段階進化",
    "進化ポイント＋３０",
    "同世代の交換",
    "１段階退化",
};
char *NIS_CARD_KIND_NAMES[2] = {
    "戦闘用",
    "進化用",
};
/* the letters of the name entry: hiragana, katakana, letters and two
   pages of kanji, eighteen rows of five each */
char NIS_KANA[5][18][11] = {
    {
        "あいうえお",
        "かきくけこ",
        "さしすせそ",
        "たちつてと",
        "なにぬねの",
        "はひふへほ",
        "まみむめも",
        "や　ゆ　よ",
        "らりるれろ",
        "わ　を　ん",
        "がぎぐげご",
        "ざじずぜぞ",
        "だぢづでど",
        "ばびぶべぼ",
        "ぱぴぷぺぽ",
        "ぁぃぅぇぉ",
        "っゃゅょ～", /* っゃゅょ〜 */
        "　　　　　", /* 　　　　　 */
    },
    {
        "アイウエオ",
        "カキクケコ",
        "サシスセソ",
        "タチツテト",
        "ナニヌネノ",
        "ハヒフヘホ",
        "マミムメモ",
        "ヤ　ユ　ヨ",
        "ラリルレロ",
        "ワ　ヲ　ン",
        "ガギグゲゴ",
        "ザジズゼゾ",
        "ダヂヅデド",
        "バビブベボ",
        "パピプペポ",
        "ァィゥェォ",
        "ッャュョー",
        "ヴ　　　　",
    },
    {
        "ＡＢＣＤＥ",
        "ＦＧＨＩＪ",
        "ＫＬＭＮＯ",
        "ＰＱＲＳＴ",
        "ＵＶＷＸＹ",
        "Ｚ　　　ー",
        "０１２３４",
        "♪★＝＋’",
        "（）：○△",
        "ａｂｃｄｅ",
        "ｆｇｈｉｊ",
        "ｋｌｍｎｏ",
        "ｐｑｒｓｔ",
        "ｕｖｗｘｙ",
        "ｚ　　　～", /* ｚ　　　〜 */
        "５６７８９",
        "・…／∞？",
        "×αβ＆！",
    },
    {
        "赤火陸闇電",
        "青炎海幻雷",
        "緑氷空邪風",
        "黒水天戦嵐",
        "黄暗熱魔波",
        "白自爆攻光",
        "紫然速守力",
        "金珍神防反",
        "銀種聖御変",
        "死忍竜号捨",
        "命時動大山",
        "撃地物小回",
        "破心鳥最復",
        "壊魂翼高進",
        "滅国飛超退",
        "亡強角王化",
        "封悪色真先",
        "印血族絶制",
    },
    {
        "勇無機運猛",
        "者敵械世仮",
        "伝鬼系界練",
        "説覇星牙習",
        "新対属正勝",
        "改気性義必",
        "獣完引逆殺",
        "恐全出究技",
        "拳体入極術",
        "打手流壁霧",
        "消札不混舞",
        "場土兄乱雪",
        "友遊弟暴影",
        "達激秘同鋼",
        "人特密永鉄",
        "間別裏久骨",
        "単謎々栄虫",
        "枚能式型夢",
    },
};

/* writes a card's details into VRAM, for the trade */
void NIS_writeCardDetails(s32 type, s32 index) {
    Rect16 rect;
    char text[0x48];
    char number[0x10];
    s32 i;

    NIS_DECK_EDIT.cardType = type;
    NIS_DECK_EDIT.cardIndex = index;
    rect.x = 0x3C0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
    fillVramRect(0x3C0, 0, 0x100, 0xDC, 0);
    if (type == 0) {
        /* "Digimon card  Element %s  No%s" */
        sprintf(text, "デジモンカード　　属性　%s　Ｎｏ%s", NIS_ELEMENT_NAMES[NIS_DIGIMON_CARDS[index].elementLevel >> 4], formatSjisNumber(index + 1, 3, number));
        uploadKanjiString(text, &rect);
        rect.y += 0x10;
        rect.x = 0x3C3;
        uploadKanjiString(NIS_DIGIMON_CARDS[index].name, &rect);
        rect.x = 0x3E4;
        uploadKanjiString("Ｌｖ", &rect);
        rect.y += 0xC;
        rect.x = 0x3C3;
        sprintf(text, "ＨＰ%s", formatSjisNumber(NIS_DIGIMON_CARDS[index].hp, 4, number));
        uploadKanjiString(text, &rect);
        rect.y += 0xC;
        rect.x = 0x3C3;
        sprintf(text, "進化Ｐ　%s", formatSjisNumber(NIS_DIGIMON_CARDS[index].dpCost, 2, number));
        uploadKanjiString(text, &rect);
        rect.x = 0x3DE;
        sprintf(text, "ＰＷ　%s", formatSjisNumber(NIS_DIGIMON_CARDS[index].dpBonus, 2, number));
        uploadKanjiString(text, &rect);
        rect.y += 0xC;
        for (i = 0; i < 3; i++, rect.y += 0xC) {
            rect.x = 0x3C3;
            sprintf(text, "　%s", NIS_DIGIMON_CARDS[index].attacks[i].name);
            uploadKanjiString(text, &rect);
            rect.x = 0x3E4;
            uploadKanjiString(formatSjisNumber(NIS_DIGIMON_CARDS[index].attacks[i].power, 3, number), &rect);
        }
        rect.y += 0x12;
        rect.x = 0x3D2;
        uploadKanjiString("援護能力", &rect);
        rect.y += 0xC;
        for (i = 0; i < 4; i++, rect.y += 0xC) {
            rect.x = 0x3D2;
            uploadKanjiString(NIS_DIGIMON_CARDS[index].supportText[i], &rect);
        }
        rect.x = 0x3C0;
    } else {
        /* "Option card  %s  No%s" */
        sprintf(text, "オプションカード　　%s　　Ｎｏ%s", NIS_CARD_KIND_NAMES[type - 1], formatSjisNumber(NIS_getCardId(type, index) + 1, 3, number));
        uploadKanjiString(text, &rect);
        rect.y += 0x10;
        rect.x = 0x3C3;
        uploadKanjiString((type == 1) ? NIS_OPTION_CARDS[index].name : NIS_DIGIVOLVE_CARDS[index].name, &rect);
        rect.y += 0x18;
        for (i = 0; i < 4; i++, rect.y += 0xC) {
            rect.x = 0x3C3;
            /* every line shows the first one */
            uploadKanjiString((type == 1) ? NIS_OPTION_CARDS[index].text[0] : NIS_DIGIVOLVE_CARDS[index].text[0], &rect);
        }
        rect.x = 0x3C0;
    }
    rect.y = 0xE8;
    uploadKanjiString("使用枚数", &rect);
}
