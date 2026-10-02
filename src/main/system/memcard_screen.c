#include "common.h"
#include "game.h"
#include "dcb/card_render.h"
#include "dcb/fade.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/overlay_calls.h"
#include "dcb/prim_desc.h"
#include "dcb/sound_play.h"
#include "dcb/task.h"
#include "dcb/text.h"
#include "dcb/vram_upload.h"

/* jp's own: the memory card screen, which saves and loads the players'
   profiles (us has it in OPENSEG's open_memcard and open_save). The title
   screen, the world map, the deck editor, the versus mode and the quit to the
   title open it, each in its mode (MemcardScreen.mode) */

/* jp's callees that no header declares yet */
u8 *formatSjisNumber(s32 value, s32 width, u8 *dst);
u8 *formatSjisNumberZeros(s32 value, s32 width, u8 *dst);
void func_8005714C(); /* libsnd: the mono counterpart of SsSetStereo */
/* memcard.c's, as this module declares them: it passes the save's blocks as
   an int */
extern CardDir *MEMORY_CARD_DIRECTORIES[2];
extern u8 *MEMORY_CARD_SAVE_HEADER;
extern s32 MEMORY_CARD_WAIT_COUNTER;
extern s32 MEMORY_CARD_SECTORS_DONE;
extern s32 MEMORY_CARD_SECTORS_TOTAL;
s32 ensureMemoryCardReady(s32 port);
s32 getMemoryCardStatus(s32 port);
s32 formatMemoryCard(s32 port);
s32 startMemoryCardSave(s32 port, s32 blocks, s32 data, s32 fileName, u8 *header);
s32 stepMemoryCardSave(void);
s32 startMemoryCardLoad(s32 port, s32 data, s32 fileName);
s32 stepMemoryCardLoad(void);
s32 readMemoryCardSavePreview(s32 port, void *dst, s32 fileName);
void scanMemoryCardFiles(s32 port);
s32 verifySaveChecksum(s32 len, u8 *data);
void writeSaveChecksum(s32 len, u8 *data);
/* player_data_jp's, as this module declares them: it takes runChoiceMenu's
   result as an int, and reads three fields of the scrolling background */
void runWindowTask();
void startAreaPakLoad(void);
void openChoiceMenu(ChoiceMenu *menu, s32 icon, s32 y, void (*cancel)(), s32 *arg);
void addChoiceMenuItem(ChoiceMenu *menu, s32 item, void (*action)());
s32 runChoiceMenu(ChoiceMenu *menu);
void startChoiceMenuAction(ChoiceMenu *menu);
void showScrollingBackground(void);
void loadScrollingBackground(s32 panelY, s32 image);
void setBackgroundScrollMode(s32 scrollMode);
void freeScrollingBackground(void);
typedef struct {
    /* 0x000 */ u8 unk0[0x1B8];
    /* 0x1B8 */ Chunk *pak;
    /* 0x1BC */ s16 icon; /* the panel's icon */
    /* 0x1BE */ s16 state; /* of the panel: 3 closes it */
} ScrollingBackgroundView;
extern ScrollingBackgroundView *SCROLLING_BACKGROUND;
/* the overlays' tasks the screen goes back to */
void NIS_runMainMenuTask();
void NIS_enterDeckListFromMenu();
void NIS_runVsMode();
void INT_introTask();
void enterWorldMap();

/* what runWindowTask hands back (player_data_jp's WindowTask) */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ s8 state; /* 2 shows it, 4 closes it */
} WindowTaskHead;

/* the memory card save header (McHeader): its title, then its icon */
typedef struct {
    /* 0x00 */ u8 data[0x20];
} McIconClut;
typedef struct {
    /* 0x000 */ u8 data[0x180];
} McIconImage;
typedef struct {
    /* 0x00 */ u8 magic[2]; /* "SC" */
    /* 0x02 */ u8 type;
    /* 0x03 */ u8 blocks;
    /* 0x04 */ u8 title[0x40];
    /* 0x44 */ u8 pad[0x1C];
    /* 0x60 */ McIconClut clut;
    /* 0x80 */ McIconImage image;
} McSaveHeader;

/* the screen's state, after its menu of choices */
typedef struct MemcardScreen {
    /* 0x000 */ ChoiceMenu menu;
    /* 0x24C */ u8 message;
    /* 0x24D */ u8 step;
    /* 0x24E */ u8 unk24E[2];
    /* 0x250 */ void (*handler)(struct MemcardScreen *screen); /* runs the step's menu */
    /* 0x254 */ s32 fileStatus[3]; /* readMemoryCardSavePreview's: 0 when the file is there */
    /* 0x260 */ u8 *buffer; /* the files' previews (0x80 each), then the profile */
    /* 0x264 */ u8 freeBlocks;
    /* 0x265 */ char fileNames[3][0x1A];
    /* 0x2B3 */ u8 mode;
    /* 0x2B4 */ u8 scrollMode; /* setBackgroundScrollMode's when the screen closes */
    /* 0x2B5 */ u8 hasOldSave; /* a Digimon World save was found */
    /* 0x2B6 */ u8 saved;
    /* 0x2B7 */ u8 missingPlayers; /* 1, 2 or 3: whose card is missing */
} MemcardScreen;

/* the card port and the file the screen works on */
typedef struct {
    /* 0x0 */ u8 port;
    /* 0x1 */ u8 file;
} MemcardSlot;

/* what a mode does on its menus: its icon, the cancel's task, and the task
   that leaves the screen */
typedef struct {
    /* 0x0 */ s32 icon;
    /* 0x4 */ void (*cancel)();
    /* 0x8 */ void (*next)();
} MemcardMode;

extern WindowTaskHead *MEMCARD_WINDOWS[2];
extern MemcardSlot MEMCARD_SLOT;
extern MemcardScreen MEMCARD_SCREEN;
extern s32 MEMCARD_PROGRESS; /* the save's or the load's progress, in sectors */
extern u8 NAME_GLYPHS_UPLOADED; /* set once a name's glyphs are uploaded */
extern char NAME_GLYPHS_TEXT[0x10]; /* the name whose glyphs are uploaded */

void fillSaveHeader(u8 *header, char *title, u8 *tim, u8 blocks);
void ignoreMemcardWindowClose(void);
void drawNothingInMemcardScreen(void);
void doNothingInMemcardScreen(void);
void drawMemcardMessage(UiWindow *window);
void drawSavePreview(UiWindow *window);
void drawMemcardSlotMessage(UiWindow *window);
void drawMemcardSlotWindow(UiWindow *window);
void drawMissingCardMessage(UiWindow *window);
s32 applyLoadedSave(MemcardScreen *screen);
void prepareSaveData(MemcardScreen *screen);
void runNewLoadMenu(void);
void openMemcardScreen(void);
void runMemcardScreen(void);
void runMemcardAccess(void);
s32 readSavePreview(u8 file, MemcardScreen *screen);
s16 countFreeBlocks(void);
s16 waitMemoryCardSave(void);
s16 waitMemoryCardLoad(void);
void closeMemcardScreen(MemcardScreen *screen);
u8 checkSaveIsCurrent(u8 player, u8 bothPorts);
s16 findMissingSaves(void);
s16 saveBothProfiles(MemcardScreen *screen);
void selectMemcardSlot(MemcardScreen *screen);
void runNoCardPrompt(MemcardScreen *screen);
void confirmFormat(MemcardScreen *screen);
void confirmNewFile(MemcardScreen *screen);
void selectSaveFile(MemcardScreen *screen);
void runNoFreeBlockPrompt(MemcardScreen *screen);
void confirmOverwriteOrLoad(MemcardScreen *screen);
void runDonePrompt(MemcardScreen *screen);
void confirmPlayWithoutCard(MemcardScreen *screen);
void func_8004B5D8(MemcardScreen *screen);
void runNoSavePrompt(MemcardScreen *screen);
void confirmSave(MemcardScreen *screen);
void runMissingCardPrompt(MemcardScreen *screen);
void runFailurePrompt(MemcardScreen *screen);
void func_8004B830(MemcardScreen *screen);
void confirmPlayer2Load(MemcardScreen *screen);
void uploadPlayer1NameGlyphs(void);
void uploadPlayer2NameGlyphs(void);
u8 hasDigimonWorldSave(void);

/* the message window and the window of the card slot */
WindowSpec MEMCARD_MESSAGE_WINDOW = { { 0x17, 0x98, 0, 0xC }, { 0x17, 0x98, 0x112, 0x43 }, 10, 0, drawNothingInMemcardScreen, ignoreMemcardWindowClose };
WindowSpec MEMCARD_SLOT_WINDOW = { { 0x128, 0x7C, 0, 0xE }, { 0x88, 0x7C, 0xA0, 0xE }, 10, 1, drawMemcardSlotWindow, ignoreMemcardWindowClose };

