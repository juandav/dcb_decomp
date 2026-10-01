#include "dcb/duel_session.h"
#include "common.h"
#include "game.h"
#include "dcb/battle_hud.h"
#include "dcb/card_motion.h"
#include "dcb/card_render.h"
#include "dcb/card_db.h"
#include "dcb/duel_setup.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
#include "dcb/camera.h"
#include "dcb/fade.h"
#include "dcb/frame_callback.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/text.h"
#include "dcb/cpu_decision.h"
#include "dcb/hud_panels.h"
#include "dcb/duel.h"
#include "dcb/pad.h"

/* jp's duel session (duel_session.c is us's and eu's): the duel's message
   bar, its first turn's toss, the marks at the screen's side, and each
   deck's record */

/* the card_render functions jp's duel calls */
void drawButtonMarks(s32 x, s32 y, s32 value, s32 arg3);
void drawStepMark(s32 x, s32 y, s32 value, s32 arg3);
void drawTurnMark(s32 x, s32 y, s32 value, s32 arg3);
void drawPlayerMark(s32 x, s32 y, s32 value, s32 arg3);
s32 KAW_allocCardPolys(s32 count);
void KAW_freeCardPolys(void);
/* jp's cursor (us's KAW_createCursor, KAW_freeCursor and KAW_renderCursor) */
s32 KAW_createCursor(s32 mode, s32 x, s32 y, s32 d, s32 count);
void KAW_freeCursor(s32 cursor);
void KAW_renderCursor(u8 *cursor, s32 z);
/* KAWSEG's */
void KAW_runTurnOrderChoice(s32 mode, s32 deckId);
void KAW_runVersusIntro(s32 mode, s32 deckId);
void KAW_tickHelp(void);
void KAW_tickGiveUpPrompt(void);
void KAW_openTutorialMessage(void);
void KAW_runCardPrize(void);
void KAW_freeTutorial(void);
void KAW_runResultScreen();
extern s32 KAW_RESULT_SCREEN_STATE;

/* the message bar's messages: P0 the turn player's name, P1 the other's,
   P2 the winner's, P3 the loser's; W0 to W3 the same players' wins */
char *DUEL_MESSAGES[26] = {
    "P0のターン",
    "オプションカード選択中！",
    "手札はこのままでいい？",
    "全部捨てる事になります",
    "入れ替えます",
    "デジモンカードが手札にない",
    "戦うデジモン選択",
    "準備を終る？",
    "進化パワーため",
    "進化オプションカード選択",
    "進化しよう！",
    "進化を終る？",
    "相手のデジモンが居ません",
    "お互いの攻撃を選択",
    "決定！",
    "オプションカード選択",
    "それでいいの？",
    "P2の勝ち！",
    "P2のW2勝だよ！",
    "P2のW2勝だよ！",
    "戦闘だ！",
    "攻撃はどれかな？",
    "このカードでいいの？",
    "カードを出さなくていいの？",
    "カード援護効果発揮中！",
    "出せるデジモンカードがない！",
};

void initDuelState(s32 isCpuDuel) {
    void *block;
    s32 i;

    initHudPanels();
    CARD_ANIMS = block = allocTaskHeapBlock(CARD_ANIM_SIZE * 60);
    DUEL_STATE = block = allocTaskHeapBlock(0x484);
    DUEL->sprites = (CardSprite *)KAW_allocCardPolys(60);
    DUEL->turnPlayer = rand() % 2;
    DUEL->messageWanted = 0;
    DUEL->messageShown = 0;
    DUEL->step = 0;
    DUEL->cursorPlayer = 0;
    DUEL->cursorSlot = -1;
    DUEL->state = -1;
    DUEL->tutorial = 0;
    DUEL->helpOpen = 0;
    DUEL->quit = 0;
    DUEL->helpPage = 0;
    DUEL->showMarks = 0;
    DUEL->unk44A = -1;
    for (i = 0; i < 7; i++) {
        DUEL->marks[i].owner = -1;
        DUEL->marks[i].shown = 0;
        DUEL->marks[i].x = -0x50;
        DUEL->marks[i].y = 0;
    }
    initDuelPlayers(isCpuDuel);
    openKanjiPage(0xF, 0x1E3);
    DUEL->cursorMode = -1;
}

