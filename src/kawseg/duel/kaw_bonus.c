#include "common.h"
#include "game.h"
#include "dcb/kaw_bonus.h"
#include "dcb/card_zones.h"
#include "dcb/text.h"
#include "dcb/card_db.h"
#include "dcb/prim_util.h"
#include "dcb/kawseg.h"

typedef struct {
    u32 f0 : 1;
    u32 f1 : 1;
    u32 f2 : 1;
    u32 f3 : 1;
    u32 f4 : 1;
    u32 f5 : 1;
    u32 f6 : 1;
    u32 f7 : 1;
    u32 f8 : 1;
    u32 f9 : 1;
    u32 f10 : 1;
    u32 f11 : 1;
    u32 count : 2;
    u32 f14 : 1;
    u32 f15 : 1;
    u32 f16 : 1;
    u32 f17 : 1;
    u32 f18 : 1;
    u32 f19 : 2;
    u32 f21 : 1;
    u32 f22 : 1;
    u32 f23 : 1;
    u32 f24 : 1;
    u32 f25 : 1;
    u32 f26 : 1;
    u32 f27 : 1;
    u32 f28 : 1;
    u32 f29 : 1;
    u32 f30 : 1;
    u32 f31 : 1;
} Flags110;

typedef struct {
    /* 0x0 */ u8 id;
    /* 0x1 */ u8 bonus;
    /* 0x4 */ char *name;
} BonusEntry;

typedef struct {
    u8 unk0[8];
    PolyF4 banner[2];
    DR_MODE bannerMode[2];
} DuelBanner;

extern s32 KAW_BONUS_ROW;

/* the bonuses a duel can give, with their experience */
BonusEntry KAW_BONUSES[32] = {
    { 0, 3, "All *b0 Attack Win" },
    { 1, 3, "All *b1 Attack Win" },
    { 2, 3, "All *b2 Attack Win" },
    { 3, 5, "All or Nothing Gamble Win" },
    { 4, 2, "Last Chance Gamble Win" },
    { 5, 5, "No Support Card Win" },
    { 6, 3, "No Digivolve Win" },
    { 7, 1, "No Discard Win" },
    { 8, 5, "4-of-a-Kind Win" },
    { 9, 2, "0 Online Card Left win" },
    { 10, 1, "Partner Win" },
    { 11, 3, "No Loss Win" },
    { 12, 3, "Come-Back Win" },
    { 13, 7, "Desperate Win" },
    { 14, 2, "All Gone Win" },
    { 15, 3, "Ultimate Level Win" },
    { 16, 5, "Option Maniac Win" },
    { 17, 8, "8 DP Cards Win" },
    { 18, 1, "Lucky Seven Win" },
    { 19, 3, "Just Enough Attack Win" },
    { 20, 10, "12 S-Jewel Cards Win" },
    { 21, 2, "Choked Loss" },
    { 22, 2, "Loss by Gamble" },
    { 23, 1, "Total Loss" },
    { 24, 2, "Rainbow" },
    { 25, 10, "Damage Fever" },
    { 26, 7, "HP Fever" },
    { 27, 2, "3 Partners" },
    { 28, 2, "3 Partners Plus" },
    { 29, 2, "Partner Normal Digivolve" },
    { 30, 1, "Lucky Name" },
    { 31, 10, "Super Bonus" },
};

#define FLAGS110(p) ((Flags110 *)&PLAYER(p)->bonusFlags)

