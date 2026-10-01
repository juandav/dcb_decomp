#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/pad.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/saiseg.h"

/* an item: its x in the list (0x140 + 0x20 * its number) and its four lines */
typedef struct {
    /* 0x00 */ s32 x;
    /* 0x04 */ char *lines[4];
} KeyItem;

/* a column of the list: where it is and the item it shows */
typedef struct {
    /* 0x00 */ s32 x;
    /* 0x04 */ KeyItem *item;
} KeyItemColumn;

/* what func_80044334 creates and obtainPartner (jp's 0x800447B4) draws */
typedef struct {
    /* 0x00 */ u8 unk0[8];
    /* 0x08 */ s16 x;
    /* 0x0A */ s16 y;
} JpIcon;

typedef struct {
    /* 0x00 */ JpIcon *icon;
    /* 0x04 */ KeyItemColumn columns[5];
    /* 0x2C */ KeyItemColumn *cursor;
    /* 0x30 */ JpWindow *listWindow;
    /* 0x34 */ JpWindow *textWindow;
    /* 0x38 */ JpWindow *dataWindow;
    /* 0x3C */ void *primBlocks[2];
    /* 0x44 */ s32 scrollX;
    /* 0x48 */ s32 timer;
    /* 0x4C */ s32 scrollY;
    /* 0x50 */ s8 unk50;
    /* 0x51 */ s8 count;
    /* 0x52 */ s8 state;
    /* 0x53 */ s8 leftBlink; /* the arrows blink while bit 5 is set */
    /* 0x54 */ s8 rightBlink;
    /* 0x55 */ s8 noItems;
    /* 0x56 */ s8 running;
} KeyItemScreen;

extern s8 SAI_OWNED_KEY_ITEMS[14];
extern KeyItemScreen SAI_KEY_ITEMS;
extern KeyItem SAI_KEY_ITEM_LIST[15];
extern void D_8002A3E0();
extern s32 *D_801E4640;
extern char *D_8007E5FC[8];
extern char *D_8007E61C[8];
extern char *D_8007E63C[8];

void openKanjiPage(s32, s32);
void clearKanjiPage(s32);
void closeKanjiPage(s32);
void func_8002B508(void *, s32, s32, s32, s32);
void func_8002CACC(s32);
JpIcon *func_80044334(s32, s32, s32, s32, s32);
void func_80044758(JpIcon *);
void obtainPartner(JpIcon *);
/* no prototype: this module passes its coordinates as ints (dcb/prim.h) */
void initVramSprite();
void drawScrollArrow(s32, s32, s32, s32, s32);

