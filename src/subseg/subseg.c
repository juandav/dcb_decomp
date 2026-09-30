#include "common.h"
#include "game.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/card_db.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/heap.h"
#include "dcb/vram_upload.h"
#include "dcb/menu.h"
#include "dcb/memcard.h"
#include "dcb/sort.h"
#include "dcb/battle_hud.h"
#include "dcb/prim.h"
#include "dcb/partner_level.h"
#include "dcb/scroll_bg.h"
#include "dcb/frame_callback.h"
#include "dcb/dialog.h"

extern s16 D_801F41BA;
extern u8 D_801F41C0;
extern s8 *D_801F404C;
extern s16 D_801F4050;
extern void func_801E89FC(s16 x, s16 y, s16 clut, s32 u, s32 v, s16 w, s16 h, s8 tp, u8 brightness, s8 abr, s32 otIndex);

typedef struct {
    u16 cardIds[30];
    char name[0x32];
} DeckRecord;
extern Menu D_801F1934;
extern UiWindow D_801F24B0;
extern CursorHighlight D_801F2500;
extern u16 D_801F2550[0x9F];
extern void func_801E0FF0();

extern Menu D_801F219C;
extern char *D_801F2154[];
/* a comparison function for sortArray */
typedef s32 (*SortCompare)(s8 *, s8 *);
extern SortCompare D_801F21C8[];
extern PlayerDeck *D_801F4328;
extern void func_801E7B30(CardSlot *cards, s32 player);
extern void func_801E7F8C(CardSlot *cards);
typedef struct {
    s16 request;
    s16 ids[8];
    s8 ages[8];
    s8 busy;
    s8 running;
} CardImageCache;
extern CardImageCache D_801F4188;
extern UiWindow D_801F3250[7];
extern void func_801F0024();

typedef struct {
    s16 col;
    s16 prevCol;
    s16 row;
    s16 prevRow;
    u8 active;
    char name[0x13];
    u8 cursor;
    u8 onButtons;
    s8 button;
    s8 prevButton;
    s8 state;
    u8 pad;
    u8 fresh;
} NameEntry;
extern NameEntry D_801F2488;
extern CursorHighlight D_801F23E8;
extern s32 D_801F3178;
extern s32 D_801F317C;
extern void *D_801F34D0[301];
extern s8 *D_801F3990[301];
extern void func_801E89FC(s16 x, s16 y, s16 clut, s32 u, s32 v, s16 w, s16 h, s8 unk7, u8 unk8, s8 unk9, s32 otIndex);
extern SprtPacket *D_801F44A4;
typedef struct {
    u8 inDecks[3][301];
    u8 unk387[301];
    u8 spare[301];
} DeckCardCounts;
extern DeckCardCounts *D_801F4324;

extern UiWindow D_801F22F8;
extern UiWindow D_801F2398;
extern UiWindow D_801F2438;
extern void func_801E0278();
extern void func_801E0948();
extern void func_801E0B08();
typedef struct {
    UiWindow window;
    s32 slot;
} TabWindow;
extern TabWindow D_801F2870[3];
extern s32 D_801F2994;
extern UiWindow D_801F27D0;
extern UiWindow D_801F2950;
extern UiWindow D_801F2820;
extern UiWindow D_801F2690;
extern UiWindow D_801F2730;
extern void func_801E14E4(void);
extern void func_801E2098();
extern void func_801E2B8C();
extern void func_801E3BB4();
extern void func_801E2DA4();
extern void func_801E4788();
extern void func_801E4560();
typedef struct {
    void *primBuffers[2];
    s16 *unk8;
    void (*task)();
    s16 slide;
    s16 player;
    s8 hidden;
    s8 editing;
    s8 running;
    u8 listShown;
    u8 blink;
    u8 useDeckCounts;
} Unk801F41A8;
/* libgpu's LINE_G3 */
typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 p1;
    s16 x1;
    s16 y1;
    u8 r2;
    u8 g2;
    u8 b2;
    u8 p2;
    s16 x2;
    s16 y2;
    u32 pad;
} LINE_G3;
typedef struct {
    u8 *deckCounts;
    LINE_G3 cursorLines[2][4];
    s8 *card;
    s16 cardId;
    s16 statsPage;
    s16 slot;
    s8 mode;
    u8 unk10F;
} DeckEditState;
extern Unk801F41A8 D_801F41A8;
extern DeckEditState D_801F3F48;
extern s16 D_801F435E;
typedef struct {
    s16 id;
    s16 count;
} CardCount;
typedef struct {
    CardCount *lists[9];
    s32 selectedId;
    s16 uniqueCount;
    s16 totalCount;
    s16 page;
    s16 showInfo;
    s16 counts[8];
    u8 unk40[0x12D];
    u8 unk16D;
    u8 unk16E[2];
    s8 *selectedCard;
} CollectionStats;
extern CollectionStats D_801F4330;
typedef struct {
    char *name;
    u8 unk4[8];
} AbilityText;
extern AbilityText D_801F1960[];
extern s32 D_801F299C;
typedef struct {
    PlayerDeck *decks[3];
    s8 count;
    s8 current;
    s16 slot;
} Unk801F4060;
extern Unk801F4060 D_801F4060;
extern u8 *D_801F342C;
typedef struct {
    s16 ids[250];
    s16 count;
    s16 pad;
} CardIdList;
extern CardIdList *D_801F42D8;
extern u8 D_801F200C;
extern u8 D_801F2100;
extern u8 D_801F21C4;
extern void func_801E86D4(void);
extern void func_801E9790();
extern void func_801EF740(UiWindow *window);
extern void func_801EDEFC(UiWindow *window);
extern UiWindow D_801F3430;
extern UiWindow D_801F3E50;
extern UiWindow D_801F32D8;
extern UiWindow D_801F3294[3];
extern UiWindow D_801F331C;
extern UiWindow D_801F3360;
extern UiWindow D_801F33A4;
extern UiWindow D_801F33E8;
extern void func_801E6DA8();
extern void func_801EE8CC();
extern void func_801EEE40();
extern void func_801EF7EC();
extern void func_801EFC78();
extern void func_801EDF6C();
extern void func_801EC998(PlayerDeck *deck);
extern PlayerDeck D_801F41C8;
extern void func_801ED070(PlayerDeck *deck);
extern void func_801EDA88(PlayerDeck *deck);
extern void func_801EBCC0();
extern CursorHighlight D_801F3EF0;
extern u8 D_801F4070[6];
extern void func_801EA8AC();
extern void func_801E9F68();
extern void func_801E6FC8();
extern void func_801EA3D4(UiWindow *window);
extern void func_801EA478(UiWindow *window);
extern void func_801EA5F0();
extern void func_801EB2BC();
extern void func_801EB92C(UiWindow *window);
extern void func_801EB8BC(UiWindow *window);
extern u16 D_801F4360[8];
extern void func_801E8E8C(void);
extern SortCompare D_801F2104[];
extern Menu D_801F20D8;
extern Menu D_801F1FE4;
extern char *D_801F2038[];
extern char *D_801F2088[];
extern void func_801E6BE8(void);
extern Menu D_801F1F8C;
extern s8 D_801F3180[];
extern Menu D_801F1F60;
extern char *strcat(char *, const char *);
extern u8 D_801F200A;
extern Rect16 D_801F20E0;
extern s32 func_801ECE70(s32 cardId);
extern void func_801ED944(void);
extern void func_801E8864(s16 cardId, s16 x, s16 y, u8 arg3, s32 otIndex);
extern void func_801F0A20(void);
extern void func_801F0BB0(void);
extern void func_801F10D8(PlayerDeck *deck);
extern s32 func_801E11D4(PlayerDeck *deck, s32 player);
extern void func_801EDD60(PlayerDeck *deck);
extern void func_801E0C08();
extern CursorHighlight D_801F2348;
extern void func_801E0BA8(void);
extern void func_801F0BE0(void);
extern char D_801F42E0[];
extern u8 D_801F2028[][4];
extern u8 D_801F18D8[];
extern s32 func_801DF9BC(void);
extern s8 *D_801F3F40;
extern u8 D_801F228C[];
extern s32 func_801F0FD8(CardSlot *slots, s32 row, s32 count);
extern s8 *D_801F44A0;
extern s32 func_801E8670(s16 id);
extern s8 D_801F4058;
extern s32 func_801ECF80(PlayerDeck *a, PlayerDeck *b);
extern Partner D_801F29A0;
extern Partner D_801F2C40;
extern Partner D_801F2EE0;
extern u8 D_801F1FD0[][3];
extern void func_801E1B70(s32 player, s32 slot, s32 x, s32 y, s32 otIndex);
extern s32 func_801E1680(s32 a, s32 b);
extern void func_801E16A8(s32 player, s32 slot, s32 x, s32 y, s32 brightness, s32 otIndex);
extern s32 D_801F2998;
extern CursorHighlight D_801F26E0;
extern CursorHighlight D_801F2780;
extern Rect16 D_801F1FB8;
extern Rect16 D_801F1FC0;
extern Rect16 D_801F1FC8;
extern s8 D_801F2861;
extern s8 D_801F2991;
extern void func_801E49CC(void);
/*
 * Strings kept as INCLUDE_RODATA: several functions use them and a shared
 * literal would land at its first use instead of where the original put it,
 * they sit inside a bigger blob, or a literal would be emitted at the wrong
 * place in .rodata.
 */
extern char D_801DDFC0[]; /* "A Deck Name has not been entered!" */
extern char D_801DE010[]; /* "BASE DECK LIST", then a 0xFE where GCC pads with a zero */
extern char D_801DF27C[]; /* "Lv" */
extern char D_801DF280[]; /* "Type" */
extern char D_801DF288[]; /* "*s0%3.3d" */
extern char D_801DF294[]; /* "%d" */
extern char D_801DF298[]; /* "Cards" */
extern char D_801DF318[]; /* "L1BACK" */
extern char D_801DF320[]; /* "CARD LIST" */
extern char D_801DF32C[]; /* "HELP" */
extern char D_801DF334[]; /* "PARTNER" */
extern char D_801DF33C[]; /* "CARD DATA" */
extern char D_801DF4F0[]; /* "CARD INFO." */
extern char D_801DF4FC[]; /* "*s0%3d" */
extern char D_801DF504[]; /* "Total" */
extern char D_801DF50C[]; /* "*s0%4d" */
extern char D_801DF54C[]; /* "*s0%2d" */
extern char D_801DF554[]; /* "Wins" */
extern char D_801DF55C[]; /* "Losses" */
extern char D_801DF564[]; /* "Support Effect" */
extern char D_801DF574[]; /* "*a0" */
extern char D_801DF578[]; /* "*a1" */
extern char D_801DF57C[]; /* "*a2" */
extern char D_801DF580[]; /* "*a3" */
extern char D_801DF584[]; /* "*a4" */
extern char D_801DF588[]; /* "Option Card" */
extern char D_801DF594[]; /* "Partner" */
extern char D_801DF600[]; /* "DECK 1" */
extern char D_801DF608[]; /* "DECK 2" */
extern char D_801DF610[]; /* "DECK 3" */
extern char D_801DF6C0[]; /* "SUM" */
extern char D_801DF6C4[]; /* "DECK" */
extern char D_801DF7D4[]; /* "Disable" */

extern s16 D_801F406E;
extern u8 D_801F4071;
extern s8 D_801F41BC;

typedef struct {
    s16 x;
    s16 w;
    u8 next[4];
} MenuItem;

extern u8 D_801F4072;
extern u8 D_801F4073;
extern u8 D_801F4074;
extern u8 D_801F4075;
extern MenuItem D_801F2224[13];
extern u8 D_801F41C1;
extern s8 D_801F4056;
extern s32 D_801F3304;
extern CursorHighlight D_801F3200;
extern CursorHighlight D_801F3480;
extern CursorHighlight D_801F3EA0;
extern void func_801E8D98(UiWindow *window, Rect16 area, s32 label, s32 flags, s32 style);
extern u8 D_801F41BE;
extern void *D_801F4350;
extern s32 D_801F32C0;
extern void func_801E93B8(void);
extern void func_801E95D0(void);
extern void func_801E4B34();

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DDF38);

