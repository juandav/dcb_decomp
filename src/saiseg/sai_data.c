#include "common.h"
#include "game.h"
#include "dcb/sai_data.h"
#include "dcb/menu.h"

/* The tables SAISEG's rodata and data start with, ahead of every module's
   own: the player data screen's, the error windows' and the VRAM glitch's. */

/* not referenced by any code */
const s32 D_801DDF38 = 6;

/* where the five windows of SAI_ERROR_WINDOWS open */
WindowDef SAI_ERROR_WINDOW_DEFS[5] = {
    { { 0xA0, 0x32, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0xF, 0x82, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0x1E, 0x14, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0x5A, 0xB4, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0xA0, 0x78, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
};

/* VRAM areas SAI_glitchVram moves with MoveImage2 */
Rect16 SAI_GLITCH_VRAM_RECTS[6] = {
    { 0x220, 0xE2, 0x20, 2 },
    { 0x220, 0xE5, 0x20, 1 },
    { 0x220, 0xEC, 0x20, 1 },
    { 0x220, 0xF4, 0x20, 1 },
    { 0x230, 0xEB, 0x20, 1 },
    { 0x280, 0x1FC, 0x100, 3 },
};

WindowDef SAI_STATS_HINT_WINDOW_DEF = { { 0x88, 0x14, 0xA4, 0xD }, 0x80, 0x31, 0, 0, 8 };

/* the rows of the player's data screen */
Menu SAI_PLAYER_DATA_MENU = { NULL, NULL, { 10, 48, 300, 126 }, 0, -1, 0, -1, 0xa, 0x16, 72, 12, 0, 15, 0, 1, 0, 14, 0, 0, 0 };

/* the help line for each row of SAI_PLAYER_DATA_MENU */
char *SAI_PLAYER_DATA_HELP[12] = {
    "*w1Player's Name.",
    "*w1Tamer Rank is based on Wins. There are 8 \nRanks. \"Beginner Tamer\" with 0 wins\nto \"Invincible Tamer\" with 500+ Wins.",
    "*w1Collector Rank is set by the number of\nCards collected. 7 Titles from \"General \n\tPublic\" to the highest, plus one more.",
    "*w1This shows how far you are in the game.\nCan you achieve 100%?",
    "*w1This is the ratio of Cards collected.",
    "*w1Collected Digi-Parts ratio, 128 total.",
    "*w1Watch your wins and losses in Digi-land.\n\tRemember the Cards you were beaten by.\nIt will make you a better Card Tamer.",
    "*w1Wins & losses in \"Battle with Friends.\"\nDo battle with all your friends.\nEverybody will love it! It's guaranteed!",
    "*w1This is the first Deck you have.",
    "*w1This is the second Deck you have.",
    "*w1This is the third Deck you have.",
    "*w1These are Partners & Digi-Eggs you own.\nVeemon alone has 3 Digi-Eggs.",
};