void KAW_resetBonusFlags(s32 player) {
    s32 count;
    s32 id;
    s32 i;

    FLAGS110(player)->f0 = 0;
    FLAGS110(player)->f1 = 0;
    FLAGS110(player)->f2 = 0;
    FLAGS110(player)->f3 = 0;
    FLAGS110(player)->f4 = 0;
    FLAGS110(player)->f5 = 0;
    FLAGS110(player)->f6 = 0;
    FLAGS110(player)->f7 = 0;
    FLAGS110(player)->f8 = 0;
    FLAGS110(player)->f9 = 0;
    FLAGS110(player)->f10 = 0;
    FLAGS110(player)->f11 = 0;
    FLAGS110(player)->count = 0;
    FLAGS110(player)->f14 = 0;
    FLAGS110(player)->f15 = 0;
    FLAGS110(player)->f16 = 0;
    FLAGS110(player)->f19 = 0;
    FLAGS110(player)->f21 = 0;
    FLAGS110(player)->f22 = 0;
    FLAGS110(player)->f23 = 0;
    FLAGS110(player)->f24 = 0;
    FLAGS110(player)->f25 = 0;
    FLAGS110(player)->f26 = 0;
    FLAGS110(player)->f27 = 0;
    FLAGS110(player)->f17 = 0;
    FLAGS110(player)->f18 = 0;
    FLAGS110(player)->f29 = 0;
    FLAGS110(player)->f30 = 0;
    count = 0;
    for (id = 0x111; id < 0x11D; id++) {
        for (i = 0; i < 30; i++) {
            if (PLAYER(player)->cards[i].id == id) {
                count++;
                break;
            }
        }
    }
    if (count == 12) {
        FLAGS110(player)->f28 = 1;
    } else {
        FLAGS110(player)->f28 = 0;
    }
    for (i = 0; i < 32; i++) {
        KAW_DUEL->bonusFlags[i] = 0;
    }
}

/* the same count; the match depends on the form: each version's compiler
   needs its own to give the original's registers */
#if VERSION_US
void KAW_countEarnedBonuses(void) {
    s32 i;
    u8 *flags;
    ProfileK *profile;

    i = 0;
    flags = KAW_DUEL->bonusFlags;
    profile = (ProfileK *)PLAYER_PROFILES;
    for (; i < 32; i++) {
        if (flags[i] != 0) {
            if (++profile->counts[i] >= 1000) {
                profile->counts[i] = 999;
            }
        }
    }
}
#elif VERSION_EU
void KAW_countEarnedBonuses(void) {
    s32 i;
    DuelK *duel;
    ProfileK *profile;

    i = 0;
    duel = KAW_DUEL;
    profile = (ProfileK *)PLAYER_PROFILES;
    for (; i < 32; i++) {
        if (duel->bonusFlags[i] != 0) {
            if (++profile->counts[i] >= 1000) {
                profile->counts[i] = 999;
            }
        }
    }
}
#else
#error "kawseg/duel/kaw_bonus: version not checked"
#endif

void KAW_trackSpecialties(s32 player) {
    Player *p;
    u32 flags;

    if (getActiveDigimonCard(player) == -1) {
        return;
    }
    p = (Player *)DUEL_PLAYERS[player];
    if (((u32)p->bonusFlags >> 15) & 1) {
        return;
    }
    switch (p->specialty) {
    case 0:
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x800000;
        break;
    case 1:
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x1000000;
        break;
    case 2:
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x2000000;
        break;
    case 3:
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x4000000;
        break;
    case 4:
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x8000000;
        break;
    }
    flags = ((Player *)DUEL_PLAYERS[player])->bonusFlags;
    if (((flags >> 23) & 1) + ((flags >> 24) & 1) + ((flags >> 25) & 1) + ((flags >> 26) & 1) + ((flags >> 27) & 1) == 5) {
        KAW_showBonusBanner(player, 0x18);
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x8000;
    }
}

