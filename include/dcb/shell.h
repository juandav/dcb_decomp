#ifndef DCB_SHELL_H
#define DCB_SHELL_H

#include "game.h"

typedef struct {
    /* 0x0 */ u8 unk0[4];
    /* 0x4 */ s8 unk4[6];
    /* 0xA */ u8 unkA[2];
} Entry12;

extern u8 STR_HACK_SYSTEM_ERROR[];
extern u8 STR_HACK_PARTNER_MOVED[];
extern u8 STR_HACK_TAUNT[];
extern s32 HACK_WAIT_FRAMES;
extern s32 HACK_BLINK_TIMER;
extern s32 HACK_TYPING_MODE;
extern s32 HACK_LINE_COUNT;
extern s32 HACK_SCRIPT_DONE;
extern s32 HACK_TEXT_BUFFER;
extern u8 *HACK_TEXT_CURSOR;
extern u8 *HACK_SCRIPT_CURSOR;
extern s32 HACK_TAUNT_WINDOW;
extern s32 HACK_PARTNER_MOVED_WINDOW;
extern s32 HACK_ERROR_WINDOW;
extern s32 HACK_TERMINAL_WINDOW;
extern s32 HACK_SCRIPT_INDEX;
extern u8 *HACKING_SCRIPTS[];
/* the same text as in startCpuDuel, kept as its own copy */
extern char PATH_SAISEG_BIN[];

void runHackingSequence(s32 scriptIndex, s32 parentTask);
s32 findNewPartnerAbility(Entry12 *abilityTable, s32 player, s32 slot);
s32 getExpForNextLevel(s32 level);
s32 func_8004994C(s32 player, s32 slot);
void drawHackErrorText(void *win);
void drawHackPartnerMovedText(void *win);
void drawHackTauntText(void *win);
void drawHackingTerminal();
void drawHackingWindows(void);
void quitToTitleOrPlayEnding(s32 mode);

#endif /* DCB_SHELL_H */
