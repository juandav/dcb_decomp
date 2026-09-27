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
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"

void initDialog(u8 *dialog, u8 *text, u32 flags) {
    Rect16 r;
    s32 labelWidth;
    s32 width;
    s32 x;
    s32 centerX;

    dialog[0xA4] = flags & 0xF;
    dialog[0xB6] = flags & 0x80;
    *(u8 **)(dialog + 0x94) = text;
    measureText(text);
    dialog[0xA7] = (TEXT_WIDTH + 1) / 2;
    *(s16 *)(dialog + 0xA8) = (TEXT_WIDTH + 1) / 2 * 2 + 4;
    *(s16 *)(dialog + 0xAA) = (TEXT_HEIGHT + 1) / 2 * 2 + 4;
    if (dialog[0xA4] != 0) {
        if (text == 0) {
            *(s16 *)(dialog + 0xAA) = 0x10;
        } else {
            *(s16 *)(dialog + 0xAA) += 0x10;
        }
    }
    if (dialog[0xA4] != 2) {
        *(char **)(dialog + 0x98) = "Yes";
        *(char **)(dialog + 0x9C) = "No";
    }
    *(s16 *)(dialog + 0xAE) = strlen(*(u8 **)(dialog + 0x98)) * 6;
    *(s16 *)(dialog + 0xB2) = strlen(*(u8 **)(dialog + 0x9C)) * 6;
    labelWidth = *(s16 *)(dialog + 0xAE);
    if (labelWidth < *(s16 *)(dialog + 0xB2)) {
        labelWidth = *(s16 *)(dialog + 0xB2);
    }
    labelWidth = labelWidth * 2 + 0x10;
    if (*(s16 *)(dialog + 0xA8) < labelWidth) {
        *(s16 *)(dialog + 0xA8) = labelWidth;
    }
    width = *(s16 *)(dialog + 0xA8);
    x = (320 - width) / 2;
    centerX = x + width / 2;
    *(s16 *)(dialog + 0xAC) = centerX - (*(s16 *)(dialog + 0xAE) + 4);
    *(s16 *)(dialog + 0xB0) = centerX + 4;
    r.x = x;
    r.y = (240 - *(s16 *)(dialog + 0xAA)) / 2;
    r.w = *(s16 *)(dialog + 0xA8);
    r.h = *(s16 *)(dialog + 0xAA);
    openWindow(dialog, &r, -1, (s16 *)-1, 8, 0x77, 0x80, 8);
    dialog[0x38] = 4;
    if (dialog[0xA4] != 0) {
        initCursorHighlight((Unk800190F4 *)(dialog + 0x44), (Rect16 *)-1, (Bytes4 *)-1);
    }
    dialog[0xA5] = 2;
    dialog[0xA6] = 0;
    *(s32 *)(dialog + 0xA0) = 0;
    dialog[0xB5] = 0;
    dialog[0xB4] = 0;
}

