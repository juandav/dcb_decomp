#ifndef DCB_SOUND_H
#define DCB_SOUND_H

#include "game.h"

typedef struct {
    /* 0x0 */ s16 id;
    /* 0x2 */ s16 vab;
    /* 0x4 */ s32 vabHeaderSize;
    /* 0x8 */ u8 *buf;
} SndSlot;
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 cur;
    /* 0x04 */ s16 seq[2];
    /* 0x08 */ u8 vol[2];
    /* 0x0A */ u8 unkA[2];
    /* 0x0C */ u8 *data[2];
    /* 0x14 */ SndSlot seBank;
    /* 0x20 */ SndSlot slot[2];
} SndState;

extern s32 SOUND_SEQ_ATTR_TABLE;
extern SndState SOUND_STATE;
extern s32 SOUND_LOAD_BUSY;
extern s8 *SE_BANK_INFO[];
extern s32 SFX_BASE_NOTE;
extern s32 NEXT_SFX_VOICE;
extern u16 SFX_BASE_FINE;

void initSound();
void loadSoundEffectBank(s32 bankId);
void setReverbType(s32 reverbType);
s32 openSlotVabHeader(SndSlot *slot, s16 vabId, s32 spuAddr);
void transferSlotVabBody(SndSlot *slot, s32 vabBody, s32 vab);
void setInstantVoiceRelease(void);
void emptySoundFunction1(void);
void emptySoundFunction2(void);
void loadMusicTrack(s32 slotIndex, s32 trackId, u8 volume);

#endif /* DCB_SOUND_H */