s32 func_801DF9BC(void) {
    Rect16 rect;
    UiWindow *window = &D_801F22F8;
    s16 r;
    if (D_801F2488.active != 0) {
        if (D_801F2488.onButtons == 0) {
            if ((u16)PAD_STATES[D_801F2488.pad]->repeat & 0xF000) {
                playMenuSound(2);
            }
            do {
                if ((u16)PAD_STATES[D_801F2488.pad]->repeat & 0x1000) {
                    D_801F2488.row--;
                    D_801F2488.row = (D_801F2488.row + 1) / 9 * 9 + (D_801F2488.row + 9) % 9;
                    r = D_801F2488.row % 9;
                    if (r == 0) {
                        PAD_STATES[D_801F2488.pad]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[D_801F2488.pad]->repeat & 0x4000) {
                    D_801F2488.row++;
                    D_801F2488.row = (D_801F2488.row - 1) / 9 * 9 + (D_801F2488.row + 9) % 9;
                    r = D_801F2488.row % 9;
                    if (r == 8) {
                        PAD_STATES[D_801F2488.pad]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[D_801F2488.pad]->repeat & 0x8000) {
                    if (--D_801F2488.col < 0) {
                        D_801F2488.col = 9;
                        D_801F2488.onButtons = 1;
                        D_801F2488.button = 7;
                    } else if (D_801F2488.col == 0) {
                        PAD_STATES[D_801F2488.pad]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[D_801F2488.pad]->repeat & 0x2000) {
                    if (++D_801F2488.col >= 10) {
                        D_801F2488.col = 0;
                        D_801F2488.onButtons = 1;
                        D_801F2488.button = 7;
                    } else if (D_801F2488.col == 9) {
                        PAD_STATES[D_801F2488.pad]->repeatEnabled = 0;
                    }
                }
            } while (D_801F18D8[D_801F2488.row * 10 + D_801F2488.col] == ' ');
        } else {
            if ((u16)PAD_STATES[D_801F2488.pad]->repeat & 0xF000) {
                playMenuSound(2);
            }
            if ((u16)PAD_STATES[D_801F2488.pad]->pressed & 0xA000) {
                playMenuSound(2);
            }
            if ((u16)PAD_STATES[D_801F2488.pad]->repeat & 0x1000) {
                if (--D_801F2488.button < 7) {
                    D_801F2488.button = 8;
                } else if (D_801F2488.button == 7) {
                    PAD_STATES[D_801F2488.pad]->repeatEnabled = 0;
                }
            } else if ((u16)PAD_STATES[D_801F2488.pad]->repeat & 0x4000) {
                if (++D_801F2488.button >= 9) {
                    D_801F2488.button = 7;
                } else if (D_801F2488.button == 8) {
                    PAD_STATES[D_801F2488.pad]->repeatEnabled = 0;
                }
            } else if ((u16)PAD_STATES[D_801F2488.pad]->pressed & 0x8000) {
                D_801F2488.onButtons = 0;
                D_801F2488.prevButton = -1;
                D_801F2488.prevCol = -1;
                D_801F2488.prevRow = -1;
                D_801F2488.col = 9;
                while (D_801F18D8[D_801F2488.row * 10 + D_801F2488.col] == ' ') {
                    D_801F2488.col--;
                }
            } else if ((u16)PAD_STATES[D_801F2488.pad]->pressed & 0x2000) {
                D_801F2488.onButtons = 0;
                D_801F2488.prevButton = -1;
                D_801F2488.prevCol = -1;
                D_801F2488.prevRow = -1;
                D_801F2488.col = 0;
                while (D_801F18D8[D_801F2488.row * 10 + D_801F2488.col] == ' ') {
                    D_801F2488.col++;
                }
            }
        }
    }
    if (D_801F2488.onButtons == 0) {
        if (D_801F2488.row != D_801F2488.prevRow || D_801F2488.col != D_801F2488.prevCol) {
            D_801F2488.prevCol = D_801F2488.col;
            D_801F2488.prevRow = D_801F2488.row;
            rect.x = window->rect.x - window->scroll[2] + D_801F2488.prevCol * 17 + 4;
            rect.y = window->rect.y - window->scroll[3] + D_801F2488.prevRow * 14 + 1;
            rect.w = 12;
            rect.h = 12;
            if (D_801F2488.prevCol >= 5) {
                rect.x = window->rect.x - window->scroll[2] + D_801F2488.prevCol * 17 + 15;
            }
            moveCursorHighlight(&D_801F2348, &rect);
        }
    } else if (D_801F2488.button != D_801F2488.prevButton) {
        D_801F2488.prevButton = D_801F2488.button;
        switch (D_801F2488.button) {
        case 0:
        case 1:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + D_801F2488.button * 14 + 1;
            rect.w = 0x30;
            rect.h = 12;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + D_801F2488.button * 14 + 1;
            rect.w = 0x24;
            rect.h = 12;
            break;
        case 7:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + 0x63;
            rect.w = 0x18;
            rect.h = 12;
            break;
        case 8:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + 0x71;
            rect.w = 0x30;
            rect.h = 12;
            break;
        }
        moveCursorHighlight(&D_801F2348, &rect);
    }
}

void func_801E0278(UiWindow *window) {
    /* not literals: GCC would share "Cancel" with func_801F0024, which has its own copy */
    static const char charFormat[] = "%c";
    static const char okLabel[] = "OK";
    static const char cancelLabel[] = "Cancel";
    char buf[64];
    u8 rowPalettes[8];
    s32 x = window->originX + 4;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 i;
    s32 dx;
    s32 button;

    for (i = 0; i < 90; i++) {
        sprintf(buf, charFormat, D_801F18D8[i]);
        dx = 0;
        if (i % 10 >= 5) {
            dx = 11;
        }
        drawText(x + 4 + (i % 10) * 17 + dx, y + (i / 10) * 14, (s32)buf, 7, z);
    }
    for (i = 0; i < 7; i++) {
        rowPalettes[i] = 4;
    }
    rowPalettes[window->view.y / window->rect.h] = 5;
    drawText(window->rect.x + 0xCC, window->rect.y + 0x63, (s32)okLabel, 6, z);
    drawText(window->rect.x + 0xCC, window->rect.y + 0x71, (s32)cancelLabel, 6, z);
    func_801DF9BC();
    if (D_801F2488.onButtons == 0) {
        if (PAD_STATES[D_801F2488.pad]->repeat & 0x40) {
            D_801F2488.name[D_801F2488.cursor] = D_801F18D8[D_801F2488.row * 10 + D_801F2488.col];
            playMenuSound(1);
            if (D_801F2488.cursor == 0 && D_801F2488.fresh == 1) {
                for (i = 1; i < 13; i++) {
                    D_801F2488.name[i] = 0;
                }
            }
            if (D_801F2488.cursor < 11) {
                D_801F2488.cursor++;
            } else {
                D_801F2488.onButtons = 1;
                D_801F2488.button = 7;
            }
            D_801F2488.fresh = 0;
        } else if (PAD_STATES[D_801F2488.pad]->repeat & 0x20) {
            for (i = 11; i >= D_801F2488.cursor + 1; i--) {
                D_801F2488.name[i] = D_801F2488.name[i - 1];
            }
            D_801F2488.name[D_801F2488.cursor] = D_801F18D8[D_801F2488.row * 10 + D_801F2488.col];
            playMenuSound(1);
            if (D_801F2488.cursor < 11) {
                D_801F2488.cursor++;
            } else {
                D_801F2488.onButtons = 1;
                D_801F2488.button = 7;
            }
            D_801F2488.fresh = 0;
        } else if (PAD_STATES[D_801F2488.pad]->repeat & 0x10) {
            if (D_801F2488.name[0] != 0) {
                playMenuSound(1);
            }
            if (D_801F2488.cursor == 0) {
                D_801F2488.cursor++;
            }
            for (i = D_801F2488.cursor; i < 13; i++) {
                D_801F2488.name[i - 1] = D_801F2488.name[i];
            }
            D_801F2488.cursor--;
            D_801F2488.fresh = 0;
        }
    } else if (PAD_STATES[D_801F2488.pad]->pressed & 0x40) {
        playMenuSound(1);
        button = D_801F2488.button;
        if (button >= 0) {
            if (button < 7) {
                D_801F2488.row = button * 9;
                D_801F2488.col = 0;
                scrollWindowTo((s16 *)window, 0, D_801F2488.row * 14);
            } else if (button < 9) {
                D_801F2488.state = D_801F2488.button;
            }
        }
    } else if (PAD_STATES[D_801F2488.pad]->repeat & 0x10) {
        if (D_801F2488.name[0] != 0) {
            playMenuSound(1);
        }
        if (D_801F2488.cursor == 0) {
            D_801F2488.cursor++;
        }
        for (i = D_801F2488.cursor; i < 13; i++) {
            D_801F2488.name[i - 1] = D_801F2488.name[i];
        }
        D_801F2488.cursor--;
        D_801F2488.fresh = 0;
    }
    if ((PAD_STATES[D_801F2488.pad]->pressed & 0x800) && !(D_801F2488.onButtons == 1 && D_801F2488.button == 7)) {
        playMenuSound(1);
        D_801F2488.onButtons = 1;
        D_801F2488.button = 7;
    }
    drawCursorHighlight(&D_801F2348, z);
}

void func_801E0948(UiWindow *window) {
    Rect16 rect;
    char buf[248];
    s32 x = window->originX + 1;
    s32 y = window->originY;
    s32 z = window->z;

    sprintf(buf, "*s0%s", D_801F2488.name);
    drawText(x, y, (s32)buf, 7, z);
    drawText(x + 0x4C, y, (s32)"Deck", 6, z);
    if (PAD_STATES[D_801F2488.pad]->repeat & 4) {
        if (D_801F2488.cursor != 0) {
            playMenuSound(2);
            D_801F2488.cursor--;
        }
    } else if (PAD_STATES[D_801F2488.pad]->repeat & 8) {
        if (D_801F2488.cursor != 11 && D_801F2488.name[D_801F2488.cursor] != 0) {
            D_801F2488.cursor++;
            playMenuSound(2);
        }
    }
    rect.x = x + D_801F2488.cursor * 6;
    rect.y = y + 13;
    rect.w = 6;
    rect.h = 0;
    moveCursorHighlight(&D_801F23E8, &rect);
    drawCursorHighlight(&D_801F23E8, z);
}

void func_801E0B08(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    s32 x = window->originX + 1;
    s32 y = window->originY + 1;
    s32 z = window->z;

    drawText(x, y, (s32)"*b0 Insert", 7, z);
    drawText(x + 8, y + 13, (s32)"*b2 OK", 7, z);
    drawText(x, y + 26, (s32)"*b1 Delete", 7, z);
}

void func_801E0BA8(void) {
    drawWindow(&D_801F22F8, func_801E0278, 1);
    drawWindow(&D_801F2398, func_801E0948, 1);
    drawWindow(&D_801F2438, func_801E0B08, 1);
}

void func_801E0C08(s32 mode, char *name, s32 pad) {
    Rect16 cursor;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];

    bzero((Scene3D *)D_801F2488.name, 13);
    strcpy(D_801F2488.name, name);
    D_801F2488.col = 0;
    D_801F2488.row = 0;
    D_801F2488.prevCol = -1;
    D_801F2488.prevRow = -1;
    D_801F2488.active = 1;
    D_801F2488.cursor = 0;
    D_801F2488.onButtons = 0;
    D_801F2488.button = 0;
    D_801F2488.prevButton = -1;
    D_801F2488.state = 0;
    D_801F2488.pad = pad;
    D_801F2488.fresh = 1;
    rect.x = 0x18;
    rect.y = 0x5E;
    rect.w = 0x110;
    rect.h = 0x7E;
    view.x = 0;
    view.y = 0;
    view.w = 0x10E;
    view.h = 0x7E;
    openWindow(&D_801F22F8, &rect, -1, (s16 *)&view, 10, 0x26, 0x80, 12);
    D_801F22F8.label = (s32)"NAME ENTRY";
    cursor.x = D_801F22F8.originX + 4;
    cursor.y = D_801F22F8.originY + 1;
    cursor.w = 12;
    cursor.h = 12;
    initCursorHighlight(&D_801F2348, &cursor, (Bytes4 *)-1);
    rect.x = 0x1E;
    rect.y = 0x1C;
    rect.w = 0x34;
    rect.h = 0x28;
    openWindow(&D_801F2438, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    D_801F2438.label = (s32)"HELP";
    rect.x = 0xBC;
    rect.y = 0x1C;
    rect.w = 0x6C;
    rect.h = 0xE;
    openWindow(&D_801F2398, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    D_801F2398.label = (s32)"DECK NAME";
    cursor.x = D_801F2398.originX;
    cursor.y = D_801F2398.originY + 13;
    cursor.w = 12;
    cursor.h = 0;
    initCursorHighlight(&D_801F23E8, &rect, (Bytes4 *)-1);
    playMenuSound(3);
    addFrameCallback((s32)func_801E0BA8);
    for (;;) {
        func_80014C08(FRAME_INTERVAL);
        if (D_801F2488.state == 0) {
            continue;
        }
        if (D_801F2488.state == 7 && D_801F2488.name[0] == 0) {
            initDialog(dialog, (u8 *)D_801DDFC0, 0);
            dialog[0xA6] = pad;
            runDialog(dialog);
            D_801F2488.state = 0;
            continue;
        }
        break;
    }
    if (D_801F2488.state == 7) {
        strcpy(name, D_801F2488.name);
    }
    playMenuSound(4);
    animateWindowTo(&D_801F2438, (Rect16 *)-1);
    animateWindowTo(&D_801F22F8, (Rect16 *)-1);
    animateWindowTo(&D_801F2398, (Rect16 *)-1);
    func_80014C08(20);
    removeFrameCallback((s32)func_801E0BA8);
    if (D_801F2488.state == 7) {
        func_800149B8(0, -1, 0, 0x1000, mode == 1 ? func_801EBCC0 : func_801F0BE0, 0, 0, 0, 0);
    } else {
        func_800149B8(0, -1, 0, 0x1000, func_801EBCC0, 0, 0, 0, 0);
    }
}

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DDFC0);

/* its own copy of "Deck": func_801E0948 was in another file of the original */
static const char D_801DDFE4[] = "Deck";

void func_801E0FF0(UiWindow *window) {
    char buf[64];
    s32 x = window->originX;
    s32 z = window->z;
    DeckRecord *records = (DeckRecord *)(*(u8 **)D_8006E054 + 8);
    s32 i;
    s32 y;
    s32 palette;
    u16 entry;

    for (i = 0; i < D_801F1934.nrows; i++) {
        if (i < window->view.y / D_801F1934.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / D_801F1934.rowH < i) {
            break;
        }
        y = window->originY + i * D_801F1934.rowH + 1;
        entry = D_801F2550[i];
        strcpy(buf, records[entry & 0x3FFF].name);
        strcat(buf, D_801DDFE4);
        palette = 8;
        if (entry & 0x4000) {
            palette = 7;
        }
        drawText(x + 2, y, (s32)buf, palette, z);
        if (palette == 7) {
            drawText(x + 0x9E, y, (s32)"Usable", 7, z);
        } else {
            drawText(x + 0x9E, y, (s32)"Not Usable", palette, z);
        }
    }
    updateMenuCursor(&D_801F1934);
}

s32 func_801E11D4(PlayerDeck *deck, s32 player) {
    u8 *file;
    DeckRecord *records;
    s32 i;
    s32 count;
    s32 done;
    s32 selected;
    u16 flags;
    DeckRecord *record;

    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\DECK2.DEK", getCurrentTaskId());
    file = (u8 *)func_80014C08(0x7FFFFFFF);
    *(u8 **)D_8006E054 = file;
    records = (DeckRecord *)(file + 8);
    markBuildableOpponentDecks(player);
    for (i = 0; i < 0x9F; i++) {
        D_801F2550[i] = 0;
    }
    count = 0;
    for (i = 0; i < 0x9F; i++) {
        flags = PLAYER_DATA(player).opponentDeckFlags[i];
        if (flags & 0x8000) {
            D_801F2550[count] = i | (flags & 0xC000);
            count++;
        }
    }
    D_801F1934.pad = player;
    D_801F1934.nrows = count;
    D_801F24B0.view.h = count * D_801F1934.rowH;
    openMenu(&D_801F1934, &D_801F24B0, &D_801F2500, (Bytes4 *)-1);
    D_801F24B0.label = (s32)D_801DE010;
    playMenuSound(3);
    done = 0;
    selected = -1;
    do {
        func_80014C08(FRAME_INTERVAL);
        drawWindow(&D_801F24B0, func_801E0FF0, 0);
        if ((PAD_STATES[player]->pressed & 0x40) && (D_801F2550[D_801F1934.row] & 0x4000)) {
            playMenuSound(1);
            selected = D_801F2550[D_801F1934.row] & 0x3FFF;
            done = 1;
        } else if (PAD_STATES[player]->pressed & 0x10) {
            playMenuSound(0);
            selected = -1;
            done = 1;
        }
    } while (!done);
    playMenuSound(4);
    if (selected >= 0) {
        record = &records[selected];
        for (i = 0; i < 30; i++) {
            setCardSlotFromId((u8 *)&deck->cards[i], record->cardIds[i]);
        }
        linkDeckCardData(player, deck);
    }
    freeHeapBlock(*(u8 **)D_8006E054);
    return selected;
}

void func_801E14E4(void) {
    if (isSpritePoolFull() == 0) {
        if (D_801F3178 != 0) {
            D_801F317C += 4;
            if (D_801F317C > 8) {
                D_801F317C = 8;
            }
        } else {
            D_801F317C -= 4;
            if (D_801F317C < -0x20) {
                D_801F317C = -0x20;
            }
        }
        CUR_SPRT->sp.x0 = 6;
        CUR_SPRT->sp.y0 = D_801F317C;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0x40;
        CUR_SPRT->sp.clut = 0x7C00;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x20;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setlen(&CUR_SPRT->dm, 1);
        CUR_SPRT->dm.code[0] = 0xE1000005;
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

s32 func_801E1680(s32 a, s32 b) {
    s32 result = 7;

    if (a < b) {
        result = 5;
    }
    if (b < a) {
        result = 2;
    }
    return result;
}

void func_801E16A8(s32 player, s32 slot, s32 x, s32 y, s32 brightness, s32 otIndex) {
    s32 specialty;

    if (PLAYER_DATA(player).partners[slot].cardId == 0) {
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 8;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = 0x7FA0;
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = brightness;
            CUR_SPRT->sp.g0 = brightness;
            CUR_SPRT->sp.b0 = brightness;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    } else {
        specialty = PLAYER_DATA(player).partners[slot].card[0].attr >> 4;
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0x30;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = getClut(0x200, specialty + 0x1F8);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = brightness;
            CUR_SPRT->sp.g0 = brightness;
            CUR_SPRT->sp.b0 = brightness;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            if (isSpritePoolFull() == 0) {
                CUR_SPRT->sp.x0 = x;
                CUR_SPRT->sp.y0 = y + 4;
                CUR_SPRT->sp.u0 = 0;
                CUR_SPRT->sp.v0 = getSlotPartnerIndex(player, slot) * 41;
                CUR_SPRT->sp.clut = getClut(0x240, specialty | 0x1F0);
                CUR_SPRT->sp.w = 40;
                CUR_SPRT->sp.h = 40;
                setSemiTrans(&CUR_SPRT->sp, 0);
                CUR_SPRT->sp.r0 = brightness;
                CUR_SPRT->sp.g0 = brightness;
                CUR_SPRT->sp.b0 = brightness;
                setDrawMode(&CUR_SPRT->dm, 0, 0, 0x97);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
                SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            }
        }
    }
}

void func_801E1B70(s32 player, s32 slot, s32 x, s32 y, s32 otIndex) {
    s32 specialty;

    if (PLAYER_DATA(player).partners[slot].unk292[0] == 0) {
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 8;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = 0x7FA0;
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    } else {
        specialty = ((DigimonCardData *)DIGIMON_CARDS)[PLAYER_DATA(player).partners[slot].unk292[0]].attr >> 4;
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0x30;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = getClut(0x200, specialty + 0x1F8);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            if (isSpritePoolFull() == 0) {
                CUR_SPRT->sp.x0 = x;
                CUR_SPRT->sp.y0 = y + 4;
                CUR_SPRT->sp.u0 = (getSelectedArmorIndex(player, getSlotPartnerIndex(player, slot)) + 1) * 44;
                CUR_SPRT->sp.v0 = getSlotPartnerIndex(player, slot) * 41;
                CUR_SPRT->sp.clut = getClut(0x240, specialty | 0x1F0);
                CUR_SPRT->sp.w = 40;
                CUR_SPRT->sp.h = 40;
                setSemiTrans(&CUR_SPRT->sp, 0);
                CUR_SPRT->sp.r0 = 0x80;
                CUR_SPRT->sp.g0 = 0x80;
                CUR_SPRT->sp.b0 = 0x80;
                setDrawMode(&CUR_SPRT->dm, 0, 0, 0x97);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
                SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            }
        }
    }
}

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DE010);

/* the characters of the name entry grid, ten to a row */
u8 D_801F18D8[] =
    "ABCDEabcde"
    "FGHIJfghij"
    "KLMNOklmno"
    "PQRSTpqrst"
    "UVWXYuvwxy"
    "Z-   z    "
    "          "
    "          "
    "0123456789"
;

/* not referenced by any code */
u8 D_801F1933 = 16;

Menu D_801F1934 = { NULL, NULL, { 50, 40, 220, 154 }, 0, -1, 0, -1, 0xa, 0x21, 220, 12, 1, 1, 2, 1, 0, 14, 0, 0, 0 };

/* the partner abilities; the code here only reads their texts */
AbilityText D_801F1960[128] = {
    { "HP+50.", { 3, 5, 1, 99, 3, 7, 0, 0 } },
    { "HP+100.", { 17, 16, 8, 12, 14, 19, 0, 0 } },
    { "HP+150.", { 29, 32, 19, 25, 32, 33, 0, 0 } },
    { "HP+200.", { 45, 48, 31, 40, 52, 59, 0, 0 } },
    { "HP+300.", { 61, 67, 51, 57, 68, 78, 0, 0 } },
    { "HP+400.", { 96, 89, 71, 81, 91, 88, 0, 0 } },
    { "HP+500.", { 99, 0xFF, 75, 91, 0xFF, 95, 0, 0 } },
    { "All Attack Powers +50.", { 18, 39, 54, 30, 57, 28, 0, 0 } },
    { "All Attack Powers +100.", { 39, 86, 72, 50, 89, 51, 0, 0 } },
    { "All Attack Powers +200.", { 75, 0xFF, 90, 93, 0xFF, 84, 0, 0 } },
    { "*b0 Attack Power +100.", { 1, 9, 15, 5, 4, 2, 0, 0 } },
    { "*b0 Attack Power +150.", { 10, 21, 38, 22, 16, 15, 0, 0 } },
    { "*b0 Attack Power +200.", { 27, 54, 62, 41, 39, 30, 0, 0 } },
    { "*b0 Attack Power +250.", { 48, 0xFF, 78, 61, 62, 61, 0, 0 } },
    { "*b0 Attack Power +300.", { 67, 0xFF, 0xFF, 85, 82, 79, 0, 0 } },
    { "*b1 Attack Power +50.", { 4, 1, 12, 2, 6, 13, 0, 0 } },
    { "*b1 Attack Power +100.", { 12, 7, 22, 16, 18, 29, 0, 0 } },
    { "*b1 Attack Power +150.", { 32, 28, 45, 36, 45, 43, 0, 0 } },
    { "*b1 Attack Power +200.", { 51, 44, 0xFF, 56, 67, 66, 0, 0 } },
    { "*b1 Attack Power +250.", { 77, 0xFF, 0xFF, 76, 86, 96, 0, 0 } },
    { "*b2 Attack Power +50.", { 6, 6, 2, 4, 10, 1, 0, 0 } },
    { "*b2 Attack Power +100.", { 19, 36, 20, 19, 27, 9, 0, 0 } },
    { "*b2 Attack Power +150.", { 35, 69, 42, 38, 53, 24, 0, 0 } },
    { "*b2 Attack Power +200.", { 82, 0xFF, 63, 67, 0xFF, 48, 0, 0 } },
    { "*b0 to 0, *b2 Attack Power -100.", { 24, 12, 4, 14, 0xFF, 34, 0, 0 } },
    { "*b1 to 0, *b2 Attack Power -100.", { 46, 23, 9, 35, 0xFF, 17, 0, 0 } },
    { "*b2 to 0, *b2 Attack Power -100.", { 58, 71, 36, 10, 0xFF, 71, 0, 0 } },
    { "Counter *b0,*b2 Attack Power to 0.", { 11, 26, 0xFF, 0xFF, 40, 62, 0, 0 } },
    { "Counter *b1,*b2 Attack Power to 0.", { 36, 14, 0xFF, 0xFF, 24, 49, 0, 0 } },
    { "Counter *b2,*b2 Attack Power to 0.", { 40, 57, 0xFF, 0xFF, 11, 21, 0, 0 } },
    { "Opponent *a0 X3, *b2 Attack Power -200.", { 87, 92, 39, 69, 31, 50, 0, 0 } },
    { "Opponent *a1 X3, *b2 Attack Power -200.", { 25, 97, 0xFF, 42, 77, 73, 0, 0 } },
    { "Opponent *a2 X3, *b2 Attack Power -200.", { 37, 0xFF, 66, 0xFF, 83, 4, 0, 0 } },
    { "Opponent *a3 X3, *b2 Attack Power -200.", { 68, 33, 46, 21, 5, 97, 0, 0 } },
    { "Opponent *a4 X3, *b2 Attack Power -200.", { 52, 41, 6, 0xFF, 0xFF, 25, 0, 0 } },
    { "1st Attack, *b2 Attack Power -200.", { 76, 50, 95, 62, 0xFF, 0xFF, 0, 0 } },
    { "Jamming Support, *b2 Attack Power -100.", { 0xFF, 74, 27, 0xFF, 54, 10, 0, 0 } },
    { "Eat-up HP, *b2 Attack Power -200.", { 0xFF, 87, 50, 0xFF, 73, 0xFF, 0, 0 } },
    { "Add + 10 DP.", { 42, 4, 61, 1, 9, 0xFF, 0, 0 } },
    { "Add + 20 DP.", { 91, 29, 79, 27, 21, 0xFF, 0, 0 } },
    { "Add + 30 DP.", { 0xFF, 98, 97, 60, 78, 85, 0, 0 } },
    { "Boost Attack Power +50.", { 2, 10, 13, 3, 7, 11, 0, 0 } },
    { "Boost Attack Power +100.", { 21, 42, 56, 33, 36, 35, 0, 0 } },
    { "Boost Attack Power +200.", { 26, 81, 73, 46, 84, 44, 0, 0 } },
    { "Boost Attack Power +300.", { 69, 0xFF, 84, 82, 0xFF, 80, 0, 0 } },
    { "Attack Power is Doubled.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Boost *b0 Attack Power +300.", { 8, 30, 47, 17, 26, 12, 0, 0 } },
    { "Boost *b0 Attack Power +400.", { 34, 77, 74, 47, 46, 39, 0, 0 } },
    { "Boost *b0 Attack Power +500.", { 55, 0xFF, 88, 88, 66, 74, 0, 0 } },
    { "*b0 Attack Power is Doubled.", { 13, 64, 67, 58, 33, 63, 0, 0 } },
    { "*b0 Attack Power is Tripled.", { 59, 0xFF, 94, 74, 0xFF, 86, 0, 0 } },
    { "Boost *b1 Attack Power +200.", { 15, 2, 0xFF, 7, 22, 18, 0, 0 } },
    { "Boost *b1 Attack Power +300.", { 28, 19, 0xFF, 23, 41, 36, 0, 0 } },
    { "Boost *b1 Attack Power +400.", { 43, 70, 0xFF, 92, 59, 53, 0, 0 } },
    { "*b1 Attack Power is Doubled.", { 22, 13, 59, 34, 0xFF, 67, 0, 0 } },
    { "*b1 Attack Power is Tripled.", { 83, 0xFF, 91, 94, 0xFF, 89, 0, 0 } },
    { "Boost *b2 Attack Power +100.", { 23, 40, 10, 18, 0xFF, 8, 0, 0 } },
    { "Boost *b2 Attack Power +200.", { 38, 58, 40, 43, 0xFF, 20, 0, 0 } },
    { "Boost *b2 Attack Power +300.", { 71, 0xFF, 60, 63, 0xFF, 90, 0, 0 } },
    { "*b2 Attack Power is Doubled.", { 44, 72, 48, 51, 0xFF, 40, 0, 0 } },
    { "*b2 Attack Power is Tripled.", { 88, 0xFF, 85, 95, 0xFF, 65, 0, 0 } },
    { "Attack Power becomes same as HP.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Get 1st Attack.", { 53, 55, 98, 71, 85, 0xFF, 0, 0 } },
    { "Attack becomes Eat-up HP.", { 0xFF, 93, 80, 0xFF, 61, 55, 0, 0 } },
    { "Lower Opponent's *b0 Attack Power to 0.", { 54, 0xFF, 17, 0xFF, 28, 75, 0, 0 } },
    { "Lower Opponent's *b1 Attack Power to 0.", { 84, 24, 28, 0xFF, 15, 0xFF, 0, 0 } },
    { "Lower Opponent's *b2 Attack Power to 0.", { 0xFF, 66, 24, 0xFF, 13, 45, 0, 0 } },
    { "*b0 Counterattack (Attack 2nd).", { 7, 0xFF, 33, 0xFF, 47, 14, 0, 0 } },
    { "*b1 Counterattack (Attack 2nd).", { 33, 17, 0xFF, 0xFF, 37, 26, 0, 0 } },
    { "*b2 Counterattack (Attack 2nd).", { 47, 43, 5, 0xFF, 0xFF, 37, 0, 0 } },
    { "If *a0 Opponent, X2 own Attack Power.", { 74, 79, 0xFF, 77, 25, 38, 0, 0 } },
    { "If *a0 Opponent, X3 own Attack Power.", { 0xFF, 91, 0xFF, 65, 74, 82, 0, 0 } },
    { "If *a1 Opponent, X2 own Attack Power.", { 5, 47, 64, 26, 80, 76, 0, 0 } },
    { "If *a1 Opponent, X3 own Attack Power.", { 60, 0xFF, 89, 83, 98, 91, 0, 0 } },
    { "If *a2 Opponent, X2 own Attack Power.", { 50, 27, 58, 59, 42, 6, 0, 0 } },
    { "If *a2 Opponent, X3 own Attack Power.", { 79, 0xFF, 93, 0xFF, 93, 70, 0, 0 } },
    { "If *a3 Opponent, X2 own Attack Power.", { 56, 37, 44, 6, 19, 93, 0, 0 } },
    { "If *a3 Opponent, X3 own Attack Power.", { 92, 60, 87, 79, 63, 0xFF, 0, 0 } },
    { "If *a4 Opponent, X2 own Attack Power.", { 78, 76, 26, 72, 69, 46, 0, 0 } },
    { "If *a4 Opponent, X3 own Attack Power.", { 89, 96, 55, 0xFF, 0xFF, 68, 0, 0 } },
    { "Change own Specialty to *a0.", { 9, 35, 0xFF, 48, 96, 0xFF, 0, 0 } },
    { "Change own Specialty to *a1.", { 0xFF, 99, 30, 87, 58, 0xFF, 0, 0 } },
    { "Change own Specialty to *a2.", { 97, 3, 41, 8, 12, 94, 0, 0 } },
    { "Change own Specialty to *a3.", { 30, 0xFF, 68, 98, 81, 3, 0, 0 } },
    { "Change own Specialty to *a4.", { 95, 82, 14, 52, 76, 99, 0, 0 } },
    { "Switch Opponent's Specialty to own.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Swap Specialty with Opponent's.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *a0 Opponent, lower its AP to 0.", { 66, 52, 0xFF, 97, 43, 0xFF, 0, 0 } },
    { "If *a1 Opponent, lower its AP to 0.", { 41, 61, 34, 84, 8, 0xFF, 0, 0 } },
    { "If *a2 Opponent, lower its AP to 0.", { 0xFF, 11, 52, 0xFF, 64, 22, 0, 0 } },
    { "If *a3 Opponent, lower its AP to 0.", { 0xFF, 25, 37, 13, 2, 72, 0, 0 } },
    { "If *a4 Opponent, lower its AP to 0.", { 0xFF, 75, 16, 0xFF, 34, 52, 0, 0 } },
    { "Reduce both Players' Atk Pwr to 0.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *e3, boost Attack Power +200.", { 14, 31, 0xFF, 9, 48, 23, 0, 0 } },
    { "If *e4, boost Attack Power +300.", { 31, 53, 0xFF, 20, 38, 57, 0, 0 } },
    { "If *e5, boost Attack Power +400.", { 57, 0xFF, 81, 70, 95, 77, 0, 0 } },
    { "Opponent uses *b0 Attack.", { 16, 62, 0xFF, 28, 97, 31, 0, 0 } },
    { "Opponent uses *b1 Attack.", { 62, 18, 11, 39, 0xFF, 47, 0, 0 } },
    { "Opponent uses *b2 Attack.", { 94, 8, 21, 0xFF, 44, 58, 0, 0 } },
    { "Opponent uses same Attack.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Recover HP +200.", { 72, 15, 32, 11, 1, 81, 0, 0 } },
    { "Recover HP +300.", { 90, 38, 53, 31, 17, 0xFF, 0, 0 } },
    { "Recover HP +400.", { 0xFF, 68, 92, 73, 55, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +400.", { 93, 20, 43, 15, 23, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +600.", { 0xFF, 46, 86, 44, 60, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +500.", { 98, 59, 23, 37, 29, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +700.", { 0xFF, 83, 69, 53, 56, 0xFF, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 300.", { 49, 34, 0xFF, 24, 30, 42, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 600.", { 0xFF, 63, 0xFF, 49, 49, 64, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 1000.", { 0xFF, 94, 0xFF, 64, 65, 98, 0, 0 } },
    { "Drop 1 Card in Opponent's Hand.", { 63, 22, 3, 45, 75, 27, 0, 0 } },
    { "Drop 2 Cards in Opponent's Hand.", { 0xFF, 78, 25, 0xFF, 92, 56, 0, 0 } },
    { "Drop Opponent's Top 2 DP Cards shown.", { 0xFF, 56, 18, 86, 35, 5, 0, 0 } },
    { "Drop Opponent's Top 3 DP Cards shown.", { 0xFF, 88, 49, 0xFF, 90, 32, 0, 0 } },
    { "Drop Opponent's Top 4 DP Cards shown.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Drop 2 Cards in Opponent's Online Deck.", { 0xFF, 51, 7, 29, 0xFF, 16, 0, 0 } },
    { "Drop 3 Cards in Opponent's Online Deck.", { 0xFF, 95, 35, 89, 0xFF, 54, 0, 0 } },
    { "Move Offline Top Card to Online Deck.", { 86, 0xFF, 70, 78, 50, 0xFF, 0, 0 } },
    { "Void Opponent's Support Effect.", { 0xFF, 84, 65, 0xFF, 87, 69, 0, 0 } },
    { "Draw until there are 4 Cards.", { 64, 45, 29, 54, 20, 0xFF, 0, 0 } },
    { "Draw Online Partner Card, then Shuffle.", { 65, 80, 82, 68, 72, 0xFF, 0, 0 } },
    { "If *e3, HP + 200 & all Attack Powers +100.", { 73, 73, 0xFF, 55, 94, 83, 0, 0 } },
    { "If *ea, HP + 200 & all Attack Powers +100.", { 81, 0xFF, 76, 66, 79, 87, 0, 0 } },
    { "Boost Battle Experience by 10%.", { 20, 49, 57, 32, 71, 41, 0, 0 } },
    { "Boost Battle Experience by 20%.", { 85, 65, 77, 75, 88, 92, 0, 0 } },
    { "Boost Battle Experience by 30%.", { 80, 0xFF, 96, 90, 99, 60, 0, 0 } },
    { "Rare Card might appear after battle.", { 70, 85, 83, 80, 51, 0xFF, 0, 0 } },
    { "Rare Card even more likely to appear.", { 0xFF, 90, 99, 96, 70, 0xFF, 0, 0 } },
};

void func_801E2098(TabWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    u8 rgb[4];
    s32 x;
    s32 y;
    s32 z;
    s32 player;
    s32 slot;
    s32 changed;
    s32 palette;
    s32 crossEffect;
    s32 next;
    s32 i;
    s32 partner;

    rgb[0] = window->window.brightness;
    rgb[1] = window->window.brightness;
    rgb[2] = window->window.brightness;
    x = window->window.originX;
    y = window->window.originY;
    z = window->window.z;
    player = D_801F299C;
    slot = window->slot;
    func_801E16A8(player, slot, x + 1, y + 12, window->window.brightness, z);
    if (PLAYER_DATA(player).partners[slot].cardId != 0) {
        D_801F29A0 = PLAYER_DATA(player).partners[slot];
        for (i = 0; i < 3; i++) {
            PLAYER_DATA(player).partners[slot].equippedAbilities[i] = -1;
        }
        updatePartnerStats(player, slot);
        D_801F2C40 = PLAYER_DATA(player).partners[slot];
        PLAYER_DATA(player).partners[slot] = D_801F29A0;
        changed = updatePartnerStats(player, slot);
        for (i = 0; i < 3; i++) {
            if (PLAYER_DATA(player).partners[slot].unlockedArmors[i] != 0) {
                partner = getSlotPartnerIndex(player, slot);
                drawIconColored(x + 2 + i * 13, y, 0, D_801F1FD0[partner][i] + 0x1B, rgb, z);
            }
        }
        drawTextColored(x + 0x32, y, (u8 *)PLAYER_DATA(player).partners[slot].card[0].name, rgb, 7, z);
        palette = func_801E1680(D_801F2C40.card[0].hp, PLAYER_DATA(player).partners[slot].card[0].hp);
        drawIconColored(x + 0x2C, y + 14, 0, 0x1A, rgb, z);
        sprintf(buf, "*s0%4d", PLAYER_DATA(player).partners[slot].card[0].hp);
        drawTextColored(x + 0x3C, y + 13, buf, rgb, palette, z);
        palette = func_801E1680(D_801F2C40.card[0].attack[0].power, PLAYER_DATA(player).partners[slot].card[0].attack[0].power);
        drawIconColored(x + 0x2C, y + 0x1A, 0, 7, rgb, z);
        sprintf(buf, "*s0%4d", PLAYER_DATA(player).partners[slot].card[0].attack[0].power);
        drawTextColored(x + 0x3C, y + 0x19, buf, rgb, palette, z);
        palette = func_801E1680(D_801F2C40.card[0].attack[1].power, PLAYER_DATA(player).partners[slot].card[0].attack[1].power);
        drawIconColored(x + 0x2C, y + 0x26, 0, 8, rgb, z);
        sprintf(buf, "*s0%4d", PLAYER_DATA(player).partners[slot].card[0].attack[1].power);
        drawTextColored(x + 0x3C, y + 0x25, buf, rgb, palette, z);
        palette = func_801E1680(D_801F2C40.card[0].attack[2].power, PLAYER_DATA(player).partners[slot].card[0].attack[2].power);
        drawIconColored(x + 0x2C, y + 0x32, 0, 9, rgb, z);
        sprintf(buf, "*s0%4d", PLAYER_DATA(player).partners[slot].card[0].attack[2].power);
        drawTextColored(x + 0x3C, y + 0x31, buf, rgb, palette, z);
        crossEffect = PLAYER_DATA(player).partners[slot].card[0].crossEffect;
        palette = 7;
        if (D_801F2C40.card[0].crossEffect != crossEffect) {
            palette = 5;
        }
        sprintf(buf, "(%s)", CROSS_EFFECT_SHORT_NAMES[crossEffect]);
        drawSmallTextColored(x + 0x56, y + 0x33, buf, palette, rgb, z);
        if (D_8006E4FC[crossEffect] != 0) {
            drawIconColored(x + 0x95, y + 0x31, 0, D_8006E4FC[crossEffect] + 0x14, rgb, z);
        }
        sprintf(buf, "RANK \f\a%3d", (s8)PLAYER_DATA(player).partners[slot].level);
        drawLargeTextColored(x + 0x5C, y + 14, buf, 6, rgb, z);
        next = 0;
        if ((s8)PLAYER_DATA(player).partners[slot].level < 99) {
            next = getExpForNextLevel((s8)PLAYER_DATA(player).partners[slot].level) - (u16)PLAYER_DATA(player).partners[slot].exp;
        }
        sprintf(buf, "NEXT \f\a%3d", next);
        drawLargeTextColored(x + 0x5C, y + 0x18, buf, 6, rgb, z);
        palette = func_801E1680(D_801F2C40.card[0].dpBonus, PLAYER_DATA(player).partners[slot].card[0].dpBonus);
        drawIconColored(x + 0x72, y + 0x26, 0, 0x19, rgb, z);
        sprintf(buf, "*s0%4d", PLAYER_DATA(player).partners[slot].card[0].dpBonus);
        drawTextColored(x + 0x84, y + 0x25, buf, rgb, palette, z);
        drawTextColored(x + 0xA8, y, (u8 *)"Support Effect", rgb, 6, z);
        palette = 7;
        if (changed) {
            palette = 5;
        }
        i = PLAYER_DATA(player).partners[slot].card[0].supportIcon;
        if (i != 0) {
            drawIconColored(x + 0x100, y, 0, i + 0x14, rgb, z);
        }
        for (i = 0; i < 4; i++) {
            drawTextColored(x + 0xA8, y + 0xD + i * 12, PLAYER_DATA(player).partners[slot].card[0].supportText[i], rgb, palette, z);
        }
    } else {
        drawTextColored(x + 0x78, y + 0x18, (u8 *)"No Data", rgb, 7, z);
    }
}

void func_801E2B8C(UiWindow *window) {
    s32 x = window->originX + 1;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = D_801F299C;
    s32 prevArmor;
    s32 i;

    if (PLAYER_DATA(player).partners[D_801F2994].unk292[0] != 0) {
        drawText(x + 6, y + 1, (s32)"Armor Change with L1 & R1", 7, z);
        prevArmor = D_801F2998;
        for (i = 0; i < 3; i++) {
            if ((u16)PAD_STATES[player]->pressed & 4) {
                D_801F2998--;
            } else if ((u16)PAD_STATES[player]->pressed & 8) {
                D_801F2998++;
            }
            D_801F2998 = (D_801F2998 + 3) % 3;
            if (PLAYER_DATA(player).partners[D_801F2994].unlockedArmors[D_801F2998] != 0) {
                break;
            }
        }
        if (prevArmor != D_801F2998) {
            playMenuSound(1);
            selectPartnerArmor(player, getSlotPartnerIndex(player, D_801F2994), D_801F2998);
        }
    }
}

void func_801E2DA4(UiWindow *window) {
    Rect16 rect;
    char buf[64];
    s32 x = window->originX + 3;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = D_801F299C;
    s32 changed;
    s32 canEquip;
    DigimonCardData *card = &PLAYER_DATA(player).partners[D_801F2994].card[0];
    s32 ability;
    s32 palette;
    s32 next;
    s32 diff;
    s32 partner;
    s32 i;

    D_801F29A0 = PLAYER_DATA(player).partners[D_801F2994];
    for (i = 0; i < 3; i++) {
        PLAYER_DATA(player).partners[D_801F2994].equippedAbilities[i] = -1;
    }
    updatePartnerStats(player, D_801F2994);
    D_801F2C40 = PLAYER_DATA(player).partners[D_801F2994];
    canEquip = 0;
    ability = D_801F3180[D_801F1F60.row];
    if (D_801F1F60.active != 0) {
        PLAYER_DATA(player).partners[D_801F2994] = D_801F29A0;
        if (getPartnerAbilityState(player, ability) == 1) {
            canEquip = canEquipPartnerAbility(player, D_801F2994, D_801F1F8C.row, ability);
            if (canEquip != 0) {
                PLAYER_DATA(player).partners[D_801F2994].equippedAbilities[D_801F1F8C.row] = ability;
                updatePartnerStats(player, D_801F2994);
                D_801F2EE0 = PLAYER_DATA(player).partners[D_801F2994];
            }
        }
    }
    PLAYER_DATA(player).partners[D_801F2994] = D_801F29A0;
    changed = updatePartnerStats(player, D_801F2994);
    func_801E16A8(player, D_801F2994, x, y, 0x80, z);
    drawMediumText(x + 0x2C, y, (s32)card->name, 7, z);
    sprintf(buf, "RANK \f\a%3d", (s8)PLAYER_DATA(player).partners[D_801F2994].level);
    drawLargeText(x + 0x2C, y + 10, (s32)buf, 6, z);
    next = 0;
    if ((s8)PLAYER_DATA(player).partners[D_801F2994].level < 99) {
        next = getExpForNextLevel((s8)PLAYER_DATA(player).partners[D_801F2994].level) - (u16)PLAYER_DATA(player).partners[D_801F2994].exp;
    }
    sprintf(buf, "NEXT \f\a%3d", next);
    drawLargeText(x + 0x2C, y + 0x14, (s32)buf, 6, z);
    palette = func_801E1680(D_801F2C40.card[0].hp, card->hp);
    drawIcon(x + 0x2C, y + 0x1E, 0, 0x1A, z);
    sprintf(buf, "*s0%4d", card->hp);
    drawText(x + 0x3A, y + 0x1E, (s32)buf, palette, z);
    palette = func_801E1680(D_801F2C40.card[0].dpBonus, card->dpBonus);
    drawIcon(x + 0x2C, y + 0x2A, 0, 0x19, z);
    sprintf(buf, "*s0%4d", card->dpBonus);
    drawText(x + 0x3A, y + 0x2A, (s32)buf, palette, z);
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).partners[D_801F2994].unlockedArmors[i] != 0) {
            partner = getSlotPartnerIndex(player, D_801F2994);
            drawIcon(x + 0x76 + i * 20, y, 0, D_801F1FD0[partner][i] + 0x1B, z);
        }
    }
    palette = func_801E1680(D_801F2C40.card[0].attack[0].power, card->attack[0].power);
    drawIcon(x + 0x76, y + 12, 0, 7, z);
    sprintf(buf, "*s0%4d", card->attack[0].power);
    drawText(x + 0x84, y + 12, (s32)buf, palette, z);
    palette = func_801E1680(D_801F2C40.card[0].attack[1].power, card->attack[1].power);
    drawIcon(x + 0x76, y + 0x18, 0, 8, z);
    sprintf(buf, "*s0%4d", card->attack[1].power);
    drawText(x + 0x84, y + 0x18, (s32)buf, palette, z);
    palette = func_801E1680(D_801F2C40.card[0].attack[2].power, card->attack[2].power);
    drawIcon(x + 0x76, y + 0x24, 0, 9, z);
    sprintf(buf, "*s0%4d", card->attack[2].power);
    drawText(x + 0x84, y + 0x24, (s32)buf, palette, z);
    palette = 7;
    if (D_801F2C40.card[0].crossEffect != card->crossEffect) {
        palette = 5;
    }
    sprintf(buf, "(%s)", CROSS_EFFECT_SHORT_NAMES[card->crossEffect]);
    drawSmallText(x + 0x74, y + 0x30, (s32)buf, palette, z);
    if (D_8006E4FC[card->crossEffect] != 0) {
        drawIcon(x + 0xB2, y + 0x2E, 0, D_8006E4FC[card->crossEffect] + 0x14, z);
    }
    if (canEquip) {
        diff = D_801F2EE0.card[0].hp - card->hp;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x54, y + 0x1E, (s32)buf, palette, z);
        }
        diff = D_801F2EE0.card[0].dpBonus - card->dpBonus;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x54, y + 0x2A, (s32)buf, palette, z);
        }
        diff = D_801F2EE0.card[0].attack[0].power - card->attack[0].power;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x9E, y + 12, (s32)buf, palette, z);
        }
        diff = D_801F2EE0.card[0].attack[1].power - card->attack[1].power;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x9E, y + 0x18, (s32)buf, palette, z);
        }
        diff = D_801F2EE0.card[0].attack[2].power - card->attack[2].power;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x9E, y + 0x24, (s32)buf, palette, z);
        }
    }
    drawText(x, y + 0x36, (s32)"Support Effect", 6, z);
    palette = 7;
    if (changed) {
        palette = 5;
    }
    if (card->supportIcon != 0) {
        drawIcon(x + 0x58, y + 0x36, 0, card->supportIcon + 0x14, z);
    }
    for (i = 0; i < 4; i++) {
        drawText(x, y + 0x46 + i * 12, (s32)card->supportText[i], palette, z);
    }
    rect.x = x;
    rect.y = y + 0x46;
    rect.w = 0x6E;
    rect.h = 0x30;
    drawWindowFrame(&rect, 0x31, 0, 0x80, 1, z);
}

