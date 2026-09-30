#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/card_db.h"
#include "dcb/vram_upload.h"
#include "dcb/sound_play.h"
#include "dcb/frame_callback.h"
#include "dcb/partner_level.h"
#include "dcb/pad.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_hand.h"
#include "dcb/kaw_hud.h"
#include "dcb/kaw_bonus.h"

typedef struct {
     char *name;
     u8 unk4[8];
} PartInfo;

extern ExpScreen *KAW_EXP_SCREEN;

/* the partner abilities; the code here only reads their texts */
PartInfo KAW_DIGI_PARTS[128] = {
    { "HP+50.", { 3, 5, 1, 99, 3, 7, 0, 0 } },
    { "HP+100.", { 17, 16, 8, 12, 14, 19, 0, 0 } },
    { "HP+150.", { 29, 32, 19, 25, 32, 33, 0, 0 } },
    { "HP+200.", { 45, 48, 31, 40, 52, 59, 0, 0 } },
    { "HP+300.", { 61, 67, 51, 57, 68, 78, 0, 0 } },
    { "HP+400.", { 96, 89, 71, 81, 91, 88, 0, 0 } },
    { "HP+500.", { 99, 0xFF, 75, 91, 0xFF, 95, 0, 0 } },
    { "All Attack Powers +50.", { 18, 39, 54, 30, 57, 28, 0, 0 } },
    { "All Attack Powers +100.", { 39, 86, 72, 50, 89, 51, 0, 0 } },
    { "All Attack Powers +200.", { 75, 0xFF, 90, 93, 0xFF, 84, 0, 0 } },
    { "*b0 Attack Power +100.", { 1, 9, 15, 5, 4, 2, 0, 0 } },
    { "*b0 Attack Power +150.", { 10, 21, 38, 22, 16, 15, 0, 0 } },
    { "*b0 Attack Power +200.", { 27, 54, 62, 41, 39, 30, 0, 0 } },
    { "*b0 Attack Power +250.", { 48, 0xFF, 78, 61, 62, 61, 0, 0 } },
    { "*b0 Attack Power +300.", { 67, 0xFF, 0xFF, 85, 82, 79, 0, 0 } },
    { "*b1 Attack Power +50.", { 4, 1, 12, 2, 6, 13, 0, 0 } },
    { "*b1 Attack Power +100.", { 12, 7, 22, 16, 18, 29, 0, 0 } },
    { "*b1 Attack Power +150.", { 32, 28, 45, 36, 45, 43, 0, 0 } },
    { "*b1 Attack Power +200.", { 51, 44, 0xFF, 56, 67, 66, 0, 0 } },
    { "*b1 Attack Power +250.", { 77, 0xFF, 0xFF, 76, 86, 96, 0, 0 } },
    { "*b2 Attack Power +50.", { 6, 6, 2, 4, 10, 1, 0, 0 } },
    { "*b2 Attack Power +100.", { 19, 36, 20, 19, 27, 9, 0, 0 } },
    { "*b2 Attack Power +150.", { 35, 69, 42, 38, 53, 24, 0, 0 } },
    { "*b2 Attack Power +200.", { 82, 0xFF, 63, 67, 0xFF, 48, 0, 0 } },
    { "*b0 to 0, *b2 Attack Power -100.", { 24, 12, 4, 14, 0xFF, 34, 0, 0 } },
    { "*b1 to 0, *b2 Attack Power -100.", { 46, 23, 9, 35, 0xFF, 17, 0, 0 } },
    { "*b2 to 0, *b2 Attack Power -100.", { 58, 71, 36, 10, 0xFF, 71, 0, 0 } },
    { "Counter *b0,*b2 Attack Power to 0.", { 11, 26, 0xFF, 0xFF, 40, 62, 0, 0 } },
    { "Counter *b1,*b2 Attack Power to 0.", { 36, 14, 0xFF, 0xFF, 24, 49, 0, 0 } },
    { "Counter *b2,*b2 Attack Power to 0.", { 40, 57, 0xFF, 0xFF, 11, 21, 0, 0 } },
    { "Opponent *a0 X3, *b2 Attack Power -200.", { 87, 92, 39, 69, 31, 50, 0, 0 } },
    { "Opponent *a1 X3, *b2 Attack Power -200.", { 25, 97, 0xFF, 42, 77, 73, 0, 0 } },
    { "Opponent *a2 X3, *b2 Attack Power -200.", { 37, 0xFF, 66, 0xFF, 83, 4, 0, 0 } },
    { "Opponent *a3 X3, *b2 Attack Power -200.", { 68, 33, 46, 21, 5, 97, 0, 0 } },
    { "Opponent *a4 X3, *b2 Attack Power -200.", { 52, 41, 6, 0xFF, 0xFF, 25, 0, 0 } },
    { "1st Attack, *b2 Attack Power -200.", { 76, 50, 95, 62, 0xFF, 0xFF, 0, 0 } },
    { "Jamming Support, *b2 Attack Power -100.", { 0xFF, 74, 27, 0xFF, 54, 10, 0, 0 } },
    { "Eat-up HP, *b2 Attack Power -200.", { 0xFF, 87, 50, 0xFF, 73, 0xFF, 0, 0 } },
    { "Add + 10 DP.", { 42, 4, 61, 1, 9, 0xFF, 0, 0 } },
    { "Add + 20 DP.", { 91, 29, 79, 27, 21, 0xFF, 0, 0 } },
    { "Add + 30 DP.", { 0xFF, 98, 97, 60, 78, 85, 0, 0 } },
    { "Boost Attack Power +50.", { 2, 10, 13, 3, 7, 11, 0, 0 } },
    { "Boost Attack Power +100.", { 21, 42, 56, 33, 36, 35, 0, 0 } },
    { "Boost Attack Power +200.", { 26, 81, 73, 46, 84, 44, 0, 0 } },
    { "Boost Attack Power +300.", { 69, 0xFF, 84, 82, 0xFF, 80, 0, 0 } },
    { "Attack Power is Doubled.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Boost *b0 Attack Power +300.", { 8, 30, 47, 17, 26, 12, 0, 0 } },
    { "Boost *b0 Attack Power +400.", { 34, 77, 74, 47, 46, 39, 0, 0 } },
    { "Boost *b0 Attack Power +500.", { 55, 0xFF, 88, 88, 66, 74, 0, 0 } },
    { "*b0 Attack Power is Doubled.", { 13, 64, 67, 58, 33, 63, 0, 0 } },
    { "*b0 Attack Power is Tripled.", { 59, 0xFF, 94, 74, 0xFF, 86, 0, 0 } },
    { "Boost *b1 Attack Power +200.", { 15, 2, 0xFF, 7, 22, 18, 0, 0 } },
    { "Boost *b1 Attack Power +300.", { 28, 19, 0xFF, 23, 41, 36, 0, 0 } },
    { "Boost *b1 Attack Power +400.", { 43, 70, 0xFF, 92, 59, 53, 0, 0 } },
    { "*b1 Attack Power is Doubled.", { 22, 13, 59, 34, 0xFF, 67, 0, 0 } },
    { "*b1 Attack Power is Tripled.", { 83, 0xFF, 91, 94, 0xFF, 89, 0, 0 } },
    { "Boost *b2 Attack Power +100.", { 23, 40, 10, 18, 0xFF, 8, 0, 0 } },
    { "Boost *b2 Attack Power +200.", { 38, 58, 40, 43, 0xFF, 20, 0, 0 } },
    { "Boost *b2 Attack Power +300.", { 71, 0xFF, 60, 63, 0xFF, 90, 0, 0 } },
    { "*b2 Attack Power is Doubled.", { 44, 72, 48, 51, 0xFF, 40, 0, 0 } },
    { "*b2 Attack Power is Tripled.", { 88, 0xFF, 85, 95, 0xFF, 65, 0, 0 } },
    { "Attack Power becomes same as HP.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Get 1st Attack.", { 53, 55, 98, 71, 85, 0xFF, 0, 0 } },
    { "Attack becomes Eat-up HP.", { 0xFF, 93, 80, 0xFF, 61, 55, 0, 0 } },
    { "Lower Opponent's *b0 Attack Power to 0.", { 54, 0xFF, 17, 0xFF, 28, 75, 0, 0 } },
    { "Lower Opponent's *b1 Attack Power to 0.", { 84, 24, 28, 0xFF, 15, 0xFF, 0, 0 } },
    { "Lower Opponent's *b2 Attack Power to 0.", { 0xFF, 66, 24, 0xFF, 13, 45, 0, 0 } },
    { "*b0 Counterattack (Attack 2nd).", { 7, 0xFF, 33, 0xFF, 47, 14, 0, 0 } },
    { "*b1 Counterattack (Attack 2nd).", { 33, 17, 0xFF, 0xFF, 37, 26, 0, 0 } },
    { "*b2 Counterattack (Attack 2nd).", { 47, 43, 5, 0xFF, 0xFF, 37, 0, 0 } },
    { "If *a0 Opponent, X2 own Attack Power.", { 74, 79, 0xFF, 77, 25, 38, 0, 0 } },
    { "If *a0 Opponent, X3 own Attack Power.", { 0xFF, 91, 0xFF, 65, 74, 82, 0, 0 } },
    { "If *a1 Opponent, X2 own Attack Power.", { 5, 47, 64, 26, 80, 76, 0, 0 } },
    { "If *a1 Opponent, X3 own Attack Power.", { 60, 0xFF, 89, 83, 98, 91, 0, 0 } },
    { "If *a2 Opponent, X2 own Attack Power.", { 50, 27, 58, 59, 42, 6, 0, 0 } },
    { "If *a2 Opponent, X3 own Attack Power.", { 79, 0xFF, 93, 0xFF, 93, 70, 0, 0 } },
    { "If *a3 Opponent, X2 own Attack Power.", { 56, 37, 44, 6, 19, 93, 0, 0 } },
    { "If *a3 Opponent, X3 own Attack Power.", { 92, 60, 87, 79, 63, 0xFF, 0, 0 } },
    { "If *a4 Opponent, X2 own Attack Power.", { 78, 76, 26, 72, 69, 46, 0, 0 } },
    { "If *a4 Opponent, X3 own Attack Power.", { 89, 96, 55, 0xFF, 0xFF, 68, 0, 0 } },
    { "Change own Specialty to *a0.", { 9, 35, 0xFF, 48, 96, 0xFF, 0, 0 } },
    { "Change own Specialty to *a1.", { 0xFF, 99, 30, 87, 58, 0xFF, 0, 0 } },
    { "Change own Specialty to *a2.", { 97, 3, 41, 8, 12, 94, 0, 0 } },
    { "Change own Specialty to *a3.", { 30, 0xFF, 68, 98, 81, 3, 0, 0 } },
    { "Change own Specialty to *a4.", { 95, 82, 14, 52, 76, 99, 0, 0 } },
    { "Switch Opponent's Specialty to own.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Swap Specialty with Opponent's.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *a0 Opponent, lower its AP to 0.", { 66, 52, 0xFF, 97, 43, 0xFF, 0, 0 } },
    { "If *a1 Opponent, lower its AP to 0.", { 41, 61, 34, 84, 8, 0xFF, 0, 0 } },
    { "If *a2 Opponent, lower its AP to 0.", { 0xFF, 11, 52, 0xFF, 64, 22, 0, 0 } },
    { "If *a3 Opponent, lower its AP to 0.", { 0xFF, 25, 37, 13, 2, 72, 0, 0 } },
    { "If *a4 Opponent, lower its AP to 0.", { 0xFF, 75, 16, 0xFF, 34, 52, 0, 0 } },
    { "Reduce both Players' Atk Pwr to 0.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *e3, boost Attack Power +200.", { 14, 31, 0xFF, 9, 48, 23, 0, 0 } },
    { "If *e4, boost Attack Power +300.", { 31, 53, 0xFF, 20, 38, 57, 0, 0 } },
    { "If *e5, boost Attack Power +400.", { 57, 0xFF, 81, 70, 95, 77, 0, 0 } },
    { "Opponent uses *b0 Attack.", { 16, 62, 0xFF, 28, 97, 31, 0, 0 } },
    { "Opponent uses *b1 Attack.", { 62, 18, 11, 39, 0xFF, 47, 0, 0 } },
    { "Opponent uses *b2 Attack.", { 94, 8, 21, 0xFF, 44, 58, 0, 0 } },
    { "Opponent uses same Attack.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Recover HP +200.", { 72, 15, 32, 11, 1, 81, 0, 0 } },
    { "Recover HP +300.", { 90, 38, 53, 31, 17, 0xFF, 0, 0 } },
    { "Recover HP +400.", { 0xFF, 68, 92, 73, 55, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +400.", { 93, 20, 43, 15, 23, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +600.", { 0xFF, 46, 86, 44, 60, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +500.", { 98, 59, 23, 37, 29, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +700.", { 0xFF, 83, 69, 53, 56, 0xFF, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 300.", { 49, 34, 0xFF, 24, 30, 42, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 600.", { 0xFF, 63, 0xFF, 49, 49, 64, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 1000.", { 0xFF, 94, 0xFF, 64, 65, 98, 0, 0 } },
    { "Drop 1 Card in Opponent's Hand.", { 63, 22, 3, 45, 75, 27, 0, 0 } },
    { "Drop 2 Cards in Opponent's Hand.", { 0xFF, 78, 25, 0xFF, 92, 56, 0, 0 } },
    { "Drop Opponent's Top 2 DP Cards shown.", { 0xFF, 56, 18, 86, 35, 5, 0, 0 } },
    { "Drop Opponent's Top 3 DP Cards shown.", { 0xFF, 88, 49, 0xFF, 90, 32, 0, 0 } },
    { "Drop Opponent's Top 4 DP Cards shown.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Drop 2 Cards in Opponent's Online Deck.", { 0xFF, 51, 7, 29, 0xFF, 16, 0, 0 } },
    { "Drop 3 Cards in Opponent's Online Deck.", { 0xFF, 95, 35, 89, 0xFF, 54, 0, 0 } },
    { "Move Offline Top Card to Online Deck.", { 86, 0xFF, 70, 78, 50, 0xFF, 0, 0 } },
    { "Void Opponent's Support Effect.", { 0xFF, 84, 65, 0xFF, 87, 69, 0, 0 } },
    { "Draw until there are 4 Cards.", { 64, 45, 29, 54, 20, 0xFF, 0, 0 } },
    { "Draw Online Partner Card, then Shuffle.", { 65, 80, 82, 68, 72, 0xFF, 0, 0 } },
    { "If *e3, HP + 200 & all Attack Powers +100.", { 73, 73, 0xFF, 55, 94, 83, 0, 0 } },
    { "If *ea, HP + 200 & all Attack Powers +100.", { 81, 0xFF, 76, 66, 79, 87, 0, 0 } },
    { "Boost Battle Experience by 10%.", { 20, 49, 57, 32, 71, 41, 0, 0 } },
    { "Boost Battle Experience by 20%.", { 85, 65, 77, 75, 88, 92, 0, 0 } },
    { "Boost Battle Experience by 30%.", { 80, 0xFF, 96, 90, 99, 60, 0, 0 } },
    { "Rare Card might appear after battle.", { 70, 85, 83, 80, 51, 0xFF, 0, 0 } },
    { "Rare Card even more likely to appear.", { 0xFF, 90, 99, 96, 70, 0xFF, 0, 0 } },
};

void KAW_drawExpTitle(UiWindow *window) {
    drawText(window->originX + 2, window->originY + 1, (s32)"Earned Experience Points", 7, 0);
}

void KAW_drawPartnerExp(ExpWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 next;
    s32 attr;
    char buf[72];

    x = w->window.originX;
    y = w->window.originY;
    z = w->window.z;
    if (KAW_EXP_SCREEN->partnerShown[w->partner]) {
        attr = ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attr >> 4;
        KAW_drawPortrait(x + 0x22, y + 1, w->partner * 20 + 0x2C0, 0x128, attr, w->clut);
        x += 0x6E;
        drawText(x, y + 1, (s32)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].name, 7, z);
        sprintf(buf, "RANK   \x0c\x07%2d", (s8)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].level);
        drawLargeText(x, y + 0xE, (s32)buf, 6, z);
        sprintf(buf, "EXP  \x0c\x07%4d", (u16)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].exp);
        drawLargeText(x, y + 0x16, (s32)buf, 6, z);
        next = 0;
        if ((s8)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].level < 99) {
            next = getExpForNextLevel((s8)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].level) -
                   (u16)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].exp;
        }
        sprintf(buf, "NEXT  \x0c\x07%3d", next);
        drawLargeText(x, y + 0x1E, (s32)buf, 6, z);
        x += 0x64;
        drawIcon(x, y + 1, 0, 0x1A, z);
        sprintf(buf, "*s0%4d", ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].hp);
        drawText(x + 0xE, y + 1, (s32)buf, 7, z);
        if (KAW_EXP_SCREEN->gains[w->partner][0]) {
            sprintf(buf, "*s0+%d", KAW_EXP_SCREEN->gains[w->partner][0]);
            drawText(x + 0x2E, y + 1, (s32)buf, 5, z);
        }
        drawIcon(x, y + 0xD, 0, 7, z);
        sprintf(buf, "*s0%4d", ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attack[0].power);
        drawText(x + 0xE, y + 0xD, (s32)buf, 7, z);
        if (KAW_EXP_SCREEN->gains[w->partner][1]) {
            sprintf(buf, "*s0+%d", KAW_EXP_SCREEN->gains[w->partner][1]);
            drawText(x + 0x2E, y + 0xD, (s32)buf, 5, z);
        }
        drawIcon(x, y + 0x19, 0, 8, z);
        sprintf(buf, "*s0%4d", ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attack[1].power);
        drawText(x + 0xE, y + 0x19, (s32)buf, 7, z);
        if (KAW_EXP_SCREEN->gains[w->partner][2]) {
            sprintf(buf, "*s0+%d", KAW_EXP_SCREEN->gains[w->partner][2]);
            drawText(x + 0x2E, y + 0x19, (s32)buf, 5, z);
        }
        drawIcon(x, y + 0x25, 0, 9, z);
        sprintf(buf, "*s0%4d", ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attack[2].power);
        drawText(x + 0xE, y + 0x25, (s32)buf, 7, z);
        if (KAW_EXP_SCREEN->gains[w->partner][3]) {
            sprintf(buf, "*s0+%d", KAW_EXP_SCREEN->gains[w->partner][3]);
            drawText(x + 0x2E, y + 0x25, (s32)buf, 5, z);
        }
    }
}