void spawnDuelTasks(s32 isCpuDuel) {
    s32 stageId;
    s32 music;

    initScene3D(0);
    spawnTask(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    waitFrames(2);
    ((Graphics *)&GRAPHICS)->rotX = 0;
    ((Graphics *)&GRAPHICS)->rotY = 0;
    ((Graphics *)&GRAPHICS)->rotZ = 0;
    ((Graphics *)&GRAPHICS)->posX = 0;
    ((Graphics *)&GRAPHICS)->posY = 0;
    ((Graphics *)&GRAPHICS)->posZ = 0;
    ((Graphics *)&GRAPHICS)->targetPitch = 0;
    ((Graphics *)&GRAPHICS)->targetDistance = 0x1C0;
    ((Graphics *)&GRAPHICS)->targetHeight = 0;
    ((Graphics *)&GRAPHICS)->targetYaw = 0;
    ((Graphics *)&GRAPHICS)->targetModel = -1;
    ((Graphics *)&GRAPHICS)->snapCamera = 1;
    DUEL->loadBusy = 0;
    waitFrames(2);
    DUEL->cursor = (u8 *)KAW_createCursor(0, 0x28, 0x30, 0xA, 1);
    if (DUEL->tutorial != 0) {
        spawnTask(0x1E, -1, 0, 0x800, &runTutorialTurnLoop, 0, 0, 0, 0);
    } else {
        spawnTask(0x1E, -1, 0, 0x800, &runDuelTurnLoop, 0, 0, 0, 0);
        if (isCpuDuel != 0) {
            spawnTask(0, -1, 0, 0x800, runCpuDecisionTask, 0, 0, 0, 0);
        }
    }
    spawnTask(0, -1, 0, 0x800, &runCardArtLoader, 0, 0, 0, 0);
    if (!((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation) {
        if (isCpuDuel != 0) {
            stageId = ((SessionData *)SESSION_DATA)->stageId;
            music = ((SessionData *)SESSION_DATA)->stageMusic;
        } else {
            stageId = -1;
            music = -1;
        }
        spawnTask(0, -1, 0, 0x1000, &runDuelStageTask, stageId, music, 0, 0);
    }
}

void teardownDuelScene(void) {
    endTask(0x19);
    freeHudPanels();
    KAW_freeCardPolys();
    freeHeapBlocksByTag(0x7F);
    closeKanjiPage(0xF);
}

/* draws the message bar's message into the shared panel, the players' names
   and wins put in */
void drawMessageBarPanel(s32 z) {
    char text[0x61];
    u8 *src;
    char *dst;
    s32 i;

    if (DUEL->messageShown != DUEL->messageWanted) {
        if (HUD_PANEL(22)->state == 3) {
            HUD_PANEL(22)->state = 6;
        }
        if ((u32)(HUD_PANEL(22)->state - 1) < 2) {
            DUEL->messageShown = DUEL->messageWanted;
        }
    }
    if (!(HUD_PANEL(22)->flags & 0x80)) {
        return;
    }
    src = (u8 *)DUEL_MESSAGES[DUEL->messageShown];
    dst = text;
    for (i = 0; i < 0x61; i++) {
        text[i] = 0;
    }
    while (*src != 0) {
        switch (*src) {
        case 'P':
            src++;
            switch (*src++) {
            case '0':
                strcat(dst, PLAYER(DUEL->turnPlayer)->name);
                dst += strlen(PLAYER(DUEL->turnPlayer)->name);
                break;
            case '1':
                strcat(dst, PLAYER((s8)(DUEL->turnPlayer ^ 1))->name);
                dst += strlen(PLAYER((s8)(DUEL->turnPlayer ^ 1))->name);
                break;
            case '2':
                strcat(dst, PLAYER(DUEL->winner)->name);
                dst += strlen(PLAYER(DUEL->winner)->name);
                break;
            case '3':
                strcat(dst, PLAYER(DUEL->winner ^ 1)->name);
                dst += strlen(PLAYER(DUEL->winner ^ 1)->name);
                break;
            }
            break;
        case 'W':
            src++;
            /* the wins as a full-width digit */
            switch (*src++) {
            case '0':
                *dst++ = 0x82;
                *dst++ = PLAYER(DUEL->turnPlayer)->wins + 0x4F;
                break;
            case '1':
                *dst++ = 0x82;
                *dst++ = PLAYER((s8)(DUEL->turnPlayer ^ 1))->wins + 0x4F;
                break;
            case '2':
                *dst++ = 0x82;
                *dst++ = PLAYER(DUEL->winner)->wins + 0x4F;
                break;
            case '3':
                *dst++ = 0x82;
                *dst++ = PLAYER(DUEL->winner ^ 1)->wins + 0x4F;
                break;
            }
            break;
        default:
            if ((u8)(*src - 0x81) >= 0x18) {
                *dst = *src;
            } else {
                /* the two bytes of a Shift-JIS character */
                *dst = *src;
                src++;
                dst++;
                *dst = *src;
            }
            src++;
            dst++;
            break;
        }
    }
    *dst = 0;
    drawIconText(HUD_PANEL(22)->sx + 0x41, HUD_PANEL(22)->sy + 0xB, 7, 1, z, (s32)text);
}

void runDuel(s32 isCpuDuel, s32 parent) {
    s32 fade;
    s32 timer;
    s32 i;
    s32 j;
    s32 winner;
    s8 c;

    initDuelState(isCpuDuel);
    KAW_runTurnOrderChoice(isCpuDuel, ((SessionData *)SESSION_DATA)->opponentDeckIndex);
    KAW_runVersusIntro(isCpuDuel, ((SessionData *)SESSION_DATA)->opponentDeckIndex);
    spawnDuelTasks(isCpuDuel);
    playLoadedMusic(0);
    fade = 0x80;
    timer = 0;
    while (1) {
        waitFrames(FRAME_INTERVAL);
        switch (DUEL->state) {
        case -1:
            timer = 0;
            clearBattleLog();
            DUEL->state++;
        case 0:
            if (timer >= 0x3D) {
                KAW_tickGiveUpPrompt();
                KAW_tickHelp();
                KAW_openTutorialMessage();
            } else {
                timer++;
            }
            break;
        case 1:
            HUD_PANEL(3)->state = 0xB;
            HUD_PANEL(14)->state = 0xB;
            HUD_PANEL(9)->state = 4;
            HUD_PANEL(20)->state = 4;
            HUD_PANEL(0)->state = 4;
            HUD_PANEL(11)->state = 4;
            HUD_PANEL(22)->state = 4;
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 3; j++) {
                    c = PLAYER(i)->digimonStack[j];
                    if (c != -1) {
                        SPRITE_KIND(c) = 0x1B;
                    }
                }
            }
            DUEL->state++;
            break;
        case 2:
            if (HUD_PANEL(3)->state == 0) {
                timer = 0;
                fade = 0x80;
                DUEL->state = 20;
            }
            break;
        case 20:
            fade -= 4;
            if (fade < 0) {
                fade = 0;
            }
            if (timer++ >= 10) {
                createWireGrid(400, 600, 9, 13, 0, 1);
                addFrameCallback((s32)renderWireGrid);
                timer = 0;
                DUEL->state++;
            }
            break;
        case 21:
            fade -= 4;
            if (fade < 0) {
                fade = 0;
            }
            if (fade == 0) {
                DB(0).scenePackets = allocHeapBlock(0xBB80, 0x7F);
                DB(1).scenePackets = allocHeapBlock(0xBB80, 0x7F);
                showArenaStage(0x400);
                addFrameCallback((s32)renderSceneModels);
                SCENE_3D->modelState[0x17] = 1;
                timer = 0;
                DUEL->state++;
            }
            break;
        case 22:
            fade -= 4;
            if (fade < 0) {
                fade = 0;
            }
            ((Graphics *)&GRAPHICS)->targetDistance += 4;
            ((Graphics *)&GRAPHICS)->targetPitch -= 10;
            ((Graphics *)&GRAPHICS)->rotZ += 6;
            if (timer++ >= 60) {
                for (i = 0; i < 2; i++) {
                    for (j = 0; j < 3; j++) {
                        c = PLAYER(i)->digimonStack[j];
                        if (c != -1) {
                            SPRITE_KIND(c) = 0x1E;
                        }
                    }
                }
                timer = 0;
                DUEL->state++;
            }
            break;
        case 23:
            ((Graphics *)&GRAPHICS)->rotZ += 8;
            if (++timer == 60) {
                spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 1, 6, 0);
            }
            if (timer == 120) {
                removeFrameCallback((s32)renderWireGrid);
            }
            if (timer >= 122) {
                timer = 0;
                DUEL->state = 3;
                freeWireGrid();
                ((Graphics *)&GRAPHICS)->targetDistance = 0x1C0;
                ((Graphics *)&GRAPHICS)->targetPitch = 0;
                ((Graphics *)&GRAPHICS)->rotZ = 0;
            }
            break;
        case 4:
            DUEL->state++;
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 4, 0);
            break;
        case 6:
            stopScreenFade();
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 3; j++) {
                    c = PLAYER(i)->digimonStack[j];
                    if (c != -1) {
                        SPRITE_KIND(c) = 0xD;
                    }
                }
            }
            HUD_PANEL(3)->state = 0xD;
            HUD_PANEL(14)->state = 0xD;
            HUD_PANEL(0)->state = 1;
            HUD_PANEL(11)->state = 1;
            HUD_PANEL(22)->state = 1;
            DUEL->state++;
            break;
        case 7:
            if (fade != 0x80) {
                fade += 4;
                if (fade > 0x80) {
                    fade = 0x80;
                }
            } else if (HUD_PANEL(3)->state == 4) {
                DUEL->state = -1;
            }
            break;
        }
        if (fade != 0) {
            renderDuelBackground(fade);
        }
        /* the polygon battle has the screen */
        if (DUEL->state >= 3 && DUEL->state < 7) {
            continue;
        }
        /* i: the pad that toggles the marks; j: the marks' y */
        i = 0;
        if (DUEL->humanPlayer == 1) {
            i = 1;
            j = 0x96;
        } else {
            j = 0xC;
        }
        if (PAD_STATES[i]->rawPressed & (PAD_L1 | PAD_R1)) {
            DUEL->showMarks ^= 1;
        }
        if ((s8)DUEL->showMarks == 1) {
            for (i = 0; i < 7; i++) {
                DUEL->marks[i].shown = 1;
            }
        }
        drawPlayerMark(8, j + 0x10, DUEL->humanPlayer, 1);
        if (DUEL->marks[1].owner != DUEL->humanPlayer) {
            for (i = 0; i < 7; i++) {
                DUEL->marks[i].shown = DUEL->marks[1].shown;
            }
        } else if (DUEL->marks[2].owner != DUEL->unk482) {
            for (i = 1; i < 7; i++) {
                DUEL->marks[i].shown = DUEL->marks[2].shown;
            }
        }
        drawStepMark(8, j, DUEL->unk47F, 1);
        drawTurnMark(8, j + 0x10, DUEL->unk482, 1);
        drawButtonMarks(8, j + 0x1E, DUEL->unk481, 1);
        tickBattleHud();
        renderBoardCards();
        if (DUEL->cursorSlot >= 0) {
            ((CardCursor *)DUEL->cursor)->sprite = SPRITE(((CardCursor *)DUEL->cursor)->id);
            KAW_renderCursor(DUEL->cursor, 0x67);
        }
        if (DUEL->step == 0x34) {
            winner = DUEL->winner;
            break;
        }
        if (DUEL->quit >= 2) {
            winner = DUEL->quit - 2;
            break;
        }
    }
    DUEL->stopTurnLoop = -1;
    DUEL->stopArtLoader = -1;
    if (DUEL->tutorial == 0 && isCpuDuel != 0) {
        DUEL->stopCpuTask = -1;
    }
    if (!((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation) {
        DUEL->stopStageTask = -1;
        while (DUEL->stopStageTask | DUEL->stopArtLoader) {
            waitFrames(FRAME_INTERVAL);
            renderDuelBackground(0x80);
            tickBattleHud();
            renderBoardCards();
        }
    }
    spawnTask(0, -1, 0, 0x800, KAW_runResultScreen, isCpuDuel, winner, ((SessionData *)SESSION_DATA)->opponentDeckIndex, 0);
    KAW_RESULT_SCREEN_STATE = -1;
    stopMusic();
    while (KAW_RESULT_SCREEN_STATE != 0) {
        waitFrames(FRAME_INTERVAL);
        if (KAW_RESULT_SCREEN_STATE == -1) {
            renderDuelBackground(0x80);
            tickBattleHud();
            renderBoardCards();
        }
    }
    while (DUEL->stopTurnLoop != 0) {
        waitFrames(FRAME_INTERVAL);
    }
    if (DUEL->tutorial == 0 && isCpuDuel != 0) {
        while (DUEL->stopCpuTask != 0) {
            waitFrames(FRAME_INTERVAL);
        }
    }
    KAW_freeCursor((s32)DUEL->cursor);
    if (DUEL->tutorial == 0) {
        if (winner == 0 && isCpuDuel != 0) {
            KAW_runCardPrize();
        }
        if (DUEL->tutorial == 0) {
            /* each deck's record, and the hall of fame's deck */
            for (i = 0; i < 2; i++) {
                if (i == winner) {
                    if (PLAYER(winner)->deck->wins != 999) {
                        PLAYER(winner)->deck->wins++;
                    }
                } else if (PLAYER(i)->deck->losses != 999) {
                    PLAYER(i)->deck->losses++;
                }
            }
            if (PLAYER_DATA(0).hallOfFameDeck.wins < PLAYER(0)->deck->wins) {
                PLAYER_DATA(0).hallOfFameDeck = *PLAYER(0)->deck;
            }
        }
    }
    teardownDuelScene();
    KAW_freeTutorial();
    waitFrames(4);
    stopMusic();
    resumeTask(parent, winner);
}
