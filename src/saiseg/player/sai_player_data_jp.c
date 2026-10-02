#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/pad.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/saiseg.h"

void runWindowTask();
extern s32 *SCROLLING_BACKGROUND;
extern char *STR_TAMER_RANKS[8];
extern char *STR_COLLECTOR_RANKS[8];
extern char *STR_BATTLE_RANKS[8];

void openKanjiPage(s32, s32);
void clearKanjiPage(s32);
void closeKanjiPage(s32);
void openChoiceMenu(void *, s32, s32, s32, s32);
void setBackgroundScrollMode(s32);
JpIcon *KAW_createCursor(s32, s32, s32, s32, s32);
void KAW_freeCursor(JpIcon *);
void KAW_drawCursor(JpIcon *);
/* no prototype: this module passes its coordinates as ints (dcb/prim.h) */
void initVramSprite();
void drawScrollArrow(s32, s32, s32, s32, s32);

void SAI_runWorldMap();

void SAI_findKeyItems(void);
void SAI_initKeyItemList(void);
void SAI_drawKeyItemText(JpWindow *win);
void SAI_drawPlayerData(JpWindow *win);
void SAI_drawKeyItemList(JpWindow *win);
void SAI_drawNothing(JpWindow *win);
void SAI_tickKeyItemInput(void);
void SAI_scrollKeyItemsLeft(void);
void SAI_scrollKeyItemsRight(void);
void SAI_openPlayerData(void);
void SAI_tickPlayerDataInput(void);
void SAI_closePlayerData(void);
void SAI_waitForKeyItems(void);
void SAI_scrollPlayerDataDown(void);
void SAI_scrollPlayerDataUp(void);

/* each item's name and the three lines about it */
char SAI_KEY_ITEM_TEXTS[14][4][0x31] = {
    {
        "c5デジヴァイス",
        "c6はじまりの街c7でc4ババモンc7にもらった",
        "デジモンワールド伝説の勇者のあかし。",
        "b0ボタンでステータス表示",
    },
    {
        "c5ニセバグカード",
        "c6ファクトリアルタウンc7でc4エテモンc7に",
        "作ってもらったバグカードのニセモノ。",
        "", /*  */
    },
    {
        "c5ニセメタルグレイモンカード",
        "c6はじまりの街c7でc4ベタモンc7にもらった",
        "メタルグレイモンカードのニセモノ。",
        "", /*  */
    },
    {
        "c5書き置き",
        "c6迷わずの森c7でc4クネモンc7にもらった",
        "c4ガブモンc7の書き置き。（ちょっと怪しい…）",
        "", /*  */
    },
    {
        "c5闇貴族の館のカギ",
        "c6オーバーデルc7のc4バケモンc7がc6竜の目の湖c7で",
        "なくしたカギ。c4イガモンc7が拾っていた。",
        "", /*  */
    },
    {
        "c5湖のぬしの笛",
        "c6フリーズランドc7でc4モジャモンc7がくれた",
        "不思議な笛。",
        "", /*  */
    },
    {
        "エンジェモンスタチュー",
        "いいいいい",
        "いいいいい",
        "いいいいい",
    },
    {
        "c5オイルタンク",
        "c6ドリルトンネルc7でc4タンクモンc7が浸かって",
        "いたオイル。",
        "", /*  */
    },
    {
        "c5トイレットペーパー",
        "c6ファクトリアルタウンc7でc4エテモンc7に作って",
        "もらったトイレットペーパー１００個。",
        "", /*  */
    },
    {
        "c5やったるでー協会会員証",
        "c6ミハラシ山c7でc4ゴブリモンc7が落としていった",
        "やったるでー協会の名誉会員証。",
        "", /*  */
    },
    {
        "c5ペーパーツリー",
        "c6ミスティツリーズc7でc4コカトリモンc7にもらった",
        "トイレットペーパーの原料。",
        "", /*  */
    },
    {
        "c5ゲコモン写真",
        "c6ゲッコー湿地c7でc4ゲコモンc7からもらった",
        "c4トノサマゲコモンc7の写真",
        "", /*  */
    },
    {
        "ペーパーツリー",
        "ミスティツリーズでコカトリモンにもらった",
        "トイレットペーパーの原料。",
        "", /*  */
    },
    {
        "c5グレートエンジェスタチュー（氷像）",
        "c6フリーズランドc7のc4エンジェモンc7が大事にして",
        "いた氷像。c4オーガモンc7が隠し持っていた。",
        "", /*  */
    },
};

