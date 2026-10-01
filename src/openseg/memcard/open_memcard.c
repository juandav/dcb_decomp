#include "common.h"
#include "game.h"
#include "dcb/open_memcard.h"
#include "dcb/heap.h"
#include "dcb/card_db.h"
#include "dcb/text.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/scroll_bg.h"
#include "dcb/sound_play.h"
#include "dcb/prim.h"
#include "dcb/openseg.h"
#include "dcb/open_save.h"

/* eu writes "slot" in lower case and MEMORY CARD in capitals */
#if VERSION_US
#define OPEN_TEXT_PLAYER_SLOT "Player %d : Slot %d"
#define OPEN_TEXT_FORMAT_CARD "Format the Memory Card in Slot 1?"
#elif VERSION_EU
#define OPEN_TEXT_PLAYER_SLOT "Player %d : slot %d"
#define OPEN_TEXT_FORMAT_CARD "Format the MEMORY CARD in slot 1?"
#else
#error "openseg/memcard/open_memcard: version not checked"
#endif

typedef struct {
    UiWindow window;
    u8 unk44[4];
} Unk801F7C88;

typedef struct {
    u8 *color;
    s32 z;
    s16 x;
    s16 y;
    u8 slot;
} SlotDraw;

typedef struct {
    /* 0x00 */ Rect16 uv;
    /* 0x08 */ s32 duration;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 y;
} CardAnimFrame;

extern Unk801F7C88 OPEN_MEMCARD_SLOT_WINDOWS[3];
extern UiWindow OPEN_MEMCARD_INFO_WINDOW;
extern UiWindow OPEN_OPERATION_WINDOW;
extern s8 OPEN_MEMCARD_EMPTY[2][3];
extern u8 OPEN_MEMCARD_EXIT_ACTION;
extern UiWindow OPEN_MEMCARD_MESSAGE_WINDOW;
extern u8 OPEN_MEMCARD_CANCELLED;
extern u8 *OPEN_SAVE_PLACE_IMAGES;
extern u8 OPEN_MEMCARD_LOADING;
extern u8 OPEN_MEMCARD_CARD;

/* "Arena": the string is followed by two leftover bytes (E0 03) in the ROM, so it stays as data */
extern const char OPEN_STR_ARENA[];

extern s32 MEMORY_CARD_WAIT_COUNTER;
extern s8 OPEN_MEMCARD_SLOT;

void OPEN_openMemcardWindows(void);
void OPEN_initTransferArrow(POLY_FT4 *poly, s32 shade);
void OPEN_drawSaveDetails(s32 x, s32 y, s32 z);
void OPEN_loadMemcardTextures(void);
void OPEN_initMemcardScreen(s32 port);
void OPEN_resetMemcardScreen(s32 port);
void OPEN_drawMemcardScreen();
void OPEN_runMemcardPrompts();
void OPEN_drawSaveSummary(SlotDraw draw);
void OPEN_confirmOverwrite(s32 port);
void OPEN_confirmPlayWithoutSaving(s32 port);
void OPEN_confirmFormat(s32 pad);
void OPEN_selectSaveFile(s32 port);

/* the text color of a save slot, lit and dimmed */
u8 OPEN_SLOT_COLOR_LIT[3] = { 0x80, 0x80, 0x80 };
u8 OPEN_SLOT_COLOR_DIM[3] = { 0x20, 0x20, 0x20 };

/* the frames of the memory card animation */
CardAnimFrame OPEN_MEMCARD_ANIM_FRAMES[23] = {
    { { 0, 0x54, 0x20, 0x20 }, 5, 0, 0 },
    { { 0x20, 0x54, 0x20, 0x20 }, 6, 0, 0 },
    { { 0x40, 0x54, 0x20, 0x20 }, 6, 0, 0 },
    { { 0, 0x74, 0x20, 0x20 }, 5, 0, 0 },
    { { 0x20, 0x74, 0x20, 0x20 }, 6, 0, 0 },
    { { 0x40, 0x74, 0x20, 0x20 }, 6, 0, 0 },
    { { 0, 0x94, 0x20, 0x20 }, 0xA, 0, 0 },
    { { 0x20, 0x94, 0x20, 0x20 }, 7, 0, 0 },
    { { 0x40, 0x94, 0x20, 0x20 }, 0xA, 0, 0 },
    { { 0x40, 0x94, 0x20, 0x20 }, 0xA, 0, 0 },
    { { 0, 0xB4, 0x20, 0x20 }, 0xA, 0, 0 },
    { { 0x20, 0xB4, 0x20, 0x20 }, 2, 0, -1 },
    { { 0x20, 0xB4, 0x20, 0x20 }, 4, 0, -3 },
    { { 0x20, 0xB4, 0x20, 0x20 }, 6, 0, -5 },
    { { 0x20, 0xB4, 0x20, 0x20 }, 5, 0, -3 },
    { { 0x20, 0xB4, 0x20, 0x20 }, 3, 0, -1 },
    { { 0, 0xB4, 0x20, 0x20 }, 0xC, 0, 0 },
    { { 0, 0x94, 0x20, 0x20 }, 4, 0, 0 },
    { { 0x40, 0xD4, 0x20, 0x20 }, 8, 0, 0 },
    { { 0, 0xD4, 0x20, 0x20 }, 0xA, 0, 0 },
    { { 0x20, 0xD4, 0x20, 0x20 }, 0xD, 0, 0 },
    { { 0x40, 0xD4, 0x20, 0x20 }, 0xA, 0, 0 },
    { { 0, 0x94, 0x20, 0x20 }, 4, 0, 0 },
};

/* where a save was made */
char *OPEN_SAVE_PLACE_NAMES[16] = {
    "Beginner City",
    "Flame City",
    "Jungle City",
    "Igloo City",
    "Junk City",
    "Dark City",
    "Desert Island",
    "Pyramid City",
    "Sky City",
    "Steep Road",
    "Wiseman Tower",
    "Infinity Tower",
    "Mega Area",
    "Giga Area",
    "Giga Area",
    "Tera Area",
};

