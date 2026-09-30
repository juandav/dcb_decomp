#ifndef DCB_SOUND_PLAY_H
#define DCB_SOUND_PLAY_H

#include "game.h"
#include "dcb/sound.h"

void stopMusic(void);
void stopAllSoundEffects(void);
void playSoundEffect(s32 sound);
void playSoundEffectAtVolume(s32 sound, s32 volume);
void playSoundEffectOnVoice(s32 voice, s32 sound);
void stopSoundVoice(s16 voice);
void fadeOutMusicTask(s32 slotIndex, s32 step);
void fadeOutMusic(s32 step);
void emptyMusicFunction(void);
void playLoadedMusic(s32 slotIndex);
void changeMusicTask(s32 slotIndex, s32 trackId, s32 volume, s32 needsLoad);
void waitForMusicChange(void);
void playMusic(s32 slotIndex, s32 trackId, s32 volume);

#endif /* DCB_SOUND_PLAY_H */
