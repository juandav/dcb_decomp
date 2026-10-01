#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/pad.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/scene3d.h"
#include "dcb/scroll_bg.h"
#include "dcb/prim_desc.h"
#include "dcb/saiseg.h"

/* jp's world map (sai_world_map.c is us's and eu's): the map of the 15
   places with the paths between them, the player walking along them, the
   menu of what to do where the player is, and a label with the place's
   name. Its screens are callbacks of the executable's callback slots. */

/* the label with the name of the place under the cursor, which slides in
   from the right and then opens */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ u8 u; /* the name's place in the names' texture */
    /* 0x5 */ u8 v;
    /* 0x6 */ u8 w; /* how much of the name shows */
    /* 0x7 */ u8 h;
    /* 0x8 */ s8 timer;
    /* 0x9 */ u8 unk9;
} MapLabel;

/* a walk along a straight path, a step a frame (Bresenham's) */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 error;
    /* 0x6 */ s16 steps;
    /* 0x8 */ s16 step;
} MapWalk;

typedef struct {
    /* 0x000 */ ChoiceMenu menu; /* what to do where the player is */
    /* 0x24C */ u8 unk24C[8];
    /* 0x254 */ MapWalk walk;
    /* 0x25E */ MapLabel label;
    /* 0x268 */ s8 labelDir; /* 1: the label slides in, -1: out */
    /* 0x269 */ s8 walkFrame;
    /* 0x26A */ u8 path[16]; /* the places to walk through, 0-terminated */
    /* 0x27A */ u8 pathStep;
    /* 0x27B */ u8 from;
    /* 0x27C */ u8 to;
    /* 0x27D */ u8 timer;
} MapScreen;

/* the map's sprites */
enum {
    MAP_SPRITE_MAP,
    MAP_SPRITE_PLACE = 2,
    MAP_SPRITE_CURSOR,
    MAP_SPRITE_PATH, /* and its shadow */
    MAP_SPRITE_PLAYER_SHADOW = 6,
    MAP_SPRITE_LABEL,
    MAP_SPRITE_LABEL_ARROW,
    MAP_SPRITE_LABEL_PLATE,
    MAP_SPRITE_DIGIT
};

/* the places off the ring of the other 12, and the place they hang from */
#define PLACE_OFF_1 15
#define PLACE_OFF_14 13
#define PLACE_OFF_2 5

/* this module's menu calls return ints */
void openChoiceMenu(ChoiceMenu *menu, s32 icon, s32 y, void (*cancel)(), s32 *arg);
void addChoiceMenuItem(ChoiceMenu *menu, s32 item, void (*action)());
s32 runChoiceMenu(ChoiceMenu *menu);
void startChoiceMenuAction(ChoiceMenu *menu);
void startAreaPakLoad(void);
extern JpGame *SCROLLING_BACKGROUND;

void runOptionScreen();
void openMemcardScreenForWorldMap();
void openDeckEditorFromArea();
void SAI_returnToArea(void);
void SAI_runKeyItems(void);
void SAI_moveOnMap(void);
u8 *formatSjisNumber(s32 value, s32 width, u8 *dst);

extern SpriteDesc *D_801E469C; /* the sprites the executable draws */
extern s8 D_801E46D8[]; /* splitDigits' digits */
extern u8 OPTION_FROM_WORLD_MAP;

/* the map's data that starts zeroed (sai_bss_jp.c) */
extern PrimDesc3D SAI_MAP_PLAYER_DRAWN;
extern s8 SAI_MAP_LABEL_PLACE;
extern s8 SAI_MAP_CURSOR;
extern s8 SAI_MAP_PLACE; /* where the player is */
extern s16 SAI_MAP_X;
extern s16 SAI_MAP_Y;