s32 KAW_checkDigimonBonuses(s32 player) {
    s32 card;
    s16 hp;

    card = getActiveDigimonCard(player);
    if (card == -1) {
        return;
    }
    hp = PLAYER(player)->stats[0];
    if (hp != 0 && hp % 1110 == 0) {
        KAW_showBonusBanner(player, 0x1A);
        PLAYER(0)->bonusFlags |= 0x400;
    }
    if (findPartnerSlot(player, PLAYER(player)->cards[card % 30].id) >= 0) {
        KAW_showBonusBanner(player, 0x1C);
        ((Flags110 *)&PLAYER(player)->bonusFlags)->count++;
    } else if (findArmorPartnerSlot(player, PLAYER(player)->cards[card % 30].id) >= 0) {
        KAW_showBonusBanner(player, 0x1C);
        ((Flags110 *)&PLAYER(player)->bonusFlags)->count++;
    }
    if (!(((u32)PLAYER(player)->bonusFlags >> 30) & 1) && PLAYER(player)->digimonStack[0] >= 0 &&
        findPartnerSlot(player, PLAYER(player)->cards[PLAYER(player)->digimonStack[2] % 30].id) >= 0) {
        KAW_showBonusBanner(player, 0x1D);
        PLAYER(player)->bonusFlags |= 0x20000000;
    }
    KAW_trackSpecialties(player);
}

s32 KAW_checkHandBonuses(s32 player) {
    s32 ids[4];
    s32 same;
    s32 partners;
    s32 i;
    s32 card;

    same = 0;
    partners = 0;
    for (i = 0; i < 4; i++) {
        card = ((Player *)DUEL_PLAYERS[player])->hand[i];
        if (card == -1) {
            ids[i] = card;
            continue;
        }
        ids[i] = ((Player *)DUEL_PLAYERS[player])->cards[card % 30].id;
        if (ids[0] == ids[i]) {
            same++;
        }
        if (findPartnerSlot(player, ids[i]) >= 0) {
            partners++;
        }
    }
    if (same == 4) {
        KAW_showBonusBanner(player, 8);
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x40;
    }
    if (partners == 3) {
        KAW_showBonusBanner(player, 0x1B);
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x800;
    }
}

s32 KAW_addBonusLine(BonusEntry *entry, s32 x, s32 y, s32 last, s32 z) {
    char buf[72];

    KAW_DUEL->bonusFlags[entry->id] = 1;
    drawText(x + 6, y + KAW_BONUS_ROW * 13, (s32)entry->name, 7, z);
    sprintf(buf, "*s0+%3d*c7(%3d)", entry->bonus, ((ProfileK *)PLAYER_PROFILES)->counts[entry->id] + 1);
    drawText(x + 0xA2, y + KAW_BONUS_ROW * 13, (s32)buf, 5, z);
    KAW_BONUS_ROW++;
    KAW_BONUS_EXP += entry->bonus;
    return KAW_BONUS_ROW == last;
}

