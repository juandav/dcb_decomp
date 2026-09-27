#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/battle_hud.h"
#include "dcb/card_render.h"
#include "dcb/duel.h"
#include "dcb/duel_rules.h"
#include "dcb/heap.h"
#include "dcb/main.h"
#include "dcb/scene3d.h"
#include "dcb/sound.h"
#include "dcb/stage.h"
#include "dcb/text.h"

void func_8003917C(void) {
    s32 diff;
    s32 p;
    s32 i;

    do {
        func_80014C08(FRAME_INTERVAL);
        diff = 0;
        for (p = 0; p < 2; p++) {
            for (i = 0; i < 5; i++) {
                if (((Player *)D_801D8348[p])->unk11C[i] !=
                    ((Player *)D_801D8348[p])->unk126[i]) {
                    diff = 1;
                }
            }
        }
    } while (diff);
}

void func_80039220(s32 p) {
    s32 idx;
    u8 *q;

    idx = func_800411C4(p);
    if (idx != -1) {
        q = D_801D83EC + (p * 0xD8 + 0x48);
        PLAYER(p)->unk130[4].value = (s8)PLAYER(p)->cards[idx % 30].card[0x1C];
        PLAYER(p)->unk130[4].type = 5;
        PLAYER(p)->unk130[4].timer = 0x30;
        PLAYER(p)->unk130[4].x = *(s16 *)(q + 0x10) + (s16)(p * 93 + 0x10);
        PLAYER(p)->unk130[4].y = *(u16 *)(q + 0x12) + 2;
    } else {
        PLAYER(p)->unk130[4].timer = 0;
    }
}

void func_80039354(s32 p, s32 v, s32 k) {
    s32 c;
    u8 *q;

    PLAYER(p)->unk130[k].value = v - PLAYER(p)->unk11C[k];
    if (PLAYER(p)->unk130[k].value == 0) {
        PLAYER(p)->unk130[k].type = 7;
    } else if (PLAYER(p)->unk130[k].value > 0) {
        PLAYER(p)->unk130[k].type = 5;
    } else {
        PLAYER(p)->unk130[k].type = 2;
    }
    PLAYER(p)->unk130[k].value = abs(PLAYER(p)->unk130[k].value);
    PLAYER(p)->unk130[k].timer = 0x30;
    if (k == 0) {
        c = func_80040764(p);
        projectCardSprite(*(void **)(D_801D833C + c * 36), c);
        PLAYER(p)->unk130[0].x = *(u16 *)(*(u8 **)(D_801D833C + c * 36) + 0x34) + 0x19;
        PLAYER(p)->unk130[0].y = *(u16 *)(*(u8 **)(D_801D833C + c * 36) + 0x36) + 0x15;
    } else {
        q = D_801D83EC + (p * 0xD8 + 0x48);
        PLAYER(p)->unk130[k].x = *(u16 *)(q + 0x10) + p * 25 + 0x1C;
        PLAYER(p)->unk130[k].y = *(s16 *)(q + 0x12) + (s16)((k - 1) * 13 + 3);
    }
}

void func_800395A0(void) {
    char buf[24];
    s32 p;
    s32 k;
    char *sign;
    s32 size;

    for (p = 0; p < 2; p++) {
        for (k = 4; k >= 0; k--) {
            if (((Player *)D_801D8348[p])->unk130[k].timer != 0) {
                ((Player *)D_801D8348[p])->unk130[k].timer--;
                if (((Player *)D_801D8348[p])->unk130[k].type == 7) {
                    sign = "=";
                } else if (((Player *)D_801D8348[p])->unk130[k].type == 5) {
                    sign = "+";
                } else {
                    sign = "-";
                }
                size = ((Player *)D_801D8348[p])->unk130[k].timer;
                if (size < 0x2C) {
                    size = 0x2C;
                }
                sprintf(buf, "%s%d", sign, ((Player *)D_801D8348[p])->unk130[k].value);
                drawBigDigits(((Player *)D_801D8348[p])->unk130[k].x + (0x30 - size),
                              ((Player *)D_801D8348[p])->unk130[k].y - (0x30 - size) * 2, (u8 *)buf, (u8 *)&D_8006E298,
                              ((Player *)D_801D8348[p])->unk130[k].type, 0);
            }
        }
    }
}

