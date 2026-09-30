#include "dcb/cpu_decision.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/duel.h"
#include "dcb/battle_hud.h"
#include "dcb/card_motion.h"
#include "dcb/hud_panels.h"
#include "dcb/duel_session.h"
#include "dcb/card_db.h"
#include "dcb/duel_setup.h"
#include "dcb/card_zones.h"
#include "dcb/effect_object.h"
#include "dcb/effect_prims.h"
#include "dcb/duel_util.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/menu.h"
#include "dcb/dialog.h"
#include "dcb/partner_level.h"
#include "dcb/hacking_shell.h"
#include "dcb/game_exit.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
#include "dcb/scroll_bg.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"

/* KAWSEG's CPU decision routines */
s32 func_801E19EC(s32);
s32 func_801E2100(s32);
s32 func_801E157C(s32);
s32 func_801E3FF4(s32);
s32 func_801E3AF8(s32);
s32 func_801E363C(s32);
s32 func_801DFFD4(s32);
s32 func_801E4E08(s32);
s32 func_801E3C00(s32);
s32 func_801E0CCC(s32);
s32 func_801E4E58(s32);
void func_801E5710(void);

/* clears the request and waits a frame for the next one */
#define WAIT_FOR_CPU_REQUEST()           \
    do {                                 \
        DUEL->cpuRequest = 0;            \
        waitFrames(FRAME_INTERVAL);   \
    } while (0)

void runCpuDecisionTask(void) {
    s32 r;

    DUEL->stopCpuTask = 0;
    WAIT_FOR_CPU_REQUEST();
    if (DUEL->stopCpuTask == 0) {
        do {
            switch (DUEL->cpuRequest) {
            case 0:
                break;
            case 1:
                DUEL->cpuResult = func_801E19EC(DUEL->cpuPlayer);
                break;
            case 2:
                DUEL->cpuResult = func_801E2100(DUEL->cpuPlayer);
                break;
            case 3:
                r = func_801E157C(DUEL->cpuPlayer);
                func_801E3FF4(DUEL->cpuPlayer);
                func_801E3AF8(r);
                DUEL->cpuResult = func_801E363C(DUEL->cpuPlayer);
                break;
            case 4:
                if (func_801DFFD4(DUEL->cpuPlayer)) {
                    DUEL->cpuResult = func_801E4E08(DUEL->cpuPlayer);
                } else {
                    DUEL->cpuResult = -1;
                }
                break;
            case 5:
                DUEL->cpuResult = func_801E3C00(DUEL->cpuPlayer);
                break;
            case 6:
                func_801E0CCC(DUEL->cpuPlayer);
                func_801E4E58(DUEL->cpuPlayer);
                waitFrames(60);
                break;
            case 7:
                func_801E0CCC(DUEL->cpuPlayer);
                func_801E5710();
                break;
            }
            WAIT_FOR_CPU_REQUEST();
        } while (DUEL->stopCpuTask == 0);
    }
    DUEL->stopCpuTask = 0;
}
