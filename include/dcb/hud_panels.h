#ifndef DCB_HUD_PANELS_H
#define DCB_HUD_PANELS_H

#include "game.h"
#include "dcb/battle_hud.h"

void startPanelMove(Panel *panel, s16 targetX, s16 targetY, s32 frames);
s32 stepPanelMove();
void holdPanelAtTarget();
void tickDeckPanel(s32 player);
void tickTurnMarkerPanel(s32 player);
void tickStatusPanel(s32 player);
void tickPlayedCardPanel(s32 player);
void tickAttackPanel(s32 player);
void tickCardInfoPanel(s32 player);
void tickBattleHud(void);

#endif /* DCB_HUD_PANELS_H */