void func_80039730(s32 n, s32 z) {
    s32 p = n / 6;
    InfoPanel *panel = (InfoPanel *)D_801D83EC + n;
    char buf[72];
    u8 rgb[2][4] = { { 0x80, 0x80, 0x80, 0 }, { 0x40, 0x40, 0x40, 0 } };
    char buf2[40];
    u8 *cols[10];
    CardInfo *card;
    s32 color;
    s32 i;
    s32 k;
    s32 rival;
    s32 x;
    s32 y;

    switch (n) {
    case 2:
    case 8: {
        s32 idx;

        idx = func_80040764(p);
        if (idx >= 0) {
            color = PLAYER(p)->unk178_15 ? 3 : 7;
            card = (CardInfo *)PLAYER(p)->cards[idx % 30].card;
            drawSmallText(panel->x - p * 14 + 17, panel->y + 2, (s32)card->name, 7, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk126[1]);
            drawText(panel->x + 42 + p * 25, panel->y + 11, (s32)buf, color, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk126[2]);
            drawText(panel->x + 42 + p * 25, panel->y + 24, (s32)buf, color, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk126[3]);
            drawText(panel->x + 42 + p * 25, panel->y + 37, (s32)buf, color, z);
            drawSmallText(panel->x + 24 + p * 24, panel->y + 51, (s32)D_8006E47C[card->unkE4], 7, z);
        }
        sprintf(buf, "*s0%2d", PLAYER(p)->unk126[4]);
        drawText(panel->x + 6 + p * 93, panel->y + 9, (s32)buf, 7, z);
        k = 8 - func_80041214(p);
        if (k != 0) {
            CUR_SPRT->sp.x0 = panel->x + 3 + p * 94;
            CUR_SPRT->sp.y0 = panel->y + 50;
            CUR_SPRT->sp.u0 = 0xF0;
            CUR_SPRT->sp.v0 = 0x47;
            CUR_SPRT->sp.clut = getClut(800, k + 0x1F7);
            CUR_SPRT->sp.w = 16;
            CUR_SPRT->sp.h = 8;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
        break;
    }
    case 5:
    case 11:
        if (panel->state == 5) {
            CUR_SPRT->sp.x0 = panel->x + 5;
            CUR_SPRT->sp.y0 = panel->y - 56 + p * 64;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0xBA;
            CUR_SPRT->sp.clut = 0x7CF3;
            CUR_SPRT->sp.w = 32;
            CUR_SPRT->sp.h = 62;
            setSemiTrans(&CUR_SPRT->sp, 1);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x3D);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
        break;
    case 4:
    case 10:
        sprintf(buf, "%s Deck", PLAYER(p)->unk0 + 1);
        drawText(panel->x + 1 + (0x82 - measureText((u8 *)buf)) / 2, panel->y + 0x33 + p * -50, (s32)buf, 7, z);
        drawText(panel->x + 0x85 + (0x78 - measureText((u8 *)PLAYER(p)->unk1CE)) / 2, panel->y + 0x33 + p * -50,
                      (s32)PLAYER(p)->unk1CE, 7, z);
        k = func_80040220(p) >= 8 ? 7 : 2;
        sprintf(buf, "*s0%2d", func_80040220(p));
        drawText(panel->x + 4 + p * 0xEC, panel->y + 0x1E + p * 14, (s32)buf, k, z);
        sprintf(buf, "*s0%2d", func_80040124(p));
        drawText(panel->x + 4 + p * 0xEC, panel->y + 6 + p * 14, (s32)buf, 7, z);
        for (k = 0; k < PLAYER(p)->unk17C; k++) {
            drawWinMarker(panel->x + 0xDF + p * -0xDD, panel->y + 4 + p * 13 + k * 15, 0x4A);
        }
        break;
    case 0:
    case 6: {
        s32 back;
        s32 shift;

        if (*(s16 *)(DUEL->unk58 + 2) == -1) {
            break;
        }
        back = DUEL->cache[DUEL->unk826].used;
        if (back == 1) {
            if (isSpritePoolFull() != 0) {
                break;
            }
            CUR_SPRT->sp.x0 = panel->x;
            CUR_SPRT->sp.y0 = panel->y + 7;
            CUR_SPRT->sp.u0 = (DUEL->unk826 & 1) << 6;
            CUR_SPRT->sp.v0 = ((DUEL->unk826 >> 1) << 6) + 0x40;
            CUR_SPRT->sp.clut = (0x1FF - DUEL->unk826) << 6;
            CUR_SPRT->sp.w = 64;
            CUR_SPRT->sp.h = 64;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x9A);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        } else if (SPRITE_KIND(*(s16 *)(DUEL->unk58 + 2)) == 0x19 || DUEL->unk81C == 4) {
            drawCardArtPlaceholder(panel->x, panel->y + 7, z, p, 0);
        } else {
            drawCardArtPlaceholder(panel->x, panel->y + 7, z, p, SPRITE(*(s16 *)(DUEL->unk58 + 2)));
        }
        if (SPRITE_KIND(*(s16 *)(DUEL->unk58 + 2)) == 0x19) {
            drawText(panel->x + 0x8E, panel->y + 0x10, (s32)"*h-1All-or-Nothing\nGamble!", 7, z);
            break;
        }
        if (DUEL->unk81C == 4) {
            if (DUEL->unk81D == 4) {
                sprintf(buf2, "*h-1All-or-Nothing\nGamble!\nCards left in the\nOnline Deck are %d.",
                        func_80040220(DUEL->unk81B));
            } else {
                sprintf(buf2, "*h-1Cards left in the\nOnline Deck is %d.", func_80040220(DUEL->unk81B));
            }
            drawText(panel->x + 0x8E, panel->y + 0x10, (s32)buf2, 7, z);
            break;
        }
        for (i = 0; i < 10; i++) {
            cols[i] = rgb[0];
        }
        switch (DUEL->unk81D) {
        case 1:
            cols[0] = rgb[1];
            cols[1] = rgb[1];
            cols[7] = rgb[1];
            cols[8] = rgb[1];
            cols[9] = rgb[1];
            break;
        case 2:
            for (i = 0; i < 10; i++) {
                cols[i] = rgb[1];
            }
            cols[1] = rgb[0];
            break;
        case 3:
        case 6:
            cols[1] = rgb[1];
            cols[7] = rgb[1];
            cols[8] = rgb[1];
            cols[9] = rgb[1];
            break;
        case 4:
            for (i = 0; i < 10; i++) {
                cols[i] = rgb[1];
            }
            cols[7] = rgb[0];
            cols[8] = rgb[0];
            break;
        case 5:
            for (i = 0; i < 10; i++) {
                cols[i] = rgb[1];
            }
            cols[9] = rgb[0];
            break;
        }
        switch (PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].state) {
        case 0:
            card = (CardInfo *)PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            sprintf(buf, "*s0%2d", card->unk1B);
            drawTextColored(panel->x + 0x7A, panel->y + 0x17, (s32)buf, (s32 *)cols[0], 7, z);
            sprintf(buf, "*s0%2d", card->level);
            drawTextColored(panel->x + 0x7C, panel->y + 0x2D, (s32)buf, (s32 *)cols[1], 7, z);
            if (DUEL->unk81D == 1 || DUEL->unk81D == 3) {
                if (DUEL->unk81C < 4 && DUEL->unk81B == DUEL->unk817) {
                    if (DUEL->unk81D == 1) {
                        shift = card->attr & 0xF;
                    } else {
                        shift = PLAYER(p)->unk178_15;
                    }
                    shift--;
                    color = 3;
                    if (shift <= 0) {
                        color = 7;
                        shift = 0;
                    }
                    sprintf(buf, "*s0%4d", (card->hp >> shift) / 10 * 10);
                    drawTextColored(panel->x + 0x56, panel->y + 0xE, (s32)buf, (s32 *)cols[2], color, z);
                    for (i = 0; i < 3; i++) {
                        sprintf(buf, "*s0%4d", (card->attack[i].power >> shift) / 10 * 10);
                        x = panel->x;
                        y = panel->y;
                        drawTextColored(x + 0x56, i * 12 + y + 0x1A, (s32)buf, (s32 *)cols[i + 3], color, z);
                    }
                    drawIcon(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
                } else if (DUEL->unk81C == 6) {
                    color = PLAYER(DUEL->unk81B)->unk178_15 ? 3 : 7;
                    for (i = 0; i < 4; i++) {
                        sprintf(buf, "*s0%4d", PLAYER(DUEL->unk81B)->unk11C[i]);
                        x = panel->x;
                        y = panel->y;
                        drawTextColored(x + 0x56, i * 12 + y + 0xE, (s32)buf, (s32 *)cols[i + 2], color, z);
                    }
                    drawIcon(panel->x + 0xC3, panel->y + 1, 0, PLAYER(DUEL->unk81B)->unk178_19, z);
                } else {
                    sprintf(buf, "*s0%4d", card->hp);
                    drawTextColored(panel->x + 0x56, panel->y + 0xE, (s32)buf, (s32 *)cols[2], 7, z);
                    for (i = 0; i < 3; i++) {
                        sprintf(buf, "*s0%4d", card->attack[i].power);
                        x = panel->x;
                        y = panel->y;
                        drawTextColored(x + 0x56, i * 12 + y + 0x1A, (s32)buf, (s32 *)cols[i + 3], 7, z);
                    }
                    drawIcon(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
                }
            } else if (DUEL->unk81C == 6) {
                color = PLAYER(DUEL->unk81B)->unk178_15 ? 3 : 7;
                for (i = 0; i < 4; i++) {
                    sprintf(buf, "*s0%4d", PLAYER(DUEL->unk81B)->unk11C[i]);
                    x = panel->x;
                    y = panel->y;
                    drawTextColored(x + 0x56, i * 12 + y + 0xE, (s32)buf, (s32 *)cols[i + 2], color, z);
                }
                drawIcon(panel->x + 0xC3, panel->y + 1, 0, PLAYER(DUEL->unk81B)->unk178_19, z);
            } else {
                sprintf(buf, "*s0%4d", card->hp);
                drawTextColored(panel->x + 0x56, panel->y + 0xE, (s32)buf, (s32 *)cols[2], 7, z);
                for (i = 0; i < 3; i++) {
                    sprintf(buf, "*s0%4d", card->attack[i].power);
                    x = panel->x;
                    y = panel->y;
                    drawTextColored(x + 0x56, i * 12 + y + 0x1A, (s32)buf, (s32 *)cols[i + 3], 7, z);
                }
                drawIcon(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
            }
            drawSmallTextColored(panel->x + 0x44, panel->y + 0x40, D_8006E47C[card->unkE4], 7, cols[6], z);
            if (D_8006E4FC[card->unkE4] != 0) {
                drawIcon(panel->x + 0x75, panel->y + 0x3B, 0, D_8006E4FC[card->unkE4] + 0x14, z);
            }
            drawText(panel->x + 0x44, panel->y + 1, (s32)card->name, 7, z);
            drawIcon(panel->x + 0xD4, panel->y + 1, 0, (card->attr & 0xF) + 0x10, z);
            if (card->unkE6 != 0) {
                drawIcon(panel->x + 0xE3, panel->y + 2, 0, card->unkE6 + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                drawTextColored(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)card->text[i], (s32 *)cols[7], 7, z);
            }
            break;
        case 1: {
            s8 *opt;

            opt = PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            drawText(panel->x + 0x44, panel->y + 1, (s32)(opt + 3), 7, z);
            drawIcon(panel->x + 0xC3, panel->y + 1, 0, 5, z);
            if (opt[0x8C] != 0) {
                drawIcon(panel->x + 0xE3, panel->y + 2, 0, opt[0x8C] + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                drawTextColored(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)(opt + 0x8D + i * 21), (s32 *)cols[8], 7,
                              z);
            }
            break;
        }
        case 2: {
            s8 *opt;

            opt = PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            drawText(panel->x + 0x44, panel->y + 1, (s32)(opt + 3), 7, z);
            drawIcon(panel->x + 0xC3, panel->y + 1, 0, 6, z);
            for (i = 0; i < 4; i++) {
                drawTextColored(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)(opt + 0x1B + i * 21), (s32 *)cols[9], 7,
                              z);
            }
            break;
        }
        }
        break;
    }
    case 1:
    case 7:
        card = (CardInfo *)PLAYER(p)->cards[func_80040764(p) % 30].card;
        color = PLAYER(p)->unk178_15 ? 3 : 7;
        drawText(panel->x + 4, panel->y + 1, (s32)card->name, 6, z);
        sprintf(buf, "*s0%4d", PLAYER(p)->unk11C[0]);
        drawText(panel->x + 0x82, panel->y + 1, (s32)buf, color, z);
        drawIconColored(panel->x + 0xA4, panel->y + 2, 0, (card->attr & 0xF) + 0x10, rgb[0], z);
        drawIconColored(panel->x + 0xB6, panel->y + 2, 0, PLAYER(p)->unk178_19, rgb[0], z);
        for (k = 0; k < 3; k++) {
            drawText(panel->x + 0x25, panel->y + 13 + k * 12, (s32)card->attack[k].name, 7, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk15C[k]);
            drawText(panel->x + 0xA2, panel->y + 13 + k * 12, (s32)buf, color, z);
        }
        drawText(panel->x + 0x47, panel->y + 0x32, (s32)D_8006E4BC[card->unkE4], 7, z);
        if (D_8006E4FC[card->unkE4] != 0) {
            drawIcon(panel->x + 0x95, panel->y + 0x32, 0, D_8006E4FC[card->unkE4] + 0x14, z);
        }
        if (PLAYER(p)->unk178_2 != 3) {
            if (PLAYER(p)->unk178_4 != PLAYER(p)->unk178_2) {
                PLAYER(p)->unk16E = 0;
            }
            PLAYER(p)->unk178_4 = PLAYER(p)->unk178_2;
            if (PLAYER(p)->unk16E < 28) {
                PLAYER(p)->unk16E++;
                panel->clut = getClut(784, p * 8 + 0x1F0 + PLAYER(p)->unk16E / 4);
            } else {
                panel->clut = getClut(784, p * 8 + 0x1F7);
            }
        } else {
            PLAYER(p)->unk178_4 = 3;
            panel->clut = getClut(784, p * 8 + 0x1F0);
        }
        rival = DUEL->unk817 != p;
        func_80044504(panel->x + 0xA7, panel->y + 0x32, rival, 0x80, z);
        break;
    }
}

void func_8003B210(s32 c, s32 p) {
    CardAnim *a;

    a = (CardAnim *)(D_801D833C + c * 36);
    a->spr->flags |= 0x80;
    switch (SPRITE_KIND(c)) {
    case 0:
        a->spr->pos.vx = SLOT(p, 0x90)->x - 0x80 + p * 0xBE;
        a->spr->pos.vy = SLOT(p, 0x90)->y - 0x54 + p * 0xE;
        a->spr->pos.vz = 0;
        UNK7F8(c).rx = 0x2000;
        UNK7F8(c).ry = 0x2800;
        UNK7F8(c).rz = 0x1C00;
        a->spr->scale = 0x800;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x90)->unkC;
        a->count = 0;
        break;
    case 1:
    case 21:
    case 26:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        playSoundEffect(0xA5);
        a->state++;
        break;
    case 2:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x80 + p * 0xBE);
            ty = (s16)(SLOT(p, 0x90)->y - 0x54 + p * 0xE);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x1C00;
            sc = 0x800;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state = 0;
        }
        break;
    case 3:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        playSoundEffect(0xA5);
        a->state++;
    case 4:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x5C + p * -10 + a->unk23 * 0x2B);
            ty = (s16)(SLOT(p, 0x90)->y - 0x69 + p * 0x21);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
            if (a->count != 0) {
                break;
            }
        }
        a->total = 4;
        a->count = 4;
        a->state++;
        playSoundEffect(0xA7);
        break;
    case 5:
    case 13:
        if (--a->count == 0) {
            ANIM_SAVE(a);
            a->total = 0xE;
            a->count = 0xC;
            a->state++;
        }
        break;
    case 6:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x5C + p * -10 + a->unk23 * 0x2B);
            ty = (s16)(SLOT(p, 0x90)->y - 0x61 + p * 0x11);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 7:
        a->spr->pos.vx = SLOT(p, 0x90)->x - 0x5C + p * -10 + a->unk23 * 0x2B;
        a->spr->pos.vy = SLOT(p, 0x90)->y - 0x61 + p * 0x11;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x1000;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x90)->unkC;
        break;
    case 8:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        playSoundEffect(0xA5);
        a->state++;
    case 9:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x80 + p * 0xBE);
            ty = (s16)(SLOT(p, 0x90)->y - 0x6C + p * 0xE);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2400;
            sc = 0x800;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 10:
        a->spr->pos.vx = SLOT(p, 0x90)->x - 0x80 + p * 0xBE;
        a->spr->pos.vy = SLOT(p, 0x90)->y - 0x6C + p * 0xE;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2400;
        a->spr->scale = 0x800;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x90)->unkC;
        break;
    case 11:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        playSoundEffect(0xA5);
        a->state++;
    case 12: {
        s32 n;
        s32 i;

            n = 0;
            if (a->count != 0) {
                s32 tx;
                s32 ty;
                s16 rx;
                s16 ry;
                s16 rz;
                s16 sc;

                for (i = 2; i >= 0; i--) {
                    if (c == ((Player *)D_801D8348[p])->unk1CA[i]) {
                        break;
                    }
                    n++;
                }
                tx = (s16)(SLOT(p, 0x48)->x + (s16)(n * 2 - 0x46) + (s16)((-0x40 - (n * 2 + 8) * 2) * p + 8));
                ty = (s16)(SLOT(p, 0x48)->y - 0x54);
                rx = 0x2000;
                ry = 0x2000;
                rz = 0x2000;
                sc = 0x1000;
                ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
            } else {
                playSoundEffect(0xA7);
                a->total = 4;
                a->count = 4;
                a->state++;
            }
            break;
    }
    case 14: {
        s32 n;
        s32 i;

            n = 0;
            if (a->count != 0) {
                s32 tx;
                s32 ty;
                s16 rx;
                s16 ry;
                s16 rz;
                s16 sc;

                for (i = 2; i >= 0; i--) {
                    if (c == ((Player *)D_801D8348[p])->unk1CA[i]) {
                        break;
                    }
                    n++;
                }
                tx = (s16)(SLOT(p, 0x48)->x + (s16)(n * 2 - 0x46) + (s16)((-0x40 - n * 4) * p));
                ty = (s16)(SLOT(p, 0x48)->y - 0x54);
                rx = 0x2000;
                ry = 0x2000;
                rz = 0x2000;
                sc = 0x1000;
                ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
            } else {
                a->state++;
                playSoundEffect(0xA7);
            }
            break;
    }
    case 15: {
        s32 n;
        s32 i;

            n = 0;
            for (i = 2; i >= 0; i--) {
                if (c == ((Player *)D_801D8348[p])->unk1CA[i]) {
                    break;
                }
                n++;
            }
            a->spr->pos.vx = SLOT(p, 0x48)->x - 0x46 + n * 2 + (-0x40 - n * 4) * p;
            a->spr->pos.vy = SLOT(p, 0x48)->y - 0x54;
            a->spr->pos.vz = 0;
            a->spr->rot.vx = 0x2000;
            a->spr->rot.vy = 0x2000;
            a->spr->rot.vz = 0x2000;
            a->spr->scale = 0x1000;
            a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x48)->unkC;
            break;
    }
    case 16:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        if (a->spr->rot.vy == 0x2000) {
            playSoundEffect(0xA5);
        } else {
            playSoundEffect(0xA6);
        }
        a->state++;
        break;
    case 17:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x50 + p * -0x3E);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            playSoundEffect(0xA7);
            a->total = 4;
            a->count = 4;
            a->state++;
        }
        break;
    case 18:
        if (--a->count == 0) {
            ANIM_SAVE(a);
            a->total = 4;
            a->count = 4;
            a->state++;
        }
        break;
    case 19:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x58 + p * -0x2E);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 20:
        a->spr->pos.vx = SLOT(p, 0x6C)->x - p - 0x89;
        a->spr->pos.vy = SLOT(p, 0x6C)->y - p * 0x2E - 0x58;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x1000;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x6C)->unkC;
        break;
    case 22:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x50 + p * -0x3E);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->total = 0x20;
            a->count = 0x20;
            a->state++;
        }
        break;
    case 23:
        if (a->count != 0) {
            a->count--;
            ANIM_SAVE(a);
            a->total = 4;
            a->count = 4;
            a->state++;
        }
        break;
    case 24:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x58 + p * -0x2E);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            playSoundEffect(0xA7);
        }
        break;
    case 25:
        a->spr->pos.vx = SLOT(p, 0x6C)->x - p - 0x89;
        a->spr->pos.vy = SLOT(p, 0x6C)->y - p * 0x2E - 0x58;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2800;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x1000;
        break;
    case 27:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x48)->x - 0x94 + p * 0x5D);
            ty = (s16)(SLOT(p, 0x48)->y - 0x54);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x800;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            func_80039220(p);
            playSoundEffect(0xA7);
        }
        break;
    case 28:
        a->spr->pos.vx = SLOT(p, 0x48)->x - 0x94 + p * 0x5D;
        a->spr->pos.vy = SLOT(p, 0x48)->y - 0x54;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x800;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x48)->unkC;
        break;
    case 29:
        ANIM_SAVE(a);
        a->total = 0x20;
        a->count = 0x20;
        a->state++;
        break;
    case 30:
        if (a->count != 0) {
            s32 ty;
            s16 r;

            ty = (s16)(0x3C - p * 0x78);
            r = 0x2000;
            ANIM_STEP(a, 0, ty, r, r, r, r);
        } else {
            a->state++;
        }
        break;
    case 31:
        a->spr->pos.vx = 0;
        a->spr->pos.vy = 0x3C - p * 0x78;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x2000;
        break;
    case 32:
        ANIM_SAVE(a);
        a->total = 0x20;
        a->count = 0x20;
        playSoundEffect(0xA6);
        a->state++;
        break;
    case 33:
        if (a->count != 0) {
            s32 ty;
            s16 ry;

            s16 r;

            ty = (s16)(0xA0 - p * 0x140);
            r = 0x2000;
            ry = 0x2800 - (p << 12);
            ANIM_STEP(a, 0, ty, r, ry, r, r);
        } else {
            a->state++;
        }
        break;
    case 34:
        a->spr->pos.vx = 0;
        a->spr->pos.vy = 0xA0 - p * 0x140;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2800 - (p << 12);
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x2000;
        break;
    }
}

