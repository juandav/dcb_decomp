#include "dcb/duel_session.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/battle_hud.h"
#include "dcb/card_motion.h"
#include "dcb/hud_panels.h"
#include "dcb/card_render.h"
#include "dcb/duel.h"
#include "dcb/cpu_decision.h"
#include "dcb/duel_setup.h"
#include "dcb/card_zones.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/scene3d.h"
#include "dcb/sound.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/text.h"

void initDuelState(s32 isCpuDuel) {
    void *block;

    D_801D833C = block = allocTaskHeapBlock(0x870);
    D_801D8340 = block = allocTaskHeapBlock(0x86C);
    (*(s32 *)((s8 *)D_801D8340 + 0x7F8)) = func_801F8854();
    (*(s8 *)((s8 *)D_801D8340 + 0x817)) = (s8) (rand() % 2);
    (*(s8 *)((s8 *)D_801D8340 + 0x818)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81B)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81C)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x810)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x81F)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x825)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x823)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x822)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x824)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x820)) = 0;
    func_801F8200();
    initDuelPlayers(isCpuDuel);
    (*(s8 *)((s8 *)D_801D8340 + 0x81D)) = -1;
}

void startDuelScene(void) {
    Unk800794F8 *camera;

    initScene3D(0);
    func_800149B8(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    func_80014C08(2);
    camera = (Unk800794F8 *)&GRAPHICS;
    camera->unk54 = 0;
    camera->unk56 = 0;
    camera->unk58 = 0;
    camera->unk7C = 0;
    camera->unk80 = 0;
    camera->unk84 = 0;
    camera->unk8E = 0;
    camera->unk90 = 0x1C0;
    camera->unk92 = 0;
    camera->unk94 = 0;
    camera->unk8C = -1;
    camera->unk74 = 1;
    (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 0;
    func_80014C08(2);
}

void spawnDuelTasks(s32 isCpuDuel) {
    s32 stageId;
    s32 stageArg;

    (*(s32 *)((s8 *)D_801D8340 + 0x58)) = func_801F8998(0, 0x26, 0x2E, 0xA, 1);
    func_800149B8(0x1E, -1, 0, 0x800, &runDuelTurnLoop, 0, 0, 0, 0);
    if ((isCpuDuel != 0) && ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) == 0)) {
        func_800149B8(0, -1, 0, 0x800, runCpuDecisionTask, 0, 0, 0, 0);
    }
    func_800149B8(0, -1, 0, 0x800, &runCardArtLoader, 0, 0, 0, 0);
    if (isCpuDuel != 0) {
        stageId = (*(u8 *)((s8 *)D_8006E054 + 0x72));
        stageArg = (*(u8 *)((s8 *)D_8006E054 + 0x71));
    } else {
        stageId = -1;
        stageArg = -1;
    }
    func_800149B8(0, -1, 0, 0x1000, &runDuelStageTask, stageId, stageArg, 0, 0);
}

void teardownDuelScene(void) {
    func_80014A00(0x19);
    func_801F848C();
    func_801F88E8();
    freeHeapBlocksByTag(0x7F);
}

void renderDuelFrame(void) {
    s16 fadeLevel;

    fadeLevel = (*(s16 *)((s8 *)D_801D8340 + 0x808));
    if (fadeLevel != 0) {
        renderDuelBackground(fadeLevel);
        renderStatusMessage((*(s16 *)((s8 *)D_801D8340 + 0x808)));
        renderHelpBar((*(s16 *)((s8 *)D_801D8340 + 0x808)));
    }
    renderPhaseBanner();
    tickBattleHud();
    renderBoardCards();
    if ((*(s32 *)((s8 *)D_801D8340 + 0x83C)) == 0) {
        if ((*(s32 *)((s8 *)D_801D8340 + 0x828)) != -1) {
            func_801F97F4();
        }
        func_801EB53C(D_801D83D1);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/duel/duel_session", DUEL_VRAM_CLEAR_RECT);

INCLUDE_ASM("asm/main/nonmatchings/duel/duel_session", runDuel);
