#include "common.h"
#include "game.h"
#include "dcb/sai_world_map.h"
#include "dcb/heap.h"
#include "dcb/frame_callback.h"
#include "dcb/task.h"
#include "dcb/vram_upload.h"
#include "dcb/scene3d.h"
#include "dcb/game_flow.h"
#include "dcb/loader.h"
#include "dcb/sound_play.h"
#include "dcb/scroll_bg.h"
#include "dcb/card_render.h"
#include "dcb/prim.h"
#include "dcb/menu.h"
#include "dcb/sound.h"
#include "dcb/pad.h"
#include "dcb/sai_area.h"
#include "dcb/sai_sprite.h"

#define SESSION_SUB (((SessionData *)SESSION_DATA)->areaSession)

/* the world map's states, SAI_MAP_STATE_FUNCS' index (WorldMap.state) */
enum MapState {
    MAP_OPENING,
    MAP_IDLE,
    MAP_WALKING,
    MAP_FADE_IN,
    MAP_CHANGE_REGION,
    MAP_SWITCH_REGION,
    MAP_ENTER_AREA,
    MAP_CLOSING,
    MAP_DONE,
    MAP_MENU
};

extern DrTPage SAI_MAP_PATH_TPAGES[2][20];
extern s8 SAI_MAP_ANIM_REGION;
extern MapAnim *SAI_MAP_ANIMS;
extern u8 SAI_MAP_ACTIVE;
extern u8 SAI_MAP_OLD_MARKER_COUNT;
extern s8 SAI_MAP_STATE;
extern u8 SAI_MAP_ALPHA;
extern u8 SAI_MAP_OPEN_MENU;
extern u8 SAI_MAP_MENU_TAB_STATE;
extern s8 SAI_MAP_LABEL_REGION;
extern MenuTab SAI_MAP_MENU_TAB;
extern s8 SAI_MAP_NODE;
extern u8 SAI_MAP_NAME_SLIDE_DIR;
extern s8 SAI_MAP_PORTRAIT_STATE;
extern u8 SAI_MAP_ANIMATING;
extern void (*SAI_ICON_MOTION_FUNCS[])(void);
extern s8 SAI_MAP_MENU_CURSOR;
extern s8 SAI_MAP_MENU_PHASE;
extern s16 SAI_MAP_FRAME_BRIGHTNESS;
extern s32 SAI_MAP_ICON_SLIDE;

long ratan2(long y, long x);
int csqrt(int a);
void SAI_initRegion0Anims(void);
void SAI_initRegion1Anims(void);
void SAI_initRegion2Anims(void);
void SAI_drawRegion0Anims(void);
void SAI_drawRegion1Anims(void);
void SAI_drawRegion2Anims(void);
void SAI_drawMapPaths(FrameBuffer *fb);
void SAI_setRegionMapImage(void);
void SAI_createMapMenuTab(s8 keepTabState);
void SAI_drawMapMenuTab(void);
void SAI_openDeckEditorFromMap(void);
void SAI_openEquipmentFromMap(void);
void SAI_initMapPaths(void);

/* the map's nodes: position, the node in each direction, unlocked */
MapNode SAI_MAP_NODES[16] = {
    { 73, 26, { -1, 12, 4, 1 }, 1, 0 },
    { 16, -17, { 3, 0, 2, 3 }, 1, 0 },
    { 14, 34, { 1, 1, -1, -1 }, 1, 0 },
    { -27, -50, { -1, 1, 1, -1 }, 1, 0 },
    { 81, 53, { 0, -1, -1, 0 }, 1, 0 },
    { -22, 25, { -1, 7, 13, 13 }, 1, 0 },
    { 61, 67, { 7, 7, -1, -1 }, 1, 0 },
    { 67, 18, { 8, -1, 6, 5 }, 1, 0 },
    { 47, -37, { 14, 7, 7, 14 }, 1, 0 },
    { 70, -36, { -1, 15, -1, 10 }, 1, 0 },
    { -31, -16, { 9, 9, 11, -1 }, 1, 0 },
    { 44, 21, { 10, -1, -1, 10 }, 1, 0 },
    { 107, -11, { -1, -1, 0, 0 }, 1, 0 },
    { -46, 33, { 5, 5, -1, -1 }, 1, 0 },
    { -16, -44, { -1, 8, 8, -1 }, 1, 0 },
    { 120, -18, { 9, -1, -1, 9 }, 1, 0 },
};

/* the nodes of each of the three regions, ended by -1 */
s8 SAI_REGION_NODES[3][7] = {
    { 0, 1, 2, 3, 4, 12, -1 },
    { 5, 6, 7, 8, 13, 14, -1 },
    { 9, 10, 11, 15, -1, -1, -1 },
};

Rect16 SAI_REGION_MAP_RECTS[3] = {
    { 0x180, 0x100, 0xC6, 0xA2 },
    { 0x200, 0x100, 0xC6, 0xA2 },
    { 0x280, 0x100, 0xC6, 0xA2 },
};

void SAI_initPathPolys(void) {
    PolyF4 *poly;
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        DB(i).primSlots[1] = (s32)(SAI_WORLD_MAP.pathPolys[i] = allocHeapBlock(20 * sizeof(PolyF4), 0x28));
        poly = SAI_WORLD_MAP.pathPolys[i];
        for (j = 0; j < 20; j++, poly++) {
            SetPolyF4(poly);
            poly->r0 = 0x7F;
            poly->g0 = 0x44;
            poly->b0 = 0xC;
            SetSemiTrans(poly, 1);
            SetDrawTPage(&SAI_MAP_PATH_TPAGES[i][j], 0, 0, 0x20);
        }
    }
}

void SAI_loadMapTextures(s32 useMap) {
    /* not a literal: GCC would share SAI_loadAreaTextures's identical string */
    static const char worldPath[] = "C:\\OBJECT\\world.TIS";
    char path[0x48];
    u32 *pack;

    *(s32 *)&((SessionData *)SESSION_DATA)->areaSession->unk0[0x194] = 1;
    if (useMap == 0) {
        sprintf(path, worldPath);
    } else {
        sprintf(path, "C:\\OBJECT\\map.TIS");
    }
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    *(s32 *)&((SessionData *)SESSION_DATA)->areaSession->unk0[0x194] = 0;
}

void SAI_initCamera(void) {
    Graphics *camera;

    initScene3D(0);
    spawnTask(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    camera = (Graphics *)&GRAPHICS;
    camera->rotX = 0;
    camera->rotY = 0;
    camera->rotZ = 0;
    camera->posX = 0;
    camera->posY = 0;
    camera->posZ = 0;
    camera->targetPitch = 0;
    camera->targetDistance = 0x1C0;
    camera->targetHeight = 0;
    camera->targetYaw = 0;
    camera->targetModel = -1;
    camera->snapCamera = 1;
}

s32 SAI_isSavedScriptFlagSet(u32 id) {
    u32 word;
    s32 mask;

    id -= 12;
    word = id >> 5;
    id &= 31;
    return (((PlayerProfile *)PLAYER_PROFILES)->areaScriptFlags[word] & (mask = 1 << id)) != 0;
}

void SAI_slideIconToTop(void) {
    s32 t = SAI_WORLD_MAP.iconSlide;

    SAI_WORLD_MAP.iconX = (t * 270 + (15 - t) * 136) / 15;
    SAI_WORLD_MAP.iconY = (t * 9 + (15 - t) * 96) / 15;
}

void SAI_runCornerIcon(s32 state) {
    Rect16 uv[2];
    u8 brightness;
    s8 dir;

    if (state == 3) {
        state = 2;
    } else if (state == 4) {
        /* not a Rect16: GCC would share SAI_glitchVram's identical constant */
        s16 rect[4] = { 0x220, 0xE1, 0x20, 1 };

        changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->activePartner, 0x380, 0, 0x380, 0x80);
        waitFrames(2);
        MoveImage2((Rect16 *)rect, 0x380, 0x80);
        setBackgroundScrollMode(1);
        state = 2;
    } else {
        changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->activePartner, 0x380, 0, 0x380, 0x80);
    }
    uv[0].x = 0;
    uv[0].y = 0;
    uv[0].w = 0x30;
    uv[0].h = 0x30;
    uv[1].x = 0x30;
    uv[1].y = 0;
    uv[1].w = 0x30;
    uv[1].h = 0x30;
    brightness = 0x80;
    dir = 1;
    SAI_WORLD_MAP.iconMotion = 0;
    SAI_WORLD_MAP.iconX = 0x88;
    SAI_WORLD_MAP.iconY = 0x60;
    SAI_WORLD_MAP.iconRunning = 1;
    SAI_WORLD_MAP.iconMotion = state;
    do {
        waitFrames(1);
        if (SAI_ICON_MOTION_FUNCS[SAI_WORLD_MAP.iconMotion] != NULL) {
            SAI_ICON_MOTION_FUNCS[SAI_WORLD_MAP.iconMotion]();
        }
        if (dir == 1) {
            brightness += 4;
        } else if (dir == -1) {
            brightness -= 4;
        }
        if (brightness >= 0xD8) {
            brightness = 0xD8;
            dir = -1;
        } else if (brightness < 0x70) {
            brightness = 0x70;
            dir = 1;
        }
        drawTexturedSprite(SAI_WORLD_MAP.iconX, SAI_WORLD_MAP.iconY, &uv[1], 0x28, 0x39E0, 0x19, brightness, 1);
        drawTexturedSprite(SAI_WORLD_MAP.iconX, SAI_WORLD_MAP.iconY, &uv[0], 8, 0x3960, 0x19, 0x80, -1);
    } while (SAI_WORLD_MAP.iconRunning != 0);
}