void func_801F35CC();

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
        "c5\x83" "f\x83W\x83\x94\x83@\x83" "C\x83X", /* c5デジヴァイス */
        "c6\x82\xCD\x82\xB6\x82\xDC\x82\xE8\x82\xCC\x8AXc7\x82\xC5" "c4\x83o\x83o\x83\x82\x83\x93" "c7\x82\xC9\x82\xE0\x82\xE7\x82\xC1\x82\xBD", /* c6はじまりの街c7でc4ババモンc7にもらった */
        "\x83" "f\x83W\x83\x82\x83\x93\x83\x8F\x81[\x83\x8B\x83h\x93`\x90\xE0\x82\xCC\x97" "E\x8E\xD2\x82\xCC\x82\xA0\x82\xA9\x82\xB5\x81" "B", /* デジモンワールド伝説の勇者のあかし。 */
        "b0\x83{\x83^\x83\x93\x82\xC5\x83X\x83" "e\x81[\x83^\x83X\x95\\\x8E\xA6", /* b0ボタンでステータス表示 */
    },
    {
        "c5\x83j\x83Z\x83o\x83O\x83J\x81[\x83h", /* c5ニセバグカード */
        "c6\x83t\x83@\x83N\x83g\x83\x8A\x83" "A\x83\x8B\x83^\x83" "E\x83\x93" "c7\x82\xC5" "c4\x83G\x83" "e\x83\x82\x83\x93" "c7\x82\xC9", /* c6ファクトリアルタウンc7でc4エテモンc7に */
        "\x8D\xEC\x82\xC1\x82\xC4\x82\xE0\x82\xE7\x82\xC1\x82\xBD\x83o\x83O\x83J\x81[\x83h\x82\xCC\x83j\x83Z\x83\x82\x83m\x81" "B", /* 作ってもらったバグカードのニセモノ。 */
        "", /*  */
    },
    {
        "c5\x83j\x83Z\x83\x81\x83^\x83\x8B\x83O\x83\x8C\x83" "C\x83\x82\x83\x93\x83J\x81[\x83h", /* c5ニセメタルグレイモンカード */
        "c6\x82\xCD\x82\xB6\x82\xDC\x82\xE8\x82\xCC\x8AXc7\x82\xC5" "c4\x83x\x83^\x83\x82\x83\x93" "c7\x82\xC9\x82\xE0\x82\xE7\x82\xC1\x82\xBD", /* c6はじまりの街c7でc4ベタモンc7にもらった */
        "\x83\x81\x83^\x83\x8B\x83O\x83\x8C\x83" "C\x83\x82\x83\x93\x83J\x81[\x83h\x82\xCC\x83j\x83Z\x83\x82\x83m\x81" "B", /* メタルグレイモンカードのニセモノ。 */
        "", /*  */
    },
    {
        "c5\x8F\x91\x82\xAB\x92u\x82\xAB", /* c5書き置き */
        "c6\x96\xC0\x82\xED\x82\xB8\x82\xCC\x90Xc7\x82\xC5" "c4\x83N\x83l\x83\x82\x83\x93" "c7\x82\xC9\x82\xE0\x82\xE7\x82\xC1\x82\xBD", /* c6迷わずの森c7でc4クネモンc7にもらった */
        "c4\x83K\x83u\x83\x82\x83\x93" "c7\x82\xCC\x8F\x91\x82\xAB\x92u\x82\xAB\x81" "B\x81i\x82\xBF\x82\xE5\x82\xC1\x82\xC6\x89\xF6\x82\xB5\x82\xA2\x81" "c\x81j", /* c4ガブモンc7の書き置き。（ちょっと怪しい…） */
        "", /*  */
    },
    {
        "c5\x88\xC5\x8BM\x91\xB0\x82\xCC\x8A\xD9\x82\xCC\x83J\x83M", /* c5闇貴族の館のカギ */
        "c6\x83I\x81[\x83o\x81[\x83" "f\x83\x8B" "c7\x82\xCC" "c4\x83o\x83P\x83\x82\x83\x93" "c7\x82\xAA" "c6\x97\xB3\x82\xCC\x96\xDA\x82\xCC\x8C\xCE" "c7\x82\xC5", /* c6オーバーデルc7のc4バケモンc7がc6竜の目の湖c7で */
        "\x82\xC8\x82\xAD\x82\xB5\x82\xBD\x83J\x83M\x81" "Bc4\x83" "C\x83K\x83\x82\x83\x93" "c7\x82\xAA\x8F" "E\x82\xC1\x82\xC4\x82\xA2\x82\xBD\x81" "B", /* なくしたカギ。c4イガモンc7が拾っていた。 */
        "", /*  */
    },
    {
        "c5\x8C\xCE\x82\xCC\x82\xCA\x82\xB5\x82\xCC\x93J", /* c5湖のぬしの笛 */
        "c6\x83t\x83\x8A\x81[\x83Y\x83\x89\x83\x93\x83hc7\x82\xC5" "c4\x83\x82\x83W\x83\x83\x83\x82\x83\x93" "c7\x82\xAA\x82\xAD\x82\xEA\x82\xBD", /* c6フリーズランドc7でc4モジャモンc7がくれた */
        "\x95s\x8Ev\x8B" "c\x82\xC8\x93J\x81" "B", /* 不思議な笛。 */
        "", /*  */
    },
    {
        "\x83G\x83\x93\x83W\x83" "F\x83\x82\x83\x93\x83X\x83^\x83`\x83\x85\x81[", /* エンジェモンスタチュー */
        "\x82\xA2\x82\xA2\x82\xA2\x82\xA2\x82\xA2", /* いいいいい */
        "\x82\xA2\x82\xA2\x82\xA2\x82\xA2\x82\xA2", /* いいいいい */
        "\x82\xA2\x82\xA2\x82\xA2\x82\xA2\x82\xA2", /* いいいいい */
    },
    {
        "c5\x83I\x83" "C\x83\x8B\x83^\x83\x93\x83N", /* c5オイルタンク */
        "c6\x83h\x83\x8A\x83\x8B\x83g\x83\x93\x83l\x83\x8B" "c7\x82\xC5" "c4\x83^\x83\x93\x83N\x83\x82\x83\x93" "c7\x82\xAA\x90Z\x82\xA9\x82\xC1\x82\xC4", /* c6ドリルトンネルc7でc4タンクモンc7が浸かって */
        "\x82\xA2\x82\xBD\x83I\x83" "C\x83\x8B\x81" "B", /* いたオイル。 */
        "", /*  */
    },
    {
        "c5\x83g\x83" "C\x83\x8C\x83" "b\x83g\x83y\x81[\x83p\x81[", /* c5トイレットペーパー */
        "c6\x83t\x83@\x83N\x83g\x83\x8A\x83" "A\x83\x8B\x83^\x83" "E\x83\x93" "c7\x82\xC5" "c4\x83G\x83" "e\x83\x82\x83\x93" "c7\x82\xC9\x8D\xEC\x82\xC1\x82\xC4", /* c6ファクトリアルタウンc7でc4エテモンc7に作って */
        "\x82\xE0\x82\xE7\x82\xC1\x82\xBD\x83g\x83" "C\x83\x8C\x83" "b\x83g\x83y\x81[\x83p\x81[\x82P\x82O\x82O\x8C\xC2\x81" "B", /* もらったトイレットペーパー１００個。 */
        "", /*  */
    },
    {
        "c5\x82\xE2\x82\xC1\x82\xBD\x82\xE9\x82\xC5\x81[\x8B\xA6\x89\xEF\x89\xEF\x88\xF5\x8F\xD8", /* c5やったるでー協会会員証 */
        "c6\x83~\x83n\x83\x89\x83V\x8ERc7\x82\xC5" "c4\x83S\x83u\x83\x8A\x83\x82\x83\x93" "c7\x82\xAA\x97\x8E\x82\xC6\x82\xB5\x82\xC4\x82\xA2\x82\xC1\x82\xBD", /* c6ミハラシ山c7でc4ゴブリモンc7が落としていった */
        "\x82\xE2\x82\xC1\x82\xBD\x82\xE9\x82\xC5\x81[\x8B\xA6\x89\xEF\x82\xCC\x96\xBC\x97_\x89\xEF\x88\xF5\x8F\xD8\x81" "B", /* やったるでー協会の名誉会員証。 */
        "", /*  */
    },
    {
        "c5\x83y\x81[\x83p\x81[\x83" "c\x83\x8A\x81[", /* c5ペーパーツリー */
        "c6\x83~\x83X\x83" "e\x83" "B\x83" "c\x83\x8A\x81[\x83Yc7\x82\xC5" "c4\x83R\x83J\x83g\x83\x8A\x83\x82\x83\x93" "c7\x82\xC9\x82\xE0\x82\xE7\x82\xC1\x82\xBD", /* c6ミスティツリーズc7でc4コカトリモンc7にもらった */
        "\x83g\x83" "C\x83\x8C\x83" "b\x83g\x83y\x81[\x83p\x81[\x82\xCC\x8C\xB4\x97\xBF\x81" "B", /* トイレットペーパーの原料。 */
        "", /*  */
    },
    {
        "c5\x83Q\x83R\x83\x82\x83\x93\x8E\xCA\x90^", /* c5ゲコモン写真 */
        "c6\x83Q\x83" "b\x83R\x81[\x8E\xBC\x92nc7\x82\xC5" "c4\x83Q\x83R\x83\x82\x83\x93" "c7\x82\xA9\x82\xE7\x82\xE0\x82\xE7\x82\xC1\x82\xBD", /* c6ゲッコー湿地c7でc4ゲコモンc7からもらった */
        "c4\x83g\x83m\x83T\x83}\x83Q\x83R\x83\x82\x83\x93" "c7\x82\xCC\x8E\xCA\x90^", /* c4トノサマゲコモンc7の写真 */
        "", /*  */
    },
    {
        "\x83y\x81[\x83p\x81[\x83" "c\x83\x8A\x81[", /* ペーパーツリー */
        "\x83~\x83X\x83" "e\x83" "B\x83" "c\x83\x8A\x81[\x83Y\x82\xC5\x83R\x83J\x83g\x83\x8A\x83\x82\x83\x93\x82\xC9\x82\xE0\x82\xE7\x82\xC1\x82\xBD", /* ミスティツリーズでコカトリモンにもらった */
        "\x83g\x83" "C\x83\x8C\x83" "b\x83g\x83y\x81[\x83p\x81[\x82\xCC\x8C\xB4\x97\xBF\x81" "B", /* トイレットペーパーの原料。 */
        "", /*  */
    },
    {
        "c5\x83O\x83\x8C\x81[\x83g\x83G\x83\x93\x83W\x83" "F\x83X\x83^\x83`\x83\x85\x81[\x81i\x95X\x91\x9C\x81j", /* c5グレートエンジェスタチュー（氷像） */
        "c6\x83t\x83\x8A\x81[\x83Y\x83\x89\x83\x93\x83hc7\x82\xCC" "c4\x83G\x83\x93\x83W\x83" "F\x83\x82\x83\x93" "c7\x82\xAA\x91\xE5\x8E\x96\x82\xC9\x82\xB5\x82\xC4", /* c6フリーズランドc7のc4エンジェモンc7が大事にして */
        "\x82\xA2\x82\xBD\x95X\x91\x9C\x81" "Bc4\x83I\x81[\x83K\x83\x82\x83\x93" "c7\x82\xAA\x89" "B\x82\xB5\x8E\x9D\x82\xC1\x82\xC4\x82\xA2\x82\xBD\x81" "B", /* いた氷像。c4オーガモンc7が隠し持っていた。 */
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

/* the ids of the events that give each item */
const s16 SAI_KEY_ITEM_EVENTS[14] = { 0x22, 0x85, 0x27, 0x36, 0x3D, 0xBE, -1, 0x4E, 0x87, 0x74, 0xB1, 0x99, -1, 0xD5 };

/* its second loop compares the item's index anew, where our C reuses the
   index it read the item with */
INCLUDE_ASM("saiseg/nonmatchings/player/sai_player_data_jp", SAI_findKeyItems);

void SAI_runKeyItems(void) {
    u8 buf[0x250];
    void (*fn)(void);

    SAI_KEY_ITEMS.running = 1;
    openKanjiPage(0xF, 0x1E3);
    clearKanjiPage(0xF);
    func_8002B508(buf, 0x3C, 0x32, 0, 0);
    SAI_findKeyItems();
    SAI_initKeyItemList();
    SAI_KEY_ITEMS.cursor = SAI_KEY_ITEMS.columns;
    SAI_KEY_ITEMS.icon = func_80044334(1, 0x20, 0x1C, 5, 1);
    spawnTask(0, -1, 0, 0x800, D_8002A3E0, &SAI_KEY_ITEM_LIST_WINDOW_DEF, getCurrentTaskId());
    SAI_KEY_ITEMS.listWindow = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, D_8002A3E0, &SAI_KEY_ITEM_TEXT_WINDOW_DEF, getCurrentTaskId());
    SAI_KEY_ITEMS.textWindow = (JpWindow *)waitFrames(0x7FFFFFFF);
    waitFrames(60);
    do {
        waitFrames(FRAME_INTERVAL);
        fn = SAI_KEY_ITEM_STATES[SAI_KEY_ITEMS.state];
        if (fn != NULL) {
            fn();
        }
    } while (SAI_KEY_ITEMS.running != 0);
    *(s16 *)((u8 *)D_801E4640 + 0x1BE) = 2;
    SAI_KEY_ITEMS.listWindow->state = 4;
    SAI_KEY_ITEMS.textWindow->state = 4;
    func_8002CACC(0);
    waitFrames(60);
    func_80044758(SAI_KEY_ITEMS.icon);
    closeKanjiPage(0xF);
    spawnTask(0, -1, 0, 0x800, func_801F35CC, 1, 0, 0, 0);
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
        spawnTask(0, -1, 0, 0x800, D_8002A3E0, &SAI_PLAYER_DATA_WINDOW_DEF, getCurrentTaskId());
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
        spawnTask(0, -1, 0, 0x800, D_8002A3E0, &SAI_KEY_ITEM_LIST_WINDOW_DEF, getCurrentTaskId());
        SAI_KEY_ITEMS.listWindow = (JpWindow *)waitFrames(0x7FFFFFFF);
        spawnTask(0, -1, 0, 0x800, D_8002A3E0, &SAI_KEY_ITEM_TEXT_WINDOW_DEF, getCurrentTaskId());
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
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"\x96\xBC\x91O"); /* 名前 */
    drawIconText(0x91, y, 7, 1, win->z, (s32)PLAYER_DATA(0).name);
    y = top + 0x4B;
    /* 所持金　　　　　　　　　d0 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"\x8F\x8A\x8E\x9D\x8B\xE0\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x81@d0");
    sprintf(money, "w-1%6d", PLAYER_DATA(0).bits);
    drawText(0x91, y, (s32)money, 7, win->z);
    y = top + 0x5B;
    /* バトル称号 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"\x83o\x83g\x83\x8B\x8F\xCC\x8D\x86");
    drawIconText(0x91, y, 7, 1, win->z, (s32)D_8007E5FC[PLAYER_DATA(0).tamerRank]);
    y = top + 0x6B;
    /* コレクト称号 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"\x83R\x83\x8C\x83N\x83g\x8F\xCC\x8D\x86");
    drawIconText(0x91, y, 7, 1, win->z, (s32)D_8007E61C[PLAYER_DATA(0).collectorRank]);
    y = top + 0x7B;
    /* ２Ｐ対戦称号 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"\x82Q\x82o\x91\xCE\x90\xED\x8F\xCC\x8D\x86");
    drawIconText(0x91, y, 7, 1, win->z, (s32)D_8007E63C[PLAYER_DATA(0).battleRank]);
    y = top + 0x8B;
    /* ＣＯＭ対戦成績 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"\x82" "b\x82n\x82l\x91\xCE\x90\xED\x90\xAC\x90\xD1");
    sprintf(count, "w-1%3d", PLAYER_DATA(0).battleWins);
    drawText(0x98, y, (s32)count, 7, win->z);
    drawIconText(0xAF, y, 7, 1, win->z, (s32)"\x8F\x9F"); /* 勝 */
    sprintf(count, "w-1%3d", PLAYER_DATA(0).battleLosses);
    drawText(0xC3, y, (s32)count, 7, win->z);
    drawIconText(0xDB, y, 7, 1, win->z, (s32)"\x94s"); /* 敗 */
    y = top + 0x9B;
    /* ２Ｐ対戦成績 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"\x82Q\x82o\x91\xCE\x90\xED\x90\xAC\x90\xD1");
    sprintf(count, "w-1%3d", PLAYER_DATA(0).versusWins);
    drawText(0x98, y, (s32)count, 7, win->z);
    drawIconText(0xAF, y, 7, 1, win->z, (s32)"\x8F\x9F");
    sprintf(count, "w-1%3d", PLAYER_DATA(0).versusLosses);
    drawText(0xC3, y, (s32)count, 7, win->z);
    drawIconText(0xDB, y, 7, 1, win->z, (s32)"\x94s");
    y = top + 0xAB;
    /* ＳＡＶＥ回数 */
    drawIconText(0x2D, y, 6, 1, win->z, (s32)"\x82r\x82`\x82u\x82" "d\x89\xF1\x90\x94");
    sprintf(count, "w-1%3d", PLAYER_DATA(0).saveCount);
    drawText(0x98, y, (s32)count, 7, win->z);
    drawIconText(0xAF, y, 7, 1, win->z, (s32)"\x89\xF1"); /* 回 */
    /* 攻撃使用率 */
    drawIconText(0x2D, top + 0xBB, 6, 1, win->z, (s32)"\x8DU\x8C\x82\x8Eg\x97p\x97\xA6");
    y = top + 0xCB;
    total = PLAYER_DATA(0).attackCounts[2] + (PLAYER_DATA(0).attackCounts[0] + PLAYER_DATA(0).attackCounts[1]);
    for (i = 0; i < 3; i++) {
        if (total != 0) {
            rate = PLAYER_DATA(0).attackCounts[i] * 1000 / total;
        } else {
            rate = 0;
        }
        /* b%d　　．w7％ */
        sprintf(line, "b%d\x81@\x81@\x81" "Dw7\x81\x93", i);
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
        obtainPartner(SAI_KEY_ITEMS.icon);
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