void func_801E3BB4(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    s32 canEquip;
    s32 x = window->originX + 3;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = D_801F299C;
    DigimonCardData *card = &PLAYER_DATA(player).partners[D_801F2994].card[1];
    s32 ability;
    s32 palette;
    s32 diff;
    s32 i;

    func_801E1B70(player, D_801F2994, x + 0x14, y + 0xA, z);
    if (PLAYER_DATA(player).partners[D_801F2994].unk292[0] != 0) {
        D_801F29A0 = PLAYER_DATA(player).partners[D_801F2994];
        for (i = 0; i < 3; i++) {
            PLAYER_DATA(player).partners[D_801F2994].equippedAbilities[i] = -1;
        }
        updatePartnerStats(player, D_801F2994);
        D_801F2C40 = PLAYER_DATA(player).partners[D_801F2994];
        canEquip = 0;
        ability = D_801F3180[D_801F1F60.row];
        if (D_801F1F60.active != 0) {
            PLAYER_DATA(player).partners[D_801F2994] = D_801F29A0;
            if (getPartnerAbilityState(player, ability) == 1) {
                canEquip = canEquipPartnerAbility(player, D_801F2994, D_801F1F8C.row, ability);
                if (canEquip != 0) {
                    PLAYER_DATA(player).partners[D_801F2994].equippedAbilities[D_801F1F8C.row] = ability;
                    updatePartnerStats(player, D_801F2994);
                    D_801F2EE0 = PLAYER_DATA(player).partners[D_801F2994];
                }
            }
        }
        PLAYER_DATA(player).partners[D_801F2994] = D_801F29A0;
        updatePartnerStats(player, D_801F2994);
        drawMediumText(x, y, (s32)card->name, 7, z);
        drawIcon(x + 0x44, y + 12, 0, D_801F1FD0[getSlotPartnerIndex(player, D_801F2994)][getSelectedArmorIndex(player, getSlotPartnerIndex(player, D_801F2994))] + 0x1B, z);
        palette = func_801E1680(D_801F2C40.card[1].hp, card->hp);
        drawIcon(x, y + 0x3E, 0, 0x1A, z);
        sprintf(buf, "*s0%4d", card->hp);
        drawText(x + 0xE, y + 0x3E, (s32)buf, palette, z);
        palette = func_801E1680(D_801F2C40.card[1].attack[0].power, card->attack[0].power);
        drawIcon(x, y + 0x4A, 0, 7, z);
        sprintf(buf, "*s0%4d", card->attack[0].power);
        drawText(x + 0xE, y + 0x4A, (s32)buf, palette, z);
        palette = func_801E1680(D_801F2C40.card[1].attack[1].power, card->attack[1].power);
        drawIcon(x, y + 0x56, 0, 8, z);
        sprintf(buf, "*s0%4d", card->attack[1].power);
        drawText(x + 0xE, y + 0x56, (s32)buf, palette, z);
        palette = func_801E1680(D_801F2C40.card[1].attack[2].power, card->attack[2].power);
        drawIcon(x, y + 0x62, 0, 9, z);
        sprintf(buf, "*s0%4d", card->attack[2].power);
        drawText(x + 0xE, y + 0x62, (s32)buf, palette, z);
        palette = 7;
        if (D_801F2C40.card[1].crossEffect != card->crossEffect) {
            palette = 5;
        }
        sprintf(buf, "(%s)", CROSS_EFFECT_SHORT_NAMES[card->crossEffect]);
        drawSmallText(x - 2, y + 0x6E, (s32)buf, palette, z);
        if (D_8006E4FC[card->crossEffect] != 0) {
            drawIcon(x + 0x3C, y + 0x6C, 0, D_8006E4FC[card->crossEffect] + 0x14, z);
        }
        if (canEquip) {
            diff = D_801F2EE0.card[1].hp - card->hp;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x3E, (s32)buf, palette, z);
            }
            diff = D_801F2EE0.card[1].attack[0].power - card->attack[0].power;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x4A, (s32)buf, palette, z);
            }
            diff = D_801F2EE0.card[1].attack[1].power - card->attack[1].power;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x56, (s32)buf, palette, z);
            }
            diff = D_801F2EE0.card[1].attack[2].power - card->attack[2].power;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x62, (s32)buf, palette, z);
            }
        }
    } else {
        drawText(x + 0x14, y + 0x4A, (s32)"No Data", 7, z);
    }
}

void func_801E4560(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    s32 x = window->originX;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = D_801F299C;
    s32 i;
    s32 ability;
    s32 palette;

    for (i = 0; i < 3; i++) {
        ability = PLAYER_DATA(player).partners[D_801F2994].equippedAbilities[i];
        if (ability == -1) {
            drawText(x, y + i * 14, (s32)"---", 7, z);
            drawText(x + 0x1C, y + i * 14, (s32)"None", 7, z);
        } else {
            palette = 7;
            if (PARTNER_ABILITIES[ability].type == 5) {
                palette = 4;
            }
            if (PARTNER_ABILITIES[ability].type == 7) {
                palette = 5;
            }
            sprintf(buf, "*s0%3.3d", ability);
            drawText(x, y + i * 14, (s32)buf, 7, z);
            drawIcon(x + 0x1A, y + i * 14, 2, PARTNER_ABILITIES[ability].type, z);
            drawText(x + 0x36, y + i * 14, (s32)D_801F1960[ability].name, palette, z);
        }
    }
    updateMenuCursor(&D_801F1F8C);
}

void func_801E4788(UiWindow *window) {
    char buf[72];
    u8 rgb[4];
    s32 player = D_801F299C;
    s32 x = window->originX;
    s32 z = window->z;
    s32 i;
    s32 ability;
    s32 y;
    s32 palette;
    s32 state;

    for (i = 0; i < D_801F1F60.nrows; i++) {
        if (i < window->view.y / D_801F1F60.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / D_801F1F60.rowH < i) {
            break;
        }
        ability = D_801F3180[i];
        y = window->originY + i * D_801F1F60.rowH + 1;
        state = getPartnerAbilityState(player, ability);
        switch (state) {
        case 1:
            rgb[0] = 0x80;
            rgb[1] = 0x80;
            rgb[2] = 0x80;
            break;
        case 2:
            rgb[0] = 0x40;
            rgb[1] = 0x40;
            rgb[2] = 0x40;
            break;
        }
        palette = 7;
        if (PARTNER_ABILITIES[ability].type == 5) {
            palette = 4;
        }
        if (PARTNER_ABILITIES[ability].type == 7) {
            palette = 5;
        }
        sprintf(buf, "*s0%3.3d", ability);
        drawTextColored(x, y, buf, rgb, palette, z);
        drawIconColored(x + 0x1A, y, 2, PARTNER_ABILITIES[ability].type, rgb, z);
        drawTextColored(x + 0x36, y, (u8 *)D_801F1960[ability].name, rgb, palette, z);
    }
    updateMenuCursor(&D_801F1F60);
}

void func_801E49CC(void) {
    s32 i;

    func_801E14E4();
    for (i = 0; i < 3; i++) {
        D_801F2870[i].window.brightness = 0x40;
    }
    D_801F2870[D_801F2994].window.brightness = 0x80;
    drawWindow(&D_801F2870[D_801F2994].window, func_801E2098, 30);
    for (i = 0; i < 3; i++) {
        if (i != D_801F2994) {
            drawWindow(&D_801F2870[i].window, func_801E2098, 30);
        }
    }
    drawWindow(&D_801F27D0, func_801E2B8C, 30);
    drawWindow(&D_801F2950, func_801E3BB4, 30);
    drawWindow(&D_801F2820, func_801E2DA4, 30);
    drawWindow(&D_801F2690, func_801E4788, 30);
    drawWindow(&D_801F2730, func_801E4560, 30);
}

