#include "dcb/duel_util.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/scroll_bg.h"
#include "dcb/effect_object.h"
#include "dcb/effect_prims.h"
#include "dcb/archive.h"
#include "dcb/decompress.h"
#include "dcb/sort.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/menu.h"
#include "dcb/dialog.h"
#include "dcb/prim3d.h"
#include "dcb/prim_util.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/transform.h"

void waitDuelFrames(s32 frames) {
    while (frames > 0) {
        waitFrames(FRAME_INTERVAL);
        if (DUEL->menuOpen == 0) {
            frames--;
        }
        if (DUEL->stopTurnLoop != 0) {
            DUEL->stopTurnLoop = 0;
            exitTask();
            return;
        }
    }
}

s32 isCrossPressedByTurnPlayer(void) {
    PadState *pad;

    if (DUEL->stopTurnLoop != 0) {
        DUEL->stopTurnLoop = 0;
        exitTask();
        return -1;
    }
    if (PLAYER(DUEL->turnPlayer)->controller == 1) {
        pad = PAD_STATES[0];
    } else {
        pad = PAD_STATES[DUEL->turnPlayer];
    }
    if (!(pad->pressed & 0x40)) {
        return 0;
    }
    playSoundEffect(0xA0);
    return 1;
}

void waitForCpuDecision(void) {
    s32 waited;

    DUEL->cpuWaitFrames = 0;
    while (1) {
        if (DUEL->stopTurnLoop != 0) {
            DUEL->stopTurnLoop = 0;
            exitTask();
            return;
        }
        waitFrames(FRAME_INTERVAL);
        if (DUEL->tutorial != 0) {
            DUEL->cpuRequest = 0;
            return;
        }
        if (DUEL->cpuRequest == 0) {
            return;
        }
        /* give up once cpuWaitFrames passes 240 */
        waited = DUEL->cpuWaitFrames++;
        if (waited >= 0xF1) {
            return;
        }
    }
}

void renderAttackChoiceIcons(void) {
    s32 i;
    s32 j;
    s32 x;
    s32 y;

    if (ATTACK_ICON_TIMER != 0) {
        ATTACK_ICON_TIMER--;
    }
    /* each player's three attack icons move in from their origins; once the
       timer runs out only the chosen attack is left */
    for (i = 0; i < 2; i++) {
        x = 0x80;
        y = i * -125 + 0x99;
        for (j = 0; j < 3; j++) {
            if (isSpritePoolFull() != 0) {
                return;
            }
            if (ATTACK_ICON_TIMER == 0 && PLAYER(i)->attackChoice != j) {
                continue;
            }
            CUR_SPRT->sp.x0 = x + (x - ATTACK_ICON_ORIGIN_X[j]) * ATTACK_ICON_TIMER / 32;
            CUR_SPRT->sp.y0 = y + (y - ATTACK_ICON_ORIGIN_Y[i][j]) * ATTACK_ICON_TIMER / 32;
            CUR_SPRT->sp.u0 = j * 64 + 64;
            CUR_SPRT->sp.v0 = 0xBA;
            CUR_SPRT->sp.clut = 0x7FF0;
            CUR_SPRT->sp.w = 64;
            CUR_SPRT->sp.h = 64;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = x;
            CUR_SPRT->sp.g0 = x;
            CUR_SPRT->sp.b0 = x;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1D);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

void runDuelMessageWindow(void) {
    s32 padIndex;
    Player *player;

    player = PLAYER(DUEL->turnPlayer);
    padIndex = player->controller & 1;
    if (player->controller == 1) {
        padIndex = 0;
    }
    runDialogForPad(&DUEL_DIALOG, padIndex);
}

/*
 * The overlays (openseg, saiseg, kawseg...) are loaded right after the
 * executable's .bss; the loaders read the address from here. It is const
 * here, so it goes to .rodata, while game.h declares it a plain s32 for the
 * loaders: declared const there, GCC would keep the value across calls,
 * which the original code doesn't (GCC only warns about the mismatch).
 */
extern u8 OVERLAY_AREA[];
const s32 OVERLAY_LOAD_ADDR = (s32)OVERLAY_AREA;
