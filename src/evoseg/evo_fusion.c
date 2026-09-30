#include "common.h"
#include "game.h"
#include "dcb/evo_fusion.h"
#include "dcb/scene3d.h"
#include "dcb/model_anim.h"
#include "dcb/frame_callback.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/card_db.h"
#include "dcb/window.h"
#include "dcb/prim_util.h"
#include "dcb/text.h"
#include "dcb/fade.h"
#include "dcb/loader.h"
#include "dcb/script.h"
#include "dcb/vram_upload.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/partner_level.h"
#include "dcb/sound_play.h"
#include "dcb/prim.h"
#include "dcb/scroll_bg.h"
#include "dcb/menu.h"
#include "dcb/sort.h"
#include "dcb/game_flow.h"
#include "dcb/evo_cutscene.h"
#include "dcb/evo_effect.h"

typedef struct {
    s16 first;
    s16 last;
} EvoRange;

typedef struct {
    Rect16 rect;
    s32 brightness;
    s32 style;
    s32 flags;
    s32 label;
    u8 labelPalette;
} EvoWindowDef;

typedef struct {
    char *name;
    s8 learnLevels[6];
    u8 unkA[2];
} EvoAbilityInfo;

typedef struct {
    u8 card;
    u8 ability;
} EvoAbilityReward;

typedef s32 (*EvoCardCompare)(s8 *, s8 *);

extern EvoCardInfo *EVO_CARDS_BY_ID[];
extern u8 *EVO_SPARE_CARD_COUNTS;
extern u8 *EVO_DECK_CARD_COUNTS[3];
extern EvoFusion EVO_FUSION;
extern EvoProgram *EVO_SCRIPT;
extern UiWindow EVO_CARD_LIST_WINDOW;
extern u8 EVO_MAX_CARD_LEVEL;
extern EvoDialog EVO_DIALOG;
extern u8 EVO_RANK_UP_STATE;
extern u8 EVO_LEVEL_UP_PENDING;
extern s16 EVO_STAT_BONUSES[4];
extern s32 EVO_NEW_DIGI_PART;
extern UiWindow EVO_RANK_UP_WINDOW;
extern EvoText EVO_TEXT_LINES[4];
extern u8 EVO_CARD_RECEIVED;
extern EvoChoice EVO_TYPE_CHOICE;
extern u8 EVO_SCRIPT_HALTED;
extern EvoCardInfo *EVO_CARD_LIST[];
extern void (*EVO_WINDOW_DRAW_FUNCS[])();
extern UiWindow EVO_SORT_WINDOW;
extern CursorHighlight EVO_CARD_LIST_CURSOR;
extern CursorHighlight EVO_SORT_CURSOR;
extern s16 EVO_CURSOR_CARD;
extern const char EVO_FMT_CARD_NUMBER[];
extern const char EVO_FMT_CARD_COUNT[];
extern const char EVO_STR_CARDS[];
extern const char EVO_STR_SPEC[];

void EVO_findPartnerReward(void);
void EVO_startSecondCardPick(void);
void EVO_addPartnerExp(void);
s32 EVO_addTextLine(u8 *src);
void EVO_closeFusionTypeChoice(void);
void EVO_findCardOfLevel(u8 type, u8 level, s16 *out, s16 excludeA, s16 excludeB);
void EVO_clearTextLines(EvoText *slot);
void EVO_openFusionTypeChoice(void);
void EVO_openPartnerList(void);
void EVO_closePartnerList(void);
void EVO_openCardList(void);
void EVO_closeCardList(void);
void EVO_startPartnerFusion(void);
void EVO_cancelPartnerFusion(void);
void EVO_leaveForCutscene(void);
void EVO_resetFusion(void);
void EVO_cancelFirstCard(void);
void EVO_drawRankUpBanner(UiWindow *w);
void EVO_tickFusionTypeChoice(void);
void EVO_tickPartnerList(void);
void EVO_tickCardList(void);
void EVO_tickPartnerReward(void);
void EVO_tickFusionResult(void);
void EVO_showBothTrays(void);
void EVO_repickSecondCard(void);
void func_801EBE08(void);
void EVO_slideTrayOut(s32 index);
void EVO_slideTrayIn(s32 index);
void EVO_showCutsceneResult(void);
void EVO_closeFusionResult(void);
s16 EVO_findFusionResult(void);

/* the Digi-Parts: their text and the rank each partner learns them at */
EvoAbilityInfo EVO_DIGI_PARTS[128] = {
    { "HP+50.", { 3, 5, 1, 99, 3, 7 } },
    { "HP+100.", { 17, 16, 8, 12, 14, 19 } },
    { "HP+150.", { 29, 32, 19, 25, 32, 33 } },
    { "HP+200.", { 45, 48, 31, 40, 52, 59 } },
    { "HP+300.", { 61, 67, 51, 57, 68, 78 } },
    { "HP+400.", { 96, 89, 71, 81, 91, 88 } },
    { "HP+500.", { 99, -1, 75, 91, -1, 95 } },
    { "All Attack Powers +50.", { 18, 39, 54, 30, 57, 28 } },
    { "All Attack Powers +100.", { 39, 86, 72, 50, 89, 51 } },
    { "All Attack Powers +200.", { 75, -1, 90, 93, -1, 84 } },
    { "*b0 Attack Power +100.", { 1, 9, 15, 5, 4, 2 } },
    { "*b0 Attack Power +150.", { 10, 21, 38, 22, 16, 15 } },
    { "*b0 Attack Power +200.", { 27, 54, 62, 41, 39, 30 } },
    { "*b0 Attack Power +250.", { 48, -1, 78, 61, 62, 61 } },
    { "*b0 Attack Power +300.", { 67, -1, -1, 85, 82, 79 } },
    { "*b1 Attack Power +50.", { 4, 1, 12, 2, 6, 13 } },
    { "*b1 Attack Power +100.", { 12, 7, 22, 16, 18, 29 } },
    { "*b1 Attack Power +150.", { 32, 28, 45, 36, 45, 43 } },
    { "*b1 Attack Power +200.", { 51, 44, -1, 56, 67, 66 } },
    { "*b1 Attack Power +250.", { 77, -1, -1, 76, 86, 96 } },
    { "*b2 Attack Power +50.", { 6, 6, 2, 4, 10, 1 } },
    { "*b2 Attack Power +100.", { 19, 36, 20, 19, 27, 9 } },
    { "*b2 Attack Power +150.", { 35, 69, 42, 38, 53, 24 } },
    { "*b2 Attack Power +200.", { 82, -1, 63, 67, -1, 48 } },
    { "*b0 to 0, *b2 Attack Power -100.", { 24, 12, 4, 14, -1, 34 } },
    { "*b1 to 0, *b2 Attack Power -100.", { 46, 23, 9, 35, -1, 17 } },
    { "*b2 to 0, *b2 Attack Power -100.", { 58, 71, 36, 10, -1, 71 } },
    { "Counter *b0,*b2 Attack Power to 0.", { 11, 26, -1, -1, 40, 62 } },
    { "Counter *b1,*b2 Attack Power to 0.", { 36, 14, -1, -1, 24, 49 } },
    { "Counter *b2,*b2 Attack Power to 0.", { 40, 57, -1, -1, 11, 21 } },
    { "Opponent *a0 X3, *b2 Attack Power -200.", { 87, 92, 39, 69, 31, 50 } },
    { "Opponent *a1 X3, *b2 Attack Power -200.", { 25, 97, -1, 42, 77, 73 } },
    { "Opponent *a2 X3, *b2 Attack Power -200.", { 37, -1, 66, -1, 83, 4 } },
    { "Opponent *a3 X3, *b2 Attack Power -200.", { 68, 33, 46, 21, 5, 97 } },
    { "Opponent *a4 X3, *b2 Attack Power -200.", { 52, 41, 6, -1, -1, 25 } },
    { "1st Attack, *b2 Attack Power -200.", { 76, 50, 95, 62, -1, -1 } },
    { "Jamming Support, *b2 Attack Power -100.", { -1, 74, 27, -1, 54, 10 } },
    { "Eat-up HP, *b2 Attack Power -200.", { -1, 87, 50, -1, 73, -1 } },
    { "Add + 10 DP.", { 42, 4, 61, 1, 9, -1 } },
    { "Add + 20 DP.", { 91, 29, 79, 27, 21, -1 } },
    { "Add + 30 DP.", { -1, 98, 97, 60, 78, 85 } },
    { "Boost Attack Power +50.", { 2, 10, 13, 3, 7, 11 } },
    { "Boost Attack Power +100.", { 21, 42, 56, 33, 36, 35 } },
    { "Boost Attack Power +200.", { 26, 81, 73, 46, 84, 44 } },
    { "Boost Attack Power +300.", { 69, -1, 84, 82, -1, 80 } },
    { "Attack Power is Doubled.", { -1, -1, -1, -1, -1, -1 } },
    { "Boost *b0 Attack Power +300.", { 8, 30, 47, 17, 26, 12 } },
    { "Boost *b0 Attack Power +400.", { 34, 77, 74, 47, 46, 39 } },
    { "Boost *b0 Attack Power +500.", { 55, -1, 88, 88, 66, 74 } },
    { "*b0 Attack Power is Doubled.", { 13, 64, 67, 58, 33, 63 } },
    { "*b0 Attack Power is Tripled.", { 59, -1, 94, 74, -1, 86 } },
    { "Boost *b1 Attack Power +200.", { 15, 2, -1, 7, 22, 18 } },
    { "Boost *b1 Attack Power +300.", { 28, 19, -1, 23, 41, 36 } },
    { "Boost *b1 Attack Power +400.", { 43, 70, -1, 92, 59, 53 } },
    { "*b1 Attack Power is Doubled.", { 22, 13, 59, 34, -1, 67 } },
    { "*b1 Attack Power is Tripled.", { 83, -1, 91, 94, -1, 89 } },
    { "Boost *b2 Attack Power +100.", { 23, 40, 10, 18, -1, 8 } },
    { "Boost *b2 Attack Power +200.", { 38, 58, 40, 43, -1, 20 } },
    { "Boost *b2 Attack Power +300.", { 71, -1, 60, 63, -1, 90 } },
    { "*b2 Attack Power is Doubled.", { 44, 72, 48, 51, -1, 40 } },
    { "*b2 Attack Power is Tripled.", { 88, -1, 85, 95, -1, 65 } },
    { "Attack Power becomes same as HP.", { -1, -1, -1, -1, -1, -1 } },
    { "Get 1st Attack.", { 53, 55, 98, 71, 85, -1 } },
    { "Attack becomes Eat-up HP.", { -1, 93, 80, -1, 61, 55 } },
    { "Lower Opponent's *b0 Attack Power to 0.", { 54, -1, 17, -1, 28, 75 } },
    { "Lower Opponent's *b1 Attack Power to 0.", { 84, 24, 28, -1, 15, -1 } },
    { "Lower Opponent's *b2 Attack Power to 0.", { -1, 66, 24, -1, 13, 45 } },
    { "*b0 Counterattack (Attack 2nd).", { 7, -1, 33, -1, 47, 14 } },
    { "*b1 Counterattack (Attack 2nd).", { 33, 17, -1, -1, 37, 26 } },
    { "*b2 Counterattack (Attack 2nd).", { 47, 43, 5, -1, -1, 37 } },
    { "If *a0 Opponent, X2 own Attack Power.", { 74, 79, -1, 77, 25, 38 } },
    { "If *a0 Opponent, X3 own Attack Power.", { -1, 91, -1, 65, 74, 82 } },
    { "If *a1 Opponent, X2 own Attack Power.", { 5, 47, 64, 26, 80, 76 } },
    { "If *a1 Opponent, X3 own Attack Power.", { 60, -1, 89, 83, 98, 91 } },
    { "If *a2 Opponent, X2 own Attack Power.", { 50, 27, 58, 59, 42, 6 } },
    { "If *a2 Opponent, X3 own Attack Power.", { 79, -1, 93, -1, 93, 70 } },
    { "If *a3 Opponent, X2 own Attack Power.", { 56, 37, 44, 6, 19, 93 } },
    { "If *a3 Opponent, X3 own Attack Power.", { 92, 60, 87, 79, 63, -1 } },
    { "If *a4 Opponent, X2 own Attack Power.", { 78, 76, 26, 72, 69, 46 } },
    { "If *a4 Opponent, X3 own Attack Power.", { 89, 96, 55, -1, -1, 68 } },
    { "Change own Specialty to *a0.", { 9, 35, -1, 48, 96, -1 } },
    { "Change own Specialty to *a1.", { -1, 99, 30, 87, 58, -1 } },
    { "Change own Specialty to *a2.", { 97, 3, 41, 8, 12, 94 } },
    { "Change own Specialty to *a3.", { 30, -1, 68, 98, 81, 3 } },
    { "Change own Specialty to *a4.", { 95, 82, 14, 52, 76, 99 } },
    { "Switch Opponent's Specialty to own.", { -1, -1, -1, -1, -1, -1 } },
    { "Swap Specialty with Opponent's.", { -1, -1, -1, -1, -1, -1 } },
    { "If *a0 Opponent, lower its AP to 0.", { 66, 52, -1, 97, 43, -1 } },
    { "If *a1 Opponent, lower its AP to 0.", { 41, 61, 34, 84, 8, -1 } },
    { "If *a2 Opponent, lower its AP to 0.", { -1, 11, 52, -1, 64, 22 } },
    { "If *a3 Opponent, lower its AP to 0.", { -1, 25, 37, 13, 2, 72 } },
    { "If *a4 Opponent, lower its AP to 0.", { -1, 75, 16, -1, 34, 52 } },
    { "Reduce both Players' Atk Pwr to 0.", { -1, -1, -1, -1, -1, -1 } },
    { "If *e3, boost Attack Power +200.", { 14, 31, -1, 9, 48, 23 } },
    { "If *e4, boost Attack Power +300.", { 31, 53, -1, 20, 38, 57 } },
    { "If *e5, boost Attack Power +400.", { 57, -1, 81, 70, 95, 77 } },
    { "Opponent uses *b0 Attack.", { 16, 62, -1, 28, 97, 31 } },
    { "Opponent uses *b1 Attack.", { 62, 18, 11, 39, -1, 47 } },
    { "Opponent uses *b2 Attack.", { 94, 8, 21, -1, 44, 58 } },
    { "Opponent uses same Attack.", { -1, -1, -1, -1, -1, -1 } },
    { "Recover HP +200.", { 72, 15, 32, 11, 1, 81 } },
    { "Recover HP +300.", { 90, 38, 53, 31, 17, -1 } },
    { "Recover HP +400.", { -1, 68, 92, 73, 55, -1 } },
    { "Halve Attack Power, recover HP +400.", { 93, 20, 43, 15, 23, -1 } },
    { "Halve Attack Power, recover HP +600.", { -1, 46, 86, 44, 60, -1 } },
    { "If HP < Opponent's HP, add HP +500.", { 98, 59, 23, 37, 29, -1 } },
    { "If HP < Opponent's HP, add HP +700.", { -1, 83, 69, 53, 56, -1 } },
    { "If KO'd in battle, revive w/ HP 300.", { 49, 34, -1, 24, 30, 42 } },
    { "If KO'd in battle, revive w/ HP 600.", { -1, 63, -1, 49, 49, 64 } },
    { "If KO'd in battle, revive w/ HP 1000.", { -1, 94, -1, 64, 65, 98 } },
    { "Drop 1 Card in Opponent's Hand.", { 63, 22, 3, 45, 75, 27 } },
    { "Drop 2 Cards in Opponent's Hand.", { -1, 78, 25, -1, 92, 56 } },
    { "Drop Opponent's Top 2 DP Cards shown.", { -1, 56, 18, 86, 35, 5 } },
    { "Drop Opponent's Top 3 DP Cards shown.", { -1, 88, 49, -1, 90, 32 } },
    { "Drop Opponent's Top 4 DP Cards shown.", { -1, -1, -1, -1, -1, -1 } },
    { "Drop 2 Cards in Opponent's Online Deck.", { -1, 51, 7, 29, -1, 16 } },
    { "Drop 3 Cards in Opponent's Online Deck.", { -1, 95, 35, 89, -1, 54 } },
    { "Move Offline Top Card to Online Deck.", { 86, -1, 70, 78, 50, -1 } },
    { "Void Opponent's Support Effect.", { -1, 84, 65, -1, 87, 69 } },
    { "Draw until there are 4 Cards.", { 64, 45, 29, 54, 20, -1 } },
    { "Draw Online Partner Card, then Shuffle.", { 65, 80, 82, 68, 72, -1 } },
    { "If *e3, HP + 200 & all Attack Powers +100.", { 73, 73, -1, 55, 94, 83 } },
    { "If *ea, HP + 200 & all Attack Powers +100.", { 81, -1, 76, 66, 79, 87 } },
    { "Boost Battle Experience by 10%.", { 20, 49, 57, 32, 71, 41 } },
    { "Boost Battle Experience by 20%.", { 85, 65, 77, 75, 88, 92 } },
    { "Boost Battle Experience by 30%.", { 80, -1, 96, 90, 99, 60 } },
    { "Rare Card might appear after battle.", { 70, 85, 83, 80, 51, -1 } },
    { "Rare Card even more likely to appear.", { -1, 90, 99, 96, 70, -1 } },
};

