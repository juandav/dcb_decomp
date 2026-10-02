#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/archive.h"
#include "dcb/vram_upload.h"
#include "dcb/script.h"
#include "dcb/pad.h"
#include "dcb/sound_play.h"
#include "dcb/frame_callback.h"
#include "dcb/fade.h"
#include "dcb/game_exit.h"
#include "dcb/saiseg.h"

/* jp's area (sai_area.c is us's and eu's): the map's textures and script,
   loaded from its PAK, and the task that runs it until the area ends */

void addCardToCollection(s32 type, s32 id, s32 count);

void SAI_clearTextVram(void);

extern JpGame *SCROLLING_BACKGROUND;
extern s8 D_801F3780; /* the address of SUBSEG's SUB_SHOP_RUNNING */
extern void renderScrollingBackground();
void startAreaPakLoad(void);
void setBackgroundScrollMode(s32);
void allocAreaFlags(void);
void SAI_clearTextLines(void);
void SAI_runMenu(void);
void func_801EE8F0(void);
s32 SAI_stepAreaScript(ScriptRunner *runner);
void SAI_returnToArea(void);
void func_801F0F48(void);
void func_801F1130();
void SAI_saveScriptFlags(void);
void SAI_loadScriptFlags(void);
void SAI_runWorldMap();

void SAI_loadMapTextures(void) {
    char path[0x18];
    u32 *tims;

    if (SAI_STATE->map < 10) {
        sprintf(path, "C:\\OBJECT\\map0%d.TIM", SAI_STATE->map);
    } else {
        sprintf(path, "C:\\OBJECT\\map%d.TIM", SAI_STATE->map);
    }
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTimList(tims);
    freeHeapBlock(tims);
}

Script *SAI_createScriptContext(ScriptData *data) {
    Script *script;

    script = allocHeapBlock(sizeof(Script), 0x190);
    script->base = (u8 *)data;
    script->start = (u8 *)data + 0x10;
    script->pc = (u8 *)data + 0x10;
    script->offset = 0;
    script->size = ((ScriptData *)script->base)->size;
    clearScriptBusy(script);
    return script;
}

ScriptRunner *SAI_loadMapScript(void) {
    char path[0x18];
    ScriptRunner *runner;

    if (SAI_STATE->map < 10) {
        sprintf(path, "C:\\EVENT\\map0%d.MSD", SAI_STATE->map);
    } else {
        sprintf(path, "C:\\EVENT\\map%d.MSD", SAI_STATE->map);
    }
    runner = allocHeapBlock(sizeof(ScriptRunner), 0x190);
    runner->unk0 = loadFile(path, getCurrentTaskId());
    runner->script = SAI_createScriptContext((ScriptData *)runner->unk0);
    return runner;
}

ScriptRunner *SAI_loadAreaScript(void) {
    /* the map scripts are in the PAK */
    char path[0x18]; /* unused, but it is in the original stack frame */
    Chunk *pak;
    s32 size;
    ScriptRunner *runner;

    SAI_STATE->unk43 = 1;
    pak = SAI_STATE->pak;
    uploadTimList(findPakChunk(pak, 5, SAI_STATE->map));
    truncatePakTextures(pak);
    size = ((s32 *)findPakChunk(pak, 2, SAI_STATE->map))[-1] + 0x10;
    pak = allocHeapBlock(size, 0x190);
    bcopy(SAI_STATE->pak, pak, size);
    freeHeapBlock(SAI_STATE->pak);
    SAI_STATE->pak = pak;
    runner = allocHeapBlock(sizeof(ScriptRunner), 0x190);
    runner->unk0 = (s32)findPakChunk(pak, 2, SAI_STATE->map);
    runner->script = SAI_createScriptContext((ScriptData *)runner->unk0);
    return runner;
}

void SAI_giveStarterCards(void) {
    u8 i;
    s32 type;
    u8 id;
    s32 j;

    for (i = 0; i < PLAYER_DATA(0).starterCardCount; i++) {
        id = PLAYER_DATA(0).starterCards[i];
        type = 0;
        if (id >= 0x6E) {
            id -= 0x6E;
            type = 1;
            if (id >= 0x2B) {
                id -= 0x2B;
                type = 2;
            }
        }
        addCardToCollection(type, id, 1);
    }
    PLAYER_DATA(0).savedDecks[0].losses = 0;
    PLAYER_DATA(0).savedDecks[0].wins = 0;
    for (j = 0; j < 3; j++) {
        PLAYER_DATA(0).savedDecks[0].attackCounts[j] = 0;
    }
    if (PLAYER_DATA(0).unk28_13) {
        PLAYER_DATA(0).bits += 2000;
        addCardToCollection(0, 0x6C, 1);
        addCardToCollection(0, 0x6D, 1);
    }
}

