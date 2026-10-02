#include "common.h"
#include "game.h"
#include "dcb/anim_control.h"
#include "dcb/archive.h"
#include "dcb/fade.h"
#include "dcb/frame_callback.h"
#include "dcb/heap.h"
#include "dcb/intseg.h"
#include "dcb/loader.h"
#include "dcb/model_anim.h"
#include "dcb/model_load.h"
#include "dcb/pad.h"
#include "dcb/player_data.h"
#include "dcb/prim_desc.h"
#include "dcb/scene3d.h"
#include "dcb/sound_play.h"
#include "dcb/task.h"
#include "dcb/text.h"
#include "dcb/vram_upload.h"

/* jp's executable: functions and data that us doesn't have */
s8 uploadKanjiString();
void formatSjisNumber();
void formatSjisNumberZeros();
void openKanjiPage();
void closeKanjiPage();
void freeScrollingBackground();
s32 KAW_createCursor();
void KAW_initCursorShape();
void KAW_freeCursor();
void KAW_drawCursorAt();
void runWindowTask();
void openMemcardScreenForNewGame();
extern s16 CAMERA_TARGET_MODEL;
extern void *D_801E469C;
extern u8 *OPTION_CARDS;
extern u8 *DIGIVOLVE_CARDS;

#if JP_DEBUG_BUILD
/* the debug code of the executable eu's INTSEG was built against */
void setTaskName();
void func_800184F0();
void func_80018F5C();
extern s32 D_801DEBF0;
/* the debug text's lines */
extern char DEBUG_TEXT_LINES[8][0x40];
/* names the task spawned next */
#define NAME_TASK(name) setTaskName(0, name)
#else
#define NAME_TASK(name)
#endif

void INT_introTask(void);
s32 INT_initIntroScene(IntState *state);
void INT_openBabamonWindow(void);
s32 INT_runYesNoPrompt();
s32 INT_runStoneChoice();
s32 INT_runNameEntry(void *arg);
s32 INT_runYearEntry(void *arg);
s32 INT_runMonthEntry(void *arg);
s32 INT_runDayEntry(void *arg);
s32 INT_formatNameAndBirthday(IntState *state);
void INT_drawPageLines(IntWindow *win);
void INT_drawTypedLines(IntWindow *win);
s32 INT_runLineAction(IntState *state);
s32 INT_runPageAction(IntState *state);
void INT_eraseFieldChar(u8 *len, u8 row, u8 *name, u8 maxLen);
void INT_typeFieldChar(IntState *state, u8 *len, u8 row, u8 *name, u8 maxLen, u8 *text);
void INT_drawFieldCursor(u8 col, u8 field);
u16 INT_getKeyValue(IntState *state);
s16 INT_isFieldBlank(u8 *name);
void INT_drawDateKeyboard(IntWindow *win);
void INT_openNameEntry(IntState *state, u8 withDate);
void INT_openNicknameEntry(IntState *state, u8 withDate);
void INT_addStarterDeck(u8 player, u8 slot, u8 color);
u8 INT_makeHeroDeck(IntState *state, u8 *deck, u8 color);
void INT_endScene3D(void);
void INT_showHeroBoy(void);
void INT_moveChosenStone(void);
s32 INT_waitForCircle(IntState *state);
void INT_drawTypingLines(IntWindow *win);
void INT_drawKeyboard(IntWindow *win);
void INT_loadKeyboardPage(u8 page);
void INT_waitAnimationEnd(s32 slot, s32 anim, s32 loopKey);
void INT_advancePage(IntState *state);
void INT_startScene3D(void);

void INT_ignoreWindowClose(void);
void INT_drawNameFields(IntWindow *win);
void INT_drawBirthdayField(IntWindow *win);
void INT_drawSpeakerName(IntWindow *win);

/* not referenced by any code */
const char D_801EA3E8[] = "\t";

/* the keyboard: nine rows per page, hiragana, katakana, letters and digits */
u8 *INT_KEYBOARD_ROWS[27] = {
    "あいうえお　わ　を　ん",
    "かきくけこ　がぎぐげご",
    "さしすせそ　ざじずぜぞ",
    "たちつてと　だぢづでど",
    "なにぬねの　ばびぶべぼ",
    "はひふへほ　ぱぴぷぺぽ",
    "まみむめも　ぁぃぅぇぉ",
    "や　ゆ　よ　っゃゅょ～", /* や　ゆ　よ　っゃゅょ〜 */
    "らりるれろ　　　　　　",
    "アイウエオ　ワ　ヲ　ン",
    "カキクケコ　ガギグゲゴ",
    "サシスセソ　ザジズゼゾ",
    "タチツテト　ダヂヅデド",
    "ナニヌネノ　バビブベボ",
    "ハヒフヘホ　パピプペポ",
    "マミムメモ　ァィゥェォ",
    "ヤ　ユ　ヨ　ッャュョー",
    "ラリルレロ　ヴ　　　　",
    "ＡＢＣＤＥ　ａｂｃｄｅ",
    "ＦＧＨＩＪ　ｆｇｈｉｊ",
    "ＫＬＭＮＯ　ｋｌｍｎｏ",
    "ＰＱＲＳＴ　ｐｑｒｓｔ",
    "ＵＶＷＸＹ　ｕｖｗｘｙ",
    "Ｚ　　　　　ｚ　　　　",
    "　　　　　　　　　　　", /* 　　　　　　　　　　　 */
    "０１２３４　５６７８９",
    "　　　　　　　　　　　", /* 　　　　　　　　　　　 */
};

/* the blinking boxes: the prompt for the next page and the field being typed */
IntBlink INT_PROMPT_BLINK = { 0, { 0x80, 0x80, 0x80 }, 0x64, 0xBD92, 0x7F79, 0, 0x1D, 0x11C, 0x54, 0xC, 8 };
IntBlink INT_FIELD_BLINKS[3] = {
    { 1, { 0x40, 0x40, 0x40 }, 0x2A, 0, 0x7F79, 0, 0, 0, 0, 0xC, 0xD },
    { 0, { 0x80, 0x80, 0x80 }, 0x64, 0, 0x14B8, 0, 0xE, 0xD9, 0x56, 0x20, 0x52 },
    { 0, { 0x80, 0x80, 0x80 }, 0x64, 0x20, 0x14F8, 0, 0xE, 0x9C, 0x56, 0x60, 0x42 },
};
/* the windows: Babamon's lines, the name fields, the birthday, the keyboard
   and Babamon's name above the lines */