u8 EVO_PARTNER_CARD_IDS[6] = { 0xAF, 0xB6, 0xBE, 0xB7, 0xB8, 0xBB };
s8 EVO_FUSION_RESULT_TYPES[6][6] = {
    { 5, 2, 3, 4, 1, 0 },
    { 2, 5, 4, 0, 2, 1 },
    { 3, 4, 5, 1, 3, 2 },
    { 4, 0, 1, 5, 0, 3 },
    { 1, 2, 3, 0, 5, 4 },
    { 0, 1, 2, 3, 4, 5 },
};
EvoRange EVO_CARD_ID_RANGES[7] = {
    { 0, 33 }, { 34, 68 }, { 69, 102 }, { 103, 138 }, { 139, 171 }, { 191, 272 }, { 294, 300 },
};
EvoAbilityReward EVO_PARTNER_FUSION_REWARDS[6][5] = {
    { { 0x00, 0x09 }, { 0x04, 0x49 }, { 0x0C, 0x78 }, { 0x08, 0x30 }, { 0x02, 0x7E } },
    { { 0x45, 0x7A }, { 0x4C, 0x4D }, { 0x0D, 0x3E }, { 0x03, 0x35 }, { 0x07, 0x7C } },
    { { 0x22, 0x06 }, { 0x8E, 0x76 }, { 0x97, 0x4F }, { 0x8C, 0x3A }, { 0x28, 0x75 } },
    { { 0x46, 0x28 }, { 0x4B, 0x5F }, { 0x52, 0x79 }, { 0x48, 0x32 }, { 0x25, 0x47 } },
    { { 0x47, 0x7F }, { 0x4D, 0x66 }, { 0x49, 0x6D }, { 0x76, 0x37 }, { 0x4E, 0x68 } },
    { { 0x68, 0x7D }, { 0x69, 0x4B }, { 0x75, 0x3F }, { 0x6B, 0x3C }, { 0x4F, 0x23 } },
};
Bytes4 EVO_TEXT_COLORS[2] = { { { 0x80, 0x80, 0x80, 0 } }, { { 0x40, 0x40, 0x40, 0 } } };
Bytes4 EVO_TEXT_COLOR_GREY = { { 0x60, 0x60, 0x60, 0 } };
Bytes4 EVO_TEXT_COLOR_RED = { { 0xC0, 0x60, 0x60, 0 } };

/* not referenced by any code */
Rect16 D_801EFF68[14] = {
    { 0x10, 0x24, 0x40, 0x40 },
    { 0x5C, 0x2A, 0xD2, 0xC },
    { 0x62, 0x3E, 0xCC, 0x24 },
    { 0x10, 0x6E, 0x40, 0x40 },
    { 0x5C, 0x74, 0xD2, 0xC },
    { 0x62, 0x88, 0xCC, 0x24 },
    { 0x10, 0xAC, 0x3E, 0x38 },
    { 0x5A, 0xB4, 0xE0, 0x30 },
    { 0x10, 0x96, 0x3E, 0x38 },
    { 0x5A, 0x9C, 0xDE, 0x30 },
    { 0x76, 0x26, 0xC4, 0x7E },
    { 0xA, 0x32, 0x58, 0x72 },
    { 0x78, 0x51, 0x88, 0x34 },
    { 0x0, 0x0, 0x0, 0x0 },
};

/* not referenced by any code */
s16 D_801EFFD8[2][10] = {
    { -0x78, 0x29, 0xE, 0x29, 0x50, 0x74, -0x1, 0x0, 0xC, 0x0 },
    { -0x78, 0x29, 0xE, 0x29, 0x50, 0x74, -0x1, 0x0, 0xC, 0x1 },
};

const u8 EVO_FUSION_RECIPES[20][4] = {
    { 0x01, 0x04, 0x00, 0xEC },
    { 0x04, 0x23, 0x00, 0xEC },
    { 0x02, 0x25, 0x01, 0xED },
    { 0x06, 0x27, 0x23, 0xFE },
    { 0x8E, 0x28, 0x22, 0xEE },
    { 0x2A, 0x2B, 0x24, 0xEF },
    { 0x4C, 0x07, 0x45, 0xF0 },
    { 0x4D, 0x4B, 0x47, 0xF1 },
    { 0x4B, 0x8F, 0x46, 0xF2 },
    { 0x51, 0x03, 0x48, 0xF3 },
    { 0x70, 0x4A, 0x68, 0xF4 },
    { 0x4E, 0x6F, 0x49, 0xF5 },
    { 0x6E, 0x6F, 0x6A, 0xF6 },
    { 0x6D, 0x90, 0x69, 0xF7 },
    { 0x93, 0x73, 0x8B, 0xF8 },
    { 0x96, 0x74, 0x8D, 0xF9 },
    { 0x91, 0x26, 0x8C, 0xFA },
    { 0x0C, 0x75, 0x04, 0xFB },
    { 0x53, 0x0D, 0x4C, 0xFC },
    { 0x52, 0x97, 0x8E, 0xFD },
};

void EVO_loadUnitTextures(void) {
    char path[24];
    u32 *pack;

    sprintf(path, "C:\\OBJECT\\unit.TIS", (s8)((SessionData *)D_8006E054)->unk100C->unk1A4);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
}

/* defined after the table: GCC emitted the table's strings in reverse order */
extern const char EVO_STR_NUMBER[];

char *EVO_SORT_LABELS[12] = {
    (char *)EVO_STR_NUMBER,
    "*a0 Fire",
    "*a1 Ice",
    "*a2 Nature",
    "*a3 Darkness",
    "*a4 Rare",
    "*a5 Option",
    "*a6 Option",
    "Level *e3",
    "Level *e4",
    "Level *e5",
    "Number of Cards that can be Fused.",
};

/* also drawCardInfo's heading */
const char EVO_STR_NUMBER[] = "Number";

Menu EVO_CARD_LIST_MENU = { 0, 0, { 0x6A, 0x2C, 0xD0, 0x6C }, 0, -1, 0, -1, 0xA, 0x61, 0x74, 0xC, 0, 301, 0x36, 1, 0, 0xC };
Menu EVO_SORT_MENU = { 0, 0, { 0x28, 0x3C, 0xCA, 0x70 }, 0, -1, 0, -1, 0xA, 0x56, 0xC0, 0xC, 0, 12, 0, 1, 0, 0xE };

void EVO_initFusionScene(void) {
    Graphics *camera;

    initScene3D(1);
    func_800149B8(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    func_80014A00(0x1B);
    func_800149B8(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
    func_80014C08(2);
    camera = (Graphics *)&GRAPHICS;
    camera->rotX = 0;
    camera->rotY = 0;
    camera->rotZ = 0;
    camera->posX = 0;
    camera->posY = 0;
    camera->posZ = 0;
    camera->unk8E = 0;
    camera->unk90 = 20;
    camera->unk92 = 0;
    camera->unk94 = 0;
    camera->targetModel = -1;
    camera->snapCamera = 1;
    func_80014C08(2);
}

void EVO_countSpareCards(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 0x12D; i++) {
        EVO_SPARE_CARD_COUNTS[i] = getOwnedCardCount(0, i);
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 0x12D; j++) {
            EVO_DECK_CARD_COUNTS[i][j] = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].inUse != 0) {
            for (j = 0; j < 30; j++) {
                EVO_DECK_CARD_COUNTS[i][getCardId(((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].type,
                                        ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].index)]++;
            }
        }
    }
    for (i = 0; i < 0x12D; i++) {
        for (j = 1; j < 3; j++) {
            if (EVO_DECK_CARD_COUNTS[0][i] < EVO_DECK_CARD_COUNTS[j][i]) {
                EVO_DECK_CARD_COUNTS[0][i] = EVO_DECK_CARD_COUNTS[j][i];
            }
        }
    }
    for (i = 0; i < 0x12D; i++) {
        EVO_SPARE_CARD_COUNTS[i] -= EVO_DECK_CARD_COUNTS[0][i];
    }
}

