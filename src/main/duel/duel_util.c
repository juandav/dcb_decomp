#include "dcb/duel_util.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/effect.h"
#include "dcb/archive.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/menu.h"
#include "dcb/prim3d.h"
#include "dcb/prim_util.h"
#include "dcb/sound.h"
#include "dcb/text.h"
#include "dcb/transform.h"

void waitDuelFrames(s32 frames) {
    while (frames > 0) {
        func_80014C08(FRAME_INTERVAL);
        if (((s8 *)D_801D8340)[0x823] == 0) {
            frames--;
        }
        if (((s8 *)D_801D8340)[0x815] != 0) {
            ((s8 *)D_801D8340)[0x815] = 0;
            func_80014A90();
            return;
        }
    }
}

s32 func_80033D9C(void) {
    void *pad;

    if ((*(s8 *)((s8 *)D_801D8340 + 0x815)) != 0) {
        (*(s8 *)((s8 *)D_801D8340 + 0x815)) = 0;
        func_80014A90();
        return -1;
    }
    if ((((u32) (*(u32 *)((s8 *)(DUEL_PLAYERS[(*(s8 *)((s8 *)D_801D8340 + 0x817))]) + 0x178)) >> 0x11) & 3) == 1) {
        pad = *PAD_STATES;
    } else {
        pad = PAD_STATES[(*(s8 *)((s8 *)D_801D8340 + 0x817))];
    }
    if (!((*(u16 *)((s8 *)pad + 0xA)) & 0x40)) {
        return 0;
    }
    playSoundEffect(0xA0);
    return 1;
}

void waitForCpuDecision(void) {
    s32 waited;

    (*(s32 *)((s8 *)D_801D8340 + 0x7FC)) = 0;
    while (1) {
        if ((*(s8 *)((s8 *)D_801D8340 + 0x815)) != 0) {
            (*(s8 *)((s8 *)D_801D8340 + 0x815)) = 0;
            func_80014A90();
            return;
        }
        func_80014C08(FRAME_INTERVAL);
        if ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) != 0) {
            (*(s8 *)((s8 *)D_801D8340 + 0x816)) = 0;
            return;
        }
        if ((*(s8 *)((s8 *)D_801D8340 + 0x816)) == 0) {
            return;
        }
        waited = (*(s32 *)((s8 *)D_801D8340 + 0x7FC))++;
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
    for (i = 0; i < 2; i++) {
        x = 0x80;
        y = i * -125 + 0x99;
        for (j = 0; j < 3; j++) {
            if (isSpritePoolFull() != 0) {
                return;
            }
            if (ATTACK_ICON_TIMER == 0 && ((*(u32 *)(DUEL_PLAYERS[i] + 0x178) >> 2) & 3) != j) {
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
    u32 playerFlags;

    playerFlags = (*(u32 *)((s8 *)(DUEL_PLAYERS[(*(s8 *)((s8 *)D_801D8340 + 0x817))]) + 0x178));
    padIndex = (playerFlags >> 0x11) & 1;
    if (((playerFlags >> 0x11) & 3) == 1) {
        padIndex = 0;
    }
    runDialogForPad(&D_801D8278, padIndex);
}

INCLUDE_RODATA("asm/main/nonmatchings/duel/duel_util", OVERLAY_LOAD_ADDR);
