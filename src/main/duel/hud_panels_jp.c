#include "dcb/hud_panels.h"
#include "common.h"
#include "game.h"
#include "dcb/battle_hud.h"
#include "dcb/card_zones.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"

/* draws a panel's quad */
void renderHudPanel(Panel *panel, s32 index, s32 z);
/* the colour of the DP a card brings */
u8 DP_GAIN_RGB[4] = { 0x80, 0x80, 0x80 };

/* jp's HUD panels (hud_panels.c is us's and eu's): eleven per player and a
   shared one, each with a function that moves it by its state: placed out
   of sight, moving in, held, moving out */

/* places the panel */
#define PANEL_PLACE(p, x, y)                                                                            \
    (p)->pos.vx = (x);                                                                                   \
    (p)->pos.vy = (y);                                                                                   \
    (p)->pos.vz = 0

/* starts a move of frames from where the panel is */
#define PANEL_START(p, frames)                                                                          \
    (p)->start.vx = (p)->pos.vx;                                                                         \
    (p)->start.vy = (p)->pos.vy;                                                                         \
    (p)->start.vz = (p)->pos.vz;                                                                         \
    (p)->total = (frames);                                                                               \
    (p)->count = (frames);                                                                               \
    (p)->state++

/* a frame of the move to x, y, z */
#define PANEL_STEP3(p, x, y, z)                                                                         \
    (p)->count--;                                                                                        \
    (p)->pos.vx = (x) - ((x) - (p)->start.vx) * (p)->count / (p)->total;                                \
    (p)->pos.vy = (y) - ((y) - (p)->start.vy) * (p)->count / (p)->total;                                \
    (p)->pos.vz = (z) - ((z) - (p)->start.vz) * (p)->count / (p)->total

/* a frame of the move to x, y */
#define PANEL_STEP(p, x, y)                                                                             \
    (p)->count--;                                                                                        \
    (p)->pos.vx = (x) - ((x) - (p)->start.vx) * (p)->count / (p)->total;                                \
    (p)->pos.vy = (y) - ((y) - (p)->start.vy) * (p)->count / (p)->total;                                \
    (p)->pos.vz = 0 - (0 - (p)->start.vz) * (p)->count / (p)->total

/* the panel all the duel shares */
void tickMessageBarPanel(void) {
    Panel *panel;

    panel = HUD_PANEL(22);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        PANEL_PLACE(panel, -0x180, -0xE);
        break;
    case 1:
        panel->flags |= 0x80;
    case 4:
    case 6:
        PANEL_START(panel, 0xC);
        break;
    case 2:
        if (panel->count != 0) {
            PANEL_STEP(panel, -0xA0, -0xE);
        } else {
            panel->state++;
        }
        break;
    case 3:
        PANEL_PLACE(panel, -0xA0, -0xE);
        break;
    case 5:
        if (panel->count != 0) {
            PANEL_STEP(panel, -0x180, -0xE);
        } else {
            panel->state = 0;
        }
        break;
    case 7:
        if (panel->count != 0) {
            PANEL_STEP(panel, -0x180, -0xE);
        } else {
            panel->state = 1;
        }
        break;
    }
}

void tickShownDigimonPanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, 6);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        PANEL_PLACE(panel, -0x168, -(player * 0x30) + 0x10);
        break;
    case 1:
        panel->flags |= 0x80;
    case 4:
        PANEL_START(panel, 8);
        break;
    case 2:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x30) + 0x10;
            PANEL_STEP(panel, -0xA0, y);
        } else {
            panel->state++;
        }
        break;
    case 3:
        PANEL_PLACE(panel, -0xA0, -(player * 0x30) + 0x10);
        break;
    case 5:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x30) + 0x10;
            PANEL_STEP(panel, -0x168, y);
        } else {
            panel->state = 0;
        }
        break;
    }
}

void tickShownOptionPanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, 7);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        PANEL_PLACE(panel, -0x140, -(player * 0x30) + 0x10);
        break;
    case 1:
        panel->flags |= 0x80;
    case 4:
        PANEL_START(panel, 8);
        break;
    case 2:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x30) + 0x10;
            PANEL_STEP(panel, -0xA0, y);
        } else {
            panel->state++;
        }
        break;
    case 3:
        PANEL_PLACE(panel, -0xA0, -(player * 0x30) + 0x10);
        break;
    case 5:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x30) + 0x10;
            PANEL_STEP(panel, -0x140, y);
        } else {
            panel->state = 0;
        }
        break;
    }
}