/* the areas, where the player is (PlayerProfile.area) */
char *AREA_NAMES[15] = {
    "はじまりの街",
    "迷わずの森",
    "ドリルトンネル",
    "ミハラシ山",
    "ピートランド",
    "ギアサバンナ",
    "ファクトリアルタウン",
    "ゲッコー湿地",
    "ミスティツリーズ",
    "フリーズランド",
    "グレートキャニオン",
    "オーバーデル",
    "ダイノ古代境",
    "トロピカジャングル",
    "ムゲンマウンテン",
};

/* the screen's messages (MemcardScreen.message) */
char *MEMCARD_MESSAGES[47] = {
    "",
    "h2このままゲームを始めるとデータのセーブを\nすることができません。よろしいですか？",
    "h2メモリーカードをチェック中……\nメモリーカードおよびコントローラを\n抜き差ししないでください！",
    "h2このメモリーカードは\n初期化されていません。\n初期化しますか？",
    "h2メモリーカード初期化中……\nメモリーカードおよびコントローラを\n抜き差ししないでください！",
    "h2メモリーカードの初期化が完了しました。",
    "h2正常に初期化できませんでした。",
    "h2メモリーカードがありません！\nメモリーカードをセットしてください。",
    "h2新規ファイル",
    "h2空きブロックが足りません！\nデータ保存には１ブロック必要です。",
    "h2「デジタルカードバトル」のデータを\n作成します。よろしいですか？",
    "h2ファイル１のデータに上書きされます！\n新規データを作成しますか？",
    "h2ファイル２のデータに上書きされます！\n新規データを作成しますか？",
    "h2ファイル３のデータに上書きされます！\n新規データを作成しますか？",
    "h2データ作成中……\nメモリーカードおよびコントローラを\n抜き差ししないでください！",
    "h2正常にデータ作成できませんでした。",
    "h2ゲームを始めます。",
    "h2このメモリーカードは\n初期化されていません。",
    "h2セーブデータがありません！",
    "h2ファイル１のデータをロードしますか？",
    "h2ファイル２のデータをロードしますか？",
    "h2ファイル３のデータをロードしますか？",
    "h2データロード中……\nメモリーカードおよびコントローラを\n抜き差ししないでください！",
    "h2正常にデータロードできませんでした。",
    "h2データロードが完了しました。\nゲームを始めます。",
    "h2現在のデータに上書きされます！\nよろしいですか？",
    "h2ファイル２のデータに上書きされます！\nよろしいですか？",
    "h2ファイル３のデータに上書きされます！\nよろしいですか？",
    "h2データセーブ中……\nメモリーカードおよびコントローラを\n抜き差ししないでください！",
    "h2正常にデータセーブできませんでした。",
    "h2データセーブが完了しました。\nゲームを続けますか？",
    "h2メモリーカードスロットに\nメモリーカードが\n差し込まれているのを確認してから\n選択して下さい。",
    "h2続いてプレイヤー２のデータを読み込みます。",
    "h2メモリーカードスロットに\n現在のゲームデータが入った\nメモリーカードが差さっていません！",
    "",
    "",
    "h2プレイヤー１と同一ファイルです。\nこのファイルをロードすると、対戦終了後に\nその結果をセーブできなくなりますが\nよろしいですか？",
    "h2プレイヤー１とプレイヤー２が\n同一ファイルなので、\n上書きセーブできません。",
    "h2対戦を終了します。",
    "h2ゲームを終了します。",
    "h2メモリーカードを使わずにゲームを始めたので\nセーブはできません。\nゲームを続けますか？",
    "h2ゲームを続けますか？",
    "h2データロードが完了しました。\n編集を始めます。",
    "h2編集を終了します。",
    "h2データロードが完了しました。\n対戦を始めます。",
    "h2データをセーブしますか？",
    "",
};

MemcardMode MEMCARD_MODES[10] = {
    { 2, NULL, enterWorldMap },
    { 3, runNewLoadMenu, enterWorldMap },
    { 0, enterWorldMap, enterWorldMap },
    { 1, NIS_runMainMenuTask, NIS_enterDeckListFromMenu },
    { 0, NIS_enterDeckListFromMenu, NIS_runMainMenuTask },
    { 0x37, NIS_runMainMenuTask, doNothingInMemcardScreen },
    { 0x38, NULL, NIS_runVsMode },
    { 0, NIS_runVsMode, NIS_runMainMenuTask },
    { 0, NULL, NIS_runMainMenuTask },
    { 0, NULL, enterWorldMap },
};

/* the frame of the progress bars */
SpriteDesc MEMCARD_PROGRESS_FRAME = { 0, 0x80, 0x80, 0x80, 0x64, 0, 0xD0, 0x7CBF, 0, 0x1F, 0x50, 0x64, 0xA0, 0x10 };

/* Makes the save header with the D:\MC_A.TIM icon */
void buildGameSaveHeader(void) {
    u8 *tim;

    spawnTask(0, -1, 0, 0x800, loadFile, "D:\\MC_A.TIM", getCurrentTaskId());
    tim = (u8 *)waitFrames(0x7FFFFFFF);
    fillSaveHeader(MEMORY_CARD_SAVE_HEADER, "デジモンワールドカードバトル", tim, 1);
    freeHeapBlock(tim);
}

/* Makes the save header of player's profile: its title says the file, the
   play time and the area, its icon is taken from VRAM (one of three, as far
   as the player got) */
void buildSaveHeader(u8 player) {
    McSaveHeader *header = (McSaveHeader *)MEMORY_CARD_SAVE_HEADER;
    McIconImage image;
    McIconClut clut;
    s16 iconX[3] = { 0x374, 0x378, 0x37C };
    Rect16 imageRect;
    Rect16 clutRect;
    char title[0x48];
    u8 file[0x10];
    u8 hours[0x10];
    u8 minutes[0x10];
    PlayerProfile *profile;
    s32 h;
    s32 m;
    u8 icon;
    s32 i;

    profile = &PLAYER_DATA(player);
    if (profile->unk28_12) {
        icon = 2;
    } else {
        icon = PLAYER_DATA(player).tradeUnlocked;
    }
    imageRect.x = iconX[icon];
    imageRect.y = 0x100;
    imageRect.w = 4;
    imageRect.h = 0x30;
    StoreImage(&imageRect, &image);
    clutRect.x = 0x370;
    clutRect.y = icon + 0x1FD;
    clutRect.w = 0x10;
    clutRect.h = 1;
    StoreImage(&clutRect, &clut);
    DrawSync(0);
    bzero(title, 0x42);
    h = profile->playTime / 216000;
    m = (profile->playTime - h * 216000) / 3600;
    if (h >= 1000) {
        h = 999;
        m = 59;
    }
    formatSjisNumber(((SessionData *)SESSION_DATA)->saves[player].file + 1, 1, file);
    formatSjisNumber(h, 3, hours);
    formatSjisNumberZeros(m, 2, minutes);
    sprintf(title, "デジモンカード［%s］　%s：%s　%s", file, hours, minutes, AREA_NAMES[profile->area]);
    header->magic[0] = 'S';
    header->magic[1] = 'C';
    header->type = 0x13;
    header->blocks = 1;
    for (i = 0; i < 0x40; i++) {
        header->title[i] = 0;
    }
    strcpy(header->title, title);
    for (i = 0; i < 0x1C; i++) {
        header->pad[i] = 0;
    }
    header->clut = clut;
    header->image = image;
}

/* Uploads the TIMs of a file */
void uploadTimFile(char *path) {
    u32 *tims;

    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTimList(tims);
    freeHeapBlock(tims);
}

void ignoreMemcardWindowClose(void) {
}

void drawNothingInMemcardScreen(void) {
}

void doNothingInMemcardScreen(void) {
}

/* draws the message */
void drawMemcardMessage(UiWindow *window) {
    char text[0xA0];
    s16 x;
    s16 y;

    x = MEMCARD_MESSAGE_WINDOW.to.x + 15;
    y = MEMCARD_MESSAGE_WINDOW.to.y + 5;
    sprintf(text, MEMCARD_MESSAGES[MEMCARD_SCREEN.message]);
    drawIconText(x, y, 7, 0, window->z, (s32)text);
}

/* draws a file's preview: the player, the wins, the play time and the area,
   or "new file" */
