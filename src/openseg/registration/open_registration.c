#include "common.h"
#include "game.h"
#include "dcb/open_registration.h"
#include "dcb/heap.h"
#include "dcb/text.h"
#include "dcb/scene3d.h"
#include "dcb/model_anim.h"
#include "dcb/frame_callback.h"
#include "dcb/model_load.h"
#include "dcb/anim_control.h"
#include "dcb/window.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/scroll_bg.h"
#include "dcb/prim_util.h"
#include "dcb/sound_play.h"
#include "dcb/fade.h"
#include "dcb/sound.h"
#include "dcb/openseg.h"
#include "dcb/open_name_entry.h"
#include "dcb/open_starter.h"
#include "dcb/open_memcard.h"

typedef struct {
    /* 0x00 */ s16 texX;
    /* 0x02 */ s16 texY;
    /* 0x04 */ s16 w;
    /* 0x06 */ s16 h;
    /* 0x08 */ s16 clutX;
    /* 0x0A */ s16 clutY;
    /* 0x0C */ u16 dx;
    /* 0x0E */ u16 dy;
    /* 0x10 */ u16 dw;
    /* 0x12 */ u16 dh;
    /* 0x14 */ u8 semiTrans;
    /* 0x15 */ u8 abr;
} TitlePart;

typedef struct {
    /* 0x0 */ char *text;
    /* 0x4 */ u8 kind;
    /* 0x5 */ u8 next;
    /* 0x6 */ u8 alt;
    /* 0x7 */ s8 image;
} ScrollPage;

typedef struct {
    /* 0x00 */ u8 unk0[0x98];
    /* 0x98 */ char *options[2];
    /* 0xA0 */ u8 unkA0[5];
    /* 0xA5 */ s8 choice;
    /* 0xA6 */ u8 unkA6[0xF];
    /* 0xB5 */ s8 cancelDisabled; /* Dialog.cancelDisabled */
    /* 0xB6 */ u8 unkB6[2];
} ChoiceDialog;

extern s32 OPEN_INTRO_IMAGE;
extern s32 OPEN_INTRO_SHOWN_IMAGE;
extern s32 OPEN_INTRO_IMAGE_FADE;
extern s32 OPEN_LABEL_X;
extern s32 OPEN_LABEL_Y;
extern UiWindow OPEN_MESSAGE_WINDOW;
extern UiWindow OPEN_PLAYER_NAME_WINDOW;
extern UiWindow OPEN_IMAGE_WINDOW;
extern POLY_FT4 OPEN_TITLE_PART_PRIMS[2][40];
extern u16 OPEN_WHITE_CLUT[256];
extern s16 CAMERA_TARGET_MODEL;

s32 loadDigimonModelPak(s32 slot, s32 id, s8 format, s32 loadAllAnims);

