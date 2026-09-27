#include "dcb/card_motion.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/battle_hud.h"
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
#include "dcb/sound.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/text.h"

void tickCardMotion(s32 cardIndex, s32 player) {
    CardAnim *anim;

    anim = (CardAnim *)(D_801D833C + cardIndex * 36);
    anim->spr->flags |= 0x80;
    switch (SPRITE_KIND(cardIndex)) {
    case 0:
        anim->spr->pos.vx = SLOT(player, 0x90)->x - 0x80 + player * 0xBE;
        anim->spr->pos.vy = SLOT(player, 0x90)->y - 0x54 + player * 0xE;
        anim->spr->pos.vz = 0;
        UNK7F8(cardIndex).rx = 0x2000;
        UNK7F8(cardIndex).ry = 0x2800;
        UNK7F8(cardIndex).rz = 0x1C00;
        anim->spr->scale = 0x800;
        anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x90)->unkC;
        anim->count = 0;
        break;
    case 1:
    case 21:
    case 26:
        ANIM_SAVE(anim);
        anim->total = 0x10;
        anim->count = 0x10;
        playSoundEffect(0xA5);
        anim->state++;
        break;
    case 2:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x90)->x - 0x80 + player * 0xBE);
            targetY = (s16)(SLOT(player, 0x90)->y - 0x54 + player * 0xE);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x1C00;
            scale = 0x800;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state = 0;
        }
        break;
    case 3:
        ANIM_SAVE(anim);
        anim->total = 0x10;
        anim->count = 0x10;
        playSoundEffect(0xA5);
        anim->state++;
    case 4:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x90)->x - 0x5C + player * -10 + anim->unk23 * 0x2B);
            targetY = (s16)(SLOT(player, 0x90)->y - 0x69 + player * 0x21);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
            if (anim->count != 0) {
                break;
            }
        }
        anim->total = 4;
        anim->count = 4;
        anim->state++;
        playSoundEffect(0xA7);
        break;
    case 5:
    case 13:
        if (--anim->count == 0) {
            ANIM_SAVE(anim);
            anim->total = 0xE;
            anim->count = 0xC;
            anim->state++;
        }
        break;
    case 6:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x90)->x - 0x5C + player * -10 + anim->unk23 * 0x2B);
            targetY = (s16)(SLOT(player, 0x90)->y - 0x61 + player * 0x11);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 7:
        anim->spr->pos.vx = SLOT(player, 0x90)->x - 0x5C + player * -10 + anim->unk23 * 0x2B;
        anim->spr->pos.vy = SLOT(player, 0x90)->y - 0x61 + player * 0x11;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2000;
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x1000;
        anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x90)->unkC;
        break;
    case 8:
        ANIM_SAVE(anim);
        anim->total = 0x10;
        anim->count = 0x10;
        playSoundEffect(0xA5);
        anim->state++;
    case 9:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x90)->x - 0x80 + player * 0xBE);
            targetY = (s16)(SLOT(player, 0x90)->y - 0x6C + player * 0xE);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2400;
            scale = 0x800;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 10:
        anim->spr->pos.vx = SLOT(player, 0x90)->x - 0x80 + player * 0xBE;
        anim->spr->pos.vy = SLOT(player, 0x90)->y - 0x6C + player * 0xE;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2000;
        anim->spr->rot.vz = 0x2400;
        anim->spr->scale = 0x800;
        anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x90)->unkC;
        break;
    case 11:
        ANIM_SAVE(anim);
        anim->total = 0x10;
        anim->count = 0x10;
        playSoundEffect(0xA5);
        anim->state++;
    case 12: {
        s32 stackDepth;
        s32 i;

            stackDepth = 0;
            if (anim->count != 0) {
                s32 targetX;
                s32 targetY;
                s16 rx;
                s16 ry;
                s16 rz;
                s16 scale;

                for (i = 2; i >= 0; i--) {
                    if (cardIndex == ((Player *)DUEL_PLAYERS[player])->unk1CA[i]) {
                        break;
                    }
                    stackDepth++;
                }
                targetX = (s16)(SLOT(player, 0x48)->x + (s16)(stackDepth * 2 - 0x46) + (s16)((-0x40 - (stackDepth * 2 + 8) * 2) * player + 8));
                targetY = (s16)(SLOT(player, 0x48)->y - 0x54);
                rx = 0x2000;
                ry = 0x2000;
                rz = 0x2000;
                scale = 0x1000;
                ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
            } else {
                playSoundEffect(0xA7);
                anim->total = 4;
                anim->count = 4;
                anim->state++;
            }
            break;
    }
    case 14: {
        s32 stackDepth;
        s32 i;

            stackDepth = 0;
            if (anim->count != 0) {
                s32 targetX;
                s32 targetY;
                s16 rx;
                s16 ry;
                s16 rz;
                s16 scale;

                for (i = 2; i >= 0; i--) {
                    if (cardIndex == ((Player *)DUEL_PLAYERS[player])->unk1CA[i]) {
                        break;
                    }
                    stackDepth++;
                }
                targetX = (s16)(SLOT(player, 0x48)->x + (s16)(stackDepth * 2 - 0x46) + (s16)((-0x40 - stackDepth * 4) * player));
                targetY = (s16)(SLOT(player, 0x48)->y - 0x54);
                rx = 0x2000;
                ry = 0x2000;
                rz = 0x2000;
                scale = 0x1000;
                ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
            } else {
                anim->state++;
                playSoundEffect(0xA7);
            }
            break;
    }
    case 15: {
        s32 stackDepth;
        s32 i;

            stackDepth = 0;
            for (i = 2; i >= 0; i--) {
                if (cardIndex == ((Player *)DUEL_PLAYERS[player])->unk1CA[i]) {
                    break;
                }
                stackDepth++;
            }
            anim->spr->pos.vx = SLOT(player, 0x48)->x - 0x46 + stackDepth * 2 + (-0x40 - stackDepth * 4) * player;
            anim->spr->pos.vy = SLOT(player, 0x48)->y - 0x54;
            anim->spr->pos.vz = 0;
            anim->spr->rot.vx = 0x2000;
            anim->spr->rot.vy = 0x2000;
            anim->spr->rot.vz = 0x2000;
            anim->spr->scale = 0x1000;
            anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x48)->unkC;
            break;
    }
    case 16:
        ANIM_SAVE(anim);
        anim->total = 0x10;
        anim->count = 0x10;
        if (anim->spr->rot.vy == 0x2000) {
            playSoundEffect(0xA5);
        } else {
            playSoundEffect(0xA6);
        }
        anim->state++;
        break;
    case 17:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x6C)->x - 0x89 + player * -1);
            targetY = (s16)(SLOT(player, 0x6C)->y - 0x50 + player * -0x3E);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            playSoundEffect(0xA7);
            anim->total = 4;
            anim->count = 4;
            anim->state++;
        }
        break;
    case 18:
        if (--anim->count == 0) {
            ANIM_SAVE(anim);
            anim->total = 4;
            anim->count = 4;
            anim->state++;
        }
        break;
    case 19:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x6C)->x - 0x89 + player * -1);
            targetY = (s16)(SLOT(player, 0x6C)->y - 0x58 + player * -0x2E);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 20:
        anim->spr->pos.vx = SLOT(player, 0x6C)->x - player - 0x89;
        anim->spr->pos.vy = SLOT(player, 0x6C)->y - player * 0x2E - 0x58;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2000;
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x1000;
        anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x6C)->unkC;
        break;
    case 22:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x6C)->x - 0x89 + player * -1);
            targetY = (s16)(SLOT(player, 0x6C)->y - 0x50 + player * -0x3E);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->total = 0x20;
            anim->count = 0x20;
            anim->state++;
        }
        break;
    case 23:
        if (anim->count != 0) {
            anim->count--;
            ANIM_SAVE(anim);
            anim->total = 4;
            anim->count = 4;
            anim->state++;
        }
        break;
    case 24:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x6C)->x - 0x89 + player * -1);
            targetY = (s16)(SLOT(player, 0x6C)->y - 0x58 + player * -0x2E);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2000;
            scale = 0x1000;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 25:
        anim->spr->pos.vx = SLOT(player, 0x6C)->x - player - 0x89;
        anim->spr->pos.vy = SLOT(player, 0x6C)->y - player * 0x2E - 0x58;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2800;
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x1000;
        break;
    case 27:
        if (anim->count != 0) {
            s32 targetX;
            s32 targetY;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 scale;

            targetX = (s16)(SLOT(player, 0x48)->x - 0x94 + player * 0x5D);
            targetY = (s16)(SLOT(player, 0x48)->y - 0x54);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            scale = 0x800;
            ANIM_STEP(anim, targetX, targetY, rx, ry, rz, scale);
        } else {
            anim->state++;
            showDpGainPopup(player);
            playSoundEffect(0xA7);
        }
        break;
    case 28:
        anim->spr->pos.vx = SLOT(player, 0x48)->x - 0x94 + player * 0x5D;
        anim->spr->pos.vy = SLOT(player, 0x48)->y - 0x54;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2000;
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x800;
        anim->spr->flags = (anim->spr->flags & 0x7F) | SLOT(player, 0x48)->unkC;
        break;
    case 29:
        ANIM_SAVE(anim);
        anim->total = 0x20;
        anim->count = 0x20;
        anim->state++;
        break;
    case 30:
        if (anim->count != 0) {
            s32 targetY;
            s16 r;

            targetY = (s16)(0x3C - player * 0x78);
            r = 0x2000;
            ANIM_STEP(anim, 0, targetY, r, r, r, r);
        } else {
            anim->state++;
        }
        break;
    case 31:
        anim->spr->pos.vx = 0;
        anim->spr->pos.vy = 0x3C - player * 0x78;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2000;
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x2000;
        break;
    case 32:
        ANIM_SAVE(anim);
        anim->total = 0x20;
        anim->count = 0x20;
        playSoundEffect(0xA6);
        anim->state++;
        break;
    case 33:
        if (anim->count != 0) {
            s32 targetY;
            s16 ry;

            s16 r;

            targetY = (s16)(0xA0 - player * 0x140);
            r = 0x2000;
            ry = 0x2800 - (player << 12);
            ANIM_STEP(anim, 0, targetY, r, ry, r, r);
        } else {
            anim->state++;
        }
        break;
    case 34:
        anim->spr->pos.vx = 0;
        anim->spr->pos.vy = 0xA0 - player * 0x140;
        anim->spr->pos.vz = 0;
        anim->spr->rot.vx = 0x2000;
        anim->spr->rot.vy = 0x2800 - (player << 12);
        anim->spr->rot.vz = 0x2000;
        anim->spr->scale = 0x2000;
        break;
    }
}