void drawSavePreview(UiWindow *window) {
    char lines[4][0x2E];
    char trade[0x20];
    u8 wins[0x10];
    u8 losses[0x10];
    u8 hours[0x10];
    u8 minutes[0x10];
    MemcardScreen *screen;
    PlayerProfile *preview;
    s16 x;
    s16 y;
    s32 h;
    s32 m;
    u16 i;
    u8 count;

    x = MEMCARD_MESSAGE_WINDOW.to.x + 15;
    y = MEMCARD_MESSAGE_WINDOW.to.y + 5;
    screen = &MEMCARD_SCREEN;
    if (screen->mode != 0) {
        count = 0;
        for (i = 0; i < 3; i++) {
            if (screen->fileStatus[i] == 0) {
                count++;
                if (screen->menu.choice + 1 == count) {
                    break;
                }
            }
        }
        preview = (PlayerProfile *)(screen->buffer + i * 0x80);
    } else {
        if (screen->fileStatus[screen->menu.choice] == 1) {
            drawIconText(x, y, 7, 0, window->z, (s32) "h2新規ファイル");
            return;
        }
        preview = (PlayerProfile *)(screen->buffer + screen->menu.choice * 0x80);
    }
    h = preview->playTime / 216000;
    m = (preview->playTime - h * 216000) / 3600;
    if (h >= 1000) {
        h = 999;
        m = 59;
    }
    formatSjisNumber(preview->battleWins, 3, wins);
    formatSjisNumber(preview->battleLosses, 3, losses);
    formatSjisNumber(h, 3, hours);
    formatSjisNumber(m, 2, minutes);
    bzero(trade, 0x10);
    if (preview->tradeUnlocked == 1) {
        sprintf(trade, "トレード可");
    }
    sprintf(lines[0], "プレイヤー：%s", preview->name);
    sprintf(lines[1], "戦績　　　：%s勝%s敗", wins, losses);
    sprintf(lines[2], "タイム　　：%s時間%s分", hours, minutes);
    sprintf(lines[3], "現在位置　：%s", AREA_NAMES[preview->area]);
    drawIconText(x, y, 7, 0, window->z, (s32)lines[0]);
    drawIconText(x + 0xB4, y, 7, 0, window->z, (s32)trade);
    y += 14;
    drawIconText(x, y, 7, 0, window->z, (s32)lines[1]);
    y += 14;
    drawIconText(x, y, 7, 0, window->z, (s32)lines[2]);
    y += 14;
    drawIconText(x, y, 7, 0, window->z, (s32)lines[3]);
}

/* draws the message with the card slot (both players' in versus mode) */
void drawMemcardSlotMessage(UiWindow *window) {
    char text[0xB8];
    u8 slot1[0x10];
    u8 slot2[0x10];
    s16 x;
    s16 y;
    MemcardScreen *screen;
    SessionData *session;

    x = MEMCARD_MESSAGE_WINDOW.to.x + 15;
    y = MEMCARD_MESSAGE_WINDOW.to.y + 5;
    screen = &MEMCARD_SCREEN;
    session = (SessionData *)SESSION_DATA;
    if (screen->mode == 7) {
        formatSjisNumber(session->saves[0].port + 1, 1, slot1);
        formatSjisNumber(session->saves[1].port + 1, 1, slot2);
        sprintf(text, "h2１Ｐ：メモリーカードスロット%sと\n２Ｐ：メモリーカードスロット%sの\n%s", slot1, slot2,
                MEMCARD_MESSAGES[screen->message]);
        drawIconText(x, y, 7, 0, window->z, (s32)text);
    } else {
        formatSjisNumber(session->saves[0].port + 1, 1, slot1);
        sprintf(text, "h2メモリーカードスロット%sの\n%s", slot1, MEMCARD_MESSAGES[MEMCARD_SCREEN.message]);
        drawIconText(x, y, 7, 0, window->z, (s32)text);
    }
}

/* draws the card slot's window */
void drawMemcardSlotWindow(UiWindow *window) {
    char text[0x30];
    u8 slot[0x10];
    s16 x;
    s16 y;

    x = MEMCARD_SLOT_WINDOW.to.x + 4;
    y = MEMCARD_SLOT_WINDOW.to.y + 2;
    formatSjisNumber(MEMCARD_SLOT.port + 1, 1, slot);
    sprintf(text, "メモリーカードスロット%s", slot);
    drawIconText(x, y, 7, 0, window->z, (s32)text);
}

/* draws whose card with the current data is missing */
void drawMissingCardMessage(UiWindow *window) {
    char text[0xB8];
    s16 x = MEMCARD_MESSAGE_WINDOW.to.x + 15;
    s16 y = MEMCARD_MESSAGE_WINDOW.to.y + 5;
    u8 *missing = &MEMCARD_SCREEN.missingPlayers;
    char *players[3] = { "１Ｐ", "２Ｐ", "１Ｐと２Ｐ" };

    sprintf(text, "h2メモリーカードスロットに\n%sの現在のゲームデータが入った\nメモリーカードが差さっていません！",
            players[*missing - 1]);
    drawIconText(x, y, 7, 0, window->z, (s32)text);
}

/* Fills a save header: its title and a TIM's icon */
void fillSaveHeader(u8 *header, char *title, u8 *tim, u8 blocks) {
    McSaveHeader *h;
    McIconClut *clut;
    McIconImage *image;
    s32 i;

    h = (McSaveHeader *)header;
    clut = (McIconClut *)(tim + 0x14);
    image = (McIconImage *)(tim + 0x40);
    h->magic[0] = 'S';
    h->magic[1] = 'C';
    h->type = 0x13;
    h->blocks = blocks;
    for (i = 0; i < 0x40; i++) {
        h->title[i] = 0;
    }
    strcpy(h->title, title);
    for (i = 0; i < 0x1C; i++) {
        h->pad[i] = 0;
    }
    h->clut = *clut;
    h->image = *image;
}

/* the file names of the three files */
void getSaveFileName(s32 file, char *name) {
    switch (file) {
    case 1:
        strcpy(name, "BISLPS-02506_A");
        break;
    case 2:
        strcpy(name, "BISLPS-02506_B");
        break;
    case 3:
        strcpy(name, "BISLPS-02506_C");
        break;
    }
}

/* Puts the profile to save in the buffer */
void prepareSaveData(MemcardScreen *screen) {
    PlayerProfile *profiles;
    PlayerProfile *buffer;
    SessionData *session;

    profiles = (PlayerProfile *)PLAYER_PROFILES;
    buffer = (PlayerProfile *)screen->buffer;
    session = (SessionData *)SESSION_DATA;
    if (screen->mode == 0) {
        profiles[0].unk28_13 = screen->hasOldSave;
        session->saves[0].port = MEMCARD_SLOT.port;
        session->saves[0].file = MEMCARD_SLOT.file;
        session->saves[0].playTime = profiles[0].playTime;
        *buffer = profiles[0];
    } else if (screen->mode == 1) {
        *buffer = profiles[0];
    } else if (screen->mode == 2) {
        if (profiles[0].saveCount < 255) {
            profiles[0].saveCount++;
        }
        screen->saved = 1;
        session->saves[0].port = MEMCARD_SLOT.port;
        session->saves[0].file = MEMCARD_SLOT.file;
        session->saves[0].playTime = profiles[0].playTime;
        *buffer = profiles[0];
    } else if (screen->mode == 3) {
        *buffer = profiles[0];
    } else if (screen->mode == 4) {
        session->saves[0].port = MEMCARD_SLOT.port;
        session->saves[0].file = MEMCARD_SLOT.file;
        session->saves[0].playTime = profiles[0].playTime;
        *buffer = profiles[0];
    } else if (screen->mode == 5) {
        *buffer = profiles[0];
    } else if (screen->mode == 6) {
        *buffer = profiles[0];
    } else if (screen->mode == 7) {
        session->saves[0].playTime = profiles[0].playTime;
        *buffer = profiles[0];
    } else if (screen->mode == 8) {
        session->saves[1].playTime = profiles[1].playTime;
        *buffer = profiles[1];
    } else if (screen->mode == 9) {
        screen->saved = 1;
        session->saves[0].port = MEMCARD_SLOT.port;
        session->saves[0].file = MEMCARD_SLOT.file;
        session->saves[0].playTime = profiles[0].playTime;
        *buffer = profiles[0];
    }
}

