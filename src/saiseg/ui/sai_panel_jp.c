#include "common.h"
#include "game.h"
#include "dcb/card_db.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/script.h"
#include "dcb/sound.h"
#include "dcb/sound_play.h"
#include "dcb/player_rank.h"
#include "dcb/frame_callback.h"
#include "dcb/fade.h"
#include "dcb/saiseg.h"

/* jp's event loop, which runs the area script's events, and the map window
   it opens and closes (sai_panel.c is us's and eu's) */

extern JpWindowDef SAI_MESSAGE_WINDOW_DEF;
extern JpWindowDef SAI_RIGHT_PORTRAIT_WINDOW_DEF;
extern JpWindowDef SAI_LEFT_PORTRAIT_WINDOW_DEF;
extern s16 SAI_MUSIC_VOLUMES[];
extern void runWindowTask();
extern void renderScrollingBackground();

void openKanjiPage(s32 page, s32 capacity);
void clearKanjiPage(s32 page);
void closeKanjiPage(s32 page);
void runSubsegFromArea(s32);
s32 enterPassword(void);
void openChoiceMenu(JpMenu *, s32, s32, s32, s32);
void addChoiceMenuItem(JpMenu *, s32, void (*)(void));
void setBackgroundScrollMode(s32);
void allocAreaFlags(void);
s32 startCpuDuel(void);

void SAI_addTextLine(s8 withBits);
void SAI_clearTextVram(void);
void SAI_runMenu(void);
s32 SAI_countSpareCopies(s8 inDecks, s16 card);
void SAI_resolveOpponentDeck(void);
void SAI_runDeckChoice(void);
void SAI_loadCardImage(s8 slot, s32 card);
void SAI_showLeftSprite(s32 kind);
void SAI_setPortraitBrightness(s8 index, u8 value);
void func_801EDEF0(void);
void func_801EDF7C(s8 fade);
void func_801EE058(void);
void SAI_giveBits(void);
void SAI_playSlotMachine(void);
void SAI_showRightSprite(s32 kind);
ScriptRunner *SAI_loadAreaScript(void);
void func_801F0F48(void);
void func_801F1130();
void SAI_saveScriptFlags(SaiState *state);
void SAI_loadScriptFlags(void);

/* no prototype: this module passes its coordinates as ints */
void initVramSprite();

void SAI_runAreaScript(void);

void func_801EE8F0(void) {
    SAI_STATE->unk4B = -1;
    SAI_STATE->unk3C = 20;
    do {
        waitFrames(1);
    } while (SAI_STATE->unk4B == -1);
    SAI_STATE->unk4B = 0;
}

void SAI_showRightSprite(s32 kind) {
    SpriteDef *def;
    s8 i;

    def = &SAI_PORTRAIT_SPRITE_DEFS[kind];
    SAI_UI.unk3B4->state = 2;
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SAI_UI.unk3B4->unk2 != 1);
    for (i = 0; i < 2; i++) {
        initVramSprite(DB(i).primSlots[0] + 0x80, 0xE6, 0x4F, def->clut, def->colorMode, def->vramX, def->vramY, def->width, def->height, -1);
    }
    SAI_UI.unk3B4->state = 1;
    exitTask();
}

