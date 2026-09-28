#include "dcb/card_render.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/duel_launch.h"
#include "dcb/battle_hud.h"
#include "dcb/card_motion.h"
#include "dcb/hud_panels.h"
#include "dcb/duel_session.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/save_checksum.h"
#include "dcb/player_rank.h"
#include "dcb/prim_util.h"
#include "dcb/transform.h"
#include "dcb/text.h"
#include "dcb/str_util.h"

s32 D_8006E294 = 0;
s32 D_8006E298 = 0x808080;

void uploadStringGlyphs(u8 *string, s32 row, s32 parentTask) {
    char path[64]; /* unused, but it is in the original stack frame */
    u8 *fontArchive;
    s32 i;

    D_8006E294 = 1;
    DUEL_VRAM_READY = 0;
    func_800149B8(0, -1, 0, 0x800, &loadFile, "B:\\FONT.ARC", getCurrentTaskId());
    fontArchive = (u8 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; *string != 0;) {
        uploadTim((u32 *)(fontArchive + ((s32 *)fontArchive)[*string - 0x20]), i * 4 + 0x2C0, (row << 5) + 0x1C0, 0x2F0,
                      row + 0x1D7);
        DrawSync(0);
        string++;
        func_80014C08(FRAME_INTERVAL);
        if (++i >= 12) {
            break;
        }
    }
    freeHeapBlock(fontArchive);
    D_8006E294 = 0;
    func_80014A48(parentTask);
}