/* Takes the loaded profile */
s32 applyLoadedSave(MemcardScreen *screen) {
    PlayerProfile *buffer;
    SessionData *session;

    buffer = (PlayerProfile *)screen->buffer;
    session = (SessionData *)SESSION_DATA;
    if (screen->mode == 0) {
        uploadTimList(findPakChunk(SCROLLING_BACKGROUND->pak, 5, 6));
        screen->scrollMode = 1;
    } else if (screen->mode == 1) {
        PLAYER_DATA(0) = *buffer;
        session->saves[0].port = MEMCARD_SLOT.port;
        session->saves[0].file = MEMCARD_SLOT.file;
        session->saves[0].playTime = buffer->playTime;
        uploadTimList(findPakChunk(SCROLLING_BACKGROUND->pak, 5, 6));
        if (!PLAYER_DATA(0).unk28_9) {
            ((SessionData *)SESSION_DATA)->areaSession->area = PLAYER_DATA(0).area;
            startAreaPakLoad();
            screen->scrollMode = 1;
        }
    } else if (screen->mode == 2) {
    } else if (screen->mode == 3) {
        PLAYER_DATA(0) = *buffer;
        session->saves[0].port = MEMCARD_SLOT.port;
        session->saves[0].file = MEMCARD_SLOT.file;
        session->saves[0].playTime = buffer->playTime;
    } else if (screen->mode == 4) {
    } else if (screen->mode == 5) {
        PLAYER_DATA(0) = *buffer;
        session->saves[0].port = MEMCARD_SLOT.port;
        session->saves[0].file = MEMCARD_SLOT.file;
        session->saves[0].playTime = buffer->playTime;
    } else if (screen->mode == 6) {
        PLAYER_DATA(1) = *buffer;
        session->saves[1].port = MEMCARD_SLOT.port;
        session->saves[1].file = MEMCARD_SLOT.file;
        session->saves[1].playTime = buffer->playTime;
        session->versusWins[1] = 0;
        session->versusWins[0] = 0;
        NAME_GLYPHS_UPLOADED = 0;
        spawnTask(0, -1, 0, 0x800, uploadPlayer1NameGlyphs, 0, 0, 0, 0);
        while (NAME_GLYPHS_UPLOADED == 0) {
            waitFrames(FRAME_INTERVAL);
        }
        NAME_GLYPHS_UPLOADED = 0;
        spawnTask(0, -1, 0, 0x800, uploadPlayer2NameGlyphs, 0, 0, 0, 0);
        while (NAME_GLYPHS_UPLOADED == 0) {
            waitFrames(FRAME_INTERVAL);
        }
    } else if (screen->mode == 7) {
    } else if (screen->mode == 8) {
    } else if (screen->mode == 9) {
        uploadTimList(findPakChunk(SCROLLING_BACKGROUND->pak, 5, 6));
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
    }
}

/* the title screen's debug menu (L1 at the title): the memory card screen or
   INTSEG */
void runNewLoadMenu(void) {
    ChoiceMenu menu;

    openChoiceMenu(&menu, 0x36, 0x32, NIS_runMainMenuTask, 0);
    addChoiceMenuItem(&menu, 5, doNothingInMemcardScreen);
    addChoiceMenuItem(&menu, 4, doNothingInMemcardScreen);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        if (menu.result < 0) {
            startChoiceMenuAction(&menu);
        }
        switch (menu.choice) {
        case 0:
            MEMCARD_SCREEN.step = 3;
            MEMCARD_SCREEN.mode = 1;
            openMemcardScreen();
            break;
        case 1:
            SCROLLING_BACKGROUND->state = 3;
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 0x10, 0);
            waitFrames(8);
            waitFrames(2);
            spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\intseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
            waitFrames(0x7FFFFFFF);
            waitFrames(2);
            spawnTask(0, -1, 0, 0x1000, INT_introTask, 0);
            exitTask();
            break;
        }
    }
}

/* the screen at the start of the game (mode 0) */
void openMemcardScreenForNewGame(void) {
    MEMCARD_SCREEN.step = 3;
    MEMCARD_SCREEN.mode = 0;
    loadScrollingBackground(0xE, 2);
    showScrollingBackground();
    setScreenFadeParams(1, 2, 0x10);
    openMemcardScreen();
}

/* saving from the world map (mode 2) */
void openMemcardScreenForWorldMap(void) {
    MEMCARD_SCREEN.saved = 0;
    if (((SessionData *)SESSION_DATA)->noMemoryCard != 1) {
        MEMCARD_SCREEN.step = 0x2A;
    } else {
        MEMCARD_SCREEN.step = 0x16;
    }
    MEMCARD_SCREEN.mode = 2;
    openMemcardScreen();
}

void openMemcardScreenToEditDecks(void) {
    MEMCARD_SCREEN.step = 3;
    MEMCARD_SCREEN.mode = 3;
    openMemcardScreen();
}

/* from the deck editor (mode 4) */
void openMemcardScreenAfterDeckEdit(void) {
    MEMCARD_SCREEN.step = 0x2A;
    MEMCARD_SCREEN.mode = 4;
    openMemcardScreen();
}

void openMemcardScreenForVersus(void) {
    MEMCARD_SCREEN.step = 3;
    MEMCARD_SCREEN.mode = 5;
    openMemcardScreen();
}

/* after a versus duel (mode 7) */
void openMemcardScreenAfterVersus(void) {
    MEMCARD_SCREEN.step = 0x2A;
    MEMCARD_SCREEN.mode = 7;
    openMemcardScreen();
}

/* before going back to the title (mode 9) */
void openMemcardScreenBeforeTitle(void) {
    MEMCARD_SCREEN.saved = 0;
    if (((SessionData *)SESSION_DATA)->noMemoryCard != 1) {
        MEMCARD_SCREEN.step = 0x2A;
    } else {
        MEMCARD_SCREEN.step = 0x16;
    }
    MEMCARD_SCREEN.mode = 9;
    openMemcardScreen();
}

/* Opens the screen: its two windows and its two tasks */
void openMemcardScreen(void) {
    openKanjiPage(0xF, 0x1B9);
    MEMCARD_SCREEN.buffer = allocHeapBlock(0x2000, 0x25A);
    if (DB(0).primSlots[16] == 0) {
        allocPrimDescPackets(0x40);
    }
    playMusic(0, 4, 0x7F);
    spawnTask(0, -1, 0, 0x800, runWindowTask, &MEMCARD_MESSAGE_WINDOW, getCurrentTaskId(), 0, 0);
    MEMCARD_WINDOWS[0] = (WindowTaskHead *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, runWindowTask, &MEMCARD_SLOT_WINDOW, getCurrentTaskId(), 0, 0);
    MEMCARD_WINDOWS[1] = (WindowTaskHead *)waitFrames(0x7FFFFFFF);
    MEMCARD_WINDOWS[1]->state = 2;
    spawnTask(0, -1, 0, 0x1000, runMemcardScreen, 0, 0, 0, 0);
    spawnTask(0, -1, 4, 0x1000, runMemcardAccess, 0, 0, 0, 0);
    exitTask();
}

/* the screen's task: each step opens its menu and its message, and the step's
   handler runs the menu; the progress bars of the saves and loads */