/* not called by any code */
void SAI_slideIconSpritesOut(void) {
    SAI_WORLD_MAP.iconSlide++;
    if (SAI_WORLD_MAP.iconSlide > 15) {
        SAI_WORLD_MAP.iconSlide = 15;
    }
    SAI_WORLD_MAP.zoomAngle = (SAI_WORLD_MAP.zoom << 10) / 35;
    SAI_SPRITES[0]->pos.vx = SAI_SPRITES[1]->pos.vx = SAI_WORLD_MAP.iconSlide * 134 / 15;
    SAI_SPRITES[0]->pos.vy = SAI_SPRITES[1]->pos.vy = SAI_WORLD_MAP.iconSlide * 87 / 15;
}

void SAI_slideIconToBottom(void) {
    s32 t = SAI_WORLD_MAP.iconSlide;

    SAI_WORLD_MAP.iconX = (t * 270 + (15 - t) * 136) / 15;
    SAI_WORLD_MAP.iconY = (t * 183 + (15 - t) * 96) / 15;
}

void (*SAI_ICON_MOTION_FUNCS[3])(void) = {
    NULL,
    SAI_slideIconToBottom,
    SAI_slideIconToTop,
};

void SAI_initRegion(void) {
    s32 i;
    s8 node;

    SAI_WORLD_MAP.labelRegion = SAI_WORLD_MAP.region;
    SAI_WORLD_MAP.route = SAI_REGION_NODES[SAI_WORLD_MAP.region];
    SAI_WORLD_MAP.node = &SAI_MAP_NODES[SAI_WORLD_MAP.nodeIndex];
    SAI_SPRITES[0] = SAI_createSprite(0);
    SAI_SPRITES[0]->pos.vx = SAI_MAP_NODES[SAI_WORLD_MAP.nodeIndex].x;
    SAI_SPRITES[0]->pos.vy = SAI_MAP_NODES[SAI_WORLD_MAP.nodeIndex].y - 15;
    SAI_setSpriteDepth(SAI_SPRITES[0], 0x1E);
    SAI_SPRITES[1] = SAI_createSprite(1);
    SAI_setSpriteDepth(SAI_SPRITES[1], 0x1F);
    for (i = 0; i < 2; i++) {
        SetSemiTrans(&SAI_SPRITES[1]->quads[i], 1);
        SAI_SPRITES[1]->quads[i].tpage = 0xC7;
    }
    SAI_WORLD_MAP.alpha = 0xFF;
    SAI_WORLD_MAP.lastRegion = SAI_WORLD_MAP.region;
    SAI_WORLD_MAP.markerCount = 0;
    for (i = 0; i < 7; i++) {
        node = SAI_REGION_NODES[SAI_WORLD_MAP.region][i];
        if (node < 0) {
            continue;
        }
        if (node >= 12) {
            SAI_SPRITES[i + 21] = SAI_createSprite(0x40);
        } else {
            SAI_SPRITES[i + 21] = SAI_createSprite(9);
        }
        SAI_moveSpriteCorners(SAI_SPRITES[i + 21], SAI_MAP_NODES[node].x, SAI_MAP_NODES[node].y);
        SAI_setSpriteDepth(SAI_SPRITES[i + 21], 0x20);
        SAI_WORLD_MAP.markerCount++;
    }
    SAI_initMapPaths();
}

void SAI_tickMapMenu(void) {
    s32 x;
    s32 i;

    if (SAI_MAP_MENU_TAB.offset != 0) {
        return;
    }
    x = ((20 - SAI_WORLD_MAP.menuSlide) * -214 + SAI_WORLD_MAP.menuSlide * -117) / 20;
    if (SAI_WORLD_MAP.menuPhase == 0) {
        SAI_WORLD_MAP.menuSlide++;
        if (SAI_WORLD_MAP.menuSlide > 20) {
            SAI_WORLD_MAP.menuSlide = 20;
            SAI_WORLD_MAP.menuPhase = 1;
            SAI_WORLD_MAP.menuCursor = 0;
        }
    } else if (SAI_WORLD_MAP.menuPhase == 1) {
        if (PAD_STATES[0]->repeat & PAD_DOWN) {
            playSoundEffect(2);
            SAI_WORLD_MAP.menuCursor++;
            if (SAI_WORLD_MAP.menuCursor >= 3) {
                SAI_WORLD_MAP.menuCursor = 0;
            }
        } else if (PAD_STATES[0]->repeat & PAD_UP) {
            playSoundEffect(2);
            SAI_WORLD_MAP.menuCursor--;
            if (SAI_WORLD_MAP.menuCursor < 0) {
                SAI_WORLD_MAP.menuCursor = 2;
            }
        } else if (PAD_STATES[0]->pressed & (PAD_TRIANGLE | PAD_CIRCLE)) {
            playSoundEffect(1);
            SAI_WORLD_MAP.menuPhase = 2;
            SAI_WORLD_MAP.menuChosen = 0;
        } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
            if (((SessionData *)SESSION_DATA)->playWithoutSaving != 1 || SAI_MAP_MENU_CURSOR != 2) {
                playSoundEffect(0);
                SAI_WORLD_MAP.menuPhase = 2;
                SAI_WORLD_MAP.menuChosen = 1;
                SAI_MAP_MENU_TAB_STATE = 3;
            }
        }
    } else if (SAI_WORLD_MAP.menuPhase == 2) {
        SAI_WORLD_MAP.menuSlide--;
        if (SAI_WORLD_MAP.menuSlide < 0) {
            SAI_WORLD_MAP.menuSlide = 0;
            SAI_WORLD_MAP.menuPhase = 3;
        }
    } else if (SAI_WORLD_MAP.menuPhase == 3) {
        if (SAI_WORLD_MAP.menuChosen == 0) {
            SAI_WORLD_MAP.state = MAP_IDLE;
        } else {
            SAI_MAP_STATE = MAP_ENTER_AREA;
        }
        SAI_MAP_MENU_PHASE = 4;
    }
    for (i = 30; i < 34; i++) {
        if (i == 30) {
            SAI_SPRITES[i]->pos.vx = x + 1;
        } else {
            SAI_SPRITES[i]->pos.vx = x + 3;
        }
        if (i != 30) {
            if (((SessionData *)SESSION_DATA)->playWithoutSaving == 1) {
                if (SAI_WORLD_MAP.menuCursor == i - 31) {
                    if (i == 33) {
                        SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xF1);
                        SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xF1);
                    } else {
                        SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xF0);
                        SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xF0);
                    }
                } else if (i == 33) {
                    SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xF2);
                    SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xF2);
                } else {
                    SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xEF);
                    SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xEF);
                }
            } else if (SAI_WORLD_MAP.menuCursor == i - 31) {
                SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xF0);
                SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xF0);
            } else {
                SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xEF);
                SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xEF);
            }
        }
        if (SAI_WORLD_MAP.menuPhase < 3) {
            SAI_drawSprite(SAI_SPRITES[i]);
        } else if (SAI_WORLD_MAP.menuPhase == 4) {
            if (SAI_MAP_MENU_TAB.state != 3) {
                SAI_MAP_MENU_TAB.state = 1;
            }
            SAI_freeSprite(SAI_SPRITES[i]);
        }
    }
}