void runCardArtLoader(void) {
    char path[72];
    u32 *tim;
    s32 i;
    s16 spriteIndex;
    s32 cardId;

    CARD_ART_LAST_SPRITE = -1;
    DUEL->unk812 = 0;
    DUEL->unk826 = 0;
    for (i = 0; i < 6; i++) {
        DUEL->cache[i].id = -1;
        DUEL->cache[i].used = 0;
        DUEL->cache[i].age = 100;
    }
    for (;;) {
        s32 slot;

        func_80014C08(FRAME_INTERVAL);
        slot = DUEL->unk826 % 6;
        DUEL->cache[slot].used = 0;
        if (DUEL->unk812 != 0) {
            break;
        }
        if (DUEL->unk81C == -1 || DUEL->unk81C == 4) {
            continue;
        }
        spriteIndex = *(s16 *)(DUEL->unk58 + 2);
        if (spriteIndex == -1) {
            continue;
        }
        if (SPRITE_KIND(spriteIndex) == 0x19) {
            continue;
        }
        if (spriteIndex != CARD_ART_LAST_SPRITE) {
            cardId = PLAYER(DUEL->unk81B)->cards[spriteIndex % 30].id;
            if (DUEL->unk811 != 0) {
                continue;
            }
            DUEL->cache[slot].used = 0;
            CARD_ART_LAST_SPRITE = *(s16 *)(DUEL->unk58 + 2);
            if (DUEL->cache[slot].id != cardId) {
                DUEL->unk811 = 1;
                DUEL->cache[slot].id = cardId;
                sprintf(path, "B:\\CARD\\LC%3.3d.TIM", cardId);
                func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
                tim = (u32 *)func_80014C08(0x7FFFFFFF);
                uploadTim(tim, slot % 2 * 32 + 0x280, slot / 2 * 64 + 0x140, 0, 0x1FF - slot);
                DrawSync(0);
                freeHeapBlock(tim);
                DUEL->unk811 = 0;
            }
            DUEL->cache[slot].used = 1;
            for (i = 0; i < 6; i++) {
                if (DUEL->cache[i].age != 0) {
                    DUEL->cache[i].age--;
                }
            }
            DUEL->cache[slot].age = 100;
        } else {
            DUEL->cache[slot].used = 1;
        }
    }
    DUEL->unk812 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/card/card_render", loadDuelCardGraphics);

/* the message on the top bar */
u8 * STATUS_MESSAGE_TEXTS[18] = {
    "Preparation: Preparing a Hand.",
    "Preparation: There is no Digimon!",
    "Preparation: No Cards in Online Deck!",
    "Preparation: Change all Cards.",
    "Preparation: Digimon Entrance.",
    "Preparation: Digi-Egg Power-Up.",
    "Preparation: Preparation Complete.",
    "Digivolve: Digivolve Points (DP).",
    "Digivolve: Digivolve Option.",
    "Digivolve: Digivolve Digimon.",
    "Digivolve: Digivolve Complete.",
    "Battle: *P1 has no Digimon.",
    "Battle: Deciding an Attack.",
    "Battle: *P1's Support Card.",
    "Battle: *P0's Support Card.",
    "Battle: Support taking Effect.",
    "Preparation: Changing all Cards.",
    "Battle:",
};
s8 STATUS_STEP_SPRITES[20] = { 0, 0, 0, 0, 1, 1, -1, 2, 3, 4, -1, -1, 5, 6, 6, 7, 0, -1, 0, 0 };
/* the button prompts on the bottom bar */
u8 * HELP_BAR_TEXTS[9] = {
    "",
    "*b2OK *b1Change All Cards *b3View Cards",
    "*b0*b1*b2OK *b3View Cards",
    "*b4Select *b2OK",
    "",
    "*b4Select *b2OK *b1Return",
    "*b4Select *b2OK *b0Cancel *b1Return",
    "*b4Select *b2OK *b0Cancel",
    "*b4Select *b1Return",
};

void drawHudSprite(SprtInfo *info, s32 unused, s32 z) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = info->x;
        CUR_SPRT->sp.y0 = info->y;
        CUR_SPRT->sp.u0 = info->u;
        CUR_SPRT->sp.v0 = info->v;
        CUR_SPRT->sp.clut = info->clut;
        CUR_SPRT->sp.w = info->w;
        CUR_SPRT->sp.h = info->h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = info->r;
        CUR_SPRT->sp.g0 = info->g;
        CUR_SPRT->sp.b0 = info->b;
        setDrawMode(&CUR_SPRT->dm, 0, 0, info->tpage);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void renderDuelBackground(s32 brightness) {
    POLY_FT4 *poly;
    u8 *polyBuf;

    polyBuf = (u8 *)CURRENT_FRAME_BUFFER->unk4078[11];
    poly = (POLY_FT4 *)(polyBuf + 0x1E0);
    initPrimByType(0xC, poly, 0, 0);
    poly->r0 = brightness;
    poly->g0 = brightness;
    poly->b0 = brightness;
    poly->u0 = 0;
    poly->v0 = 0x47;
    poly->u1 = 0xA0;
    poly->v1 = 0x47;
    poly->u2 = 0;
    poly->v2 = 0xB6;
    poly->u3 = 0xA0;
    poly->v3 = 0xB6;
    poly->x0 = 0;
    poly->y0 = 0xB;
    poly->x1 = 0xA0;
    poly->y1 = 0xB;
    poly->x2 = 0;
    poly->y2 = 0x7A;
    poly->x3 = 0xA0;
    poly->y3 = 0x7A;
    poly->tpage = 0x1C;
    poly->clut = 0x7C33;
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], poly);
    poly = (POLY_FT4 *)(polyBuf + 0x208);
    initPrimByType(0xC, poly, 0, 0);
    poly->r0 = brightness;
    poly->g0 = brightness;
    poly->b0 = brightness;
    poly->u0 = 0;
    poly->v0 = 0x47;
    poly->u1 = 0xA0;
    poly->v1 = 0x47;
    poly->u2 = 0;
    poly->v2 = 0xB6;
    poly->u3 = 0xA0;
    poly->v3 = 0xB6;
    poly->x0 = 0x13F;
    poly->y0 = 0xB;
    poly->x1 = 0x9F;
    poly->y1 = 0xB;
    poly->x2 = 0x13F;
    poly->y2 = 0x7A;
    poly->x3 = 0x9F;
    poly->y3 = 0x7A;
    poly->tpage = 0x1C;
    poly->clut = 0x7C33;
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], poly);
    poly = (POLY_FT4 *)(polyBuf + 0x230);
    initPrimByType(0xC, poly, 0, 0);
    poly->r0 = brightness;
    poly->g0 = brightness;
    poly->b0 = brightness;
    poly->u0 = 0;
    poly->v0 = 0x47;
    poly->u1 = 0xA0;
    poly->v1 = 0x47;
    poly->u2 = 0;
    poly->v2 = 0xB6;
    poly->u3 = 0xA0;
    poly->v3 = 0xB6;
    poly->x0 = 0;
    poly->y0 = 0xE8;
    poly->x1 = 0xA0;
    poly->y1 = 0xE8;
    poly->x2 = 0;
    poly->y2 = 0x79;
    poly->x3 = 0xA0;
    poly->y3 = 0x79;
    poly->tpage = 0x1C;
    poly->clut = 0x7C33;
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], poly);
    poly = (POLY_FT4 *)(polyBuf + 0x258);
    initPrimByType(0xC, poly, 0, 0);
    poly->r0 = brightness;
    poly->g0 = brightness;
    poly->b0 = brightness;
    poly->u0 = 0;
    poly->v0 = 0x47;
    poly->u1 = 0xA0;
    poly->v1 = 0x47;
    poly->u2 = 0;
    poly->v2 = 0xB6;
    poly->u3 = 0xA0;
    poly->v3 = 0xB6;
    poly->x0 = 0x13F;
    poly->y0 = 0xE8;
    poly->x1 = 0x9F;
    poly->y1 = 0xE8;
    poly->x2 = 0x13F;
    poly->y2 = 0x79;
    poly->x3 = 0x9F;
    poly->y3 = 0x79;
    poly->tpage = 0x1C;
    poly->clut = 0x7C33;
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], poly);
}

void drawCardArtPlaceholder(s32 x, s32 y, s32 z, s32 index, u8 *cardSprite) {
    POLY_FT4 *poly;
    s32 u;

    poly = (POLY_FT4 *)((u8 *)CURRENT_FRAME_BUFFER->unk4078[11] + (index * 80 + 0x280));
    u = ((((Unk8006E050 *)PLAYER_PROFILES)->unk24 / 4) % 4) * 32;
    initPrimByType(0xC, poly, 1, 0);
    poly->r0 = 0x80;
    poly->g0 = 0x80;
    poly->b0 = 0x80;
    poly->u0 = u;
    poly->v0 = 0x40;
    poly->u1 = u + 0x20;
    poly->v1 = 0x40;
    poly->u2 = u;
    poly->v2 = 0x80;
    poly->u3 = u + 0x20;
    poly->v3 = 0x80;
    poly->x0 = x;
    poly->y0 = y;
    poly->x1 = x + 0x40;
    poly->y1 = y;
    poly->x2 = x;
    poly->y2 = y + 0x40;
    poly->x3 = x + 0x40;
    poly->y3 = y + 0x40;
    poly->tpage = 0x1E;
    poly->clut = 0x7FB0;
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], poly);
    if (cardSprite != 0) {
        poly++;
        initPrimByType(0xC, poly, 1, 0);
        poly->r0 = 0x80;
        poly->g0 = 0x80;
        poly->b0 = 0x80;
        poly->u0 = cardSprite[0x16];
        poly->v0 = cardSprite[0x17];
        poly->u1 = cardSprite[0x16] + 0x28;
        poly->v1 = cardSprite[0x17];
        poly->u2 = cardSprite[0x16];
        poly->v2 = cardSprite[0x17] + 0x27;
        poly->u3 = cardSprite[0x16] + 0x28;
        poly->v3 = cardSprite[0x17] + 0x27;
        poly->x0 = x;
        poly->y0 = y;
        poly->x1 = x + 0x40;
        poly->y1 = y;
        poly->x2 = x;
        poly->y2 = y + 0x40;
        poly->x3 = x + 0x40;
        poly->y3 = y + 0x40;
        poly->tpage = *(u16 *)(cardSprite + 0x12);
        poly->clut = *(u16 *)(cardSprite + 0x10);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], poly);
    }
}

