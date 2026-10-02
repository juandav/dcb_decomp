#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/text.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/str_util.h"

/* jp's text (text.c is us's and eu's): the system sprites come from
   B:\SYSTEM.ARC with four palettes per CLUT row, the small fonts are
   half-width kana, and kanji are drawn from glyphs that are uploaded to
   VRAM pages on first use and cached there */

/* a cached kanji: its Shift-JIS code and the columns its pixels span */
typedef struct {
    /* 0x0 */ u16 sjis;
    /* 0x2 */ u16 left;
    /* 0x4 */ u16 right;
} KanjiEntry;

/* one of the 32 texture pages (16 across, 2 down) that cache kanji: its
   cells are 3 halfwords by 11 lines, 21 to a row */
typedef struct {
    /* 0x0 */ u16 capacity;
    /* 0x2 */ u16 count;
    /* 0x4 */ u16 tpage;
    /* 0x8 */ KanjiEntry *entries;
} KanjiPage;

/* where a kanji is: its cell in VRAM, its columns and the page's tpage */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ u16 left;
    /* 0x6 */ u16 right;
    /* 0x8 */ u16 tpage;
} KanjiGlyph;

extern u16 KANJI_BUFFER_COUNT;
extern u8 *KANJI_BUFFERS; /* a ring of 0x48-byte glyphs waiting for LoadImage */
extern u16 KANJI_BUFFER_INDEX;
extern KanjiPage *KANJI_PAGES;
extern KanjiEntry LAST_KANJI;

s32 DEFAULT_TEXT_RGB = 0x808080;

/* the half-width katakana of each kana, from 1 for ァ and ぁ on (its trail
   byte less 0x3F for a katakana, 0x40 past 0x7F, which Shift-JIS skips, and
   0x9E for a hiragana): a voiced kana, which takes two half-width
   characters, has the index of its pair in TINY_KANA_PAIRS instead */
s8 SJIS_KANA_GLYPHS[85] = {
    0xA0, 0xA7, 0xB1, 0xA8, 0xB2, 0xA9, 0xB3, 0xAA, 0xB4, 0xAB, 0xB5, 0xB6, 0x00, 0xB7, 0x01, 0xB8, 0x02,
    0xB9, 0x03, 0xBA, 0x04, 0xBB, 0x05, 0xBC, 0x06, 0xBD, 0x07, 0xBE, 0x08, 0xBF, 0x09, 0xC0, 0x0A, 0xC1,
    0x0B, 0xAF, 0xC2, 0x0C, 0xC3, 0x0D, 0xC4, 0x0E, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0x0F, 0x10, 0xCB,
    0x11, 0x12, 0xCC, 0x13, 0x14, 0xCD, 0x15, 0x16, 0xCE, 0x17, 0x18, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xAC,
    0xD4, 0xAD, 0xD5, 0xAE, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xAC, 0xDC, 0xA0, 0xA0, 0xA6, 0xDD, 0x19,
};

/* the voiced kana as half-width katakana and their voicing mark */
u8 TINY_KANA_PAIRS[52] = "ｶﾞｷﾞｸﾞｹﾞｺﾞｻﾞｼﾞｽﾞｾﾞｿﾞﾀﾞﾁﾞﾂﾞﾃﾞﾄﾞﾊﾞﾊﾟﾋﾞﾋﾟﾌﾞﾌﾟﾍﾞﾍﾟﾎﾞﾎﾟｳﾞ";

/* the full-width digits */
u8 SJIS_DIGITS[] = "０１２３４５６７８９";

void allocKanjiPages(void);
void uploadKanji(u8 *sjis, Rect16 *rect);
s32 findKanji(u8 *sjis, KanjiGlyph *glyph);
s32 addKanji(u8 *sjis, KanjiGlyph *glyph);
void drawIconColored(s32 x, s32 y, s32 iconSet, s32 icon, u8 *rgb, s32 z);

