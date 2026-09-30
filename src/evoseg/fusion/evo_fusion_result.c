#include "common.h"
#include "game.h"
#include "dcb/evo_fusion_result.h"
#include "dcb/card_db.h"
#include "dcb/window.h"
#include "dcb/evo_card_list.h"
#include "dcb/evo_data.h"

void EVO_findCardOfLevel(u8 type, u8 level, s16 *out, s16 excludeA, s16 excludeB);

void EVO_resetFusion(void) {
    EVO_FUSION.secondCard = -1;
    EVO_FUSION.firstCard = -1;
    EVO_FUSION.hideResult = 1;
    EVO_FUSION.scriptState = 4;
    EVO_FUSION.sortMenuOpen = 0;
    EVO_FUSION.previewOpen = 0;
    EVO_FUSION.pickSlot = 1;
    EVO_FUSION.busy[1] = 0;
    EVO_FUSION.busy[2] = 0;
}

void EVO_cancelFirstCard(void) {
    EVO_FUSION.pickSlot = 1;
    EVO_FUSION.step = 8;
    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
    EVO_CARD_LIST_MENU.active = 0;
}

void EVO_startSecondCardPick(void) {
    EVO_FUSION.pickSlot = 2;
    EVO_SCRIPT->vars[8] = -1;
    EVO_FUSION.step = 7;
}

s16 EVO_findFusionResult(void) {
    s8 types[2];
    s8 i;
    s8 j;
    s8 level;
    s8 kind;

    EVO_FUSION.result = -1;
    level = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->fusionPoints + EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->fusionPoints;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            types[0] = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->type;
        } else {
            types[i] = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->type;
        }
        switch (types[i]) {
        case 0:
            if (i == 0) {
                types[0] = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->attr >> 4;
            } else {
                types[i] = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->attr >> 4;
            }
            break;
        case 1:
        case 2:
            types[i] = 5;
            break;
        }
    }
    kind = EVO_FUSION_RESULT_TYPES[types[0]][types[1]];
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 20; i++) {
            if (EVO_FUSION_RECIPES[i][j] == EVO_FUSION.firstCard && EVO_FUSION_RECIPES[i][1 - j] == EVO_FUSION.secondCard) {
                EVO_FUSION.result = EVO_FUSION_RECIPES[i][2];
                EVO_CUTSCENE_MODELS[2] = EVO_FUSION_RECIPES[i][3];
            }
        }
    }
    EVO_SCRIPT->vars[14] = 0;
    EVO_SCRIPT->vars[13] = 0;
    if (EVO_FUSION.result == -1) {
        EVO_FUSION.roll = rand() % 100;
        if (EVO_FUSION.roll < 0) {
            EVO_FUSION.roll = EVO_FUSION.roll * -1;
        }
        if (EVO_FUSION.roll <= (s8)(level / 10)) {
            EVO_SCRIPT->vars[13] = 1;
            EVO_FUSION.resultKind = 2;
            EVO_FUSION.roll = rand() % 100;
            if (EVO_FUSION.roll < 0) {
                EVO_FUSION.roll = EVO_FUSION.roll * -1;
            }
            if (EVO_FUSION.roll < 21) {
                EVO_FUSION.result = rand() % 12 + 0x111;
                if (getOwnedCardCount(0, EVO_FUSION.result) >= 6) {
                    EVO_FUSION.result = 200;
                }
            } else if (EVO_FUSION.roll < 61) {
                EVO_FUSION.result = 200;
            } else {
                EVO_FUSION.roll = rand() % 3 + 1;
                EVO_findCardOfLevel(kind, level + EVO_FUSION.roll, &EVO_FUSION.result, EVO_FUSION.firstCard, EVO_FUSION.secondCard);
                if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                    EVO_FUSION.result = -1;
                }
                for (i = 0; EVO_FUSION.result == -1;) {
                    i++;
                    EVO_findCardOfLevel(kind, level + EVO_FUSION.roll + i, &EVO_FUSION.candidates[0], 500, 500);
                    EVO_findCardOfLevel(kind, level + EVO_FUSION.roll - i, &EVO_FUSION.candidates[1], 500, 500);
                    if (EVO_FUSION.candidates[0] != -1 && EVO_FUSION.candidates[1] != -1) {
                        EVO_FUSION.result = EVO_FUSION.candidates[rand() % 2];
                    } else if (EVO_FUSION.candidates[0] != -1) {
                        EVO_FUSION.result = EVO_FUSION.candidates[0];
                    } else if (EVO_FUSION.candidates[1] != -1) {
                        EVO_FUSION.result = EVO_FUSION.candidates[1];
                    }
                    if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                        EVO_FUSION.result = -1;
                    }
                }
            }
        } else {
            EVO_findCardOfLevel(kind, level, &EVO_FUSION.result, EVO_FUSION.firstCard, EVO_FUSION.secondCard);
            if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                EVO_FUSION.result = -1;
            }
            for (i = 0; EVO_FUSION.result == -1;) {
                i++;
                EVO_findCardOfLevel(kind, level + i, &EVO_FUSION.candidates[0], 500, 500);
                EVO_findCardOfLevel(kind, level - i, &EVO_FUSION.candidates[1], 500, 500);
                if (EVO_FUSION.candidates[0] != -1 && EVO_FUSION.candidates[1] != -1) {
                    EVO_FUSION.result = EVO_FUSION.candidates[rand() % 2];
                } else if (EVO_FUSION.candidates[0] != -1) {
                    EVO_FUSION.result = EVO_FUSION.candidates[0];
                } else if (EVO_FUSION.candidates[1] != -1) {
                    EVO_FUSION.result = EVO_FUSION.candidates[1];
                }
                if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                    EVO_FUSION.result = -1;
                }
            }
            EVO_FUSION.resultKind = 0;
        }
    } else {
        EVO_FUSION.resultKind = 1;
        EVO_SCRIPT->vars[14] = 1;
    }
    switch (EVO_CARDS_BY_ID[EVO_FUSION.result]->type) {
    case 0:
        EVO_SCRIPT->vars[15] = EVO_CARDS_BY_ID[EVO_FUSION.result]->attr & 0xF;
        EVO_SCRIPT->vars[16] = EVO_CARDS_BY_ID[EVO_FUSION.result]->attr >> 4;
        break;
    case 1:
        EVO_SCRIPT->vars[15] = 5;
        EVO_SCRIPT->vars[16] = 5;
        break;
    case 2:
        EVO_SCRIPT->vars[15] = 6;
        EVO_SCRIPT->vars[16] = 6;
        break;
    }
    EVO_SCRIPT->vars[17] = EVO_FUSION.resultKind;
    EVO_SCRIPT->vars[11] = EVO_FUSION.result;
    return EVO_FUSION.result;
}

