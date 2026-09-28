#ifndef DCB_DUEL_LAUNCH_H
#define DCB_DUEL_LAUNCH_H

#include "game.h"

/* a POLY_FT4 filled in whole words */
typedef struct {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 rgbc;
    /* 0x08 */ u32 xy0;
    /* 0x0C */ u32 uv0;
    /* 0x10 */ u32 xy1;
    /* 0x14 */ u32 uv1;
    /* 0x18 */ u32 xy2;
    /* 0x1C */ u32 uv2;
    /* 0x20 */ u32 xy3;
    /* 0x24 */ u32 uv3;
} RawPolyFT4;

extern s32 D_8006E294;
extern s32 DUEL_VRAM_READY;
extern s32 CARD_ART_LAST_SPRITE;
extern s8 STATUS_STEP_SPRITES[];
extern DR_AREA STATUS_MSG_RESTORE_AREA[2];
extern DR_AREA STATUS_MSG_CLIP_AREA[2];
extern u8 *STATUS_MESSAGE_TEXTS[];
extern DR_AREA HELP_BAR_RESTORE_AREA[2];
extern DR_AREA HELP_BAR_CLIP_AREA[2];
extern u8 *HELP_BAR_TEXTS[];
extern u8 *D_8006E31C[];
extern s32 CARD_POLY_COUNT;

void startCpuDuel(s32 deckIndex);
void startVersusDuel(void);

#endif /* DCB_DUEL_LAUNCH_H */
