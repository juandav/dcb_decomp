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
    if ((*dst = *src) == 0) {
        return dst;
    }
    return copyString(dst + 1, src + 1);
}

s8 *formatNumberWithCommas(s8 *buf, s8 pad, s32 value, s32 width) {
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
        if (--width <= 0 && value != 0) {
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

    ch = str;
    width = 0;
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
    *str = width;
    return width;
}

s16 *copyAsciiToWideString(s16 *dst, u8 *src) {
    if ((*dst = -*src) == 0) {
        return dst;
    }
    src++;
    dst++;
    return copyAsciiToWideString(dst, src);
}

s16 *copyWideString(s16 *dst, s16 *src) {
    if ((*dst = *src) == 0) {
        return dst;
    }
    return copyWideString(dst + 1, src + 1);
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
