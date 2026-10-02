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
#include "dcb/duel.h"
#include "dcb/overlay_calls.h"

s32 GLYPH_UPLOAD_BUSY = 0;
s32 STAT_POPUP_RGB = 0x808080;

void uploadStringGlyphs(u8 *string, s32 row, s32 parentTask) {
    char path[64]; /* unused, but it is in the original stack frame */
    u8 *fontArchive;
    s32 i;

    GLYPH_UPLOAD_BUSY = 1;
    DUEL_VRAM_READY = 0;
    spawnTask(0, -1, 0, 0x800, &loadFile, "B:\\FONT.ARC", getCurrentTaskId());
    fontArchive = (u8 *)waitFrames(0x7FFFFFFF);
    i = 0;
    while (1) {
        if (*string == 0) {
            break;
        }
        uploadTim((u32 *)(fontArchive + ((s32 *)fontArchive)[*string - 0x20]), i * 4 + 0x2C0, (row << 5) + 0x1C0, 0x2F0,
                      row + 0x1D7);
        DrawSync(0);
        string++;
        waitFrames(FRAME_INTERVAL);
        if (++i >= 12) {
            break;
        }
    }
    freeHeapBlock(fontArchive);
    GLYPH_UPLOAD_BUSY = 0;
    resumeTask(parentTask);
}

