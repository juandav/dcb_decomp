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

s32 KAW_chooseDigivolveTarget(s32 player) {
    Player *p;
    s8 *card;
    s32 specialty;
    s32 level;
    s32 points;
    s32 cost;
    s32 target;
    s32 targetSpecialty;
    s32 sel;
    s32 self;
    s32 need;

    /* a copy of player: it only changes the register allocation */
    self = player;
    if (getActiveDigimonCard(self) == -1) {
        return -1;
    }
    if (KAW_DUEL->selected == NULL) {
        return -1;
    }
    if (PLAYER(self)->cards[KAW_DUEL->selected->card % 30].card[2] != 0) {
        return -1;
    }
    sel = KAW_DUEL->selected->card;
    if (getPlayedCard(player) == -1) {
        need = PLAYER(self)->cards[sel % 30].card[0x1B];
        if (sumDigivolvePoints(self) < need) {
            return -1;
        }
    } else {
        specialty = PLAYER(self)->specialty;
        level = (u8)PLAYER(self)->cards[getActiveDigimonCard(self) % 30].card[0x1A] & 0xF;
        points = sumDigivolvePoints(self);
        p = PLAYER(self);
        card = p->cards[sel % 30].card;
        targetSpecialty = (u8)card[0x1A] >> 4;
        target = (u8)card[0x1A] & 0xF;
        cost = card[0x1B];
        switch (p->cards[getPlayedCard(self) % 30].card[0x1A]) {
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
    }
    return KAW_DUEL->selected->card;
}

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
        if (card != -1 && PLAYER(player)->cards[card % 30].type == 0 && KAW_DUEL->slots[i].kind == 1 &&
            ((u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF) == 2) {
            return;
        }
    }
    for (i = 0; i < 4; i++) {
        option = KAW_DUEL->slots[i].card;
        if (option == -1 || PLAYER(player)->cards[option % 30].type != 2) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            card = KAW_DUEL->slots[j].card;
            if (card == -1 || PLAYER(player)->cards[card % 30].type != 0) {
                continue;
            }
            specialty = PLAYER(player)->specialty;
            level = (u8)PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card[0x1A] & 0xF;
            targetSpecialty = (u8)PLAYER(player)->cards[card % 30].card[0x1A] >> 4;
            target = (u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF;
            switch (PLAYER(player)->cards[option % 30].card[0x1A]) {
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
                    if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                        continue;
                    }
                    if (PLAYER(player)->cards[hand % 30].card[0x1C] < KAW_DUEL->slots[j].need - 20) {
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
                    if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                        continue;
                    }
                    if (PLAYER(player)->cards[hand % 30].card[0x1C] < KAW_DUEL->slots[j].need) {
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
                flag = PLAYER(player)->displayedStats[0] < ((DigimonCardData *)PLAYER(player)->cards[card % 30].card)->hp;
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
                    if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                        continue;
                    }
                    if (PLAYER(player)->cards[hand % 30].card[0x1C] < KAW_DUEL->slots[j].need) {
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
                        if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                            continue;
                        }
                        if (PLAYER(player)->cards[hand % 30].card[0x1C] < KAW_DUEL->slots[j].need) {
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
        if (option == -1 || PLAYER(player)->cards[option % 30].type != 2) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            if (i == j) {
                continue;
            }
            card = KAW_DUEL->slots[j].card;
            if (card == -1 || PLAYER(player)->cards[card % 30].type != 2) {
                continue;
            }
            if (PLAYER(player)->cards[option % 30].card[0x1A] == PLAYER(player)->cards[card % 30].card[0x1A]) {
                KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
            }
        }
    }
}

s32 KAW_chooseDigivolveOption(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (KAW_DUEL->slots[i].option != -1) {
            return KAW_DUEL->slots[i].option;
        }
    }
    return -1;
}
