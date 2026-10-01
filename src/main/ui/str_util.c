#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/str_util.h"
#include "dcb/text.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"

s8 *copyString(s8 *dst, s8 *src) {
#if VERSION_JP
    while ((*dst = *src) != 0) {
        dst++;
        src++;
    }
    return dst;
#elif VERSION_US
    if ((*dst = *src) == 0) {
        return dst;
    }
    return copyString(dst + 1, src + 1);
#else
#error "untested version"
#endif
}

s8 *formatNumberWithCommas(s8 *buf, s8 pad, s32 value, s32 width) {
    s32 full; /* no room left in front for another digit */
    s8 *digit;
    s8 *shift;
    s32 digitCount;
    s8 c;

    buf += width;
    digit = buf;
    *buf = 0;
    digitCount = 0;
    do {
        digit--;
        c = value % 10 + '0';
        *digit = c;
        value /= 10;
        full = --width <= 0;
        if (full && value != 0) {
            buf++;
            for (shift = buf; digit < shift; shift--) {
                *shift = shift[-1];
            }
            digit++;
        }
        if (++digitCount % 3 == 0) {
            if (value == 0) {
                break;
            }
            *--digit = ',';
            if (--width <= 0) {
                buf++;
                for (shift = buf; digit < shift; shift--) {
                    *shift = shift[-1];
                }
                digit++;
            }
        }
    } while (value != 0);
    while (--width >= 0) {
        *--digit = pad;
    }
    return buf;
}

s8 *formatNumber(s8 *buf, s8 pad, s32 value, s32 width) {
    s8 *digit;
    s8 *shift;

    buf += width;
    digit = buf;
    *buf = 0;
    do {
        *--digit = value % 10 + '0';
        value /= 10;
        if (--width <= 0 && value != 0) {
            buf++;
            for (shift = buf; digit < shift; shift--) {
                *shift = shift[-1];
            }
            digit++;
        }
    } while (value != 0);
    while (--width >= 0) {
        *--digit = pad;
    }
    return buf;
}

void formatSignedNumber(s8 *buf, s32 value, s32 width) {
    if (value >= 0) {
        *buf++ = '+';
    } else {
        *buf++ = '-';
        value = -value;
    }
    formatNumber(buf, '0', value, width - 1);
}

s8 *formatOrdinalGlyphs(s8 *buf, s32 rank) {
    s8 *suffix;

    switch (rank) {
    case 1:
        suffix = "1ab";
        break;
    case 2:
        suffix = "2cd";
        break;
    case 3:
        suffix = "3ef";
        break;
    default:
        if (rank < 10) {
            return copyString(formatNumber(buf, ' ', rank, 1), "gh");
        }
        return copyString(formatNumber(buf, ' ', rank, 2), "i");
    }
    return copyString(buf, suffix);
}

s8 *centerString(s8 *dst, s8 *src, s32 width) {
    s32 padding;
    s32 i;
    s8 *p;

    padding = width - strlen(src);
    if (padding < 0) {
        p = dst;
        for (i = 0; i < width; i++) {
            *p++ = '*';
        }
    } else {
        padding /= 2;
        p = dst;
        while (padding-- > 0) {
            *p++ = ' ';
            width--;
        }
        while ((*p = *src++) != 0) {
            p++;
            width--;
        }
        while (width-- > 0) {
            *p++ = ' ';
        }
    }
    *p = 0;
    return p;
}

u16 readSjisChar(u8 **cursor) {
    u8 *s = *cursor;

    if (s[0] > 0x80 && (s[0] < 0xA0 || (s[0] >= 0xE0 && s[0] < 0xF0))) {
        if (s[1] >= 0x40 && (s[1] < 0x7F || (s[1] >= 0x80 && s[1] < 0xFD))) {
            *cursor += 2;
            return ((*cursor)[-2] << 8) | (*cursor)[-1];
        }
    }
    return *(*cursor)++;
}

u16 peekSjisChar(u8 *s) {
    if (s[0] > 0x80 && (s[0] < 0xA0 || (s[0] >= 0xE0 && s[0] < 0xF0))) {
        if (s[1] >= 0x40 && (s[1] < 0x7F || (s[1] >= 0x80 && s[1] < 0xFD))) {
            return (s[0] << 8) | s[1];
        }
    }
    return s[0];
}

s32 measureWideString(s16 *str) {
    s16 *ch;
    s32 width;
    s32 c;

    ch = str;
    width = 0;
#if VERSION_JP
    while ((c = *++ch) != 0) {
        if (c < 0) {
            width += 1;
        } else {
            width += 2;
        }
    }
#elif VERSION_US
loop:
    ch++;
    if (*ch != 0) {
        if (*ch < 0) {
            width += 1;
        } else {
            width += 2;
        }
        goto loop;
    }
#else
#error "untested version"
#endif
    *str = width;
    return width;
}

s16 *copyAsciiToWideString(s16 *dst, u8 *src) {
#if VERSION_JP
    while ((*dst = -*src) != 0) {
        src++;
        dst++;
    }
    return dst;
#elif VERSION_US
    if ((*dst = -*src) == 0) {
        return dst;
    }
    src++;
    dst++;
    return copyAsciiToWideString(dst, src);
#else
#error "untested version"
#endif
}

s16 *copyWideString(s16 *dst, s16 *src) {
#if VERSION_JP
    while ((*dst = *src) != 0) {
        dst++;
        src++;
    }
    return dst;
#elif VERSION_US
    if ((*dst = *src) == 0) {
        return dst;
    }
    return copyWideString(dst + 1, src + 1);
#else
#error "untested version"
#endif
}

