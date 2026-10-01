#include "common.h"
#include "game.h"
#include "dcb/sub_partner.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/card_db.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/heap.h"
#include "dcb/vram_upload.h"
#include "dcb/menu.h"
#include "dcb/memcard.h"
#include "dcb/battle_hud.h"
#include "dcb/partner_level.h"
#include "dcb/scroll_bg.h"
#include "dcb/frame_callback.h"
#include "dcb/dialog.h"
#include "dcb/subseg.h"

typedef struct {
    char *name;
    u8 unk4[8];
} AbilityText;

extern s32 SUB_PARTNER_TITLE_SHOWN;
extern s32 SUB_PARTNER_TITLE_Y;
extern TabWindow SUB_PARTNER_TABS[3];
extern s32 SUB_PARTNER_SLOT;
extern UiWindow SUB_ARMOR_CHANGE_WINDOW;
extern UiWindow SUB_ARMOR_WINDOW;
extern UiWindow SUB_PARTNER_WINDOW;
extern UiWindow SUB_ABILITY_WINDOW;
extern UiWindow SUB_EQUIPMENT_WINDOW;
extern s32 SUB_PARTNER_PLAYER;
extern s8 SUB_ABILITY_IDS[];
extern Partner SUB_SAVED_PARTNER;
extern Partner SUB_UNEQUIPPED_PARTNER;
extern Partner SUB_PREVIEW_PARTNER;
extern s32 SUB_ARMOR_INDEX;
extern CursorHighlight SUB_ABILITY_CURSOR;
extern CursorHighlight SUB_EQUIPMENT_CURSOR;
extern s8 SUB_PARTNER_WINDOW_ANIM_DONE;
extern s8 SUB_ARMOR_WINDOW_ANIM_DONE;

void SUB_drawPartnerTab(TabWindow *window);

/* The texts SUB_drawPartnerTab shares with the details windows; GCC keeps
   one copy of each, emitted with SUB_drawPartnerTab, the first function
   that uses them */
#define SUB_STR_STAT "*s0%4d"
#define SUB_STR_CROSS_EFFECT "(%s)"
#define SUB_STR_RANK "RANK \f\a%3d"
#define SUB_STR_NEXT "NEXT \f\a%3d"
#define SUB_STR_PARTNER_SUPPORT "Support Effect"
#define SUB_STR_NO_DATA "No Data"