void runMemcardScreen(void) {
    MemcardScreen *screen = &MEMCARD_SCREEN;
    /* the progress bars (gouraud quads at 108, 107, 6 high; x1 is the
       length): [0] for the loads, [1] for the saves */
    PrimDesc bars[2] = {
        { 0, 0, 0, 0xFF, 0x38, 0, 0, 0, 0, 0, 0, 108, 107, 0, 6, 0, 0, 0, 0, 0xFF0000, 0xFF7070, 0xFF7070, 0, 0, 0, 0 },
        { 0, 0xFF, 0, 0, 0x38, 0, 0, 0, 0, 0, 0, 108, 107, 0, 6, 0, 0, 0, 0, 0xFF, 0x7070FF, 0x7070FF, 0, 0, 0, 0 },
    };
    PlayerProfile *profile; /* a file's profile, or the loaded profile */

    screen->hasOldSave = 0;
    while (1) {
        waitFrames(FRAME_INTERVAL);
        switch (screen->step) {
        case 0:
        case 49:
            break;
        case 14:
            SCROLLING_BACKGROUND->icon = 0;
            MEMCARD_SCREEN.message = 2;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            screen->step = 15;
            break;
        case 3:
            MEMCARD_WINDOWS[1]->state = 2;
            openChoiceMenu(&screen->menu, MEMCARD_MODES[screen->mode].icon, 0x32, MEMCARD_MODES[screen->mode].cancel, 0);
            addChoiceMenuItem(&screen->menu, 6, doNothingInMemcardScreen);
            addChoiceMenuItem(&screen->menu, 7, doNothingInMemcardScreen);
            if (screen->mode == 0) {
                addChoiceMenuItem(&screen->menu, 0x3D, doNothingInMemcardScreen);
            }
            MEMCARD_SCREEN.message = 0x1F;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            MEMCARD_SCREEN.handler = selectMemcardSlot;
            screen->step = 4;
            break;
        case 1:
            openChoiceMenu(&screen->menu, 2, 0x32, doNothingInMemcardScreen, 0);
            addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
            addChoiceMenuItem(&screen->menu, 0xC, doNothingInMemcardScreen);
            MEMCARD_SCREEN.message = 1;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            MEMCARD_SCREEN.handler = confirmPlayWithoutCard;
            screen->step = 2;
            break;
        case 7:
            if (screen->mode == 0) {
                openChoiceMenu(&screen->menu, 0xB, 0x32, doNothingInMemcardScreen, 0);
            } else if (screen->mode == 1 || screen->mode == 3 || screen->mode == 5 || screen->mode == 6) {
                openChoiceMenu(&screen->menu, 0xB, 0x32, doNothingInMemcardScreen, 0);
            } else if (screen->mode == 2 || screen->mode == 4 || screen->mode == 7 ||
                       screen->mode == 9) {
                openChoiceMenu(&screen->menu, 0xB, 0x32, doNothingInMemcardScreen, 0);
            }
            addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
            MEMCARD_SCREEN.message = 7;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            MEMCARD_SCREEN.handler = runNoCardPrompt;
            screen->step = 8;
            break;
        case 9:
            openChoiceMenu(&screen->menu, 0xB, 0x32, doNothingInMemcardScreen, 0);
            addChoiceMenuItem(&screen->menu, 0xC, doNothingInMemcardScreen);
            addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
            MEMCARD_SCREEN.message = 3;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            MEMCARD_SCREEN.handler = confirmFormat;
            screen->step = 10;
            break;
        case 11:
            MEMCARD_SCREEN.message = 4;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            screen->step = 12;
            break;
        case 16:
            openChoiceMenu(&screen->menu, 5, 0x32, doNothingInMemcardScreen, 0);
            addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
            MEMCARD_SCREEN.message = 9;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            MEMCARD_SCREEN.handler = runNoFreeBlockPrompt;
            screen->step = 17;
            break;
        case 20:
            openChoiceMenu(&screen->menu, 5, 0x32, doNothingInMemcardScreen, 0);
            addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
            addChoiceMenuItem(&screen->menu, 0xC, doNothingInMemcardScreen);
            MEMCARD_SCREEN.message = 0xA;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            MEMCARD_SCREEN.handler = confirmNewFile;
            screen->step = 21;
            break;
        case 18:
            SCROLLING_BACKGROUND->icon = 0;
            MEMCARD_SCREEN.message = 0xE;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            screen->step = 19;
            break;
        case 5:
            openChoiceMenu(&screen->menu, 4, 0x32, doNothingInMemcardScreen, 0);
            if (screen->mode == 0) {
                addChoiceMenuItem(&screen->menu, screen->fileStatus[0] == 0 ? 8 : 0x3E, doNothingInMemcardScreen);
                addChoiceMenuItem(&screen->menu, screen->fileStatus[1] == 0 ? 9 : 0x3E, doNothingInMemcardScreen);
                addChoiceMenuItem(&screen->menu, screen->fileStatus[2] == 0 ? 0xA : 0x3E, doNothingInMemcardScreen);
            } else if (screen->mode == 1 || screen->mode == 3 || screen->mode == 5 || screen->mode == 6) {
                if (screen->fileStatus[0] == 0) {
                    addChoiceMenuItem(&screen->menu, 8, doNothingInMemcardScreen);
                }
                if (screen->fileStatus[1] == 0) {
                    addChoiceMenuItem(&screen->menu, 9, doNothingInMemcardScreen);
                }
                if (screen->fileStatus[2] == 0) {
                    addChoiceMenuItem(&screen->menu, 0xA, doNothingInMemcardScreen);
                }
                /* no file to load */
                if (screen->fileStatus[0] == 1 && screen->fileStatus[1] == screen->fileStatus[0] &&
                    screen->fileStatus[2] == screen->fileStatus[1]) {
                    openChoiceMenu(&screen->menu, 0xB, 0x32, doNothingInMemcardScreen, 0);
                    addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
                    MEMCARD_SCREEN.message = 0x12;
                    MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
                    MEMCARD_SCREEN.handler = runNoSavePrompt;
                    screen->step = 0x20;
                    break;
                }
            }
            MEMCARD_MESSAGE_WINDOW.draw = drawSavePreview;
            MEMCARD_SCREEN.handler = selectSaveFile;
            screen->step = 6;
            break;
        case 24:
            if (screen->mode == 0) {
                openChoiceMenu(&screen->menu, 5, 0x32, doNothingInMemcardScreen, 0);
            } else if (screen->mode == 1 || screen->mode == 3) {
                openChoiceMenu(&screen->menu, 1, 0x32, doNothingInMemcardScreen, 0);
            } else if (screen->mode == 5) {
                openChoiceMenu(&screen->menu, 0x37, 0x32, doNothingInMemcardScreen, 0);
            } else if (screen->mode == 6) {
                openChoiceMenu(&screen->menu, 0x38, 0x32, doNothingInMemcardScreen, 0);
            }
            addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
            addChoiceMenuItem(&screen->menu, 0xC, doNothingInMemcardScreen);
            if (screen->mode == 0) {
                MEMCARD_SCREEN.message = MEMCARD_SLOT.file + 0xB;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            } else if (screen->mode == 1 || screen->mode == 3) {
                /* one block per mode, as the icons above: the match depends on
                   it, since the loop then has enough uses of MEMCARD_SLOT for
                   loop.c to hoist its address */
                MEMCARD_SCREEN.message = MEMCARD_SLOT.file + 0x13;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            } else if (screen->mode == 5) {
                MEMCARD_SCREEN.message = MEMCARD_SLOT.file + 0x13;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            } else if (screen->mode == 6) {
                /* the second player's file is the first one's */
                profile = (PlayerProfile *)(screen->buffer + MEMCARD_SLOT.file * 0x80);
                if (PLAYER_DATA(0).profileId == profile->profileId) {
                    MEMCARD_SCREEN.message = 0x24;
                    MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
                } else {
                    MEMCARD_SCREEN.message = MEMCARD_SLOT.file + 0x13;
                    MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
                }
            }
            MEMCARD_SCREEN.handler = confirmOverwriteOrLoad;
            screen->step = 25;
            break;
        case 48:
            openChoiceMenu(&screen->menu, 0, 0x32, doNothingInMemcardScreen, 0);
            addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
            addChoiceMenuItem(&screen->menu, 0xC, doNothingInMemcardScreen);
            MEMCARD_SCREEN.message = 0x19;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardSlotMessage;
            MEMCARD_SCREEN.handler = confirmPlayer2Load;
            screen->step = 49;
            break;
        case 26:
            MEMCARD_PROGRESS = 0;
            SCROLLING_BACKGROUND->state = 1;
            if (screen->mode == 2) {
                MEMCARD_SCREEN.message = 0x1C;
            } else if (screen->mode == 4) {
                MEMCARD_SCREEN.message = 0x1C;
            } else if (screen->mode == 7) {
                MEMCARD_SCREEN.message = 0x1C;
            } else if (screen->mode == 8) {
                MEMCARD_SCREEN.message = 0x1C;
            } else if (screen->mode == 9) {
                MEMCARD_SCREEN.message = 0x1C;
            } else {
                MEMCARD_SCREEN.message = 0xE;
            }
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            if (screen->mode == 7) {
                MEMCARD_WINDOWS[1]->state = 1;
                screen->step = 28;
            } else {
                screen->step = 19;
            }
            break;
        case 27:
            bars[1].x1 = (MEMCARD_PROGRESS << 7) / 100;
            drawPrimDesc(&bars[1]);
            drawPrimDesc((PrimDesc *)&MEMCARD_PROGRESS_FRAME);
            break;
        case 28:
            bars[1].x1 = (MEMCARD_PROGRESS << 7) / 200;
            if (screen->mode == 8 && MEMCARD_PROGRESS < 100) {
                bars[1].x1 += 0x40;
            }
            drawPrimDesc(&bars[1]);
            drawPrimDesc((PrimDesc *)&MEMCARD_PROGRESS_FRAME);
            break;
        case 22:
            openChoiceMenu(&screen->menu, 6, 0x32, NULL, 0);
            addChoiceMenuItem(&screen->menu, 0xB, MEMCARD_MODES[screen->mode].next);
            if (screen->mode == 2 || screen->mode == 9) {
                addChoiceMenuItem(&screen->menu, 0xC, NIS_runTitleScreen);
            }
            if (screen->mode == 0) {
                ((SessionData *)SESSION_DATA)->areaSession->area = ((PlayerProfile *)screen->buffer)->area;
                startAreaPakLoad();
                MEMCARD_SCREEN.message = 0x10;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
                screen->scrollMode = 1;
            } else if (screen->mode == 1) {
                MEMCARD_SCREEN.message = 0x18;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
                screen->scrollMode = 0;
            } else if (screen->mode == 2 || screen->mode == 9) {
                if (screen->saved != 0) {
                    MEMCARD_SCREEN.message = 0x1E;
                } else if (((SessionData *)SESSION_DATA)->noMemoryCard == 1) {
                    MEMCARD_SCREEN.message = 0x28;
                } else {
                    MEMCARD_SCREEN.message = 0x29;
                }
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
                screen->scrollMode = 0;
            } else if (screen->mode == 3) {
                MEMCARD_SCREEN.message = 0x2A;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
                screen->scrollMode = 2;
            } else if (screen->mode == 4) {
                MEMCARD_SCREEN.message = 0x2B;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
                screen->scrollMode = 2;
            } else if (screen->mode == 5) {
                MEMCARD_SCREEN.message = 0x20;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            } else if (screen->mode == 6) {
                MEMCARD_SCREEN.message = 0x2C;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
                screen->scrollMode = 4;
            } else if (screen->mode == 7) {
                MEMCARD_SCREEN.message = 0x10;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
                screen->scrollMode = 2;
            } else if (screen->mode == 8) {
                MEMCARD_SCREEN.message = 0x26;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
                screen->scrollMode = 2;
            }
            profile = (PlayerProfile *)screen->buffer;
            if (!profile->monoSound) {
                SsSetStereo();
            } else {
                func_8005714C();
            }
            MEMCARD_SCREEN.handler = runDonePrompt;
            screen->step = 23;
            break;
        case 33:
            MEMCARD_PROGRESS = 0;
            SCROLLING_BACKGROUND->icon = 0;
            MEMCARD_SCREEN.message = 0x16;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            screen->step = 34;
            break;
        case 34:
            bars[0].x1 = (MEMCARD_PROGRESS << 7) / 100;
            drawPrimDesc(&bars[0]);
            drawPrimDesc((PrimDesc *)&MEMCARD_PROGRESS_FRAME);
            break;
        case 29:
            openChoiceMenu(&screen->menu, 0xB, 0x32, doNothingInMemcardScreen, 0);
            addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
            MEMCARD_SCREEN.message = 0x11;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            MEMCARD_SCREEN.handler = func_8004B5D8;
            screen->step = 30;
            break;
        case 42:
            openChoiceMenu(&screen->menu, MEMCARD_MODES[screen->mode].icon, 0x32, MEMCARD_MODES[screen->mode].cancel, 0);
            if (screen->mode == 2) {
                MEMCARD_SLOT.port = ((SessionData *)SESSION_DATA)->saves[0].port;
                MEMCARD_WINDOWS[1]->state = 1;
                addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
                addChoiceMenuItem(&screen->menu, 0xC, doNothingInMemcardScreen);
                screen->scrollMode = 0;
                MEMCARD_SCREEN.message = 0x2D;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            } else if (screen->mode == 4) {
                MEMCARD_SLOT.port = ((SessionData *)SESSION_DATA)->saves[0].port;
                MEMCARD_WINDOWS[1]->state = 1;
                addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
                addChoiceMenuItem(&screen->menu, 0xC, NIS_runMainMenuTask);
                screen->scrollMode = 2;
                MEMCARD_SCREEN.message = 0x2D;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            } else if (screen->mode == 7) {
                screen->scrollMode = 2;
                if (PLAYER_DATA(0).profileId == PLAYER_DATA(1).profileId) {
                    addChoiceMenuItem(&screen->menu, 0xB, NIS_runMainMenuTask);
                    MEMCARD_SCREEN.message = 0x25;
                } else {
                    MEMCARD_WINDOWS[1]->state = 2;
                    addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
                    addChoiceMenuItem(&screen->menu, 0xC, NIS_runMainMenuTask);
                    MEMCARD_SCREEN.message = 0x2D;
                }
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            } else if (screen->mode == 9) {
                MEMCARD_SLOT.port = ((SessionData *)SESSION_DATA)->saves[0].port;
                MEMCARD_WINDOWS[1]->state = 1;
                addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
                addChoiceMenuItem(&screen->menu, 0xC, doNothingInMemcardScreen);
                screen->scrollMode = 2;
                MEMCARD_SCREEN.message = 0x2D;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            }
            MEMCARD_SCREEN.handler = confirmSave;
            screen->step = 43;
            break;
        case 44:
            openChoiceMenu(&screen->menu, 0, 0x32, NULL, 0);
            MEMCARD_SCREEN.message = 2;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            screen->step = 45;
            break;
        case 15:
        case 45:
            MEMORY_CARD_WAIT_COUNTER++;
            break;
        case 46:
            openChoiceMenu(&screen->menu, 0xB, 0x32, doNothingInMemcardScreen, 0);
            addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
            if (screen->mode == 7 || screen->mode == 8) {
                MEMCARD_MESSAGE_WINDOW.draw = drawMissingCardMessage;
            } else {
                MEMCARD_SCREEN.message = 0x21;
                MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            }
            MEMCARD_SCREEN.handler = runMissingCardPrompt;
            screen->step = 47;
            break;
        case 36:
            openChoiceMenu(&screen->menu, 0xB, 0x32, NULL, 0);
            addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
            if (screen->mode == 0) {
                MEMCARD_SCREEN.message = 0xF;
            } else {
                MEMCARD_SCREEN.message = 0x1D;
            }
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            MEMCARD_SCREEN.handler = runFailurePrompt;
            screen->step = 37;
            break;
        case 38:
        case 40:
            openChoiceMenu(&screen->menu, 0xB, 0x32, doNothingInMemcardScreen, 0);
            addChoiceMenuItem(&screen->menu, 0xB, doNothingInMemcardScreen);
            MEMCARD_SCREEN.message = 0x17;
            MEMCARD_MESSAGE_WINDOW.draw = drawMemcardMessage;
            MEMCARD_SCREEN.handler = func_8004B830;
            screen->step = 39;
            break;
        case 13:
            exitTask();
            break;
        }
        if (screen->handler != NULL) {
            screen->handler(screen);
        }
    }
}

