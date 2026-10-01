#ifndef DCB_KAW_MATCH_INTRO_H
#define DCB_KAW_MATCH_INTRO_H

#include "game.h"

extern s32 KAW_VS_PANEL_POS[2][4];
extern s32 KAW_VS_NAME_POS[2][4];
extern s32 KAW_VS_INNER_LINE_POS[2][4];
extern s32 KAW_VS_OUTER_LINE_POS[2][4];

void KAW_drawDeckName(s32 x, s32 y, char *name);
void KAW_drawBattleRecord(s32 x, s32 y, s32 wins, s32 losses);
void KAW_renderVersusScreen(void);

#endif /* DCB_KAW_MATCH_INTRO_H */
