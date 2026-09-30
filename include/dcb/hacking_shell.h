#ifndef DCB_HACKING_SHELL_H
#define DCB_HACKING_SHELL_H

#include "game.h"
#include "dcb/partner_level.h"

void runHackingSequence(s32 scriptIndex, s32 parentTask);
void drawHackErrorText(UiWindow *win);
void drawHackPartnerMovedText(UiWindow *win);
void drawHackTauntText(UiWindow *win);
void drawHackingTerminal(UiWindow *win);
void drawHackingWindows(void);

#endif /* DCB_HACKING_SHELL_H */
