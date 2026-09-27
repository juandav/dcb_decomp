#ifndef DCB_SOUND_H
#define DCB_SOUND_H

#include "game.h"

typedef struct {
    /* 0x0 */ s16 id;
    /* 0x2 */ s16 vab;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ u8 *buf;
} SndSlot;
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 cur;
    /* 0x04 */ s16 seq[2];
    /* 0x08 */ u8 vol[2];
    /* 0x0A */ u8 unkA[2];
    /* 0x0C */ u8 *data[2];
    /* 0x14 */ SndSlot unk14;
    /* 0x20 */ SndSlot slot[2];
} SndState;

extern s32 SOUND_SEQ_ATTR_TABLE;
extern s32 SOUND_STATE;
extern s32 SOUND_LOAD_BUSY;
extern s8 *SE_BANK_INFO[];
extern s32 SFX_BASE_NOTE;
extern s32 PATH_OPENSEG_BIN;
extern s32 PATH_DIGIMON_MOV;
extern s32 NEXT_SFX_VOICE;
extern u16 D_8006E04C;
extern s16 D_801D813E;
extern s16 D_801D812A;

void initSound();
void playOpeningMovie(s32 movieMode, s32 parentTask);
void loadSoundEffectBank(s32 bankId);
void stopMusic(void);
void setReverbType(s32 reverbType);
void stopAllSoundEffects(void);
s32 openSlotVabHeader(void *slot, s16 vabId, s32 spuAddr);
void transferSlotVabBody(void *slot, s32 vabBody, s32 vab);
void setInstantVoiceRelease(void);
void func_8002B3DC(void);
void func_8002B3E4(void);
void playSoundEffect(s32 sound);
void playSoundEffectAtVolume(s32 sound, s32 volume);
void playSoundEffectOnVoice(s32 voice, s32 sound);
void stopSoundVoice(s16 voice);
void fadeOutMusicTask(s32 slotIndex, s32 step);
void fadeOutMusic(s32 step);
void func_8002B850(void);
void playLoadedMusic(s32 slotIndex);
void loadMusicTrack(s32 slotIndex, s32 trackId, u8 volume);
void changeMusicTask(s32 slotIndex, s32 trackId, s32 volume, s32 needsLoad);
void waitForMusicChange(void);
void playMusic(s32 slotIndex, s32 trackId, s32 volume);

#endif /* DCB_SOUND_H */