/* the pages of the introduction */
ScrollPage OPEN_INTRO_PAGES[33] = {
    { "Welcome to *c2Digital Card Battle*c7!", 0, 0x16, 0, -1 },
    { "Please proceed to *c4User Registration*c7.\nPlease enter your nickname\nfor this World.", 1, 2, 0, -1 },
    { "Please choose a Partner Card.\nYou can choose Veemon, Hawkmon,\nor Armadillomon.", 0, 3, 0, 0 },
    { "Each Partner Card comes with a matching\nStarter *c5Deck*c7, so you can\nbattle right from the start.", 0, 4, 0, 0 },
    { "Your Partner will grow with every battle\nand will become a reliable ally in \nthe battles you'll face.", 2, 5, 0, 1 },
    { "Now, please choose a Partner.\nUse the directional and *b2 buttons.", 0, 6, 0, -1 },
    { "This is a Veemon *c5Deck*c7,\nwith Fire and Darkness Digimon.\nIt's a strong offensive *c5Deck*c7.", 0, 9, 0, -1 },
    { "This is a Hawkmon *c5Deck*c7, with Nature\nand tricky Rare Digimon.\nIt's a well-balanced *c5Deck*c7.", 0, 9, 0, -1 },
    { "An Armadillomon *c5Deck*c7 has the\nstrong support of Rare Digimon.\nIt's a defensive *c5Deck*c7.", 0, 9, 0, -1 },
    { "Next is how you appear in this world.\nWe only have one character available.\nWe hope you like him.", 3, 0xA, 0, -1 },
    { "*c4User Registration*c7 is complete.\nWould you like to know more about\nthis world?", 4, 0xB, 0x20, -1 },
    { "In this Cyber World, you play Digimon\nDigital Card Battles. There are\nalways many opponents to play with.", 0, 0x10, 0, 2 },
    { "ERROR! (12)", 5, 0x15, 0x1B, 2 },
    { "ERROR! (13)", 0, 0xE, 0, -1 },
    { "ERROR! (14)", 6, 0x11, 0xF, -1 },
    { "ERROR! (15)", 0, 0x10, 0, -1 },
    { "Do you want to learn about the game?", 4, 0x1B, 0x20, -1 },
    { "ERROR! (17)", 0, 0x12, 0, -1 },
    { "ERROR! (18)", 0, 0x13, 0, -1 },
    { "ERROR! (19)", 7, 0x1A, 0x14, -1 },
    { "ERROR! (20)", 0, 0x10, 0, -1 },
    { "ERROR! (21)", 5, 0xD, 0x10, 2 },
    { "First, select the *c4Sound Settings*c7.\nThis can be changed during the game.", 8, 0x17, 0x16, -1 },
    { "Next, select *c4Polygon Battle Settings*c7.\nThis can also be changed during the game.", 9, 1, 0x17, -1 },
    { "Enjoy *c2Digital Card Battle*c7!", 0xB, 0x18, 0, -1 },
    { "Card data conversion had failed.\nDo you want to learn about this game?", 4, 0x1B, 0x20, -1 },
    { "ERROR! (26)", 4, 0x1B, 0x20, -1 },
    { "Then I'll quickly tell you about \n*c2Digital Card Battle*c7.", 0, 0x1C, 0, -1 },
    { "This is a fun world where a Player\ngets to play with Digimon and other\ncharacters, using Digimon Battle Cards.", 0, 0x1D, 0, 3 },
    { "Collect Cards through battles. Trade and\nFuse them to create your own Decks.", 0, 0x1E, 0, 4 },
    { "A *c5Deck *c7is a group of 30 Cards.\nYou'll use this to Battle Opponents.", 0, 0x1F, 0, 5 },
    { "Betamon in Beginner City will teach\nyou the basics. You'll learn the rest\nas you go along.", 0, 0x20, 0, 6 },
    { "Let's save your *c4Registration*c7.\nPlease insert a *c5MEMORY CARD*c7 with at\nleast 2 free blocks into MEMORY CARD slot 1.", 0xA, 0x18, 0, -1 },
};

/* not referenced by any code */
s16 D_801F0140[160] = {
    5, 8, 3, 9, 0xA, 0xE, 0x11, 0x12, 0x14, 0x13, 0x15, 0x17, 0x10, 0x18, 0x16, 0x1A,
    0x1E, 0x1F, 0x1B, 0x1D, 0x2A, 0x2B, 0x29, 0x2C, 0x2D, 0x36, 0x32, 0x31, 0x2E, 0x34, 0x37, 0x3B,
    0x3A, 0x38, 0x3C, 0x35, 0x34, 0x44, 0x42, 0x3F, 0x3E, 0x40, 0x4A, 0x8F, 0x50, 0x96, 0xB, 0x57,
    0x56, 0x52, 0x58, 0x55, 0x5A, 0x5B, 0x5D, 0x59, 0xA4, 0x60, 0x5C, 0x65, 0xA2, 0x5F, 0xA3, 0x5E,
    0x61, 0x64, -1, 0x66, 0x62, 0x6C, 0x8D, 0x6F, 0x71, 0x74, 0x70, 0x73, 0x78, 0x7A, 0x7B, 0x80,
    0x7D, 0x7C, 0x7E, 0x82, 0x81, 0x83, 0x7F, 0x89, 0x87, 0x8A, 0x88, 0x90, 0x91, 0x92, 0x93, 0x95,
    0x94, 0x9A, 0x9F, 0x9C, 0x9D, 0x9E, 0xA1, 0xA0, 0xAA, 0xAB, 0xA7, 0xA8, 0x51, 6, 0xC0, 0xDD,
    0xCB, 0xD4, 0xE2, 0xBF, 0xEB, 0xEC, 0xED, 0xEE, 0xE3, 0xE4, 0xE5, 0xCC, 0xCD, 0xE6, 0xEF, 0xF0,
    0xF5, 0xF6, 0x107, 0xFC, 0xFD, 0xFE, 0xFF, 0x100, 0x108, 0x10B, 0x10C, 0x10D, 0x10E, 0x10F, 0x110, 0x109,
    0x10A, -1, -1, -1, -1, -1, -1, -1, -1, 0x128, 0x127, 0x129, 0x12B, 0x12C, -1, 0,
};

