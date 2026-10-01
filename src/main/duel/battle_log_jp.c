#include "common.h"
#include "game.h"
#include "dcb/card_db.h"
#include "dcb/text.h"

/* jp's own: the battle log, the last 7 lines of what happened in the duel
   for each player, which a HUD panel shows (drawHudPanelContents) */

extern char BATTLE_LOG[2][7][41];
extern s16 BATTLE_LOG_LINE_COUNTS[2];

void clearBattleLog(void) {
    s32 player;
    s32 line;
    s32 i;

    for (player = 0; player < 2; player++) {
        for (line = 0; line < 7; line++) {
            for (i = 0; i < 41; i++) {
                BATTLE_LOG[player][line][i] = 0;
            }
        }
        BATTLE_LOG_LINE_COUNTS[player] = 0;
    }
}

s32 countBattleLogLines(s32 player) {
    return BATTLE_LOG_LINE_COUNTS[player];
}

void addBattleLogLine(s32 player, char *text) {
    if (BATTLE_LOG_LINE_COUNTS[player] < 7) {
        strcpy(BATTLE_LOG[player][BATTLE_LOG_LINE_COUNTS[player]], text);
        BATTLE_LOG_LINE_COUNTS[player]++;
    }
}

void drawBattleLog(s32 player, s32 x, s32 y, s32 z) {
    s32 line;

    for (line = 0; line < 7; line++) {
        drawIconText(x, y, 7, 1, z, (s32)BATTLE_LOG[player][line]);
        y += 12;
    }
}