/* the partner abilities; the code here only reads their texts */
AbilityText SUB_ABILITY_TEXTS[128] = {
    { "HP+50.", { 3, 5, 1, 99, 3, 7, 0, 0 } },
    { "HP+100.", { 17, 16, 8, 12, 14, 19, 0, 0 } },
    { "HP+150.", { 29, 32, 19, 25, 32, 33, 0, 0 } },
    { "HP+200.", { 45, 48, 31, 40, 52, 59, 0, 0 } },
    { "HP+300.", { 61, 67, 51, 57, 68, 78, 0, 0 } },
    { "HP+400.", { 96, 89, 71, 81, 91, 88, 0, 0 } },
    { "HP+500.", { 99, 0xFF, 75, 91, 0xFF, 95, 0, 0 } },
    { "All Attack Powers +50.", { 18, 39, 54, 30, 57, 28, 0, 0 } },
    { "All Attack Powers +100.", { 39, 86, 72, 50, 89, 51, 0, 0 } },
    { "All Attack Powers +200.", { 75, 0xFF, 90, 93, 0xFF, 84, 0, 0 } },
    { "*b0 Attack Power +100.", { 1, 9, 15, 5, 4, 2, 0, 0 } },
    { "*b0 Attack Power +150.", { 10, 21, 38, 22, 16, 15, 0, 0 } },
    { "*b0 Attack Power +200.", { 27, 54, 62, 41, 39, 30, 0, 0 } },
    { "*b0 Attack Power +250.", { 48, 0xFF, 78, 61, 62, 61, 0, 0 } },
    { "*b0 Attack Power +300.", { 67, 0xFF, 0xFF, 85, 82, 79, 0, 0 } },
    { "*b1 Attack Power +50.", { 4, 1, 12, 2, 6, 13, 0, 0 } },
    { "*b1 Attack Power +100.", { 12, 7, 22, 16, 18, 29, 0, 0 } },
    { "*b1 Attack Power +150.", { 32, 28, 45, 36, 45, 43, 0, 0 } },
    { "*b1 Attack Power +200.", { 51, 44, 0xFF, 56, 67, 66, 0, 0 } },
    { "*b1 Attack Power +250.", { 77, 0xFF, 0xFF, 76, 86, 96, 0, 0 } },
    { "*b2 Attack Power +50.", { 6, 6, 2, 4, 10, 1, 0, 0 } },
    { "*b2 Attack Power +100.", { 19, 36, 20, 19, 27, 9, 0, 0 } },
    { "*b2 Attack Power +150.", { 35, 69, 42, 38, 53, 24, 0, 0 } },
    { "*b2 Attack Power +200.", { 82, 0xFF, 63, 67, 0xFF, 48, 0, 0 } },
    { "*b0 to 0, *b2 Attack Power -100.", { 24, 12, 4, 14, 0xFF, 34, 0, 0 } },
    { "*b1 to 0, *b2 Attack Power -100.", { 46, 23, 9, 35, 0xFF, 17, 0, 0 } },
    { "*b2 to 0, *b2 Attack Power -100.", { 58, 71, 36, 10, 0xFF, 71, 0, 0 } },
    { "Counter *b0,*b2 Attack Power to 0.", { 11, 26, 0xFF, 0xFF, 40, 62, 0, 0 } },
    { "Counter *b1,*b2 Attack Power to 0.", { 36, 14, 0xFF, 0xFF, 24, 49, 0, 0 } },
    { "Counter *b2,*b2 Attack Power to 0.", { 40, 57, 0xFF, 0xFF, 11, 21, 0, 0 } },
    { "Opponent *a0 X3, *b2 Attack Power -200.", { 87, 92, 39, 69, 31, 50, 0, 0 } },
    { "Opponent *a1 X3, *b2 Attack Power -200.", { 25, 97, 0xFF, 42, 77, 73, 0, 0 } },
    { "Opponent *a2 X3, *b2 Attack Power -200.", { 37, 0xFF, 66, 0xFF, 83, 4, 0, 0 } },
    { "Opponent *a3 X3, *b2 Attack Power -200.", { 68, 33, 46, 21, 5, 97, 0, 0 } },
    { "Opponent *a4 X3, *b2 Attack Power -200.", { 52, 41, 6, 0xFF, 0xFF, 25, 0, 0 } },
    { "1st Attack, *b2 Attack Power -200.", { 76, 50, 95, 62, 0xFF, 0xFF, 0, 0 } },
    { "Jamming Support, *b2 Attack Power -100.", { 0xFF, 74, 27, 0xFF, 54, 10, 0, 0 } },
    { "Eat-up HP, *b2 Attack Power -200.", { 0xFF, 87, 50, 0xFF, 73, 0xFF, 0, 0 } },
    { "Add + 10 DP.", { 42, 4, 61, 1, 9, 0xFF, 0, 0 } },
    { "Add + 20 DP.", { 91, 29, 79, 27, 21, 0xFF, 0, 0 } },
    { "Add + 30 DP.", { 0xFF, 98, 97, 60, 78, 85, 0, 0 } },
    { "Boost Attack Power +50.", { 2, 10, 13, 3, 7, 11, 0, 0 } },
    { "Boost Attack Power +100.", { 21, 42, 56, 33, 36, 35, 0, 0 } },
    { "Boost Attack Power +200.", { 26, 81, 73, 46, 84, 44, 0, 0 } },
    { "Boost Attack Power +300.", { 69, 0xFF, 84, 82, 0xFF, 80, 0, 0 } },
    { "Attack Power is Doubled.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Boost *b0 Attack Power +300.", { 8, 30, 47, 17, 26, 12, 0, 0 } },
    { "Boost *b0 Attack Power +400.", { 34, 77, 74, 47, 46, 39, 0, 0 } },
    { "Boost *b0 Attack Power +500.", { 55, 0xFF, 88, 88, 66, 74, 0, 0 } },
    { "*b0 Attack Power is Doubled.", { 13, 64, 67, 58, 33, 63, 0, 0 } },
    { "*b0 Attack Power is Tripled.", { 59, 0xFF, 94, 74, 0xFF, 86, 0, 0 } },
    { "Boost *b1 Attack Power +200.", { 15, 2, 0xFF, 7, 22, 18, 0, 0 } },
    { "Boost *b1 Attack Power +300.", { 28, 19, 0xFF, 23, 41, 36, 0, 0 } },
    { "Boost *b1 Attack Power +400.", { 43, 70, 0xFF, 92, 59, 53, 0, 0 } },
    { "*b1 Attack Power is Doubled.", { 22, 13, 59, 34, 0xFF, 67, 0, 0 } },
    { "*b1 Attack Power is Tripled.", { 83, 0xFF, 91, 94, 0xFF, 89, 0, 0 } },
    { "Boost *b2 Attack Power +100.", { 23, 40, 10, 18, 0xFF, 8, 0, 0 } },
    { "Boost *b2 Attack Power +200.", { 38, 58, 40, 43, 0xFF, 20, 0, 0 } },
    { "Boost *b2 Attack Power +300.", { 71, 0xFF, 60, 63, 0xFF, 90, 0, 0 } },
    { "*b2 Attack Power is Doubled.", { 44, 72, 48, 51, 0xFF, 40, 0, 0 } },
    { "*b2 Attack Power is Tripled.", { 88, 0xFF, 85, 95, 0xFF, 65, 0, 0 } },
    { "Attack Power becomes same as HP.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Get 1st Attack.", { 53, 55, 98, 71, 85, 0xFF, 0, 0 } },
    { "Attack becomes Eat-up HP.", { 0xFF, 93, 80, 0xFF, 61, 55, 0, 0 } },
    { "Lower Opponent's *b0 Attack Power to 0.", { 54, 0xFF, 17, 0xFF, 28, 75, 0, 0 } },
    { "Lower Opponent's *b1 Attack Power to 0.", { 84, 24, 28, 0xFF, 15, 0xFF, 0, 0 } },
    { "Lower Opponent's *b2 Attack Power to 0.", { 0xFF, 66, 24, 0xFF, 13, 45, 0, 0 } },
    { "*b0 Counterattack (Attack 2nd).", { 7, 0xFF, 33, 0xFF, 47, 14, 0, 0 } },
    { "*b1 Counterattack (Attack 2nd).", { 33, 17, 0xFF, 0xFF, 37, 26, 0, 0 } },
    { "*b2 Counterattack (Attack 2nd).", { 47, 43, 5, 0xFF, 0xFF, 37, 0, 0 } },
    { "If *a0 Opponent, X2 own Attack Power.", { 74, 79, 0xFF, 77, 25, 38, 0, 0 } },
    { "If *a0 Opponent, X3 own Attack Power.", { 0xFF, 91, 0xFF, 65, 74, 82, 0, 0 } },
    { "If *a1 Opponent, X2 own Attack Power.", { 5, 47, 64, 26, 80, 76, 0, 0 } },
    { "If *a1 Opponent, X3 own Attack Power.", { 60, 0xFF, 89, 83, 98, 91, 0, 0 } },
    { "If *a2 Opponent, X2 own Attack Power.", { 50, 27, 58, 59, 42, 6, 0, 0 } },
    { "If *a2 Opponent, X3 own Attack Power.", { 79, 0xFF, 93, 0xFF, 93, 70, 0, 0 } },
    { "If *a3 Opponent, X2 own Attack Power.", { 56, 37, 44, 6, 19, 93, 0, 0 } },
    { "If *a3 Opponent, X3 own Attack Power.", { 92, 60, 87, 79, 63, 0xFF, 0, 0 } },
    { "If *a4 Opponent, X2 own Attack Power.", { 78, 76, 26, 72, 69, 46, 0, 0 } },
    { "If *a4 Opponent, X3 own Attack Power.", { 89, 96, 55, 0xFF, 0xFF, 68, 0, 0 } },
    { "Change own Specialty to *a0.", { 9, 35, 0xFF, 48, 96, 0xFF, 0, 0 } },
    { "Change own Specialty to *a1.", { 0xFF, 99, 30, 87, 58, 0xFF, 0, 0 } },
    { "Change own Specialty to *a2.", { 97, 3, 41, 8, 12, 94, 0, 0 } },
    { "Change own Specialty to *a3.", { 30, 0xFF, 68, 98, 81, 3, 0, 0 } },
    { "Change own Specialty to *a4.", { 95, 82, 14, 52, 76, 99, 0, 0 } },
    { "Switch Opponent's Specialty to own.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Swap Specialty with Opponent's.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *a0 Opponent, lower its AP to 0.", { 66, 52, 0xFF, 97, 43, 0xFF, 0, 0 } },
    { "If *a1 Opponent, lower its AP to 0.", { 41, 61, 34, 84, 8, 0xFF, 0, 0 } },
    { "If *a2 Opponent, lower its AP to 0.", { 0xFF, 11, 52, 0xFF, 64, 22, 0, 0 } },
    { "If *a3 Opponent, lower its AP to 0.", { 0xFF, 25, 37, 13, 2, 72, 0, 0 } },
    { "If *a4 Opponent, lower its AP to 0.", { 0xFF, 75, 16, 0xFF, 34, 52, 0, 0 } },
    { "Reduce both Players' Atk Pwr to 0.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *e3, boost Attack Power +200.", { 14, 31, 0xFF, 9, 48, 23, 0, 0 } },
    { "If *e4, boost Attack Power +300.", { 31, 53, 0xFF, 20, 38, 57, 0, 0 } },
    { "If *e5, boost Attack Power +400.", { 57, 0xFF, 81, 70, 95, 77, 0, 0 } },
    { "Opponent uses *b0 Attack.", { 16, 62, 0xFF, 28, 97, 31, 0, 0 } },
    { "Opponent uses *b1 Attack.", { 62, 18, 11, 39, 0xFF, 47, 0, 0 } },
    { "Opponent uses *b2 Attack.", { 94, 8, 21, 0xFF, 44, 58, 0, 0 } },
    { "Opponent uses same Attack.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Recover HP +200.", { 72, 15, 32, 11, 1, 81, 0, 0 } },
    { "Recover HP +300.", { 90, 38, 53, 31, 17, 0xFF, 0, 0 } },
    { "Recover HP +400.", { 0xFF, 68, 92, 73, 55, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +400.", { 93, 20, 43, 15, 23, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +600.", { 0xFF, 46, 86, 44, 60, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +500.", { 98, 59, 23, 37, 29, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +700.", { 0xFF, 83, 69, 53, 56, 0xFF, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 300.", { 49, 34, 0xFF, 24, 30, 42, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 600.", { 0xFF, 63, 0xFF, 49, 49, 64, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 1000.", { 0xFF, 94, 0xFF, 64, 65, 98, 0, 0 } },
    { "Drop 1 Card in Opponent's Hand.", { 63, 22, 3, 45, 75, 27, 0, 0 } },
    { "Drop 2 Cards in Opponent's Hand.", { 0xFF, 78, 25, 0xFF, 92, 56, 0, 0 } },
    { "Drop Opponent's Top 2 DP Cards shown.", { 0xFF, 56, 18, 86, 35, 5, 0, 0 } },
    { "Drop Opponent's Top 3 DP Cards shown.", { 0xFF, 88, 49, 0xFF, 90, 32, 0, 0 } },
    { "Drop Opponent's Top 4 DP Cards shown.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Drop 2 Cards in Opponent's Online Deck.", { 0xFF, 51, 7, 29, 0xFF, 16, 0, 0 } },
    { "Drop 3 Cards in Opponent's Online Deck.", { 0xFF, 95, 35, 89, 0xFF, 54, 0, 0 } },
    { "Move Offline Top Card to Online Deck.", { 86, 0xFF, 70, 78, 50, 0xFF, 0, 0 } },
    { "Void Opponent's Support Effect.", { 0xFF, 84, 65, 0xFF, 87, 69, 0, 0 } },
    { "Draw until there are 4 Cards.", { 64, 45, 29, 54, 20, 0xFF, 0, 0 } },
    { "Draw Online Partner Card, then Shuffle.", { 65, 80, 82, 68, 72, 0xFF, 0, 0 } },
    { "If *e3, HP + 200 & all Attack Powers +100.", { 73, 73, 0xFF, 55, 94, 83, 0, 0 } },
    { "If *ea, HP + 200 & all Attack Powers +100.", { 81, 0xFF, 76, 66, 79, 87, 0, 0 } },
    { "Boost Battle Experience by 10%.", { 20, 49, 57, 32, 71, 41, 0, 0 } },
    { "Boost Battle Experience by 20%.", { 85, 65, 77, 75, 88, 92, 0, 0 } },
    { "Boost Battle Experience by 30%.", { 80, 0xFF, 96, 90, 99, 60, 0, 0 } },
    { "Rare Card might appear after battle.", { 70, 85, 83, 80, 51, 0xFF, 0, 0 } },
    { "Rare Card even more likely to appear.", { 0xFF, 90, 99, 96, 70, 0xFF, 0, 0 } },
};

Menu SUB_ABILITY_MENU = { NULL, NULL, { 12, 184, 288, 42 }, 0, -1, 0, -1, 0xa, 0x21, 264, 12, 1, 128, 24, 1, 0, 14, 0, 0, 0 };
Menu SUB_EQUIPMENT_MENU = { NULL, NULL, { 12, 184, 288, 42 }, 0, -1, 0, -1, 0x8, 0x21, 264, 12, 1, 3, 26, 1, 0, 14, 0, 0, 0 };
Rect16 SUB_PARTNER_WINDOW_RECT = { 12, 48, 192, 122 };
Rect16 SUB_ARMOR_WINDOW_RECT = { 214, 48, 88, 122 };
Rect16 SUB_ARMOR_CHANGE_WINDOW_RECT = { 146, 19, 157, 14 };

/* the icons of each partner's armors */
u8 SUB_ARMOR_ICONS[6][3] = {
    { 1, 2, 9 },
    { 3, 5, 0 },
    { 4, 6, 0 },
    { 7, 6, 0 },
    { 8, 1, 0 },
    { 1, 7, 0 },
};

/* not referenced by any code */
#if VERSION_EU
s16 D_801F1FE2 = 0x380;
#elif VERSION_US
s16 D_801F1FE2 = 1;
#else
#error "subseg/partner/sub_partner: version not checked"
#endif

void SUB_drawPartnerTitle(void) {
    if (isSpritePoolFull() == 0) {
        if (SUB_PARTNER_TITLE_SHOWN != 0) {
            SUB_PARTNER_TITLE_Y += 4;
            if (SUB_PARTNER_TITLE_Y > 8) {
                SUB_PARTNER_TITLE_Y = 8;
            }
        } else {
            SUB_PARTNER_TITLE_Y -= 4;
            if (SUB_PARTNER_TITLE_Y < -0x20) {
                SUB_PARTNER_TITLE_Y = -0x20;
            }
        }
        CUR_SPRT->sp.x0 = 6;
        CUR_SPRT->sp.y0 = SUB_PARTNER_TITLE_Y;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0x40;
        CUR_SPRT->sp.clut = 0x7C00;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x20;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setlen(&CUR_SPRT->dm, 1);
        CUR_SPRT->dm.code[0] = 0xE1000005;
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

s32 SUB_getStatPalette(s32 a, s32 b) {
    s32 result = 7;

    if (a < b) {
        result = 5;
    }
    if (b < a) {
        result = 2;
    }
    return result;
}

void SUB_drawPartnerPortrait(s32 player, s32 slot, s32 x, s32 y, s32 brightness, s32 otIndex) {
    s32 specialty;

    if (PLAYER_DATA(player).partners[slot].cardId == 0) {
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 8;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = 0x7FA0;
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = brightness;
            CUR_SPRT->sp.g0 = brightness;
            CUR_SPRT->sp.b0 = brightness;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    } else {
        specialty = PLAYER_DATA(player).partners[slot].card[0].attr >> 4;
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0x30;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = getClut(0x200, specialty + 0x1F8);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = brightness;
            CUR_SPRT->sp.g0 = brightness;
            CUR_SPRT->sp.b0 = brightness;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            if (isSpritePoolFull() == 0) {
                CUR_SPRT->sp.x0 = x;
                CUR_SPRT->sp.y0 = y + 4;
                CUR_SPRT->sp.u0 = 0;
                CUR_SPRT->sp.v0 = getSlotPartnerIndex(player, slot) * 41;
                CUR_SPRT->sp.clut = getClut(0x240, specialty | 0x1F0);
                CUR_SPRT->sp.w = 40;
                CUR_SPRT->sp.h = 40;
                setSemiTrans(&CUR_SPRT->sp, 0);
                CUR_SPRT->sp.r0 = brightness;
                CUR_SPRT->sp.g0 = brightness;
                CUR_SPRT->sp.b0 = brightness;
                setDrawMode(&CUR_SPRT->dm, 0, 0, 0x97);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
                SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            }
        }
    }
}

void SUB_drawArmorPortrait(s32 player, s32 slot, s32 x, s32 y, s32 otIndex) {
    s32 specialty;

    if (PLAYER_DATA(player).partners[slot].armorCardId == 0) {
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 8;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = 0x7FA0;
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    } else {
        specialty = ((DigimonCardData *)DIGIMON_CARDS)[PLAYER_DATA(player).partners[slot].armorCardId].attr >> 4;
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0x30;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = getClut(0x200, specialty + 0x1F8);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            if (isSpritePoolFull() == 0) {
                CUR_SPRT->sp.x0 = x;
                CUR_SPRT->sp.y0 = y + 4;
                CUR_SPRT->sp.u0 = (getSelectedArmorIndex(player, getSlotPartnerIndex(player, slot)) + 1) * 44;
                CUR_SPRT->sp.v0 = getSlotPartnerIndex(player, slot) * 41;
                CUR_SPRT->sp.clut = getClut(0x240, specialty | 0x1F0);
                CUR_SPRT->sp.w = 40;
                CUR_SPRT->sp.h = 40;
                setSemiTrans(&CUR_SPRT->sp, 0);
                CUR_SPRT->sp.r0 = 0x80;
                CUR_SPRT->sp.g0 = 0x80;
                CUR_SPRT->sp.b0 = 0x80;
                setDrawMode(&CUR_SPRT->dm, 0, 0, 0x97);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
                SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            }
        }
    }
}

void SUB_drawPartnerTab(TabWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    u8 rgb[4];
    s32 x;
    s32 y;
    s32 z;
    s32 player;
    s32 slot;
    s32 changed;
    s32 palette;
    s32 next;
    s32 i; /* also the cross effect and the support icon */
    s32 partner;

    rgb[0] = window->window.brightness;
    rgb[1] = window->window.brightness;
    rgb[2] = window->window.brightness;
    x = window->window.originX;
    y = window->window.originY;
    z = window->window.z;
    player = SUB_PARTNER_PLAYER;
    slot = window->slot;
    SUB_drawPartnerPortrait(player, slot, x + 1, y + 12, window->window.brightness, z);
    if (PLAYER_DATA(player).partners[slot].cardId != 0) {
        SUB_SAVED_PARTNER = PLAYER_DATA(player).partners[slot];
        for (i = 0; i < 3; i++) {
            PLAYER_DATA(player).partners[slot].equippedAbilities[i] = -1;
        }
        updatePartnerStats(player, slot);
        SUB_UNEQUIPPED_PARTNER = PLAYER_DATA(player).partners[slot];
        PLAYER_DATA(player).partners[slot] = SUB_SAVED_PARTNER;
        changed = updatePartnerStats(player, slot);
        for (i = 0; i < 3; i++) {
            if (PLAYER_DATA(player).partners[slot].unlockedArmors[i] != 0) {
                partner = getSlotPartnerIndex(player, slot);
                drawIconColored(x + 2 + i * 13, y, 0, SUB_ARMOR_ICONS[partner][i] + 0x1B, rgb, z);
            }
        }
        drawTextColored(x + 0x32, y, (u8 *)PLAYER_DATA(player).partners[slot].card[0].name, rgb, 7, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].hp, PLAYER_DATA(player).partners[slot].card[0].hp);
        drawIconColored(x + 0x2C, y + 14, 0, 0x1A, rgb, z);
        sprintf(buf, SUB_STR_STAT, PLAYER_DATA(player).partners[slot].card[0].hp);
        drawTextColored(x + 0x3C, y + 13, buf, rgb, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[0].power, PLAYER_DATA(player).partners[slot].card[0].attack[0].power);
        drawIconColored(x + 0x2C, y + 0x1A, 0, 7, rgb, z);
        sprintf(buf, SUB_STR_STAT, PLAYER_DATA(player).partners[slot].card[0].attack[0].power);
        drawTextColored(x + 0x3C, y + 0x19, buf, rgb, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[1].power, PLAYER_DATA(player).partners[slot].card[0].attack[1].power);
        drawIconColored(x + 0x2C, y + 0x26, 0, 8, rgb, z);
        sprintf(buf, SUB_STR_STAT, PLAYER_DATA(player).partners[slot].card[0].attack[1].power);
        drawTextColored(x + 0x3C, y + 0x25, buf, rgb, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[2].power, PLAYER_DATA(player).partners[slot].card[0].attack[2].power);
        drawIconColored(x + 0x2C, y + 0x32, 0, 9, rgb, z);
        sprintf(buf, SUB_STR_STAT, PLAYER_DATA(player).partners[slot].card[0].attack[2].power);
        drawTextColored(x + 0x3C, y + 0x31, buf, rgb, palette, z);
        i = PLAYER_DATA(player).partners[slot].card[0].crossEffect;
        palette = 7;
        if (SUB_UNEQUIPPED_PARTNER.card[0].crossEffect != i) {
            palette = 5;
        }
        sprintf(buf, SUB_STR_CROSS_EFFECT, CROSS_EFFECT_SHORT_NAMES[i]);
        drawSmallTextColored(x + 0x56, y + 0x33, buf, palette, rgb, z);
        if (CROSS_EFFECT_ICONS[i] != 0) {
            drawIconColored(x + 0x95, y + 0x31, 0, CROSS_EFFECT_ICONS[i] + 0x14, rgb, z);
        }
        sprintf(buf, SUB_STR_RANK, (s8)PLAYER_DATA(player).partners[slot].level);
        drawLargeTextColored(x + 0x5C, y + 14, buf, 6, rgb, z);
        next = 0;
        if ((s8)PLAYER_DATA(player).partners[slot].level < 99) {
            next = getExpForNextLevel((s8)PLAYER_DATA(player).partners[slot].level) - (u16)PLAYER_DATA(player).partners[slot].exp;
        }
        sprintf(buf, SUB_STR_NEXT, next);
        drawLargeTextColored(x + 0x5C, y + 0x18, buf, 6, rgb, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].dpBonus, PLAYER_DATA(player).partners[slot].card[0].dpBonus);
        drawIconColored(x + 0x72, y + 0x26, 0, 0x19, rgb, z);
        sprintf(buf, SUB_STR_STAT, PLAYER_DATA(player).partners[slot].card[0].dpBonus);
        drawTextColored(x + 0x84, y + 0x25, buf, rgb, palette, z);
        drawTextColored(x + 0xA8, y, (u8 *)SUB_STR_PARTNER_SUPPORT, rgb, 6, z);
        palette = 7;
        if (changed) {
            palette = 5;
        }
        i = PLAYER_DATA(player).partners[slot].card[0].supportIcon;
        if (i != 0) {
            drawIconColored(x + 0x100, y, 0, i + 0x14, rgb, z);
        }
        for (i = 0; i < 4; i++) {
            drawTextColored(x + 0xA8, y + 0xD + i * 12, PLAYER_DATA(player).partners[slot].card[0].supportText[i], rgb, palette, z);
        }
    } else {
        drawTextColored(x + 0x78, y + 0x18, (u8 *)SUB_STR_NO_DATA, rgb, 7, z);
    }
}

void SUB_drawArmorChange(UiWindow *window) {
    s32 x = window->originX + 1;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = SUB_PARTNER_PLAYER;
    s32 prevArmor;
    s32 i;

    if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].armorCardId != 0) {
        drawText(x + 6, y + 1, (s32)"Armor Change with L1 & R1", 7, z);
        prevArmor = SUB_ARMOR_INDEX;
        for (i = 0; i < 3; i++) {
            if ((u16)PAD_STATES[player]->pressed & 4) {
                SUB_ARMOR_INDEX--;
            } else if ((u16)PAD_STATES[player]->pressed & 8) {
                SUB_ARMOR_INDEX++;
            }
            SUB_ARMOR_INDEX = (SUB_ARMOR_INDEX + 3) % 3;
            if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].unlockedArmors[SUB_ARMOR_INDEX] != 0) {
                break;
            }
        }
        if (prevArmor != SUB_ARMOR_INDEX) {
            playMenuSound(1);
            selectPartnerArmor(player, getSlotPartnerIndex(player, SUB_PARTNER_SLOT), SUB_ARMOR_INDEX);
        }
    }
}

