#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/frame_callback.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/prim_util.h"
#include "dcb/text.h"
#include "dcb/str_util.h"

void addFrameCallback(s32 callback) {
    s32 *slot;
    s32 entry;

    slot = &FRAME_CALLBACKS;
    if (callback != 0) {
loop_1:
        entry = (*(s32 *)((s8 *)slot + 0));
        if (entry != callback) {
            if (entry != 0) {
                slot += 1;
                goto loop_1;
            }
            (*(s32 *)((s8 *)slot + 0)) = callback;
            (*(s32 *)((s8 *)slot + 4)) = 0;
        }
    }
}

void removeFrameCallback(s32 callback) {
    s32 *slot;
    s32 entry;

    slot = &FRAME_CALLBACKS;
    if (callback == 0) {
        return;
    }
loop:
    entry = *slot;
    if (entry == callback) {
        goto found;
    }
    slot++;
    if (entry == 0) {
        return;
    }
    goto loop;
found:
    if ((*slot = slot[1]) == 0) {
        return;
    }
    slot++;
    goto found;
}

void clearFramePrimSlots(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[0] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[1] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[2] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[3] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[4] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[5] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[6] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[7] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[8] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[9] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[10] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[11] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[12] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[13] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[14] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[15] = 0;
    }
}