void func_801E4B34(s32 player, s32 parentTask, s32 viewOnly) {
    Rect16 rect;
    u8 dialog[0xB8];
    u32 *archive;
    s32 state;
    s32 count;
    s32 ability;
    s32 result;
    s32 i;

    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\PARTNER.ARC", getCurrentTaskId());
    archive = (u32 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < (s32)(archive[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)archive + archive[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(archive);
    D_801F3178 = 1;
    D_801F317C = -0x20;
    state = 0;
    D_801F2994 = 0;
    D_801F299C = player;
    D_801F2998 = 0;
    count = 0;
    for (i = 0; i < 128; i++) {
        D_801F3180[i] = -1;
    }
    for (i = 0; i < 128; i++) {
        if (getPartnerAbilityState(player, i)) {
            D_801F3180[count] = i;
            count++;
        }
    }
    D_801F1F60.nrows = count;
    D_801F1F60.pad = player;
    D_801F1F8C.pad = player;
    for (i = 0; i < 3; i++) {
        rect.x = 0x10;
        rect.y = i * 65 + 0x26;
        rect.w = 0x120;
        rect.h = 0x3C;
        openWindow(&D_801F2870[i], &rect, -1, (s16 *)-1, 8, 0x46, 0x80, 12);
        D_801F2870[i].slot = i;
        switch (i) {
        case 0:
            D_801F2870[i].window.label = (s32)"PARTNER 1 ";
            break;
        case 1:
            D_801F2870[i].window.label = (s32)"PARTNER 2 ";
            break;
        case 2:
            D_801F2870[i].window.label = (s32)"PARTNER 3 ";
            break;
        }
    }
    openMenu(&D_801F1F60, &D_801F2690, &D_801F26E0, (Bytes4 *)-1);
    D_801F2690.label = (s32)"Digi-Parts List";
    D_801F2690.palette = 4;
    animateWindowTo(&D_801F2690, (Rect16 *)-1);
    D_801F1F60.active = 0;
    openMenu(&D_801F1F8C, &D_801F2730, &D_801F2780, (Bytes4 *)-1);
    D_801F2730.label = (s32)"EQUIPMENT";
    animateWindowTo(&D_801F2730, (Rect16 *)-1);
    openWindow(&D_801F27D0, &D_801F1FC8, -1, (s16 *)-1, 8, 0x32, 0x80, 12);
    animateWindowTo(&D_801F27D0, (Rect16 *)-1);
    openWindow(&D_801F2820, &D_801F1FB8, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    D_801F2820.label = (s32)"PARTNER";
    animateWindowTo(&D_801F2820, (Rect16 *)-1);
    openWindow(&D_801F2950, &D_801F1FC0, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    D_801F2950.label = (s32)"ARMOR";
    animateWindowTo(&D_801F2950, (Rect16 *)-1);
    addFrameCallback((s32)func_801E49CC);
    for (;;) {
        func_80014C08(FRAME_INTERVAL);
        switch (state) {
        case -1:
            D_801F3178 = 0;
            func_80014C08(20);
            removeFrameCallback((s32)func_801E49CC);
            func_80014A48(parentTask);
            return;
        case 0:
            if (D_801F2991 != 0 && D_801F2861 != 0) {
                if (PAD_STATES[player]->pressed & 0x1000) {
                    playMenuSound(2);
                    D_801F2994--;
                } else if (PAD_STATES[player]->pressed & 0x4000) {
                    playMenuSound(2);
                    D_801F2994++;
                }
                D_801F2994 = (D_801F2994 + 3) % 3;
            }
            if (PAD_STATES[player]->pressed & 0x40) {
                if (PLAYER_DATA(player).partners[D_801F2994].cardId != 0) {
                    playMenuSound(1);
                    D_801F2998 = getSelectedArmorIndex(player, getSlotPartnerIndex(player, D_801F2994));
                    for (i = 0; i < 3; i++) {
                        animateWindowTo(&D_801F2870[i].window, (Rect16 *)-1);
                    }
                    animateWindowTo(&D_801F2820, &D_801F1FB8);
                    if (countUnlockedPartnerArmors(player, getSlotPartnerIndex(player, D_801F2994)) >= 2) {
                        animateWindowTo(&D_801F27D0, &D_801F1FC8);
                    }
                    animateWindowTo(&D_801F2950, &D_801F1FC0);
                    animateWindowTo(&D_801F2730, &D_801F1F8C.rect);
                    if (viewOnly == 0) {
                        PLAYER_DATA(player).unk56 = getSlotPartnerIndex(0, D_801F2994);
                        changeScrollingBackground(PLAYER_DATA(player).unk56, 0x380, 0, 0x380, 0x80);
                    }
                    state = 1;
                }
            } else if (PAD_STATES[player]->pressed & 0x10) {
                for (i = 0; i < 3; i++) {
                    animateWindowTo(&D_801F2870[i].window, (Rect16 *)-1);
                }
                playMenuSound(4);
                state = -1;
            }
            break;
        case 1:
            if (PAD_STATES[player]->pressed & 0x40) {
                playMenuSound(1);
                animateWindowTo(&D_801F2690, &D_801F1F60.rect);
                D_801F1F8C.active = 0;
                D_801F1F60.active = 1;
                ability = PLAYER_DATA(player).partners[D_801F2994].equippedAbilities[D_801F1F8C.row];
                D_801F1F60.row = 0;
                if (ability != -1) {
                    for (i = 0; i < D_801F1F60.nrows && D_801F3180[i] != ability; i++) {
                        D_801F1F60.row++;
                    }
                }
                centerMenuOnCursor(&D_801F1F60);
                state = 2;
            } else if (PAD_STATES[player]->pressed & 0x80) {
                if (PLAYER_DATA(player).partners[D_801F2994].equippedAbilities[D_801F1F8C.row] != -1) {
                    playMenuSound(1);
                    unequipPartnerAbility(player, D_801F2994, D_801F1F8C.row);
                }
            } else if (PAD_STATES[player]->pressed & 0x10) {
                playMenuSound(0);
                for (i = 0; i < 3; i++) {
                    rect.x = 0x10;
                    rect.y = i * 65 + 0x26;
                    rect.w = 0x120;
                    rect.h = 0x3C;
                    animateWindowTo(&D_801F2870[i].window, &rect);
                }
                animateWindowTo(&D_801F2950, (Rect16 *)-1);
                animateWindowTo(&D_801F2820, (Rect16 *)-1);
                animateWindowTo(&D_801F27D0, (Rect16 *)-1);
                animateWindowTo(&D_801F2730, (Rect16 *)-1);
                state = 0;
            }
            break;
        case 2:
            if (PAD_STATES[player]->pressed & 0x40) {
                ability = D_801F3180[D_801F1F60.row];
                result = getPartnerAbilityState(player, ability);
                if (result == 1) {
                    playMenuSound(1);
                    if (canEquipPartnerAbility(player, D_801F2994, D_801F1F8C.row, ability)) {
                        equipPartnerAbility(player, D_801F2994, D_801F1F8C.row, ability);
                        animateWindowTo(&D_801F2690, (Rect16 *)-1);
                        D_801F1F8C.active = result;
                        D_801F1F60.active = 0;
                        state = 1;
                    } else {
                        initDialog(dialog, "The same Digi-Part is already used. Only\n1 Digi-Part of each kind can be used.", 0);
                        runDialogForPad((s32 *)dialog, player);
                    }
                }
            } else if (PAD_STATES[player]->pressed & 0x10) {
                playMenuSound(0);
                animateWindowTo(&D_801F2690, (Rect16 *)-1);
                D_801F1F8C.active = 1;
                D_801F1F60.active = 0;
                state = 1;
            }
            break;
        }
    }
}

s32 func_801E56CC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E57FC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 1) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 1) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E592C(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5A60(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5B94(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 4) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 4) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5CC8(s8 **a, s8 **b) {
    s32 x = (*a)[2] == 1;
    s32 y = (*b)[2] == 1;

    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5DB0(s8 **a, s8 **b) {
    s32 x = (*a)[2] == 2;
    s32 y = (*b)[2] == 2;

    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5E98(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5FC8(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E60FC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E6230(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 1) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 1) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E6360(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->hp;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->hp;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E6468(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if (D_801F41A8.useDeckCounts == 0) {
        x = getOwnedCardCount(D_801F41A8.player, *(s16 *)*a);
        y = getOwnedCardCount(D_801F41A8.player, *(s16 *)*b);
    } else {
        x = D_801F3F48.deckCounts[*(s16 *)*a];
        y = D_801F3F48.deckCounts[*(s16 *)*b];
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E65DC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->dpCost;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->dpCost;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E66E4(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->dpBonus;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->dpBonus;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E67EC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attack[0].power;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attack[0].power;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E68F4(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attack[1].power;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attack[1].power;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E69FC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attack[2].power;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attack[2].power;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E6B04(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if (PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x80) {
        x = 1;
    } else {
        x = 0;
    }
    y = (PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x80) != 0;
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

void func_801E6BE8(void) {
    s32 n = 0;
    s32 i;

    for (i = 0; i < 0xBF; i++) {
        D_801F34D0[n++] = &((DigimonCardData *)DIGIMON_CARDS)[i];
    }
    for (i = 0; i < 0x66; i++) {
        D_801F34D0[n++] = &((OptionCardData *)OPTION_CARDS)[i];
    }
    for (i = 0; i < 8; i++) {
        D_801F34D0[n++] = &((DigivolveCardData *)DIGIVOLVE_CARDS)[i];
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(D_801F41A8.player).partners[i].cardId != 0) {
            D_801F34D0[PLAYER_DATA(D_801F41A8.player).partners[i].cardId] = &PLAYER_DATA(D_801F41A8.player).partners[i].card[0];
            if (PLAYER_DATA(D_801F41A8.player).partners[i].unk292[0] != 0) {
                D_801F34D0[PLAYER_DATA(D_801F41A8.player).partners[i].unk292[0]] = &PLAYER_DATA(D_801F41A8.player).partners[i].card[1];
            }
        }
    }
    for (i = 0; i < 301; i++) {
        D_801F3990[i] = D_801F34D0[i];
    }
}

void func_801E6DA8(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 z = window->z;
    s32 i;
    s32 y;

    for (i = 0; i < D_801F20D8.nrows; i++) {
        if (i < window->view.y / D_801F20D8.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / D_801F20D8.rowH < i) {
            break;
        }
        y = window->originY + i * D_801F20D8.rowH + 1;
        if (D_801F41A8.useDeckCounts == 0) {
            drawText(x, y, (s32)D_801F2038[i], 7, z);
        } else {
            drawText(x, y, (s32)D_801F2088[i], 7, z);
        }
    }
    updateMenuCursor(&D_801F20D8);
    if (D_801F20D8.active && D_801F41A8.listShown && (PAD_STATES[D_801F41A8.player]->pressed & 0x40)) {
        playMenuSound(1);
        D_801F1FE4.row = 0;
        centerMenuOnCursor(&D_801F1FE4);
        if (D_801F2104[D_801F20D8.row] != NULL) {
            sortArray((s8 *)D_801F34D0, 301, 4, D_801F2104[D_801F20D8.row]);
        } else {
            func_801E6BE8();
        }
    }
}


Menu D_801F1F60 = { NULL, NULL, { 12, 184, 288, 42 }, 0, -1, 0, -1, 0xa, 0x21, 264, 12, 1, 128, 24, 1, 0, 14, 0, 0, 0 };
Menu D_801F1F8C = { NULL, NULL, { 12, 184, 288, 42 }, 0, -1, 0, -1, 0x8, 0x21, 264, 12, 1, 3, 26, 1, 0, 14, 0, 0, 0 };
Rect16 D_801F1FB8 = { 12, 48, 192, 122 };
Rect16 D_801F1FC0 = { 214, 48, 88, 122 };
Rect16 D_801F1FC8 = { 146, 19, 157, 14 };

/* the icons of each partner's armors */
u8 D_801F1FD0[6][3] = {
    { 1, 2, 9 },
    { 3, 5, 0 },
    { 4, 6, 0 },
    { 7, 6, 0 },
    { 8, 1, 0 },
    { 1, 7, 0 },
};

/* not referenced by any code */
s16 D_801F1FE2 = 1;

Menu D_801F1FE4 = { NULL, NULL, { 15, 44, 298, 96 }, 0, -1, 0, -1, 0xa, 0x41, 115, 12, 0, 301, 147, 1, 0, 12, 0, 0, 0 };

/* the partners' names; not referenced by any code */
char *D_801F2010[6] = {
    "\xCC\xDE\xB2\xD3\xDD", /* ﾌﾞｲﾓﾝ */
    "\xCE\xB0\xB8\xD3\xDD", /* ﾎｰｸﾓﾝ */
    "\xB1\xD9\xCF\xBC\xDE\xD3\xDD", /* ｱﾙﾏｼﾞﾓﾝ */
    "\xC3\xB2\xD9\xD3\xDD", /* ﾃｲﾙﾓﾝ */
    "\xCA\xDF\xC0\xD3\xDD", /* ﾊﾟﾀﾓﾝ */
    "\xDC\xB0\xD1\xD3\xDD", /* ﾜｰﾑﾓﾝ */
};

u8 D_801F2028[4][4] = {
    { 0x80, 0x80, 0x80, 0 },
    { 0x40, 0x40, 0x40, 0 },
    { 0x60, 0x60, 0x60, 0 },
    { 0xC0, 0x60, 0x60, 0 },
};

/* the card list's sort orders */
char *D_801F2038[20] = {
    "Number",
    "*a0 Fire",
    "*a1 Ice",
    "*a2 Nature",
    "*a3 Darkness",
    "*a4 Rare",
    "*a5 Option",
    "*a6 Option",
    "Level *e3",
    "Level *ea",
    "Level *e4",
    "Level *e5",
    "*d6 Strength",
    "*d4 Required DP",
    "*d5 Added DP",
    "*b0 Attack Power",
    "*b1 Attack Power",
    "*b2 Attack Power",
    "*g9 Newly Obtained",
    "Cards in Stock",
};

char *D_801F2088[20] = {
    "Number",
    "*a0 Fire",
    "*a1 Ice",
    "*a2 Nature",
    "*a3 Darkness",
    "*a4 Rare",
    "*a5 Option",
    "*a6 Option",
    "Level *e3",
    "Level *ea",
    "Level *e4",
    "Level *e5",
    "*d6 Strength",
    "*d4 Required DP",
    "*d5 Added DP",
    "*b0 Attack Power",
    "*b1 Attack Power",
    "*b2 Attack Power",
    "*g9 Newly Obtained",
    "Max Usable Cards",
};

Menu D_801F20D8 = { NULL, NULL, { 40, 60, 124, 112 }, 0, -1, 0, -1, 0xa, 0x16, 114, 12, 0, 20, 0, 1, 0, 14, 0, 0, 0 };

SortCompare D_801F2104[20] = {
    NULL,
    (SortCompare)func_801E56CC,
    (SortCompare)func_801E57FC,
    (SortCompare)func_801E592C,
    (SortCompare)func_801E5A60,
    (SortCompare)func_801E5B94,
    (SortCompare)func_801E5CC8,
    (SortCompare)func_801E5DB0,
    (SortCompare)func_801E5E98,
    (SortCompare)func_801E6230,
    (SortCompare)func_801E5FC8,
    (SortCompare)func_801E60FC,
    (SortCompare)func_801E6360,
    (SortCompare)func_801E65DC,
    (SortCompare)func_801E66E4,
    (SortCompare)func_801E67EC,
    (SortCompare)func_801E68F4,
    (SortCompare)func_801E69FC,
    (SortCompare)func_801E6B04,
    (SortCompare)func_801E6468,
};

void func_801E6FC8(UiWindow *window) {
    char buf[64];
    s32 x = window->originX - 4;
    s32 z = window->z;
    s32 i;
    s32 y;
    s32 palette;
    s8 type;
    u8 *rgb;
    s32 count;

    for (i = 0; i < D_801F1FE4.nrows; i++) {
        if (i < window->view.y / D_801F1FE4.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / D_801F1FE4.rowH < i) {
            break;
        }
        y = window->originY + i * D_801F1FE4.rowH + 1;
        type = ((s8 *)D_801F34D0[i])[2];
        rgb = D_801F2028[0];
        palette = 7;
        count = getOwnedCardCount(D_801F41A8.player, *(s16 *)D_801F34D0[i]);
        if (PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)D_801F34D0[i]] & 0x40) {
            if (count == 0) {
                rgb = D_801F2028[2];
            }
            drawTextColored(x + 0x86, y, (u8 *)D_801F34D0[i] + 3, rgb, palette, z);
            switch (type) {
            case 0:
                drawIconColored(x + 0x74, y, 0, ((DigimonCardData *)D_801F34D0[i])->attr >> 4, rgb, z);
                if (palette == 3) {
                    drawIconColored(x + 0x46, y, 0, (((DigimonCardData *)D_801F34D0[i])->attr & 0xF) + 0x10, D_801F2028[3], z);
                } else {
                    drawIconColored(x + 0x46, y, 0, (((DigimonCardData *)D_801F34D0[i])->attr & 0xF) + 0x10, rgb, z);
                }
                break;
            case 1:
                drawIconColored(x + 0x74, y, 0, 5, rgb, z);
                break;
            case 2:
                drawIconColored(x + 0x74, y, 0, 6, rgb, z);
                break;
            }
        } else {
            palette = 9;
            drawTextColored(x + 0x46, y, "?", rgb, palette, z);
            drawTextColored(x + 0x86, y, "-------------------", rgb, palette, z);
            drawTextColored(x + 0x74, y, "?", rgb, palette, z);
        }
        if (type == 0 || !(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)D_801F34D0[i]] & 0x40)) {
            drawTextColored(x + 0x38, y, D_801DF27C, rgb, palette, z);
            drawTextColored(x + 0x56, y, D_801DF280, rgb, palette, z);
        }
        sprintf(buf, D_801DF288, *(s16 *)D_801F34D0[i]);
        drawTextColored(x + 0x20, y, buf, rgb, palette, z);
        sprintf(buf, D_801DF294, count);
        drawTextColored(x + 0x100, y, buf, rgb, palette, z);
        drawTextColored(x + 0x108, y, D_801DF298, rgb, palette, z);
        if (PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)D_801F34D0[i]] & 0x80) {
            drawIconColored(x + 4, y, 2, 9, rgb, z);
        }
        if (PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)D_801F34D0[i]] & 0x10) {
            drawIconColored(x + 10, y, 0, 0x14, rgb, z);
        }
    }
    updateMenuCursor(&D_801F1FE4);
    D_801F4330.selectedCard = D_801F34D0[D_801F1FE4.row];
    D_801F4330.selectedId = *(s16 *)D_801F4330.selectedCard;
}

s32 func_801E75A0(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type == 0xFF) {
        x = -2;
    } else {
        x = 301 - *(s16 *)a->card;
    }
    if (b->type == 0xFF) {
        y = -2;
    } else {
        y = 301 - *(s16 *)b->card;
    }
    return y - x;
}

s32 func_801E7600(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7690(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 1) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 1) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7724(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E77B8(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E784C(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 4) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 4) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E78E0(CardSlot *a, CardSlot *b) {
    s32 x = a->type == 1;
    s32 y = b->type == 1;

    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E792C(CardSlot *a, CardSlot *b) {
    s32 x = a->type == 2;
    s32 y = b->type == 2;

    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7978(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr & 0xF;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr & 0xF;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7A08(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr & 0xF;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr & 0xF;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7A9C(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr & 0xF;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr & 0xF;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

void func_801E7B30(CardSlot *cards, s32 player) {
    s32 i;
    s32 j;
    s32 cardId;

    for (i = 0; i < 3; i++) {
        cardId = PLAYER_DATA(player).partners[i].cardId;
        if (cardId != 0) {
            for (j = 0; j < 30; j++) {
                if (cards[j].type != 0xFF && *(s16 *)cards[j].card == cardId) {
                    cards[j].card = (s8 *)&PLAYER_DATA(player).partners[i];
                    break;
                }
            }
        }
    }
}

s32 func_801E7C44(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->hp;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->hp;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7CAC(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->dpCost;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->dpCost;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7D14(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->dpBonus;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->dpBonus;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7D7C(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attack[0].power;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attack[0].power;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7DE4(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attack[1].power;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attack[1].power;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7E4C(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attack[2].power;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attack[2].power;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7EB4(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type == 0xFF) {
        x = -1;
    } else {
        x = getOwnedCardCount(D_801F41A8.player, *(s16 *)a->card) - D_801F3F48.deckCounts[*(s16 *)a->card];
    }
    if (b->type == 0xFF) {
        y = -1;
    } else {
        y = getOwnedCardCount(D_801F41A8.player, *(s16 *)b->card) - D_801F3F48.deckCounts[*(s16 *)b->card];
    }
    return y - x;
}

void func_801E7F8C(CardSlot *cards) {
    CardSlot sorted[30];
    s16 ids[30];
    CardSlot *out = sorted;
    s8 count = 0;
    s32 i;
    s32 j;

    for (i = 0; i < 30; i++) {
        sorted[i].type = 0xFF;
        if (cards[i].type == 0xFF) {
            ids[i] = -1;
        } else {
            ids[i] = *(s16 *)cards[i].card;
        }
    }
    for (i = 0; i < 30; i++) {
        if (ids[i] != -1) {
            for (j = i; j < 30; j++) {
                if (j == i) {
                    count = 0;
                } else if (ids[i] == ids[j]) {
                    ids[j] = -1;
                    count++;
                }
            }
            while (count >= 0) {
                count--;
                *out++ = cards[i];
            }
        }
    }
    for (i = 0; i < 30; i++) {
        cards[i] = sorted[i];
    }
}

void func_801E8110(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 z = window->z;
    s32 i;

    for (i = 0; i < D_801F219C.nrows; i++) {
        if (i < window->view.y / D_801F219C.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / D_801F219C.rowH < i) {
            break;
        }
        drawText(x, window->originY + i * D_801F219C.rowH + 1, (s32)D_801F2154[i], 7, z);
    }
    updateMenuCursor(&D_801F219C);
    if (D_801F219C.active && (PAD_STATES[D_801F41A8.player]->pressed & 0x40)) {
        playMenuSound(1);
        if (D_801F21C8[D_801F219C.row] != NULL) {
            linkDeckCardData(D_801F41A8.player, D_801F4328);
            func_801E7B30(D_801F4328->cards, D_801F41A8.player);
            sortArray((s8 *)D_801F4328->cards, 30, 8, D_801F21C8[D_801F219C.row]);
            if (D_801F219C.row == 17) {
                func_801E7F8C(D_801F4328->cards);
                linkDeckCardData(D_801F41A8.player, D_801F4328);
            }
        }
    }
}

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF27C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF280);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF288);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF294);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF298);

/* the deck's sort orders */
char *D_801F2154[18] = {
    "Number",
    "*a0 Fire",
    "*a1 Ice",
    "*a2 Nature",
    "*a3 Darkness",
    "*a4 Rare",
    "*a5 Option",
    "*a6 Option",
    "Level *e3",
    "Level *e4",
    "Level *e5",
    "*d6 Strength",
    "*d4 Required DP",
    "*d5 Added DP",
    "*b0 Attack Power",
    "*b1 Attack Power",
    "*b2 Attack Power",
    "Cards Used",
};

Menu D_801F219C = { NULL, NULL, { 40, 60, 100, 112 }, 0, -1, 0, -1, 0xa, 0x16, 90, 12, 0, 18, 0, 1, 0, 14, 0, 0, 0 };

void func_801E831C(void) {
    char path[64];
    u32 *tim;

    sprintf(path, "C:\\OBJECT\\c_map.tim");
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTim(tim, 0x140, 0, 0, 0x1F4);
    freeHeapBlock(tim);
    printf("aaa\n");
}

void func_801E83BC(void) {
    char path[64];
    s32 i;
    u32 *tim;
    s32 slot;
    s8 found;

    for (i = 0; i < 8; i++) {
        D_801F4188.ids[i] = -1;
        D_801F4188.ages[i] = 0;
    }
    D_801F4188.running = 1;
    D_801F4188.request = -1;
    D_801F4188.busy = 0;
    do {
        func_80014C08(1);
        if (D_801F4188.busy == 0) {
            found = -1;
            for (i = 0; i < 8; i++) {
                if (D_801F4188.request == D_801F4188.ids[i]) {
                    found = i;
                }
            }
            if (found == -1) {
                slot = 0;
                D_801F4188.busy = 1;
                for (i = 1; i < 8; i++) {
                    if (i == 1) {
                        slot = 0;
                    }
                    if (D_801F4188.ages[slot] > D_801F4188.ages[i]) {
                        slot = i;
                    }
                }
                for (i = 0; i < 8; i++) {
                    if (i != slot) {
                        if (--D_801F4188.ages[i] < 0) {
                            D_801F4188.ages[i] = 0;
                        }
                    } else {
                        D_801F4188.ids[i] = D_801F4188.request;
                        D_801F4188.ages[i] = 0;
                    }
                }
                sprintf(path, "B:\\Card\\LC%3.3d.TIM", D_801F4188.ids[slot]);
                func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
                tim = (u32 *)func_80014C08(0x7FFFFFFF);
                uploadTim(tim, (slot / 4) * 32 + 0x140, (slot % 4) * 64 + 0x100, 0, slot + 0x1F4);
                freeHeapBlock(tim);
                D_801F4188.ages[slot] = 100;
                D_801F4188.busy = 0;
            }
        }
    } while (D_801F4188.running != 0);
    D_801F4188.busy = -1;
}

s32 func_801E8670(s16 id) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (id == D_801F4188.ids[i] && D_801F4188.ages[i] != 0) {
            return i;
        }
    }
    return -1;
}

void func_801E86D4(void) {
    char path[24];
    u32 *pack;

    sprintf(path, "C:\\OBJECT\\deck.TIS");
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    D_801F342C = (u8 *)func_80014C08(0x7FFFFFFF);
}

void func_801E8798(s8 keepBuffers) {
    s32 i;
    s16 *p;

    if (!keepBuffers) {
        for (i = 0; i < 2; i++) {
            DB(i).primSlots[0] = (s32)(D_801F41A8.primBuffers[i] = allocHeapBlock(0xAF0, 0x3C));
        }
        p = allocHeapBlock(0x320, 0x3C);
        D_801F41A8.unk8 = p;
    }
    p = D_801F41A8.unk8;
    for (i = 0; i < 100; i++) {
        p[1] = 0;
        p[0] = 0;
        p[3] = 0;
        p[2] = 0;
        p += 4;
    }
}

void func_801E8864(s16 cardId, s16 x, s16 y, u8 arg3, s32 otIndex) {
    s32 specialty = ((DigimonCardData *)DIGIMON_CARDS)[cardId].attr >> 4;
    s32 u;
    s32 v;
    s32 clutY;
    s16 page;
    s16 column;
    s16 row;
    s32 pageU;
    s32 columnU;

    if (cardId == 300) {
        u = 0x230;
        v = 0x1E0;
    } else {
        page = cardId / 50;
        pageU = page << 6;
        column = cardId % 5;
        columnU = column * 12 + 0x240;
        u = pageU + columnU;
        row = cardId % 50;
        row = row / 5;
        v = row * 24 + 0x100;
    }
    if (cardId >= 0x11D || cardId == 200) {
        clutY = 0x1FE;
    } else if (cardId < 0xBF) {
        clutY = specialty + 0x1F8;
    } else {
        clutY = 0x1FD;
    }
    func_801E89FC(x, y, getClut(0x240, clutY), u, v, 24, 24, 1, arg3, -1, otIndex);
}

void func_801E89FC(s16 x, s16 y, s16 clut, s32 u, s32 v, s16 w, s16 h, s8 tp, u8 brightness, s8 abr, s32 otIndex) {
    u16 tpage = ((tp & 3) << 7) | ((abr & 3) << 5) | ((v & 0x100) >> 4) | ((u & 0x3C0) >> 6) | ((v & 0x200) << 2);

    setlen(&D_801F44A4->sp, 4);
    setcode(&D_801F44A4->sp, 0x64);
    D_801F44A4->sp.clut = clut;
    D_801F44A4->sp.w = w;
    D_801F44A4->sp.h = h;
    D_801F44A4->sp.x0 = x;
    D_801F44A4->sp.y0 = y;
    if (tp != 0) {
        D_801F44A4->sp.u0 = (u % 64) << 1;
    } else {
        D_801F44A4->sp.u0 = (u % 64) << 2;
    }
    D_801F44A4->sp.v0 = v;
    D_801F44A4->sp.r0 = brightness;
    D_801F44A4->sp.g0 = brightness;
    D_801F44A4->sp.b0 = brightness;
    if (abr >= 0) {
        tpage |= (abr & 3) << 5;
        setSemiTrans(&D_801F44A4->sp, 1);
    } else {
        setSemiTrans(&D_801F44A4->sp, 0);
    }
    setDrawMode(&D_801F44A4->dm, 0, 0, tpage);
    addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &D_801F44A4->sp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &D_801F44A4->dm);
    D_801F44A4++;
}

void func_801E8C04(s32 player, s32 parentTask) {
    func_801E86D4();
    func_801E8798(0);
    D_801F41A8.running = 1;
    D_801F41A8.player = player;
    D_801F200C = D_801F41A8.player;
    D_801F2100 = D_801F41A8.player;
    D_801F21C4 = D_801F41A8.player;
    func_800149B8(0, -1, 0, 0x1000, D_801F41A8.task = func_801E9790, 0, 0, 0, 0);
    func_800149B8(0, -1, 0, 0x1000, func_801E83BC, 0, getCurrentTaskId(), 0, 0);
    do {
        func_80014C08(1);
        D_801F44A4 = D_801F41A8.primBuffers[FRAME_BUFFER_INDEX];
    } while (D_801F41A8.running == 1);
    D_801F4188.running = 0;
    do {
        func_80014C08(1);
    } while (D_801F4188.busy != -1);
    clearCollectionNewFlags(player);
    func_80014C08(2);
    freeHeapBlocksByTag(0x3C);
    freeHeapBlock(D_801F342C);
    func_80014A48(parentTask);
}

void func_801E8D98(UiWindow *window, Rect16 area, s32 label, s32 flags, s32 style) {
    Rect16 rect;
    s32 unused[2]; /* unused, but it is in the original stack frame */
    Rect16 view;

    rect.x = area.x - area.w / 2;
    rect.y = area.y - area.h / 2;
    rect.w = area.w & ~1;
    rect.h = area.h & ~1;
    view.x = 0;
    view.y = 0;
    view.w = (area.w + 10) & ~1;
    view.h = 0x2000;
    openWindow(window, &rect, -1, (s16 *)&view, flags, style, 0x80, 12);
    window->label = label;
}

void func_801E8E8C(void) {
    CardCount *fire = D_801F4330.lists[0];
    CardCount *ice = D_801F4330.lists[1];
    CardCount *nature = D_801F4330.lists[2];
    CardCount *darkness = D_801F4330.lists[3];
    CardCount *rare = D_801F4330.lists[4];
    CardCount *options1 = D_801F4330.lists[5];
    CardCount *options2 = D_801F4330.lists[6];
    CardCount *options3 = D_801F4330.lists[7];
    s32 i;

    for (i = 0; i < 0xBF; i++) {
        switch (((DigimonCardData *)DIGIMON_CARDS)[i].attr >> 4) {
        case 0:
            fire->id = i;
            fire->count = getOwnedCardCount(D_801F41A8.player, i);
            fire++;
            break;
        case 1:
            ice->id = i;
            ice->count = getOwnedCardCount(D_801F41A8.player, i);
            ice++;
            break;
        case 2:
            nature->id = i;
            nature->count = getOwnedCardCount(D_801F41A8.player, i);
            nature++;
            break;
        case 3:
            darkness->id = i;
            darkness->count = getOwnedCardCount(D_801F41A8.player, i);
            darkness++;
            break;
        case 4:
            rare->id = i;
            rare->count = getOwnedCardCount(D_801F41A8.player, i);
            rare++;
            break;
        }
    }
    for (i = 0; i < 0x66; i++) {
        if (i + 0xBF < 0xE4) {
            options1->id = getCardId(1, i);
            options1->count = getOwnedCardCount(D_801F41A8.player, options1->id);
            options1++;
        } else if (i + 0xBF < 0x109) {
            options2->id = getCardId(1, i);
            options2->count = getOwnedCardCount(D_801F41A8.player, options2->id);
            options2++;
        } else {
            options3->id = getCardId(1, i);
            options3->count = getOwnedCardCount(D_801F41A8.player, options3->id);
            options3++;
        }
    }
    for (i = 0; i < 8; i++) {
        options3->id = getCardId(2, i);
        options3->count = getOwnedCardCount(D_801F41A8.player, options3->id);
        options3++;
    }
    fire->id = -1;
    ice->id = -1;
    nature->id = -1;
    darkness->id = -1;
    rare->id = -1;
    options1->id = -1;
    options2->id = -1;
    options3->id = -1;
}

void func_801E913C(u8 player) {
    s32 i;
    s32 j;
    s32 id;

    for (i = 0; i < 301; i++) {
        D_801F4324->spare[i] = getOwnedCardCount(player, i);
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 301; j++) {
            D_801F4324->inDecks[i][j] = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).savedDecks[i].inUse != 0) {
            for (j = 0; j < 30; j++) {
                id = getCardId(PLAYER_DATA(player).savedDecks[i].cards[j].type, ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].index);
                D_801F4324->inDecks[i][id]++;
            }
        }
    }
    for (i = 0; i < 301; i++) {
        for (j = 1; j < 3; j++) {
            if (D_801F4324->inDecks[0][i] < D_801F4324->inDecks[j][i]) {
                D_801F4324->inDecks[0][i] = D_801F4324->inDecks[j][i];
            }
        }
    }
    for (i = 0; i < 301; i++) {
        D_801F4324->spare[i] -= D_801F4324->inDecks[0][i];
    }
}

void func_801E93B8(void) {
    s32 unused[6]; /* unused, but it is in the original stack frame */
    s32 i;
    s32 j;
    CardCount *entry;

    D_801F4330.page = 0;
    D_801F4330.unk16D = 0;
    for (i = 0; i < 8; i++) {
        D_801F4360[i] = 0;
    }
    D_801F4330.totalCount = 0;
    D_801F4330.uniqueCount = 0;
    for (i = 0; i < 301; i++) {
        D_801F4330.totalCount += getOwnedCardCount(D_801F41A8.player, i);
        if (PLAYER_DATA(D_801F41A8.player).cardCollection[i] & 0x40) {
            D_801F4330.uniqueCount++;
        }
        if (i >= 0xBF) {
            if (i >= 0x125) {
                D_801F4330.counts[6] += getOwnedCardCount(D_801F41A8.player, i);
            } else {
                D_801F4330.counts[5] += getOwnedCardCount(D_801F41A8.player, i);
            }
        }
    }
    func_801E8E8C();
    for (j = 0; j < 5; j++) {
        entry = D_801F4330.lists[j];
        for (i = 0; i < 41; i++, entry++) {
            if (entry->id >= 0) {
                D_801F4360[j] += entry->count;
            } else {
                break;
            }
        }
    }
    for (i = 0; i < 7; i++) {
        D_801F4330.counts[7] += D_801F4360[i];
    }
}

void func_801E95D0(void) {
    Rect16 uv;
    s16 y;

    drawWindow(&D_801F33E8, func_801EA8AC, 0);
    drawWindow(&D_801F3430, func_801E6DA8, 0);
    drawWindow(D_801F3250, func_801E9F68, 0);
    drawWindow(D_801F3294, func_801E6FC8, 0);
    drawWindow(&D_801F32D8, func_801EA3D4, 0);
    drawWindow(&D_801F331C, func_801EA478, 0);
    drawWindow(&D_801F3360, func_801EA5F0, 0);
    if (D_801F41A8.hidden == 0) {
        if (++D_801F41A8.slide > 20) {
            D_801F41A8.slide = 20;
        }
    } else {
        if (--D_801F41A8.slide < 0) {
            D_801F41A8.slide = 0;
        }
    }
    y = (D_801F41A8.slide * 8 - (20 - D_801F41A8.slide) * 33) / 20;
    uv.x = 0;
    uv.y = 0x59;
    uv.w = 0x80;
    uv.h = 0x20;
    drawTexturedSprite(6, y, &uv, 0x18, 0x7E21, 30, 0x80, -1);
}

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF318);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF320);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF32C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF334);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF33C);

