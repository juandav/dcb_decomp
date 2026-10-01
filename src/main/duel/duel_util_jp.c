#include "dcb/duel_util.h"
#include "common.h"
#include "game.h"
#include "dcb/pad.h"
#include "dcb/task.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/kaw_hand.h"

/* jp's duel waits (duel_util.c is us's and eu's): the waits hold while the
   duel is paused, the turn player confirms with circle, and us's wait
   for the CPU has no tutorial case. KAWSEG's KAW_drawSprite comes first, in the
   executable */

void KAW_drawSprite(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 clutX, s32 clutY, s32 tp, s32 semi, s32 abr, s32 brightness,
                   s32 otz) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = (u % 64) * (4 >> tp);
        CUR_SPRT->sp.v0 = v % 256;
        CUR_SPRT->sp.clut = getClut(clutX, clutY);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, semi);
        CUR_SPRT->sp.r0 = brightness;
        CUR_SPRT->sp.g0 = brightness;
        CUR_SPRT->sp.b0 = brightness;
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(tp, abr, u, v));
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void waitDuelFrames(s32 frames) {
    while (1) {
        if (DUEL->stopTurnLoop != 0) {
            DUEL->stopTurnLoop = 0;
            exitTask();
            return;
        }
        if (DUEL->helpOpen | DUEL->quit) {
            waitFrames(FRAME_INTERVAL);
            continue;
        }
        waitFrames(frames);
        return;
    }
}

void isCrossPressedByTurnPlayer(void) {
    PadState *pad;

    while (1) {
        if (DUEL->stopTurnLoop != 0) {
            DUEL->stopTurnLoop = 0;
            exitTask();
            return;
        }
        waitFrames(FRAME_INTERVAL);
        if (PLAYER(DUEL->turnPlayer)->controller == 2) {
            pad = PAD_STATES[0];
        } else {
            pad = PAD_STATES[DUEL->turnPlayer];
        }
        if (pad->rawPressed & PAD_CIRCLE) {
            break;
        }
    }
    playSoundEffect(0xA0);
}

/* waits a second, then for the turn player's circle (jp's KAWSEG calls it
   where us waits for the CPU) */
void waitForCpuDecision(void) {
    PadState *pad;
    s32 wait;

    wait = 60;
    while (1) {
        if (DUEL->stopTurnLoop != 0) {
            DUEL->stopTurnLoop = 0;
            exitTask();
            return;
        }
        waitFrames(FRAME_INTERVAL);
        if (wait == 0) {
            if (PLAYER(DUEL->turnPlayer)->controller == 2) {
                pad = PAD_STATES[0];
            } else {
                pad = PAD_STATES[DUEL->turnPlayer];
            }
            if (pad->rawPressed & PAD_CIRCLE) {
                return;
            }
        } else {
            wait--;
        }
    }
}

/* us's waitForCpuDecision, without the tutorial's case: jp's turn loop
   calls it where us's does */
void waitForCpuDecisionNoTutorial(void) {
    s32 waited;

    DUEL->cpuWaitFrames = 0;
    while (1) {
        if (DUEL->stopTurnLoop != 0) {
            DUEL->stopTurnLoop = 0;
            exitTask();
            return;
        }
        waitFrames(FRAME_INTERVAL);
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
