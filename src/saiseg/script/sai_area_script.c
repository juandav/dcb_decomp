#include "common.h"
#include "game.h"
#include "dcb/sai_area_script.h"
#include "dcb/window.h"
#include "dcb/heap.h"
#include "dcb/script.h"
#include "dcb/task.h"
#include "dcb/card_db.h"
#include "dcb/sound_play.h"
#include "dcb/memcard.h"
#include "dcb/dialog.h"
#include "dcb/sai_text.h"
#include "dcb/sai_choice.h"
#include "dcb/sai_flags.h"
#include "dcb/sai_labels.h"
#include "dcb/sai_player_data.h"
#include "dcb/sai_area.h"
#include "dcb/sai_opponent_select.h"
#include "dcb/sai_opponent_info.h"
#include "dcb/sai_hacking.h"
#include "dcb/sai_word_input.h"
#include "dcb/sai_reward.h"
#include "dcb/sai_partner_get.h"
#include "dcb/sai_digi_parts.h"


extern u8 SAI_DIALOG[];

/* Runs the area script and carries out the events it stops on: op 10 starts
   panels and tasks, op 11 sets up choices, opponents, music and exits, op 13
   fades the portraits and gives cards. */
void SAI_runAreaScript(ScriptRunner *runner) {
    s32 result;
    s32 i;
    s32 offset;

    do {
        result = runScriptToNextEvent(runner->script, runner->regs);
        if (result == 1) {
            switch (runner->script->eventOp) {
            case 10:
                switch (runner->script->eventArg) {
                case 0:
                    if (SAI_AREA.openPanel == 0) {
                        SAI_AREA.mode = AREA_MODE_BUSY;
                        SAI_clearOpponents();
                        spawnTask(0, -1, 0, 0x800, SAI_runTalkPanel, 0, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    if (SAI_AREA.openPanel == 1) {
                        return;
                    }
                    SAI_AREA.closePanel = 1;
                    do {
                        waitFrames(1);
                    } while (SAI_AREA.openPanel != 0);
                    SAI_AREA.mode = AREA_MODE_BUSY;
                    SAI_clearOpponents();
                    spawnTask(0, -1, 0, 0x800, SAI_runTalkPanel, 0, getCurrentTaskId(), 0, 0);
                    return;
                case 1:
                    offset = runner->script->pc - runner->script->start;
                    SAI_saveScriptFlags();
                    for (i = 0; i < SAI_AREA.choiceCount;) {
                        runner->regs[1] = ++i;
                        SAI_runChoiceScript(runner);
                        runner->script->pc = runner->script->start + offset;
                        SAI_loadScriptFlags();
                    }
                    runner->regs[1] = 0;
                    SAI_AREA.mode = AREA_MODE_CHOICES;
                    return;
                case 2:
                    if (SAI_AREA.openPanel == 0) {
                        SAI_AREA.mode = AREA_MODE_BUSY;
                        SAI_OPPONENTS.current[0] = 0x80;
                        SAI_OPPONENTS.target[0] = 0x80;
                        SAI_OPPONENTS.current[1] = 0x80;
                        SAI_OPPONENTS.target[1] = 0x80;
                        spawnTask(0, -1, 0, 0x800, SAI_runOpponentSelectPanel, 0, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    if (SAI_AREA.openPanel == 2) {
                        return;
                    }
                    SAI_AREA.closePanel = 1;
                    do {
                        waitFrames(1);
                    } while (SAI_AREA.openPanel != 0);
                    SAI_AREA.mode = AREA_MODE_BUSY;
                    SAI_OPPONENTS.current[0] = 0x80;
                    SAI_OPPONENTS.target[0] = 0x80;
                    SAI_OPPONENTS.current[1] = 0x80;
                    SAI_OPPONENTS.target[1] = 0x80;
                    spawnTask(0, -1, 0, 0x800, SAI_runOpponentSelectPanel, 0, getCurrentTaskId(), 0, 0);
                    return;
                case 3:
                    if (SAI_AREA.opponentPicked != 0) {
                        SAI_AREA.opponentPicked = 0;
                        SAI_AREA.mode = AREA_MODE_BUSY;
                        SAI_AREA.selectState = 7;
                        return;
                    }
                    SAI_AREA.mode = AREA_MODE_SELECT_OPPONENT;
                    return;
                case 4:
                    if (SAI_addTextLine((u8 *)runner->regs[4]) == -1) {
                        SAI_AREA.mode = AREA_MODE_TEXT_FULL;
                        return;
                    }
                    break;
                case 5:
                    SAI_AREA.mode = AREA_MODE_WAIT_CROSS;
                    SAI_AREA.waitingForCross = 1;
                    return;
                case 6:
                    SAI_clearTextLines(SAI_TEXT_LINES);
                    break;
                case 7:
                    if (SAI_PLAYER_STATS.state == 0) {
                        SAI_unlockArmorsFromFlags(SAI_SCRIPT[0]->regs);
                        SAI_PLAYER_STATS.state = 1;
                        spawnTask(0, -1, 0, 0x400, SAI_runPlayerData, 0, getCurrentTaskId(), 0, 0);
                        SAI_AREA.mode = AREA_MODE_BUSY;
                    }
                    return;
                case 9:
                    runner->regs[0] = 0;
                    SAI_AREA.exitAction = AREA_EXIT_DECK_EDITOR;
                    return;
                case 10:
                    SAI_AREA.mode = AREA_MODE_BUSY;
                    spawnTask(0, -1, 0, 0x400, SAI_runPartnerGet, getCurrentTaskId(), 0, 0, 0);
                    return;
                case 11:
                    SAI_AREA.partnerCount = 0;
                    for (i = 0; i < 4; i++) {
                        SAI_AREA.partners[i] = -1;
                    }
                    break;
                case 12:
                    if (SAI_AREA.openPanel == 0) {
                        SAI_AREA.mode = AREA_MODE_BUSY;
                        spawnTask(0, -1, 0, 0x800, SAI_runOpponentInfoPanel, SAI_AREA.location, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    if (SAI_AREA.openPanel == SAI_AREA.location + 1) {
                        return;
                    }
                    SAI_AREA.closePanel = 1;
                    do {
                        waitFrames(1);
                    } while (SAI_AREA.openPanel != 0);
                    SAI_AREA.mode = AREA_MODE_BUSY;
                    spawnTask(0, -1, 0, 0x800, SAI_runOpponentInfoPanel, SAI_AREA.location, getCurrentTaskId(), 0, 0);
                    return;
                case 13:
                    SAI_showOpponentInfo();
                    return;
                case 14:
                    break;
                case 15:
                    animateWindowTo(&SAI_MESSAGE_WINDOW, (Rect16 *)-1);
                    playSoundEffect(4);
                    SAI_AREA.mode = AREA_MODE_BUSY;
                    bzero((Scene3D *)SAI_AREA.keyword, 0xD);
                    spawnTask(0, -1, 0, 0x800, SAI_runWordInput, SAI_AREA.keyword, getCurrentTaskId(), 0, 0);
                    return;
                case 16:
                    runner->regs[1] = ((PlayerProfile *)PLAYER_PROFILES)->battleWins;
                    break;
                case 17:
                    runner->regs[0] = 0;
                    SAI_AREA.exitAction = AREA_EXIT_EQUIPMENT;
                    return;
                case 18:
                    ((PlayerProfile *)PLAYER_PROFILES)->tradeUnlocked = 1;
                    break;
                case 19:
                    ((PlayerProfile *)PLAYER_PROFILES)->completionPoints = 0;
                    break;
                case 20:
                    spawnTask(0, -1, 0, 0x400, SAI_runHackOverlay, 0, 0, 0, 0);
                    break;
                case 21:
                    SAI_closeHackOverlay();
                    break;
                case 22:
                    initDialog(SAI_DIALOG, (u8 *)SAI_SCRIPT[0]->regs[9], 0);
                    runDialog(SAI_DIALOG);
                    break;
                default:
                    runner->regs[0] = 0;
                    return;
                }
                break;
            case 11:
                switch (runner->script->eventArg) {
                case 0:
                    SAI_openChoiceMenu((s16)runner->script->params[0] == 0x78);
                    break;
                case 1:
                    SAI_addChoice(runner->script->params[0]);
                    break;
                case 2:
                    SESSION->resumeMode = 1;
                    SAI_AREA.exitArg = (s16)runner->script->params[0];
                    runner->regs[0] = 0;
                    SAI_AREA.exitAction = AREA_EXIT_DUEL;
                    SESSION->scriptOffset = SAI_SCRIPT[0]->script->pc - SAI_SCRIPT[0]->script->start;
                    return;
                case 3:
                    SAI_addOpponent((s16)runner->script->params[0]);
                    break;
                case 4:
                    SAI_copyScriptFlagsForFusion();
                    runner->regs[0] = 0;
                    SAI_AREA.exitAction = AREA_EXIT_FUSION;
                    SAI_AREA.exitArg = (s16)runner->script->params[0];
                    return;
                case 6:
                    SAI_saveScriptFlags();
                    ((PlayerProfile *)PLAYER_PROFILES)->scriptOffset = SAI_SCRIPT[0]->script->pc - SAI_SCRIPT[0]->script->start;
                    ((PlayerProfile *)PLAYER_PROFILES)->resumeInArea = runner->script->params[0];
                    runner->regs[0] = 0;
                    SAI_AREA.exitAction = AREA_EXIT_SAVE;
                    return;
                case 7:
                    SESSION->selectImage = runner->script->params[0];
                    break;
                case 8:
                    SAI_AREA.location = SESSION->location = runner->script->params[0];
                    break;
                case 9:
                    SAI_AREA.prizePack = runner->script->params[0];
                    if (SAI_AREA.rewardBusy == 0) {
                        SAI_AREA.rewardBusy = 1;
                        SAI_AREA.mode = AREA_MODE_BUSY;
                        spawnTask(0, -1, 0, 0x400, SAI_runRewardTask, 0, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    break;
                case 10:
                    SAI_AREA.partners[SAI_AREA.partnerCount] = runner->script->params[0];
                    SAI_AREA.partnerCount++;
                    break;
                case 11:
                case 12:
                    break;
                case 13:
                    playSoundEffect((s16)runner->script->params[0]);
                    break;
                case 14:
                    waitFrames((s16)runner->script->params[0]);
                    break;
                case 15:
                    do {
                        waitFrames(1);
                    } while (isMusicIdle() != 1);
                    if (SAI_SCRIPT[0]->regs[0xB8] == 0 && (s16)runner->script->params[0] != 0x6F) {
                        SESSION->music = (s16)runner->script->params[0];
                        playMusic(0, (s16)runner->script->params[0], 100);
                    }
                    break;
                case 16:
                    SAI_AREA.mode = AREA_MODE_BUSY;
                    spawnTask(0, -1, 0, 0x400, SAI_grantDigiPart, (s16)runner->script->params[0], getCurrentTaskId(), 0, 0);
                    return;
                case 17:
                    ((SessionData *)SESSION_DATA)->npcDeckIndex[0] = runner->script->params[0];
                    break;
                case 18:
                    ((PlayerProfile *)PLAYER_PROFILES)->completionPoints += runner->script->params[0];
                    break;
                case 19:
                    SESSION->resumeMode = 1;
                    SAI_AREA.exitArg = (s16)runner->script->params[0];
                    runner->regs[0] = 0;
                    SAI_AREA.exitAction = AREA_EXIT_TITLE_OR_ENDING;
                    SESSION->scriptOffset = SAI_SCRIPT[0]->script->pc - SAI_SCRIPT[0]->script->start;
                    return;
                case 20:
                    SAI_AREA.mode = AREA_MODE_BUSY;
                    spawnTask(0, -1, 0, 0x400, SAI_runHackingEvent, (s16)runner->script->params[0], getCurrentTaskId(), 0, 0);
                    return;
                case 21:
                    for (i = 0; i < 3; i++) {
                        if (((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].inUse == 0) {
                            ((SessionData *)SESSION_DATA)->deckAllowed[i] = 0;
                        } else if (countDeckCardsByFilter(0, &((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i], (s16)runner->script->params[0]) != 0) {
                            ((SessionData *)SESSION_DATA)->deckAllowed[i] = 0;
                        } else {
                            ((SessionData *)SESSION_DATA)->deckAllowed[i] = 1;
                        }
                    }
                    if (((SessionData *)SESSION_DATA)->deckAllowed[0] == 1 || ((SessionData *)SESSION_DATA)->deckAllowed[1] == 1 ||
                        ((SessionData *)SESSION_DATA)->deckAllowed[2] == 1) {
                        ((SessionData *)SESSION_DATA)->deckRuleActive = 1;
                        SAI_SCRIPT[0]->regs[1] = 0;
                    } else {
                        ((SessionData *)SESSION_DATA)->deckRuleActive = 0;
                        SAI_SCRIPT[0]->regs[1] = 1;
                    }
                    break;
                default:
                    runner->regs[0] = 0;
                    return;
                }
                break;
            case 13:
                switch (runner->script->eventArg) {
                case 0:
                    if ((s16)runner->script->params[0] == 0) {
                        SAI_OPPONENTS.target[0] = runner->script->params[1];
                        SAI_OPPONENTS.step[0] = runner->script->params[2];
                    } else {
                        SAI_OPPONENTS.target[1] = runner->script->params[1];
                        SAI_OPPONENTS.step[1] = runner->script->params[2];
                    }
                    break;
                case 1:
                    SAI_AREA.mode = AREA_MODE_BUSY;
                    SAI_SCRIPT_REWARD_CARDS[0] = runner->script->params[0];
                    SAI_SCRIPT_REWARD_CARDS[1] = runner->script->params[1];
                    SAI_SCRIPT_REWARD_CARDS[2] = runner->script->params[2];
                    spawnTask(0, -1, 0, 0x400, SAI_runRewardTask, 1, getCurrentTaskId(), 0, 0);
                    return;
                default:
                    runner->regs[0] = 0;
                    return;
                }
                break;
            case 12:
            default:
                runner->regs[0] = 0;
                return;
            }
        }
        clearScriptBusy(runner->script);
    } while (result != 0);
}

s32 SAI_stepAreaScript(ScriptRunner *runner) {
    *runner->regs = 1;
    SAI_runAreaScript(runner);
    return *runner->regs;
}

Script *SAI_createScriptContext(u8 *scriptData) {
    Script *script = allocHeapBlock(sizeof(Script), 0x31);
    u8 *codeStart;

    script->base = scriptData;
    codeStart = scriptData + 0x10;
    script->start = codeStart;
    script->pc = codeStart;
    script->offset = 0;
    script->size = *(u32 *)(script->base + 8);
    clearScriptBusy(script);
    return script;
}

s32 *SAI_allocScriptRegisters(s32 count) {
    s32 *block = allocHeapBlock(count * 4, 0x31);
    s32 *p = block;
    s32 i;

    for (i = 0; i < count; i++) {
        *p++ = 0;
    }
    return block;
}

ScriptRunner *SAI_createAreaScript(void) {
    u8 unused[0x18]; /* unused, but it is in the original stack frame */
    ScriptRunner *obj = allocHeapBlock(sizeof(ScriptRunner), 0x31);

    obj->unk0 = *(s32 *)&((SessionData *)SESSION_DATA)->areaSession->unk0[0x190];
    obj->script = SAI_createScriptContext((u8 *)obj->unk0);
    return obj;
}