void EVO_initCardList(void) {
    s32 i;
    s32 j;

    for (i = 0, j = 0; j < 0xBF; j++, i++) {
        EVO_CARD_LIST[i] = (EvoCardInfo *)(DIGIMON_CARDS + j * 0x13C);
        EVO_CARDS_BY_ID[i] = EVO_CARD_LIST[i];
    }
    for (j = 0; j < 0x66; j++, i++) {
        EVO_CARD_LIST[i] = (EvoCardInfo *)(OPTION_CARDS + j * 0xE2);
        EVO_CARDS_BY_ID[i] = EVO_CARD_LIST[i];
    }
    for (j = 0; j < 8; j++, i++) {
        EVO_CARD_LIST[i] = (EvoCardInfo *)(DIGIVOLVE_CARDS + j * 0x70);
        EVO_CARDS_BY_ID[i] = EVO_CARD_LIST[i];
    }
    for (j = 0; j < 3; j++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->partners[j].cardId != 0) {
            EVO_CARD_LIST[((PlayerProfile *)PLAYER_PROFILES)->partners[j].cardId] =
                (EvoCardInfo *)&((PlayerProfile *)PLAYER_PROFILES)->partners[j].card[0];
        }
    }
}

s32 EVO_compareFireCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 0) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 0) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareIceCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 1) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 1) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareNatureCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 2) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 2) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareDarknessCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 3) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 3) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareRareCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 4) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 4) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareOptionCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = (*a)->type == 1;
    s32 kb = (*b)->type == 1;

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareDigivolveCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = (*a)->type == 2;
    s32 kb = (*b)->type == 2;

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareLevel0Cards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 0) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 0) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareLevel2Cards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 2) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 2) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareLevel3Cards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 3) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 3) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareSpareCounts(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = EVO_SPARE_CARD_COUNTS[(*a)->id];
    s32 kb = EVO_SPARE_CARD_COUNTS[(*b)->id];

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 (*EVO_SORT_COMPARES[12])(s8 *, s8 *) = {
    0,
    (EvoCardCompare)EVO_compareFireCards,
    (EvoCardCompare)EVO_compareIceCards,
    (EvoCardCompare)EVO_compareNatureCards,
    (EvoCardCompare)EVO_compareDarknessCards,
    (EvoCardCompare)EVO_compareRareCards,
    (EvoCardCompare)EVO_compareOptionCards,
    (EvoCardCompare)EVO_compareDigivolveCards,
    (EvoCardCompare)EVO_compareLevel0Cards,
    (EvoCardCompare)EVO_compareLevel2Cards,
    (EvoCardCompare)EVO_compareLevel3Cards,
    (EvoCardCompare)EVO_compareSpareCounts,
};

Rect16 EVO_RANK_UP_RECT = { 0xD, 0x4A, 0x48, 0x9 };

void EVO_drawSortMenu(UiWindow *w) {
    s32 x = w->originX;
    s32 z = w->z;
    s32 i;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    for (i = 0; i < EVO_SORT_MENU.nrows; i++) {
        if (i < w->view.y / EVO_SORT_MENU.rowH) {
            continue;
        }
        if ((w->view.y + w->rect.h) / EVO_SORT_MENU.rowH < i) {
            break;
        }
        drawText(x, w->originY + i * EVO_SORT_MENU.rowH + 1, (s32)EVO_SORT_LABELS[i], 7, z);
    }
    updateMenuCursor(&EVO_SORT_MENU);
    if (EVO_SORT_MENU.active != 0 && (PAD_STATES[0]->pressed & 0x40)) {
        playMenuSound(1);
        EVO_CARD_LIST_MENU.row = 0;
        centerMenuOnCursor(&EVO_CARD_LIST_MENU);
        if (EVO_SORT_COMPARES[EVO_SORT_MENU.row] != NULL) {
            sortArray((s8 *)EVO_CARD_LIST, 0x12D, 4, EVO_SORT_COMPARES[EVO_SORT_MENU.row]);
        } else {
            EVO_initCardList();
        }
    }
}

void EVO_drawCardList(UiWindow *w) {
    char text[72];
    u8 *color;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 type;
    s32 palette;

    x = w->originX;
    z = w->z;
    x -= 10;
    if (EVO_TYPE_CHOICE.unkE == 0) {
        w->brightness = 0x80;
    } else {
        w->brightness = 0x40;
    }
    for (i = 0; i < EVO_CARD_LIST_MENU.nrows; i++) {
        if (i < w->view.y / EVO_CARD_LIST_MENU.rowH) {
            continue;
        }
        if ((w->view.y + w->rect.h) / EVO_CARD_LIST_MENU.rowH < i) {
            break;
        }
        y = w->originY + i * EVO_CARD_LIST_MENU.rowH + 1;
        type = EVO_CARD_LIST[i]->type;
        color = EVO_TEXT_COLORS[0].b;
        palette = 7;
        if (EVO_CARD_LIST[i]->fusionPoints == 0) {
            palette = 3;
        }
        if (type == 0 && (EVO_CARD_LIST[i]->attr & 0xF) > EVO_MAX_CARD_LEVEL) {
            palette = 3;
        }
        if (((PlayerProfile *)PLAYER_PROFILES)->cardCollection[EVO_CARD_LIST[i]->id] & 0x40) {
            if (EVO_SPARE_CARD_COUNTS[EVO_CARD_LIST[i]->id] == 0) {
                color = EVO_TEXT_COLOR_GREY.b;
            }
            drawTextColored(x + 0x3C, y, EVO_CARD_LIST[i]->name, color, palette, z);
            switch (type) {
            case 0:
                drawIconColored(x + 0x20, y, 0, EVO_CARD_LIST[i]->attr >> 4, color, z);
                if (palette == 3) {
                    drawIconColored(x + 0x2C, y, 0, (EVO_CARD_LIST[i]->attr & 0xF) + 0x10, EVO_TEXT_COLOR_RED.b, z);
                } else {
                    drawIconColored(x + 0x2C, y, 0, (EVO_CARD_LIST[i]->attr & 0xF) + 0x10, color, z);
                }
                break;
            case 1:
                drawIconColored(x + 0x20, y, 0, 5, color, z);
                break;
            case 2:
                drawIconColored(x + 0x20, y, 0, 6, color, z);
                break;
            }
        } else {
            palette = 9;
            drawTextColored(x + 0x20, y, "??", color, palette, z);
            drawTextColored(x + 0x3C, y, "------------------", color, palette, z);
        }
        sprintf(text, EVO_FMT_CARD_NUMBER, EVO_CARD_LIST[i]->id);
        drawTextColored(x + 0xA, y, text, color, palette, z);
        sprintf(text, EVO_FMT_CARD_COUNT, EVO_SPARE_CARD_COUNTS[EVO_CARD_LIST[i]->id]);
        drawTextColored(x + 0xB5, y, text, color, palette, z);
        drawTinyTextColored(x + 0xBD, y + 6, (u8 *)EVO_STR_CARDS, palette, color, z);
    }
    updateMenuCursor(&EVO_CARD_LIST_MENU);
}

const char EVO_FMT_CARD_NUMBER[] = "*s0%3.3d";

const char EVO_FMT_CARD_COUNT[] = "%d";

const char EVO_STR_CARDS[] = "Cards";

void EVO_drawFusionTypeTitle(EvoWindow *w) {
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    if (w->isPartner == 0) {
        drawTextColored(x + 0x4B, y, "Card Fusion", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 6, z);
    } else {
        drawTextColored(x + 0x41, y, "Partner Fusion", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 6, z);
    }
}

void EVO_drawFusionTypeHelp(EvoWindow *w) {
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    if (w->isPartner == 0) {
        x += 2;
        drawTextColored(x, y, "*w1Create a New Card", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 8, z);
        drawTextColored(x, y + 0xC, "*w1by Fusing 2 Cards.", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 8, z);
        drawTextColored(x, y + 0x18, "*w1Partner Cards can't be used.", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 8, z);
    } else {
        x += 2;
        drawTextColored(x, y, "*w1Increase Experience Points by", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 8, z);
        drawTextColored(x, y + 0xC, "*w1Fusing a Card to a Partner Card.", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 8, z);
        drawTextColored(x, y + 0x18, "*w1Also,2 Partner Cards can't be Fused.", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 8, z);
    }
}

void EVO_drawTray(EvoTray *tray) {
    char text[40];
    Rect16 uv;
    POLY_FT4 *poly;
    s32 player;

    poly = tray->polys[FRAME_BUFFER_INDEX];
    player = 2;
    if (tray == &EVO_TRAYS[0]) {
        player = 1;
    }
    /* the index is added before the field offset in the original */
    if (((EvoFusion *)((u8 *)&EVO_FUSION + player))->busy[0] == 0) {
        bzero((Scene3D *)text, 0x21);
        sprintf(text, "TRAY%d", player);
        drawLargeText(tray->x + 16, 0x5C, (s32)text, 7, 0x1D);
        uv.x = 0;
        uv.y = 0x74;
        uv.w = 0x50;
        uv.h = 0x74;
        drawTexturedSprite(tray->x, tray->y, &uv, 0x18, 0x7BDF, 0x1E, 0x80, 0);
        return;
    }
    setlen(poly, 9);
    setcode(poly, 0x2C);
    poly->clut = 0x7BD8;
    poly->tpage = 0x18;
    poly->r0 = 0x80;
    poly->g0 = 0x80;
    poly->b0 = 0x80;
    poly->x0 = tray->x;
    poly->y0 = tray->y;
    poly->x1 = tray->x + 0x50;
    poly->y1 = tray->y;
    poly->x2 = tray->x;
    poly->y2 = tray->y + 0x74;
    poly->x3 = tray->x + 0x50;
    poly->y3 = tray->y + 0x74;
    poly->u0 = 0;
    poly->v0 = 0;
    poly->u1 = 0x50;
    poly->v1 = 0;
    poly->u2 = 0;
    poly->v2 = 0x74;
    poly->u3 = 0x50;
    poly->v3 = 0x74;
    addPrim(&CURRENT_FRAME_BUFFER->ot[29], poly);
    poly++;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    if (player == 1 && EVO_FUSION.hideResult == 0) {
        poly->clut = 0x7A98;
        poly->u0 = 0;
        poly->v0 = 0x40;
        poly->u1 = 0x3F;
        poly->v1 = 0x40;
        poly->u2 = 0;
        poly->v2 = 0x7F;
        poly->u3 = 0x3F;
        poly->v3 = 0x7F;
    } else {
        poly->clut = getClut(0x180, player + 0x1E7);
        poly->u0 = (player - 1) * 64;
        poly->v0 = 0;
        poly->u1 = (player - 1) * 64 + 0x3F;
        poly->v1 = 0;
        poly->u2 = (player - 1) * 64;
        poly->v2 = 0x3F;
        poly->u3 = (player - 1) * 64 + 0x3F;
        poly->v3 = 0x3F;
    }
    poly->tpage = 0x99;
    poly->r0 = 0x80;
    poly->g0 = 0x80;
    poly->b0 = 0x80;
    poly->x0 = tray->x + 8;
    poly->y0 = tray->y + 0x14;
    poly->x1 = tray->x + 0x48;
    poly->y1 = tray->y + 0x14;
    poly->x2 = tray->x + 8;
    poly->y2 = tray->y + 0x54;
    poly->x3 = tray->x + 0x48;
    poly->y3 = tray->y + 0x54;
    addPrim(&CURRENT_FRAME_BUFFER->ot[28], poly);
}

EvoWindowDef EVO_WINDOW_DEFS[14] = {
    { { 0x10, 0x24, 0x40, 0x40 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5C, 0x2A, 0xDE, 0xC }, 0x80, 0x56, 8, 0, 8 },
    { { 0x62, 0x3E, 0xD8, 0x24 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x10, 0x4C, 0x40, 0x40 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5C, 0x52, 0xDE, 0xC }, 0x80, 0x56, 8, 0, 8 },
    { { 0x62, 0x66, 0xD8, 0x24 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x10, 0xAC, 0x3E, 0x38 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5A, 0xB4, 0xE0, 0x30 }, 0x80, 0x56, 8, (s32)"MESSAGE", 8 },
    { { 0x10, 0x96, 0x3E, 0x38 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5A, 0x9C, 0xDE, 0x30 }, 0x80, 0x56, 8, 0, 8 },
    { { 0x76, 0x26, 0xC4, 0x7E }, 0x80, 0x66, 8, 0, 8 },
    { { 0xA, 0x2C, 0x52, 0x78 }, 0x80, 0x56, 8, 0, 8 },
    { { 0x78, 0x51, 0x88, 0x34 }, 0x80, 0x56, 8, 0, 8 },
    { { 0x42, 0x78, 0x48, 0x9 }, 0x80, 0x36, 0, 0, 8 },
};
const char D_801DF260[] = "";

void EVO_slideInFirstTray(void) {
    EVO_TRAYS[0].x += 10;
    if (EVO_TRAYS[0].x >= 15) {
        EVO_TRAYS[0].x = 14;
        EVO_FUSION.step = 3;
        EVO_CARD_LIST_MENU.active = 1;
        EVO_FUSION.scriptState = 0;
    }
}

void EVO_slideOutFirstTray(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x58) {
        EVO_TRAYS[0].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = -1;
    }
}

