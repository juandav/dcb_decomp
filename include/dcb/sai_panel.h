#ifndef DCB_SAI_PANEL_H
#define DCB_SAI_PANEL_H

#include "game.h"

void SAI_initPanelCover(void);
void SAI_createPanelFrame(void);
void SAI_createPanelFrameShadow(void);
void SAI_drawPanelCover(void);
void SAI_drawPanelFrame(void);
void SAI_drawPanelFrameShadow(void);
s32 SAI_spinPanel(void);
s32 SAI_uncoverPanel(void);
s32 SAI_coverPanel(void);
void SAI_freePanelFrame(void);
void SAI_freePanelFrameShadow(void);

#endif /* DCB_SAI_PANEL_H */
