#include "common.h"
#include "game.h"
#include "dcb/task.h"
#include "dcb/pad.h"
#include "dcb/nisseg.h"

/* The debug build's DECK EDIT menu, which Sugano's menu opens (only eu's
   NISSEG has it): a random deck, the deck screens' windows and scene, the
   title's background and a menu of choices, each tried on its own */

extern NisWindowTest NIS_WINDOW_TESTS[4];

void func_801FFBE0(); /* a task the MODEL TEST spawns, outside NISSEG */

void NIS_testModelViewer(s32 arg0, s32 parentTask);
void NIS_runMainMenu();
s32 NIS_loadDeckTims(void);
void NIS_writeDeckName(s32 deck);
void NIS_writeCardDetails(s32 type, s32 index);
void NIS_openDeckScene(s32 modelId);
void NIS_closeDeckScene(void);

void func_801F8BE0(NisDebugMenu *menu, s32 parentTask);
void func_801F8C38(NisDebugMenu *menu, s32 parentTask);
void func_801F8CB8(NisDebugMenu *menu, s32 parentTask);
void func_801F8E3C(NisDebugMenu *menu, s32 parentTask);
void func_801F8E84(NisDebugMenu *menu, s32 parentTask);
void func_801F8F44(NisDebugMenu *menu, s32 parentTask);

extern NisDebugMenu D_801F9328;

/* each item's value is the deck, the card, the model or the window to try */
NisDebugMenuItem D_801F9228[8] = {
    { func_801F8BE0, &D_801F9328, 0x400, 0, 0, 2, 1, "RANDOM DECK" },
    { func_801F8C18, &D_801F9328, 0x1000, 0, 0, 2, 1, "DECK DISP" },
    { func_801F8C38, &D_801F9328, 0x1000, 0, 0, 0x1D, 1, "DECK DISP" },
    { func_801F8CB8, &D_801F9328, 0x800, 0, 0, 0xC8, 1, "MODEL TEST" },
    { func_801F8E3C, &D_801F9328, 0x800, 0, 0, 0, 0, "TITLE TEST" },
    { func_801F8E84, &D_801F9328, 0x800, 0, 0, 0, 0, "CMDE TEST" },
    { func_801F8F44, &D_801F9328, 0x800, 0, 0, 0x63, 1, "WINDOW TEST" },
    { NIS_runMainMenu, NULL, 0x800, 0, 0, 0, 0, "GAME START" },
};
NisDebugMenu D_801F9328 = { D_801F9228, 0, NIS_loadDeckTims, 0, 0x18, 0x28, 8, 8, { 0 }, "DECK EDIT" };

/* Sugano's menu: the deck menu above ("deck editing") and the model viewer */
NisDebugMenuItem D_801F9350[2] = {
    { func_8002D15C, &D_801F9328, 0x400, 0, 0, 0, 0, "ﾃﾞｯｷﾍﾝｼｭｳ" },
    { NIS_testModelViewer, NULL, 0x400, 0, 0, 0, 0, "MODEL VIEW" },
};
NisDebugMenu NIS_SUGANO_MENU = { D_801F9350, 0, NULL, 0, 0x10, 0x20, 2, 2, { 0 }, "SUGMENU" };

/* fills saved deck `slot` of player 1 with 30 random cards, named "random",
   and gives the player a copy of each */
void func_801F89C4(s32 slot) {
    /* "ランダム" padded with two ideographic spaces, and the leftover bytes
       after it */
    static const char name[16] = "ランダム　　\0\0\a.";
    NisDeck *deck;
    s32 i;

    deck = &NIS_PROFILE(0)->savedDecks[slot];
    sprintf(deck->name, name);
    for (i = 0; i < 30; i++) {
        deck->cards[i].type = rand() % 3;
        switch (deck->cards[i].type) {
        case 0:
            deck->cards[i].index = rand() % NIS_DIGIMON_COUNT;
            NIS_PROFILE(0)->digimonCards[deck->cards[i].index]++;
            break;
        case 1:
            deck->cards[i].index = rand() % NIS_OPTION_COUNT;
            NIS_PROFILE(0)->optionCards[deck->cards[i].index]++;
            break;
        case 2:
            deck->cards[i].index = rand() % NIS_OTHER_COUNT;
            NIS_PROFILE(0)->otherCards[deck->cards[i].index]++;
            break;
        }
    }
    deck->inUse = 1;
}