void SAI_initMapAnims(s8 mode) {
    SAI_MAP_ANIM_REGION = mode;
    switch (mode) {
    case 0:
        SAI_initRegion0Anims();
        break;
    case 1:
        SAI_initRegion1Anims();
        break;
    case 2:
        SAI_initRegion2Anims();
        break;
    }
}

void SAI_initRegion0Anims(void) {
    s16 xs[6] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xCF, 0x8E };
    s16 ys[6] = { 0x54, 0x4B, 0x4C, 0x4C, 0x30, 0x4B };
    s32 i;

    for (i = 0; i < 6; i++) {
        SAI_MAP_ANIMS[i].x = xs[i];
        SAI_MAP_ANIMS[i].y = ys[i];
        SAI_MAP_ANIMS[i].timer = 0;
        SAI_MAP_ANIMS[i].frames = 6;
        SAI_MAP_ANIMS[i].frameTime = 4;
    }
    SAI_MAP_ANIMS[4].frameTime = 8;
    for (i = 0; i < 6; i++) {
        SAI_MAP_ANIMS[i].frameTime *= 2;
    }
}

void SAI_initRegion1Anims(void) {
    s32 x = 0xAB;
    s32 y = 0x28;

    SAI_MAP_ANIMS->x = x;
    SAI_MAP_ANIMS->y = y;
    SAI_MAP_ANIMS->timer = 0;
    SAI_MAP_ANIMS->frames = 4;
    SAI_MAP_ANIMS->frameTime = 6;
    SAI_MAP_ANIMS->frameTime *= 2;
}

void SAI_initRegion2Anims(void) {
    s16 xs[7] = { 0x6B, 0x9E, 0x90, 0xA8, 0xCA, 0xDC, 0xD7 };
    s16 ys[7] = { 0x30, 0x60, 0x75, 0x8B, 0x8F, 0x7A, 0x62 };

    SAI_MAP_ANIMS->x = xs[0];
    SAI_MAP_ANIMS->y = ys[0];
    SAI_MAP_ANIMS->timer = 0;
    SAI_MAP_ANIMS->frames = 6;
    SAI_MAP_ANIMS->frameTime = 4;
    SAI_MAP_ANIMS->frameTime *= 2;
}

void SAI_drawMapAnims(s8 mode) {
    switch (mode) {
    case 0:
        SAI_drawRegion0Anims();
        break;
    case 1:
        SAI_drawRegion1Anims();
        break;
    case 2:
        SAI_drawRegion2Anims();
        break;
    }
}

void SAI_drawRegion0Anims(void) {
    Rect16 uv[8] = {
        { 0x00, 0x00, 0x20, 0x10 },
        { 0x20, 0x00, 0x20, 0x10 },
        { 0x40, 0x00, 0x20, 0x10 },
        { 0x60, 0x00, 0x20, 0x10 },
        { 0x80, 0x00, 0x28, 0x20 },
        { 0xD0, 0x00, 0x20, 0x10 },
        { 0x50, 0x00, 0x20, 0x1A },
        { 0x50, 0x1A, 0x20, 0x20 },
    };
    s32 i;
    u8 frame;

    for (i = 0; i < 6; i++) {
        SAI_MAP_ANIMS[i].timer++;
        if (SAI_MAP_ANIMS[i].timer >= SAI_MAP_ANIMS[i].frames * SAI_MAP_ANIMS[i].frameTime) {
            SAI_MAP_ANIMS[i].timer = 0;
        }
        frame = SAI_MAP_ANIMS[i].timer / SAI_MAP_ANIMS[i].frameTime;
        if (i == 4) {
            uv[i].x += uv[i].w * (frame / 3);
            uv[i].y = (frame % 3) * uv[i].h;
            drawTexturedSprite(SAI_MAP_ANIMS[i].x, SAI_MAP_ANIMS[i].y, &uv[i], 0x3E, 0x7C3B, 0x23, 0x80, 1);
        } else {
            uv[i].y = frame * uv[i].h;
            drawTexturedSprite(SAI_MAP_ANIMS[i].x, SAI_MAP_ANIMS[i].y, &uv[i], 0x3E, 0x7C3B, 0x21, 0x80, 1);
        }
    }
    drawTexturedSprite(0xCC, 0x74, &uv[6], 0x9B, 0x6A18, 0x21, 0x80, 1);
    drawTexturedSprite(0x8F, 0x46, &uv[7], 0x9B, 0x6A18, 0x21, 0x80, 1);
}

void SAI_drawRegion1Anims(void) {
    Rect16 uv[4] = {
        { 0x40, 0x00, 0x48, 0x48 },
        { 0x20, 0x60, 0x48, 0x48 },
        { 0x20, 0xA8, 0x48, 0x48 },
        { 0x20, 0x60, 0x48, 0x48 },
    };
    u8 frame;

    SAI_MAP_ANIMS->timer++;
    if (SAI_MAP_ANIMS->timer >= SAI_MAP_ANIMS->frames * SAI_MAP_ANIMS->frameTime) {
        SAI_MAP_ANIMS->timer = 0;
    }
    frame = SAI_MAP_ANIMS->timer / SAI_MAP_ANIMS->frameTime;
    if (frame != 0) {
        drawTexturedSprite(SAI_MAP_ANIMS->x, SAI_MAP_ANIMS->y, &uv[frame], 0x9E, 0x6A58, 0x23, 0x80, -1);
    }
}

void SAI_drawRegion2Anims(void) {
    Rect16 uv[8] = {
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
    };
    u8 frame;

    drawTexturedSprite(0xBE, 0x28, &uv[1], 0x9B, 0x6A98, 0x1D, 0x80, 1);
    SAI_MAP_ANIMS->timer++;
    if (SAI_MAP_ANIMS->timer >= SAI_MAP_ANIMS->frames * SAI_MAP_ANIMS->frameTime) {
        SAI_MAP_ANIMS->timer = 0;
    }
    frame = SAI_MAP_ANIMS->timer / SAI_MAP_ANIMS->frameTime;
    if (frame != 0) {
        uv[0].y = (frame - 1) * uv[0].h + 0x60;
        drawTexturedSprite(SAI_MAP_ANIMS->x, SAI_MAP_ANIMS->y, &uv[0], 0x9E, 0x6A98, 0x23, 0x80, -1);
    }
}

void SAI_openMap(void) {
    s32 i;

    SAI_WORLD_MAP.zoom++;
    if (SAI_WORLD_MAP.zoom >= 36) {
        SAI_WORLD_MAP.zoom = 35;
    }
    if (SAI_MAP_ICON_SLIDE == 5) {
        playSoundEffect(10);
    }
    if (SAI_WORLD_MAP.zoom > 20) {
        SAI_WORLD_MAP.iconMotion = 1;
        SAI_WORLD_MAP.iconSlide++;
        if (SAI_WORLD_MAP.iconSlide >= 16) {
            SAI_WORLD_MAP.iconSlide = 15;
            if (SAI_MAP_FRAME_BRIGHTNESS < 0x80) {
                SAI_MAP_FRAME_BRIGHTNESS += 8;
            } else {
                SAI_WORLD_MAP.state = MAP_FADE_IN;
                SAI_initRegion();
                SAI_WORLD_MAP.portraitState = 2;
                SAI_WORLD_MAP.nameSlideDir = 1;
            }
            if (SAI_MAP_FRAME_BRIGHTNESS >= 0x80) {
                SAI_MAP_FRAME_BRIGHTNESS = 0x80;
            }
        }
    }
    SAI_WORLD_MAP.zoomAngle = (SAI_WORLD_MAP.zoom << 10) / 35;
    if (SAI_WORLD_MAP.zoomAngle > 0x400) {
        SAI_WORLD_MAP.zoomAngle = 0x400;
    }
    for (i = 40; i < 48; i++) {
        setRGB0(&SAI_SPRITES[i]->quads[FRAME_BUFFER_INDEX], SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS);
        SAI_SPRITES[i]->pos.vx = SAI_WORLD_MAP.iconSlide * 38 / 15;
        SAI_SPRITES[i]->pos.vy = SAI_WORLD_MAP.iconSlide / 15;
        SAI_SPRITES[i]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
        SAI_SPRITES[i]->rot.vz = -(SAI_WORLD_MAP.zoom << 12) / 35;
    }
    for (i = 48; i < 56; i++) {
        setRGB0(&SAI_SPRITES[i]->quads[FRAME_BUFFER_INDEX], SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS);
        SAI_SPRITES[i]->pos.vx = SAI_WORLD_MAP.iconSlide * 38 / 15;
        SAI_SPRITES[i]->pos.vy = SAI_WORLD_MAP.iconSlide / 15;
        SAI_SPRITES[i]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
        SAI_SPRITES[i]->rot.vz = (SAI_WORLD_MAP.zoom << 12) / 35;
    }
    setRGB0(&SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX], SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS);
    SAI_SPRITES[2]->pos.vx = SAI_WORLD_MAP.iconSlide * 38 / 15;
    SAI_SPRITES[2]->pos.vy = SAI_WORLD_MAP.iconSlide / 15;
    SAI_SPRITES[2]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
    SAI_SPRITES[2]->rot.vz = (SAI_WORLD_MAP.zoom << 12) / 35;
}

