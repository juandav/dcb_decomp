#include "common.h"
#include "game.h"
#include "dcb/kaw_tutorial.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/script.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/card_db.h"
#include "dcb/sound_play.h"
#include "dcb/duel.h"
#include "dcb/pad.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_hud.h"

extern UiWindow KAW_TUTORIAL_WINDOW;

s32 KAW_tickTutorial();

void KAW_startTutorial(void) {
    DUEL->tutorial = 1;
    KAW_DUEL->tutorialScript = allocTaskHeapBlock(sizeof(ScriptRunner));
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\BETA.MSD", getCurrentTaskId());
    KAW_DUEL->tutorialScript->data = (void *)waitFrames(0x7FFFFFFF);
    KAW_DUEL->tutorialScript->script = createScriptContext(KAW_DUEL->tutorialScript->data);
    KAW_DUEL->tutorialScript->regs = allocScriptRegisters(10);
    KAW_DUEL->tutorialScript->delay = 0;
    KAW_tickTutorial();
}

void KAW_freeTutorial(void) {
    if (DUEL->tutorial) {
        freeScriptContext(KAW_DUEL->tutorialScript->script, KAW_DUEL->tutorialScript->regs);
        freeHeapBlock(KAW_DUEL->tutorialScript->data);
        freeHeapBlock(KAW_DUEL->tutorialScript);
    }
}

void KAW_drawTutorialText(UiWindow *window) {
    drawText(window->originX, window->originY, KAW_DUEL->tutorialScript->text, 7, window->z);
}

s32 KAW_showTutorialMessage(s32 y, u8 *src) {
    Rect16 rect;
    u8 text[200];
    u8 *dst;
    s32 w;
    s32 h;
    s32 i;

    text[0] = '*';
    text[1] = 's';
    text[2] = '0';
    dst = &text[3];
    DUEL->awaitingInput = 0;
    do {
        if (*src < 0x81 || *src > 0x98) {
            if (src[0] == '*' && src[1] == 'p') {
                src += 2;
                *dst = 0;
                strcpy(dst, PLAYER(0)->name);
                dst += strlen(PLAYER(0)->name);
                continue;
            }
        } else {
            *dst++ = *src++;
        }
        *dst++ = *src++;
    } while (src[-1] != 0);
    KAW_DUEL->tutorialScript->text = (s32)text;
    measureText((u8 *)KAW_DUEL->tutorialScript->text);
    w = (TEXT_WIDTH + 1) / 2;
    rect.w = w * 2;
    h = (TEXT_HEIGHT + 1) / 2;
    rect.h = h * 2;
    rect.x = (320 - rect.w) / 2;
    rect.y = y - rect.h / 2;
    openWindow(&KAW_TUTORIAL_WINDOW, &rect, -1, (s16 *)-1, 8, 0x15, 0x80, 8);
    KAW_TUTORIAL_WINDOW.label = (s32)"TUTORIAL";
    KAW_TUTORIAL_WINDOW.palette = 4;
    playSoundEffect(0xA3);
    PAD_INPUT_ENABLED = 0;
    do {
        waitFrames(FRAME_INTERVAL);
    } while (drawWindow(&KAW_TUTORIAL_WINDOW, KAW_drawTutorialText, 0) == 0 || ((PAD_STATES[0]->rawPressed & PAD_CROSS) >> 6) == 0);
    playSoundEffect(0xA4);
    animateWindowTo(&KAW_TUTORIAL_WINDOW, (Rect16 *)-1);
    for (i = 0; i < 16; i++) {
        waitFrames(FRAME_INTERVAL);
        drawWindow(&KAW_TUTORIAL_WINDOW, KAW_drawTutorialText, 0);
    }
    PAD_INPUT_ENABLED = 0;
}

