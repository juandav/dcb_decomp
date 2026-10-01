#include "common.h"
#include "game.h"
#include "dcb/text.h"
#include "dcb/vram_upload.h"
#include "dcb/card_db.h"
#include "dcb/window.h"
#include "dcb/memcard.h"
#include "dcb/sound_play.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/scroll_bg.h"
#include "dcb/loader.h"
#include "dcb/duel_launch.h"
#include "dcb/pad.h"

extern char *STR_TAMER_RANKS[8];
extern char *STR_COLLECTOR_RANKS[8];
extern char *STR_BATTLE_RANKS[8];

/* not referenced by any code */
const char D_801DDF38[] = "\n";

/* the epithets the ending can give */
char *END_EPITHETS[36] = {
    "*c3Emperor Cassiopeia",
    "*c3King of 5 Worlds",
    "*c3Guardian of 4 Stars",
    "*c3Illusive Sky Master",
    "*c3Tricolor Master",
    "*c3Sorcerer of Swords",
    "*c3Fire Ocean Master",
    "*c3Lonely Red Wolf",
    "*c3Time Traveler",
    "*c3Freed Fire Demon",
    "*c3Mystic Magician",
    "*c3Wild Black Lion",
    "*c3Paradise Machine",
    "*c3Cheerful Pirate",
    "*c3Survivor",
    "*c3Dark Clown",
    "*c23-headed Dragon",
    "*c5King of the Sea",
    "*c4Clover Prince",
    "*c8Dark Orion",
    "*c6Mysterious Alien",
    "*c2Bomber King",
    "*c5Legendary Tsunami",
    "*c4Wild Hunter",
    "*c8Black Assasin",
    "*c6Golden Invincible",
    "*c2Red Knight",
    "*c5Blue Ice Lord",
    "*c4Emerald Meteor",
    "*c8Black Lightning",
    "*c6Pyramid Mirage",
    "*c2Scarlet Magician",
    "*c5Blue Guardian",
    "*c4Prophet of the Wild",
    "*c8Shadowy Doom",
    "*c6Jamming King",
};

/* the specialty icons, by specialty ("Rank 1ST *a0 Card") */
char *END_SPECIALTY_ICONS[5] = {
    "*a0",
    "*a1",
    "*a2",
    "*a3",
    "*a4",
};

/* the ranks of the specialty table */
char *END_RANK_ORDINALS[5] = {
    "1ST",
    "2ND",
    "3RD",
    "4TH",
    "5TH",
};

/* the end-of-duel bonuses, as PlayerProfile.bonusCounts counts them */
char *END_BONUS_NAMES[32] = {
    "All *b0 Attack Win",
    "All *b1 Attack Win",
    "All *b2 Attack Win",
    "All or Nothing Gamble Win",
    "Last Chance Gamble Win",
    "No Support Card Win",
    "No Digivolve Win",
    "No Discard Win",
    "4-of-a-Kind Win",
    "0 Online Card Left win",
    "Partner Win",
    "No Loss Win",
    "Come-Back Win",
    "Desperate Win",
    "All Gone Win",
    "Ultimate Level Win",
    "Option Maniac Win",
    "8 DP Cards Win",
    "Lucky Seven Win",
    "Just Enough Attack Win",
    "12 S-Jewel Cards Win",
    "Choked Loss",
    "Loss by Gamble",
    "Total Loss",
    "Rainbow",
    "Damage Fever",
    "HP Fever",
    "3 Partners",
    "3 Partners Plus",
    "Partner Normal Digivolve",
    "Lucky Name",
    "Super Bonus",
};

/* per specialty, [5] for all the cards: filled by END_computeEpithet */
s32 END_SPECIALTY_WINS[8] = { 0 };
s32 END_SPECIALTY_LOSSES[8] = { 0 };
s32 END_SPECIALTY_CARDS[8] = { 0 };
s32 END_SPECIALTY_ORDER[6] = { 0 };
u8 *END_CARD_ART = 0;
s32 D_801E0D68 = 0;
/* where each section of the records starts, in the list the screen scrolls */
s32 END_SECTION_OFFSETS[12] = { 0 };