IntWindowDesc INT_TEXT_WINDOW = {
    { 0x17, 0x1E, 0, 0xC }, { 0x17, 0x1E, 0x112, 0x3F }, 10, 0, 0, INT_drawTypingLines, INT_ignoreWindowClose,
};
IntWindowDesc INT_NAME_FIELDS_WINDOW = {
    { 0x124, 0x1E, 0, 0x10 }, { 0x50, 0x1E, 0xD4, 0x10 }, 10, 0, 1, INT_drawNameFields, INT_ignoreWindowClose,
};
IntWindowDesc INT_BIRTHDAY_WINDOW = {
    { 0x124, 0x38, 0, 0x10 }, { 0x50, 0x38, 0xD4, 0x10 }, 10, 0, 1, INT_drawBirthdayField, INT_ignoreWindowClose,
};
IntWindowDesc INT_KEYBOARD_WINDOW = {
    { 0x124, 0x52, 0, 0x10 }, { 0x74, 0x52, 0xB0, 0x82 }, 10, 0, 1, INT_drawKeyboard, INT_ignoreWindowClose,
};
IntWindowDesc INT_SPEAKER_WINDOW = {
    { 0x1A, 0xD2, 0, 0xC }, { 0x1A, 0xD2, 0x48, 0xC }, 10, 0, 0, INT_drawSpeakerName, INT_ignoreWindowClose,
};
/* the pages of the intro: Babamon's lines */
IntPage INT_PAGES[32] = {
    { 3, { 0, 0, 0 }, 1, { 0 }, {
        "良く来たのう！ワシは、ババモンじゃ。",
        "まずはおぬしの［名前］と",
        "［生年月日］を入力してくれい。",
        NULL,
    } },
    { 4, { 0, 1, 0 }, 2, { 0 }, {
        "ああああああああああ",
        "９９９９年９９月９９日生まれ",
        "これでいいのかえ？",
        "　　　　　　　ＹＥＳ／ＮＯ",
    } },
    { 4, { 0, 0, 0 }, 3, { 0 }, {
        "よし、ところでおぬし。",
        "トレーディングカードゲームを知っておるか？",
        "　", /* 　 */
        "　　　　　　　ＹＥＳ／ＮＯ",
    } },
    { 1, { 0, 0, 0 }, 4, { 0 }, {
        "そうかそれなら話は早い。",
        NULL,
        NULL,
        NULL,
    } },
    { 4, { 0, 0, 0 }, 5, { 0 }, {
        "うむ、トレーディングカードゲームとは！",
        "好きなカードを集めたり、交換しては自分だけ",
        "の［デック］（カードの集まりの事じゃな）を",
        "つくり、対戦して遊ぶゲームの事じゃ。",
    } },
    { 4, { 0, 0, 0 }, 6, { 0 }, {
        "この［ゲームソフト］はな、デジモン達が",
        "デジモンカードで闘う不思議な世界なのじゃ。",
        "突っ込みはいらんぞ、とにかく不思議な不思議",
        "な世界なのじゃ。",
    } },
    { 4, { 0, 2, 0 }, 7, { 0 }, {
        "とても平和な世界じゃった……",
        "デジモン達もみな仲良く暮らしておってのう。",
        "じゃが、いまこの世界は一転して滅亡の危機に",
        "みまわれておる……",
    } },
    { 3, { 0, 0, 0 }, 8, { 0 }, {
        "くわしくは後で話すが、きょーれつな事件が",
        "起こっての。色々とワシらも手をつくしたの",
        "じゃが、未だに解決できん。",
        NULL,
    } },
    { 4, { 3, 4, 0 }, 9, { 0 }, {
        "そこでじゃ！",
        "おぬしに来てもらったのはほかでもない。",
        "選ばれたる伝説のなんたらとか……",
        "異世界から呼ばれた光のかんたらとか……",
    } },
    { 3, { 5, 6, 0 }, 10, { 0 }, {
        "ほひょほひょほひょ！ほれ、よくあるじゃろ？",
        "つまりなんじゃ。",
        "おぬしはこの世界を救ってしまうのじゃ！",
        NULL,
    } },
    { 1, { 0, 0, 0 }, 11, { 0 }, {
        "よいか、この世界ではおぬしはこの少年じゃ。",
        NULL,
        NULL,
        NULL,
    } },
    { 2, { 0, 0, 0 }, 12, { 0 }, {
        "たとえおぬしが、おなごじゃろーと、じーさん",
        "じゃろーと、この少年の身体を使ってもらう。",
        NULL,
        NULL,
    } },
    { 4, { 4, 0, 0 }, 13, { 0 }, {
        "時間が無くての、別のマトリクスが用意できん",
        "じゃった。しかしな、由緒ある身体じゃぞえ。",
        "この島の伝説の勇者でな。腕力は無いが知恵と",
        "勇気でもってこの危機を解決するのじゃ！",
    } },
    { 2, { 0, 0, 0 }, 14, { 0 }, {
        "おおそうじゃ、この世界でのおぬしの呼び名を",
        "決めてもらおうかの。",
        NULL,
        NULL,
    } },
    { 4, { 1, 0, 0 }, 15, { 0 }, {
        "ああああああああああ",
        "これでいいのかえ？",
        "　", /* 　 */
        "　　　　　　　ＹＥＳ／ＮＯ",
    } },
    { 4, { 1, 0, 0 }, 16, { 0 }, {
        "よし！これを見よ、ここに３色の石がある。",
        "これは１０００年の昔に圧縮されたと伝わる",
        "おぬし専用のデック……［勇者のデック］",
        "じゃ！",
    } },
    { 2, { 0, 0, 0 }, 0, { 0 }, {
        "全部やりたいのじゃが、今のワシの力ではこの",
        "内の１個しか解凍できん！",
        NULL,
        NULL,
    } },
    { 1, { 0, 0, 0 }, 18, { 0 }, {
        "どれか１つをえらんでくれい。",
        NULL,
        NULL,
        NULL,
    } },
    { 4, { 0, 0, 0 }, 0, { 0 }, {
        "高い攻撃力で相手をたたき圧倒する",
        "前のめりなデックじゃ！",
        "ボタンの使い分けが",
        "ポイントになるときくぞえ。",
    } },
    { 4, { 0, 0, 0 }, 20, { 0 }, {
        "これでいいのかえ？",
        "　", /* 　 */
        "　", /* 　 */
        "　　　　　　　ＹＥＳ／ＮＯ",
    } },
    { 1, { 0, 0, 0 }, 0, { 0 }, {
        "よし！赤い石じゃな。",
        NULL,
        NULL,
        NULL,
    } },
    { 1, { 0, 0, 0 }, 22, { 0 }, {
        "きえぇぇぇぇっつ！",
        NULL,
        NULL,
        NULL,
    } },
    { 4, { 0, 0, 0 }, 0, { 0 }, {
        "かたい守りでじっくりと相手と闘う",
        "もろさの無いデックじゃ！",
        "あせらずに進化させてから反撃にうつれば",
        "一気に強さを発揮すると言われておるぞえ。",
    } },
    { 4, { 0, 0, 0 }, 20, { 0 }, {
        "これでいいのかえ？",
        "　", /* 　 */
        "　", /* 　 */
        "　　　　　　　ＹＥＳ／ＮＯ",
    } },
    { 1, { 0, 0, 0 }, 0, { 0 }, {
        "よし！青い石じゃな。",
        NULL,
        NULL,
        NULL,
    } },
    { 1, { 0, 0, 0 }, 22, { 0 }, {
        "きえぇぇぇぇっつ！",
        NULL,
        NULL,
        NULL,
    } },
    { 4, { 0, 0, 0 }, 0, { 0 }, {
        "攻・防・速のバランスにすぐれる",
        "オールラウンダーデックじゃ！",
        "さまざまなパターンで相手のうらをとる事が",
        "可能ときくぞえ。",
    } },
    { 4, { 0, 0, 0 }, 20, { 0 }, {
        "これでいいのかえ？",
        "　", /* 　 */
        "　", /* 　 */
        "　　　　　　　ＹＥＳ／ＮＯ",
    } },
    { 1, { 0, 0, 0 }, 0, { 0 }, {
        "よし！緑の石じゃな。",
        NULL,
        NULL,
        NULL,
    } },
    { 1, { 0, 0, 0 }, 22, { 0 }, {
        "きえぇぇぇぇっつ！",
        NULL,
        NULL,
        NULL,
    } },
    { 4, { 0, 7, 0 }, 31, { 0 }, {
        "では、このデックはおぬしの物じゃ。",
        "……そろそろ時間じゃな。",
        "いま少しでおぬしもこちらの世界に",
        "降りられる。",
    } },
    { 3, { 0, 0, 8 }, 32, { 0 }, {
        "さらにくわしい話をするゆえ、",
        "こちらに降りしだいワシの家まで来てくれい。",
        "では、たのんだぞえ［勇者殿］！",
        NULL,
    } },
};

void INT_uploadTimFile(s32 path) {
    s32 data;

#if JP_DEBUG_BUILD
    spawnTask(0, -1, 4, 0x800, loadFile, path, getCurrentTaskId());
#else
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
#endif
    data = waitFrames(0x7FFFFFFF);
    if (data != 0) {
        uploadTimList((u32 *)data);
        freeHeapBlock((void *)data);
    }
}

void INT_ignoreWindowClose(void) {
}

void INT_func_801EB3D8(void) {
}

void INT_startIntro(void) {
#if JP_DEBUG_BUILD
    D_801DEBF0 = 0;
#endif
    NAME_TASK("INTRO");
    spawnTask(0, -1, 0, 0x800, INT_introTask, 0, 0, 0, 0);
}

void INT_introTask(void) {
#if JP_DEBUG_BUILD
    D_801DEBF0 = 0;
#endif
    INT_initIntroScene(&INT_STATE);
    stopMusic();
    while (1) {
#if JP_DEBUG_BUILD
        func_80018F5C();
#endif
        waitFrames(FRAME_INTERVAL);
        D_801E469C = &INT_PROMPT_BLINK;
        runCallbackSlots(&INT_STATE, 7, 3);
    }
}

s32 INT_initIntroScene(IntState *state) {
    Rect16 rect = { 0x380, 0, 0x40, 0x100 };
    s16 i;
    s32 pak;
    Scene3D *scene;

    ClearImage(&rect, 0, 0, 0);
    if (DB(0).primSlots[16] == 0) {
        allocPrimDescPackets(0x40);
    }
    i = 0;
    pak = loadFileTagged((s32 *)"A:\\NIS.PAK", getCurrentTaskId(), 0x7F);
    uploadTimList(findPakChunk((Chunk *)pak, 5, 200));
    resetPlayerData();
    freeScrollingBackground();
    INT_startScene3D();
    loadModel(0, 0x84, -1, pak);
    loadModel(1, 0x85, -1, pak);
    loadModel(2, 0x86, -1, pak);
    loadModel(3, 0x82, -1, pak);
    loadModel(4, 0x87, -1, pak);
    truncatePakTextures((Chunk *)pak);
    scene = SCENE_3D;
    ((Model *)scene->models[0])->pos.vx = 0x8C;
    ((Model *)scene->models[1])->pos.vx = 0x118;
    ((Model *)scene->models[2])->pos.vx = 0x1A4;
    ((Model *)scene->models[0])->pos.vy = -0xFA;
    ((Model *)scene->models[1])->pos.vy = -0xFA;
    ((Model *)scene->models[2])->pos.vy = -0xFA;
    ((Model *)scene->models[4])->pos.vx = 0x226;
    ((Model *)scene->models[4])->pos.vy = 0;
    ((Model *)scene->models[4])->rot.vy = 0x80;
    for (; i < 6; i++) {
        loadModelAnimation(0, i, i, pak);
    }
    for (i = 0; i < 6; i++) {
        loadModelAnimation(1, i, i, pak);
    }
    for (i = 0; i < 6; i++) {
        loadModelAnimation(2, i, i, pak);
    }
    for (i = 0; i < 6; i++) {
        loadModelAnimation(3, i + 10, i, pak);
    }
    for (i = 0; i < 6; i++) {
        loadModelAnimation(3, i + 20, i + 6, pak);
    }
    for (i = 0; i < 5; i++) {
        loadModelAnimation(3, i + 30, i + 12, pak);
    }
    for (i = 0; i < 3; i++) {
        loadModelAnimation(4, i + 40, i, pak);
    }
    SCENE_3D->modelState[3] = 1;
    CAMERA_TARGET_MODEL = 3;
    applyAnimationFirstFrame(3, 1);
    startModelAnimation(3, 1, -2, 0);
    openKanjiPage(0xF, 0x1B9);
    state->cursor = KAW_createCursor(1, 6, 6, 4, 1);
    state->typedChars = 0;
    state->typedLines = 0;
    state->unkBA = 0;
    state->blinkTimer = 0;
    state->typedLines = 1;
    state->typedChars = 0;
    state->page = 0;
    stopScreenFade();
    clearCallbackSlots(7, 3);
    CALLBACK_SLOTS[7] = (void (*)(void *))INT_openBabamonWindow;
}

s32 INT_func_801EB878(void) {
    return 0;
}

s32 INT_showNextPage(void *arg) {
    IntState *state = arg;
    Rect16 rect = { 0x3C0, 0, 0x40, 0x30 };
    s16 i;

    for (i = 0; i < INT_PAGES[state->page].lineCount; i++) {
        strcpy(state->lines[i].text, INT_PAGES[state->page].lines[i]);
    }
    state->lineCount = INT_PAGES[state->page].lineCount;
    state->page++;
    ClearImage(&rect, 0, 0, 0);
    for (i = 0; i < state->lineCount; i++) {
        rect.x = 0x3C0;
        rect.y = i * 12;
        rect.w = 0;
        rect.h = 0;
        state->lineLengths[i] = uploadKanjiString(&state->lines[i], &rect);
    }
    *CURRENT_CALLBACK_SLOT = NULL;
    return 0;
}

s32 INT_loadNextPage(void *arg) {
    IntState *state = arg;
    Rect16 rect = { 0x3C0, 0, 0x40, 0x30 };
    s16 i;

    for (i = 0; i < INT_PAGES[state->page].lineCount; i++) {
        strcpy(state->lines[i].text, INT_PAGES[state->page].lines[i]);
    }
    state->lineCount = INT_PAGES[state->page].lineCount;
    state->page++;
    ClearImage(&rect, 0, 0, 0);
    for (i = 0; i < state->lineCount; i++) {
        rect.x = 0x3C0;
        rect.y = i * 12;
        rect.w = 0;
        rect.h = 0;
        state->lineLengths[i] = uploadKanjiString(&state->lines[i], &rect);
    }
    return 0;
}