/* the screen's other task: the memory card's work for the steps */
/* while a menu is up: back to step 14 to check the card again when it was
   taken out or changed. A statement macro; the match depends on its
   do-while, whose loop notes weigh the step 14's uses (it gets s4 before
   SESSION_DATA's address) */
#define CHECK_CARD()                                              \
    do {                                                          \
        if (ensureMemoryCardReady(MEMCARD_SLOT.port) == 1) {      \
            screen->step = 14;                                    \
        }                                                         \
    } while (0)

void runMemcardAccess(void) {
    MemcardScreen *screen;
    s32 status;
    s32 result;
    s32 blocks; /* a save takes one block */

    screen = &MEMCARD_SCREEN;
    blocks = 1;
    getSaveFileName(1, screen->fileNames[0]);
    getSaveFileName(2, screen->fileNames[1]);
    getSaveFileName(3, screen->fileNames[2]);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        switch (screen->step) {
        case 0:
            break;
        case 15:
            status = getMemoryCardStatus(MEMCARD_SLOT.port);
            if (status == 1) {
                screen->step = 7;
            } else if (status == 2) {
                if (screen->mode == 0) {
                    screen->step = 9;
                } else if (screen->mode == 1 || screen->mode == 3 || screen->mode == 5 || screen->mode == 6) {
                    screen->step = 0x1D;
                }
            } else {
                scanMemoryCardFiles(MEMCARD_SLOT.port);
                readSavePreview(1, screen);
                readSavePreview(2, screen);
                readSavePreview(3, screen);
                if (screen->mode == 0) {
                    screen->hasOldSave = hasDigimonWorldSave();
                } else {
                    screen->hasOldSave = 0;
                }
                screen->freeBlocks = countFreeBlocks();
                screen->step = 5;
            }
            break;
        case 12:
            formatMemoryCard(MEMCARD_SLOT.port);
            screen->step = 14;
            break;
        case 6: /* the file list */
            CHECK_CARD();
            break;
        case 10: /* format the card? */
            CHECK_CARD();
            break;
        case 21: /* make a new file? */
            CHECK_CARD();
            break;
        case 25: /* overwrite or load this file? */
            CHECK_CARD();
            break;
        case 19:
            MEMCARD_PROGRESS = 0;
            prepareSaveData(screen);
            screen->step = 27;
        case 27:
            buildSaveHeader(0);
            writeSaveChecksum(0x145C, screen->buffer);
            if (startMemoryCardSave(MEMCARD_SLOT.port, blocks, (s32)screen->buffer,
                                    (s32)screen->fileNames[MEMCARD_SLOT.file],
                                    MEMORY_CARD_SAVE_HEADER) == -1 ||
                (result = waitMemoryCardSave()) == -1) {
                screen->step = 0x24;
            } else {
                screen->step = 0x16;
            }
            break;
        case 28:
            result = saveBothProfiles(screen);
            if (result == 1) {
                screen->step = 0x24;
            } else if (result == 2) {
                screen->step = 0x24;
            } else if (result == 0) {
                screen->step = 0x16;
            }
            break;
        case 49:
            if (ensureMemoryCardReady(((SessionData *)SESSION_DATA)->saves[0].port) == 1) {
                MEMCARD_SLOT.port = ((SessionData *)SESSION_DATA)->saves[0].port;
                MEMCARD_WINDOWS[1]->state = 1;
                screen->step = 7;
            } else if (screen->mode == 7 &&
                       ensureMemoryCardReady(((SessionData *)SESSION_DATA)->saves[1].port) == 1) {
                MEMCARD_SLOT.port = ((SessionData *)SESSION_DATA)->saves[1].port;
                MEMCARD_WINDOWS[1]->state = 1;
                screen->step = 7;
            }
            break;
        case 34:
            if (startMemoryCardLoad(MEMCARD_SLOT.port, (s32)screen->buffer,
                                    (s32)screen->fileNames[MEMCARD_SLOT.file]) == -1 ||
                (result = waitMemoryCardLoad()) == -1) {
                screen->step = 0x26;
            } else if (verifySaveChecksum(0x145C, screen->buffer) == 1) {
                screen->step = 0x28;
            } else {
                screen->step = 0x16;
            }
            break;
        case 45:
            if (screen->mode == 7) {
                result = findMissingSaves();
                screen->missingPlayers = result;
                if (result == 1) {
                    screen->step = 0x2E;
                } else if (result == 2) {
                    screen->step = 0x2E;
                } else if (result == 3) {
                    screen->step = 0x2E;
                } else if (result == 0) {
                    screen->step = 0x30;
                }
            } else {
                status = checkSaveIsCurrent(0, 1);
                if (status == 0) {
                    screen->step = 0x2E;
                } else if (status == 1) {
                    screen->step = 0x30;
                    ((SessionData *)SESSION_DATA)->saves[0].port = MEMCARD_SLOT.port;
                }
            }
            break;
        case 13:
            exitTask();
            break;
        }
    }
}