void SUB_drawPartnerDetails(UiWindow *window) {
    Rect16 rect;
    char buf[64];
    s32 x = window->originX + 3;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = SUB_PARTNER_PLAYER;
    s32 changed;
    s32 canEquip;
    DigimonCardData *card = &PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].card[0];
    s32 ability;
    s32 palette;
    s32 next;
    s32 diff;
    s32 partner;
    s32 i;

    SUB_SAVED_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
    for (i = 0; i < 3; i++) {
        PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[i] = -1;
    }
    updatePartnerStats(player, SUB_PARTNER_SLOT);
    SUB_UNEQUIPPED_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
    canEquip = 0;
    ability = SUB_ABILITY_IDS[SUB_ABILITY_MENU.row];
    if (SUB_ABILITY_MENU.active != 0) {
        PLAYER_DATA(player).partners[SUB_PARTNER_SLOT] = SUB_SAVED_PARTNER;
        if (getPartnerAbilityState(player, ability) == 1) {
            canEquip = canEquipPartnerAbility(player, SUB_PARTNER_SLOT, SUB_EQUIPMENT_MENU.row, ability);
            if (canEquip != 0) {
                PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[SUB_EQUIPMENT_MENU.row] = ability;
                updatePartnerStats(player, SUB_PARTNER_SLOT);
                SUB_PREVIEW_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
            }
        }
    }
    PLAYER_DATA(player).partners[SUB_PARTNER_SLOT] = SUB_SAVED_PARTNER;
    changed = updatePartnerStats(player, SUB_PARTNER_SLOT);
    SUB_drawPartnerPortrait(player, SUB_PARTNER_SLOT, x, y, 0x80, z);
    drawMediumText(x + 0x2C, y, (s32)card->name, 7, z);
    sprintf(buf, SUB_STR_RANK, (s8)PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].level);
    drawLargeText(x + 0x2C, y + 10, (s32)buf, 6, z);
    next = 0;
    if ((s8)PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].level < 99) {
        next = getExpForNextLevel((s8)PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].level) - (u16)PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].exp;
    }
    sprintf(buf, SUB_STR_NEXT, next);
    drawLargeText(x + 0x2C, y + 0x14, (s32)buf, 6, z);
    palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].hp, card->hp);
    drawIcon(x + 0x2C, y + 0x1E, 0, 0x1A, z);
    sprintf(buf, SUB_STR_STAT, card->hp);
    drawText(x + 0x3A, y + 0x1E, (s32)buf, palette, z);
    palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].dpBonus, card->dpBonus);
    drawIcon(x + 0x2C, y + 0x2A, 0, 0x19, z);
    sprintf(buf, SUB_STR_STAT, card->dpBonus);
    drawText(x + 0x3A, y + 0x2A, (s32)buf, palette, z);
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].unlockedArmors[i] != 0) {
            partner = getSlotPartnerIndex(player, SUB_PARTNER_SLOT);
            drawIcon(x + 0x76 + i * 20, y, 0, SUB_ARMOR_ICONS[partner][i] + 0x1B, z);
        }
    }
    palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[0].power, card->attack[0].power);
    drawIcon(x + 0x76, y + 12, 0, 7, z);
    sprintf(buf, SUB_STR_STAT, card->attack[0].power);
    drawText(x + 0x84, y + 12, (s32)buf, palette, z);
    palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[1].power, card->attack[1].power);
    drawIcon(x + 0x76, y + 0x18, 0, 8, z);
    sprintf(buf, SUB_STR_STAT, card->attack[1].power);
    drawText(x + 0x84, y + 0x18, (s32)buf, palette, z);
    palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[2].power, card->attack[2].power);
    drawIcon(x + 0x76, y + 0x24, 0, 9, z);
    sprintf(buf, SUB_STR_STAT, card->attack[2].power);
    drawText(x + 0x84, y + 0x24, (s32)buf, palette, z);
    palette = 7;
    if (SUB_UNEQUIPPED_PARTNER.card[0].crossEffect != card->crossEffect) {
        palette = 5;
    }
    sprintf(buf, SUB_STR_CROSS_EFFECT, CROSS_EFFECT_SHORT_NAMES[card->crossEffect]);
    drawSmallText(x + 0x74, y + 0x30, (s32)buf, palette, z);
    if (CROSS_EFFECT_ICONS[card->crossEffect] != 0) {
        drawIcon(x + 0xB2, y + 0x2E, 0, CROSS_EFFECT_ICONS[card->crossEffect] + 0x14, z);
    }
    if (canEquip) {
        diff = SUB_PREVIEW_PARTNER.card[0].hp - card->hp;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x54, y + 0x1E, (s32)buf, palette, z);
        }
        diff = SUB_PREVIEW_PARTNER.card[0].dpBonus - card->dpBonus;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x54, y + 0x2A, (s32)buf, palette, z);
        }
        diff = SUB_PREVIEW_PARTNER.card[0].attack[0].power - card->attack[0].power;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x9E, y + 12, (s32)buf, palette, z);
        }
        diff = SUB_PREVIEW_PARTNER.card[0].attack[1].power - card->attack[1].power;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x9E, y + 0x18, (s32)buf, palette, z);
        }
        diff = SUB_PREVIEW_PARTNER.card[0].attack[2].power - card->attack[2].power;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x9E, y + 0x24, (s32)buf, palette, z);
        }
    }
    drawText(x, y + 0x36, (s32)SUB_STR_PARTNER_SUPPORT, 6, z);
    palette = 7;
    if (changed) {
        palette = 5;
    }
    if (card->supportIcon != 0) {
        drawIcon(x + 0x58, y + 0x36, 0, card->supportIcon + 0x14, z);
    }
    for (i = 0; i < 4; i++) {
        drawText(x, y + 0x46 + i * 12, (s32)card->supportText[i], palette, z);
    }
    rect.x = x;
    rect.y = y + 0x46;
    rect.w = 0x6E;
    rect.h = 0x30;
    drawWindowFrame(&rect, 0x31, 0, 0x80, 1, z);
}

