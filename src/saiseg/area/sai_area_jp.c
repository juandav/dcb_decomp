#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/archive.h"
#include "dcb/vram_upload.h"
#include "dcb/script.h"
#include "dcb/saiseg.h"

/* jp's area (sai_area.c is us's and eu's): the map's textures and script,
   loaded from its PAK, and the task that runs it until the area ends */

void func_80043374(s32 type, s32 id, s32 count);

void SAI_clearTextVram(void);

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
    char path[0x18]; /* unused: the map scripts are in the PAK */
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
        func_80043374(type, id, 1);
    }
    PLAYER_DATA(0).savedDecks[0].losses = 0;
    PLAYER_DATA(0).savedDecks[0].wins = 0;
    for (j = 0; j < 3; j++) {
        PLAYER_DATA(0).savedDecks[0].attackCounts[j] = 0;
    }
    if (PLAYER_DATA(0).unk28_13) {
        PLAYER_DATA(0).bits += 2000;
        func_80043374(0, 0x6C, 1);
        func_80043374(0, 0x6D, 1);
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

/* our C swaps the registers of `running` and of the state's address */
INCLUDE_ASM("saiseg/nonmatchings/area/sai_area_jp", SAI_runArea);
