#include "common.h"
#include "game.h"
#include "dcb/card_zones.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/script.h"
#include "dcb/text.h"
#include "dcb/duel_util.h"
#include "dcb/window.h"
#include "dcb/duel_launch.h"
#include "dcb/battle_hud.h"
#include "dcb/transform.h"
#include "dcb/effect_object.h"

s32 func_801E0558(s32 id, s32 player, s32 card);
extern s16 D_801FB9D8[];
s32 func_801E05CC(s32 id, s32 player);
extern s16 D_801FB9A8[];
extern s16 D_801FBA28[];
extern s16 D_801FB9E4[];
typedef struct {
    s16 id;
    s16 unk2;
    s16 unk4;
} Slot7CC;
typedef struct {
    u8 unk0[0x7CC];
    Slot7CC slots[4];
} DuelK;
typedef struct {
     void *data;
     Script *script;
     s32 *regs;
     s32 unkC;
     s32 unk10;
} ScriptRunner;
s32 func_801EAB4C();
s32 func_801EC9A4(s32 player, s32 slot);
s32 func_801ECA30(s32 card, s32 player, s32 slot);
void func_801ED65C(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 clutX, s32 clutY, s32 arg8, s32 arg9, s32 argA, s32 brightness, s32 z);
extern u8 *D_801FC454;
void func_801EDD88(void);
void func_801EFB78(void);
typedef struct {
    UiWindow window;
    s32 rank;
} RankUpWindow;
typedef struct {
    UiWindow window;
    s32 index;
} RewardWindow;
typedef struct {
    UiWindow window;
    s32 unk44;
    s32 unk48;
} PrizeWindow;
typedef struct {
     UiWindow window;
     PrizeWindow prizes[3];
     RewardWindow rewards[3];
     s32 showRewards;
} PrizeScreen;
extern PrizeScreen *D_801FC73C;
void func_801F5C14(UiWindow *window);
void func_801F5B94(RewardWindow *w);
void func_801F54BC(PrizeWindow *w);
void func_801F6294(s32 entry, s32 player1, s32 player2, s32 mode1, s32 mode2);
typedef struct {
     s16 kind;
     s16 unk2;
     s32 value;
} EffectTableEntry;
typedef struct {
     u8 unk0[0xC];
     EffectTableEntry entries[16];
     s32 count;
} EffectTable;
extern void (*D_801FC158[])(s32);
void func_801F893C(CardSprite *sprite, u8 *to);
void func_801E6424(Rect16 *rect, u8 *rgb, u8 *rgb2, u8 arg3, s32 arg4, u8 arg5);
void func_801F7128(EffectTemplate *template, s32 arg1, s32 arg2);
extern s32 (*D_801FC148[])(s32, EffectTable *);
extern UiWindow D_801FC884;
void func_801F7A64(void);
typedef struct {
    u8 data[0x14F0];
} Unk14F0;
extern Unk14F0 *D_801D83F8;
extern void *D_801D83F4;
void func_801F8E14(void *arg0);
void func_801F8E34(void *arg0, s32 arg1);
typedef struct {
    u8 data[0x788];
} Unk788;
typedef struct {
    u8 unk0[0x848];
    u16 counts[32];
} ProfileK;

s32 func_801DFE84(s32 player) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    i = 0;
    p = (Player *)DUEL_PLAYERS[player];
    for (; i < 30; i++) {
        card = p->onlineDeck[i];
        if (card != -1 && p->cards[card % 30].type == 0) {
            count++;
        }
    }
    return count;
}

s32 func_801DFF2C(s32 player) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    i = 0;
    p = (Player *)DUEL_PLAYERS[player];
    for (; i < 4; i++) {
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 0) {
            count++;
        }
    }
    return count;
}

s32 func_801DFFD4(s32 player) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    i = 0;
    p = (Player *)DUEL_PLAYERS[player];
    for (; i < 4; i++) {
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 2) {
            count++;
        }
    }
    return count;
}

s32 func_801E0080(s32 player, s32 level) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    for (i = 0; i < 30; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->onlineDeck[i];
        if (card != -1 && p->cards[card % 30].type == 0 && (p->cards[card % 30].card[0x1A] & 0xF) == level) {
            count++;
        }
    }
    return count;
}

s32 func_801E0148(s32 player, s32 level) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    for (i = 0; i < 4; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 0 && (p->cards[card % 30].card[0x1A] & 0xF) == level) {
            count++;
        }
    }
    return count;
}

s32 func_801E0210(s32 player, s32 attr) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    for (i = 0; i < 30; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->onlineDeck[i];
        if (card != -1 && p->cards[card % 30].type == 0 && ((u8)p->cards[card % 30].card[0x1A] >> 4) == attr) {
            count++;
        }
    }
    return count;
}

