#include "dcb/hud_panels.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/battle_hud.h"
#include "dcb/card_motion.h"
#include "dcb/duel_launch.h"
#include "dcb/card_render.h"
#include "dcb/duel.h"
#include "dcb/cpu_decision.h"
#include "dcb/duel_setup.h"
#include "dcb/card_zones.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
#include "dcb/camera.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
#include "dcb/scroll_bg.h"
#include "dcb/text.h"
#include "dcb/str_util.h"

void startPanelMove(Panel *panel, s16 targetX, s16 targetY, s32 frames) {
    if (frames == 0) {
        frames = 1;
    }
    panel->flags |= 0x80;
    if (panel->parent != 0) {
        panel->flags = panel->parent->flags;
        panel->startX = panel->x - panel->parent->x;
        panel->startY = panel->y - panel->parent->y;
    } else {
        panel->startX = panel->x;
        panel->startY = panel->y;
    }
    panel->targetX = targetX;
    panel->targetY = targetY;
    panel->total = frames;
    panel->count = frames;
    panel->state++;
}

s32 stepPanelMove(Panel *panel) {
    s16 parentX;
    s16 parentY;

    parentX = 0;
    parentY = 0;
    panel->count--;
    if (panel->parent != 0) {
        panel->flags = panel->parent->flags;
        parentX = panel->parent->x;
        parentY = panel->parent->y;
    }
    panel->x = parentX + (panel->targetX - (panel->targetX - panel->startX) * panel->count / panel->total);
    panel->y = parentY + (panel->targetY - (panel->targetY - panel->startY) * panel->count / panel->total);
    if (panel->count == 0) {
        panel->state++;
    }
    return panel->count;
}

void holdPanelAtTarget(Panel *panel) {
    s16 x;
    s16 y;

    x = panel->targetX;
    y = panel->targetY;
    if (panel->parent != 0) {
        panel->flags = panel->parent->flags;
        x += panel->parent->x;
        y += panel->parent->y;
    }
    panel->x = x;
    panel->y = y;
}

void tickDeckPanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, HUD_DECK);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        panel->x = 0x20;
        panel->y = player * -0x12F + 0xF0;
        break;
    case 1:
        PLAYER_PANEL(player, HUD_STATUS)->state = 1;
        PLAYER_PANEL(player, HUD_CARD_INFO)->state = 5;
        panel->state++;
        break;
    case 2:
        startPanelMove(panel, 0x20, -(player * 0x7D) + 0x99, 0x10);
        break;
    case 3:
        stepPanelMove(panel);
        break;
    case 4:
        holdPanelAtTarget(panel);
        break;
    case 7:
        PLAYER_PANEL(player, HUD_STATUS)->state = 6;
        PLAYER_PANEL(player, HUD_CARD_INFO)->state = 5;
        panel->state = 2;
        break;
    case 11:
        startPanelMove(panel, 0xE8, -(player * 0x7D) + 0x99, 0x10);
        break;
    case 12:
        if (stepPanelMove(panel) == 0) {
            panel->state = 0;
        }
        break;
    }
}

void tickTurnMarkerPanel(s32 player) {
    Panel *panel;
    s32 y;

    panel = PLAYER_PANEL(player, HUD_TURN_MARKER);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        panel->x = 0x164;
        y = 0x31 - player * 0x31;
        panel->y = y;
        startPanelMove(panel, 0x164, y, 0);
        stepPanelMove(panel);
        panel->state = 0;
        break;
    case 1:
        startPanelMove(panel, 0x100, 0x31 - player * 0x31, 8);
        break;
    case 2:
        if (stepPanelMove(panel) == 0) {
            playSoundEffect(0xA7);
        }
        break;
    case 3:
        startPanelMove(panel, 0xF9, 0x31 - player * 0x31, 8);
        break;
    case 4:
        stepPanelMove(panel);
        break;
    case 5:
        holdPanelAtTarget(panel);
        break;
    case 6:
        startPanelMove(panel, 0x164, 0x31 - player * 0x31, 8);
        break;
    case 7:
        if (stepPanelMove(panel) == 0) {
            panel->state = 0;
        }
        break;
    }
}

void tickStatusPanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, HUD_STATUS);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        panel->x = player * 0x2A0 - 0xEC;
        panel->y = 0x5C;
        break;
    case 1:
        startPanelMove(panel, player * 0x7C + 0x28, 0x5C, 0x10);
        PLAYER_PANEL(player, HUD_PLAYED_CARD)->state = 4;
        break;
    case 2:
        stepPanelMove(panel);
        break;
    case 3:
        holdPanelAtTarget(panel);
        break;
    case 4:
        startPanelMove(panel, player * 0x2A0 - 0xEC, 0x5C, 0x10);
        break;
    case 5:
        if (stepPanelMove(panel) == 0) {
            panel->state = 0;
        }
        break;
    case 6:
        startPanelMove(panel, player * 0xCC, 0x5C, 0x10);
        break;
    case 7:
        if (stepPanelMove(panel) == 0) {
            PLAYER_PANEL(player, HUD_PLAYED_CARD)->state = 1;
        }
        break;
    case 8:
        holdPanelAtTarget(panel);
        break;
    }
}

