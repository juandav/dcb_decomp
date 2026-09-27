#include "dcb/hud_panels.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/battle_hud.h"
#include "dcb/card_motion.h"
#include "dcb/card_render.h"
#include "dcb/duel.h"
#include "dcb/cpu_decision.h"
#include "dcb/duel_setup.h"
#include "dcb/card_zones.h"
#include "dcb/heap.h"
#include "dcb/main.h"
#include "dcb/scene3d.h"
#include "dcb/sound.h"
#include "dcb/stage.h"
#include "dcb/text.h"

void startPanelMove(Panel *panel, s16 targetX, s16 targetY, s32 frames) {
    if (frames == 0) {
        frames = 1;
    }
    panel->unkC |= 0x80;
    if (panel->parent != 0) {
        panel->unkC = panel->parent->unkC;
        panel->unk18 = panel->unk10 - panel->parent->unk10;
        panel->unk1A = panel->unk12 - panel->parent->unk12;
    } else {
        panel->unk18 = panel->unk10;
        panel->unk1A = panel->unk12;
    }
    panel->unk14 = targetX;
    panel->unk16 = targetY;
    panel->unkE = frames;
    panel->unkF = frames;
    panel->unkD++;
}

s32 stepPanelMove(Panel *panel) {
    s16 parentX;
    s16 parentY;

    parentX = 0;
    parentY = 0;
    panel->unkF--;
    if (panel->parent != 0) {
        panel->unkC = panel->parent->unkC;
        parentX = panel->parent->unk10;
        parentY = panel->parent->unk12;
    }
    panel->unk10 = parentX + (panel->unk14 - (panel->unk14 - panel->unk18) * panel->unkF / panel->unkE);
    panel->unk12 = parentY + (panel->unk16 - (panel->unk16 - panel->unk1A) * panel->unkF / panel->unkE);
    if (panel->unkF == 0) {
        panel->unkD++;
    }
    return panel->unkF;
}

void holdPanelAtTarget(Panel *panel) {
    s16 x;
    s16 y;

    x = panel->unk14;
    y = panel->unk16;
    if (panel->parent != 0) {
        panel->unkC = panel->parent->unkC;
        x += panel->parent->unk10;
        y += panel->parent->unk12;
    }
    panel->unk10 = x;
    panel->unk12 = y;
}

void tickDeckPanel(s32 player) {
    void *panel;

    panel = D_801D83EC + (player * 0xD8 + 0x90);
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)panel + 0x10)) = 0x20;
        (*(s16 *)((s8 *)panel + 0x12)) = player * -0x12F + 0xF0;
        break;
    case 1:
        (D_801D83EC + player * 0xD8)[0x55] = 1;
        (D_801D83EC + player * 0xD8)[0xD] = 5;
        (*(u8 *)((s8 *)panel + 0xD)) += 1;
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
        (D_801D83EC + player * 0xD8)[0x55] = 6;
        (D_801D83EC + player * 0xD8)[0xD] = 5;
        (*(u8 *)((s8 *)panel + 0xD)) = 2;
        break;
    case 11:
        startPanelMove(panel, 0xE8, -(player * 0x7D) + 0x99, 0x10);
        break;
    case 12:
        if (stepPanelMove(panel) == 0) {
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
        }
        break;
    }
}

void func_8003DD9C(s32 player) {
    void *panel;
    s32 y;

    panel = D_801D83EC + (player * 0xD8 + 0xB4);
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)panel + 0x10)) = 0x164;
        y = 0x31 - player * 0x31;
        (*(s16 *)((s8 *)panel + 0x12)) = y;
        startPanelMove(panel, 0x164, y, 0);
        stepPanelMove(panel);
        (*(u8 *)((s8 *)panel + 0xD)) = 0;
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
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
        }
        break;
    }
}

void tickStatusPanel(s32 player) {
    void *panel;

    panel = D_801D83EC + (player * 0xD8 + 0x48);
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)panel + 0x10)) = player * 0x2A0 - 0xEC;
        (*(s16 *)((s8 *)panel + 0x12)) = 0x5C;
        break;
    case 1:
        startPanelMove(panel, player * 0x7C + 0x28, 0x5C, 0x10);
        (D_801D83EC + player * 0xD8)[0x79] = 4;
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
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
        }
        break;
    case 6:
        startPanelMove(panel, player * 0xCC, 0x5C, 0x10);
        break;
    case 7:
        if (stepPanelMove(panel) == 0) {
            (D_801D83EC + player * 0xD8)[0x79] = 1;
        }
        break;
    case 8:
        holdPanelAtTarget(panel);
        break;
    }
}

