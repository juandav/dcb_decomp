#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/text.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"

s32 DEFAULT_TEXT_RGB = 0x808080;
u8 FONT_GLYPH_METRICS[96] = {
    0x4, 0x13, 0x4, 0x6, 0x6, 0x6, 0x6, 0x14,
    0x5, 0x15, 0x6, 0x6, 0x4, 0x6, 0x14, 0x6,
    0x6, 0x14, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x23, 0x14, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x14, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x23, 0x6, 0x4, 0x14, 0x6,
    0x4, 0x6, 0x5, 0x5, 0x5, 0x5, 0x14, 0x5,
    0x15, 0x22, 0x5, 0x5, 0x14, 0x6, 0x5, 0x5,
    0x15, 0x5, 0x5, 0x5, 0x5, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x5, 0x6, 0x6, 0x6, 0x6, 0x6,
};

void initSystemSprites(s32 vramX, s32 vramY, s32 poolSize) {
    Rect16 r;
    u32 *tim;
    SprtPacket *pool;
    s32 i;
    TIM_IMAGE *image;

    SYSTEM_TEX_X = vramX;
    SYSTEM_TEX_Y = vramY;
    SYSTEM_CLUT_X = vramX + 0x20;
    SYSTEM_CLUT_Y = vramY + 0xF8;
    SPRITE_POOL_SIZE = poolSize;
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\SYSTEM.TIM", getCurrentTaskId());
    tim = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTim(tim, SYSTEM_TEX_X, SYSTEM_TEX_Y, -2, -2);
    image = &LOADED_TIM;
    r.x = SYSTEM_CLUT_X;
    r.y = SYSTEM_CLUT_Y;
    r.w = 0x20;
    r.h = 8;
    LoadImage((s16 *)&r, (s32)image->caddr);
    DrawSync(0);
    freeHeapBlock(tim);
    pool = allocPermanentHeapBlock(SPRITE_POOL_SIZE * sizeof(SprtPacket) * 2);
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
        CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
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
        CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
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
        CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
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

void drawTinyTextColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 ch;

    left = x;
    clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
    while (*text != 0) {
        switch (*text) {
        case '\f':
            text++;
            palette = *text++;
            clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
            break;
        case '*':
            text++;
            switch (*text) {
            case 'a':
                text++;
                palette = *text++;
                drawIconColored(x, y, 3, palette - '0', rgb, z);
                break;
            case 'b':
                text++;
                palette = *text++;
                drawIconColored(x, y, 3, palette - ')', rgb, z);
                break;
            case 'c':
                text++;
                palette = *text - '0';
                text++;
                clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
                break;
            case 'd':
                text++;
                palette = *text++;
                drawIconColored(x, y, 3, palette - 0x1C, rgb, z);
                break;
            case 'e':
                text++;
                palette = *text;
                if (*text < '4') {
                    palette -= '#';
                } else if (*text == 'a') {
                    palette = 0x11;
                } else {
                    palette = *text - '"';
                }
                drawIconColored(x, y, 3, palette, rgb, z);
                text++;
                break;
            }
        case '\n':
            x = left;
            y += 6;
            text++;
            break;
        case ' ':
            x += 4;
            text++;
            break;
        default:
            ch = *text++;
            if (ch >= 'a') {
                ch -= 0x20;
            }
            if (isSpritePoolFull() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 4;
            CUR_SPRT->sp.u0 = (ch & 0xF) * 4;
            CUR_SPRT->sp.v0 = ((ch - 0x20) >> 4) * 5 - 0x16;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 4;
            CUR_SPRT->sp.h = 5;
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

void drawSmallText(s32 x, s32 y, s32 text, s32 palette, s32 z) {
    drawSmallTextColored(x, y, (u8 *)text, palette, (u8 *)&DEFAULT_TEXT_RGB, z);
}

void drawSmallTextColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 ch;
    s32 icon;

    left = x;
    clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
    while (*text != 0) {
        switch (*text) {
        case 1:
            text++;
            drawIconColored(x, y, 3, *text++ - 1, rgb, z);
            x += 6;
            break;
        case '*':
            text++;
            switch (*text) {
            case 'a':
                text++;
                drawIconColored(x, y, 3, *text++ - '0', rgb, z);
                x += 6;
                break;
            case 'b':
                text++;
                drawIconColored(x, y, 3, *text++ - 0x29, rgb, z);
                x += 6;
                break;
            case 'c':
                text++;
                palette = *text++ - '0';
                clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
                break;
            case 'd':
                text++;
                drawIconColored(x, y, 3, *text++ - 0x1C, rgb, z);
                x += 6;
                break;
            case 'e':
                text++;
                if (*text < 0x34) {
                    icon = *text - 0x23;
                } else if (*text == 'a') {
                    icon = 0x11;
                } else {
                    icon = *text - 0x22;
                }
                drawIconColored(x, y, 3, icon, rgb, z);
                x += 6;
                text++;
                break;
            }
            break;
        case '\f':
            text++;
            palette = *text++;
            clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
            break;
        case '\n':
            x = left;
            y += 6;
            text++;
            break;
        case ' ':
            x += 5;
            text++;
            break;
        default:
            ch = *text++;
            if (ch >= 'a') {
                ch -= 0x20;
            }
            if (isSpritePoolFull() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 5;
            CUR_SPRT->sp.u0 = (ch & 0xF) * 4 + 0x40;
            CUR_SPRT->sp.v0 = ((ch - 0x20) >> 4) * 5 - 0x16;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 4;
            CUR_SPRT->sp.h = 5;
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

void drawVerticalText(s32 x, s32 y, s32 text, s32 palette, s32 z) {
    drawVerticalTextColored(x, y, (u8 *)text, palette, (u8 *)&DEFAULT_TEXT_RGB, z);
}

void drawVerticalTextColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z) {
    s16 clut;
    s32 top;
    s32 ch;

    clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
    top = y - 5;
    y = top;
    while (*text != 0) {
        switch (*text) {
        case '\f':
            text++;
            palette = *text++;
            clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
            break;
        case '\n':
            x += 6;
            y = top;
            text++;
            break;
        case ' ':
            y -= 5;
            text++;
            break;
        default:
            ch = *text++;
            if (isSpritePoolFull() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            y -= 5;
            CUR_SPRT->sp.u0 = (ch & 0xF) * 6;
            CUR_SPRT->sp.v0 = ((ch - 0x20) >> 4) * 4 - 0x26;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 5;
            CUR_SPRT->sp.h = 4;
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

void drawMediumText(s32 x, s32 y, s32 text, s32 palette, s32 z) {
    drawMediumTextColored(x, y, (u8 *)text, palette, (u8 *)&DEFAULT_TEXT_RGB, z);
}

void drawMediumTextColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 ch;

    left = x;
    clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
    while (*text != 0) {
        switch (*text) {
        case '*':
            text++;
            switch (*text) {
            case 'a':
                text++;
                drawIconColored(x, y, 3, *text++ - '0', rgb, z);
                x += 6;
                break;
            case 'b':
                text++;
                drawIconColored(x, y, 3, *text++ - ')', rgb, z);
                x += 6;
                break;
            case 'c':
                text++;
                palette = *text++ - '0';
                clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
                break;
            case 'd':
                text++;
                drawIconColored(x, y, 3, *text++ - 0x1C, rgb, z);
                x += 6;
                break;
            case 'e':
                text++;
                palette = *text;
                if (*text < '4') {
                    palette -= '#';
                } else if (*text == 'a') {
                    palette = 0x11;
                } else {
                    palette = *text - '"';
                }
                drawIconColored(x, y, 3, palette, rgb, z);
                x += 6;
                text++;
                break;
            }
            break;
        case '\f':
            text++;
            palette = *text++;
            clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
            break;
        case '\n':
            x = left;
            y += 7;
            text++;
            break;
        case ' ':
            x += 6;
            text++;
            break;
        default:
            ch = *text++;
            if (isSpritePoolFull() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 6;
            CUR_SPRT->sp.u0 = (ch & 0xF) * 6;
            CUR_SPRT->sp.v0 = (((ch - 0x20) & 0xF0) >> 4) * 6 - 0x4C;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 6;
            CUR_SPRT->sp.h = 6;
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

void drawLargeText(s32 x, s32 y, s32 text, s32 palette, s32 z) {
    drawLargeTextColored(x, y, (u8 *)text, palette, (u8 *)&DEFAULT_TEXT_RGB, z);
}

void drawLargeTextColored(s32 x, s32 y, u8 *text, s32 palette, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 ch;

    left = x;
    clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
    while (*text != 0) {
        switch (*text) {
        case 1:
            text++;
            drawIconColored(x, y, 1, *text++ - 1, rgb, z);
            x += 8;
            break;
        case '\f':
            text++;
            palette = *text++;
            clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
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
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 8;
            CUR_SPRT->sp.u0 = (ch & 0xF) * 8;
            CUR_SPRT->sp.v0 = ((ch - 0x20) >> 4) * 7;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 7;
            CUR_SPRT->sp.h = 7;
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

s32 drawTextColored(s32 x, s32 y, u8 *text, u8 *rgb, s32 palette, s32 z) {
    s32 charSpacing;
    s32 startX;
    s32 startY;
    s32 lineSpacing;
    s32 proportional;
    s16 clut;
    s32 ch;
    s32 icon;

    charSpacing = 0;
    startX = x;
    startY = y;
    lineSpacing = 0;
    proportional = 1;
    clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
    TEXT_WIDTH = 0;
    TEXT_HEIGHT = 0;
    while (*text != 0) {
        if (*text == '*') {
            text++;
            switch (*text) {
            case 'a':
                text++;
                drawIconColored(x, y + 1, 0, *text++ - '0', rgb, z);
                x += 12 + charSpacing;
                if (TEXT_WIDTH < x) {
                    TEXT_WIDTH = x;
                }
                continue;
            case 'b':
                text++;
                drawIconColored(x, y + 1, 0, *text++ - ')', rgb, z);
                x += 12 + charSpacing;
                if (TEXT_WIDTH < x) {
                    TEXT_WIDTH = x;
                }
                continue;
            case 'c':
                text++;
                palette = *text - '0';
                text++;
                clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
                continue;
            case 'd':
                text++;
                drawIconColored(x, y + 1, 0, *text++ - 0x1C, rgb, z);
                x += 12 + charSpacing;
                if (TEXT_WIDTH < x) {
                    TEXT_WIDTH = x;
                }
                continue;
            case 'e':
                text++;
                icon = *text;
                if (*text < '4') {
                    icon -= '#';
                } else if (*text == 'a') {
                    icon = 0x11;
                } else {
                    icon = *text - '"';
                }
                drawIconColored(x, y + 1, 0, icon, rgb, z);
                x += 12 + charSpacing;
                if (TEXT_WIDTH < x) {
                    TEXT_WIDTH = x;
                }
                text++;
                continue;
            case 'g':
                text++;
                drawIconColored(x, y, 2, *text++ - '0', rgb, z);
                x += 25 + charSpacing;
                if (TEXT_WIDTH < x) {
                    TEXT_WIDTH = x;
                }
                continue;
            case 'h':
                text++;
                if (*text == '-') {
                    text++;
                    lineSpacing = '0' - *text++;
                } else {
                    lineSpacing = *text++ - '0';
                }
                continue;
            case 's':
                text++;
                proportional = *text++ - '0';
                continue;
            case 'w':
                text++;
                if (*text == '-') {
                    text++;
                    charSpacing = '0' - *text++;
                } else {
                    charSpacing = *text++ - '0';
                }
                continue;
            }
        }
        switch (*text) {
        case '\\':
            text++;
            if (*text != 'n') {
                text++;
                break;
            }
            text++;
            x = startX;
            y += 13 + lineSpacing;
            if (TEXT_WIDTH < x) {
                TEXT_WIDTH = x;
            }
            if (TEXT_HEIGHT < y) {
                TEXT_HEIGHT = y;
            }
            break;
        case '\n':
            text++;
            x = startX;
            y += 13 + lineSpacing;
            if (TEXT_WIDTH < x) {
                TEXT_WIDTH = x;
            }
            if (TEXT_HEIGHT < y) {
                TEXT_HEIGHT = y;
            }
            break;
        default:
            ch = *text++ - 0x20;
            if (isSpritePoolFull() != 0) {
                return; /* no value: the caller never reads it */
            }
            x += charSpacing;
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y + 1;
            CUR_SPRT->sp.u0 = (ch % 16) * 6;
            if (proportional != 0) {
                CUR_SPRT->sp.u0 += FONT_GLYPH_METRICS[ch] >> 4;
            }
            CUR_SPRT->sp.v0 = (ch / 16) * 12 + 0x30;
            CUR_SPRT->sp.clut = clut;
            if (proportional != 0) {
                CUR_SPRT->sp.w = (u8)(FONT_GLYPH_METRICS[ch] & 0xF);
            } else {
                CUR_SPRT->sp.w = 6;
            }
            CUR_SPRT->sp.h = 12;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            x += CUR_SPRT->sp.w;
            if (TEXT_WIDTH < x) {
                TEXT_WIDTH = x;
            }
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            break;
        }
    }
    TEXT_WIDTH -= startX;
    TEXT_HEIGHT = TEXT_HEIGHT - startY + 12;
    return TEXT_WIDTH;
}

s32 measureText(u8 *text) {
    s32 charSpacing;
    s32 lineSpacing;
    s32 proportional;
    const s32 startX = 0;
    const s32 startY = 0;
    s32 x;
    s32 y;
    s32 ch;

    charSpacing = 0;
    lineSpacing = 0;
    proportional = 1;
    TEXT_WIDTH = 0;
    TEXT_HEIGHT = 0;
    x = startX;
    y = startY;
    while (*text != 0) {
        if (*text == '*') {
            text++;
            switch (*text) {
            case 'a':
            case 'b':
            case 'd':
            case 'e':
                text += 2;
                x += 12 + charSpacing;
                if (TEXT_WIDTH < x) {
                    TEXT_WIDTH = x;
                }
                continue;
            case 'g':
                text += 2;
                x += 25 + charSpacing;
                if (TEXT_WIDTH < x) {
                    TEXT_WIDTH = x;
                }
                continue;
            case 'c':
                text++;
                continue;
            case 'h':
                text++;
                if (*text == '-') {
                    text++;
                    lineSpacing = '0' - *text++;
                } else {
                    lineSpacing = *text++ - '0';
                }
                continue;
            case 's':
                text++;
                proportional = *text++ - '0';
                continue;
            case 'w':
                text++;
                if (*text == '-') {
                    text++;
                    charSpacing = '0' - *text++;
                } else {
                    charSpacing = *text++ - '0';
                }
                continue;
            }
        }
        switch (*text) {
        case '\\':
            text++;
            if (*text != 'n') {
                text++;
                break;
            }
            text++;
            x = startX;
            y += 13 + lineSpacing;
            if (TEXT_WIDTH < x) {
                TEXT_WIDTH = x;
            }
            if (TEXT_HEIGHT < y) {
                TEXT_HEIGHT = y;
            }
            break;
        case '\n':
            text++;
            x = startX;
            y += 13 + lineSpacing;
            if (TEXT_WIDTH < x) {
                TEXT_WIDTH = x;
            }
            if (TEXT_HEIGHT < y) {
                TEXT_HEIGHT = y;
            }
            break;
        default:
            ch = *text++ - 0x20;
            x += charSpacing;
            if (proportional != 0) {
                x += FONT_GLYPH_METRICS[ch] & 0xF;
            } else {
                x += 6;
            }
            if (TEXT_WIDTH < x) {
                TEXT_WIDTH = x;
            }
            break;
        }
    }
    TEXT_WIDTH -= startX;
    TEXT_HEIGHT = TEXT_HEIGHT - startY + 12;
    return TEXT_WIDTH;
}

void drawBigDigits(s32 x, s32 y, u8 *text, u8 *rgb, s32 palette, s32 z) {
    s16 clut;
    s32 glyph;

    clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
    while (*text != 0) {
        switch (*text) {
        case ' ':
            x += 0x10;
            text++;
            break;
        case 'c':
            text++;
            palette = *text++ & 0xF;
            clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
            break;
        default:
            glyph = *text++;
            switch (glyph) {
            case '+':
                glyph = 10;
                break;
            case '-':
                glyph = 11;
                break;
            case '=':
                glyph = 12;
                break;
            default:
                glyph -= '0';
                break;
            }
            if (isSpritePoolFull() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 0xC;
            CUR_SPRT->sp.u0 = (glyph % 8) * 16 - 0x80;
            CUR_SPRT->sp.v0 = (glyph / 8) * 0x15;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 0x10;
            CUR_SPRT->sp.h = 0x15;
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

s32 isSpritePoolFull(void) {
    if (SPRITE_POOL_CURSOR == CURRENT_FRAME_BUFFER->spritePool + SPRITE_POOL_SIZE * 0x1C) {
        return -1;
    }
    return 0;
}

void drawIcon(s32 x, s32 y, s32 iconSet, s32 icon, s32 z) {
    drawIconColored(x, y, iconSet, icon, (u8 *)&DEFAULT_TEXT_RGB, z);
}

void drawIconColored(s32 x, s32 y, s32 iconSet, s32 icon, u8 *rgb, s32 z) {
    s16 w;
    s16 h;

    w = 0xB;
    h = 0xB;
    if (isSpritePoolFull() == 0) {
        switch (iconSet) {
        case 0:
            CUR_SPRT->sp.u0 = (icon % 14) * 12;
            CUR_SPRT->sp.v0 = (icon / 14) * 11 + 0x7F;
            if (icon >= 0x15 && icon < 0x18) {
                CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + 16, SYSTEM_CLUT_Y + 5);
            } else if (icon == 0x1B) {
                CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X, SYSTEM_CLUT_Y + 3);
            } else if (icon >= 0x1C && icon < 0x25) {
                CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X, SYSTEM_CLUT_Y + 7);
            } else {
                CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X, SYSTEM_CLUT_Y + 5);
            }
            w = 12;
            h = 11;
            break;
        case 1:
            CUR_SPRT->sp.u0 = icon * 8;
            CUR_SPRT->sp.v0 = 0x78;
            CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X, SYSTEM_CLUT_Y + 5);
            w = 7;
            h = 7;
            break;
        case 2:
            if (icon < 10) {
                CUR_SPRT->sp.u0 = 0xD8;
                CUR_SPRT->sp.v0 = icon * 12 + 0x18;
            } else {
                CUR_SPRT->sp.u0 = 0xC0;
                CUR_SPRT->sp.v0 = (icon - 10) * 12 + 0x60;
            }
            CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X, SYSTEM_CLUT_Y + 5);
            w = 0x18;
            h = 0xC;
            break;
        case 3:
            CUR_SPRT->sp.u0 = (icon % 14) * 6 + 0x30;
            CUR_SPRT->sp.v0 = (icon / 14) * 6 - 0x60;
            CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X, SYSTEM_CLUT_Y + 5);
            w = 5;
            h = 5;
            break;
        }
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
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

/*
 * Like drawText, but the escape codes (a0 icon, b1 button, c7 palette, h/w
 * spacing...) need no '*' in front: SUBSEG draws the attack button icons
 * with "b0" .. "b2". Digits and two-byte characters all draw the same
 * 12x12 tile of the system texture.
 */
void drawIconText(s32 x, s32 y, s32 palette, s32 unused, s32 z, s32 text) {
    drawIconTextColored(x, y, palette, unused, (u8 *)&DEFAULT_TEXT_RGB, z, (u8 *)text);
}

s32 drawIconTextColored(s32 x, s32 y, s32 palette, s32 unused, u8 *rgb, s32 z, u8 *text) {
    s32 startX;
    s32 spacing;
    s32 lineSpacing;
    s32 spaceWidth;
    s32 icon;

    startX = x;
    spacing = 0;
    lineSpacing = 0;
    spaceWidth = 6;
    if (isSpritePoolFull() != 0) {
        return;
    }
    while (*text != 0) {
        if ((u8)(*text + 0x7F) >= 0x18) {
            switch (*text) {
            case '\\':
                text++;
                if (*text == 'n') {
                    text++;
                    x = startX;
                    y += 13 + lineSpacing;
                } else {
                    text++;
                }
                break;
            case 'a':
                text++;
                icon = *text++;
                drawIconColored(x, y + 1, 0, icon - '0', rgb, z);
                x += 12;
                x += spacing;
                break;
            case 'b':
                text++;
                icon = *text++;
                drawIconColored(x, y + 1, 0, icon - ')', rgb, z);
                x += 12;
                x += spacing;
                break;
            case 'c':
                text++;
                palette = *text - '0';
                text++;
                break;
            case 'd':
                text++;
                icon = *text++;
                drawIconColored(x, y + 1, 0, icon - 0x1C, rgb, z);
                x += 12;
                x += spacing;
                break;
            case 'e':
                text++;
                if (*text < '4') {
                    icon = *text - '#';
                } else if (*text == 'a') {
                    icon = 0x11;
                } else {
                    icon = *text - '"';
                }
                drawIconColored(x, y + 1, 0, icon, rgb, z);
                x += 12;
                x += spacing;
                text++;
                break;
            case 'g':
                text++;
                icon = *text++;
                drawIconColored(x, y, 2, icon - '0', rgb, z);
                x += 25 + spacing;
                break;
            case 'h':
                text++;
                if (*text == '-') {
                    text++;
                    lineSpacing = '0' - *text;
                    text++;
                } else {
                    lineSpacing = *text - '0';
                    text++;
                }
                break;
            case 'w':
                text++;
                if (*text == '-') {
                    text++;
                    spacing = '0' - *text;
                    text++;
                } else {
                    spacing = *text - '0';
                    text++;
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
                x = startX;
                y += 13;
                y += lineSpacing;
                break;
            case 's':
                text += 2;
                break;
            default:
                if ((u32)(*text - '0') < 10) {
                    if (isSpritePoolFull() != 0) {
                        return x - startX;
                    }
                    CUR_SPRT->sp.x0 = x;
                    CUR_SPRT->sp.y0 = y;
                    CUR_SPRT->sp.u0 = 0x6C;
                    CUR_SPRT->sp.v0 = 0x30;
                    CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
                    CUR_SPRT->sp.w = 12;
                    CUR_SPRT->sp.h = 12;
                    setSemiTrans(&CUR_SPRT->sp, 1);
                    CUR_SPRT->sp.r0 = 0x80;
                    CUR_SPRT->sp.g0 = 0x80;
                    CUR_SPRT->sp.b0 = 0x80;
                    setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
                    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
                    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
                    SPRITE_POOL_CURSOR += sizeof(SprtPacket);
                    x += 6 + spacing;
                }
                text++;
                break;
            }
        } else {
            if (isSpritePoolFull() != 0) {
                return x - startX;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0x6C;
            CUR_SPRT->sp.v0 = 0x30;
            CUR_SPRT->sp.clut = getClut(SYSTEM_CLUT_X + (palette % 2) * 16, SYSTEM_CLUT_Y + palette / 2);
            CUR_SPRT->sp.w = 12;
            CUR_SPRT->sp.h = 12;
            setSemiTrans(&CUR_SPRT->sp, 1);
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, SYSTEM_TEX_X, SYSTEM_TEX_Y));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            x += 12 + spacing;
            text += 2;
        }
    }
    TEXT_WIDTH = x - startX;
    TEXT_HEIGHT = y + 12;
    return x - startX;
}
