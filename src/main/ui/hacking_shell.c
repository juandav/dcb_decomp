#include "dcb/hacking_shell.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/partner_level.h"
#include "dcb/card_db.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/save_checksum.h"
#include "dcb/player_rank.h"
#include "dcb/menu.h"
#include "dcb/dialog.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
#include "dcb/scroll_bg.h"
#include "dcb/vblank.h"
#include "dcb/screen_copy.h"
#include "dcb/render_loop.h"
#include "dcb/boot.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"

/* the hacking screens, typed out by drawHackingTerminal. Control codes:
   \001 types what follows one character per frame (as does a '>' line),
   \002 n waits n frames, \004/\005/\006 pop up the system error, partner
   moved and taunt windows; a new line types instantly again after 20 frames */
u8 *HACKING_SCRIPTS[4] = {
    "Hacking System 2000\n"
    " (C)Analogman Software\n"
    "\n"
    ">NETHACK 197.045.060.080\n"
    ">...\002\020...........\002 .....\0020....\n"
    " CONNECT OK!\n"
    "\002 \n"
    " USER NAME: \001\002 \001ANALOGMAN\n"
    "  PASSWORD: \001\002 \001********\n"
    "\002\020\n"
    " LOGIN\n"
    "\n"
    ">SECURE mem:1FFFFF-8FFFFF\n"
    "\002  SECUER mem OK!\n"
    ">PIPELINE indefin CLR\n"
    "\002  *** VIOLATION ERROR! ***\n"
    " [ILLEGAL COMMAND]\n"
    "\n"
    ">DOWNLOAD\n"
    ">1FFFFF > \"ANALOGMAN\"\n"
    " WISH YOU ACCESS\n"
    " \"DIGINET\" ? (Y/N):\002 y\n"
    ">.....\002\n"
    ".......\002(DONE!\n"
    " OK!\n"
    "\n"
    " START DOWNLOAD\"ANALOGMAN\"\n"
    "\n"
    ">GOODBYE!\n"
    "\n"
    " LOGOUT\n"
    "\002\377\002\300",
    "Hacking System 2000\n"
    " (C)Analogman Software\n"
    "\n"
    ">NETHACK 197.045.060.080\n"
    ">...\002\020...........\002 .....\0020....\n"
    "\n"
    " CONNECT OK!\n"
    "\002 \n"
    " USER NAME: \001\002 \001ANALOGMAN\n"
    "  PASSWORD: \001\002 \001********\n"
    "\002\020\n"
    " LOGIN\n"
    "\n"
    ">PIPELINE indefin CLR\n"
    "\002  *** VIOLATION ERROR! ***\n"
    " [ILLEGAL COMMAND]\n"
    "\n"
    ">SIGNin OPENGAME\n"
    ">SET ARENA\n"
    " ARENA NAME ?\n"
    ">\002 \"ANALOG ARENA\"\n"
    " SIGNin OPENGAME OK!\n"
    "\002 \n"
    "\n"
    ">GOODBYE!\n"
    "\n"
    " LOGOUT\n"
    "\002\377\002\300",
    "Hacking System 2000\n"
    " (C)Analogman Software\n"
    "\n"
    ">NETHACK 197.045.060.080\n"
    ">...\002\020...........\002 .....\0020....\n"
    "\n"
    " CONNECT OK!\n"
    "\002 \n"
    " USER NAME: \001\002 \001ANALOGMAN\n"
    "  PASSWORD: \001\002 \001********\n"
    "\002\020\n"
    " LOGIN\n"
    "\n"
    ">6e46521a68FbD30\n"
    ">OPEN [SYScom]\n"
    ">PIPELINE indefin CLR\n"
    "\002  *** VIOLATION ERROR! ***\n"
    " [ILLEGAL COMMAND]\n"
    "\004\n"
    ">GET memDATA:ALL\n"
    " GET memDATA OK!\n"
    "> PIPELINE indefin CLR\n"
    "\002  *** VIOLATION ERROR! ***\n"
    " [ILLEGAL COMMAND]\n"
    "\005\n"
    ">MOVE PT_C>end\n"
    " MOVE PT_C :\n"
    " mf481!['&634naF61AIs31n&\n"
    "\006\n"
    "\n"
    ">GOODBYE!\n"
    "\n"
    " LOGOUT\n"
    "\002\377\002\300",
    "Hacking System 2000\n"
    " (C)Analogman Software\n"
    "\n"
    ">NETHACK 197.045.060.080\n"
    ">...\002\020...........\002 .....\0020....\n"
    "\n"
    " CONNECT OK!\n"
    "\002 \n"
    " USER NAME: \001\002 \001ANALOGMAN\n"
    "  PASSWORD: \001\002 \001********\n"
    "\002\020\n"
    " LOGIN\n"
    "\n"
    " ERROR!\n"
    " ERROR!\n"
    " ERROR!\n"
    " ERROR!\n"
    " ERROR!\n"
    " SYSTEM DOWN!\n"
    "\n"
    " LOGOUT\n"
    "\002\377\002\300",
};