void runCardArtLoader(void) {
    char path[72];
    u32 *tim;
    s32 i;
    s16 spriteIndex;
    s32 cardId;

    CARD_ART_LAST_SPRITE = -1;
    DUEL->stopArtLoader = 0;
    DUEL->artSlot = 0;
    for (i = 0; i < 6; i++) {
        DUEL->cache[i].id = -1;
        DUEL->cache[i].used = 0;
        DUEL->cache[i].age = 100;
    }
    for (;;) {
        s32 slot;

        waitFrames(FRAME_INTERVAL);
        slot = DUEL->artSlot % 6;
        DUEL->cache[slot].used = 0;
        if (DUEL->stopArtLoader != 0) {
            break;
        }
        if (DUEL->cursorSlot == -1 || DUEL->cursorSlot == 4) {
            continue;
        }
        spriteIndex = CUR_CARD;
        if (spriteIndex == -1) {
            continue;
        }
        if (SPRITE_KIND(spriteIndex) == 0x19) {
            continue;
        }
        if (spriteIndex != CARD_ART_LAST_SPRITE) {
            cardId = PLAYER(DUEL->cursorPlayer)->cards[spriteIndex % 30].id;
            if (DUEL->loadBusy != 0) {
                continue;
            }
            DUEL->cache[slot].used = 0;
            CARD_ART_LAST_SPRITE = CUR_CARD;
            if (DUEL->cache[slot].id != cardId) {
                DUEL->loadBusy = 1;
                DUEL->cache[slot].id = cardId;
                sprintf(path, "B:\\CARD\\LC%3.3d.TIM", cardId);
                spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
                tim = (u32 *)waitFrames(0x7FFFFFFF);
                uploadTim(tim, slot % 2 * 32 + 0x280, slot / 2 * 64 + 0x140, 0, 0x1FF - slot);
                DrawSync(0);
                freeHeapBlock(tim);
                DUEL->loadBusy = 0;
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
    DUEL->stopArtLoader = 0;
}

/* Uploads the duel's card graphics to VRAM: the CBTL_SYS.ARC images, the art
   of both players' 30 cards (setting up their card sprites) and their partners'
   cards, plus the extra archive users when withExtras is set. */
#if VERSION_US || VERSION_EU
void loadDuelCardGraphics(s32 withExtras) {
#if VERSION_US
    /* fake match: only nextAnims[0] is used; the array keeps the cursor in
       memory and gives the frame the original's size (0x58), where reload
       left two spill slots that no C shape reproduces */
    CardAnim *nextAnims[3];
#define nextAnim nextAnims[0]
#elif VERSION_EU
    char path[16]; /* unused, but it is in the original stack frame */
    CardAnim *nextAnim;
#endif
    CardAnim *anim;
    u32 *arc;
    s32 i;
    s32 j;
    CardSprite *sprite;
    u8 id;
    u8 level;
    Partner *partners;

    DUEL_VRAM_READY = 0;
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\CBTL_SYS.ARC", getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    /* the archive starts with its offset table: its size / 4 - 1 images */
    for (i = 0; i < (s32)(arc[0] >> 2) - 1; i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        DrawSync(0);
        waitFrames(FRAME_INTERVAL);
    }
    freeHeapBlock(arc);

    sprite = DUEL->sprites;
    nextAnim = (CardAnim *)CARD_ANIMS;
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < 2; i++) {
        /* the card art, in a 6x5 grid of 40x40 cells per player */
        for (j = 0; j < 30; j++) {
            uploadTim((u32 *)((u8 *)arc + arc[PLAYER(i)->cards[j].id]), (((i << 8) + (j % 6) * 40) >> 1) + 0x2C0,
                      (j / 6) * 40, -1, -1);
            anim = nextAnim;
            anim->spr = sprite;
            anim->state = 0;
            sprite->flags = 0;
            sprite->rgbc[3] = 0x2C;
            sprite->fade[3] = 0x2E;
            sprite->rgbc[0] = 0x80;
            sprite->rgbc[1] = 0x80;
            sprite->rgbc[2] = 0x80;
            sprite->fade[0] = 0;
            sprite->fade[1] = 0;
            sprite->fade[2] = 0;
            sprite->tpage = getTPage(1, 0, (i << 7) + 0x2C0, 0);
            sprite->clut = getClut(LOADED_TIM.crect->x, LOADED_TIM.crect->y);
            /* option cards use palette 5, Digimon the one of their level */
            if (PLAYER(i)->cards[j].type != 0) {
                level = 5;
            } else {
                level = (u8)PLAYER(i)->cards[j].card[0x1A] >> 4;
            }
            sprite->pal = level;
            sprite->u = (j % 6) * 40;
            sprite->v = (j / 6) * 40;
            sprite++;
            nextAnim++;
        }
        /* the partners' cards and their armor cards, in the row below */
        for (j = 0; j < 3; j++) {
            id = PLAYER_DATA(i).partners[j].cardId;
            if (id != 0) {
                uploadTim((u32 *)((u8 *)arc + arc[id]), (((i << 8) + j * 40) >> 1) + 0x2C0, 0xC8, -1, -1);
                partners = PLAYER_DATA(i).partners;
                id = partners[j].armorCardId;
                if (id != 0) {
                    uploadTim((u32 *)((u8 *)arc + arc[id]), (((i << 8) + (j + 3) * 40) >> 1) + 0x2C0, 0xC8, -1,
                              -1);
                    PLAYER(i)->armorCluts[j + 1] = getClut(LOADED_TIM.crect->x, LOADED_TIM.crect->y);
                }
            }
        }
    }
    if (withExtras != 0) {
        KAW_rollPrizeCards((u8 *)arc);
        KAW_uploadPartnerPortraits((u8 *)arc);
    }
    DrawSync(0);
    waitFrames(FRAME_INTERVAL);
    freeHeapBlock(arc);
    waitFrames(10);
    DUEL_VRAM_READY = 1;
}
#undef nextAnim
#else
#error "main/card/card_render: version not checked"
#endif

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
    POLY_FT4 *polys;

    /* four quads, one per screen quarter, from the same texture (mirrored) */
    polys = (POLY_FT4 *)CURRENT_FRAME_BUFFER->primSlots[11];
    poly = &polys[12];
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
    poly = &polys[13];
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
    poly = &polys[14];
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
    poly = &polys[15];
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

void drawCardArtPlaceholder(s32 x, s32 y, s32 z, s32 index, CardSprite *cardSprite) {
    POLY_FT4 *poly;
    s32 u;

    poly = &((POLY_FT4 *)CURRENT_FRAME_BUFFER->primSlots[11])[16 + index * 2];
    /* a 4-frame animation, one frame every 4 ticks */
    u = ((PLAYER_DATA(0).playTime / 4) % 4) * 32;
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
        poly->u0 = cardSprite->u;
        poly->v0 = cardSprite->v;
        poly->u1 = cardSprite->u + 0x28;
        poly->v1 = cardSprite->v;
        poly->u2 = cardSprite->u;
        poly->v2 = cardSprite->v + 0x27;
        poly->u3 = cardSprite->u + 0x28;
        poly->v3 = cardSprite->v + 0x27;
        poly->x0 = x;
        poly->y0 = y;
        poly->x1 = x + 0x40;
        poly->y1 = y;
        poly->x2 = x;
        poly->y2 = y + 0x40;
        poly->x3 = x + 0x40;
        poly->y3 = y + 0x40;
        poly->tpage = cardSprite->tpage;
        poly->clut = cardSprite->clut;
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], poly);
    }
}

