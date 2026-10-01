#include "common.h"
#include "game.h"
#include "dcb/evo_lists.h"
#include "dcb/card_db.h"
#include "dcb/window.h"
#include "dcb/vram_upload.h"
#include "dcb/dialog.h"
#include "dcb/sound_play.h"
#include "dcb/evoseg.h"
#include "dcb/evo_card_list.h"
#include "dcb/evo_trays.h"
#include "dcb/evo_fusion.h"
#include "dcb/evo_fusion_result.h"
#include "dcb/evo_data.h"

extern s16 EVO_CURSOR_CARD;

void EVO_openPartnerList(void) {
    s32 i;

    if (EVO_FUSION.partnerListOpen == 0) {
        EVO_FUSION.partnerListOpen = 1;
        EVO_FUSION.partnerCount = 0;
        EVO_FUSION.partner = 0;
        for (i = 10; i < 12; i++) {
            animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i].rect);
        }
        for (i = 0; i < 3; i++) {
            if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId != 0) {
                EVO_FUSION.partnerCount++;
            }
        }
    }
    EVO_SCRIPT->vars[8] = -1;
    EVO_FUSION.step = 2;
}

void EVO_closePartnerList(void) {
    s32 i;

    EVO_FUSION.partnerListOpen = 0;
    EVO_FUSION.step = 0;
    for (i = 10; i < 12; i++) {
        animateWindowTo(&EVO_WINDOWS[i].win, (Rect16 *)-1);
    }
}

void EVO_tickPartnerList(void) {
    if (PAD_STATES[0]->pressed & 0x40) {
        playSoundEffect(0);
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 1;
        EVO_FUSION.firstCard = ((PlayerProfile *)PLAYER_PROFILES)->partners[EVO_FUSION.partner].cardId;
    } else if (PAD_STATES[0]->pressed & 0x10) {
        playSoundEffect(1);
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 2;
    } else if (PAD_STATES[0]->repeat & 0x4000) {
        EVO_FUSION.partner++;
        if (EVO_FUSION.partner >= EVO_FUSION.partnerCount) {
            EVO_FUSION.partner = 0;
        } else {
            playSoundEffect(2);
        }
        if (EVO_FUSION.partnerCount == 0 || EVO_FUSION.partner >= EVO_FUSION.partnerCount - 1) {
            PAD_STATES[0]->repeatEnabled = 0;
        }
    } else if (PAD_STATES[0]->repeat & 0x1000) {
        EVO_FUSION.partner--;
        if (EVO_FUSION.partner < 0) {
            EVO_FUSION.partner = EVO_FUSION.partnerCount - 1;
        } else {
            playSoundEffect(2);
        }
        if (EVO_FUSION.partner <= 0) {
            PAD_STATES[0]->repeatEnabled = 0;
        }
    }
}

void EVO_openCardList(void) {
    animateWindowTo(&EVO_WINDOWS[10].win, (Rect16 *)-1);
    animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
    EVO_CARD_LIST_MENU.active = 1;
    EVO_FUSION.sortMenuOpen = 0;
    EVO_FUSION.step = 3;
    EVO_FUSION.previewOpen = 0;
    EVO_CARD_LIST_MENU.row = 0;
    scrollWindowTo((s16 *)EVO_CARD_LIST_MENU.win, 0, 0);
    EVO_SCRIPT->vars[8] = -1;
}

void EVO_closeCardList(void) {
    animateWindowTo(&EVO_WINDOWS[10].win, &EVO_WINDOW_DEFS[10].rect);
    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
    EVO_CARD_LIST_MENU.active = 0;
}

void EVO_slideTrayOut(s32 index) {
    EvoTray *trays = EVO_TRAYS;

    trays[index].x -= 10;
    if (trays[index].x < -0x58) {
        trays[index].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_SCRIPT->vars[8] = -1;
        if (index == 0) {
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.firstCard]--;
        } else {
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.firstCard]++;
        }
    }
}

void EVO_slideTrayIn(s32 index) {
    EvoTray *trays = EVO_TRAYS;

    trays[index].x += 10;
    if (trays[index].x >= 15) {
        trays[index].x = 14;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_SCRIPT->vars[8] = -1;
    }
}

void EVO_showBothTrays(void) {
    EVO_TRAYS[0].x += 10;
    EVO_TRAYS[1].x += 10;
    if (EVO_TRAYS[0].x >= 15) {
        EVO_TRAYS[0].x = 14;
    }
    if (EVO_TRAYS[1].x >= 0x67) {
        EVO_TRAYS[1].x = 0x66;
    }
    if (EVO_TRAYS[0].x == 14 && EVO_TRAYS[1].x == 0x66) {
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 1;
    }
}

void EVO_repickSecondCard(void) {
    EVO_TRAYS[0].x -= 10;
    EVO_TRAYS[1].x -= 10;
    if (EVO_TRAYS[1].x < 14) {
        EVO_TRAYS[0].x = -0x58;
        EVO_TRAYS[1].x = 14;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
        EVO_startSecondCardPick();
    }
}

