#ifndef DCB_SCROLL_BG_H
#define DCB_SCROLL_BG_H

#include "game.h"
#include "dcb/stage.h"

void resetScrollingBackground(void);
void fadeOutScrollingBackground(void);
void changeScrollingBackground(s32 image, s32 x, s32 y, s32 w, s32 h);
void loadScrollingBackground(void);
void hideScrollingBackground(void);
void setBackgroundScrollMode(s8 scrollMode);
void freeScrollingBackground(void);

#endif /* DCB_SCROLL_BG_H */
