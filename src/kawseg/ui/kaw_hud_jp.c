#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/card_db.h"
#include "dcb/window.h"
#include "dcb/kawseg.h"

/* jp's KAWSEG keeps only the tutorial's setup and teardown here (kaw_hud.c
   is us's and eu's): the card polygons and the cursor are in jp's
   executable */

/* the tutorial's decks: each card's type and index, per player */
extern u8 D_801FF6E4[2][30][2];

/* Sets the tutorial up: its message window, opening from the screen's
   centre, and the two fixed decks the players duel with */
void func_801FEC84(void) {
    Rect16 rect;
    s32 player;
    s32 i;

    ((SessionData *)SESSION_DATA)->unk8 = allocPermanentHeapBlock(0x36C);
    rect.x = 0x28;
    rect.y = 0x60;
    rect.w = 0x140;
    rect.h = 0xF0;
    openWindow(KAW_TUTORIAL, &rect, -1, (s16 *)-1, 0, 0xFF, 8);
    rect.x = 0xA0;
    rect.y = 0x78;
    rect.w = 0;
    rect.h = 0;
    animateWindowTo(&KAW_TUTORIAL->window, &rect, -1, -1);
    KAW_TUTORIAL->window.colors[0].r = 0;
    KAW_TUTORIAL->window.colors[0].g = 0;
    KAW_TUTORIAL->window.colors[0].b = 0x30;
    KAW_TUTORIAL->window.colors[1].r = 0xC0;
    KAW_TUTORIAL->window.colors[1].g = 0xC0;
    KAW_TUTORIAL->window.colors[1].b = 0xC0;
    KAW_TUTORIAL->window.colors[2].r = 0xC0;
    KAW_TUTORIAL->window.colors[2].g = 0xC0;
    KAW_TUTORIAL->window.colors[2].b = 0xC0;
    DUEL->turnPlayer = 1;
    DUEL->tutorialClosing = 0;
    DUEL->tutorialMessage = 0;
    DUEL->tutorialShown = 0;
    DUEL->tutorialVisible = 0;
    DUEL->tutorialOpen = 0;
    DUEL->unk449 = 0;
    PLAYER(0)->deck = &KAW_TUTORIAL->decks[0];
    PLAYER(1)->deck = &KAW_TUTORIAL->decks[1];
    strcpy(PLAYER(0)->deck->name, "レンタル");
    strcpy(PLAYER(1)->deck->name, "ごちゃまぜ");
    for (player = 0; player < 2; player++) {
        for (i = 0; i < 30; i++) {
            PLAYER_CARDS(PLAYER(player))[i].type = D_801FF6E4[player][i][0];
            PLAYER_CARDS(PLAYER(player))[i].index = D_801FF6E4[player][i][1];
            switch (PLAYER_CARDS(PLAYER(player))[i].type) {
            case 0:
                PLAYER_CARDS(PLAYER(player))[i].card =
                    (s8 *)&((DigimonCardData *)DIGIMON_CARDS)[PLAYER_CARDS(PLAYER(player))[i].index];
                break;
            case 1:
                PLAYER_CARDS(PLAYER(player))[i].card =
                    (s8 *)&((OptionCardData *)OPTION_CARDS)[PLAYER_CARDS(PLAYER(player))[i].index];
                break;
            case 2:
                PLAYER_CARDS(PLAYER(player))[i].card =
                    (s8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[PLAYER_CARDS(PLAYER(player))[i].index];
                break;
            }
        }
    }
}

void func_801FF0B8(void) {
    if (DUEL->tutorial != 0) {
        freeHeapBlock(((SessionData *)SESSION_DATA)->unk8);
    }
}