s32 INT_runLineAction(IntState *state) {
    switch (INT_PAGES[state->page - 1].lineActions[state->typedLines - 2]) {
    case 1:
        startModelAnimation(3, 4, -2, 0);
        break;
    case 2:
        startModelAnimation(3, 0xC, -2, 0);
        break;
    case 3:
        startModelAnimation(3, 9, -2, 0);
        INT_waitAnimationEnd(3, -1, -2);
        break;
    case 4:
        startModelAnimation(3, 3, -2, 0);
        break;
    case 5:
        INT_waitAnimationEnd(3, -1, -2);
        startModelAnimation(3, 0xB, -2, 0);
        INT_waitAnimationEnd(3, -1, -2);
        break;
    case 7:
        startModelAnimation(3, 0x10, -2, 0);
        INT_waitAnimationEnd(3, -1, -2);
        break;
    case 6:
    case 8:
        startModelAnimation(3, 8, -2, 0);
        INT_waitAnimationEnd(3, -1, -2);
        break;
    }
    INT_TEXT_WINDOW.draw = INT_drawTypingLines;
    *CURRENT_CALLBACK_SLOT = NULL;
    return 0;
}

s32 INT_runPageAction(IntState *state) {
    u8 action = INT_PAGES[state->page - 1].action;
    s16 i;

    if (action == 2 || action == 3 || action == 0xF || action == 0x14) {
        state->keyX = 0;
        state->keyY = 0;
        state->unkC0 = 0;
        *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runYesNoPrompt;
        return 0;
    } else if (action == 0x12) {
    } else if (action == 0x16) {
        startModelAnimation(3, 0xF, -2, 0);
        waitFrames(100);
        startModelAnimation(state->stone, 4, -2, 0);
        for (i = 0; i < 5; i++) {
            NAME_TASK("FADE OUT");
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 1, 0x40, 0);
            waitFrames(5);
            setScreenFadeParams(1, 1, 0x40);
            waitFrames(5);
        }
        *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_waitForCircle;
        return 0;
    }
    if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
        playSoundEffect(0);
        switch (action) {
        case 0:
            INT_advancePage(state);
            return 0;
        case 1:
            INT_openNameEntry(state, 1);
            return 0;
        case 4:
            startModelAnimation(3, 3, -2, 0);
            state->page++;
            INT_advancePage(state);
            break;
        case 6:
            startModelAnimation(3, 0xA, -2, 0);
            INT_advancePage(state);
            break;
        case 7:
            startModelAnimation(3, 8, -2, 0);
            INT_waitAnimationEnd(3, 3, -2);
            INT_advancePage(state);
            return 0;
        case 8:
            startModelAnimation(3, 9, -2, 0);
            INT_waitAnimationEnd(3, -1, -2);
            INT_advancePage(state);
            return 0;
        case 9:
            startModelAnimation(3, 0xD, -2, 0);
            INT_advancePage(state);
            break;
        case 10:
            NAME_TASK("BOY");
            spawnTask(0, -1, 0, 0x400, INT_showHeroBoy, 0, 0, 0, 0);
            startModelAnimation(3, 7, -2, 0);
            INT_waitAnimationEnd(3, -1, -2);
            INT_advancePage(state);
            return 0;
        case 11:
            startModelAnimation(3, 8, -2, 0);
            INT_waitAnimationEnd(3, -1, -2);
            INT_advancePage(state);
            return 0;
        case 12:
            startModelAnimation(3, 0xC, -2, 0);
            INT_waitAnimationEnd(3, -1, -2);
            INT_advancePage(state);
            return 0;
        case 13:
            startModelAnimation(3, 9, -2, 0);
            INT_waitAnimationEnd(3, -1, -2);
            INT_advancePage(state);
            return 0;
        case 14:
            INT_openNicknameEntry(state, 1);
            return 0;
        case 16:
            startModelAnimation(3, 2, -2, 0);
            INT_waitAnimationEnd(3, 3, -2);
            INT_advancePage(state);
            return 0;
        case 18:
            state->stone = 1;
            startModelAnimation(1, 2, -2, 0);
            INT_waitAnimationEnd(state->stone, 3, 0);
            state->page = 0x16;
            CALLBACK_SLOTS[9] = (void (*)(void *))INT_showNextPage;
            *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runStoneChoice;
            return 0;
        case 31:
            INT_addStarterDeck(0, 0, state->stone);
            ((IntProfile *)PLAYER_PROFILES)->deckSize = INT_makeHeroDeck(state, ((IntProfile *)PLAYER_PROFILES)->deck, state->stone);
        case 5:
            startModelAnimation(3, 3, -2, 0);
            INT_advancePage(state);
            break;
        case 32:
            INT_WINDOWS[0][1] = 4;
            INT_WINDOWS[4][1] = 4;
            NAME_TASK("FADE OUT");
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 0x10, 0);
            waitFrames(30);
            KAW_freeCursor(state->cursor);
            closeKanjiPage(0xF);
            INT_endScene3D();
            playMusic(0, 4, 0x7F);
            NAME_TASK("NEW");
#if JP_DEBUG_BUILD
            spawnTask(0, -1, 4, 0x1000, openMemcardScreenForNewGame, 0, 0, 0, 0);
#else
            spawnTask(0, -1, 0, 0x1000, openMemcardScreenForNewGame, 0, 0, 0, 0);
#endif
            exitTask();
            return 0;
        }
    }
    if (++state->blinkTimer & 0x10) {
        drawPrimDesc(D_801E469C);
    }
    return 0;
}

void INT_advancePage(IntState *state) {
    *CURRENT_CALLBACK_SLOT = NULL;
    CALLBACK_SLOTS[9] = (void (*)(void *))INT_showNextPage;
    INT_TEXT_WINDOW.draw = INT_drawTypingLines;
}