/* runs the area script up to its next event and does what the event says */
void SAI_runAreaScript(void) {
    Rect16 rect = { 0x180, 0x100, 0x40, 0x100 };
    s32 result;
    s32 won;
    SpriteDef *def;
    s8 i;
    s32 arg;
    s32 bits;
    char *text;
    TypedLine *line;
    u8 *sprite;
    s16 item;
    SaiState *state;
    u16 icon;

    if (SAI_UI.unk3C0 != 0) {
        SAI_UI.unk3C0--;
        return;
    }
    do {
        result = runScriptToNextEvent(SAI_STATE->runner->script, SAI_STATE->regs);
        if (result == 1) {
            switch (SAI_STATE->runner->script->eventOp) {
            case 10:
                switch (SAI_STATE->runner->script->eventArg) {
                case 0:
                    spawnTask(0, -1, 0, 0x400, runWindowTask, &SAI_MESSAGE_WINDOW_DEF, getCurrentTaskId());
                    SAI_UI.unk3B0 = (JpWindow *)waitFrames(0x7FFFFFFF);
                    break;
                case 1:
                    SAI_UI.unk3B0->state = 4;
                    break;
                case 2:
                    SAI_addTextLine(0);
                    break;
                case 4:
                    SAI_UI.typing = 1;
                    SAI_STATE->unk18 = 1;
                    return;
                case 3:
                    SAI_STATE->unk18 = 3;
                    return;
                case 5:
                    SAI_UI.typing = 1;
                    break;
                case 6:
                    SAI_STATE->unk18 = 2;
                    return;
                case 7:
                    SAI_UI.unk3B4->state = 4;
                    SAI_STATE->unk48 = 0;
                    break;
                case 8:
                    func_801EDF7C(0);
                    ClearImage(&rect, 0, 0, 0);
                    runSubsegFromArea(0);
                    func_801EDEF0();
                    return;
                case 9:
                    func_801EDF7C(0);
                    ClearImage(&rect, 0, 0, 0);
                    runSubsegFromArea(1);
                    func_801EDEF0();
                    return;
                case 10:
                    SAI_STATE->regs[12] = PLAYER_DATA(0).bits;
                    break;
                case 11:
                    strcpy(((SessionData *)SESSION_DATA)->opponentName, (u8 *)SAI_STATE->regs[17]);
                    strcpy(((SessionData *)SESSION_DATA)->opponentDeck.name, (u8 *)SAI_STATE->regs[11]);
                    ((SessionData *)SESSION_DATA)->opponentDeckIndex = SAI_STATE->regs[2];
                    ((SessionData *)SESSION_DATA)->cpuStyle[0] = SAI_STATE->regs[7];
                    ((SessionData *)SESSION_DATA)->cpuStyle[1] = SAI_STATE->regs[8];
                    ((SessionData *)SESSION_DATA)->cpuStyle[2] = SAI_STATE->regs[9];
                    ((SessionData *)SESSION_DATA)->cpuStyle[3] = SAI_STATE->regs[10];
                    SAI_STATE->bits = SAI_STATE->regs[6];
                    SAI_UI.unk3C9 = 0;
                    if (((SessionData *)SESSION_DATA)->cpuStyle[0] == 3 || ((SessionData *)SESSION_DATA)->cpuStyle[1] == 3 ||
                        ((SessionData *)SESSION_DATA)->cpuStyle[2] == 3 || ((SessionData *)SESSION_DATA)->cpuStyle[3] == 3) {
                        ((SessionData *)SESSION_DATA)->cpuStyle[2] = ((SessionData *)SESSION_DATA)->cpuStyle[3] = 0;
                        ((SessionData *)SESSION_DATA)->cpuStyle[0] = ((SessionData *)SESSION_DATA)->cpuStyle[1] = 0;
                        ((SessionData *)SESSION_DATA)->tutorial = 1;
                    } else {
                        ((SessionData *)SESSION_DATA)->tutorial = 0;
                    }
                    break;
                case 12:
                    ((SessionData *)SESSION_DATA)->opponentDeck.cards[(s8)SAI_UI.unk3C9].type = SAI_STATE->regs[7];
                    ((SessionData *)SESSION_DATA)->opponentDeck.cards[(s8)SAI_UI.unk3C9].index = SAI_STATE->regs[8];
                    SAI_UI.unk3C9++;
                    break;
                case 13:
                    setBackgroundScrollMode(2);
                    func_801EE8F0();
                    removeFrameCallback((s32)func_801F1130);
                    SAI_resolveOpponentDeck();
                    SAI_STATE->unk4E = SAI_UI.unk3C8;
                    SAI_UI.unk3B4->state = 4;
                    SAI_UI.unk3B0->state = 4;
                    waitFrames(30);
                    if (((SessionData *)SESSION_DATA)->tutorial == 0) {
                        SAI_runDeckChoice();
                    }
                    SAI_STATE->unk43 = 0;
                    state = SAI_STATE;
                    state->scriptOffset = state->runner->script->pc - state->runner->script->start;
                    SAI_saveScriptFlags(state);
                    SAI_STATE->unk44 = 1;
                    freeHeapBlocksByTag(0x190);
                    waitFrames(30);
                    won = startCpuDuel() ^ 1;
                    do {
                        waitFrames(1);
                    } while (SAI_STATE->unk43 != 2);
                    SAI_STATE->runner = SAI_loadAreaScript();
                    SAI_STATE->unk43 = 0;
                    allocAreaFlags();
                    SAI_STATE->runner->regs = SAI_STATE->regs;
                    SAI_STATE->runner->script->pc = SAI_STATE->runner->script->start + SAI_STATE->scriptOffset;
                    SAI_loadScriptFlags();
                    SAI_STATE->regs[1] = won;
                    *SAI_STATE->runner->regs = 1;
                    func_801F0F48();
                    loadSoundEffectBank(1);
                    if (SAI_STATE->unk45 == 0) {
                        SAI_STATE->unk45 = 1;
                    } else {
                        SAI_STATE->unk45 = 0;
                    }
                    if (((SessionData *)SESSION_DATA)->tutorial != 1) {
                        if (SAI_STATE->regs[1] == 0) {
                            if (PLAYER_DATA(0).battleLosses != 999) {
                                PLAYER_DATA(0).battleLosses++;
                            }
                        } else {
                            if (PLAYER_DATA(0).battleWins != 999) {
                                PLAYER_DATA(0).battleWins++;
                            }
                        }
                    }
                    updatePlayerRanks(0);
                    for (i = 0; i < 7; i++) {
                        if (PLAYER_DATA(0).shops[i].timer >= 0) {
                            PLAYER_DATA(0).shops[i].timer++;
                        }
                        if (PLAYER_DATA(0).shops[i].timer >= PLAYER_DATA(0).shops[i].period) {
                            PLAYER_DATA(0).shops[i].timer = 0;
                            PLAYER_DATA(0).shops[i].soldBits = 0;
                            PLAYER_DATA(0).shops[i].starterStock = 0;
                            PLAYER_DATA(0).shops[i].seeds[6][0] = PLAYER_DATA(0).shops[i].seeds[6][1];
                            PLAYER_DATA(0).shops[i].seeds[6][1] = rand();
                        }
                    }
                    SAI_giveBits();
                    SAI_clearTextVram();
                    return;
                case 14:
                    SAI_UI.unk3B4->state = 2;
                    break;
                case 15:
                    SAI_STATE->unk4E = SAI_UI.unk3C8;
                    SAI_UI.unk3B4->state = 4;
                    SAI_UI.unk3B0->state = 4;
                    func_801EE8F0();
                    removeFrameCallback((s32)func_801F1130);
                    waitFrames(30);
                    setBackgroundScrollMode(3);
                    SAI_STATE->regs[1] = enterPassword();
                    setBackgroundScrollMode(1);
                    waitFrames(60);
                    SAI_STATE->unk4B = 1;
                    addFrameCallback((s32)func_801F1130);
                    SAI_clearTextVram();
                    SAI_UI.unk3C8 = SAI_STATE->unk4E;
                    openChoiceMenu(&SAI_UI.menu, (s8)SAI_UI.unk3C8, 0x32, 0, 0);
                    SAI_STATE->unk49 = 0;
                    SAI_STATE->unk48 = 0;
                    return;
                case 16:
                    openKanjiPage(0xF, 0xE7);
                    spawnTask(0, -1, 0, 0x400, runWindowTask, &SAI_BITS_WINDOW_DEF, getCurrentTaskId());
                    SAI_UI.unk3B8 = (JpWindow *)waitFrames(0x7FFFFFFF);
                    break;
                case 17:
                    SAI_UI.unk3B8->state = 4;
                    waitFrames(5);
                    closeKanjiPage(0xF);
                    break;
                case 18:
                    SAI_UI.unk3BC->state = 4;
                    SAI_STATE->unk49 = 0;
                    break;
                case 19:
                    if (SAI_STATE->regs[7] == 0) {
                        SAI_setPortraitBrightness(0, SAI_STATE->regs[8]);
                    } else {
                        SAI_setPortraitBrightness(1, SAI_STATE->regs[8]);
                    }
                    break;
                case 20:
                    SAI_STATE->unk4E = SAI_UI.unk3C8;
                    func_801EDF7C(1);
                    SAI_playSlotMachine();
                    addFrameCallback((s32)renderScrollingBackground);
                    openChoiceMenu(&SAI_UI.menu, (s8)SAI_UI.unk3C8, 0x32, 0, 0);
                    func_801EDEF0();
                    return;
                case 21:
                    fadeOutMusic(2);
                    break;
                case 22:
                    playSoundEffect(0x17);
                    PLAYER_DATA(0).bits += SAI_STATE->regs[12];
                    if (PLAYER_DATA(0).bits > 999998) {
                        PLAYER_DATA(0).bits = 999999;
                    }
                    break;
                case 23:
                    openKanjiPage(0xF, 0x1B9);
                    clearKanjiPage(0xF);
                    for (i = 0; i < 4; i++) {
                        SAI_TYPED_TEXT.lines[i].active = 0;
                    }
                    SAI_TYPED_TEXT.next = SAI_TYPED_TEXT.lines;
                    break;
                case 24:
                    text = (char *)SAI_STATE->regs[16];
                    SAI_TYPED_TEXT.next->active = 1;
                    sprintf(SAI_TYPED_TEXT.next->text, "s0%s", text);
                    line = SAI_TYPED_TEXT.next;
                    line->length = strlen(line->text);
                    SAI_TYPED_TEXT.next->shown = 0;
                    SAI_TYPED_TEXT.next++;
                    break;
                case 25:
                    SAI_STATE->unk18 = 1;
                    return;
                case 26:
                    clearKanjiPage(0xF);
                    for (i = 0; i < 4; i++) {
                        SAI_TYPED_TEXT.lines[i].active = 0;
                    }
                    SAI_TYPED_TEXT.next = SAI_TYPED_TEXT.lines;
                    break;
                case 27:
                    clearKanjiPage(0xF);
                    closeKanjiPage(0xF);
                    break;
                }
                break;
            case 11:
                switch (SAI_STATE->runner->script->eventArg) {
                case 0:
                    def = &SAI_PORTRAIT_SPRITE_DEFS[(s16)SAI_STATE->runner->script->params[0]];
                    for (i = 0; i < 2; i++) {
                        initVramSprite(DB(i).primSlots[0] + 0x80, 0xE6, 0x4F, def->clut, def->colorMode, def->vramX, def->vramY, def->width, def->height, -1);
                        ((VramSprite *)DB(i).primSlots[0])[4].sp.r0 = ((VramSprite *)DB(i).primSlots[0])[4].sp.g0 =
                            ((VramSprite *)DB(i).primSlots[0])[4].sp.b0 = SAI_UI.unk3CA;
                    }
                    SAI_setPortraitBrightness(0, 0x80);
                    if (SAI_STATE->unk48 == 0) {
                        SAI_STATE->unk48 = 1;
                        SAI_UI.unk3CC[2] = 0;
                        SAI_UI.unk3CC[0] = 0x80;
                        SAI_UI.unk3CA = 0x80;
                        spawnTask(0, -1, 0, 0x600, runWindowTask, &SAI_RIGHT_PORTRAIT_WINDOW_DEF, getCurrentTaskId(), 0, 0);
                        SAI_UI.unk3B4 = (JpWindow *)waitFrames(0x7FFFFFFF);
                    } else {
                        SAI_UI.unk3B4->state = 1;
                    }
                    break;
                case 2:
                    item = SAI_STATE->runner->script->params[0];
                    SAI_STATE->menuItems[(s8)SAI_STATE->menuCount++] = item;
                    addChoiceMenuItem(&SAI_UI.menu, item + 0x27, SAI_runMenu);
                    break;
                case 3:
                    /* the menu's icon: the script's, or -1 for the map's */
                    icon = SAI_STATE->runner->script->params[0];
                    if ((s16)SAI_STATE->runner->script->params[0] == -1) {
                        if (SAI_STATE->map >= 5) {
                            SAI_UI.unk3C8 = -((u8)SAI_STATE->map + 7);
                        } else {
                            SAI_UI.unk3C8 = -((u8)SAI_STATE->map + 6);
                        }
                    } else {
                        SAI_UI.unk3C8 = -(icon + 0x20);
                    }
                    openChoiceMenu(&SAI_UI.menu, (s8)SAI_UI.unk3C8, 0x32, 0, 0);
                    SAI_STATE->menuCount = 0;
                    break;
                case 4:
                    addChoiceMenuItem(&SAI_UI.menu, (s16)SAI_STATE->runner->script->params[0], SAI_runMenu);
                    break;
                case 5:
                    if (SAI_STATE->unk4B == 0x7F) {
                        SAI_STATE->unk4B = 1;
                        SAI_STATE->unk3C = 1;
                        SAI_STATE->unk47 = SAI_STATE->runner->script->params[0];
                        SAI_STATE->unk46 = SAI_STATE->runner->script->params[0];
                    } else {
                        SAI_STATE->unk4B = -1;
                        SAI_STATE->unk3C = 20;
                        SAI_STATE->unk47 = SAI_STATE->runner->script->params[0];
                    }
                    break;
                case 6:
                    SAI_UI.unk3C0 = (s16)SAI_STATE->runner->script->params[0];
                    return;
                case 7:
                    SAI_STATE->regs[1] = SAI_countSpareCopies(0, SAI_STATE->runner->script->params[0]);
                    break;
                case 8:
                    bits = PLAYER_DATA(0).bits += SAI_STATE->regs[12];
                    if (bits > 999998) {
                        bits = 999999;
                    }
                    PLAYER_DATA(0).bits = bits;
                    break;
                case 9:
                    arg = (s16)SAI_STATE->runner->script->params[0];
                    SAI_setPortraitBrightness(0, 0x80);
                    spawnTask(0, -1, 0, 0x400, SAI_showRightSprite, arg, getCurrentTaskId(), 0, 0);
                    break;
                case 10:
                    SAI_STATE->unk18 = 7;
                    PLAYER_DATA(0).area = SAI_STATE->runner->script->params[0];
                    SAI_STATE->unk42 = PLAYER_DATA(0).area;
                    return;
                case 11:
                    def = &SAI_PORTRAIT_SPRITE_DEFS[(s16)SAI_STATE->runner->script->params[0]];
                    for (i = 0; i < 2; i++) {
                        initVramSprite(DB(i).primSlots[0] + 0x40, 0x17, 0x4F, def->clut, def->colorMode, def->vramX, def->vramY, def->width, def->height, -1);
                        ((VramSprite *)DB(i).primSlots[0])[2].sp.r0 = ((VramSprite *)DB(i).primSlots[0])[2].sp.g0 =
                            ((VramSprite *)DB(i).primSlots[0])[2].sp.b0 = SAI_UI.unk3CB;
                    }
                    SAI_setPortraitBrightness(1, 0x80);
                    if (SAI_STATE->unk49 == 0) {
                        SAI_STATE->unk49 = 1;
                        SAI_UI.unk3CC[3] = 0;
                        SAI_UI.unk3CC[1] = 0x80;
                        SAI_UI.unk3CB = 0x80;
                        spawnTask(0, -1, 0, 0x600, runWindowTask, &SAI_LEFT_PORTRAIT_WINDOW_DEF, getCurrentTaskId(), 0, 0);
                        SAI_UI.unk3BC = (JpWindow *)waitFrames(0x7FFFFFFF);
                    } else {
                        SAI_UI.unk3BC->state = 1;
                    }
                    break;
                case 12:
                    arg = (s16)SAI_STATE->runner->script->params[0];
                    SAI_setPortraitBrightness(1, 0x80);
                    spawnTask(0, -1, 0, 0x400, SAI_showLeftSprite, arg, getCurrentTaskId(), 0, 0);
                    break;
                case 13:
                    SAI_STATE->regs[1] = SAI_countSpareCopies(1, SAI_STATE->runner->script->params[0]);
                    break;
                case 14:
                    addCardToCollection(SAI_STATE->regs[8], SAI_STATE->regs[7], SAI_STATE->regs[9]);
                    break;
                case 15:
                    removeCardFromCollection(SAI_STATE->regs[8], SAI_STATE->regs[7], SAI_STATE->regs[9]);
                    break;
                case 16:
                    playSoundEffect((s16)SAI_STATE->runner->script->params[0]);
                    break;
                case 17:
                    SAI_STATE->unk50 = SAI_STATE->runner->script->params[0];
                    if (SAI_STATE->unk50 == 2) {
                        SAI_STATE->unk50 = 0;
                        func_801EE058();
                    } else {
                        SAI_STATE->unk18 = 8;
                        result = 0;
                        if (SAI_STATE->unk50 != 1) {
                            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 4, 0);
                        }
                    }
                    break;
                }
                break;
            case 12:
                switch (SAI_STATE->runner->script->eventArg) {
                case 0:
                    SAI_loadCardImage(SAI_STATE->runner->script->params[0], (s16)SAI_STATE->runner->script->params[1]);
                    break;
                case 1:
                    if (SAI_STATE->unk45 == 0) {
                        SAI_STATE->unk45 = 1;
                    } else {
                        SAI_STATE->unk45 = 0;
                    }
                    SAI_STATE->musicTrack = SAI_STATE->runner->script->params[0];
                    SAI_STATE->musicVolume = SAI_STATE->runner->script->params[1];
                    if (SAI_STATE->musicVolume < 0) {
                        SAI_STATE->musicVolume = SAI_MUSIC_VOLUMES[SAI_STATE->musicTrack - 1];
                    }
                    playMusic(0, SAI_STATE->musicTrack, SAI_STATE->musicVolume);
                    break;
                }
                break;
            case 13:
                if (SAI_STATE->runner->script->eventArg == 0) {
                    ((SessionData *)SESSION_DATA)->stageId = SAI_STATE->runner->script->params[0];
                    ((SessionData *)SESSION_DATA)->stageMusic = SAI_STATE->runner->script->params[1];
                    ((SessionData *)SESSION_DATA)->duelMusic = SAI_STATE->runner->script->params[2];
                }
                break;
            }
        }
        clearScriptBusy(SAI_STATE->runner->script);
    } while (result != 0);
}


s32 SAI_stepAreaScript(void) {
    *SAI_STATE->runner->regs = 1;
    SAI_runAreaScript();
    return *SAI_STATE->runner->regs;
}
