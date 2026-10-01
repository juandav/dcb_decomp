#include "common.h"
#include "game.h"
#include "dcb/text.h"
#include "dcb/vram_upload.h"
#include "dcb/card_db.h"
#include "dcb/sound_play.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/duel_launch.h"
#include "dcb/pad.h"

/* jp's ending records (endseg.c is us's and eu's): the screen shows the deck
   the player beat the game with, every card in one list, the attack rates,
   the specialties, the results, the time the game took and the titles. It
   scrolls by itself the first time; once the game is beaten the player
   scrolls it by hand. Its texts are Japanese, drawn with the kanji glyphs
   the executable caches */

extern char *STR_TAMER_RANKS[8];
extern char *STR_COLLECTOR_RANKS[8];
extern char *STR_BATTLE_RANKS[8];

/* per specialty, [5] for all the cards: filled by END_computeEpithet (jp
   keeps them with the movie player's data, which ENDSEG starts zeroed) */
extern s32 END_SPECIALTY_WINS[8];
extern s32 END_SPECIALTY_LOSSES[8];
extern s32 END_SPECIALTY_CARDS[8];
extern s32 END_SPECIALTY_ORDER[6];

/* jp's kanji glyph cache in the executable (text_jp.c) */
void openKanjiPage(s32 page, s32 capacity);
void closeKanjiPage(s32 page);

/* not referenced by any code */
const char D_801EA3E8[] = "\n";

/* the epithets the ending can give; the last one when none fits */
char *END_EPITHETS[37] = {
    "c3エンペラー・カシオペア",
    "c3天地五界の覇王",
    "c3四星の守護者",
    "c3惑わしの四方天",
    "c3トリコロールマスター",
    "c3三光剣の魔匠",
    "c3炎海を司る導師",
    "c3孤高のレッドウルフ",
    "c3時を渡る風水士",
    "c3放たれた炎鬼",
    "c3夜霧の幻術師",
    "c3鋼牙の黒獅子",
    "c3パラダイスマシーン",
    "c3陽気な海賊野郎",
    "c3サバイバル・ガイ",
    "c3諸刃の裏道化師",
    "c2三つ首の炎竜",
    "c5三叉矛の海王",
    "c4クローバー・プリンス",
    "c8ダーク・オリオン",
    "c6謎の○△×星人",
    "c2猛炎の破壊王",
    "c5伝説の大津波",
    "c4ワイルダネスハンター",
    "c8黒翼の抹殺者",
    "c6黄金色の浮沈戦艦",
    "c2天駆ける赤騎士",
    "c5蒼風の氷神",
    "c4エメラルドの流星",
    "c8踊る黒い稲妻",
    "c6ピラミッドミラージュ",
    "c2紅蓮の魔術師",
    "c5ブルー・ガーディアン",
    "c4大自然の予言者",
    "c8冥界の影使い",
    "c6世紀末妨害王",
    "？？？？？？",
};

/* the specialties' names, by specialty ("1位 火炎カード") */
char *END_SPECIALTY_NAMES[5] = {
    "火炎",
    "氷水",
    "自然",
    "暗黒",
    "珍種",
};

#define PROFILE ((PlayerProfile *)PLAYER_PROFILES)

/* jp has 110 Digimon cards, and leaves the strongest two specialties to
   chance */
