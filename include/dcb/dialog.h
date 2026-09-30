#ifndef DCB_DIALOG_H
#define DCB_DIALOG_H

#include "game.h"
#include "dcb/menu.h"

/* A message box, optionally with two choices ("Yes"/"No" or custom labels). */
typedef struct {
    /* 0x00 */ UiWindow win;
    /* 0x44 */ CursorHighlight cursor;
    /* 0x94 */ u8 *text;
    /* 0x98 */ char *yesLabel;
    /* 0x9C */ char *noLabel;
    /* 0xA0 */ void (*onFrame)(void);
    /* 0xA4 */ u8 type; /* 0: message only, 1: Yes/No, 2: labels set by the caller */
    /* 0xA5 */ s8 choice; /* 0: cancelled, 1: yes, 2: no, 3: closed from outside (closed set) */
    /* 0xA6 */ u8 pad;
    /* 0xA7 */ u8 halfTextWidth;
    /* 0xA8 */ s16 width;
    /* 0xAA */ s16 height;
    /* 0xAC */ s16 yesX;
    /* 0xAE */ s16 yesWidth;
    /* 0xB0 */ s16 noX;
    /* 0xB2 */ s16 noWidth;
    /* 0xB4 */ u8 closed;
    /* 0xB5 */ u8 cancelDisabled;
    /* 0xB6 */ u8 unkB6; /* flag 0x80 of initDialog's flags */
} Dialog;

/* (Dialog *dialog, u8 *text, u32 flags): left without a prototype because the
   callers still pass their dialogs as other types */
void initDialog();
void dialogTask();
void drawDialogBody(Dialog *dialog);
s32 runDialogForPad(void *dialog, s32 pad);
s8 runDialog(void *dialog);

#endif /* DCB_DIALOG_H */
