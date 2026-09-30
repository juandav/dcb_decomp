#ifndef DCB_CARD_RENDER_H
#define DCB_CARD_RENDER_H

#include "game.h"
#include "dcb/duel_launch.h"

void uploadStringGlyphs(u8 *string, s32 row, s32 parentTask);
void runCardArtLoader(void);
void drawHudSprite(SprtInfo *info, s32 unused, s32 z);
void renderDuelBackground(s32 brightness);
void drawCardArtPlaceholder(s32 x, s32 y, s32 z, s32 index, CardSprite *cardSprite);
void renderPhaseBanner(void);
void renderStatusMessage(s32 brightness);
void renderHelpBar(s32 brightness);
void drawTurnSideBadge(s32 x, s32 y, s32 side, s32 brightness, s32 z);
void drawWinMarker(s32 x, s32 y, s32 z);
void resetCardPolyCount(void);
void projectCardSprite(CardSprite *sprite, s32 spriteIndex);
void renderCardSprite(CardSprite *sprite, s32 spriteIndex);
MATRIX *buildRotTransMatrix(VECTOR *pos, SVECTOR *rot, MATRIX *m);

#endif /* DCB_CARD_RENDER_H */