/* the entrances of the Areas */
char *OPEN_AREA_ENTRANCE_NAMES[4] = {
    "To Giga Area Entrance",
    "To Mega Area Entrance",
    "To Tera Area Entrance",
    "To Giga Area Entrance",
};

/* the arenas of each place, four per place */
char *OPEN_ARENA_NAMES[48] = {
    "Beginner Arena",
    "",
    "",
    "",
    "Flame Arena",
    "Extra Arena",
    "",
    "",
    "Jungle Arena",
    "Extra Arena",
    "Beet Arena",
    "",
    "Igloo Arena",
    "Extra Arena",
    "",
    "",
    "Junk Arena",
    "",
    "",
    "",
    "Dark Arena",
    "Extra Arena",
    "",
    "Haunted Arena",
    "Desert Arena",
    "",
    "",
    "",
    "Pyramid Arena",
    "Extra Arena",
    "",
    "",
    "Sky Arena",
    "Extra Arena",
    "",
    "",
    "Steep Arena",
    "",
    "",
    "",
    "Wiseman Arena",
    "",
    "",
    "",
    "Infinity Arena",
    "",
    "",
    "",
};

/* the memory card messages */
char *OPEN_MEMCARD_MESSAGES[26] = {
    "*s0Checking MEMORY CARD in\nMEMORY CARD slot *S. Do not insert\nor remove MEMORY CARD or Controller.",
    "*s0MEMORY CARD in MEMORY CARD slot *E\ncontains no Digimon\nDigital Card Battle data.",
    "*s0Checking MEMORY CARD in\nMEMORY CARD slot *E. Do not insert\nor remove MEMORY CARD or Controller.",
    "*s0MEMORY CARD in MEMORY CARD slot *E\nis not formatted.",
    "*s0There is no MEMORY CARD in\nMEMORY CARD slot *S. If you start now,\nyou won't be able to save. Is this OK?",
    "*s0Failed to create new save data in\nMEMORY CARD slot *S. Data may be\ncorrupted. Try saving again.",
    "*s0Return to title?",
    "*s0Formatting MEMORY CARD in\nMEMORY CARD slot *S... Do not insert\nor remove MEMORY CARD or Controller.",
    "*s0Do you want to stop Data Conversion?",
    "*s0Not enough free blocks. At least\n2 free blocks are required to save.",
    "*s0You cannot save your game if you\nbegin the game without creating\na save data here. Is this OK?",
    "*s0Updating save data on MEMORY CARD in\nMEMORY CARD slot *S... Do not insert\nor remove MEMORY CARD or Controller.",
    "*s0There is no MEMORY CARD\nin MEMORY CARD slot *E.",
    "*s0MEMORY CARD in MEMORY CARD slot *E\ndoes not contain game data\ncurrently in play.",
    "*s0Creating new data in MEMORY CARD\nslot *S... Do not insert or remove\nMEMORY CARD or Controller.",
    "*s0Failed to update data in MEMORY CARD\nslot *S. Data may be corrupted.",
    "*s0Load which save data?",
    "*s0Save to which file?",
    "*s0Loading data from MEMORY CARD slot *S.\nDo not insert or remove\nMEMORY CARD or Controller.",
    "*s0Failed to load data from MEMORY CARD\nslot *S. Data may be corrupted.",
    "*s0Finished loading data from\nMEMORY CARD slot *S.",
    "*s0Finished creating data on MEMORY CARD\nin MEMORY CARD slot *S.",
    "*s0Finished updating data on MEMORY CARD\nin MEMORY CARD slot *S.",
    "*s0MEMORY CARD in MEMORY CARD slot 1\ncontains no Digimon\nDigital Card Battle game data.",
    "*s0You cannot save your game if you\nbegin the game as is. Is this OK?",
    "*s0Stop saving?",
};

void OPEN_runMemcardScreen(s32 mode, s32 parentTask, s32 port) {
    OPEN_MEMCARD_MODE = mode;
    OPEN_SAVE_PLACE_IMAGES = NULL;
    OPEN_loadMemcardTextures();
    OPEN_initMemcardScreen(port);
    addFrameCallback((s32)OPEN_drawMemcardScreen);
    do {
        OPEN_resetMemcardScreen(port);
        spawnTask(0, -1, 4, 0x800, OPEN_runMemcardAccess, 0, 0, 0, 0);
        spawnTask(0, -1, 0, 0x800, OPEN_runMemcardPrompts, 0, 0, 0, 0);
        while (OPEN_MEMCARD.ready != 1) {
            waitFrames(FRAME_INTERVAL);
        }
        if (OPEN_MEMCARD.cancelled != 0) {
            break;
        }
        refreshPartners(port);
        linkSavedDecks(port);
        port++;
    } while (OPEN_MEMCARD.again != 0);
    animateWindowTo(&OPEN_MEMCARD_MESSAGE_WINDOW, (Rect16 *)-1);
    playMenuSound(4);
    waitFrames(20);
    removeFrameCallback((s32)OPEN_drawMemcardScreen);
    switch (OPEN_MEMCARD_EXIT_ACTION) {
    case 0:
        break;
    case 1:
        initDialog((u8 *)&OPEN_DIALOG, "Quit the game?", 1);
        runDialog(&OPEN_DIALOG);
        if (OPEN_DIALOG.choice == 1) {
            stopMusic();
            fadeOutScrollingBackground();
            waitFrames(20);
            resumeTask(0);
            exitTask();
        }
        break;
    }
    freeHeapBlock(OPEN_SAVE_PLACE_IMAGES);
    waitFrames(20);
    freeHeapBlocksByTag(0x63);
    resumeTask(parentTask);
}