void initSystemSprites(s32 vramX, s32 vramY, s32 poolSize, s32 kanjiBuffers) {
    Rect16 r;
    u32 *arc;
    SprtPacket *pool;
    u16 *clut;
    s32 i;

    SYSTEM_TEX_X = vramX;
    SYSTEM_TEX_Y = vramY;
    SYSTEM_CLUT_X = vramX;
    SYSTEM_CLUT_Y = vramY + 0xFC;
    SPRITE_POOL_SIZE = poolSize;
    KANJI_BUFFER_COUNT = kanjiBuffers;
    KANJI_BUFFER_INDEX = 0;
    allocKanjiPages();
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\SYSTEM.ARC", getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTim((u32 *)((u8 *)arc + arc[0]), SYSTEM_TEX_X, SYSTEM_TEX_Y, -2, -2);
    r.x = SYSTEM_CLUT_X;
    r.y = SYSTEM_CLUT_Y;
    r.w = 0x40;
    r.h = 4;
    clut = (u16 *)LOADED_TIM.caddr;
    LoadImage((s16 *)&r, (s32)clut);
    DrawSync(0);
    /* the same palettes, semi-transparent, in the four rows above */
    for (i = 0; i < 0x100; i++, clut++) {
        if (*clut != 0) {
            *clut |= 0x8000;
        }
    }
    r.x = SYSTEM_CLUT_X;
    r.y = SYSTEM_CLUT_Y - 4;
    r.w = 0x40;
    r.h = 4;
    LoadImage((s16 *)&r, (s32)LOADED_TIM.caddr);
    DrawSync(0);
    for (i = 1; i < 5; i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    pool = allocPermanentHeapBlock(SPRITE_POOL_SIZE * sizeof(SprtPacket) * 2);
    KANJI_BUFFERS = allocPermanentHeapBlock(KANJI_BUFFER_COUNT * 0x48);
    for (i = 0; i < 2; i++) {
        DB(i).spritePool = (s32)(pool + SPRITE_POOL_SIZE * i);
    }
    initSpritePoolPackets();
    SPRITE_POOL_CURSOR = CURRENT_FRAME_BUFFER->spritePool;
}

void initSpritePoolPackets(void) {
    s32 i;
    s32 j;

    if (SPRITE_POOL_SIZE == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < SPRITE_POOL_SIZE; j++) {
            setDrawMode(&((SprtPacket *)DB(i).spritePool)[j].dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
            setSprt(&((SprtPacket *)DB(i).spritePool)[j].sp);
            setSemiTrans(&((SprtPacket *)DB(i).spritePool)[j].sp, 1);
            setShadeTex(&((SprtPacket *)DB(i).spritePool)[j].sp, 0);
            setRGB0(&((SprtPacket *)DB(i).spritePool)[j].sp, 0x80, 0x80, 0x80);
        }
    }
}

void resetSpritePool(void) {
    SPRITE_POOL_CURSOR = CURRENT_FRAME_BUFFER->spritePool;
}

void drawPageSprite(s32 x, s32 y, s32 uvRect, u16 tpage, s32 palette, s32 z) {
    drawPageSpriteColored(x, y, (Rect16 *)uvRect, (u8 *)&DEFAULT_TEXT_RGB, tpage, palette, z);
}

void drawPageSpriteColored(s32 x, s32 y, Rect16 *uvRect, u8 *rgb, u16 tpage, s32 palette, s32 z) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = uvRect->x;
        CUR_SPRT->sp.v0 = uvRect->y;
        CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
        CUR_SPRT->sp.w = uvRect->w;
        CUR_SPRT->sp.h = uvRect->h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, tpage);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void drawSystemSprite(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h,
                   s32 palette, s32 z) {
    drawSystemSpriteColored(x, y, u, v, w, h, palette, (u8 *)&DEFAULT_TEXT_RGB, z);
}

void drawSystemSpriteColored(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 palette, u8 *rgb, s32 z) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = u;
        CUR_SPRT->sp.v0 = v;
        CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void drawGlyph(s32 x, s32 y, u8 ch, s32 palette, s32 z, s32 w, s32 h, s32 baseU, s32 baseV) {
    drawGlyphColored(x, y, ch, palette, (u8 *)&DEFAULT_TEXT_RGB, z, w, h, baseU, baseV);
}

