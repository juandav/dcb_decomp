#include "dcb/sound_play.h"
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

void playSoundEffect(s32 sound) {
    s32 tone = sound & 0xF;

    SsUtKeyOnV(NEXT_SFX_VOICE, D_801D813E, sound >> 4, tone, SFX_BASE_NOTE,
               D_8006E04C + tone, 0x6E, 0x6E);
    if (++NEXT_SFX_VOICE >= 0x16) {
        NEXT_SFX_VOICE = 0x12;
    }
}

void playSoundEffectAtVolume(s32 sound, s32 volume) {
    s32 tone = sound & 0xF;

    SsUtKeyOnV(NEXT_SFX_VOICE, D_801D813E, sound >> 4, tone, SFX_BASE_NOTE,
               D_8006E04C + tone, volume, volume);
    if (++NEXT_SFX_VOICE >= 0x16) {
        NEXT_SFX_VOICE = 0x12;
    }
}

void playSoundEffectOnVoice(s32 voice, s32 sound) {
    s32 tone = sound & 0xF;

    SsUtKeyOnV(voice, D_801D813E, sound >> 4, tone, SFX_BASE_NOTE,
               D_8006E04C + tone, 0x6E, 0x6E);
}

void stopSoundVoice(s16 voice) {
    SsUtKeyOffV(voice);
}

void stopAllSoundEffects(void) {
    SsUtAllKeyOff(0);
}

void stopMusic(void) {
    s16 *state;

    func_80014A00(0x1C);
    state = (s16 *)&SOUND_STATE;
    if (((s16 *)&SOUND_STATE)[1] >= 0) {
        SsSeqStop(((s16 *)&SOUND_STATE)[state[1] + 2]);
        func_80014C08(4);
        ((s16 *)&SOUND_STATE)[1] = -1;
    }
}

void fadeOutMusicTask(s32 slotIndex, s32 step) {
    s16 volL;
    s16 volR;

    for (;;) {
        func_80014C08(FRAME_INTERVAL);
        if (((SndState *)&SOUND_STATE)->cur != slotIndex) {
            func_80014A90();
        }
        SsSeqGetVol(((SndState *)&SOUND_STATE)->seq[slotIndex], 0, &volL, &volR);
        if (volL == 0) {
            SsSeqStop(((SndState *)&SOUND_STATE)->seq[slotIndex]);
            func_80014C08(4);
            ((SndState *)&SOUND_STATE)->cur = -1;
            func_80014A90();
        }
        volL -= step;
        if (volL < 0) {
            volL = 0;
        }
        SsSeqSetVol(((SndState *)&SOUND_STATE)->seq[slotIndex], volL, volL);
    }
}

void fadeOutMusic(s32 step) {
    s16 *state = (s16 *)&SOUND_STATE;

    if (state[1] >= 0) {
        func_80014A00(0x1C);
        func_800149B8(0x1C, -1, 0, 0x1000, &fadeOutMusicTask, state[1], step);
    }
}

void func_8002B850(void) {
}

void playLoadedMusic(s32 slotIndex) {
    if (((SndState *)&SOUND_STATE)->slot[slotIndex].id != 0xFF) {
        if (((SndState *)&SOUND_STATE)->cur >= 0) {
            stopMusic();
        }
        SsSeqPlay(((SndState *)&SOUND_STATE)->seq[slotIndex], 1, 0);
        SsSeqSetVol(((SndState *)&SOUND_STATE)->seq[slotIndex],
                    ((SndState *)&SOUND_STATE)->vol[slotIndex],
                    ((SndState *)&SOUND_STATE)->vol[slotIndex]);
        ((SndState *)&SOUND_STATE)->cur = slotIndex;
    }
}

void changeMusicTask(s32 slotIndex, s32 trackId, s32 volume, s32 needsLoad) {
    PENDING_MUSIC_CHANGES++;
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (MUSIC_CHANGE_BUSY != 0);
    MUSIC_CHANGE_BUSY = 1;
    if (((SndState *)&SOUND_STATE)->cur >= 0) {
        fadeOutMusic(2);
        while (((SndState *)&SOUND_STATE)->cur >= 0) {
            func_80014C08(FRAME_INTERVAL);
        }
    }
    if (needsLoad) {
        loadMusicTrack(slotIndex, trackId, volume);
    }
    playLoadedMusic(slotIndex);
    MUSIC_CHANGE_BUSY = 0;
    PENDING_MUSIC_CHANGES--;
    func_80014A90();
}

void waitForMusicChange(void) {
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (PENDING_MUSIC_CHANGES != 0);
}

void playMusic(s32 slotIndex, s32 trackId, s32 volume) {
    s8 *state;

    state = (s8 *)&SOUND_STATE;
    if ((*(s16 *)(state + slotIndex * 0xC + 0x20)) != trackId) {
        waitForMusicChange();
        func_800149B8(0, -1, 0, 0x1000, &changeMusicTask, slotIndex, trackId, volume, 1);
        return;
    }
    if (D_801D812A != slotIndex) {
        waitForMusicChange();
        func_800149B8(0, -1, 0, 0x1000, &changeMusicTask, slotIndex, trackId, volume, 0);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/system/sound_play", PATH_OPENSEG_BIN);

INCLUDE_RODATA("asm/main/nonmatchings/system/sound_play", PATH_DIGIMON_MOV);