s32 OPEN_loadFriendSaves(void) {
    s32 task;

    task = getCurrentTaskId();
    spawnTask(0, -1, 0, 0x600, OPEN_runMemcardScreen, 7, task, 0, 0);
    waitFrames(0x7FFFFFFF);
    if (OPEN_MEMCARD.cancelled == 0) {
        spawnTask(0, -1, 0, 0x600, OPEN_runMemcardScreen, 7, task, 1, 0);
        waitFrames(0x7FFFFFFF);
        if (OPEN_MEMCARD.cancelled == 0) {
            return 0;
        }
    }
    return 1;
}

s32 OPEN_saveFriendGame(void) {
    spawnTask(0, -1, 0, 0x600, OPEN_runMemcardScreen, 6, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
    return OPEN_MEMCARD_CANCELLED != 0;
}

void OPEN_createNewSave(void) {
    spawnTask(0, -1, 0, 0x600, OPEN_runMemcardScreen, 0, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
}

void OPEN_initMemcardScreen(s32 port) {
    OPEN_MEMCARD.animPhase = 0;
    OPEN_MEMCARD.message = -1;
    OPEN_MEMCARD.animTime = 0;
    OPEN_MEMCARD.buffer = allocHeapBlock(0x4000, 0x63);
}

/* libgpu's setRECT */
#define setRECT(r, _x, _y, _w, _h) (r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h)

void OPEN_resetMemcardScreen(s32 port) {
    Rect16 rect;
    s32 i;
    s32 j;

    if (OPEN_MEMCARD.mode == 6) {
        OPEN_MEMCARD.progress = port * 50;
    } else {
        OPEN_MEMCARD_PROGRESS = 0;
    }
    OPEN_MEMCARD.previewsRead = 0;
    OPEN_MEMCARD.cancelled = 1;
    OPEN_MEMCARD.ready = 0;
    OPEN_MEMCARD.card = port;
    OPEN_MEMCARD.slot = 0;
    OPEN_MEMCARD.again = 0;
    switch (OPEN_MEMCARD.mode) {
    case 0:
        countSeenCards(port);
        OPEN_MEMCARD.loading = 0;
        ((SessionView *)SESSION_DATA)->saves[port].slot = 0;
        ((SessionView *)SESSION_DATA)->saves[port].file = 0;
        OPEN_MEMCARD.state = 1;
        OPEN_MEMCARD.exitAction = 0;
        break;
    case 2:
    case 4:
    case 5:
        countSeenCards(port);
        OPEN_MEMCARD.loading = 0;
        OPEN_MEMCARD.state = 0x11;
        OPEN_MEMCARD.exitAction = 1;
        break;
    case 8:
        countSeenCards(port);
        OPEN_MEMCARD.loading = 0;
        OPEN_MEMCARD.state = 0x11;
        OPEN_MEMCARD.exitAction = 2;
        PLAYER_DATA(0).areaId = 0;
        PLAYER_DATA(0).resumeInArea = 1;
        break;
    case 7:
        OPEN_MEMCARD.loading = 1;
        ((SessionView *)SESSION_DATA)->saves[port].slot = port;
        ((SessionView *)SESSION_DATA)->saves[port].file = 0;
        OPEN_MEMCARD.state = 1;
        OPEN_MEMCARD.exitAction = 0;
        break;
    case 6:
        countSeenCards(port);
        OPEN_MEMCARD.loading = 0;
        OPEN_MEMCARD.state = 0x11;
        if (port == 0) {
            OPEN_MEMCARD.again = 1;
        }
        OPEN_MEMCARD_EXIT_ACTION = 0;
        break;
    case 0xFF:
        OPEN_MEMCARD.loading = 1;
        ((SessionView *)SESSION_DATA)->saves[port].slot = 0;
        ((SessionView *)SESSION_DATA)->saves[port].file = 0;
        OPEN_MEMCARD.state = 1;
        OPEN_MEMCARD.exitAction = 0;
        break;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            OPEN_MEMCARD.empty[i][j] = 1;
        }
    }
    for (i = 0; i < 2; i++) {
        OPEN_initTransferArrow(&OPEN_MEMCARD.arrows[i], 0x80);
        OPEN_initTransferArrow(&OPEN_MEMCARD.arrowShadows[i], 0x40);
    }
    OPEN_openMemcardWindows();
    OPEN_MEMCARD.sprites[1].x = 0x37;
    OPEN_MEMCARD.sprites[1].y = 0x70;
    OPEN_MEMCARD.sprites[0].x = 0x37;
    OPEN_MEMCARD.sprites[0].y = 0x56;
    OPEN_MEMCARD.sprites[2].x = 0x38;
    OPEN_MEMCARD.sprites[2].y = 0x76;
    OPEN_MEMCARD.sprites[3].x = 0xFC;
    OPEN_MEMCARD.sprites[3].y = 0x76;
    OPEN_MEMCARD.sprites[4].x = 0x77;
    OPEN_MEMCARD.sprites[4].y = 0x85;
    OPEN_MEMCARD.sprites[5].x = 0x32;
    OPEN_MEMCARD.sprites[5].y = 0x4C;
    setRECT(&OPEN_MEMCARD.sprites[0].uv, 0, 0x54, 0x20, 0x20);
    setRECT(&OPEN_MEMCARD.sprites[1].uv, 0x40, 4, 0x20, 8);
    setRECT(&OPEN_MEMCARD.sprites[2].uv, 0, 0x48, 0xC, 0xC);
    setRECT(&OPEN_MEMCARD.sprites[3].uv, 0xC, 0x48, 0xC, 0xC);
    setRECT(&OPEN_MEMCARD.sprites[4].uv, OPEN_MEMCARD.loading * 88 + 0x18, 0x48, 0x58, 8);
    setRECT(&OPEN_MEMCARD.sprites[5].uv, 0, 0, 0xDC, 0x48);
    rect.x = 0xC;
    rect.y = 0xBC;
    rect.w = 0x128;
    rect.h = 0x2A;
    openWindow(&OPEN_MEMCARD_MESSAGE_WINDOW, &rect, -1, (s16 *)-1, 8, 0x51, 0x80, 0xC);
    OPEN_MEMCARD_MESSAGE_WINDOW.labelPalette = 8;
    OPEN_MEMCARD_MESSAGE_WINDOW.label = (s32)"MESSAGE";
    playMenuSound(3);
}

void OPEN_drawMemcardMessage(UiWindow *window) {
    char text[136];
    char *src;
    char *dst;
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    if (OPEN_MEMCARD_MESSAGE >= 0) {
        src = OPEN_MEMCARD_MESSAGES[OPEN_MEMCARD_MESSAGE];
        dst = text;
        while (*src != 0) {
            if (*src == '*') {
                src++;
                switch (*src) {
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'g':
                case 's':
                    *dst++ = '*';
                    *dst++ = *src++;
                    break;
                case 'h':
                case 'w':
                    *dst++ = '*';
                    *dst++ = *src++;
                    if (*src == '-') {
                        *dst++ = *src++;
                    }
                    break;
                case 'S':
                    src++;
                    *dst++ = OPEN_MEMCARD.card + '1';
                    continue;
                case 'E':
                    src++;
                    *dst++ = OPEN_MEMCARD.messagePort + '1';
                    continue;
                /* '\n' and a code above 'w' (which one is a guess) had cases of their
                   own with the default's code: they shape the switch's compare tree */
                case '\n':
                    *dst++ = '*';
                    break;
                case 'x':
                    *dst++ = '*';
                    break;
                default:
                    *dst++ = '*';
                    break;
                }
            }
            *dst++ = *src++;
        }
        *dst = 0;
        drawText(x + 2, y + 1, (s32)text, 7, z);
    }
}

void OPEN_loadMemcardTextures(void) {
    u32 *pack;

    spawnTask(0, -1, 0, 0x800, loadFile, "C:\\OBJECT\\saveload.TIS", getCurrentTaskId(), 0, 0);
    pack = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    switch (OPEN_MEMCARD_MODE) {
    case 0:
    case 7:
    case 0xFF:
        spawnTask(0, -1, 0, 0x800, loadFile, "B:\\SAVE.ARC", getCurrentTaskId());
        OPEN_SAVE_PLACE_IMAGES = (u8 *)waitFrames(0x7FFFFFFF);
        break;
    }
}

void OPEN_initTransferArrow(POLY_FT4 *poly, s32 shade) {
    s32 u0;
    s32 u1;

    SetPolyFT4(poly);
    poly->r0 = shade;
    poly->g0 = shade;
    poly->b0 = shade;
    SetSemiTrans(poly, 0);
    poly->clut = 0x7F57;
    poly->tpage = 0x15;
    if (OPEN_MEMCARD_LOADING == 0) {
        poly->u0 = u0 = 0xE2;
        poly->v0 = 0;
        u1 = 0xE6;
    } else {
        poly->u0 = u0 = 0xEA;
        poly->v0 = 0;
        u1 = 0xEE;
    }
    poly->u1 = u1;
    poly->v1 = 0;
    poly->u2 = u0;
    poly->v2 = 8;
    poly->u3 = u1;
    poly->v3 = 8;
    poly->x0 = 0x46;
    poly->y0 = 0x78;
    poly->x1 = 0xF7;
    poly->y1 = 0x78;
    poly->x2 = 0x46;
    poly->y2 = 0x80;
    poly->x3 = 0xF7;
    poly->y3 = 0x80;
}

void OPEN_openMemcardWindows(void) {
    Rect16 rect;
    s32 i;

    for (i = 0; i < 3; i++) {
        OPEN_MEMCARD.slotWindows[i].slot = i;
        OPEN_MEMCARD.slotWindows[i].message = 0x1D;
        rect.x = 16;
        rect.y = i * 42 + 0x3E;
        rect.w = 0x90;
        rect.h = 0x1A;
        openWindow(&OPEN_MEMCARD.slotWindows[i].window, &rect, -1, (s16 *)-1, 8, 0x21, 0x80, 0xC);
        switch (i) {
        case 0:
            OPEN_MEMCARD.slotWindows[i].window.label = (s32)"FILE 1";
            break;
        case 1:
            OPEN_MEMCARD.slotWindows[i].window.label = (s32)"FILE 2";
            break;
        case 2:
            OPEN_MEMCARD.slotWindows[i].window.label = (s32)"FILE 3";
            break;
        }
        animateWindowTo(&OPEN_MEMCARD.slotWindows[i].window, (Rect16 *)-1);
    }
    rect.x = 0xAC;
    rect.y = 0x5E;
    rect.w = 0x84;
    rect.h = 0x4E;
    openWindow(&OPEN_MEMCARD.infoWindow.window, &rect, -1, (s16 *)-1, 8, 0x21, 0x80, 0xC);
    OPEN_MEMCARD.infoWindow.window.label = (s32)"INFO.";
    animateWindowTo(&OPEN_MEMCARD.infoWindow.window, (Rect16 *)-1);
    rect.x = 0xAC;
    rect.y = 0x3E;
    rect.w = 0x84;
    rect.h = 0xE;
    openWindow(&OPEN_OPERATION_WINDOW, &rect, -1, (s16 *)-1, 8, 0x21, 0x80, 0xC);
    OPEN_OPERATION_WINDOW.label = (s32)"OPERATION";
    animateWindowTo(&OPEN_OPERATION_WINDOW, (Rect16 *)-1);
}

void OPEN_uploadSavePlaceImage(s32 slot) {
    SaveSlot *save;

    if (OPEN_SAVE_PLACE_IMAGES != NULL && OPEN_MEMCARD.empty[OPEN_MEMCARD.card][slot] == 0) {
        save = &OPEN_MEMCARD.slots[OPEN_MEMCARD.card][slot];
        if (save->location < 16) {
            uploadTim((u32 *)(OPEN_SAVE_PLACE_IMAGES + ((s32 *)OPEN_SAVE_PLACE_IMAGES)[save->location + 16]), 0x180, 0x14E, 0x180, 0x1FD);
        }
    }
}

void OPEN_showSaveSlots(void) {
    Rect16 rect;
    SaveSlot *save;
    s32 i;

    for (i = 0; i < 3; i++) {
        if (OPEN_SAVE_PLACE_IMAGES != NULL && OPEN_MEMCARD.empty[OPEN_MEMCARD.card][i] == 0) {
            save = &OPEN_MEMCARD.slots[OPEN_MEMCARD.card][i];
            if (save->location < 16) {
                uploadTim((u32 *)(OPEN_SAVE_PLACE_IMAGES + ((s32 *)OPEN_SAVE_PLACE_IMAGES)[save->location]), 0x180, i * 26 + 0x100, 0x180, i + 0x1FA);
            }
        }
        rect.x = 16;
        rect.y = i * 42 + 0x3E;
        rect.w = 0x90;
        rect.h = 0x1A;
        animateWindowTo(&OPEN_MEMCARD.slotWindows[i].window, &rect);
    }
    OPEN_uploadSavePlaceImage(OPEN_MEMCARD.slot);
    rect.x = 0xAC;
    rect.y = 0x5E;
    rect.w = 0x84;
    rect.h = 0x4E;
    animateWindowTo(&OPEN_MEMCARD.infoWindow.window, &rect);
    rect.x = 0xAC;
    rect.y = 0x3E;
    rect.w = 0x84;
    rect.h = 0xE;
    animateWindowTo(&OPEN_OPERATION_WINDOW, &rect);
    playMenuSound(3);
}

void OPEN_hideSaveSlots(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        animateWindowTo(&OPEN_MEMCARD_SLOT_WINDOWS[i].window, (Rect16 *)-1);
    }
    animateWindowTo(&OPEN_MEMCARD_INFO_WINDOW, (Rect16 *)-1);
    animateWindowTo(&OPEN_OPERATION_WINDOW, (Rect16 *)-1);
    playMenuSound(4);
}