JpWindowDef SAI_KEY_ITEM_TEXT_WINDOW_DEF = { { 0x17, 0x98, 0, 0xC }, { 0x17, 0x98, 0x112, 0x43 }, 0xA, 0, SAI_drawKeyItemText, SAI_drawNothing };
JpWindowDef SAI_KEY_ITEM_LIST_WINDOW_DEF = { { 0x17, 0x37, 0, 0xC }, { 0x17, 0x37, 0x112, 0x50 }, 0xA, 0, SAI_drawKeyItemList, SAI_drawNothing };
JpWindowDef SAI_PLAYER_DATA_WINDOW_DEF = { { 0x17, 0x37, 0, 0xC }, { 0x17, 0x37, 0x112, 0x70 }, 0xA, 0, SAI_drawPlayerData, SAI_drawNothing };

/* what the screen does each frame, by its state */
void (*SAI_KEY_ITEM_STATES[])(void) = {
    SAI_tickKeyItemInput, SAI_scrollKeyItemsLeft, SAI_scrollKeyItemsRight, SAI_openPlayerData, SAI_tickPlayerDataInput,
    SAI_closePlayerData, SAI_waitForKeyItems, SAI_scrollPlayerDataDown, SAI_scrollPlayerDataUp, NULL,
};

void SAI_findKeyItems(void) {
    /* the ids of the events that give each item */
    s16 events[14] = { 0x22, 0x85, 0x27, 0x36, 0x3D, 0xBE, -1, 0x4E, 0x87, 0x74, 0xB1, 0x99, -1, 0xD5 };
    s8 i;
    s32 n;
    s32 word;
    s32 bit;

    for (i = 0; i < 14; i++) {
        if (events[i] != -1) {
            n = events[i] - 0x22;
            bit = 0;
            if (n == 0) {
                word = bit;
            } else {
                word = n / 32;
                bit = n % 32;
            }
            if (PLAYER_DATA(0).eventFlags[word] & (1 << bit)) {
                SAI_OWNED_KEY_ITEMS[i] = 1;
            } else {
                SAI_OWNED_KEY_ITEMS[i] = 0;
            }
        }
    }
    SAI_KEY_ITEMS.noItems = 0;
    for (i = 0; i < 14 && SAI_OWNED_KEY_ITEMS[i] != 1; i++) {
        /* fake match: the empty loop ends a CSE block, so i is sign-extended
           again for the test below instead of reusing the index, as in the original */
        do {
        } while (0);
        if (i == 13) {
            SAI_KEY_ITEMS.noItems = 1;
        }
    }
}

void SAI_runKeyItems(void) {
    u8 buf[0x250];
    void (*fn)(void);

    SAI_KEY_ITEMS.running = 1;
    openKanjiPage(0xF, 0x1E3);
    clearKanjiPage(0xF);
    openChoiceMenu(buf, 0x3C, 0x32, 0, 0);
    SAI_findKeyItems();
    SAI_initKeyItemList();
    SAI_KEY_ITEMS.cursor = SAI_KEY_ITEMS.columns;
    SAI_KEY_ITEMS.icon = KAW_createCursor(1, 0x20, 0x1C, 5, 1);
    spawnTask(0, -1, 0, 0x800, runWindowTask, &SAI_KEY_ITEM_LIST_WINDOW_DEF, getCurrentTaskId());
    SAI_KEY_ITEMS.listWindow = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, runWindowTask, &SAI_KEY_ITEM_TEXT_WINDOW_DEF, getCurrentTaskId());
    SAI_KEY_ITEMS.textWindow = (JpWindow *)waitFrames(0x7FFFFFFF);
    waitFrames(60);
    do {
        waitFrames(FRAME_INTERVAL);
        fn = SAI_KEY_ITEM_STATES[SAI_KEY_ITEMS.state];
        if (fn != NULL) {
            fn();
        }
    } while (SAI_KEY_ITEMS.running != 0);
    *(s16 *)((u8 *)SCROLLING_BACKGROUND + 0x1BE) = 2;
    SAI_KEY_ITEMS.listWindow->state = 4;
    SAI_KEY_ITEMS.textWindow->state = 4;
    setBackgroundScrollMode(0);
    waitFrames(60);
    KAW_freeCursor(SAI_KEY_ITEMS.icon);
    closeKanjiPage(0xF);
    spawnTask(0, -1, 0, 0x800, SAI_runWorldMap, 1, 0, 0, 0);
    exitTask();
}

