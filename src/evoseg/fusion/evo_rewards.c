#include "common.h"
#include "game.h"
#include "dcb/evo_rewards.h"
#include "dcb/card_db.h"
#include "dcb/window.h"
#include "dcb/fade.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/partner_level.h"
#include "dcb/sound_play.h"
#include "dcb/evoseg.h"
#include "dcb/evo_card_list.h"
#include "dcb/evo_trays.h"
#include "dcb/evo_effect.h"
#include "dcb/evo_data.h"

extern u8 EVO_LEVEL_UP_PENDING;
extern s32 EVO_NEW_DIGI_PART;

void EVO_findPartnerReward(void);
void EVO_addPartnerExp(void);

void EVO_startPartnerFusion(void) {
    PlayerProfile *profile;

    EVO_SCRIPT->vars[8] = -2;
    EVO_SCRIPT->vars[18] = 0;
    EVO_SCRIPT->vars[19] = 0;
    EVO_SCRIPT->vars[7] = 1;
    EVO_playEffect(0, 0);
    EVO_findPartnerReward();
    EVO_FUSION.step = 4;
    removeCardFromCollection(0, EVO_FUSION.secondCard, 1);
    EVO_SPARE_CARD_COUNTS[EVO_FUSION.secondCard]--;
    profile = (PlayerProfile *)PLAYER_PROFILES;
    profile->fusionCardsUsed++;
    if ((u16)profile->fusionCardsUsed >= 10000) {
        profile->fusionCardsUsed = 9999;
    }
}

void EVO_cancelPartnerFusion(void) {
    EVO_SCRIPT->vars[8] = -1;
    EVO_CARD_LIST_MENU.active = 1;
    EVO_FUSION.previewOpen = 0;
    animateWindowTo(&EVO_WINDOWS[12].win, (Rect16 *)-1);
    EVO_FUSION.step = 3;
}

void EVO_leaveForCutscene(void) {
    setScreenFadeParams(0, 2, 6);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 1, 6, 0);
    EVO_FUSION.cutscene = 1;
}

void EVO_findPartnerReward(void) {
    s32 i;
    s32 ability;

    EVO_FUSION.partnerKind = -1;
    ability = -1;
    for (i = 0; i < 6; i++) {
        if (EVO_PARTNER_CARD_IDS[i] == EVO_FUSION.firstCard && EVO_FUSION.partnerKind == -1) {
            EVO_FUSION.partnerKind = i;
        }
    }
    if (EVO_FUSION.partnerKind != -1) {
        for (i = 0; i < 5; i++) {
            if (EVO_PARTNER_FUSION_REWARDS[EVO_FUSION.partnerKind][i].card == EVO_FUSION.secondCard && ability == -1) {
                ability = EVO_PARTNER_FUSION_REWARDS[EVO_FUSION.partnerKind][i].ability;
                if (getPartnerAbilityState(0, ability) == 0) {
                    grantPartnerAbility(0, ability);
                    EVO_FUSION.rewardStep = 0;
                } else {
                    ability = -1;
                }
                EVO_FUSION.result = ability;
            }
        }
    }
    if (ability == -1) {
        EVO_FUSION.rewardStep = 3;
        EVO_SCRIPT->vars[19] = ability;
        EVO_FUSION.partnerKind = ability;
        EVO_FUSION.result = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->fusionPoints;
    }
}