void renderPhaseBanner(void) {
    POLY_FT4 *poly;
    s32 i;
    s32 age;
    s32 brightness;
    s8 step;
    s8 phase;
    u8 playerLabel;

    phase = DUEL_MSG_BAR.unk3;
    if (phase == -1) {
        return;
    }
    playerLabel = DUEL_MSG_BAR.unk1;
    step = STATUS_STEP_SPRITES[DUEL_MSG_BAR.next];
    if (DUEL_MSG_BAR.unkC != playerLabel || DUEL_MSG_BAR.unkB != step || DUEL_MSG_BAR.unkD != phase) {
        DUEL_MSG_BAR.unk0 = 0;
        DUEL_MSG_BAR.unkE = 0;
        DUEL_MSG_BAR.unk10 = 0;
        DUEL_MSG_BAR.px = 0x154;
        DUEL_MSG_BAR.py = 0x66;
        DUEL_MSG_BAR.unkC = playerLabel;
        DUEL_MSG_BAR.unkB = step;
        DUEL_MSG_BAR.unkD = phase;
    }
    poly = (POLY_FT4 *)(CURRENT_FRAME_BUFFER->unk4078[11] + 0x320);
    switch ((u8)DUEL_MSG_BAR.unk0) {
    case 0:
        DUEL_MSG_BAR.px -= 14;
        if (DUEL_MSG_BAR.px < 0x5B) {
            DUEL_MSG_BAR.px = 0x5A;
            DUEL_MSG_BAR.unk0++;
        }
        break;
    case 1:
        DUEL_MSG_BAR.unkE += 2;
        for (i = 0; i < 6; i++) {
            age = DUEL_MSG_BAR.unkE - i * 3;
            brightness = 0x100 - age * 20;
            if (brightness >= 0) {
                initPrimByType(0xC, poly, 1, 0);
                poly->r0 = brightness;
                poly->g0 = brightness;
                poly->b0 = brightness;
                poly->u0 = 0xD0;
                poly->v0 = (DUEL_MSG_BAR.unk1 * 12 + 0x100) % 0x100;
                poly->u1 = 0xFF;
                poly->v1 = (DUEL_MSG_BAR.unk1 * 12 + 0x100) % 0x100;
                poly->u2 = 0xD0;
                poly->v2 = (DUEL_MSG_BAR.unk1 * 12 + 0x100) % 0x100 + 12;
                poly->u3 = 0xFF;
                poly->v3 = (DUEL_MSG_BAR.unk1 * 12 + 0x100) % 0x100 + 12;
                poly->x0 = DUEL_MSG_BAR.px - age * 2;
                poly->y0 = DUEL_MSG_BAR.py - age * 2;
                poly->x1 = DUEL_MSG_BAR.px + 0x30;
                poly->y1 = DUEL_MSG_BAR.py - age * 2;
                poly->x2 = DUEL_MSG_BAR.px - age * 2;
                poly->y2 = DUEL_MSG_BAR.py + 12;
                poly->x3 = DUEL_MSG_BAR.px + 0x30;
                poly->y3 = DUEL_MSG_BAR.py + 12;
                poly->tpage = 0x3E;
                poly->clut = 0x7CB3;
                addPrim(&CURRENT_FRAME_BUFFER->ot[0x1E], poly);
                poly++;
                initPrimByType(0xC, poly, 1, 0);
                poly->r0 = brightness;
                poly->g0 = brightness;
                poly->b0 = brightness;
                poly->u0 = 0xD0;
                poly->v0 = (DUEL_MSG_BAR.unk3 * 24 + 0x130) % 0x100;
                poly->u1 = 0xFC;
                poly->v1 = (DUEL_MSG_BAR.unk3 * 24 + 0x130) % 0x100;
                poly->u2 = 0xD0;
                poly->v2 = (DUEL_MSG_BAR.unk3 * 24 + 0x130) % 0x100 + 0x18;
                poly->u3 = 0xFC;
                poly->v3 = (DUEL_MSG_BAR.unk3 * 24 + 0x130) % 0x100 + 0x18;
                poly->x0 = DUEL_MSG_BAR.px - (s16)(age * 2 - 10);
                poly->y0 = DUEL_MSG_BAR.py - (s16)(age - 8);
                poly->x1 = (s16)(DUEL_MSG_BAR.px - (s16)(age * 2 - 10) + 0x2C) + age * 2;
                poly->y1 = DUEL_MSG_BAR.py - (s16)(age - 8);
                poly->x2 = DUEL_MSG_BAR.px - (s16)(age * 2 - 10);
                poly->y2 = (s16)(DUEL_MSG_BAR.py - (s16)(age - 8) + 0x18) + age * 2;
                poly->x3 = (s16)(DUEL_MSG_BAR.px - (s16)(age * 2 - 10) + 0x2C) + age * 2;
                poly->y3 = (s16)(DUEL_MSG_BAR.py - (s16)(age - 8) + 0x18) + age * 2;
                poly->tpage = 0x3E;
                poly->clut = 0x7CB3;
                addPrim(&CURRENT_FRAME_BUFFER->ot[0x1E], poly);
                poly++;
                initPrimByType(0xC, poly, 1, 0);
                poly->r0 = brightness;
                poly->g0 = brightness;
                poly->b0 = brightness;
                poly->u0 = 0xA0;
                poly->v0 = 0xA0;
                poly->u1 = 0xF0;
                poly->v1 = 0xA0;
                poly->u2 = 0xA0;
                poly->v2 = 0xB8;
                poly->u3 = 0xF0;
                poly->v3 = 0xB8;
                poly->x0 = DUEL_MSG_BAR.px + 0x30;
                poly->y0 = DUEL_MSG_BAR.py - (s16)(age - 8);
                poly->x1 = DUEL_MSG_BAR.px + 0x80 + age * 2;
                poly->y1 = DUEL_MSG_BAR.py - (s16)(age - 8);
                poly->x2 = DUEL_MSG_BAR.px + 0x30;
                poly->y2 = (s16)(DUEL_MSG_BAR.py - (s16)(age - 8) + 0x18) + age * 2;
                poly->x3 = DUEL_MSG_BAR.px + 0x80 + age * 2;
                poly->y3 = (s16)(DUEL_MSG_BAR.py - (s16)(age - 8) + 0x18) + age * 2;
                poly->tpage = 0x3C;
                poly->clut = 0x7CB3;
                addPrim(&CURRENT_FRAME_BUFFER->ot[0x1E], poly);
                poly++;
            }
        }
        if (++DUEL_MSG_BAR.unk10 > 0x10) {
            DUEL_MSG_BAR.unk0++;
        }
        break;
    case 2:
        DUEL_MSG_BAR.unkE = 0;
        DUEL_MSG_BAR.unk10 = 0;
        DUEL_MSG_BAR.tx = (DUEL_MSG_BAR.unk1 % 2) * -170 + 0xB8;
        DUEL_MSG_BAR.ty = (DUEL_MSG_BAR.unk1 % 2) * -136 + 0xA8;
        DUEL_MSG_BAR.unk0++;
        break;
    case 3:
        DUEL_MSG_BAR.unk10++;
        DUEL_MSG_BAR.px = (DUEL_MSG_BAR.tx - 0x5A) * DUEL_MSG_BAR.unk10 / 8 + 0x5A;
        DUEL_MSG_BAR.py = (DUEL_MSG_BAR.ty - 0x66) * DUEL_MSG_BAR.unk10 / 8 + 0x66;
        if (DUEL_MSG_BAR.unk10 >= 8) {
            DUEL_MSG_BAR.unk0++;
        }
        break;
    case 4:
        DUEL_MSG_BAR.px = DUEL_MSG_BAR.tx;
        DUEL_MSG_BAR.py = DUEL_MSG_BAR.ty;
        break;
    }
    if (STATUS_STEP_SPRITES[DUEL_MSG_BAR.next] != -1) {
        if (isSpritePoolFull() != 0) {
            return;
        }
        CUR_SPRT->sp.x0 = DUEL_MSG_BAR.px + 0xE;
        CUR_SPRT->sp.y0 = DUEL_MSG_BAR.py + 0x18;
        CUR_SPRT->sp.u0 = STATUS_STEP_SPRITES[DUEL_MSG_BAR.next] / 4 * 100;
        CUR_SPRT->sp.v0 = ((s8)(STATUS_STEP_SPRITES[DUEL_MSG_BAR.next] % 4) * 14 + 0x1B8) % 0x100;
        CUR_SPRT->sp.clut = 0x7DF3;
        CUR_SPRT->sp.w = 100;
        CUR_SPRT->sp.h = 14;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0x1E], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0x1E], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
    if (isSpritePoolFull() != 0) {
        return;
    }
    CUR_SPRT->sp.x0 = DUEL_MSG_BAR.px;
    CUR_SPRT->sp.y0 = DUEL_MSG_BAR.py;
    CUR_SPRT->sp.u0 = 0xD0;
    CUR_SPRT->sp.v0 = (DUEL_MSG_BAR.unk1 * 12 + 0x100) % 0x100;
    CUR_SPRT->sp.clut = 0x7C73;
    CUR_SPRT->sp.w = 0x2F;
    CUR_SPRT->sp.h = 12;
    setSemiTrans(&CUR_SPRT->sp, 1);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0x1E], &CUR_SPRT->sp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0x1E], &CUR_SPRT->dm);
    SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    if (isSpritePoolFull() != 0) {
        return;
    }
    CUR_SPRT->sp.x0 = DUEL_MSG_BAR.px + 10;
    CUR_SPRT->sp.y0 = DUEL_MSG_BAR.py + 8;
    CUR_SPRT->sp.u0 = 0xD0;
    CUR_SPRT->sp.v0 = (DUEL_MSG_BAR.unk3 * 24 + 0x130) % 0x100;
    CUR_SPRT->sp.clut = 0x7C73;
    CUR_SPRT->sp.w = 0x2C;
    CUR_SPRT->sp.h = 0x18;
    setSemiTrans(&CUR_SPRT->sp, 1);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0x1E], &CUR_SPRT->sp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0x1E], &CUR_SPRT->dm);
    SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    if (isSpritePoolFull() != 0) {
        return;
    }
    CUR_SPRT->sp.x0 = DUEL_MSG_BAR.px + 0x30;
    CUR_SPRT->sp.y0 = DUEL_MSG_BAR.py + 8;
    CUR_SPRT->sp.u0 = 0xA0;
    CUR_SPRT->sp.v0 = 0xA0;
    CUR_SPRT->sp.clut = 0x7C73;
    CUR_SPRT->sp.w = 0x50;
    CUR_SPRT->sp.h = 0x18;
    setSemiTrans(&CUR_SPRT->sp, 1);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0x1E], &CUR_SPRT->sp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0x1E], &CUR_SPRT->dm);
    SPRITE_POOL_CURSOR += sizeof(SprtPacket);
}

