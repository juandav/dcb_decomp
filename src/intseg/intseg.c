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
void func_8002CF74();
void func_8002DDC0();
void func_8002E538();
void func_8002E5B8();
void func_8002EA60();
s32 func_80044334();
void func_800445FC();
void func_80044758();
void func_80044790();
void D_8002A3E0();
void D_800490B4();
extern s16 CAMERA_TARGET_MODEL;
extern s32 D_8008CD50;
extern void *D_801E469C;
extern void *D_801E46C4;
extern void *D_801E46CC;
extern s32 *D_801E46D0;
extern u8 *OPTION_CARDS;
extern u8 *DIGIVOLVE_CARDS;

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
s8 INT_makeHeroDeck(IntState *state, u8 *deck, u8 color);
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
    "\x82\xA0\x82\xA2\x82\xA4\x82\xA6\x82\xA8\x81@\x82\xED\x81@\x82\xF0\x81@\x82\xF1", /* あいうえお　わ　を　ん */
    "\x82\xA9\x82\xAB\x82\xAD\x82\xAF\x82\xB1\x81@\x82\xAA\x82\xAC\x82\xAE\x82\xB0\x82\xB2", /* かきくけこ　がぎぐげご */
    "\x82\xB3\x82\xB5\x82\xB7\x82\xB9\x82\xBB\x81@\x82\xB4\x82\xB6\x82\xB8\x82\xBA\x82\xBC", /* さしすせそ　ざじずぜぞ */
    "\x82\xBD\x82\xBF\x82\xC2\x82\xC4\x82\xC6\x81@\x82\xBE\x82\xC0\x82\xC3\x82\xC5\x82\xC7", /* たちつてと　だぢづでど */
    "\x82\xC8\x82\xC9\x82\xCA\x82\xCB\x82\xCC\x81@\x82\xCE\x82\xD1\x82\xD4\x82\xD7\x82\xDA", /* なにぬねの　ばびぶべぼ */
    "\x82\xCD\x82\xD0\x82\xD3\x82\xD6\x82\xD9\x81@\x82\xCF\x82\xD2\x82\xD5\x82\xD8\x82\xDB", /* はひふへほ　ぱぴぷぺぽ */
    "\x82\xDC\x82\xDD\x82\xDE\x82\xDF\x82\xE0\x81@\x82\x9F\x82\xA1\x82\xA3\x82\xA5\x82\xA7", /* まみむめも　ぁぃぅぇぉ */
    "\x82\xE2\x81@\x82\xE4\x81@\x82\xE6\x81@\x82\xC1\x82\xE1\x82\xE3\x82\xE5\x81`", /* や　ゆ　よ　っゃゅょ〜 */
    "\x82\xE7\x82\xE8\x82\xE9\x82\xEA\x82\xEB\x81@\x81@\x81@\x81@\x81@\x81@", /* らりるれろ　　　　　　 */
    "\x83" "A\x83" "C\x83" "E\x83G\x83I\x81@\x83\x8F\x81@\x83\x92\x81@\x83\x93", /* アイウエオ　ワ　ヲ　ン */
    "\x83J\x83L\x83N\x83P\x83R\x81@\x83K\x83M\x83O\x83Q\x83S", /* カキクケコ　ガギグゲゴ */
    "\x83T\x83V\x83X\x83Z\x83\\\x81@\x83U\x83W\x83Y\x83[\x83]", /* サシスセソ　ザジズゼゾ */
    "\x83^\x83`\x83" "c\x83" "e\x83g\x81@\x83_\x83" "a\x83" "d\x83" "f\x83h", /* タチツテト　ダヂヅデド */
    "\x83i\x83j\x83k\x83l\x83m\x81@\x83o\x83r\x83u\x83x\x83{", /* ナニヌネノ　バビブベボ */
    "\x83n\x83q\x83t\x83w\x83z\x81@\x83p\x83s\x83v\x83y\x83|", /* ハヒフヘホ　パピプペポ */
    "\x83}\x83~\x83\x80\x83\x81\x83\x82\x81@\x83@\x83" "B\x83" "D\x83" "F\x83H", /* マミムメモ　ァィゥェォ */
    "\x83\x84\x81@\x83\x86\x81@\x83\x88\x81@\x83" "b\x83\x83\x83\x85\x83\x87\x81[", /* ヤ　ユ　ヨ　ッャュョー */
    "\x83\x89\x83\x8A\x83\x8B\x83\x8C\x83\x8D\x81@\x83\x94\x81@\x81@\x81@\x81@", /* ラリルレロ　ヴ　　　　 */
    "\x82`\x82" "a\x82" "b\x82" "c\x82" "d\x81@\x82\x81\x82\x82\x82\x83\x82\x84\x82\x85", /* ＡＢＣＤＥ　ａｂｃｄｅ */
    "\x82" "e\x82" "f\x82g\x82h\x82i\x81@\x82\x86\x82\x87\x82\x88\x82\x89\x82\x8A", /* ＦＧＨＩＪ　ｆｇｈｉｊ */
    "\x82j\x82k\x82l\x82m\x82n\x81@\x82\x8B\x82\x8C\x82\x8D\x82\x8E\x82\x8F", /* ＫＬＭＮＯ　ｋｌｍｎｏ */
    "\x82o\x82p\x82q\x82r\x82s\x81@\x82\x90\x82\x91\x82\x92\x82\x93\x82\x94", /* ＰＱＲＳＴ　ｐｑｒｓｔ */
    "\x82t\x82u\x82v\x82w\x82x\x81@\x82\x95\x82\x96\x82\x97\x82\x98\x82\x99", /* ＵＶＷＸＹ　ｕｖｗｘｙ */
    "\x82y\x81@\x81@\x81@\x81@\x81@\x82\x9A\x81@\x81@\x81@\x81@", /* Ｚ　　　　　ｚ　　　　 */
    "\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x81@", /* 　　　　　　　　　　　 */
    "\x82O\x82P\x82Q\x82R\x82S\x81@\x82T\x82U\x82V\x82W\x82X", /* ０１２３４　５６７８９ */
    "\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x81@", /* 　　　　　　　　　　　 */
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
        "\x97\xC7\x82\xAD\x97\x88\x82\xBD\x82\xCC\x82\xA4\x81I\x83\x8F\x83V\x82\xCD\x81" "A\x83o\x83o\x83\x82\x83\x93\x82\xB6\x82\xE1\x81" "B", /* 良く来たのう！ワシは、ババモンじゃ。 */
        "\x82\xDC\x82\xB8\x82\xCD\x82\xA8\x82\xCA\x82\xB5\x82\xCC\x81m\x96\xBC\x91O\x81n\x82\xC6", /* まずはおぬしの［名前］と */
        "\x81m\x90\xB6\x94N\x8C\x8E\x93\xFA\x81n\x82\xF0\x93\xFC\x97\xCD\x82\xB5\x82\xC4\x82\xAD\x82\xEA\x82\xA2\x81" "B", /* ［生年月日］を入力してくれい。 */
        NULL,
    } },
    { 4, { 0, 1, 0 }, 2, { 0 }, {
        "\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0", /* ああああああああああ */
        "\x82X\x82X\x82X\x82X\x94N\x82X\x82X\x8C\x8E\x82X\x82X\x93\xFA\x90\xB6\x82\xDC\x82\xEA", /* ９９９９年９９月９９日生まれ */
        "\x82\xB1\x82\xEA\x82\xC5\x82\xA2\x82\xA2\x82\xCC\x82\xA9\x82\xA6\x81H", /* これでいいのかえ？ */
        "\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x82x\x82" "d\x82r\x81^\x82m\x82n", /* 　　　　　　　ＹＥＳ／ＮＯ */
    } },
    { 4, { 0, 0, 0 }, 3, { 0 }, {
        "\x82\xE6\x82\xB5\x81" "A\x82\xC6\x82\xB1\x82\xEB\x82\xC5\x82\xA8\x82\xCA\x82\xB5\x81" "B", /* よし、ところでおぬし。 */
        "\x83g\x83\x8C\x81[\x83" "f\x83" "B\x83\x93\x83O\x83J\x81[\x83h\x83Q\x81[\x83\x80\x82\xF0\x92m\x82\xC1\x82\xC4\x82\xA8\x82\xE9\x82\xA9\x81H", /* トレーディングカードゲームを知っておるか？ */
        "\x81@", /* 　 */
        "\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x82x\x82" "d\x82r\x81^\x82m\x82n", /* 　　　　　　　ＹＥＳ／ＮＯ */
    } },
    { 1, { 0, 0, 0 }, 4, { 0 }, {
        "\x82\xBB\x82\xA4\x82\xA9\x82\xBB\x82\xEA\x82\xC8\x82\xE7\x98" "b\x82\xCD\x91\x81\x82\xA2\x81" "B", /* そうかそれなら話は早い。 */
        NULL,
        NULL,
        NULL,
    } },
    { 4, { 0, 0, 0 }, 5, { 0 }, {
        "\x82\xA4\x82\xDE\x81" "A\x83g\x83\x8C\x81[\x83" "f\x83" "B\x83\x93\x83O\x83J\x81[\x83h\x83Q\x81[\x83\x80\x82\xC6\x82\xCD\x81I", /* うむ、トレーディングカードゲームとは！ */
        "\x8D" "D\x82\xAB\x82\xC8\x83J\x81[\x83h\x82\xF0\x8FW\x82\xDF\x82\xBD\x82\xE8\x81" "A\x8C\xF0\x8A\xB7\x82\xB5\x82\xC4\x82\xCD\x8E\xA9\x95\xAA\x82\xBE\x82\xAF", /* 好きなカードを集めたり、交換しては自分だけ */
        "\x82\xCC\x81m\x83" "f\x83" "b\x83N\x81n\x81i\x83J\x81[\x83h\x82\xCC\x8FW\x82\xDC\x82\xE8\x82\xCC\x8E\x96\x82\xB6\x82\xE1\x82\xC8\x81j\x82\xF0", /* の［デック］（カードの集まりの事じゃな）を */
        "\x82\xC2\x82\xAD\x82\xE8\x81" "A\x91\xCE\x90\xED\x82\xB5\x82\xC4\x97V\x82\xD4\x83Q\x81[\x83\x80\x82\xCC\x8E\x96\x82\xB6\x82\xE1\x81" "B", /* つくり、対戦して遊ぶゲームの事じゃ。 */
    } },
    { 4, { 0, 0, 0 }, 6, { 0 }, {
        "\x82\xB1\x82\xCC\x81m\x83Q\x81[\x83\x80\x83\\\x83t\x83g\x81n\x82\xCD\x82\xC8\x81" "A\x83" "f\x83W\x83\x82\x83\x93\x92" "B\x82\xAA", /* この［ゲームソフト］はな、デジモン達が */
        "\x83" "f\x83W\x83\x82\x83\x93\x83J\x81[\x83h\x82\xC5\x93\xAC\x82\xA4\x95s\x8Ev\x8B" "c\x82\xC8\x90\xA2\x8A" "E\x82\xC8\x82\xCC\x82\xB6\x82\xE1\x81" "B", /* デジモンカードで闘う不思議な世界なのじゃ。 */
        "\x93\xCB\x82\xC1\x8D\x9E\x82\xDD\x82\xCD\x82\xA2\x82\xE7\x82\xF1\x82\xBC\x81" "A\x82\xC6\x82\xC9\x82\xA9\x82\xAD\x95s\x8Ev\x8B" "c\x82\xC8\x95s\x8Ev\x8B" "c", /* 突っ込みはいらんぞ、とにかく不思議な不思議 */
        "\x82\xC8\x90\xA2\x8A" "E\x82\xC8\x82\xCC\x82\xB6\x82\xE1\x81" "B", /* な世界なのじゃ。 */
    } },
    { 4, { 0, 2, 0 }, 7, { 0 }, {
        "\x82\xC6\x82\xC4\x82\xE0\x95\xBD\x98" "a\x82\xC8\x90\xA2\x8A" "E\x82\xB6\x82\xE1\x82\xC1\x82\xBD\x81" "c\x81" "c", /* とても平和な世界じゃった…… */
        "\x83" "f\x83W\x83\x82\x83\x93\x92" "B\x82\xE0\x82\xDD\x82\xC8\x92\x87\x97\xC7\x82\xAD\x95\xE9\x82\xE7\x82\xB5\x82\xC4\x82\xA8\x82\xC1\x82\xC4\x82\xCC\x82\xA4\x81" "B", /* デジモン達もみな仲良く暮らしておってのう。 */
        "\x82\xB6\x82\xE1\x82\xAA\x81" "A\x82\xA2\x82\xDC\x82\xB1\x82\xCC\x90\xA2\x8A" "E\x82\xCD\x88\xEA\x93]\x82\xB5\x82\xC4\x96\xC5\x96S\x82\xCC\x8A\xEB\x8B@\x82\xC9", /* じゃが、いまこの世界は一転して滅亡の危機に */
        "\x82\xDD\x82\xDC\x82\xED\x82\xEA\x82\xC4\x82\xA8\x82\xE9\x81" "c\x81" "c", /* みまわれておる…… */
    } },
    { 3, { 0, 0, 0 }, 8, { 0 }, {
        "\x82\xAD\x82\xED\x82\xB5\x82\xAD\x82\xCD\x8C\xE3\x82\xC5\x98" "b\x82\xB7\x82\xAA\x81" "A\x82\xAB\x82\xE5\x81[\x82\xEA\x82\xC2\x82\xC8\x8E\x96\x8C\x8F\x82\xAA", /* くわしくは後で話すが、きょーれつな事件が */
        "\x8BN\x82\xB1\x82\xC1\x82\xC4\x82\xCC\x81" "B\x90" "F\x81X\x82\xC6\x83\x8F\x83V\x82\xE7\x82\xE0\x8E\xE8\x82\xF0\x82\xC2\x82\xAD\x82\xB5\x82\xBD\x82\xCC", /* 起こっての。色々とワシらも手をつくしたの */
        "\x82\xB6\x82\xE1\x82\xAA\x81" "A\x96\xA2\x82\xBE\x82\xC9\x89\xF0\x8C\x88\x82\xC5\x82\xAB\x82\xF1\x81" "B", /* じゃが、未だに解決できん。 */
        NULL,
    } },
    { 4, { 3, 4, 0 }, 9, { 0 }, {
        "\x82\xBB\x82\xB1\x82\xC5\x82\xB6\x82\xE1\x81I", /* そこでじゃ！ */
        "\x82\xA8\x82\xCA\x82\xB5\x82\xC9\x97\x88\x82\xC4\x82\xE0\x82\xE7\x82\xC1\x82\xBD\x82\xCC\x82\xCD\x82\xD9\x82\xA9\x82\xC5\x82\xE0\x82\xC8\x82\xA2\x81" "B", /* おぬしに来てもらったのはほかでもない。 */
        "\x91I\x82\xCE\x82\xEA\x82\xBD\x82\xE9\x93`\x90\xE0\x82\xCC\x82\xC8\x82\xF1\x82\xBD\x82\xE7\x82\xC6\x82\xA9\x81" "c\x81" "c", /* 選ばれたる伝説のなんたらとか…… */
        "\x88\xD9\x90\xA2\x8A" "E\x82\xA9\x82\xE7\x8C\xC4\x82\xCE\x82\xEA\x82\xBD\x8C\xF5\x82\xCC\x82\xA9\x82\xF1\x82\xBD\x82\xE7\x82\xC6\x82\xA9\x81" "c\x81" "c", /* 異世界から呼ばれた光のかんたらとか…… */
    } },
    { 3, { 5, 6, 0 }, 10, { 0 }, {
        "\x82\xD9\x82\xD0\x82\xE5\x82\xD9\x82\xD0\x82\xE5\x82\xD9\x82\xD0\x82\xE5\x81I\x82\xD9\x82\xEA\x81" "A\x82\xE6\x82\xAD\x82\xA0\x82\xE9\x82\xB6\x82\xE1\x82\xEB\x81H", /* ほひょほひょほひょ！ほれ、よくあるじゃろ？ */
        "\x82\xC2\x82\xDC\x82\xE8\x82\xC8\x82\xF1\x82\xB6\x82\xE1\x81" "B", /* つまりなんじゃ。 */
        "\x82\xA8\x82\xCA\x82\xB5\x82\xCD\x82\xB1\x82\xCC\x90\xA2\x8A" "E\x82\xF0\x8B~\x82\xC1\x82\xC4\x82\xB5\x82\xDC\x82\xA4\x82\xCC\x82\xB6\x82\xE1\x81I", /* おぬしはこの世界を救ってしまうのじゃ！ */
        NULL,
    } },
    { 1, { 0, 0, 0 }, 11, { 0 }, {
        "\x82\xE6\x82\xA2\x82\xA9\x81" "A\x82\xB1\x82\xCC\x90\xA2\x8A" "E\x82\xC5\x82\xCD\x82\xA8\x82\xCA\x82\xB5\x82\xCD\x82\xB1\x82\xCC\x8F\xAD\x94N\x82\xB6\x82\xE1\x81" "B", /* よいか、この世界ではおぬしはこの少年じゃ。 */
        NULL,
        NULL,
        NULL,
    } },
    { 2, { 0, 0, 0 }, 12, { 0 }, {
        "\x82\xBD\x82\xC6\x82\xA6\x82\xA8\x82\xCA\x82\xB5\x82\xAA\x81" "A\x82\xA8\x82\xC8\x82\xB2\x82\xB6\x82\xE1\x82\xEB\x81[\x82\xC6\x81" "A\x82\xB6\x81[\x82\xB3\x82\xF1", /* たとえおぬしが、おなごじゃろーと、じーさん */
        "\x82\xB6\x82\xE1\x82\xEB\x81[\x82\xC6\x81" "A\x82\xB1\x82\xCC\x8F\xAD\x94N\x82\xCC\x90g\x91\xCC\x82\xF0\x8Eg\x82\xC1\x82\xC4\x82\xE0\x82\xE7\x82\xA4\x81" "B", /* じゃろーと、この少年の身体を使ってもらう。 */
        NULL,
        NULL,
    } },
    { 4, { 4, 0, 0 }, 13, { 0 }, {
        "\x8E\x9E\x8A\xD4\x82\xAA\x96\xB3\x82\xAD\x82\xC4\x82\xCC\x81" "A\x95\xCA\x82\xCC\x83}\x83g\x83\x8A\x83N\x83X\x82\xAA\x97p\x88\xD3\x82\xC5\x82\xAB\x82\xF1", /* 時間が無くての、別のマトリクスが用意できん */
        "\x82\xB6\x82\xE1\x82\xC1\x82\xBD\x81" "B\x82\xB5\x82\xA9\x82\xB5\x82\xC8\x81" "A\x97R\x8F\x8F\x82\xA0\x82\xE9\x90g\x91\xCC\x82\xB6\x82\xE1\x82\xBC\x82\xA6\x81" "B", /* じゃった。しかしな、由緒ある身体じゃぞえ。 */
        "\x82\xB1\x82\xCC\x93\x87\x82\xCC\x93`\x90\xE0\x82\xCC\x97" "E\x8E\xD2\x82\xC5\x82\xC8\x81" "B\x98r\x97\xCD\x82\xCD\x96\xB3\x82\xA2\x82\xAA\x92m\x8C" "b\x82\xC6", /* この島の伝説の勇者でな。腕力は無いが知恵と */
        "\x97" "E\x8B" "C\x82\xC5\x82\xE0\x82\xC1\x82\xC4\x82\xB1\x82\xCC\x8A\xEB\x8B@\x82\xF0\x89\xF0\x8C\x88\x82\xB7\x82\xE9\x82\xCC\x82\xB6\x82\xE1\x81I", /* 勇気でもってこの危機を解決するのじゃ！ */
    } },
    { 2, { 0, 0, 0 }, 14, { 0 }, {
        "\x82\xA8\x82\xA8\x82\xBB\x82\xA4\x82\xB6\x82\xE1\x81" "A\x82\xB1\x82\xCC\x90\xA2\x8A" "E\x82\xC5\x82\xCC\x82\xA8\x82\xCA\x82\xB5\x82\xCC\x8C\xC4\x82\xD1\x96\xBC\x82\xF0", /* おおそうじゃ、この世界でのおぬしの呼び名を */
        "\x8C\x88\x82\xDF\x82\xC4\x82\xE0\x82\xE7\x82\xA8\x82\xA4\x82\xA9\x82\xCC\x81" "B", /* 決めてもらおうかの。 */
        NULL,
        NULL,
    } },
    { 4, { 1, 0, 0 }, 15, { 0 }, {
        "\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0\x82\xA0", /* ああああああああああ */
        "\x82\xB1\x82\xEA\x82\xC5\x82\xA2\x82\xA2\x82\xCC\x82\xA9\x82\xA6\x81H", /* これでいいのかえ？ */
        "\x81@", /* 　 */
        "\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x82x\x82" "d\x82r\x81^\x82m\x82n", /* 　　　　　　　ＹＥＳ／ＮＯ */
    } },
    { 4, { 1, 0, 0 }, 16, { 0 }, {
        "\x82\xE6\x82\xB5\x81I\x82\xB1\x82\xEA\x82\xF0\x8C\xA9\x82\xE6\x81" "A\x82\xB1\x82\xB1\x82\xC9\x82R\x90" "F\x82\xCC\x90\xCE\x82\xAA\x82\xA0\x82\xE9\x81" "B", /* よし！これを見よ、ここに３色の石がある。 */
        "\x82\xB1\x82\xEA\x82\xCD\x82P\x82O\x82O\x82O\x94N\x82\xCC\x90\xCC\x82\xC9\x88\xB3\x8Fk\x82\xB3\x82\xEA\x82\xBD\x82\xC6\x93`\x82\xED\x82\xE9", /* これは１０００年の昔に圧縮されたと伝わる */
        "\x82\xA8\x82\xCA\x82\xB5\x90\xEA\x97p\x82\xCC\x83" "f\x83" "b\x83N\x81" "c\x81" "c\x81m\x97" "E\x8E\xD2\x82\xCC\x83" "f\x83" "b\x83N\x81n", /* おぬし専用のデック……［勇者のデック］ */
        "\x82\xB6\x82\xE1\x81I", /* じゃ！ */
    } },
    { 2, { 0, 0, 0 }, 0, { 0 }, {
        "\x91S\x95\x94\x82\xE2\x82\xE8\x82\xBD\x82\xA2\x82\xCC\x82\xB6\x82\xE1\x82\xAA\x81" "A\x8D\xA1\x82\xCC\x83\x8F\x83V\x82\xCC\x97\xCD\x82\xC5\x82\xCD\x82\xB1\x82\xCC", /* 全部やりたいのじゃが、今のワシの力ではこの */
        "\x93\xE0\x82\xCC\x82P\x8C\xC2\x82\xB5\x82\xA9\x89\xF0\x93\x80\x82\xC5\x82\xAB\x82\xF1\x81I", /* 内の１個しか解凍できん！ */
        NULL,
        NULL,
    } },
    { 1, { 0, 0, 0 }, 18, { 0 }, {
        "\x82\xC7\x82\xEA\x82\xA9\x82P\x82\xC2\x82\xF0\x82\xA6\x82\xE7\x82\xF1\x82\xC5\x82\xAD\x82\xEA\x82\xA2\x81" "B", /* どれか１つをえらんでくれい。 */
        NULL,
        NULL,
        NULL,
    } },
    { 4, { 0, 0, 0 }, 0, { 0 }, {
        "\x8D\x82\x82\xA2\x8DU\x8C\x82\x97\xCD\x82\xC5\x91\x8A\x8E\xE8\x82\xF0\x82\xBD\x82\xBD\x82\xAB\x88\xB3\x93|\x82\xB7\x82\xE9", /* 高い攻撃力で相手をたたき圧倒する */
        "\x91O\x82\xCC\x82\xDF\x82\xE8\x82\xC8\x83" "f\x83" "b\x83N\x82\xB6\x82\xE1\x81I", /* 前のめりなデックじゃ！ */
        "\x83{\x83^\x83\x93\x82\xCC\x8Eg\x82\xA2\x95\xAA\x82\xAF\x82\xAA", /* ボタンの使い分けが */
        "\x83|\x83" "C\x83\x93\x83g\x82\xC9\x82\xC8\x82\xE9\x82\xC6\x82\xAB\x82\xAD\x82\xBC\x82\xA6\x81" "B", /* ポイントになるときくぞえ。 */
    } },
    { 4, { 0, 0, 0 }, 20, { 0 }, {
        "\x82\xB1\x82\xEA\x82\xC5\x82\xA2\x82\xA2\x82\xCC\x82\xA9\x82\xA6\x81H", /* これでいいのかえ？ */
        "\x81@", /* 　 */
        "\x81@", /* 　 */
        "\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x82x\x82" "d\x82r\x81^\x82m\x82n", /* 　　　　　　　ＹＥＳ／ＮＯ */
    } },
    { 1, { 0, 0, 0 }, 0, { 0 }, {
        "\x82\xE6\x82\xB5\x81I\x90\xD4\x82\xA2\x90\xCE\x82\xB6\x82\xE1\x82\xC8\x81" "B", /* よし！赤い石じゃな。 */
        NULL,
        NULL,
        NULL,
    } },
    { 1, { 0, 0, 0 }, 22, { 0 }, {
        "\x82\xAB\x82\xA6\x82\xA5\x82\xA5\x82\xA5\x82\xA5\x82\xC1\x82\xC2\x81I", /* きえぇぇぇぇっつ！ */
        NULL,
        NULL,
        NULL,
    } },
    { 4, { 0, 0, 0 }, 0, { 0 }, {
        "\x82\xA9\x82\xBD\x82\xA2\x8E\xE7\x82\xE8\x82\xC5\x82\xB6\x82\xC1\x82\xAD\x82\xE8\x82\xC6\x91\x8A\x8E\xE8\x82\xC6\x93\xAC\x82\xA4", /* かたい守りでじっくりと相手と闘う */
        "\x82\xE0\x82\xEB\x82\xB3\x82\xCC\x96\xB3\x82\xA2\x83" "f\x83" "b\x83N\x82\xB6\x82\xE1\x81I", /* もろさの無いデックじゃ！ */
        "\x82\xA0\x82\xB9\x82\xE7\x82\xB8\x82\xC9\x90i\x89\xBB\x82\xB3\x82\xB9\x82\xC4\x82\xA9\x82\xE7\x94\xBD\x8C\x82\x82\xC9\x82\xA4\x82\xC2\x82\xEA\x82\xCE", /* あせらずに進化させてから反撃にうつれば */
        "\x88\xEA\x8B" "C\x82\xC9\x8B\xAD\x82\xB3\x82\xF0\x94\xAD\x8A\xF6\x82\xB7\x82\xE9\x82\xC6\x8C\xBE\x82\xED\x82\xEA\x82\xC4\x82\xA8\x82\xE9\x82\xBC\x82\xA6\x81" "B", /* 一気に強さを発揮すると言われておるぞえ。 */
    } },
    { 4, { 0, 0, 0 }, 20, { 0 }, {
        "\x82\xB1\x82\xEA\x82\xC5\x82\xA2\x82\xA2\x82\xCC\x82\xA9\x82\xA6\x81H", /* これでいいのかえ？ */
        "\x81@", /* 　 */
        "\x81@", /* 　 */
        "\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x82x\x82" "d\x82r\x81^\x82m\x82n", /* 　　　　　　　ＹＥＳ／ＮＯ */
    } },
    { 1, { 0, 0, 0 }, 0, { 0 }, {
        "\x82\xE6\x82\xB5\x81I\x90\xC2\x82\xA2\x90\xCE\x82\xB6\x82\xE1\x82\xC8\x81" "B", /* よし！青い石じゃな。 */
        NULL,
        NULL,
        NULL,
    } },
    { 1, { 0, 0, 0 }, 22, { 0 }, {
        "\x82\xAB\x82\xA6\x82\xA5\x82\xA5\x82\xA5\x82\xA5\x82\xC1\x82\xC2\x81I", /* きえぇぇぇぇっつ！ */
        NULL,
        NULL,
        NULL,
    } },
    { 4, { 0, 0, 0 }, 0, { 0 }, {
        "\x8DU\x81" "E\x96h\x81" "E\x91\xAC\x82\xCC\x83o\x83\x89\x83\x93\x83X\x82\xC9\x82\xB7\x82\xAE\x82\xEA\x82\xE9", /* 攻・防・速のバランスにすぐれる */
        "\x83I\x81[\x83\x8B\x83\x89\x83" "E\x83\x93\x83_\x81[\x83" "f\x83" "b\x83N\x82\xB6\x82\xE1\x81I", /* オールラウンダーデックじゃ！ */
        "\x82\xB3\x82\xDC\x82\xB4\x82\xDC\x82\xC8\x83p\x83^\x81[\x83\x93\x82\xC5\x91\x8A\x8E\xE8\x82\xCC\x82\xA4\x82\xE7\x82\xF0\x82\xC6\x82\xE9\x8E\x96\x82\xAA", /* さまざまなパターンで相手のうらをとる事が */
        "\x89\xC2\x94\\\x82\xC6\x82\xAB\x82\xAD\x82\xBC\x82\xA6\x81" "B", /* 可能ときくぞえ。 */
    } },
    { 4, { 0, 0, 0 }, 20, { 0 }, {
        "\x82\xB1\x82\xEA\x82\xC5\x82\xA2\x82\xA2\x82\xCC\x82\xA9\x82\xA6\x81H", /* これでいいのかえ？ */
        "\x81@", /* 　 */
        "\x81@", /* 　 */
        "\x81@\x81@\x81@\x81@\x81@\x81@\x81@\x82x\x82" "d\x82r\x81^\x82m\x82n", /* 　　　　　　　ＹＥＳ／ＮＯ */
    } },
    { 1, { 0, 0, 0 }, 0, { 0 }, {
        "\x82\xE6\x82\xB5\x81I\x97\xCE\x82\xCC\x90\xCE\x82\xB6\x82\xE1\x82\xC8\x81" "B", /* よし！緑の石じゃな。 */
        NULL,
        NULL,
        NULL,
    } },
    { 1, { 0, 0, 0 }, 22, { 0 }, {
        "\x82\xAB\x82\xA6\x82\xA5\x82\xA5\x82\xA5\x82\xA5\x82\xC1\x82\xC2\x81I", /* きえぇぇぇぇっつ！ */
        NULL,
        NULL,
        NULL,
    } },
    { 4, { 0, 7, 0 }, 31, { 0 }, {
        "\x82\xC5\x82\xCD\x81" "A\x82\xB1\x82\xCC\x83" "f\x83" "b\x83N\x82\xCD\x82\xA8\x82\xCA\x82\xB5\x82\xCC\x95\xA8\x82\xB6\x82\xE1\x81" "B", /* では、このデックはおぬしの物じゃ。 */
        "\x81" "c\x81" "c\x82\xBB\x82\xEB\x82\xBB\x82\xEB\x8E\x9E\x8A\xD4\x82\xB6\x82\xE1\x82\xC8\x81" "B", /* ……そろそろ時間じゃな。 */
        "\x82\xA2\x82\xDC\x8F\xAD\x82\xB5\x82\xC5\x82\xA8\x82\xCA\x82\xB5\x82\xE0\x82\xB1\x82\xBF\x82\xE7\x82\xCC\x90\xA2\x8A" "E\x82\xC9", /* いま少しでおぬしもこちらの世界に */
        "\x8D~\x82\xE8\x82\xE7\x82\xEA\x82\xE9\x81" "B", /* 降りられる。 */
    } },
    { 3, { 0, 0, 8 }, 32, { 0 }, {
        "\x82\xB3\x82\xE7\x82\xC9\x82\xAD\x82\xED\x82\xB5\x82\xA2\x98" "b\x82\xF0\x82\xB7\x82\xE9\x82\xE4\x82\xA6\x81" "A", /* さらにくわしい話をするゆえ、 */
        "\x82\xB1\x82\xBF\x82\xE7\x82\xC9\x8D~\x82\xE8\x82\xB5\x82\xBE\x82\xA2\x83\x8F\x83V\x82\xCC\x89\xC6\x82\xDC\x82\xC5\x97\x88\x82\xC4\x82\xAD\x82\xEA\x82\xA2\x81" "B", /* こちらに降りしだいワシの家まで来てくれい。 */
        "\x82\xC5\x82\xCD\x81" "A\x82\xBD\x82\xCC\x82\xF1\x82\xBE\x82\xBC\x82\xA6\x81m\x97" "E\x8E\xD2\x93" "a\x81n\x81I", /* では、たのんだぞえ［勇者殿］！ */
        NULL,
    } },
};

