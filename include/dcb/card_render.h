#ifndef DCB_CARD_RENDER_H
#define DCB_CARD_RENDER_H

#include "game.h"
#include "dcb/duel_launch.h"

void uploadStringGlyphs(u8 *string, s32 row, s32 parentTask);
void runCardArtLoader(void);
void drawHudSprite(SprtInfo *info, s32 unused, s32 z);
void renderDuelBackground(s32 brightness);
#if VERSION_JP
/* card_render_jp.c: jp's HUD panels, and its HUD sprites */
void initHudPanels(void);
void freeHudPanels(void);
void renderHudPanel(struct Panel *panel, s32 index, s32 z);
void drawCardArtPlaceholder(s32 x, s32 y, s32 z);
void drawWinMarker(s32 x, s32 y, s32 size, s32 z);
#elif VERSION_US || VERSION_EU
void drawCardArtPlaceholder(s32 x, s32 y, s32 z, s32 index, CardSprite *cardSprite);
void renderPhaseBanner(void);
void renderStatusMessage(s32 brightness);
void renderHelpBar(s32 brightness);
void drawWinMarker(s32 x, s32 y, s32 z);
#else
#error "card_render.h: version not checked"
#endif
void drawTurnSideBadge(s32 x, s32 y, s32 side, s32 brightness, s32 z);
void resetCardPolyCount(void);
void projectCardSprite(CardSprite *sprite, s32 spriteIndex);
void renderCardSprite(CardSprite *sprite, s32 spriteIndex);
MATRIX *buildRotTransMatrix(VECTOR *pos, SVECTOR *rot, MATRIX *m);

#endif /* DCB_CARD_RENDER_H */
