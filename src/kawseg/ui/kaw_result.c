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

extern s32 KAW_RESULT_SCREEN_STATE;

/* the last three bytes are leftovers in the original, not zero padding,
   and not the same in every version */
#if VERSION_US
const char KAW_FMT_WIN_ARC_PATH[20] = "B:\\WIN\\%3.3d.ARC\0\x02\x24\x41";
#elif VERSION_EU
const char KAW_FMT_WIN_ARC_PATH[20] = "B:\\WIN\\%3.3d.ARC\0\0\0\x02";
#else
#error "kawseg/ui/kaw_result: version not checked"
#endif

void KAW_runResultScreen(s32 mode, s32 winner, s32 deckId) {
    char path[64];
    s32 scale;
    u32 *arc;
    s32 i;
    s32 frame;
    VersusPrims *prims;

    KAW_RESULT_SCREEN_STATE = -1;
    if (mode == 0) {
        deckId = 999;
    }
    sprintf(path, KAW_FMT_WIN_ARC_PATH, deckId);
    i = 0;
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        waitFrames(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    KAW_MATCH_SCREEN = allocTaskHeapBlock(sizeof(DeckScreen));
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[15] = (s32)&KAW_MATCH_SCREEN->prims[i];
    }
    KAW_MATCH_SCREEN->mode = mode;
    KAW_MATCH_SCREEN->deckId = deckId;
    if (mode != 0) {
        i = getBaseDeckId(deckId);
        ((PlayerProfile *)PLAYER_PROFILES)[1].battleWins = ((PlayerProfile *)PLAYER_PROFILES)->comLosses[i];
        ((PlayerProfile *)PLAYER_PROFILES)[1].battleLosses = ((PlayerProfile *)PLAYER_PROFILES)->comWins[i];
        if (!KAW_DUEL->tutorial && winner != 0) {
            loadMusicTrack(0, 0x96, 0x7F);
        } else {
            loadMusicTrack(0, 0x95, 0x7F);
        }
    } else {
        loadMusicTrack(0, 0x95, 0x7F);
    }
    playLoadedMusic(0);
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
    KAW_VS_NAME_POS[1][0] = -PLAYER(1)->nameWidth;
    KAW_VS_NAME_POS[1][1] = 0x10;
    KAW_VS_NAME_POS[1][2] = 0x138 - PLAYER(1)->nameWidth;
    KAW_VS_INNER_LINE_POS[0][0] = 0x140;
    KAW_VS_INNER_LINE_POS[0][1] = 0x9F;
    KAW_VS_INNER_LINE_POS[1][0] = -0xC0;
    KAW_VS_INNER_LINE_POS[1][1] = 0x42;
    KAW_VS_OUTER_LINE_POS[0][0] = 0x140;
    KAW_VS_OUTER_LINE_POS[0][1] = 0xB1;
    KAW_VS_OUTER_LINE_POS[1][0] = -0xC0;
    KAW_VS_OUTER_LINE_POS[1][1] = 0x30;
    frame = 0;
    scale = 200;
    for (i = 0; i < 2; i++) {
        if (mode != 0) {
            if (winner == i) {
                KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleWins;
                if (!KAW_DUEL->tutorial) {
                    KAW_MATCH_SCREEN->wins[i]++;
                }
                KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleLosses;
            } else {
                KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleWins;
                KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleLosses;
                if (!KAW_DUEL->tutorial) {
                    KAW_MATCH_SCREEN->losses[i]++;
                }
            }
        } else {
            if (winner == i) {
                KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusWins;
                if (!KAW_DUEL->tutorial) {
                    KAW_MATCH_SCREEN->wins[i]++;
                }
                KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusLosses;
            } else {
                KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusWins;
                KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusLosses;
                if (!KAW_DUEL->tutorial) {
                    KAW_MATCH_SCREEN->losses[i]++;
                }
            }
        }
        if (KAW_MATCH_SCREEN->wins[i] >= 1000) {
            KAW_MATCH_SCREEN->wins[i] = 999;
        }
        if (KAW_MATCH_SCREEN->losses[i] >= 1000) {
            KAW_MATCH_SCREEN->losses[i] = 999;
        }
    }
    do {
        waitFrames(FRAME_INTERVAL);
        frame++;
        prims = (VersusPrims *)CURRENT_FRAME_BUFFER->primSlots[15];
        if (!KAW_DUEL->tutorial && frame >= 56) {
            scale -= 8;
            if (scale < 100) {
                scale = 100;
            }
            initPrimByType(0xC, &prims->intro, 0, 0);
            setRGB0(&prims->intro, 0x80, 0x80, 0x80);
            if (winner == 0) {
                setXYWH(&prims->intro, KAW_VS_PANEL_POS[1][0] - (s16)(scale * 64 / 100 - 74), KAW_VS_PANEL_POS[1][1] - (s16)(scale * 56 / 100 - 60),
                        scale * 128 / 100, scale * 112 / 100);
            } else {
                setXYWH(&prims->intro, KAW_VS_PANEL_POS[0][0] - (s16)(scale * 64 / 100 - 248), KAW_VS_PANEL_POS[0][1] - (s16)(scale * 56 / 100 - 60),
                        scale * 128 / 100, scale * 112 / 100);
            }
            prims->intro.u0 = 0;
            prims->intro.v0 = 0;
            prims->intro.u1 = 0x80;
            prims->intro.v1 = 0;
            prims->intro.u2 = 0;
            prims->intro.v2 = 0x70;
            prims->intro.u3 = 0x80;
            prims->intro.v3 = 0x70;
            prims->intro.tpage = 8;
            prims->intro.clut = 0x3E98;
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &prims->intro);
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
        if (!KAW_DUEL->tutorial && frame == 70) {
            playSoundEffect(0x83);
        }
        i = 0;
        KAW_drawSprite(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][1], 0x2C0, 0x1C0, PLAYER(i)->nameWidth, 0x20, 0x2F0, 0x1D7, 0, 1, 0, 0x80, 0);
        KAW_drawDeckName(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][1], (char *)DUEL_PLAYERS[i] + 1);
        KAW_drawBattleRecord(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][1], KAW_MATCH_SCREEN->wins[i], KAW_MATCH_SCREEN->losses[i]);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0xB8, KAW_VS_PANEL_POS[i][1] + 4, 0x140, 0, 0x80, 0x70, 0x140, 0xFE, 1, 0, 0, 0x80, 4);
        if (!KAW_DUEL->tutorial) {
            if (winner == 0) {
                KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 8, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0, 0xA0, 0x5C, 0x180, 0xF8, 0, 0, 0, 0x80, 4);
            } else {
                KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 8, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0x5C, 0xA0, 0x5C, 0x180, 0xFC, 0, 0, 0, 0x80, 4);
            }
        }
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][1], 0x180, 0x78, 0x100, 0x78, 0x180, 0xF9, 0, 0, 0, 0x80, 4);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x100, KAW_VS_PANEL_POS[i][1], 0x1C0, 0x78, 0x40, 0x78, 0x180, 0xF9, 0, 0, 0, 0x80, 4);
        i = 1;
        KAW_drawSprite(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][1], 0x2C0, 0x1E0, PLAYER(i)->nameWidth, 0x20, 0x2F0, 0x1D8, 0, 1, 0, 0x80, 0);
        KAW_drawDeckName(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][1], (char *)DUEL_PLAYERS[i] + 1);
        KAW_drawBattleRecord(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][1], KAW_MATCH_SCREEN->wins[i], KAW_MATCH_SCREEN->losses[i]);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 8, KAW_VS_PANEL_POS[i][1] + 4, 0x140, 0x70, 0x80, 0x70, 0x140, 0xFF, 1, 0, 0, 0x80, 4);
        if (!KAW_DUEL->tutorial) {
            if (winner != 0) {
                KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x96, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0, 0xA0, 0x5C, 0x180, 0xF8, 0, 0, 0, 0x80, 4);
            } else {
                KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x96, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0x5C, 0xA0, 0x5C, 0x180, 0xFC, 0, 0, 0, 0x80, 4);
            }
        }
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][1], 0x180, 0, 0x100, 0x78, 0x180, 0xFB, 0, 0, 0, 0x80, 4);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x100, KAW_VS_PANEL_POS[i][1], 0x1C0, 0, 0x40, 0x78, 0x180, 0xFB, 0, 0, 0, 0x80, 4);
        if (frame > 30) {
            KAW_RESULT_SCREEN_STATE = 1;
        }
    } while (frame < 100 || !(PAD_STATES[0]->pressed & PAD_CROSS));
    KAW_RESULT_SCREEN_STATE = 0;
    waitFrames(2);
    freeHeapBlock(KAW_MATCH_SCREEN);
    waitFrames(2);
}