s16 *formatWideNumber(s16 *buf, u8 pad, s32 value, s32 width) {
    s16 *digit;
    s16 *shift;
    s32 fill;

    fill = -pad;
    buf += width;
    digit = buf;
    *buf = 0;
    do {
        *--digit = -'0' - value % 10;
        value /= 10;
        if (--width <= 0 && value != 0) {
            buf++;
            for (shift = buf; digit < shift; shift--) {
                *shift = shift[-1];
            }
            digit++;
        }
    } while (value != 0);
    while (--width >= 0) {
        *--digit = fill;
    }
    return buf;
}

void formatWideSignedNumber(s16 *buf, s32 value, s32 width) {
    if (value >= 0) {
        *buf++ = -0x2B;
    } else {
        *buf++ = -0x2D;
        value = -value;
    }
    formatWideNumber(buf, 0x30, value, width - 1);
}

s16 *formatWideOrdinal(s16 *buf, s32 rank) {
    u8 *suffix;

    switch (rank) {
    case 1:
        suffix = "1st";
        break;
    case 2:
        suffix = "2nd";
        break;
    case 3:
        suffix = "3rd";
        break;
    default:
        return copyAsciiToWideString(formatWideNumber(buf, ' ', rank, 1), "th");
    }
    return copyAsciiToWideString(buf, suffix);
}

s8 *formatOrdinalUpper(s8 *buf, s32 rank) {
    s8 *suffix;

    switch (rank) {
    case 1:
        suffix = "1ST";
        break;
    case 2:
        suffix = "2ND";
        break;
    case 3:
        suffix = "3RD";
        break;
    default:
        return copyString(formatNumber(buf, ' ', rank, 1), "TH");
    }
    return copyString(buf, suffix);
}

#if VERSION_JP
/* jp's own font: the Shift-JIS characters it draws, 11x11 glyphs of 0x16
   bytes. A run of codes and the glyph its first code has: */
typedef struct {
    u16 code;
    u16 glyph;
} SjisGlyphRun;

extern u8 D_8006B8FC[];         /* the glyphs of the symbols, kana and the alphabets */
extern u8 D_8006E604[];         /* the glyphs of the kanji (0x889F-0x9872) */
extern SjisGlyphRun D_8007E4D4[]; /* the runs of lead bytes 0x81-0x84 */
extern SjisGlyphRun D_8007E520[]; /* the kanji, two runs per lead byte from 0x88 */

/* The glyph of the Shift-JIS character at str */
u8 *getSjisGlyph(u8 *str) {
    u16 code;
    u8 *glyphs;

    code = str[0] << 8 | str[1];
    if ((u16)(code - 0x8140) < 0x37F) {
        glyphs = D_8006B8FC;
    } else if ((u16)(code - 0x889F) < 0xFD4) {
        glyphs = D_8006E604;
    } else {
        glyphs = D_8006B8FC;
    }
    return glyphs + getSjisGlyphIndex(code) * 0x16;
}

/* The glyph number of a Shift-JIS code in its glyph table */
s32 getSjisGlyphIndex(u16 code) {
    u8 bytes[2];
    s32 glyph;
    u8 run;

    run = 0;
    bcopy(&code, bytes, 2);
    switch (bytes[1]) {
    case 0x81:
        if ((u8)(bytes[0] - 0x40) < 0x3F) {
            run = 0;
        } else if ((u8)(bytes[0] - 0x80) < 0x2D) {
            run = 1;
        } else if ((u8)(bytes[0] - 0xB8) < 8) {
            run = 2;
        } else if ((u8)(bytes[0] - 0xC8) < 7) {
            run = 3;
        } else if ((u8)(bytes[0] - 0xDA) < 0xF) {
            run = 4;
        } else if ((u8)(bytes[0] - 0xF0) < 8) {
            run = 5;
        } else if (bytes[0] == 0xFC) {
            run = 6;
        }
        glyph = code - D_8007E4D4[run].code + D_8007E4D4[run].glyph;
        break;
    case 0x82:
        if ((u8)(bytes[0] - 0x4F) < 0xA) {
            run = 7;
        } else if ((u8)(bytes[0] - 0x60) < 0x1A) {
            run = 8;
        } else if ((u8)(bytes[0] - 0x81) < 0x1A) {
            run = 9;
        } else if ((u8)(bytes[0] - 0x9F) < 0x53) {
            run = 10;
        }
        glyph = code - D_8007E4D4[run].code + D_8007E4D4[run].glyph;
        break;
    case 0x83:
        if ((u8)(bytes[0] - 0x40) < 0x3F) {
            run = 11;
        } else if ((u8)(bytes[0] - 0x80) < 0x17) {
            run = 12;
        } else if ((u8)(bytes[0] - 0x9F) < 0x18) {
            run = 13;
        } else if ((u8)(bytes[0] - 0xBF) < 0x18) {
            run = 14;
        }
        glyph = code - D_8007E4D4[run].code + D_8007E4D4[run].glyph;
        break;
    case 0x84:
        if ((u8)(bytes[0] - 0x40) < 0x21) {
            run = 15;
        } else if ((u8)(bytes[0] - 0x70) < 0xF) {
            run = 16;
        } else if ((u8)(bytes[0] - 0x80) < 0x12) {
            run = 17;
        } else if ((u8)(bytes[0] - 0x9F) < 0x20) {
            run = 18;
        }
        glyph = code - D_8007E4D4[run].code + D_8007E4D4[run].glyph;
        break;
    default:
        /* a lead byte's codes are two runs, split at the trail byte 0x7F */
        run = (bytes[1] - 0x88) * 2 - (bytes[0] < 0x7F);
        glyph = code - D_8007E520[run].code + D_8007E520[run].glyph;
        break;
    }
    return glyph;
}
#endif
