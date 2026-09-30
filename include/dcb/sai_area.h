#ifndef DCB_SAI_AREA_H
#define DCB_SAI_AREA_H

#include "game.h"

void SAI_glitchVram(s8 animate);
void SAI_runTalkPanel(void);
void SAI_stepBrightness(s8 index);
void SAI_createPanel(void);
void SAI_freePanel(void);
void SAI_runArea(s32 resume);

#endif /* DCB_SAI_AREA_H */
