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
#include "dcb/overlay_calls.h"

/* answers the duel's requests for the CPU's decisions, one per frame, until
   the duel stops it */
void runCpuDecisionTask(void) {
    s32 r;

    DUEL->stopCpuTask = 0;
    DUEL->cpuRequest = 0;
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (DUEL->stopCpuTask != 0) {
            break;
        }
        switch (DUEL->cpuRequest) {
        case 0:
            break;
        case 1:
            DUEL->cpuResult = KAW_decideRedraw(DUEL->cpuPlayer);
            break;
        case 2:
            DUEL->cpuResult = KAW_chooseDigimonToPlace(DUEL->cpuPlayer);
            break;
        case 3:
            r = KAW_planDigivolves(DUEL->cpuPlayer);
            KAW_planDigivolveOptions(DUEL->cpuPlayer);
            KAW_selectDigivolvePlan(r);
            DUEL->cpuResult = KAW_chooseDpCard(DUEL->cpuPlayer);
            break;
        case 4:
            if (KAW_countHandDigivolves(DUEL->cpuPlayer)) {
                DUEL->cpuResult = KAW_chooseDigivolveOption(DUEL->cpuPlayer);
            } else {
                DUEL->cpuResult = -1;
            }
            break;
        case 5:
            DUEL->cpuResult = KAW_chooseDigivolveTarget(DUEL->cpuPlayer);
            break;
        case 6:
            KAW_simulateBattles(DUEL->cpuPlayer);
            KAW_chooseAttack(DUEL->cpuPlayer);
            waitFrames(60);
            break;
        case 7:
            KAW_simulateBattles(DUEL->cpuPlayer);
            KAW_chooseSupportCard();
            break;
        }
        DUEL->cpuRequest = 0;
    }
    DUEL->stopCpuTask = 0;
}
