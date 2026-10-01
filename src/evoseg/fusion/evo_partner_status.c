#include "common.h"
#include "game.h"
#include "dcb/evo_partner_status.h"
#include "dcb/card_db.h"
#include "dcb/text.h"
#include "dcb/partner_level.h"
#include "dcb/prim.h"
#include "dcb/evoseg.h"
#include "dcb/evo_card_list.h"
#include "dcb/evo_data.h"

extern const char EVO_STR_SPEC[];

#define PARTNER(i) (((PlayerProfile *)PLAYER_PROFILES)->partners[i])

void EVO_drawPartnerList(UiWindow *w) {
    Rect16 rect;
    char text[72];
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 dim;
    s32 level;
    s32 index;
    s32 next;
    s32 labelPalette;

    x = w->originX;
    y = w->originY;
    z = w->z;
    for (i = 0; i < 3; i++) {
        if (PARTNER(i).cardId == 0) {
            continue;
        }
        if (EVO_FUSION.partner == i) {
            labelPalette = 6;
            dim = 0;
        } else {
            labelPalette = 6;
            dim = 1;
        }
        level = PARTNER(i).card[0].attr >> 4;
        if (level != 0) {
            level--;
        }
        index = getSlotPartnerIndex(0, i);
        rect.x = (index % 3) * 40;
        rect.y = (index / 3) * 40 + 0x140;
        rect.w = 40;
        rect.h = 40;
        drawTexturedSprite(x, y, &rect, 0x97, ((level + 0x1F8) << 6) | 0x18, z, dim ? 0x40 : 0x80, -1);
        drawTextColored(x + 0x2C, y + 2, PARTNER(i).card[0].name, EVO_TEXT_COLORS[dim].b, 8, z);
        drawLargeTextColored(x + 0x82, y + 4, "NEXT", labelPalette, EVO_TEXT_COLORS[dim].b, z);
        if ((s8)PARTNER(i).level < 99) {
            next = getExpForNextLevel((s8)PARTNER(i).level) - (u16)PARTNER(i).exp;
        } else {
            next = 0;
        }
        sprintf(text, "*s0%3d", next);
        drawTextColored(x + 0xB0, y + 2, text, EVO_TEXT_COLORS[dim].b, 8, z);
        y += 2;
        drawLargeTextColored(x + 0x2C, y + 0x10, "RANK", labelPalette, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%2d", (s8)PARTNER(i).level);
        drawTextColored(x + 0x56, y + 0xE, text, EVO_TEXT_COLORS[dim].b, 8, z);
        drawIconColored(x + 0x6C, y + 0xE, 0, 0x1A, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].hp);
        drawTextColored(x + 0x7A, y + 0xE, text, EVO_TEXT_COLORS[dim].b, 8, z);
        drawIconColored(x + 0x9C, y + 0xE, 0, 7, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].attack[0].power);
        drawTextColored(x + 0xAA, y + 0xE, text, EVO_TEXT_COLORS[dim].b, 8, z);
        drawIconColored(x + 0x6C, y + 0x1A, 0, 8, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].attack[1].power);
        drawTextColored(x + 0x7A, y + 0x1A, text, EVO_TEXT_COLORS[dim].b, 8, z);
        drawIconColored(x + 0x9C, y + 0x1A, 0, 9, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].attack[2].power);
        drawTextColored(x + 0xAA, y + 0x1A, text, EVO_TEXT_COLORS[dim].b, 8, z);
        drawLargeTextColored(x + 0x2C, y + 0x1C, "EXP", labelPalette, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%4d", (u16)PARTNER(i).exp);
        drawTextColored(x + 0x4A, y + 0x1A, text, EVO_TEXT_COLORS[dim].b, 8, z);
        y += 0x28;
    }
}