void func_8003E11C(s32 player) {
    void *panel;
    s32 x;
    s32 y;

    panel = D_801D83EC + (player * 0xD8 + 0x6C);
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        x = -(player * 0x41) + 0x44;
        (*(s16 *)((s8 *)panel + 0x10)) = x;
        y = player * 0x1E + 0xA;
        (*(s16 *)((s8 *)panel + 0x12)) = y;
        startPanelMove(panel, x, y, 0);
        stepPanelMove(panel);
        (*(u8 *)((s8 *)panel + 0xD)) = 0;
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
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
        }
        break;
    }
}

void tickAttackPanel(s32 player) {
    void *panel;

    panel = D_801D83EC + (player * 0xD8 + 0x24);
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)panel + 0x10)) = 0x38;
        (*(s16 *)((s8 *)panel + 0x12)) = player * -0x12F + 0xF0;
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
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
        }
        break;
    }
}

void tickCardInfoPanel(s32 player) {
    void *panel;

    panel = D_801D83EC + player * 0xD8;
    switch ((*(u8 *)((s8 *)panel + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)panel + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)panel + 0x10)) = -0xFF;
        (*(s16 *)((s8 *)panel + 0x12)) = player * 0x7E + 0x16;
        break;
    case 1:
        startPanelMove(panel, 0x22, 0x16, 0xC);
        (*(u8 *)((s8 *)panel + 0xD)) = 3;
        break;
    case 2:
        startPanelMove(panel, 0x22, 0x94, 0xA);
        (*(u8 *)((s8 *)panel + 0xD)) = 3;
        break;
    case 3:
        stepPanelMove(panel);
        break;
    case 4:
        holdPanelAtTarget(panel);
        break;
    case 5:
        startPanelMove(panel, -0xFF, (*(s16 *)((s8 *)panel + 0x12)), 0xC);
        break;
    case 6:
        if (stepPanelMove(panel) == 0) {
            (*(u8 *)((s8 *)panel + 0xD)) = 0;
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
        func_8003DD9C(i);
        tickAttackPanel(i);
        tickStatusPanel(i);
        func_8003E11C(i);
        tickCardInfoPanel(i);
    }
    for (player = 0; player < 2; player++) {
        PLAYER(player)->unk11C[4] = sumDigivolvePoints(player);
        if (getActiveDigimonCard(player) == -1) {
            for (i = 0; i < 4; i++) {
                PLAYER(player)->unk126[i] = 0;
                PLAYER(player)->unk11C[i] = 0;
            }
        }
        for (i = 0; i < 5; i++) {
            delta = PLAYER(player)->unk126[i] - PLAYER(player)->unk11C[i];
            step = (delta < 0 ? -delta : delta) / 16 + 1;
            if (PLAYER(player)->unk126[i] < PLAYER(player)->unk11C[i]) {
                PLAYER(player)->unk126[i] += step;
                if (PLAYER(player)->unk126[i] > PLAYER(player)->unk11C[i]) {
                    PLAYER(player)->unk126[i] = PLAYER(player)->unk11C[i];
                }
            } else if (PLAYER(player)->unk126[i] > PLAYER(player)->unk11C[i]) {
                PLAYER(player)->unk126[i] -= step;
                if (PLAYER(player)->unk126[i] < PLAYER(player)->unk11C[i]) {
                    PLAYER(player)->unk126[i] = PLAYER(player)->unk11C[i];
                }
            }
        }
    }
    rollingCount = 0;
    for (player = 0; player < 2; player++) {
        for (i = 0; i < 5; i++) {
            if (PLAYER(player)->unk126[i] != PLAYER(player)->unk11C[i]) {
                rollingCount++;
            }
        }
    }
    if (rollingCount != 0 && !(((Unk8006E050 *)PLAYER_PROFILES)->unk24 & 3)) {
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