/* Reads file's preview (1 to 3), trying five times: 0 when it is there */
s32 readSavePreview(u8 file, MemcardScreen *screen) {
    s16 tries;
    u8 i;

    i = file - 1;
    screen->fileStatus[i] = 1;
    for (tries = 0; tries < 5; tries++) {
        if ((screen->fileStatus[i] = readMemoryCardSavePreview(MEMCARD_SLOT.port, screen->buffer + i * 0x80,
                                                               (s32)screen->fileNames[i])) == 0) {
            return 0;
        }
    }
    bzero(screen->buffer + i * 0x80, 0x80);
    return 1;
}

/* the card's free blocks */
s16 countFreeBlocks(void) {
    return 15 - MEMORY_CARD_DIRECTORIES[MEMCARD_SLOT.port]->blocks;
}

/* Writes the save: 0 when it is done, -1 when it failed */
s16 waitMemoryCardSave(void) {
    do {
        if ((MEMCARD_PROGRESS = stepMemoryCardSave()) == -1) {
            return -1;
        }
    } while (MEMORY_CARD_SECTORS_DONE != MEMORY_CARD_SECTORS_TOTAL);
    return 0;
}

/* Reads the save: 0 when it is done, -1 when it failed */
s16 waitMemoryCardLoad(void) {
    do {
        waitFrames(FRAME_INTERVAL);
        if ((MEMCARD_PROGRESS = stepMemoryCardLoad()) == -1) {
            return -1;
        }
    } while (MEMORY_CARD_SECTORS_DONE != MEMORY_CARD_SECTORS_TOTAL);
    return 0;
}

/* Closes the screen and goes on to the menu's task */
void closeMemcardScreen(MemcardScreen *screen) {
    MEMCARD_WINDOWS[0]->state = 4;
    MEMCARD_WINDOWS[1]->state = 4;
    waitFrames(30);
    closeKanjiPage(0xF);
    freeHeapBlocksByTag(0x25A);
    screen->handler = NULL;
    screen->step = 13;
    setBackgroundScrollMode(screen->scrollMode);
    startChoiceMenuAction(&screen->menu);
}

/* Finds player's save where it was loaded from (on either card when
   bothPorts): 1 when it is still there, the same profile at the same play
   time */
u8 checkSaveIsCurrent(u8 player, u8 bothPorts) {
    MemcardScreen *screen;
    PlayerProfile *profiles;
    SessionData *session;
    PlayerProfile *preview;
    s32 playTime;
    s16 i;
    u8 found;

    screen = &MEMCARD_SCREEN;
    profiles = (PlayerProfile *)PLAYER_PROFILES;
    session = (SessionData *)SESSION_DATA;
    found = 0;
    MEMCARD_SLOT.port = session->saves[player].port;
    MEMCARD_SLOT.file = session->saves[player].file;
    playTime = session->saves[player].playTime;
    preview = (PlayerProfile *)(screen->buffer + MEMCARD_SLOT.file * 0x80);
    if (bothPorts) {
        i = 0;
        do {
            if (getMemoryCardStatus(MEMCARD_SLOT.port) == 0) {
                scanMemoryCardFiles(MEMCARD_SLOT.port);
                readSavePreview(MEMCARD_SLOT.file + 1, screen);
                if (screen->fileStatus[MEMCARD_SLOT.file] == 0 && profiles[player].profileId == preview->profileId &&
                    playTime == preview->playTime) {
                    found = 1;
                    break;
                }
            }
            i++;
            MEMCARD_SLOT.port ^= 1;
        } while (i < 2);
    } else {
        i = 0;
        do {
            if (getMemoryCardStatus(MEMCARD_SLOT.port) == 0) {
                scanMemoryCardFiles(MEMCARD_SLOT.port);
                readSavePreview(MEMCARD_SLOT.file + 1, screen);
                if (screen->fileStatus[MEMCARD_SLOT.file] == 0 && profiles[player].profileId == preview->profileId &&
                    playTime == preview->playTime) {
                    /* fake match: the empty loop keeps this found = 1 inside the
                       loop's notes, which gives found the weighted uses that win it s5
                       in global-alloc, as in the original */
                    do {
                        found = 1;
                    } while (0);
                    break;
                }
            }
            i++;
        } while (i < 1);
    }
    return found;
}

/* Finds both players' saves: which are missing (1: 1P, 2: 2P, 3: both) */
s16 findMissingSaves(void) {
    s16 missing;
    s16 i;

    missing = 0;
    for (i = 0; i < 2; i++) {
        if (checkSaveIsCurrent(i, 1) == 0) {
            missing += i + 1;
        } else {
            ((SessionData *)SESSION_DATA)->saves[i].port = MEMCARD_SLOT.port;
            ((SessionData *)SESSION_DATA)->saves[i].file = MEMCARD_SLOT.file;
        }
    }
    return missing;
}

/* Saves both players' profiles after a versus duel: 0 when done, else the
   player whose save failed (1 or 2) */
s16 saveBothProfiles(MemcardScreen *screen) {
    s16 i;
    s32 blocks; /* a save takes one block */
    SessionData *session;
    s32 save;

    blocks = 1;
    for (i = 0; i < 2; i++) {
        /* fake match: the save's offset is built in `save` in two steps, and
           `save` is reused below for the save's address. That makes it a
           global pseudo, so global-alloc gives it v1 after local-alloc has
           put the SESSION_DATA base in v0, as in the original */
        MEMCARD_SLOT.port = (session = (SessionData *)SESSION_DATA, save = i << 16, save >>= 13,
                             ((SessionData *)((u8 *)session + save))->saves[0].port);
        if (checkSaveIsCurrent(i, 0) == 0) {
            return i + 1;
        }
        prepareSaveData(screen);
        buildSaveHeader(i);
        writeSaveChecksum(0x145C, screen->buffer);
        save = (s32)SESSION_DATA;
        save += i * sizeof(SaveLocation);
        if (startMemoryCardSave(((SessionData *)save)->saves[0].port, blocks, (s32)screen->buffer,
                                (s32)screen->fileNames[((SessionData *)save)->saves[0].file],
                                MEMORY_CARD_SAVE_HEADER) == -1) {
            return i + 1;
        }
        if (waitMemoryCardSave() == -1) {
            return i + 1;
        }
        screen->mode = 8;
    }
    return 0;
}

/* step 4: the first menu (slot 1, slot 2, or no card at the start) */
void selectMemcardSlot(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0) {
            screen->scrollMode = 2;
            closeMemcardScreen(screen);
        }
        switch (screen->menu.choice) {
        case 0:
            MEMCARD_WINDOWS[1]->state = 1;
            MEMCARD_SLOT.port = 0;
            screen->step = 14;
            ((SessionData *)SESSION_DATA)->noMemoryCard = 0;
            break;
        case 1:
            MEMCARD_WINDOWS[1]->state = 1;
            MEMCARD_SLOT.port = 1;
            screen->step = 14;
            ((SessionData *)SESSION_DATA)->noMemoryCard = 0;
            break;
        case 2:
            MEMCARD_SLOT.port = 0;
            MEMCARD_SLOT.file = 0;
            screen->step = 1;
            ((SessionData *)SESSION_DATA)->noMemoryCard = 1;
            break;
        }
        screen->handler = NULL;
    }
}