void EVO_swapToSecondTray(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x78) {
        EVO_TRAYS[0].x = -0x78;
        EVO_TRAYS[1].x += 10;
        if (EVO_TRAYS[1].x >= 15) {
            EVO_TRAYS[1].x = 14;
            EVO_FUSION.step = 3;
            EVO_CARD_LIST_MENU.active = 1;
        }
    }
}

void EVO_swapToFirstTray(void) {
    EVO_TRAYS[1].x -= 10;
    if (EVO_TRAYS[1].x < -0x78) {
        EVO_TRAYS[1].x = -0x78;
        EVO_TRAYS[0].x += 10;
        if (EVO_TRAYS[0].x >= 15) {
            EVO_TRAYS[0].x = 14;
            EVO_FUSION.step = 3;
            EVO_CARD_LIST_MENU.active = 1;
        }
    }
}

void EVO_cancelSecondCard(s32 active) {
    if (active != 0) {
        EVO_TRAYS[0].x -= 10;
        EVO_TRAYS[1].x -= 10;
        if (EVO_TRAYS[0].x < -0x58) {
            EVO_TRAYS[0].x = -0x58;
        }
        if (EVO_TRAYS[1].x < 14) {
            EVO_TRAYS[1].x = 14;
        }
        if (EVO_TRAYS[0].x == -0x58 && EVO_TRAYS[1].x == 14) {
            EVO_FUSION.pickSlot = 2;
            EVO_SCRIPT->vars[8] = -1;
            EVO_FUSION.scriptState = 0;
            EVO_FUSION.step = 0;
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.secondCard]++;
        }
    }
}

void EVO_toggleMessageWindows(s8 mode) {
    if (mode == 0) {
        animateWindowTo(&EVO_WINDOWS[6].win, &EVO_WINDOW_DEFS[6].rect);
        animateWindowTo(&EVO_WINDOWS[7].win, &EVO_WINDOW_DEFS[7].rect);
    } else if (mode == 1) {
        animateWindowTo(&EVO_WINDOWS[6].win, (Rect16 *)-1);
        animateWindowTo(&EVO_WINDOWS[7].win, (Rect16 *)-1);
    }
}

void EVO_runChoiceDialog(s32 mode) {
    initDialog((u8 *)&EVO_DIALOG, NULL, 1);
    if (mode != 1) {
        EVO_DIALOG.choice = 1;
    }
    runDialog(&EVO_DIALOG);
    switch (EVO_DIALOG.choice) {
    case 0:
        EVO_SCRIPT->vars[1] = 0;
        break;
    case 1:
        EVO_SCRIPT->vars[1] = 1;
        break;
    case 2:
        EVO_SCRIPT->vars[1] = 2;
        break;
    }
}

void EVO_drawScreenFlash(void) {
    if (EVO_SCREEN_FLASH.on == 0) {
        EVO_SCREEN_FLASH.brightness -= 8;
        if (EVO_SCREEN_FLASH.brightness < 0) {
            EVO_SCREEN_FLASH.brightness = 0;
        }
    } else {
        EVO_SCREEN_FLASH.brightness += 8;
        if (EVO_SCREEN_FLASH.brightness >= 0x100) {
            EVO_SCREEN_FLASH.brightness = 0xFF;
        }
    }
    if (EVO_SCREEN_FLASH.brightness != 0) {
        EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX].r0 = EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX].g0 =
            EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX].b0 = EVO_SCREEN_FLASH.brightness;
        addPrim(&CURRENT_FRAME_BUFFER->ot[25], &EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[25], &EVO_SCREEN_FLASH.tpage[FRAME_BUFFER_INDEX]);
    }
}

void EVO_initScreenFlash(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_80067784(&EVO_SCREEN_FLASH.poly[i]);
        SetSemiTrans(&EVO_SCREEN_FLASH.poly[i], 1);
        setPrimQuadRect(&EVO_SCREEN_FLASH.poly[i], 0, 0, 320, 240);
        SetDrawTPage(&EVO_SCREEN_FLASH.tpage[i], 0, 0, 0x20);
        EVO_SCREEN_FLASH.poly[i].b0 = 0;
        EVO_SCREEN_FLASH.poly[i].g0 = 0;
        EVO_SCREEN_FLASH.poly[i].r0 = 0;
    }
    EVO_SCREEN_FLASH.on = 0;
    EVO_SCREEN_FLASH.brightness = 0;
    addFrameCallback((s32)EVO_drawScreenFlash);
}

void EVO_runFusionScript(EvoProgram *data) {
    s32 result;

    if (EVO_SCRIPT_HALTED == 1) {
        return;
    }
    do {
        result = runScriptToNextEvent(data->script, data->vars);
        if (result == 1) {
            switch (data->script->eventOp) {
            case 10:
                switch (data->script->eventArg) {
                case 0:
                    if (EVO_addTextLine((u8 *)data->vars[4]) == -1) {
                        EVO_FUSION.scriptState = 1;
                        return;
                    }
                    break;
                case 1:
                    return;
                case 2:
                    EVO_toggleMessageWindows(0);
                    break;
                case 4:
                    EVO_openFusionTypeChoice();
                    return;
                case 5:
                    EVO_closeFusionTypeChoice();
                    return;
                case 11:
                    EVO_FUSION.scriptState = 2;
                    return;
                case 12:
                    EVO_clearTextLines(EVO_TEXT_LINES);
                    break;
                case 8:
                    EVO_openPartnerList();
                    break;
                case 9:
                    EVO_closePartnerList();
                    break;
                case 10:
                    EVO_openCardList();
                    break;
                case 13:
                    EVO_closeCardList();
                    break;
                case 6:
                    EVO_resetFusion();
                    break;
                case 7:
                    EVO_cancelFirstCard();
                    break;
                case 14:
                    EVO_startSecondCardPick();
                    break;
                case 15:
                    EVO_FUSION.pickSlot = 1;
                    break;
                case 16:
                    EVO_leaveForCutscene();
                    break;
                case 17:
                    EVO_startPartnerFusion();
                    return;
                case 18:
                    EVO_cancelPartnerFusion();
                    return;
                case 19:
                    EVO_FUSION.resumeOffset = EVO_SCRIPT->script->pc - EVO_SCRIPT->script->start;
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.resultStep = 0;
                    EVO_FUSION.step = 10;
                    return;
                case 20:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.resultStep = 0;
                    EVO_FUSION.step = 12;
                    return;
                case 21:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 13;
                    return;
                case 22:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 5;
                    animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
                    EVO_CARD_LIST_MENU.active = 1;
                    return;
                default:
                    EVO_SCRIPT_HALTED = 0;
                    break;
                }
                break;
            case 11:
                switch (data->script->eventArg) {
                case 0:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 15;
                    return;
                case 1:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 14;
                    return;
                case 2:
                    EVO_runChoiceDialog((s16)data->script->params[0]);
                    break;
                case 3:
                    animateWindowTo(&EVO_WINDOWS[(s16)data->script->params[0]].win, (Rect16 *)-1);
                    if ((s16)data->script->params[0] == -1) {
                        animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
                        EVO_CARD_LIST_MENU.active = 1;
                    }
                    break;
                case 4:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 16;
                    return;
                case 5:
                    playSoundEffect((s16)data->script->params[0]);
                    break;
                case 6:
                    func_80014C08((s16)data->script->params[0]);
                    break;
                case 7:
                    EVO_FUSION.rewardStep = data->script->params[0];
                    return;
                case 8:
                    if ((s16)data->script->params[0] == 0) {
                        EVO_FUSION.busy[1] = 0;
                    } else {
                        EVO_FUSION.busy[2] = 0;
                    }
                    break;
                }
                break;
            case 12:
            case 13:
                break;
            }
        }
        clearScriptBusy(data->script);
    } while (result != 0);
}

s32 EVO_tickFusionScript(EvoProgram *program) {
    *program->vars = 1;
    EVO_runFusionScript(program);
    return *program->vars;
}

Script *EVO_createScriptContext(EvoMsd *data) {
    Script *script;

    script = allocHeapBlock(sizeof(Script), 0x2C);
    script->base = (u8 *)data;
    script->start = data->code;
    script->pc = data->code;
    script->offset = 0;
    script->size = data->size;
    clearScriptBusy(script);
    return script;
}

s32 *EVO_allocScriptRegisters(s32 count) {
    s32 *flags;
    s32 *p;
    s32 i;

    flags = allocHeapBlock(count * 4, 0x2C);
    p = flags;
    for (i = 0; i < count; i++) {
        *p++ = 0;
    }
    return flags;
}

EvoProgram *EVO_loadUnitScript(s32 index) {
    char path[24];
    EvoMsd *data;
    EvoProgram *program;

    sprintf(path, "C:\\EVENT\\unit0%d.MSD", index);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    data = (EvoMsd *)func_80014C08(0x7FFFFFFF);
    program = allocHeapBlock(sizeof(EvoProgram), 0x2C);
    program->data = data;
    program->script = EVO_createScriptContext(data);
    return program;
}

