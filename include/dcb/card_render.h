#ifndef DCB_CARD_RENDER_H
#define DCB_CARD_RENDER_H

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
extern char STR_THINKING[];
extern s32 CARD_POLY_COUNT;

void startCpuDuel(s32 deckIndex);
void startVersusDuel(void);
void uploadStringGlyphs(u8 *string, s32 row, s32 parentTask);
void runCardArtLoader(void);
void drawHudSprite(SprtInfo *info, s32 unused, s32 z);
void renderDuelBackground(s32 brightness);
void drawCardArtPlaceholder(s32 x, s32 y, s32 z, s32 index, u8 *cardSprite);
void renderPhaseBanner(void);
void renderStatusMessage(s32 brightness);
void renderHelpBar(s32 brightness);
void func_80044504(s32 x, s32 y, s32 n, s32 brightness, s32 z);
void drawWinMarker(s32 x, s32 y, s32 z);
void resetCardPolyCount(void);
void projectCardSprite(void *cardSprite, s32 spriteIndex);
void renderCardSprite(CardSprite *sprite, s32 spriteIndex);
MATRIX *buildRotTransMatrix(VECTOR *pos, SVECTOR *rot, MATRIX *m);

#endif /* DCB_CARD_RENDER_H */