void OPEN_drawSaveSlot(SlotWindow *window) {
    SlotDraw draw;
    SaveSlot *save;

    if (window->window.brightness == 0x80) {
        draw.color = OPEN_SLOT_COLOR_LIT;
    } else {
        draw.color = OPEN_SLOT_COLOR_DIM;
    }
    draw.x = window->window.originX;
    draw.y = window->window.originY;
    draw.z = window->window.z;
    draw.slot = window->slot;
    if (draw.slot >= 3) {
        draw.slot = 0;
    }
    if (OPEN_MEMCARD.empty[OPEN_MEMCARD.card][draw.slot] == 0) {
        save = &OPEN_MEMCARD.slots[OPEN_MEMCARD.card][draw.slot];
        if (save->size == 0x2774) {
            OPEN_drawSaveSummary(draw);
        } else {
            drawTextColored(draw.x + 0xC, draw.y + 7, "File is corrupted!", draw.color, 7, draw.z);
        }
    } else {
        drawTextColored(draw.x + 0x30, draw.y + 7, "NO DATA", draw.color, 7, draw.z);
    }
}

void OPEN_drawSaveInfo(UiWindow *window) {
    SaveSlot *save;

    if (OPEN_MEMCARD.empty[OPEN_MEMCARD.card][OPEN_MEMCARD.slot] == 0) {
        save = &OPEN_MEMCARD.slots[OPEN_MEMCARD.card][OPEN_MEMCARD.slot];
        if (save->size == 0x2774) {
            OPEN_drawSaveDetails(window->originX, window->originY, window->z);
        } else {
            drawText(window->originX + 0x2C, window->originY + 0x20, (s32)"NO DATA", 7, window->z);
        }
    } else {
        drawText(window->originX + 0x2C, window->originY + 0x20, (s32)"NO DATA", 7, window->z);
    }
}