void INT_openNameEntry(IntState *state, u8 withDate) {
    Rect16 rect;
    s16 i;

    INT_WINDOWS[0][1] = 4;
    waitFrames(30);
    NAME_TASK("WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &INT_NAME_FIELDS_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[1] = (u8 *)waitFrames(0x7FFFFFFF);
    NAME_TASK("WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &INT_BIRTHDAY_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[2] = (u8 *)waitFrames(0x7FFFFFFF);
    NAME_TASK("WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &INT_KEYBOARD_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[3] = (u8 *)waitFrames(0x7FFFFFFF);
    state->keyX = 12;
    state->keyY = 7;
    state->field = 0;
    INT_KEYBOARD_WINDOW.draw = INT_drawKeyboard;
    state->keyPage = 0;
    rect.x = 0x3C0;
    rect.y = 0;
    rect.w = 0x40;
    rect.h = 0xD0;
    ClearImage(&rect, 0, 0, 0);
    INT_loadKeyboardPage(state->keyPage);
    rect.x = 0x3E4;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("かな", &rect);
    rect.x = 0x3E4;
    rect.y = 0xE;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("カナ", &rect);
    rect.x = 0x3E4;
    rect.y = 0x1C;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("英数", &rect);
    rect.x = 0x3E4;
    rect.y = 0x62;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("戻る", &rect);
    rect.x = 0x3E4;
    rect.y = 0x70;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("決定", &rect);
    rect.x = 0x3A4;
    rect.y = 0x62;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("戻る", &rect);
    rect.x = 0x3A4;
    rect.y = 0x70;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("決定", &rect);
    if (withDate) {
        rect.x = 0x3C0;
        rect.y = 0xDC;
        rect.w = 0x40;
        rect.h = 0x24;
        ClearImage(&rect, 0, 0, 0);
        rect.x = 0x3C0;
        rect.y = 0xDC;
        rect.w = 0;
        rect.h = 0;
        uploadKanjiString("＿⇒◆》", &rect);
        rect.x = 0x3C0;
        rect.y = 0xE8;
        rect.w = 0;
        rect.h = 0;
        /* 　姓：　　　　　　名： */
        uploadKanjiString("　姓：　　　　　　名：", &rect);
        rect.x = 0x3C0;
        rect.y = 0xF4;
        rect.w = 0;
        rect.h = 0;
        /* 　生年月日：　　　　年　　月　　日 */
        uploadKanjiString("　生年月日：　　　　年　　月　　日", &rect);
        for (i = 0; i < 6; i++) {
            bzero((Scene3D *)state->fields[i], 14);
            bzero((Scene3D *)state->values[i], 7);
            state->lengths[i] = 0;
        }
        state->keyX = 0;
        state->keyY = 0;
        state->unkC0 = 0;
    }
    waitFrames(36);
    *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runNameEntry;
}

void INT_openNicknameEntry(IntState *state, u8 withDate) {
    Rect16 rect;

    INT_WINDOWS[0][1] = 4;
    waitFrames(30);
    NAME_TASK("WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &INT_NAME_FIELDS_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[1] = (u8 *)waitFrames(0x7FFFFFFF);
    NAME_TASK("WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &INT_KEYBOARD_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[3] = (u8 *)waitFrames(0x7FFFFFFF);
    state->keyX = 12;
    state->keyY = 7;
    state->field = 2;
    INT_KEYBOARD_WINDOW.draw = INT_drawKeyboard;
    state->keyPage = 0;
    rect.x = 0x3C0;
    rect.y = 0;
    rect.w = 0x40;
    rect.h = 0xD0;
    ClearImage(&rect, 0, 0, 0);
    INT_loadKeyboardPage(state->keyPage);
    rect.x = 0x3E4;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("かな", &rect);
    rect.x = 0x3E4;
    rect.y = 0xE;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("カナ", &rect);
    rect.x = 0x3E4;
    rect.y = 0x1C;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("英数", &rect);
    rect.x = 0x3E4;
    rect.y = 0x62;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("戻る", &rect);
    rect.x = 0x3E4;
    rect.y = 0x70;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("決定", &rect);
    if (withDate) {
        rect.x = 0x3C0;
        rect.y = 0xDC;
        rect.w = 0x40;
        rect.h = 0x24;
        ClearImage(&rect, 0, 0, 0);
        rect.x = 0x3C0;
        rect.y = 0xDC;
        rect.w = 0;
        rect.h = 0;
        uploadKanjiString("＿⇒◆》", &rect);
        rect.x = 0x3C0;
        rect.y = 0xE8;
        rect.w = 0;
        rect.h = 0;
        uploadKanjiString("　名前　　：", &rect);
        rect.x = 0x3C0;
        rect.y = 0xF4;
        rect.w = 0;
        rect.h = 0;
        /* 　生年月日：　　　　年　　月　　日 */
        uploadKanjiString("　生年月日：　　　　年　　月　　日", &rect);
        bzero((Scene3D *)state->fields[2], 14);
        bzero((Scene3D *)state->values[2], 7);
        state->lengths[2] = 0;
        state->keyX = 0;
        state->keyY = 0;
        state->unkC0 = 0;
    }
    waitFrames(36);
    *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runNameEntry;
}

void INT_openBabamonWindow(void) {
    Rect16 rect;

    rect.x = 0x3C0;
    rect.y = 0xD0;
    rect.w = 0x40;
    rect.h = 0xC;
    ClearImage(&rect, 0, 0, 0);
    rect.x = 0x3C0;
    rect.y = 0xD0;
    rect.w = 0;
    rect.h = 0;
    INT_waitAnimationEnd(3, 2, -2);
    NAME_TASK("WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &INT_SPEAKER_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[4] = (u8 *)waitFrames(0x7FFFFFFF);
    waitFrames(10);
    uploadKanjiString("バ", &rect);
    waitFrames(4);
    uploadKanjiString("バ", &rect);
    waitFrames(4);
    uploadKanjiString("モ", &rect);
    waitFrames(4);
    uploadKanjiString("ン", &rect);
    INT_waitAnimationEnd(3, 3, -2);
    NAME_TASK("WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, &INT_TEXT_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[0] = (u8 *)waitFrames(0x7FFFFFFF);
    *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_showNextPage;
}

void INT_waitAnimationEnd(s32 slot, s32 anim, s32 loopKey) {
    ModelAnimState *animState;

    while (1) {
        animState = &((Model *)SCENE_3D->models[slot])->anim;
        waitFrames(1);
        if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
            animState->key = -1;
        }
        if (animState->key == -1) {
            if (anim != animState->key) {
                startModelAnimation(slot, anim, loopKey, 0);
            }
            return;
        }
    }
}

/* "no" to a name or nickname: back to the page before, to type it again */
#define INT_REENTER_IF_NO(state, openEntry) \
    do {                                    \
        if ((state)->keyX != 0) {           \
            (state)->page--;                \
            openEntry(state, 0);            \
            return 0;                       \
        }                                   \
    } while (0)

/* the yes/no prompt after a typed name, nickname or the chosen stone */
s32 INT_runYesNoPrompt(void *arg) {
    IntState *state = arg;
    u8 action = INT_PAGES[state->page - 1].action;
    s16 i;
    Scene3D *scene;

    if (PAD_STATES[0]->rawRepeat & PAD_RIGHT) {
        state->keyX = (state->keyX + 1) & 1;
        playSoundEffect(2);
    } else if (PAD_STATES[0]->rawRepeat & PAD_LEFT) {
        state->keyX = (state->keyX + 1) & 1;
        playSoundEffect(2);
    } else if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
        playSoundEffect(0);
        if (action == 2) {
            startModelAnimation(3, 3, -2, 0);
            INT_REENTER_IF_NO(state, INT_openNameEntry);
        } else if (action == 3) {
            if (state->keyX != 0) {
                startModelAnimation(3, 0xE, -2, 0);
                INT_waitAnimationEnd(3, 3, -2);
                state->page++;
            } else {
                startModelAnimation(3, 0xE, -2, 0);
            }
        } else if (action == 0xF) {
            INT_REENTER_IF_NO(state, INT_openNicknameEntry);
            for (i = 0; i < 3; i++) {
                applyAnimationFirstFrame(i, 0);
                startModelAnimation(i, 0, -2, 0);
                SCENE_3D->modelState[i] = 1;
                INT_waitAnimationEnd(i, 1, 0);
            }
            startModelAnimation(3, 9, -2, 0);
            INT_waitAnimationEnd(3, -1, -2);
            strcpy(((IntProfile *)PLAYER_PROFILES)->name, state->fields[2]);
        } else if (action == 0x14) {
            if (state->keyX != 0) {
                startModelAnimation(state->stone, 4, -2, 0);
                INT_waitAnimationEnd(state->stone, -1, -2);
                waitFrames(36);
                state->page = 0x11;
                scene = SCENE_3D;
                ((Model *)scene->models[0])->pos.vx = 0x8C;
                ((Model *)scene->models[1])->pos.vx = 0x118;
                ((Model *)scene->models[2])->pos.vx = 0x1A4;
                ((Model *)scene->models[0])->pos.vy = -0xFA;
                ((Model *)scene->models[1])->pos.vy = -0xFA;
                ((Model *)scene->models[2])->pos.vy = -0xFA;
                applyAnimationFirstFrame(0, 0);
                applyAnimationFirstFrame(1, 0);
                applyAnimationFirstFrame(2, 0);
                startModelAnimation(0, 0, -2, 0);
                INT_waitAnimationEnd(0, 1, 0);
                startModelAnimation(1, 0, -2, 0);
                INT_waitAnimationEnd(1, 1, 0);
                startModelAnimation(2, 0, -2, 0);
                INT_waitAnimationEnd(2, 1, 0);
            } else {
                startModelAnimation(3, 9, -2, 0);
            }
        } else {
            return 0;
        }
        INT_advancePage(state);
        return 0;
    }
    if (state->keyX != 0) {
        KAW_initCursorShape(state->cursor, 0xC, 6, 4);
        KAW_drawCursorAt(state->cursor, 0xB6, 0x52);
    } else {
        KAW_initCursorShape(state->cursor, 0x12, 6, 4);
        KAW_drawCursorAt(state->cursor, 0x8C, 0x52);
    }
    return 0;
}

s32 INT_runStoneChoice(void *arg) {
    IntState *state = arg;
    s8 pages[3] = { 0x12, 0x16, 0x1A };
    s8 *stone = &state->stone;
    s16 i;

    if (PAD_STATES[0]->rawRepeat & PAD_RIGHT) {
        playSoundEffect(2);
        startModelAnimation(3, 3, -2, 0);
        if (++state->stone >= 3) {
            state->stone = 0;
        }
        for (i = 0; i < 3; i++) {
            startModelAnimation(i, 1, 0, 0);
        }
        state->page = pages[*stone];
        INT_loadNextPage(arg);
        startModelAnimation(*stone, 2, -2, 0);
        INT_waitAnimationEnd(*stone, 3, 0);
    } else if (PAD_STATES[0]->rawRepeat & PAD_LEFT) {
        playSoundEffect(2);
        startModelAnimation(3, 3, -2, 0);
        if (--*stone < 0) {
            *stone = 2;
        }
        for (i = 0; i < 3; i++) {
            startModelAnimation(i, 1, 0, 0);
        }
        state->page = pages[*stone];
        INT_loadNextPage(arg);
        startModelAnimation(*stone, 2, -2, 0);
        INT_waitAnimationEnd(*stone, 3, 0);
    } else if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
        playSoundEffect(0);
        NAME_TASK("MOVE");
        spawnTask(0, -1, 0, 0x400, INT_moveChosenStone, 0, 0, 0, 0);
        for (i = 0; i < 3; i++) {
            if (*stone == i) {
                startModelAnimation(i, 5, -2, 0);
            } else {
                startModelAnimation(i, 4, -2, 0);
            }
        }
        if (*stone == 0) {
            state->page = 0x13;
        } else if (*stone == 1) {
            state->page = 0x17;
        } else if (*stone == 2) {
            state->page = 0x1B;
        }
        startModelAnimation(3, 0xE, -2, 0);
        INT_advancePage(state);
        return 0;
    }
#if JP_DEBUG_BUILD
    sprintf(DEBUG_TEXT_LINES[6], "CUR_X = %d\n", *stone);
#endif
    return 0;
}

s32 INT_waitForCircle(IntState *state) {
    if (PAD_STATES[0]->rawPressed & 0x20) {
        playSoundEffect(0);
        startModelAnimation(3, 8, -2, 0);
        INT_waitAnimationEnd(3, -1, -2);
        state->page = 0x1E;
        INT_advancePage(state);
        return 0;
    }
    if (++state->blinkTimer & 0x10) {
        drawPrimDesc(D_801E469C);
    }
    return 0;
}

s32 INT_runNameEntry(void *arg) {
    IntState *state = arg;
    s16 *x = &state->keyX;
    s16 *y = &state->keyY;
    u8 decided = 0;
    s8 maxLens[3] = { 5, 5, 6 };
    u8 field = state->field;
    u8 *name = state->fields[field];
    u8 *kinds = state->values[field];
    u8 maxLen = state->maxLength = maxLens[field];
    u8 *len = &state->lengths[field];

    state->keysPalette = 7;
    state->buttonsPalette = 7;
    if (PAD_STATES[0]->rawRepeat & PAD_RIGHT) {
        playSoundEffect(2);
        if (++state->keyX == 5) {
            state->keyX++;
        } else if (*x == 11) {
            (*x)++;
        } else if (*x == 13) {
            *x = 0;
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_LEFT) {
        playSoundEffect(2);
        if (--*x == 11) {
            (*x)--;
        } else if (*x == 5) {
            (*x)--;
        } else if (*x == -1) {
            *x = 12;
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_UP) {
        playSoundEffect(2);
        (*y)--;
        if (*x == 12 && *y == 6) {
            *y = 2;
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_DOWN) {
        playSoundEffect(2);
        (*y)++;
        if (*x == 12 && *y == 3) {
            *y = 7;
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_START) {
        playSoundEffect(0);
        *x = 12;
        *y = 8;
    } else if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
        playSoundEffect(0);
        decided = 1;
    } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
        playSoundEffect(1);
        if (*len != 0) {
            INT_eraseFieldChar(len, field, name, maxLen);
            kinds[*len >> 1] = 0;
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_R1) {
        if (field == 0) {
            if (kinds[0] != 0 && INT_isFieldBlank(name) == 0) {
                playSoundEffect(0);
                state->field = 1;
                return 0;
            }
        } else if (field == 1) {
            if (kinds[0] != 0 && INT_isFieldBlank(name) == 0) {
                playSoundEffect(0);
                *x = 0;
                *y = 0;
                state->field = 3;
                INT_KEYBOARD_WINDOW.draw = INT_drawDateKeyboard;
                *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runYearEntry;
                return 0;
            }
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_L1) {
        if (field == 1) {
            playSoundEffect(0);
            state->field = 0;
            return 0;
        }
    }
    if (*y >= 9) {
        *y = 0;
    } else if (*y < 0) {
        *y = 8;
    }
    if (*x == 12) {
        if (*y == 3 || *y == 4) {
            *y = 2;
        } else if (*y == 5 || *y == 6) {
            *y = 7;
        }
    }
    if (decided == 1) {
        if (*x == 12) {
            if (*y < 3) {
                state->keyPage = *y;
                INT_loadKeyboardPage(state->keyPage);
            } else if (*y == 7) {
                if (*len != 0) {
                    INT_eraseFieldChar(len, field, name, maxLen);
                    kinds[*len >> 1] = 0;
                }
            } else if (*y == 8) {
                if (field == 0) {
                    if (kinds[0] != 0 && INT_isFieldBlank(name) == 0) {
                        state->field = 1;
                        *x = 0;
                        *y = 0;
                        return 0;
                    }
                } else if (field == 1) {
                    if (kinds[0] != 0 && INT_isFieldBlank(name) == 0) {
                        *x = 0;
                        *y = 0;
                        state->field = 3;
                        INT_KEYBOARD_WINDOW.draw = INT_drawDateKeyboard;
                        *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runYearEntry;
                        return 0;
                    }
                } else if (field == 2) {
                    if (kinds[0] != 0 && INT_isFieldBlank(name) == 0) {
                        strcpy(INT_PAGES[14].lines[0], name);
                        INT_WINDOWS[1][1] = 4;
                        INT_WINDOWS[3][1] = 4;
                        waitFrames(30);
                        INT_advancePage(state);
                        NAME_TASK("WIN");
                        spawnTask(0, -1, 0, 0x800, runWindowTask, &INT_TEXT_WINDOW, getCurrentTaskId(), 0, 0);
                        INT_WINDOWS[0] = (u8 *)waitFrames(0x7FFFFFFF);
                        return 0;
                    }
                }
            }
        } else if (*len < maxLen * 2) {
            kinds[*len >> 1] = INT_getKeyValue(state);
            INT_typeFieldChar(state, len, field, name, maxLen, NULL);
        } else {
            *x = 12;
            *y = 8;
        }
    }
#if JP_DEBUG_BUILD
    sprintf(DEBUG_TEXT_LINES[0], "CUR_X = %d CUR_Y = %d\n", *x, *y);
    sprintf(DEBUG_TEXT_LINES[1], "in_xp = %d \n", state->lengths[field]);
    sprintf(DEBUG_TEXT_LINES[2], "%d %d %d %d %d %d %d \n", kinds[0], kinds[1], kinds[2], kinds[3], kinds[4], kinds[5],
            kinds[6]);
    sprintf(DEBUG_TEXT_LINES[3], "PLACE = %d \n", field);
    sprintf(DEBUG_TEXT_LINES[4], "%2x %2x %2x %2x %2x %2x \n", name[0], name[1], name[2], name[3], name[4], name[5]);
#endif
    if (*x == 12) {
        KAW_initCursorShape(state->cursor, 12, 6, 4);
        KAW_drawCursorAt(state->cursor, *x * 12 + 0x83, *y * 14 + 0x5C);
    } else {
        KAW_initCursorShape(state->cursor, 6, 6, 4);
        KAW_drawCursorAt(state->cursor, *x * 12 + 0x7D, *y * 14 + 0x5C);
    }
    INT_drawFieldCursor(*len >> 1, field);
    return 0;
}

s32 INT_runYearEntry(void *arg) {
    IntState *state = arg;
    s16 *x = &state->keyX;
    s16 *y = &state->keyY;
    u8 decided = 0;
    s8 maxLens[6] = { 5, 5, 6, 4, 2, 2 };
    u8 *digits[10] = {
        "０", "１", "２", "３", "４",
        "５", "６", "７", "８", "９",
    };
    u8 field = state->field;
    u8 *name = state->fields[field];
    u8 *kinds = state->values[field];
    u8 *len;
    u32 year;
    u8 maxLen = state->maxLength = maxLens[field];

    len = &state->lengths[field];
    state->keysPalette = 7;
    state->buttonsPalette = 7;
    if (PAD_STATES[0]->rawRepeat & PAD_RIGHT) {
        playSoundEffect(2);
        if (++*x == 10) {
            *x = 12;
            *y = 7;
        } else if (*x == 13) {
            *x = 0;
            *y = 0;
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_LEFT) {
        playSoundEffect(2);
        if (--*x < 0) {
            *x = 12;
            *y = 7;
        } else if (*x == 11) {
            *x = 9;
            *y = 0;
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_UP) {
        if (*y == 7) {
            *y = 8;
            playSoundEffect(2);
        } else if (*y == 8) {
            *y = 7;
            playSoundEffect(2);
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_DOWN) {
        if (*y == 7) {
            *y = 8;
            playSoundEffect(2);
        } else if (*y == 8) {
            *y = 7;
            playSoundEffect(2);
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_START) {
        playSoundEffect(0);
        *x = 12;
        *y = 8;
    } else if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
        playSoundEffect(0);
        decided = 1;
    } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
        playSoundEffect(1);
        if (*len != 0) {
            INT_eraseFieldChar(len, field, name, maxLen);
            kinds[*len >> 1] = 0;
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_R1) {
        year = kinds[0] * 1000 + kinds[1] * 100 + kinds[2] * 10 + kinds[3];
        if (year >= 1900 && *len == maxLen * 2) {
            playSoundEffect(0);
            *x = 0;
            *y = 1;
            state->field = 4;
            *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runMonthEntry;
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_L1) {
        playSoundEffect(0);
        *x = 0;
        *y = 0;
        state->field = 1;
        INT_KEYBOARD_WINDOW.draw = INT_drawKeyboard;
        *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runNameEntry;
    }
    if (decided == 1) {
        if (*x == 12) {
            if (*y == 7) {
                if (*len != 0) {
                    INT_eraseFieldChar(len, field, name, maxLen);
                    kinds[*len >> 1] = 0;
                }
            } else if (*y == 8) {
                year = kinds[0] * 1000 + kinds[1] * 100 + kinds[2] * 10 + kinds[3];
                if (year >= 1900 && *len == maxLen * 2) {
                    *x = 0;
                    *y = 1;
                    state->field = 4;
                    *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runMonthEntry;
                }
            }
        } else if (*len < maxLen * 2) {
            kinds[*len >> 1] = *x;
            INT_typeFieldChar(state, len, field, name, maxLen, digits[*x]);
        } else {
            *x = 12;
            *y = 8;
        }
    }
#if JP_DEBUG_BUILD
    sprintf(DEBUG_TEXT_LINES[0], "CUR_X = %d CUR_Y = %d\n", *x, *y);
    sprintf(DEBUG_TEXT_LINES[1], "in_xp = %d \n", state->lengths[field]);
    sprintf(DEBUG_TEXT_LINES[2], "%d %d %d %d %d %d %d \n", kinds[0], kinds[1], kinds[2], kinds[3], kinds[4], kinds[5],
            kinds[6]);
    sprintf(DEBUG_TEXT_LINES[3], "PLACE = %d \n", field);
#endif
    if (*x == 12) {
        KAW_initCursorShape(state->cursor, 12, 6, 4);
        KAW_drawCursorAt(state->cursor, *x * 12 + 0x83, *y * 14 + 0x5C);
    } else {
        KAW_initCursorShape(state->cursor, 6, 6, 4);
        KAW_drawCursorAt(state->cursor, *x * 12 + 0x7D, *y * 14 + 0x5C);
    }
    INT_drawFieldCursor(*len >> 1, field);
    return 0;
}

s32 INT_runMonthEntry(void *arg) {
    IntState *state = arg;
    s16 *x = &state->keyX;
    s16 *y = &state->keyY;
    u8 decided = 0;
    s8 maxLens[6] = { 5, 5, 6, 4, 2, 2 };
    u8 tens[16];
    u8 ones[16];
    u8 field = state->field;
    u8 *name = state->fields[field];
    u8 *kinds = state->values[field];
    u8 *len;
    u8 maxLen = state->maxLength = maxLens[field];
    u8 next;
    u8 *nextLen;
    u32 month;

    len = &state->lengths[field];
    state->keysPalette = 7;
    state->buttonsPalette = 7;
    if (PAD_STATES[0]->rawRepeat & PAD_RIGHT) {
        playSoundEffect(2);
        if (++state->keyX == 6) {
            state->keyX = 12;
            state->keyY = 7;
        } else if (*x == 13) {
            *x = 0;
            *y = 2;
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_LEFT) {
        playSoundEffect(2);
        if (--*x < 0) {
            *x = 12;
            *y = 7;
        } else if (*x == 11) {
            *x = 5;
            *y = 2;
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_UP) {
        playSoundEffect(2);
        if (*y == 7) {
            *y = 8;
        } else if (*y == 8) {
            *y = 7;
        } else if (*y == 1) {
            *y = 2;
        } else if (*y == 2) {
            *y = 1;
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_DOWN) {
        playSoundEffect(2);
        if (*y == 7) {
            *y = 8;
        } else if (*y == 8) {
            *y = 7;
        } else if (*y == 1) {
            *y = 2;
        } else if (*y == 2) {
            *y = 1;
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_START) {
        playSoundEffect(0);
        *x = 12;
        *y = 8;
    } else if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
        playSoundEffect(0);
        decided = 1;
    } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
        playSoundEffect(1);
        if (*len != 0) {
            do {
                INT_eraseFieldChar(len, field, name, maxLen);
                kinds[*len >> 1] = 0;
            } while (*len != 0);
            next = state->field + 1;
            nextLen = &state->lengths[next];
            if (*nextLen != 0) {
                do {
                    INT_eraseFieldChar(nextLen, next, state->fields[next], maxLens[next]);
                    state->values[next][*nextLen >> 1] = 0;
                } while (*nextLen != 0);
            }
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_R1) {
        if (kinds[0] != 0 || kinds[1] != 0) {
            playSoundEffect(0);
            *x = 0;
            *y = 3;
            state->field = 5;
            *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runDayEntry;
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_L1) {
        playSoundEffect(0);
        *x = 0;
        *y = 0;
        state->field = 3;
        *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runYearEntry;
        return 0;
    }
    if (decided == 1) {
        if (*x == 12) {
            if (*y == 7) {
                if (*len != 0) {
                    do {
                        INT_eraseFieldChar(len, field, name, maxLen);
                        kinds[*len >> 1] = 0;
                    } while (*len != 0);
                    next = state->field + 1;
                    nextLen = &state->lengths[next];
                    if (*nextLen != 0) {
                        do {
                            INT_eraseFieldChar(nextLen, next, state->fields[next], maxLens[next]);
                            state->values[next][*nextLen >> 1] = 0;
                        } while (*nextLen != 0);
                    }
                }
            } else if (*y == 8) {
                if (kinds[0] != 0 || kinds[1] != 0) {
                    *x = 0;
                    *y = 3;
                    state->field = 5;
                    *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runDayEntry;
                }
            }
        } else if (*len < maxLen * 2) {
            month = (*y - 1) * 6 + *x + 1;
            kinds[*len >> 1] = month / 10;
            formatSjisNumber(kinds[*len >> 1], 1, tens);
            INT_typeFieldChar(state, len, field, name, maxLen, tens);
            kinds[*len >> 1] = month % 10;
            formatSjisNumber(kinds[*len >> 1], 1, ones);
            INT_typeFieldChar(state, len, field, name, maxLen, ones);
        } else {
            *x = 12;
            *y = 8;
        }
    }
#if JP_DEBUG_BUILD
    sprintf(DEBUG_TEXT_LINES[0], "CUR_X = %d CUR_Y = %d\n", *x, *y);
    sprintf(DEBUG_TEXT_LINES[1], "in_xp = %d \n", state->lengths[field]);
    sprintf(DEBUG_TEXT_LINES[2], "%d %d %d %d %d %d %d \n", kinds[0], kinds[1], kinds[2], kinds[3], kinds[4], kinds[5],
            kinds[6]);
    sprintf(DEBUG_TEXT_LINES[3], "PLACE = %d \n", field);
#endif
    if (*x == 12) {
        KAW_initCursorShape(state->cursor, 12, 6, 4);
        KAW_drawCursorAt(state->cursor, *x * 12 + 0x83, *y * 14 + 0x5C);
    } else {
        KAW_initCursorShape(state->cursor, 6, 6, 4);
        KAW_drawCursorAt(state->cursor, *x * 12 + 0x7D, *y * 14 + 0x5C);
    }
    INT_drawFieldCursor(*len >> 1, field);
    return 0;
}

s32 INT_runDayEntry(void *arg) {
    IntState *state = arg;
    s16 *x = &state->keyX;
    s16 *y = &state->keyY;
    u8 decided = 0;
    s8 maxLens[6] = { 5, 5, 6, 4, 2, 2 };
    u8 tens[16];
    u8 ones[16];
    u8 monthDays[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    u8 field = state->field;
    u8 *name = state->fields[field];
    u8 *kinds = state->values[field];
    u8 maxLen = state->maxLength = maxLens[field];
    u8 *len = &state->lengths[field];
    u8 maxDay = monthDays[state->values[field - 1][0] * 10 + state->values[field - 1][1] - 1];
    u32 day;

    if (PAD_STATES[0]->rawRepeat & PAD_RIGHT) {
        playSoundEffect(2);
        if (++state->keyX == 7) {
            state->keyX = 12;
            state->keyY = 7;
        } else if (*x == 13) {
            *x = 0;
            *y = 6;
        } else {
            day = (*y - 3) * 7 + *x + 1;
            if (maxDay < day) {
                *x = 12;
                *y = 7;
            }
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_LEFT) {
        playSoundEffect(2);
        if (--*x < 0) {
            *x = 12;
            *y = 7;
        } else if (*x == 11) {
            *x = 6;
            *y = 6;
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_UP) {
        playSoundEffect(2);
        if (*x == 12) {
            if (*y == 7) {
                *y = 8;
            } else if (*y == 8) {
                *y = 7;
            }
        } else if (--*y < 3) {
            *y = 7;
            day = (*y - 3) * 7 + *x + 1;
            if (maxDay < day) {
                (*y)--;
            }
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_DOWN) {
        playSoundEffect(2);
        if (*x == 12) {
            if (*y == 7) {
                *y = 8;
            } else if (*y == 8) {
                *y = 7;
            }
        } else {
            day = (++*y - 3) * 7 + *x + 1;
            if (maxDay < day) {
                *y = 3;
            }
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_START) {
        playSoundEffect(0);
        *x = 12;
        *y = 8;
    } else if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
        playSoundEffect(0);
        decided = 1;
    } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
        playSoundEffect(1);
        if (*len != 0) {
            do {
                INT_eraseFieldChar(len, field, name, maxLen);
                kinds[*len >> 1] = 0;
            } while (*len != 0);
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_R1) {
    } else if (PAD_STATES[0]->rawPressed & PAD_L1) {
        playSoundEffect(0);
        *x = 0;
        *y = 1;
        state->field = 4;
        *CURRENT_CALLBACK_SLOT = (void (*)(void *))INT_runMonthEntry;
    }
    if (decided == 1) {
        if (*x == 12) {
            if (*y == 7) {
                if (*len != 0) {
                    do {
                        INT_eraseFieldChar(len, field, name, maxLen);
                        kinds[*len >> 1] = 0;
                    } while (*len != 0);
                }
            } else if (*y == 8) {
                if (kinds[0] != 0 || kinds[1] != 0) {
                    INT_formatNameAndBirthday(state);
                    state->keysPalette = 7;
                    state->buttonsPalette = 7;
                    INT_WINDOWS[1][1] = 4;
                    INT_WINDOWS[2][1] = 4;
                    INT_WINDOWS[3][1] = 4;
                    waitFrames(30);
                    INT_advancePage(state);
                    NAME_TASK("WIN");
                    spawnTask(0, -1, 0, 0x800, runWindowTask, &INT_TEXT_WINDOW, getCurrentTaskId(), 0, 0);
                    INT_WINDOWS[0] = (u8 *)waitFrames(0x7FFFFFFF);
                    return 0;
                }
            }
        } else if (*len < maxLen * 2) {
            day = (*y - 3) * 7 + *x + 1;
            kinds[*len >> 1] = day / 10;
            formatSjisNumber(kinds[*len >> 1], 1, tens);
            INT_typeFieldChar(state, len, field, name, maxLen, tens);
            kinds[*len >> 1] = day % 10;
            formatSjisNumber(kinds[*len >> 1], 1, ones);
            INT_typeFieldChar(state, len, field, name, maxLen, ones);
        } else {
            *x = 12;
            *y = 8;
        }
    }
#if JP_DEBUG_BUILD
    sprintf(DEBUG_TEXT_LINES[0], "CUR_X = %d CUR_Y = %d\n", *x, *y);
    sprintf(DEBUG_TEXT_LINES[1], "in_xp = %d \n", state->lengths[field]);
    sprintf(DEBUG_TEXT_LINES[2], "%d %d %d %d %d %d %d \n", kinds[0], kinds[1], kinds[2], kinds[3], kinds[4], kinds[5],
            kinds[6]);
    sprintf(DEBUG_TEXT_LINES[3], "PLACE = %d \n", field);
#endif
    if (*x == 12) {
        KAW_initCursorShape(state->cursor, 12, 6, 4);
        KAW_drawCursorAt(state->cursor, *x * 12 + 0x83, *y * 14 + 0x5C);
    } else {
        KAW_initCursorShape(state->cursor, 6, 6, 4);
        KAW_drawCursorAt(state->cursor, *x * 12 + 0x7D, *y * 14 + 0x5C);
    }
    INT_drawFieldCursor(*len >> 1, field);
    return 0;
}

void INT_eraseFieldChar(u8 *len, u8 row, u8 *name, u8 maxLen) {
    Rect16 rect;
    u8 bottom;
    u8 col;
    u8 columns[6][10] = {
        { 0x09, 0x0C, 0x0F, 0x12, 0x15 },
        { 0x21, 0x24, 0x27, 0x2A, 0x2D },
        { 0x12, 0x15, 0x18, 0x1B, 0x1E, 0x21 },
        { 0x12, 0x15, 0x18, 0x1B },
        { 0x21, 0x24 },
        { 0x2A, 0x2D },
    };

    *len -= 2;
    col = *len >> 1;
    bottom = row >= 3;
    rect.x = columns[row][col] + 0x3C0;
    rect.y = bottom * 12 + 0xE8;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("　", &rect);
    name[*len] = 0;
    name[*len + 1] = 0;
}

void INT_typeFieldChar(IntState *state, u8 *len, u8 row, u8 *name, u8 maxLen, u8 *text) {
    Rect16 rect;
    u8 glyph[3];
    s16 *keyX = &state->keyX;
    s16 *keyY = &state->keyY;
    u8 *key = NULL;
    u8 columns[6][10] = {
        { 0x09, 0x0C, 0x0F, 0x12, 0x15 },
        { 0x21, 0x24, 0x27, 0x2A, 0x2D },
        { 0x12, 0x15, 0x18, 0x1B, 0x1E, 0x21 },
        { 0x12, 0x15, 0x18, 0x1B },
        { 0x21, 0x24 },
        { 0x2A, 0x2D },
    };
    u8 *str;
    u8 col;
    u8 bottom;

    col = *len >> 1;
    bottom = row >= 3;
    if (text == NULL) {
        key = INT_KEYBOARD_ROWS[state->keyPage * 9 + *keyY] + *keyX * 2;
        glyph[0] = key[0];
        glyph[1] = key[1];
        glyph[2] = 0;
        rect.x = columns[row][col] + 0x3C0;
        rect.y = bottom * 12 + 0xE8;
        rect.w = 0;
        rect.h = 0;
        str = glyph;
    } else {
        rect.x = columns[row][col] + 0x3C0;
        rect.y = bottom * 12 + 0xE8;
        rect.w = 0;
        rect.h = 0;
        str = text;
    }
    uploadKanjiString(str, &rect);
    name[(*len)++] = key[0];
    name[(*len)++] = key[1];
    if (*len >= maxLen * 2) {
        *keyX = 12;
        *keyY = 8;
    }
}

void INT_drawFieldCursor(u8 col, u8 field) {
    IntState *state = &INT_STATE;
    Rect16 rect;
    Bytes4 rgb = { { 0x80, 0x80, 0x80, 0 } };
    u16 xs[6][11] = {
        { 0x78, 0x84, 0x90, 0x9C, 0xA8, 0, 0, 0, 0, 0, 0x140 },
        { 0xD8, 0xE4, 0xF0, 0xFC, 0x108, 0, 0, 0, 0, 0, 0x140 },
        { 0x9C, 0xA8, 0xB4, 0xC0, 0xCC, 0xD8, 0, 0, 0, 0, 0x140 },
        { 0x9C, 0xA8, 0xB4, 0xC0, 0, 0, 0, 0, 0, 0, 0x140 },
        { 0xD8, 0xE4, 0, 0, 0, 0, 0, 0, 0, 0, 0x140 },
        { 0xFC, 0x108, 0, 0, 0, 0, 0, 0, 0, 0, 0x140 },
    };
    u16 ys[2];
    u16 y;
    IntBlink *blink;

    ys[0] = 0x24;
    ys[1] = 0x3E;
    if (field < 3) {
        y = ys[0];
    } else {
        y = ys[1];
    }
    if (state->maxLength == col) {
        col = 10;
    }
    rect.x = 0;
    rect.y = 0xDC;
    rect.w = 0xC;
    rect.h = 0xC;
    drawPageSpriteColored(xs[field][col], y, &rect, rgb.b, 0xF, 6, 0);
    blink = INT_FIELD_BLINKS;
    if (field == 0) {
        blink->x = 0x77;
        blink->y = 0x21;
        blink->w = 0x3E;
    } else if (field == 1) {
        blink->x = 0xD7;
        blink->y = 0x21;
        blink->w = 0x3E;
    } else if (field == 2) {
        blink->x = 0x9B;
        blink->y = 0x21;
        blink->w = 0x4A;
    } else if (field == 3) {
        blink->x = 0x9B;
        blink->y = 0x3B;
        blink->w = 0x30;
    } else if (field == 4) {
        blink->x = 0xD7;
        blink->y = 0x3B;
        blink->w = 0x1A;
    } else if (field == 5) {
        blink->x = 0xFB;
        blink->y = 0x3B;
        blink->w = 0x1A;
    }
    drawPrimDesc((PrimDesc *)blink);
}

u16 INT_getKeyValue(IntState *state) {
    s16 *x = &state->keyX;
    s16 *y = &state->keyY;
    u16 kind = 0;

    switch (state->keyPage) {
    case 0:
    case 1:
        if (*x < 5) {
            if (*x == 1 && *y == 7) {
                kind = 10;
            } else if (*x == 3 && *y == 7) {
                kind = 10;
            } else {
                kind = *x + 1;
            }
        } else if (*y == 8) {
            kind = 10;
        } else if (*x == 7 && *y == 0) {
            kind = 10;
        } else if (*x == 9 && *y == 0) {
            kind = 10;
        } else if (*x == 10 && *y == 0) {
            kind = 6;
        } else if (*x == 10 && *y == 7) {
            kind = 6;
        } else {
            kind = *x - 5;
        }
        break;
    case 2:
        if (*x < 5) {
            kind = *y * 5 + *x;
            if (kind < 26) {
                kind++;
            } else if (*y != 7) {
                kind = 10;
            } else if (*x == 0) {
                kind = 10;
            } else {
                kind = *x;
            }
        } else {
            kind = *y * 5 + (*x - 6);
            if (kind < 26) {
                kind++;
            } else if (*y == 7) {
                kind = *x - 1;
            } else {
                kind = 10;
            }
        }
        break;
    }
    return kind;
}

s16 INT_isFieldBlank(u8 *name) {
    u8 hi;
    u8 lo;
    u8 used;
    s16 i;

    used = 0;
    i = 0;
    while (1) {
        hi = *name++;
        lo = *name++;
        if (hi == 0 && lo == 0) {
            break;
        }
        if (hi != 0x81 || lo != 0x40) {
            used++;
            break;
        }
        if (++i >= 6) {
            break;
        }
    }
    return used == 0;
}

s32 INT_formatNameAndBirthday(IntState *state) {
    u8 text[3][10];
    u8 *date = state->values[3];
    u16 yearValue = date[0] * 1000 + date[1] * 100 + date[2] * 10 + date[3];
    u16 monthValue = state->values[4][0] * 10 + state->values[4][1];
    u16 dayValue = state->values[5][0] * 10 + state->values[5][1];

    formatSjisNumberZeros(yearValue, 4, text[0]);
    formatSjisNumberZeros(monthValue, 2, text[1]);
    formatSjisNumberZeros(dayValue, 2, text[2]);
    sprintf(INT_PAGES[1].lines[0], "%s　%s", state->fields[0], state->fields[1]);
    /* %s年%s月%s日生まれ */
    sprintf(INT_PAGES[1].lines[1], "%s年%s月%s日生まれ", text[0], text[1], text[2]);
    return 0;
}

void INT_drawTypingLines(IntWindow *win) {
    IntState *state = &INT_STATE;
    Rect16 rect;
    Bytes4 rgb = { { 0x80, 0x80, 0x80, 0 } };
    s16 i;

    state->typedChars++;
    for (i = 0; i < state->typedLines; i++) {
        if (i == state->typedLines - 1) {
            rect.x = 0;
            rect.y = i * 12;
            rect.w = state->typedChars * 12;
            rect.h = 0xB;
        } else {
            rect.x = 0;
            rect.y = i * 12;
            rect.w = 0xFC;
            rect.h = 0xB;
        }
        drawPageSpriteColored(0x26, win->y + 5 + i * 14, &rect, rgb.b, 0xF, 7, win->z);
    }
    if (state->typedChars >= (u8)state->lineLengths[state->typedLines - 1]) {
        state->typedLines++;
        state->typedChars = 0;
        if (INT_PAGES[state->page - 1].lineActions[state->typedLines - 2] != 0) {
            if (state->lineCount >= state->typedLines) {
                INT_TEXT_WINDOW.draw = INT_drawTypedLines;
                CALLBACK_SLOTS[7] = (void (*)(void *))INT_runLineAction;
            } else {
                state->typedLines = 1;
                INT_TEXT_WINDOW.draw = INT_drawPageLines;
                CALLBACK_SLOTS[7] = (void (*)(void *))INT_runPageAction;
            }
            return;
        }
    }
    if (state->typedLines > state->lineCount) {
        state->typedChars = 0;
        state->typedLines = 1;
        INT_TEXT_WINDOW.draw = INT_drawPageLines;
        CALLBACK_SLOTS[7] = (void (*)(void *))INT_runPageAction;
    }
}

void INT_drawPageLines(IntWindow *win) {
    IntState *state = &INT_STATE;
    Rect16 rect;
    Bytes4 rgb = { { 0x80, 0x80, 0x80, 0 } };
    s16 i;

    for (i = 0; i < state->lineCount; i++) {
        rect.x = 0;
        rect.y = i * 12;
        rect.w = 0xFC;
        rect.h = 0xB;
        drawPageSpriteColored(0x26, win->y + 5 + i * 14, &rect, rgb.b, 0xF, 7, win->z);
    }
}

void INT_drawTypedLines(IntWindow *win) {
    IntState *state = &INT_STATE;
    Rect16 rect;
    Bytes4 rgb = { { 0x80, 0x80, 0x80, 0 } };
    s16 i;

    for (i = 0; i < state->typedLines; i++) {
        if (i == state->typedLines - 1) {
            rect.x = 0;
            rect.y = i * 12;
            rect.w = state->typedChars * 12;
            rect.h = 0xB;
        } else {
            rect.x = 0;
            rect.y = i * 12;
            rect.w = 0xFC;
            rect.h = 0xB;
        }
        drawPageSpriteColored(0x26, win->y + 5 + i * 14, &rect, rgb.b, 0xF, 7, win->z);
    }
}

void INT_drawKeyboard(IntWindow *win) {
    IntState *state = &INT_STATE;
    Rect16 rect;
    Bytes4 rgb = { { 0x80, 0x80, 0x80, 0 } };

    rect.x = 0;
    rect.y = 0;
    rect.w = 0xA8;
    rect.h = 0x62;
    drawPageSpriteColored(win->x + 4, win->y + 4, &rect, rgb.b, 0xF, state->keysPalette, win->z);
    rect.x = 0;
    rect.y = 0x62;
    rect.w = 0xA8;
    rect.h = 0x1C;
    drawPageSpriteColored(win->x + 4, win->y + 0x66, &rect, rgb.b, 0xF, state->buttonsPalette, win->z);
}

void INT_drawDateKeyboard(IntWindow *win) {
    IntState *state = &INT_STATE;
    Rect16 rect;
    Bytes4 rgb = { { 0x80, 0x80, 0x80, 0 } };
    s16 w = 0;
    u8 palette;
    u8 month;

    rect.x = 0x90;
    rect.y = 0x62;
    rect.w = 0x18;
    rect.h = 0x1C;
    drawPageSpriteColored(win->x + 0x94, win->y + 0x66, &rect, rgb.b, 0xE, 7, win->z);
    palette = 0;
    if (state->field == 3) {
        palette = 7;
    }
    rect.x = 0;
    rect.y = 0;
    rect.w = 0x78;
    rect.h = 0xE;
    drawPageSpriteColored(win->x + 4, win->y + 4, &rect, rgb.b, 0xE, palette, win->z);
    palette = 0;
    if (state->field == 4) {
        palette = 7;
    }
    rect.x = 0;
    rect.y = 0xE;
    rect.w = 0x78;
    rect.h = 0x1C;
    drawPageSpriteColored(win->x + 4, win->y + 0x12, &rect, rgb.b, 0xE, palette, win->z);
    palette = 0;
    if (state->field == 5) {
        palette = 7;
    }
    rect.x = 0;
    rect.y = 0x2A;
    rect.w = 0x78;
    rect.h = 0x38;
    drawPageSpriteColored(win->x + 4, win->y + 0x2E, &rect, rgb.b, 0xE, palette, win->z);
    palette = 0;
    if (state->field == 5) {
        palette = 7;
        month = state->values[4][0] * 10 + state->values[4][1];
        switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            w = 0x24;
            break;
        case 2:
            w = 0xC;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            w = 0x18;
            break;
        }
    } else {
        w = 0x24;
    }
    rect.x = 0;
    rect.y = 0x62;
    rect.w = w;
    rect.h = 0xE;
    drawPageSpriteColored(win->x + 4, win->y + 0x66, &rect, rgb.b, 0xE, palette, win->z);
}

void INT_drawNameFields(IntWindow *win) {
    Rect16 rect;
    Bytes4 rgb = { { 0x80, 0x80, 0x80, 0 } };

    rect.x = 0;
    rect.y = 0xE8;
    rect.w = 0xCC;
    rect.h = 0xC;
    drawPageSpriteColored(win->x + 4, win->y + 4, &rect, rgb.b, 0xF, 7, win->z);
}

void INT_drawBirthdayField(IntWindow *win) {
    Rect16 rect;
    Bytes4 rgb = { { 0x80, 0x80, 0x80, 0 } };

    rect.x = 0;
    rect.y = 0xF4;
    rect.w = 0xCC;
    rect.h = 0xC;
    drawPageSpriteColored(win->x + 4, win->y + 4, &rect, rgb.b, 0xF, 7, win->z);
}

void INT_drawSpeakerName(IntWindow *win) {
    Rect16 rect;
    Bytes4 rgb = { { 0x80, 0x80, 0x80, 0 } };

    rect.x = 0;
    rect.y = 0xD0;
    rect.w = 0x30;
    rect.h = 0xC;
    drawPageSpriteColored(win->x + 0xD, win->y, &rect, rgb.b, 0xF, 4, win->z);
}

void INT_loadKeyboardPage(u8 page) {
    Rect16 rect = { 0x3C0, 0, 0x21, 0x6C };
    u8 first;
    s16 i;

    ClearImage(&rect, 0, 0, 0);
    first = page * 9;
    for (i = 0; i < 9; i++) {
        rect.x = 0x3C0;
        rect.y = i * 14;
        rect.w = 0;
        rect.h = 0;
        uploadKanjiString(INT_KEYBOARD_ROWS[first + i], &rect);
    }
}

/* deck[i]: the r-th of the 53 cards (then of the 60 cards) the hero deck
   draws its third to fifth (then sixth to tenth) cards from */
#define INT_PICK_CARD_53(r) do { \
    if (r < 8) { \
        deck[i] = r + 3; \
    } else if (r >= 8 && r < 16) { \
        deck[i] = r + 15; \
    } else if (r >= 16 && r < 28) { \
        deck[i] = r + 29; \
    } else if (r >= 28 && r < 37) { \
        deck[i] = r + 44; \
    } else if (r >= 37 && r < 43) { \
        deck[i] = r + 57; \
    } else if (r >= 43 && r < 50) { \
        deck[i] = r + 86; \
    } else if (r >= 50 && r < 53) { \
        deck[i] = r + 104; \
    } \
} while (0)
#define INT_PICK_CARD_60(r) do { \
    if (r < 9) { \
        deck[i] = r + 11; \
    } else if (r >= 9 && r < 20) { \
        deck[i] = r + 22; \
    } else if (r >= 20 && r < 32) { \
        deck[i] = r + 37; \
    } else if (r >= 32 && r < 42) { \
        deck[i] = r + 49; \
    } else if (r >= 42 && r < 50) { \
        deck[i] = r + 58; \
    } else if (r >= 50 && r < 59) { \
        deck[i] = r + 86; \
    } else if (r == 59) { \
        deck[i] = 157; \
    } \
} while (0)

/* the hero's first deck: 10 to 15 cards drawn from the birth date and the
   names, the last ones random cards of the chosen stone's colour; returns
   how many. The tables are statement macros, and the match depends on
   their do-while, which lays the copies out as the original's (the bodies
   after the colour cases) */
u8 INT_makeHeroDeck(IntState *state, u8 *deck, u8 color) {
    u8 cards[15] = { 0x00, 0x01, 0x02, 0x14, 0x15, 0x16, 0x2A, 0x2B, 0x2C, 0x45, 0x46, 0x47, 0x5B, 0x5C, 0x5D };
    s16 i;
    u8 *date;
    u16 year;
    u16 month;
    u16 day;
    u16 nameSum;
    u16 seed;
    u16 r;
    u8 count;

    for (i = 0; i < 15; i++) {
        deck[i] = 0xFF;
    }
    date = state->values[3];
    year = date[0] * 1000 + date[1] * 100 + date[2] * 10 + date[3];
    month = state->values[4][0] * 10 + state->values[4][1];
    day = state->values[5][0] * 10 + state->values[5][1];
    nameSum = 0;
    date = state->values[0];
    for (i = 0; date[i] != 0; i++) {
        nameSum += date[i];
    }
    date = state->values[1];
    for (i = 0; date[i] != 0; i++) {
        nameSum += date[i];
    }
    seed = nameSum < 6 ? 6 : nameSum;
    seed %= 6;
    count = seed + 10;
    for (i = 0; i < count; i++) {
        switch (i) {
        case 0:
            r = (year + month + day) % 15;
            deck[i] = cards[r];
            break;
        case 1:
            seed = nameSum < 20 ? 20 : nameSum;
            r = seed % 20;
            if (r == 19) {
                deck[i] = 0x99;
            } else {
                deck[i] = r + 0x6E;
            }
            break;
        case 2:
            r = (year + month * day) % 53;
            INT_PICK_CARD_53(r);
            break;
        case 3:
            seed = nameSum < 20 ? 20 : nameSum;
            r = seed * 10 % 53;
            INT_PICK_CARD_53(r);
            break;
        case 4:
            r = (nameSum + year + month + day) % 53;
            INT_PICK_CARD_53(r);
            break;
        case 5:
            r = (year - month - day) % 60;
            INT_PICK_CARD_60(r);
            break;
        case 6:
            r = (year - month * day) % 60;
            INT_PICK_CARD_60(r);
            break;
        case 7:
            r = (nameSum + year + month - day) % 60;
            INT_PICK_CARD_60(r);
            break;
        case 8:
            r = (nameSum + year + month * day) % 60;
            INT_PICK_CARD_60(r);
            break;
        case 9:
            r = (nameSum + year - month + day) % 60;
            INT_PICK_CARD_60(r);
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            if (color == 0) {
                r = rand() % 17;
                deck[i] = r + 3;
            } else if (color == 1) {
                r = rand() % 19;
                deck[i] = r + 23;
            } else if (color == 2) {
                r = rand() % 24;
                deck[i] = r + 45;
            }
            break;
        }
    }
    return count;
}

void INT_addStarterDeck(u8 player, u8 slot, u8 color) {
    IntDeck *deck = &((IntProfile *)PLAYER_PROFILES)[player].decks[slot];
    u8 starters[3][30] = {
        {
            0x03, 0x05, 0x06, 0x0C, 0x0C, 0x0F, 0x22, 0x1F, 0x3D, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13,
            0x11, 0x28, 0x28, 0x28, 0x25, 0x25, 0x40, 0x40, 0x88, 0x88, 0x89, 0x89, 0x8A, 0x8A, 0x8B,
        },
        {
            0x17, 0x1C, 0x1D, 0x21, 0x21, 0x22, 0x39, 0x3C, 0x0C, 0x28, 0x28, 0x28, 0x29, 0x29, 0x27,
            0x27, 0x43, 0x43, 0x40, 0x40, 0x42, 0x11, 0x11, 0x90, 0x90, 0x8C, 0x8C, 0x8D, 0x8D, 0x8E,
        },
        {
            0x2D, 0x2F, 0x3F, 0x34, 0x34, 0x35, 0x08, 0x0D, 0x1E, 0x42, 0x42, 0x42, 0x40, 0x40, 0x44,
            0x44, 0x13, 0x13, 0x11, 0x11, 0x10, 0x26, 0x26, 0x90, 0x90, 0x88, 0x88, 0x89, 0x89, 0x8C,
        },
    };
#if JP_DEBUG_BUILD
    static const char name[] = "勇者の";
#else
    /* "勇者の", and the leftover byte after it */
    static const char name[8] = "\x97" "E\x8E\xD2\x82\xCC\0\x04";
#endif
    s32 i;
    u8 card;

    sprintf(deck->name, name);
    for (i = 0; i < 30; i++) {
        card = starters[color][i];
        if (card < 0x6E) {
            deck->cards[i].type = 0;
            deck->cards[i].id = card;
            ((IntProfile *)PLAYER_PROFILES)->digimonCards[deck->cards[i].id]++;
            deck->cards[i].data = DIGIMON_CARDS + deck->cards[i].id * 0x122;
        } else if (card >= 0x6E && card < 0x99) {
            deck->cards[i].type = 1;
            deck->cards[i].id = card - 0x6E;
            ((IntProfile *)PLAYER_PROFILES)->optionCards[deck->cards[i].id]++;
            deck->cards[i].data = OPTION_CARDS + deck->cards[i].id * 0xD4;
        } else if (card >= 0x99 && card < 0x9F) {
            deck->cards[i].type = 2;
            deck->cards[i].id = card - 0x99;
            ((IntProfile *)PLAYER_PROFILES)->otherCards[deck->cards[i].id]++;
            deck->cards[i].data = DIGIVOLVE_CARDS + deck->cards[i].id * 0x62;
        }
    }
    deck->valid = 1;
}

void INT_startScene3D(void) {
    initScene3D(1);
    addFrameCallback((s32)renderSceneModels);
    spawnTask(0x19, -1, 0, 0x800, runSceneCameraTask, 1);
    spawnTask(0x1B, -1, 0, 0x800, runModelAnimationTask, 1);
}

void INT_endScene3D(void) {
    endTask(0x1B);
    endTask(0x19);
    removeFrameCallback((s32)renderSceneModels);
    unloadAllModels();
    freeHeapBlocksByTag(0x7F);
}

s32 INT_loadModelFromDisc(s32 slot, s32 id, s32 anim, s32 index, s32 loopKey) {
    loadOmdModelFromDisc(slot, id, -1);
    SCENE_3D->modelState[slot] = 1;
    /* loadModelAnimationFile is void, but it leaves loadModelAnimation's
       result in v0, and this caller reads it */
    if (((s32 (*)(s32, s32, s32))loadModelAnimationFile)(slot, anim, index) != 0) {
        startModelAnimation(slot, index, loopKey, 0);
    }
    return 0;
}

void INT_showHeroBoy(void) {
    SCENE_3D->modelState[4] = 1;
    startModelAnimation(4, 0, -2, 0);
    INT_waitAnimationEnd(4, 1, -2);
    INT_waitAnimationEnd(4, 2, 0);
}

void INT_moveChosenStone(void) {
    s32 *x;
    s32 *y;
    u8 done;
    IntState *state = &INT_STATE;
    Model *model = SCENE_3D->models[state->stone];

    x = &model->pos.vx;
    y = &model->pos.vy;
    do {
        waitFrames(1);
        done = 0;
        if (*x > 0x118) {
            *x -= 4;
        } else if (*x < 0x118) {
            *x += 4;
        } else {
            done++;
        }
        if (*y > -0x96) {
            *y -= 2;
        } else if (*y < -0x96) {
            *y += 2;
        } else {
            done++;
        }
    } while (done != 2);
}

#if JP_DEBUG_BUILD
/* the debug animation viewer's model, animation and loop flag, and where
   it puts model 2 */
s32 INT_DEBUG_MODEL = 0;
s32 INT_DEBUG_ANIM = 0;
s32 INT_DEBUG_LOOP = 0;
s16 INT_DEBUG_X = 0;
s16 INT_DEBUG_Y = 0;

extern const char INT_FMT_DEBUG_ANIMATION[];

/* a debug task nothing spawns: the first pad picks a model and an animation
   and plays it, the second moves model 2 */
void INT_debugAnimationTask(void) {
    Model *model;

    func_800184F0(0, 0x10, 1);
    while (1) {
        waitFrames(1);
        if (PAD_STATES[0]->rawRepeat & PAD_RIGHT) {
            INT_DEBUG_MODEL++;
        }
        if (PAD_STATES[0]->rawRepeat & PAD_LEFT) {
            INT_DEBUG_MODEL--;
        }
        if (PAD_STATES[0]->rawRepeat & PAD_UP) {
            INT_DEBUG_ANIM--;
        }
        if (PAD_STATES[0]->rawRepeat & PAD_DOWN) {
            INT_DEBUG_ANIM++;
        }
        if (PAD_STATES[0]->rawRepeat & PAD_START) {
            INT_DEBUG_LOOP = (INT_DEBUG_LOOP + 1) & 1;
        }
        if (PAD_STATES[0]->rawRepeat & PAD_R1) {
            startModelAnimation(INT_DEBUG_MODEL, INT_DEBUG_ANIM, INT_DEBUG_LOOP - 1, 0);
        }
        if (PAD_STATES[1]->rawHeld & PAD_UP) {
            INT_DEBUG_Y -= 10;
        }
        if (PAD_STATES[1]->rawHeld & PAD_DOWN) {
            INT_DEBUG_Y += 10;
        }
        if (PAD_STATES[1]->rawHeld & PAD_RIGHT) {
            INT_DEBUG_X += 10;
        }
        if (PAD_STATES[1]->rawHeld & PAD_LEFT) {
            INT_DEBUG_X -= 10;
        }
        ((Model *)SCENE_3D->models[2])->pos.vx = INT_DEBUG_X;
        ((Model *)SCENE_3D->models[2])->pos.vy = INT_DEBUG_Y;
        sprintf(DEBUG_TEXT_LINES[7], "POSTION X.%d Y.%d\n", INT_DEBUG_X, INT_DEBUG_Y);
        sprintf(DEBUG_TEXT_LINES[5], INT_FMT_DEBUG_ANIMATION, INT_DEBUG_MODEL, INT_DEBUG_ANIM, INT_DEBUG_LOOP);
    }
}

/* the last three bytes are leftovers, not zero padding */
const char INT_FMT_DEBUG_ANIMATION[32] = "POSTION X.%d ANM.%d LOOP.%d\n\0ttt";
#endif