void renderStatusMessage(s32 brightness) {
    DISPENV env;
    Rect16 clipRect;
    u8 rgb[4];
    u8 buf[0x48];
    u8 *src;
    u8 *dst;
    s32 player;

    if (DUEL_MSG_BAR.next == -1) {
        return;
    }
    rgb[0] = brightness;
    rgb[1] = brightness;
    rgb[2] = brightness;
    GetDispEnv(&env);
    SetDrawArea(&STATUS_MSG_RESTORE_AREA[FRAME_BUFFER_INDEX], (Rect16 *)env.disp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFE], &STATUS_MSG_RESTORE_AREA[FRAME_BUFFER_INDEX]);
    if (DUEL_MSG_BAR.cur != DUEL_MSG_BAR.next) {
        if (++DUEL_MSG_BAR.y > 0x10) {
            DUEL_MSG_BAR.cur = DUEL_MSG_BAR.next;
            DUEL_MSG_BAR.player = ((u8 *)D_801D8340)[0x817];
        }
    } else if (DUEL_MSG_BAR.y != 0) {
        DUEL_MSG_BAR.y--;
    }
    if (DUEL_MSG_BAR.cur != -1) {
        src = STATUS_MESSAGE_TEXTS[DUEL_MSG_BAR.cur];
        dst = buf;
        do {
            if (*src < 0x81 || *src >= 0x99) {
                if (*src == '*' && src[1] == 'P') {
                    src += 2;
                    player = *src++ - '0';
                    player ^= DUEL_MSG_BAR.player;
                    *dst = 0;
                    strcpy((char *)dst, (char *)DUEL_PLAYERS[player] + 0x1CE);
                    dst += strlen(DUEL_PLAYERS[player] + 0x1CE);
                    continue;
                }
            } else {
                *dst++ = *src++;
            }
            *dst++ = *src++;
        } while (src[-1] != 0);
        drawTextColored(0x10, DUEL_MSG_BAR.y + 0xE, (s32)buf, (s32 *)rgb, 7, 0xFFE);
    }
    clipRect.x = env.disp[0] + 0x10;
    clipRect.y = env.disp[1] + 0xE;
    clipRect.w = 0x120;
    clipRect.h = 0xC;
    SetDrawArea(&STATUS_MSG_CLIP_AREA[FRAME_BUFFER_INDEX], &clipRect);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFE], &STATUS_MSG_CLIP_AREA[FRAME_BUFFER_INDEX]);
}

