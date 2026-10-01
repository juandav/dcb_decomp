#include "common.h"
#include "game.h"
#include "dcb/card_render.h"
#include "dcb/battle_hud.h"
#include "dcb/duel.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/sound.h"
#include "dcb/task.h"
#include "dcb/vram_upload.h"

/* jp's own object with card_render's first three functions (us's
   card_render.c): the duel's uploads to VRAM. jp's cards are numbered by kind
   (Digimon, then option cards from 0x6E, then Digivolve cards from 0x99), a
   name's glyphs are files of their own, and the duel's music is chosen here */

/* Uploads the glyphs of the first 8 characters of a Shift JIS string, one
   B:\NAME\<code>.tim file each, to row row of the name area */
void uploadStringGlyphs(u8 *string, s32 row, s32 parentTask) {
    char path[64];
    u32 *tim;
    s32 i;

    GLYPH_UPLOAD_BUSY = 1;
    i = 0;
    while (1) {
        if (*string == 0) {
            break;
        }
        sprintf(path, "B:\\NAME\\%4.4X.tim", (string[0] << 8) | string[1]);
        string += 2;
        spawnTask(0, -1, 0, 0x800, &loadFile, path, getCurrentTaskId());
        tim = (u32 *)waitFrames(0x7FFFFFFF);
        uploadTim(tim, i * 8 + 0x2C0, (row << 5) + 0x1C0, 0x2F0, row + 0x1D7);
        DrawSync(0);
        freeHeapBlock(tim);
        if (++i >= 8) {
            break;
        }
    }
    GLYPH_UPLOAD_BUSY = 0;
    resumeTask(parentTask);
}

void runCardArtLoader(void) {
    char path[64];
    u32 *tim;
    s32 i;
    s16 spriteIndex;
    s32 cardId;
    s32 slot;

    CARD_ART_LAST_SPRITE = -1;
    DUEL->stopArtLoader = 0;
    DUEL->artSlot = 0;
    for (i = 0; i < 6; i++) {
        DUEL->cache[i].id = -1;
        DUEL->cache[i].used = 0;
        DUEL->cache[i].age = 100;
    }
    for (;;) {
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
        if (SPRITE_KIND(spriteIndex) == 0x17) {
            continue;
        }
        if (spriteIndex != CARD_ART_LAST_SPRITE) {
            cardId = PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[spriteIndex % 30].index;
            switch (PLAYER_CARDS(PLAYER(DUEL->cursorPlayer))[spriteIndex % 30].type) {
            case 0:
                break;
            case 1:
                cardId += 0x6E;
                break;
            case 2:
                cardId += 0x99;
                break;
            }
            if (DUEL->loadBusy == 0) {
                DUEL->cache[slot].used = 0;
                CARD_ART_LAST_SPRITE = CUR_CARD;
                if (DUEL->cache[slot].id != cardId) {
                    DUEL->loadBusy = 1;
                    DUEL->cache[slot].id = cardId;
                    sprintf(path, "B:\\L_CARD\\LC%3.3d.TIM", cardId);
                    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
                    tim = (u32 *)waitFrames(0x7FFFFFFF);
                    uploadTim(tim, slot % 2 * 32 + 0x280, slot / 2 * 64 + 0x140, 0x2C0, slot + 0xF2);
                    DrawSync(0);
                    freeHeapBlock(tim);
                    DUEL->loadBusy = 0;
                }
                for (i = 0; i < 6; i++) {
                    if (DUEL->cache[i].age != 0) {
                        DUEL->cache[i].age--;
                    }
                }
                DUEL->cache[slot].age = 100;
            }
        }
        DUEL->cache[slot].used = 1;
    }
    DUEL->stopArtLoader = 0;
}

/* Uploads the duel's card graphics to VRAM: the 23 CBTL_SYS.ARC images and
   the art of both players' 30 cards (setting up their card sprites), then
   starts the duel's music: the CPU duel's (vsCpu), else one of two */
void loadDuelCardGraphics(s32 vsCpu) {
    char path[72]; /* unused, but it is in the original stack frame */
    u32 *arc;
    s32 i;
    s32 j;
    CardSprite *sprite;
    CardAnim *anim;
    s32 cardId;

    DUEL_VRAM_READY = 0;
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\CBTL_SYS.ARC", getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < 23; i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        DrawSync(0);
        waitFrames(FRAME_INTERVAL);
    }
    freeHeapBlock(arc);

    sprite = DUEL->sprites;
    anim = (CardAnim *)CARD_ANIMS;
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < 2; i++) {
        /* the card art, in a 6x5 grid of 42x41 cells per player */
        for (j = 0; j < 30; j++) {
            cardId = PLAYER_CARDS(PLAYER(i))[j].index;
            switch (PLAYER_CARDS(PLAYER(i))[j].type) {
            case 0:
                break;
            case 1:
                cardId += 0x6E;
                break;
            case 2:
                cardId += 0x99;
                break;
            }
            uploadTim((u32 *)((u8 *)arc + arc[cardId]),
                      (((i << 8) + (j % 6) * 42) >> 1) + 0x2C0, (j / 6) * 41, -1, -1);
            anim->spr = sprite;
            anim->state = 0;
            sprite->fade[3] = 0x2C;
            sprite->fade[0] = 0x80;
            sprite->fade[1] = 0x80;
            sprite->fade[2] = 0x80;
            sprite->tpage = getTPage(1, 0, (i << 7) + 0x2C0, 0);
            sprite->clut = getClut(LOADED_TIM.crect->x, LOADED_TIM.crect->y);
            /* option cards use palette 5, Digimon the one of their level */
            if (PLAYER_CARDS(PLAYER(i))[j].type != 0) {
                sprite->pal = 5;
            } else {
                sprite->pal = (u8)PLAYER_CARDS(PLAYER(i))[j].card[0x14] >> 4;
            }
            sprite->u = (j % 6) * 42;
            sprite->v = (j / 6) * 41;
            sprite++;
            anim++;
        }
    }
    DrawSync(0);
    waitFrames(FRAME_INTERVAL);
    freeHeapBlock(arc);
    waitFrames(10);
    waitForMusicChange();
    if (vsCpu != 0) {
        loadMusicTrack(0, ((SessionData *)SESSION_DATA)->duelMusic, 0x7F);
    } else if (rand() & 1) {
        loadMusicTrack(0, 0x1F, 0x7F);
    } else {
        loadMusicTrack(0, 0x25, 0x7F);
    }
    DUEL_VRAM_READY = 1;
}