s32 END_computeEpithet(void) {
    s32 order[3];
    s32 i;
    s32 j;
    s32 k;
    s32 specialty;
    s32 tmp;

    for (i = 0; i < 6; i++) {
        END_SPECIALTY_WINS[i] = 0;
        END_SPECIALTY_LOSSES[i] = 0;
        END_SPECIALTY_CARDS[i] = 0;
        END_SPECIALTY_ORDER[i] = i;
    }
    for (i = 0; i < 3; i++) {
        order[i] = i;
    }
    for (i = 0; i < 0xBF; i++) {
        specialty = ((DigimonCardData *)DIGIMON_CARDS)[i].attr >> 4;
        END_SPECIALTY_WINS[specialty] += ((PlayerProfile *)PLAYER_PROFILES)->cardWins[i];
        END_SPECIALTY_LOSSES[specialty] += ((PlayerProfile *)PLAYER_PROFILES)->cardLosses[i];
        END_SPECIALTY_CARDS[specialty] += getOwnedCardCount(0, i);
        END_SPECIALTY_WINS[5] += ((PlayerProfile *)PLAYER_PROFILES)->cardWins[i];
        END_SPECIALTY_LOSSES[5] += ((PlayerProfile *)PLAYER_PROFILES)->cardLosses[i];
        END_SPECIALTY_CARDS[5] += getOwnedCardCount(0, i);
    }
    for (k = 0; k < 4; k++) {
        for (i = 0; i < 4; i++) {
            if (END_SPECIALTY_WINS[END_SPECIALTY_ORDER[i]] < END_SPECIALTY_WINS[END_SPECIALTY_ORDER[i + 1]]) {
                tmp = END_SPECIALTY_ORDER[i];
                END_SPECIALTY_ORDER[i] = END_SPECIALTY_ORDER[i + 1];
                END_SPECIALTY_ORDER[i + 1] = tmp;
            }
        }
    }
    for (k = 0; k < 2; k++) {
        for (i = 0; i < 2; i++) {
            if (((PlayerProfile *)PLAYER_PROFILES)->attackCounts[order[i]] < ((PlayerProfile *)PLAYER_PROFILES)->attackCounts[order[i + 1]]) {
                tmp = order[i];
                order[i] = order[i + 1];
                order[i + 1] = tmp;
            }
        }
    }
    if (END_SPECIALTY_WINS[END_SPECIALTY_ORDER[0]] * 9 / 10 <= END_SPECIALTY_WINS[END_SPECIALTY_ORDER[4]]) {
        return 0;
    }
    if (END_SPECIALTY_WINS[END_SPECIALTY_ORDER[0]] * 4 / 5 <= END_SPECIALTY_WINS[END_SPECIALTY_ORDER[4]]) {
        return 1;
    }
    if (END_SPECIALTY_WINS[END_SPECIALTY_ORDER[0]] * 9 / 10 <= END_SPECIALTY_WINS[END_SPECIALTY_ORDER[3]]) {
        return 2;
    }
    if (END_SPECIALTY_WINS[END_SPECIALTY_ORDER[0]] * 4 / 5 <= END_SPECIALTY_WINS[END_SPECIALTY_ORDER[3]]) {
        return 3;
    }
    if (END_SPECIALTY_WINS[END_SPECIALTY_ORDER[0]] * 19 / 20 <= END_SPECIALTY_WINS[END_SPECIALTY_ORDER[2]]) {
        return 4;
    }
    if (END_SPECIALTY_WINS[END_SPECIALTY_ORDER[0]] * 9 / 10 <= END_SPECIALTY_WINS[END_SPECIALTY_ORDER[2]]) {
        return 5;
    }
    if (END_SPECIALTY_WINS[END_SPECIALTY_ORDER[0]] * 9 / 10 <= END_SPECIALTY_WINS[END_SPECIALTY_ORDER[1]]) {
        switch ((1 << END_SPECIALTY_ORDER[0]) | (1 << END_SPECIALTY_ORDER[1])) {
        case 3:
            return 6;
        case 5:
            return 7;
        case 6:
            return 8;
        case 9:
            return 9;
        case 10:
            return 10;
        case 12:
            return 11;
        case 17:
            return 12;
        case 18:
            return 13;
        case 20:
            return 14;
        case 24:
            return 15;
        }
    }
    if (((PlayerProfile *)PLAYER_PROFILES)->attackCounts[order[0]] * 9 / 10 < ((PlayerProfile *)PLAYER_PROFILES)->attackCounts[order[2]]) {
        return END_SPECIALTY_ORDER[0] + 0x10;
    }
    switch (order[0]) {
    case 0:
        return END_SPECIALTY_ORDER[0] + 0x15;
    case 1:
        return END_SPECIALTY_ORDER[0] + 0x1A;
    case 2:
        return END_SPECIALTY_ORDER[0] + 0x1F;
    }
    return 0x24;
}