/* the parts of the title screen */
TitlePart OPEN_TITLE_PARTS[22] = {
    { 0x340, 0xA0, 0x6C, 0x2E, 0x360, 0xD1, 9, 0xB, 0x6C, 0x2E, 1, 0 },
    { 0x340, 0, 0x80, 0x4A, 0x340, 0xCE, 0, 0, 0x80, 0x4A, 1, 1 },
    { 0x360, 0, 0x80, 0x4A, 0x340, 0xCF, 6, 6, 0x80, 0x4A, 1, 2 },
    { 0x35C, 0x83, 0x2C, 0x12, 0x340, 0xD1, -4, -3, 0x2C, 0x12, 1, 1 },
    { 0x350, 0x4A, 0x98, 0xC, 0x340, 0xD0, 0, 0, 0x98, 0xC, 1, 1 },
    { 0x350, 0x83, 0x30, 0x14, 0x340, 0xD2, 0, 0, 0x30, 0x14, 1, 2 },
    { 0x350, 0x6F, 0x78, 0x14, 0x350, 0xD0, 0, 0, 0x78, 0x13, 1, 0 },
    { 0x350, 0x56, 0x7C, 0x1A, 0x350, 0xD1, 0, 0, 0x7C, 0x19, 1, 2 },
    { 0x376, 0x4A, 4, 9, 0x350, 0xD3, 0, 0x10, 4, 9, 0, 0 },
    { 0x377, 0x4A, 4, 9, 0x350, 0xD4, 4, 0x10, 0x96, 9, 0, 0 },
    { 0x36E, 0x6F, 0x10, 0x10, 0x350, 0xCE, 0, 0, 0x10, 0x10, 0, 0 },
    { 0x372, 0x6F, 4, 8, 0x340, 0xD3, 0x10, 0, 0x104, 7, 0, 0 },
    { 0x373, 0x6F, 0x10, 0x10, 0x360, 0xCF, 0x114, 0, 0x10, 0x10, 0, 0 },
    { 0x36E, 0x7F, 9, 1, 0x350, 0xD5, 0, 0x10, 8, 0xF0, 0, 0 },
    { 0x36E, 0x7F, 9, 1, 0x350, 0xD5, 0x124, 0x10, -8, 0xF0, 0, 0 },
    { 0x36F, 0x56, 0x18, 0x18, 0x350, 0xCF, 4, 4, 0x18, 0x18, 1, 2 },
    { 0x375, 0x56, 4, 0xD, 0x340, 0xD4, 0x1C, 4, 0xF4, 0xC, 1, 2 },
    { 0x376, 0x56, 0x18, 0x18, 0x360, 0xD0, 0x110, 4, 0x18, 0x18, 1, 2 },
    { 0x36F, 0x6E, 0xD, 1, 0x360, 0xCE, 4, 0x1C, 0xC, 0xF0, 1, 2 },
    { 0x36F, 0x6E, 0xD, 1, 0x360, 0xCE, 0x128, 0x1C, -0xC, 0xF0, 1, 2 },
    { 0x340, 0x4A, 0x40, 0x56, 0x340, 0xD5, 0x1A, 0x5A, 0x40, 0x56, 1, 0 },
    { 0x350, 0x82, 0x60, 1, 0x350, 0xD2, 0, 0, 0x60, 0xF0, 0, 0 },
};

/* the title part's primitive; the tpage and clut stores index the array directly */
#define PRIM (&OPEN_TITLE_PART_PRIMS[FRAME_BUFFER_INDEX][OPEN_TITLE_PART_COUNT])