void renderHelpBar(s32 brightness) {
    DISPENV env;
    Rect16 clipRect;
    u8 rgb[4];

    if (DUEL_MSG_BAR.next2 == -1) {
        return;
    }
    rgb[0] = brightness;
    rgb[1] = brightness;
    rgb[2] = brightness;
    GetDispEnv(&env);
    SetDrawArea(&HELP_BAR_RESTORE_AREA[FRAME_BUFFER_INDEX], (Rect16 *)env.disp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFE], &HELP_BAR_RESTORE_AREA[FRAME_BUFFER_INDEX]);
    if (DUEL_MSG_BAR.cur2 != DUEL_MSG_BAR.next2 || DUEL_MSG_BAR.unkA != DUEL_MSG_BAR.unk1) {
        if (++DUEL_MSG_BAR.y2 > 0x10) {
            DUEL_MSG_BAR.cur2 = DUEL_MSG_BAR.next2;
            DUEL_MSG_BAR.unkA = DUEL_MSG_BAR.unk1;
        }
    } else if (DUEL_MSG_BAR.y2 != 0) {
        DUEL_MSG_BAR.y2--;
    }
    if (DUEL_MSG_BAR.cur2 != -1) {
        if (isSpritePoolFull() != 0) {
            return;
        }
        CUR_SPRT->sp.x0 = 0x10;
        CUR_SPRT->sp.y0 = 0xDB - DUEL_MSG_BAR.y2;
        CUR_SPRT->sp.u0 = 0xD0;
        CUR_SPRT->sp.v0 = (DUEL_MSG_BAR.unkA * 12 + 0x100) % 256;
        CUR_SPRT->sp.clut = 0x7C73;
        CUR_SPRT->sp.w = 0x2F;
        CUR_SPRT->sp.h = 0xC;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = brightness;
        CUR_SPRT->sp.g0 = brightness;
        CUR_SPRT->sp.b0 = brightness;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFE], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFE], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (DUEL_MSG_BAR.unkA == 1 && DUEL_MSG_BAR.cur2 != 2 && DUEL_MSG_BAR.cur2 != 0) {
            drawTextColored(0x50, 0xDB - DUEL_MSG_BAR.y2, (s32)"Thinking.....", (s32 *)rgb, 7, 0xFFE);
        } else {
            drawTextColored(0x40, 0xDB - DUEL_MSG_BAR.y2, (s32)HELP_BAR_TEXTS[DUEL_MSG_BAR.cur2], (s32 *)rgb, 7, 0xFFE);
        }
    }
    clipRect.x = env.disp[0] + 0x10;
    clipRect.y = env.disp[1] + 0xDB;
    clipRect.w = 0x120;
    clipRect.h = 0xC;
    SetDrawArea(&HELP_BAR_CLIP_AREA[FRAME_BUFFER_INDEX], &clipRect);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFE], &HELP_BAR_CLIP_AREA[FRAME_BUFFER_INDEX]);
}

