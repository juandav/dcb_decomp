#include "common.h"
#include "game.h"
#include "dcb/open_title.h"
#include "dcb/heap.h"
#include "dcb/text.h"
#include "dcb/frame_callback.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/memcard.h"
#include "dcb/scroll_bg.h"
#include "dcb/prim_util.h"
#include "dcb/sound_play.h"
#include "dcb/fade.h"
#include "dcb/sound.h"
#include "dcb/player_data.h"
#include "dcb/openseg.h"

extern s32 OPEN_TITLE_SPIN_RADIUS;
extern s32 OPEN_TITLE_SPIN_ANGLE;
extern s32 OPEN_TITLE_SPIN_SIZE;
extern s32 OPEN_TITLE_SPIN_SHADE;
extern s32 OPEN_TITLE_STATE;
extern s32 OPEN_TITLE_TIMER;
extern s32 OPEN_TITLE_BANNER_X;
extern s32 OPEN_TITLE_LOGO_RISE;
extern s32 OPEN_TITLE_FOOTER_SHADE;
extern s32 D_801F52B0;
extern s32 OPEN_PRESS_START_SHADE;
extern s32 OPEN_PRESS_START_STEP;
extern u8 D_801F52BC[3];
extern u8 D_801F52C0[3];
extern s32 D_801F52C4;
extern s32 D_801F52C8;
extern POLY_FT4 OPEN_TITLE_SPIN_QUADS[][2];
extern POLY_F4 OPEN_TITLE_BAND_QUADS[2];
extern DR_MODE OPEN_TITLE_BAND_MODES[2];

s32 OPEN_TITLE_OPTION_DIMMED[3] = { 1, 0, 1 };

void OPEN_drawTitleSpinQuad(s32 idx) {
    POLY_FT4 *poly;
    s32 cx;
    s32 cy;
    s32 angle;

    OPEN_TITLE_SPIN_ANGLE += 32;
    cx = rsin(OPEN_TITLE_SPIN_ANGLE) * OPEN_TITLE_SPIN_RADIUS / 4096 + 160;
    cy = OPEN_TITLE_SPIN_RADIUS * (rcos(OPEN_TITLE_SPIN_ANGLE) << 1) / 4096 + 60;
    if (idx & 1) {
        angle = OPEN_TITLE_SPIN_ANGLE;
    } else {
        angle = -OPEN_TITLE_SPIN_ANGLE;
    }
    poly = &OPEN_TITLE_SPIN_QUADS[idx][FRAME_BUFFER_INDEX];
    initPrimByType(0xC, poly, 1, 0);
    poly->r0 = OPEN_TITLE_SPIN_SHADE;
    poly->g0 = OPEN_TITLE_SPIN_SHADE;
    poly->b0 = OPEN_TITLE_SPIN_SHADE;
    poly->u0 = 0;
    poly->v0 = 0;
    poly->u1 = 0xEF;
    poly->v1 = 0;
    poly->u2 = 0;
    poly->v2 = 0xEF;
    poly->u3 = 0xEF;
    poly->v3 = 0xEF;
    poly->x0 = cx + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rsin(angle) / 4096;
    poly->y0 = cy + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rcos(angle) / 4096;
    poly->x1 = cx + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rsin(angle + 0x400) / 4096;
    poly->y1 = cy + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rcos(angle + 0x400) / 4096;
    poly->x2 = cx + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rsin(angle + 0xC00) / 4096;
    poly->y2 = cy + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rcos(angle + 0xC00) / 4096;
    poly->x3 = cx + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rsin(angle + 0x800) / 4096;
    poly->y3 = cy + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rcos(angle + 0x800) / 4096;
    poly->tpage = 0x2A;
    poly->clut = 0x3E64;
    addPrim(&CURRENT_FRAME_BUFFER->ot[0], poly);
}

void OPEN_drawSprite(s32 x, s32 y, s32 texX, s32 texY, s32 w, s32 h, s32 clutX, s32 clutY, s32 texMode,
                   s32 semiTrans, s32 blendMode, s32 shade, s32 otIndex) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = (texX % 64) * (4 >> texMode);
        CUR_SPRT->sp.v0 = texY % 256;
        CUR_SPRT->sp.clut = getClut(clutX, clutY);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, semiTrans);
        CUR_SPRT->sp.r0 = shade;
        CUR_SPRT->sp.g0 = shade;
        CUR_SPRT->sp.b0 = shade;
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(texMode, blendMode, texX, texY));
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/* libgpu's setDrawTPage */
#define setDrawTPage(p, dfe, dtd, tpage) (setlen(p, 1), ((u32 *)(p))[1] = _get_mode(dfe, dtd, tpage))

