#ifndef DCB_EVO_TRAYS_H
#define DCB_EVO_TRAYS_H

#include "game.h"
#include "dcb/evoseg.h"

extern EvoWindowDef EVO_WINDOW_DEFS[];

void EVO_runChoiceDialog(s32 mode);
void EVO_drawTray(EvoTray *tray);
void EVO_slideInFirstTray(void);
void EVO_swapToFirstTray(void);
void EVO_swapToSecondTray(void);
void EVO_slideOutFirstTray(void);
void EVO_cancelSecondCard(s32 active);
void EVO_drawFusionTypeTitle(EvoWindow *w);
void EVO_drawFusionTypeHelp(EvoWindow *w);
void EVO_toggleMessageWindows(s8 mode);

#endif /* DCB_EVO_TRAYS_H */