void SUB_drawArmorDetails(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    s32 canEquip;
    s32 x = window->originX + 3;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = SUB_PARTNER_PLAYER;
    DigimonCardData *card = &PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].card[1];
    s32 ability;
    s32 palette;
    s32 diff;
    s32 i;

    SUB_drawArmorPortrait(player, SUB_PARTNER_SLOT, x + 0x14, y + 0xA, z);
    if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].armorCardId != 0) {
        SUB_SAVED_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
        for (i = 0; i < 3; i++) {
            PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[i] = -1;
        }
        updatePartnerStats(player, SUB_PARTNER_SLOT);
        SUB_UNEQUIPPED_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
        canEquip = 0;
        ability = SUB_ABILITY_IDS[SUB_ABILITY_MENU.row];
        if (SUB_ABILITY_MENU.active != 0) {
            PLAYER_DATA(player).partners[SUB_PARTNER_SLOT] = SUB_SAVED_PARTNER;
            if (getPartnerAbilityState(player, ability) == 1) {
                canEquip = canEquipPartnerAbility(player, SUB_PARTNER_SLOT, SUB_EQUIPMENT_MENU.row, ability);
                if (canEquip != 0) {
                    PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[SUB_EQUIPMENT_MENU.row] = ability;
                    updatePartnerStats(player, SUB_PARTNER_SLOT);
                    SUB_PREVIEW_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
                }
            }
        }
        PLAYER_DATA(player).partners[SUB_PARTNER_SLOT] = SUB_SAVED_PARTNER;
        updatePartnerStats(player, SUB_PARTNER_SLOT);
        drawMediumText(x, y, (s32)card->name, 7, z);
        drawIcon(x + 0x44, y + 12, 0, SUB_ARMOR_ICONS[getSlotPartnerIndex(player, SUB_PARTNER_SLOT)][getSelectedArmorIndex(player, getSlotPartnerIndex(player, SUB_PARTNER_SLOT))] + 0x1B, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[1].hp, card->hp);
        drawIcon(x, y + 0x3E, 0, 0x1A, z);
        sprintf(buf, SUB_STR_STAT, card->hp);
        drawText(x + 0xE, y + 0x3E, (s32)buf, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[1].attack[0].power, card->attack[0].power);
        drawIcon(x, y + 0x4A, 0, 7, z);
        sprintf(buf, SUB_STR_STAT, card->attack[0].power);
        drawText(x + 0xE, y + 0x4A, (s32)buf, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[1].attack[1].power, card->attack[1].power);
        drawIcon(x, y + 0x56, 0, 8, z);
        sprintf(buf, SUB_STR_STAT, card->attack[1].power);
        drawText(x + 0xE, y + 0x56, (s32)buf, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[1].attack[2].power, card->attack[2].power);
        drawIcon(x, y + 0x62, 0, 9, z);
        sprintf(buf, SUB_STR_STAT, card->attack[2].power);
        drawText(x + 0xE, y + 0x62, (s32)buf, palette, z);
        palette = 7;
        if (SUB_UNEQUIPPED_PARTNER.card[1].crossEffect != card->crossEffect) {
            palette = 5;
        }
        sprintf(buf, SUB_STR_CROSS_EFFECT, CROSS_EFFECT_SHORT_NAMES[card->crossEffect]);
        drawSmallText(x - 2, y + 0x6E, (s32)buf, palette, z);
        if (CROSS_EFFECT_ICONS[card->crossEffect] != 0) {
            drawIcon(x + 0x3C, y + 0x6C, 0, CROSS_EFFECT_ICONS[card->crossEffect] + 0x14, z);
        }
        if (canEquip) {
            diff = SUB_PREVIEW_PARTNER.card[1].hp - card->hp;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x3E, (s32)buf, palette, z);
            }
            diff = SUB_PREVIEW_PARTNER.card[1].attack[0].power - card->attack[0].power;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x4A, (s32)buf, palette, z);
            }
            diff = SUB_PREVIEW_PARTNER.card[1].attack[1].power - card->attack[1].power;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x56, (s32)buf, palette, z);
            }
            diff = SUB_PREVIEW_PARTNER.card[1].attack[2].power - card->attack[2].power;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x62, (s32)buf, palette, z);
            }
        }
    } else {
        drawText(x + 0x14, y + 0x4A, (s32)SUB_STR_NO_DATA, 7, z);
    }
}