s32 func_801E02D8(s32 player, s32 attr) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    for (i = 0; i < 4; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 0 && ((u8)p->cards[card % 30].card[0x1A] >> 4) == attr) {
            count++;
        }
    }
    return count;
}

s32 func_801E03A0(s32 player, s32 attr, s32 level) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;
    u8 *data;

    count = 0;
    for (i = 0; i < 30; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->onlineDeck[i];
        if (card != -1 && p->cards[card % 30].type == 0) {
            data = (u8 *)p->cards[card % 30].card;
            if ((data[0x1A] >> 4) == attr && (data[0x1A] & 0xF) == level) {
                count++;
            }
        }
    }
    return count;
}

s32 func_801E047C(s32 player, s32 attr, s32 level) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;
    u8 *data;

    count = 0;
    for (i = 0; i < 4; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 0) {
            data = (u8 *)p->cards[card % 30].card;
            if ((data[0x1A] >> 4) == attr && (data[0x1A] & 0xF) == level) {
                count++;
            }
        }
    }
    return count;
}

s32 func_801E0558(s32 id, s32 player, s32 card) {
    if (card != -1 && ((Player *)DUEL_PLAYERS[player])->cards[card % 30].id == id) {
        return 1;
    }
    return 0;
}

s32 func_801E05CC(s32 id, s32 player) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (func_801E0558(id, player, ((Player *)DUEL_PLAYERS[player])->hand[i])) {
            return i + 1;
        }
    }
    return 0;
}

s32 func_801E0650(s32 id, s32 player) {
    s32 i;
    Player *p;
    s8 card;

    for (i = 0; i < 4; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 2 && p->cards[card % 30].index == id) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E0708);

s32 func_801E0868(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < 5; i++) {
        if (func_801E0558(D_801FB9D8[i], player, card)) {
            return 1;
        }
    }
    return 0;
}

s32 func_801E08E4(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 5; i++) {
        found = func_801E05CC(D_801FB9D8[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 func_801E094C(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < 24; i++) {
        if (func_801E0558(D_801FB9A8[i], player, card)) {
            if (D_801FB9A8[i] != 0x97) {
                return 1;
            }
            if (countOfflineDeckCards(player) >= 8) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801E09FC(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 24; i++) {
        found = func_801E05CC(D_801FB9A8[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 func_801E0A64(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 4; i++) {
        found = func_801E05CC(D_801FBA28[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 func_801E0ACC(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < 33; i++) {
        if (func_801E0558(D_801FB9E4[i], player, card)) {
            return 1;
        }
    }
    return 0;
}

s32 func_801E0B48(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 33; i++) {
        found = func_801E05CC(D_801FB9E4[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 func_801E0BB0(s32 player) {
    Player *p;

    if (getActiveDigimonCard(player) == -1) {
        return 0;
    }
    p = (Player *)DUEL_PLAYERS[player];
    return p->cards[getActiveDigimonCard(player) % 30].card[0xE4];
}

s32 func_801E0C50(s32 player, s32 card) {
    if (card == -1) {
        return -1;
    }
    if (((Player *)DUEL_PLAYERS[player])->cards[card % 30].card[2] == 2) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E0CCC);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E157C);

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DDF38);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E19EC);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E201C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E2100);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E2A50);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E2C98);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E2ED8);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E30D0);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E3364);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E3574);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E363C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E3AF8);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E3C00);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E3FF4);

s32 func_801E4E08(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (((DuelK *)D_801D8340)->slots[i].id != -1) {
            return ((DuelK *)D_801D8340)->slots[i].id;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E4E58);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E5710);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E6424);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E6AA4);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E7DD4);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E81DC);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E9700);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E9ABC);

s32 func_801E9BAC(s32 a, s32 op, s32 b) {
    switch (op) {
    case 0:
        return a < b;
    case 1:
        return a <= b;
    case 2:
        return a > b;
    case 3:
        return a >= b;
    case 4:
        return a != b;
    case 5:
        return a == b;
    }
    return 0;
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E9C1C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801E9F5C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EA374);

s32 func_801EA558(s32 card, s32 player) {
    Player *p = (Player *)DUEL_PLAYERS[player];

    p->statPenalty = ((u8 *)p->cards[card % 30].card)[0x1A];
    ((Player *)DUEL_PLAYERS[player])->unk110 &= ~0x40000000;
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EA5F4);