void tickDeckNamePanel(s32 player) {
    Panel *panel;
    Panel *other;

    panel = PLAYER_PANEL(player, 3);
    other = PLAYER_PANEL(player, 10);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        PANEL_PLACE(panel, -0x188, -(player * 0x44) + 2);
        break;
    case 1:
        panel->flags |= 0x80;
        PANEL_START(panel, 0x10);
        break;
    case 2:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x44) + 2;
            PANEL_STEP(panel, -0xA0, y);
        } else {
            PANEL_START(panel, 0xC);
        }
        break;
    case 3:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x70) + 0x18;
            PANEL_STEP(panel, -0xA0, y);
        } else {
            panel->state++;
            HUD_PANEL(22)->state = 1;
            other->state = 1;
        }
        break;
    case 4:
        PANEL_PLACE(panel, -0xA0, -(player * 0x70) + 0x18);
        break;
    case 5:
    case 8:
        other->state = 4;
        panel->flags |= 0x80;
        PANEL_START(panel, 0x10);
        break;
    case 6:
        if (panel->count != 0) {
            s16 y;

            y = -(player << 6) - 0x30;
            PANEL_STEP(panel, -0xA0, y);
        } else {
            panel->state++;
            HUD_PANEL(4)->state = 1;
            HUD_PANEL(5)->state = 1;
            switch (PLAYER_CARDS(PLAYER(0))[((CardCursor *)DUEL->cursor)->id % 30].type) {
            case 0:
                HUD_PANEL(6)->state = 1;
                break;
            case 1:
            case 2:
                HUD_PANEL(7)->state = 1;
                break;
            }
        }
        break;
    case 7:
        PANEL_PLACE(panel, -0xA0, -(player << 6) - 0x30);
        break;
    case 9:
        if (panel->count != 0) {
            s16 y;

            y = -(player << 6) + 0x30;
            PANEL_STEP(panel, -0xA0, y);
        } else {
            panel->state++;
            HUD_PANEL(15)->state = 1;
            HUD_PANEL(16)->state = 1;
            switch (PLAYER_CARDS(PLAYER(1))[((CardCursor *)DUEL->cursor)->id % 30].type) {
            case 0:
                HUD_PANEL(17)->state = 1;
                break;
            case 1:
            case 2:
                HUD_PANEL(18)->state = 1;
                break;
            }
        }
        break;
    case 10:
        PANEL_PLACE(panel, -0xA0, -(player << 6) + 0x30);
        break;
    case 11:
        other->state = 4;
        PANEL_START(panel, 0x10);
        break;
    case 12:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x70) + 0x18;
            PANEL_STEP(panel, -0x188, y);
        } else {
            panel->state = 0;
        }
        break;
    case 13:
        panel->flags |= 0x80;
        panel->start.vx = panel->pos.vx;
        panel->start.vy = -(player * 0x70) + 0x18;
        panel->start.vz = panel->pos.vz;
        panel->total = 0x10;
        panel->count = 0x10;
        panel->state = 3;
        break;
    case 14:
        other->state = 4;
        PANEL_START(panel, 0x10);
        break;
    case 15:
        if (panel->count != 0) {
            panel->count--;
            panel->pos.vx = -0x188 - (-0x188 - panel->start.vx) * panel->count / panel->total;
            panel->pos.vz = 0 - (0 - panel->start.vz) * panel->count / panel->total;
        } else {
            panel->state++;
            HUD_PANEL(22)->state = 1;
            PLAYER_PANEL(player, 8)->state = 1;
        }
        break;
    case 17:
        other->state = 4;
        PANEL_START(panel, 0x10);
        break;
    case 18:
        if (panel->count != 0) {
            panel->count--;
            panel->pos.vx = -0x188 - (-0x188 - panel->start.vx) * panel->count / panel->total;
            panel->pos.vz = 0 - (0 - panel->start.vz) * panel->count / panel->total;
        } else {
            panel->state++;
            HUD_PANEL(22)->state = 1;
            PLAYER_PANEL(player, 9)->state = 1;
        }
        break;
    case 16:
    case 19:
        panel->flags &= 0x7F;
        PANEL_PLACE(panel, -0x188, -(player * 0x70) + 0x18);
        break;
    }
}

void tickPlayerNamePanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, 10);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        PANEL_PLACE(panel, -0x120, -(player * 0xC0) + 0x58);
        break;
    case 1:
        panel->flags |= 0x80;
    case 4:
        PANEL_START(panel, 0xC);
        break;
    case 2:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0xC0) + 0x58;
            PANEL_STEP(panel, -0x40, y);
        } else {
            panel->state++;
        }
        break;
    case 3:
        PANEL_PLACE(panel, -0x40, -(player * 0xC0) + 0x58);
        break;
    case 5:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0xC0) + 0x58;
            PANEL_STEP(panel, -0x120, y);
        } else {
            panel->state = 0;
        }
        break;
    }
}

void tickAttackListPanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, 8);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        PANEL_PLACE(panel, -0x178, -(player * 0x74) + 0x12);
        break;
    case 1:
        panel->flags |= 0x80;
    case 4:
    case 6:
        PANEL_START(panel, 0x10);
        break;
    case 2:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x74) + 0x12;
            PANEL_STEP(panel, -0xA0, y);
        } else {
            panel->state++;
        }
        break;
    case 3:
        PANEL_PLACE(panel, -0xA0, -(player * 0x74) + 0x12);
        break;
    case 5:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x74) + 0x12;
            PANEL_STEP(panel, -0x178, y);
        } else {
            panel->state = 0;
            HUD_PANEL(22)->state = 4;
            PLAYER_PANEL(player, 3)->state = DUEL->viewPlayer * 3 + 5;
            if (DUEL->viewPlayer == 0) {
                PLAYER_PANEL(player, 3)->pos.vy = -(player << 6) - 0x30;
            } else {
                PLAYER_PANEL(player, 3)->pos.vy = -(player << 6) + 0x30;
            }
        }
        break;
    case 7:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x74) + 0x12;
            PANEL_STEP(panel, -0x178, y);
        } else {
            panel->state = 0;
            PLAYER_PANEL(player, 3)->state = 0xD;
        }
        break;
    }
}

void tickBattleLogPanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, 9);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        PANEL_PLACE(panel, -0x150, -(player * 0x80) + 0x10);
        break;
    case 1:
        panel->flags |= 0x80;
    case 4:
        PANEL_START(panel, 0x10);
        break;
    case 2:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x80) + 0x10;
            PANEL_STEP(panel, -0x9A, y);
        } else {
            panel->state++;
        }
        break;
    case 3:
        PANEL_PLACE(panel, -0x9A, -(player * 0x80) + 0x10);
        break;
    case 5:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x80) + 0x10;
            PANEL_STEP(panel, -0x150, y);
        } else {
            panel->state = 0;
        }
        break;
    }
}

void tickActiveDigimonPanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, 0);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        PANEL_PLACE(panel, 0xE7, -(player * 0x4E));
        break;
    case 1:
        panel->flags |= 0x80;
    case 4:
        PANEL_START(panel, 0x10);
        break;
    case 2:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x4E);
            PANEL_STEP(panel, 0x43, y);
        } else {
            panel->state++;
            PLAYER_PANEL(player, 1)->state = 1;
        }
        break;
    case 3:
        PANEL_PLACE(panel, 0x43, -(player * 0x4E));
        break;
    case 5:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x52) + 2;
            PANEL_STEP(panel, 0xE7, y);
        } else {
            panel->state = 0;
        }
        break;
    }
}

void tickDpPanel(s32 player) {
    Panel *parent;
    Panel *panel;

    parent = PLAYER_PANEL(player, 0);
    panel = PLAYER_PANEL(player, 1);
    panel->flags = parent->flags;
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        {
            s32 y;

            panel->pos.vx = parent->pos.vx + 0x10;
            y = player * -0xC7 + 0x6F;
            panel->pos.vy = y + parent->pos.vy;
            panel->pos.vz = parent->pos.vz;
        }
        break;
    case 2:
        if (panel->count != 0) {
            s16 x;
            s16 y;
            s16 z;
            s32 offset;

            x = parent->pos.vx + 0x10;
            y = parent->pos.vy;
            offset = player * -0x57 + 0x37;
            y += offset;
            z = parent->pos.vz;
            PANEL_STEP3(panel, x, y, z);
        } else {
            panel->state++;
        }
        break;
    case 3:
        {
            s32 y;

            panel->pos.vx = parent->pos.vx + 0x10;
            y = player * -0x57 + 0x37;
            panel->pos.vy = y + parent->pos.vy;
            panel->pos.vz = parent->pos.vz;
        }
        break;
    case 1:
    case 4:
        panel->flags |= 0x80;
        PANEL_START(panel, 0xC);
        break;
    case 5:
        if (panel->count != 0) {
            s16 x;
            s16 y;
            s16 z;
            s32 offset;

            x = parent->pos.vx + 0x10;
            y = parent->pos.vy;
            offset = player * -0xC7 + 0x6F;
            y += offset;
            z = parent->pos.vz;
            PANEL_STEP3(panel, x, y, z);
        } else {
            panel->state = 0;
        }
        break;
    }
    panel->clut = GetClut(0x370, 0x1AA - countEmptyDpSlots(player));
}

