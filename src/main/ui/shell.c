#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/shell.h"
#include "dcb/card_db.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/menu.h"
#include "dcb/sound.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/system.h"
#include "dcb/screen_copy.h"
#include "dcb/render_loop.h"
#include "dcb/boot.h"
#include "dcb/text.h"
#include "dcb/window.h"

s32 findNewPartnerAbility(Entry12 *abilityTable, s32 player, s32 slot) {
    s8 level;
    s32 partnerIndex;
    s32 i;

    level = ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk289;
    partnerIndex = getSlotPartnerIndex(player, slot);
    if (partnerIndex >= 0) {
        for (i = 0; i < 0x80; i++) {
            if (abilityTable[i].unk4[partnerIndex] == level) {
                if (getPartnerAbilityState(player, i) == 0) {
                    return i;
                }
                return -1;
            }
        }
    }
    return -1;
}

s32 getExpForNextLevel(s32 level) {
    level++;
    return (level + 2) * level;
}

s32 func_8004994C(s32 player, s32 slot) {
    if ((s8)((s8)((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk289 % 5) != 0) {
        return -1;
    }
    return rand() % 4;
}

/* the hacking screens */
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

INCLUDE_RODATA("asm/main/nonmatchings/ui/shell", STR_HACK_SYSTEM_ERROR);

INCLUDE_RODATA("asm/main/nonmatchings/ui/shell", STR_HACK_PARTNER_MOVED);

INCLUDE_RODATA("asm/main/nonmatchings/ui/shell", STR_HACK_TAUNT);

void drawHackingTerminal(s16 *win) {
    s16 rect[4];
    s32 x;
    s32 y;
    s16 z;
    u8 *script;

    x = win[0] + 1;
    y = win[1];
    if (HACK_LINE_COUNT >= 12) {
        y -= (HACK_LINE_COUNT - 11) * 7;
    }
    z = win[0x1D];
    if (HACK_WAIT_FRAMES > 0 || HACK_SCRIPT_DONE != 0) {
        HACK_WAIT_FRAMES--;
    } else {
        do {
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
                measureText(STR_HACK_SYSTEM_ERROR);
                rect[2] = (TEXT_WIDTH + 1) / 2 * 2;
                rect[3] = (TEXT_HEIGHT + 1) / 2 * 2;
                rect[0] = 0x28;
                rect[1] = 0x28;
                animateWindowTo((Unk80016F38 *)&HACK_ERROR_WINDOW, (Rect16 *)rect);
                playMenuSound(3);
                break;
            case 5:
                measureText(STR_HACK_PARTNER_MOVED);
                rect[2] = (TEXT_WIDTH + 1) / 2 * 2;
                rect[3] = (TEXT_HEIGHT + 1) / 2 * 2;
                rect[0] = 0x50;
                rect[1] = 0x78;
                animateWindowTo((Unk80016F38 *)&HACK_PARTNER_MOVED_WINDOW, (Rect16 *)rect);
                playMenuSound(3);
                break;
            case 6:
                measureText(STR_HACK_TAUNT);
                rect[2] = (TEXT_WIDTH + 1) / 2 * 2;
                rect[3] = (TEXT_HEIGHT + 1) / 2 * 2;
                rect[0] = (0x140 - rect[2]) >> 1;
                rect[1] = 0xB4 - rect[3] / 2;
                animateWindowTo((Unk80016F38 *)&HACK_TAUNT_WINDOW, (Rect16 *)rect);
                playMenuSound(3);
                break;
            case '>':
                HACK_TYPING_MODE = 1;
                goto copy;
            case '\n':
                HACK_TYPING_MODE = 0;
                HACK_WAIT_FRAMES = 20;
                HACK_LINE_COUNT++;
            default:
            copy:
                *HACK_TEXT_CURSOR++ = *HACK_SCRIPT_CURSOR;
                break;
            }
            if (*++HACK_SCRIPT_CURSOR == 0) {
                HACK_SCRIPT_DONE = 1;
                break;
            }
        } while (HACK_TYPING_MODE == 0 && HACK_WAIT_FRAMES == 0);
    }
    if ((HACK_BLINK_TIMER & 0x10) || HACK_WAIT_FRAMES == 0) {
        *HACK_TEXT_CURSOR = '|';
    } else {
        *HACK_TEXT_CURSOR = ' ';
    }
    HACK_BLINK_TIMER++;
    HACK_TEXT_CURSOR[1] = 0;
    drawMediumText(x, y, HACK_TEXT_BUFFER, 4, z);
}

void drawHackErrorText(void *win) {
    drawText((*(s16 *)((s8 *)win + 0)), (*(s16 *)((s8 *)win + 2)), &STR_HACK_SYSTEM_ERROR, 0, (s32) (*(s16 *)((s8 *)win + 0x3A)));
}

void drawHackPartnerMovedText(void *win) {
    drawText((*(s16 *)((s8 *)win + 0)), (*(s16 *)((s8 *)win + 2)), &STR_HACK_PARTNER_MOVED, 7, (s32) (*(s16 *)((s8 *)win + 0x3A)));
}

void drawHackTauntText(void *win) {
    drawText((*(s16 *)((s8 *)win + 0)), (*(s16 *)((s8 *)win + 2)), &STR_HACK_TAUNT, 7, (s32) (*(s16 *)((s8 *)win + 0x3A)));
}

void drawHackingWindows(void) {
    drawWindow((Unk80016F38 *)&HACK_TAUNT_WINDOW, &drawHackTauntText, 0xA);
    drawWindow((Unk80016F38 *)&HACK_PARTNER_MOVED_WINDOW, &drawHackPartnerMovedText, 0xA);
    drawWindow((Unk80016F38 *)&HACK_ERROR_WINDOW, &drawHackErrorText, 0xA);
    drawWindow((Unk80016F38 *)&HACK_TERMINAL_WINDOW, &drawHackingTerminal, 0xA);
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
    ((Unk80016F38 *)&HACK_TERMINAL_WINDOW)->unk2C = (s32)"SHELL COMMAND";
    ((Unk80016F38 *)&HACK_TERMINAL_WINDOW)->unk38 = 2;
    ((Unk80016F38 *)&HACK_TERMINAL_WINDOW)->unk39 = 8;
    playMenuSound(3);
    measureText(STR_HACK_SYSTEM_ERROR);
    r.w = (TEXT_WIDTH + 1) / 2 * 2;
    r.h = (TEXT_HEIGHT + 1) / 2 * 2;
    openWindow(&HACK_ERROR_WINDOW, &r, -1, (s16 *)-1, 0, 0x77, 0x80, 0xC);
    animateWindowTo((Unk80016F38 *)&HACK_ERROR_WINDOW, (Rect16 *)-1);
    ((Unk80016F38 *)&HACK_ERROR_WINDOW)->unk38 = 2;
    measureText(STR_HACK_PARTNER_MOVED);
    r.w = (TEXT_WIDTH + 1) / 2 * 2;
    r.h = (TEXT_HEIGHT + 1) / 2 * 2;
    openWindow((Unk80016F38 *)&HACK_ERROR_WINDOW + 1, &r, -1, (s16 *)-1, 0, 0x77, 0x80, 0xC);
    animateWindowTo((Unk80016F38 *)&HACK_ERROR_WINDOW + 1, (Rect16 *)-1);
    ((Unk80016F38 *)&HACK_ERROR_WINDOW)[1].unk38 = 2;
    measureText(STR_HACK_TAUNT);
    r.w = (TEXT_WIDTH + 1) / 2 * 2;
    r.h = (TEXT_HEIGHT + 1) / 2 * 2;
    r.x = (0x140 - r.w) >> 1;
    r.y = 0xB4 - r.h / 2;
    openWindow(&HACK_TAUNT_WINDOW, &r, -1, (s16 *)-1, 8, 0x15, 0x80, 8);
    ((Unk80016F38 *)&HACK_TAUNT_WINDOW)->unk2C = (s32)"MESSAGE";
    ((Unk80016F38 *)&HACK_TAUNT_WINDOW)->unk38 = 4;
    animateWindowTo((Unk80016F38 *)&HACK_TAUNT_WINDOW, (Rect16 *)-1);
    addFrameCallback((s32)drawHackingWindows);
    do {
        func_80014C08(FRAME_INTERVAL);
        if (HACK_SCRIPT_DONE != 0) {
            done = 1;
        }
    } while (done == 0);
    playMenuSound(4);
    animateWindowTo((Unk80016F38 *)&HACK_TERMINAL_WINDOW, (Rect16 *)-1);
    animateWindowTo((Unk80016F38 *)&HACK_ERROR_WINDOW, (Rect16 *)-1);
    animateWindowTo((Unk80016F38 *)&HACK_PARTNER_MOVED_WINDOW, (Rect16 *)-1);
    animateWindowTo((Unk80016F38 *)&HACK_TAUNT_WINDOW, (Rect16 *)-1);
    func_80014C08(20);
    removeFrameCallback((s32)drawHackingWindows);
    func_80014A48(parentTask);
}

void quitToTitleOrPlayEnding(s32 mode) {
    u8 dialog[0xB8];
    Rect16 vramRect = { 0, 0, 480, 512 };
    s32 parentTask;
    s32 done;

    parentTask = getCurrentTaskId();
    if (mode == 0) {
        freeScrollingBackground();
        func_80014C08(10);
        ClearImage(&vramRect, 0, 0, 0);
        DrawSync(0);
        func_80014C08(10);
        done = 0;
        stopMusic();
        func_80014C08(10);
        func_800149B8(0, -1, 0, 0x800, playOpeningMovie, 1, parentTask);
        func_80014C08(0x7FFFFFFF);
        resetDisplay(0x140, 0xF0, 0);
        func_800149B8(0x1F, 0, 0, 0x800, runRenderLoop, 0, 0, 0, 0);
        func_80014C08(2);
        do {
            func_800149B8(0, -1, 0, 0x600, D_801EBAFC, 8, parentTask, 0, 0);
            func_80014C08(0x7FFFFFFF);
            playMenuSound(3);
            initDialog(dialog,
                          "*c6 Is it OK to return to Title Screen?\n*c3(Unless you save the game now,\nyou won't be able "
                          "to continue.)",
                          1);
            runDialog(dialog);
            switch ((s8)dialog[0xA5]) {
            case 1:
                done = 1;
                break;
            case 0:
            case 2:
                done = 0;
                break;
            }
        } while (!done);
        func_80014C08(20);
        func_80014A48(0);
        func_80014A90();
    } else {
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, "P:\\endseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x800, D_801DF47C, parentTask, mode, 0, 0);
        func_80014C08(0x7FFFFFFF);
        func_80014C08(10);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, PATH_SAISEG_BIN, OVERLAY_LOAD_ADDR, getCurrentTaskId());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, parentTask, 0, 0);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/ui/shell", PATH_SAISEG_BIN);
