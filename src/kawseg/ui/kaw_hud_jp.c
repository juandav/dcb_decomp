#include "common.h"
#include "game.h"
#include "dcb/heap.h"

/* jp's KAWSEG keeps only this here (kaw_hud.c is us's and eu's): the card
   polygons and the cursor are in jp's executable */

void func_801FF0B8(void) {
    if (DUEL->tutorial != 0) {
        freeHeapBlock(((SessionData *)SESSION_DATA)->unk8);
    }
}