/* step 8: no card */
void runNoCardPrompt(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0) {
            if (screen->mode == 2 || screen->mode == 4 || screen->mode == 7 || screen->mode == 9) {
                screen->step = 0x2A;
            } else {
                screen->step = 3;
            }
        } else if (screen->menu.choice == 0) {
            if (screen->mode == 2 || screen->mode == 4 || screen->mode == 7 || screen->mode == 9) {
                screen->step = 0x2A;
            } else {
                screen->step = 3;
            }
        }
        screen->handler = NULL;
    }
}

/* step 10: format the card? */
void confirmFormat(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0) {
            screen->step = 3;
        } else {
            switch (screen->menu.choice) {
            case 0:
                screen->step = 3;
                break;
            case 1:
                screen->step = 11;
                break;
            }
        }
        screen->handler = NULL;
    }
}

/* step 21: make a new file? */
void confirmNewFile(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0) {
            screen->step = 5;
        } else {
            switch (screen->menu.choice) {
            case 0:
                screen->step = 18;
                break;
            case 1:
                screen->step = 5;
                break;
            }
        }
        screen->handler = NULL;
    }
}

/* step 6: the file list */
void selectSaveFile(MemcardScreen *screen) {
    s16 file;
    u8 count;

    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0) {
            screen->step = 3;
        } else {
            switch (screen->menu.choice) {
            case 0:
            case 1:
            case 2:
                if (screen->mode != 0) {
                    count = 0;
                    for (file = 0; file < 3; file++) {
                        if (screen->fileStatus[file] == 0) {
                            count++;
                            if (screen->menu.choice + 1 == count) {
                                MEMCARD_SLOT.file = file;
                                break;
                            }
                        }
                    }
                    if (screen->mode == 0 || screen->mode == 1 || screen->mode == 3 || screen->mode == 5 ||
                        screen->mode == 6) {
                        screen->step = 0x18;
                    }
                } else {
                    if (screen->fileStatus[screen->menu.choice] == 0) {
                        if (screen->mode == 0 || screen->mode == 1 || screen->mode == 3 || screen->mode == 5 ||
                            screen->mode == 6) {
                            screen->step = 0x18;
                        }
                    } else if (screen->mode == 0) {
                        if (countFreeBlocks() == 0) {
                            screen->step = 0x10;
                        } else {
                            screen->step = 0x14;
                        }
                    } else if (screen->mode == 1 || screen->mode == 3 || screen->mode == 5 || screen->mode == 6) {
                        screen->step = 0x1F;
                    }
                    MEMCARD_SLOT.file = screen->menu.choice;
                }
                break;
            }
        }
        screen->handler = NULL;
    }
}

/* step 17: no free block */
void runNoFreeBlockPrompt(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0 || screen->menu.choice == 0) {
            screen->step = 3;
        }
        screen->handler = NULL;
    }
}

/* step 25: overwrite or load this file? */
void confirmOverwriteOrLoad(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0) {
            screen->step = 5;
        } else {
            switch (screen->menu.choice) {
            case 0:
                if (screen->mode == 0) {
                    screen->step = 0x1A;
                } else if (screen->mode == 1 || screen->mode == 3 || screen->mode == 5 || screen->mode == 6) {
                    screen->step = 0x21;
                }
                break;
            case 1:
                screen->step = 5;
                break;
            }
        }
        screen->handler = NULL;
    }
}

/* step 23: done; go on, or back to the title */
void runDonePrompt(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        switch (screen->menu.choice) {
        case 0:
            if (screen->mode == 5) {
                applyLoadedSave(screen);
                screen->mode = 6;
                screen->step = 3;
            } else {
                applyLoadedSave(screen);
                closeMemcardScreen(screen);
            }
            break;
        case 1:
            waitFrames(2);
            spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\nisseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
            waitFrames(0x7FFFFFFF);
            waitFrames(2);
            SCROLLING_BACKGROUND->state = 3;
            MEMCARD_WINDOWS[0]->state = 4;
            MEMCARD_WINDOWS[1]->state = 4;
            waitFrames(0x1B);
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 8, 0);
            waitFrames(0x21);
            stopScreenFade();
            freeScrollingBackground();
            stopMusic();
            closeKanjiPage(0xF);
            freeHeapBlocksByTag(0x25A);
            screen->handler = NULL;
            screen->step = 13;
            startChoiceMenuAction(&screen->menu);
            break;
        }
        screen->handler = NULL;
    }
}

/* step 2: start without a card? */
void confirmPlayWithoutCard(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0) {
            screen->step = 3;
        } else {
            switch (screen->menu.choice) {
            case 0:
                prepareSaveData(screen);
                screen->step = 0x16;
                break;
            case 1:
                screen->step = 3;
                break;
            }
        }
        screen->handler = NULL;
    }
}

/* step 30 */
void func_8004B5D8(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0 || screen->menu.choice == 0) {
            screen->step = 3;
        }
        screen->handler = NULL;
    }
}

/* step 32: no save on the card */
void runNoSavePrompt(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0 || screen->menu.choice == 0) {
            screen->step = 3;
        }
        screen->handler = NULL;
    }
}

/* step 43: save? */
void confirmSave(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0) {
            if (screen->mode == 7) {
                screen->scrollMode = 4;
            }
            closeMemcardScreen(screen);
        } else {
            switch (screen->menu.choice) {
            case 0:
                if (MEMCARD_SCREEN.message != 0x25) {
                    screen->step = 0x2C;
                } else {
                    closeMemcardScreen(screen);
                }
                break;
            case 1:
                if (screen->mode == 2 || screen->mode == 9) {
                    screen->step = 0x16;
                } else {
                    closeMemcardScreen(screen);
                }
                break;
            }
        }
        screen->handler = NULL;
    }
}

/* step 47: the card with the current data is missing */
void runMissingCardPrompt(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0 || screen->menu.choice == 0) {
            screen->step = 0x2A;
        }
        screen->handler = NULL;
    }
}

/* step 37: the save or the load failed */
void runFailurePrompt(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.choice == 0) {
            if (screen->mode == 0) {
                screen->step = 3;
            } else if (screen->mode == 2 || screen->mode == 4 || screen->mode == 7) {
                screen->step = 0x2A;
            } else if (screen->mode == 8) {
                /* back to the first player's save */
                screen->mode = 7;
                screen->step = 0x2A;
            } else if (screen->mode == 9) {
                screen->step = 0x2A;
            }
        }
        screen->handler = NULL;
    }
}

/* step 39 */
void func_8004B830(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0 || screen->menu.choice == 0) {
            screen->step = 3;
        }
        screen->handler = NULL;
    }
}

/* step 49: load player 2's data? */
void confirmPlayer2Load(MemcardScreen *screen) {
    if (runChoiceMenu(&screen->menu) != 0) {
        if (screen->menu.result < 0) {
            screen->step = 0x2A;
        } else {
            switch (screen->menu.choice) {
            case 0:
                screen->step = 0x1A;
                break;
            case 1:
                screen->step = 0x2A;
                break;
            }
        }
        screen->handler = NULL;
    }
}

/* uploads player 1's name glyphs */
void uploadPlayer1NameGlyphs(void) {
    strcpy(NAME_GLYPHS_TEXT, PLAYER_DATA(0).name);
    NAME_GLYPHS_UPLOADED = 0;
    spawnTask(0, -1, 0, 0x800, uploadStringGlyphs, NAME_GLYPHS_TEXT, 0, getCurrentTaskId(), 0);
    waitFrames(0x7FFFFFFF);
    NAME_GLYPHS_UPLOADED = 1;
}

/* uploads player 2's name glyphs */
void uploadPlayer2NameGlyphs(void) {
    strcpy(NAME_GLYPHS_TEXT, PLAYER_DATA(1).name);
    NAME_GLYPHS_UPLOADED = 0;
    spawnTask(0, -1, 0, 0x800, uploadStringGlyphs, NAME_GLYPHS_TEXT, 1, getCurrentTaskId(), 0);
    waitFrames(0x7FFFFFFF);
    NAME_GLYPHS_UPLOADED = 1;
}

/* Looks for a Digimon World save (BISLPS-01797DMR0 to E): 1 when there is one */
u8 hasDigimonWorldSave(void) {
    char name[0x20];
    u8 preview[0x88];
    s16 i;
    s16 tries;

    bzero(name, 0x20);
    for (i = 0; i < 15; i++) {
        sprintf(name, "BISLPS-01797DMR%X", i);
        for (tries = 0; tries < 3; tries++) {
            if (readMemoryCardSavePreview(MEMCARD_SLOT.port, preview, (s32)name) == 0) {
                return 1;
            }
        }
    }
    return 0;
}