s32 END_computeEpithet(void) {
    s32 order[3];
    s32 i;
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
    for (i = 0; i < 0x6E; i++) {
        specialty = ((DigimonCardData *)DIGIMON_CARDS)[i].attr >> 4;
        END_SPECIALTY_WINS[specialty] += PROFILE->cardWins[i];
        END_SPECIALTY_LOSSES[specialty] += PROFILE->cardLosses[i];
        END_SPECIALTY_CARDS[specialty] += PROFILE->cardCollection[i] & 0xF;
        END_SPECIALTY_WINS[5] += PROFILE->cardWins[i];
        END_SPECIALTY_LOSSES[5] += PROFILE->cardLosses[i];
        END_SPECIALTY_CARDS[5] += PROFILE->cardCollection[i] & 0xF;
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
            if (PROFILE->attackCounts[order[i]] < PROFILE->attackCounts[order[i + 1]]) {
                tmp = order[i];
                order[i] = order[i + 1];
                order[i + 1] = tmp;
            }
        }
    }
    if (END_SPECIALTY_WINS[END_SPECIALTY_ORDER[0]] * 4 / 5 <= END_SPECIALTY_WINS[END_SPECIALTY_ORDER[4]]) {
        return rand() % 2;
    }
    if (END_SPECIALTY_WINS[END_SPECIALTY_ORDER[0]] * 4 / 5 <= END_SPECIALTY_WINS[END_SPECIALTY_ORDER[3]]) {
        return rand() % 2 + 2;
    }
    if (END_SPECIALTY_WINS[END_SPECIALTY_ORDER[0]] * 9 / 10 <= END_SPECIALTY_WINS[END_SPECIALTY_ORDER[2]]) {
        return rand() % 2 + 4;
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
    if (PROFILE->attackCounts[order[0]] * 9 / 10 < PROFILE->attackCounts[order[2]]) {
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

/* type: 0 Digimon, 1 option, 2 Digivolve card; id within that type. jp's card
   art is already in VRAM, 36 cards to a texture page */
void END_drawCardThumbnail(s32 x, s32 y, s32 type, s32 id) {
    s32 index;
    s32 texX;
    s32 texY;
    s32 palette;

    index = id;
    if (type == 1) {
        index = id + 0x6E;
    }
    if (type == 2) {
        index += 0x99;
    }
    texX = 0x180 + (index % 36) / 6 * 20 + (index / 36) * 128;
    texY = (index % 6) * 40;
    if (type == 0) {
        palette = (((DigimonCardData *)DIGIMON_CARDS)[id].attr >> 4) + 0xF9;
    } else {
        palette = type + 0xFD;
    }
    if (id >= 0x6C) {
        palette = 0xFD;
    }
    if (type == 1 && (u32)(id - 0x23) < 8) {
        palette = 0xFF;
    }
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = (texX % 64) * 2;
        CUR_SPRT->sp.v0 = texY % 256;
        CUR_SPRT->sp.clut = getClut(0x180, palette);
        CUR_SPRT->sp.w = 40;
        CUR_SPRT->sp.h = 40;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, texX, texY));
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (type == 0) {
            texX = (((DigimonCardData *)DIGIMON_CARDS)[id].attr >> 4) * 10 + 0x1C0;
        } else {
            texX = 0x1F2;
        }
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y - 3;
            CUR_SPRT->sp.u0 = (texX % 64) * 4;
            CUR_SPRT->sp.v0 = 0xBD;
            CUR_SPRT->sp.clut = getClut(0x1C0, 0x1FF);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, texX, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

void END_drawUnknownCardThumbnail(s32 x, s32 y) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y - 3;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0xC0;
        CUR_SPRT->sp.clut = getClut(0, 0x1F9);
        CUR_SPRT->sp.w = 40;
        CUR_SPRT->sp.h = 48;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x96);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/*
 * The player records screen: the deck the game was beaten with, the cards
 * owned, the player's attack rates, the specialties, the results, the time
 * the game took, the titles and the epithet END_computeEpithet picks. The
 * first time (the ending) it scrolls by itself to the end; once the game is
 * beaten the pad scrolls it and Circle quits at the end. Start quits.
 * parentTask is woken at the end.
 */