void OPEN_drawMemcardOperation(UiWindow *window) {
    char text[40];
    s32 x;

    sprintf(text, OPEN_TEXT_PLAYER_SLOT, OPEN_MEMCARD_CARD + 1, ((SessionData *)SESSION_DATA)->saveSlots[OPEN_MEMCARD_CARD][0] + 1);
    x = (0x84 - strlen(text) * 6) / 2;
    drawText(window->originX + x, window->originY + 1, (s32)text, 7, window->z);
}

void OPEN_drawMemcardScreen(void) {
    s8 firstFrames[7] = { 0, 6, 9, 17, 19, 21, 23 };
    char text[24];
    MenuSprite *sprite;
    POLY_FT4 *arrow;
    POLY_FT4 *shadow;
    s32 time;
    s32 i;
    s8 frame;

    sprite = OPEN_MEMCARD.sprites;
    arrow = &OPEN_MEMCARD.arrows[FRAME_BUFFER_INDEX];
    shadow = &OPEN_MEMCARD.arrowShadows[FRAME_BUFFER_INDEX];
    drawWindow(&OPEN_MEMCARD_MESSAGE_WINDOW, OPEN_drawMemcardMessage, 1);
    for (i = 0; i < 3; i++) {
        drawWindow(&OPEN_MEMCARD.slotWindows[i].window, OPEN_drawSaveSlot, OPEN_MEMCARD.slotWindows[i].message);
    }
    drawWindow(&OPEN_MEMCARD.infoWindow.window, OPEN_drawSaveInfo, 1);
    drawWindow(&OPEN_OPERATION_WINDOW, OPEN_drawMemcardOperation, 1);
    frame = -1;
    if (OPEN_MEMCARD.progress != 0) {
        OPEN_MEMCARD.animTime++;
        do {
            time = OPEN_MEMCARD.animTime;
            for (i = firstFrames[OPEN_MEMCARD.animPhase]; i < firstFrames[OPEN_MEMCARD.animPhase + 1]; i++) {
                if (time < OPEN_MEMCARD_ANIM_FRAMES[i].duration) {
                    frame = i;
                    break;
                }
                time -= OPEN_MEMCARD_ANIM_FRAMES[i].duration;
            }
            if (frame == -1) {
                OPEN_MEMCARD.animTime = time;
                if (OPEN_MEMCARD.animPhase == 1) {
                    OPEN_MEMCARD.animPhase = 2;
                }
                if (OPEN_MEMCARD.animPhase == 3) {
                    OPEN_MEMCARD.animPhase = 4;
                }
                if (OPEN_MEMCARD.animPhase == 5) {
                    OPEN_MEMCARD.animPhase = 0;
                }
            }
        } while (frame == -1);
        sprite[0].uv = OPEN_MEMCARD_ANIM_FRAMES[frame].uv;
        sprite[0].x = ((100 - OPEN_MEMCARD_PROGRESS) * 55 + OPEN_MEMCARD_PROGRESS * 232) / 100;
        sprite[0].y = OPEN_MEMCARD_ANIM_FRAMES[frame].y + 0x56;
        sprite[1].x = sprite[0].x;
        if (frame >= 11 && frame < 16) {
            sprite[1].uv.y = 0xBC;
        } else {
            sprite[1].uv.y = 0xB4;
        }
        arrow->x1 = arrow->x3 = sprite[0].x + 15;
        addPrim(&CURRENT_FRAME_BUFFER->ot[31], arrow);
        addPrim(&CURRENT_FRAME_BUFFER->ot[31], shadow);
        for (i = 0; i < 6; i++, sprite++) {
            if (i != 4 || (OPEN_MEMCARD.progress >= 1 && OPEN_MEMCARD.progress < 100)) {
                if (i == 0) {
                    drawTexturedSprite(sprite->x, sprite->y, &sprite->uv, 0x15, 0x7F97, 0x1E, 0x80, -1);
                } else {
                    drawTexturedSprite(sprite->x, sprite->y, &sprite->uv, 0x15, 0x7FD7, 0x1E, 0x80, -1);
                }
            }
        }
        sprintf(text, "*s0%3d*w3%%", OPEN_MEMCARD_PROGRESS);
        drawText(0xE6, 0x82, (s32)text, 6, 0x1D);
    }
}