void func_801EA708(void) {
    DUEL->tutorial = 1;
    *(ScriptRunner **)D_801D8340 = allocTaskHeapBlock(sizeof(ScriptRunner));
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\BETA.MSD", getCurrentTaskId());
    (*(ScriptRunner **)D_801D8340)->data = (void *)func_80014C08(0x7FFFFFFF);
    (*(ScriptRunner **)D_801D8340)->script = createScriptContext((*(ScriptRunner **)D_801D8340)->data);
    (*(ScriptRunner **)D_801D8340)->regs = allocScriptRegisters(10);
    (*(ScriptRunner **)D_801D8340)->unkC = 0;
    func_801EAB4C();
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EA7E8);

void func_801EA868(UiWindow *window) {
    drawText(window->originX, window->originY, *(s32 *)(*(u8 **)D_801D8340 + 0x10), 7, window->z);
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EA8B4);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EAB4C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EB32C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EB53C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EBACC);

s32 func_801EC4CC(s32 player) {
    D_801D83D1 = (*(u32 *)(DUEL_PLAYERS[player] + 0x178) >> 17) & 3;
    D_801D83EC[player * 0xD8 + 0xD] = player + 1;
}

s32 func_801EC528(s32 index) {
    DUEL->cursorSlot = -1;
    DUEL->unk81D = -1;
    D_801D83EC[index * 0xD8 + 0xD] = 5;
}

s32 func_801EC570(s32 player) {
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
    *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
    return card;
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EC608);

s32 func_801EC704(s32 player) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (((Player *)DUEL_PLAYERS[player])->hand[i] != -1) {
            SPRITE_KIND(((Player *)DUEL_PLAYERS[player])->hand[i]) = 8;
            discardCardToOfflineDeck(((Player *)DUEL_PLAYERS[player])->hand[i], player);
            removeCardFromHand(((Player *)DUEL_PLAYERS[player])->hand[i], player);
        }
    }
}

s32 func_801EC7C0(s32 card, s32 player) {
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

s32 func_801EC84C(s32 card, s32 player, s32 slot) {
    if (removeCardFromDigimonStack(card, player) != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
    }
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EC8E0);

s32 func_801EC9A4(s32 player, s32 slot) {
    s32 card;

    card = takeOfflineDeckTopCard(player);
    if (card != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
    }
}

s32 func_801ECA30(s32 card, s32 player, s32 slot) {
    if (removeCardFromDpSlots(card, player) != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
    }
}

s32 func_801ECAC4(s32 player) {
    s32 card;

    if (countOnlineDeckCards(player) && isPlayedCardSlotEmpty(player)) {
        card = drawOnlineDeckCard(player);
        setPlayedCard(card, player);
        SPRITE_KIND(card) = 21;
    }
}

s32 func_801ECB40(s32 card, s32 player) {
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

s32 func_801ECBCC(s32 card, s32 player) {
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

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801ECC58);

s32 func_801ECD68(void) {
    if (DUEL->unk80C >= 0) {
        func_801EC9A4(DUEL->turnPlayer, DUEL->unk80C);
        DUEL->unk80C = -1;
    }
    waitDuelFrames(30);
    if (DUEL->unk80E >= 0) {
        func_801ECA30(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer, DUEL->unk80E);
        DUEL->unk80E = -1;
    }
    waitDuelFrames(30);
    DUEL->step = 11;
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801ECE24);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801ECF0C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801ED334);

void func_801ED608(void) {
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (!(PAD_STATES[0]->pressed & 0x40));
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801ED65C);

void func_801ED8BC(s32 x, s32 y, char *name) {
    char buf[64];

    sprintf(buf, "%s Deck", name);
    drawText(x + 0x18, y + 3, (s32)buf, 7, 1);
    func_801ED65C(x, y, 0x1D0, 0xCA, 0xC0, 0x12, 0x190, 0xF9, 0, 0, 0, 0x80, 1);
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801ED968);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EDA84);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EDD88);

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DE268);

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DE274);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EE170);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EFB78);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EFCEC);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801EFEFC);

void func_801EFF98(void) {
    drawWindow((UiWindow *)(D_801FC454 + 0x4), func_801EDD88, 10);
    drawWindow((UiWindow *)(D_801FC454 + 0x134), func_801EFB78, 10);
    if (*(s16 *)(D_801FC454 + 0x770) == 0) {
        drawWindow((UiWindow *)(D_801FC454 + 0x4C), func_801EDD88, 10);
        drawWindow((UiWindow *)(D_801FC454 + 0x17C), func_801EFB78, 10);
    }
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F003C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F0A30);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F1AA8);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F2A40);

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DE41C);