void EVO_openWindows(void) {
    EvoWindowDef *def;
    s32 i;

    for (def = EVO_WINDOW_DEFS, i = 0; i < 14; i++, def++) {
        EVO_WINDOWS[i].z = 30;
        openWindow(&EVO_WINDOWS[i].win, def, -1, (s16 *)-1, def->flags, def->style, def->brightness, 12);
        if (def->label != 0) {
            EVO_WINDOWS[i].win.label = def->label;
        }
        EVO_WINDOWS[i].win.labelPalette = def->labelPalette;
        animateWindowTo(&EVO_WINDOWS[i].win, (Rect16 *)-1);
    }
    for (i = 0; i < 3; i++) {
        EVO_WINDOWS[i].isPartner = 0;
        EVO_WINDOWS[i + 3].isPartner = 1;
    }
    openMenu(&EVO_CARD_LIST_MENU, &EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_CURSOR, (Bytes4 *)-1);
    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
    EVO_CARD_LIST_WINDOW.labelPalette = 8;
    EVO_CARD_LIST_WINDOW.label = (s32)"CARD LIST";
    EVO_CARD_LIST_MENU.active = 0;
    openMenu(&EVO_SORT_MENU, &EVO_SORT_WINDOW, &EVO_SORT_CURSOR, (Bytes4 *)-1);
    animateWindowTo(&EVO_SORT_WINDOW, (Rect16 *)-1);
    EVO_SORT_WINDOW.label = (s32)"SORT MENU";
    EVO_SORT_WINDOW.labelPalette = 8;
    EVO_WINDOWS[12].z = 0x1B;
    EVO_WINDOWS[13].z = 5;
    EVO_WINDOWS[13].win.palette = 2;
    EVO_RANK_UP_STATE = 0;
    openWindow(&EVO_RANK_UP_WINDOW, &EVO_RANK_UP_RECT, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    animateWindowTo(&EVO_RANK_UP_WINDOW, (Rect16 *)-1);
    EVO_RANK_UP_WINDOW.palette = 2;
}

void EVO_drawEmptyWindow(void) {
}

void EVO_renderFusion(void) {
    s32 i;
    s32 t;

    if (EVO_FUSION.resultStep == 1 && EVO_TRAYS[1].merge == 1 && EVO_TRAYS[0].merge < 30) {
        EVO_TRAYS[0].merge++;
        if (EVO_TRAYS[0].merge >= 30) {
            EVO_TRAYS[0].merge = 30;
            EVO_FUSION.hideResult = 0;
        }
        t = EVO_TRAYS[0].merge;
        EVO_TRAYS[0].x = (t * 58 + (30 - t) * 14) / 30;
        EVO_TRAYS[1].x = (t * 58 + (30 - t) * 102) / 30;
    }
    for (i = 0; i < 14; i++) {
        drawWindow(&EVO_WINDOWS[i].win, EVO_WINDOW_DRAW_FUNCS[i], EVO_WINDOWS[i].z);
    }
    drawWindow(&EVO_RANK_UP_WINDOW, EVO_drawRankUpBanner, 5);
    drawWindow(&EVO_CARD_LIST_WINDOW, EVO_drawCardList, 0x1D);
    drawWindow(&EVO_SORT_WINDOW, EVO_drawSortMenu, 0x1C);
    for (i = 0; i < 2; i++) {
        EVO_drawTray(&EVO_TRAYS[i]);
    }
}

void EVO_advanceText(void) {
    if (EVO_FUSION.textTyping == 0 && (PAD_STATES[0]->pressed & 0x40)) {
        EVO_clearTextLines(EVO_TEXT_LINES);
        if (EVO_FUSION.scriptState == 1) {
            EVO_addTextLine((u8 *)EVO_SCRIPT->vars[4]);
        }
        EVO_FUSION.scriptState = 0;
        playSoundEffect(0);
    }
}

void EVO_loadCardImages(void) {
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    EVO_FUSION.cardArchive = (s32 *)func_80014C08(0x7FFFFFFF);
}

void EVO_loadCardImage(s32 id, s32 slot) {
    char path[64];
    u32 *tim;

    EVO_FUSION.busy[0] = 1;
    sprintf(path, "B:\\CARD\\LC%3.3d.TIM", id);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTim(tim, (slot & 1) * 32 + 0x240, (slot >> 1) * 64 + 0x100, 0x180, slot + 0x1E8);
    DrawSync(0);
    freeHeapBlock(tim);
    EVO_FUSION.busy[0] = 0;
}

void EVO_loadScriptFlags(void) {
    s32 i;
    s32 bit;
    PlayerProfile *profile;

    for (i = 20, bit = 0, profile = (PlayerProfile *)PLAYER_PROFILES; i < 30; i++, bit++) {
        if ((1 << bit) & profile->scriptFlags) {
            EVO_SCRIPT->vars[i] = 1;
        }
    }
}

void EVO_saveScriptFlags(void) {
    s32 i;
    s32 bit;

    i = 20;
    bit = 0;
    while (bit < 32) {
        if (EVO_SCRIPT->vars[i] != 0) {
            ((PlayerProfile *)PLAYER_PROFILES)->scriptFlags |= 1 << bit;
        }
        i++;
        bit++;
        if (i >= 30) {
            break;
        }
    }
}

void EVO_runFusion(s32 unit) {
    s32 running = 1;
    s32 i;

    EVO_loadUnitTextures();
    EVO_loadCardImages();
    EVO_initFusionScene();
    EVO_loadEffectArchive();
    for (i = 0; i < 3; i++) {
        EVO_DECK_CARD_COUNTS[i] = allocTaskHeapBlock(0x12D);
    }
    EVO_SPARE_CARD_COUNTS = allocTaskHeapBlock(0x12D);
    EVO_countSpareCards();
    EVO_openWindows();
    EVO_clearTextLines(EVO_TEXT_LINES);
    if (unit >= 0) {
        EVO_FUSION.unit = unit;
        EVO_initScreenFlash();
    }
    EVO_SCRIPT = EVO_loadUnitScript(EVO_FUSION.unit);
    EVO_SCRIPT->vars = EVO_allocScriptRegisters(30);
    if (unit < 0) {
        playMusic(0, EVO_FUSION.unit + 0x85, 100);
        EVO_FUSION.step = 17;
        EVO_FUSION.scriptState = 0;
        EVO_FUSION.hideResult = 0;
        EVO_SCRIPT->script->pc = EVO_SCRIPT->script->start + EVO_FUSION.resumeOffset;
        EVO_toggleMessageWindows(0);
    } else {
        EVO_initCardList();
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_FUSION.hideResult = 1;
    }
    EVO_FUSION.swapTimer = 0;
    EVO_FUSION.typeChoiceOpen = 0;
    EVO_FUSION.unkBD = 0;
    EVO_FUSION.partnerListOpen = 0;
    EVO_FUSION.fusionType = 0;
    EVO_FUSION.sortMenuOpen = 0;
    EVO_FUSION.resultStep = 0;
    for (i = 0; i < 2; i++) {
        EVO_TRAYS[i].x = -0x78;
        EVO_TRAYS[i].y = 0x29;
    }
    addFrameCallback((s32)EVO_renderFusion);
    EVO_loadScriptFlags();
    do {
        func_80014C08(1);
        switch (EVO_FUSION.scriptState) {
        case 0:
            running = EVO_tickFusionScript(EVO_SCRIPT);
            break;
        case 1:
        case 2:
            EVO_advanceText();
            break;
        }
        switch (EVO_FUSION.step) {
        case 0:
            break;
        case 1:
            EVO_tickFusionTypeChoice();
            break;
        case 2:
            EVO_tickPartnerList();
            break;
        case 3:
            EVO_tickCardList();
            break;
        case 4:
            EVO_tickPartnerReward();
            break;
        case 5:
            EVO_slideInFirstTray();
            break;
        case 6:
            EVO_swapToFirstTray();
            break;
        case 7:
            EVO_swapToSecondTray();
            break;
        case 8:
            EVO_slideOutFirstTray();
            break;
        case 10:
            EVO_tickFusionResult();
            break;
        case 11:
            EVO_showBothTrays();
            break;
        case 12:
            EVO_repickSecondCard();
            break;
        case 13:
            func_801EBE08();
            break;
        case 14:
            EVO_slideTrayOut((s16)EVO_SCRIPT->script->params[0]);
            break;
        case 15:
            EVO_slideTrayIn((s16)EVO_SCRIPT->script->params[0]);
            break;
        case 16:
            EVO_cancelSecondCard((s16)EVO_SCRIPT->script->params[0]);
            break;
        case 17:
            EVO_showCutsceneResult();
            break;
        case 18:
            EVO_closeFusionResult();
            break;
        }
    } while (running != 0 && EVO_FUSION.cutscene == 0);
    if (running == 0) {
        animateWindowTo(&EVO_WINDOWS[6].win, (Rect16 *)-1);
        animateWindowTo(&EVO_WINDOWS[7].win, (Rect16 *)-1);
        func_80014C08(20);
    }
    EVO_saveScriptFlags();
    removeFrameCallback((s32)EVO_renderFusion);
    removeFrameCallback((s32)renderSceneModels);
    func_80014C08(1);
    func_80014A00(0x1B);
    freeHeapBlock(EVO_FUSION.cardArchive);
    freeHeapBlock(EVO_SCRIPT->data);
    freeHeapBlocksByTag(0x2C);
    freeHeapBlock(EVO_EFFECT_ARCHIVE);
    freeHeapBlocksByTag(0x7F);
    if (EVO_FUSION.cutscene != 0) {
        func_80014C08(60);
        hideScrollingBackground();
        func_800149B8(0, -1, 0, 0x400, EVO_runFusionCutscene, 0, getCurrentTaskId(), 0, 0);
    } else {
        removeFrameCallback((s32)EVO_drawScreenFlash);
        func_800149B8(0, -1, 0, 0x400, returnToWorldMap, 0, 0, 0, 0);
    }
}

void EVO_clearTextLines(EvoText *slot) {
    s32 i;

    for (i = 0; i < 4; i++) {
        slot->active = 0;
        slot++;
    }
}

EvoText *EVO_allocTextLine(EvoText *slot) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (slot->active == 0) {
            bzero((Scene3D *)slot, 0x3C);
            slot->active = 1;
            if (i == 0) {
                slot->pos = -1;
            } else {
                slot->pos = 0;
            }
            return slot;
        }
        slot++;
    }
    return NULL;
}

s32 EVO_addTextLine(u8 *src) {
    u8 *playerName;
    u8 *cardName;
    EvoText *t;
    u8 *dst;
    s32 i;
    s32 end;

    playerName = (u8 *)PLAYER_PROFILES;
    cardName = EVO_CARDS_BY_ID[EVO_FUSION.result]->name;
    t = EVO_allocTextLine(EVO_TEXT_LINES);
    if (t == NULL) {
        /* Shift-JIS: "the text has run out of lines" */
        printf("\x83" "e\x83L\x83X\x83g\x82\xCC\x8Ds\x90\x94\x82\xAA\x82\xA2\x82\xC1\x82\xCF\x82\xA2\x82\xC9\x82\xC8\x82\xE8\x82\xDC\x82\xB5\x82\xBD\n");
        return -1;
    }
    t->text[0] = '*';
    t->text[1] = 'w';
    t->text[2] = '1';
    dst = t->text + 3;
    while (*src != 0) {
        if (*src < 0x81 || *src > 0x98) {
            if (*src == '*') {
                if (src[1] == 'h') {
                    if (src[2] == '0') {
                        src += 3;
                        for (i = 0; i < 12 && *playerName != 0; i++) {
                            *dst++ = *playerName++;
                        }
                        continue;
                    } else if (src[2] == '1') {
                        src += 3;
                        for (i = 0; i < 21 && *cardName != 0; i++) {
                            *dst++ = *cardName++;
                        }
                        continue;
                    } else if (src[2] == '2' || src[2] == '3') {
                        src += 3;
                        for (i = 0; i < 21 && *cardName != 0; i++) {
                            if (i <= 0) {
                                *dst++ = *cardName++;
                            } else {
                                *dst++ = '?';
                                cardName++;
                                i++;
                            }
                        }
                        continue;
                    }
                }
            } else if (*src == '\\') {
                end = 0;
                for (i = 1; i < 5; i++) {
                    if (end == 0 && src[i] == 0) {
                        end = 1;
                        break;
                    }
                }
                if (end != 1 && src[1] == '0' && src[2] == 'x' && src[3] == '2' && src[4] == '2') {
                    src += 5;
                    *dst++ = '"';
                    continue;
                }
            }
        } else {
            *dst++ = *src++;
            *dst++ = *src++;
            continue;
        }
        *dst++ = *src++;
    }
    *dst = 0;
    dst = t->text;
    for (i = 0; i < 60 && *dst != 0; i++) {
        dst++;
    }
    if (t->pos == -1) {
        t->pos = i;
    }
    if (i >= 60) {
        t->active = 0;
    } else {
        t->active = 1;
    }
    t->len = i;
    for (i = 0; i < 4 && t != EVO_TEXT_LINES; i++) {
        t--;
    }
    return i;
}

const char D_801DF3B0[] = "";

s32 EVO_typeTextLine(s32 x, s32 y, EvoText *t, s32 z) {
    u8 buf[64];
    u8 *dst;
    u8 *src;
    s8 i;
    u8 c;

    dst = buf;
    if (t->len == t->pos) {
        drawText(x, y, (s32)t, 7, z);
        return -1;
    }
    src = t->text;
    for (i = 0; i < t->pos; i++) {
        *dst++ = *src++;
    }
    c = *src;
    if (c == 0) {
        return 1;
    }
    if (*src < 0x81 || *src > 0x98) {
        if (c == '*') {
            switch (src[1]) {
            case 'a':
            case 'b':
            case 'c':
            case 'e':
            case 's':
            case 'w':
                if (src[2] >= '0' && src[2] <= '9') {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    t->pos += 2;
                }
                break;
            }
        }
        *dst = *src;
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        t->pos += 1;
    } else {
        *dst++ = src[0];
        *dst = src[1];
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        t->pos += 2;
    }
    return 1;
}

s32 EVO_typeTextLines(s16 x, s16 y, s32 z) {
    EvoText *t;
    s32 i;

    for (t = EVO_TEXT_LINES, i = 0; i < 4; i++, t++) {
        if (t->active != 0 && EVO_typeTextLine(x, y + i * 12, t, z) == 1) {
            return 1;
        }
    }
    return 0;
}

void EVO_drawMessageWindow(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    EVO_FUSION.textTyping = EVO_typeTextLines(x, y, z);
    if (EVO_FUSION.textTyping == 0 && ((u8)EVO_FUSION.scriptState == 2 || (u8)EVO_FUSION.scriptState == 3)) {
        if (++EVO_FUSION.blinkTimer & 0x10) {
            drawIcon(x + 200, y + 0x25, 0, 0x1B, z);
        }
    } else {
        EVO_FUSION.blinkTimer = 0;
    }
}

void EVO_drawUnitPortrait(UiWindow *w) {
    Rect16 uv;
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    uv.x = 0x28;
    uv.y = EVO_FUSION.unit * 56;
    uv.w = 0x40;
    uv.h = 0x38;
    drawTexturedSprite(x, y, &uv, 0x98, 0x7C18, z, w->brightness, -1);
}