void END_drawCardThumbnail(s32 x, s32 y, s32 frame, s32 index) {
    u8 *tim;
    s32 u;

    tim = END_CARD_ART + ((s32 *)END_CARD_ART)[index];
    index %= 6;
    u = (index << 2) + index;
    uploadTim((u32 *)tim, u * 4 + 0x2C0, 0, -1, -1);
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x + 2;
        CUR_SPRT->sp.y0 = y + 5;
        CUR_SPRT->sp.u0 = u * 8 + 2;
        CUR_SPRT->sp.v0 = 2;
        CUR_SPRT->sp.clut = getClut(LOADED_TIM.crect->x, LOADED_TIM.crect->y);
        CUR_SPRT->sp.w = 36;
        CUR_SPRT->sp.h = 36;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x8B);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (frame == 6) {
            frame = 5;
        }
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0x80;
            CUR_SPRT->sp.clut = getClut(0x2C0, frame + 0xB1);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0xB);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

void END_drawUnknownCardThumbnail(s32 x, s32 y) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0x40;
        CUR_SPRT->sp.v0 = 0x80;
        CUR_SPRT->sp.clut = 0x2DEC;
        CUR_SPRT->sp.w = 40;
        CUR_SPRT->sp.h = 48;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0xB);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void END_drawScrollHelp(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    x += 40;
    drawText(x, y + 1, (s32)"*s0*b4: Scroll L1, R1: Fast Scroll", 7, z);
    drawText(x, y + 15, (s32)"*s0L2: Previous R2: Next *b6: Quit", 7, z);
}

#define PROFILE ((PlayerProfile *)PLAYER_PROFILES)

/*
 * The player records screen: the cards owned, the wins and losses per event
 * deck and per Com, the bonuses, trades, fusions, attack rates, specialties,
 * results and titles, and the epithet END_computeEpithet picks. mode 1 is
 * the ending's (it scrolls by itself until Cross), 2 the one browsed by hand
 * (Start quits). parentTask is woken at the end.
 */