void OPEN_drawSaveSummary(SlotDraw draw) {
    char text[24];
    SaveSlot *save;
    s32 time;
    s32 hour;
    s32 minute;
    s32 second;

    save = &OPEN_MEMCARD.slots[OPEN_MEMCARD.card][draw.slot];
    drawTextColored(draw.x + 1, draw.y + 1, "Name", draw.color, 6, draw.z);
    drawTextColored(draw.x + 0x43, draw.y + 1, save->name, draw.color, 7, draw.z);
    time = save->playTime;
    hour = time / 216000;
    minute = (time - hour * 216000) / 3600;
    second = (time - hour * 216000 - minute * 3600) / 60;
    if (hour >= 1000) {
        hour = 999;
        minute = 59;
        second = 59;
    }
    drawTextColored(draw.x + 1, draw.y + 13, "Playing Time", draw.color, 6, draw.z);
    sprintf(text, "%3d:%2.2d:%2.2d", hour, minute, second);
    drawTextColored(draw.x + 0x43, draw.y + 13, text, draw.color, 7, draw.z);
    if (save->location < 16) {
        CUR_SPRT->sp.x0 = draw.x;
        CUR_SPRT->sp.y0 = draw.y;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = draw.slot * 26;
        CUR_SPRT->sp.clut = getClut(0x180, draw.slot + 0x1FA);
        CUR_SPRT->sp.w = 0x90;
        CUR_SPRT->sp.h = 0x1A;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = draw.color[0];
        CUR_SPRT->sp.g0 = draw.color[1];
        CUR_SPRT->sp.b0 = draw.color[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x96);
        addPrim(&CURRENT_FRAME_BUFFER->ot[draw.z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[draw.z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void OPEN_drawSaveDetails(s32 x, s32 y, s32 z) {
    char text[24];
    SaveSlot *save;
    s32 completion;
    s32 collection;
    s32 slot;

    slot = OPEN_MEMCARD.slot;
    save = &OPEN_MEMCARD.slots[OPEN_MEMCARD.card][slot];
    completion = save->progress * 1000 / 166;
    collection = save->seenCardCount * 1000 / 301;
    drawText(x + 2, y + 1, (s32)"Game Completion", 6, z);
    sprintf(text, "%3d.%1d*w3*c6%%", completion / 10, completion % 10);
    drawText(x + 0x5B, y + 1, (s32)text, 7, z);
    drawText(x + 2, y + 14, (s32)"Card Collection", 6, z);
    sprintf(text, "%3d.%1d*w3*c6%%", collection / 10, collection % 10);
    drawText(x + 0x5B, y + 14, (s32)text, 7, z);
    if (save->tradeUnlocked) {
        drawIcon(x + 0x68, y + 0x1A, 2, 11, z);
    }
    if (save->location < 16) {
        drawText(x + 2, y + 0x1B, (s32)"Current Position", 6, z);
        drawText(x + 13, y + 0x28, (s32)OPEN_SAVE_PLACE_NAMES[save->location], 7, z);
        if (save->location >= 12) {
            drawText(x + 13, y + 0x35, (s32)OPEN_AREA_ENTRANCE_NAMES[save->location - 12], 7, z);
        } else if (save->arena >= 2 && save->arena < 6) {
            drawText(x + 2, y + 0x35, (s32)OPEN_STR_ARENA, 6, z);
            drawText(x + 13, y + 0x42, (s32)OPEN_ARENA_NAMES[save->location * 4 + save->arena - 2], 7, z);
        }
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0x4E;
        CUR_SPRT->sp.clut = 0x7F58;
        CUR_SPRT->sp.w = 0x84;
        CUR_SPRT->sp.h = 0x4E;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x96);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/* the last two bytes are leftovers in the original, not zero padding, and
   not the same in every version */
#if VERSION_US
const char OPEN_STR_ARENA[8] = "Arena\0\xE0\x03";
#elif VERSION_EU
const char OPEN_STR_ARENA[8] = "Arena\0\0\x03";
#else
#error "openseg/memcard/open_memcard: version not checked"
#endif

void OPEN_runMemcardPrompts(void) {
    s32 port;

    do {
        waitFrames(FRAME_INTERVAL);
        MEMORY_CARD_WAIT_COUNTER++;
        port = OPEN_MEMCARD.card;
        switch (OPEN_MEMCARD.state) {
        case 17:
            if (OPEN_MEMCARD.mode == 6) {
                OPEN_MEMCARD.state = 0x17;
            } else {
                initDialog((u8 *)&OPEN_DIALOG, "Save the game up to this point?", 1);
                OPEN_DIALOG.choice = 1;
                runDialogForPad((s32 *)&OPEN_DIALOG, port);
                switch (OPEN_DIALOG.choice) {
                case 1:
                    OPEN_MEMCARD.state = 0x17;
                    break;
                case 0:
                case 2:
                    OPEN_MEMCARD.state = 0x1B;
                    break;
                }
            }
            break;
        case 24:
            if (OPEN_MEMCARD.mode != 6) {
                OPEN_confirmOverwrite(port);
            } else {
                OPEN_MEMCARD.state = 0x12;
                if (port != 0) {
                    OPEN_MEMCARD.animPhase = 5;
                }
            }
            break;
        case 10:
            OPEN_MEMCARD.message = 0x10;
            OPEN_selectSaveFile(port);
            break;
        case 3:
            OPEN_MEMCARD.message = 0x11;
            OPEN_selectSaveFile(port);
            break;
        case 16:
            OPEN_MEMCARD.message = 6;
            initDialog((u8 *)&OPEN_DIALOG, NULL, 1);
            runDialogForPad((s32 *)&OPEN_DIALOG, port);
            switch (OPEN_DIALOG.choice) {
            case 1:
                OPEN_MEMCARD.state = 0x1B;
                break;
            case 0:
            case 2:
                OPEN_MEMCARD.state = 1;
                break;
            }
            break;
        case 22:
            OPEN_MEMCARD.message = 0x19;
            initDialog((u8 *)&OPEN_DIALOG, NULL, 1);
            runDialogForPad((s32 *)&OPEN_DIALOG, port);
            switch (OPEN_DIALOG.choice) {
            case 1:
                OPEN_MEMCARD.state = 0x1B;
                break;
            case 0:
            case 2:
                OPEN_MEMCARD.state = 0x11;
                break;
            }
            break;
        case 29:
            if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                if (OPEN_MEMCARD.mode == 0) {
                    OPEN_MEMCARD.message = 4;
                    OPEN_MEMCARD.state = 2;
                } else {
                    OPEN_MEMCARD.state = 0x10;
                }
            } else if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD.state = 1;
            }
            break;
        case 19:
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD.state = 0x11;
            } else if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                OPEN_MEMCARD.state = 0x16;
            }
            break;
        case 28:
            OPEN_MEMCARD.message = 1;
            if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                OPEN_MEMCARD.state = 0x10;
            } else if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD.state = 1;
            }
            break;
        case 21:
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD.state = 0x17;
            }
            break;
        case 11:
            if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                OPEN_MEMCARD.state = 0x10;
            } else if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD.state = 1;
            }
            break;
        case 14:
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD.state = 1;
            }
            break;
        case 7:
            OPEN_MEMCARD.message = 3;
            OPEN_confirmFormat(port);
            break;
        case 2:
            OPEN_confirmPlayWithoutSaving(port);
            break;
        case 5:
            OPEN_MEMCARD.progress = 0;
            OPEN_MEMCARD.message = 5;
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD.state = 1;
            }
            break;
        case 6:
            OPEN_MEMCARD.message = 9;
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD.state = 1;
            } else if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                OPEN_MEMCARD.message = 0x18;
                OPEN_MEMCARD.state = 2;
            }
            break;
        case 18:
            OPEN_MEMCARD.message = 0xB;
            break;
        case 4:
            OPEN_MEMCARD.message = 0xE;
            break;
        case 9:
            OPEN_MEMCARD.message = 0x12;
            break;
        case 15:
            OPEN_MEMCARD.message = 0x13;
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD.state = 1;
            }
            break;
        case 12:
            if (PAD_STATES[port]->pressed & 0x40) {
                OPEN_MEMCARD.cancelled = 0;
                OPEN_MEMCARD.ready = 1;
            }
            break;
        case 26:
            if (OPEN_MEMCARD.mode == 6) {
                port = 0;
            }
            if (PAD_STATES[port]->pressed & 0x40) {
                OPEN_MEMCARD.ready = 1;
            }
            break;
        case 27:
            OPEN_MEMCARD.ready = 1;
            break;
        case 8:
            OPEN_MEMCARD.message = 7;
            break;
        case 1:
            break;
        }
    } while (OPEN_MEMCARD.ready != 1);
    waitFrames(FRAME_INTERVAL);
}