void EVO_drawRankUpBanner(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    if (EVO_RANK_UP_STATE == 2) {
        drawLargeText(x + 1, y + 1, (s32)"RANK MAX!", 7, z);
    } else if (EVO_RANK_UP_STATE == 1) {
        drawLargeText(x + 1, y + 1, (s32)"RANK UP!", 7, z);
    }
}

void EVO_drawReceivedBanner(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    if (EVO_CARD_RECEIVED == 0) {
        drawLargeText(x + 1, y + 1, (s32)"FULL SET!", 7, z);
    } else {
        drawLargeText(x + 1, y + 1, (s32)"RECEIVED!", 7, z);
    }
}

void EVO_tickFusionTypeChoice(void) {
    Rect16 rects[6];
    s32 i;
    s32 dx;
    s32 dy;

    if (EVO_FUSION.swapState == 0) {
        if (PAD_STATES[0]->pressed & 0x5000) {
            playSoundEffect(2);
            if (EVO_FUSION.fusionType == 0) {
                EVO_FUSION.swapState = 1;
            } else if (EVO_FUSION.fusionType == 1) {
                EVO_FUSION.swapState = 2;
            }
        } else if (PAD_STATES[0]->pressed & 0x40) {
            playSoundEffect(0);
            EVO_SCRIPT->vars[8] = 1;
            EVO_SCRIPT->vars[1] = EVO_TYPE_CHOICE.side;
            EVO_FUSION.scriptState = 0;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            playSoundEffect(1);
            EVO_SCRIPT->vars[8] = 2;
            EVO_FUSION.scriptState = 0;
        }
    } else if (EVO_FUSION.swapState == 1) {
        EVO_FUSION.swapTimer++;
        if (EVO_FUSION.swapTimer >= 21) {
            EVO_FUSION.swapTimer = 20;
            EVO_FUSION.swapState = 0;
            EVO_FUSION.fusionType = 1;
        }
    } else if (EVO_FUSION.swapState == 2) {
        EVO_FUSION.swapTimer++;
        if (EVO_FUSION.swapTimer >= 41) {
            EVO_FUSION.swapTimer = 0;
            EVO_FUSION.swapState = 0;
            EVO_FUSION.fusionType = 0;
        }
    }
    EVO_TYPE_CHOICE.side = EVO_FUSION.fusionType;
    for (i = 0; i < 3; i++) {
        EVO_WINDOWS[i].win.brightness = 0x80 - EVO_TYPE_CHOICE.side * 64;
        EVO_WINDOWS[i + 3].win.brightness = EVO_TYPE_CHOICE.side * 64 + 64;
    }
    dy = rsin(EVO_FUSION.swapTimer * 1024 / 20) * 40 / 4096;
    dx = rsin(EVO_FUSION.swapTimer * 2048 / 20) * 60 / 4096;
    for (i = 0; i < 3; i++) {
        rects[i] = EVO_WINDOW_DEFS[i].rect;
        rects[i + 3] = EVO_WINDOW_DEFS[i + 3].rect;
        rects[i].x += dx;
        rects[i].y += dy;
        rects[i + 3].x -= dx;
        rects[i + 3].y -= dy;
        EVO_WINDOWS[i].win.animFrame = EVO_WINDOWS[i].win.animFrames;
        EVO_WINDOWS[i + 3].win.animFrame = EVO_WINDOWS[i + 3].win.animFrames;
        animateWindowTo(&EVO_WINDOWS[i].win, &rects[i]);
        animateWindowTo(&EVO_WINDOWS[i + 3].win, &rects[i + 3]);
    }
    for (i = 0; i < 6; i++) {
        if (i < 3) {
            EVO_WINDOWS[i].z = EVO_TYPE_CHOICE.side + 30;
        } else {
            EVO_WINDOWS[i].z = (EVO_TYPE_CHOICE.side + 30) ^ 1;
        }
    }
}

void EVO_openFusionTypeChoice(void) {
    s32 i;

    EVO_MAX_CARD_LEVEL = EVO_SCRIPT->vars[12];
    for (i = 0; i < 3; i++) {
        EVO_WINDOWS[i].win.brightness = 0x80 - EVO_TYPE_CHOICE.side * 64;
        EVO_WINDOWS[i + 3].win.brightness = EVO_TYPE_CHOICE.side * 64 + 64;
    }
    if (EVO_FUSION.typeChoiceOpen == 0) {
        EVO_FUSION.typeChoiceOpen = 1;
        EVO_FUSION.swapState = 0;
        EVO_FUSION.step = 1;
        EVO_SCRIPT->vars[8] = -1;
        for (i = 0; i < 6; i++) {
            if (EVO_FUSION.fusionType == 0) {
                animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i].rect);
            } else if (i < 3) {
                animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i + 3].rect);
            } else {
                animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i - 3].rect);
            }
        }
        func_80014C08(30);
    }
}

void EVO_closeFusionTypeChoice(void) {
    s32 i;

    if (EVO_FUSION.typeChoiceOpen == 1) {
        EVO_FUSION.typeChoiceOpen = 0;
        EVO_FUSION.step = 0;
        for (i = 0; i < 6; i++) {
            EVO_WINDOWS[i].win.animFrame = EVO_WINDOWS[i].win.animFrames - 1;
            animateWindowTo(&EVO_WINDOWS[i].win, (Rect16 *)-1);
        }
    }
}

void EVO_drawFusionTypeIcon(EvoWindow *w) {
    Rect16 uv;
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;

    if (w->isPartner == 0) {
        uv.x = 0;
        uv.y = 0;
        uv.w = 0x40;
        uv.h = 0x40;
        drawTexturedSprite(x, y, &uv, 0x97, 0x7C58, z, w->win.brightness, -1);
    } else {
        uv.x = 0x40;
        uv.y = 0;
        uv.w = 0x40;
        uv.h = 0x40;
        drawTexturedSprite(x, y, &uv, 0x97, 0x7C58, z, w->win.brightness, -1);
    }
}

void EVO_openPartnerList(void) {
    s32 i;

    if (EVO_FUSION.partnerListOpen == 0) {
        EVO_FUSION.partnerListOpen = 1;
        EVO_FUSION.partnerCount = 0;
        EVO_FUSION.partner = 0;
        for (i = 10; i < 12; i++) {
            animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i].rect);
        }
        for (i = 0; i < 3; i++) {
            if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId != 0) {
                EVO_FUSION.partnerCount++;
            }
        }
    }
    EVO_SCRIPT->vars[8] = -1;
    EVO_FUSION.step = 2;
}

void EVO_closePartnerList(void) {
    s32 i;

    EVO_FUSION.partnerListOpen = 0;
    EVO_FUSION.step = 0;
    for (i = 10; i < 12; i++) {
        animateWindowTo(&EVO_WINDOWS[i].win, (Rect16 *)-1);
    }
}

void EVO_tickPartnerList(void) {
    if (PAD_STATES[0]->pressed & 0x40) {
        playSoundEffect(0);
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 1;
        EVO_FUSION.firstCard = ((PlayerProfile *)PLAYER_PROFILES)->partners[EVO_FUSION.partner].cardId;
    } else if (PAD_STATES[0]->pressed & 0x10) {
        playSoundEffect(1);
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 2;
    } else if (PAD_STATES[0]->repeat & 0x4000) {
        EVO_FUSION.partner++;
        if (EVO_FUSION.partner >= EVO_FUSION.partnerCount) {
            EVO_FUSION.partner = 0;
        } else {
            playSoundEffect(2);
        }
        if (EVO_FUSION.partnerCount == 0 || EVO_FUSION.partner >= EVO_FUSION.partnerCount - 1) {
            PAD_STATES[0]->repeatEnabled = 0;
        }
    } else if (PAD_STATES[0]->repeat & 0x1000) {
        EVO_FUSION.partner--;
        if (EVO_FUSION.partner < 0) {
            EVO_FUSION.partner = EVO_FUSION.partnerCount - 1;
        } else {
            playSoundEffect(2);
        }
        if (EVO_FUSION.partner <= 0) {
            PAD_STATES[0]->repeatEnabled = 0;
        }
    }
}

void EVO_openCardList(void) {
    animateWindowTo(&EVO_WINDOWS[10].win, (Rect16 *)-1);
    animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
    EVO_CARD_LIST_MENU.active = 1;
    EVO_FUSION.sortMenuOpen = 0;
    EVO_FUSION.step = 3;
    EVO_FUSION.previewOpen = 0;
    EVO_CARD_LIST_MENU.row = 0;
    scrollWindowTo((s16 *)EVO_CARD_LIST_MENU.win, 0, 0);
    EVO_SCRIPT->vars[8] = -1;
}

void EVO_closeCardList(void) {
    animateWindowTo(&EVO_WINDOWS[10].win, &EVO_WINDOW_DEFS[10].rect);
    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
    EVO_CARD_LIST_MENU.active = 0;
}

void EVO_slideTrayOut(s32 index) {
    EvoTray *trays = EVO_TRAYS;

    trays[index].x -= 10;
    if (trays[index].x < -0x58) {
        trays[index].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_SCRIPT->vars[8] = -1;
        if (index == 0) {
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.firstCard]--;
        } else {
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.firstCard]++;
        }
    }
}

void EVO_slideTrayIn(s32 index) {
    EvoTray *trays = EVO_TRAYS;

    trays[index].x += 10;
    if (trays[index].x >= 15) {
        trays[index].x = 14;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_SCRIPT->vars[8] = -1;
    }
}

void EVO_showBothTrays(void) {
    EVO_TRAYS[0].x += 10;
    EVO_TRAYS[1].x += 10;
    if (EVO_TRAYS[0].x >= 15) {
        EVO_TRAYS[0].x = 14;
    }
    if (EVO_TRAYS[1].x >= 0x67) {
        EVO_TRAYS[1].x = 0x66;
    }
    if (EVO_TRAYS[0].x == 14 && EVO_TRAYS[1].x == 0x66) {
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 1;
    }
}

void EVO_repickSecondCard(void) {
    EVO_TRAYS[0].x -= 10;
    EVO_TRAYS[1].x -= 10;
    if (EVO_TRAYS[1].x < 14) {
        EVO_TRAYS[0].x = -0x58;
        EVO_TRAYS[1].x = 14;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
        EVO_startSecondCardPick();
    }
}

void EVO_checkCardCapacity(s16 cardId) {
    if (cardId < 0) {
        cardId = -1;
    }
    if (EVO_SCRIPT->vars[13] != 0) {
        EVO_SCRIPT->vars[11] = 0;
        return;
    }
    EVO_SCRIPT->vars[11] = getOwnedCardCount(0, cardId);
    if ((((PlayerProfile *)PLAYER_PROFILES)->cardCollection[cardId] & 7) + 1 >= 7) {
        EVO_SCRIPT->vars[11] = -1;
    } else {
        EVO_SCRIPT->vars[11] = 0;
    }
}