void OPEN_drawTitlePart(s32 x, s32 y, s32 part) {
    initPrimByType(0xC, PRIM, OPEN_TITLE_PARTS[part].semiTrans, 0);
    PRIM->r0 = 0x80;
    PRIM->g0 = 0x80;
    PRIM->b0 = 0x80;
    PRIM->x0 = x + OPEN_TITLE_PARTS[part].dx;
    PRIM->y0 = y + OPEN_TITLE_PARTS[part].dy;
    PRIM->x1 = x + OPEN_TITLE_PARTS[part].dx + OPEN_TITLE_PARTS[part].dw;
    PRIM->y1 = y + OPEN_TITLE_PARTS[part].dy;
    PRIM->x2 = x + OPEN_TITLE_PARTS[part].dx;
    PRIM->y2 = y + OPEN_TITLE_PARTS[part].dy + OPEN_TITLE_PARTS[part].dh;
    PRIM->x3 = x + OPEN_TITLE_PARTS[part].dx + OPEN_TITLE_PARTS[part].dw;
    PRIM->y3 = y + OPEN_TITLE_PARTS[part].dy + OPEN_TITLE_PARTS[part].dh;
    PRIM->u0 = OPEN_TITLE_PARTS[part].texX % 64 * 4;
    PRIM->v0 = OPEN_TITLE_PARTS[part].texY % 256;
    PRIM->u1 = OPEN_TITLE_PARTS[part].texX % 64 * 4 + (OPEN_TITLE_PARTS[part].w - 1);
    PRIM->v1 = OPEN_TITLE_PARTS[part].texY % 256;
    PRIM->u2 = OPEN_TITLE_PARTS[part].texX % 64 * 4;
    PRIM->v2 = OPEN_TITLE_PARTS[part].texY % 256 + (OPEN_TITLE_PARTS[part].h - 1);
    PRIM->u3 = OPEN_TITLE_PARTS[part].texX % 64 * 4 + (OPEN_TITLE_PARTS[part].w - 1);
    PRIM->v3 = OPEN_TITLE_PARTS[part].texY % 256 + (OPEN_TITLE_PARTS[part].h - 1);
    OPEN_TITLE_PART_PRIMS[FRAME_BUFFER_INDEX][OPEN_TITLE_PART_COUNT].tpage = getTPage(0, OPEN_TITLE_PARTS[part].abr, OPEN_TITLE_PARTS[part].texX, OPEN_TITLE_PARTS[part].texY);
    OPEN_TITLE_PART_PRIMS[FRAME_BUFFER_INDEX][OPEN_TITLE_PART_COUNT].clut = getClut(OPEN_TITLE_PARTS[part].clutX, OPEN_TITLE_PARTS[part].clutY);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFF6], PRIM);
    OPEN_TITLE_PART_COUNT++;
}

#undef PRIM

void OPEN_drawIntroImage(s32 x, s32 y, s32 z) {
    Rect16 rect; /* unused, but it is in the original stack frame */
    s32 next;
    s32 cur;
    s32 semiTrans;

    if (OPEN_INTRO_IMAGE != -1 || OPEN_INTRO_SHOWN_IMAGE != OPEN_INTRO_IMAGE) {
        if (OPEN_INTRO_IMAGE != OPEN_INTRO_SHOWN_IMAGE) {
            cur = OPEN_INTRO_SHOWN_IMAGE;
            next = OPEN_INTRO_IMAGE;
            OPEN_INTRO_IMAGE_FADE += 4;
            if (OPEN_INTRO_IMAGE_FADE > 0x80) {
                OPEN_INTRO_IMAGE_FADE = 0;
                OPEN_INTRO_SHOWN_IMAGE = next;
                cur = next;
                next = -1;
            }
        } else {
            cur = OPEN_INTRO_IMAGE;
            next = -1;
        }
        if (next != -1) {
            semiTrans = OPEN_INTRO_IMAGE_FADE != 0x80;
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = next % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, next + 0x1F1);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.g0 = OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.b0 = OPEN_INTRO_IMAGE_FADE;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 1, next / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = next % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, 0x1FF);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.g0 = OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.b0 = OPEN_INTRO_IMAGE_FADE;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 2, next / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
        if (cur != -1) {
            semiTrans = OPEN_INTRO_IMAGE_FADE != 0;
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = cur % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, cur + 0x1F1);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.g0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.b0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 1, cur / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = cur % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, 0x1FF);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.g0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.b0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 2, cur / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

