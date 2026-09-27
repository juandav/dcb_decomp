#include "psyq.h"

extern long D_8006F594[2];
extern long D_801D8614;
long GetVideoMode(void);

void SsSetTickMode(long tick_mode) {
    long video = GetVideoMode();

    if (tick_mode & SS_NOTICK) {
        D_8006F594[1] = 1;
        D_8006F594[0] = tick_mode & 0xFFF;
    } else {
        D_8006F594[1] = 0;
        D_8006F594[0] = tick_mode;
    }
    if (D_8006F594[0] < SS_TICKMODE_MAX) {
        switch (D_8006F594[0]) {
        case SS_TICK50:
            D_801D8614 = 50;
            if (video == MODE_PAL) {
                D_8006F594[0] = SS_TICKVSYNC;
            } else {
                D_8006F594[0] = D_801D8614;
            }
            break;
        case SS_TICK60:
            D_801D8614 = 60;
            if (video == MODE_NTSC) {
                D_8006F594[0] = SS_TICKVSYNC;
            } else {
                D_8006F594[0] = D_801D8614;
            }
            break;
        case SS_TICK120:
            D_801D8614 = 120;
            break;
        case SS_TICK240:
            D_801D8614 = 240;
            break;
        case SS_TICKVSYNC:
            if (video == MODE_NTSC) {
                D_801D8614 = 60;
            } else if (video == MODE_PAL) {
                D_801D8614 = 50;
            } else {
                D_801D8614 = 60;
            }
            break;
        case SS_NOTICK0:
            if (video == MODE_NTSC) {
                D_801D8614 = 60;
            } else if (video == MODE_PAL) {
                D_801D8614 = 50;
            } else {
                D_801D8614 = 60;
            }
            break;
        default:
            D_801D8614 = 60;
            break;
        }
    } else {
        D_801D8614 = D_8006F594[0];
    }
}

/* ASPSX padded the jump table of the object as well */
__asm__(".section .rodata\n\t.space 8\n");

OBJECT_END(2);