void SAI_drawRegionFade(void) {
    if (SAI_MAP_STATE == MAP_FADE_IN || SAI_MAP_STATE == MAP_OPENING) {
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].r0 = SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].g0 = SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].b0 = SAI_WORLD_MAP.alpha;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].x0 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].x0;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].y0 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].y0;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].x1 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].x1;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].y1 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].y1;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].x2 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].x2;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].y2 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].y2;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].x3 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].x3;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].y3 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].y3;
        addPrim(&CURRENT_FRAME_BUFFER->ot[28], &SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[28], &SAI_WORLD_MAP.tpages[FRAME_BUFFER_INDEX]);
    }
}

void SAI_openMapMenu(void) {
    s32 i;

    SAI_MAP_MENU_TAB_STATE = 2;
    SAI_WORLD_MAP.state = MAP_MENU;
    SAI_WORLD_MAP.menuPhase = 0;
    SAI_WORLD_MAP.menuSlide = 0;
    SAI_WORLD_MAP.menuCursor = 100;
    SAI_SPRITES[30] = SAI_createSprite(0x16);
    SAI_SPRITES[31] = SAI_createSprite(0x18);
    SAI_SPRITES[32] = SAI_createSprite(0x19);
    SAI_SPRITES[33] = SAI_createSprite(0x17);
    if (((SessionData *)SESSION_DATA)->playWithoutSaving == 1) {
        SAI_SPRITES[33]->quads[0].clut = getClut(0x200, 0xF1);
        SAI_SPRITES[33]->quads[1].clut = getClut(0x200, 0xF1);
    }
    SAI_SPRITES[30]->pos.vy = -0x24;
    SAI_setSpriteDepth(SAI_SPRITES[30], 0x1A);
    SAI_SPRITES[31]->pos.vy = -0x39;
    SAI_setSpriteDepth(SAI_SPRITES[31], 0x1B);
    SAI_SPRITES[32]->pos.vy = -0x29;
    SAI_setSpriteDepth(SAI_SPRITES[32], 0x1B);
    SAI_SPRITES[33]->pos.vy = -0x19;
    SAI_setSpriteDepth(SAI_SPRITES[33], 0x1B);
    SAI_setSpriteBlendMode(SAI_SPRITES[30], 1);
    for (i = 0; i < 3; i++) {
        SAI_setSpriteBlendMode(SAI_SPRITES[i + 31], 0);
    }
}

void SAI_fadeInRegion(void) {
    s16 value;

    if (SAI_WORLD_MAP.state == MAP_FADE_IN) {
        value = SAI_WORLD_MAP.alpha;
        value -= 8;
        if (value < 0) {
            value = 0;
            if (SAI_WORLD_MAP.openMenu == 0) {
                SAI_WORLD_MAP.state = MAP_IDLE;
                SAI_MAP_MENU_TAB_STATE = 1;
            } else {
                SAI_MAP_OPEN_MENU = 0;
                SAI_openMapMenu();
            }
        }
        SAI_MAP_ALPHA = value;
    }
}

void SAI_selectMapNode(void) {
    if (SAI_WORLD_MAP.nodeIndex >= 0 && SAI_WORLD_MAP.nodeIndex < 12) {
        SAI_WORLD_MAP.state = MAP_ENTER_AREA;
    } else if (SAI_WORLD_MAP.nodeIndex == 12) {
        SAI_WORLD_MAP.nodeIndex = 13;
        SAI_WORLD_MAP.region = 1;
        SAI_WORLD_MAP.state = MAP_CHANGE_REGION;
        playSoundEffect(5);
    } else if (SAI_WORLD_MAP.nodeIndex == 13) {
        SAI_WORLD_MAP.nodeIndex = 12;
        SAI_WORLD_MAP.region = 0;
        SAI_WORLD_MAP.state = MAP_CHANGE_REGION;
        playSoundEffect(5);
    } else if (SAI_WORLD_MAP.nodeIndex == 14) {
        SAI_WORLD_MAP.nodeIndex = 15;
        SAI_WORLD_MAP.region = 2;
        SAI_WORLD_MAP.state = MAP_CHANGE_REGION;
        playSoundEffect(5);
    } else if (SAI_WORLD_MAP.nodeIndex == 15) {
        SAI_WORLD_MAP.nodeIndex = 14;
        SAI_WORLD_MAP.region = 1;
        SAI_WORLD_MAP.state = MAP_CHANGE_REGION;
        playSoundEffect(5);
    }
    SAI_MAP_LABEL_REGION = -1;
}

void SAI_tickMapInput(void) {
    s8 dir = -1;
    s32 dx;
    s32 dy;

    if (SAI_WORLD_MAP.moving == 0) {
        stopSoundVoice(0x17);
        if (PAD_STATES[0]->held & PAD_UP) {
            dir = 0;
        } else if (PAD_STATES[0]->held & PAD_RIGHT) {
            dir = 1;
        } else if (PAD_STATES[0]->held & PAD_DOWN) {
            dir = 2;
        } else if ((u16)PAD_STATES[0]->held & PAD_LEFT) {
            dir = 3;
        } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
            dir = 4;
        } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
            dir = 5;
        }
        if (dir == -1) {
            return;
        }
        if (dir == 4) {
            SAI_selectMapNode();
        } else if (dir == 5) {
            playSoundEffect(1);
            SAI_openMapMenu();
        } else {
            if (SAI_WORLD_MAP.node->next[dir] != -1) {
                SAI_WORLD_MAP.target = &SAI_MAP_NODES[SAI_WORLD_MAP.node->next[dir]];
            } else {
                if (PAD_STATES[0]->pressed & PAD_CROSS) {
                    SAI_selectMapNode();
                } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
                    playSoundEffect(1);
                    SAI_openMapMenu();
                }
                return;
            }
            if (SAI_WORLD_MAP.target->unlocked != 1) {
                if (PAD_STATES[0]->pressed & PAD_CROSS) {
                    SAI_selectMapNode();
                } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
                    playSoundEffect(1);
                    SAI_openMapMenu();
                }
                return;
            }
            playSoundEffectOnVoice(0x17, 7);
            SAI_WORLD_MAP.nodeIndex = SAI_WORLD_MAP.node->next[dir];
            SAI_WORLD_MAP.moving++;
            dx = SAI_WORLD_MAP.target->x - SAI_WORLD_MAP.node->x;
            dy = SAI_WORLD_MAP.target->y - SAI_WORLD_MAP.node->y;
            SAI_WORLD_MAP.angle = ratan2(dy, dx);
            SAI_WORLD_MAP.distance = csqrt(dx * dx + dy * dy);
            SAI_WORLD_MAP.distance /= 64;
            SAI_MAP_STATE = MAP_WALKING;
        }
    }
}

void SAI_setSpriteImage8Bit(Sprite3D *sprite, Rect16 *rect, s8 flip) {
    s32 i;

    for (i = 0; i < 2; i++) {
        sprite->quads[i].tpage = 0x80 | ((rect->y & 0x100) >> 4) | ((rect->x & 0x3C0) >> 6) | ((rect->y & 0x200) << 2);
        if (flip == -1) {
            sprite->quads[i].u0 = sprite->quads[i].u2 = ((rect->x % 64) << 1) + rect->w;
            sprite->quads[i].u1 = sprite->quads[i].u3 = (rect->x % 64) << 1;
        } else {
            sprite->quads[i].u0 = sprite->quads[i].u2 = (rect->x % 64) << 1;
            sprite->quads[i].u1 = sprite->quads[i].u3 = sprite->quads[i].u2 + rect->w;
        }
        sprite->quads[i].v0 = sprite->quads[i].v1 = rect->y;
        sprite->quads[i].v2 = sprite->quads[i].v3 = rect->y + rect->h;
    }
}