void func_8003E2A4(s32 player) {
    Panel *parent;
    Panel *panel;

    parent = PLAYER_PANEL(player, 0);
    panel = PLAYER_PANEL(player, 2);
    panel->flags = parent->flags;
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        {
            s32 y;

            panel->pos.vx = parent->pos.vx - 4;
            y = -(player * 0xF4) + 0x7F;
            panel->pos.vy = y + parent->pos.vy;
            panel->pos.vz = parent->pos.vz;
        }
        break;
    case 2:
        if (panel->count != 0) {
            s16 x;
            s16 y;
            s16 z;

            x = parent->pos.vx - 4;
            y = parent->pos.vy - player * 0x64 + 0x37;
            z = parent->pos.vz;
            PANEL_STEP3(panel, x, y, z);
        } else {
            panel->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 3:
        {
            s32 y;

            panel->pos.vx = parent->pos.vx - 4;
            y = -(player * 0x64) + 0x37;
            panel->pos.vy = y + parent->pos.vy;
            panel->pos.vz = parent->pos.vz;
        }
        break;
    case 1:
    case 4:
        panel->flags |= 0x80;
        PANEL_START(panel, 0xC);
        break;
    case 5:
        if (panel->count != 0) {
            s16 x;
            s16 y;
            s16 z;

            x = parent->pos.vx - 4;
            y = parent->pos.vy - player * 0xF4 + 0x7F;
            z = parent->pos.vz;
            PANEL_STEP3(panel, x, y, z);
        } else {
            panel->state = 0;
        }
        break;
    }
    panel->clut = GetClut(0x370, 0x1AB);
}

void tickCardTextPanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, 5);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        PANEL_PLACE(panel, -0x52, -(player * 0x160) + 0x80);
        break;
    case 1:
    case 4:
        panel->flags |= 0x80;
        PANEL_START(panel, 0xC);
        break;
    case 2:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0xA0) + 0x20;
            PANEL_STEP(panel, -0x52, y);
        } else {
            panel->state++;
        }
        break;
    case 3:
        PANEL_PLACE(panel, -0x52, -(player * 0xA0) + 0x20);
        break;
    case 5:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x160) + 0x80;
            PANEL_STEP(panel, -0x52, y);
        } else {
            panel->state = 0;
        }
        break;
    }
}

void tickCardArtPanel(s32 player) {
    Panel *panel;

    panel = PLAYER_PANEL(player, 4);
    switch (panel->state) {
    case 0:
        panel->flags &= 0x7F;
        PANEL_PLACE(panel, -0xF0, -(player * 0x42) + 0x20);
        break;
    case 1:
    case 4:
        panel->flags |= 0x80;
        PANEL_START(panel, 0xC);
        break;
    case 2:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x42) + 0x20;
            PANEL_STEP(panel, -0xA0, y);
        } else {
            panel->state++;
        }
        break;
    case 3:
        PANEL_PLACE(panel, -0xA0, -(player * 0x42) + 0x20);
        break;
    case 5:
        if (panel->count != 0) {
            s16 y;

            y = -(player * 0x42) + 0x20;
            PANEL_STEP(panel, -0xF0, y);
        } else {
            panel->state = 0;
        }
        break;
    }
}

void tickBattleHud(void) {
    char text[16];
    s32 player;
    s32 i;
    s32 delta;
    s32 step;
    s32 rollingCount;
    s32 card;
    s32 dx;

    tickMessageBarPanel();
    for (i = 0; i < 2; i++) {
        tickShownDigimonPanel(i);
        tickShownOptionPanel(i);
        tickDeckNamePanel(i);
        tickPlayerNamePanel(i);
        tickAttackListPanel(i);
        tickBattleLogPanel(i);
        tickActiveDigimonPanel(i);
        tickDpPanel(i);
        func_8003E2A4(i);
        tickCardTextPanel(i);
        tickCardArtPanel(i);
    }
    for (player = 0; player < 2; player++) {
        PLAYER(player)->stats[4] = sumDigivolvePoints(player);
        if (getActiveDigimonCard(player) == -1) {
            for (i = 0; i < 4; i++) {
                PLAYER(player)->displayedStats[i] = PLAYER(player)->stats[i] = 0;
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
    /* the DP a card brings, rising over the DP slots */
    for (player = 0; player < 2; player++) {
        if (PLAYER(player)->dpGainTimer != 0) {
            PLAYER(player)->dpGainTimer--;
            card = peekDpSlotTop(player);
            if (card != -1) {
                if (PLAYER(player)->dpGainTimer >= 0x5D) {
                    dx = (0x64 - PLAYER(player)->dpGainTimer) * 10;
                } else if (PLAYER(player)->dpGainTimer < 8) {
                    dx = PLAYER(player)->dpGainTimer * 10;
                } else {
                    dx = 0x50;
                }
                sprintf(text, "+%dp", CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, dpBonus));
                drawBigDigits(SPRITE(card)->sx + 10 - dx, SPRITE(card)->sy, (u8 *)text, DP_GAIN_RGB, 0);
            }
        }
    }
    for (i = 0; i < 23; i++) {
        if (HUD_PANEL(i)->flags & 0x80) {
            renderHudPanel(HUD_PANEL(i), i, i * 2 + 0xC9);
            drawHudPanelContents(i, i * 2 + 0xC8);
        }
    }
}
