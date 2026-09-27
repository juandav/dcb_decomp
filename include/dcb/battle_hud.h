#ifndef DCB_BATTLE_HUD_H
#define DCB_BATTLE_HUD_H

#include "game.h"

#define SPRITE(c) (*(void **)(D_801D833C + (c) * 36))
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
#define SLOT(p, o) ((BoardSlot *)(D_801D83EC + (p) * 0xD8 + (o)))
#define UNK7F8(c) ((*(Unk7F8 **)((u8 *)D_801D8340 + 0x7F8))[c])
#define PANEL(i) (((InfoPanel *)D_801D83EC)[i])

typedef struct Panel {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 unkF;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u8 unk1C[4];
    /* 0x20 */ struct Panel *parent;
} Panel;
typedef struct {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s16 clut;
    /* 0x06 */ u8 unk6[6];
    /* 0x0C */ u8 flags;
    /* 0x0D */ u8 state;
    /* 0x0E */ u8 unkE[2];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ u8 unk14[8];
    /* 0x1C */ s32 z;
    /* 0x20 */ u8 unk20[4];
} InfoPanel;
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
    /* 0x23 */ s8 unk23;
} CardAnim;

extern s32 D_8006E298;
extern u8 *CROSS_EFFECT_SHORT_NAMES[];
extern u8 *CROSS_EFFECT_NAMES[];
extern u8 D_8006E4FC[];

void waitForStatCountersToSettle(void);
void showStatChangePopup(s32 player, s32 newValue, s32 stat);
void renderStatPopups(void);
void drawHudPanelContents(s32 panelIndex, s32 z);
void showDpGainPopup(s32 player);
void tickCardMotion(s32 cardIndex, s32 player);
void startPanelMove(Panel *panel, s16 targetX, s16 targetY, s32 frames);
s32 stepPanelMove();
void holdPanelAtTarget();
void tickDeckPanel(s32 player);
void func_8003DD9C(s32 player);
void tickStatusPanel(s32 player);
void func_8003E11C(s32 player);
void tickAttackPanel(s32 player);
void tickCardInfoPanel(s32 player);
void initDuelState(s32 isCpuDuel);
void startDuelScene(void);
void spawnDuelTasks(s32 isCpuDuel);
void teardownDuelScene(void);
void renderBoardCards(void);
void tickBattleHud(void);
void renderDuelFrame(void);
void runDuel();

#endif /* DCB_BATTLE_HUD_H */