/* RANDOM DECK: a random deck in the chosen slot */
void func_801F8BE0(NisDebugMenu *menu, s32 parentTask) {
    func_801F89C4(menu->items[0].value);
    resumeTask(parentTask);
}

/* the first DECK DISP, which only picks the deck for the second, and the
   WINDOW TEST of the deck summary: nothing to do */
void func_801F8C18(NisDebugMenu *menu, s32 parentTask) {
    resumeTask(parentTask);
}

/* the second DECK DISP: the chosen card of the chosen deck in the card
   details */
void func_801F8C38(NisDebugMenu *menu, s32 parentTask) {
    NisDeck *deck;

    deck = &NIS_PROFILE(0)->savedDecks[menu->items[1].value];
    NIS_writeCardDetails(deck->cards[menu->items[2].value].type, deck->cards[menu->items[2].value].index);
    resumeTask(parentTask);
}

/* MODEL TEST: the deck screens' scene with the chosen model, until the task
   it starts resumes this one */
void func_801F8CB8(NisDebugMenu *menu, s32 parentTask) {
    NIS_openDeckScene(menu->items[3].value);
    spawnTask(0, -1, 0, 0x1000, func_801FFBE0, 0, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    NIS_closeDeckScene();
    resumeTask(parentTask);
}

/* the WINDOW TEST of the card details: the chosen card of the chosen deck,
   and its model if it is a Digimon */
void func_801F8D34(NisDebugMenu *menu, s32 parentTask) {
    NisDeck *deck;

    deck = &NIS_PROFILE(0)->savedDecks[menu->items[1].value];
    NIS_writeCardDetails(deck->cards[menu->items[2].value].type, deck->cards[menu->items[2].value].index);
    if (NIS_DECK_EDIT.cardType == 0) {
        NIS_openDeckScene(NIS_DIGIMON_CARDS[deck->cards[menu->items[2].value].index].unkD3);
    }
}

/* the WINDOW TEST of the deck name: the chosen deck's */
void func_801F8E10(NisDebugMenu *menu, s32 parentTask) {
    NIS_writeDeckName(menu->items[1].value);
}

/* TITLE TEST: the title's scrolling background, until something resumes
   this task */
void func_801F8E3C(NisDebugMenu *menu, s32 parentTask) {
    loadScrollingBackground(8, 0);
    showScrollingBackground();
    waitFrames(0x7FFFFFFF);
    resumeTask(parentTask);
}

/* CMDE TEST: a menu of four choices that do nothing */
void func_801F8E84(NisDebugMenu *menu, s32 parentTask) {
    NisMenu choices;

    D_801DEBF0 = 0;
    openChoiceMenu(&choices, -1, 0x2C, NULL, NULL);
    addChoiceMenuItem(&choices, 0, NULL);
    addChoiceMenuItem(&choices, 1, NULL);
    addChoiceMenuItem(&choices, 2, NULL);
    addChoiceMenuItem(&choices, 3, NULL);
    do {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
    } while (!runChoiceMenu(&choices));
    startChoiceMenuAction(&choices);
    resumeTask(parentTask);
}

/* WINDOW TEST: the chosen window of the deck screens, which Square, Triangle
   and Cross pass a state to; Cross closes it */
void func_801F8F44(NisDebugMenu *menu, s32 parentTask) {
    NisWindowTest *test;
    NisWindow *window;

    test = &NIS_WINDOW_TESTS[menu->items[6].value];
    D_801DEBF0 = 0;
    if (test->open != NULL) {
        test->open(menu, getCurrentTaskId());
    }
    spawnTask(0, -1, 0, 0x1000, runWindowTask, test->window, getCurrentTaskId());
    window = (NisWindow *)waitFrames(0x7FFFFFFF);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (PAD_STATES[0]->rawPressed & PAD_SQUARE) {
            window->state = 1;
        }
        if (PAD_STATES[0]->rawPressed & PAD_TRIANGLE) {
            window->state = 2;
        }
        if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
            window->state = 3;
            break;
        }
    }
    if (test->close != NULL) {
        test->close();
    }
    D_801DEBF0 = 1;
    resumeTask(parentTask);
}