void OPEN_drawPlayerName(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    drawText(x, y, PLAYER_PROFILES, 7, z);
}

void OPEN_drawIntroMessage(UiWindow *window) {
    char text[136];
    char *src;
    char *dst;
    s32 stop;
    s32 i;
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    if (OPEN_INTRO_TEXT.page != OPEN_INTRO_TEXT.shownPage) {
        OPEN_INTRO_TEXT.shownPage = OPEN_INTRO_TEXT.page;
        OPEN_INTRO_TEXT.length = 0;
    }
    OPEN_INTRO_TEXT.done = 0;
    for (i = 0; i < 133; i++) {
        text[i] = 0;
    }
    text[0] = '*';
    text[1] = 's';
    text[2] = '0';
    dst = &text[3];
    src = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].text;
    for (i = 0; i < OPEN_INTRO_TEXT.length; i++) {
        stop = 0;
        while (!stop) {
            if (*src == '*') {
                *dst++ = *src++;
                *dst++ = *src++;
            } else {
                *dst++ = *src++;
                stop = 1;
            }
            if (dst[-1] == 0) {
                OPEN_INTRO_TEXT.done = 1;
                break;
            }
        }
    }
    if (OPEN_INTRO_TEXT.done == 0) {
        OPEN_INTRO_TEXT.length++;
    }
    dst = text;
    drawText(x + 2, y + 1, (s32)dst, 7, z);
    if (OPEN_INTRO_TEXT.done != 0 && OPEN_INTRO_TEXT.waitInput != 0) {
        if (++OPEN_INTRO_TEXT.blink & 0x10) {
            drawIcon(x + 0x119, y + 0x1C, 0, 0x1B, z);
        }
    } else {
        OPEN_INTRO_TEXT.blink = 0;
    }
}

void OPEN_drawIntroImageWindow(UiWindow *window) {
    OPEN_drawIntroImage(window->originX, window->originY, window->z);
}

void OPEN_drawIntroScreen(void) {
    Rect16 rect;
    s32 i;

    if (OPEN_INTRO_IMAGE != OPEN_INTRO_SHOWN_IMAGE) {
        if (OPEN_INTRO_SHOWN_IMAGE == -1) {
            rect.x = 0x32;
            rect.y = 0x48;
            rect.w = 0xDC;
            rect.h = 0x52;
            animateWindowTo(&OPEN_IMAGE_WINDOW, &rect);
            playMenuSound(3);
            OPEN_INTRO_IMAGE_FADE = 0;
            OPEN_INTRO_SHOWN_IMAGE = OPEN_INTRO_IMAGE;
        } else if (OPEN_INTRO_IMAGE == -1) {
            if (OPEN_INTRO_IMAGE_FADE == 0) {
                animateWindowTo(&OPEN_IMAGE_WINDOW, (Rect16 *)-1);
                playMenuSound(4);
            } else if (OPEN_IMAGE_WINDOW.from.w == 0) {
                OPEN_INTRO_IMAGE_FADE = 0;
                OPEN_INTRO_SHOWN_IMAGE = -1;
            }
        }
    }
    drawWindow(&OPEN_PLAYER_NAME_WINDOW, OPEN_drawPlayerName, 0x19);
    drawWindow(&OPEN_MESSAGE_WINDOW, OPEN_drawIntroMessage, 0x19);
    drawWindow(&OPEN_IMAGE_WINDOW, OPEN_drawIntroImageWindow, 0x19);
    OPEN_CORNER_Y -= 16;
    if (OPEN_CORNER_Y < 16) {
        OPEN_CORNER_Y = 16;
    }
    OPEN_SIDEBAR_X += 8;
    if (OPEN_SIDEBAR_X > 0) {
        OPEN_SIDEBAR_X = 0;
    }
    OPEN_PANEL_Y += 4;
    if (OPEN_PANEL_Y > 28) {
        OPEN_PANEL_Y = 28;
    }
    OPEN_LABEL_X -= 12;
    if (OPEN_LABEL_X < 0xAA) {
        OPEN_LABEL_X = 0xAA;
    }
    OPEN_FRAME_Y -= 16;
    if (OPEN_FRAME_Y < 20) {
        OPEN_FRAME_Y = 20;
    }
    OPEN_TITLE_PART_COUNT = 0;
    OPEN_drawTitlePart(OPEN_CORNER_X, OPEN_CORNER_Y, 0);
    OPEN_drawTitlePart(OPEN_CORNER_X, OPEN_CORNER_Y, 1);
    OPEN_drawTitlePart(OPEN_CORNER_X, OPEN_CORNER_Y, 2);
    for (i = 0; i < OPEN_INTRO_TEXT.step; i++) {
        OPEN_drawTitlePart(i * 38 + OPEN_PANEL_X, OPEN_PANEL_Y, 3);
    }
    OPEN_drawTitlePart(OPEN_PANEL_X, OPEN_PANEL_Y, 4);
    OPEN_drawTitlePart(OPEN_PANEL_X, OPEN_PANEL_Y, 5);
    OPEN_drawTitlePart(OPEN_PANEL_X + 0x26, OPEN_PANEL_Y, 5);
    OPEN_drawTitlePart(OPEN_PANEL_X + 0x4C, OPEN_PANEL_Y, 5);
    OPEN_drawTitlePart(OPEN_PANEL_X + 0x72, OPEN_PANEL_Y, 5);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 10);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 11);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 12);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 13);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 14);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 15);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 16);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 17);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 18);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 19);
    OPEN_drawTitlePart(OPEN_LABEL_X, OPEN_LABEL_Y, 6);
    OPEN_drawTitlePart(OPEN_LABEL_X, OPEN_LABEL_Y, 7);
    OPEN_drawTitlePart(OPEN_LABEL_X, OPEN_LABEL_Y, 8);
    OPEN_drawTitlePart(OPEN_LABEL_X, OPEN_LABEL_Y, 9);
    OPEN_drawTitlePart(OPEN_SIDEBAR_X, OPEN_SIDEBAR_Y, 20);
    OPEN_drawTitlePart(OPEN_SIDEBAR_X, OPEN_SIDEBAR_Y, 21);
}

