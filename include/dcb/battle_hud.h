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
#if VERSION_JP
/* jp's HUD panels are in its session block: eleven per player, then one
   for both */
#define HUD_PANEL(i) (&((SessionData *)SESSION_DATA)->panels[i])
#define PLAYER_PANEL(p, slot) HUD_PANEL((p) * 11 + (slot))

/* jp's HUD panel: a textured quad that the duel moves around, in 3D */
typedef struct Panel {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ u8 rgb[3];
    /* 0x07 */ u8 code;
    /* 0x08 */ u16 clut;
    /* 0x0A */ u16 tpage;
    /* 0x0C */ u8 uv[4][2];
    /* 0x14 */ VECTOR pos;
    /* 0x24 */ SVECTOR rot;
    /* 0x2C */ s16 w;
    /* 0x2E */ s16 h;
    /* 0x30 */ VECTOR start; /* where a move started */
    /* 0x40 */ u8 flags; /* 0x80: drawn */
    /* 0x41 */ u8 state;
    /* 0x42 */ u8 total; /* frames of the move */
    /* 0x43 */ u8 count; /* frames left */
    /* where its first corner lands on screen, once projected (320, 256
       before): drawHudPanelContents draws the panel's contents from there */
    /* 0x44 */ s16 sx;
    /* 0x46 */ s16 sy;
} Panel;
#elif VERSION_US || VERSION_EU
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
#endif
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
    /* 0x06 */ s16 tpage;
    /* 0x08 */ u8 uv[4]; /* its rect in the texture page (as KAWSEG sets it) */
    /* 0x0C */ u8 flags;
    /* 0x0D */ u8 state;
    /* 0x0E */ u8 unkE[2];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ u8 unk14[8];
    /* 0x1C */ s32 z;
    /* 0x20 */ u8 parent[4];
} HudPanel;
#if VERSION_JP
/* jp keeps the state and hand slot right after the sprite */
typedef struct {
    /* 0x00 */ CardSprite *spr;
    /* 0x04 */ s8 state;
    /* 0x05 */ s8 handSlot;
    /* 0x06 */ u8 unk6[2];
    /* 0x08 */ s32 x;
    /* 0x0C */ s32 y;
    /* 0x10 */ s32 z;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s16 rx;
    /* 0x1A */ s16 ry;
    /* 0x1C */ s16 rz;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 scale;
    /* 0x22 */ s16 total;
    /* 0x24 */ s16 count;
    /* 0x26 */ u8 unk26[2];
} CardAnim;
#elif VERSION_US || VERSION_EU
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
#endif

extern s32 STAT_POPUP_RGB;
extern u8 *CROSS_EFFECT_SHORT_NAMES[];
extern u8 *CROSS_EFFECT_NAMES[];
extern u8 CROSS_EFFECT_ICONS[];

void waitForStatCountersToSettle(void);
void showStatChangePopup(s32 player, s32 newValue, s32 stat);
void renderStatPopups(void);
void drawHudPanelContents(s32 panelIndex, s32 z);
void showDpGainPopup(s32 player);

#if VERSION_JP
void func_8004923C(void); /* jp's own: a task quitToTitleOrPlayEnding starts */
#endif

#endif /* DCB_BATTLE_HUD_H */