void OPEN_confirmOverwrite(s32 port) {
    char text[136];

    sprintf(text, "Will write over File%d.\nIs this OK?", ((SessionView *)SESSION_DATA)->saves[port].file + 1);
    initDialog((u8 *)&OPEN_DIALOG, text, 1);
    runDialogForPad((s32 *)&OPEN_DIALOG, port);
    switch (OPEN_DIALOG.choice) {
    case 1:
        OPEN_MEMCARD_STATE = 0x12;
        break;
    case 0:
    case 2:
        OPEN_MEMCARD_STATE = 0x11;
        break;
    }
}

void OPEN_confirmPlayWithoutSaving(s32 port) {
    initDialog((u8 *)&OPEN_DIALOG, NULL, 1);
    runDialogForPad((s32 *)&OPEN_DIALOG, port);
    switch (OPEN_DIALOG.choice) {
    case 1:
        OPEN_MEMCARD_READY = 1;
        ((SessionData *)SESSION_DATA)->playWithoutSaving = 1;
        exitTask();
        break;
    case 0:
    case 2:
        OPEN_MEMCARD_STATE = 1;
        ((SessionData *)SESSION_DATA)->playWithoutSaving = 0;
        break;
    }
}

void OPEN_confirmFormat(s32 pad) {
    initDialog((u8 *)&OPEN_DIALOG, OPEN_TEXT_FORMAT_CARD, 1);
    runDialogForPad((s32 *)&OPEN_DIALOG, pad);
    switch (OPEN_DIALOG.choice) {
    case 1:
        OPEN_MEMCARD_STATE = 8;
        break;
    case 0:
    case 2:
        OPEN_MEMCARD.message = 0x18;
        OPEN_MEMCARD.state = 2;
        break;
    }
}

