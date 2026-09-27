#ifndef DCB_DIALOG_H
#define DCB_DIALOG_H

#include "game.h"
#include "dcb/menu.h"

void initDialog(u8 *dialog, u8 *text, u32 flags);
void dialogTask();
void drawDialogBody(u8 *dialog);
s32 runDialogForPad(s32 *dialog, s32 pad);
s8 runDialog(void *dialog);

#endif /* DCB_DIALOG_H */