void SAI_setSpriteImage4Bit(Sprite3D *sprite, Rect16 *rect) {
    s32 i;

    for (i = 0; i < 2; i++) {
        sprite->quads[i].tpage = ((rect->y & 0x100) >> 4) | ((rect->x & 0x3C0) >> 6) | ((rect->y & 0x200) << 2);
        sprite->quads[i].u0 = sprite->quads[i].u2 = (rect->x % 64) << 2;
        sprite->quads[i].u1 = sprite->quads[i].u3 = sprite->quads[i].u2 + rect->w;
        sprite->quads[i].v0 = sprite->quads[i].v1 = rect->y;
        sprite->quads[i].v2 = sprite->quads[i].v3 = rect->y + rect->h;
    }
}

void SAI_walkMapMarker(void) {
    Rect16 rect;
    s32 angle = SAI_WORLD_MAP.angle & 0xFFF;
    s8 flip = 0;

    SAI_WORLD_MAP.moving++;
    if (SAI_WORLD_MAP.moving >= SAI_WORLD_MAP.distance) {
        SAI_WORLD_MAP.moving = 0;
        SAI_SPRITES[0]->pos.vx = SAI_WORLD_MAP.target->x;
        SAI_SPRITES[0]->pos.vy = SAI_WORLD_MAP.target->y - 15;
        SAI_WORLD_MAP.node = SAI_WORLD_MAP.target;
        SAI_WORLD_MAP.state = MAP_IDLE;
    } else {
        SAI_SPRITES[0]->pos.vx = rcos(SAI_WORLD_MAP.angle) * SAI_WORLD_MAP.moving / 4096;
        SAI_SPRITES[0]->pos.vy = rsin(SAI_WORLD_MAP.angle) * SAI_WORLD_MAP.moving / 4096;
        SAI_SPRITES[0]->pos.vx += SAI_WORLD_MAP.node->x;
        SAI_SPRITES[0]->pos.vy += SAI_WORLD_MAP.node->y - 15;
    }
    if (angle < 0x400) {
        flip = -1;
        rect.x = SAI_WORLD_MAP.moving / 6 % 6 * 13 + 0x19A;
        rect.y = 0xA8;
    } else if (angle < 0x800) {
        rect.x = SAI_WORLD_MAP.moving / 6 % 6 * 13 + 0x19A;
        rect.y = 0xA8;
    } else {
        if (angle >= 0xC00) {
            flip = -1;
        }
        rect.x = SAI_WORLD_MAP.moving / 6 % 6 * 13 + 0x19A;
        rect.y = 0xD0;
    }
    rect.w = 0x1A;
    rect.h = 0x28;
    if (SAI_WORLD_MAP.moving == 0) {
        rect.x = 0x180;
    }
    SAI_setSpriteImage8Bit(SAI_SPRITES[0], &rect, flip);
}

void SAI_fadeOutRegion(void) {
    s16 alpha;

    alpha = SAI_WORLD_MAP.alpha;
    alpha += 8;
    if (alpha > 0xFF) {
        alpha = 0xFF;
        SAI_WORLD_MAP.oldMarkerCount = SAI_WORLD_MAP.markerCount;
        SAI_WORLD_MAP.markerCount = 0;
        if (SAI_WORLD_MAP.state == MAP_CHANGE_REGION) {
            SAI_WORLD_MAP.state = MAP_SWITCH_REGION;
            SAI_setRegionMapImage();
        } else {
            if (SAI_WORLD_MAP.menuChosen != 1) {
                ((PlayerProfile *)PLAYER_PROFILES)->areaId = SESSION->area = SAI_WORLD_MAP.nodeIndex;
                spawnTask(0, -1, 0, 0x400, SAI_loadAreaPak, 0, getCurrentTaskId, 0, 0);
            }
            SAI_MAP_MENU_TAB_STATE = 3;
            SAI_WORLD_MAP.labelRegion = -1;
            SAI_WORLD_MAP.nameSlideDir = 2;
            SAI_WORLD_MAP.portraitState = 4;
            SAI_WORLD_MAP.state = MAP_CLOSING;
            playSoundEffect(11);
        }
        removeFrameCallback((s32)SAI_drawMapPaths);
        SAI_MAP_ANIMATING = 0;
    }
    SAI_WORLD_MAP.alpha = alpha;
    SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].r0 = SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].g0 = SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].b0 = alpha;
    addPrim(&CURRENT_FRAME_BUFFER->ot[29], &SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[29], &SAI_WORLD_MAP.tpages[FRAME_BUFFER_INDEX]);
}

void SAI_switchRegion(void) {
    s32 i = 0;

    if (SAI_MAP_OLD_MARKER_COUNT != 0) {
        do {
            SAI_freeSprite(SAI_SPRITES[i + 21]);
            i++;
        } while (i < SAI_WORLD_MAP.oldMarkerCount);
    }
    SAI_freeSprite(SAI_SPRITES[0]);
    SAI_freeSprite(SAI_SPRITES[1]);
    SAI_MAP_STATE = MAP_FADE_IN;
    SAI_initRegion();
}

void SAI_closeMap(void) {
    s32 i;

    if (SAI_WORLD_MAP.iconSlide == 0) {
        SAI_WORLD_MAP.zoom--;
        if (SAI_WORLD_MAP.zoom < 0) {
            SAI_WORLD_MAP.zoom = 0;
        }
        SAI_WORLD_MAP.zoomAngle = (SAI_WORLD_MAP.zoom << 10) / 35;
        if (SAI_WORLD_MAP.zoomAngle <= 0) {
            SAI_WORLD_MAP.zoomAngle = 0;
        }
    } else {
        SAI_WORLD_MAP.iconSlide--;
        if (SAI_WORLD_MAP.iconSlide < 0) {
            SAI_WORLD_MAP.iconSlide = 0;
        }
    }
    for (i = 40; i < 48; i++) {
        setRGB0(&SAI_SPRITES[i]->quads[FRAME_BUFFER_INDEX], SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS);
        SAI_SPRITES[i]->pos.vx = SAI_WORLD_MAP.zoom * 38 / 35;
        SAI_SPRITES[i]->pos.vy = SAI_WORLD_MAP.zoom / 35;
        SAI_SPRITES[i]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
        SAI_SPRITES[i]->rot.vz = -(SAI_WORLD_MAP.zoom << 12) / 35;
    }
    for (i = 48; i < 56; i++) {
        setRGB0(&SAI_SPRITES[i]->quads[FRAME_BUFFER_INDEX], SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS);
        SAI_SPRITES[i]->pos.vx = SAI_WORLD_MAP.zoom * 38 / 35;
        SAI_SPRITES[i]->pos.vy = SAI_WORLD_MAP.zoom / 35;
        SAI_SPRITES[i]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
        SAI_SPRITES[i]->rot.vz = (SAI_WORLD_MAP.zoom << 12) / 35;
    }
    setRGB0(&SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX], 0, 0, 0);
    SAI_SPRITES[2]->pos.vx = SAI_WORLD_MAP.zoom * 38 / 35;
    SAI_SPRITES[2]->pos.vy = SAI_WORLD_MAP.zoom / 35;
    SAI_SPRITES[2]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
    SAI_SPRITES[2]->rot.vz = (SAI_WORLD_MAP.zoom << 12) / 35;
    SAI_MAP_FRAME_BRIGHTNESS -= 8;
    if (SAI_MAP_FRAME_BRIGHTNESS < 0x40) {
        SAI_MAP_FRAME_BRIGHTNESS = 0x40;
    }
    if (SAI_WORLD_MAP.zoomAngle == 0 && SAI_WORLD_MAP.zoom == 0) {
        SAI_WORLD_MAP.state = MAP_DONE;
    }
}

void SAI_deactivateMap(void) {
    SAI_MAP_ACTIVE = 0;
}

void (*SAI_MAP_STATE_FUNCS[10])(void) = {
    SAI_openMap,
    SAI_tickMapInput,
    SAI_walkMapMarker,
    SAI_fadeInRegion,
    SAI_fadeOutRegion,
    SAI_switchRegion,
    SAI_fadeOutRegion,
    SAI_closeMap,
    SAI_deactivateMap,
    SAI_tickMapMenu,
};

void SAI_createRegionLabel(void) {
    SAI_WORLD_MAP.shownLabelRegion = -1;
    SAI_WORLD_MAP.labelRegion = -1;
    SAI_WORLD_MAP.labelSlide = 0;
    SAI_SPRITES[4] = SAI_createSprite(6);
    SAI_SPRITES[4]->pos.vx = -0x5F;
    SAI_SPRITES[4]->pos.vy = -0x98;
    SAI_setSpriteDepth(SAI_SPRITES[4], 0x18);
    SAI_SPRITES[3] = SAI_createSprite(7);
    SAI_SPRITES[3]->pos.vx = -0x5F;
    SAI_SPRITES[3]->pos.vy = -0x97;
    SAI_setSpriteDepth(SAI_SPRITES[3], 0x19);
    SAI_SPRITES[3]->quads[0].clut = SAI_SPRITES[3]->quads[1].clut = getClut(0x200, 0xEA);
}

