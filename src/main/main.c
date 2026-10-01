#include "common.h"
#include "game.h"
#include "dcb/main.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/vblank.h"
#include "dcb/screen_copy.h"
#include "dcb/render_loop.h"
#include "dcb/boot.h"

#if VERSION_US
s32 UNUSED_MAIN_WORDS[2] = { 0, 0 };
#endif

/*
 * The memory sizes PsyQ's startup code (__SN_ENTRY_POINT) reads before it
 * calls main(): how much RAM the machine has and how much of its top the
 * stack gets; the heap is what lies between the end of .bss and the stack.
 * Nothing in the game reads them.
 */
u32 _ramsize = 0x200000; /* the PlayStation's 2 MB */
u32 _stacksize = 0x8000;   /* 32 KB */

#if VERSION_JP
/* libsnd's attribute table for one sequence (SS_SEQ_TABSIZ bytes) */
extern s32 BOOT_SEQ_ATTR_TABLE[0xB0 / 4];

/* jp's main() starts the CD, the sound and the pads itself, which us's
   runMainTask does; initGraphics resets the GPU and clears VRAM */
int main(void) {
    ResetCallback();
    ResetGraph(0);
    CdInit();
    SsInit();
    SsSetTableSize(BOOT_SEQ_ATTR_TABLE, 1, 1);
    resetHeap(1);
    initDiscDrive();
    initGraphics();
    initMemoryCard();
    initPads();
    ChangeClearPad(0);
    launchTaskScheduler(1, 0x400, runMainTask, 0, 0, 0, 0);
    for (;;) {
        rand();
    }
}
#elif VERSION_US || VERSION_EU
#if VERSION_EU
/* libetc's video modes */
#define MODE_PAL 1
long SetVideoMode(long mode);
#endif

int main(void) {
    Rect16 vramRect;

#if VERSION_EU
    SetVideoMode(MODE_PAL);
#endif
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
    launchTaskScheduler(1, 0x400, runMainTask, 0, 0, 0, 0);
    for (;;) {
        rand();
    }
}
#endif