void renderPhaseBanner(void) {
    POLY_FT4 *poly;
    s32 i;
    s32 age;
    s32 brightness;
    s32 step;
    s32 phase;
    u8 playerLabel;

    phase = DUEL_MSG_BAR.phase;
    if (phase == -1) {
        return;
    }
    playerLabel = DUEL_MSG_BAR.playerLabel;
    step = STATUS_STEP_SPRITES[DUEL_MSG_BAR.next];
    if (DUEL_MSG_BAR.bannerLabel != playerLabel || DUEL_MSG_BAR.bannerStep != step || DUEL_MSG_BAR.bannerPhase != phase) {
        DUEL_MSG_BAR.bannerState = 0;
        DUEL_MSG_BAR.echoAge = 0;
        DUEL_MSG_BAR.timer = 0;
        DUEL_MSG_BAR.px = 0x154;
        DUEL_MSG_BAR.py = 0x66;
        DUEL_MSG_BAR.bannerLabel = playerLabel;
        DUEL_MSG_BAR.bannerStep = step;
        DUEL_MSG_BAR.bannerPhase = phase;
    }
    poly = &((POLY_FT4 *)CURRENT_FRAME_BUFFER->primSlots[11])[20];
    /* 0: slide in from the right, 1: flash with fading echoes, 2-3: move to the
       corner of playerLabel's side, 4: stay there */
    switch ((u8)DUEL_MSG_BAR.bannerState) {
    case 0:
        DUEL_MSG_BAR.px -= 14;
        if (DUEL_MSG_BAR.px < 0x5B) {
            DUEL_MSG_BAR.px = 0x5A;
            DUEL_MSG_BAR.bannerState++;
        }
        break;
    case 1:
        DUEL_MSG_BAR.echoAge += 2;
        for (i = 0; i < 6; i++) {
            age = DUEL_MSG_BAR.echoAge - i * 3;
            brightness = 0x100 - age * 20;
            if (brightness >= 0) {
                initPrimByType(0xC, poly, 1, 0);
                poly->r0 = brightness;
                poly->g0 = brightness;
                poly->b0 = brightness;
                poly->u0 = 0xD0;
                poly->v0 = (DUEL_MSG_BAR.playerLabel * 12 + 0x100) % 0x100;
                poly->u1 = 0xFF;
                poly->v1 = (DUEL_MSG_BAR.playerLabel * 12 + 0x100) % 0x100;
                poly->u2 = 0xD0;
                poly->v2 = (DUEL_MSG_BAR.playerLabel * 12 + 0x100) % 0x100 + 12;
                poly->u3 = 0xFF;
                poly->v3 = (DUEL_MSG_BAR.playerLabel * 12 + 0x100) % 0x100 + 12;
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
                poly->v0 = (DUEL_MSG_BAR.phase * 24 + 0x130) % 0x100;
                poly->u1 = 0xFC;
                poly->v1 = (DUEL_MSG_BAR.phase * 24 + 0x130) % 0x100;
                poly->u2 = 0xD0;
                poly->v2 = (DUEL_MSG_BAR.phase * 24 + 0x130) % 0x100 + 0x18;
                poly->u3 = 0xFC;
                poly->v3 = (DUEL_MSG_BAR.phase * 24 + 0x130) % 0x100 + 0x18;
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
        if (++DUEL_MSG_BAR.timer > 0x10) {
            DUEL_MSG_BAR.bannerState++;
        }
        break;
    case 2:
        DUEL_MSG_BAR.echoAge = 0;
        DUEL_MSG_BAR.timer = 0;
        DUEL_MSG_BAR.tx = (DUEL_MSG_BAR.playerLabel % 2) * -170 + 0xB8;
        DUEL_MSG_BAR.ty = (DUEL_MSG_BAR.playerLabel % 2) * -136 + 0xA8;
        DUEL_MSG_BAR.bannerState++;
        break;
    case 3:
        DUEL_MSG_BAR.timer++;
        DUEL_MSG_BAR.px = (DUEL_MSG_BAR.tx - 0x5A) * DUEL_MSG_BAR.timer / 8 + 0x5A;
        DUEL_MSG_BAR.py = (DUEL_MSG_BAR.ty - 0x66) * DUEL_MSG_BAR.timer / 8 + 0x66;
        if (DUEL_MSG_BAR.timer >= 8) {
            DUEL_MSG_BAR.bannerState++;
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
    CUR_SPRT->sp.v0 = (DUEL_MSG_BAR.playerLabel * 12 + 0x100) % 0x100;
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
    CUR_SPRT->sp.v0 = (DUEL_MSG_BAR.phase * 24 + 0x130) % 0x100;
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
            DUEL_MSG_BAR.player = DUEL->turnPlayer;
        }
    } else if (DUEL_MSG_BAR.y != 0) {
        DUEL_MSG_BAR.y--;
    }
    if (DUEL_MSG_BAR.cur != -1) {
        src = STATUS_MESSAGE_TEXTS[DUEL_MSG_BAR.cur];
        dst = buf;
        /* copy the message, replacing "*P0"/"*P1" with a player's name; a
           two-byte Shift-JIS character (lead byte 0x81-0x98) is copied whole */
        do {
            if (*src < 0x81 || *src >= 0x99) {
                if (*src == '*' && src[1] == 'P') {
                    src += 2;
                    player = *src++ - '0';
                    player ^= DUEL_MSG_BAR.player;
                    *dst = 0;
                    strcpy((char *)dst, PLAYER(player)->name);
                    dst += strlen(PLAYER(player)->name);
                    continue;
                }
            } else {
                *dst++ = *src++;
            }
            *dst++ = *src++;
        } while (src[-1] != 0);
        drawTextColored(0x10, DUEL_MSG_BAR.y + 0xE, buf, rgb, 7, 0xFFE);
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
    if (DUEL_MSG_BAR.cur2 != DUEL_MSG_BAR.next2 || DUEL_MSG_BAR.curLabel2 != DUEL_MSG_BAR.playerLabel) {
        if (++DUEL_MSG_BAR.y2 > 0x10) {
            DUEL_MSG_BAR.cur2 = DUEL_MSG_BAR.next2;
            DUEL_MSG_BAR.curLabel2 = DUEL_MSG_BAR.playerLabel;
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
        CUR_SPRT->sp.v0 = (DUEL_MSG_BAR.curLabel2 * 12 + 0x100) % 256;
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
        if (DUEL_MSG_BAR.curLabel2 == 1 && DUEL_MSG_BAR.cur2 != 2 && DUEL_MSG_BAR.cur2 != 0) {
            drawTextColored(0x50, 0xDB - DUEL_MSG_BAR.y2, "Thinking.....", rgb, 7, 0xFFE);
        } else {
            drawTextColored(0x40, 0xDB - DUEL_MSG_BAR.y2, HELP_BAR_TEXTS[DUEL_MSG_BAR.cur2], rgb, 7, 0xFFE);
        }
    }
    clipRect.x = env.disp[0] + 0x10;
    clipRect.y = env.disp[1] + 0xDB;
    clipRect.w = 0x120;
    clipRect.h = 0xC;
    SetDrawArea(&HELP_BAR_CLIP_AREA[FRAME_BUFFER_INDEX], &clipRect);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFE], &HELP_BAR_CLIP_AREA[FRAME_BUFFER_INDEX]);
}

/* Draws the 24x12 badge of the attack panel: side 0 for the turn player,
   1 for the other (each has its own row and CLUT in the system texture). */
void drawTurnSideBadge(s32 x, s32 y, s32 side, s32 brightness, s32 z) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0xD0;
        CUR_SPRT->sp.v0 = (side * 12 + 0x153) % 256;
        CUR_SPRT->sp.clut = getClut(0x300, side + 0x1FC);
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

void projectCardSprite(CardSprite *sprite, s32 spriteIndex) {
    MATRIX matrix;
    SVECTOR vertices[4];
    s32 sxy[4];
    s32 depthCue;
    s32 otz;
    s32 flag;

    if (!(sprite->flags & 0x80)) {
        return;
    }
    PushMatrix();
    buildRotTransMatrix(&sprite->pos, &sprite->rot, &matrix);
    CompMatrix((MATRIX *)SCENE_3D->viewMatrix, &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    SetTransMatrix(&matrix);
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
    RotAverageNclip4((s32)&vertices[0], (s32)&vertices[1], (s32)&vertices[2], (s32)&vertices[3], (s32)&sxy[0], (s32)&sxy[1], (s32)&sxy[2],
                     (s32)&sxy[3], &depthCue, &otz, &flag);
    sprite->z = 0x57 - CARD_ANIM(spriteIndex)->count;
    /* the card under the cursor is drawn in front */
    if (DUEL->cursorSlot >= 0 && spriteIndex == CUR_CARD) {
        sprite->z = 0x33;
    }
    sprite->sx = sxy[0];
    sprite->sy = sxy[0] >> 16;
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
    Duel *duel;
    CardCursor *cursor;
    u16 clut;
    u16 tpage;
    u8 u0, v0, u1, v1, u2, v2, u3, v3;

    if (!(sprite->flags & 0x80)) {
        return;
    }
    PushMatrix();
    buildRotTransMatrix(&sprite->pos, &sprite->rot, &matrix);
    CompMatrix((MATRIX *)SCENE_3D->viewMatrix, &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    SetTransMatrix(&matrix);
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
    buf = (RawPolyFT4 *)CURRENT_FRAME_BUFFER->primSlots[10];
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
    sprite->z = 0x57 - CARD_ANIM(spriteIndex)->count;
    duel = DUEL;
    if (duel->cursorSlot >= 0) {
        cursor = (CardCursor *)duel->cursor;
        if (spriteIndex == cursor->id) {
            cursor->sprite = sprite;
            sprite->z = 0x33;
            KAW_renderCursor(duel->cursor, 0x33);
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
u8 * CARD_PACK_NAMES[16] = {
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
s8 REWARD_CARD_RANGES[16][6][3] = {
    { { 0, 0xD, 0x63 }, { 0, 0xE, 0x63 }, { 0, 0xC, 0x63 }, { 0, 0xD, 0x63 }, { 0, 0xD, 0x63 }, { 0, 0, 1 } },
    { { 0x1A, 0x16, 0x63 }, { 0x1B, 0x17, 0x63 }, { 0x1A, 0x15, 0x63 }, { 0x1A, 0x16, 0x63 }, { 0x19, 0x16, 0x63 }, { 0, 0, 1 } },
    { { 0x1E, 0x63, 0 }, { 0x1F, 0x63, 0 }, { 0x1E, 0x63, 0 }, { 0x1E, 0x63, 0 }, { 0x1D, 0x63, 0 }, { 0xA, 0, 1 } },
    { { 0x22, -0x12, 0 }, { 0x22, -0x14, 0 }, { 0x1F, -0x13, 0 }, { 0x22, -0x13, 0 }, { 0x20, -0x12, 0 }, { 0x63, 0xA, 5 } },
    { { 0x19, 0x63, 0x63 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 3 } },
    { { 0, 0, 0 }, { 0x1A, 0x63, 0x63 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 3 } },
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0x19, 0x63, 0x63 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 3 } },
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0x19, 0x63, 0x63 }, { 0, 0, 0 }, { 0, 0, 3 } },
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0x18, 0x63, 0x63 }, { 0, 0, 3 } },
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0xF, 0, 1 } },
    { { 0x20, 0x63, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 3 } },
    { { 0, 0, 0 }, { 0x20, 0x63, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 3 } },
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0x1D, 0x63, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 3 } },
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0x20, 0x63, 0 }, { 0, 0, 0 }, { 0, 0, 3 } },
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0x1E, 0x63, 0 }, { 0, 0, 3 } },
    { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0x1D, 0xA, 6 } },
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
u8 CROSS_EFFECT_ICONS[16] = { 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 3, 1, 1, 1, 1, 1 };