void SAI_openWorldMap(MapScreen *map);
s32 SAI_slideWorldMapIn(MapScreen *map);
s32 SAI_slideWorldMapOut(MapScreen *map);
s32 SAI_tickWorldMapMenu(MapScreen *map);
s32 SAI_drawWorldMap(MapScreen *map);
void SAI_drawMapPaths(void);
void SAI_tickMapCursor(MapScreen *map);
s32 SAI_switchMapLabel(MapScreen *map);
void SAI_startMapWalk(MapScreen *map, u8 from, u8 to);
s32 SAI_tickMapWalk(void *arg);
void SAI_stepMapWalk(MapWalk *walk, s16 x0, s16 y0, s16 x1, s16 y1);
void SAI_animateMapPlayer(MapScreen *map);
s32 SAI_drawMapLabels(MapLabel *labels, s16 unused);
s32 SAI_slideMapLabel(MapLabel *label, s8 dir);
s16 SAI_findMapPath(u8 *path);
void SAI_drawMapBits(JpWindow *win);
void SAI_tickMapBitsWindow(JpWindow *win);

/* not opened by any code */
JpWindowDef SAI_MAP_BITS_WINDOW = { { 0x126, 0x36, 0, 0xE }, { 0xAE, 0x36, 0x78, 0xE }, 0xA, 1, SAI_drawMapBits, SAI_tickMapBitsWindow };

/* the map's sprites (MAP_SPRITE_*) */
SpriteDesc SAI_MAP_SPRITES[11] = {
    { 0x90, 128, 128, 128, 0x64, 0, 0, 0x7C40, 0, 0x99, 96, 29, 224, 176 },
    { 0x80, 128, 128, 128, 0x64, 112, 189, 0x7F79, 0, 0x1D, 0, 0, 16, 15 },
    { 0x80, 128, 128, 128, 0x64, 128, 189, 0x7F79, 0, 0x1D, 0, 0, 14, 14 },
    { 0x80, 128, 128, 128, 0x64, 160, 189, 0x7F79, 0, 0x1D, 0, 0, 16, 15 },
    { 0x80, 255, 136, 24, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x80, 255, 136, 24, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x7F, 128, 128, 128, 0x66, 208, 176, 0x7C80, 0, 0xD9, 0, 0, 24, 16 },
    { 0x80, 128, 128, 128, 0x2C, 0, 0, 0x7F38, 0x157F, 0x1E, 0, 0, 0, 0 },
    { 0x80, 128, 128, 128, 0x64, 64, 189, 0x7F79, 0, 0x1D, 0, 0, 20, 21 },
    { 0x80, 128, 128, 128, 0x64, 0, 212, 0x7FF8, 0, 0x1C, 0, 0, 180, 36 },
    { 0x7F, 128, 128, 128, 0x64, 64, 229, 0x7FF8, 0, 0x1D, 0, 0, 8, 11 },
};

/* the player's figure walking on the map */
PrimDesc3D SAI_MAP_PLAYER = { 0, 128, 128, 128, 0x2C, 0, 176, 0x7C80, 26, 40, 0x99, { 0, 0 }, { 0, 0, 448, 0 }, { 0, 0, 0, 0 }, 26, 40 };

/* where each place is on the map */
s16 SAI_MAP_PLACE_X[16] = { 105, 110, 80, 73, 46, 39, 33, 58, 75, 140, 188, 140, 183, 156, 96, 0 };
s16 SAI_MAP_PLACE_Y[16] = { 103, 139, 114, 92, 123, 95, 63, 50, 24, 34, 64, 72, 96, 113, 65, 0 };
/* the places each path joins, bit n for place n + 1 */
u16 SAI_MAP_PATH_ENDS[16] = { 0x5, 0xC, 0x28, 0x60, 0xC0, 0x180, 0x300, 0x600, 0xC00, 0x2800, 0x2002, 0x3, 0x3000, 0x12, 0x4001, 0x0 };
u16 SAI_MAP_PATH_CLUTS[15] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
/* the place the cursor moves to from each place: left, down, right, up */
u8 SAI_MAP_NEIGHBOURS[16][4] = {
    { 3, 2, 12, 15 }, { 5, 0, 14, 1 }, { 4, 2, 1, 4 }, { 6, 3, 3, 8 },
    { 6, 2, 2, 6 }, { 7, 5, 4, 7 }, { 0, 6, 8, 8 }, { 7, 7, 9, 9 },
    { 8, 8, 10, 0 }, { 9, 12, 11, 9 }, { 12, 13, 0, 10 }, { 1, 14, 11, 10 },
    { 14, 14, 0, 11 }, { 2, 2, 13, 12 }, { 4, 1, 12, 9 }, { 1, 0, 0, 1 },
};