void SAI_drawRegionLabel(void) {
    Rect16 rect;

    rect.x = 0x200;
    rect.y = SAI_WORLD_MAP.labelRegion * 24 + 0x30;
    rect.w = 0x74;
    rect.h = 0x18;
    if (SAI_WORLD_MAP.labelRegion != SAI_WORLD_MAP.shownLabelRegion || SAI_WORLD_MAP.labelRegion == -1) {
        SAI_WORLD_MAP.labelSlide--;
        if (SAI_WORLD_MAP.labelSlide < 0) {
            SAI_WORLD_MAP.labelSlide = 0;
            SAI_WORLD_MAP.shownLabelRegion = SAI_WORLD_MAP.labelRegion;
            if (SAI_WORLD_MAP.shownLabelRegion != -1) {
                SAI_setSpriteImage4Bit(SAI_SPRITES[3], &rect);
            }
        }
    } else {
        SAI_WORLD_MAP.labelSlide++;
        if (SAI_WORLD_MAP.labelSlide > 20) {
            SAI_WORLD_MAP.labelSlide = 20;
        }
    }
    SAI_SPRITES[4]->pos.vy = (SAI_WORLD_MAP.labelSlide * -97 + (20 - SAI_WORLD_MAP.labelSlide) * -152) / 20;
    SAI_SPRITES[3]->pos.vy = SAI_SPRITES[4]->pos.vy + 2;
    SAI_drawSprite(SAI_SPRITES[3]);
    SAI_drawSprite(SAI_SPRITES[4]);
}

void SAI_createAreaPortrait(void) {
    s32 i;

    if (SAI_MAP_PORTRAIT_STATE == 0) {
        SAI_SPRITES[35] = SAI_createSprite(0x1A);
        SAI_SPRITES[36] = SAI_createSprite(0x1B);
        SAI_SPRITES[37] = SAI_createSprite(0x1C);
        for (i = 35; i < 38; i++) {
            SAI_setSpriteDepth(SAI_SPRITES[i], 0x1B);
        }
        SAI_SPRITES[35]->pos.vy = 0x3B;
        SAI_SPRITES[36]->pos.vy = SAI_SPRITES[37]->pos.vy = 0x32;
        SAI_MAP_PORTRAIT_STATE = 1;
    }
    SAI_WORLD_MAP.portraitSlide = 0;
    SAI_WORLD_MAP.portraitBlink = 0;
}

void SAI_drawAreaPortrait(void) {
    Rect16 rect;

    SAI_SPRITES[35]->pos.vx = (SAI_WORLD_MAP.portraitSlide * -108 + (20 - SAI_WORLD_MAP.portraitSlide) * -200) / 20;
    SAI_SPRITES[36]->pos.vx = SAI_SPRITES[37]->pos.vx = SAI_SPRITES[35]->pos.vx - 7;
    SAI_WORLD_MAP.portraitBlink = (SAI_WORLD_MAP.portraitBlink + 1) & 0xFFF;
    if (SAI_WORLD_MAP.portraitState == 2) {
        SAI_WORLD_MAP.portraitSlide += 2;
        if (SAI_WORLD_MAP.portraitSlide > 20) {
            SAI_WORLD_MAP.portraitSlide = 20;
            SAI_WORLD_MAP.portraitState = 3;
        }
    } else if (SAI_WORLD_MAP.portraitState == 4) {
        SAI_WORLD_MAP.portraitSlide -= 2;
        if (SAI_WORLD_MAP.portraitSlide < 0) {
            SAI_WORLD_MAP.portraitSlide = 0;
            SAI_WORLD_MAP.portraitState = 1;
        }
    }
    SAI_drawSprite(SAI_SPRITES[35]);
    if (SAI_WORLD_MAP.nodeIndex >= 12 || SAI_WORLD_MAP.moving != 0 || SAI_WORLD_MAP.portraitSlide != 20) {
        if (SAI_WORLD_MAP.portraitBlink & 4) {
            rect.x = 0x318;
        } else {
            rect.x = 0x328;
        }
        rect.y = 0xC8;
        rect.w = 0x40;
        rect.h = 0x37;
        SAI_setSpriteImage4Bit(SAI_SPRITES[36], &rect);
        SAI_drawSprite(SAI_SPRITES[36]);
    } else {
        rect.x = SAI_MAP_NODE % 4 * 32 + 0x180;
        rect.y = SAI_MAP_NODE / 4 * 56;
        rect.w = 0x40;
        rect.h = 0x37;
        SAI_setSpriteImage8Bit(SAI_SPRITES[37], &rect, 0);
        SAI_drawSprite(SAI_SPRITES[37]);
    }
}

void SAI_createMapAreaName(void) {
    SAI_MAP_NAME_SLIDE_DIR = 0;
    SAI_SPRITES[38] = SAI_createSprite(0x1F);
    SAI_SPRITES[39] = SAI_createSprite(0x1E);
    SAI_SPRITES[38]->pos.vx = 0x26;
    SAI_SPRITES[39]->pos.vx = 0x25;
    SAI_setSpriteDepth(SAI_SPRITES[38], 0x19);
    SAI_setSpriteDepth(SAI_SPRITES[39], 0x19);
}

void SAI_drawMapAreaName(void) {
    Rect16 rect;
    s32 i;

    SAI_SPRITES[39]->pos.vy = (SAI_WORLD_MAP.nameSlide * 89 + (20 - SAI_WORLD_MAP.nameSlide) * 134) / 20;
    SAI_SPRITES[38]->pos.vy = SAI_SPRITES[39]->pos.vy + 1;
    if (SAI_WORLD_MAP.nameSlideDir == 1) {
        SAI_WORLD_MAP.nameSlide += 2;
        if (SAI_WORLD_MAP.nameSlide > 20) {
            SAI_WORLD_MAP.nameSlide = 20;
            SAI_WORLD_MAP.nameSlideDir = 0;
        }
    } else if (SAI_WORLD_MAP.nameSlideDir == 2) {
        SAI_WORLD_MAP.nameSlide -= 2;
        if (SAI_WORLD_MAP.nameSlide < 0) {
            SAI_WORLD_MAP.nameSlide = 0;
            SAI_WORLD_MAP.nameSlideDir = 0;
        }
    }
    if (SAI_MAP_NODE >= 12) {
        switch (SAI_MAP_NODE) {
        case 12:
            rect.x = 0x2C0;
            rect.y = 0x5A;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        case 13:
            rect.x = 0x2C0;
            rect.y = 0x46;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        case 14:
            rect.x = 0x2C0;
            rect.y = 0x6E;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        case 15:
            rect.x = 0x2C0;
            rect.y = 0x5A;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        }
    } else {
        rect.x = SAI_MAP_NODE / 6 * 24 + 0x340;
        rect.y = SAI_MAP_NODE % 6 * 20;
        rect.w = 0x60;
        rect.h = 0x14;
    }
    SAI_setSpriteImage4Bit(SAI_SPRITES[38], &rect);
    for (i = 38; i < 40; i++) {
        SAI_drawSprite(SAI_SPRITES[i]);
    }
}

void SAI_createMapFrame(void) {
    s16 xs[4] = { -0x80, -0x58, 0x58, 0x7C };
    s16 ys[4] = { -0x78, -0x48, 0x48, 0x78 };
    s32 i;
    s32 col;
    s32 row;

    for (i = 40; i < 48; i++) {
        SAI_SPRITES[i] = SAI_createSprite(i - 8);
        SAI_setSpriteDepth(SAI_SPRITES[i], 0x1A);
    }
    SAI_SPRITES[40]->corners[0].vx = SAI_SPRITES[40]->corners[2].vx = xs[1];
    SAI_SPRITES[40]->corners[1].vx = SAI_SPRITES[40]->corners[3].vx = xs[2];
    SAI_SPRITES[40]->corners[0].vy = SAI_SPRITES[40]->corners[1].vy = ys[0];
    SAI_SPRITES[40]->corners[2].vy = SAI_SPRITES[40]->corners[3].vy = ys[1];
    SAI_SPRITES[41]->corners[0].vx = SAI_SPRITES[41]->corners[2].vx = xs[1];
    SAI_SPRITES[41]->corners[1].vx = SAI_SPRITES[41]->corners[3].vx = xs[2];
    SAI_SPRITES[41]->corners[0].vy = SAI_SPRITES[41]->corners[1].vy = ys[2];
    SAI_SPRITES[41]->corners[2].vy = SAI_SPRITES[41]->corners[3].vy = ys[3];
    for (i = 0; i < 6; i++) {
        col = i / 3;
        row = i % 3;
        SAI_SPRITES[i + 42]->corners[0].vx = SAI_SPRITES[i + 42]->corners[2].vx = xs[col * 2];
        SAI_SPRITES[i + 42]->corners[1].vx = SAI_SPRITES[i + 42]->corners[3].vx = xs[col * 2 + 1];
        SAI_SPRITES[i + 42]->corners[0].vy = SAI_SPRITES[i + 42]->corners[1].vy = ys[row];
        SAI_SPRITES[i + 42]->corners[2].vy = SAI_SPRITES[i + 42]->corners[3].vy = ys[row + 1];
    }
}

