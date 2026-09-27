#ifndef DCB_HACKING_SHELL_H
#define DCB_HACKING_SHELL_H

#include "game.h"
#include "dcb/shell.h"

void runHackingSequence(s32 scriptIndex, s32 parentTask);
void drawHackErrorText(void *win);
void drawHackPartnerMovedText(void *win);
void drawHackTauntText(void *win);
void drawHackingTerminal();
void drawHackingWindows(void);

#endif /* DCB_HACKING_SHELL_H */