void SAI_clearKeyItemColumns(void) {
    s8 i;

    SAI_KEY_ITEMS.count = 0;
    SAI_KEY_ITEMS.scrollX = 0;
    for (i = 0; i < 5; i++) {
        SAI_KEY_ITEMS.columns[i].x = 0;
        SAI_KEY_ITEMS.columns[i].item = NULL;
    }
}

void SAI_initKeyItemList(void) {
    s32 x;
    s8 i;
    s8 j;
    s8 line;

    x = 0x140;
    SAI_clearKeyItemColumns();
    for (i = 0; i < 14; i++) {
        if (SAI_OWNED_KEY_ITEMS[i] != 0) {
            SAI_KEY_ITEM_LIST[SAI_KEY_ITEMS.count].x = x;
            line = i * 4;
            for (j = 0; j < 4; j++) {
                SAI_KEY_ITEM_LIST[SAI_KEY_ITEMS.count].lines[j] = SAI_KEY_ITEM_TEXTS[0][line];
                line++;
            }
            SAI_KEY_ITEMS.count++;
        }
        x += 0x20;
    }
    for (j = 0; j < 4; j++) {
        SAI_KEY_ITEM_LIST[SAI_KEY_ITEMS.count].lines[j] = NULL;
    }
    SAI_KEY_ITEM_LIST[SAI_KEY_ITEMS.count].x = 0;
    switch (SAI_KEY_ITEMS.count) {
    case 1:
        break;
    default:
        SAI_KEY_ITEMS.columns[3].x = 0x12A;
        SAI_KEY_ITEMS.columns[4].x = -0x2A;
        SAI_KEY_ITEMS.columns[3].item = &SAI_KEY_ITEM_LIST[3];
        SAI_KEY_ITEMS.columns[4].item = &SAI_KEY_ITEM_LIST[SAI_KEY_ITEMS.count - 1];
    case 3:
        SAI_KEY_ITEMS.columns[2].x = 0xD5;
        SAI_KEY_ITEMS.columns[2].item = &SAI_KEY_ITEM_LIST[2];
    case 2:
        SAI_KEY_ITEMS.columns[1].x = 0x80;
        SAI_KEY_ITEMS.columns[1].item = &SAI_KEY_ITEM_LIST[1];
        break;
    }
    SAI_KEY_ITEMS.columns[0].x = 0x2B;
    SAI_KEY_ITEMS.columns[0].item = SAI_KEY_ITEM_LIST;
    for (i = 0; i < 2; i++) {
        DB(i).primSlots[0] = (s32)(SAI_KEY_ITEMS.primBlocks[i] = allocTaskHeapBlock(0xA0));
    }
}

void SAI_drawKeyItemText(JpWindow *win) {
    s32 y;
    s8 i;

    y = 0xA0;
    if (SAI_KEY_ITEMS.noItems != 1) {
        for (i = 0; i < 4; i++) {
            drawIconText(0x2D, y, 7, 1, win->z, (s32)SAI_KEY_ITEMS.cursor->item->lines[i]);
            y += 0xE;
        }
    }
}

void SAI_scrollKeyItemsLeft(void) {
    s8 i;

    SAI_KEY_ITEMS.scrollX -= 10;
    if (SAI_KEY_ITEMS.scrollX < -0x54) {
        SAI_KEY_ITEMS.scrollX = 0;
        SAI_KEY_ITEMS.state = 0;
        for (i = 0; i < 5; i++) {
            SAI_KEY_ITEMS.columns[i].item++;
            if (SAI_KEY_ITEMS.columns[i].item->lines[0] == NULL) {
                SAI_KEY_ITEMS.columns[i].item = SAI_KEY_ITEM_LIST;
            }
        }
    }
}