void func_80044504(s32 x, s32 y, s32 n, s32 brightness, s32 z) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0xD0;
        CUR_SPRT->sp.v0 = (n * 12 + 0x153) % 256;
        CUR_SPRT->sp.clut = getClut(0x300, n + 0x1FC);
        CUR_SPRT->sp.w = 0x18;
        CUR_SPRT->sp.h = 0xC;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = brightness;
        CUR_SPRT->sp.g0 = brightness;
        CUR_SPRT->sp.b0 = brightness;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void drawWinMarker(s32 x, s32 y, s32 z) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0xD0;
        CUR_SPRT->sp.v0 = 0x47;
        CUR_SPRT->sp.clut = 0x7EF0;
        CUR_SPRT->sp.w = 0x20;
        CUR_SPRT->sp.h = 0xC;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void resetCardPolyCount(void) {
    CARD_POLY_COUNT = 0;
}

void projectCardSprite(void *cardSprite, s32 spriteIndex) {
    u8 *sprite;
    MATRIX matrix;
    SVECTOR vertices[4];
    s32 sxy[4];
    s32 depthCue;
    s32 otz;
    s32 flag;

    sprite = cardSprite;
    if (!(sprite[0x15] & 0x80)) {
        return;
    }
    PushMatrix();
    buildRotTransMatrix((VECTOR *)(sprite + 0x18), (SVECTOR *)(sprite + 0x28), &matrix);
    CompMatrix((MATRIX *)((u8 *)SCENE_3D + 0x78), &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    func_8005C444(&matrix);
    vertices[0].vx = -(*(s32 *)(sprite + 0x30) * 40) / 8192;
    vertices[0].vy = -(*(s32 *)(sprite + 0x30) * 48) / 8192;
    vertices[0].vz = 0;
    vertices[1].vx = (*(s32 *)(sprite + 0x30) * 40) / 8192;
    vertices[1].vy = -(*(s32 *)(sprite + 0x30) * 48) / 8192;
    vertices[1].vz = 0;
    vertices[2].vx = -(*(s32 *)(sprite + 0x30) * 40) / 8192;
    vertices[2].vy = (*(s32 *)(sprite + 0x30) * 48) / 8192;
    vertices[2].vz = 0;
    vertices[3].vx = (*(s32 *)(sprite + 0x30) * 40) / 8192;
    vertices[3].vy = (*(s32 *)(sprite + 0x30) * 48) / 8192;
    vertices[3].vz = 0;
    RotAverageNclip4((s32)&vertices[0], (s32)&vertices[1], (s32)&vertices[2], (s32)&vertices[3], (s32)&sxy[0], (s32)&sxy[1], (s32)&sxy[2],
                     (s32)&sxy[3], &depthCue, &otz, &flag);
    *(s32 *)(sprite + 0x38) = 0x57 - *(s16 *)(D_801D833C + spriteIndex * 36 + 0x20);
    if (*((s8 *)D_801D8340 + 0x81C) >= 0 && spriteIndex == *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x58) + 2)) {
        *(s32 *)(sprite + 0x38) = 0x33;
    }
    *(s16 *)(sprite + 0x34) = sxy[0];
    *(s16 *)(sprite + 0x36) = sxy[0] >> 16;
    PopMatrix();
}

