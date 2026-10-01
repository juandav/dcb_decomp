#include "common.h"
#include "game.h"
#include "dcb/evo_fusion_script.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/window.h"
#include "dcb/loader.h"
#include "dcb/script.h"
#include "dcb/sound_play.h"
#include "dcb/evo_card_list.h"
#include "dcb/evo_trays.h"
#include "dcb/evo_text.h"
#include "dcb/evo_type_choice.h"
#include "dcb/evo_lists.h"
#include "dcb/evo_rewards.h"
#include "dcb/evo_fusion_result.h"

extern u8 EVO_SCRIPT_HALTED;

void EVO_runFusionScript(EvoProgram *data) {
    s32 result;

    if (EVO_SCRIPT_HALTED == 1) {
        return;
    }
    do {
        result = runScriptToNextEvent(data->script, data->vars);
        if (result == 1) {
            switch (data->script->eventOp) {
            case 10:
                switch (data->script->eventArg) {
                case 0:
                    if (EVO_addTextLine((u8 *)data->vars[4]) == -1) {
                        EVO_FUSION.scriptState = 1;
                        return;
                    }
                    break;
                case 1:
                    return;
                case 2:
                    EVO_toggleMessageWindows(0);
                    break;
                case 4:
                    EVO_openFusionTypeChoice();
                    return;
                case 5:
                    EVO_closeFusionTypeChoice();
                    return;
                case 11:
                    EVO_FUSION.scriptState = 2;
                    return;
                case 12:
                    EVO_clearTextLines(EVO_TEXT_LINES);
                    break;
                case 8:
                    EVO_openPartnerList();
                    break;
                case 9:
                    EVO_closePartnerList();
                    break;
                case 10:
                    EVO_openCardList();
                    break;
                case 13:
                    EVO_closeCardList();
                    break;
                case 6:
                    EVO_resetFusion();
                    break;
                case 7:
                    EVO_cancelFirstCard();
                    break;
                case 14:
                    EVO_startSecondCardPick();
                    break;
                case 15:
                    EVO_FUSION.pickSlot = 1;
                    break;
                case 16:
                    EVO_leaveForCutscene();
                    break;
                case 17:
                    EVO_startPartnerFusion();
                    return;
                case 18:
                    EVO_cancelPartnerFusion();
                    return;
                case 19:
                    EVO_FUSION.resumeOffset = EVO_SCRIPT->script->pc - EVO_SCRIPT->script->start;
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.resultStep = 0;
                    EVO_FUSION.step = 10;
                    return;
                case 20:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.resultStep = 0;
                    EVO_FUSION.step = 12;
                    return;
                case 21:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 13;
                    return;
                case 22:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 5;
                    animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
                    EVO_CARD_LIST_MENU.active = 1;
                    return;
                default:
                    EVO_SCRIPT_HALTED = 0;
                    break;
                }
                break;
            case 11:
                switch (data->script->eventArg) {
                case 0:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 15;
                    return;
                case 1:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 14;
                    return;
                case 2:
                    EVO_runChoiceDialog((s16)data->script->params[0]);
                    break;
                case 3:
                    animateWindowTo(&EVO_WINDOWS[(s16)data->script->params[0]].win, (Rect16 *)-1);
                    if ((s16)data->script->params[0] == -1) {
                        animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
                        EVO_CARD_LIST_MENU.active = 1;
                    }
                    break;
                case 4:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 16;
                    return;
                case 5:
                    playSoundEffect((s16)data->script->params[0]);
                    break;
                case 6:
                    waitFrames((s16)data->script->params[0]);
                    break;
                case 7:
                    EVO_FUSION.rewardStep = data->script->params[0];
                    return;
                case 8:
                    if ((s16)data->script->params[0] == 0) {
                        EVO_FUSION.busy[1] = 0;
                    } else {
                        EVO_FUSION.busy[2] = 0;
                    }
                    break;
                }
                break;
            case 12:
            case 13:
                break;
            }
        }
        clearScriptBusy(data->script);
    } while (result != 0);
}

s32 EVO_tickFusionScript(EvoProgram *program) {
    *program->vars = 1;
    EVO_runFusionScript(program);
    return *program->vars;
}

Script *EVO_createScriptContext(EvoMsd *data) {
    Script *script;

    script = allocHeapBlock(sizeof(Script), 0x2C);
    script->base = (u8 *)data;
    script->start = data->code;
    script->pc = data->code;
    script->offset = 0;
    script->size = ((EvoMsd *)script->base)->size;
    clearScriptBusy(script);
    return script;
}

s32 *EVO_allocScriptRegisters(s32 count) {
    s32 *flags;
    s32 *p;
    s32 i;

    flags = allocHeapBlock(count * 4, 0x2C);
    p = flags;
    for (i = 0; i < count; i++) {
        *p++ = 0;
    }
    return flags;
}

EvoProgram *EVO_loadUnitScript(s32 index) {
    char path[24];
    EvoMsd *data;
    EvoProgram *program;

    sprintf(path, "C:\\EVENT\\unit0%d.MSD", index);
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    data = (EvoMsd *)waitFrames(0x7FFFFFFF);
    program = allocHeapBlock(sizeof(EvoProgram), 0x2C);
    program->data = data;
    program->script = EVO_createScriptContext(data);
    return program;
}