#define SET_CALLBACK(f) (*CURRENT_CALLBACK_SLOT = (void (*)(void *))(f))

#define OPEN_MAP_MENU(map)                                                \
    openChoiceMenu(&(map)->menu, -6, 50, NULL, NULL);                     \
    addChoiceMenuItem(&(map)->menu, 13, SAI_moveOnMap);                   \
    addChoiceMenuItem(&(map)->menu, 14, SAI_returnToArea);                   \
    addChoiceMenuItem(&(map)->menu, 2, openDeckEditorFromArea);           \
    addChoiceMenuItem(&(map)->menu, 16, openMemcardScreenForWorldMap);                      \
    addChoiceMenuItem(&(map)->menu, 3, runOptionScreen);                    \
    addChoiceMenuItem(&(map)->menu, 60, SAI_runKeyItems)

/* the place's name in the names' texture: two columns of 11 */
#define SET_LABEL_NAME(map, place)                                        \
    (map)->label.u = ((place) + 6 >= 11) << 7;                            \
    (map)->label.v = ((place) + 6 >= 11 ? (place) - 5 : (place) + 6) * 21

void SAI_runWorldMap(void) {
    MapScreen map;

    SAI_openWorldMap(&map);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        D_801E469C = SAI_MAP_SPRITES;
        runCallbackSlots(&map, 3, 2);
    }
}

void SAI_openWorldMap(MapScreen *map) {
    SAI_MAP_LABEL_PLACE = SAI_MAP_CURSOR = SAI_MAP_PLACE = PLAYER_DATA(0).area + 1;
    if (DB(0).primSlots[16] == 0) {
        allocPrimDescPackets(0x40);
    }
    OPEN_MAP_MENU(map);
    map->label.x = -320;
    map->label.y = 186;
    SET_LABEL_NAME(map, SAI_MAP_PLACE);
    map->label.w = 0;
    map->label.h = 21;
    map->label.timer = 0;
    map->labelDir = 1;
    initScene3D(0);
    freeHeapBlocksByTag(0x7F);
    SAI_MAP_X = SAI_MAP_SPRITES[MAP_SPRITE_MAP].x0 = 320;
    SAI_MAP_Y = SAI_MAP_SPRITES[MAP_SPRITE_MAP].y0;
    SAI_MAP_PLAYER.pos.vx = SAI_MAP_PLACE_X[SAI_MAP_PLACE - 1] - 154;
    SAI_MAP_PLAYER.pos.vy = SAI_MAP_PLACE_Y[SAI_MAP_PLACE - 1] - 128;
    map->timer = 0;
    openKanjiPage(0xF, 0x1B9);
    playMusic(0, 0x1F, 0x7F);
    clearCallbackSlots(3, 2);
    CALLBACK_SLOTS[3] = (void (*)(void *))SAI_slideWorldMapIn;
    CALLBACK_SLOTS[4] = (void (*)(void *))SAI_drawWorldMap;
}

s32 SAI_slideWorldMapIn(MapScreen *map) {
    s16 *x = &SAI_MAP_SPRITES[MAP_SPRITE_MAP].x0;

    map->timer++;
    *x = 320 - map->timer * 224 / 25;
    SAI_MAP_X = SAI_MAP_SPRITES[MAP_SPRITE_MAP].x0;
    SAI_MAP_Y = SAI_MAP_SPRITES[MAP_SPRITE_MAP].y0;
    if (map->timer == 1) {
        playSoundEffect(11);
    }
    if (map->timer >= 25) {
        SAI_MAP_X = SAI_MAP_SPRITES[MAP_SPRITE_MAP].x0 = 96;
        SAI_MAP_Y = SAI_MAP_SPRITES[MAP_SPRITE_MAP].y0;
        map->timer = 0;
        SET_CALLBACK(SAI_tickWorldMapMenu);
    }
    return 0;
}