void EVO_tickPartnerReward(void) {
    char text[168];
    s32 i;

    switch (EVO_FUSION.rewardStep) {
    case 0:
        EVO_SCRIPT->vars[18] = 2;
        break;
    case 1:
        sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c6[%s]", EVO_FUSION.result, EVO_DIGI_PARTS[EVO_FUSION.result].name);
        initDialog((u8 *)&EVO_DIALOG, text, 0x80);
        runDialog(&EVO_DIALOG);
        EVO_FUSION.rewardStep = 10;
        EVO_FUSION.step = 3;
        EVO_CARD_LIST_MENU.active = 1;
        EVO_FUSION.previewOpen = 0;
        for (i = 0; i < 4; i++) {
            EVO_STAT_BONUSES[i] = 0;
        }
        EVO_SCRIPT->vars[8] = -1;
        break;
    case 2:
        if (EVO_SCRIPT->vars[19] == -1) {
            EVO_addPartnerExp();
        } else if (EVO_FUSION.scriptState == 0) {
            if (EVO_LEVEL_UP_PENDING == 0) {
                if (EVO_NEW_DIGI_PART != -1) {
                    grantPartnerAbility(0, EVO_NEW_DIGI_PART);
                    sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c6[%s]", EVO_NEW_DIGI_PART, EVO_DIGI_PARTS[EVO_NEW_DIGI_PART].name);
                    initDialog((u8 *)&EVO_DIALOG, text, 0x80);
                    runDialog(&EVO_DIALOG);
                }
                EVO_SCRIPT->vars[19] = -1;
                if (EVO_RANK_UP_STATE != 0) {
                    animateWindowTo(&EVO_RANK_UP_WINDOW, (Rect16 *)-1);
                }
            } else {
                EVO_LEVEL_UP_PENDING = 0;
            }
        }
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        break;
    }
}

void EVO_addPartnerExp(void) {
    char text[168];
    Partner *partner;
    s32 ability;
    s32 i;

    partner = &((PlayerProfile *)PLAYER_PROFILES)->partners[EVO_FUSION.partner];
    bzero((Scene3D *)text, 0xA1);
    if (EVO_FUSION.partnerKind >= 0) {
        EVO_SCRIPT->vars[18] = 2;
        sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c7[%s]", EVO_FUSION.result, EVO_DIGI_PARTS[EVO_FUSION.result].name);
        initDialog((u8 *)&EVO_DIALOG, text, 0x80);
        runDialog(&EVO_DIALOG);
        EVO_FUSION.partnerKind = -2;
        return;
    }
    if (EVO_FUSION.partnerKind == -1) {
        if (EVO_FUSION.result > 0) {
            EVO_FUSION.result--;
            if ((s8)partner->level < 99) {
                partner->exp++;
                if (getExpForNextLevel((s8)partner->level) - (u16)partner->exp > 0) {
                    return;
                }
                partner->level++;
                if ((s8)partner->level >= 99) {
                    EVO_FUSION.result = 0;
                }
                ability = findNewPartnerAbility((AbilityLearnEntry *)EVO_DIGI_PARTS, 0, EVO_FUSION.partner);
                EVO_SCRIPT->vars[19] = 1;
                EVO_NEW_DIGI_PART = -1;
                EVO_LEVEL_UP_PENDING = 1;
                EVO_RANK_UP_STATE = 1;
                animateWindowTo(&EVO_RANK_UP_WINDOW, &EVO_RANK_UP_RECT);
                if (ability >= 0) {
                    EVO_SCRIPT->vars[19] = 2;
                    EVO_NEW_DIGI_PART = ability;
                }
                ability = rollPartnerAbility(0, EVO_FUSION.partner);
                if (ability >= 0) {
                    EVO_STAT_BONUSES[ability] += 10;
                }
            } else {
                EVO_RANK_UP_STATE = 2;
                animateWindowTo(&EVO_RANK_UP_WINDOW, &EVO_RANK_UP_RECT);
                do {
                    waitFrames(1);
                } while (!(PAD_STATES[0]->pressed & 0x40));
                playMenuSound(1);
                animateWindowTo(&EVO_RANK_UP_WINDOW, (Rect16 *)-1);
                EVO_FUSION.result = 0;
            }
        } else {
            for (i = 0; i < 4; i++) {
                if (i == 0) {
                    partner->hpBonus += EVO_STAT_BONUSES[0];
                } else {
                    partner->attackBonus[i - 1] += EVO_STAT_BONUSES[i];
                }
            }
            updatePartnerStats(0, EVO_FUSION.partner);
            EVO_FUSION.partnerKind = -2;
        }
        return;
    }
    EVO_FUSION.rewardStep = 10;
    EVO_FUSION.step = 3;
    EVO_CARD_LIST_MENU.active = 1;
    EVO_FUSION.previewOpen = 0;
    for (i = 0; i < 4; i++) {
        EVO_STAT_BONUSES[i] = 0;
    }
    EVO_SCRIPT->vars[8] = -1;
}