/* The texts of the three message windows; GCC keeps one copy of each,
   emitted with drawHackingTerminal, the first function that uses them */
#define STR_HACK_SYSTEM_ERROR "         *c6SYSTEM ERROR\n*c2Illegal Sharing:*c7Unauthorized command\nwas used."
#define STR_HACK_PARTNER_MOVED "Player's Partner Card was moved to\nthe bottom of Online Deck."
#define STR_HACK_TAUNT "Ha ha. Isn't this interesting!"

void drawHackingTerminal(UiWindow *win) {
    Rect16 rect;
    s32 x;
    s32 y;
    s32 z;
    u8 *script;

    x = win->originX + 1;
    y = win->originY;
    if (HACK_LINE_COUNT >= 12) {
        y -= (HACK_LINE_COUNT - 11) * 7; /* scroll up once 12 lines are shown */
    }
    z = win->z;
    if (HACK_WAIT_FRAMES > 0 || HACK_SCRIPT_DONE != 0) {
        HACK_WAIT_FRAMES--;
    } else {
        for (;;) {
            switch (*HACK_SCRIPT_CURSOR) {
            case 1:
                HACK_TYPING_MODE = 1;
                break;
            case 2:
                script = HACK_SCRIPT_CURSOR;
                HACK_SCRIPT_CURSOR = script + 1;
                HACK_WAIT_FRAMES = script[1];
                break;
            case 4:
                /* the windows grow to the text's size rounded up to even */
                measureText((u8 *)STR_HACK_SYSTEM_ERROR);
                rect.w = (TEXT_WIDTH + 1) / 2 * 2;
                rect.h = (TEXT_HEIGHT + 1) / 2 * 2;
                rect.x = 0x28;
                rect.y = 0x28;
                animateWindowTo(&HACK_ERROR_WINDOW, &rect);
                playMenuSound(3);
                break;
            case 5:
                measureText((u8 *)STR_HACK_PARTNER_MOVED);
                rect.w = (TEXT_WIDTH + 1) / 2 * 2;
                rect.h = (TEXT_HEIGHT + 1) / 2 * 2;
                rect.x = 0x50;
                rect.y = 0x78;
                animateWindowTo(&HACK_PARTNER_MOVED_WINDOW, &rect);
                playMenuSound(3);
                break;
            case 6:
                measureText((u8 *)STR_HACK_TAUNT);
                rect.w = (TEXT_WIDTH + 1) / 2 * 2;
                rect.h = (TEXT_HEIGHT + 1) / 2 * 2;
                rect.x = (0x140 - rect.w) >> 1;
                rect.y = 0xB4 - rect.h / 2;
                animateWindowTo(&HACK_TAUNT_WINDOW, &rect);
                playMenuSound(3);
                break;
            case '>':
                HACK_TYPING_MODE = 1;
                *HACK_TEXT_CURSOR++ = *HACK_SCRIPT_CURSOR;
                break;
            case '\n':
                HACK_TYPING_MODE = 0;
                HACK_WAIT_FRAMES = 20;
                HACK_LINE_COUNT++;
                *HACK_TEXT_CURSOR++ = *HACK_SCRIPT_CURSOR;
                break;
            default:
                *HACK_TEXT_CURSOR++ = *HACK_SCRIPT_CURSOR;
                break;
            }
            if (*++HACK_SCRIPT_CURSOR == 0) {
                break;
            }
            /* typing or a pause hands over to the next frames */
            if (HACK_TYPING_MODE != 0 || HACK_WAIT_FRAMES != 0) {
                goto blink;
            }
        }
        /* the script ran out */
        HACK_SCRIPT_DONE = 1;
    }
blink:
    /* the cursor blinks every 16 frames while the script waits */
    if ((HACK_BLINK_TIMER & 0x10) || HACK_WAIT_FRAMES == 0) {
        *HACK_TEXT_CURSOR = '|';
    } else {
        *HACK_TEXT_CURSOR = ' ';
    }
    HACK_BLINK_TIMER++;
    HACK_TEXT_CURSOR[1] = 0;
    drawMediumText(x, y, HACK_TEXT_BUFFER, 4, z);
}

void drawHackErrorText(UiWindow *win) {
    drawText(win->originX, win->originY, (s32)STR_HACK_SYSTEM_ERROR, 0, win->z);
}