s32 SAI_slideWorldMapOut(MapScreen *map) {
    s16 *x = &SAI_MAP_SPRITES[MAP_SPRITE_MAP].x0;

    map->timer++;
    *x = map->timer * 224 / 25 + 96;
    SAI_MAP_X = SAI_MAP_SPRITES[MAP_SPRITE_MAP].x0;
    SAI_MAP_Y = SAI_MAP_SPRITES[MAP_SPRITE_MAP].y0;
    if (map->timer == 1) {
        playSoundEffect(12);
    }
    if (map->timer >= 25) {
        switch (map->menu.choice) {
        case 0:
            break;
        case 1:
            do {
                waitFrames(1);
            } while (SAI_STATE->unk43 != 2);
            setBackgroundScrollMode(1);
            freePrimDescPackets();
            break;
        case 2:
            setBackgroundScrollMode(3);
            break;
        case 3:
        case 4:
        case 5:
            setBackgroundScrollMode(2);
            break;
        }
        closeKanjiPage(0xF);
        startChoiceMenuAction(&map->menu);
    }
    return 0;
}

s32 SAI_tickWorldMapMenu(MapScreen *map) {
    if (map->menu.choice != 0 && map->menu.state == 6) {
        SCROLLING_BACKGROUND->unk1BE = 2;
    }
    if (runChoiceMenu(&map->menu) != 0) {
        switch (map->menu.choice) {
        case 0:
            SET_CALLBACK(SAI_tickMapCursor);
            break;
        case 4:
            OPTION_FROM_WORLD_MAP = 1;
        case 1:
        case 2:
        case 3:
        case 5:
        default:
            PLAYER_DATA(0).area = SAI_MAP_PLACE - 1;
            if (map->menu.choice == 1) {
                SAI_STATE->unk42 = PLAYER_DATA(0).area;
                startAreaPakLoad();
            }
            SET_CALLBACK(SAI_slideWorldMapOut);
            break;
        }
    }
    return 0;
}

s32 SAI_drawWorldMap(MapScreen *map) {
    s16 count;
    s16 i;

    SAI_MAP_PLAYER_DRAWN = SAI_MAP_PLAYER;
    SAI_MAP_PLAYER_DRAWN.pos.vx = SAI_MAP_X + SAI_MAP_PLAYER.pos.vx;
    SAI_MAP_PLAYER_DRAWN.pos.vy = SAI_MAP_Y + SAI_MAP_PLAYER.pos.vy;
    drawPrimDesc3D(&SAI_MAP_PLAYER_DRAWN, (PrimDesc3DWork *)0x1F800000);
    D_801E469C[MAP_SPRITE_PLAYER_SHADOW].x0 = SAI_MAP_PLAYER_DRAWN.pos.vx + 148;
    D_801E469C[MAP_SPRITE_PLAYER_SHADOW].y0 = SAI_MAP_PLAYER_DRAWN.pos.vy + 126;
    drawPrimDesc((PrimDesc *)&D_801E469C[MAP_SPRITE_PLAYER_SHADOW]);
    SAI_drawMapPaths();
    drawPrimDesc((PrimDesc *)&D_801E469C[MAP_SPRITE_MAP]);
    if (SCROLLING_BACKGROUND->unk1BE == 2) {
        map->labelDir = -1;
    }
    SAI_slideMapLabel(&map->label, map->labelDir);
    SAI_drawMapLabels(&map->label, 1);
    count = splitDigits(PLAYER_DATA(0).bits, (u8 *)D_801E46D8);
    if (count == 0) {
        SAI_MAP_SPRITES[MAP_SPRITE_DIGIT].u0 = 0x40;
        D_801E469C[MAP_SPRITE_DIGIT].x0 = SAI_MAP_SPRITES[MAP_SPRITE_LABEL_PLATE].x0 + 135;
        D_801E469C[MAP_SPRITE_DIGIT].y0 = SAI_MAP_SPRITES[MAP_SPRITE_LABEL_PLATE].y0 + 5;
        drawPrimDesc((PrimDesc *)&D_801E469C[MAP_SPRITE_DIGIT]);
        return 0;
    }
    for (i = 0; i < count; i++) {
        SAI_MAP_SPRITES[MAP_SPRITE_DIGIT].u0 = D_801E46D8[i] * 8 + 0x40;
        D_801E469C[MAP_SPRITE_DIGIT].x0 = SAI_MAP_SPRITES[MAP_SPRITE_LABEL_PLATE].x0 - (s16)(i * 8 - 135);
        D_801E469C[MAP_SPRITE_DIGIT].y0 = SAI_MAP_SPRITES[MAP_SPRITE_LABEL_PLATE].y0 + 5;
        drawPrimDesc((PrimDesc *)&D_801E469C[MAP_SPRITE_DIGIT]);
    }
    return 0;
}

