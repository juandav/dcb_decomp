#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/card_db.h"
#include "dcb/vram_upload.h"
#include "dcb/sound_play.h"
#include "dcb/prim_util.h"
#include "dcb/pad.h"
#include "dcb/sound.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_hand.h"
#include "dcb/kaw_match_intro.h"

/* jp's result screen (kaw_result.c is us's and eu's): its eleven TIMs come
   from B:\WINLOSE, its winner's card image is drawn into a polygon of the
   overlay's own, a duel against the CPU shows only the player's record, and
   the screen ends its own task */

extern s32 KAW_RESULT_SCREEN_STATE;
extern POLY_FT4 KAW_RESULT_WINNER_POLYS[2];

void startAreaPakLoad(void);

void KAW_runResultScreen(s32 mode, s32 winner, s32 deckId) {
    char path[64];
    s32 wins[2];
    s32 losses[2];
    s32 nameWidth0;
    s32 nameWidth1;
    s32 scale;
    u32 *arc;
    s32 i;
    s32 frame;
    POLY_FT4 *poly;

    if (mode == 0) {
        deckId = 999;
    }
    sprintf(path, "B:\\WINLOSE\\%3.3d.ARC", deckId);
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < 11; i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
    }
    DrawSync(0);
    freeHeapBlock(arc);
    if (mode != 0) {
        if (winner != 0) {
            loadMusicTrack(0, 0x31, 0x7F);
        } else {
            loadMusicTrack(0, 0x30, 0x7F);
        }
        startAreaPakLoad();
    } else {
        loadMusicTrack(0, 0x30, 0x7F);
    }
    playLoadedMusic(0);
    nameWidth0 = PLAYER(0)->nameWidth;
    nameWidth1 = PLAYER(1)->nameWidth;
    KAW_VS_PANEL_POS[0][0] = 0x140;
    KAW_VS_PANEL_POS[0][1] = 0x78;
    KAW_VS_PANEL_POS[0][2] = 0;
    KAW_VS_PANEL_POS[0][3] = 0x78;
    KAW_VS_PANEL_POS[1][0] = -0x140;
    KAW_VS_PANEL_POS[1][1] = 0;
    KAW_VS_PANEL_POS[1][2] = 0;
    KAW_VS_PANEL_POS[1][3] = 0;
    KAW_VS_NAME_POS[0][0] = 0x140;
    KAW_VS_NAME_POS[0][1] = 0xC3;
    KAW_VS_NAME_POS[1][0] = -nameWidth1;
    KAW_VS_NAME_POS[1][1] = 0x10;
    KAW_VS_NAME_POS[1][2] = 0x138 - nameWidth1;
    KAW_VS_INNER_LINE_POS[0][0] = 0x140;
    KAW_VS_INNER_LINE_POS[0][1] = 0x9F;
    KAW_VS_INNER_LINE_POS[1][0] = -0x88;
    KAW_VS_INNER_LINE_POS[1][1] = 0x42;
    KAW_VS_OUTER_LINE_POS[0][0] = 0x140;
    KAW_VS_OUTER_LINE_POS[0][1] = 0xB1;
    KAW_VS_OUTER_LINE_POS[1][0] = -0x88;
    KAW_VS_OUTER_LINE_POS[1][1] = 0x30;
    frame = 0;
    scale = 200;
    for (i = 0; i < 2; i++) {
        if (mode != 0) {
            if (winner == i) {
                wins[i] = ((ProfileK *)PLAYER_PROFILES)[i].battleWins;
                if (!KAW_DUEL->tutorial) {
                    wins[i]++;
                }
                losses[i] = ((ProfileK *)PLAYER_PROFILES)[i].battleLosses;
            } else {
                wins[i] = ((ProfileK *)PLAYER_PROFILES)[i].battleWins;
                losses[i] = ((ProfileK *)PLAYER_PROFILES)[i].battleLosses;
                if (!KAW_DUEL->tutorial) {
                    losses[i]++;
                }
            }
        } else {
            if (winner == i) {
                wins[i] = ((ProfileK *)PLAYER_PROFILES)[i].versusWins;
                if (!KAW_DUEL->tutorial) {
                    wins[i]++;
                }
                losses[i] = ((ProfileK *)PLAYER_PROFILES)[i].versusLosses;
            } else {
                wins[i] = ((ProfileK *)PLAYER_PROFILES)[i].versusWins;
                losses[i] = ((ProfileK *)PLAYER_PROFILES)[i].versusLosses;
                if (!KAW_DUEL->tutorial) {
                    losses[i]++;
                }
            }
        }
        if (wins[i] >= 1000) {
            wins[i] = 999;
        }
        if (losses[i] >= 1000) {
            losses[i] = 999;
        }
    }
    do {
        frame++;
        waitFrames(FRAME_INTERVAL);
        if (frame >= 56) {
            scale -= 8;
            if (scale < 100) {
                scale = 100;
            }
            initPrimByType(0xC, &KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX], 0, 0);
            setRGB0(&KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX], 0x80, 0x80, 0x80);
            if (winner == 0) {
                setXYWH(&KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX], KAW_VS_PANEL_POS[1][0] - (s16)(scale * 64 / 100 - 74), KAW_VS_PANEL_POS[1][1] - (s16)(scale * 56 / 100 - 60),
                        scale * 128 / 100, scale * 112 / 100);
            } else {
                setXYWH(&KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX], KAW_VS_PANEL_POS[0][0] - (s16)(scale * 64 / 100 - 248), KAW_VS_PANEL_POS[0][1] - (s16)(scale * 56 / 100 - 60),
                        scale * 128 / 100, scale * 112 / 100);
            }
            KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX].u0 = 0;
            KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX].v0 = 0;
            KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX].u1 = 0x80;
            KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX].v1 = 0;
            KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX].u2 = 0;
            KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX].v2 = 0x70;
            KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX].u3 = 0x80;
            KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX].v3 = 0x70;
            poly = &KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX];
            poly->tpage = 8;
            poly->clut = 0x3E98;
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], poly);
        }
        for (i = 0; i < 2; i++) {
            if (frame > 0) {
                STEP_TOWARD(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][2], 16);
            }
            if (frame > 20) {
                STEP_TOWARD(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][2], 24);
            }
            if (frame > 30) {
                STEP_TOWARD(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][2], 24);
            }
            if (frame > 40) {
                STEP_TOWARD(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][2], 24);
            }
        }
        if (frame == 26) {
            playSoundEffect(0x8D);
            playSoundEffect(0x8D);
        }
        if (frame == 30) {
            playSoundEffect(0xA7);
        }
        if (frame == 40) {
            playSoundEffect(0xA7);
        }
        if (frame == 50) {
            playSoundEffect(0xA7);
        }
        if (frame == 70) {
            playSoundEffect(0x83);
        }
        i = 0;
        KAW_drawSprite(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][1], 0x2C0, 0x1C0, nameWidth0, 0x20, 0x2F0, 0x1D7, 0, 1, 0, 0x80, 0);
        KAW_drawDeckName(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][1], (char *)PLAYER(i)->deck + 1);
        KAW_drawBattleRecord(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][1], wins[i], losses[i]);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0xB8, KAW_VS_PANEL_POS[i][1] + 4, 0x140, 0, 0x80, 0x70, 0x140, 0xFE, 1, 0, 0, 0x80, 4);
        if (winner == 0) {
            KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 8, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0, 0xA0, 0x5C, 0x180, 0xF8, 0, 0, 0, 0x80, 4);
        } else {
            KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 8, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0x5C, 0xA0, 0x5C, 0x180, 0xFC, 0, 0, 0, 0x80, 4);
        }
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][1], 0x180, 0x78, 0x100, 0x78, 0x180, 0xF9, 0, 0, 0, 0x80, 4);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x100, KAW_VS_PANEL_POS[i][1], 0x1C0, 0x78, 0x40, 0x78, 0x180, 0xF9, 0, 0, 0, 0x80, 4);
        i = 1;
        KAW_drawSprite(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][1], 0x2C0, 0x1E0, nameWidth1, 0x20, 0x2F0, 0x1D8, 0, 1, 0, 0x80, 0);
        KAW_drawDeckName(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][1], (char *)PLAYER(i)->deck + 1);
        if (mode == 0) {
            KAW_drawBattleRecord(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][1], wins[i], losses[i]);
        }
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 8, KAW_VS_PANEL_POS[i][1] + 4, 0x140, 0x70, 0x80, 0x70, 0x140, 0xFF, 1, 0, 0, 0x80, 4);
        if (winner != 0) {
            KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x96, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0, 0xA0, 0x5C, 0x180, 0xF8, 0, 0, 0, 0x80, 4);
        } else {
            KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x96, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0x5C, 0xA0, 0x5C, 0x180, 0xFC, 0, 0, 0, 0x80, 4);
        }
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][1], 0x180, 0, 0x100, 0x78, 0x180, 0xFB, 0, 0, 0, 0x80, 4);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x100, KAW_VS_PANEL_POS[i][1], 0x1C0, 0, 0x40, 0x78, 0x180, 0xFB, 0, 0, 0, 0x80, 4);
        if (frame > 30) {
            KAW_RESULT_SCREEN_STATE = 1;
        }
    } while (frame <= 100 || !(PAD_STATES[0]->rawPressed & PAD_CIRCLE));
    waitFrames(10);
    KAW_RESULT_SCREEN_STATE = 0;
    exitTask();
}
