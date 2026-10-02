#include "common.h"
#include "game.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/menu.h"
#include "dcb/sound_play.h"
#include "dcb/dialog.h"
#include "dcb/pad.h"
#include "dcb/kawseg.h"

extern UiWindow KAW_DUEL_MENU_WINDOW;
extern Menu KAW_DUEL_MENU;
extern CursorHighlight KAW_DUEL_MENU_CURSOR;
extern DialogK KAW_MENU_DIALOG;
extern UiWindow KAW_HELP_WINDOW;
extern const char KAW_STR_GIVE_UP[];

void SsSetMono(void);
void KAW_drawDuelMenu(UiWindow *window);

/* the lines of the special effect descriptions */
char *KAW_EFFECT_HELP_LINES[51] = {
    "          *c6*b2 Special Effect Descriptions *h-6*c7*h0",
    "",
    "*c5*d3\"Jamming Support\" *c7",
    " Opponent's Support Effect is Voided.",
    " Can't Void Option Effect.",
    "*c5*d2\"1st Attack\"*c7",
    " Attack first, regardless of Turn.",
    " If you're 1st, Void Foe's \"1st Attack.\"",
    "*c5*d2\"Eat-up HP\" *c7",
    " Recover the same amount of HP as",
    " the damage inflicted.",
    "*c5*d1\"Crash\"*c7",
    " Attack Power becomes same as HP.",
    " HP becomes 10.",
    " No Effect until just before Attack.",
    "",
    "*c5*d1\"Opponent *a0 X3\"*c7",
    " If Opponent's Specialty is *a0,",
    " own *b2 Attack is Tripled.",
    "*c5*d1\"Opponent *a1 X3\"*c7",
    " If Opponent's Specialty is *a1,",
    " own *b2 Attack is Tripled.",
    "*c5*d1\"Opponent *a2 X3\"*c7",
    " If Opponent's Specialty is *a2,",
    " own *b2 Attack is Tripled.",
    "*c5*d1\"Opponent *a3 X3\"*c7",
    " If Opponent's Specialty is *a3,",
    " own *b2 Attack is Tripled.",
    "*c5*d1\"Opponent *a4 X3\"*c7",
    " If Opponent's Specialty is *a4,",
    " own *b2 Attack is Tripled.",
    "",
    "*c5*d1\"*b0 Counter Attack\"*c7",
    " If Opponent uses *b0 Attack, it will miss.",
    " Then you Counter with Opponent's ",
    " *b0 Attack power.",
    "*c5*d1\"*b1 Counter Attack\"*c7",
    " If Opponent uses *b1 Attack, it will miss.",
    " Then you Counter with Opponent's ",
    " *b1 Attack power.",
    "*c5*d1\"*b2 Counter Attack\"*c7",
    " If Opponent uses *b2 Attack, it will miss.",
    " Then you Counter with Opponent's ",
    " *b2 Attack power.",
    "",
    "*c5*d1\"*b0 to 0\"*c7",
    " Lowers Opponent's *b0 Attack Power to 0.",
    "*c5*d1\"*b1 to 0\"*c7",
    " Lowers Opponent's *b1 Attack Power to 0.",
    "*c5*d1\"*b2 to 0\"*c7",
    " Lowers Opponent's *b2 Attack Power to 0.",
};

/* the options of the duel menu, KAW_DUEL_MENU */
char *KAW_DUEL_MENU_LABELS[4] = {
    "Sound",
    "Polygon Battle",
    "*b2 Special Effect Descriptions",
    "Give Up",
};

void KAW_renderDuelMenu(void) {
    drawWindow(&KAW_DUEL_MENU_WINDOW, KAW_drawDuelMenu, 0);
}