void EVO_checkCardCapacity(s16 cardId) {
    if (cardId < 0) {
        cardId = -1;
    }
    if (EVO_SCRIPT->vars[13] != 0) {
        EVO_SCRIPT->vars[11] = 0;
        return;
    }
    EVO_SCRIPT->vars[11] = getOwnedCardCount(0, cardId);
    if ((((PlayerProfile *)PLAYER_PROFILES)->cardCollection[cardId] & 7) + 1 >= 7) {
        EVO_SCRIPT->vars[11] = -1;
    } else {
        EVO_SCRIPT->vars[11] = 0;
    }
}

void EVO_tickCardList(void) {
    s16 cardId;
    u8 blocked = 0;
    s32 i;

    cardId = EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->id;
    EVO_CURSOR_CARD = cardId;
    if (PAD_STATES[0]->pressed & 0x100) {
        if (EVO_FUSION.previewOpen != 0) {
            return;
        }
        if (EVO_FUSION.sortMenuOpen == 0) {
            EVO_FUSION.sortMenuOpen = 1;
            EVO_CARD_LIST_MENU.active = 0;
            animateWindowTo(&EVO_SORT_WINDOW, &EVO_SORT_MENU.rect);
            playSoundEffect(3);
            return;
        }
        playSoundEffect(4);
        EVO_FUSION.sortMenuOpen = 0;
        EVO_CARD_LIST_MENU.active = 1;
        animateWindowTo(&EVO_SORT_WINDOW, (Rect16 *)-1);
    } else if (EVO_FUSION.sortMenuOpen == 0) {
        if (EVO_FUSION.previewOpen == 0) {
            if (PAD_STATES[0]->pressed & 0x40) {
                if (EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->fusionPoints == 0) {
                    blocked = 1;
                } else if (EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->type == 0 &&
                           (EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->attr & 0xF) > EVO_MAX_CARD_LEVEL) {
                    blocked = 1;
                }
                if (EVO_SPARE_CARD_COUNTS[cardId] == 0) {
                    return;
                }
                if (cardId >= 0xAC && cardId < 0xBF) {
                    for (i = 0; i < 6; i++) {
                        if (EVO_PARTNER_CARD_IDS[i] == cardId) {
                            i = -1;
                            break;
                        }
                    }
                    if (i == -1) {
                        initDialog((u8 *)&EVO_DIALOG, "You can't use Partner Cards\nin Fusion.", 0);
                    } else {
                        initDialog((u8 *)&EVO_DIALOG, "You can't use that Card in Fusion.", 0);
                    }
                    runDialog(&EVO_DIALOG);
                    return;
                }
                if (blocked) {
                    return;
                }
                playSoundEffect(0);
                if (EVO_FUSION.fusionType == 0) {
                    if (EVO_FUSION.pickSlot != 2) {
                        EVO_CARD_LIST_MENU.active = 0;
                        EVO_SCRIPT->vars[8] = 1;
                        EVO_FUSION.step = 0;
                        EVO_loadCardImage(cardId, 0);
                        EVO_FUSION.firstCard = cardId;
                        EVO_FUSION.busy[1] = 1;
                        return;
                    }
                    EVO_CARD_LIST_MENU.active = 0;
                    EVO_loadCardImage(cardId, 1);
                    do {
                        waitFrames(1);
                    } while (EVO_FUSION.busy[0] != 0);
                    EVO_FUSION.secondCard = cardId;
                    EVO_findFusionResult();
                    EVO_checkCardCapacity(EVO_FUSION.result);
                    EVO_FUSION.step = 11;
                    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
                    EVO_SPARE_CARD_COUNTS[EVO_FUSION.secondCard]--;
                    EVO_FUSION.busy[2] = 1;
                    EVO_loadCardImage(EVO_FUSION.result, 2);
                    do {
                        waitFrames(1);
                    } while (EVO_FUSION.busy[0] != 0);
                } else if (EVO_FUSION.fusionType == 1) {
                    EVO_CARD_LIST_MENU.active = 0;
                    uploadTim((u32 *)((u8 *)EVO_FUSION.cardArchive + EVO_FUSION.cardArchive[cardId]), 0x1C0, 0x190, 0x180, 0x1FC);
                    EVO_FUSION.secondCard = cardId;
                    EVO_FUSION.previewOpen = 1;
                    animateWindowTo(&EVO_WINDOWS[12].win, &EVO_WINDOW_DEFS[12].rect);
                }
            } else if (PAD_STATES[0]->pressed & 0x10) {
                EVO_FUSION.previewOpen = 0;
                EVO_FUSION.step = 0;
                EVO_SCRIPT->vars[8] = 2;
                playSoundEffect(1);
            }
        } else if (PAD_STATES[0]->pressed & 0x40) {
            playSoundEffect(0);
            EVO_FUSION.step = 0;
            EVO_SCRIPT->vars[8] = 1;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            playSoundEffect(1);
            EVO_CARD_LIST_MENU.active = 1;
            EVO_FUSION.previewOpen = 0;
            animateWindowTo(&EVO_WINDOWS[12].win, (Rect16 *)-1);
        }
    } else if (PAD_STATES[0]->pressed & 0x10) {
        playSoundEffect(4);
        EVO_FUSION.sortMenuOpen = 0;
        EVO_CARD_LIST_MENU.active = 1;
        animateWindowTo(&EVO_SORT_WINDOW, (Rect16 *)-1);
    }
}