void SUB_drawEquipment(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    s32 x = window->originX;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = SUB_PARTNER_PLAYER;
    s32 i;
    s32 ability;
    s32 palette;

    for (i = 0; i < 3; i++) {
        ability = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[i];
        if (ability == -1) {
            drawText(x, y + i * 14, (s32)"---", 7, z);
            drawText(x + 0x1C, y + i * 14, (s32)"None", 7, z);
        } else {
            palette = 7;
            if (PARTNER_ABILITIES[ability].type == 5) {
                palette = 4;
            }
            if (PARTNER_ABILITIES[ability].type == 7) {
                palette = 5;
            }
            sprintf(buf, "*s0%3.3d", ability);
            drawText(x, y + i * 14, (s32)buf, 7, z);
            drawIcon(x + 0x1A, y + i * 14, 2, PARTNER_ABILITIES[ability].type, z);
            drawText(x + 0x36, y + i * 14, (s32)SUB_ABILITY_TEXTS[ability].name, palette, z);
        }
    }
    updateMenuCursor(&SUB_EQUIPMENT_MENU);
}

void SUB_drawAbilityList(UiWindow *window) {
    char buf[72];
    u8 rgb[4];
    s32 player = SUB_PARTNER_PLAYER;
    s32 x = window->originX;
    s32 z = window->z;
    s32 i;
    s32 ability;
    s32 y;
    s32 palette;
    s32 state;

    for (i = 0; i < SUB_ABILITY_MENU.nrows; i++) {
        if (i < window->view.y / SUB_ABILITY_MENU.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / SUB_ABILITY_MENU.rowH < i) {
            break;
        }
        ability = SUB_ABILITY_IDS[i];
        y = window->originY + i * SUB_ABILITY_MENU.rowH + 1;
        state = getPartnerAbilityState(player, ability);
        switch (state) {
        case 1:
            rgb[0] = 0x80;
            rgb[1] = 0x80;
            rgb[2] = 0x80;
            break;
        case 2:
            rgb[0] = 0x40;
            rgb[1] = 0x40;
            rgb[2] = 0x40;
            break;
        }
        palette = 7;
        if (PARTNER_ABILITIES[ability].type == 5) {
            palette = 4;
        }
        if (PARTNER_ABILITIES[ability].type == 7) {
            palette = 5;
        }
        sprintf(buf, "*s0%3.3d", ability);
        drawTextColored(x, y, buf, rgb, palette, z);
        drawIconColored(x + 0x1A, y, 2, PARTNER_ABILITIES[ability].type, rgb, z);
        drawTextColored(x + 0x36, y, (u8 *)SUB_ABILITY_TEXTS[ability].name, rgb, palette, z);
    }
    updateMenuCursor(&SUB_ABILITY_MENU);
}

