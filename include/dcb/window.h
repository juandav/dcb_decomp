#ifndef DCB_WINDOW_H
#define DCB_WINDOW_H

#include "game.h"
#include "dcb/frame_callback.h"

void resetWindowPrimPool(void);
void initWindowPrimPool(s32 count);
#if VERSION_JP
void animateWindowTo(UiWindow *win, Rect16 *target, s32 scrollX, s32 scrollY);
void drawWindowFrame(Rect16 *rect, s32 offsetX, s32 offsetY, s32 semiTrans, s32 brightness, CVECTOR *colors, s32 z);
void openWindow(void *winPtr, void *rectPtr, s32 fromPtr, s16 *viewPtr, s32 flags, s32 colors, s32 frames);
s32 moveWindowListCursor(s32 row, UiWindow *win);
#elif VERSION_US || VERSION_EU
void animateWindowTo(UiWindow *win, Rect16 *target);
void drawWindowFrame(Rect16 *rect, u8 style, s32 semiTrans, s32 brightness, s32 palette, s32 z);
void openWindow(void *winPtr, void *rectPtr, s32 fromPtr, s16 *viewPtr, s32 flags, s32 style, s32 brightness, s32 frames);
void scrollWindowTo(s16 *win, s32 x, s32 y);
#else
#error "main/ui/window: version not checked"
#endif
s32 stepWindowAnimation(UiWindow *w);
void clipRectToBounds(Rect16 *rect, Rect16 *bounds);
s32 drawWindow(UiWindow *w, void (*draw)(), s32 z);
void drawVerticalScrollbar(UiWindow *w, s32 z);
void drawHorizontalScrollbar(UiWindow *w, s32 z);
int isWindowPrimPoolFull(void);
s32 drawWindow(UiWindow *, void (*)(), s32);

#endif /* DCB_WINDOW_H */