void func_8003D4C4(void) {
    char buf[8];
    Rect16 rect;
    u8 rgb[4] = "@@@";
    s32 i;
    s32 j;
    s32 done;
    s8 c;
    s32 color;
    s32 z;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 30; j++) {
            func_8003B210(i * 30 + j, i);
        }
    }
    resetCardPolyCount();
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            c = PLAYER(i)->unk1C2[j];
            if (c >= 0) {
                renderCardSprite(SPRITE(c), c);
                if (SPRITE_KIND(c) == 0x1C) {
                    break;
                }
            }
        }
        c = PLAYER(i)->unk1CD;
        if (c >= 0) {
            renderCardSprite(SPRITE(c), c);
        }
        done = 0;
        for (j = 0; j < 3; j++) {
            c = PLAYER(i)->unk1CA[j];
            if (c >= 0) {
                if (!done) {
                    ((u8 *)SPRITE(c))[0x14] = PLAYER(i)->unk178_19;
                    done = 1;
                    if (SPRITE_KIND(c) < 0x1D) {
                        color = PLAYER(i)->unk178_15 ? 3 : 7;
                        projectCardSprite(SPRITE(c), c);
                        z = *(s32 *)((u8 *)SPRITE(c) + 0x38);
                        drawIcon(*(s16 *)((u8 *)SPRITE(c) + 0x34) + 2, *(s16 *)((u8 *)SPRITE(c) + 0x36) + 30, 0,
                                      0x1A, z);
                        sprintf(buf, "%4d", PLAYER(i)->unk126[0]);
                        drawText(*(s16 *)((u8 *)SPRITE(c) + 0x34) + 15, *(s16 *)((u8 *)SPRITE(c) + 0x36) + 30,
                                      (s32)buf, color, z);
                        rect.x = 0x60;
                        rect.y = 0xDB;
                        rect.w = 0x26;
                        rect.h = 0xC;
                        drawPageSpriteColored(*(s16 *)((u8 *)SPRITE(c) + 0x34) + 1, *(s16 *)((u8 *)SPRITE(c) + 0x36) + 30,
                                      &rect, rgb, getTPage(0, 2, SYSTEM_TEX_X, SYSTEM_TEX_Y), 0xC, z);
                        done = 1;
                    }
                }
                renderCardSprite(SPRITE(c), c);
            }
        }
        for (j = 0; j < 30; j++) {
            c = PLAYER(i)->unk19B[j];
            if (c >= 0) {
                renderCardSprite(SPRITE(c), c);
                if (SPRITE_KIND(c) == 10) {
                    break;
                }
            }
        }
        for (j = 3; j >= 0; j--) {
            c = PLAYER(i)->unk1B9[j];
            if (c >= 0) {
                renderCardSprite(SPRITE(c), c);
            }
        }
        for (j = 0; j < 30; j++) {
            c = PLAYER(i)->unk17D[j];
            if (c >= 0) {
                renderCardSprite(SPRITE(c), c);
                if (SPRITE_KIND(c) == 0) {
                    break;
                }
            }
        }
    }
}