void tickPlayedCardPanel(s32 player) {
    Panel *panel;
    s32 x;
    s32 y;

    panel = PLAYER_PANEL(player, HUD_PLAYED_CARD);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        x = -(player * 0x41) + 0x44;
        panel->x = x;
        y = player * 0x1E + 0xA;
        panel->y = y;
        startPanelMove(panel, x, y, 0);
        stepPanelMove(panel);
        panel->state = 0;
        break;
    case 1:
        startPanelMove(panel, -(player * 0xA1) + 0x74, player * 0x1E + 0xA, 8);
        break;
    case 2:
        if (stepPanelMove(panel) == 0) {
            playSoundEffect(0xA7);
        }
        break;
    case 3:
        holdPanelAtTarget(panel);
        break;
    case 4:
        startPanelMove(panel, -(player * 0x41) + 0x44, player * 0x1E + 0xA, 8);
        break;
    case 5:
        if (stepPanelMove(panel) == 0) {
            panel->state = 0;
        }
        break;
    }
}

void tickAttackPanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, HUD_ATTACK);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        panel->x = 0x38;
        panel->y = player * -0x12F + 0xF0;
        break;
    case 1:
        startPanelMove(panel, 0x38, player * -0x7F + 0x99, 0x10);
        break;
    case 2:
        stepPanelMove(panel);
        break;
    case 3:
        holdPanelAtTarget(panel);
        break;
    case 4:
        startPanelMove(panel, 0x38, player * -0x12F + 0xF0, 0x10);
        break;
    case 5:
        if (stepPanelMove(panel) == 0) {
            panel->state = 0;
        }
        break;
    }
}

void tickCardInfoPanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, HUD_CARD_INFO);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        panel->x = -0xFF;
        panel->y = player * 0x7E + 0x16;
        break;
    case 1:
        startPanelMove(panel, 0x22, 0x16, 0xC);
        panel->state = 3;
        break;
    case 2:
        startPanelMove(panel, 0x22, 0x94, 0xA);
        panel->state = 3;
        break;
    case 3:
        stepPanelMove(panel);
        break;
    case 4:
        holdPanelAtTarget(panel);
        break;
    case 5:
        startPanelMove(panel, -0xFF, panel->y, 0xC);
        break;
    case 6:
        if (stepPanelMove(panel) == 0) {
            panel->state = 0;
        }
        break;
    }
}

void tickBattleHud(void) {
    s32 player;
    s32 i;
    s32 delta;
    s32 step;
    s32 rollingCount;

    for (i = 0; i < 2; i++) {
        tickDeckPanel(i);
        tickTurnMarkerPanel(i);
        tickAttackPanel(i);
        tickStatusPanel(i);
        tickPlayedCardPanel(i);
        tickCardInfoPanel(i);
    }
    for (player = 0; player < 2; player++) {
        PLAYER(player)->stats[4] = sumDigivolvePoints(player);
        if (getActiveDigimonCard(player) == -1) {
            for (i = 0; i < 4; i++) {
                PLAYER(player)->displayedStats[i] = 0;
                PLAYER(player)->stats[i] = 0;
            }
        }
        /* roll each displayed stat toward the real one, 1/16 of the gap per frame */
        for (i = 0; i < 5; i++) {
            delta = PLAYER(player)->displayedStats[i] - PLAYER(player)->stats[i];
            step = (delta < 0 ? -delta : delta) / 16 + 1;
            if (PLAYER(player)->displayedStats[i] < PLAYER(player)->stats[i]) {
                PLAYER(player)->displayedStats[i] += step;
                if (PLAYER(player)->displayedStats[i] > PLAYER(player)->stats[i]) {
                    PLAYER(player)->displayedStats[i] = PLAYER(player)->stats[i];
                }
            } else if (PLAYER(player)->displayedStats[i] > PLAYER(player)->stats[i]) {
                PLAYER(player)->displayedStats[i] -= step;
                if (PLAYER(player)->displayedStats[i] < PLAYER(player)->stats[i]) {
                    PLAYER(player)->displayedStats[i] = PLAYER(player)->stats[i];
                }
            }
        }
    }
    /* tick sound every 4 frames while a counter is still rolling */
    rollingCount = 0;
    for (player = 0; player < 2; player++) {
        for (i = 0; i < 5; i++) {
            if (PLAYER(player)->displayedStats[i] != PLAYER(player)->stats[i]) {
                rollingCount++;
            }
        }
    }
    if (rollingCount != 0 && !(((PlayerProfile *)PLAYER_PROFILES)->playTime & 3)) {
        playSoundEffect(0xAA);
    }
    renderStatPopups();
    for (i = 0; i < 12; i++) {
        if (PANEL(i).flags & 0x80) {
            drawHudSprite((SprtInfo *)&PANEL(i), i, i * 2 + PANEL(i).z + 1);
            drawHudPanelContents(i, i * 2 + PANEL(i).z);
        }
    }
}