void END_runPlayerRecords(s32 parentTask, s32 mode) {
    char buf[0x48];
    u16 comList[0x8E];
    u16 deckList[0x9F];
    UiWindow window;
    Rect16 rect;
    s32 owned;
    s32 epithet;
    s32 deckCount;
    s32 comCount;
    u8 *decks;
    u32 *tims;
    s32 scroll;
    s32 i;
    s32 cardId;
    s32 type;
    s32 base;
    s32 total;
    s32 pct;
    s32 idx;
    u8 *deckFile;
    s32 pos;

    changeScrollingBackground(7, 0x380, 0, 0x380, 0x80);
    DUEL_VRAM_READY = 0;
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\DECK2.DEK", getCurrentTaskId());
    deckFile = (u8 *)waitFrames(0x7FFFFFFF);
    *(u8 **)SESSION_DATA = deckFile;
    decks = deckFile + 8;
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    END_CARD_ART = (u8 *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\BCARD.ARC", getCurrentTaskId());
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < (s32)(tims[0] >> 2); i++) {
        uploadTim((u32 *)((u8 *)tims + tims[i]), -1, -1, -1, -1);
        DrawSync(0);
    }
    freeHeapBlock(tims);
    playMusic(0, 0x32, 0x7F);
    scroll = 40;
    epithet = END_computeEpithet();
    if (mode == 1) {
        scroll = 240;
    }
    for (i = 0; i < 0x8E; i++) {
        comList[i] = 0;
    }
    for (i = 0; i < 0x9F; i++) {
        deckList[i] = 0;
    }
    comCount = 0;
    for (i = 0; i < 0x8E; i++) {
        if (PROFILE->comWins[i] != 0) {
            comList[comCount] = i;
            comCount++;
        }
    }
    deckCount = 0;
    for (i = 0; i < 0x9F; i++) {
        if (PROFILE->opponentDeckFlags[i] != 0) {
            deckList[deckCount] = i;
            deckCount++;
        }
    }
    END_SECTION_OFFSETS[0] = 0;
    END_SECTION_OFFSETS[1] = 0x5E74;
    END_SECTION_OFFSETS[2] = deckCount * 14 + 0x5EC8;
    pos = deckCount * 14 + 0x5EE0;
    pos += comCount * 14;
    END_SECTION_OFFSETS[3] = pos + 0x3C;
    END_SECTION_OFFSETS[4] = pos + 0x250;
    END_SECTION_OFFSETS[5] = pos + 0x2CC;
    END_SECTION_OFFSETS[6] = pos + 0x35C;
    END_SECTION_OFFSETS[7] = pos + 0x3C2;
    END_SECTION_OFFSETS[8] = pos + 0x452;
    END_SECTION_OFFSETS[9] = pos + 0x4B8;
    END_SECTION_OFFSETS[10] = pos + 0x5D2;
    END_SECTION_OFFSETS[11] = pos + 0x63A;
    rect.x = 0xA;
    rect.y = 0xC6;
    rect.w = 0x12C;
    rect.h = 0x1C;
    openWindow(&window, &rect, -1, (s16 *)-1, 0, 0x31, 0x80, 0xC);
    if (mode == 2) {
        playMenuSound(3);
    }
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (mode == 1) {
            scroll--;
        } else {
            drawWindow(&window, END_drawScrollHelp, 0);
            if (PAD_STATES[0]->held & PAD_L1) {
                scroll += 40;
            } else if (PAD_STATES[0]->held & PAD_R1) {
                scroll -= 40;
            } else if (PAD_STATES[0]->held & PAD_UP) {
                scroll += 5;
            } else if (PAD_STATES[0]->held & PAD_DOWN) {
                scroll -= 4;
            } else if (PAD_STATES[0]->repeat & PAD_L2) {
                for (i = 1; i < 11; i++) {
                    if (-END_SECTION_OFFSETS[i] < scroll) {
                        if (scroll != 40) {
                            playMenuSound(2);
                            scroll = -END_SECTION_OFFSETS[i - 1] + 40;
                        }
                        break;
                    }
                }
            } else if (PAD_STATES[0]->repeat & PAD_R2) {
                for (i = 1; i < 11; i++) {
                    if (-END_SECTION_OFFSETS[i] + 40 < scroll) {
                        if (scroll != 200 - END_SECTION_OFFSETS[11]) {
                            playMenuSound(2);
                            scroll = -END_SECTION_OFFSETS[i] + 40;
                        }
                        break;
                    }
                }
            }
            if (PAD_STATES[0]->pressed & PAD_START) {
                playMenuSound(4);
                break;
            }
            if (END_SECTION_OFFSETS[0] + 40 < scroll) {
                scroll = -END_SECTION_OFFSETS[0] + 40;
            }
        }
        if (scroll < 200 - END_SECTION_OFFSETS[11]) {
            scroll = 200 - END_SECTION_OFFSETS[11];
        }

        base = END_SECTION_OFFSETS[0];
        if (scroll + base > -0x82 && scroll + base < 0xF0) {
            drawText(0x76, scroll + base, (s32)"Cards you own.", 6, 0);
        }
        base += 40;
        for (cardId = 0; cardId < 0x12D; cardId++) {
            if (scroll + base + cardId * 80 >= -60) {
                if (scroll + base + cardId * 80 <= 240) {
                    type = getCardSpecialty(cardId);
                    owned = getOwnedCardCount(0, cardId);
                    switch (type) {
                    case 5:
                        strcpy(buf, ((OptionCardData *)OPTION_CARDS)[cardId - 0xBF].name);
                        break;
                    case 6:
                        strcpy(buf, ((DigivolveCardData *)DIGIVOLVE_CARDS)[cardId - 0x125].name);
                        break;
                    default:
                        if (PROFILE->cardCollection[cardId] & 0x40) {
                            sprintf(buf, "*s0%3d*s1 *c6Win *c7*s0%3d*s1 *c6Loss", PROFILE->cardWins[cardId], PROFILE->cardLosses[cardId]);
                            drawText(0xD8, scroll + base + cardId * 80, (s32)buf, 7, 0);
                            for (i = 0; i < 3; i++) {
                                sprintf(buf, "Max *b%d Attack Power *s0%5d", i, (u16)PROFILE->maxAttackPowers[cardId][i]);
                                drawText(0x42, scroll + base + cardId * 80 + 14 + i * 14, (s32)buf, 7, 0);
                            }
                        }
                        strcpy(buf, ((DigimonCardData *)DIGIMON_CARDS)[cardId].name);
                        break;
                    }
                    if (PROFILE->cardCollection[cardId] & 0x40) {
                        drawText(0x46, scroll + base + cardId * 80, (s32)buf, 6, 0);
                        drawText(0xDE, scroll + base + cardId * 80 + 0x14, (s32)"Cards you own.", 6, 0);
                        sprintf(buf, "*s0%d*s1 *c6Cards", owned);
                        drawText(0xF4, scroll + base + cardId * 80 + 0x22, (s32)buf, 7, 0);
                        if (PROFILE->cardCollection[cardId] & 8) {
                            drawText(0xD6, scroll + base + cardId * 80 + 0x30, (s32)"Received by Trading.", 3, 0);
                        }
                        END_drawCardThumbnail(0x14, scroll + base + cardId * 80 + 0xF, type, cardId);
                    } else {
                        drawText(0x46, scroll + base + cardId * 80, (s32)"?????????????", 6, 0);
                        END_drawUnknownCardThumbnail(0x14, scroll + base + cardId * 80 + 0xF);
                    }
                    sprintf(buf, "No.%d", cardId);
                    drawText((0x28 - measureText(buf)) / 2 + 0x14, scroll + base + cardId * 80, (s32)buf, 7, 0);
                }
            }
        }

        base = END_SECTION_OFFSETS[1];
        if (scroll + base > -0x10 && scroll + base < 0xF0) {
            drawText(0x5C, scroll + base, (s32)"Wins & Losses per Event Deck", 6, 0);
        }
        base = END_SECTION_OFFSETS[1] + 0x18;
        for (i = 0; i < deckCount; i++) {
            if (scroll + base + i * 14 >= -16) {
                if (scroll + base + i * 14 <= 240) {
                    idx = deckList[i];
                    sprintf(buf, "%s Deck", decks + idx * 0x6E + 0x3C);
                    drawText(0x3C, scroll + base + i * 14, (s32)buf, 7, 0);
                    sprintf(buf, "*s0%3d*s1 *c6Win *c7*s0%3d*s1 *c6Loss", PROFILE->opponentDeckFlags[idx] & 0x3FFF, PROFILE->opponentDeckLosses[idx]);
                    drawText(0xC2, scroll + base + i * 14, (s32)buf, 7, 0);
                }
            }
        }

        base = END_SECTION_OFFSETS[2];
        if (scroll + base > -0x10 && scroll + base < 0xF0) {
            drawText(0x70, scroll + base, (s32)"Wins & Losses per Com", 6, 0);
        }
        base = END_SECTION_OFFSETS[2] + 0x18;
        for (i = 0; i < comCount; i++) {
            if (scroll + base + i * 14 < -16) {
                continue;
            }
            if (scroll + base + i * 14 > 240) {
                continue;
            }
            idx = comList[i];
            drawText(0x3C, scroll + base + i * 14, (s32)(decks + idx * 0x6E + 0x4F), 7, 0);
            sprintf(buf, "*s0%3d*s1 *c6Win *c7*s0%3d*s1 *c6Loss", PROFILE->comWins[idx], PROFILE->comLosses[idx]);
            drawText(0xC2, scroll + base + i * 14, (s32)buf, 7, 0);
        }

        base = END_SECTION_OFFSETS[3];
        if (scroll + base > -0x10 && scroll + base < 0xF0) {
            drawText(0x78, scroll + base, (s32)"Number of \"Bonuses\"", 6, 0);
        }
        base = END_SECTION_OFFSETS[3] + 0x18;
        for (i = 0; i < 0x20; i++) {
            if (scroll + base + i * 14 >= -16) {
                if (scroll + base + i * 14 <= 240) {
                    if (i != 0x1E) {
                        drawText(0x3C, scroll + base + i * 14, (s32)END_BONUS_NAMES[i], 7, 0);
                        sprintf(buf, "*s0%3d*s1 *c6Times", (u16)PROFILE->bonusCounts[i]);
                        drawText(0xDC, scroll + base + i * 14, (s32)buf, 7, 0);
                    }
                }
            }
        }

        base = END_SECTION_OFFSETS[4];
        if (scroll + base > -0x10 && scroll + base < 0xF0) {
            drawText(0x80, scroll + base, (s32)"Trading Info.", 6, 0);
        }
        base = END_SECTION_OFFSETS[4] + 0x18;
        if (scroll + base > -0x10 && scroll + base < 0xF0) {
            drawText(0x50, scroll + base, (s32)"Cards given away.", 6, 0);
            sprintf(buf, "*s0%4d*s1 *c6Cards", (u16)PROFILE->cardsGivenAway);
            drawText(0xB6, scroll + base, (s32)buf, 7, 0);
        }
        base = END_SECTION_OFFSETS[4] + 0x2C;
        if (scroll + base > -0x10 && scroll + base < 0xF0) {
            drawText(0x50, scroll + base, (s32)"Received Cards", 6, 0);
            sprintf(buf, "*s0%4d*s1 *c6Cards", (u16)PROFILE->cardsReceived);
            drawText(0xB6, scroll + base, (s32)buf, 7, 0);
        }

        base = END_SECTION_OFFSETS[5];
        if (scroll + base > -0x10 && scroll + base < 0xF0) {
            drawText(0x88, scroll + base, (s32)"Fusion Info.", 6, 0);
        }
        base = END_SECTION_OFFSETS[5] + 0x18;
        if (scroll + base > -0x10 && scroll + base < 0xF0) {
            drawText(0x50, scroll + base, (s32)"Used Cards", 6, 0);
            sprintf(buf, "*s0%4d*s1 *c6Cards", (u16)PROFILE->fusionCardsUsed);
            drawText(0xBE, scroll + base, (s32)buf, 7, 0);
        }
        base = END_SECTION_OFFSETS[5] + 0x2C;
        if (scroll + base > -0x10 && scroll + base < 0xF0) {
            drawText(0x50, scroll + base, (s32)"Fused Cards", 6, 0);
            sprintf(buf, "*s0%4d*s1 *c6Cards", (u16)PROFILE->fusedCards);
            drawText(0xBE, scroll + base, (s32)buf, 7, 0);
        }
        base = END_SECTION_OFFSETS[5] + 0x40;
        if (scroll + base > -0x10 && scroll + base < 0xF0) {
            drawText(0x50, scroll + base, (s32)"Fusion Mutations", 6, 0);
            sprintf(buf, "*s0%4d*s1 *c6Times", (u16)PROFILE->fusionMutations);
            drawText(0xBE, scroll + base, (s32)buf, 7, 0);
        }

        base = END_SECTION_OFFSETS[6];
        if (scroll + base > -0x3C && scroll + base < 0xF0) {
            drawText(0x64, scroll + base, (s32)"Player's Attack Rate", 6, 0);
            total = PROFILE->attackCounts[0] + PROFILE->attackCounts[1] + PROFILE->attackCounts[2];
            for (i = 0; i < 3; i++) {
                if (total != 0) {
                    pct = PROFILE->attackCounts[i] * 1000 / total;
                } else {
                    pct = 0;
                }
                sprintf(buf, "*s0*b%d%3d.%1d*w4*c6%%", i, pct / 10, pct % 10);
                drawText(0x7E, scroll + base + 14 + i * 14, (s32)buf, 7, 0);
            }
        }

        base = END_SECTION_OFFSETS[7];
        if (scroll + base > -0x14 && scroll + base < 0xF0) {
            drawText(0x50, scroll + base, (s32)"Speciality Data of Each Card", 6, 0);
        }
        for (i = 0; i < 5; i++) {
            if (scroll + base + 14 + i * 14 > -0x14 && scroll + base + 14 + i * 14 < 0xF0) {
                sprintf(buf, "Rank *s0%s*s1 *c7%s Card", END_RANK_ORDINALS[i], END_SPECIALTY_ICONS[END_SPECIALTY_ORDER[i]]);
                drawText(0x18, scroll + base + 14 + i * 14, (s32)buf, 6, 0);
                sprintf(buf, "*s0%4d*s1 *c6Win *c7*s0%4d*s1 *c6Loss *c7*s0%4d*s1 *c6Cards",
                        END_SPECIALTY_WINS[END_SPECIALTY_ORDER[i]], END_SPECIALTY_LOSSES[END_SPECIALTY_ORDER[i]], END_SPECIALTY_CARDS[END_SPECIALTY_ORDER[i]]);
                drawText(0x82, scroll + base + 14 + i * 14, (s32)buf, 7, 0);
            }
        }
        if (scroll + base + 14 + i * 14 > -0x14 && scroll + base + 14 + i * 14 < 0xF0) {
            drawText(0x36, scroll + base + 14 + i * 14, (s32)"All Cards", 6, 0);
            sprintf(buf, "*s0%4d*s1 *c6Win *c7*s0%4d*s1 *c6Loss *c7*s0%4d*s1 *c6Cards",
                    END_SPECIALTY_WINS[5], END_SPECIALTY_LOSSES[5], END_SPECIALTY_CARDS[5]);
            drawText(0x82, scroll + base + 14 + i * 14, (s32)buf, 7, 0);
        }

        base = END_SECTION_OFFSETS[8];
        if (scroll + base > -0x50 && scroll + base < 0xF0) {
            drawText(0x3C, scroll + base, (s32)"COM Battle Results", 6, 0);
            sprintf(buf, "*s0%3d*s1 *c6Win *c7*s0%3d*s1 *c6Loss", PROFILE->battleWins, PROFILE->battleLosses);
            drawText(0xB6, scroll + base, (s32)buf, 7, 0);
        }
        if (scroll + base + 0x14 > -0x50 && scroll + base + 0x14 < 0xF0) {
            drawText(0x3C, scroll + base + 0x14, (s32)"2P Battle Results", 6, 0);
            sprintf(buf, "*s0%3d*s1 *c6Win *c7*s0%3d*s1 *c6Loss", PROFILE->versusWins, PROFILE->versusLosses);
            drawText(0xB6, scroll + base + 0x14, (s32)buf, 7, 0);
        }
        if (scroll + base + 0x28 > -0x50 && scroll + base + 0x28 < 0xF0) {
            drawText(0x3C, scroll + base + 0x28, (s32)"Number of Saves", 6, 0);
            sprintf(buf, "*s0%3d*s1 *c6Times", PROFILE->saveCount);
            drawText(0xB6, scroll + base + 0x28, (s32)buf, 7, 0);
        }

        base = END_SECTION_OFFSETS[9];
        if (scroll + base > -0x3C && scroll + base < 0xF0) {
            drawText(0x4A, scroll + base, (s32)"Battle Title", 6, 0);
            drawText(0xAC, scroll + base, (s32)STR_TAMER_RANKS[PROFILE->tamerRank], 7, 0);
        }
        if (scroll + base + 0x14 > -0x3C && scroll + base + 0x14 < 0xF0) {
            drawText(0x4A, scroll + base + 0x14, (s32)"Collector Title", 6, 0);
            drawText(0xAC, scroll + base + 0x14, (s32)STR_COLLECTOR_RANKS[PROFILE->collectorRank], 7, 0);
        }
        if (scroll + base + 0x28 > -0x3C && scroll + base + 0x28 < 0xF0) {
            drawText(0x4A, scroll + base + 0x28, (s32)"2P Battle Title", 6, 0);
            drawText(0xAC, scroll + base + 0x28, (s32)STR_BATTLE_RANKS[PROFILE->battleRank], 7, 0);
        }

        base = END_SECTION_OFFSETS[10];
        if (scroll + base > -0x3C && scroll + base < 0xF0) {
            sprintf(buf, "You're \"%s*c7\"!", END_EPITHETS[epithet]);
            drawText((0x140 - measureText(buf)) / 2, scroll + base, (s32)buf, 7, 0);
        }

        if (mode == 1) {
            base = END_SECTION_OFFSETS[11];
            if (scroll + base > -0x3C && scroll + base < 0xF0) {
                sprintf(buf, "Push *b2 Button to Quit", END_EPITHETS[epithet]);
                drawText((0x140 - measureText(buf)) / 2, scroll + base, (s32)buf, 7, 0);
                if (PAD_STATES[0]->pressed & PAD_CROSS) {
                    break;
                }
            }
        }
    }
    if (mode == 2) {
        animateWindowTo(&window, (Rect16 *)-1);
        for (i = 0; i < 16; i++) {
            waitFrames(FRAME_INTERVAL);
            drawWindow(&window, END_drawScrollHelp, 0);
        }
    }
    PROFILE->unk28_12 = 1;
    waitFrames(10);
    freeHeapBlock(END_CARD_ART);
    freeHeapBlock(*(void **)SESSION_DATA);
    stopMusic();
    resumeTask(parentTask);
}

