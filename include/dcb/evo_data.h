#ifndef DCB_EVO_DATA_H
#define DCB_EVO_DATA_H

#include "game.h"
#include "dcb/evoseg.h"

extern EvoRange EVO_CARD_ID_RANGES[];
extern EvoAbilityInfo EVO_DIGI_PARTS[];
extern u8 EVO_PARTNER_CARD_IDS[6];
extern EvoAbilityReward EVO_PARTNER_FUSION_REWARDS[][5];
extern s8 EVO_FUSION_RESULT_TYPES[][6];
extern Bytes4 EVO_TEXT_COLORS[];
extern Bytes4 EVO_TEXT_COLOR_GREY;
extern Bytes4 EVO_TEXT_COLOR_RED;
extern const u8 EVO_FUSION_RECIPES[20][4];

#endif /* DCB_EVO_DATA_H */
