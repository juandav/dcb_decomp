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

void startCpuDuel(s32 deckIndex) {
    u8 *deckFile;
    SavedDeck *decks;
    s32 result;

    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_KAWSEG_BIN, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x800, loadFile, &PATH_DECK2_DEK, getCurrentTaskId());
    deckFile = (u8 *)func_80014C08(0x7FFFFFFF);
    ((SessionData *)D_8006E054)->npcDeckFile = deckFile;
    decks = (SavedDeck *)(deckFile + 8);
    ((SessionData *)D_8006E054)->opponentDeckIndex = deckIndex;
    ((SessionData *)D_8006E054)->opponentDeck = decks[deckIndex];
    func_800149B8(0, -1, 0, 0x800, runDuel, 1, getCurrentTaskId(), 0, 0);
    result = func_80014C08(0x7FFFFFFF);
    if (*((s8 *)D_801D8340 + 0x81F) == 0) {
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
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    ((u8 *)((SessionData *)D_8006E054)->unk100C)[0x1A6] = result;
    func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
}

void startVersusDuel(void) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_KAWSEG_BIN, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x800, loadFile, &PATH_DECK2_DEK, getCurrentTaskId());
    ((SessionData *)D_8006E054)->npcDeckFile = (u8 *)func_80014C08(0x7FFFFFFF);
    ((SessionData *)D_8006E054)->unk1010[0x12] = 0;
    func_800149B8(0, -1, 0, 0x800, runDuel, 0, getCurrentTaskId(), 0, 0);
    if (func_80014C08(0x7FFFFFFF) != 0) {
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
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, "P:\\openseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x800, D_801EB2E8, getCurrentTaskId(), 0, 0, 0);
}
