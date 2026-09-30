#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/duel_launch.h"
#include "dcb/battle_hud.h"
#include "dcb/card_motion.h"
#include "dcb/hud_panels.h"
#include "dcb/duel_session.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/save_checksum.h"
#include "dcb/player_rank.h"
#include "dcb/prim_util.h"
#include "dcb/transform.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/overlay_calls.h"

void startCpuDuel(s32 deckIndex) {
    u8 *deckFile;
    PresetDeck *decks;
    s32 result;

    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\kawseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\DECK2.DEK", getCurrentTaskId());
    deckFile = (u8 *)waitFrames(0x7FFFFFFF);
    ((SessionData *)SESSION_DATA)->npcDeckFile = deckFile;
    decks = (PresetDeck *)(deckFile + 8);
    ((SessionData *)SESSION_DATA)->opponentDeckIndex = deckIndex;
    ((SessionData *)SESSION_DATA)->opponentDeck = decks[deckIndex];
    spawnTask(0, -1, 0, 0x800, runDuel, 1, getCurrentTaskId(), 0, 0);
    result = waitFrames(0x7FFFFFFF);
    if (DUEL->tutorial == 0) {
        if (result != 0) {
            if (++PLAYER_DATA(0).battleLosses >= 1000) {
                PLAYER_DATA(0).battleLosses = 999;
            }
        } else {
            if (++PLAYER_DATA(0).battleWins >= 1000) {
                PLAYER_DATA(0).battleWins = 999;
            }
        }
        updatePlayerRanks(0);
    }
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    ((SessionData *)SESSION_DATA)->areaSession->duelResult = result;
    spawnTask(0, -1, 0, 0x1600, SAI_runArea, 0, getCurrentTaskId(), 0, 0);
}

void startVersusDuel(void) {
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\kawseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\DECK2.DEK", getCurrentTaskId());
    ((SessionData *)SESSION_DATA)->npcDeckFile = (u8 *)waitFrames(0x7FFFFFFF);
    ((SessionData *)SESSION_DATA)->deckRuleActive = 0;
    spawnTask(0, -1, 0, 0x800, runDuel, 0, getCurrentTaskId(), 0, 0);
    if (waitFrames(0x7FFFFFFF) != 0) {
        if (++PLAYER_DATA(0).versusLosses >= 1000) {
            PLAYER_DATA(0).versusLosses = 999;
        }
        if (++PLAYER_DATA(1).versusWins >= 1000) {
            PLAYER_DATA(1).versusWins = 999;
        }
    } else {
        if (++PLAYER_DATA(0).versusWins >= 1000) {
            PLAYER_DATA(0).versusWins = 999;
        }
        if (++PLAYER_DATA(1).versusLosses >= 1000) {
            PLAYER_DATA(1).versusLosses = 999;
        }
    }
    updatePlayerRanks(0);
    updatePlayerRanks(1);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\openseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x800, OPEN_runBattleWithFriend, getCurrentTaskId(), 0, 0, 0);
}
