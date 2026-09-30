#ifndef DCB_BATTLE_HUD_H
#define DCB_BATTLE_HUD_H

#include "game.h"

#define SPRITE(c) (((CardAnim *)CARD_ANIMS)[c].spr)
#define CARD_ANIM(c) (&((CardAnim *)CARD_ANIMS)[c])
#define ANIM_SAVE(a)                   \
    (a)->x = (a)->spr->pos.vx;         \
    (a)->y = (a)->spr->pos.vy;         \
    (a)->z = (a)->spr->pos.vz;         \
    (a)->rx = (a)->spr->rot.vx;        \
    (a)->ry = (a)->spr->rot.vy;        \
    (a)->rz = (a)->spr->rot.vz;        \
    (a)->scale = (a)->spr->scale
#define ANIM_STEP(a, TX, TY, RX, RY, RZ, SC)                                  \
    (a)->count--;                                                             \
    (a)->spr->pos.vx = TX - (TX - (a)->x) * (a)->count / (a)->total;         \
    (a)->spr->pos.vy = TY - (TY - (a)->y) * (a)->count / (a)->total;         \
    (a)->spr->pos.vz = 0 - (0 - (a)->z) * (a)->count / (a)->total;            \
    (a)->spr->rot.vx = RX - (RX - (a)->rx) * (a)->count / (a)->total;        \
    (a)->spr->rot.vy = RY - (RY - (a)->ry) * (a)->count / (a)->total;        \
    (a)->spr->rot.vz = RZ - (RZ - (a)->rz) * (a)->count / (a)->total;        \
    (a)->spr->scale = SC - (SC - (a)->scale) * (a)->count / (a)->total
#define PANEL(i) (((HudPanel *)HUD_PANELS)[i])
/* HUD_PANELS holds six panels per player (enum HudPanelSlot) */
#define PLAYER_PANEL(p, slot) (&((Panel *)HUD_PANELS)[(p) * 6 + (slot)])

typedef struct Panel {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ u8 flags; /* 0x80: drawn */
    /* 0x0D */ u8 state;
    /* 0x0E */ u8 total;
    /* 0x0F */ u8 count;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 targetX;
    /* 0x16 */ s16 targetY;
    /* 0x18 */ s16 startX;
    /* 0x1A */ s16 startY;
    /* 0x1C */ u8 unk1C[4];
    /* 0x20 */ struct Panel *parent;
} Panel;
enum HudPanelSlot {
    HUD_CARD_INFO,
    HUD_ATTACK,
    HUD_STATUS,
    HUD_PLAYED_CARD, /* the played card sits against it (tickCardMotion 16-20) */
    HUD_DECK,
    HUD_TURN_MARKER /* slides in for the player whose turn starts */
};
typedef struct {
    /* 0x00 */ u8 rgb[4];
    /* 0x04 */ s16 clut;
    /* 0x06 */ u8 unk6[6];
    /* 0x0C */ u8 flags;
    /* 0x0D */ u8 state;
    /* 0x0E */ u8 unkE[2];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ u8 unk14[8];
    /* 0x1C */ s32 z;
    /* 0x20 */ u8 parent[4];
} HudPanel;
typedef struct {
    /* 0x00 */ CardSprite *spr;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 rx;
    /* 0x16 */ s16 ry;
    /* 0x18 */ s16 rz;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 scale;
    /* 0x1E */ s16 total;
    /* 0x20 */ s16 count;
    /* 0x22 */ s8 state;
    /* 0x23 */ s8 handSlot;
} CardAnim;

extern s32 STAT_POPUP_RGB;
extern u8 *CROSS_EFFECT_SHORT_NAMES[];
extern u8 *CROSS_EFFECT_NAMES[];
extern u8 CROSS_EFFECT_ICONS[];

void waitForStatCountersToSettle(void);
void showStatChangePopup(s32 player, s32 newValue, s32 stat);
void renderStatPopups(void);
void drawHudPanelContents(s32 panelIndex, s32 z);
void showDpGainPopup(s32 player);

#endif /* DCB_BATTLE_HUD_H */