void func_801E9790(void) {
    Rect16 rects[7];
    Rect16 infoRect;
    char *labels[7] = { D_801DF318, D_801DF320, D_801DF32C, D_801DF32C, D_801DF334, D_801DF32C, D_801DF33C };
    s32 running;
    s32 i;
    s32 action;
    s32 flags;
    s32 style;

    running = 1;
    D_801F41A8.useDeckCounts = 0;
    D_801F41A8.listShown = 0;
    func_801E6BE8();
    rects[0].x = 0xF6;
    rects[0].y = 0xBA;
    rects[0].w = 0x80;
    rects[0].h = 0x3C;
    rects[1].x = 0x41;
    rects[1].y = 0x60;
    rects[1].w = 0x122;
    rects[1].h = 0x60;
    rects[2].x = 0x5B;
    rects[2].y = 0x99;
    rects[2].w = 0xA2;
    rects[2].h = 0xC;
    rects[3].x = 0xE6;
    rects[3].y = 0x1B;
    rects[3].w = 0xA0;
    rects[3].h = 0xE;
    rects[4].x = 0x5A;
    rects[4].y = 0xC6;
    rects[4].w = 0xA0;
    rects[4].h = 0x34;
    rects[6].x = 0x9E;
    rects[6].y = 0x5E;
    rects[6].w = 0x122;
    rects[6].h = 0x56;
    D_801F1FE4.rect.h = 0x60;
    D_801F1FE4.ox = 0x85;
    openMenu(&D_801F1FE4, D_801F3294, &D_801F3200, (Bytes4 *)-1);
    D_801F32C0 = (s32)D_801DF320;
    D_801F1FE4.active = running;
    D_801F1FE4.row = 0;
    centerMenuOnCursor(&D_801F1FE4);
    openMenu(&D_801F20D8, &D_801F3430, &D_801F3480, (Bytes4 *)-1);
    animateWindowTo(&D_801F3430, (Rect16 *)-1);
    D_801F3430.label = (s32)"SORT MENU";
    for (i = 0; i < 7; i++) {
        if (i == 1 || i == 5) {
            continue;
        }
        switch (i) {
        case 0:
            flags = 8;
            style = 0x21;
            break;
        case 2:
        case 3:
            flags = 0;
            style = 0x31;
            break;
        case 4:
            flags = 8;
            style = 0x21;
            break;
        case 5:
            flags = 0;
            style = 0x41;
            break;
        case 6:
            flags = 8;
            style = 0x21;
            break;
        default:
            flags = 0;
            style = 0x21;
            break;
        }
        func_801E8D98(&D_801F3250[i], rects[i], (s32)labels[i], flags, style);
    }
    animateWindowTo(&D_801F33E8, (Rect16 *)-1);
    D_801F4350 = allocTaskHeapBlock(0x2A);
    for (i = 0; i < 8; i++) {
        D_801F4330.lists[i] = allocTaskHeapBlock(0xA4);
    }
    func_801E93B8();
    D_801F435E = 0;
    D_801F41A8.slide = 0;
    D_801F41A8.hidden = 0;
    playMenuSound(3);
    addFrameCallback((s32)func_801E95D0);
    do {
        func_80014C08(1);
        if (PAD_STATES[D_801F41A8.player]->pressed & 0x100) {
            action = 1;
        } else if (PAD_STATES[D_801F41A8.player]->pressed & 0x40) {
            action = 2;
        } else if (PAD_STATES[D_801F41A8.player]->pressed & 0x20) {
            action = 3;
        } else if (PAD_STATES[D_801F41A8.player]->pressed & 0x10) {
            action = 5;
        } else {
            action = 0;
        }
        if (action == 1) {
            if (D_801F4330.showInfo == 0) {
                D_801F41A8.listShown ^= 1;
                if (D_801F41A8.listShown != 0) {
                    D_801F1FE4.active = 0;
                    animateWindowTo(&D_801F3430, &D_801F20E0);
                    playMenuSound(3);
                } else {
                    D_801F1FE4.active = 1;
                    animateWindowTo(&D_801F3430, (Rect16 *)-1);
                    playMenuSound(4);
                }
            }
        } else if (action == 2) {
            if (D_801F41A8.listShown == 0 && (PLAYER_DATA(D_801F41A8.player).cardCollection[D_801F4330.selectedId] & 0x40)) {
                D_801F4330.showInfo ^= 1;
                if (D_801F4330.showInfo != 0) {
                    playMenuSound(4);
                    D_801F1FE4.active = 0;
                    infoRect.x = rects[6].x - rects[6].w / 2;
                    infoRect.y = rects[6].y - rects[6].h / 2;
                    infoRect.w = rects[6].w & ~1;
                    infoRect.h = rects[6].h & ~1;
                    animateWindowTo(&D_801F33E8, &infoRect);
                    animateWindowTo(D_801F3294, (Rect16 *)-1);
                } else {
                    playMenuSound(3);
                    D_801F1FE4.active = 1;
                    animateWindowTo(D_801F3294, &D_801F1FE4.rect);
                    animateWindowTo(&D_801F33E8, (Rect16 *)-1);
                }
            }
        } else if (action == 3 || action == 4) {
            if (D_801F41A8.listShown == 0) {
                playMenuSound(1);
                D_801F1FE4.active = 0;
                running = 0;
                for (i = 0; i < 7; i++) {
                    animateWindowTo(&D_801F3250[i], (Rect16 *)-1);
                }
            }
        } else if (action == 5) {
            if (D_801F41A8.listShown == 1) {
                D_801F41A8.listShown ^= 1;
                D_801F1FE4.active = 1;
                animateWindowTo(&D_801F3430, (Rect16 *)-1);
                playMenuSound(4);
            } else if (D_801F4330.showInfo == 0) {
                playMenuSound(3);
                for (i = 0; i < 7; i++) {
                    if (i != 5) {
                        animateWindowTo(&D_801F3250[i], (Rect16 *)-1);
                    }
                }
                running = 0;
            } else {
                D_801F4330.showInfo = 0;
                playMenuSound(3);
                D_801F1FE4.active = 1;
                animateWindowTo(D_801F3294, &D_801F1FE4.rect);
                animateWindowTo(&D_801F33E8, (Rect16 *)-1);
            }
        }
    } while (running);
    D_801F41BC = -1;
    func_80014C08(20);
    removeFrameCallback((s32)func_801E95D0);
    if (action == 3) {
        func_800149B8(0, -1, 0, 0x1000, func_801EBCC0, 0, 0, 0, 0);
    } else if (action == 4) {
        func_800149B8(0, -1, 0, 0x1000, func_801E4B34, D_801F41BA, 0, 0, 0);
    } else if (action == 5) {
        D_801F41BE = 0;
    }
    func_80014A90();
}

void func_801E9F68(UiWindow *window) {
    char buf[72];
    u8 palettes[8] = { 2, 1, 4, 9, 6, 8, 8, 8 };
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    CardCount *entry;
    s32 i;

    if (PAD_STATES[D_801F41BA]->pressed & 4) {
        D_801F4330.page--;
    } else if (PAD_STATES[D_801F41BA]->pressed & 8) {
        D_801F4330.page++;
    }
    if (D_801F4330.page >= 8) {
        D_801F4330.page = 0;
    } else if (D_801F4330.page < 0) {
        D_801F4330.page = 7;
    }
    entry = D_801F4330.lists[D_801F4330.page];
    window->palette = palettes[D_801F4330.page];
    switch (D_801F4330.page) {
    case 0:
        sprintf(D_801F42E0, "L1_BACK   FIRE    NEXT_R1");
        break;
    case 1:
        sprintf(D_801F42E0, "L1_BACK    ICE    NEXT_R1");
        break;
    case 2:
        sprintf(D_801F42E0, "L1_BACK  NATURE   NEXT_R1");
        break;
    case 3:
        sprintf(D_801F42E0, "L1_BACK DARKNESS  NEXT_R1");
        break;
    case 4:
        sprintf(D_801F42E0, "L1_BACK   RARE    NEXT_R1");
        break;
    case 5:
        sprintf(D_801F42E0, "L1_BACK OPTION1   NEXT_R1");
        break;
    case 6:
        sprintf(D_801F42E0, "L1_BACK OPTION2   NEXT_R1");
        break;
    case 7:
        sprintf(D_801F42E0, "L1_BACK OPTION3   NEXT_R1");
        break;
    default:
        sprintf(D_801F42E0, "L1_BACK DARKNESS  NEXT_R1");
        break;
    }
    window->label = (s32)D_801F42E0;
    for (i = 0; i < 40; i++) {
        if (entry->id < 0) {
            func_801E89FC(x + (i % 8) * 16, y + 1 + (i / 8) * 12, 0x7E35, 0x344, 0x1F0, 15, 11, 0, 0x80, -1, z);
        } else {
            sprintf(buf, "%3.3d", entry->id);
            drawTinyText(x + 2 + (i % 8) * 16, y + 3 + (i / 8) * 12, (s32)buf, 8, z);
            func_801E89FC(x + (i % 8) * 16, y + (i / 8) * 12, getClut(0x350, entry->count + 0x1F8), 0x340, 0x1F0, 15, 11, 0, 0x80, -1, z);
            entry++;
        }
    }
}

void func_801EA3D4(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    drawText(x, y, (s32)"*b0:Edit Decks", 7, z);
    if (D_801F435E == 0) {
        drawText(x + 0x66, y, (s32)"*b5:Sort", 7, z);
    } else {
        drawText(x + 0x66, y, (s32)"*b5:Sort", 8, z);
    }
}

void func_801EA478(UiWindow *window) {
    char buf[72];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    y += 2;
    if (D_801F4330.uniqueCount >= 301) {
        drawIcon(x, y, 0, 20, z);
    }
    x += 12;
    drawTinyText(x, y, (s32)"All", 6, z);
    drawTinyText(x, y + 6, (s32)"Types", 6, z);
    sprintf(buf, "%3d", D_801F4330.uniqueCount);
    drawText(x + 26, y, (s32)buf, 7, z);
    x += 80;
    if (D_801F4330.totalCount >= 1608) {
        drawIcon(x - 24, y, 0, 20, z);
    }
    drawTinyText(x - 12, y, (s32)"Total Number", 6, z);
    drawTinyText(x - 12, y + 6, (s32)"of Cards", 6, z);
    sprintf(buf, "%4d", D_801F4330.totalCount);
    drawText(x + 38, y, (s32)buf, 7, z);
}

void func_801EA5F0(UiWindow *window) {
    char buf[144];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 i;

    window->label = (s32)D_801DF4F0;
    for (i = 0; i < 8; i++) {
        if (i != 7) {
            drawIcon(x + (i / 4) * 80, y + (i % 4) * 13, 0, i, z);
            sprintf(buf, D_801DF4FC, D_801F4360[i]);
            drawText(x + 0x10 + (i / 4) * 95, y + (i % 4) * 13, (s32)buf, 7, z);
            drawText(x + 0x25 + (i / 4) * 95, y + (i % 4) * 13, (s32)D_801DF298, 7, z);
        } else {
            drawText(x + (i / 4) * 80 - 6, y + 0x27, (s32)D_801DF504, 7, z);
            sprintf(buf, D_801DF50C, D_801F4360[i]);
            drawText(x + 0x19 + (i / 4) * 80, y + 0x27, (s32)buf, 7, z);
            drawText(x + 0x34 + (i / 4) * 80, y + (i % 4) * 13, (s32)D_801DF298, 7, z);
        }
    }
}

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF4F0);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF4FC);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF504);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF50C);

SortCompare D_801F21C8[18] = {
    (SortCompare)func_801E75A0,
    (SortCompare)func_801E7600,
    (SortCompare)func_801E7690,
    (SortCompare)func_801E7724,
    (SortCompare)func_801E77B8,
    (SortCompare)func_801E784C,
    (SortCompare)func_801E78E0,
    (SortCompare)func_801E792C,
    (SortCompare)func_801E7978,
    (SortCompare)func_801E7A08,
    (SortCompare)func_801E7A9C,
    (SortCompare)func_801E7C44,
    (SortCompare)func_801E7CAC,
    (SortCompare)func_801E7D14,
    (SortCompare)func_801E7D7C,
    (SortCompare)func_801E7DE4,
    (SortCompare)func_801E7E4C,
    (SortCompare)func_801E7EB4,
};

/* the specialties' names in Japanese; not referenced by any code */
char *D_801F2210[5] = {
    "\x89\xCE\x89\x8A", /* 火炎 */
    "\x95X\x90\x85", /* 氷水 */
    "\x8E\xA9\x91R", /* 自然 */
    "\x88\xC3\x8D\x95", /* 暗黒 */
    "\x92\xBF\x8E\xED", /* 珍種 */
};

MenuItem D_801F2224[13] = {
    { 84, 36, { 11, 3, 1, 1 } },
    { 156, 36, { 12, 5, 0, 0 } },
    { 54, 18, { 0, 7, 6, 3 } },
    { 84, 12, { 0, 7, 2, 4 } },
    { 108, 36, { 0, 7, 3, 5 } },
    { 156, 48, { 1, 8, 4, 6 } },
    { 216, 24, { 1, 8, 5, 2 } },
    { 96, 42, { 3, 9, 8, 8 } },
    { 168, 42, { 5, 10, 7, 7 } },
    { 96, 24, { 7, 11, 10, 10 } },
    { 168, 18, { 8, 12, 9, 9 } },
    { 60, 36, { 9, 0, 12, 12 } },
    { 120, 36, { 10, 1, 11, 11 } },
};

/* the level of each option card */
u8 D_801F228C[108] = {
    1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
    0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0,
};

/* the u8 arrays among these are not referenced by any code */
UiWindow D_801F22F8 = { 0 };
u8 D_801F233C[12] = { 0 };
CursorHighlight D_801F2348 = { { { 0 } } };
UiWindow D_801F2398 = { 0 };
u8 D_801F23DC[12] = { 0 };
CursorHighlight D_801F23E8 = { { { 0 } } };
UiWindow D_801F2438 = { 0 };
u8 D_801F247C[12] = { 0 };
NameEntry D_801F2488 = { 0 };
u8 D_801F24AC[4] = { 0 };
UiWindow D_801F24B0 = { 0 };
u8 D_801F24F4[12] = { 0 };
CursorHighlight D_801F2500 = { { { 0 } } };
u16 D_801F2550[0x9F] = { 0 };
UiWindow D_801F2690 = { 0 };
u8 D_801F26D4[12] = { 0 };
CursorHighlight D_801F26E0 = { { { 0 } } };
UiWindow D_801F2730 = { 0 };
u8 D_801F2774[12] = { 0 };
CursorHighlight D_801F2780 = { { { 0 } } };
UiWindow D_801F27D0 = { 0 };
u8 D_801F2814[12] = { 0 };
UiWindow D_801F2820 = { 0 };
u8 D_801F2864[12] = { 0 };
TabWindow D_801F2870[3] = { { { 0 } } };
u8 D_801F2948[8] = { 0 };
UiWindow D_801F2950 = { 0 };
s32 D_801F2994 = 0;
s32 D_801F2998 = 0;
s32 D_801F299C = 0;
Partner D_801F29A0 = { { { 0 } } };
u8 D_801F2C38[8] = { 0 };
Partner D_801F2C40 = { { { 0 } } };
u8 D_801F2ED8[8] = { 0 };
Partner D_801F2EE0 = { { { 0 } } };
s32 D_801F3178 = 0;
s32 D_801F317C = 0;
s8 D_801F3180[128] = { 0 };
CursorHighlight D_801F3200 = { { { 0 } } };
UiWindow D_801F3250[7] = { { 0 } };
u8 *D_801F342C = NULL;
UiWindow D_801F3430 = { 0 };
u8 D_801F3474[12] = { 0 };
CursorHighlight D_801F3480 = { { { 0 } } };
void *D_801F34D0[301] = { 0 };
u8 D_801F3984[12] = { 0 };
s8 *D_801F3990[301] = { 0 };
u8 D_801F3E44[12] = { 0 };
UiWindow D_801F3E50 = { 0 };
u8 D_801F3E94[12] = { 0 };
CursorHighlight D_801F3EA0 = { { { 0 } } };
CursorHighlight D_801F3EF0 = { { { 0 } } };
s8 *D_801F3F40 = NULL;
u8 D_801F3F44[4] = { 0 };
DeckEditState D_801F3F48 = { 0 };
s8 D_801F4058 = 0;
u8 D_801F405C[4] = { 0 };
Unk801F4060 D_801F4060 = { { 0 } };
u8 D_801F4070[6] = { 0 };
u8 D_801F4078[0x110] = { 0 };
CardImageCache D_801F4188 = { 0 };
u8 D_801F41A4[4] = { 0 };
Unk801F41A8 D_801F41A8 = { { 0 } };
u8 D_801F41C4[4] = { 0 };
PlayerDeck D_801F41C8 = { 0 };
CardIdList *D_801F42D8 = NULL;
u8 D_801F42DC[4] = { 0 };
char D_801F42E0[0x44] = { 0 };
DeckCardCounts *D_801F4324 = NULL;
PlayerDeck *D_801F4328 = NULL;
u8 D_801F432C[4] = { 0 };
CollectionStats D_801F4330 = { { 0 } };
SprtPacket *D_801F44A4 = NULL;