void EVO_tickCardList(void) {
    s16 cardId;
    s32 blocked = 0;
    s32 i;

    cardId = EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->id;
    EVO_CURSOR_CARD = cardId;
    if (PAD_STATES[0]->pressed & 0x100) {
        if (EVO_FUSION.previewOpen != 0) {
            return;
        }
        if (EVO_FUSION.sortMenuOpen == 0) {
            EVO_FUSION.sortMenuOpen = 1;
            EVO_CARD_LIST_MENU.active = 0;
            animateWindowTo(&EVO_SORT_WINDOW, &EVO_SORT_MENU.rect);
            playSoundEffect(3);
            return;
        }
        playSoundEffect(4);
        EVO_FUSION.sortMenuOpen = 0;
        EVO_CARD_LIST_MENU.active = 1;
        animateWindowTo(&EVO_SORT_WINDOW, (Rect16 *)-1);
    } else if (EVO_FUSION.sortMenuOpen == 0) {
        if (EVO_FUSION.previewOpen == 0) {
            if (PAD_STATES[0]->pressed & 0x40) {
                if (EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->fusionPoints == 0) {
                    blocked = 1;
                } else if (EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->type == 0 &&
                           (EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->attr & 0xF) > EVO_MAX_CARD_LEVEL) {
                    blocked = 1;
                }
                if (EVO_SPARE_CARD_COUNTS[cardId] == 0) {
                    return;
                }
                if (cardId >= 0xAC && cardId < 0xBF) {
                    for (i = 0; i < 6; i++) {
                        if (EVO_PARTNER_CARD_IDS[i] == cardId) {
                            i = -1;
                            break;
                        }
                    }
                    if (i == -1) {
                        initDialog((u8 *)&EVO_DIALOG, "You can't use Partner Cards\nin Fusion.", 0);
                    } else {
                        initDialog((u8 *)&EVO_DIALOG, "You can't use that Card in Fusion.", 0);
                    }
                    runDialog(&EVO_DIALOG);
                    return;
                }
                if (blocked) {
                    return;
                }
                playSoundEffect(0);
                if (EVO_FUSION.fusionType == 0) {
                    if (EVO_FUSION.pickSlot != 2) {
                        EVO_CARD_LIST_MENU.active = 0;
                        EVO_SCRIPT->vars[8] = 1;
                        EVO_FUSION.step = 0;
                        EVO_loadCardImage(cardId, 0);
                        EVO_FUSION.firstCard = cardId;
                        EVO_FUSION.busy[1] = 1;
                        return;
                    }
                    EVO_CARD_LIST_MENU.active = 0;
                    EVO_loadCardImage(cardId, 1);
                    do {
                        func_80014C08(1);
                    } while (EVO_FUSION.busy[0] != 0);
                    EVO_FUSION.secondCard = cardId;
                    EVO_findFusionResult();
                    EVO_checkCardCapacity(EVO_FUSION.result);
                    EVO_FUSION.step = 11;
                    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
                    EVO_SPARE_CARD_COUNTS[EVO_FUSION.secondCard]--;
                    EVO_FUSION.busy[2] = 1;
                    EVO_loadCardImage(EVO_FUSION.result, 2);
                    do {
                        func_80014C08(1);
                    } while (EVO_FUSION.busy[0] != 0);
                } else if (EVO_FUSION.fusionType == 1) {
                    EVO_CARD_LIST_MENU.active = 0;
                    uploadTim((u32 *)((u8 *)EVO_FUSION.cardArchive + EVO_FUSION.cardArchive[cardId]), 0x1C0, 0x190, 0x180, 0x1FC);
                    EVO_FUSION.secondCard = cardId;
                    EVO_FUSION.previewOpen = 1;
                    animateWindowTo(&EVO_WINDOWS[12].win, &EVO_WINDOW_DEFS[12].rect);
                }
            } else if (PAD_STATES[0]->pressed & 0x10) {
                EVO_FUSION.previewOpen = 0;
                EVO_FUSION.step = 0;
                EVO_SCRIPT->vars[8] = 2;
                playSoundEffect(1);
            }
        } else if (PAD_STATES[0]->pressed & 0x40) {
            playSoundEffect(0);
            EVO_FUSION.step = 0;
            EVO_SCRIPT->vars[8] = 1;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            playSoundEffect(1);
            EVO_CARD_LIST_MENU.active = 1;
            EVO_FUSION.previewOpen = 0;
            animateWindowTo(&EVO_WINDOWS[12].win, (Rect16 *)-1);
        }
    } else if (PAD_STATES[0]->pressed & 0x10) {
        playSoundEffect(4);
        EVO_FUSION.sortMenuOpen = 0;
        EVO_CARD_LIST_MENU.active = 1;
        animateWindowTo(&EVO_SORT_WINDOW, (Rect16 *)-1);
    }
}

void EVO_startPartnerFusion(void) {
    PlayerProfile *profile;

    EVO_SCRIPT->vars[8] = -2;
    EVO_SCRIPT->vars[18] = 0;
    EVO_SCRIPT->vars[19] = 0;
    EVO_SCRIPT->vars[7] = 1;
    EVO_playEffect(0, 0);
    EVO_findPartnerReward();
    EVO_FUSION.step = 4;
    removeCardFromCollection(0, EVO_FUSION.secondCard, 1);
    EVO_SPARE_CARD_COUNTS[EVO_FUSION.secondCard]--;
    profile = (PlayerProfile *)PLAYER_PROFILES;
    profile->fusionCardsUsed++;
    if ((u16)profile->fusionCardsUsed >= 10000) {
        profile->fusionCardsUsed = 9999;
    }
}

void EVO_cancelPartnerFusion(void) {
    EVO_SCRIPT->vars[8] = -1;
    EVO_CARD_LIST_MENU.active = 1;
    EVO_FUSION.previewOpen = 0;
    animateWindowTo(&EVO_WINDOWS[12].win, (Rect16 *)-1);
    EVO_FUSION.step = 3;
}

void EVO_leaveForCutscene(void) {
    setScreenFadeParams(0, 2, 6);
    func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 1, 6, 0);
    EVO_FUSION.cutscene = 1;
}

void EVO_findPartnerReward(void) {
    s32 i;
    s32 ability;

    EVO_FUSION.partnerKind = -1;
    ability = -1;
    for (i = 0; i < 6; i++) {
        if (EVO_PARTNER_CARD_IDS[i] == EVO_FUSION.firstCard && EVO_FUSION.partnerKind == -1) {
            EVO_FUSION.partnerKind = i;
        }
    }
    if (EVO_FUSION.partnerKind != -1) {
        for (i = 0; i < 5; i++) {
            if (EVO_PARTNER_FUSION_REWARDS[EVO_FUSION.partnerKind][i].card == EVO_FUSION.secondCard && ability == -1) {
                ability = EVO_PARTNER_FUSION_REWARDS[EVO_FUSION.partnerKind][i].ability;
                if (getPartnerAbilityState(0, ability) == 0) {
                    grantPartnerAbility(0, ability);
                    EVO_FUSION.rewardStep = 0;
                } else {
                    ability = -1;
                }
                EVO_FUSION.result = ability;
            }
        }
    }
    if (ability == -1) {
        EVO_FUSION.rewardStep = 3;
        EVO_SCRIPT->vars[19] = ability;
        EVO_FUSION.partnerKind = ability;
        EVO_FUSION.result = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->fusionPoints;
    }
}

void EVO_tickPartnerReward(void) {
    char text[168];
    s32 i;

    switch (EVO_FUSION.rewardStep) {
    case 0:
        EVO_SCRIPT->vars[18] = 2;
        break;
    case 1:
        sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c6[%s]", EVO_FUSION.result, EVO_DIGI_PARTS[EVO_FUSION.result].name);
        initDialog((u8 *)&EVO_DIALOG, text, 0x80);
        runDialog(&EVO_DIALOG);
        EVO_FUSION.rewardStep = 10;
        EVO_FUSION.step = 3;
        EVO_CARD_LIST_MENU.active = 1;
        EVO_FUSION.previewOpen = 0;
        for (i = 0; i < 4; i++) {
            EVO_STAT_BONUSES[i] = 0;
        }
        EVO_SCRIPT->vars[8] = -1;
        break;
    case 2:
        if (EVO_SCRIPT->vars[19] == -1) {
            EVO_addPartnerExp();
        } else if (EVO_FUSION.scriptState == 0) {
            if (EVO_LEVEL_UP_PENDING == 0) {
                if (EVO_NEW_DIGI_PART != -1) {
                    grantPartnerAbility(0, EVO_NEW_DIGI_PART);
                    sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c6[%s]", EVO_NEW_DIGI_PART, EVO_DIGI_PARTS[EVO_NEW_DIGI_PART].name);
                    initDialog((u8 *)&EVO_DIALOG, text, 0x80);
                    runDialog(&EVO_DIALOG);
                }
                EVO_SCRIPT->vars[19] = -1;
                if (EVO_RANK_UP_STATE != 0) {
                    animateWindowTo(&EVO_RANK_UP_WINDOW, (Rect16 *)-1);
                }
            } else {
                EVO_LEVEL_UP_PENDING = 0;
            }
        }
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        break;
    }
}

void EVO_addPartnerExp(void) {
    char text[168];
    Partner *partner;
    s32 ability;
    s32 i;

    partner = &((PlayerProfile *)PLAYER_PROFILES)->partners[EVO_FUSION.partner];
    bzero((Scene3D *)text, 0xA1);
    if (EVO_FUSION.partnerKind >= 0) {
        EVO_SCRIPT->vars[18] = 2;
        sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c7[%s]", EVO_FUSION.result, EVO_DIGI_PARTS[EVO_FUSION.result].name);
        initDialog((u8 *)&EVO_DIALOG, text, 0x80);
        runDialog(&EVO_DIALOG);
        EVO_FUSION.partnerKind = -2;
        return;
    }
    if (EVO_FUSION.partnerKind == -1) {
        if (EVO_FUSION.result > 0) {
            EVO_FUSION.result--;
            if ((s8)partner->level < 99) {
                partner->exp++;
                if (getExpForNextLevel((s8)partner->level) - (u16)partner->exp > 0) {
                    return;
                }
                partner->level++;
                if ((s8)partner->level >= 99) {
                    EVO_FUSION.result = 0;
                }
                ability = findNewPartnerAbility((AbilityLearnEntry *)EVO_DIGI_PARTS, 0, EVO_FUSION.partner);
                EVO_SCRIPT->vars[19] = 1;
                EVO_NEW_DIGI_PART = -1;
                EVO_LEVEL_UP_PENDING = 1;
                EVO_RANK_UP_STATE = 1;
                animateWindowTo(&EVO_RANK_UP_WINDOW, &EVO_RANK_UP_RECT);
                if (ability >= 0) {
                    EVO_SCRIPT->vars[19] = 2;
                    EVO_NEW_DIGI_PART = ability;
                }
                ability = func_8004994C(0, EVO_FUSION.partner);
                if (ability >= 0) {
                    EVO_STAT_BONUSES[ability] += 10;
                }
            } else {
                EVO_RANK_UP_STATE = 2;
                animateWindowTo(&EVO_RANK_UP_WINDOW, &EVO_RANK_UP_RECT);
                do {
                    func_80014C08(1);
                } while (!(PAD_STATES[0]->pressed & 0x40));
                playMenuSound(1);
                animateWindowTo(&EVO_RANK_UP_WINDOW, (Rect16 *)-1);
                EVO_FUSION.result = 0;
            }
        } else {
            for (i = 0; i < 4; i++) {
                if (i == 0) {
                    partner->hpBonus += EVO_STAT_BONUSES[0];
                } else {
                    partner->attackBonus[i - 1] += EVO_STAT_BONUSES[i];
                }
            }
            updatePartnerStats(0, EVO_FUSION.partner);
            EVO_FUSION.partnerKind = -2;
        }
        return;
    }
    EVO_FUSION.rewardStep = 10;
    EVO_FUSION.step = 3;
    EVO_CARD_LIST_MENU.active = 1;
    EVO_FUSION.previewOpen = 0;
    for (i = 0; i < 4; i++) {
        EVO_STAT_BONUSES[i] = 0;
    }
    EVO_SCRIPT->vars[8] = -1;
}

void EVO_tickFusionResult(void) {
    s32 i;

    if (EVO_FUSION.resultStep == 0) {
        EVO_FUSION.resultStep = 1;
        for (i = 0; i < 2; i++) {
            EVO_TRAYS[i].merge = 0;
        }
        EVO_SCRIPT->vars[8] = -1;
        return;
    }
    if (EVO_FUSION.resultStep == 1) {
        if (EVO_FUSION.resultKind == 1) {
            EVO_playEffect(7, 0);
        } else {
            EVO_playEffect(6, 0);
        }
        removeCardFromCollection(0, EVO_FUSION.firstCard, 1);
        removeCardFromCollection(0, EVO_FUSION.secondCard, 1);
        if (addCardToCollection(0, EVO_FUSION.result, 1) >= 0) {
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.result]++;
            EVO_CARD_RECEIVED = 1;
        } else {
            EVO_CARD_RECEIVED = 0;
        }
        ((PlayerProfile *)PLAYER_PROFILES)->fusedCards++;
        if ((u16)((PlayerProfile *)PLAYER_PROFILES)->fusedCards >= 10000) {
            ((PlayerProfile *)PLAYER_PROFILES)->fusedCards = 9999;
        }
        ((PlayerProfile *)PLAYER_PROFILES)->fusionCardsUsed += 2;
        if ((u16)((PlayerProfile *)PLAYER_PROFILES)->fusionCardsUsed >= 10000) {
            ((PlayerProfile *)PLAYER_PROFILES)->fusionCardsUsed = 9999;
        }
        if (EVO_SCRIPT->vars[13] != 0) {
            ((PlayerProfile *)PLAYER_PROFILES)->fusionMutations++;
            if ((u16)((PlayerProfile *)PLAYER_PROFILES)->fusionMutations >= 10000) {
                ((PlayerProfile *)PLAYER_PROFILES)->fusionMutations = 9999;
            }
        }
        if (EVO_FUSION.resultKind == 1) {
            EVO_FUSION.cutscene = 1;
            EVO_CUTSCENE_MODELS[0] = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->modelId;
            EVO_CUTSCENE_MODELS[1] = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->modelId;
            return;
        }
        animateWindowTo(&EVO_WINDOWS[13].win, &EVO_WINDOW_DEFS[13].rect);
        EVO_FUSION.resultStep = 2;
    }
    EVO_TRAYS[0].x = 0x3A;
    EVO_TRAYS[1].x = -0x58;
    EVO_FUSION.scriptState = 0;
    if (PAD_STATES[0]->pressed & 0x40) {
        animateWindowTo(&EVO_WINDOWS[13].win, (Rect16 *)-1);
        playSoundEffect(0);
        EVO_FUSION.step = 0x12;
        EVO_FUSION.resultStep = 3;
    }
}

