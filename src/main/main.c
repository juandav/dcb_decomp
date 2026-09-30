#include "common.h"
#include "game.h"
#include "dcb/main.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/vblank.h"
#include "dcb/screen_copy.h"
#include "dcb/render_loop.h"
#include "dcb/boot.h"

s32 D_8006DD3C[2] = { 0, 0 };

/*
 * The memory sizes PsyQ's startup code (__SN_ENTRY_POINT) reads before it
 * calls main(): how much RAM the machine has and how much of its top the
 * stack gets; the heap is what lies between the end of .bss and the stack.
 * Nothing in the game reads them.
 */
u32 D_8006DD44 = 0x200000; /* _ramsize: the PlayStation's 2 MB */
u32 D_8006DD48 = 0x8000;   /* _stacksize: 32 KB */

int main(void) {
    Rect16 vramRect;

    ResetCallback();
    VSync(0);
    SetDispMask(0);
    GsInitGraph(320, 240, 0, 0, 0);
    vramRect.x = 0;
    vramRect.y = 0;
    vramRect.w = 640;
    vramRect.h = 511;
    ClearImage(&vramRect, 0, 0, 0);
    DrawSync(0);
    SsInit();
    resetHeap(1);
    func_800149A8(1, 0x400, runMainTask, 0, 0, 0, 0);
    for (;;) {
        rand();
    }
}