void OPEN_drawTitleScreen(void) {
    s32 shade;
    s32 i; /* also the shade of the dark band */

    switch (OPEN_TITLE_STATE) {
    case 7:
        if (OPEN_PRESS_START_SHADE > 0x80) {
            shade = 0x80;
        } else {
            shade = OPEN_PRESS_START_SHADE;
        }
        OPEN_drawSprite(0x48, 0xAC, 0x200, 0x98, 0xB0, 0x10, 0x240, 0xF8, 0, 1, 1, shade, 0xA);
        OPEN_drawSprite(0x48, 0xAC, 0x200, 0x98, 0xB0, 0x10, 0x250, 0xF8, 0, 1, 2, shade, 0xA);
        break;
    case 8:
        for (i = 0; i < 3; i++) {
            OPEN_drawSprite(i * 100 + 0xC, 0x9C, 0x2C0, i * 0x1C + 0x80, 0x60, 0x1C, OPEN_TITLE_OPTION_DIMMED[i] * 16 + 0x2C0, 0xFA, 0, 0, 0, 0x80, 5);
        }
        break;
    }
    switch (OPEN_TITLE_STATE) {
    case 8:
        OPEN_TITLE_LOGO_RISE += 2;
        if (OPEN_TITLE_LOGO_RISE > 16) {
            OPEN_TITLE_LOGO_RISE = 16;
        }
    case 7:
        if (OPEN_TITLE_STATE == 7) {
            OPEN_TITLE_LOGO_RISE -= 2;
            if (OPEN_TITLE_LOGO_RISE < 0) {
                OPEN_TITLE_LOGO_RISE = 0;
            }
        }
        i = OPEN_TITLE_LOGO_RISE * 8;
        initPrimByType(8, &OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX], 1, 0);
        OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX].r0 = i;
        OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX].g0 = i;
        OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX].b0 = i;
        OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX].x0 = 0;
        OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX].y0 = 0x9A;
        OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX].x1 = 0x140;
        OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX].y1 = 0x9A;
        OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX].x2 = 0;
        OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX].y2 = 0xBA;
        OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX].x3 = 0x140;
        OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX].y3 = 0xBA;
        setDrawTPage(&OPEN_TITLE_BAND_MODES[FRAME_BUFFER_INDEX], 0, 0, 0x40);
        addPrim(&CURRENT_FRAME_BUFFER->ot[6], &OPEN_TITLE_BAND_QUADS[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[6], &OPEN_TITLE_BAND_MODES[FRAME_BUFFER_INDEX]);
    case 6:
        if (OPEN_TITLE_STATE == 6) {
            if (++OPEN_TITLE_TIMER > 60) {
                OPEN_TITLE_STATE = 7;
                OPEN_TITLE_TIMER = 0;
            }
        }
        OPEN_TITLE_FOOTER_SHADE += 4;
        if (OPEN_TITLE_FOOTER_SHADE > 0x80) {
            OPEN_TITLE_FOOTER_SHADE = 0x80;
        }
        OPEN_drawSprite(0x20, 0xC8, 0x200, 0xA8, 0xFF, 0x20, 0x240, 0xFA, 0, 1, 1, OPEN_TITLE_FOOTER_SHADE, 0xA);
        OPEN_drawSprite(0x20, 0xC8, 0x200, 0xA8, 0xFF, 0x20, 0x250, 0xFA, 0, 1, 2, OPEN_TITLE_FOOTER_SHADE, 0xA);
    case 5:
        if (OPEN_TITLE_STATE == 5) {
            OPEN_TITLE_BANNER_X -= 20;
            if (OPEN_TITLE_BANNER_X < 0) {
                OPEN_TITLE_STATE = 6;
                OPEN_TITLE_TIMER = 0;
                OPEN_TITLE_BANNER_X = 0;
            }
        }
        OPEN_drawSprite(OPEN_TITLE_BANNER_X, 0x78 - OPEN_TITLE_LOGO_RISE, 0x2C0, 0, 0x100, 0x30, 0x140, 0xF8, 1, 1, 0, 0x80, 5);
        OPEN_drawSprite(OPEN_TITLE_BANNER_X + 0x100, 0x78 - OPEN_TITLE_LOGO_RISE, 0x340, 0, 0x40, 0x30, 0x140, 0xF8, 1, 1, 0, 0x80, 5);
    case 4:
        if (OPEN_TITLE_STATE == 4) {
            OPEN_TITLE_SPIN_SHADE -= 16;
            if (OPEN_TITLE_SPIN_SHADE < 0) {
                OPEN_TITLE_SPIN_SHADE = 0;
            }
            OPEN_TITLE_SPIN_SIZE += 25;
            if (++OPEN_TITLE_TIMER > 60) {
                OPEN_TITLE_STATE = 5;
                OPEN_TITLE_TIMER = 0;
                OPEN_TITLE_BANNER_X = 0x140;
                OPEN_TITLE_LOGO_RISE = 0;
            }
        }
        OPEN_drawSprite(0x20, 0x10 - OPEN_TITLE_LOGO_RISE, 0x200, 0, 0x100, 0x88, 0x140, 0xF9, 1, 1, 0, 0x80, 0xA);
    case 2:
    case 3:
        if (OPEN_TITLE_STATE == 3) {
            if (++OPEN_TITLE_TIMER > 60) {
                OPEN_TITLE_STATE = 4;
                OPEN_TITLE_TIMER = 0;
            }
        }
        OPEN_drawSprite(0, 0, 0x140, 0, 0x100, 0xF0, 0x140, 0xFA, 1, 0, 0, 0x80, 0xA);
        OPEN_drawSprite(0x100, 0, 0x1C0, 0, 0x40, 0xF0, 0x140, 0xFA, 1, 0, 0, 0x80, 0xA);
    case 1:
        if (OPEN_TITLE_STATE == 1) {
            if (++OPEN_TITLE_TIMER > 100) {
                setScreenFadeParams(1, 1, 8);
                OPEN_TITLE_STATE = 3;
                OPEN_TITLE_TIMER = 0;
            }
        }
    case 0:
        if (OPEN_TITLE_STATE == 0) {
            OPEN_TITLE_TIMER++;
            OPEN_TITLE_SPIN_SHADE += 8;
            if (OPEN_TITLE_SPIN_SHADE > 0x40) {
                OPEN_TITLE_SPIN_SHADE = 0x40;
            }
            OPEN_TITLE_SPIN_SIZE += 2;
            if (OPEN_TITLE_SPIN_SIZE > 100) {
                OPEN_TITLE_SPIN_SIZE = 100;
            }
            OPEN_TITLE_SPIN_RADIUS -= 2;
            if (OPEN_TITLE_SPIN_RADIUS < 0) {
                OPEN_TITLE_SPIN_RADIUS = 0;
                spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 1, 4, 0);
                OPEN_TITLE_TIMER = 0;
                OPEN_TITLE_STATE = 1;
            }
        }
        OPEN_drawTitleSpinQuad(0);
        OPEN_drawTitleSpinQuad(1);
        break;
    }
}