s32 KAW_drawBonuses(s32 x, s32 y, s32 count, s32 z, s32 exp) {
    BonusEntry *entry;
    s32 i;
    char buf[72];

    entry = KAW_BONUSES;
    if (count == 0) {
        return 0;
    }
    KAW_BONUS_EXP = exp;
    KAW_BONUS_ROW = 0;
    if (DUEL->winner == 0) {
        if (!(FLAGS110(0)->f1 | FLAGS110(0)->f2) && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!(FLAGS110(0)->f0 | FLAGS110(0)->f2) && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!(FLAGS110(0)->f0 | FLAGS110(0)->f1) && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f16 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (KAW_DUEL->bonusFlags[3] == 0 && FLAGS110(0)->f17 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f4 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f3 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f5 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f6 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countOnlineDeckCards(0) > 0 && countOnlineDeckCards(1) == 0 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f14 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (PLAYER(1)->wins == 0 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f7 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (PLAYER(0)->wins == 3 && PLAYER(1)->wins == 2
            && countOnlineDeckCards(0) + countOnlineDeckCards(1) - countEmptyHandSlots(0) + 8 == countEmptyHandSlots(1)
            && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (KAW_DUEL->bonusFlags[13] == 0 && countOnlineDeckCards(0) + 4 == countEmptyHandSlots(0)
            && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f19 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countDeckCardsByFilter(0, (PlayerDeck *)PLAYER(0), 0xC0) >= 25 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countEmptyDpSlots(0) == 0 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countOnlineDeckCards(0) == 7 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f22 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f28 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry += 4;
    } else {
        entry += 21;
        if (FLAGS110(0)->f8 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f18 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (PLAYER(0)->wins == 0 && PLAYER(1)->wins == 3 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
    }
    if (FLAGS110(0)->f15 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f9 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f10 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f11 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->count == 3 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f29 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry += 2;
    if (KAW_BONUS_ROW >= 7 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    i = 46;
    do {
        drawText(x + i * 5, y + KAW_BONUS_ROW * 13, (s32)"-", 7, z);
    } while (--i >= 0);
    sprintf(buf, "+%3d", KAW_BONUS_EXP);
    drawText(x + 0xA2, y + KAW_BONUS_ROW * 13 + 11, (s32)buf, 5, z);
    return 1;
}

#define BANNER ((DuelBanner *)DUEL_STATE)

void KAW_showBonusBanner(s32 player, s32 id) {
    s32 show;
    s32 x;
    s32 y;
    s32 h;
    s32 frame;

    if (PLAYER(1)->controller != 1 || player != 0) {
        return;
    }
    show = 0;
    switch (id) {
    case 24:
        if (!FLAGS110(player)->f15) {
            show = 1;
        }
        break;
    case 28:
        if (FLAGS110(player)->count == 2) {
            show = 1;
        }
        break;
    case 29:
        if (!FLAGS110(player)->f29) {
            show = 1;
        }
        break;
    case 10:
        if (!FLAGS110(player)->f14) {
            show = 1;
        }
        break;
    case 27:
        if (!FLAGS110(player)->f11) {
            show = 1;
        }
        break;
    case 8:
        if (!FLAGS110(player)->f6) {
            show = 1;
        }
        break;
    case 15:
        show = 1;
        FLAGS110(player)->f19 = 1;
        break;
    case 19:
        show = 1;
        FLAGS110(player)->f22 = 1;
        break;
    case 25:
        show = 1;
        FLAGS110(player)->f9 = 1;
        break;
    case 26:
        show = 1;
        FLAGS110(player)->f10 = 1;
        break;
    }
    if (show == 0) {
        return;
    }
    x = -200;
    y = 0x6E;
    h = 0;
    frame = 0;
    do {
        waitFrames(FRAME_INTERVAL);
        if (frame < 100) {
            x += 16;
            if (x > 160) {
                x = 160;
            }
            h++;
            if (h >= 9) {
                h = 8;
            }
        } else {
            x += 16;
            if (x > 0x208) {
                x = 0x208;
            }
            h--;
            if (h < 0) {
                h = 0;
            }
        }
        drawText(x - measureText(KAW_BONUSES[id].name) / 2, y - 6, (s32)KAW_BONUSES[id].name, 7, 0);
        SetDrawTPage(&BANNER->bannerMode[FRAME_BUFFER_INDEX], 0, 0, GetTPage(0, 2, 0, 0));
        initPrimByType(8, &BANNER->banner[FRAME_BUFFER_INDEX], 1, 0);
        setPrimRgb0(&BANNER->banner[FRAME_BUFFER_INDEX], 0xC0, 0xC0, 0xC0);
        SetSemiTrans(&BANNER->banner[FRAME_BUFFER_INDEX], 1);
        (&BANNER->banner[FRAME_BUFFER_INDEX])->x0 = 0;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->y0 = y - h;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->x1 = 0x140;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->y1 = y - h;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->x2 = 0;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->y2 = y - h + h * 2;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->x3 = 0x140;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->y3 = y - h + h * 2;
        AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)&BANNER->banner[FRAME_BUFFER_INDEX]);
        AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)&BANNER->bannerMode[FRAME_BUFFER_INDEX]);
        frame++;
    } while (frame < 120);
}