void SAI_scrollKeyItemsRight(void) {
    s8 i;

    SAI_KEY_ITEMS.scrollX += 10;
    if (SAI_KEY_ITEMS.scrollX >= 0x55) {
        SAI_KEY_ITEMS.scrollX = 0;
        SAI_KEY_ITEMS.state = 0;
        for (i = 0; i < 5; i++) {
            if (SAI_KEY_ITEMS.columns[i].item == SAI_KEY_ITEM_LIST) {
                SAI_KEY_ITEMS.columns[i].item = &SAI_KEY_ITEM_LIST[SAI_KEY_ITEMS.count - 1];
            } else {
                SAI_KEY_ITEMS.columns[i].item--;
            }
        }
    }
}

void SAI_tickKeyItemInput(void) {
    if (SAI_KEY_ITEMS.noItems == 1) {
        if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
            SAI_KEY_ITEMS.running = 0;
            playSoundEffect(1);
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_RIGHT) {
        if (SAI_KEY_ITEMS.cursor[1].item == NULL) {
            return;
        }
        if (SAI_KEY_ITEMS.cursor == &SAI_KEY_ITEMS.columns[2]) {
            if (SAI_KEY_ITEMS.cursor[1].item == SAI_KEY_ITEM_LIST) {
                return;
            }
            SAI_KEY_ITEMS.state = 1;
        } else {
            SAI_KEY_ITEMS.cursor++;
        }
        playSoundEffect(2);
    } else if (PAD_STATES[0]->rawRepeat & PAD_LEFT) {
        if (SAI_KEY_ITEMS.cursor == SAI_KEY_ITEMS.columns) {
            if (SAI_KEY_ITEMS.columns[4].item == NULL) {
                return;
            }
            if (SAI_KEY_ITEMS.columns[4].item == &SAI_KEY_ITEM_LIST[SAI_KEY_ITEMS.count - 1]) {
                return;
            }
            SAI_KEY_ITEMS.state = 2;
        } else {
            if (SAI_KEY_ITEMS.cursor[-1].item == NULL) {
                return;
            }
            if (SAI_KEY_ITEMS.cursor == SAI_KEY_ITEMS.columns) {
                return;
            }
            SAI_KEY_ITEMS.cursor--;
        }
        playSoundEffect(2);
    } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
        SAI_KEY_ITEMS.running = 0;
        playSoundEffect(1);
    } else if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
        if ((SAI_KEY_ITEMS.cursor->item->x - 0x140) / 32 == 0) {
            playSoundEffect(0);
            SAI_KEY_ITEMS.state = 3;
            SAI_KEY_ITEMS.timer = 0;
            SAI_KEY_ITEMS.listWindow->state = 4;
            SAI_KEY_ITEMS.textWindow->state = 4;
        }
    }
}

void SAI_openPlayerData(void) {
    if (++SAI_KEY_ITEMS.timer == 60) {
        clearKanjiPage(0xF);
        spawnTask(0, -1, 0, 0x800, runWindowTask, &SAI_PLAYER_DATA_WINDOW_DEF, getCurrentTaskId());
        SAI_KEY_ITEMS.dataWindow = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
    if (SAI_KEY_ITEMS.timer >= 120) {
        SAI_KEY_ITEMS.timer = 0;
        SAI_KEY_ITEMS.state = 4;
        SAI_KEY_ITEMS.scrollY = 0;
        SAI_KEY_ITEMS.unk50 = 0;
    }
}

void SAI_tickPlayerDataInput(void) {
    if (PAD_STATES[0]->rawPressed & (PAD_CROSS | PAD_CIRCLE)) {
        SAI_KEY_ITEMS.dataWindow->state = 4;
        SAI_KEY_ITEMS.state = 5;
        SAI_KEY_ITEMS.timer = 0;
        if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
            playSoundEffect(0);
        } else {
            playSoundEffect(1);
        }
    } else if (PLAYER_DATA(0).unk28_12) {
        if ((PAD_STATES[0]->rawRepeat & PAD_UP) && SAI_KEY_ITEMS.scrollY < 0) {
            SAI_KEY_ITEMS.state = 8;
            playSoundEffect(2);
        } else if ((PAD_STATES[0]->rawRepeat & PAD_DOWN) && SAI_KEY_ITEMS.scrollY >= -0x4F) {
            SAI_KEY_ITEMS.state = 7;
            playSoundEffect(2);
        }
    }
}