void EVO_findCardOfLevel(u8 type, u8 level, s16 *out, s16 excludeA, s16 excludeB) {
    s16 candidates[10];
    s16 i;
    s16 j;
    s16 tmp;
    s8 count;

    *out = -1;
    if (level < 2) {
        level = 2;
    }
    if (level >= 36) {
        level = 35;
    }
    count = 0;
    for (i = 0; i < 10; i++) {
        candidates[i] = -1;
    }
    if (type == 5) {
        for (i = EVO_CARD_ID_RANGES[5].first; i <= EVO_CARD_ID_RANGES[5].last; i++) {
            if (EVO_CARDS_BY_ID[i]->level == level && i != excludeA && i != excludeB) {
                candidates[count] = i;
                count++;
            }
        }
        for (i = EVO_CARD_ID_RANGES[6].first; i <= EVO_CARD_ID_RANGES[6].last; i++) {
            if (EVO_CARDS_BY_ID[i]->level == level && i != excludeA && i != excludeB) {
                candidates[count] = i;
                count++;
            }
        }
        for (i = 0; i < count; i++) {
            j = rand() % count;
            tmp = candidates[i];
            candidates[i] = candidates[j];
            candidates[j] = tmp;
        }
        *out = candidates[0];
    } else {
        for (i = EVO_CARD_ID_RANGES[type].first; i <= EVO_CARD_ID_RANGES[type].last; i++) {
            if (EVO_CARDS_BY_ID[i]->level == level) {
                *out = i;
                return;
            }
        }
    }
}

void EVO_uploadShadedClut(EvoClut *clut, u16 flags) {
    u16 *src;
    u16 *dst;
    s32 brighten;
    s32 level;
    s32 i;
    s32 color;
    s32 r;
    s32 g;
    s32 b;

    brighten = clut->brighten;
    level = clut->level;
    src = clut->src;
    dst = clut->dst;
    for (i = 0; i < clut->rect.w * clut->rect.h; i++) {
        color = *src++;
        if (color != 0) {
            r = color & 0x1F;
            g = (color >> 5) & 0x1F;
            b = (color >> 10) & 0x1F;
            if (brighten == 0) {
                r = r * level / 255;
                g = g * level / 255;
                b = b * level / 255;
            } else {
                r += (31 - r) * level / 255;
                g += (31 - g) * level / 255;
                b += (31 - b) * level / 255;
            }
            if (r < 0) {
                r = 0;
            } else if (r >= 32) {
                r = 31;
            }
            if (g < 0) {
                g = 0;
            } else if (g >= 32) {
                g = 31;
            }
            if (b < 0) {
                b = 0;
            } else if (b >= 32) {
                b = 31;
            }
            color = flags | (color & 0x8000) | (b << 10) | (g << 5) | r;
        }
        *dst++ = color;
    }
    LoadImage((s16 *)&clut->rect, (s32)clut->dst);
    DrawSync(0);
}