void renderBoardCards(void) {
    char text[8];
    Rect16 hpLabelRect;
    u8 labelRgb[4] = "@@@";
    s32 i;
    s32 j;
    s32 hpLabelDrawn;
    s8 card;
    s32 hpColor;
    s32 z;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 30; j++) {
            tickCardMotion(i * 30 + j, i);
        }
    }
    resetCardPolyCount();
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            card = PLAYER(i)->unk1C2[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), card);
                if (SPRITE_KIND(card) == 0x1C) {
                    break;
                }
            }
        }
        card = PLAYER(i)->unk1CD;
        if (card >= 0) {
            renderCardSprite(SPRITE(card), card);
        }
        hpLabelDrawn = 0;
        for (j = 0; j < 3; j++) {
            card = PLAYER(i)->unk1CA[j];
            if (card >= 0) {
                if (!hpLabelDrawn) {
                    ((u8 *)SPRITE(card))[0x14] = PLAYER(i)->unk178_19;
                    hpLabelDrawn = 1;
                    if (SPRITE_KIND(card) < 0x1D) {
                        hpColor = PLAYER(i)->unk178_15 ? 3 : 7;
                        projectCardSprite(SPRITE(card), card);
                        z = *(s32 *)((u8 *)SPRITE(card) + 0x38);
                        drawIcon(*(s16 *)((u8 *)SPRITE(card) + 0x34) + 2, *(s16 *)((u8 *)SPRITE(card) + 0x36) + 30, 0,
                                      0x1A, z);
                        sprintf(text, "%4d", PLAYER(i)->unk126[0]);
                        drawText(*(s16 *)((u8 *)SPRITE(card) + 0x34) + 15, *(s16 *)((u8 *)SPRITE(card) + 0x36) + 30,
                                      (s32)text, hpColor, z);
                        hpLabelRect.x = 0x60;
                        hpLabelRect.y = 0xDB;
                        hpLabelRect.w = 0x26;
                        hpLabelRect.h = 0xC;
                        drawPageSpriteColored(*(s16 *)((u8 *)SPRITE(card) + 0x34) + 1, *(s16 *)((u8 *)SPRITE(card) + 0x36) + 30,
                                      &hpLabelRect, labelRgb, getTPage(0, 2, SYSTEM_TEX_X, SYSTEM_TEX_Y), 0xC, z);
                        hpLabelDrawn = 1;
                    }
                }
                renderCardSprite(SPRITE(card), card);
            }
        }
        for (j = 0; j < 30; j++) {
            card = PLAYER(i)->unk19B[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), card);
                if (SPRITE_KIND(card) == 10) {
                    break;
                }
            }
        }
        for (j = 3; j >= 0; j--) {
            card = PLAYER(i)->unk1B9[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), card);
            }
        }
        for (j = 0; j < 30; j++) {
            card = PLAYER(i)->unk17D[j];
            if (card >= 0) {
                renderCardSprite(SPRITE(card), card);
                if (SPRITE_KIND(card) == 0) {
                    break;
                }
            }
        }
    }
}

/* the original file padded its strings with an empty word here */
__asm__(".section .rodata\n\t.word 0\n\t.section .text\n");