void SAI_closePlayerData(void) {
    if (SAI_KEY_ITEMS.timer++ >= 60) {
        clearKanjiPage(0xF);
        SAI_KEY_ITEMS.timer = 0;
        SAI_KEY_ITEMS.state = 6;
        spawnTask(0, -1, 0, 0x800, runWindowTask, &SAI_KEY_ITEM_LIST_WINDOW_DEF, getCurrentTaskId());
        SAI_KEY_ITEMS.listWindow = (JpWindow *)waitFrames(0x7FFFFFFF);
        spawnTask(0, -1, 0, 0x800, runWindowTask, &SAI_KEY_ITEM_TEXT_WINDOW_DEF, getCurrentTaskId());
        SAI_KEY_ITEMS.textWindow = (JpWindow *)waitFrames(0x7FFFFFFF);
    }
}

void SAI_waitForKeyItems(void) {
    if (SAI_KEY_ITEMS.timer++ >= 60) {
        SAI_KEY_ITEMS.timer = 0;
        SAI_KEY_ITEMS.state = 0;
    }
}

void SAI_scrollPlayerDataDown(void) {
    if (SAI_KEY_ITEMS.timer++ >= 16) {
        SAI_KEY_ITEMS.timer = 0;
        SAI_KEY_ITEMS.state = 4;
        return;
    }
    SAI_KEY_ITEMS.scrollY--;
}

void SAI_scrollPlayerDataUp(void) {
    if (SAI_KEY_ITEMS.timer++ >= 16) {
        SAI_KEY_ITEMS.timer = 0;
        SAI_KEY_ITEMS.state = 4;
        return;
    }
    SAI_KEY_ITEMS.scrollY++;
}

void SAI_drawPlayerData(JpWindow *win) {
    char line[0x48];
    char money[0x10];
    char count[8];
    s32 top;
    s32 y;
    u16 total;
    u16 rate;
    s8 i;

    top = SAI_KEY_ITEMS.scrollY;
    y = top + 0x3B;
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"名前");
    drawIconText(0x91, y, 7, 1, win->z, (s32)PLAYER_DATA(0).name);
    y = top + 0x4B;
    /* 所持金　　　　　　　　　d0 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"所持金　　　　　　　　　d0");
    sprintf(money, "w-1%6d", PLAYER_DATA(0).bits);
    drawText(0x91, y, (s32)money, 7, win->z);
    y = top + 0x5B;
    /* バトル称号 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"バトル称号");
    drawIconText(0x91, y, 7, 1, win->z, (s32)STR_TAMER_RANKS[PLAYER_DATA(0).tamerRank]);
    y = top + 0x6B;
    /* コレクト称号 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"コレクト称号");
    drawIconText(0x91, y, 7, 1, win->z, (s32)STR_COLLECTOR_RANKS[PLAYER_DATA(0).collectorRank]);
    y = top + 0x7B;
    /* ２Ｐ対戦称号 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"２Ｐ対戦称号");
    drawIconText(0x91, y, 7, 1, win->z, (s32)STR_BATTLE_RANKS[PLAYER_DATA(0).battleRank]);
    y = top + 0x8B;
    /* ＣＯＭ対戦成績 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"ＣＯＭ対戦成績");
    sprintf(count, "w-1%3d", PLAYER_DATA(0).battleWins);
    drawText(0x98, y, (s32)count, 7, win->z);
    drawIconText(0xAF, y, 7, 1, win->z, (s32)"勝");
    sprintf(count, "w-1%3d", PLAYER_DATA(0).battleLosses);
    drawText(0xC3, y, (s32)count, 7, win->z);
    drawIconText(0xDB, y, 7, 1, win->z, (s32)"敗");
    y = top + 0x9B;
    /* ２Ｐ対戦成績 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"２Ｐ対戦成績");
    sprintf(count, "w-1%3d", PLAYER_DATA(0).versusWins);
    drawText(0x98, y, (s32)count, 7, win->z);
    drawIconText(0xAF, y, 7, 1, win->z, (s32)"勝");
    sprintf(count, "w-1%3d", PLAYER_DATA(0).versusLosses);
    drawText(0xC3, y, (s32)count, 7, win->z);
    drawIconText(0xDB, y, 7, 1, win->z, (s32)"敗");
    y = top + 0xAB;
    /* ＳＡＶＥ回数 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"ＳＡＶＥ回数");
    sprintf(count, "w-1%3d", PLAYER_DATA(0).saveCount);
    drawText(0x98, y, (s32)count, 7, win->z);
    drawIconText(0xAF, y, 7, 1, win->z, (s32)"回");
    /* 攻撃使用率 */
    drawIconText(0x2D, top + 0xBB, 6, 1, win->z, (s32)"攻撃使用率");
    y = top + 0xCB;
    total = PLAYER_DATA(0).attackCounts[2] + (PLAYER_DATA(0).attackCounts[0] + PLAYER_DATA(0).attackCounts[1]);
    for (i = 0; i < 3; i++) {
        if (total != 0) {
            rate = PLAYER_DATA(0).attackCounts[i] * 1000 / total;
        } else {
            rate = 0;
        }
        /* b%d　　．w7％ */
        sprintf(line, "b%d　　．w7％", i);
        drawIconText(0x91, y, 7, 0, win->z, (s32)line);
        sprintf(line, "w-1%3d", (u16)(rate / 10));
        drawText(0x9E, y, (s32)line, 7, 0);
        sprintf(line, "w-1%1d", (u16)(rate % 10));
        drawText(0xB9, y, (s32)line, 7, 0);
        y += 0xE;
    }
    if (PLAYER_DATA(0).unk28_12) {
        if (++SAI_KEY_ITEMS.unk50 > 0x40) {
            SAI_KEY_ITEMS.unk50 = 0;
        }
        if (SAI_KEY_ITEMS.unk50 > 0x20) {
            if (SAI_KEY_ITEMS.scrollY < 0) {
                drawScrollArrow(0x116, 0x42, 4, 5, win->z);
            }
            if (SAI_KEY_ITEMS.scrollY >= -0x4F) {
                drawScrollArrow(0x116, 0x94, 6, 5, win->z);
            }
        }
    }
}