/* the original file padded its strings with an empty word here */
__asm__(".section .rodata\n\t.word 0\n\t.section .text\n");

void func_8003D9C0(Panel *p, s16 x, s16 y, s32 speed) {
    if (speed == 0) {
        speed = 1;
    }
    p->unkC |= 0x80;
    if (p->parent != 0) {
        p->unkC = p->parent->unkC;
        p->unk18 = p->unk10 - p->parent->unk10;
        p->unk1A = p->unk12 - p->parent->unk12;
    } else {
        p->unk18 = p->unk10;
        p->unk1A = p->unk12;
    }
    p->unk14 = x;
    p->unk16 = y;
    p->unkE = speed;
    p->unkF = speed;
    p->unkD++;
}

s32 func_8003DA64(Panel *p) {
    s16 px;
    s16 py;

    px = 0;
    py = 0;
    p->unkF--;
    if (p->parent != 0) {
        p->unkC = p->parent->unkC;
        px = p->parent->unk10;
        py = p->parent->unk12;
    }
    p->unk10 = px + (p->unk14 - (p->unk14 - p->unk18) * p->unkF / p->unkE);
    p->unk12 = py + (p->unk16 - (p->unk16 - p->unk1A) * p->unkF / p->unkE);
    if (p->unkF == 0) {
        p->unkD++;
    }
    return p->unkF;
}