void END_runPlayerRecords(s32 parentTask) {
    char buf[0x40];
    s32 frame;
    s32 owned;
    s32 epithet;
    u32 *tims;
    s32 scroll;
    s32 i;
    s32 card;
    s32 base;
    s32 total;
    s32 pct;
    s32 type;
    s32 id;
    s32 hours;
    s32 minutes;

    openKanjiPage(0xF, 0x1E3);
    DUEL_VRAM_READY = 0;
    owned = 0;
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\ENDING.ARC", getCurrentTaskId());
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < 9; i++) {
        uploadTim((u32 *)((u8 *)tims + tims[i]), -1, -1, -1, -1);
        DrawSync(0);
        waitFrames(FRAME_INTERVAL);
    }
    freeHeapBlock(tims);
    playMusic(0, 0x32, 0x7F);
    if (!PROFILE->unk28_12) {
        PROFILE->clearTime = PROFILE->playTime;
    }
    scroll = 240;
    epithet = END_computeEpithet();
    frame = 0;
    while (1) {
        waitFrames(FRAME_INTERVAL);
        frame++;
        if (!(frame & 1)) {
            scroll--;
        }
        if (PROFILE->unk28_12) {
            if (PAD_STATES[0]->rawHeld & PAD_L1) {
                scroll += 41;
            }
            if (PAD_STATES[0]->rawHeld & PAD_R1) {
                scroll -= 40;
            }
            if (PAD_STATES[0]->rawHeld & PAD_UP) {
                scroll += 5;
            }
            if (PAD_STATES[0]->rawHeld & PAD_DOWN) {
                scroll -= 4;
            }
            if (scroll > 240) {
                scroll = 240;
            }
            if (scroll < -0x36EC) {
                scroll = -0x36EC;
            }
        }

        base = 0;
        if (scroll + base > -0x82 && scroll + base < 0xF0) {
            drawIconText(0x76, scroll + base, 6, 1, 0,
                         (s32)"殿堂入りデック" /* 殿堂入りデック */);
            drawIconText(0x28, scroll + base + 0x28, 6, 1, 0, (s32)"デック名" /* デック名 */);
            sprintf(buf, "%sデック" /* %sデック */, PROFILE->hallOfFameDeck.name);
            drawIconText(0x64, scroll + base + 0x28, 7, 1, 0, (s32)buf);
            sprintf(buf, "w-1%3d", PROFILE->hallOfFameDeck.wins);
            drawText(0xCE, scroll + base + 0x28, (s32)buf, 7, 0);
            drawIconText(0xE5, scroll + base + 0x28, 7, 1, 0, (s32)"勝" /* 勝 */);
            sprintf(buf, "w-1%3d", PROFILE->hallOfFameDeck.losses);
            drawText(0xF1, scroll + base + 0x28, (s32)buf, 7, 0);
            drawIconText(0x108, scroll + base + 0x28, 7, 1, 0, (s32)"敗" /* 敗 */);
            drawIconText(0x56, scroll + base + 0x50, 6, 1, 0,
                         (s32)"殿堂デック攻撃使用率" /* 殿堂デック攻撃使用率 */);
            total = PROFILE->hallOfFameDeck.attackCounts[0] + PROFILE->hallOfFameDeck.attackCounts[1] +
                    PROFILE->hallOfFameDeck.attackCounts[2];
            for (i = 0; i < 3; i++) {
                if (total != 0) {
                    pct = PROFILE->hallOfFameDeck.attackCounts[i] * 1000 / total;
                } else {
                    pct = 0;
                }
                sprintf(buf, "b%d　　．w7％" /* b%d　　．w7％ */, i);
                drawIconText(0x6E, scroll + base + 0x50 + 14 + i * 14, 7, 0, 0, (s32)buf);
                sprintf(buf, "w-1%3d", pct / 10);
                drawText(0x7B, scroll + 0x50 + base + 14 + i * 14, (s32)buf, 7, 0);
                sprintf(buf, "w-1%1d", pct % 10);
                drawText(0x96, scroll + 0x50 + base + 14 + i * 14, (s32)buf, 7, 0);
            }
        }
        for (card = 0; card < 30; card++) {
            if (scroll + ((card / 5) * 80 + 0xB4) >= -0x3C) {
                if (scroll + ((card / 5) * 80 + 0xB4) <= 0xF0) {
                    type = PROFILE->hallOfFameDeck.cards[card].type;
                    id = PROFILE->hallOfFameDeck.cards[card].index;
                    END_drawCardThumbnail((card % 5) * 50 + 0x1E, scroll + ((card / 5) * 80 + 0xB4) + 8, type, id);
                }
            }
        }

        base = 0x320;
        if (scroll + base > -0x82 && scroll + base < 0xF0) {
            drawIconText(0x76, scroll + base, 6, 1, 0, (s32)"所持カード" /* 所持カード */);
        }
        for (card = 0; card < 0x9F; card++) {
            if (scroll + (card * 80 + 0x348) >= -0x3C) {
                if (scroll + (card * 80 + 0x348) <= 0xF0) {
                    if (card < 0x6E) {
                        type = 0;
                        id = card;
                    } else if (card < 0x99) {
                        type = 1;
                        id = card - 0x6E;
                    } else {
                        type = 2;
                        id = card - 0x99;
                    }
                    switch (type) {
                    case 0:
                        owned = PROFILE->cardCollection[id] & 0xF;
                        if (owned != 0) {
                            sprintf(buf, "w-1%3d", PROFILE->cardWins[id]);
                            drawText(0xCC, scroll + (card * 80 + 0x348), (s32)buf, 7, 0);
                            drawIconText(0xE3, scroll + (card * 80 + 0x348), 7, 1, 0, (s32)"勝" /* 勝 */);
                            sprintf(buf, "w-1%3d", PROFILE->cardLosses[id]);
                            drawText(0xEF, scroll + (card * 80 + 0x348), (s32)buf, 7, 0);
                            drawIconText(0x106, scroll + (card * 80 + 0x348), 7, 1, 0, (s32)"敗" /* 敗 */);
                            for (i = 0; i < 3; i++) {
                                sprintf(buf, "w-1%5d", (u16)PROFILE->maxAttackPowers[id][i]);
                                drawText(0xAC, scroll + (card * 80 + 0x348) + 14 + i * 14, (s32)buf, 7, 0);
                                sprintf(buf, "最大b%d攻撃力" /* 最大b%d攻撃力 */, i);
                                drawIconText(0x64, scroll + (card * 80 + 0x348) + 14 + i * 14, 7, 1, 0, (s32)buf);
                            }
                        }
                        strcpy(buf, ((DigimonCardData *)DIGIMON_CARDS)[id].name);
                        break;
                    case 1:
                        strcpy(buf, ((OptionCardData *)OPTION_CARDS)[id].name);
                        owned = PROFILE->optionCollection[id] & 0xF;
                        break;
                    case 2:
                        strcpy(buf, ((DigivolveCardData *)DIGIVOLVE_CARDS)[id].name);
                        owned = PROFILE->digivolveCollection[id] & 0xF;
                        break;
                    }
                    if (owned != 0) {
                        drawIconText(0x64, scroll + (card * 80 + 0x348), 6, 1, 0, (s32)buf);
                        drawIconText(0xE1, scroll + (card * 80 + 0x348) + 0x1C, 6, 1, 0,
                                     (s32)"所持枚数" /* 所持枚数 */);
                        sprintf(buf, "%d", owned);
                        drawText(0xF1, scroll + (card * 80 + 0x348) + 0x2A, (s32)buf, 7, 0);
                        drawIconText(0xFB, scroll + (card * 80 + 0x348) + 0x2A, 7, 1, 0, (s32)"枚" /* 枚 */);
                        END_drawCardThumbnail(0x28, scroll + (card * 80 + 0x348) + 0xF, type, id);
                    } else {
                        drawIconText(0x64, scroll + (card * 80 + 0x348), 6, 1, 0,
                                     (s32)"？？？？？？？？" /* ？？？？？？？？ */);
                        END_drawUnknownCardThumbnail(0x28, scroll + (card * 80 + 0x348) + 0xF);
                    }
                    sprintf(buf, "Ｎｏ．%d" /* Ｎｏ．%d */, card + 1);
                    drawIconText((0x28 - measureText(1, (u8 *)buf)) / 2 + 0x28, scroll + (card * 80 + 0x348), 7, 1, 0, (s32)buf);
                }
            }
        }

        base = 0x3534;
        if (scroll + base > -0x3C && scroll + base < 0xF0) {
            drawIconText(0x56, scroll + base, 6, 1, 0,
                         (s32)"プレイヤー攻撃使用率" /* プレイヤー攻撃使用率 */);
            total = PROFILE->attackCounts[0] + PROFILE->attackCounts[1] + PROFILE->attackCounts[2];
            for (i = 0; i < 3; i++) {
                if (total != 0) {
                    pct = PROFILE->attackCounts[i] * 1000 / total;
                } else {
                    pct = 0;
                }
                sprintf(buf, "b%d　　．w7％" /* b%d　　．w7％ */, i);
                drawIconText(0x6E, scroll + base + 14 + i * 14, 7, 0, 0, (s32)buf);
                sprintf(buf, "w-1%3d", pct / 10);
                drawText(0x7B, scroll + base + 14 + i * 14, (s32)buf, 7, 0);
                sprintf(buf, "w-1%1d", pct % 10);
                drawText(0x96, scroll + base + 14 + i * 14, (s32)buf, 7, 0);
            }
        }

        base = 0x3598;
        if (scroll + base > -0x14 && scroll + base < 0xF0) {
            drawIconText(0x5A, scroll + base, 6, 1, 0,
                         (s32)"各カード属性別データ" /* 各カード属性別データ */);
        }
        for (i = 0; i < 5; i++) {
            if (scroll + base + 14 + i * 14 > -0x14 && scroll + base + 14 + i * 14 < 0xF0) {
                sprintf(buf, "%d", i + 1);
                drawText(0x32, scroll + base + 14 + i * 14, (s32)buf, 7, 0);
                sprintf(buf, "位 %sカード" /* 位 %sカード */, END_SPECIALTY_NAMES[END_SPECIALTY_ORDER[i]]);
                drawIconText(0x3C, scroll + base + 14 + i * 14, 7, 1, 0, (s32)buf);
                sprintf(buf, "w-1%3d", END_SPECIALTY_WINS[END_SPECIALTY_ORDER[i]]);
                drawText(0x92, scroll + base + 14 + i * 14, (s32)buf, 7, 0);
                drawIconText(0xA9, scroll + base + 14 + i * 14, 7, 1, 0, (s32)"勝" /* 勝 */);
                sprintf(buf, "w-1%3d", END_SPECIALTY_LOSSES[END_SPECIALTY_ORDER[i]]);
                drawText(0xB5, scroll + base + 14 + i * 14, (s32)buf, 7, 0);
                drawIconText(0xCC, scroll + base + 14 + i * 14, 7, 1, 0, (s32)"敗" /* 敗 */);
                sprintf(buf, "w-1%4d", END_SPECIALTY_CARDS[END_SPECIALTY_ORDER[i]]);
                drawText(0xD8, scroll + base + 14 + i * 14, (s32)buf, 7, 0);
                drawIconText(0xF6, scroll + base + 14 + i * 14, 7, 1, 0, (s32)"枚" /* 枚 */);
            }
        }
        if (scroll + base + 14 + i * 14 > -0x14 && scroll + base + 14 + i * 14 < 0xF0) {
            drawIconText(0x50, scroll + base + 14 + i * 14, 6, 1, 0, (s32)"全カード" /* 全カード */);
            sprintf(buf, "w-1%3d", END_SPECIALTY_WINS[5]);
            drawText(0x92, scroll + base + 14 + i * 14, (s32)buf, 6, 0);
            drawIconText(0xA9, scroll + base + 14 + i * 14, 6, 1, 0, (s32)"勝" /* 勝 */);
            sprintf(buf, "w-1%3d", END_SPECIALTY_LOSSES[5]);
            drawText(0xB5, scroll + base + 14 + i * 14, (s32)buf, 6, 0);
            drawIconText(0xCC, scroll + base + 14 + i * 14, 6, 1, 0, (s32)"敗" /* 敗 */);
            sprintf(buf, "w-1%4d", END_SPECIALTY_CARDS[5]);
            drawText(0xD8, scroll + base + 14 + i * 14, (s32)buf, 6, 0);
            drawIconText(0xF6, scroll + base + 14 + i * 14, 6, 1, 0, (s32)"枚" /* 枚 */);
        }

        base = 0x3624;
        if (scroll + base > -0x50 && scroll + base < 0xF0) {
            drawIconText(0x50, scroll + base, 6, 1, 0,
                         (s32)"ＣＯＭ対戦成績" /* ＣＯＭ対戦成績 */);
            sprintf(buf, "w-1%3d", PROFILE->battleWins);
            drawText(0xA0, scroll + base, (s32)buf, 7, 0);
            drawIconText(0xB7, scroll + base, 7, 1, 0, (s32)"勝" /* 勝 */);
            sprintf(buf, "w-1%3d", PROFILE->battleLosses);
            drawText(0xC3, scroll + base, (s32)buf, 7, 0);
            drawIconText(0xDA, scroll + base, 7, 1, 0, (s32)"敗" /* 敗 */);
        }
        if (scroll + base + 0x14 > -0x50 && scroll + base + 0x14 < 0xF0) {
            drawIconText(0x50, scroll + base + 0x14, 6, 1, 0,
                         (s32)"２Ｐ対戦成績" /* ２Ｐ対戦成績 */);
            sprintf(buf, "w-1%3d", PROFILE->versusWins);
            drawText(0xA0, scroll + base + 0x14, (s32)buf, 7, 0);
            drawIconText(0xB7, scroll + base + 0x14, 7, 1, 0, (s32)"勝" /* 勝 */);
            sprintf(buf, "w-1%3d", PROFILE->versusLosses);
            drawText(0xC3, scroll + base + 0x14, (s32)buf, 7, 0);
            drawIconText(0xDA, scroll + base + 0x14, 7, 1, 0, (s32)"敗" /* 敗 */);
        }
        if (scroll + base + 0x28 > -0x50 && scroll + base + 0x28 < 0xF0) {
            drawIconText(0x50, scroll + base + 0x28, 6, 1, 0,
                         (s32)"ＳＡＶＥ回数" /* ＳＡＶＥ回数 */);
            sprintf(buf, "w-1%3d", PROFILE->saveCount);
            drawText(0xA0, scroll + base + 0x28, (s32)buf, 7, 0);
            drawIconText(0xB7, scroll + base + 0x28, 7, 1, 0, (s32)"回" /* 回 */);
        }
        if (scroll + base + 0x3C > -0x50 && scroll + base + 0x3C < 0xF0) {
            hours = PROFILE->clearTime / 216000;
            minutes = PROFILE->clearTime % 216000 / 3600;
            drawIconText(0x50, scroll + base + 0x3C, 6, 1, 0,
                         (s32)"クリア時間" /* クリア時間 */);
            sprintf(buf, "%5d", hours);
            drawText(0xA0, scroll + base + 0x3C, (s32)buf, 7, 0);
            drawIconText(0xC8, scroll + base + 0x3C, 7, 1, 0, (s32)"時間" /* 時間 */);
            sprintf(buf, "%2d", minutes);
            drawText(0xE4, scroll + base + 0x3C, (s32)buf, 7, 0);
            drawIconText(0xF4, scroll + base + 0x3C, 7, 1, 0, (s32)"分" /* 分 */);
        }

        base = 0x3688;
        if (scroll + base > -0x3C && scroll + base < 0xF0) {
            drawIconText(0x50, scroll + base, 6, 1, 0, (s32)"バトル称号" /* バトル称号 */);
            drawIconText(0xA0, scroll + base, 7, 1, 0, (s32)STR_TAMER_RANKS[PROFILE->tamerRank]);
        }
        if (scroll + base + 0x14 > -0x3C && scroll + base + 0x14 < 0xF0) {
            drawIconText(0x50, scroll + base + 0x14, 6, 1, 0,
                         (s32)"コレクト称号" /* コレクト称号 */);
            drawIconText(0xA0, scroll + base + 0x14, 7, 1, 0, (s32)STR_COLLECTOR_RANKS[PROFILE->collectorRank]);
        }
        if (scroll + base + 0x28 > -0x3C && scroll + base + 0x28 < 0xF0) {
            drawIconText(0x50, scroll + base + 0x28, 6, 1, 0,
                         (s32)"２Ｐ対戦称号" /* ２Ｐ対戦称号 */);
            drawIconText(0xA0, scroll + base + 0x28, 7, 1, 0, (s32)STR_BATTLE_RANKS[PROFILE->battleRank]);
        }

        base = 0x3750;
        if (scroll + base > -0x3C && scroll + base < 0xF0) {
            sprintf(buf, "君こそ「%sc7」だ！" /* 君こそ「%sc7」だ！ */, END_EPITHETS[epithet]);
            drawIconText((0x140 - measureText(1, (u8 *)buf)) / 2, scroll + base, 7, 1, 0, (s32)buf);
        }
        if (PROFILE->unk28_12) {
            base = 0x37B4;
            if (scroll + base > -0x3C && scroll + base < 0xF0) {
                /* the epithet is passed but the text has no %s */
                sprintf(buf, "b0ボタンで終ります。" /* b0ボタンで終ります。 */,
                        END_EPITHETS[epithet]);
                drawIconText((0x140 - measureText(1, (u8 *)buf)) / 2, scroll + base, 7, 1, 0, (s32)buf);
                if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
                    break;
                }
            }
        } else if (scroll < -0x3778) {
            break;
        }
        if (PAD_STATES[0]->rawPressed & PAD_START) {
            break;
        }
    }
    PROFILE->unk28_12 = 1;
    closeKanjiPage(0xF);
    waitFrames(10);
    resumeTask(parentTask, -1);
    stopMusic();
}