void func_801F3BAC(UiWindow *window) {
    drawText(window->originX + 2, window->originY + 1, (s32)"Earned Experience Points", 7, 0);
}

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DF404);

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DF414);

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DF420);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F3BE8);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F4174);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F4408);

void func_801F4794(RankUpWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    char buf[24];

    x = w->window.originX;
    y = w->window.originY;
    z = w->window.z;
    sprintf(buf, "%2d RANK UP!", w->rank);
    drawLargeText(x + 1, y + 1, (s32)buf, 7, z);
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F47FC);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F48E0);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F4A24);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F54BC);

void func_801F5B94(RewardWindow *w) {
    s32 x;
    s32 y;
    s32 z;

    x = w->window.originX;
    y = w->window.originY;
    z = w->window.z;
    if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[w->index] < 0) {
        drawLargeText(x + 1, y + 1, (s32)"FULL SET!", 7, z);
    } else {
        drawLargeText(x + 1, y + 1, (s32)"RECEIVED!", 7, z);
    }
}

void func_801F5C14(UiWindow *window) {
    drawText(window->originX + 2, window->originY + 1, (s32)"Earned a Prize Pack", 7, 0);
    drawText(window->originX + 0x92, window->originY + 1, (s32)D_8006E31C[((u8 *)D_8006E054)[0x73]], 6, 0);
}

void func_801F5C9C(void) {
    s32 i;

    drawWindow(&D_801FC73C->window, func_801F5C14, 0);
    for (i = 0; i < 3; i++) {
        if (D_801FC73C->showRewards) {
            drawWindow(&D_801FC73C->rewards[i].window, func_801F5B94, 0);
        }
        drawWindow(&D_801FC73C->prizes[i].window, func_801F54BC, 0);
    }
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F5D58);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F5E50);

void func_801F6170(void) {
    func_800149B8(0, -1, 0, 0x800, loadFileTagged, "B:\\CBTL_EFF.ARC", getCurrentTaskId(), 0x38E);
    *(s32 *)((u8 *)D_801D8340 + 0x4C) = func_80014C08(0x7FFFFFFF);
}

void func_801F61E4(void) {
    freeHeapBlock(*(void **)((u8 *)D_801D8340 + 0x4C));
}

s32 func_801F6214(s32 entry, s32 player) {
    func_801F6294(entry, player, player, 0, 0);
}

void func_801F623C(s32 entry, s32 player, s32 mode) {
    func_801F6294(entry, player, player, mode, mode);
}

s32 func_801F6268(s32 entry, s32 player) {
    func_801F6294(entry, player, player ^ 1, 0, 0);
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F6294);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F63AC);

void func_801F64D8(EffectTable *table) {
    s32 i;

    func_80014C08(FRAME_INTERVAL);
    for (i = 0; i < 16; i++) {
        if (table->entries[i].kind != -1) {
            D_801FC158[table->entries[i].kind](table->entries[i].value);
        }
    }
}

void func_801F6578(s32 index, u8 *fx) {
    CardAnim *anim;
    s32 base;

    if (index >= 0) {
        base = (s32)D_801D833C;
        anim = (CardAnim *)(index * 36 + base);
        *(s32 *)(fx + 0x248) = anim->spr->pos.vx;
        *(s32 *)(fx + 0x24C) = anim->spr->pos.vy;
        *(s32 *)(fx + 0x250) = anim->spr->pos.vz;
    }
}

void func_801F65D8(s32 index, u8 *fx) {
    u8 rgb[3];

    if (index >= 0) {
        rgb[0] = *(s32 *)(fx + 0x94);
        rgb[1] = *(s32 *)(fx + 0x98);
        rgb[2] = *(s32 *)(fx + 0x9C);
        func_801F893C(*(CardSprite **)(D_801D833C + index * 36), rgb);
    }
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F663C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F6D38);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F6F44);

void func_801F70DC(void *xform, u8 *fx) {
    SVECTOR pos;

    getTransformWorldPos(xform, &pos);
    *(s32 *)(fx + 0x248) = pos.vx;
    *(s32 *)(fx + 0x24C) = pos.vy;
    *(s32 *)(fx + 0x250) = pos.vz;
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F7128);

void func_801F71A8(u8 *fx) {
    Rect16 rect;
    u8 rgb[3];
    u8 rgb2[3];

    rect.x = *(s32 *)(fx + 0x128);
    rect.y = *(s32 *)(fx + 0x12C);
    rect.w = *(s32 *)(fx + 0x130);
    rect.h = *(s32 *)(fx + 0x134);
    rgb[0] = *(s32 *)(fx + 0x94);
    rgb[1] = *(s32 *)(fx + 0x98);
    rgb[2] = *(s32 *)(fx + 0x9C);
    rgb2[0] = *(s32 *)(fx + 0xA0);
    rgb2[1] = *(s32 *)(fx + 0xA4);
    rgb2[2] = *(s32 *)(fx + 0xA8);
    func_801E6424(&rect, rgb, rgb2, *(s32 *)(fx + 0x6C), *(s16 *)(fx + 0x70), *(s32 *)(fx + 0x110));
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F7264);

