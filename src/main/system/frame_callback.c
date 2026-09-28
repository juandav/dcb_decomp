#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/frame_callback.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/prim_util.h"
#include "dcb/transform.h"
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
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[0] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[1] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[2] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[3] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[4] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[5] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[6] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[7] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[8] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[9] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[10] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[11] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[12] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[13] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[14] = 0;
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[15] = 0;
    }
}