void func_801EA8AC(UiWindow *window) {
    char buf[72];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 slot;
    s32 i;

    if (PLAYER_DATA(D_801F41A8.player).cardCollection[D_801F4330.selectedId] & 0x40) {
        D_801F4188.request = D_801F4330.selectedId;
        slot = func_801E8670(D_801F4188.request);
        if (slot != -1) {
            func_801E89FC(x + 3, y + 14, getClut(0, slot + 0x1F4), (slot / 4) * 32 + 0x140, (slot % 4) * 64 + 0x100, 0x40, 0x40, 1, 0x80, -1, z);
            sprintf(buf, "%3.3d", *(s16 *)D_801F4330.selectedCard);
            drawText(x + 3, y, (s32)buf, 7, z);
            drawText(x + 0x7B, y, (s32)(D_801F4330.selectedCard + 3), 7, z);
            switch (D_801F4330.selectedCard[2]) {
            case 0:
                drawIcon(x + 0x5F, y, 0, ((DigimonCardData *)D_801F4330.selectedCard)->attr >> 4, z);
                drawIcon(x + 0x29, y, 0, (((DigimonCardData *)D_801F4330.selectedCard)->attr & 0xF) + 0x10, z);
                drawText(x + 0x1B, y, (s32)D_801DF27C, 7, z);
                drawText(x + 0x41, y, (s32)D_801DF280, 7, z);
                sprintf(buf, D_801DF294, getOwnedCardCount(D_801F41A8.player, *(s16 *)D_801F4330.selectedCard));
                drawText(x + 0xFA, y, (s32)buf, 7, z);
                drawText(x + 0x106, y, (s32)D_801DF298, 7, z);
                drawIcon(x + 0x4B, y + 12, 0, 0x1A, z);
                sprintf(buf, D_801DF50C, ((DigimonCardData *)D_801F4330.selectedCard)->hp);
                drawText(x + 0x69, y + 12, (s32)buf, 7, z);
                for (i = 0; i < 3; i++) {
                    sprintf(buf, "b%d", i);
                    func_80029EC4(x + 0x4B, y + (i + 3) * 12, 7, 1, z, (s32)buf);
                    sprintf(buf, "*s0%4d/%4d", (u16)PLAYER_DATA(D_801F41A8.player).unkD3C[*(s16 *)D_801F4330.selectedCard][i], ((DigimonCardData *)D_801F4330.selectedCard)->attack[i].power);
                    drawText(x + 0x6F, y + (i + 3) * 12, (s32)buf, 7, z);
                }
                drawSmallText(x + 0x57, y + 0x48, (s32)CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)D_801F4330.selectedCard)->crossEffect], 7, z);
                if (D_8006E4FC[((DigimonCardData *)D_801F4330.selectedCard)->crossEffect] != 0) {
                    drawIcon(x + 0x91, y + 0x4E, 0, D_8006E4FC[((DigimonCardData *)D_801F4330.selectedCard)->crossEffect] + 0x14, z);
                }
                drawIcon(x + 0x4B, y + 0x18, 0, 0x18, z);
                sprintf(buf, D_801DF54C, ((DigimonCardData *)D_801F4330.selectedCard)->dpCost);
                drawText(x + 0x69, y + 0x18, (s32)buf, 7, z);
                drawIcon(x + 0x81, y + 0x18, 0, 0x19, z);
                sprintf(buf, D_801DF54C, ((DigimonCardData *)D_801F4330.selectedCard)->dpBonus);
                drawText(x + 0x93, y + 0x18, (s32)buf, 7, z);
                sprintf(buf, D_801DF4FC, PLAYER_DATA(D_801F41A8.player).unk11B6[*(s16 *)D_801F4330.selectedCard]);
                drawText(x + 0xBD, y + 12, (s32)buf, 7, z);
                drawText(x + 0xD5, y + 12, (s32)D_801DF554, 6, z);
                sprintf(buf, D_801DF4FC, PLAYER_DATA(D_801F41A8.player).unk1334[*(s16 *)D_801F4330.selectedCard]);
                drawText(x + 0xEE, y + 12, (s32)buf, 7, z);
                drawText(x + 0x106, y + 12, (s32)D_801DF55C, 6, z);
                drawText(x + 0xB9, y + 0x18, (s32)D_801DF564, 6, z);
                if (((DigimonCardData *)D_801F4330.selectedCard)->supportIcon != 0) {
                    drawIcon(x + 0x10C, y + 0x18, 0, ((DigimonCardData *)D_801F4330.selectedCard)->supportIcon + 0x14, z);
                }
                for (i = 0; i < 4; i++) {
                    drawText(x + 0xB9, y + (i + 3) * 12, (s32)((DigimonCardData *)D_801F4330.selectedCard)->supportText[i], 7, z);
                }
                break;
            case 1:
                drawIcon(x + 0x18, y, 0, 5, z);
                sprintf(buf, D_801DF294, getOwnedCardCount(D_801F41A8.player, *(s16 *)D_801F4330.selectedCard));
                drawText(x + 0xFA, y, (s32)buf, 7, z);
                drawText(x + 0x106, y, (s32)D_801DF298, 7, z);
                if (D_801F4330.selectedCard[0x8C] != 0) {
                    drawIcon(x + 0x8D, y + 13, 0, D_801F4330.selectedCard[0x8C] + 0x14, z);
                }
                y += 0x1A;
                for (i = 0; i < 4; i++) {
                    drawText(x + 0x8D, y, (s32)((u8 *)&((OptionCardData *)OPTION_CARDS)[*(s16 *)D_801F4330.selectedCard - 0xBF] + 0x8D + i * 21), 7, z);
                    y += 12;
                }
                break;
            case 2:
                drawIcon(x + 0x18, y, 0, 6, z);
                sprintf(buf, D_801DF294, getOwnedCardCount(D_801F41A8.player, *(s16 *)D_801F44A0));
                drawText(x + 0xFA, y, (s32)buf, 7, z);
                drawText(x + 0x106, y, (s32)D_801DF298, 7, z);
                y += 0x1A;
                for (i = 0; i < 4; i++) {
                    drawText(x + 0x8D, y, (s32)((u8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[*(s16 *)D_801F4330.selectedCard - 0x125] + 0x1B + i * 21), 7, z);
                    y += 12;
                }
                break;
            }
        }
    }
}

void func_801EB1FC(void) {
    s32 i;

    D_801F4060.count = 0;
    for (i = 0; i < 3; i++) {
        D_801F4060.decks[i] = &PLAYER_DATA(D_801F41A8.player).savedDecks[i];
        if (D_801F4060.decks[i]->inUse == 1) {
            D_801F4060.count++;
        } else {
            D_801F4060.decks[i]->inUse = 0;
        }
    }
}

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF54C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF554);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF55C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF564);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF574);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF578);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF57C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF580);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF584);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF588);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF594);

void func_801EB2BC(UiWindow *window) {
    char buf[72];
    char *labels[7] = { D_801DF574, D_801DF578, D_801DF57C, D_801DF580, D_801DF584, D_801DF588, D_801DF594 };
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 palette;
    s32 labelPalette;
    PlayerDeck *deck;
    u32 battles;
    s32 i;
    s32 dx;
    s32 dy;

    if (D_801F4060.current == D_801F4060.slot) {
        window->palette = 6;
        palette = 7;
        labelPalette = 6;
    } else {
        window->palette = 1;
        labelPalette = 8;
        palette = 8;
    }
    deck = &PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.current];
    sprintf(buf, "%s Deck", deck->unk1);
    drawText(x, y, (s32)buf, palette, z);
    battles = deck->unk108[1] + deck->unk108[2];
    if (battles >= 1000) {
        battles = 999;
    }
    sprintf(buf, D_801DF4FC, battles);
    drawText(x + 0x62, y, (s32)buf, palette, z);
    drawTinyText(x + 0x76, y + 7, (s32)"Battles", palette, z);
    sprintf(buf, D_801DF4FC, deck->unk108[1]);
    drawText(x + 0x95, y, (s32)buf, palette, z);
    drawTinyText(x + 0xA9, y + 7, (s32)D_801DF554, palette, z);
    sprintf(buf, D_801DF4FC, deck->unk108[2]);
    drawText(x + 0xBD, y, (s32)buf, palette, z);
    drawTinyText(x + 0xD1, y + 7, (s32)D_801DF55C, palette, z);
    y += 13;
    for (i = 0; i < 6; i++) {
        dy = 0;
        if (i >= 4) {
            dy = 13;
        }
        dx = (i % 4) * 59;
        drawText(x + dx, y + dy, (s32)labels[i], labelPalette, z);
        if (i == 5) {
            sprintf(buf, D_801DF54C, countDeckCardsByFilter(D_801F41A8.player, deck, 0xC0));
        } else {
            sprintf(buf, D_801DF54C, countDeckCardsByFilter(D_801F41A8.player, deck, 1 << i));
        }
        if (i == 5) {
            drawText(x + 0x3A + dx, y + dy, (s32)buf, palette, z);
            drawSmallText(x + 0x4A + dx, y + dy + 7, (s32)D_801DF298, palette, z);
        } else {
            drawText(dx + x + 0x10, y + dy, (s32)buf, palette, z);
            drawSmallText(x + 0x10 + dx + 0x10, y + dy + 7, (s32)D_801DF298, palette, z);
        }
    }
    drawText(x + 0xA0, y + dy, (s32)labels[6], labelPalette, z);
    sprintf(buf, D_801DF54C, countDeckCardsByFilter(D_801F41A8.player, deck, 0x20));
    drawText(x + 0xC1, y + dy, (s32)buf, palette, z);
    drawSmallText(x + 0xD1, y + dy + 7, (s32)D_801DF298, palette, z);
    drawText(x, y + 26, (s32)D_801DF27C, labelPalette, z);
    sprintf(buf, D_801DF54C, countDeckCardsByFilter(D_801F41A8.player, deck, 0x200));
    drawIcon(x + 0xC, y + 26, 0, 0x10, z);
    drawText(x + 0x1E, y + 26, (s32)buf, palette, z);
    drawSmallText(x + 0x2E, y + 26 + 7, (s32)D_801DF298, palette, z);
    x += 0x51;
    drawText(x, y + 26, (s32)D_801DF27C, labelPalette, z);
    sprintf(buf, D_801DF54C, countDeckCardsByFilter(D_801F41A8.player, deck, 0x800));
    drawIcon(x + 0xC, y + 26, 0, 0x12, z);
    drawText(x + 0x1E, y + 26, (s32)buf, palette, z);
    drawSmallText(x + 0x2E, y + 26 + 7, (s32)D_801DF298, palette, z);
    x += 0x52;
    drawText(x, y + 26, (s32)D_801DF27C, labelPalette, z);
    sprintf(buf, D_801DF54C, countDeckCardsByFilter(D_801F41A8.player, deck, 0x1000));
    drawIcon(x + 0xC, y + 26, 0, 0x13, z);
    drawText(x + 0x1E, y + 26, (s32)buf, palette, z);
    drawSmallText(x + 0x2E, y + 26 + 7, (s32)D_801DF298, palette, z);
}

void func_801EB8BC(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 palette;

    if (D_801F4060.current == D_801F4060.slot) {
        window->palette = 6;
        palette = 7;
    } else {
        window->palette = 1;
        palette = 8;
    }
    drawText(x + 0x50, y + 0x12, (s32)"NO DATA", palette, z);
}

void func_801EB92C(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    drawIcon(x, y, 1, 11, z);
    drawMediumText(x + 8, y, (s32)":Cursor", 7, z);
    y += 10;
    drawIcon(x, y, 1, 9, z);
    drawMediumText(x + 8, y, (s32)":Edit", 7, z);
    y += 10;
    drawIcon(x, y, 1, 7, z);
    drawMediumText(x + 8, y, (s32)":Delete", 7, z);
    y += 10;
    drawIcon(x, y, 1, 10, z);
    drawMediumText(x + 8, y, (s32)":Copy", 7, z);
    y += 10;
    drawIcon(x, y, 1, 13, z);
    drawMediumText(x + 8, y, (s32)":Name", 7, z);
    y += 10;
    drawIcon(x, y, 1, 8, z);
    drawMediumText(x + 8, y, (s32)":Back", 7, z);
}

void func_801EBAC0(void) {
    Rect16 uv;
    s16 y;
    s32 i;

    drawWindow(D_801F3250, func_801EB92C, 30);
    for (i = 0; i < 3; i++) {
        D_801F4060.current = i;
        if (PLAYER_DATA(D_801F41A8.player).savedDecks[i].inUse != 0) {
            drawWindow(&D_801F3294[i], func_801EB2BC, 30);
        } else {
            drawWindow(&D_801F3294[i], func_801EB8BC, 30);
        }
    }
    if (D_801F41A8.hidden == 0) {
        if (++D_801F41A8.slide > 20) {
            D_801F41A8.slide = 20;
        }
    } else {
        if (--D_801F41A8.slide < 0) {
            D_801F41A8.slide = 0;
        }
    }
    y = (D_801F41A8.slide * 8 - (20 - D_801F41A8.slide) * 33) / 20;
    uv.x = 0;
    uv.y = 0x79;
    uv.w = 0x80;
    uv.h = 0x20;
    drawTexturedSprite(6, y, &uv, 0x18, 0x7E21, 30, 0x80, -1);
}

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF600);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF608);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF610);

void func_801EBCC0(void) {
    u8 dialog[0xB8];
    Rect16 rects[4];
    char *labels[4] = { D_801DF32C, D_801DF600, D_801DF608, D_801DF610 };
    s32 running = 1;
    s32 created = 0;
    s32 action;
    s32 i;
    s32 flags;
    s8 result;
    s32 style;

    func_801EB1FC();
    rects[0].x = 0x28;
    rects[0].y = 0x4E;
    rects[0].w = 0x3C;
    rects[0].h = 0x3A;
    rects[1].x = 0xC4;
    rects[1].y = 0x4A;
    rects[1].w = 0xE8;
    rects[1].h = 0x32;
    rects[2].x = 0xC4;
    rects[2].y = 0x8B;
    rects[2].w = 0xE8;
    rects[2].h = 0x32;
    rects[3].x = 0xC4;
    rects[3].y = 0xCC;
    rects[3].w = 0xE8;
    rects[3].h = 0x32;
    for (i = 0; i < 4; i++) {
        flags = 8;
        style = 0x21;
        func_801E8D98(&D_801F3250[i], rects[i], (s32)labels[i], flags, style);
    }
    D_801F41A8.slide = 0;
    D_801F41A8.hidden = 0;
    playMenuSound(3);
    addFrameCallback((s32)func_801EBAC0);
    do {
        func_80014C08(1);
        if (PAD_STATES[D_801F41BA]->pressed & 0x10) {
            action = 1;
        } else if (PAD_STATES[D_801F41BA]->pressed & 0x40) {
            action = 2;
        } else if (PAD_STATES[D_801F41BA]->pressed & 0x800) {
            action = 3;
        } else if ((u16)PAD_STATES[D_801F41BA]->repeat & 0x1000) {
            if (D_801F406E == 1) {
                PAD_STATES[D_801F41BA]->repeatEnabled = 0;
            }
            action = 4;
        } else if ((u16)PAD_STATES[D_801F41BA]->repeat & 0x4000) {
            if (D_801F406E == 1) {
                PAD_STATES[D_801F41BA]->repeatEnabled = 0;
            }
            action = 5;
        } else if ((u16)PAD_STATES[D_801F41BA]->repeat & 0x2000) {
            action = 6;
        } else if ((u16)PAD_STATES[D_801F41BA]->repeat & 0x8000) {
            action = 7;
        } else if (PAD_STATES[D_801F41BA]->pressed & 0x20) {
            action = 8;
        } else if (PAD_STATES[D_801F41BA]->pressed & 0x80) {
            action = 9;
        } else {
            action = 0;
        }
        switch (action) {
        case 2:
            if (PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot].inUse == 0) {
                playMenuSound(1);
                initDialog(dialog, (u8 *)"Do you want to create a new Deck?", 1);
                dialog[0xA6] = D_801F41A8.player;
                runDialog(dialog);
                result = dialog[0xA5];
                if (result != 1) {
                    break;
                }
                PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot].unk108[1] = PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot].unk108[2] = 0;
                D_801F4058 = result;
                D_801F4071 = 0;
                created = 1;
                D_801F4060.slot = D_801F4060.count;
            } else {
                D_801F4058 = 0;
            }
            /* fallthrough */
        case 1:
        case 3:
            if (action == 3) {
                if (PLAYER_DATA(D_801F41BA).savedDecks[D_801F406E].inUse == 0) {
                    break;
                }
                playMenuSound(1);
            } else {
                playMenuSound(4);
            }
            for (i = 0; i < 4; i++) {
                animateWindowTo(&D_801F3250[i], (Rect16 *)-1);
            }
            running = 0;
            break;
        case 4:
            playMenuSound(2);
            if (--D_801F4060.slot < 0) {
                D_801F4060.slot = 2;
            }
            break;
        case 5:
            playMenuSound(2);
            if (++D_801F4060.slot >= 3) {
                D_801F4060.slot = 0;
            }
            break;
        case 8:
            if (D_801F4060.decks[D_801F4060.slot]->inUse != 0 && D_801F4060.count >= 2) {
                playMenuSound(1);
                initDialog(dialog, (u8 *)"Do you want to delete this Deck?", 1);
                dialog[0xA6] = D_801F41A8.player;
                runDialog(dialog);
                if ((s8)dialog[0xA5] == 1) {
                    deleteSavedDeck(D_801F41A8.player, D_801F4060.slot);
                    D_801F4060.count--;
                    for (i = 0; i < 3; i++) {
                        if (PLAYER_DATA(D_801F41A8.player).savedDecks[i].inUse == 0) {
                            PLAYER_DATA(D_801F41A8.player).savedDecks[i].unk108[1] = PLAYER_DATA(D_801F41A8.player).savedDecks[i].unk108[2] = 0;
                        }
                    }
                }
            }
            break;
        case 9:
            if (D_801F4060.decks[D_801F4060.slot]->inUse != 0 && D_801F4060.count < 3) {
                playMenuSound(1);
                initDialog(dialog, (u8 *)"Do you want to copy this Deck?", 1);
                dialog[0xA6] = D_801F41A8.player;
                runDialog(dialog);
                if ((s8)dialog[0xA5] == 1) {
                    storeSavedDeck(D_801F41A8.player, &PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot], D_801F4060.count);
                    D_801F4060.count++;
                }
            }
            break;
        }
    } while (running);
    D_801F41BC = -1;
    func_80014C08(20);
    removeFrameCallback((s32)func_801EBAC0);
    func_80014C08(1);
    switch (action) {
    case 1:
        func_800149B8(0, -1, 0, 0x1000, D_801F41A8.task, 0, 0, 0, 0);
        break;
    case 2:
        if (created == 1) {
            sprintf(PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot].unk1, "NEW ");
            func_800149B8(0, -1, 0, 0x1000, func_801E0C08, 0, PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot].unk1, D_801F41A8.player, 0);
        } else {
            func_800149B8(0, -1, 0, 0x1000, func_801EDD60, &PLAYER_DATA(D_801F41BA).savedDecks[D_801F406E], 0, 0, 0);
        }
        break;
    case 3:
        func_800149B8(0, -1, 0, 0x1000, func_801E0C08, 1, PLAYER_DATA(D_801F41BA).savedDecks[D_801F406E].unk1, D_801F41BA, 0);
        break;
    }
}

void func_801EC75C(s16 index) {
    u8 unused[0x40];

    uploadTim((u32 *)(D_801F342C + ((s32 *)D_801F342C)[index + 1]), 0x220, 0x100, 0x240, 0x1F5);
}

void func_801EC7A8(void) {
    Rect16 uv;
    s16 y;

    drawWindow(&D_801F3430, func_801E6DA8, 29);
    drawWindow(&D_801F3E50, func_801E8110, 29);
    drawWindow(&D_801F32D8, func_801EE8CC, 30);
    drawWindow(D_801F3250, func_801EEE40, 30);
    drawWindow(D_801F3294, func_801EF740, 30);
    drawWindow(&D_801F331C, func_801EF7EC, 30);
    drawWindow(&D_801F3360, func_801EFC78, 30);
    drawWindow(&D_801F33A4, func_801EDF6C, 30);
    drawWindow(&D_801F33E8, func_801EDEFC, 30);
    if (D_801F41A8.hidden == 0) {
        if (++D_801F41A8.slide > 20) {
            D_801F41A8.slide = 20;
        }
    } else {
        if (--D_801F41A8.slide < 0) {
            D_801F41A8.slide = 0;
        }
    }
    y = (D_801F41A8.slide * 8 - (20 - D_801F41A8.slide) * 33) / 20;
    uv.x = 0;
    uv.y = 0x99;
    uv.w = 0x80;
    uv.h = 0x20;
    drawTexturedSprite(6, y, &uv, 0x18, 0x7E21, 30, 0x80, -1);
}

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF6C0);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF6C4);