void INT_uploadTimFile(s32 path) {
    s32 data;

    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
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
    spawnTask(0, -1, 0, 0x800, INT_introTask, 0, 0, 0, 0);
}

void INT_introTask(void) {
    INT_initIntroScene(&INT_STATE);
    stopMusic();
    while (1) {
        waitFrames(FRAME_INTERVAL);
        D_801E469C = &INT_PROMPT_BLINK;
        func_8002E5B8(&INT_STATE, 7, 3);
    }
}

s32 INT_initIntroScene(IntState *state) {
    Rect16 rect = { 0x380, 0, 0x40, 0x100 };
    s16 i;
    s32 pak;
    Scene3D *scene;

    ClearImage(&rect, 0, 0, 0);
    if (D_8008CD50 == 0) {
        func_8002EA60(0x40);
    }
    i = 0;
    pak = loadFileTagged((s32 *)"A:\\NIS.PAK", getCurrentTaskId(), 0x7F);
    uploadTimList(findPakChunk((Chunk *)pak, 5, 200));
    resetPlayerData();
    func_8002CF74();
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
    state->cursor = func_80044334(1, 6, 6, 4, 1);
    state->typedChars = 0;
    state->typedLines = 0;
    state->unkBA = 0;
    state->blinkTimer = 0;
    state->typedLines = 1;
    state->typedChars = 0;
    state->page = 0;
    stopScreenFade();
    func_8002E538(7, 3);
    D_801E46C4 = INT_openBabamonWindow;
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
    *D_801E46D0 = 0;
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
    *D_801E46D0 = 0;
    return 0;
}