void OPEN_startSceneTasks(void) {
    initScene3D(1);
    endTask(0x19);
    spawnTask(0x19, 0x1F, 0, 0x800, &runSceneCameraTask, 1);
    endTask(0x1B);
    spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
}

void OPEN_stopSceneTasks(void) {
    endTask(0x1B);
    endTask(0x19);
    removeFrameCallback((s32)renderSceneModels);
    unloadAllModels();
    freeHeapBlocksByTag(0x7F);
}

void OPEN_showSceneModel(void) {
    ((Graphics *)&GRAPHICS)->snapCamera = 0;
    ((Graphics *)&GRAPHICS)->targetDistance = 3000;
    ((Graphics *)&GRAPHICS)->targetHeight = -((Model2220 *)SCENE_3D->models[0])->bonepos[0][1] * 3;
    SCENE_3D->modelState[0] = 1;
    applyAnimationFirstFrame(0, 0);
    startModelAnimation(0, 0, -2, 0);
}

void OPEN_unloadSceneModel(void) {
    freeHeapBlocksByTag(500);
    unloadModel(0);
    unloadModelAnimations(0);
}

void OPEN_runUserRegistration(s32 parentTask) {
    /* their own copies: the MESSAGE and PLAYER NAME literals would be merged
     * with later functions'; the first two keep the original order */
    static const char arcPath[] = "B:\\OPENING.ARC";
    static const char imageLabel[] = "IMAGE";
    static const char messageLabel[] = "MESSAGE";
    static const char nameLabel[] = "PLAYER NAME";
    Rect16 rect;
    ChoiceDialog dialog;
    u32 *arc;
    s32 i;
    s32 start;
    s32 model;
    s32 done;

    loadMusicTrack(0, 0x6E, 0x7F);
    playLoadedMusic(0);
    i = 0;
    spawnTask(0, -1, 0, 0x800, loadFile, arcPath, getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        waitFrames(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    for (i = 0; i < 256; i++) {
        OPEN_WHITE_CLUT[i] = 0xFFFF;
    }
    rect.x = 0;
    rect.y = 0x1FF;
    rect.w = 0x100;
    rect.h = 1;
    LoadImage((s16 *)&rect, (s32)OPEN_WHITE_CLUT);
    rect.x = 0x32;
    rect.y = 0x48;
    rect.w = 0xDC;
    rect.h = 0x52;
    openWindow(&OPEN_IMAGE_WINDOW, &rect, -1, (s16 *)-1, 8, 0x81, 0x80, 0xC);
    OPEN_IMAGE_WINDOW.label = (s32)imageLabel;
    OPEN_IMAGE_WINDOW.labelPalette = 7;
    animateWindowTo(&OPEN_IMAGE_WINDOW, (Rect16 *)-1);
    rect.x = 0xC;
    rect.y = 0xBC;
    rect.w = 0x128;
    rect.h = 0x2A;
    openWindow(&OPEN_MESSAGE_WINDOW, &rect, -1, (s16 *)-1, 8, 0x51, 0x80, 0xC);
    OPEN_MESSAGE_WINDOW.labelPalette = 8;
    OPEN_MESSAGE_WINDOW.label = (s32)messageLabel;
    rect.x = 0x18;
    rect.y = 0x18;
    rect.w = 0x48;
    rect.h = 0xC;
    openWindow(&OPEN_PLAYER_NAME_WINDOW, &rect, -1, (s16 *)-1, 8, 0x11, 0x80, 0xC);
    OPEN_PLAYER_NAME_WINDOW.label = (s32)nameLabel;
    animateWindowTo(&OPEN_PLAYER_NAME_WINDOW, (Rect16 *)-1);
    OPEN_INTRO_IMAGE = -1;
    OPEN_INTRO_SHOWN_IMAGE = -1;
    OPEN_INTRO_IMAGE_FADE = 0;
    OPEN_CORNER_X = 0xC;
    OPEN_CORNER_Y = 0xF0;
    OPEN_SIDEBAR_X = -0x60;
    OPEN_SIDEBAR_Y = 0;
    OPEN_PANEL_X = 0x90;
    OPEN_PANEL_Y = -0x14;
    OPEN_LABEL_X = 0x140;
    OPEN_LABEL_Y = 0x32;
    OPEN_FRAME_X = 0xE;
    OPEN_FRAME_Y = 0xF0;
    OPEN_INTRO_TEXT.step = 0;
    OPEN_INTRO_TEXT.page = 0;
    OPEN_INTRO_TEXT.shownPage = 0;
    OPEN_INTRO_TEXT.length = 0;
    OPEN_INTRO_TEXT.waitInput = 1;
    OPEN_INTRO_TEXT.unk18 = 0;
    done = 0;
    OPEN_startSceneTasks();
    loadDigimonModelPak(0, 0xEB, 0, 1);
    CAMERA_TARGET_MODEL = 0;
    addFrameCallback((s32)OPEN_drawIntroScreen);
    playMenuSound(3);
    model = 0;
    while (!done) {
        waitFrames(FRAME_INTERVAL);
        OPEN_INTRO_IMAGE = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].image;
        if ((PAD_STATES[0]->pressed & 0x40) && OPEN_INTRO_TEXT.done != 0) {
            switch (OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].kind) {
            /* the dialog pages: the switch below handles them */
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 1:
                animateWindowTo(&OPEN_MESSAGE_WINDOW, (Rect16 *)-1);
                OPEN_INTRO_TEXT.step = 1;
                spawnTask(0, -1, 0, 0x400, OPEN_runNameEntry, PLAYER_PROFILES, getCurrentTaskId(), 0, 0);
                waitFrames(0x7FFFFFFF);
                rect.x = 0xC;
                rect.y = 0xBC;
                rect.w = 0x128;
                rect.h = 0x2A;
                animateWindowTo(&OPEN_MESSAGE_WINDOW, &rect);
                rect.x = 0x18;
                rect.y = 0x18;
                rect.w = 0x48;
                rect.h = 0xC;
                animateWindowTo(&OPEN_PLAYER_NAME_WINDOW, &rect);
                OPEN_INTRO_TEXT.step = 2;
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                break;
            case 2:
                OPEN_INTRO_TEXT.step = 3;
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                OPEN_INTRO_IMAGE = -1;
                OPEN_INTRO_TEXT.waitInput = 0;
                spawnTask(0, -1, 0, 0x300, OPEN_runStarterSelect, getCurrentTaskId(), 0, 0, 0);
                waitFrames(0x7FFFFFFF);
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                OPEN_showSceneModel();
                model = 1;
                OPEN_INTRO_TEXT.step = 4;
                start = ((PlayerProfile *)PLAYER_PROFILES)->playTime;
                do {
                    waitFrames(FRAME_INTERVAL);
                } while (((PlayerProfile *)PLAYER_PROFILES)->playTime - start < 400);
                OPEN_INTRO_TEXT.waitInput = 1;
                break;
            case 0:
            case 3:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                playMenuSound(1);
                break;
            case 10:
                animateWindowTo(&OPEN_MESSAGE_WINDOW, (Rect16 *)-1);
                playMenuSound(4);
                waitFrames(16);
                OPEN_createNewSave();
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                rect.x = 0xC;
                rect.y = 0xBC;
                rect.w = 0x128;
                rect.h = 0x2A;
                animateWindowTo(&OPEN_MESSAGE_WINDOW, &rect);
                playMenuSound(3);
                break;
            case 11:
                done = 1;
                break;
            }
        }
        switch (OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].kind) {
        case 4:
            dialog.options[0] = "Yes";
            dialog.options[1] = "No";
            initDialog((u8 *)&dialog, NULL, 2);
            dialog.choice = 1;
            OPEN_INTRO_TEXT.waitInput = 0;
            dialog.cancelDisabled = 1;
            runDialog(&dialog);
            OPEN_INTRO_TEXT.waitInput = 1;
            if (model) {
                startModelAnimation(0, 1, -2, 0);
                OPEN_INTRO_TEXT.waitInput = 0;
                waitFrames(0x98);
                OPEN_INTRO_TEXT.waitInput = 1;
                OPEN_unloadSceneModel();
                OPEN_stopSceneTasks();
                model = 0;
            }
            switch (dialog.choice) {
            case 0:
            case 2:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].alt;
                break;
            case 1:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                break;
            }
            break;
        case 5:
            dialog.options[0] = "\x82\xa0\x82\xe9";
            dialog.options[1] = "\x82\xc8\x82\xa2";
            initDialog((u8 *)&dialog, NULL, 2);
            dialog.choice = 1;
            OPEN_INTRO_TEXT.waitInput = 0;
            dialog.cancelDisabled = 1;
            runDialog(&dialog);
            OPEN_INTRO_TEXT.waitInput = 1;
            switch (dialog.choice) {
            case 0:
            case 2:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].alt;
                break;
            case 1:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].alt;
                break;
            }
            break;
        case 8:
            dialog.options[0] = "Stereo";
            dialog.options[1] = "Mono";
            initDialog((u8 *)&dialog, "Sound Settings", 2);
            dialog.choice = 1;
            OPEN_INTRO_TEXT.waitInput = 0;
            dialog.cancelDisabled = 1;
            runDialog(&dialog);
            OPEN_INTRO_TEXT.waitInput = 1;
            switch (dialog.choice) {
            case 2:
                PLAYER_DATA(0).monoSound = 1;
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                SsSetMono();
                break;
            case 1:
                PLAYER_DATA(0).monoSound = 0;
                SsSetStereo();
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                break;
            case 0:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].alt;
                break;
            }
            break;
        case 9:
            dialog.options[0] = "On";
            dialog.options[1] = "Off";
            initDialog((u8 *)&dialog, "Polygon Battle", 2);
            dialog.choice = 1;
            OPEN_INTRO_TEXT.waitInput = 0;
            dialog.cancelDisabled = 1;
            runDialog(&dialog);
            OPEN_INTRO_TEXT.waitInput = 1;
            switch (dialog.choice) {
            case 2:
                PLAYER_DATA(0).skipBattleAnimation = 1;
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                break;
            case 1:
                PLAYER_DATA(0).skipBattleAnimation = 0;
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                break;
            case 0:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].alt;
                break;
            }
            break;
        }
    }
    animateWindowTo(&OPEN_MESSAGE_WINDOW, (Rect16 *)-1);
    playMenuSound(4);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 8, 0);
    waitFrames(20);
    removeFrameCallback((s32)OPEN_drawIntroScreen);
    hideScrollingBackground();
    stopScreenFade();
    waitFrames(10);
    resumeTask(parentTask, -1);
}