void func_801EC998(PlayerDeck *deck) {
    Rect16 rects[7];
    char *labels[7] = { D_801DF4F0, D_801DF600, D_801DF320, D_801DF6C0, D_801DF6C4, D_801DF4F0, D_801DF32C };
    s32 i;
    s32 j;
    LINE_G3 *line;
    s32 id;
    s32 flags;
    s32 style;

    D_801F3F48.deckCounts = allocTaskHeapBlock(301);
    D_801F4328 = allocTaskHeapBlock(0x110);
    *D_801F4328 = *deck;
    D_801F3F48.statsPage = 0;
    D_801F3F48.slot = 0;
    D_801F3F48.unk10F = 0;
    for (i = 0; i < 301; i++) {
        D_801F3F48.deckCounts[i] = getOwnedCardCount(D_801F41A8.player, i);
    }
    if (deck->inUse == 0) {
        deck->inUse = 1;
        D_801F4328->inUse = 1;
        for (i = 0; i < 30; i++) {
            D_801F4328->cards[i].type = 0xFF;
        }
    } else {
        for (i = 0; i < 30; i++) {
            id = getCardId(D_801F4328->cards[i].type, D_801F4328->cards[i].index);
            if (D_801F3F48.deckCounts[id] != 0) {
                D_801F3F48.deckCounts[id]--;
            }
        }
    }
    for (j = 0; j < 2; j++) {
        line = D_801F3F48.cursorLines[j];
        for (i = 0; i < 4; i++) {
            SetLineG3(line);
            line->r0 = 0x80;
            line->g0 = 0x80;
            line->b0 = 0x80;
            line->r1 = 0x80;
            line->g1 = 0x80;
            line->b1 = 0x80;
            line->r2 = 0x80;
            line->g2 = 0x80;
            line->b2 = 0x80;
            line++;
        }
    }
    D_801F41C1 = 1;
    func_801E6BE8();
    D_801F4056 = 1;
    D_801F1FE4.rect.h = 0x60;
    D_801F1FE4.ox = 0x62;
    openMenu(&D_801F1FE4, &D_801F32D8, &D_801F3200, (Bytes4 *)-1);
    D_801F3304 = (s32)D_801DF320;
    openMenu(&D_801F20D8, &D_801F3430, &D_801F3480, (Bytes4 *)-1);
    animateWindowTo(&D_801F3430, (Rect16 *)-1);
    D_801F3430.label = (s32)"SORT MENU";
    D_801F1FE4.row = 0;
    centerMenuOnCursor(&D_801F1FE4);
    openMenu(&D_801F219C, &D_801F3E50, &D_801F3EA0, (Bytes4 *)-1);
    animateWindowTo(&D_801F3E50, (Rect16 *)-1);
    D_801F3E50.label = (s32)"SORT MENU";
    rects[0].x = 0xA3;
    rects[0].y = 0xBB;
    rects[0].w = 0x128;
    rects[0].h = 0x4E;
    rects[1].x = 0xE6;
    rects[1].y = 0x1B;
    rects[1].w = 0xA0;
    rects[1].h = 0xE;
    rects[2].x = 0xD8;
    rects[2].y = 0x5C;
    rects[2].w = 0xD7;
    rects[2].h = 0x60;
    rects[3].x = 0x23;
    rects[3].y = 0xAD;
    rects[3].w = 0x36;
    rects[3].h = 0x66;
    rects[4].x = 0xBF;
    rects[4].y = 0xBB;
    rects[4].w = 0xF2;
    rects[4].h = 0x4A;
    rects[5].x = 0xBF;
    rects[5].y = 0x5C;
    rects[5].w = 0xF2;
    rects[5].h = 0x64;
    rects[6].x = 0x23;
    rects[6].y = 0x35;
    rects[6].w = 0x36;
    rects[6].h = 0x8;
    for (i = 0; i < 7; i++) {
        if (i == 2) {
            continue;
        }
        switch (i) {
        case 0:
        case 2:
            flags = 8;
            style = 0x41;
            break;
        case 1:
        case 4:
        case 5:
            flags = 0;
            style = 0x31;
            break;
        case 3:
        case 6:
            flags = 8;
            style = 0x21;
            break;
        default:
            style = 0;
            flags = 0;
            break;
        }
        func_801E8D98(&D_801F3250[i], rects[i], (s32)labels[i], flags, style);
    }
    animateWindowTo(&D_801F3250[0], (Rect16 *)-1);
    animateWindowTo(&D_801F32D8, (Rect16 *)-1);
}

s32 func_801ECE70(s32 cardId) {
    s16 unused[6] = { 0xAF, 0xB6, 0xB7, 0xB8, 0xBB, 0xBE }; /* unused, but it is in the original stack frame */
    s32 unused2[2];
    s32 count;
    s32 i;

    if (cardId >= 0xAC && cardId <= 0xAE) {
        return 0;
    }
    if (cardId >= 0xB0 && cardId <= 0xB5) {
        return 0;
    }
    if (cardId >= 0xB9 && cardId <= 0xBA) {
        return 0;
    }
    if (cardId == 0xBC) {
        return 0;
    }
    if (cardId == 0xBD) {
        return 0;
    }
    count = 0;
    for (i = 0; i < 30; i++) {
        if (D_801F4328->cards[i].type != 0xFF && cardId == getCardId(D_801F4328->cards[i].type, D_801F4328->cards[i].index)) {
            count++;
        }
    }
    return count < 4;
}

s32 func_801ECF80(PlayerDeck *a, PlayerDeck *b) {
    s8 matched[30];
    s32 i;
    s32 j;
    s32 result = 0;

    for (i = 0; i < 30; i++) {
        matched[i] = 0;
    }
    for (i = 0; i < 30; i++) {
        for (j = 0; j < 30; j++) {
            if (matched[j] == 0 && a->cards[i].type == b->cards[j].type) {
                if (a->cards[i].type == 0xFF) {
                    matched[j] = 1;
                    j = 30;
                } else if (a->cards[i].index == b->cards[j].index) {
                    matched[j] = 1;
                    j = 30;
                }
            }
        }
    }
    for (i = 0; i < 30; i++) {
        if (matched[i] == 0) {
            result = 1;
        }
    }
    return result;
}

void func_801ED070(PlayerDeck *deck) {
    u8 dialog[0xC0];
    Rect16 from;
    Rect16 to;
    s16 r;
    s16 w;
    s16 h;
    s16 ty;
    s32 result;
    s32 i;

    D_801F41A8.useDeckCounts = 1;
    if (D_801F41A8.listShown == 0 && ((u16)PAD_STATES[D_801F41A8.player]->repeat & 0xF000)) {
        playMenuSound(2);
    }
    if ((u16)PAD_STATES[D_801F41A8.player]->repeat & 0x1000) {
        if (D_801F41A8.listShown == 0) {
            D_801F3F48.slot -= 10;
            if (D_801F3F48.slot < 10) {
                PAD_STATES[D_801F41A8.player]->repeatEnabled = 0;
            }
            if (D_801F3F48.slot < 0) {
                D_801F3F48.slot += 30;
            }
        }
    } else if ((u16)PAD_STATES[D_801F41A8.player]->repeat & 0x4000) {
        if (D_801F41A8.listShown == 0) {
            D_801F3F48.slot += 10;
            if (D_801F3F48.slot >= 20) {
                PAD_STATES[D_801F41A8.player]->repeatEnabled = 0;
            }
            if (D_801F3F48.slot >= 30) {
                D_801F3F48.slot -= 30;
            }
        }
    } else if ((u16)PAD_STATES[D_801F41A8.player]->repeat & 0x2000) {
        if (D_801F41A8.listShown == 0) {
            D_801F3F48.slot++;
            r = D_801F3F48.slot % 10;
            if (r == 9) {
                PAD_STATES[D_801F41A8.player]->repeatEnabled = 0;
            }
            r = D_801F3F48.slot % 10;
            if (r == 0) {
                D_801F3F48.slot -= 10;
            }
        }
    } else if ((u16)PAD_STATES[D_801F41A8.player]->repeat & 0x8000) {
        if (D_801F41A8.listShown == 0) {
            D_801F3F48.slot--;
            r = D_801F3F48.slot % 10;
            if (r == 0) {
                PAD_STATES[D_801F41A8.player]->repeatEnabled = 0;
            }
            r = D_801F3F48.slot % 10;
            if (r == 9) {
                D_801F3F48.slot += 10;
            } else if (D_801F3F48.slot < 0) {
                D_801F3F48.slot = 9;
            }
        }
    } else if (PAD_STATES[D_801F41A8.player]->pressed & 4) {
        if (D_801F3F48.statsPage != 0) {
            playMenuSound(1);
        }
        D_801F3F48.statsPage = 0;
    } else if (PAD_STATES[D_801F41A8.player]->pressed & 8) {
        if (D_801F3F48.statsPage == 0) {
            playMenuSound(1);
        }
        D_801F3F48.statsPage = 1;
    } else if (PAD_STATES[D_801F41A8.player]->pressed & 0x40) {
        if (D_801F41A8.listShown == 0) {
            from.x = 0xA3;
            from.y = 0xBB;
            from.w = 0x128;
            from.h = 0x4E;
            w = from.w;
            ty = 0x94;
            to.x = 0xF;
            h = from.h;
            to.y = ty;
            to.w = w;
            to.h = h;
            D_801F3F48.mode = 2;
            animateWindowTo(D_801F3250, &to);
            animateWindowTo(&D_801F32D8, &D_801F1FE4.rect);
            animateWindowTo(&D_801F331C, (Rect16 *)-1);
            animateWindowTo(&D_801F3360, (Rect16 *)-1);
            animateWindowTo(&D_801F33A4, (Rect16 *)-1);
            playMenuSound(1);
        }
    } else if (PAD_STATES[D_801F41A8.player]->pressed & 0x10) {
        if (D_801F41A8.listShown == 0) {
            playMenuSound(4);
            result = 0;
            for (i = 0; i < 30; i++) {
                if (D_801F4328->cards[i].type == 0xFF) {
                    result = 1;
                    break;
                }
            }
            if (result != 1) {
                result = 2;
                for (i = 0; i < 30; i++) {
                    if (D_801F4328->cards[i].type == 0) {
                        result = 0;
                        break;
                    }
                }
            }
            if (result == 1) {
                initDialog(dialog, "There are not enough Cards.\nDo you want to quit editing this Deck?", 1);
                dialog[0xA6] = D_801F41A8.player;
                runDialog(dialog);
                if ((s8)dialog[0xA5] == 1) {
                    if (D_801F4058 == 1) {
                        deck->inUse = 0;
                    }
                    D_801F41A8.editing = 0;
                }
            } else if (result == 2) {
                initDialog(dialog, "Please place Digimon Cards in a Deck.", 0);
                dialog[0xA6] = D_801F41A8.player;
                runDialog(dialog);
            } else if (func_801ECF80(&D_801F41C8, D_801F4328) != 0 || D_801F4058 == 1) {
                initDialog(dialog, "Do you want to update this Deck?", 1);
                dialog[0xA6] = D_801F41A8.player;
                runDialog(dialog);
                if ((s8)dialog[0xA5] == 1) {
                    storeSavedDeck(D_801F41A8.player, D_801F4328, D_801F4060.slot);
                    linkSavedDecks(D_801F41A8.player);
                    D_801F41A8.editing = 0;
                } else if ((s8)dialog[0xA5] == 2) {
                    initDialog(dialog, "Do you want to quit editing this Deck?", 1);
                    dialog[0xA6] = D_801F41A8.player;
                    runDialog(dialog);
                    if ((s8)dialog[0xA5] == 1) {
                        if (D_801F4058 == 1) {
                            deck->inUse = 0;
                        }
                        D_801F41A8.editing = 0;
                    }
                }
            } else {
                D_801F41A8.editing = 0;
                *deck = *D_801F4328;
            }
        } else {
            playMenuSound(4);
            D_801F200A = 1;
            animateWindowTo(&D_801F3E50, (Rect16 *)-1);
            D_801F41A8.listShown ^= 1;
        }
    } else if (PAD_STATES[D_801F41A8.player]->pressed & 0x100) {
        D_801F41A8.listShown ^= 1;
        if (D_801F41A8.listShown != 0) {
            playMenuSound(3);
            D_801F200A = 0;
            animateWindowTo(&D_801F3E50, &D_801F219C.rect);
        } else {
            playMenuSound(4);
            D_801F200A = 1;
            animateWindowTo(&D_801F3E50, (Rect16 *)-1);
        }
    }
}

void func_801ED944(void) {
    Rect16 from;
    Rect16 to;
    s16 w;
    s16 h;

    D_801F3F48.mode = 1;
    animateWindowTo(D_801F3250, (Rect16 *)-1);
    animateWindowTo(&D_801F32D8, (Rect16 *)-1);
    from.x = 0x23;
    from.y = 0xAD;
    from.w = 0x36;
    from.h = 0x66;
    w = from.w;
    to.x = 0x8;
    h = from.h;
    to.y = 0x7A;
    to.w = w;
    to.h = h;
    animateWindowTo(&D_801F331C, &to);
    from.x = 0xBF;
    from.y = 0xBB;
    from.w = 0xF2;
    from.h = 0x4A;
    w = from.w;
    to.x = 0x46;
    h = from.h;
    to.y = 0x96;
    to.w = w;
    to.h = h;
    animateWindowTo(&D_801F3360, &to);
    from.x = 0xBF;
    from.y = 0x5C;
    from.w = 0xF2;
    from.h = 0x64;
    w = from.w;
    to.x = 0x46;
    h = from.h;
    to.y = 0x2A;
    to.w = w;
    to.h = h;
    animateWindowTo(&D_801F33A4, &to);
}

void func_801EDA88(PlayerDeck *deck) {
    Rect16 rect;
    s32 cardId;

    D_801F41A8.useDeckCounts = 2;
    if (PAD_STATES[D_801F41A8.player]->pressed & 0x40) {
        if (D_801F41A8.listShown == 0 && D_801F3F48.deckCounts[D_801F3F48.cardId] != 0 && func_801ECE70(D_801F3F48.cardId) != 0) {
            cardId = getCardId(D_801F4328->cards[D_801F3F48.slot].type, D_801F4328->cards[D_801F3F48.slot].index);
            D_801F3F48.deckCounts[D_801F3F48.cardId]--;
            if (D_801F4328->cards[D_801F3F48.slot].type != 0xFF) {
                D_801F3F48.deckCounts[cardId]++;
            }
            setCardSlotFromId((u8 *)&D_801F4328->cards[D_801F3F48.slot], D_801F3F48.cardId);
            playMenuSound(1);
            func_801ED944();
        }
    } else if (PAD_STATES[D_801F41A8.player]->pressed & 0x10) {
        if (D_801F41A8.listShown == 0) {
            playMenuSound(0);
            func_801ED944();
        } else {
            playMenuSound(4);
            D_801F41A8.listShown ^= 1;
            D_801F200A = 1;
            animateWindowTo(&D_801F3430, (Rect16 *)-1);
        }
    } else if (PAD_STATES[D_801F41A8.player]->pressed & 0x100) {
        D_801F41A8.listShown ^= 1;
        if (D_801F41A8.listShown != 0) {
            playMenuSound(3);
            D_801F200A = 0;
            rect.x = 0x28;
            rect.y = 0x3C;
            rect.w = 0x52;
            rect.h = 0x6E;
            animateWindowTo(&D_801F3430, &D_801F20E0);
        } else {
            playMenuSound(4);
            D_801F200A = 1;
            animateWindowTo(&D_801F3430, (Rect16 *)-1);
        }
    }
}

void func_801EDD60(PlayerDeck *deck) {
    u8 unused[0xC0]; /* unused, but it is in the original stack frame */
    s32 i;

    func_801EC998(deck);
    D_801F41A8.useDeckCounts = 1;
    D_801F41C8 = *deck;
    playMenuSound(3);
    addFrameCallback((s32)func_801EC7A8);
    D_801F41A8.editing = 1;
    D_801F41A8.slide = 0;
    D_801F41A8.hidden = 0;
    D_801F41A8.listShown = 0;
    do {
        func_80014C08(1);
        if (D_801F3F48.mode == 1) {
            func_801ED070(deck);
        } else {
            func_801EDA88(deck);
        }
    } while (D_801F41A8.editing != 0);
    D_801F41A8.hidden = -1;
    for (i = 0; i < 7; i++) {
        animateWindowTo(&D_801F3250[i], (Rect16 *)-1);
    }
    animateWindowTo(&D_801F3430, (Rect16 *)-1);
    func_80014C08(20);
    removeFrameCallback((s32)func_801EC7A8);
    func_80014C08(1);
    func_800149B8(0, -1, 0, 0x1000, func_801EBCC0, 0, 0, 0, 0);
}

void func_801EDEFC(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    drawIcon(x, y, 1, 12, z);
    drawMediumText(x + 8, y, (s32)":Sort", 7, z);
}

void func_801EDF6C(UiWindow *window) {
    char buf[72];
    s32 cardId = 0;
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 slot;
    s32 i;

    slot = getCardId(D_801F4328->cards[D_801F3F48.slot].type, D_801F4328->cards[D_801F3F48.slot].index);
    if (slot != -1) {
        if (slot >= 0) {
            D_801F4188.request = getCardId(D_801F4328->cards[D_801F3F48.slot].type, D_801F4328->cards[D_801F3F48.slot].index);
            cardId = D_801F4188.request;
            slot = func_801E8670(cardId);
        }
        if (slot == -1) {
            if (++D_801F41A8.blink & 4) {
                func_801E89FC(x + 3, y + 0x1A, 0x7E20, 0x200, 0x100, 0x40, 0x40, 0, 0x80, slot, z);
            } else {
                func_801E89FC(x + 3, y + 0x1A, 0x7E20, 0x210, 0x100, 0x40, 0x40, 0, 0x80, -1, z);
            }
        } else {
            D_801F41A8.blink = 0;
            func_801E89FC(x + 3, y + 0x1A, getClut(0, slot + 0x1F4), (slot / 4) * 32 + 0x140, (slot % 4) * 64 + 0x100, 0x40, 0x40, 1, 0x80, -1, z);
        }
        sprintf(buf, D_801DF288, *(s16 *)D_801F3990[cardId]);
        drawText(x + 3, y, (s32)buf, 7, z);
        drawText(x + 0x7B, y, (s32)(D_801F3990[cardId] + 3), 7, z);
        switch (D_801F3990[cardId][2]) {
        case 0:
            drawIcon(x + 0x57, y, 0, ((DigimonCardData *)D_801F3990[cardId])->attr >> 4, z);
            drawIcon(x + 0x27, y, 0, (((DigimonCardData *)D_801F3990[cardId])->attr & 0xF) + 0x10, z);
            drawText(x + 0x1B, y, (s32)D_801DF27C, 7, z);
            drawText(x + 0x39, y, (s32)D_801DF280, 7, z);
            sprintf(buf, D_801DF4FC, PLAYER_DATA(D_801F41A8.player).unk11B6[*(s16 *)D_801F3990[cardId]]);
            drawText(x + 0x1B, y + 12, (s32)buf, 7, z);
            drawText(x + 0x33, y + 12, (s32)D_801DF554, 6, z);
            sprintf(buf, D_801DF4FC, PLAYER_DATA(D_801F41A8.player).unk1334[*(s16 *)D_801F3990[cardId]]);
            drawText(x + 0x53, y + 12, (s32)buf, 7, z);
            drawText(x + 0x6B, y + 12, (s32)D_801DF55C, 6, z);
            drawIcon(x + 0x4B, y + 0x18, 0, 0x1A, z);
            sprintf(buf, D_801DF50C, ((DigimonCardData *)D_801F3990[cardId])->hp);
            drawText(x + 0x5D, y + 0x18, (s32)buf, 7, z);
            drawIcon(x + 0x85, y + 0x18, 0, 0x18, z);
            sprintf(buf, D_801DF54C, ((DigimonCardData *)D_801F3990[cardId])->dpCost);
            drawText(x + 0x97, y + 0x18, (s32)buf, 7, z);
            drawIcon(x + 0xAF, y + 0x18, 0, 0x19, z);
            sprintf(buf, D_801DF54C, ((DigimonCardData *)D_801F3990[cardId])->dpBonus);
            drawText(x + 0xC1, y + 0x18, (s32)buf, 7, z);
            y += 12;
            for (i = 0; i < 3; i++) {
                sprintf(buf, "b%d", i);
                func_80029EC4(x + 0x4B, y + (i + 2) * 12, 7, 1, z, (s32)buf);
                sprintf(buf, D_801DF50C, ((DigimonCardData *)D_801F3990[cardId])->attack[i].power);
                drawText(x + 0x5D, y + (i + 2) * 12, (s32)buf, 7, z);
            }
            drawSmallText(x + 0x45, y + 0x3F, (s32)CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)D_801F3990[cardId])->crossEffect], 7, z);
            if (D_8006E4FC[((DigimonCardData *)D_801F3990[cardId])->crossEffect] != 0) {
                drawIcon(x + 0x51, y + 0x48, 0, D_8006E4FC[((DigimonCardData *)D_801F3990[cardId])->crossEffect] + 0x14, z);
            }
            drawText(x + 0x85, y + 0x18, (s32)D_801DF564, 6, z);
            if (((DigimonCardData *)D_801F3990[cardId])->supportIcon != 0) {
                drawIcon(x + 0xD9, y + 0x18, 0, ((DigimonCardData *)D_801F3990[cardId])->supportIcon + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                drawText(x + 0x85, y + (i + 3) * 12, (s32)((DigimonCardData *)D_801F3990[cardId])->supportText[i], 7, z);
            }
            break;
        case 1:
            drawIcon(x + 0x18, y, 0, 5, z);
            if (D_801F3990[cardId][0x8C] != 0) {
                drawIcon(x + 0x85, y + 13, 0, D_801F3990[cardId][0x8C] + 0x14, z);
            }
            y += 0x1A;
            for (i = 0; i < 4; i++) {
                drawText(x + 0x85, y, (s32)((u8 *)&((OptionCardData *)OPTION_CARDS)[*(s16 *)D_801F3990[cardId] - 0xBF] + 0x8D + i * 21), 7, z);
                y += 12;
            }
            break;
        case 2:
            drawIcon(x + 0x18, y, 0, 6, z);
            y += 0x1A;
            for (i = 0; i < 4; i++) {
                drawText(x + 0x85, y, (s32)((u8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[*(s16 *)D_801F3990[cardId] - 0x125] + 0x1B + i * 21), 7, z);
                y += 12;
            }
            break;
        }
    }
}

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF7D4);

void func_801EE8CC(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    s32 x = window->originX - 11;
    s32 z = window->z;
    s32 i;
    s32 y;
    s32 palette;
    s8 type;
    u8 *rgb;
    s32 usable;

    for (i = 0; i < D_801F1FE4.nrows; i++) {
        if (i < window->view.y / D_801F1FE4.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / D_801F1FE4.rowH < i) {
            break;
        }
        y = window->originY + i * D_801F1FE4.rowH + 1;
        palette = 7;
        type = ((s8 *)D_801F34D0[i])[2];
        rgb = D_801F2028[0];
        if (func_801ECE70(*(s16 *)D_801F34D0[i]) == 0) {
            palette = 3;
            usable = 0;
        } else {
            usable = D_801F3F48.deckCounts[*(s16 *)D_801F34D0[i]] != 0;
        }
        if (PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)D_801F34D0[i]] & 0x40) {
            if (D_801F3F48.deckCounts[*(s16 *)D_801F34D0[i]] == 0) {
                rgb = D_801F2028[2];
            }
            if (!usable) {
                drawTextColored(x + 0xE3, y, D_801DF7D4, rgb, palette, z);
            } else {
                drawTextColored(x + 0xE3, y, "Able", rgb, palette, z);
            }
            drawTextColored(x + 0x69, y, (u8 *)D_801F34D0[i] + 3, rgb, palette, z);
            switch (type) {
            case 0:
                drawIconColored(x + 0x5B, y, 0, ((DigimonCardData *)D_801F34D0[i])->attr >> 4, rgb, z);
                if (palette == 3) {
                    drawIconColored(x + 0x2D, y, 0, (((DigimonCardData *)D_801F34D0[i])->attr & 0xF) + 0x10, D_801F2028[3], z);
                } else {
                    drawIconColored(x + 0x2D, y, 0, (((DigimonCardData *)D_801F34D0[i])->attr & 0xF) + 0x10, rgb, z);
                }
                break;
            case 1:
                drawIconColored(x + 0x5B, y, 0, 5, rgb, z);
                break;
            case 2:
                drawIconColored(x + 0x5B, y, 0, 6, rgb, z);
                break;
            }
        } else {
            palette = 9;
            drawTextColored(x + 0x59, y, "?", rgb, palette, z);
            drawTextColored(x + 0x69, y, "------------------", rgb, palette, z);
        }
        if (type == 0 || !(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)D_801F34D0[i]] & 0x40)) {
            drawTextColored(x + 0x20, y, D_801DF27C, rgb, palette, z);
            drawTextColored(x + 0x3D, y, D_801DF280, rgb, palette, z);
        }
        sprintf(buf, D_801DF288, *(s16 *)D_801F34D0[i]);
        drawTextColored(x + 10, y, buf, rgb, palette, z);
        sprintf(buf, D_801DF294, D_801F3F48.deckCounts[*(s16 *)D_801F34D0[i]]);
        drawTextColored(x + 0x10F, y, buf, rgb, palette, z);
        drawTinyTextColored(x + 0x118, y + 6, D_801DF298, palette, rgb, z);
    }
    updateMenuCursor(&D_801F1FE4);
    D_801F3F48.cardId = *(s16 *)D_801F34D0[D_801F1FE4.row];
    D_801F3F48.card = D_801F34D0[D_801F1FE4.row];
}

