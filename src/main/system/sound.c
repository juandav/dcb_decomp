#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/sound.h"
#include "dcb/sound_play.h"
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

/* The chunk number of a music track's VAB and sequence in its PAK: jp's
   music PAKs number them from 200 */
#if VERSION_JP
#define MUSIC_CHUNK_ID(id) ((id) + 200)
#elif VERSION_US || VERSION_EU
#define MUSIC_CHUNK_ID(id) (id)
#else
#error "untested version"
#endif

s32 SOUND_LOAD_BUSY = 0;
extern s8 SE0_BANK_INFO[];
extern s8 SE1_BANK_INFO[];
extern s8 SE2_BANK_INFO[];
s8 *SE_BANK_INFO[3] = { SE0_BANK_INFO, SE2_BANK_INFO, SE1_BANK_INFO };
/* a name, then the note played at byte 15 */
s8 SE0_BANK_INFO[16] = { 'S', 'E', '0', [15] = 0x3C };
s8 SE1_BANK_INFO[16] = { 'S', 'E', '1', [15] = 0x24 };
s8 SE2_BANK_INFO[16] = { 'S', 'E', '2', [15] = 0x3C };
s32 MUSIC_CHANGE_BUSY = 0;
s32 PENDING_MUSIC_CHANGES = 0;
s32 NEXT_SFX_VOICE = 0x12;
s32 SFX_BASE_NOTE = 0x24;
u16 SFX_BASE_FINE = 0x3C;

#if VERSION_JP
void initSound(void) {
    SOUND_STATE.seBank.buf = allocHeapBlock(0x2100, -2);
    SOUND_STATE.slot[0].buf = allocHeapBlock(0x9300, -2);
    SOUND_STATE.slot[1].buf = allocHeapBlock(0x9300, -2);
    SsInit();
    SsSetTableSize(&SOUND_SEQ_ATTR_TABLE, 0x20, 1);
    SsSetTickMode(1);
    SsStart();
    setReverbType(1);
    SsSetStereo();
    SsSetMVol(0x7F, 0x7F);
    /* nothing loaded, nothing playing */
    SOUND_STATE.slot[1].id = 0xFF;
    SOUND_STATE.slot[0].id = 0xFF;
    SOUND_STATE.seBank.id = 0xFF;
    SOUND_STATE.cur = -1;
    loadSoundEffectBank(1);
}
#elif VERSION_US || VERSION_EU
void initSound(void) {
    SsSetTableSize(&SOUND_SEQ_ATTR_TABLE, 0x20, 1);
    SsSetMVol(0, 0);
    /* the sequencer ticks with the display: SS_TICK60 on NTSC, SS_TICK50 on PAL */
#if VERSION_US
    SsSetTickMode(1);
#elif VERSION_EU
    SsSetTickMode(4);
#endif
    SsStart();
    setReverbType(1);
    SsSetStereo();
    SOUND_STATE.seBank.buf = allocHeapBlock(0x2100, -2);
    SOUND_STATE.slot[0].buf = allocHeapBlock(0x9300, -2);
    SOUND_STATE.slot[1].buf = allocHeapBlock(0x9300, -2);
    /* nothing loaded, nothing playing */
    SOUND_STATE.slot[1].id = 0xFF;
    SOUND_STATE.slot[0].id = 0xFF;
    SOUND_STATE.seBank.id = 0xFF;
    SOUND_STATE.cur = -1;
    loadSoundEffectBank(1);
    SsSetMVol(0x7F, 0x7F);
}
#else
#error "untested version"
#endif

void loadSoundEffectBank(s32 bankId) {
    char name[32];
    u8 *pak;
    SndSlot *bank;

    bank = &SOUND_STATE.seBank;
    if (bank->id != bankId) {
        while (SOUND_LOAD_BUSY != 0) {
            waitFrames(FRAME_INTERVAL);
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

    slot = &SOUND_STATE.slot[slotIndex];
    if (slot->id == trackId) {
        return;
    }
    while (SOUND_LOAD_BUSY != 0) {
        waitFrames(FRAME_INTERVAL);
    }
    SOUND_LOAD_BUSY = 1;
    setInstantVoiceRelease();
    if (slot->id != 0xFF) {
        if (SOUND_STATE.cur == slotIndex) {
            stopMusic();
        }
        SsSeqClose(SOUND_STATE.seq[slotIndex]);
        SsVabClose(slot->vab);
        waitFrames(FRAME_INTERVAL);
    }
    slot->id = trackId;
    SOUND_STATE.vol[slotIndex] = volume;
    sprintf(name, "A:\\BGM\\BGM%02d.PAK", trackId);
    pak = (u8 *)loadFileTagged((s32 *)name, getCurrentTaskId(), -2);
    if (pak == 0) {
        slot->id = 0xFF;
    } else {
        bcopy(pak, slot->buf, 0x9210);
        /* each track's VAB gets its own area of sound RAM */
        if (openSlotVabHeader(slot, slotIndex + 1, slotIndex * 0x1A300 + 0x49E90) == 0) {
            freeHeapBlock(pak);
            slot->id = 0xFF;
        } else {
            transferSlotVabBody(slot, (s32)findPakChunk((Chunk *)pak, 8, MUSIC_CHUNK_ID(slot->id)), slot->vab);
            SOUND_STATE.data[slotIndex] = findPakChunk((Chunk *)slot->buf, 6, MUSIC_CHUNK_ID(slot->id));
            SOUND_STATE.seq[slotIndex] = SsSeqOpen(SOUND_STATE.data[slotIndex], slot->vab);
            freeHeapBlock(pak);
        }
    }
    SOUND_LOAD_BUSY = 0;
}

void setReverbType(s32 reverbType) {
    if (reverbType == 0) {
        SsUtReverbOff();
        SsUtSetReverbType(0);
        SsUtSetReverbDepth(0, 0);
        SpuClearReverbWorkArea(0);
        return;
    }
    SsUtSetReverbType((s16) reverbType);
    SsUtReverbOn();
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

/* The PAK's chunks: 6 is the SEQ, 7 the VAB header, 8 the VAB body. */
s32 openSlotVabHeader(SndSlot *slot, s16 vabId, s32 spuAddr) {
    u8 *vabHeader;

#if VERSION_JP
    /* vabId 0 is the sound effects' bank, the others music */
    vabHeader = findPakChunk((Chunk *)slot->buf, 7, vabId != 0 ? MUSIC_CHUNK_ID(slot->id) : slot->id);
#elif VERSION_US || VERSION_EU
    vabHeader = findPakChunk((Chunk *)slot->buf, 7, slot->id);
#else
#error "untested version"
#endif
    if (vabHeader != 0) {
        slot->vabHeaderSize = ((Chunk *)vabHeader - 1)->size;
        if ((slot->vab = SsVabOpenHeadSticky(vabHeader, vabId, spuAddr)) != -1) {
            return 1;
        }
    }
    return 0;
}

void transferSlotVabBody(SndSlot *slot, s32 vabBody, s32 vab) {
    if (vabBody == 0 || SsVabTransBody(vabBody, slot->vab) == slot->vab) {
        SsVabTransCompleted(1);
    }
}

/* two functions left empty, which nothing calls */
void emptySoundFunction1(void) {
}

void emptySoundFunction2(void) {
}