s32 KAW_tickTutorial(void) {
    s32 *vars;
    s32 result;
    s32 player;
    s32 slot;
    s32 id;

    if (KAW_DUEL->menuOpen != 0 || DUEL->tutorial == 0) {
        return;
    }
    if (KAW_DUEL->tutorialScript->delay != 0) {
        KAW_DUEL->tutorialScript->delay--;
        return;
    }
    vars = KAW_DUEL->tutorialScript->regs;
    vars[2] = DUEL->step;
    if (DUEL->cursorPlayer == 0) {
        vars[3] = DUEL->cursorSlot;
    } else {
        vars[3] = -1;
    }
    vars[4] = 0;
    vars[5] = 0;
    vars[6] = 0;
    vars[7] = 0;
    if (PAD_STATES[0]->pressed & PAD_CIRCLE) {
        vars[4] = 1;
    }
    if (PAD_STATES[0]->pressed & PAD_SQUARE) {
        vars[5] = 1;
    }
    if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
        vars[6] = 1;
    }
    if (PAD_STATES[0]->pressed & PAD_CROSS) {
        vars[7] = 1;
    }
    do {
        KAW_DUEL->tutorialBusy = 1;
        result = runScriptToNextEvent(KAW_DUEL->tutorialScript->script, vars);
        if (result == 1) {
            KAW_DUEL->cursorMode = vars[9];
            switch (KAW_DUEL->tutorialScript->script->eventOp) {
            case 10:
                switch (KAW_DUEL->tutorialScript->script->eventArg) {
                case 0:
                    KAW_showTutorialMessage(KAW_DUEL->tutorialScript->regs[8], (u8 *)KAW_DUEL->tutorialScript->regs[0]);
                    break;
                case 1:
                    KAW_DUEL->tutorialBusy = 0;
                    return;
                case 2:
                    break;
                case 3:
                    player = vars[1];
                    strcpy((char *)DUEL_PLAYERS[player] + 1, (char *)vars[0]);
                    linkDeckCardData(player, (PlayerDeck *)DUEL_PLAYERS[player]);
                    break;
                case 4:
                    KAW_DUEL->quit = 2;
                    KAW_DUEL->tutorialBusy = 0;
                    return;
                case 5:
                    DUEL->cursorSlot = -1;
                    ((CardCursor *)DUEL->cursor)->id = -1;
                    break;
                case 6:
                    KAW_openRing();
                    break;
                case 7:
                    PAD_INPUT_ENABLED = 1;
                    break;
                case 8:
                    PAD_INPUT_ENABLED = 0;
                    break;
                }
                break;
            case 11:
                switch (KAW_DUEL->tutorialScript->script->eventArg) {
                case 0:
                    waitFrames((s16)KAW_DUEL->tutorialScript->script->params[0]);
                    break;
                case 1:
                    DUEL->cpuResult = (s16)KAW_DUEL->tutorialScript->script->params[0];
                    break;
                case 2:
                    DUEL_MSG_BAR.next = KAW_DUEL->tutorialScript->script->params[0];
                    break;
                case 3:
                    playSoundEffect((s16)KAW_DUEL->tutorialScript->script->params[0]);
                    break;
                case 4:
                    KAW_DUEL->cursorMode = vars[9];
                    KAW_openCardSelect((s16)KAW_DUEL->tutorialScript->script->params[0]);
                    break;
                case 5:
                    vars[9] = -1;
                    KAW_DUEL->cursorMode = -1;
                    KAW_closeCardSelect((s16)KAW_DUEL->tutorialScript->script->params[0]);
                    break;
                case 6:
                    DUEL_MSG_BAR.playerLabel = KAW_DUEL->tutorialScript->script->params[0];
                    break;
                }
                break;
            case 12:
                switch (KAW_DUEL->tutorialScript->script->eventArg) {
                case 0:
                    DUEL_MSG_BAR.playerLabel = KAW_DUEL->tutorialScript->script->params[0];
                    DUEL_MSG_BAR.phase = KAW_DUEL->tutorialScript->script->params[1];
                    waitFrames(60);
                    break;
                case 1:
                    player = vars[1];
                    slot = (s16)KAW_DUEL->tutorialScript->script->params[0];
                    id = (s16)KAW_DUEL->tutorialScript->script->params[1];
                    PLAYER(player)->cards[slot].id = id;
                    if (id < 0xBF) {
                        PLAYER(player)->cards[slot].type = 0;
                        PLAYER(player)->cards[slot].index = id;
                    } else if (id < 0x125) {
                        PLAYER(player)->cards[slot].type = 1;
                        PLAYER(player)->cards[slot].index = id - 0xBF;
                    } else {
                        PLAYER(player)->cards[slot].type = 2;
                        PLAYER(player)->cards[slot].index = id - 0x125;
                    }
                    PLAYER(player)->unk0 = 1;
                    break;
                case 2:
                    DUEL_MSG_BAR.playerLabel = KAW_DUEL->tutorialScript->script->params[0];
                    DUEL_MSG_BAR.next2 = KAW_DUEL->tutorialScript->script->params[1];
                    break;
                case 3:
                    ((CardCursor *)DUEL->cursor)->id = KAW_DUEL->tutorialScript->script->params[0];
                    DUEL->cursorSlot = KAW_DUEL->tutorialScript->script->params[1];
                    if (((CardCursor *)DUEL->cursor)->id < 30) {
                        DUEL->cursorPlayer = 0;
                    } else {
                        DUEL->cursorPlayer = 1;
                    }
                    break;
                }
                break;
            case 13:
                if (KAW_DUEL->tutorialScript->script->eventArg == 0) {
                    KAW_closeRing((s16)KAW_DUEL->tutorialScript->script->params[0], (s16)KAW_DUEL->tutorialScript->script->params[1],
                                  (s16)KAW_DUEL->tutorialScript->script->params[2]);
                }
                break;
            }
        }
        clearScriptBusy(KAW_DUEL->tutorialScript->script);
    } while (result != 0);
    KAW_DUEL->tutorialBusy = 0;
}