void func_801EEE40(UiWindow *window) {
    char buf[72];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 slot;
    s32 i;

    if (PLAYER_DATA(D_801F41A8.player).cardCollection[D_801F4050] & 0x40) {
        D_801F4188.request = D_801F4050;
        slot = func_801E8670(D_801F4050);
        if (slot == -1) {
            if (++D_801F41A8.blink & 4) {
                func_801E89FC(x + 3, y + 14, 0x7E20, 0x200, 0x100, 0x40, 0x40, 0, 0x80, slot, z);
            } else {
                func_801E89FC(x + 3, y + 14, 0x7E20, 0x210, 0x100, 0x40, 0x40, 0, 0x80, -1, z);
            }
        } else {
            D_801F41C0 = 0;
            func_801E89FC(x + 3, y + 14, getClut(0, slot + 0x1F4), (slot / 4) * 32 + 0x140, (slot % 4) * 64 + 0x100, 0x40, 0x40, 1, 0x80, -1, z);
        }
        sprintf(buf, D_801DF288, *(s16 *)D_801F3F48.card);
        drawText(x + 3, y, (s32)buf, 7, z);
        drawText(x + 0x7B, y, (s32)(D_801F3F48.card + 3), 7, z);
        switch (*(s16 *)D_801F3F48.card) {
        case 0xAF:
        case 0xB6:
        case 0xB7:
        case 0xB8:
        case 0xBB:
        case 0xBE:
            drawText(x + 0xCC, y, (s32)D_801DF594, 6, z);
            break;
        }
        switch (D_801F404C[2]) {
        case 0:
            drawIcon(x + 0x63, y, 0, ((DigimonCardData *)D_801F3F48.card)->attr >> 4, z);
            drawIcon(x + 0x27, y, 0, (((DigimonCardData *)D_801F3F48.card)->attr & 0xF) + 0x10, z);
            drawText(x + 0x1B, y, (s32)D_801DF27C, 7, z);
            drawText(x + 0x45, y, (s32)D_801DF280, 7, z);
            sprintf(buf, D_801DF294, getOwnedCardCount(D_801F41BA, *(s16 *)D_801F3F48.card));
            drawText(x + 0xFD, y, (s32)buf, 7, z);
            drawText(x + 0x109, y, (s32)D_801DF298, 7, z);
            drawIcon(x + 0x4B, y + 12, 0, 0x1A, z);
            sprintf(buf, D_801DF50C, ((DigimonCardData *)D_801F3F48.card)->hp);
            drawText(x + 0x69, y + 12, (s32)buf, 7, z);
            for (i = 0; i < 3; i++) {
                sprintf(buf, "*b%d", i);
                drawText(x + 0x4B, y + (i + 2) * 12, (s32)buf, 7, z);
                sprintf(buf, D_801DF50C, ((DigimonCardData *)D_801F3F48.card)->attack[i].power);
                drawText(x + 0x69, y + (i + 2) * 12, (s32)buf, 7, z);
            }
            drawSmallText(x + 0x57, y + 0x3E, (s32)CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)D_801F3F48.card)->crossEffect], 7, z);
            if (D_8006E4FC[((DigimonCardData *)D_801F3F48.card)->crossEffect] != 0) {
                drawIcon(x + 0x91, y + 0x45, 0, D_8006E4FC[((DigimonCardData *)D_801F3F48.card)->crossEffect] + 0x14, z);
            }
            drawIcon(x + 0x91, y + 12, 0, 0x18, z);
            sprintf(buf, D_801DF54C, ((DigimonCardData *)D_801F3F48.card)->dpCost);
            drawText(x + 0xA3, y + 12, (s32)buf, 7, z);
            drawIcon(x + 0x91, y + 0x18, 0, 0x19, z);
            sprintf(buf, D_801DF54C, ((DigimonCardData *)D_801F3F48.card)->dpBonus);
            drawText(x + 0xA3, y + 0x18, (s32)buf, 7, z);
            drawText(x + 0xB9, y + 12, (s32)D_801DF564, 6, z);
            if (((DigimonCardData *)D_801F3F48.card)->supportIcon != 0) {
                drawIcon(x + 0x10E, y + 12, 0, ((DigimonCardData *)D_801F3F48.card)->supportIcon + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                drawText(x + 0xB9, y + (i + 2) * 12, (s32)((DigimonCardData *)D_801F3F48.card)->supportText[i], 7, z);
            }
            break;
        case 1:
            drawIcon(x + 0x18, y, 0, 5, z);
            sprintf(buf, D_801DF294, getOwnedCardCount(D_801F41BA, *(s16 *)D_801F3F48.card));
            drawText(x + 0xFD, y, (s32)buf, 7, z);
            drawText(x + 0x109, y, (s32)D_801DF298, 7, z);
            if (D_801F3F48.card[0x8C] != 0) {
                drawIcon(x + 0x8D, y + 13, 0, D_801F3F48.card[0x8C] + 0x14, z);
            }
            y += 0x1A;
            for (i = 0; i < 4; i++) {
                drawText(x + 0x8D, y, (s32)((u8 *)&((OptionCardData *)OPTION_CARDS)[*(s16 *)D_801F3F48.card - 0xBF] + 0x8D + i * 21), 7, z);
                y += 12;
            }
            break;
        case 2:
            drawIcon(x + 0x18, y, 0, 6, z);
            sprintf(buf, D_801DF294, getOwnedCardCount(D_801F41BA, *(s16 *)D_801F404C));
            drawText(x + 0xFD, y, (s32)buf, 7, z);
            drawText(x + 0x109, y, (s32)D_801DF298, 7, z);
            y += 0x1A;
            for (i = 0; i < 4; i++) {
                drawText(x + 0x8D, y, (s32)((u8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[*(s16 *)D_801F3F48.card - 0x125] + 0x1B + i * 21), 7, z);
                y += 12;
            }
            break;
        }
    }
}

void func_801EF740(UiWindow *window) {
    char buf[72];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 width;

    if (D_801F3F48.mode == 1) {
        sprintf(buf, "%s Deck", D_801F4328->unk1);
        width = measureText(buf) - 0xA0;
        drawText(x - width, y + 1, (s32)buf, 7, z);
    } else {
        drawText(x + 0x30, y + 1, (s32)"Card Selection", 7, z);
    }
}

void func_801EF7EC(UiWindow *window) {
    char buf[72];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 i;

    switch (D_801F3F48.statsPage) {
    case 0:
        sprintf(D_801F42E0, "         R1");
        break;
    case 1:
        sprintf(D_801F42E0, "L1      ");
        break;
    }
    window->label = (s32)D_801F42E0;
    if (D_801F3F48.statsPage == 0) {
        s32 masks[7] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x40, 0x80 };

        for (i = 0; i < 7; i++) {
            drawIcon(x, y, 0, i, z);
            sprintf(buf, "%2d", countDeckCardsByFilter(D_801F41A8.player, D_801F4328, masks[i]));
            drawText(x + 0x15, y, (s32)buf, 7, z);
            drawTinyText(x + 0x23, y + 6, (s32)D_801DF298, 7, z);
            y += 13;
        }
        drawTinyText(x, y + 6, (s32)D_801DF504, 7, z);
        sprintf(buf, "%2d", countDeckCardsByFilter(D_801F41A8.player, D_801F4328, 0xDF));
        drawText(x + 0x15, y, (s32)buf, 7, z);
        drawTinyText(x + 0x23, y + 6, (s32)D_801DF298, 7, z);
    } else if (D_801F3F48.statsPage == 1) {
        s32 masks[5] = { 0x200, 0x800, 0x1000, 0x40, 0x80 };

        for (i = 0; i < 5; i++) {
            if (i == 0) {
                drawIcon(x, y, 0, 0x10, z);
            } else if (i >= 3) {
                drawIcon(x, y, 0, i + 2, z);
            } else {
                drawIcon(x, y, 0, i + 0x11, z);
            }
            sprintf(buf, "%2d", countDeckCardsByFilter(D_801F41A8.player, D_801F4328, masks[i]));
            drawText(x + 0x15, y, (s32)buf, 7, z);
            drawTinyText(x + 0x23, y + 6, (s32)D_801DF298, 7, z);
            y += 13;
        }
        y += 13;
        drawTinyText(x, y + 6, (s32)D_801DF504, 7, z);
        sprintf(buf, "%2d", countDeckCardsByFilter(D_801F41A8.player, D_801F4328, 0xDF));
        drawText(x + 0x15, y, (s32)buf, 7, z);
        drawTinyText(x + 0x23, y + 6, (s32)D_801DF298, 7, z);
        y += 13;
        drawText(x, y, (s32)"Pa", 6, z);
        sprintf(buf, "%2d", countDeckCardsByFilter(D_801F41A8.player, D_801F4328, 0x20));
        drawText(x + 0x15, y, (s32)buf, 6, z);
        drawTinyText(x + 0x23, y + 6, (s32)D_801DF298, 6, z);
    }
}

void func_801EFC78(UiWindow *window) {
    s16 xs[4];
    s16 ys[4];
    char buf[72]; /* unused, but it is in the original stack frame */
    LINE_G3 *line = D_801F3F48.cursorLines[FRAME_BUFFER_INDEX];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 i;
    u8 brightness;
    s16 col;
    s16 row;

    if (D_801F3F48.mode == 1 || D_801F3F48.mode == 2) {
        col = D_801F3F48.slot % 10;
        xs[0] = x - 1 + col * 24;
        xs[1] = xs[0] + 25;
        row = D_801F3F48.slot / 10;
        ys[0] = y - 1 + row * 24;
        ys[1] = ys[0] + 25;
        line->x0 = line->x1 = line->x2 = xs[0];
        line->y0 = line->y1 = ys[0];
        line->y2 = ys[1];
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], line);
        line++;
        line->x0 = line->x1 = xs[0];
        line->x2 = xs[1];
        line->y0 = line->y1 = line->y2 = ys[0];
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], line);
        line++;
        line->x0 = line->x1 = line->x2 = xs[1];
        line->y0 = line->y1 = ys[0];
        line->y2 = ys[1];
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], line);
        line++;
        line->x0 = line->x1 = xs[0];
        line->x2 = xs[1];
        line->y0 = line->y1 = line->y2 = ys[1];
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], line);
    }
    for (i = 0; i < 30; i++) {
        if (D_801F4328->cards[i].type != 0xFF) {
            if (D_801F3F48.mode == 0) {
                brightness = 0x80;
            } else if (D_801F3F48.slot == i) {
                brightness = 0x90;
            } else {
                brightness = 0x40;
            }
            func_801E8864(getCardId(D_801F4328->cards[i].type, D_801F4328->cards[i].index), x + (i % 10) * 24, y + (i / 10) * 24, brightness, z);
        }
    }
}

void func_801F0024(UiWindow *window) {
    Rect16 rect;
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 dir = 0;
    MenuItem *item;
    s32 dy;

    if ((u16)PAD_STATES[D_801F41BA]->repeat & 0xF000) {
        playMenuSound(2);
    }
    if ((u16)PAD_STATES[D_801F41BA]->repeat & 0x1000) {
        if (D_801F4070[1] == 0) {
            if (D_801F4070[0] == 11 || D_801F4070[0] == 12) {
                PAD_STATES[D_801F41BA]->repeatEnabled = 0;
            }
        } else if (D_801F4070[0] >= 2 && D_801F4070[0] <= 6) {
            PAD_STATES[D_801F41BA]->repeatEnabled = 0;
        }
        dir = 1;
    } else if ((u16)PAD_STATES[D_801F41BA]->repeat & 0x4000) {
        if (D_801F4070[1] == 0) {
            if (D_801F4070[0] < 2) {
                PAD_STATES[D_801F41BA]->repeatEnabled = 0;
            }
        } else if (D_801F4070[0] == 9 || D_801F4070[0] == 10) {
            PAD_STATES[D_801F41BA]->repeatEnabled = 0;
        }
        dir = 2;
    } else if ((u16)PAD_STATES[D_801F41BA]->repeat & 0x8000) {
        if (D_801F4070[0] == 1 || D_801F4070[0] == 3 || D_801F4070[0] == 8 || D_801F4070[0] == 10 || D_801F4070[0] == 12) {
            PAD_STATES[D_801F41BA]->repeatEnabled = 0;
        }
        dir = 3;
    } else if ((u16)PAD_STATES[D_801F41BA]->repeat & 0x2000) {
        if (D_801F4070[0] == 0 || D_801F4070[0] == 5 || D_801F4070[0] == 7 || D_801F4070[0] == 9 || D_801F4070[0] == 11) {
            PAD_STATES[D_801F41BA]->repeatEnabled = 0;
        }
        dir = 4;
    } else if (PAD_STATES[D_801F41BA]->pressed & 0x40) {
        dir = 5;
        playMenuSound(1);
    }
    if (dir >= 1 && dir <= 4) {
        D_801F4070[0] = D_801F2224[D_801F4070[0]].next[dir - 1];
        if (D_801F4070[1] == 0) {
            switch (D_801F4070[0]) {
            case 3:
                D_801F4070[0] = 11;
                break;
            case 5:
                D_801F4070[0] = 12;
                break;
            case 9:
                D_801F4070[0] = 0;
                break;
            case 10:
                D_801F4070[0] = 1;
                break;
            }
        }
        item = &D_801F2224[D_801F4070[0]];
        if (D_801F4070[0] < 2) {
            dy = 0;
        } else if (D_801F4070[0] < 7) {
            dy = 0x18;
        } else if (D_801F4070[0] < 9) {
            dy = 0x30;
        } else if (D_801F4070[0] < 11) {
            dy = 0x48;
        } else {
            dy = 0x60;
        }
        rect.x = window->rect.x + item->x;
        rect.y = window->rect.y + dy;
        rect.w = item->w;
        rect.h = 12;
        moveCursorHighlight(&D_801F3EF0, &rect);
    } else if (dir == 5) {
        switch (D_801F4070[0]) {
        case 0:
            D_801F4071 = 0;
            break;
        case 1:
            D_801F4071 = 1;
            break;
        case 2:
            D_801F4072 = 0;
            break;
        case 3:
            D_801F4072 = 1;
            break;
        case 4:
            D_801F4072 = 2;
            break;
        case 5:
            D_801F4072 = 3;
            break;
        case 6:
            D_801F4072 = 4;
            break;
        case 7:
            D_801F4073 = 0;
            break;
        case 8:
            D_801F4073 = 1;
            break;
        case 9:
            D_801F4074 = 0;
            break;
        case 10:
            D_801F4074 = 1;
            break;
        case 11:
            D_801F4075 = 1;
            break;
        case 12:
            D_801F4075 = 3;
            break;
        }
    }
    drawText(x, y, (s32)"Auto Deck", 6, z);
    drawText(x + D_801F2224[0].x, y, (s32)D_801DF7D4, D_801F4071 ? 8 : 7, z);
    drawText(x + D_801F2224[1].x, y, (s32)"Enable", D_801F4071 ? 7 : 8, z);
    drawText(x, y + 0x18, (s32)"Specialty", 6, z);
    drawText(x + D_801F2224[2].x, y + 0x18, (s32)"Fire", (D_801F4070[2] == 0 && D_801F4070[1]) ? 7 : 8, z);
    drawText(x + D_801F2224[3].x, y + 0x18, (s32)"Ice", (D_801F4070[2] == 1 && D_801F4070[1]) ? 7 : 8, z);
    drawText(x + D_801F2224[4].x, y + 0x18, (s32)"Nature", (D_801F4070[2] == 2 && D_801F4070[1]) ? 7 : 8, z);
    drawText(x + D_801F2224[5].x, y + 0x18, (s32)"Darkness", (D_801F4070[2] == 3 && D_801F4070[1]) ? 7 : 8, z);
    drawText(x + D_801F2224[6].x, y + 0x18, (s32)"Rare", (D_801F4070[2] == 4 && D_801F4070[1]) ? 7 : 8, z);
    drawText(x, y + 0x30, (s32)D_801DF280, 6, z);
    drawText(x + D_801F2224[7].x, y + 0x30, (s32)"Offensive", (D_801F4070[3] == 0 && D_801F4070[1]) ? 7 : 8, z);
    drawText(x + D_801F2224[8].x, y + 0x30, (s32)"Defensive", (D_801F4070[3] != 0 && D_801F4070[1]) ? 7 : 8, z);
    drawText(x, y + 0x48, (s32)"Option Cards", 6, z);
    drawText(x + D_801F2224[9].x, y + 0x48, (s32)"Many", (D_801F4070[4] == 0 && D_801F4070[1]) ? 7 : 8, z);
    drawText(x + D_801F2224[10].x, y + 0x48, (s32)"Few", (D_801F4070[4] != 0 && D_801F4070[1]) ? 7 : 8, z);
    drawText(x + D_801F2224[11].x, y + 0x60, (s32)"Create", 6, z);
    drawText(x + D_801F2224[12].x, y + 0x60, (s32)"Cancel", 6, z);
    drawCursorHighlight(&D_801F3EF0, z);
}

void func_801F0A20(void) {
    Rect16 rects[2];
    s32 flags[2] = { 8, 0x21 };
    s32 styles[2] = { 0x21, 0x31 };
    char *labels[2] = { "AUTO DECK", D_801DF6C4 };
    s32 i;

    rects[0].x = 0xAB;
    rects[0].y = 0x8D;
    rects[0].w = 0xF2;
    rects[0].h = 0x6E;
    for (i = 0; i < 7; i++) {
        if (i != 1) {
            func_801E8D98(&D_801F3250[i], rects[i], (s32)labels[i], flags[i], styles[i]);
        }
    }
    rects[0].x = D_801F3250[0].originX + 0x54;
    rects[0].y = D_801F3250[0].originY;
    rects[0].w = 0x24;
    rects[0].h = 0xC;
    initCursorHighlight(&D_801F3EF0, &rects[0], (Bytes4 *)-1);
    D_801F4070[0] = D_801F4070[5] = D_801F4070[1] = D_801F4070[2] = D_801F4070[3] = D_801F4070[4] = 0;
}

void func_801F0BB0(void) {
    drawWindow(D_801F3250, func_801F0024, 30);
}

void func_801F0BE0(void) {
    u8 dialog[0xB8];
    s8 result;

    playMenuSound(3);
    func_801F0A20();
    addFrameCallback((s32)func_801F0BB0);
    do {
        func_80014C08(1);
    } while (D_801F4070[5] == 0);
    animateWindowTo(D_801F3250, (Rect16 *)-1);
    func_80014C08(20);
    removeFrameCallback((s32)func_801F0BB0);
    switch (D_801F4070[5]) {
    case 1:
        if (D_801F4070[1] == 1) {
            PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot].inUse = 1;
            func_801F10D8(&PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot]);
        } else {
            PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot].inUse = 0;
            initDialog(dialog, "Do you want to choose from a Base Deck?", 1);
            runDialogForPad((s32 *)dialog, D_801F41A8.player);
            result = dialog[0xA5];
            if (result == 1 && func_801E11D4(&PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot], D_801F41A8.player) >= 0) {
                PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot].inUse = result;
            }
        }
        func_800149B8(0, -1, 0, 0x1000, func_801EDD60, &PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot], 0, 0, 0);
        break;
    case 3:
        PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot].inUse = 0;
        func_800149B8(0, -1, 0, 0x1000, func_801E0C08, 0, PLAYER_DATA(D_801F41A8.player).savedDecks[D_801F4060.slot].unk1, D_801F41A8.player, 0);
        break;
    }
}

s32 func_801F0FD8(CardSlot *slots, s32 row, s32 count) {
    s32 i;

    for (i = 0; i < count && D_801F42D8[row].ids[i] != -1; i++) {
        setCardSlotFromId((u8 *)slots, D_801F42D8[row].ids[i]);
        D_801F42D8[row].ids[i] = -1;
        slots++;
    }
    return i;
}

void func_801F10D8(PlayerDeck *deck) {
    u8 counts[8] = { 10, 10, 6, 4, 5, 12, 8, 5 };
    s16 specialty = 0;
    s32 level = 0;
    s16 *ids = NULL;
    s32 i;
    s32 j;
    s32 n;
    s16 count;

    D_801F42D8 = allocTaskHeapBlock(0x2760);
    for (i = 0; i < 20; i++) {
        D_801F42D8[i].count = 0;
    }
    for (i = 0; i < 301; i++) {
        if (i >= 0xAC && i < 0xBF) {
            continue;
        }
        n = getOwnedCardCount(D_801F41A8.player, i);
        if (i < 0xBF) {
            D_801F3F40 = (s8 *)&((DigimonCardData *)DIGIMON_CARDS)[i];
        } else if (i - 0xBF < 0x66) {
            D_801F3F40 = (s8 *)&((OptionCardData *)OPTION_CARDS)[i - 0xBF];
        } else {
            D_801F3F40 = (s8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[i - 0x125];
        }
        switch (D_801F3F40[2]) {
        case 0:
            specialty = ((DigimonCardData *)D_801F3F40)->attr >> 4;
            level = ((DigimonCardData *)D_801F3F40)->attr & 0xF;
            if (level > 0) {
                level--;
            }
            ids = D_801F42D8[specialty * 3 + level].ids;
            ids += D_801F42D8[specialty * 3 + level].count;
            break;
        case 1:
            specialty = 5;
            level = D_801F228C[i - 0xBF];
            ids = D_801F42D8[specialty * 3 + level].ids;
            ids += D_801F42D8[specialty * 3 + level].count;
            break;
        case 2:
            specialty = 5;
            level = 2;
            ids = D_801F42D8[specialty * 3 + level].ids;
            ids += D_801F42D8[specialty * 3 + level].count;
            break;
        }
        if (n > 0) {
            if (n >= 4) {
                n = 4;
            }
            for (j = 0; j < n; j++) {
                D_801F42D8[specialty * 3 + level].count++;
                *ids++ = i;
            }
        }
    }
    for (i = 0; i < 20; i++) {
        for (j = D_801F42D8[i].count; j < 250; j++) {
            D_801F42D8[i].ids[j] = -1;
        }
    }
    for (i = 0; i < 20; i++) {
        for (j = 0; j < D_801F42D8[i].count; j++) {
            specialty = abs(rand() % D_801F42D8[i].count);
            count = D_801F42D8[i].ids[j];
            D_801F42D8[i].ids[j] = D_801F42D8[i].ids[specialty];
            D_801F42D8[i].ids[specialty] = count;
        }
    }
    n = 0;
    count = 0;
    for (i = 0; i < 5; i++) {
        if (i != D_801F4070[2]) {
            if (D_801F42D8[i * 3].count + D_801F42D8[i * 3 + 1].count + D_801F42D8[i * 3 + 2].count >= count) {
                specialty = i;
                count = D_801F42D8[i * 3].count + D_801F42D8[i * 3 + 1].count + D_801F42D8[i * 3 + 2].count;
            }
        }
    }
    n += func_801F0FD8(&deck->cards[n], D_801F4070[3] + 15, counts[D_801F4070[4] * 4]);
    if (n < counts[D_801F4070[4] * 4]) {
        n += func_801F0FD8(&deck->cards[n], 17, counts[D_801F4070[4] * 4] - n);
        if (n < counts[D_801F4070[4] * 4]) {
            n += func_801F0FD8(&deck->cards[n], 16 - D_801F4070[3], counts[D_801F4070[4] * 4] - n);
        }
    }
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            count = 0;
            for (j = 0; j < 3; j++) {
                if (PLAYER_DATA(D_801F41A8.player).partners[j].cardId != 0) {
                    setCardSlotFromId((u8 *)&deck->cards[n + count], PLAYER_DATA(D_801F41A8.player).partners[j].cardId);
                    count++;
                }
            }
            count = count + func_801F0FD8(&deck->cards[n + count], D_801F4070[2] * 3 + i, counts[i + 1 + D_801F4070[4] * 4] - count);
        } else {
            count = func_801F0FD8(&deck->cards[n], D_801F4070[2] * 3 + i, counts[i + 1 + D_801F4070[4] * 4]);
        }
        if (count < counts[i + 1 + D_801F4070[4] * 4]) {
            count = count + func_801F0FD8(&deck->cards[n + count], specialty * 3 + i, counts[i + 1 + D_801F4070[4] * 4] - count);
        }
        n += count;
    }
    if (n < 30) {
        for (i = 0; i < 20 && n < 30; i++) {
            for (j = 0; j < D_801F42D8[i].count; j++) {
                if (D_801F42D8[i].ids[j] != -1) {
                    setCardSlotFromId((u8 *)&deck->cards[n], D_801F42D8[i].ids[j]);
                    D_801F42D8[i].ids[j] = -1;
                    n++;
                    if (n >= 30) {
                        break;
                    }
                }
            }
        }
    }
    linkDeckCardData(D_801F41A8.player, deck);
}

