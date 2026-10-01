#include "dcb/dialog.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/menu.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/save_checksum.h"
#include "dcb/player_rank.h"
#include "dcb/prim_util.h"
#include "dcb/transform.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"
#include "dcb/pad.h"

void initDialog(Dialog *dialog, u8 *text, u32 flags) {
    Rect16 r;
    s32 labelWidth;
    s32 widest;

    dialog->type = flags & 0xF;
    dialog->unkB6 = flags & 0x80;
    dialog->text = text;
    measureText(text);
    dialog->halfTextWidth = (TEXT_WIDTH + 1) / 2;
    dialog->width = (TEXT_WIDTH + 1) / 2 * 2 + 4;
    dialog->height = (TEXT_HEIGHT + 1) / 2 * 2 + 4;
    if (dialog->type != 0) {
        /* room for the row of choices */
        if (text == 0) {
            dialog->height = 0x10;
        } else {
            dialog->height += 0x10;
        }
    }
    if (dialog->type != 2) {
        dialog->yesLabel = "Yes";
        dialog->noLabel = "No";
    }
    /* the labels are 6 pixels per character */
    dialog->yesWidth = strlen(dialog->yesLabel) * 6;
    dialog->noWidth = strlen(dialog->noLabel) * 6;
    widest = dialog->yesWidth;
    if (widest < dialog->noWidth) {
        widest = dialog->noWidth;
    }
    /* room for both labels side by side, and a margin */
#if VERSION_EU
    labelWidth = widest * 2 + 0x10;
#elif VERSION_US
    labelWidth = widest = widest * 2 + 0x10;
#else
#error "main/ui/dialog: version not checked"
#endif
    if (dialog->width < labelWidth) {
        dialog->width = labelWidth;
    }
    /* centred on the 320x240 screen, the two choices either side of the middle */
    dialog->yesX = (320 - dialog->width) / 2 + dialog->width / 2 - (dialog->yesWidth + 4);
    dialog->noX = (320 - dialog->width) / 2 + dialog->width / 2 + 4;
    r.x = (320 - dialog->width) / 2;
    r.y = (240 - dialog->height) / 2;
    r.w = dialog->width;
    r.h = dialog->height;
    openWindow(dialog, &r, -1, (s16 *)-1, 8, 0x77, 0x80, 8);
    dialog->win.palette = 4;
    if (dialog->type != 0) {
        initCursorHighlight(&dialog->cursor, (Rect16 *)-1, (Bytes4 *)-1);
    }
    dialog->choice = 2;
    dialog->pad = 0;
    dialog->onFrame = 0;
    dialog->cancelDisabled = 0;
    dialog->closed = 0;
}

s8 runDialog(void *dialog) {
    spawnTask(0, -1, 0, 0x400, &dialogTask, dialog, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
    return ((Dialog *)dialog)->choice;
}

s32 runDialogForPad(void *dialog, s32 pad) {
    s32 task;

    task = getCurrentTaskId();
    ((Dialog *)dialog)->pad = pad;
    spawnTask(0, -1, 0, 0x400, dialogTask, dialog, task, 0, 0);
    waitFrames(0x7FFFFFFF);
    return ((Dialog *)dialog)->choice;
}

void dialogTask(Dialog *dialog, s32 parentTask) {
    Rect16 r;
    s16 x;
    s16 width;
    s16 closeButtons;

    PAD_INPUT_ENABLED = 0;
    if (dialog->choice == 1) {
        x = dialog->yesX;
        width = dialog->yesWidth;
    } else {
        x = dialog->noX;
        width = dialog->noWidth;
    }
    r.x = x;
    r.y = (240 - dialog->height) / 2 + dialog->height - 14;
    r.w = width;
    r.h = 12;
    setCursorHighlight(&dialog->cursor, &r, (Bytes4 *)-1);
    for (;;) {
        waitFrames(FRAME_INTERVAL);
        drawWindow(&dialog->win, drawDialogBody, 0);
        if (dialog->onFrame != 0) {
            dialog->onFrame();
        }
        /* Cross confirms; Triangle cancels a choice unless that is disabled */
        if (dialog->type != 0) {
            if (dialog->cancelDisabled != 0) {
                closeButtons = PAD_CROSS;
            } else {
                closeButtons = PAD_CROSS | PAD_TRIANGLE;
            }
        } else {
            closeButtons = PAD_CROSS;
        }
        if ((PAD_STATES[dialog->pad]->rawPressed & closeButtons) || dialog->closed != 0) {
            if (dialog->closed != 0) {
                dialog->choice = 3;
            } else if (PAD_STATES[dialog->pad]->rawPressed & PAD_TRIANGLE) {
                dialog->choice = 0;
                dialog->closed = 1;
                playMenuSound(0);
            } else {
                dialog->closed = 1;
                playMenuSound(1);
            }
            /* close the window and keep drawing until the animation ends */
            animateWindowTo(&dialog->win, (Rect16 *)-1);
            do {
                waitFrames(FRAME_INTERVAL);
                drawWindow(&dialog->win, drawDialogBody, 0);
                if (dialog->onFrame != 0) {
                    dialog->onFrame();
                }
            } while (dialog->win.animDone == 0);
            PAD_INPUT_ENABLED = 1;
            resumeTask(parentTask, dialog->choice);
            exitTask();
            return;
        }
    }
}

void drawDialogBody(Dialog *dialog) {
    Rect16 r;
    s32 x;
    s32 y;

    x = dialog->win.originX + (dialog->width - dialog->halfTextWidth * 2) / 2;
    y = dialog->win.originY + 2;
    if (dialog->text != 0) {
        drawText(x, y, (s32)dialog->text, 7, dialog->win.z);
    }
    y = dialog->win.originY + dialog->height - 0xE;
    if (dialog->type != 0) {
        if (dialog->closed == 0) {
            if ((PAD_STATES[dialog->pad]->rawPressed & PAD_LEFT) && dialog->choice != 1) {
                dialog->choice = 1;
                r.x = dialog->yesX;
                r.y = y;
                r.w = dialog->yesWidth;
                r.h = 0xC;
                moveCursorHighlight(&dialog->cursor, &r);
                playMenuSound(2);
            }
            if ((PAD_STATES[dialog->pad]->rawPressed & PAD_RIGHT) && dialog->choice != 2) {
                dialog->choice = 2;
                r.x = dialog->noX;
                r.y = y;
                r.w = dialog->noWidth;
                r.h = 0xC;
                moveCursorHighlight(&dialog->cursor, &r);
                playMenuSound(2);
            }
        }
        drawText(dialog->yesX, y, (s32)dialog->yesLabel, 7, dialog->win.z);
        drawText(dialog->noX, y, (s32)dialog->noLabel, 7, dialog->win.z);
        drawCursorHighlight(&dialog->cursor, dialog->win.z);
    }
}
