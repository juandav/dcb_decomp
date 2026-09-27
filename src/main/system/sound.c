#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/sound.h"
#include "dcb/archive.h"
#include "dcb/decompress.h"
#include "dcb/sort.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"

s32 SOUND_LOAD_BUSY = 0;
extern s8 D_8006E00C[];
extern s8 D_8006E01C[];
extern s8 D_8006E02C[];
s8 *SE_BANK_INFO[3] = { D_8006E00C, D_8006E02C, D_8006E01C };
/* a name, then the note played at byte 15 */
s8 D_8006E00C[16] = { 'S', 'E', '0', [15] = 0x3C };
s8 D_8006E01C[16] = { 'S', 'E', '1', [15] = 0x24 };
s8 D_8006E02C[16] = { 'S', 'E', '2', [15] = 0x3C };
s32 MUSIC_CHANGE_BUSY = 0;
s32 PENDING_MUSIC_CHANGES = 0;
s32 NEXT_SFX_VOICE = 0x12;
s32 SFX_BASE_NOTE = 0x24;
u16 D_8006E04C = 0x3C;

void initSound(void) {
    s8 *state;

    SsSetTableSize(&SOUND_SEQ_ATTR_TABLE, 0x20, 1);
    SsSetMVol(0, 0);
    SsSetTickMode(1);
    SsStart();
    setReverbType(1);
    func_80055740();
    state = (s8 *)&SOUND_STATE;
    *(void **)(state + 0x1C) = allocHeapBlock(0x2100, -2);
    *(void **)(state + 0x28) = allocHeapBlock(0x9300, -2);
    *(void **)(state + 0x34) = allocHeapBlock(0x9300, -2);
    *(s16 *)(state + 0x2C) = 0xFF;
    *(s16 *)(state + 0x20) = 0xFF;
    *(s16 *)(state + 0x14) = 0xFF;
    *(s16 *)(state + 2) = -1;
    loadSoundEffectBank(1);
    SsSetMVol(0x7F, 0x7F);
}

void loadSoundEffectBank(s32 bankId) {
    char name[32];
    u8 *pak;
    SndSlot *bank;

    bank = &((SndState *)&SOUND_STATE)->unk14;
    if (bank->id != bankId) {
        while (SOUND_LOAD_BUSY != 0) {
            func_80014C08(FRAME_INTERVAL);
        }
        SOUND_LOAD_BUSY = 1;
        if (bank->id != 0xFF) {
            stopAllSoundEffects();
            SsVabClose(bank->vab);
        }
        bank->id = bankId;
        /* written as a word here, read as a halfword by the SFX players */
        SFX_BASE_NOTE = SE_BANK_INFO[bankId][0xF];
        sprintf(name, "A:\\SE%d.PAK", bankId);
        pak = (u8 *)loadFileTagged((s32 *)name, getCurrentTaskId(), -2);
        if (pak == 0) {
            bank->id = 0xFF;
        } else {
            bcopy(pak, bank->buf, 0x2030);
            if (openSlotVabHeader(bank, 0, 0x1010) != 0) {
                transferSlotVabBody(bank, (s32)findPakChunk((Chunk *)pak, 8, bank->id), bank->vab);
            } else {
                bank->id = 0xFF;
            }
            freeHeapBlock(pak);
        }
        SOUND_LOAD_BUSY = 0;
    }
}

void loadMusicTrack(s32 slotIndex, s32 trackId, u8 volume) {
    char name[32];
    u8 *pak;
    SndSlot *slot;

    slot = &((SndState *)&SOUND_STATE)->slot[slotIndex];
    if (slot->id == trackId) {
        return;
    }
    while (SOUND_LOAD_BUSY != 0) {
        func_80014C08(FRAME_INTERVAL);
    }
    SOUND_LOAD_BUSY = 1;
    setInstantVoiceRelease();
    if (slot->id != 0xFF) {
        if (((SndState *)&SOUND_STATE)->cur == slotIndex) {
            stopMusic();
        }
        SsSeqClose(((SndState *)&SOUND_STATE)->seq[slotIndex]);
        SsVabClose(slot->vab);
        func_80014C08(FRAME_INTERVAL);
    }
    slot->id = trackId;
    ((SndState *)&SOUND_STATE)->vol[slotIndex] = volume;
    sprintf(name, "A:\\BGM\\BGM%02d.PAK", trackId);
    pak = (u8 *)loadFileTagged((s32 *)name, getCurrentTaskId(), -2);
    if (pak == 0) {
        slot->id = 0xFF;
    } else {
        bcopy(pak, slot->buf, 0x9210);
        if (openSlotVabHeader(slot, slotIndex + 1, slotIndex * 0x1A300 + 0x49E90) == 0) {
            freeHeapBlock(pak);
            slot->id = 0xFF;
        } else {
            transferSlotVabBody(slot, (s32)findPakChunk((Chunk *)pak, 8, slot->id), slot->vab);
            ((SndState *)&SOUND_STATE)->data[slotIndex] = findPakChunk((Chunk *)slot->buf, 6, slot->id);
            ((SndState *)&SOUND_STATE)->seq[slotIndex] = SsSeqOpen(((SndState *)&SOUND_STATE)->data[slotIndex], slot->vab);
            freeHeapBlock(pak);
        }
    }
    SOUND_LOAD_BUSY = 0;
}

void setReverbType(s32 reverbType) {
    if (reverbType == 0) {
        func_80051C70();
        SsUtSetReverbType(0);
        SsUtSetReverbDepth(0, 0);
        SpuClearReverbWorkArea(0);
        return;
    }
    SsUtSetReverbType((s16) reverbType);
    func_80051C90();
    SsUtSetReverbDepth(0x64, 0x64);
}

void setInstantVoiceRelease(void) {
    SpuVoiceAttr attr;

    attr.mask = 0x4000;
    attr.voice = 0xFFFFFF;
    attr.rr = 0;
    SpuSetVoiceAttr(&attr);
    VSync(0);
}

s32 openSlotVabHeader(void *slot, s16 vabId, s32 spuAddr) {
    u8 *vabHeader;

    vabHeader = findPakChunk(*(Chunk **)((s8 *)slot + 8), 7, (*(s16 *)((s8 *)slot + 0)));
    if (vabHeader != 0) {
        (*(s32 *)((s8 *)slot + 4)) = (*(s32 *)(vabHeader - 4));
        if (((*(s16 *)((s8 *)slot + 2)) = SsVabOpenHeadSticky(vabHeader, vabId, spuAddr)) != -1) {
            return 1;
        }
    }
    return 0;
}

void transferSlotVabBody(void *slot, s32 vabBody, s32 vab) {
    if ((vabBody == 0) || (SsVabTransBody(vabBody, (*(s16 *)((s8 *)slot + 2))) == (*(s16 *)((s8 *)slot + 2)))) {
        SsVabTransCompleted(1);
    }
}

void func_8002B3DC(void) {
}

void func_8002B3E4(void) {
}