void SAI_resetArea(void) {
    s8 i;

    SAI_UI.unk3C0 = 0;
    SAI_STATE->unk49 = 0;
    SAI_STATE->unk18 = 0;
    for (i = 0; i < 3; i++) {
        SAI_UI.lines[i].active = 0;
        SAI_UI.lines[i].shown = 0;
    }
    SAI_UI.unk3B4 = NULL;
    SAI_STATE->unk48 = 0;
    SAI_STATE->unk4A = 0;
    SAI_STATE->unk4B = 0x7F;
    SAI_UI.unk0 = 4;
    SAI_UI.unk3C4 = SAI_STATE->map + 6;
    SAI_clearTextVram();
}

/* the area's task: it runs the area's script, event by event, until it
   ends in another area (7), the title or the ending (8), or the map */
void SAI_runArea(void) {
    s32 running;

    running = 0;
    if (!PLAYER_DATA(0).unk28_9) {
        SAI_giveStarterCards();
        PLAYER_DATA(0).unk28_9 = 1;
    }
    SAI_STATE->unk45 = 1;
    SAI_STATE->map = PLAYER_DATA(0).area + 1;
    func_801F0F48();
    do {
        waitFrames(FRAME_INTERVAL);
        SAI_resetArea();
        SAI_STATE->runner = SAI_loadAreaScript();
        allocAreaFlags();
        SAI_STATE->runner->regs = SAI_STATE->regs;
        SAI_loadScriptFlags();
        addFrameCallback((s32)func_801F1130);
        waitFrames(4);
        do {
            waitFrames(FRAME_INTERVAL);
            switch (SAI_STATE->unk18) {
            case 1:
                running = 1;
                SAI_STATE->flags++;
                if ((PAD_STATES[0]->rawPressed & PAD_CIRCLE) && SAI_UI.typing == 0) {
                    SAI_STATE->flags = 0;
                    SAI_STATE->unk18 = 0;
                    playSoundEffectAtVolume(0, 0x32);
                    SAI_clearTextLines();
                }
                break;
            case 2:
                running = 1;
                if (SAI_UI.typing == 0) {
                    SAI_STATE->flags = 0;
                    SAI_STATE->unk18 = 0;
                    SAI_clearTextLines();
                }
                break;
            case 3:
                running = 1;
                if (SAI_UI.typing == 0) {
                    spawnTask(0, -1, 0, 0x400, SAI_runMenu);
                    SAI_STATE->unk18 = 4;
                    SAI_UI.typing = 0;
                }
                break;
            case 4:
                running = 1;
                SAI_STATE->flags++;
                if (SAI_STATE->regs[15] != 0) {
                    SAI_STATE->flags = 0;
                    SAI_STATE->unk18 = 0;
                }
                break;
            case 5:
                if (D_801F3780 == 0) {
                    running = 1;
                    SAI_STATE->unk18 = 0;
                    SAI_loadMapTextures();
                    SAI_clearTextVram();
                    SAI_STATE->unk4B = 1;
                    addFrameCallback((s32)func_801F1130);
                    waitFrames(4);
                }
                break;
            case 7:
                running = 0;
                break;
            case 8:
                running = 0;
                break;
            default:
                running = SAI_stepAreaScript(SAI_STATE->runner);
                break;
            }
        } while (running != 0);
        SAI_saveScriptFlags();
        SCROLLING_BACKGROUND->unk1BE = 2;
        func_801EE8F0();
        waitFrames(2);
        removeFrameCallback((s32)func_801F1130);
        waitFrames(2);
        freeHeapBlocksByTag(0x190);
        freeHeapBlocksByTag(0x194);
    } while (running != 0);
    waitFrames(40);
    SAI_STATE->unk43 = 0;
    if (SAI_STATE->unk18 == 7) {
        startAreaPakLoad();
        SAI_STATE->unk18 = 0;
        spawnTask(0, -1, 0, 0x1000, SAI_returnToArea, 0, getCurrentTaskId(), 0, 0);
    } else if (SAI_STATE->unk18 == 8) {
        if (SAI_STATE->unk50 == 1) {
            SAI_STATE->unk18 = 0;
            SCROLLING_BACKGROUND->unk1C0 = -1;
            setBackgroundScrollMode(2);
            waitFrames(60);
            quitToTitleOrPlayEnding(SAI_STATE->unk50);
        } else {
            SCROLLING_BACKGROUND->unk1C0 = -1;
            waitFrames(90);
            removeFrameCallback((s32)renderScrollingBackground);
            waitFrames(1);
            stopScreenFade();
            SAI_STATE->unk18 = 0;
            SAI_STATE->unk42 = PLAYER_DATA(0).area;
            quitToTitleOrPlayEnding(SAI_STATE->unk50);
            PLAYER_DATA(0).area = 0;
            SAI_STATE->unk42 = 0;
            startAreaPakLoad();
            spawnTask(0, -1, 0, 0x1000, SAI_returnToArea, 0, getCurrentTaskId(), 0, 0);
            addFrameCallback((s32)renderScrollingBackground);
        }
    } else {
        setBackgroundScrollMode(0);
        spawnTask(0, -1, 0, 0x800, SAI_runWorldMap, 1, 0, 0, 0);
    }
    exitTask();
}