void KAW_drawEffectHelp(UiWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    char unused[8]; /* unused, but it is in the original stack frame */

    x = w->originX;
    y = w->originY;
    z = w->z;
    for (i = 0; i < 51; i++) {
        if (i < (s16)(w->view.y / 12)) {
            continue;
        }
        if ((w->view.y + w->rect.h) / 12 < i) {
            break;
        }
        drawText(x, y + i * 12, (s32)KAW_EFFECT_HELP_LINES[i], 7, z);
    }
    if (PAD_STATES[KAW_DUEL->menuPlayer]->repeat & PAD_R2) {
        scrollWindowTo((s16 *)w, w->scroll[2], w->scroll[3] + w->rect.h);
    }
    if (PAD_STATES[KAW_DUEL->menuPlayer]->repeat & PAD_L2) {
        scrollWindowTo((s16 *)w, w->scroll[2], w->scroll[3] - w->rect.h);
    }
    if (PAD_STATES[KAW_DUEL->menuPlayer]->repeat & PAD_UP) {
        scrollWindowTo((s16 *)w, w->scroll[2], w->scroll[3] - 12);
    }
    if (PAD_STATES[KAW_DUEL->menuPlayer]->repeat & PAD_DOWN) {
        scrollWindowTo((s16 *)w, w->scroll[2], w->scroll[3] + 12);
    }
}

void KAW_drawDuelMenu(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    char *text;
    char unused[8]; /* unused, but it is in the original stack frame */

    x = window->originX;
    y = window->originY;
    z = window->z;
    for (i = 0; i < 4; i++) {
        text = KAW_DUEL_MENU_LABELS[i];
        if (DUEL->tutorial && i == 3) {
            text = "Quit";
        }
        drawText(x, y + i * 14, (s32)text, 7, z);
    }
    updateMenuCursor(&KAW_DUEL_MENU);
}