void SAI_drawMapPaths(void) {
    u16 paths = PLAYER_DATA(0).unkF1E;
    SpriteDesc *line = &SAI_MAP_SPRITES[MAP_SPRITE_PATH];
    s16 i = 0;
    u16 places = PLAYER_DATA(0).unkF1C;
    u8 ends[0x18];
    s16 path;
    s16 dx;
    s16 dy;
    s16 x;
    s16 y;
    u8 n;

    for (; i < 15; i++) {
        if ((places >> i) & 1) {
            D_801E469C[MAP_SPRITE_PLACE].x0 = SAI_MAP_X + SAI_MAP_PLACE_X[i];
            D_801E469C[MAP_SPRITE_PLACE].y0 = SAI_MAP_Y + SAI_MAP_PLACE_Y[i];
            drawPrimDesc((PrimDesc *)&D_801E469C[MAP_SPRITE_PLACE]);
        }
    }
    for (path = 0; path < 15; path++) {
        n = 0;
        if ((paths >> path) & 1) {
            for (i = 0; i < 15; i++) {
                if ((SAI_MAP_PATH_ENDS[path] >> i) & 1) {
                    ends[n] = i;
                    n++;
                }
            }
            x = SAI_MAP_PLACE_X[ends[0]];
            dx = x - SAI_MAP_PLACE_X[ends[1]];
            if (dx < 0) {
                dx *= -1;
            }
            y = SAI_MAP_PLACE_Y[ends[0]];
            dy = y - SAI_MAP_PLACE_Y[ends[1]];
            if (dy < 0) {
                dy *= -1;
            }
            /* a line's width is one pixel across the way it runs */
            if (dx >= dy) {
                line[0].tpage = 1;
            } else {
                line[0].tpage = 0;
            }
            line[0].x0 = SAI_MAP_X + SAI_MAP_PLACE_X[ends[0]] + 5;
            line[0].y0 = SAI_MAP_Y + SAI_MAP_PLACE_Y[ends[0]] + 4;
            line[0].w = SAI_MAP_X + SAI_MAP_PLACE_X[ends[1]] + 5;
            line[0].h = SAI_MAP_Y + SAI_MAP_PLACE_Y[ends[1]] + 4;
            line[0].clut = SAI_MAP_PATH_CLUTS[path];
            drawPrimDesc((PrimDesc *)&line[0]);
            if (dx >= dy) {
                line[1].tpage = 1;
            } else {
                line[1].tpage = 0;
            }
            line[1].x0 = SAI_MAP_X + SAI_MAP_PLACE_X[ends[0]] + 4;
            line[1].y0 = SAI_MAP_Y + SAI_MAP_PLACE_Y[ends[0]] + 3;
            line[1].w = SAI_MAP_X + SAI_MAP_PLACE_X[ends[1]] + 4;
            line[1].h = SAI_MAP_Y + SAI_MAP_PLACE_Y[ends[1]] + 3;
            line[1].clut = SAI_MAP_PATH_CLUTS[path] + 2;
            drawPrimDesc((PrimDesc *)&line[1]);
        }
    }
}

