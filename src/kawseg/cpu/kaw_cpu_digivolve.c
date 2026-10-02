#include "common.h"
#include "game.h"
#include "dcb/card_zones.h"
#include "dcb/kawseg.h"

DigivolvePlan *KAW_selectDigivolvePlan(s32 kind) {
    s32 i;

    if (KAW_DUEL->selected == NULL) {
        switch (kind) {
        case 0:
            KAW_DUEL->selected = NULL;
            break;
        case 1:
            for (i = 0; i < 4; i++) {
                if (KAW_DUEL->slots[i].kind == 1) {
                    KAW_DUEL->selected = &KAW_DUEL->slots[i];
                }
            }
            break;
        case 2:
            for (i = 0; i < 4; i++) {
                if (KAW_DUEL->slots[i].kind == 2) {
                    KAW_DUEL->selected = &KAW_DUEL->slots[i];
                }
            }
            break;
        }
    }
    return KAW_DUEL->selected;
}

/* jp reads the target card's fields again for each check */
#define TARGET_FIELD(field) CARD_BYTE(PLAYER_CARDS(PLAYER(self))[sel % 30].card, field)

s32 KAW_chooseDigivolveTarget(s32 player) {
    s32 specialty;
    s32 level;
    s32 points;
    s32 sel;
    s32 self;
    s32 need;
#if VERSION_JP
    PlayerDeck *deck;
#elif VERSION_US || VERSION_EU
    Player *p;
    s8 *card;
    s32 cost;
    s32 target;
    s32 targetSpecialty;
#else
#error "untested version"
#endif

    /* a copy of player: the match depends on it, for the register allocation */
    self = player;
    if (getActiveDigimonCard(self) == -1) {
        return -1;
    }
    if (KAW_DUEL->selected == NULL) {
        return -1;
    }
    if (CARD_BYTE(PLAYER_CARDS(PLAYER(self))[KAW_DUEL->selected->card % 30].card, type) != 0) {
        return -1;
    }
    sel = KAW_DUEL->selected->card;
    if (getPlayedCard(player) == -1) {
        need = CARD_BYTE(PLAYER_CARDS(PLAYER(self))[sel % 30].card, dpCost);
        if (sumDigivolvePoints(self) < need) {
            return -1;
        }
    } else {
        specialty = PLAYER(self)->specialty;
        level = (u8)CARD_BYTE(PLAYER_CARDS(PLAYER(self))[getActiveDigimonCard(self) % 30].card, attr) & 0xF;
        points = sumDigivolvePoints(self);
#if VERSION_JP
        deck = PLAYER(self)->deck;
        switch (CARD_BYTE(deck->cards[getPlayedCard(self) % 30].card, attr)) {
        case 0:
            if (((u8)TARGET_FIELD(attr) & 0xF) != level + 1) {
                return -1;
            }
            if (TARGET_FIELD(dpCost) > points + 30) {
                return -1;
            }
            break;
        case 1:
            if (level != 0) {
                return -1;
            }
            if (level >= ((u8)TARGET_FIELD(attr) & 0xF)) {
                return -1;
            }
            if (((u8)TARGET_FIELD(attr) >> 4) != specialty) {
                return -1;
            }
            if (points < TARGET_FIELD(dpCost)) {
                return -1;
            }
            break;
        case 2:
            if (PLAYER(self)->statPenalty != 0) {
                return -1;
            }
            if (((u8)TARGET_FIELD(attr) & 0xF) != level + 1) {
                return -1;
            }
            if (((u8)TARGET_FIELD(attr) >> 4) != specialty) {
                return -1;
            }
            break;
        case 3:
            if (points < TARGET_FIELD(dpCost)) {
                return -1;
            }
            if (((u8)TARGET_FIELD(attr) & 0xF) != level) {
                return -1;
            }
            break;
        case 4:
        case 5:
            break;
        }
#elif VERSION_US || VERSION_EU
        p = PLAYER(self);
        card = PLAYER_CARDS(p)[sel % 30].card;
        targetSpecialty = (u8)card[0x1A] >> 4;
        target = (u8)card[0x1A] & 0xF;
        cost = card[0x1B];
        switch (CARD_BYTE(PLAYER_CARDS(p)[getPlayedCard(self) % 30].card, attr)) {
        case 0:
            if (level == 1) {
                return -1;
            }
            if (level == 0) {
                level = 1;
            }
            if (level + 1 != target) {
                return -1;
            }
            if (points + 20 < cost) {
                return -1;
            }
            break;
        case 1:
            if (level != 0) {
                return -1;
            }
            if (target != 3) {
                return -1;
            }
            if (targetSpecialty != specialty) {
                return -1;
            }
            if (points < cost) {
                return -1;
            }
            break;
        case 2:
            if (level == 1) {
                return -1;
            }
            if (level == 0) {
                level = 1;
            }
            if (PLAYER(self)->statPenalty != 0) {
                return -1;
            }
            if (level + 1 != target) {
                return -1;
            }
            if (targetSpecialty != specialty) {
                return -1;
            }
            break;
        case 3:
            if (points < cost) {
                return -1;
            }
            if (target != level) {
                return -1;
            }
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            break;
        }
#else
#error "untested version"
#endif
    }
    return KAW_DUEL->selected->card;
}