void SUB_drawPartnerEquipment(void) {
    s32 i;

    SUB_drawPartnerTitle();
    for (i = 0; i < 3; i++) {
        SUB_PARTNER_TABS[i].window.brightness = 0x40;
    }
    SUB_PARTNER_TABS[SUB_PARTNER_SLOT].window.brightness = 0x80;
    drawWindow(&SUB_PARTNER_TABS[SUB_PARTNER_SLOT].window, SUB_drawPartnerTab, 30);
    for (i = 0; i < 3; i++) {
        if (i != SUB_PARTNER_SLOT) {
            drawWindow(&SUB_PARTNER_TABS[i].window, SUB_drawPartnerTab, 30);
        }
    }
    drawWindow(&SUB_ARMOR_CHANGE_WINDOW, SUB_drawArmorChange, 30);
    drawWindow(&SUB_ARMOR_WINDOW, SUB_drawArmorDetails, 30);
    drawWindow(&SUB_PARTNER_WINDOW, SUB_drawPartnerDetails, 30);
    drawWindow(&SUB_ABILITY_WINDOW, SUB_drawAbilityList, 30);
    drawWindow(&SUB_EQUIPMENT_WINDOW, SUB_drawEquipment, 30);
}

void SUB_runPartnerEquipment(s32 player, s32 parentTask, s32 viewOnly) {
    Rect16 rect;
    u8 dialog[0xB8];
    u32 *archive;
    s32 state;
    s32 count;
    s32 ability;
    s32 result;
    s32 i;

    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\PARTNER.ARC", getCurrentTaskId());
    archive = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < (s32)(archive[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)archive + archive[i]), -1, -1, -1, -1);
        waitFrames(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(archive);
    SUB_PARTNER_TITLE_SHOWN = 1;
    SUB_PARTNER_TITLE_Y = -0x20;
    state = 0;
    SUB_PARTNER_SLOT = 0;
    SUB_PARTNER_PLAYER = player;
    SUB_ARMOR_INDEX = 0;
    count = 0;
    for (i = 0; i < 128; i++) {
        SUB_ABILITY_IDS[i] = -1;
    }
    for (i = 0; i < 128; i++) {
        if (getPartnerAbilityState(player, i)) {
            SUB_ABILITY_IDS[count] = i;
            count++;
        }
    }
    SUB_ABILITY_MENU.nrows = count;
    SUB_ABILITY_MENU.pad = player;
    SUB_EQUIPMENT_MENU.pad = player;
    for (i = 0; i < 3; i++) {
        rect.x = 0x10;
        rect.y = i * 65 + 0x26;
        rect.w = 0x120;
        rect.h = 0x3C;
        openWindow(&SUB_PARTNER_TABS[i], &rect, -1, (s16 *)-1, 8, 0x46, 0x80, 12);
        SUB_PARTNER_TABS[i].slot = i;
        switch (i) {
        case 0:
            SUB_PARTNER_TABS[i].window.label = (s32)"PARTNER 1 ";
            break;
        case 1:
            SUB_PARTNER_TABS[i].window.label = (s32)"PARTNER 2 ";
            break;
        case 2:
            SUB_PARTNER_TABS[i].window.label = (s32)"PARTNER 3 ";
            break;
        }
    }
    openMenu(&SUB_ABILITY_MENU, &SUB_ABILITY_WINDOW, &SUB_ABILITY_CURSOR, (Bytes4 *)-1);
    SUB_ABILITY_WINDOW.label = (s32)"Digi-Parts List";
    SUB_ABILITY_WINDOW.palette = 4;
    animateWindowTo(&SUB_ABILITY_WINDOW, (Rect16 *)-1);
    SUB_ABILITY_MENU.active = 0;
    openMenu(&SUB_EQUIPMENT_MENU, &SUB_EQUIPMENT_WINDOW, &SUB_EQUIPMENT_CURSOR, (Bytes4 *)-1);
    SUB_EQUIPMENT_WINDOW.label = (s32)"EQUIPMENT";
    animateWindowTo(&SUB_EQUIPMENT_WINDOW, (Rect16 *)-1);
    openWindow(&SUB_ARMOR_CHANGE_WINDOW, &SUB_ARMOR_CHANGE_WINDOW_RECT, -1, (s16 *)-1, 8, 0x32, 0x80, 12);
    animateWindowTo(&SUB_ARMOR_CHANGE_WINDOW, (Rect16 *)-1);
    openWindow(&SUB_PARTNER_WINDOW, &SUB_PARTNER_WINDOW_RECT, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    SUB_PARTNER_WINDOW.label = (s32)"PARTNER";
    animateWindowTo(&SUB_PARTNER_WINDOW, (Rect16 *)-1);
    openWindow(&SUB_ARMOR_WINDOW, &SUB_ARMOR_WINDOW_RECT, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    SUB_ARMOR_WINDOW.label = (s32)"ARMOR";
    animateWindowTo(&SUB_ARMOR_WINDOW, (Rect16 *)-1);
    addFrameCallback((s32)SUB_drawPartnerEquipment);
    for (;;) {
        waitFrames(FRAME_INTERVAL);
        switch (state) {
        case -1:
            SUB_PARTNER_TITLE_SHOWN = 0;
            waitFrames(20);
            removeFrameCallback((s32)SUB_drawPartnerEquipment);
            resumeTask(parentTask);
            return;
        case 0:
            if (SUB_ARMOR_WINDOW_ANIM_DONE != 0 && SUB_PARTNER_WINDOW_ANIM_DONE != 0) {
                if (PAD_STATES[player]->pressed & 0x1000) {
                    playMenuSound(2);
                    SUB_PARTNER_SLOT--;
                } else if (PAD_STATES[player]->pressed & 0x4000) {
                    playMenuSound(2);
                    SUB_PARTNER_SLOT++;
                }
                SUB_PARTNER_SLOT = (SUB_PARTNER_SLOT + 3) % 3;
            }
            if (PAD_STATES[player]->pressed & 0x40) {
                if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].cardId != 0) {
                    playMenuSound(1);
                    SUB_ARMOR_INDEX = getSelectedArmorIndex(player, getSlotPartnerIndex(player, SUB_PARTNER_SLOT));
                    for (i = 0; i < 3; i++) {
                        animateWindowTo(&SUB_PARTNER_TABS[i].window, (Rect16 *)-1);
                    }
                    animateWindowTo(&SUB_PARTNER_WINDOW, &SUB_PARTNER_WINDOW_RECT);
                    if (countUnlockedPartnerArmors(player, getSlotPartnerIndex(player, SUB_PARTNER_SLOT)) >= 2) {
                        animateWindowTo(&SUB_ARMOR_CHANGE_WINDOW, &SUB_ARMOR_CHANGE_WINDOW_RECT);
                    }
                    animateWindowTo(&SUB_ARMOR_WINDOW, &SUB_ARMOR_WINDOW_RECT);
                    animateWindowTo(&SUB_EQUIPMENT_WINDOW, &SUB_EQUIPMENT_MENU.rect);
                    if (viewOnly == 0) {
                        PLAYER_DATA(player).activePartner = getSlotPartnerIndex(0, SUB_PARTNER_SLOT);
                        changeScrollingBackground(PLAYER_DATA(player).activePartner, 0x380, 0, 0x380, 0x80);
                    }
                    state = 1;
                }
            } else if (PAD_STATES[player]->pressed & 0x10) {
                for (i = 0; i < 3; i++) {
                    animateWindowTo(&SUB_PARTNER_TABS[i].window, (Rect16 *)-1);
                }
                playMenuSound(4);
                state = -1;
            }
            break;
        case 1:
            if (PAD_STATES[player]->pressed & 0x40) {
                playMenuSound(1);
                animateWindowTo(&SUB_ABILITY_WINDOW, &SUB_ABILITY_MENU.rect);
                SUB_EQUIPMENT_MENU.active = 0;
                SUB_ABILITY_MENU.active = 1;
                ability = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[SUB_EQUIPMENT_MENU.row];
                SUB_ABILITY_MENU.row = 0;
                if (ability != -1) {
                    for (i = 0; i < SUB_ABILITY_MENU.nrows && SUB_ABILITY_IDS[i] != ability; i++) {
                        SUB_ABILITY_MENU.row++;
                    }
                }
                centerMenuOnCursor(&SUB_ABILITY_MENU);
                state = 2;
            } else if (PAD_STATES[player]->pressed & 0x80) {
                if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[SUB_EQUIPMENT_MENU.row] != -1) {
                    playMenuSound(1);
                    unequipPartnerAbility(player, SUB_PARTNER_SLOT, SUB_EQUIPMENT_MENU.row);
                }
            } else if (PAD_STATES[player]->pressed & 0x10) {
                playMenuSound(0);
                for (i = 0; i < 3; i++) {
                    rect.x = 0x10;
                    rect.y = i * 65 + 0x26;
                    rect.w = 0x120;
                    rect.h = 0x3C;
                    animateWindowTo(&SUB_PARTNER_TABS[i].window, &rect);
                }
                animateWindowTo(&SUB_ARMOR_WINDOW, (Rect16 *)-1);
                animateWindowTo(&SUB_PARTNER_WINDOW, (Rect16 *)-1);
                animateWindowTo(&SUB_ARMOR_CHANGE_WINDOW, (Rect16 *)-1);
                animateWindowTo(&SUB_EQUIPMENT_WINDOW, (Rect16 *)-1);
                state = 0;
            }
            break;
        case 2:
            if (PAD_STATES[player]->pressed & 0x40) {
                ability = SUB_ABILITY_IDS[SUB_ABILITY_MENU.row];
                result = getPartnerAbilityState(player, ability);
                if (result == 1) {
                    playMenuSound(1);
                    if (canEquipPartnerAbility(player, SUB_PARTNER_SLOT, SUB_EQUIPMENT_MENU.row, ability)) {
                        equipPartnerAbility(player, SUB_PARTNER_SLOT, SUB_EQUIPMENT_MENU.row, ability);
                        animateWindowTo(&SUB_ABILITY_WINDOW, (Rect16 *)-1);
                        SUB_EQUIPMENT_MENU.active = result;
                        SUB_ABILITY_MENU.active = 0;
                        state = 1;
                    } else {
                        initDialog(dialog, "The same Digi-Part is already used. Only\n1 Digi-Part of each kind can be used.", 0);
                        runDialogForPad((s32 *)dialog, player);
                    }
                }
            } else if (PAD_STATES[player]->pressed & 0x10) {
                playMenuSound(0);
                animateWindowTo(&SUB_ABILITY_WINDOW, (Rect16 *)-1);
                SUB_EQUIPMENT_MENU.active = 1;
                SUB_ABILITY_MENU.active = 0;
                state = 1;
            }
            break;
        }
    }
}