void KAW_tickDuelMenu(void) {
    Rect16 rect;
    Rect16 view;
    s32 player;
    s32 i;

    if (DUEL_MSG_BAR.playerLabel == 1 || KAW_DUEL->awaitingInput == 0 || KAW_DUEL->tutorialBusy != 0) {
        return;
    }
    player = DUEL_MSG_BAR.playerLabel & 1;
    if (!(PAD_STATES[player]->pressed & PAD_START)) {
        return;
    }
    playSoundEffect(0xA3);
    KAW_DUEL->menuOpen = 1;
    KAW_DUEL->menuPlayer = player;
    KAW_DUEL_MENU.pad = player;
    openMenu(&KAW_DUEL_MENU, &KAW_DUEL_MENU_WINDOW, &KAW_DUEL_MENU_CURSOR, (Bytes4 *)-1);
    KAW_DUEL_MENU_WINDOW.label = (s32)"MENU";
    for (;;) {
        waitFrames(FRAME_INTERVAL);
        drawWindow(&KAW_DUEL_MENU_WINDOW, KAW_drawDuelMenu, 0);
        if (PAD_STATES[KAW_DUEL->menuPlayer]->pressed & PAD_CROSS) {
            playSoundEffect(0xA0);
            switch (KAW_DUEL_MENU.row) {
            case 0:
                KAW_MENU_DIALOG.yes = "Stereo";
                KAW_MENU_DIALOG.no = "Mono";
                initDialog((u8 *)&KAW_MENU_DIALOG, "Sound Settings", 2);
                KAW_MENU_DIALOG.pad = KAW_DUEL->menuPlayer;
                KAW_MENU_DIALOG.draw = KAW_renderDuelMenu;
                KAW_MENU_DIALOG.result = ((PlayerProfile *)PLAYER_PROFILES)->monoSound + 1;
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, (Rect16 *)-1);
                runDialog(&KAW_MENU_DIALOG);
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, &KAW_DUEL_MENU.rect);
                switch (KAW_MENU_DIALOG.result) {
                case 2:
                    ((PlayerProfile *)PLAYER_PROFILES)->monoSound = 1;
                    SsSetMono();
                    break;
                case 1:
                    ((PlayerProfile *)PLAYER_PROFILES)->monoSound = 0;
                    SsSetStereo();
                    break;
                case 0:
                    break;
                }
                break;
            case 1:
                KAW_MENU_DIALOG.yes = "On";
                KAW_MENU_DIALOG.no = "Off";
                initDialog((u8 *)&KAW_MENU_DIALOG, "Polygon Battle", 2);
                KAW_MENU_DIALOG.pad = KAW_DUEL->menuPlayer;
                KAW_MENU_DIALOG.draw = KAW_renderDuelMenu;
                KAW_MENU_DIALOG.result = ((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation + 1;
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, (Rect16 *)-1);
                runDialog(&KAW_MENU_DIALOG);
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, &KAW_DUEL_MENU.rect);
                switch (KAW_MENU_DIALOG.result) {
                case 2:
                    ((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation = 1;
                    break;
                case 1:
                    ((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation = 0;
                    break;                    break;
                case 0:
                    break;
                }
                break;
            case 2:
                rect.x = 0x32;
                rect.y = 0x1A;
                rect.w = 0xDC;
                rect.h = 0xBE;
                view.x = 0;
                view.y = 0;
                view.w = 0xDC;
                view.h = 0x264;
                openWindow(&KAW_HELP_WINDOW, &rect, -1, (s16 *)&view, 10, 0x16, 0x80, 12);
                KAW_HELP_WINDOW.label = (s32)"HELP";
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, (Rect16 *)-1);
                do {
                    waitFrames(FRAME_INTERVAL);
                    drawWindow(&KAW_HELP_WINDOW, KAW_drawEffectHelp, 0);
                    drawWindow(&KAW_DUEL_MENU_WINDOW, KAW_drawDuelMenu, 0);
                } while (!(PAD_STATES[KAW_DUEL->menuPlayer]->pressed & PAD_TRIANGLE));
                playSoundEffect(0xA4);
                animateWindowTo(&KAW_HELP_WINDOW, (Rect16 *)-1);
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, &KAW_DUEL_MENU.rect);
                for (i = 0; i < 16; i++) {
                    waitFrames(FRAME_INTERVAL);
                    drawWindow(&KAW_HELP_WINDOW, KAW_drawEffectHelp, 0);
                    drawWindow(&KAW_DUEL_MENU_WINDOW, KAW_drawDuelMenu, 0);
                }
                break;
            case 3:
                if (KAW_DUEL->tutorial) {
                    initDialog((u8 *)&KAW_MENU_DIALOG, "Quit Tutorial?", 1);
                } else {
                    initDialog((u8 *)&KAW_MENU_DIALOG, KAW_STR_GIVE_UP, 1);
                }
                KAW_MENU_DIALOG.pad = KAW_DUEL->menuPlayer;
                KAW_MENU_DIALOG.draw = KAW_renderDuelMenu;
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, (Rect16 *)-1);
                runDialog(&KAW_MENU_DIALOG);
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, &KAW_DUEL_MENU.rect);
                switch (KAW_MENU_DIALOG.result) {
                case 2:
                case 0:
                    KAW_DUEL->quit = 0;
                    break;
                case 1:
                    KAW_DUEL->quit = ((MSG_BAR_PLAYER_LABEL & 1) + 2) ^ 1;
                    break;
                }
                break;
            }
        }
        if (!(PAD_STATES[KAW_DUEL->menuPlayer]->pressed & (PAD_TRIANGLE | PAD_START))) {
            if (KAW_DUEL->quit == 0) {
                continue;
            }
        } else {
            playSoundEffect(0xA4);
            animateWindowTo(&KAW_DUEL_MENU_WINDOW, (Rect16 *)-1);
            for (i = 0; i < 16; i++) {
                waitFrames(FRAME_INTERVAL);
                drawWindow(&KAW_DUEL_MENU_WINDOW, KAW_drawDuelMenu, 0);
            }
        }
        KAW_DUEL->menuOpen = 0;
        return;
    }
}

/* the last three bytes are leftovers in the original, not zero padding,
   and not the same in every version */
#if VERSION_US
const char KAW_STR_GIVE_UP[12] = "Give Up?\0\xD0\x12\x2B";
#elif VERSION_EU
const char KAW_STR_GIVE_UP[12] = "Give Up?\0fak";
#else
#error "kawseg/ui/kaw_duel_menu: version not checked"
#endif

Menu KAW_DUEL_MENU = { NULL, NULL, { 20, 40, 180, 56 }, 0, -1, 0, -1, 8, 0x16, 180, 12, 1, 4, 0, 0, 0, 14, 0, 0, 0 };