void OPEN_selectSaveFile(s32 port) {
    char text[136];
    s32 i; /* first the dialog's yes/no flag, then the slot loop's index */

    if (PAD_STATES[port]->repeat & 0x4000) {
        playMenuSound(2);
        if (++OPEN_MEMCARD.slot == 2) {
            PAD_STATES[port]->repeatEnabled = 0;
        }
        if (OPEN_MEMCARD.slot >= 3) {
            OPEN_MEMCARD.slot = 0;
        }
        OPEN_uploadSavePlaceImage(OPEN_MEMCARD_SLOT);
    } else if (PAD_STATES[port]->repeat & 0x1000) {
        playMenuSound(2);
        if (--OPEN_MEMCARD.slot == 0) {
            PAD_STATES[port]->repeatEnabled = 0;
        }
        if (OPEN_MEMCARD.slot < 0) {
            OPEN_MEMCARD.slot = 2;
        }
        OPEN_uploadSavePlaceImage(OPEN_MEMCARD_SLOT);
    } else if (PAD_STATES[port]->pressed & 0x40) {
        if (OPEN_MEMCARD.empty[port][OPEN_MEMCARD.slot] == 0 || OPEN_MEMCARD.mode == 0) {
            playMenuSound(1);
            i = 0;
            if (OPEN_MEMCARD.mode == 0) {
                if (OPEN_MEMCARD.empty[port][OPEN_MEMCARD.slot] == 0) {
                    sprintf(text, "Will write over File %d.\nCreate a New File?", OPEN_MEMCARD.slot + 1);
                    i = 1;
                } else if (OPEN_MEMCARD_FREE_BLOCKS < 2) {
                    sprintf(text, "Not enough Free Blocks.\nYou need 2 Blocks to save.");
                } else {
                    sprintf(text, "Creating a \"Digital Card Battle\" File.\nIs this OK?");
                    i = 1;
                }
            } else if ((OPEN_MEMCARD.slots[port] + OPEN_MEMCARD.slot)->size != 0x2774) {
                sprintf(text, "File %d is corrupted!", OPEN_MEMCARD.slot + 1);
            } else {
                sprintf(text, "Do you want to Load File%d?", OPEN_MEMCARD_SLOT + 1);
                i = 1;
            }
            initDialog((u8 *)&OPEN_DIALOG, text, i);
            runDialogForPad((s32 *)&OPEN_DIALOG, port);
            if (i == 1 && OPEN_DIALOG.choice == 1) {
                ((SessionView *)SESSION_DATA)->saves[port].file = OPEN_MEMCARD.slot;
                if (OPEN_MEMCARD.mode == 0) {
                    OPEN_MEMCARD.state = 4;
                } else {
                    OPEN_MEMCARD.state = 9;
                    if ((OPEN_MEMCARD.slots[port] + OPEN_MEMCARD.slot)->activePartner < 6) {
                        changeScrollingBackground((OPEN_MEMCARD.slots[port] + OPEN_MEMCARD.slot)->activePartner, 0x380, 0, 0x380, 0x80);
                    }
                }
                OPEN_hideSaveSlots();
            }
        }
    } else if (PAD_STATES[port]->pressed & 0x10) {
        playMenuSound(0);
        if (OPEN_MEMCARD.mode == 0) {
            OPEN_MEMCARD.message = 10;
        } else {
            OPEN_MEMCARD_MESSAGE = 6;
        }
        initDialog((u8 *)&OPEN_DIALOG, NULL, 1);
        runDialogForPad((s32 *)&OPEN_DIALOG, port);
        switch (OPEN_DIALOG.choice) {
        case 1:
            OPEN_MEMCARD.ready = 1;
            OPEN_hideSaveSlots();
            OPEN_MEMCARD.state = 0x1A;
            ((SessionData *)SESSION_DATA)->playWithoutSaving = 1;
            break;
        case 0:
        case 2:
            ((SessionData *)SESSION_DATA)->playWithoutSaving = 0;
            if (OPEN_MEMCARD.mode == 0) {
                OPEN_MEMCARD.message = 0x11;
            } else {
                OPEN_MEMCARD_MESSAGE = 0x10;
            }
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (OPEN_MEMCARD.slot == i) {
            OPEN_MEMCARD.slotWindows[i].message = 0x1B;
            OPEN_MEMCARD.slotWindows[i].window.brightness = 0x80;
        } else {
            OPEN_MEMCARD.slotWindows[i].message = 0x1C;
            OPEN_MEMCARD.slotWindows[i].window.brightness = 0x40;
        }
    }
}