/* moves the title menu's cursor through its three options, wrapping around:
   step 2 goes to the previous one (Left), 4 to the next (Right). The match
   depends on the macro: in eu, its do-while's loop notes weigh choice's uses
   more, so choice gets its register before idle, as in the original */
#define STEP_TITLE_CURSOR(step) \
    do {                        \
        idle = 0;               \
        playMenuSound(2);       \
        choice += (step);       \
        choice %= 3;            \
    } while (0)

#if VERSION_US || VERSION_EU
void OPEN_runTitleScreen(s32 parentTask) {
    /* a dialog buffer, declared as SUB_tickDeckSlots does */
    u8 dialog[0xC0]; /* unused, but it is in the original stack frame */
    /* never read: only its empty string is left in .rodata. It is written
       "\0" so that it stays apart from the "" of the arena table
       OPEN_ARENA_NAMES, which the original built in another file */
    char *name;
    u32 *arc;
    s32 i;
    s32 choice;
    s32 idle;
    s32 fade;
    s32 done;

    loadMusicTrack(0, 0x66, 0x7F);
    i = 0;
    hideScrollingBackground();
    loadScrollingBackground();
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\TITLE.ARC", getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        waitFrames(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    name = "\0";
    choice = 1;
    for (i = 0; i < 3; i++) {
        OPEN_TITLE_OPTION_DIMMED[i] = 1;
    }
    OPEN_TITLE_OPTION_DIMMED[choice] = 0;
    fade = 0;
    playLoadedMusic(0);
    OPEN_TITLE_FOOTER_SHADE = 0;
    D_801F52B0 = 0;
    OPEN_PRESS_START_SHADE = 0x80;
    OPEN_PRESS_START_STEP = 4;
    OPEN_TITLE_TIMER = 0;
    OPEN_TITLE_SPIN_RADIUS = 0xA0;
    OPEN_TITLE_SPIN_ANGLE = 0;
    OPEN_TITLE_SPIN_SIZE = 0;
    OPEN_TITLE_SPIN_SHADE = 0;
    OPEN_TITLE_STATE = 0;
    done = 0;
    D_801F52BC[0] = 0;
    D_801F52BC[1] = 0x80;
    D_801F52BC[2] = 0x80;
    D_801F52C0[0] = 0;
    D_801F52C0[1] = 0x80;
    D_801F52C0[2] = 0x80;
    D_801F52C4 = 0;
    D_801F52C8 = 1;
    resetPlayerData();
    addFrameCallback((s32)OPEN_drawTitleScreen);
    idle = 0;
    OPEN_TITLE_LOGO_RISE = 0;
    do {
        waitFrames(FRAME_INTERVAL);
        switch (OPEN_TITLE_STATE) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if (PAD_STATES[0]->pressed & 0x800) {
                idle = 0;
                fade = 1;
                OPEN_TITLE_BANNER_X = 0;
                OPEN_TITLE_LOGO_RISE = 0x20;
                OPEN_TITLE_SPIN_SHADE = 0;
                OPEN_TITLE_STATE = 8;
                playMenuSound(1);
            }
            break;
        case 6:
        case 7:
            OPEN_PRESS_START_SHADE += OPEN_PRESS_START_STEP;
            if (OPEN_PRESS_START_SHADE >= 0x100) {
                OPEN_PRESS_START_SHADE = 0xFF;
                OPEN_PRESS_START_STEP = -4;
            } else if (OPEN_PRESS_START_SHADE < 0) {
                OPEN_PRESS_START_SHADE = 0;
                OPEN_PRESS_START_STEP = 4;
            }
            if (PAD_STATES[0]->pressed & 0x800) {
                idle = 0;
                OPEN_TITLE_STATE = 8;
                playMenuSound(1);
            }
            idle++;
            break;
        case 8:
            if (fade) {
                if (isScreenFadeActive()) {
                    setScreenFadeParams(1, 1, 8);
                }
                fade = 0;
            }
            if (isScreenFadeActive() == 0) {
                idle++;
                if ((u16)PAD_STATES[0]->pressed & 0x8000) {
                    STEP_TITLE_CURSOR(2);
                } else if (PAD_STATES[0]->pressed & 0x2000) {
                    STEP_TITLE_CURSOR(4);
                }
                for (i = 0; i < 3; i++) {
                    OPEN_TITLE_OPTION_DIMMED[i] = 1;
                }
                OPEN_TITLE_OPTION_DIMMED[choice] = 0;
                if (PAD_STATES[0]->pressed & 0x10) {
                    idle = 0;
                    playMenuSound(0);
                    OPEN_TITLE_STATE = 7;
                    OPEN_PRESS_START_SHADE = 0;
                    OPEN_PRESS_START_STEP = 4;
                } else if (PAD_STATES[0]->pressed & 0x40) {
                    idle = 0;
                    playMenuSound(1);
                    done = 1;
                }
            }
            break;
        }
    } while (idle < 0xE11 && !done);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, NULL, 2, 8, 0);
    waitFrames(20);
    removeFrameCallback((s32)OPEN_drawTitleScreen);
    stopScreenFade();
    waitFrames(10);
    if (idle >= 0xE11) {
        stopMusic();
        resumeTask(0);
        exitTask();
    }
    resumeTask(parentTask, choice);
}
#else
#error "openseg/title/open_title: version not checked"
#endif
