#include "common.h"
#include "game.h"
#include "dcb/window.h"
#include "dcb/text.h"
#include "dcb/kawseg.h"

/* jp's KAWSEG keeps the tutorial's message window here (kaw_exp.c is us's
   and eu's) */

/* the tutorial's messages and where each shows */
extern u8 *D_801FF938[];
extern s16 D_801FF75C[][2];

/* copies the tutorial's message into its window, the player's name for each
   'P' */
void func_801FF100(void) {
    u8 *src;
    char *dst;
    s32 i;

    src = D_801FF938[DUEL->tutorialShown];
    dst = KAW_TUTORIAL->text;
    for (i = 0; i < 0x101; i++) {
        KAW_TUTORIAL->text[i] = 0;
    }
    while (*src != 0) {
        if (*src == 'P') {
            src++;
            strcat(dst, "c5");
            strcat(dst, PLAYER(0)->name);
            strcat(dst, "c7");
            dst += strlen(PLAYER(0)->name) + 4;
        } else {
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
        }
    }
    *dst = 0;
}

void func_801FF264(UiWindow *window) {
    drawIconText(window->originX + 5, window->originY + 2, 7, 1, window->z, (s32)KAW_TUTORIAL->text);
}

/* opens the tutorial's message window on its message, closing the one
   shown first */
void func_801FF2C0(void) {
    Rect16 rect;

    if (DUEL->tutorial == 0) {
        return;
    }
    if (DUEL->tutorialShown != DUEL->tutorialMessage) {
        if (DUEL->tutorialOpen == 0) {
            DUEL->tutorialShown = DUEL->tutorialMessage;
            DUEL->tutorialVisible = 1;
        } else {
            DUEL->tutorialVisible = 0;
        }
    }
    if (DUEL->tutorialOpen == 0) {
        if (DUEL->tutorialVisible != 0) {
            func_801FF100();
            measureText(1, (u8 *)KAW_TUTORIAL->text);
            KAW_TUTORIAL->window.cur.x = D_801FF75C[DUEL->tutorialMessage][0];
            KAW_TUTORIAL->window.cur.y = D_801FF75C[DUEL->tutorialMessage][1];
            KAW_TUTORIAL->window.cur.w = 0;
            KAW_TUTORIAL->window.cur.h = 0;
            rect.x = (s16)(D_801FF75C[DUEL->tutorialMessage][0] - 5) - (TEXT_WIDTH + 1) / 2;
            rect.y = (s16)(D_801FF75C[DUEL->tutorialMessage][1] - 2) - (TEXT_HEIGHT + 1) / 2;
            rect.w = (TEXT_WIDTH + 1) / 2 * 2 + 10;
            rect.h = (TEXT_HEIGHT + 1) / 2 * 2 + 4;
            animateWindowTo(&KAW_TUTORIAL->window, &rect, -1, -1);
            DUEL->tutorialOpen = DUEL->tutorialVisible;
        }
    } else if (DUEL->tutorialOpen != DUEL->tutorialVisible && DUEL->tutorialClosing == 0) {
        animateWindowTo(&KAW_TUTORIAL->window, (Rect16 *)-1, -1, -1);
        DUEL->tutorialClosing = 1;
    }
    if (DUEL->tutorialOpen | DUEL->tutorialVisible) {
        drawWindow(&KAW_TUTORIAL->window, func_801FF264, 0);
        if (DUEL->tutorialClosing != 0 && KAW_TUTORIAL->window.animDone == 1) {
            DUEL->tutorialOpen = 0;
            DUEL->tutorialClosing = 0;
            if (DUEL->tutorialShown != DUEL->tutorialMessage) {
                DUEL->tutorialShown = DUEL->tutorialMessage;
                DUEL->tutorialVisible = 1;
            }
        }
    }
}