s8 runDialog(void *dialog) {
    func_800149B8(0, -1, 0, 0x400, &dialogTask, dialog, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    return (*(s8 *)((s8 *)dialog + 0xA5));
}

s32 runDialogForPad(s32 *dialog, s32 pad) {
    s32 task;

    task = getCurrentTaskId();
    (*(s8 *)((s8 *)dialog + 0xA6)) = (s8) pad;
    func_800149B8(0, -1, 0, 0x400, dialogTask, dialog, task, 0, 0);
    func_80014C08(0x7FFFFFFF);
    return (s32) (*(s8 *)((s8 *)dialog + 0xA5));
}

void dialogTask(u8 *dialog, s32 parentTask) {
    Rect16 r;
    s32 x;
    s32 width;
    u16 closeButtons;
    PadState **pads;
    s32 on;

    PAD_INPUT_ENABLED = 0;
    if ((s8)dialog[0xA5] == 1) {
        x = *(s16 *)(dialog + 0xAC);
        width = *(s16 *)(dialog + 0xAE);
    } else {
        x = *(s16 *)(dialog + 0xB0);
        width = *(s16 *)(dialog + 0xB2);
    }
    r.x = x;
    r.y = (240 - *(s16 *)(dialog + 0xAA)) / 2 + *(s16 *)(dialog + 0xAA) - 14;
    r.w = width;
    r.h = 12;
    setCursorHighlight((Unk800190F4 *)(dialog + 0x44), &r, (Bytes4 *)-1);
    pads = PAD_STATES;
    on = 1;
    do {
        func_80014C08(FRAME_INTERVAL);
        drawWindow((Unk80016F38 *)dialog, drawDialogBody, 0);
        if (*(void (**)(void))(dialog + 0xA0) != 0) {
            (*(void (**)(void))(dialog + 0xA0))();
        }
        if (dialog[0xA4] != 0) {
            if (dialog[0xB5] != 0) {
                closeButtons = 0x40;
            } else {
                closeButtons = 0x50;
            }
        } else {
            closeButtons = 0x40;
        }
        if (pads[dialog[0xA6]]->unk2 & closeButtons) {
            break;
        }
    } while (dialog[0xB4] == 0);
    if (dialog[0xB4] != 0) {
        dialog[0xA5] = 3;
    } else if (pads[dialog[0xA6]]->unk2 & 0x10) {
        dialog[0xA5] = 0;
        dialog[0xB4] = on;
        playMenuSound(0);
    } else {
        dialog[0xB4] = on;
        playMenuSound(1);
    }
    animateWindowTo((Unk80016F38 *)dialog, (Rect16 *)-1);
    do {
        func_80014C08(FRAME_INTERVAL);
        drawWindow((Unk80016F38 *)dialog, drawDialogBody, 0);
        if (*(void (**)(void))(dialog + 0xA0) != 0) {
            (*(void (**)(void))(dialog + 0xA0))();
        }
    } while (*(s8 *)(dialog + 0x41) == 0);
    PAD_INPUT_ENABLED = 1;
    func_80014A48(parentTask, (s8)dialog[0xA5]);
    func_80014A90();
}

void drawDialogBody(u8 *dialog) {
    Rect16 r;
    s32 x;
    s32 y;

    x = *(s16 *)dialog + (*(s16 *)(dialog + 0xA8) - dialog[0xA7] * 2) / 2;
    y = *(s16 *)(dialog + 2) + 2;
    if (*(s32 *)(dialog + 0x94) != 0) {
        drawText(x, y, *(s32 *)(dialog + 0x94), 7, *(s16 *)(dialog + 0x3A));
    }
    y = *(s16 *)(dialog + 2) + *(s16 *)(dialog + 0xAA) - 0xE;
    if (dialog[0xA4] != 0) {
        if (dialog[0xB4] == 0) {
            if ((PAD_STATES[dialog[0xA6]]->unk2 & 0x8000) && (s8)dialog[0xA5] != 1) {
                dialog[0xA5] = 1;
                r.x = *(s16 *)(dialog + 0xAC);
                r.y = y;
                r.w = *(s16 *)(dialog + 0xAE);
                r.h = 0xC;
                moveCursorHighlight((Unk800190F4 *)(dialog + 0x44), &r);
                playMenuSound(2);
            }
            if ((PAD_STATES[dialog[0xA6]]->unk2 & 0x2000) && (s8)dialog[0xA5] != 2) {
                dialog[0xA5] = 2;
                r.x = *(s16 *)(dialog + 0xB0);
                r.y = y;
                r.w = *(s16 *)(dialog + 0xB2);
                r.h = 0xC;
                moveCursorHighlight((Unk800190F4 *)(dialog + 0x44), &r);
                playMenuSound(2);
            }
        }
        drawText(*(s16 *)(dialog + 0xAC), y, *(s32 *)(dialog + 0x98), 7, *(s16 *)(dialog + 0x3A));
        drawText(*(s16 *)(dialog + 0xB0), y, *(s32 *)(dialog + 0x9C), 7, *(s16 *)(dialog + 0x3A));
        drawCursorHighlight((Unk800190F4 *)(dialog + 0x44), *(s16 *)(dialog + 0x3A));
    }
}
