#ifndef DCB_PARTNER_LEVEL_H
#define DCB_PARTNER_LEVEL_H

#include "game.h"

typedef struct {
    /* 0x0 */ u8 unk0[4];
    /* 0x4 */ s8 learnLevels[6];
    /* 0xA */ u8 unkA[2];
} AbilityLearnEntry;

extern s32 HACK_WAIT_FRAMES;
extern s32 HACK_BLINK_TIMER;
extern s32 HACK_TYPING_MODE;
extern s32 HACK_LINE_COUNT;
extern s32 HACK_SCRIPT_DONE;
extern s32 HACK_TEXT_BUFFER;
extern u8 *HACK_TEXT_CURSOR;
extern u8 *HACK_SCRIPT_CURSOR;
extern UiWindow HACK_TAUNT_WINDOW;
extern UiWindow HACK_PARTNER_MOVED_WINDOW;
extern UiWindow HACK_ERROR_WINDOW;
extern UiWindow HACK_TERMINAL_WINDOW;
extern s32 HACK_SCRIPT_INDEX;
extern u8 *HACKING_SCRIPTS[];

s32 findNewPartnerAbility(AbilityLearnEntry *abilityTable, s32 player, s32 slot);
s32 getExpForNextLevel(s32 level);
s32 rollPartnerAbility(s32 player, s32 slot);

#endif /* DCB_PARTNER_LEVEL_H */