#undef TARGET_FIELD

#if VERSION_JP
void KAW_planDigivolveOptions(s32 player) {
    s32 minNeed[4];
    s32 i;
    s32 j;
    s32 k;
    s32 card;
    s32 option;
    s32 hand;
    s32 specialty;
    s32 level;
    s32 flag;
    s32 n; /* the target's level, then a slot index */

    for (i = 0; i < 4; i++) {
        card = KAW_DUEL->slots[i].card;
        if (card != -1 && PLAYER_CARDS(PLAYER(player))[card % 30].type == 0 && KAW_DUEL->slots[i].kind == 1 &&
            ((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF) == 2) {
            return;
        }
    }
    specialty = PLAYER(player)->specialty;
    level = (u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[getActiveDigimonCard(player) % 30].card, attr) & 0xF;
    for (i = 0; i < 4; i++) {
        option = KAW_DUEL->slots[i].card;
        if (option == -1 || PLAYER_CARDS(PLAYER(player))[option % 30].type != 2) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            card = KAW_DUEL->slots[j].card;
            if (card == -1 || PLAYER_CARDS(PLAYER(player))[card % 30].type != 0) {
                continue;
            }
            switch (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[option % 30].card, attr)) {
            case 0:
                if (((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF) != level + 1) {
                    break;
                }
                for (k = 0; k < 4; k++) {
                    minNeed[k] = 100;
                    if (k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER_CARDS(PLAYER(player))[hand % 30].type != 0) {
                        continue;
                    }
                    if (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[hand % 30].card, dpBonus) < KAW_DUEL->slots[j].need - 30) {
                        continue;
                    }
                    if (KAW_DUEL->slots[j].need - 30 < 0) {
                        minNeed[k] = 0;
                    } else {
                        minNeed[k] = KAW_DUEL->slots[j].need - 30;
                    }
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                for (k = 0; k < 4; k++) {
                    if (minNeed[k] < KAW_DUEL->slots[j].need) {
                        KAW_DUEL->slots[j].need = minNeed[k];
                    }
                }
                break;
            case 1:
                if (level != 0 || ((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF) != 2 ||
                    ((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) >> 4) != specialty) {
                    break;
                }
                for (k = 0; k < 4; k++) {
                    if (k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER_CARDS(PLAYER(player))[hand % 30].type != 0) {
                        continue;
                    }
                    if (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[hand % 30].card, dpBonus) < KAW_DUEL->slots[j].need) {
                        continue;
                    }
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            case 2:
                if (PLAYER(player)->statPenalty != 0 ||
                    ((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF) != level + 1 ||
                    ((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) >> 4) != specialty) {
                    break;
                }
                KAW_DUEL->slots[j].kind = 1;
                KAW_DUEL->slots[j].need = 0;
                KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                KAW_DUEL->selected = &KAW_DUEL->slots[j];
                break;
            case 3:
                flag = 0;
                n = (u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF;
                if (n != level || level == 0) {
                    break;
                }
                if (PLAYER(player)->statPenalty != 0) {
                    flag = 1;
                } else if (!PLAYER(player)->hasBattled) {
                    break;
                }
                /* without a stat penalty, only when no slot is planned yet
                   (k counts them) */
                if (flag == 0) {
                    k = 0;
                    for (n = 0; n < 4; n++) {
                        if (KAW_DUEL->slots[n].kind != 0) {
                            k++;
                        }
                    }
                    if (k != 0) {
                        break;
                    }
                }
                for (k = 0; k < 4; k++) {
                    if (k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER_CARDS(PLAYER(player))[hand % 30].type != 0) {
                        continue;
                    }
                    if (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[hand % 30].card, dpBonus) < KAW_DUEL->slots[j].need) {
                        continue;
                    }
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            case 5:
                if (level < ((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF)) {
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].need = 0;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            }
        }
    }
    if (KAW_DUEL->selected != NULL) {
        return;
    }
    for (i = 0; i < 4; i++) {
        option = KAW_DUEL->slots[i].card;
        if (option == -1 || PLAYER_CARDS(PLAYER(player))[option % 30].type != 2) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            if (i == j) {
                continue;
            }
            card = KAW_DUEL->slots[j].card;
            if (card == -1 || PLAYER_CARDS(PLAYER(player))[card % 30].type != 2) {
                continue;
            }
            if (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[option % 30].card, attr) == CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr)) {
                KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
            }
        }
    }
}
#elif VERSION_US || VERSION_EU
void KAW_planDigivolveOptions(s32 player) {
    s32 minNeed[4];
    s32 i;
    s32 j;
    s32 k;
    s32 card;
    s32 option;
    s32 hand;
    s32 specialty;
    s32 level;
    s32 target;
    s32 targetSpecialty;
    s32 flag;

    for (i = 0; i < 4; i++) {
        card = KAW_DUEL->slots[i].card;
        if (card != -1 && PLAYER_CARDS(PLAYER(player))[card % 30].type == 0 && KAW_DUEL->slots[i].kind == 1 &&
            ((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF) == 2) {
            return;
        }
    }
    for (i = 0; i < 4; i++) {
        option = KAW_DUEL->slots[i].card;
        if (option == -1 || PLAYER_CARDS(PLAYER(player))[option % 30].type != 2) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            card = KAW_DUEL->slots[j].card;
            if (card == -1 || PLAYER_CARDS(PLAYER(player))[card % 30].type != 0) {
                continue;
            }
            specialty = PLAYER(player)->specialty;
            level = (u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[getActiveDigimonCard(player) % 30].card, attr) & 0xF;
            targetSpecialty = (u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) >> 4;
            target = (u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF;
            switch (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[option % 30].card, attr)) {
            case 0:
                if (level == 1) {
                    break;
                }
                if (level == 0) {
                    level = 1;
                }
                if (level + 1 != target) {
                    break;
                }
                for (k = 0; k < 4; k++) {
                    minNeed[k] = 100;
                    if (KAW_DUEL->slots[j].need - 20 > 0 && k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER_CARDS(PLAYER(player))[hand % 30].type != 0) {
                        continue;
                    }
                    if (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[hand % 30].card, dpBonus) < KAW_DUEL->slots[j].need - 20) {
                        continue;
                    }
                    if (KAW_DUEL->slots[j].need - 20 < 0) {
                        minNeed[k] = 0;
                    } else {
                        minNeed[k] = KAW_DUEL->slots[j].need - 20;
                    }
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                for (k = 0; k < 4; k++) {
                    if (minNeed[k] < KAW_DUEL->slots[j].need) {
                        KAW_DUEL->slots[j].need = minNeed[k];
                    }
                }
                break;
            case 1:
                if (level != 0 || target != 3 || targetSpecialty != specialty) {
                    break;
                }
                for (k = 0; k < 4; k++) {
                    if (KAW_DUEL->slots[j].need != 0 && k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER_CARDS(PLAYER(player))[hand % 30].type != 0) {
                        continue;
                    }
                    if (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[hand % 30].card, dpBonus) < KAW_DUEL->slots[j].need) {
                        continue;
                    }
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            case 2:
                if (level == 1) {
                    break;
                }
                if (level == 0) {
                    level = 1;
                }
                if (PLAYER(player)->statPenalty != 0 || level + 1 != target || targetSpecialty != specialty) {
                    break;
                }
                KAW_DUEL->slots[j].kind = 1;
                KAW_DUEL->slots[j].need = 0;
                KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                KAW_DUEL->selected = &KAW_DUEL->slots[j];
                break;
            case 3:
                if (level != target) {
                    break;
                }
                flag = PLAYER(player)->displayedStats[0] < ((DigimonCardData *)PLAYER_CARDS(PLAYER(player))[card % 30].card)->hp;
                if (PLAYER(player)->statPenalty != 0) {
                    flag = 1;
                }
                if (!PLAYER(player)->hasBattled || flag != 1 || KAW_DUEL->selected != NULL) {
                    break;
                }
                for (k = 0; k < 4; k++) {
                    if (KAW_DUEL->slots[j].need != 0 && k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER_CARDS(PLAYER(player))[hand % 30].type != 0) {
                        continue;
                    }
                    if (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[hand % 30].card, dpBonus) < KAW_DUEL->slots[j].need) {
                        continue;
                    }
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            case 4:
                if (countEmptyDigimonStackSlots(player) < 2 && PLAYER(player)->displayedStats[0] < 300) {
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].need = 0;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            case 5:
                if (level < target) {
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].need = 0;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            case 6:
                if (level != 1 || targetSpecialty != specialty) {
                    break;
                }
                if ((target == 2 && PLAYER(player)->displayedStats[0] < 300) || target == 3) {
                    for (k = 0; k < 4; k++) {
                        if (KAW_DUEL->slots[j].need != 0 && k == j) {
                            continue;
                        }
                        hand = PLAYER(player)->hand[k];
                        if (hand == -1 || PLAYER_CARDS(PLAYER(player))[hand % 30].type != 0) {
                            continue;
                        }
                        if (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[hand % 30].card, dpBonus) < KAW_DUEL->slots[j].need) {
                            continue;
                        }
                        KAW_DUEL->slots[j].kind = 1;
                        KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                        KAW_DUEL->selected = &KAW_DUEL->slots[j];
                    }
                }
                break;
            case 7:
                if (level == 1 && PLAYER(player)->displayedStats[0] < 300) {
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].need = 0;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            }
        }
    }
    if (KAW_DUEL->selected != NULL) {
        return;
    }
    for (i = 0; i < 4; i++) {
        option = KAW_DUEL->slots[i].card;
        if (option == -1 || PLAYER_CARDS(PLAYER(player))[option % 30].type != 2) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            if (i == j) {
                continue;
            }
            card = KAW_DUEL->slots[j].card;
            if (card == -1 || PLAYER_CARDS(PLAYER(player))[card % 30].type != 2) {
                continue;
            }
            if (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[option % 30].card, attr) == CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr)) {
                KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
            }
        }
    }
}
#else
#error "untested version"
#endif

s32 KAW_chooseDigivolveOption(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (KAW_DUEL->slots[i].option != -1) {
            return KAW_DUEL->slots[i].option;
        }
    }
    return -1;
}