void EVO_closeFusionResult(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x58) {
        EVO_TRAYS[0].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 1;
        EVO_FUSION.resultStep = 0;
    }
}

void EVO_showCutsceneResult(void) {
    if (EVO_FUSION.cutscene != 0) {
        EVO_playEffect(8, 0);
        EVO_FUSION.cutscene = 0;
        animateWindowTo(&EVO_WINDOWS[13].win, &EVO_WINDOW_DEFS[13].rect);
    }
    if (PAD_STATES[0]->pressed & 0x40) {
        animateWindowTo(&EVO_WINDOWS[13].win, (Rect16 *)-1);
        playSoundEffect(0);
        EVO_FUSION.step = 0x12;
        EVO_FUSION.resultStep = 3;
    }
}

void func_801EBE08(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x58) {
        EVO_TRAYS[0].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_SCRIPT->vars[8] = 1;
    }
}

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

    x = w->originX;
    y = w->originY;
    z = w->z;
    for (i = 0; i < 3; i++) {
        if (PARTNER(i).cardId == 0) {
            continue;
        }
        if (EVO_FUSION.partner == i) {
            dim = 0;
            level = 0; /* dead store, but it keeps the branch the original has */
        } else {
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
        drawLargeTextColored(x + 0x82, y + 4, "NEXT", 6, EVO_TEXT_COLORS[dim].b, z);
        next = 0;
        if ((s8)PARTNER(i).level < 99) {
            next = getExpForNextLevel((s8)PARTNER(i).level) - (u16)PARTNER(i).exp;
        }
        sprintf(text, "*s0%3d", next);
        drawTextColored(x + 0xB0, y + 2, text, EVO_TEXT_COLORS[dim].b, 8, z);
        y += 2;
        drawLargeTextColored(x + 0x2C, y + 0x10, "RANK", 6, EVO_TEXT_COLORS[dim].b, z);
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
        drawLargeTextColored(x + 0x2C, y + 0x1C, "EXP", 6, EVO_TEXT_COLORS[dim].b, z);
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

/* the last two bytes are leftovers in the original, not zero padding */
const char EVO_STR_SPEC[8] = "Spec.\0\x85\xA4";

void (*EVO_WINDOW_DRAW_FUNCS[14])() = {
    EVO_drawFusionTypeIcon, EVO_drawFusionTypeTitle, EVO_drawFusionTypeHelp, EVO_drawFusionTypeIcon, EVO_drawFusionTypeTitle, EVO_drawFusionTypeHelp, EVO_drawUnitPortrait,
    EVO_drawMessageWindow, EVO_drawEmptyWindow, EVO_drawEmptyWindow, EVO_drawPartnerList, EVO_drawPartnerStatus, EVO_drawCardInfo, EVO_drawReceivedBanner,
};

void EVO_resetFusion(void) {
    EVO_FUSION.secondCard = -1;
    EVO_FUSION.firstCard = -1;
    EVO_FUSION.hideResult = 1;
    EVO_FUSION.scriptState = 4;
    EVO_FUSION.sortMenuOpen = 0;
    EVO_FUSION.previewOpen = 0;
    EVO_FUSION.pickSlot = 1;
    EVO_FUSION.busy[1] = 0;
    EVO_FUSION.busy[2] = 0;
}

void EVO_cancelFirstCard(void) {
    EVO_FUSION.pickSlot = 1;
    EVO_FUSION.step = 8;
    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
    EVO_CARD_LIST_MENU.active = 0;
}

void EVO_startSecondCardPick(void) {
    EVO_FUSION.pickSlot = 2;
    EVO_SCRIPT->vars[8] = -1;
    EVO_FUSION.step = 7;
}

s16 EVO_findFusionResult(void) {
    s8 types[2];
    s8 i;
    s8 j;
    s8 level;
    s8 kind;

    EVO_FUSION.result = -1;
    level = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->fusionPoints + EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->fusionPoints;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            types[0] = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->type;
        } else {
            types[i] = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->type;
        }
        switch (types[i]) {
        case 0:
            if (i == 0) {
                types[0] = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->attr >> 4;
            } else {
                types[i] = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->attr >> 4;
            }
            break;
        case 1:
        case 2:
            types[i] = 5;
            break;
        }
    }
    kind = EVO_FUSION_RESULT_TYPES[types[0]][types[1]];
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 20; i++) {
            if (EVO_FUSION_RECIPES[i][j] == EVO_FUSION.firstCard && EVO_FUSION_RECIPES[i][1 - j] == EVO_FUSION.secondCard) {
                EVO_FUSION.result = EVO_FUSION_RECIPES[i][2];
                EVO_CUTSCENE_MODELS[2] = EVO_FUSION_RECIPES[i][3];
            }
        }
    }
    EVO_SCRIPT->vars[14] = 0;
    EVO_SCRIPT->vars[13] = 0;
    if (EVO_FUSION.result == -1) {
        EVO_FUSION.roll = rand() % 100;
        if (EVO_FUSION.roll < 0) {
            EVO_FUSION.roll = EVO_FUSION.roll * -1;
        }
        if (EVO_FUSION.roll <= (s8)(level / 10)) {
            EVO_SCRIPT->vars[13] = 1;
            EVO_FUSION.resultKind = 2;
            EVO_FUSION.roll = rand() % 100;
            if (EVO_FUSION.roll < 0) {
                EVO_FUSION.roll = EVO_FUSION.roll * -1;
            }
            if (EVO_FUSION.roll < 21) {
                EVO_FUSION.result = rand() % 12 + 0x111;
                if (getOwnedCardCount(0, EVO_FUSION.result) >= 6) {
                    EVO_FUSION.result = 200;
                }
            } else if (EVO_FUSION.roll < 61) {
                EVO_FUSION.result = 200;
            } else {
                EVO_FUSION.roll = rand() % 3 + 1;
                EVO_findCardOfLevel(kind, level + EVO_FUSION.roll, &EVO_FUSION.result, EVO_FUSION.firstCard, EVO_FUSION.secondCard);
                if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                    EVO_FUSION.result = -1;
                }
                for (i = 0; EVO_FUSION.result == -1;) {
                    i++;
                    EVO_findCardOfLevel(kind, level + EVO_FUSION.roll + i, &EVO_FUSION.candidates[0], 500, 500);
                    EVO_findCardOfLevel(kind, level + EVO_FUSION.roll - i, &EVO_FUSION.candidates[1], 500, 500);
                    if (EVO_FUSION.candidates[0] != -1 && EVO_FUSION.candidates[1] != -1) {
                        EVO_FUSION.result = EVO_FUSION.candidates[rand() % 2];
                    } else if (EVO_FUSION.candidates[0] != -1) {
                        EVO_FUSION.result = EVO_FUSION.candidates[0];
                    } else if (EVO_FUSION.candidates[1] != -1) {
                        EVO_FUSION.result = EVO_FUSION.candidates[1];
                    }
                    if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                        EVO_FUSION.result = -1;
                    }
                }
            }
        } else {
            EVO_findCardOfLevel(kind, level, &EVO_FUSION.result, EVO_FUSION.firstCard, EVO_FUSION.secondCard);
            if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                EVO_FUSION.result = -1;
            }
            for (i = 0; EVO_FUSION.result == -1;) {
                i++;
                EVO_findCardOfLevel(kind, level + i, &EVO_FUSION.candidates[0], 500, 500);
                EVO_findCardOfLevel(kind, level - i, &EVO_FUSION.candidates[1], 500, 500);
                if (EVO_FUSION.candidates[0] != -1 && EVO_FUSION.candidates[1] != -1) {
                    EVO_FUSION.result = EVO_FUSION.candidates[rand() % 2];
                } else if (EVO_FUSION.candidates[0] != -1) {
                    EVO_FUSION.result = EVO_FUSION.candidates[0];
                } else if (EVO_FUSION.candidates[1] != -1) {
                    EVO_FUSION.result = EVO_FUSION.candidates[1];
                }
                if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                    EVO_FUSION.result = -1;
                }
            }
            EVO_FUSION.resultKind = 0;
        }
    } else {
        EVO_FUSION.resultKind = 1;
        EVO_SCRIPT->vars[14] = 1;
    }
    switch (EVO_CARDS_BY_ID[EVO_FUSION.result]->type) {
    case 0:
        EVO_SCRIPT->vars[15] = EVO_CARDS_BY_ID[EVO_FUSION.result]->attr & 0xF;
        EVO_SCRIPT->vars[16] = EVO_CARDS_BY_ID[EVO_FUSION.result]->attr >> 4;
        break;
    case 1:
        EVO_SCRIPT->vars[15] = 5;
        EVO_SCRIPT->vars[16] = 5;
        break;
    case 2:
        EVO_SCRIPT->vars[15] = 6;
        EVO_SCRIPT->vars[16] = 6;
        break;
    }
    EVO_SCRIPT->vars[17] = EVO_FUSION.resultKind;
    EVO_SCRIPT->vars[11] = EVO_FUSION.result;
    return EVO_FUSION.result;
}

void EVO_findCardOfLevel(u8 type, u8 level, s16 *out, s16 excludeA, s16 excludeB) {
    s16 candidates[10];
    s16 i;
    s16 j;
    s16 tmp;
    s8 count;

    *out = -1;
    if (level < 2) {
        level = 2;
    }
    if (level >= 36) {
        level = 35;
    }
    count = 0;
    for (i = 0; i < 10; i++) {
        candidates[i] = -1;
    }
    if (type == 5) {
        for (i = EVO_CARD_ID_RANGES[5].first; i <= EVO_CARD_ID_RANGES[5].last; i++) {
            if (EVO_CARDS_BY_ID[i]->level == level && i != excludeA && i != excludeB) {
                candidates[count] = i;
                count++;
            }
        }
        for (i = EVO_CARD_ID_RANGES[6].first; i <= EVO_CARD_ID_RANGES[6].last; i++) {
            if (EVO_CARDS_BY_ID[i]->level == level && i != excludeA && i != excludeB) {
                candidates[count] = i;
                count++;
            }
        }
        for (i = 0; i < count; i++) {
            j = rand() % count;
            tmp = candidates[i];
            candidates[i] = candidates[j];
            candidates[j] = tmp;
        }
        *out = candidates[0];
    } else {
        for (i = EVO_CARD_ID_RANGES[type].first; i <= EVO_CARD_ID_RANGES[type].last; i++) {
            if (EVO_CARDS_BY_ID[i]->level == level) {
                *out = i;
                return;
            }
        }
    }
}

void EVO_uploadShadedClut(EvoClut *clut, u16 flags) {
    u16 *src;
    u16 *dst;
    s32 brighten;
    s32 level;
    s32 i;
    s32 color;
    s32 r;
    s32 g;
    s32 b;

    brighten = clut->brighten;
    level = clut->level;
    src = clut->src;
    dst = clut->dst;
    for (i = 0; i < clut->rect.w * clut->rect.h; i++) {
        color = *src++;
        if (color != 0) {
            r = color & 0x1F;
            g = (color >> 5) & 0x1F;
            b = (color >> 10) & 0x1F;
            if (brighten == 0) {
                r = r * level / 255;
                g = g * level / 255;
                b = b * level / 255;
            } else {
                r += (31 - r) * level / 255;
                g += (31 - g) * level / 255;
                b += (31 - b) * level / 255;
            }
            if (r < 0) {
                r = 0;
            } else if (r >= 32) {
                r = 31;
            }
            if (g < 0) {
                g = 0;
            } else if (g >= 32) {
                g = 31;
            }
            if (b < 0) {
                b = 0;
            } else if (b >= 32) {
                b = 31;
            }
            color = flags | (color & 0x8000) | (b << 10) | (g << 5) | r;
        }
        *dst++ = color;
    }
    LoadImage((s16 *)&clut->rect, (s32)clut->dst);
    DrawSync(0);
}