void drawGlyphColored(s32 x, s32 y, u8 ch, s32 palette, u8 *rgb, s32 z, s32 w, s32 h, s32 baseU, s32 baseV) {
    if (ch > 0x20 && isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = baseU + (ch & 0xF) * 8;
        CUR_SPRT->sp.v0 = baseV + (((ch - 0x20) & 0xF0) >> 1);
        CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void drawTinyText(s32 x, s32 y, s32 text, s32 palette, s32 z) {
    drawTinyTextColored(x, y, (u8 *)text, palette, (u8 *)&DEFAULT_TEXT_RGB, z);
}

/* 8x8 half-width kana: '~' switches between katakana and hiragana, a
   (han)dakuten (0xDE, 0xDF) is drawn over the character before it, and
   \001 n draws icon n - 1 */
void drawTinyTextColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z) {
    s32 left;
    u16 clut;
    s32 hiragana;
    s32 ch;
    s32 v;

    left = x;
    hiragana = 0;
    clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
    while (*text != 0) {
        switch (*text) {
        case 1:
            text++;
            palette = *text++;
            drawIconColored(x, y, 1, palette - 1, rgb, z);
            x += 8;
            break;
        case '\f':
            text++;
            palette = *text++;
            clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
            break;
        case '~':
            hiragana ^= 1;
            text++;
            break;
        case '\n':
            x = left;
            y += 9;
            text++;
            break;
        case ' ':
            x += 8;
            text++;
            break;
        default:
            ch = *text++;
            if (isSpritePoolFull() != 0) {
                return;
            }
            v = 0;
            if (ch >= 0xDE) {
                CUR_SPRT->sp.x0 = x - 2;
                CUR_SPRT->sp.y0 = y - 2;
            } else {
                v = 0x10;
                if (ch >= 0xA0) {
                    v = ((u8)hiragana == 0) << 5;
                }
                CUR_SPRT->sp.x0 = x;
                CUR_SPRT->sp.y0 = y;
                x += 8;
            }
            CUR_SPRT->sp.u0 = (ch & 0xF) * 8;
            CUR_SPRT->sp.v0 = v + (((ch - 0x20) & 0xF0) >> 1);
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 8;
            CUR_SPRT->sp.h = 8;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            break;
        }
    }
}

void drawText(s32 x, s32 y, s32 text, s32 palette, s32 z) {
    drawTextColored(x, y, (u8 *)text, (u8 *)&DEFAULT_TEXT_RGB, palette, z);
}

/* digits and a few symbols, 8x16 or ('b') 16x24: 'c' n changes the
   palette, 's' goes back to the small size and 'w' n sets the spacing */
void drawTextColored(s32 x, s32 y, u8 *text, u8 *rgb, s32 palette, s32 z) {
    s32 width;
    s32 height;
    s32 spacing;
    u16 clut;
    s32 ch;
    s32 u;
    s32 v;

    width = 8;
    height = 0x10;
    spacing = 0;
    clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
    while (*text != 0) {
        switch (*text) {
        case ' ':
            x += width + spacing;
            text++;
            break;
        case 'c':
            text++;
            palette = *text++ & 0xF;
            clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
            break;
        case 'b':
            width = 0x10;
            height = 0x18;
            text++;
            break;
        case 's':
            width = 8;
            height = 0x10;
            text++;
            break;
        case 'w':
            text++;
            if (*text == '-') {
                text++;
                spacing = '0' - *text++;
            } else {
                spacing = *text++ - '0';
            }
            break;
        default:
            ch = *text++;
            if (ch == '/' && width == 8) {
                u = 0xD0;
                v = 0x80;
                ch = 0;
            } else if (ch == '(' && width == 8) {
                u = 0xD8;
                v = 0x80;
                ch = 0;
            } else if (ch == ')' && width == 8) {
                u = 0xE0;
                v = 0x80;
                ch = 0;
            } else if (width == 0x10) {
                u = 0;
                v = 0x90;
            } else {
                u = 0x80;
                v = 0x80;
            }
            if (isSpritePoolFull() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += width + spacing;
            CUR_SPRT->sp.u0 = u + (ch & 0xF) * width;
            CUR_SPRT->sp.v0 = v;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = width;
            CUR_SPRT->sp.h = height;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            break;
        }
    }
}

/* 16x24 digits in palette 14, '+' and '-' and a 'p' 8 pixels lower,
   drawn subtractively */
void drawBigDigits(s32 x, s32 y, u8 *text, u8 *rgb, s32 z) {
    s32 height;
    u16 clut;
    s32 ch;
    s32 v;
    s32 u;

    height = 0x18;
    clut = getClut(SYSTEM_CLUT_X + 0x20, SYSTEM_CLUT_Y + 3);
    while (*text != 0) {
        if (*text == ' ') {
            x += 0x10;
            text++;
            continue;
        }
        ch = *text++;
        if (ch == '+' || ch == '-') {
            u = 0xE0;
            v = 0x90;
            ch = 0;
        } else if (ch == 'p') {
            u = 0xD8;
            v = 0xA8;
            ch = 0;
            y += 8;
            height = 0x10;
        } else {
            u = 0;
            v = 0xE0;
        }
        if (isSpritePoolFull() != 0) {
            return;
        }
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        x += 0xE;
        CUR_SPRT->sp.u0 = u + (ch & 0xF) * 16;
        CUR_SPRT->sp.v0 = v;
        CUR_SPRT->sp.clut = clut;
        CUR_SPRT->sp.w = 0x10;
        CUR_SPRT->sp.h = height;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 2, SYSTEM_TEX_X, SYSTEM_TEX_Y));
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/* uploads the kanji of a Shift-JIS string (0x1A ends it, \r\n breaks the
   line) next to each other from rect's corner; returns the longest line's
   length in characters */
s32 uploadKanjiString(u8 *text, Rect16 *rect) {
    s32 left;
    s32 count;
    s32 longest;

    left = rect->x;
    count = 0;
    longest = 0;
    while (*text != 0) {
        if (*text == 0x1A) {
            break;
        }
        if (*text == '\r') {
            rect->x = left;
            rect->y += 12;
            text++;
            if (*text == '\n') {
                text++;
            }
            if (longest < count) {
                longest = count;
            }
            count = 0;
        } else {
            uploadKanji(text, rect);
            rect->x += 3;
            text += 2;
            count++;
        }
    }
    if (longest < count) {
        longest = count;
    }
    return longest;
}

/* expands the 12x11 1-bit glyph of a kanji to 4 bits into the next upload
   buffer, notes the columns it spans in LAST_KANJI and loads it at rect */
void uploadKanji(u8 *sjis, Rect16 *rect) {
    u8 *dst;
    u8 *glyph;
    s32 row;
    s32 col;
    u16 bits;
    u8 lo;
    u8 hi;

    dst = KANJI_BUFFERS + KANJI_BUFFER_INDEX * 0x48;
    glyph = getSjisGlyph(sjis);
    LAST_KANJI.left = 10;
    LAST_KANJI.right = 0;
    for (row = 0; row < 11; row++) {
        bits = (glyph[row * 2] << 8) | glyph[row * 2 + 1];
        for (col = 0; col < 12; col += 2, dst++) {
            lo = (bits & (0x8000 >> col)) >> (15 - col);
            hi = (bits & (0x4000 >> col)) >> (10 - col);
            *dst = lo | hi;
            if (lo != 0) {
                if (col < LAST_KANJI.left) {
                    LAST_KANJI.left = col;
                }
                if (LAST_KANJI.right < col) {
                    LAST_KANJI.right = col;
                }
            }
            if (hi != 0) {
                if (col + 1 < LAST_KANJI.left) {
                    LAST_KANJI.left = col + 1;
                }
                if (LAST_KANJI.right < col + 1) {
                    LAST_KANJI.right = col + 1;
                }
            }
        }
    }
    LAST_KANJI.right += 2;
    dst = KANJI_BUFFERS + KANJI_BUFFER_INDEX * 0x48;
    rect->w = 3;
    rect->h = 11;
    LoadImage((s16 *)rect, (s32)dst);
    KANJI_BUFFER_INDEX++;
    if (KANJI_BUFFER_INDEX == KANJI_BUFFER_COUNT) {
        KANJI_BUFFER_INDEX = 0;
    }
}

/* writes value as full-width digits right-aligned in width characters
   (width < 0: with a sign), returns where the number starts */
u8 *formatSjisNumber(s32 value, s32 width, u8 *dst) {
    s32 i;
    s32 digit;
    s32 quot;
    s32 minus;

    minus = 0;
    if (width < 0) {
        if (value < 0) {
            *dst++ = 0x81;
            *dst++ = 0x7C;
        } else {
            *dst++ = 0x81;
            *dst++ = 0x7B;
        }
    } else if (value < 0) {
        minus = 1;
    }
    width = abs(width);
    value = abs(value);
    for (i = 0; i < width; i++) {
        *dst++ = 0x81;
        *dst++ = 0x40;
    }
    *dst = 0;
    if (minus) {
        width--;
    }
    do {
        if (width-- <= 0) {
            break;
        }
        dst--;
        quot = value / 10;
        digit = value % 10;
        *dst = digit + 0x4F;
        *--dst = (digit + 0x824F) >> 8;
        value = quot;
    } while (value != 0);
    if (minus) {
        *--dst = 0x7C;
        *--dst = 0x81;
    }
    return dst;
}

/* the same with leading zeros */
u8 *formatSjisNumberZeros(s32 value, s32 width, u8 *dst) {
    s32 i;
    s32 digit;
    s32 quot;

    if (value < 0) {
        *dst++ = 0x81;
        *dst++ = 0x7C;
    } else if (width < 0) {
        *dst++ = 0x81;
        *dst++ = 0x7B;
    }
    width = abs(width);
    value = abs(value);
    for (i = 0; i < width; i++) {
        *dst++ = 0x82;
        *dst++ = 0x4F;
    }
    *dst = 0;
    do {
        if (width-- <= 0) {
            break;
        }
        dst--;
        quot = value / 10;
        digit = value % 10;
        *dst = digit + 0x4F;
        *--dst = (digit + 0x824F) >> 8;
        value = quot;
    } while (value != 0);
    return dst;
}

/* the length of tiny text's last line, in characters */
s32 countTinyTextColumns(u8 *text) {
    s32 count;
    u8 ch;

    count = 0;
    while (*text != 0) {
        ch = *text++;
        if (ch == '\n') {
            count = 0;
        }
        if (ch != '\f' && ch != '~' && ch < 0xDE) {
            count++;
        }
    }
    return count;
}

s32 isSpritePoolFull(void) {
    if (SPRITE_POOL_CURSOR == CURRENT_FRAME_BUFFER->spritePool + SPRITE_POOL_SIZE * 0x1C) {
        return -1;
    }
    return 0;
}

/* icon sets: 12x12 in the even ones, 8x8 in the odd ones, low on the system
   page; sets 0 and 1 take palette 10, 2 and 3 palette 11 */
void drawIcon(s32 x, s32 y, s32 iconSet, s32 icon, s32 z) {
    s32 palette;
    s32 small;
    s32 size;

    if (isSpritePoolFull() == 0) {
        palette = iconSet / 2;
        small = iconSet % 2;
        size = 12 - small * 4;
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = icon * size;
        CUR_SPRT->sp.v0 = -0x58 - small * 0x28;
        CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + ((palette + 10) % 4) * 16, SYSTEM_CLUT_Y + 2);
        CUR_SPRT->sp.w = size - 1;
        CUR_SPRT->sp.h = size - 1;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void drawIconColored(s32 x, s32 y, s32 iconSet, s32 icon, u8 *rgb, s32 z) {
    s32 palette;
    s32 small;
    s32 size;

    if (isSpritePoolFull() == 0) {
        palette = iconSet / 2;
        small = iconSet % 2;
        size = 12 - small * 4;
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = icon * size;
        CUR_SPRT->sp.v0 = -0x58 - small * 0x28;
        CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + ((palette + 10) % 4) * 16, SYSTEM_CLUT_Y + 2);
        CUR_SPRT->sp.w = size - 1;
        CUR_SPRT->sp.h = size - 1;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/* the scroll arrows: 0 to 3 are 8x16, from 4 on 16x8 (the partner list
   draws 4 at its top and 6 at its bottom) */
void drawScrollArrow(s32 x, s32 y, s32 arrow, s32 palette, s32 z) {
    s32 u;
    s32 v;
    s32 w;
    s32 h;

    if (isSpritePoolFull() == 0) {
        w = 8;
        if (arrow < 4) {
            h = 0x10;
            u = arrow * 8 + 0xA0;
            v = 0x90;
        } else {
            w = 0x10;
            h = 8;
            u = (arrow - 4) / 2 * 0x10 + 0xC0;
            v = (arrow % 2) * 8 + 0x90;
        }
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = u;
        CUR_SPRT->sp.v0 = v;
        CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/* small 6x10 digits, right-aligned: the last character is drawn at
   x + (length - 1) * 5 */
void drawSmallDigits(s32 x, s32 y, u8 *text, s32 palette, s32 z) {
    s16 clut;
    s32 last;
    s32 i;
    u8 ch;

    clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
    last = strlen(text) - 1;
    x += last * 5;
    for (i = last; i >= 0; i--) {
        if (text[i] == ' ') {
            x -= 5;
            continue;
        }
        ch = text[i];
        if (isSpritePoolFull() != 0) {
            return;
        }
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        x -= 5;
        CUR_SPRT->sp.u0 = (ch & 0xF) * 8 - 0x80;
        CUR_SPRT->sp.v0 = 0;
        CUR_SPRT->sp.clut = clut;
        CUR_SPRT->sp.w = 6;
        CUR_SPRT->sp.h = 10;
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void drawSmallDigitsColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z) {
    s16 clut;
    s32 last;
    s32 i;
    u8 ch;

    clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
    last = strlen(text) - 1;
    x += last * 5;
    for (i = last; i >= 0; i--) {
        if (text[i] == ' ') {
            x -= 5;
            continue;
        }
        ch = text[i];
        if (isSpritePoolFull() != 0) {
            return;
        }
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        x -= 5;
        CUR_SPRT->sp.u0 = (ch & 0xF) * 8 - 0x80;
        CUR_SPRT->sp.v0 = 0;
        CUR_SPRT->sp.clut = clut;
        CUR_SPRT->sp.w = 6;
        CUR_SPRT->sp.h = 10;
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/* converts a Shift-JIS string to tiny text: full-width kana to half-width
   ones (with '~' around hiragana), "a".."e" n to icon codes; returns the
   number of characters */
s32 convertSjisToTinyText(u8 *src, u8 *dst) {
    s32 hiragana;
    s32 count;

    hiragana = 0;
    count = 0;
    while (*src != 0) {
        count++;
        if ((u8)(*src + 0x7F) >= 0x18) {
            /* the icon codes: 'a' becomes icon 49, 'b' 57, 'c' 51, 'd' 65
               and 'e' 61 (in bytes) */
            switch (*src) {
            case 'a':
                *dst++ = 1;
                *dst++ = *src++ - '0';
                break;
            case 'b':
                *dst++ = 1;
                *dst++ = *src++ + 0xD7;
                break;
            case 'c':
                *dst++ = 1;
                *dst++ = *src++ - '0';
                break;
            case 'd':
                *dst++ = 1;
                *dst++ = *src++ + 0xDD;
                break;
            case 'e':
                *dst++ = 1;
                *dst++ = *src++ + 0xD8;
                break;
            default:
                src++;
                break;
            }
            continue;
        }
        if (src[0] == 0x8E && src[1] == 0x74) {
            if (hiragana == 0) {
                hiragana = 1;
                *dst++ = '~';
                count++;
            }
            *dst++ = 0xBC;
        } else if (src[0] == 0x8F && src[1] == 0xA0) {
            *dst++ = 0xBC;
            *dst++ = 0xAE;
            *dst++ = 0xB3;
        } else if (src[0] == 0x81 && src[1] == 0x5B) {
            *dst++ = '-';
        } else if (src[0] == 0x82 && src[1] < 0x9E) {
            if (src[1] < 0x7A) {
                *dst++ = src[1] - 0x1F;
            } else {
                *dst++ = src[1] - 0x20;
            }
        } else if (src[0] == 0x82) {
            if (hiragana == 0) {
                hiragana = 1;
                *dst++ = '~';
                count++;
            }
            if (SJIS_KANA_GLYPHS[src[1] - 0x9E] >= 0) {
                *dst++ = TINY_KANA_PAIRS[(u8)SJIS_KANA_GLYPHS[src[1] - 0x9E] * 2];
                *dst++ = TINY_KANA_PAIRS[(u8)SJIS_KANA_GLYPHS[src[1] - 0x9E] * 2 + 1];
                count++;
            } else {
                *dst++ = SJIS_KANA_GLYPHS[src[1] - 0x9E];
            }
        } else if ((s8)src[1] >= 0) {
            if (hiragana == 1) {
                hiragana = 0;
                *dst++ = '~';
                count++;
            }
            if (SJIS_KANA_GLYPHS[src[1] - 0x3F] >= 0) {
                *dst++ = TINY_KANA_PAIRS[(u8)SJIS_KANA_GLYPHS[src[1] - 0x3F] * 2];
                *dst++ = TINY_KANA_PAIRS[(u8)SJIS_KANA_GLYPHS[src[1] - 0x3F] * 2 + 1];
                count++;
            } else {
                *dst++ = SJIS_KANA_GLYPHS[src[1] - 0x3F];
            }
        } else {
            if (hiragana == 1) {
                hiragana = 0;
                *dst++ = '~';
                count++;
            }
            if (SJIS_KANA_GLYPHS[src[1] - 0x40] >= 0) {
                *dst++ = TINY_KANA_PAIRS[(u8)SJIS_KANA_GLYPHS[src[1] - 0x40] * 2];
                *dst++ = TINY_KANA_PAIRS[(u8)SJIS_KANA_GLYPHS[src[1] - 0x40] * 2 + 1];
                count++;
            } else {
                *dst++ = SJIS_KANA_GLYPHS[src[1] - 0x40];
            }
        }
        src += 2;
    }
    *dst = 0;
    return count;
}

void allocKanjiPages(void) {
    KANJI_PAGES = allocPermanentHeapBlock(32 * sizeof(KanjiPage));
}

void resetKanjiPages(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        KANJI_PAGES[i].capacity = 0;
        KANJI_PAGES[i].count = 0;
        KANJI_PAGES[i].entries = 0;
    }
}

/* gives VRAM page `page` room for `capacity` kanji */
void openKanjiPage(s32 page, s32 capacity) {
    s32 i;

    if (capacity != KANJI_PAGES[page].capacity) {
        freeHeapBlock(KANJI_PAGES[page].entries);
        /* i is first the page's x in VRAM */
        i = (page & 0xF) << 6;
        KANJI_PAGES[page].tpage = GetTPage(0, 0, i, (page & 0x10) << 4);
        KANJI_PAGES[page].entries = allocHeapBlock(capacity * sizeof(KanjiEntry), 0x2A8);
        for (i = 0; i < capacity; i++) {
            KANJI_PAGES[page].entries[i].sjis = 0;
            KANJI_PAGES[page].entries[i].left = 0;
            KANJI_PAGES[page].entries[i].right = 0;
        }
        KANJI_PAGES[page].capacity = capacity;
        KANJI_PAGES[page].count = 0;
    }
}

void closeKanjiPage(s32 page) {
    freeHeapBlock(KANJI_PAGES[page].entries);
    KANJI_PAGES[page].entries = 0;
    KANJI_PAGES[page].capacity = 0;
    KANJI_PAGES[page].count = 0;
}

void clearKanjiPage(s32 page) {
    KANJI_PAGES[page].count = 0;
}

/* 0: found, 1: a full-width space, -1: not cached */
s32 findKanji(u8 *sjis, KanjiGlyph *glyph) {
    u16 code;
    s32 page;
    s32 i;
    KanjiEntry *entry;

    code = (sjis[0] << 8) | sjis[1];
    if (code == 0x8140) {
        return 1;
    }
    for (page = 0; page < 32; page++) {
        if (KANJI_PAGES[page].capacity != 0) {
            entry = KANJI_PAGES[page].entries;
            for (i = 0; i < KANJI_PAGES[page].count; i++, entry++) {
                if (entry->sjis == code) {
                    glyph->x = ((page & 0xF) << 6) + (i % 21) * 3;
                    glyph->y = (page & 0x10) * 16 + (i / 21) * 11;
                    glyph->tpage = KANJI_PAGES[page].tpage;
                    glyph->left = entry->left;
                    glyph->right = entry->right;
                    return 0;
                }
            }
        }
    }
    return -1;
}

/* uploads a kanji to the first page with room; 0: added, 1: a full-width
   space, -1: every page is full (they are all emptied) */
s32 addKanji(u8 *sjis, KanjiGlyph *glyph) {
    Rect16 rect;
    s16 code;
    s32 page;
    KanjiPage *p;

    code = (sjis[0] << 8) | sjis[1];
    if ((u16)code == 0x8140) {
        return 1;
    }
    for (page = 0; page < 32; page++) {
        if (KANJI_PAGES[page].count < KANJI_PAGES[page].capacity) {
            rect.x = ((page & 0xF) << 6) + (KANJI_PAGES[page].count % 21) * 3;
            rect.y = (page & 0x10) * 16 + (KANJI_PAGES[page].count / 21) * 11;
            uploadKanji(sjis, &rect);
            glyph->x = rect.x;
            glyph->y = rect.y;
            p = &KANJI_PAGES[page];
            glyph->tpage = p->tpage;
            glyph->left = LAST_KANJI.left;
            glyph->right = LAST_KANJI.right;
            p->entries[p->count].sjis = code;
            p->entries[p->count].left = LAST_KANJI.left;
            p->entries[p->count].right = LAST_KANJI.right;
            p->count++;
            return 0;
        }
    }
    for (page = 0; page < 32; page++) {
        KANJI_PAGES[page].count = 0;
    }
    return -1;
}

void drawIconText(s32 x, s32 y, s32 palette, s32 proportional, s32 z, s32 text) {
    drawIconTextColored(x, y, palette, proportional, (u8 *)&DEFAULT_TEXT_RGB, z, (u8 *)text);
}

/* text with icons and kanji: 'a', 'b', 'd' and 'e' n draw an icon, 'c' n
   changes the palette, 's' n turns proportional spacing on or off, 'w' n
   and 'h' n set the spacing and the line spacing, 'z' toggles the width
   of a space; returns the width of the widest line */
s32 drawIconTextColored(s32 x, s32 y, s32 palette, s32 proportional, u8 *rgb, s32 z, u8 *text) {
    KanjiGlyph glyph;
    s32 startX;
    s32 lineSpacing;
    s32 spaceWidth;
    s32 spacing;
    s32 found;
    u8 *sjis;

    startX = x;
    lineSpacing = 0;
    spaceWidth = 6;
    TEXT_WIDTH = 0;
    TEXT_HEIGHT = 0;
    glyph.left = 0;
    glyph.right = 0;
    spacing = 0;
    while (*text != 0) {
        if ((u8)(*text + 0x7F) >= 0x18) {
            switch (*text) {
            case 'a':
                text++;
                drawIconColored(x, y, 0, *text++ - '0', rgb, z);
                x += 12;
                break;
            case 'b':
                text++;
                drawIconColored(x, y, 0, *text++ - ')', rgb, z);
                x += 12;
                break;
            case 'c':
                text++;
                palette = *text - '0';
                text++;
                break;
            case 'd':
                text++;
                drawIconColored(x, y, 0, *text++ - '"', rgb, z);
                x += 12;
                break;
            case 'e':
                text++;
                drawIconColored(x, y, 0, *text++ - '(', rgb, z);
                x += 12;
                break;
            case 's':
                text++;
                proportional = *text - '0';
                text++;
                break;
            case 'w':
                text++;
                if (*text == '-') {
                    text++;
                    spacing = '0' - *text;
                } else {
                    spacing = *text - '0';
                }
                text++;
                break;
            case 'h':
                text++;
                if (*text == '-') {
                    text++;
                    lineSpacing = '0' - *text;
                } else {
                    lineSpacing = *text - '0';
                }
                text++;
                break;
            case 'z':
                text++;
                if (spaceWidth == 6) {
                    spaceWidth = 12;
                } else {
                    spaceWidth = 6;
                }
                break;
            case ' ':
                text++;
                x += spaceWidth + spacing;
                break;
            case '\n':
                text++;
                if (TEXT_WIDTH < x - startX) {
                    TEXT_WIDTH = x - startX;
                }
                x = startX;
                y += 13;
                y += lineSpacing;
                break;
            default:
                if (*text >= '0' && *text <= '9') {
                    sjis = &SJIS_DIGITS[(*text - '0') * 2];
                    found = findKanji(sjis, &glyph);
                    if (found == -1) {
                        found = addKanji(sjis, &glyph);
                    }
                    if (proportional == 0) {
                        glyph.left = 0;
                        glyph.right = 12;
                    }
                    if (found == 0) {
                        if (isSpritePoolFull() != 0) {
                            return x - startX;
                        }
                        CUR_SPRT->sp.x0 = x - glyph.left;
                        CUR_SPRT->sp.y0 = y;
                        CUR_SPRT->sp.u0 = (glyph.x & 0x3F) * 4;
                        CUR_SPRT->sp.v0 = glyph.y;
                        CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
                        CUR_SPRT->sp.w = glyph.right - 1;
                        CUR_SPRT->sp.h = 11;
                        setSemiTrans(&CUR_SPRT->sp, 1);
                        CUR_SPRT->sp.r0 = rgb[0];
                        CUR_SPRT->sp.g0 = rgb[1];
                        CUR_SPRT->sp.b0 = rgb[2];
                        setDrawMode(&CUR_SPRT->dm, 0, 0, glyph.tpage);
                        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
                        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
                        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
                        x += glyph.right - glyph.left + spacing;
                    } else if (found == 1) {
                        x += glyph.right + spacing;
                    }
                }
                text++;
                break;
            }
        } else {
            found = findKanji(text, &glyph);
            if (found == -1) {
                found = addKanji(text, &glyph);
            }
            if (proportional == 0) {
                glyph.left = 0;
                glyph.right = 12;
            }
            if (found == 0) {
                if (isSpritePoolFull() != 0) {
                    return x - startX;
                }
                CUR_SPRT->sp.x0 = x - glyph.left;
                CUR_SPRT->sp.y0 = y;
                CUR_SPRT->sp.u0 = (glyph.x & 0x3F) * 4;
                CUR_SPRT->sp.v0 = glyph.y;
                CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + (palette % 4) * 16, SYSTEM_CLUT_Y + palette / 4);
                CUR_SPRT->sp.w = glyph.right - 1;
                CUR_SPRT->sp.h = 11;
                setSemiTrans(&CUR_SPRT->sp, 1);
                CUR_SPRT->sp.r0 = rgb[0];
                CUR_SPRT->sp.g0 = rgb[1];
                CUR_SPRT->sp.b0 = rgb[2];
                setDrawMode(&CUR_SPRT->dm, 0, 0, glyph.tpage);
                addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
                addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
                SPRITE_POOL_CURSOR += sizeof(SprtPacket);
                x += glyph.right - glyph.left + spacing;
            } else if (found == 1) {
                x += glyph.right + spacing;
            }
            text += 2;
        }
    }
    if (TEXT_WIDTH < x - startX) {
        TEXT_WIDTH = x - startX;
    }
    TEXT_HEIGHT = y + 12;
    return TEXT_WIDTH;
}

/* drawIconTextColored's layout without drawing: sets TEXT_WIDTH and
   TEXT_HEIGHT and returns the width */
s32 measureText(s32 proportional, u8 *text) {
    KanjiGlyph glyph;
    s32 x;
    s32 y;
    s32 startX; /* 0: drawIconTextColored's layout from x = 0 */
    s32 lineSpacing;
    s32 spaceWidth;
    s32 spacing;
    s32 found;
    u8 *sjis;

    x = 0;
    y = 0;
    spacing = 0;
    startX = 0;
    lineSpacing = 0;
    spaceWidth = 6;
    TEXT_WIDTH = 0;
    TEXT_HEIGHT = 0;
    glyph.left = 0;
    glyph.right = 0;
    while (*text != 0) {
        if ((u8)(*text + 0x7F) >= 0x18) {
            switch (*text) {
            case 'a':
            case 'b':
            case 'd':
            case 'e':
                text += 2;
                x += 12;
                break;
            case 'c':
                text += 2;
                break;
            case 's':
                text++;
                proportional = *text++ - '0';
                break;
            case 'w':
                text++;
                if (*text == '-') {
                    text++;
                    spacing = '0' - *text++;
                } else {
                    spacing = *text++ - '0';
                }
                break;
            case 'h':
                text++;
                if (*text == '-') {
                    text++;
                    lineSpacing = '0' - *text++;
                } else {
                    lineSpacing = *text++ - '0';
                }
                break;
            case 'z':
                text++;
                if (spaceWidth == 6) {
                    spaceWidth = 12;
                } else {
                    spaceWidth = 6;
                }
                break;
            case ' ':
                text++;
                x += spaceWidth + spacing;
                break;
            case '\n':
                text++;
                /* no braces: the match depends on it. They would add a note to
                   the loop that makes GCC's loop.c hoist TEXT_WIDTH's
                   address, which jp's doesn't */
                if (TEXT_WIDTH < x - startX)
                    TEXT_WIDTH = x - startX;
                x = startX;
                y += 13;
                y += lineSpacing;
                break;
            default:
                if (*text >= '0' && *text <= '9') {
                    sjis = &SJIS_DIGITS[(*text - '0') * 2];
                    found = findKanji(sjis, &glyph);
                    if (found == -1) {
                        found = addKanji(sjis, &glyph);
                    }
                    if (proportional == 0) {
                        glyph.left = 0;
                        glyph.right = 12;
                    }
                    if (found == 0) {
                        x += glyph.right - glyph.left + spacing;
                    } else if (found == 1) {
                        x += glyph.right + spacing;
                    }
                }
                text++;
                break;
            }
        } else {
            found = findKanji(text, &glyph);
            if (found == -1) {
                found = addKanji(text, &glyph);
            }
            if (proportional == 0) {
                glyph.left = 0;
                glyph.right = 12;
            }
            if (found == 0) {
                x += glyph.right - glyph.left + spacing;
            } else if (found == 1) {
                x += glyph.right + spacing;
            }
            text += 2;
        }
    }
    if (TEXT_WIDTH < x - startX) {
        TEXT_WIDTH = x - startX;
    }
    TEXT_HEIGHT = y + 12;
    return TEXT_WIDTH;
}
