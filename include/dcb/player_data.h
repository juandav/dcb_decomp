#ifndef DCB_PLAYER_DATA_H
#define DCB_PLAYER_DATA_H

#include "game.h"

#if VERSION_JP
/* jp's scrolling background (A:\ADVBG.PAK) and the panel over it that
   shows the open menu's icon */
typedef struct {
    /* 0x000 */ TexSprite tiles[15]; /* the picture, 2 sprites or 15 tiles */
    /* 0x12C */ TexSprite panel[3]; /* the panel, its bar and the icon */
    /* 0x168 */ POLY_FT4 bars[2]; /* per frame buffer */
    /* 0x1B8 */ Chunk *pak;
    /* 0x1BC */ s16 icon; /* the icon shown, negative when it was requested so */
    /* 0x1BE */ s16 state; /* of the panel: 0 still, 1 opening, 2 changing icon, 3 closing */
    /* 0x1C0 */ s16 requestedIcon; /* -1: none */
    /* 0x1C2 */ s16 scrollX;
    /* 0x1C4 */ s16 scrollY;
    /* 0x1C6 */ s8 shownImage;
    /* 0x1C7 */ s8 image; /* 0 and 1 still, from 2 on scrolling tiles */
    /* 0x1C8 */ u8 brightness;
    /* 0x1C9 */ u8 ready;
    /* 0x1CA */ u8 pad1CA[2];
} ScrollingBackground;

extern ScrollingBackground *SCROLLING_BACKGROUND;

void runWindowTask(WindowSpec *spec, s32 parent);
void startAreaPakLoad(void);
void openChoiceMenu(ChoiceMenu *menu, s32 icon, s32 y, void (*cancel)(), s32 *arg);
void addChoiceMenuItem(ChoiceMenu *menu, s32 item, void (*action)());
void openChoiceMenuFromList(ChoiceMenu *menu, s8 *list, s32 *arg);
s8 runChoiceMenu(ChoiceMenu *menu);
void startChoiceMenuAction(ChoiceMenu *menu);
void showScrollingBackground(void);
#endif

void initPlayerData(void);
void resetScriptProgress(void);
void resetPlayerData(void);
void renderFullscreenBackground(void);
void playModelAnimation(s32 modelSlot, s32 animId);
void setModelAnimationPose(s32 modelSlot, s32 animId);
void *findDigimonCardByModelId(s32 modelId);
s32 loadSkill(s32 skillId, s32 pak);
void loadSkillFromDisc(s32 skillId);

#endif /* DCB_PLAYER_DATA_H */
