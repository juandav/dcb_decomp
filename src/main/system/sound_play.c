#include "dcb/sound_play.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
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

    SsUtKeyOnV(NEXT_SFX_VOICE, SOUND_STATE.seBank.vab, sound >> 4, tone, SFX_BASE_NOTE,
               SFX_BASE_FINE + tone, 0x6E, 0x6E);
    if (++NEXT_SFX_VOICE >= 0x16) {
        NEXT_SFX_VOICE = 0x12;
    }
}

void playSoundEffectAtVolume(s32 sound, s32 volume) {
    s32 tone = sound & 0xF;

    SsUtKeyOnV(NEXT_SFX_VOICE, SOUND_STATE.seBank.vab, sound >> 4, tone, SFX_BASE_NOTE,
               SFX_BASE_FINE + tone, volume, volume);
    if (++NEXT_SFX_VOICE >= 0x16) {
        NEXT_SFX_VOICE = 0x12;
    }
}

void playSoundEffectOnVoice(s32 voice, s32 sound) {
    s32 tone = sound & 0xF;

    SsUtKeyOnV(voice, SOUND_STATE.seBank.vab, sound >> 4, tone, SFX_BASE_NOTE,
               SFX_BASE_FINE + tone, 0x6E, 0x6E);
}

void stopSoundVoice(s16 voice) {
    SsUtKeyOffV(voice);
}

void stopAllSoundEffects(void) {
    SsUtAllKeyOff(0);
}

void stopMusic(void) {
    endTask(0x1C);
    if (SOUND_STATE.cur >= 0) {
        SsSeqStop(SOUND_STATE.seq[SOUND_STATE.cur]);
#if VERSION_US || VERSION_EU
        waitFrames(4);
#endif
        SOUND_STATE.cur = -1;
    }
}

void fadeOutMusicTask(s32 slotIndex, s32 step) {
    s16 volL;
    s16 volR;

    for (;;) {
        waitFrames(FRAME_INTERVAL);
        if (SOUND_STATE.cur != slotIndex) {
            exitTask();
        }
        SsSeqGetVol(SOUND_STATE.seq[slotIndex], 0, &volL, &volR);
        if (volL == 0) {
            SsSeqStop(SOUND_STATE.seq[slotIndex]);
#if VERSION_US || VERSION_EU
            waitFrames(4);
#endif
            SOUND_STATE.cur = -1;
            exitTask();
        }
        volL -= step;
        if (volL < 0) {
            volL = 0;
        }
        SsSeqSetVol(SOUND_STATE.seq[slotIndex], volL, volL);
    }
}

void fadeOutMusic(s32 step) {
    if (SOUND_STATE.cur >= 0) {
        endTask(0x1C);
        spawnTask(0x1C, -1, 0, 0x1000, &fadeOutMusicTask, SOUND_STATE.cur, step);
    }
}

/* left empty; nothing calls it */
void emptyMusicFunction(void) {
}

void playLoadedMusic(s32 slotIndex) {
    if (SOUND_STATE.slot[slotIndex].id != 0xFF) {
        if (SOUND_STATE.cur >= 0) {
            stopMusic();
        }
        SsSeqPlay(SOUND_STATE.seq[slotIndex], 1, 0);
        SsSeqSetVol(SOUND_STATE.seq[slotIndex],
                    SOUND_STATE.vol[slotIndex],
                    SOUND_STATE.vol[slotIndex]);
        SOUND_STATE.cur = slotIndex;
    }
}

void changeMusicTask(s32 slotIndex, s32 trackId, s32 volume, s32 needsLoad) {
    PENDING_MUSIC_CHANGES++;
    do {
        waitFrames(FRAME_INTERVAL);
    } while (MUSIC_CHANGE_BUSY != 0);
    MUSIC_CHANGE_BUSY = 1;
    if (SOUND_STATE.cur >= 0) {
        fadeOutMusic(2);
        while (SOUND_STATE.cur >= 0) {
            waitFrames(FRAME_INTERVAL);
        }
    }
    if (needsLoad) {
        loadMusicTrack(slotIndex, trackId, volume);
    }
    playLoadedMusic(slotIndex);
    MUSIC_CHANGE_BUSY = 0;
    PENDING_MUSIC_CHANGES--;
    exitTask();
}

#if VERSION_US || VERSION_EU
void waitForMusicChange(void) {
    do {
        waitFrames(FRAME_INTERVAL);
    } while (PENDING_MUSIC_CHANGES != 0);
}
#endif

/* us and eu wait for the music changes already under way first */
void playMusic(s32 slotIndex, s32 trackId, s32 volume) {
    if (SOUND_STATE.slot[slotIndex].id != trackId) {
#if VERSION_US || VERSION_EU
        waitForMusicChange();
#endif
        spawnTask(0, -1, 0, 0x1000, &changeMusicTask, slotIndex, trackId, volume, 1);
        return;
    }
    if (SOUND_STATE.cur != slotIndex) {
#if VERSION_US || VERSION_EU
        waitForMusicChange();
#endif
        spawnTask(0, -1, 0, 0x1000, &changeMusicTask, slotIndex, trackId, volume, 0);
    }
}

#if VERSION_JP
/* jp has it after playMusic, which doesn't call it yet */
void waitForMusicChange(void) {
    do {
        waitFrames(FRAME_INTERVAL);
    } while (PENDING_MUSIC_CHANGES != 0);
}
#endif
