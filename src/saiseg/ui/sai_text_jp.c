#include "common.h"
#include "game.h"
#include "dcb/text.h"
#include "dcb/task.h"
#include "dcb/saiseg.h"

/* jp's message window (sai_text.c is us's and eu's): up to four lines,
   rendered into VRAM as they are added and typed out a glyph at a time */

typedef struct {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 vramY; /* where the line's glyphs are rendered */
    /* 0x08 */ u32 width;
    /* 0x0C */ u32 shown;
    /* 0x10 */ u8 text[0x30];
    /* 0x40 */ s8 palettes[0x18]; /* one for each two bytes of text */
} MsgLine;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 typing;
    /* 0x02 */ u8 unk2[2];
    /* 0x04 */ MsgLine lines[4];
} MsgBox;

/* a line typed as text instead */
typedef struct {
    /* 0x00 */ s32 shown;
    /* 0x04 */ s32 length;
    /* 0x08 */ s8 active;
    /* 0x09 */ char text[0x43];
} TypedLine;

extern MsgBox SAI_MESSAGE_BOX;
extern TypedLine SAI_TYPED_LINES[4];

void func_8006689C(char *, char *, s32);
char *formatSjisNumber(s32, s32, char *);
u32 uploadKanjiString(u8 *, Rect16 *);

MsgLine *SAI_allocTextLine(MsgBox *box);
s32 SAI_isLetter(s8 c);

/* the color the message window's glyphs are drawn in */
const Bytes4 SAI_TEXT_RGB = { { 0xA4, 0xA4, 0xA4 } };

void SAI_drawMessageWindow(JpWindow *win) {
    Bytes4 rgb = SAI_TEXT_RGB;
    Rect16 uv;
    char buf[0x48];
    MsgLine *line;
    s32 y;
    u32 i;
    u32 j;
    s32 n;
    u32 count;
    s32 x;

    line = SAI_MESSAGE_BOX.lines;
    SAI_MESSAGE_BOX.typing = 0;
    y = 4;
    if (SAI_STATE->flags & 0x10) {
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[win->z], DB(FRAME_BUFFER_INDEX).primSlots[0] + 0x60);
    }
    for (i = 0; i < 4; i++) {
        if (SAI_TYPED_LINES[i].active != 0 && SAI_MESSAGE_BOX.typing == 0) {
            SAI_TYPED_LINES[i].shown += 400;
            n = SAI_TYPED_LINES[i].shown / 100 * 2;
            if (n >= SAI_TYPED_LINES[i].length) {
                n = SAI_TYPED_LINES[i].length;
            } else {
                SAI_MESSAGE_BOX.typing = 1;
            }
            func_8006689C(buf, SAI_TYPED_LINES[i].text, n);
            buf[n] = 0;
            drawIconText(0x26, y + 0x99, 7, 1, win->z, (s32)buf);
        }
        y += 0xE;
    }
    for (i = 0; i < 4; i++, line++) {
        if (line->active == 1) {
            line->shown += 0x28;
            count = line->shown / line->width;
            if (count >= line->width) {
                count = line->width;
            } else {
                SAI_MESSAGE_BOX.typing = 1;
            }
            for (j = 0; j < count; j++) {
                x = j * 12;
                uv.x = x;
                uv.y = line->vramY;
                uv.w = 0xC;
                uv.h = 0xB;
                drawPageSpriteColored(x + 0x26, line->vramY + 0x19, &uv, rgb.b, GetTPage(0, 0, 0x3C0, 0), line->palettes[j], win->z);
            }
        }
        if (SAI_MESSAGE_BOX.typing == 1) {
            break;
        }
    }
}

/* c6(%s)c7(Ｂｉｔを手に入れた！) */
const char SAI_FMT_GOT_BITS[] = "c6(%s)c7(\x82" "a\x82\x89\x82\x94\x82\xF0\x8E\xE8\x82\xC9\x93\xFC\x82\xEA\x82\xBD\x81I)";

/* it reads SESSION_DATA's state again for the Bits, where our C reuses the
   pointer it read the event with */
INCLUDE_ASM("saiseg/nonmatchings/ui/sai_text_jp", SAI_addTextLine);

MsgLine *SAI_allocTextLine(MsgBox *box) {
    MsgLine *line;
    s8 i;

    line = box->lines;
    for (i = 0; i < 4; i++, line++) {
        if (line->active == 0) {
            line->active = 1;
            line->vramY = i * 14 + 0x84;
            line->shown = 0;
            return line;
        }
    }
    return NULL;
}

s32 SAI_isLetter(s8 c) {
    return c >= 'A' && c <= 'z';
}

void SAI_clearTextLines(void) {
    Rect16 rect = { 0x3C0, 0x80, 0x40, 0x80 };
    s8 i;

    for (i = 0; i < 4; i++) {
        SAI_MESSAGE_BOX.lines[i].active = 0;
    }
    ClearImage(&rect, 0, 0, 0);
}