void EVO_tickFusionResult(void) {
    s32 i;

    if (EVO_FUSION.resultStep == 0) {
        EVO_FUSION.resultStep = 1;
        for (i = 0; i < 2; i++) {
            EVO_TRAYS[i].merge = 0;
        }
        EVO_SCRIPT->vars[8] = -1;
        return;
    }
    if (EVO_FUSION.resultStep == 1) {
        if (EVO_FUSION.resultKind == 1) {
            EVO_playEffect(7, 0);
        } else {
            EVO_playEffect(6, 0);
        }
        removeCardFromCollection(0, EVO_FUSION.firstCard, 1);
        removeCardFromCollection(0, EVO_FUSION.secondCard, 1);
        if (addCardToCollection(0, EVO_FUSION.result, 1) >= 0) {
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.result]++;
            EVO_CARD_RECEIVED = 1;
        } else {
            EVO_CARD_RECEIVED = 0;
        }
        ((PlayerProfile *)PLAYER_PROFILES)->fusedCards++;
        if ((u16)((PlayerProfile *)PLAYER_PROFILES)->fusedCards >= 10000) {
            ((PlayerProfile *)PLAYER_PROFILES)->fusedCards = 9999;
        }
        ((PlayerProfile *)PLAYER_PROFILES)->fusionCardsUsed += 2;
        if ((u16)((PlayerProfile *)PLAYER_PROFILES)->fusionCardsUsed >= 10000) {
            ((PlayerProfile *)PLAYER_PROFILES)->fusionCardsUsed = 9999;
        }
        if (EVO_SCRIPT->vars[13] != 0) {
            ((PlayerProfile *)PLAYER_PROFILES)->fusionMutations++;
            if ((u16)((PlayerProfile *)PLAYER_PROFILES)->fusionMutations >= 10000) {
                ((PlayerProfile *)PLAYER_PROFILES)->fusionMutations = 9999;
            }
        }
        if (EVO_FUSION.resultKind == 1) {
            EVO_FUSION.cutscene = 1;
            EVO_CUTSCENE_MODELS[0] = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->modelId;
            EVO_CUTSCENE_MODELS[1] = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->modelId;
            return;
        }
        animateWindowTo(&EVO_WINDOWS[13].win, &EVO_WINDOW_DEFS[13].rect);
        EVO_FUSION.resultStep = 2;
    }
    EVO_TRAYS[0].x = 0x3A;
    EVO_TRAYS[1].x = -0x58;
    EVO_FUSION.scriptState = 0;
    if (PAD_STATES[0]->pressed & 0x40) {
        animateWindowTo(&EVO_WINDOWS[13].win, (Rect16 *)-1);
        playSoundEffect(0);
        EVO_FUSION.step = 0x12;
        EVO_FUSION.resultStep = 3;
    }
}

void EVO_closeFusionResult(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x58) {
        EVO_TRAYS[0].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 1;
        EVO_FUSION.resultStep = 0;
    }
}

void EVO_showCutsceneResult(void) {
    if (EVO_FUSION.cutscene != 0) {
        EVO_playEffect(8, 0);
        EVO_FUSION.cutscene = 0;
        animateWindowTo(&EVO_WINDOWS[13].win, &EVO_WINDOW_DEFS[13].rect);
    }
    if (PAD_STATES[0]->pressed & 0x40) {
        animateWindowTo(&EVO_WINDOWS[13].win, (Rect16 *)-1);
        playSoundEffect(0);
        EVO_FUSION.step = 0x12;
        EVO_FUSION.resultStep = 3;
    }
}

void EVO_slideOutFirstTrayWithResult(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x58) {
        EVO_TRAYS[0].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_SCRIPT->vars[8] = 1;
    }
}