void SAI_drawKeyItemList(JpWindow *win) {
    s8 i;
    s8 j;

    if (SAI_KEY_ITEMS.noItems != 1) {
        SAI_KEY_ITEMS.icon->x = SAI_KEY_ITEMS.cursor->x + 0x20;
        SAI_KEY_ITEMS.icon->y = 0x5E;
        KAW_drawCursor(SAI_KEY_ITEMS.icon);
        if (SAI_KEY_ITEMS.columns[4].item != NULL && SAI_KEY_ITEMS.columns[4].item != &SAI_KEY_ITEM_LIST[SAI_KEY_ITEMS.count - 1]) {
            if (SAI_KEY_ITEMS.rightBlink != 0) {
                SAI_KEY_ITEMS.leftBlink = SAI_KEY_ITEMS.rightBlink + 1;
            } else {
                SAI_KEY_ITEMS.leftBlink++;
            }
        } else {
            SAI_KEY_ITEMS.leftBlink = 0;
        }
        if (SAI_KEY_ITEMS.leftBlink & 0x20) {
            drawScrollArrow(0x1E, 0x58, 0, 5, win->z);
        }
        if (SAI_KEY_ITEMS.columns[3].item != NULL && SAI_KEY_ITEMS.columns[3].item != SAI_KEY_ITEM_LIST) {
            if (SAI_KEY_ITEMS.leftBlink != 0) {
                SAI_KEY_ITEMS.rightBlink = SAI_KEY_ITEMS.leftBlink;
            } else {
                SAI_KEY_ITEMS.rightBlink++;
            }
        } else {
            SAI_KEY_ITEMS.rightBlink = 0;
        }
        if (SAI_KEY_ITEMS.rightBlink & 0x20) {
            drawScrollArrow(0x11A, 0x58, 2, 5, win->z);
        }
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 5; j++) {
                if (SAI_KEY_ITEMS.columns[j].item != NULL) {
                    initVramSprite(DB(i).primSlots[0] + j * 0x20, SAI_KEY_ITEMS.columns[j].x + SAI_KEY_ITEMS.scrollX, 0x42, 0x7E40, 1,
                                   SAI_KEY_ITEMS.columns[j].item->x, 0xB0, 0x40, 0x38, -1);
                }
            }
        }
        for (i = 0; i < 5; i++) {
            if (SAI_KEY_ITEMS.columns[i].item != NULL) {
                AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[win->z], DB(FRAME_BUFFER_INDEX).primSlots[0] + i * 0x20);
            }
        }
    }
}

void SAI_drawNothing(JpWindow *win) {
}
