#ifndef DCB_STR_UTIL_H
#define DCB_STR_UTIL_H

#include "game.h"

s8 *formatNumberWithCommas(s8 *buf, s8 pad, s32 value, s32 width);
s8 *formatNumber(s8 *buf, s8 pad, s32 value, s32 width);
void formatSignedNumber(s8 *buf, s32 value, s32 width);
s8 *formatOrdinalGlyphs(s8 *buf, s32 rank);
s8 *centerString(s8 *dst, s8 *src, s32 width);
u16 readSjisChar(u8 **cursor);
u16 peekSjisChar(u8 *s);
s32 measureWideString(s16 *str);
s16 *copyAsciiToWideString(s16 *dst, u8 *src);
s16 *copyWideString(s16 *dst, s16 *src);
s16 *formatWideNumber(s16 *buf, u8 pad, s32 value, s32 width);
void formatWideSignedNumber(s16 *buf, s32 value, s32 width);
s16 *formatWideOrdinal(s16 *buf, s32 rank);
s8 *formatOrdinalUpper(s8 *buf, s32 rank);
#if VERSION_JP
u8 *getSjisGlyph(u8 *str);
s32 getSjisGlyphIndex(u16 code);
#endif

#include "dcb/text.h"

s8 *copyString(s8 *dst, s8 *src);

#endif /* DCB_STR_UTIL_H */