void func_8003DB64(Panel *p) {
    s16 x;
    s16 y;

    x = p->unk14;
    y = p->unk16;
    if (p->parent != 0) {
        p->unkC = p->parent->unkC;
        x += p->parent->unk10;
        y += p->parent->unk12;
    }
    p->unk10 = x;
    p->unk12 = y;
}

void func_8003DBBC(s32 arg0) {
    void *p;

    p = D_801D83EC + (arg0 * 0xD8 + 0x90);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = 0x20;
        (*(s16 *)((s8 *)p + 0x12)) = arg0 * -0x12F + 0xF0;
        break;
    case 1:
        (D_801D83EC + arg0 * 0xD8)[0x55] = 1;
        (D_801D83EC + arg0 * 0xD8)[0xD] = 5;
        (*(u8 *)((s8 *)p + 0xD)) += 1;
        break;
    case 2:
        func_8003D9C0(p, 0x20, -(arg0 * 0x7D) + 0x99, 0x10);
        break;
    case 3:
        func_8003DA64(p);
        break;
    case 4:
        func_8003DB64(p);
        break;
    case 7:
        (D_801D83EC + arg0 * 0xD8)[0x55] = 6;
        (D_801D83EC + arg0 * 0xD8)[0xD] = 5;
        (*(u8 *)((s8 *)p + 0xD)) = 2;
        break;
    case 11:
        func_8003D9C0(p, 0xE8, -(arg0 * 0x7D) + 0x99, 0x10);
        break;
    case 12:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003DD9C(s32 arg0) {
    void *p;
    s32 y;

    p = D_801D83EC + (arg0 * 0xD8 + 0xB4);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = 0x164;
        y = 0x31 - arg0 * 0x31;
        (*(s16 *)((s8 *)p + 0x12)) = y;
        func_8003D9C0(p, 0x164, y, 0);
        func_8003DA64(p);
        (*(u8 *)((s8 *)p + 0xD)) = 0;
        break;
    case 1:
        func_8003D9C0(p, 0x100, 0x31 - arg0 * 0x31, 8);
        break;
    case 2:
        if (func_8003DA64(p) == 0) {
            playSoundEffect(0xA7);
        }
        break;
    case 3:
        func_8003D9C0(p, 0xF9, 0x31 - arg0 * 0x31, 8);
        break;
    case 4:
        func_8003DA64(p);
        break;
    case 5:
        func_8003DB64(p);
        break;
    case 6:
        func_8003D9C0(p, 0x164, 0x31 - arg0 * 0x31, 8);
        break;
    case 7:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003DF48(s32 arg0) {
    void *p;

    p = D_801D83EC + (arg0 * 0xD8 + 0x48);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = arg0 * 0x2A0 - 0xEC;
        (*(s16 *)((s8 *)p + 0x12)) = 0x5C;
        break;
    case 1:
        func_8003D9C0(p, arg0 * 0x7C + 0x28, 0x5C, 0x10);
        (D_801D83EC + arg0 * 0xD8)[0x79] = 4;
        break;
    case 2:
        func_8003DA64(p);
        break;
    case 3:
        func_8003DB64(p);
        break;
    case 4:
        func_8003D9C0(p, arg0 * 0x2A0 - 0xEC, 0x5C, 0x10);
        break;
    case 5:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    case 6:
        func_8003D9C0(p, arg0 * 0xCC, 0x5C, 0x10);
        break;
    case 7:
        if (func_8003DA64(p) == 0) {
            (D_801D83EC + arg0 * 0xD8)[0x79] = 1;
        }
        break;
    case 8:
        func_8003DB64(p);
        break;
    }
}

void func_8003E11C(s32 arg0) {
    void *p;
    s32 x;
    s32 y;

    p = D_801D83EC + (arg0 * 0xD8 + 0x6C);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        x = -(arg0 * 0x41) + 0x44;
        (*(s16 *)((s8 *)p + 0x10)) = x;
        y = arg0 * 0x1E + 0xA;
        (*(s16 *)((s8 *)p + 0x12)) = y;
        func_8003D9C0(p, x, y, 0);
        func_8003DA64(p);
        (*(u8 *)((s8 *)p + 0xD)) = 0;
        break;
    case 1:
        func_8003D9C0(p, -(arg0 * 0xA1) + 0x74, arg0 * 0x1E + 0xA, 8);
        break;
    case 2:
        if (func_8003DA64(p) == 0) {
            playSoundEffect(0xA7);
        }
        break;
    case 3:
        func_8003DB64(p);
        break;
    case 4:
        func_8003D9C0(p, -(arg0 * 0x41) + 0x44, arg0 * 0x1E + 0xA, 8);
        break;
    case 5:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003E298(s32 arg0) {
    void *p;

    p = D_801D83EC + (arg0 * 0xD8 + 0x24);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = 0x38;
        (*(s16 *)((s8 *)p + 0x12)) = arg0 * -0x12F + 0xF0;
        break;
    case 1:
        func_8003D9C0(p, 0x38, arg0 * -0x7F + 0x99, 0x10);
        break;
    case 2:
        func_8003DA64(p);
        break;
    case 3:
        func_8003DB64(p);
        break;
    case 4:
        func_8003D9C0(p, 0x38, arg0 * -0x12F + 0xF0, 0x10);
        break;
    case 5:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003E3C8(s32 arg0) {
    void *p;

    p = D_801D83EC + arg0 * 0xD8;
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = -0xFF;
        (*(s16 *)((s8 *)p + 0x12)) = arg0 * 0x7E + 0x16;
        break;
    case 1:
        func_8003D9C0(p, 0x22, 0x16, 0xC);
        (*(u8 *)((s8 *)p + 0xD)) = 3;
        break;
    case 2:
        func_8003D9C0(p, 0x22, 0x94, 0xA);
        (*(u8 *)((s8 *)p + 0xD)) = 3;
        break;
    case 3:
        func_8003DA64(p);
        break;
    case 4:
        func_8003DB64(p);
        break;
    case 5:
        func_8003D9C0(p, -0xFF, (*(s16 *)((s8 *)p + 0x12)), 0xC);
        break;
    case 6:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003E4F0(void) {
    s32 p;
    s32 i;
    s32 d;
    s32 step;
    s32 count;

    for (i = 0; i < 2; i++) {
        func_8003DBBC(i);
        func_8003DD9C(i);
        func_8003E298(i);
        func_8003DF48(i);
        func_8003E11C(i);
        func_8003E3C8(i);
    }
    for (p = 0; p < 2; p++) {
        PLAYER(p)->unk11C[4] = func_8004110C(p);
        if (func_80040764(p) == -1) {
            for (i = 0; i < 4; i++) {
                PLAYER(p)->unk126[i] = 0;
                PLAYER(p)->unk11C[i] = 0;
            }
        }
        for (i = 0; i < 5; i++) {
            d = PLAYER(p)->unk126[i] - PLAYER(p)->unk11C[i];
            step = (d < 0 ? -d : d) / 16 + 1;
            if (PLAYER(p)->unk126[i] < PLAYER(p)->unk11C[i]) {
                PLAYER(p)->unk126[i] += step;
                if (PLAYER(p)->unk126[i] > PLAYER(p)->unk11C[i]) {
                    PLAYER(p)->unk126[i] = PLAYER(p)->unk11C[i];
                }
            } else if (PLAYER(p)->unk126[i] > PLAYER(p)->unk11C[i]) {
                PLAYER(p)->unk126[i] -= step;
                if (PLAYER(p)->unk126[i] < PLAYER(p)->unk11C[i]) {
                    PLAYER(p)->unk126[i] = PLAYER(p)->unk11C[i];
                }
            }
        }
    }
    count = 0;
    for (p = 0; p < 2; p++) {
        for (i = 0; i < 5; i++) {
            if (PLAYER(p)->unk126[i] != PLAYER(p)->unk11C[i]) {
                count++;
            }
        }
    }
    if (count != 0 && !(((Unk8006E050 *)PLAYER_PROFILES)->unk24 & 3)) {
        playSoundEffect(0xAA);
    }
    func_800395A0();
    for (i = 0; i < 12; i++) {
        if (PANEL(i).flags & 0x80) {
            drawHudSprite((SprtInfo *)&PANEL(i), i, i * 2 + PANEL(i).z + 1);
            func_80039730(i, i * 2 + PANEL(i).z);
        }
    }
}

void func_8003E844(s32 arg0) {
    void *p;

    D_801D833C = p = allocTaskHeapBlock(0x870);
    D_801D8340 = p = allocTaskHeapBlock(0x86C);
    (*(s32 *)((s8 *)D_801D8340 + 0x7F8)) = func_801F8854();
    (*(s8 *)((s8 *)D_801D8340 + 0x817)) = (s8) (rand() % 2);
    (*(s8 *)((s8 *)D_801D8340 + 0x818)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81B)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81C)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x810)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x81F)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x825)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x823)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x822)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x824)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x820)) = 0;
    func_801F8200();
    func_8003FB3C(arg0);
    (*(s8 *)((s8 *)D_801D8340 + 0x81D)) = -1;
}

