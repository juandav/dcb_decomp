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

/* FRAME_CALLBACKS is a 0-terminated list of functions called every frame;
   addFrameCallback appends one unless it's already in the list */
void addFrameCallback(s32 callback) {
    s32 *slot;
    s32 entry;

    slot = &FRAME_CALLBACKS;
#if VERSION_US || VERSION_EU
    /* jp doesn't check for a null callback */
    if (callback == 0) {
        return;
    }
#endif
loop_1:
    entry = slot[0];
    if (entry != callback) {
        if (entry != 0) {
            slot += 1;
            goto loop_1;
        }
        slot[0] = callback;
        slot[1] = 0;
    }
}

/* the same search and shift; the match depends on the form of the loops:
   each version's compiler needs its own to lay them out as the original does */
#if VERSION_US
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
    /* move the rest of the list down over it */
    if ((*slot = slot[1]) == 0) {
        return;
    }
    slot++;
    goto found;
}
#elif VERSION_JP || VERSION_EU
void removeFrameCallback(s32 callback) {
    s32 *slot;

    slot = &FRAME_CALLBACKS;
#if VERSION_EU
    if (callback == 0) {
        return;
    }
#endif
    while (*slot != callback) {
        if (*slot == 0) {
            return;
        }
        slot++;
    }
    /* move the rest of the list down over it */
    while ((*slot = slot[1]) != 0) {
        slot++;
    }
}
#else
#error "main/system/frame_callback: version not checked"
#endif

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
#if VERSION_JP
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[16] = 0;
#endif
    }
}