void EVO_drawPartnerStatus(UiWindow *w) {
    u8 palettes[8] = { 2, 1, 4, 9, 6, 8, 8, 8 };
    char text[72];
    Rect16 rect;
    Partner *partner;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 index;
    s32 next;

    x = w->originX;
    y = w->originY;
    z = w->z;
    partner = &PARTNER(EVO_FUSION.partner);
    w->palette = palettes[partner->card[0].attr >> 4];
    drawTextColored(x, y, partner->card[0].name, EVO_TEXT_COLORS[0].b, 7, z);
    drawLargeTextColored(x, y + 0x10, "RANK", 6, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%2d", (s8)partner->level);
    drawTextColored(x + 0x34, y + 0xE, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawLargeTextColored(x, y + 0x1E, "EXP", 6, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%4d", (u16)partner->exp);
    drawTextColored(x + 0x28, y + 0x1C, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawLargeTextColored(x, y + 0x2C, "NEXT", 6, EVO_TEXT_COLORS[0].b, z);
    next = 0;
    if ((s8)partner->level < 99) {
        next = getExpForNextLevel((s8)partner->level) - (u16)partner->exp;
    }
    sprintf(text, "*s0%3d", next);
    drawTextColored(x + 0x2E, y + 0x2A, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawIconColored(x, y + 0x38, 0, 0x1A, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].hp);
    drawTextColored(x + 0x28, y + 0x38, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawIconColored(x, y + 0x46, 0, 7, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].attack[0].power);
    drawTextColored(x + 0x28, y + 0x46, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawIconColored(x, y + 0x54, 0, 8, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].attack[1].power);
    drawTextColored(x + 0x28, y + 0x54, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawIconColored(x, y + 0x62, 0, 9, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].attack[2].power);
    drawTextColored(x + 0x28, y + 0x62, text, EVO_TEXT_COLORS[0].b, 7, z);
    for (i = 0; i < 4; i++) {
        if (EVO_STAT_BONUSES[i] > 0) {
            sprintf(text, "+%d", EVO_STAT_BONUSES[i]);
            drawTextColored(x + 0x42, y + (i + 4) * 14, text, EVO_TEXT_COLORS[0].b, 5, z);
        }
    }
    index = getSlotPartnerIndex(0, EVO_FUSION.partner);
    rect.x = (index % 3) * 84;
    rect.y = (index / 3) * 123;
    rect.w = 0x54;
    rect.h = 0x7B;
    drawTexturedSprite(x, y, &rect, 0x1A, getClut(index * 16 + 0x190, 0x1EF), z, 0x80, -1);
}

void EVO_drawCardInfo(UiWindow *w) {
    Rect16 rect;
    char text[72];
    s32 x;
    s32 y;
    s32 z;
    s16 id;

    x = w->originX;
    y = w->originY;
    z = w->z;
    id = EVO_FUSION.secondCard;
    if (id < 0xBF) {
        sprintf(text, EVO_FMT_CARD_NUMBER, ((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->id);
        drawText(x + 0x40, y + 0xD, (s32)text, 7, z);
        drawText(x, y, (s32)((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->name, 7, z);
        drawIcon(x + 0x46, y + 0x1A, 0, ((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->attr >> 4, z);
        drawIcon(x + 0x46, y + 0x27, 0, (((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->attr & 0xF) + 0x10, z);
        drawLargeText(x, y + 0x2A, (s32)"Level", 6, z);
    } else if ((id -= 0xBF) < 0x66) {
        sprintf(text, EVO_FMT_CARD_NUMBER, ((DigimonCardData *)(OPTION_CARDS + id * 0xE2))->id);
        drawText(x + 0x40, y + 0xD, (s32)text, 7, z);
        drawText(x, y, (s32)((DigimonCardData *)(OPTION_CARDS + id * 0xE2))->name, 7, z);
        drawIcon(x + 0x46, y + 0x1A, 0, 5, z);
    } else {
        id -= 0x66;
        sprintf(text, EVO_FMT_CARD_NUMBER, ((DigimonCardData *)(DIGIVOLVE_CARDS + id * 0x70))->id);
        drawText(x + 0x40, y + 0xD, (s32)text, 7, z);
        drawText(x, y, (s32)((DigimonCardData *)(DIGIVOLVE_CARDS + id * 0x70))->name, 7, z);
        drawIcon(x + 0x46, y + 0x1A, 0, 6, z);
    }
    drawLargeText(x, y + 0x12, (s32)EVO_STR_NUMBER, 6, z);
    drawLargeText(x, y + 0x1E, (s32)EVO_STR_SPEC, 6, z);
    rect.x = 0;
    rect.y = 0x90;
    rect.w = 0x28;
    rect.h = 0x28;
    drawTexturedSprite(x + 0x60, y + 0xD, &rect, 0x97, 0x7F18, z, 0x80, -1);
}

/* us's last two bytes are leftovers in the original, not zero padding */
#if VERSION_US
const char EVO_STR_SPEC[8] = "Spec.\0\x85\xA4";
#elif VERSION_EU
const char EVO_STR_SPEC[8] = "Spec.";
#else
#error "evoseg/fusion/evo_partner_status: version not checked"
#endif