void func_8003E94C(void) {
    Unk800794F8 *p;

    func_80024460(0);
    func_800149B8(0x19, -1, 0, 0x800, &func_800250F4, 0);
    func_80014C08(2);
    p = (Unk800794F8 *)&GRAPHICS;
    p->unk54 = 0;
    p->unk56 = 0;
    p->unk58 = 0;
    p->unk7C = 0;
    p->unk80 = 0;
    p->unk84 = 0;
    p->unk8E = 0;
    p->unk90 = 0x1C0;
    p->unk92 = 0;
    p->unk94 = 0;
    p->unk8C = -1;
    p->unk74 = 1;
    (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 0;
    func_80014C08(2);
}

void func_8003E9F4(s32 arg0) {
    s32 var_a0;
    s32 var_v1;

    (*(s32 *)((s8 *)D_801D8340 + 0x58)) = func_801F8998(0, 0x26, 0x2E, 0xA, 1);
    func_800149B8(0x1E, -1, 0, 0x800, &func_80034260, 0, 0, 0, 0);
    if ((arg0 != 0) && ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) == 0)) {
        func_800149B8(0, -1, 0, 0x800, func_80038F68, 0, 0, 0, 0);
    }
    func_800149B8(0, -1, 0, 0x800, &runCardArtLoader, 0, 0, 0, 0);
    if (arg0 != 0) {
        var_a0 = (*(u8 *)((s8 *)D_8006E054 + 0x72));
        var_v1 = (*(u8 *)((s8 *)D_8006E054 + 0x71));
    } else {
        var_a0 = -1;
        var_v1 = -1;
    }
    func_800149B8(0, -1, 0, 0x1000, &func_8002E034, var_a0, var_v1, 0, 0);
}

void func_8003EB50(void) {
    func_80014A00(0x19);
    func_801F848C();
    func_801F88E8();
    freeHeapBlocksByTag(0x7F);
}

void func_8003EB88(void) {
    s16 temp_a0;

    temp_a0 = (*(s16 *)((s8 *)D_801D8340 + 0x808));
    if (temp_a0 != 0) {
        renderDuelBackground(temp_a0);
        renderStatusMessage((*(s16 *)((s8 *)D_801D8340 + 0x808)));
        renderHelpBar((*(s16 *)((s8 *)D_801D8340 + 0x808)));
    }
    renderPhaseBanner();
    func_8003E4F0();
    func_8003D4C4();
    if ((*(s32 *)((s8 *)D_801D8340 + 0x83C)) == 0) {
        if ((*(s32 *)((s8 *)D_801D8340 + 0x828)) != -1) {
            func_801F97F4();
        }
        func_801EB53C(D_801D83D1);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/duel/battle_hud", D_80011350);

INCLUDE_ASM("asm/main/nonmatchings/duel/battle_hud", func_8003EC4C);