void drawHackPartnerMovedText(UiWindow *win) {
    drawText(win->originX, win->originY, (s32)STR_HACK_PARTNER_MOVED, 7, win->z);
}

void drawHackTauntText(UiWindow *win) {
    drawText(win->originX, win->originY, (s32)STR_HACK_TAUNT, 7, win->z);
}

void drawHackingWindows(void) {
    drawWindow(&HACK_TAUNT_WINDOW, &drawHackTauntText, 0xA);
    drawWindow(&HACK_PARTNER_MOVED_WINDOW, &drawHackPartnerMovedText, 0xA);
    drawWindow(&HACK_ERROR_WINDOW, &drawHackErrorText, 0xA);
    drawWindow(&HACK_TERMINAL_WINDOW, &drawHackingTerminal, 0xA);
}

void runHackingSequence(s32 scriptIndex, s32 parentTask) {
    Rect16 r;
    u8 textBuf[0x401];
    s32 i;
    s32 done;

    done = 0;
    HACK_SCRIPT_INDEX = scriptIndex;
    HACK_WAIT_FRAMES = 20;
    HACK_BLINK_TIMER = 0;
    HACK_TYPING_MODE = 0;
    HACK_SCRIPT_DONE = 0;
    HACK_LINE_COUNT = 0;
    for (i = 0; i < 0x401; i++) {
        textBuf[i] = 0;
    }
    HACK_TEXT_BUFFER = (s32)textBuf;
    HACK_TEXT_CURSOR = textBuf;
    HACK_SCRIPT_CURSOR = HACKING_SCRIPTS[HACK_SCRIPT_INDEX];
    r.x = 0x94;
    r.y = 0x20;
    r.w = 0xA0;
    r.h = 0x54;
    openWindow(&HACK_TERMINAL_WINDOW, &r, -1, (s16 *)-1, 8, 0x58, 0x80, 0xC);
    HACK_TERMINAL_WINDOW.label = (s32)"SHELL COMMAND";
    HACK_TERMINAL_WINDOW.palette = 2;
    HACK_TERMINAL_WINDOW.labelPalette = 8;
    playMenuSound(3);
    measureText((u8 *)STR_HACK_SYSTEM_ERROR);
    r.w = (TEXT_WIDTH + 1) / 2 * 2;
    r.h = (TEXT_HEIGHT + 1) / 2 * 2;
    openWindow(&HACK_ERROR_WINDOW, &r, -1, (s16 *)-1, 0, 0x77, 0x80, 0xC);
    animateWindowTo(&HACK_ERROR_WINDOW, (Rect16 *)-1);
    HACK_ERROR_WINDOW.palette = 2;
    measureText((u8 *)STR_HACK_PARTNER_MOVED);
    r.w = (TEXT_WIDTH + 1) / 2 * 2;
    r.h = (TEXT_HEIGHT + 1) / 2 * 2;
    /* HACK_ERROR_WINDOW + 1 is HACK_PARTNER_MOVED_WINDOW */
    openWindow(&HACK_ERROR_WINDOW + 1, &r, -1, (s16 *)-1, 0, 0x77, 0x80, 0xC);
    animateWindowTo(&HACK_ERROR_WINDOW + 1, (Rect16 *)-1);
    (&HACK_ERROR_WINDOW)[1].palette = 2;
    measureText((u8 *)STR_HACK_TAUNT);
    r.w = (TEXT_WIDTH + 1) / 2 * 2;
    r.h = (TEXT_HEIGHT + 1) / 2 * 2;
    r.x = (0x140 - r.w) >> 1;
    r.y = 0xB4 - r.h / 2;
    openWindow(&HACK_TAUNT_WINDOW, &r, -1, (s16 *)-1, 8, 0x15, 0x80, 8);
    HACK_TAUNT_WINDOW.label = (s32)"MESSAGE";
    HACK_TAUNT_WINDOW.palette = 4;
    animateWindowTo(&HACK_TAUNT_WINDOW, (Rect16 *)-1);
    addFrameCallback((s32)drawHackingWindows);
    do {
        waitFrames(FRAME_INTERVAL);
        if (HACK_SCRIPT_DONE != 0) {
            done = 1;
        }
    } while (done == 0);
    playMenuSound(4);
    animateWindowTo(&HACK_TERMINAL_WINDOW, (Rect16 *)-1);
    animateWindowTo(&HACK_ERROR_WINDOW, (Rect16 *)-1);
    animateWindowTo(&HACK_PARTNER_MOVED_WINDOW, (Rect16 *)-1);
    animateWindowTo(&HACK_TAUNT_WINDOW, (Rect16 *)-1);
    waitFrames(20);
    removeFrameCallback((s32)drawHackingWindows);
    resumeTask(parentTask);
}