s32 INT_runPageAction(IntState *state) {
    u8 action = INT_PAGES[state->page - 1].action;
    s16 i;

    if (action == 2 || action == 3 || action == 0xF || action == 0x14) {
        state->keyX = 0;
        state->keyY = 0;
        state->unkC0 = 0;
        *D_801E46D0 = (s32)INT_runYesNoPrompt;
        return 0;
    } else if (action == 0x12) {
    } else if (action == 0x16) {
        startModelAnimation(3, 0xF, -2, 0);
        waitFrames(100);
        startModelAnimation(state->stone, 4, -2, 0);
        for (i = 0; i < 5; i++) {
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 1, 0x40, 0);
            waitFrames(5);
            setScreenFadeParams(1, 1, 0x40);
            waitFrames(5);
        }
        *D_801E46D0 = (s32)INT_waitForCircle;
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
            D_801E46CC = INT_showNextPage;
            *D_801E46D0 = (s32)INT_runStoneChoice;
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
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 0x10, 0);
            waitFrames(30);
            func_80044758(state->cursor);
            closeKanjiPage(0xF);
            INT_endScene3D();
            playMusic(0, 4, 0x7F);
            spawnTask(0, -1, 0, 0x1000, D_800490B4, 0, 0, 0, 0);
            exitTask();
            return 0;
        }
    }
    if (++state->blinkTimer & 0x10) {
        func_8002DDC0(D_801E469C);
    }
    return 0;
}

