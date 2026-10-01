#ifndef DCB_TEXT_H
#define DCB_TEXT_H

#include "game.h"

#define setSprt(p) setlen(p, 4), setcode(p, 0x64)

extern u16 SPRITE_POOL_SIZE;
extern u16 SYSTEM_CLUT_X;
extern s16 SYSTEM_CLUT_Y;
extern s32 DEFAULT_TEXT_RGB;
extern u8 FONT_GLYPH_METRICS[];

s32 isSpritePoolFull(void);
void resetSpritePool(void);
void drawSmallText(s32 x, s32 y, s32 text, s32 palette, s32 z);
void drawVerticalText(s32 x, s32 y, s32 text, s32 palette, s32 z);
void drawText(s32 x, s32 y, s32 text, s32 palette, s32 z);
void initSpritePoolPackets(void);
void drawPageSpriteColored(s32 x, s32 y, Rect16 *uvRect, u8 *rgb, u16 tpage, s32 palette, s32 z);
void drawPageSprite(s32 x, s32 y, s32 uvRect, u16 tpage, s32 palette, s32 z);
void drawSystemSpriteColored(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 palette, u8 *rgb, s32 z);
void drawSystemSprite(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h,
                   s32 palette, s32 z);
void drawGlyphColored(s32 x, s32 y, u8 ch, s32 palette, u8 *rgb, s32 z, s32 w, s32 h, s32 baseU, s32 baseV);
void drawGlyph(s32 x, s32 y, u8 ch, s32 palette, s32 z, s32 w, s32 h, s32 baseU, s32 baseV);
void drawTinyTextColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z);
void drawTinyText(s32 x, s32 y, s32 text, s32 palette, s32 z);
void drawIconColored(s32 x, s32 y, s32 iconSet, s32 icon, u8 *rgb, s32 z);
void drawSmallTextColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z);
void drawVerticalTextColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z);
void drawMediumTextColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z);
void drawMediumText(s32 x, s32 y, s32 text, s32 palette, s32 z);
void drawLargeTextColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z);
void drawLargeText(s32 x, s32 y, s32 text, s32 palette, s32 z);
void drawIcon(s32 x, s32 y, s32 iconSet, s32 icon, s32 z);
s32 drawIconTextColored(s32 x, s32 y, s32 palette, s32 unused, u8 *rgb, s32 z, u8 *text);
void drawIconText(s32 x, s32 y, s32 palette, s32 unused, s32 z, s32 text);
#if VERSION_JP
void initSystemSprites(s32 vramX, s32 vramY, s32 poolSize, s32 kanjiBuffers);
s32 measureText(s32 proportional, u8 *text);
void drawTextColored(s32 x, s32 y, u8 *text, u8 *rgb, s32 palette, s32 z);
void drawBigDigits(s32 x, s32 y, u8 *text, u8 *rgb, s32 z);
/* forgets the kanji cached in VRAM */
void resetKanjiPages(void);
void openKanjiPage(s32 page, s32 capacity);
void closeKanjiPage(s32 page);
void clearKanjiPage(s32 page);
#elif VERSION_US || VERSION_EU
void initSystemSprites(s32 vramX, s32 vramY, s32 poolSize);
s32 measureText(u8 *);
s32 drawTextColored(s32 x, s32 y, u8 *text, u8 *rgb, s32 palette, s32 z);
void drawBigDigits(s32 x, s32 y, u8 *text, u8 *rgb, s32 palette, s32 z);
#else
#error "main/ui/text: version not checked"
#endif

#endif /* DCB_TEXT_H */
