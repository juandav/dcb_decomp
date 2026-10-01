#include "common.h"
#include "game.h"
#include "dcb/task.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/pad.h"
#include "dcb/nisseg.h"

/* VS mode's deck choice: each player picks one of their three saved decks
   with Triangle, Circle or Cross, or a random one with Square, in their own
   window; then the duel starts */

void runVersusDuel();

s32 NIS_chooseVsDecks(void);
void NIS_drawDeckChoice1(NisWindow *window);
void NIS_drawDeckChoice2(NisWindow *window);

void NIS_closeDeckChoice(NisWindow *window) {
}

/* each player's window: "<Triangle> <name> deck" (or "none") for the three
   saved decks, "<Square> random" and "each button chooses". Matching them
   needs the x position allocated before the colour's address, and no
   form of the C tried (nor the permuter) gets GCC to do that */
INCLUDE_ASM("nisseg/nonmatchings/vs/nis_vs_deck_select", NIS_drawDeckChoice1);

INCLUDE_ASM("nisseg/nonmatchings/vs/nis_vs_deck_select", NIS_drawDeckChoice2);

/* what the deck choice windows draw (defined after them, so that GCC
   switches back to .rodata after the asm) */
const u8 NIS_DECK_CHOICE_RGB[4] = { 0x80, 0x80, 0x80 };
const char NIS_FMT_TRIANGLE_DECK[] = "b1%sデック";
const char NIS_STR_TRIANGLE_NONE[] = "b1無し";
const char NIS_FMT_CIRCLE_DECK[] = "b0%sデック";
const char NIS_STR_CIRCLE_NONE[] = "b0無し";
const char NIS_FMT_CROSS_DECK[] = "b2%sデック";
const char NIS_STR_CROSS_NONE[] = "b2無し";
const char NIS_STR_SQUARE_RANDOM[] = "b3ランダム";
const char NIS_STR_CHOOSE_BY_BUTTON[] = "　各ボタンで決定";

void NIS_startVsDeckSelect(void) {
    NIS_chooseVsDecks();
    spawnTask(0, -1, 0, 0x1000, runVersusDuel, 0);
    exitTask();
}

s32 NIS_chooseVsDecks(void) {
    NisMenu menu;
    NisWindow *windows[2];
    u8 pads[2];
    u8 deckCounts[2];
    u8 done[2] = { 0 };
    NisWindowDef defs[2] = {
        { { 0x17, 0x40, 0, 0xC }, { 0x17, 0x40, 0x7E, 0x5F }, 0xA, 0, NIS_drawDeckChoice1, NIS_closeDeckChoice },
        { { 0x122, 0x40, 0, 0xC }, { 0xA4, 0x40, 0x7E, 0x5F }, 0xA, 1, NIS_drawDeckChoice2, NIS_closeDeckChoice },
    };
    s16 player;
    s16 i;
    s32 deck;

    openKanjiPage(0xF, 0x1E3);
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &defs[0], getCurrentTaskId());
    windows[0] = (NisWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &defs[1], getCurrentTaskId());
    windows[1] = (NisWindow *)waitFrames(0x7FFFFFFF);
    pads[0] = 0;
    pads[1] = 1;
    for (player = 0; player < 2; player++) {
        for (i = 0; i < 3; i++) {
            if (!((NisProfile *)PLAYER_PROFILES)[player].savedDecks[i].inUse) {
                break;
            }
        }
        deckCounts[player] = i;
    }
    openChoiceMenu(&menu, 0x3B, 0x32, 0, 0);
    do {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        for (i = 0; i < 2; i++) {
            if (done[i]) {
                continue;
            }
            if (PAD_STATES[pads[i]]->rawPressed & PAD_TRIANGLE) {
                if (((NisProfile *)PLAYER_PROFILES)[i].savedDecks[0].inUse) {
                    playSoundEffect(0);
                    windows[i]->state = 4;
                    NIS_STATE->deckChoice[i] = 0;
                    done[i] = 1;
                } else {
                    playSoundEffect(1);
                }
            } else if (PAD_STATES[pads[i]]->rawPressed & PAD_CIRCLE) {
                if (((NisProfile *)PLAYER_PROFILES)[i].savedDecks[1].inUse) {
                    playSoundEffect(0);
                    windows[i]->state = 4;
                    NIS_STATE->deckChoice[i] = 1;
                    done[i] = 1;
                } else {
                    playSoundEffect(1);
                }
            } else if (PAD_STATES[pads[i]]->rawPressed & PAD_CROSS) {
                if (((NisProfile *)PLAYER_PROFILES)[i].savedDecks[2].inUse) {
                    playSoundEffect(0);
                    windows[i]->state = 4;
                    NIS_STATE->deckChoice[i] = 2;
                    done[i] = 1;
                } else {
                    playSoundEffect(1);
                }
            } else if (PAD_STATES[pads[i]]->rawPressed & PAD_SQUARE) {
                playSoundEffect(0);
                windows[i]->state = 4;
                deck = rand() % deckCounts[i];
                NIS_STATE->deckChoice[i] = deck;
                done[i] = 1;
            }
        }
    } while (done[0] != 1 || done[1] != 1);
    SCROLLING_BACKGROUND->unk1BE = 2;
    waitFrames(0x1E);
    closeKanjiPage(0xF);
    return 0;
}

#if JP_DEBUG_BUILD
/* the debug menu's "MODEL VIEW": the model viewer on Digimon 0x73, which
   Cross starts and Cross leaves */
void NIS_testModelViewer(s32 arg0, s32 parentTask) {
    loadScrollingBackground(0x50, 0);
    showScrollingBackground();
    NIS_openViewerScene();
    do {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
    } while (!(PAD_STATES[0]->rawRepeat & PAD_CROSS));
    NIS_runModelViewer(0x73);
    do {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
    } while (!(PAD_STATES[0]->rawRepeat & PAD_CROSS));
    NIS_closeViewerScene();
    freeScrollingBackground();
    resumeTask(parentTask);
}
#endif