void INT_advancePage(IntState *state) {
    *D_801E46D0 = 0;
    D_801E46CC = INT_showNextPage;
    INT_TEXT_WINDOW.draw = INT_drawTypingLines;
}

void INT_openNameEntry(IntState *state, u8 withDate) {
    Rect16 rect;
    s16 i;

    INT_WINDOWS[0][1] = 4;
    waitFrames(30);
    spawnTask(0, -1, 0, 0x800, D_8002A3E0, &INT_NAME_FIELDS_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[1] = (u8 *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, D_8002A3E0, &INT_BIRTHDAY_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[2] = (u8 *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, D_8002A3E0, &INT_KEYBOARD_WINDOW, getCurrentTaskId(), 0, 0);
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
    uploadKanjiString("\x82\xA9\x82\xC8", &rect); /* かな */
    rect.x = 0x3E4;
    rect.y = 0xE;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("\x83J\x83i", &rect); /* カナ */
    rect.x = 0x3E4;
    rect.y = 0x1C;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("\x89p\x90\x94", &rect); /* 英数 */
    rect.x = 0x3E4;
    rect.y = 0x62;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("\x96\xDF\x82\xE9", &rect); /* 戻る */
    rect.x = 0x3E4;
    rect.y = 0x70;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("\x8C\x88\x92\xE8", &rect); /* 決定 */
    rect.x = 0x3A4;
    rect.y = 0x62;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("\x96\xDF\x82\xE9", &rect); /* 戻る */
    rect.x = 0x3A4;
    rect.y = 0x70;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("\x8C\x88\x92\xE8", &rect); /* 決定 */
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
        uploadKanjiString("\x81Q\x81\xCB\x81\x9F\x81t", &rect);
        rect.x = 0x3C0;
        rect.y = 0xE8;
        rect.w = 0;
        rect.h = 0;
        /* 　姓：　　　　　　名： */
        uploadKanjiString("\x81@\x90\xA9\x81" "F\x81@\x81@\x81@\x81@\x81@\x81@\x96\xBC\x81" "F", &rect);
        rect.x = 0x3C0;
        rect.y = 0xF4;
        rect.w = 0;
        rect.h = 0;
        /* 　生年月日：　　　　年　　月　　日 */
        uploadKanjiString("\x81@\x90\xB6\x94N\x8C\x8E\x93\xFA\x81" "F\x81@\x81@\x81@\x81@\x94N\x81@\x81@\x8C\x8E\x81@\x81@\x93\xFA", &rect);
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
    *D_801E46D0 = (s32)INT_runNameEntry;
}

void INT_openNicknameEntry(IntState *state, u8 withDate) {
    Rect16 rect;

    INT_WINDOWS[0][1] = 4;
    waitFrames(30);
    spawnTask(0, -1, 0, 0x800, D_8002A3E0, &INT_NAME_FIELDS_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[1] = (u8 *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, D_8002A3E0, &INT_KEYBOARD_WINDOW, getCurrentTaskId(), 0, 0);
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
    uploadKanjiString("\x82\xA9\x82\xC8", &rect); /* かな */
    rect.x = 0x3E4;
    rect.y = 0xE;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("\x83J\x83i", &rect); /* カナ */
    rect.x = 0x3E4;
    rect.y = 0x1C;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("\x89p\x90\x94", &rect); /* 英数 */
    rect.x = 0x3E4;
    rect.y = 0x62;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("\x96\xDF\x82\xE9", &rect); /* 戻る */
    rect.x = 0x3E4;
    rect.y = 0x70;
    rect.w = 0;
    rect.h = 0;
    uploadKanjiString("\x8C\x88\x92\xE8", &rect); /* 決定 */
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
        uploadKanjiString("\x81Q\x81\xCB\x81\x9F\x81t", &rect);
        rect.x = 0x3C0;
        rect.y = 0xE8;
        rect.w = 0;
        rect.h = 0;
        uploadKanjiString("\x81@\x96\xBC\x91O\x81@\x81@\x81" "F", &rect); /* 　名前　　： */
        rect.x = 0x3C0;
        rect.y = 0xF4;
        rect.w = 0;
        rect.h = 0;
        /* 　生年月日：　　　　年　　月　　日 */
        uploadKanjiString("\x81@\x90\xB6\x94N\x8C\x8E\x93\xFA\x81" "F\x81@\x81@\x81@\x81@\x94N\x81@\x81@\x8C\x8E\x81@\x81@\x93\xFA", &rect);
        bzero((Scene3D *)state->fields[2], 14);
        bzero((Scene3D *)state->values[2], 7);
        state->lengths[2] = 0;
        state->keyX = 0;
        state->keyY = 0;
        state->unkC0 = 0;
    }
    waitFrames(36);
    *D_801E46D0 = (s32)INT_runNameEntry;
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
    spawnTask(0, -1, 0, 0x800, D_8002A3E0, &INT_SPEAKER_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[4] = (u8 *)waitFrames(0x7FFFFFFF);
    waitFrames(10);
    uploadKanjiString("\x83o", &rect); /* バ */
    waitFrames(4);
    uploadKanjiString("\x83o", &rect); /* バ */
    waitFrames(4);
    uploadKanjiString("\x83\x82", &rect); /* モ */
    waitFrames(4);
    uploadKanjiString("\x83\x93", &rect); /* ン */
    INT_waitAnimationEnd(3, 3, -2);
    spawnTask(0, -1, 0, 0x800, D_8002A3E0, &INT_TEXT_WINDOW, getCurrentTaskId(), 0, 0);
    INT_WINDOWS[0] = (u8 *)waitFrames(0x7FFFFFFF);
    *D_801E46D0 = (s32)INT_showNextPage;
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

/* only the registers of the state and the loop counter differ (s1 and s2
   swapped): GCC allocates the counter first */
INCLUDE_ASM("intseg/nonmatchings/intseg", INT_runYesNoPrompt);

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
    }
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
        func_8002DDC0(D_801E469C);
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
                *D_801E46D0 = (s32)INT_runYearEntry;
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
                        *D_801E46D0 = (s32)INT_runYearEntry;
                        return 0;
                    }
                } else if (field == 2) {
                    if (kinds[0] != 0 && INT_isFieldBlank(name) == 0) {
                        strcpy(INT_PAGES[14].lines[0], name);
                        INT_WINDOWS[1][1] = 4;
                        INT_WINDOWS[3][1] = 4;
                        waitFrames(30);
                        INT_advancePage(state);
                        spawnTask(0, -1, 0, 0x800, D_8002A3E0, &INT_TEXT_WINDOW, getCurrentTaskId(), 0, 0);
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
    if (*x == 12) {
        func_800445FC(state->cursor, 12, 6, 4);
        func_80044790(state->cursor, *x * 12 + 0x83, *y * 14 + 0x5C);
    } else {
        func_800445FC(state->cursor, 6, 6, 4);
        func_80044790(state->cursor, *x * 12 + 0x7D, *y * 14 + 0x5C);
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
        "\x82O", "\x82P", "\x82Q", "\x82R", "\x82S", /* ０１２３４ */
        "\x82T", "\x82U", "\x82V", "\x82W", "\x82X", /* ５６７８９ */
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
            *D_801E46D0 = (s32)INT_runMonthEntry;
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_L1) {
        playSoundEffect(0);
        *x = 0;
        *y = 0;
        state->field = 1;
        INT_KEYBOARD_WINDOW.draw = INT_drawKeyboard;
        *D_801E46D0 = (s32)INT_runNameEntry;
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
                    *D_801E46D0 = (s32)INT_runMonthEntry;
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
    if (*x == 12) {
        func_800445FC(state->cursor, 12, 6, 4);
        func_80044790(state->cursor, *x * 12 + 0x83, *y * 14 + 0x5C);
    } else {
        func_800445FC(state->cursor, 6, 6, 4);
        func_80044790(state->cursor, *x * 12 + 0x7D, *y * 14 + 0x5C);
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
            *D_801E46D0 = (s32)INT_runDayEntry;
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_L1) {
        playSoundEffect(0);
        *x = 0;
        *y = 0;
        state->field = 3;
        *D_801E46D0 = (s32)INT_runYearEntry;
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
                    *D_801E46D0 = (s32)INT_runDayEntry;
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
    if (*x == 12) {
        func_800445FC(state->cursor, 12, 6, 4);
        func_80044790(state->cursor, *x * 12 + 0x83, *y * 14 + 0x5C);
    } else {
        func_800445FC(state->cursor, 6, 6, 4);
        func_80044790(state->cursor, *x * 12 + 0x7D, *y * 14 + 0x5C);
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
        *D_801E46D0 = (s32)INT_runMonthEntry;
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
                    spawnTask(0, -1, 0, 0x800, D_8002A3E0, &INT_TEXT_WINDOW, getCurrentTaskId(), 0, 0);
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
    if (*x == 12) {
        func_800445FC(state->cursor, 12, 6, 4);
        func_80044790(state->cursor, *x * 12 + 0x83, *y * 14 + 0x5C);
    } else {
        func_800445FC(state->cursor, 6, 6, 4);
        func_80044790(state->cursor, *x * 12 + 0x7D, *y * 14 + 0x5C);
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
    uploadKanjiString("\x81\x40", &rect);
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
    func_8002DDC0(blink);
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
    sprintf(INT_PAGES[1].lines[0], "%s\x81@%s", state->fields[0], state->fields[1]); /* %s　%s */
    /* %s年%s月%s日生まれ */
    sprintf(INT_PAGES[1].lines[1], "%s\x94N%s\x8C\x8E%s\x93\xFA\x90\xB6\x82\xDC\x82\xEA", text[0], text[1], text[2]);
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
                D_801E46C4 = INT_runLineAction;
            } else {
                state->typedLines = 1;
                INT_TEXT_WINDOW.draw = INT_drawPageLines;
                D_801E46C4 = INT_runPageAction;
            }
            return;
        }
    }
    if (state->typedLines > state->lineCount) {
        state->typedChars = 0;
        state->typedLines = 1;
        INT_TEXT_WINDOW.draw = INT_drawPageLines;
        D_801E46C4 = INT_runPageAction;
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

/* the C doesn't lay out the two rarity tables' cases as the original (it
   moved their bodies after the third deck's cards) nor allocate its registers
   the same; its rodata comes with it */
INCLUDE_RODATA("intseg/nonmatchings/intseg", D_801EB2AC);

INCLUDE_ASM("intseg/nonmatchings/intseg", INT_makeHeroDeck);

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
    /* "勇者の", and the leftover byte after it */
    static const char name[8] = "\x97" "E\x8E\xD2\x82\xCC\0\x04";
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