void SAI_createMapFrameShadow(void) {
    s16 xs[4] = { -0x8B, -0x53, 0x5D, 0x89 };
    s16 ys[4] = { -0x75, -0x45, 0x43, 0x77 };
    s32 i;
    s32 col;
    s32 row;
    s32 id;

    for (i = 48, id = 40; i < 56; i++, id++) {
        SAI_SPRITES[i] = SAI_createSprite(id);
        SAI_setSpriteDepth(SAI_SPRITES[i], 0x1C);
    }
    SAI_SPRITES[48]->corners[0].vx = SAI_SPRITES[48]->corners[2].vx = xs[1];
    SAI_SPRITES[48]->corners[1].vx = SAI_SPRITES[48]->corners[3].vx = xs[2];
    SAI_SPRITES[48]->corners[0].vy = SAI_SPRITES[48]->corners[1].vy = ys[0];
    SAI_SPRITES[48]->corners[2].vy = SAI_SPRITES[48]->corners[3].vy = ys[1];
    SAI_SPRITES[49]->corners[0].vx = SAI_SPRITES[49]->corners[2].vx = xs[1];
    SAI_SPRITES[49]->corners[1].vx = SAI_SPRITES[49]->corners[3].vx = xs[2];
    SAI_SPRITES[49]->corners[0].vy = SAI_SPRITES[49]->corners[1].vy = ys[2];
    SAI_SPRITES[49]->corners[2].vy = SAI_SPRITES[49]->corners[3].vy = ys[3];
    for (i = 0; i < 6; i++) {
        col = i / 3;
        row = i % 3;
        SAI_SPRITES[i + 50]->corners[0].vx = SAI_SPRITES[i + 50]->corners[2].vx = xs[col * 2];
        SAI_SPRITES[i + 50]->corners[1].vx = SAI_SPRITES[i + 50]->corners[3].vx = xs[col * 2 + 1];
        SAI_SPRITES[i + 50]->corners[0].vy = SAI_SPRITES[i + 50]->corners[1].vy = ys[row];
        SAI_SPRITES[i + 50]->corners[2].vy = SAI_SPRITES[i + 50]->corners[3].vy = ys[row + 1];
    }
}

void SAI_drawMapFrame(void) {
    s32 i;

    for (i = 0x28; i < 0x30; i++) {
        SAI_drawSprite(SAI_SPRITES[i]);
    }
}

void SAI_drawMapFrameShadow(void) {
    s32 i;

    for (i = 0x30; i < 0x38; i++) {
        SAI_drawSprite(SAI_SPRITES[i]);
    }
}

void SAI_unlockMapNodes(void) {
    u16 ids[12] = { 0x11A, 0x11B, 0x11C, 0x11D, 0x11E, 0x11F, 0x120, 0x121, 0x122, 0x123, 0x124, 0x125 };
    s32 i;

    for (i = 0; i < 12; i++) {
        SAI_MAP_NODES[i].unlocked = SAI_isSavedScriptFlagSet(ids[i]);
    }
    for (i = 12; i < 16; i++) {
        SAI_MAP_NODES[i].unlocked = 0;
    }
    if (SAI_isSavedScriptFlagSet(ids[5])) {
        SAI_MAP_NODES[12].unlocked = 1;
        SAI_MAP_NODES[13].unlocked = 1;
    }
    if (SAI_isSavedScriptFlagSet(ids[9])) {
        SAI_MAP_NODES[14].unlocked = 1;
        SAI_MAP_NODES[15].unlocked = 1;
    }
}

void SAI_setRegionMapImage(void) {
    SAI_setSpriteImage8Bit(SAI_SPRITES[2], &SAI_REGION_MAP_RECTS[SAI_WORLD_MAP.region], 0);
    SAI_SPRITES[2]->quads[0].clut = SAI_SPRITES[2]->quads[1].clut = getClut(0x180, SAI_WORLD_MAP.region + 0x1A8);
}

/*
 * The world map task: the player's marker walks between the unlocked nodes of
 * the three regions; entering a node loads its area, and the menu leads to the
 * deck editor, the partner equipment or the save screen.
 */
void SAI_runWorldMap(s32 resume, s32 openMenu) {
    s32 i;
    s32 j;
    s8 node;

    if (resume == 0) {
        spawnTask(0, -1, 0, 0x400, SAI_loadMapTextures, 0, getCurrentTaskId, 0, 0);
        do {
            waitFrames(1);
        } while (SESSION->loading != 0);
    }
    loadSoundEffectBank(1);
    SAI_initCamera();
    SAI_initPathPolys();
    SAI_unlockMapNodes();
    SAI_createMapMenuTab(0);
    if (SAI_ICON_RUNNING != 1) {
        spawnTask(0, -1, 0, 0x400, SAI_runCornerIcon, 0, getCurrentTaskId(), 0, 0);
    }
    SAI_MAP_ANIMS = allocTaskHeapBlock(sizeof(MapAnim) * 6);
    SAI_initMapAnims(SAI_WORLD_MAP.region);
    SAI_createMapFrame();
    SAI_createMapFrameShadow();
    SESSION_SUB->area = SAI_WORLD_MAP.nodeIndex = ((PlayerProfile *)PLAYER_PROFILES)->areaId;
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 7; i++) {
            if (((PlayerProfile *)PLAYER_PROFILES)->areaId == SAI_REGION_NODES[j][i]) {
                SAI_WORLD_MAP.region = j;
            }
        }
    }
    SAI_MAP_FRAME_BRIGHTNESS = 0x40;
    SAI_WORLD_MAP.active = 1;
    SAI_WORLD_MAP.iconSlide = 0;
    SAI_WORLD_MAP.zoom = 0;
    SAI_WORLD_MAP.state = MAP_OPENING;
    SAI_WORLD_MAP.portraitState = 0;
    SAI_WORLD_MAP.alpha = 0xFF;
    SAI_WORLD_MAP.menuChosen = 0;
    SAI_WORLD_MAP.markerCount = 0;
    SAI_WORLD_MAP.animating = 0;
    SAI_WORLD_MAP.lastRegion = SAI_WORLD_MAP.region;
    SAI_WORLD_MAP.openMenu = openMenu;
    SAI_createRegionLabel();
    SAI_createAreaPortrait();
    SAI_createMapAreaName();
    SAI_SPRITES[2] = SAI_createSprite(2);
    SAI_setRegionMapImage();
    for (i = 0; i < 2; i++) {
        setRGB0(&SAI_SPRITES[2]->quads[i], 0, 0, 0);
    }
    SAI_setSpriteDepth(SAI_SPRITES[2], 0x23);
    for (i = 0; i < 2; i++) {
        SetPolyF4(&SAI_WORLD_MAP.fades[i]);
        SetSemiTrans(&SAI_WORLD_MAP.fades[i], 1);
        SetDrawTPage(&SAI_WORLD_MAP.tpages[i], 0, 0, 0x40);
        SAI_WORLD_MAP.fades[i].r0 = SAI_WORLD_MAP.fades[i].g0 = SAI_WORLD_MAP.fades[i].b0 = 0xFF;
        SAI_WORLD_MAP.fades[i].x0 = SAI_WORLD_MAP.fades[i].x2 = 100;
        SAI_WORLD_MAP.fades[i].x1 = SAI_WORLD_MAP.fades[i].x3 = 0x128;
        SAI_WORLD_MAP.fades[i].y0 = SAI_WORLD_MAP.fades[i].y1 = 0x28;
        SAI_WORLD_MAP.fades[i].y2 = SAI_WORLD_MAP.fades[i].y3 = 0xC8;
    }
    do {
        waitFrames(1);
        if (SAI_MAP_STATE_FUNCS[SAI_WORLD_MAP.state] != NULL) {
            SAI_MAP_STATE_FUNCS[SAI_WORLD_MAP.state]();
        }
        for (i = 0; i < SAI_WORLD_MAP.markerCount; i++) {
            node = SAI_REGION_NODES[SAI_WORLD_MAP.lastRegion][i];
            if (SAI_MAP_NODES[node].unlocked == 1) {
                SAI_drawSprite(SAI_SPRITES[i + 21]);
            }
        }
        if (SAI_WORLD_MAP.markerCount != 0) {
            SAI_drawSprite(SAI_SPRITES[0]);
            SAI_SPRITES[1]->pos.vx = SAI_SPRITES[0]->pos.vx;
            SAI_SPRITES[1]->pos.vy = SAI_SPRITES[0]->pos.vy + 15;
            SAI_drawSprite(SAI_SPRITES[1]);
        }
        SAI_drawMapMenuTab();
        SAI_drawMapFrame();
        SAI_drawMapFrameShadow();
        if (SAI_WORLD_MAP.animating != 0) {
            SAI_drawMapAnims(SAI_WORLD_MAP.animRegion);
        }
        SAI_drawSprite(SAI_SPRITES[2]);
        SAI_drawRegionFade();
        SAI_drawMapAreaName();
        SAI_drawRegionLabel();
        if (SAI_WORLD_MAP.portraitState >= 2) {
            SAI_drawAreaPortrait();
        }
    } while (SAI_WORLD_MAP.state != MAP_DONE);
    waitFrames(1);
    freeHeapBlocksByTag(0x2E);
    SAI_WORLD_MAP.active = -1;
    endTask(0x19);
    freeHeapBlocksByTag(0x28);
    freeHeapBlocksByTag(0x7F);
    freeHeapBlocksByTag(0x29);
    waitFrames(0x1E);
    SESSION_SUB->unk1A2 = 0;
    SESSION_SUB->unk1A5 = 0;
    ((PlayerProfile *)PLAYER_PROFILES)->areaId = SESSION_SUB->area = (u8)SAI_WORLD_MAP.nodeIndex;
    if (SAI_WORLD_MAP.menuChosen == 1) {
        SAI_WORLD_MAP.iconRunning = 0;
        switch (SAI_WORLD_MAP.menuCursor) {
        case 0:
            spawnTask(0, -1, 0, 0x1600, SAI_openDeckEditorFromMap, 0, getCurrentTaskId(), 0, 0);
            break;
        case 1:
            spawnTask(0, -1, 0, 0x1600, SAI_openEquipmentFromMap, 0, getCurrentTaskId(), 0, 0);
            break;
        case 2:
            ((PlayerProfile *)PLAYER_PROFILES)->resumeInArea = 0;
            spawnTask(0, -1, 0, 0x400, openSaveScreenFromMap, 2, getCurrentTaskId(), 0, 0);
            break;
        }
    } else {
        do {
            waitFrames(1);
        } while (SESSION->loading != 0);
        waitFrames(1);
        spawnTask(0, -1, 0, 0x1600, SAI_runArea, 1, getCurrentTaskId(), 0, 0);
    }
    exitTask();
}