void KAW_drawBonusList(UiWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 exp;
    s32 rowY;
    char buf[24];

    x = w->originX;
    y = w->originY;
    z = w->z;
    drawText(x + 0x28, y + 1, (s32)"Detail of Earned Experience Points", 6, 0);
    exp = DUEL->winner == 0 ? ((u8 *)SESSION_DATA)[0x74] : 0;
    rowY = y + 15;
    drawText(x + 6, rowY, (s32)"Experience Points from Opponent", 7, z);
    sprintf(buf, "*s0+%3d", exp);
    drawText(x + 0xA2, rowY, (s32)buf, 5, z);
    KAW_EXP_SCREEN->done = KAW_drawBonuses(x, y + 0x1C, KAW_EXP_SCREEN->progress / 32, z, exp);
    if (PAD_STATES[0]->pressed & PAD_CROSS) {
        KAW_EXP_SCREEN->speed = 0x20;
    }
    if (KAW_EXP_SCREEN->done == 0) {
        KAW_EXP_SCREEN->progress += KAW_EXP_SCREEN->speed;
        w->view.h = KAW_EXP_SCREEN->progress / 32 * 13 + 0x26;
        if (w->view.h - w->rect.h >= 0) {
            scrollWindowTo((s16 *)w, 0, w->view.h - w->rect.h);
        }
    } else {
        if (PAD_STATES[0]->repeat & PAD_L2) {
            scrollWindowTo((s16 *)w, 0, 0);
        }
        if (PAD_STATES[0]->repeat & PAD_R2) {
            scrollWindowTo((s16 *)w, 0, w->view.h - w->rect.h);
        }
        if (PAD_STATES[0]->repeat & PAD_UP) {
            scrollWindowTo((s16 *)w, 0, w->view.y - 13);
        }
        if (PAD_STATES[0]->repeat & PAD_DOWN) {
            scrollWindowTo((s16 *)w, 0, w->view.y + 13);
        }
    }
}