void renderCardSprite(CardSprite *sprite, s32 spriteIndex) {
    MATRIX matrix;
    SVECTOR vertices[4];
    SVECTOR w[4];
    s32 sxy[4];
    s32 depthCue;
    s32 otz;
    s32 flag;
    s32 nclip;
    u32 *col;
    u32 *fade;
    RawPolyFT4 *buf;
    RawPolyFT4 *pk;
    u8 *duel;
    u8 *t;
    u16 clut;
    u16 tpage;
    u8 u0, v0, u1, v1, u2, v2, u3, v3;

    if (!(sprite->flags & 0x80)) {
        return;
    }
    PushMatrix();
    buildRotTransMatrix(&sprite->pos, &sprite->rot, &matrix);
    CompMatrix((MATRIX *)((u8 *)SCENE_3D + 0x78), &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    func_8005C444(&matrix);
    vertices[0].vx = -(sprite->scale * 40) / 8192;
    vertices[0].vy = -(sprite->scale * 48) / 8192;
    vertices[0].vz = 0;
    vertices[1].vx = (sprite->scale * 40) / 8192;
    vertices[1].vy = -(sprite->scale * 48) / 8192;
    vertices[1].vz = 0;
    vertices[2].vx = -(sprite->scale * 40) / 8192;
    vertices[2].vy = (sprite->scale * 48) / 8192;
    vertices[2].vz = 0;
    vertices[3].vx = (sprite->scale * 40) / 8192;
    vertices[3].vy = (sprite->scale * 48) / 8192;
    vertices[3].vz = 0;
    col = (u32 *)sprite->rgbc;
    fade = (u32 *)sprite->fade;
    buf = (RawPolyFT4 *)CURRENT_FRAME_BUFFER->unk4078[10];
    nclip = RotAverageNclip4((s32)&vertices[0], (s32)&vertices[1], (s32)&vertices[2], (s32)&vertices[3], (s32)&sxy[0], (s32)&sxy[1],
                             (s32)&sxy[2], (s32)&sxy[3], &depthCue, &otz, &flag);
    if (nclip <= 0) {
        otz = RotAverage4(&vertices[1], &vertices[0], &vertices[3], &vertices[2], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
    }
    if ((sprite->flags & 0x20) && isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = sxy[0] - 10;
        CUR_SPRT->sp.y0 = (sxy[0] >> 16) + 6;
        CUR_SPRT->sp.u0 = (u8)(sprite->num / 5) * 60;
        CUR_SPRT->sp.v0 = (u8)(sprite->num % 5) * 21 - 0x80;
        CUR_SPRT->sp.clut = 0x7DF2;
        CUR_SPRT->sp.w = 60;
        CUR_SPRT->sp.h = 21;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
    sprite->z = 0x57 - *(s16 *)(D_801D833C + spriteIndex * 36 + 0x20);
    duel = D_801D8340;
    if (*(s8 *)(duel + 0x81C) >= 0) {
        t = *(u8 **)(duel + 0x58);
        if (spriteIndex == *(s16 *)(t + 2)) {
            *(CardSprite **)(t + 4) = sprite;
            sprite->z = 0x33;
            func_801F8E34(*(u8 **)(duel + 0x58), 0x33);
        }
    }
    if (sprite->flags & 0x40) {
        if (sprite->t < 16) {
            sprite->t++;
        } else if ((sprite->to[0] | sprite->to[1] | sprite->to[2]) == 0) {
            sprite->flags &= ~0x40;
        }
        sprite->fade[0] = sprite->from[0] + (sprite->to[0] - sprite->from[0]) * sprite->t / 16;
        sprite->fade[1] = sprite->from[1] + (sprite->to[1] - sprite->from[1]) * sprite->t / 16;
        sprite->fade[2] = sprite->from[2] + (sprite->to[2] - sprite->from[2]) * sprite->t / 16;
        sprite->sx = sxy[0];
        sprite->sy = sxy[0] >> 16;
        pk = &buf[CARD_POLY_COUNT++];
        pk->tag = 0x09000000;
        pk->rgbc = *fade;
        pk->xy0 = sxy[0];
        pk->uv0 = 0x7DB24080;
        pk->xy1 = sxy[1];
        pk->uv1 = 0x3E40A8;
        pk->xy2 = sxy[2];
        pk->uv2 = 0x7080;
        pk->xy3 = sxy[3];
        pk->uv3 = 0x70A8;
        addPrim(&CURRENT_FRAME_BUFFER->ot[sprite->z], pk);
    }
    if (nclip <= 0) {
        otz = RotAverage4(&vertices[1], &vertices[0], &vertices[3], &vertices[2], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
        tpage = 0x1C;
        clut = 0x7C32;
        u0 = 0xA0;
        v0 = 0x6F;
        u1 = 0xC8;
        v1 = 0x6F;
        u2 = 0xA0;
        v2 = 0x9F;
        u3 = 0xC8;
        v3 = 0x9F;
    } else {
        w[0].vx = -(sprite->scale * 18) / 4096;
        w[0].vy = (sprite->scale * -19) / 4096;
        w[0].vz = 0;
        w[1].vx = (sprite->scale * 18) / 4096;
        w[1].vy = (sprite->scale * -19) / 4096;
        w[1].vz = 0;
        w[2].vx = -(sprite->scale * 18) / 4096;
        w[2].vy = (sprite->scale * 17) / 4096;
        w[2].vz = 0;
        w[3].vx = (sprite->scale * 18) / 4096;
        w[3].vy = (sprite->scale * 17) / 4096;
        w[3].vz = 0;
        u0 = sprite->u + 2;
        v0 = sprite->v + 2;
        u1 = sprite->u + 38;
        v1 = v0;
        u2 = u0;
        v2 = sprite->v + 38;
        u3 = u1;
        v3 = v2;
        otz = RotAverage4(&w[0], &w[1], &w[2], &w[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
        clut = sprite->clut;
        tpage = sprite->tpage;
        pk = &buf[CARD_POLY_COUNT++];
        pk->tag = 0x09000000;
        pk->rgbc = *col;
        pk->xy0 = sxy[0];
        pk->uv0 = (clut << 16) | (v0 << 8) | u0;
        pk->xy1 = sxy[1];
        pk->uv1 = (tpage << 16) | (v1 << 8) | u1;
        pk->xy2 = sxy[2];
        pk->uv2 = (v2 << 8) | u2;
        pk->xy3 = sxy[3];
        pk->uv3 = (v3 << 8) | u3;
        addPrim(&CURRENT_FRAME_BUFFER->ot[sprite->z], pk);
        otz = RotAverage4(&vertices[0], &vertices[1], &vertices[2], &vertices[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
        tpage = 0x1C;
        clut = getClut(800, 497 + sprite->pal);
        u0 = 0xCC;
        v0 = 0x6F;
        u1 = 0xF4;
        v1 = 0x6F;
        u2 = 0xCC;
        v2 = 0x9F;
        u3 = 0xF4;
        v3 = 0x9F;
    }
    sprite->sx = sxy[0];
    sprite->sy = sxy[0] >> 16;
    pk = &buf[CARD_POLY_COUNT++];
    pk->tag = 0x09000000;
    pk->rgbc = *col;
    pk->xy0 = sxy[0];
    pk->uv0 = (clut << 16) | (v0 << 8) | u0;
    pk->xy1 = sxy[1];
    pk->uv1 = (tpage << 16) | (v1 << 8) | u1;
    pk->xy2 = sxy[2];
    pk->uv2 = (v2 << 8) | u2;
    pk->xy3 = sxy[3];
    pk->uv3 = (v3 << 8) | u3;
    addPrim(&CURRENT_FRAME_BUFFER->ot[sprite->z], pk);
    PopMatrix();
}

MATRIX *buildRotTransMatrix(VECTOR *pos, SVECTOR *rot, MATRIX *m) {
    MATRIX tmp;
    SVECTOR r;

    r.vx = 0;
    r.vy = rot->vy;
    r.vz = 0;
    RotMatrix(&r, m);
    r.vx = rot->vx;
    r.vy = 0;
    r.vz = 0;
    RotMatrix(&r, &tmp);
    MulMatrix(m, &tmp);
    r.vx = 0;
    r.vy = 0;
    r.vz = rot->vz;
    RotMatrix(&r, &tmp);
    MulMatrix2(&tmp, m);
    MatrixNormal(m, &tmp);
    TransposeMatrix(&tmp, m);
    m->t[0] = pos->vx;
    m->t[1] = pos->vy;
    m->t[2] = pos->vz;
    return m;
}

/* the names of the card packs */
u8 * D_8006E31C[16] = {
    "Basic Pack",
    "Hyper Pack",
    "Super Pack",
    "Great Pack",
    "Red Pack",
    "Blue Pack",
    "Green Pack",
    "Black Pack",
    "Yellow Pack",
    "Option Pack",
    "S-Red Pack",
    "S-Blue Pack",
    "S-Green Pack",
    "S-Black Pack",
    "S-Yellow Pack",
    "S-Option Pack",
};
u8 REWARD_CARD_RANGES[16][18] = {
    { 0, 0xD, 0x63, 0, 0xE, 0x63, 0, 0xC, 0x63, 0, 0xD, 0x63, 0, 0xD, 0x63, 0, 0, 1 },
    { 0x1A, 0x16, 0x63, 0x1B, 0x17, 0x63, 0x1A, 0x15, 0x63, 0x1A, 0x16, 0x63, 0x19, 0x16, 0x63, 0, 0, 1 },
    { 0x1E, 0x63, 0, 0x1F, 0x63, 0, 0x1E, 0x63, 0, 0x1E, 0x63, 0, 0x1D, 0x63, 0, 0xA, 0, 1 },
    { 0x22, 0xEE, 0, 0x22, 0xEC, 0, 0x1F, 0xED, 0, 0x22, 0xED, 0, 0x20, 0xEE, 0, 0x63, 0xA, 5 },
    { 0x19, 0x63, 0x63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 0, 0, 0, 0x1A, 0x63, 0x63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 0, 0, 0, 0, 0, 0, 0x19, 0x63, 0x63, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x19, 0x63, 0x63, 0, 0, 0, 0, 0, 3 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x18, 0x63, 0x63, 0, 0, 3 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xF, 0, 1 },
    { 0x20, 0x63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 0, 0, 0, 0x20, 0x63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 0, 0, 0, 0, 0, 0, 0x1D, 0x63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x20, 0x63, 0, 0, 0, 0, 0, 0, 3 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1E, 0x63, 0, 0, 0, 3 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1D, 0xA, 6 },
};
/* the cross effects, as shown on a card */
u8 * CROSS_EFFECT_SHORT_NAMES[16] = {
    "none",
    "1stAttack",
    "\001\010 to 0",
    "\001\011 to 0",
    "\001\n"
        " to 0",
    "\001\010 Counter",
    "\001\011 Counter",
    "\001\n"
        " Counter",
    "Crash",
    "Eat-up HP",
    "Jamming",
    "\001\001 Foe X3",
    "\001\002 Foe X3",
    "\001\003 Foe X3",
    "\001\004 Foe X3",
    "\001\005 Foe X3",
};
/* the cross effects, as shown in the battle panel */
u8 * CROSS_EFFECT_NAMES[16] = {
    "none",
    "1stAttack",
    "*b0 to 0",
    "*b1 to 0",
    "*b2 to 0",
    "*b0 Counter",
    "*b1 Counter",
    "*b2 Counter",
    "Crash",
    "Eat-up HP",
    "Jamming",
    "*a0 Foe X3",
    "*a1 Foe X3",
    "*a2 Foe X3",
    "*a3 Foe X3",
    "*a4 Foe X3",
};
u8 D_8006E4FC[16] = { 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 3, 1, 1, 1, 1, 1 };