void func_801F73C8(s32 arg0, s32 arg1) {
    EffectTemplate template;

    func_801F7128(&template, arg0, arg1);
    cloneEffectObject(&template);
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F73FC);

void func_801F7530(void *ptr) {
    freeHeapBlock(ptr);
}

void func_801F7550(s32 index, s32 kind, s32 arg2, EffectTable *table) {
    if (D_801FC148[kind] != NULL) {
        table->entries[index].kind = kind;
        table->entries[index].unk2 = 0;
        table->entries[index].value = D_801FC148[kind](arg2, table);
        if ((++table->count & 0xF) == 0) {
            func_80014C08(FRAME_INTERVAL);
        }
    }
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F75E4);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F7760);

void func_801F77E0(void) {
    drawWindow(&D_801FC884, func_801F7A64, 0);
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F7810);

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DF604);

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DFB54);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F7A64);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F7B2C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F8200);

s32 func_801F848C(void) {
    freeHeapBlock(*(void **)((u8 *)D_801D8340 + 0x48));
    freeHeapBlock(D_801D83EC);
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F84CC);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F851C);

s32 func_801F8854(void) {
    s32 i;

    D_801D83F8 = allocTaskHeapBlock(sizeof(Unk14F0) * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[10] = (s32)&D_801D83F8[i];
    }
    allocTaskHeapBlock(0xE10);
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F88E8);

void func_801F8910(CardSprite *sprite, u8 num) {
    sprite->flags |= 0x20;
    sprite->num = num;
}

void func_801F8928(CardSprite *sprite) {
    sprite->flags &= ~0x20;
}

void func_801F893C(CardSprite *sprite, u8 *to) {
    sprite->flags |= 0x40;
    sprite->t = 0;
    sprite->from[0] = sprite->fade[0];
    sprite->from[1] = sprite->fade[1];
    sprite->from[2] = sprite->fade[2];
    sprite->to[0] = to[0];
    sprite->to[1] = to[1];
    sprite->to[2] = to[2];
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F8998);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F8C58);

void func_801F8DB4(void *ptr) {
    if (ptr != NULL) {
        freeHeapBlock(D_801D83F4);
        freeHeapBlock(ptr);
    }
}

void func_801F8DF0(s16 *arg0, s16 x, s16 y) {
    arg0[4] = x;
    arg0[5] = y;
    func_801F8E14(arg0);
}

void func_801F8E14(void *arg0) {
    func_801F8E34(arg0, 1);
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F8E34);

void func_801F96F0(void) {
    s32 i;
    u8 *duel;

    *(Unk788 **)((u8 *)D_801D8340 + 0x4) = allocTaskHeapBlock(sizeof(Unk788) * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[13] = (s32)&(*(Unk788 **)((u8 *)D_801D8340 + 0x4))[i];
    }
    duel = D_801D8340;
    *(s32 *)(duel + 0x828) = -1;
    *(s32 *)(duel + 0x834) = 0x140;
}

void func_801F9794(void) {
    freeHeapBlock(*(void **)((u8 *)D_801D8340 + 0x4));
}

void func_801F97C4(s32 arg0, s32 arg1, s32 arg2) {
    u8 *duel = D_801D8340;

    *(s32 *)(duel + 0x828) = 1;
    *(s32 *)(duel + 0x82C) = arg0;
    *(s32 *)(duel + 0x830) = arg1;
    *(s32 *)(duel + 0x838) = arg2;
}

void func_801F97E4(void) {
    *(s32 *)((u8 *)D_801D8340 + 0x828) = 0;
}

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F97F4);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801F9EAC);

void func_801FA290(void) {
    s32 i;
    u8 *flags;
    ProfileK *profile;

    i = 0;
    flags = (u8 *)D_801D8340 + 0x840;
    profile = (ProfileK *)PLAYER_PROFILES;
    for (; i < 32; i++) {
        if (flags[i] != 0) {
            if (++profile->counts[i] >= 1000) {
                profile->counts[i] = 999;
            }
        }
    }
}

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DFBBC);

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DFBC8);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801FA30C);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801FA4E4);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801FA780);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801FA918);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801FAA54);

INCLUDE_ASM("asm/kawseg/nonmatchings/kawseg", func_801FB444);