void KAW_drawEarnedParts(UiWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 n;
    s32 icon;
    s32 palette;
    char buf[24];

    x = w->originX;
    y = w->originY;
    z = w->z;
    drawText(x + 0x5A, y + 1, (s32)"Earned Digi-Parts", 6, 0);
    n = 0;
    for (i = 0; i < 128; i++) {
        if ((KAW_EXP_SCREEN->partFlags[i / 8] >> (i % 8)) & 1) {
            n++;
            if (n < (w->view.y - 15) / 13) {
                continue;
            }
            if ((w->view.y + w->rect.h) / 13 < n) {
                continue;
            }
            sprintf(buf, "*s0%3.3d", i);
            drawText(x + 2, y + 15 + (n - 1) * 13, (s32)buf, 5, z);
            if (i < 7) {
                icon = 0;
            } else if (i < 10) {
                icon = 1;
            } else if (i < 15) {
                icon = 2;
            } else if (i < 20) {
                icon = 3;
            } else if (i < 24) {
                icon = 4;
            } else if (i < 38) {
                icon = 5;
            } else if (i < 41) {
                icon = 6;
            } else if (i < 123) {
                icon = 7;
            } else {
                icon = 8;
            }
            drawIcon(x + 0x16, y + 15 + (n - 1) * 13, 2, icon, z);
            palette = 7;
            if (i >= 24 && i < 38) {
                palette = 4;
            }
            if (i >= 41 && i < 123) {
                palette = 5;
            }
            drawText(x + 0x30, y + 15 + (n - 1) * 13, (s32)KAW_DIGI_PARTS[i].name, palette, z);
        }
    }
    w->view.h = n * 13 + 15;
    if (w->view.h - w->rect.h >= 0) {
        if (PAD_STATES[0]->repeat & PAD_L2) {
            scrollWindowTo((s16 *)w, 0, w->view.y - w->rect.h);
        }
        if (PAD_STATES[0]->repeat & PAD_R2) {
            scrollWindowTo((s16 *)w, 0, w->view.y + w->rect.h);
        }
        if (PAD_STATES[0]->repeat & PAD_UP) {
            scrollWindowTo((s16 *)w, 0, w->view.y - 13);
        }
        if (PAD_STATES[0]->repeat & PAD_DOWN) {
            scrollWindowTo((s16 *)w, 0, w->view.y + 13);
        }
    }
    if (n == 0) {
        drawText(x + 0x1A, y + 0xE, (s32)"None", 7, 0);
    }
}