void SAI_tickMapCursor(MapScreen *map) {
    u8 *neighbours;
    u16 places;

    places = PLAYER_DATA(0).unkF1C;
    neighbours = SAI_MAP_NEIGHBOURS[SAI_MAP_CURSOR - 1];
    if (PAD_STATES[0]->rawRepeat & PAD_RIGHT) {
        if ((places >> (neighbours[2] - 1)) & 1) {
            SAI_MAP_CURSOR = neighbours[2];
            SET_CALLBACK(SAI_switchMapLabel);
            playSoundEffect(2);
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_LEFT) {
        if ((places >> (neighbours[0] - 1)) & 1) {
            SAI_MAP_CURSOR = neighbours[0];
            SET_CALLBACK(SAI_switchMapLabel);
            playSoundEffect(2);
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_UP) {
        if ((places >> (neighbours[3] - 1)) & 1) {
            SAI_MAP_CURSOR = neighbours[3];
            SET_CALLBACK(SAI_switchMapLabel);
            playSoundEffect(2);
        }
    } else if (PAD_STATES[0]->rawRepeat & PAD_DOWN) {
        if ((places >> (neighbours[1] - 1)) & 1) {
            SAI_MAP_CURSOR = neighbours[1];
            SET_CALLBACK(SAI_switchMapLabel);
            playSoundEffect(2);
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
        playSoundEffect(0);
        if (SAI_MAP_CURSOR == SAI_MAP_PLACE) {
            OPEN_MAP_MENU(map);
            SET_CALLBACK(SAI_tickWorldMapMenu);
        } else if (SAI_findMapPath(map->path) != 0) {
            map->pathStep = 0;
            map->from = SAI_MAP_PLACE;
            map->to = map->path[0];
            SAI_startMapWalk(map, map->from, map->to);
            SET_CALLBACK(SAI_tickMapWalk);
        }
    } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
        SAI_MAP_CURSOR = SAI_MAP_PLACE;
        SET_LABEL_NAME(map, SAI_MAP_CURSOR);
        OPEN_MAP_MENU(map);
        SET_CALLBACK(SAI_tickWorldMapMenu);
        playSoundEffect(1);
    }
    D_801E469C[MAP_SPRITE_CURSOR].x0 = SAI_MAP_X + SAI_MAP_PLACE_X[SAI_MAP_CURSOR - 1] - 1;
    D_801E469C[MAP_SPRITE_CURSOR].y0 = SAI_MAP_Y + SAI_MAP_PLACE_Y[SAI_MAP_CURSOR - 1] - 1;
    drawPrimDesc((PrimDesc *)&D_801E469C[MAP_SPRITE_CURSOR]);
}

/* the label slides out, and back in with the name under the cursor */
s32 SAI_switchMapLabel(MapScreen *map) {
    map->labelDir = -1;
    if (map->label.timer < 27) {
        SAI_MAP_LABEL_PLACE = SAI_MAP_CURSOR;
        SET_LABEL_NAME(map, SAI_MAP_CURSOR);
        map->labelDir = 1;
        SET_CALLBACK(SAI_tickMapCursor);
    }
    D_801E469C[MAP_SPRITE_CURSOR].x0 = SAI_MAP_X + SAI_MAP_PLACE_X[SAI_MAP_CURSOR - 1] - 1;
    D_801E469C[MAP_SPRITE_CURSOR].y0 = SAI_MAP_Y + SAI_MAP_PLACE_Y[SAI_MAP_CURSOR - 1] - 1;
    drawPrimDesc((PrimDesc *)&D_801E469C[MAP_SPRITE_CURSOR]);
    return 0;
}

void SAI_startMapWalk(MapScreen *map, u8 from, u8 to) {
    MapWalk *walk;

    walk = &map->walk;
    walk->x = SAI_MAP_PLACE_X[from - 1];
    walk->y = SAI_MAP_PLACE_Y[from - 1];
    walk->error = 0;
    walk->steps = 0;
    walk->step = 0;
    /* the player faces the way it walks */
    if (SAI_MAP_PLACE_X[from - 1] < SAI_MAP_PLACE_X[to - 1]) {
        SAI_MAP_PLAYER.rot.vy = 0x800;
    } else {
        SAI_MAP_PLAYER.rot.vy = 0;
    }
    /* and its picture shows it from the back when it walks up */
    if (SAI_MAP_PLACE_Y[from - 1] < SAI_MAP_PLACE_Y[to - 1]) {
        SAI_MAP_PLAYER.v0 = 0xB0;
    } else {
        SAI_MAP_PLAYER.v0 = 0xD7;
    }
    map->walkFrame = 0;
}

s32 SAI_tickMapWalk(void *arg) {
    MapScreen *map;
    MapWalk *walk;

    map = arg;
    walk = &map->walk;
    SAI_stepMapWalk(walk, SAI_MAP_PLACE_X[map->from - 1], SAI_MAP_PLACE_Y[map->from - 1],
                    SAI_MAP_PLACE_X[map->to - 1], SAI_MAP_PLACE_Y[map->to - 1]);
    walk->step++;
    SAI_MAP_PLAYER.pos.vx = walk->x - 154;
    SAI_MAP_PLAYER.pos.vy = walk->y - 128;
    SAI_animateMapPlayer(map);
    if (!(walk->step & 0xF)) {
        playSoundEffect(10);
    }
    if (walk->step >= walk->steps) {
        map->from = map->to;
        SAI_MAP_PLAYER.pos.vx = SAI_MAP_PLACE_X[map->from - 1] - 154;
        SAI_MAP_PLAYER.pos.vy = SAI_MAP_PLACE_Y[map->from - 1] - 128;
        SAI_MAP_PLAYER.u0 = 0;
        map->pathStep++;
        map->to = map->path[map->pathStep];
        if (map->to == 0) {
            SAI_MAP_PLACE = map->from;
            SAI_MAP_CURSOR = SAI_MAP_PLACE;
            OPEN_MAP_MENU(map);
            SET_CALLBACK(SAI_tickWorldMapMenu);
        } else {
            SAI_startMapWalk(map, map->from, map->to);
        }
    }
    return 0;
}

void SAI_stepMapWalk(MapWalk *walk, s16 x0, s16 y0, s16 x1, s16 y1) {
    s16 dy;
    s16 dx;
    s16 sx;
    s16 sy;

    dy = y1 - y0;
    dx = x1 - x0;
    if (dy < 0) {
        dy = -dy;
        sy = -1;
    } else {
        sy = 1;
    }
    sx = 1;
    if (dx < 0) {
        dx = -dx;
        sx = -1;
    }
    if (dx > dy) {
        walk->steps = dx + 1;
        walk->x += sx;
        walk->error += dy;
        if (dx < walk->error) {
            walk->error -= dx;
            walk->y += sy;
        }
    } else {
        walk->steps = dy + 1;
        walk->y += sy;
        walk->error += dx;
        if (walk->error > 0) {
            walk->error -= dy;
            walk->x += sx;
        }
    }
}

/* 7 frames of 6 ticks, 26 pixels apart */
void SAI_animateMapPlayer(MapScreen *map) {
    SAI_MAP_PLAYER.u0 = ((s8)(map->walkFrame / 6) + 1) * 26;
    if (++map->walkFrame >= 42) {
        map->walkFrame = 0;
    }
}

s32 SAI_drawMapLabels(MapLabel *labels, s16 unused) {
    SpriteDesc *sprite;
    s32 count;
    s16 i;

    sprite = &SAI_MAP_SPRITES[MAP_SPRITE_LABEL];
    count = 1;
    for (i = 0; i < count; i++) {
        if (labels[i].timer < 26) {
            D_801E469C[MAP_SPRITE_LABEL_PLATE].x0 = labels[i].x + 13;
            D_801E469C[MAP_SPRITE_LABEL_PLATE].y0 = labels[i].y;
            drawPrimDesc((PrimDesc *)&D_801E469C[MAP_SPRITE_LABEL_PLATE]);
            D_801E469C[MAP_SPRITE_LABEL_ARROW].x0 = labels[i].x;
            D_801E469C[MAP_SPRITE_LABEL_ARROW].y0 = labels[i].y + 15;
            drawPrimDesc((PrimDesc *)&D_801E469C[MAP_SPRITE_LABEL_ARROW]);
        } else {
            D_801E469C[MAP_SPRITE_LABEL_PLATE].x0 = labels[i].w + labels[i].x - 6;
            D_801E469C[MAP_SPRITE_LABEL_PLATE].y0 = labels[i].y;
            drawPrimDesc((PrimDesc *)&D_801E469C[MAP_SPRITE_LABEL_PLATE]);
            sprite->x0 = labels[i].x;
            sprite->y0 = labels[i].y + 15;
            sprite->u0 = labels[i].u;
            sprite->v0 = labels[i].v;
            sprite->w = labels[i].w;
            sprite->h = labels[i].h;
            drawPrimDesc((PrimDesc *)&D_801E469C[MAP_SPRITE_LABEL]);
        }
    }
    return 0;
}

/* in 25 frames from the right edge, then the name opens in 12 */
s32 SAI_slideMapLabel(MapLabel *label, s8 dir) {
    if (dir == 1) {
        if (label->timer < 25) {
            label->timer++;
            label->x = 320 - label->timer * 191 / 25;
        } else if (label->timer < 37) {
            label->timer++;
            label->w = (label->timer - 25) * 127 / 12;
            label->x = 148 - label->w;
        }
    } else if (dir == -1) {
        if (label->timer >= 26) {
            label->timer--;
            label->w = (label->timer - 25) * 127 / 12;
            label->x = 148 - label->w;
            if (label->w == 0) {
                label->x = 129;
            }
        } else if (label->timer > 0) {
            label->timer--;
            label->x = 320 - label->timer * 191 / 25;
        }
    }
    return 0;
}

/* the shorter way from the player's place to the cursor's around the ring
   of 12 places, through the open paths: 0 if there is none */
s16 SAI_findMapPath(u8 *path) {
    u8 from = SAI_MAP_PLACE;
    u8 to = SAI_MAP_CURSOR;
    u16 paths = PLAYER_DATA(0).unkF1E;
    u8 ring[12] = { 1, 3, 4, 6, 7, 8, 9, 10, 11, 12, 14, 2 };
    u8 ways[2][16];
    s16 lengths[2];
    s16 start;
    s16 i;
    s16 j;
    s16 way;

    start = -1;
    for (i = 0; i < 16; i++) {
        path[i] = 0;
    }
    if (from == PLACE_OFF_1) {
        from = 1;
    }
    if (from == PLACE_OFF_14) {
        from = 14;
    }
    if (from == PLACE_OFF_2) {
        from = 2;
    }
    if (to == PLACE_OFF_1) {
        to = 1;
    }
    if (to == PLACE_OFF_14) {
        to = 14;
    }
    if (to == PLACE_OFF_2) {
        to = 2;
    }
    for (i = 0; i < 12; i++) {
        if (from == ring[i]) {
            start = i;
            break;
        }
    }
    /* one way round: path n joins ring[n] and ring[n + 1] */
    j = start;
    ways[0][0] = ring[j];
    for (i = 1; i < 12; i++) {
        if (to == ring[j]) {
            break;
        }
        if (!((paths >> j) & 1)) {
            i = 13;
            break;
        }
        if (++j >= 12) {
            j = 0;
        }
        ways[0][i] = ring[j];
    }
    lengths[0] = i;
    /* and the other */
    j = start;
    ways[1][0] = ring[j];
    for (i = 1; i < 12; i++) {
        if (to == ring[j]) {
            break;
        }
        if (--j < 0) {
            j = 11;
        }
        if (!((paths >> j) & 1)) {
            i = 13;
            break;
        }
        ways[1][i] = ring[j];
    }
    lengths[1] = i;
    if (lengths[0] < 13 && lengths[1] < 13) {
        way = lengths[1] < lengths[0];
    } else if (lengths[0] < 13) {
        way = 0;
    } else if (lengths[1] < 13) {
        way = 1;
    } else {
        return 0;
    }
    if (SAI_MAP_CURSOR == PLACE_OFF_1 || SAI_MAP_CURSOR == PLACE_OFF_14 || SAI_MAP_CURSOR == PLACE_OFF_2) {
        ways[way][lengths[way]] = SAI_MAP_CURSOR;
        lengths[way]++;
    }
    for (i = 0; i < lengths[way]; i++) {
        path[i] = ways[way][i];
    }
    return lengths[way];
}

void SAI_drawMapBits(JpWindow *win) {
    char line[0x88];
    u8 digits[0x10];

    formatSjisNumber(PLAYER_DATA(0).bits, 6, digits);
    {
        /* and the leftover byte after it */
        static const char format[20] = " 所持金 s0w-4%s d0\0\xC9";

        sprintf(line, format, digits);
    }
    drawIconText(0xAE, 0x38, 7, 1, win->z, (s32)line);
}

void SAI_tickMapBitsWindow(JpWindow *win) {
}
