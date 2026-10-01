#include "common.h"
#include "game.h"
#include "dcb/sai_flags.h"
#include "dcb/card_db.h"
#include "dcb/saiseg.h"

typedef struct {
    u8 pad[0x1020];
    u16 armorFlags;
} SaisegSessionData;

extern u8 SAI_OPPONENT_COUNT;

void SAI_setPartnerObtainedFlag(s32 index) {
    s16 flagIds[6] = { 0x126, 0x12A, 0x12D, 0x133, 0x130, 0x136 };

    SAI_SCRIPT[0]->regs[flagIds[index]] = 1;
}

void SAI_copyScriptFlagsForFusion(void) {
    if (SAI_SCRIPT[0]->regs[266] != 0) {
        ((PlayerProfile *)PLAYER_PROFILES)->scriptFlags |= 1;
    }
    if (SAI_SCRIPT[0]->regs[267] != 0) {
        ((PlayerProfile *)PLAYER_PROFILES)->scriptFlags |= 2;
    }
}

void SAI_saveScriptFlags(void) {
    s32 flag;
    s32 i;
    s32 bit;

    flag = 12;
    for (i = 0; i < 12; i++) {
        for (bit = 0; bit < 32; bit++) {
            if (SAI_SCRIPT[0]->regs[flag++] != 0) {
                ((PlayerProfile *)PLAYER_PROFILES)->areaScriptFlags[i] |= 1 << bit;
            } else {
                ((PlayerProfile *)PLAYER_PROFILES)->areaScriptFlags[i] &= ~(1 << bit);
            }
            if (flag >= 0x16B) {
                break;
            }
        }
    }
    for (flag = 0x16B, bit = 0; bit < 9 && flag < 0x175; bit++, flag++) {
        ((PlayerProfile *)PLAYER_PROFILES)->areaScriptValues[bit] = SAI_SCRIPT[0]->regs[flag];
    }
}

void SAI_loadScriptFlags(void) {
    s32 i;
    s32 bit;
    s32 flag;

    flag = 12;
    for (i = 0; i < 12; i++) {
        for (bit = 0; bit < 32 && flag < 0x16B; bit++, flag++) {
            SAI_SCRIPT[0]->regs[flag] = ((u32)((PlayerProfile *)PLAYER_PROFILES)->areaScriptFlags[i] >> bit) & 1;
        }
    }
    for (flag = 0x16B, bit = 0; bit < 9 && flag < 0x174; bit++, flag++) {
        SAI_SCRIPT[0]->regs[flag] = ((PlayerProfile *)PLAYER_PROFILES)->areaScriptValues[bit];
    }
}

/* Appends an opponent to the select list; every six entries the last one
   moves to the next page behind the page arrows 0x11 (next) and 0x10 (back). */
void SAI_addOpponent(s32 opponent) {
    s8 last;

    if (SAI_OPPONENTS.count % 6 == 0 && SAI_OPPONENTS.count != 0) {
        last = SAI_OPPONENTS.ids[SAI_OPPONENTS.count - 1];
        SAI_OPPONENTS.ids[SAI_OPPONENTS.count - 1] = 0x11;
        SAI_OPPONENTS.ids[SAI_OPPONENTS.count++] = 0x10;
        SAI_OPPONENTS.ids[SAI_OPPONENTS.count++] = last;
        SAI_OPPONENTS.ids[SAI_OPPONENTS.count++] = opponent;
    } else {
        SAI_OPPONENTS.ids[SAI_OPPONENTS.count++] = opponent;
    }
}

void SAI_clearOpponents(void) {
    s32 i;

    SAI_OPPONENT_COUNT = 0;
    for (i = 0; i < 24; i++) {
        SAI_OPPONENTS.ids[i] = -1;
    }
}

void SAI_unlockArmorsFromFlags(s32 *regs) {
    s16 ids[13] = { 0x127, 0x128, 0x129, 0x12B, 0x12C, 0x12E, 0x12F, 0x134, 0x135, 0x131, 0x132, 0x137, 0x138 };
    s32 i;
    s32 partner;
    s32 slot;
    s32 first;

    ((SaisegSessionData *)SESSION_DATA)->armorFlags = 0;
    for (slot = 0; slot < 13; slot++) {
        if (regs[ids[slot]] != 0) {
            ((SaisegSessionData *)SESSION_DATA)->armorFlags |= 1 << slot;
        }
    }
    for (slot = 0; slot < 3; slot++) {
        partner = getSlotPartnerIndex(0, slot);
        if (partner == -1) {
            continue;
        }
        if (partner == 0) {
            for (i = 0; i < 3; i++) {
                if (regs[ids[i]] != 0) {
                    unlockPartnerArmor(0, partner, i);
                }
            }
        } else {
            for (i = 0, first = partner * 2 + 1; i < 2; i++) {
                if (regs[ids[i + first]] != 0) {
                    unlockPartnerArmor(0, partner, i);
                }
            }
        }
    }
}