void KAW_drawRankUp(RankUpWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    char buf[24];

    x = w->window.originX;
    y = w->window.originY;
    z = w->window.z;
    sprintf(buf, "%2d RANK UP!", w->rank);
    drawLargeText(x + 1, y + 1, (s32)buf, 7, z);
}

void KAW_renderExpScreen(void) {
    s32 i;

    drawWindow(&KAW_EXP_SCREEN->partsWindow, KAW_drawEarnedParts, 0);
    drawWindow(&KAW_EXP_SCREEN->window, KAW_drawBonusList, 0);
    drawWindow(&KAW_EXP_SCREEN->titleWindow, KAW_drawExpTitle, 0);
    for (i = 0; i < 3; i++) {
        drawWindow(&KAW_EXP_SCREEN->rankWindows[i].window, KAW_drawRankUp, 0);
        drawWindow(&KAW_EXP_SCREEN->expWindows[i].window, KAW_drawPartnerExp, 0);
    }
}

void KAW_uploadPartnerPortraits(u8 *archive) {
    s32 i;
    s32 id;

    for (i = 0; i < 3; i++) {
        if (((SessionData *)SESSION_DATA)->npcDeckIndex[0] != -1) {
            id = ((SessionData *)SESSION_DATA)->partnerBackup[0][i].cardId;
        } else {
            id = ((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId;
        }
        if (id != 0) {
            uploadTim((u32 *)(archive + ((s32 *)archive)[id]), i * 20 + 0x2C0, 0x128, -1, -1);
            KAW_DUEL->partnerCluts[i] = getClut(LOADED_TIM.crect->x, LOADED_TIM.crect->y);
        }
    }
}

void KAW_runExpScreen(void) {
    Rect16 rect;
    s32 i;
    s32 j;
    s32 n;
    s32 gained;

    KAW_EXP_SCREEN = allocPermanentHeapBlock(sizeof(ExpScreen));
    KAW_EXP_SCREEN->progress = 0;
    KAW_EXP_SCREEN->speed = 1;
    KAW_EXP_SCREEN->done = 0;
    for (i = 0; i < 3; i++) {
        KAW_EXP_SCREEN->partnerShown[i] = 0;
        for (j = 0; j < 4; j++) {
            KAW_EXP_SCREEN->gains[i][j] = 0;
        }
    }
    for (i = 0; i < 30; i++) {
        n = findPartnerSlot(0, PLAYER(0)->cards[i].id);
        if (n >= 0 && n < 3) {
            KAW_EXP_SCREEN->partnerShown[n] = 1;
        }
        n = findArmorPartnerSlot(0, PLAYER(0)->cards[i].id);
        if (n >= 0 && n < 3) {
            KAW_EXP_SCREEN->partnerShown[n] = 1;
        }
    }
    for (i = 0; i < 16; i++) {
        KAW_EXP_SCREEN->partFlags[i] = 0;
    }
    rect.x = 0x30;
    rect.y = 0x2C;
    rect.w = 0xE0;
    rect.h = 0xA8;
    openWindow(&KAW_EXP_SCREEN->window, &rect, -1, (s16 *)-1, 10, 0x16, 0x80, 12);
    KAW_EXP_SCREEN->window.label = (s32)"BONUS LIST";
    animateWindowTo(&KAW_EXP_SCREEN->window, (Rect16 *)-1);
    rect.x = 0x10;
    rect.y = 0x2C;
    rect.w = 0x120;
    rect.h = 0xA8;
    openWindow(&KAW_EXP_SCREEN->partsWindow, &rect, -1, (s16 *)-1, 10, 0x16, 0x80, 12);
    KAW_EXP_SCREEN->partsWindow.label = (s32)"DIGI-PARTS RECEIVED";
    animateWindowTo(&KAW_EXP_SCREEN->partsWindow, (Rect16 *)-1);
    rect.x = 0x10;
    rect.y = 0x14;
    rect.w = 0x120;
    rect.h = 0xE;
    openWindow(&KAW_EXP_SCREEN->titleWindow, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    for (i = 0; i < 3; i++) {
        KAW_EXP_SCREEN->expWindows[i].partner = i;
        rect.x = 0x10;
        rect.y = i * 60 + 0x2C;
        rect.w = 0x120;
        rect.h = 0x32;
        openWindow(&KAW_EXP_SCREEN->expWindows[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
        rect.x = 0x14;
        rect.y = i * 60 + 0x40;
        rect.w = 0x60;
        rect.h = 9;
        openWindow(&KAW_EXP_SCREEN->rankWindows[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
        KAW_EXP_SCREEN->rankWindows[i].rank = 0;
        KAW_EXP_SCREEN->rankWindows[i].window.palette = 2;
        animateWindowTo(&KAW_EXP_SCREEN->rankWindows[i].window, (Rect16 *)-1);
        if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId != 0) {
            KAW_EXP_SCREEN->expWindows[i].clut = KAW_DUEL->partnerCluts[i];
        }
    }
    playSoundEffect(0xA3);
    addFrameCallback((s32)KAW_renderExpScreen);
    waitFrames(20);
    playSoundEffect(0xA3);
    rect.x = 0x30;
    rect.y = 0x2C;
    rect.w = 0xE0;
    rect.h = 0xA8;
    animateWindowTo(&KAW_EXP_SCREEN->window, &rect);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (KAW_EXP_SCREEN->done == 0 || !(PAD_STATES[0]->pressed & PAD_CROSS));
    playSoundEffect(0xA4);
    animateWindowTo(&KAW_EXP_SCREEN->window, (Rect16 *)-1);
    waitFrames(20);
    for (i = 0; i < 3; i++) {
        KAW_EXP_SCREEN->pendingExp[i] = 0;
        if (KAW_EXP_SCREEN->partnerShown[i] && (s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level < 99) {
            KAW_EXP_SCREEN->pendingExp[i] = KAW_BONUS_EXP + ((PlayerProfile *)PLAYER_PROFILES)->partners[i].expBonus * KAW_BONUS_EXP / 100;
        }
    }
    gained = 0;
    if (KAW_EXP_SCREEN->pendingExp[0] + KAW_EXP_SCREEN->pendingExp[1] + KAW_EXP_SCREEN->pendingExp[2] != 0) {
        do {
            waitFrames(3);
            for (i = 0; i < 3; i++) {
                if (KAW_EXP_SCREEN->partnerShown[i] && KAW_EXP_SCREEN->pendingExp[i] != 0) {
                    if ((s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level < 99) {
                        ((PlayerProfile *)PLAYER_PROFILES)->partners[i].exp++;
                        if ((u16)((PlayerProfile *)PLAYER_PROFILES)->partners[i].exp >= getExpForNextLevel((s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level)) {
                            ((PlayerProfile *)PLAYER_PROFILES)->partners[i].level++;
                            rect.x = 0x14;
                            rect.y = i * 60 + 0x40;
                            rect.w = 0x60;
                            rect.h = 9;
                            animateWindowTo(&KAW_EXP_SCREEN->rankWindows[i].window, &rect);
                            KAW_EXP_SCREEN->rankWindows[i].rank++;
                            n = findNewPartnerAbility((AbilityLearnEntry *)KAW_DIGI_PARTS, 0, i);
                            if (n >= 0) {
                                KAW_EXP_SCREEN->partFlags[n / 8] |= 1 << (n % 8);
                                grantPartnerAbility(0, n);
                            }
                            n = rollPartnerAbility(0, i);
                            if (n >= 0) {
                                KAW_EXP_SCREEN->gains[i][n] += 10;
                                gained = 1;
                            }
                        }
                        KAW_EXP_SCREEN->pendingExp[i]--;
                    } else {
                        KAW_EXP_SCREEN->pendingExp[i] = 0;
                    }
                }
            }
            playSoundEffect(0xAA);
        } while (KAW_EXP_SCREEN->pendingExp[0] + KAW_EXP_SCREEN->pendingExp[1] + KAW_EXP_SCREEN->pendingExp[2] != 0);
    }
    KAW_waitForCross();
    if (gained) {
        do {
            waitFrames(3);
            n = 0;
            for (i = 0; i < 3; i++) {
                if (KAW_EXP_SCREEN->partnerShown[i]
                    && KAW_EXP_SCREEN->gains[i][0] + KAW_EXP_SCREEN->gains[i][1] + KAW_EXP_SCREEN->gains[i][2] + KAW_EXP_SCREEN->gains[i][3] != 0) {
                    n++;
                    if (KAW_EXP_SCREEN->gains[i][0] != 0) {
                        ((PlayerProfile *)PLAYER_PROFILES)->partners[i].hpBonus++;
                        KAW_EXP_SCREEN->gains[i][0]--;
                    }
                    for (j = 0; j < 3; j++) {
                        if (KAW_EXP_SCREEN->gains[i][j + 1] != 0) {
                            ((PlayerProfile *)PLAYER_PROFILES)->partners[i].attackBonus[j]++;
                            KAW_EXP_SCREEN->gains[i][j + 1]--;
                        }
                    }
                    updatePartnerStats(0, i);
                }
            }
            playSoundEffect(0xAA);
        } while (n != 0);
        KAW_waitForCross();
    }
    playSoundEffect(0xA3);
    rect.x = 0x10;
    rect.y = 0x2C;
    rect.w = 0x120;
    rect.h = 0xA8;
    animateWindowTo(&KAW_EXP_SCREEN->partsWindow, &rect);
    KAW_waitForCross();
    playSoundEffect(0xA4);
    animateWindowTo(&KAW_EXP_SCREEN->partsWindow, (Rect16 *)-1);
    animateWindowTo(&KAW_EXP_SCREEN->titleWindow, (Rect16 *)-1);
    for (i = 0; i < 3; i++) {
        animateWindowTo(&KAW_EXP_SCREEN->rankWindows[i].window, (Rect16 *)-1);
        animateWindowTo(&KAW_EXP_SCREEN->expWindows[i].window, (Rect16 *)-1);
    }
    waitFrames(30);
    KAW_countEarnedBonuses();
    removeFrameCallback((s32)KAW_renderExpScreen);
    waitFrames(2);
    freeHeapBlock(KAW_EXP_SCREEN);
    waitFrames(2);
}