void SAI_initMapPaths(void) {
    MapPath *path;
    MapNode *node;
    s32 dx;
    s32 dy;
    s32 i;
    s32 j;
    s8 next;
    s32 angle;

    SAI_initMapAnims(SAI_WORLD_MAP.region);
    path = SAI_WORLD_MAP.paths;
    SAI_WORLD_MAP.animating = 1;
    SAI_WORLD_MAP.pathCount = 0;
    for (i = 0; i < 7; i++) {
        if (SAI_WORLD_MAP.route[i] == -1) {
            continue;
        }
        node = &SAI_MAP_NODES[SAI_WORLD_MAP.route[i]];
        if (node->unlocked != 1) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            next = node->next[j];
            if (i < next && next >= 0 && SAI_MAP_NODES[next].unlocked != 0) {
                dx = SAI_MAP_NODES[next].x - node->x;
                dy = SAI_MAP_NODES[next].y - node->y;
                angle = (s16)ratan2(dy, dx);
                path->rot.vz = -angle;
                path->pos.vx = node->x;
                path->pos.vy = node->y;
                path->length = csqrt(dx * dx + dy * dy);
                path->length /= 64;
                SAI_WORLD_MAP.pathCount++;
                path++;
            }
        }
    }
    addFrameCallback((s32)SAI_drawMapPaths);
}

void SAI_drawMapPaths(FrameBuffer *fb) {
    MATRIX matrix;
    SVECTOR corners[4];
    s32 sxy[4];
    s32 depthCue;
    s32 flag;
    s8 i;

    for (i = 0; i < SAI_WORLD_MAP.pathCount; i++) {
        buildRotTransMatrix(&SAI_WORLD_MAP.paths[i].pos, &SAI_WORLD_MAP.paths[i].rot, &matrix);
        CompMatrix((MATRIX *)SCENE_3D->viewMatrix, &matrix, &matrix);
        SetRotMatrix((s32)&matrix);
        SetTransMatrix(&matrix);
        corners[0].vx = 0;
        corners[0].vy = 2;
        corners[0].vz = 0;
        corners[1].vx = SAI_WORLD_MAP.paths[i].length;
        corners[1].vy = 2;
        corners[1].vz = 0;
        corners[2].vx = 0;
        corners[2].vy = -2;
        corners[2].vz = 0;
        corners[3].vx = SAI_WORLD_MAP.paths[i].length;
        corners[3].vy = -2;
        corners[3].vz = 0;
        RotAverage4(&corners[0], &corners[1], &corners[2], &corners[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
        ((PolyF4 *)fb->primSlots[1])[i].x0 = sxy[0];
        ((PolyF4 *)fb->primSlots[1])[i].y0 = sxy[0] >> 16;
        ((PolyF4 *)fb->primSlots[1])[i].x1 = sxy[1];
        ((PolyF4 *)fb->primSlots[1])[i].y1 = sxy[1] >> 16;
        ((PolyF4 *)fb->primSlots[1])[i].x2 = sxy[2];
        ((PolyF4 *)fb->primSlots[1])[i].y2 = sxy[2] >> 16;
        ((PolyF4 *)fb->primSlots[1])[i].x3 = sxy[3];
        ((PolyF4 *)fb->primSlots[1])[i].y3 = sxy[3] >> 16;
        addPrim(&fb->ot[34], &((PolyF4 *)fb->primSlots[1])[i]);
        addPrim(&fb->ot[34], &SAI_WORLD_MAP.pathTpages[FRAME_BUFFER_INDEX][i]);
    }
}

void SAI_createMapMenuTab(s8 keepTabState) {
    if (keepTabState == 0) {
        SAI_MAP_MENU_TAB_STATE = keepTabState;
    }
    SAI_MAP_MENU_TAB.offset = 0;
    SAI_SPRITES[5] = SAI_createSprite(0x41);
    SAI_SPRITES[5]->pos.vx = -0xD5;
    SAI_SPRITES[5]->pos.vy = -0x24;
    SAI_setSpriteDepth(SAI_SPRITES[5], 0x1A);
}

void SAI_drawMapMenuTab(void) {
    char buf[0x48]; /* unused, but it is in the original stack frame */
    s32 x;
    s32 state;

    state = SAI_MAP_MENU_TAB.state;
    if (state == 1) {
        SAI_MAP_MENU_TAB.offset++;
        if (SAI_MAP_MENU_TAB.offset > 20) {
            SAI_MAP_MENU_TAB.offset = 20;
        }
    } else if (state > 0) {
        if (state < 4) {
            SAI_MAP_MENU_TAB.offset--;
            if (SAI_MAP_MENU_TAB.offset < 0) {
                SAI_MAP_MENU_TAB.offset = 0;
            }
        }
    }
    x = ((20 - SAI_MAP_MENU_TAB.offset) * -213 - SAI_MAP_MENU_TAB.offset * 116) / 20;
    SAI_SPRITES[5]->pos.vx = x;
    SAI_drawSprite(SAI_SPRITES[5]);
}

/* not called by any code */
void SAI_doNothingOnMap(void) {
}

void SAI_openDeckEditorFromMap(void) {
    openDeckEditor(0);
}

void SAI_openEquipmentFromMap(void) {
    openPartnerEquipment(0);
}
