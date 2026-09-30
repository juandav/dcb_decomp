#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/card_db.h"
#include "dcb/text.h"
#include "dcb/scene3d.h"
#include "dcb/model_anim.h"
#include "dcb/frame_callback.h"
#include "dcb/model_load.h"
#include "dcb/anim_control.h"
#include "dcb/window.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/player_rank.h"
#include "dcb/menu.h"
#include "dcb/sort.h"
#include "dcb/scroll_bg.h"
#include "dcb/prim_util.h"
#include "dcb/sound_play.h"
#include "dcb/display.h"
#include "dcb/vblank.h"
#include "dcb/pad.h"
#include "dcb/prim.h"
#include "dcb/fade.h"
#include "dcb/sound.h"
#include "dcb/player_data.h"
#include "dcb/save_checksum.h"

extern s32 OPEN_SAVE_FILE_NAMES[3];

extern s32 OPEN_INTRO_IMAGE;
extern s32 OPEN_INTRO_SHOWN_IMAGE;
extern s32 OPEN_INTRO_IMAGE_FADE;
extern s32 D_801F4880;
extern s32 D_801F4884;
typedef struct {
    s32 step;
    s32 page;
    s32 shownPage;
    s32 length;
    s32 done;
    s32 waitInput;
    s32 unk18;
    s32 blink;
} TextScroll;
extern TextScroll OPEN_INTRO_TEXT;
extern UiWindow OPEN_MESSAGE_WINDOW;
extern UiWindow OPEN_PLAYER_NAME_WINDOW;
extern UiWindow OPEN_IMAGE_WINDOW;
void OPEN_drawIntroMessage(UiWindow *window);
typedef struct {
    u16 cards[30];
    char name[0x32];
} StarterDeck;
typedef struct {
    u8 unk0[0x3D04];
    StarterDeck starters[3];
} DeckFile;
extern s16 OPEN_STARTER_BONUS_CARDS[];
extern s32 OPEN_TITLE_SPIN_RADIUS;
extern s32 OPEN_TITLE_SPIN_ANGLE;
extern s32 OPEN_TITLE_SPIN_SIZE;
extern s32 OPEN_TITLE_SPIN_SHADE;
extern s32 OPEN_TITLE_OPTION_DIMMED[3];
extern s32 OPEN_TITLE_STATE;
extern s32 OPEN_TITLE_TIMER;
extern s32 D_801F52A4;
extern s32 D_801F52A8;
extern s32 D_801F52AC;
extern s32 D_801F52B0;
extern s32 OPEN_PRESS_START_SHADE;
extern s32 OPEN_PRESS_START_STEP;
extern u8 D_801F52BC[3];
extern u8 D_801F52C0[3];
extern s32 D_801F52C4;
extern s32 D_801F52C8;
extern POLY_FT4 OPEN_TITLE_SPIN_QUADS[][2];

extern s8 OPEN_TRADABLE_COUNTS[2][301];
typedef struct {
    UiWindow window;
    u8 unk44[4];
} Unk801F7C88;
extern Unk801F7C88 OPEN_MEMCARD_SLOT_WINDOWS[3];
extern UiWindow OPEN_MEMCARD_INFO_WINDOW;
extern UiWindow OPEN_OPERATION_WINDOW;

void StUnSetRing(void);
s32 DecDCTvlc2(u32 *bs, u32 *buf, u16 *table);
s32 StFreeRing(u32 *base);
typedef struct {
    UiWindow window;
    s32 player;
} PlayerWindow;
extern Menu OPEN_CARD_LIST_MENUS[2];
extern Menu OPEN_SORT_MENUS[2];
extern char *OPEN_SORT_OPTIONS[];
typedef s32 (*CompareFunc)(s8 *, s8 *);
extern CompareFunc OPEN_SORT_COMPARES[];
void OPEN_initTradeCardList(s32 player);
extern s32 D_801F4F08;
extern s32 D_801F4F0C;
extern s32 D_801F4F10;
extern s32 D_801F4F14;
extern s32 D_801F4F18;
extern s32 D_801F4F1C;
extern s32 OPEN_TITLE_PART_COUNT;
void OPEN_drawPlayerRecord(PlayerWindow *window);
void OPEN_drawTitlePart(s32 x, s32 y, s32 part);

extern s32 OPEN_TRADE_BANNER_SHOWN;
extern s32 OPEN_TRADE_BANNER_Y;
u16 OPEN_findUnsharedCardSerial(s32 player, s32 card);
extern s8 OPEN_DECK_CARD_COUNTS[2][301];
typedef struct {
    /* 0x00 */ s16 col;
    /* 0x02 */ s16 prevCol;
    /* 0x04 */ s16 row;
    /* 0x06 */ s16 prevRow;
    /* 0x08 */ u8 inputEnabled;
    /* 0x09 */ char name[13];
    /* 0x16 */ u8 cursor;
    /* 0x17 */ u8 onSideMenu;
    /* 0x18 */ s8 sideRow;
    /* 0x19 */ s8 prevSideRow;
    /* 0x1A */ s8 state;
    /* 0x1B */ u8 replaceName;
} NameEntry;
typedef struct {
    u8 deck;
    s8 chosen;
} StarterSelect;
extern StarterSelect OPEN_STARTER_SELECT;
extern CursorHighlight OPEN_STARTER_CURSOR;
extern NameEntry OPEN_NAME_ENTRY;
extern u8 OPEN_NAME_ENTRY_LETTERS[];
s32 OPEN_moveNameEntryCursor();
extern CursorHighlight OPEN_NAME_ENTRY_CURSOR;
extern CursorHighlight OPEN_NAME_CARET;
extern s32 OPEN_FRIEND_MENU_SHOWN;
extern s32 D_801F4F00;
extern s32 D_801F4F04;
extern s32 OPEN_TRADE_ENABLED;
void OPEN_drawSprite(s32 x, s32 y, s32 tpage, s32 v, s32 u, s32 w, s32 clutX, s32 clutY, s32 a8, s32 a9, s32 a10, s32 brightness, s32 z);

typedef struct {
    SpuVolume volume;
    s32 reverb;
    s32 mix;
} SpuExtAttr;
typedef struct {
    u32 mask;
    SpuVolume mvol;
    SpuVolume mvolmode;
    SpuVolume mvolx;
    SpuExtAttr cd;
    SpuExtAttr ext;
} SpuCommonAttr;
void SpuSetCommonAttr(SpuCommonAttr *attr);
typedef struct {
    u8 val0;
    u8 val1;
    u8 val2;
    u8 val3;
} CdAttenuation;
s32 func_8005A784(CdAttenuation *atv);
void SsSetSerialAttr(s8 s_num, s8 attr, s8 mode);
void SsSetSerialVol(s8 s_num, s16 voll, s16 volr);
typedef struct {
    u8 pos[4];
    u32 size;
    char name[16];
} CdFileEntry;
extern s32 OPEN_MOVIE_FILE_SECTOR;
void OPEN_searchCdFile(void *file, char *name);
typedef struct {
     u8 *vlcbuf[2];
     s32 vlcid;
     u8 *imgbuf[2];
     s32 imgid;
     Rect16 rect[2];
     s32 rectid;
     Rect16 slice;
     s32 isdone;
} DecEnv;
/* libcd's CdlLOC */
typedef struct {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
} CdLocation;
extern DecEnv OPEN_DEC_ENV;
extern CdLocation OPEN_MOVIE_START_LOC;
extern CdLocation OPEN_MOVIE_LOC;
extern s8 OPEN_MOVIE_STARTED;
extern u8 *OPEN_MOVIE_IMAGE_BUFFER;
extern u8 *OPEN_MOVIE_VLC_BUFFER;
extern s32 OPEN_MOVIE_HEIGHT;
extern s32 OPEN_MOVIE_FRAMES_SHOWN;
extern u16 *OPEN_VLC_TABLE;
extern u32 *OPEN_STREAM_RING;
extern u32 OPEN_MOVIE_FRAME;
extern s8 OPEN_MOVIE_ENDED;
void OPEN_showMovieFrame();
void OPEN_startCdStream(CdLocation *loc);
void DecDCTReset(s32 mode);
void DecDCToutCallback(void (*func)());
void StSetRing(u32 *ring_addr, u32 ring_size);
void StClearRing(void);
void StSetStream(u32 mode, u32 start_frame, u32 end_frame, void (*func1)(), void (*func2)());
typedef struct {
    u16 id;
    u16 type;
    u16 secCount;
    u16 nSectors;
    u32 frameCount;
    u32 frameSize;
    u16 width;
    u16 height;
} StHeader;
extern s32 OPEN_MOVIE_END_FRAME;
s32 StGetNext(u32 **addr, StHeader **header);
void OPEN_muteCdAudio(void);
s32 CdRead2(s32 mode);
/* DigimonCardData with a signed type */
typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ s8 type;
    /* 0x03 */ char name[0x15];
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 attr;
} CardEntry;
extern CardEntry *OPEN_TRADE_CARD_LISTS[2][301];
extern s16 OPEN_TRADE_PICKS[2][3];
extern s32 OPEN_TRADE_STATE;
extern UiWindow OPEN_TRADE_OK_WINDOW;
extern s32 OPEN_SORT_PLAYER;
void OPEN_drawIntroImage(s32 x, s32 y, s32 z);
extern UiWindow OPEN_NAME_ENTRY_WINDOW;
extern UiWindow OPEN_NAME_WINDOW;
extern UiWindow OPEN_NAME_HELP_WINDOW;
void OPEN_drawNameEntryKeyboard(UiWindow *window);
void OPEN_drawNameEntryName(UiWindow *window);
void OPEN_drawNameEntryHelp(UiWindow *window);
extern UiWindow OPEN_STARTER_WINDOW;
void OPEN_drawStarterSelect(UiWindow *window);
extern PlayerWindow OPEN_PLAYER_RECORD_WINDOWS[2];
extern s32 OPEN_FRIEND_MENU_DONE;
typedef struct {
    s32 x;
    s32 y;
    Rect16 uv;
} MenuSprite;
typedef struct {
    UiWindow window;
    u8 slot;
    s8 message;
    u8 unk46[2];
} SlotWindow;
typedef struct {
    /* 0x00 */ char name[0xD];
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 location;
    /* 0x0F */ u8 arena;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 seenCardCount;
    /* 0x14 */ u16 progress;
    /* 0x16 */ u16 size;
    /* 0x18 */ u8 unk18[0xC];
    /* 0x24 */ s32 playTime;
    /* 0x28 */ u32 unk28_0 : 10;
    /* 0x29 */ u32 unk28_10 : 1;
    /* 0x29 */ u32 unk28_11 : 21;
    /* 0x2C */ u8 unk2C[0x2A];
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u8 unk58[0x28];
} SaveSlot;
typedef struct {
    u8 *color;
    s32 z;
    s16 x;
    s16 y;
    u8 slot;
} SlotDraw;
typedef struct {
    /* 0x000 */ POLY_FT4 arrows[2];
    /* 0x050 */ POLY_FT4 arrowShadows[2];
    /* 0x0A0 */ MenuSprite sprites[6];
    /* 0x100 */ SlotWindow slotWindows[3];
    /* 0x1D8 */ SlotWindow infoWindow;
    /* 0x220 */ u8 unk220[8];
    /* 0x228 */ SaveSlot slots[2][3];
    /* 0x528 */ void *buffer;
    /* 0x52C */ s8 empty[2][3];
    /* 0x532 */ s8 animTime;
    /* 0x533 */ u8 animPhase;
    /* 0x534 */ s8 message;
    /* 0x535 */ u8 state;
    /* 0x536 */ u8 freeBlocks;
    /* 0x537 */ u8 mode;
    /* 0x538 */ u8 unk538;
    /* 0x539 */ u8 cancelled;
    /* 0x53A */ s8 ready;
    /* 0x53B */ s8 slot;
    /* 0x53C */ u8 card;
    /* 0x53D */ u8 messagePort;
    /* 0x53E */ u8 loading;
    /* 0x53F */ u8 progress;
    /* 0x540 */ u8 unk540;
    /* 0x541 */ u8 again;
    /* 0x542 */ u8 unk542;
} MemcardScreen;
extern MemcardScreen OPEN_MEMCARD;
extern s8 OPEN_MEMCARD_EMPTY[2][3];
extern u8 OPEN_MEMCARD_PROGRESS;
extern u8 D_801F80CA;
extern UiWindow OPEN_MEMCARD_MESSAGE_WINDOW;
void OPEN_openMemcardWindows(void);
void OPEN_initTransferArrow(POLY_FT4 *poly, s32 shade);
void OPEN_drawSaveDetails(s32 x, s32 y, s32 z);
void OPEN_runMemcardScreen();
extern u8 OPEN_MEMCARD_CANCELLED;
extern u8 OPEN_MEMCARD_MODE;
extern s8 OPEN_MEMCARD_MESSAGE;
extern u8 *OPEN_SAVE_PLACE_IMAGES;
extern u8 OPEN_MEMCARD_LOADING;
extern u8 OPEN_MEMCARD_CARD;
extern Window OPEN_DIALOG;
extern u8 OPEN_MEMCARD_STATE;
typedef struct {
    u8 slot;
    u8 file;
    u8 unk2[2];
    s32 playTime;
} SaveInfo;
typedef struct {
    /* 0x0000 */ u8 unk0[0x1010];
    /* 0x1010 */ SaveInfo saves[2];
    /* 0x1020 */ u8 unk1020[7];
    /* 0x1027 */ u8 unk1027;
    /* 0x1028 */ s8 menuRow;
} SessionView;
extern s8 OPEN_MEMCARD_READY;
extern PlayerProfile *OPEN_MEMCARD_BUFFER;
extern u8 OPEN_MEMCARD_MESSAGE_PORT;

void OPEN_searchCdFile(void *file, char *name) {
    s32 found;

    while (1) {
        func_8005A364(0, 0);
        found = CdSearchFile(file, name);
        if (found == 0) continue;
        if (found != -1) break;
    }
}

void OPEN_clearScreen(s32 r, s32 g, s32 b) {
    Rect16 rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 480;
    rect.h = 480;
    ClearImage(&rect, (u8)r, (u8)g, (u8)b);
}

void OPEN_muteCdAudio(void) {
    SpuCommonAttr attr;

    attr.mask = 0x200;
    attr.cd.mix = 0;
    SpuSetCommonAttr(&attr);
}

void OPEN_setMovieVolume(s32 cdVolume, s32 masterVolume) {
    SpuCommonAttr attr;
    CdAttenuation atv;

    attr.mask = 0x2C3;
    attr.mvol.left = masterVolume;
    attr.mvol.right = masterVolume;
    attr.cd.volume.left = cdVolume;
    attr.cd.volume.right = cdVolume;
    attr.cd.mix = 1;
    SpuSetCommonAttr(&attr);
    atv.val0 = 0xFF;
    atv.val1 = 0;
    atv.val2 = 0xFF;
    atv.val3 = 0;
    func_8005A784(&atv);
    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, 0x7F, 0x7F);
}

void OPEN_setSpuVolume(s32 volume) {
    SpuCommonAttr attr;

    attr.mask = 0x2C3;
    attr.mvol.left = volume;
    attr.mvol.right = volume;
    attr.cd.volume.left = volume;
    attr.cd.volume.right = volume;
    attr.cd.mix = 0;
    SpuSetCommonAttr(&attr);
}

s32 OPEN_findMovieFile(s32 *name) {
    CdFileEntry file;

    OPEN_searchCdFile(&file, name);
    return OPEN_MOVIE_FILE_SECTOR = CdPosToInt(&file);
}

void OPEN_initMovieStream(CdLocation *loc, void (*callback)());
void OPEN_initDecEnv(DecEnv *env, s32 x0, s32 y0, s32 x1, s32 y1);
s32 OPEN_decodeMovieFrame(DecEnv *env);
void OPEN_uploadMovieSlice(void);
void DecDCTvlcBuild(u16 *table);

void OPEN_startMovie(s32 sector, s32 endFrame, s32 volume, s32 frames, s32 height) {
    s32 i;
    s32 y;

    OPEN_MOVIE_HEIGHT = height;
    OPEN_MOVIE_END_FRAME = endFrame;
    OPEN_muteCdAudio();
    OPEN_clearScreen(0, 0, 0);
    func_80014C08(2);
    CdIntToPos(sector + OPEN_MOVIE_FILE_SECTOR, (u8 *)&OPEN_MOVIE_START_LOC);
    OPEN_initMovieStream(&OPEN_MOVIE_START_LOC, OPEN_uploadMovieSlice);
    y = (240 - OPEN_MOVIE_HEIGHT) / 2;
    OPEN_initDecEnv(&OPEN_DEC_ENV, 0, y, 0, y + 240);
    DecDCTvlcBuild(OPEN_VLC_TABLE);
    for (i = 0; i < frames; i++) {
        while (OPEN_decodeMovieFrame(&OPEN_DEC_ENV) == -1) {
            OPEN_MOVIE_LOC = OPEN_MOVIE_START_LOC;
            OPEN_startCdStream(&OPEN_MOVIE_LOC);
        }
    }
    OPEN_setMovieVolume(volume, volume);
    OPEN_MOVIE_STARTED = 0;
}

extern s32 OPEN_RING_FREE_SECTORS;
extern s32 OPEN_RING_OVER_SECTORS;
extern s8 D_801F0850;
void StRingStatus(s32 *freeSectors, s32 *overSectors);
void MoveImage(Rect16 *rect, s32 x, s32 y);
void DecDCTin(u8 *buf, s32 mode);
void DecDCTout(u8 *buf, s32 size);
s32 StGetBackloc(CdLocation *loc);
void OPEN_waitMovieFrame(DecEnv *env, s32 unused);

void OPEN_showMovieFrame(FrameBuffer *fb, s32 start) {
    DISPENV disp;
    Rect16 copy;
    Rect16 rect;
    s32 y;
    s32 pos;

    rect.x = 0;
    rect.y = 0;
    rect.w = 480;
    rect.h = OPEN_MOVIE_HEIGHT;
    copy = rect;
    StRingStatus(&OPEN_RING_FREE_SECTORS, &OPEN_RING_OVER_SECTORS);
    if (OPEN_MOVIE_STARTED != 0 || start != 0) {
        OPEN_MOVIE_STARTED = 1;
        D_801F0850 = start;
        if (OPEN_MOVIE_ENDED != 0) {
            GetDispEnv(&disp);
            y = disp.disp[1];
            disp.disp[1] ^= 240;
            MoveImage(&copy, 0, y + (240 - OPEN_MOVIE_HEIGHT) / 2);
            DrawSync(0);
        } else {
            DecDCTin(OPEN_DEC_ENV.vlcbuf[OPEN_DEC_ENV.vlcid], 3);
            DecDCTout(OPEN_DEC_ENV.imgbuf[OPEN_DEC_ENV.imgid], OPEN_DEC_ENV.slice.w * OPEN_DEC_ENV.slice.h / 2);
            while (OPEN_decodeMovieFrame(&OPEN_DEC_ENV) == -1) {
                pos = StGetBackloc(&OPEN_MOVIE_LOC);
                if (pos > OPEN_MOVIE_END_FRAME || pos <= 0) {
                    OPEN_MOVIE_LOC = OPEN_MOVIE_START_LOC;
                }
                OPEN_startCdStream(&OPEN_MOVIE_LOC);
            }
            OPEN_waitMovieFrame(&OPEN_DEC_ENV, 0);
        }
    }
}

void OPEN_initDecEnv(DecEnv *env, s32 x0, s32 y0, s32 x1, s32 y1) {
    env->vlcbuf[0] = OPEN_MOVIE_VLC_BUFFER;
    env->vlcbuf[1] = OPEN_MOVIE_VLC_BUFFER + 0x28000;
    env->vlcid = 0;
    env->imgbuf[0] = OPEN_MOVIE_IMAGE_BUFFER;
    env->imgbuf[1] = OPEN_MOVIE_IMAGE_BUFFER + 0x2D00;
    env->imgid = 0;
    env->rect[0].x = x0;
    env->rect[0].y = y0;
    env->rect[1].x = x1;
    env->rect[1].y = y1;
    env->rectid = 0;
    env->slice.x = x0;
    env->slice.y = y0;
    env->slice.w = 24;
    env->rect[0].w = env->rect[1].w = 480;
    env->slice.h = env->rect[0].h = env->rect[1].h = OPEN_MOVIE_HEIGHT;
    env->isdone = 0;
}

void OPEN_initMovieStream(CdLocation *loc, void (*callback)()) {
    s32 *callbacks;

    callbacks = &FRAME_CALLBACKS;
    OPEN_MOVIE_FRAMES_SHOWN = 0;
    OPEN_VLC_TABLE = allocTaskHeapBlock(0x11000);
    OPEN_STREAM_RING = allocTaskHeapBlock(0x20000);
    OPEN_MOVIE_VLC_BUFFER = allocTaskHeapBlock(0x50000);
    OPEN_MOVIE_IMAGE_BUFFER = allocTaskHeapBlock(0x5A00);
    OPEN_MOVIE_FRAME = 0;
    OPEN_MOVIE_ENDED = 0;
    callbacks[0] = (s32)OPEN_showMovieFrame;
    callbacks[1] = 0;
    DecDCTReset(0);
    DecDCToutCallback(callback);
    StSetRing(OPEN_STREAM_RING, 0x40);
    StClearRing();
    StSetStream(1, 1, -1, 0, 0);
    OPEN_startCdStream(loc);
}

void OPEN_stopMovie(void) {
    VSync(0);
    OPEN_MOVIE_ENDED = 1;
    OPEN_muteCdAudio();
    func_80014C08(2);
    freeHeapBlock(OPEN_MOVIE_IMAGE_BUFFER);
    freeHeapBlock(OPEN_MOVIE_VLC_BUFFER);
    freeHeapBlock(OPEN_STREAM_RING);
    freeHeapBlock(OPEN_VLC_TABLE);
    DecDCToutCallback(0);
    StUnSetRing();
    OPEN_setSpuVolume(0x3FFF);
}

extern s32 D_801D98BC;
void StCdInterrupt(void);

void OPEN_uploadMovieSlice(void) {
    Rect16 snap;
    s32 id;

    if (D_801D98BC != 0) {
        StCdInterrupt();
        D_801D98BC = 0;
    }
    id = OPEN_DEC_ENV.imgid;
    snap = OPEN_DEC_ENV.slice;
    OPEN_DEC_ENV.imgid = id == 0;
    OPEN_DEC_ENV.slice.x += OPEN_DEC_ENV.slice.w;
    if (OPEN_DEC_ENV.slice.x < OPEN_DEC_ENV.rect[OPEN_DEC_ENV.rectid].x + OPEN_DEC_ENV.rect[OPEN_DEC_ENV.rectid].w) {
        DecDCTout(OPEN_DEC_ENV.imgbuf[OPEN_DEC_ENV.imgid], OPEN_DEC_ENV.slice.w * OPEN_DEC_ENV.slice.h / 2);
    } else {
        OPEN_DEC_ENV.isdone = 1;
        OPEN_DEC_ENV.rectid = OPEN_DEC_ENV.rectid == 0;
        OPEN_DEC_ENV.slice.x = OPEN_DEC_ENV.rect[OPEN_DEC_ENV.rectid].x;
        OPEN_DEC_ENV.slice.y = OPEN_DEC_ENV.rect[OPEN_DEC_ENV.rectid].y;
    }
    LoadImage((s16 *)&snap, (s32)OPEN_DEC_ENV.imgbuf[id]);
}

s32 OPEN_decodeMovieFrame(DecEnv *env) {
    u32 *frame;
    s32 timeout;

    timeout = 2000;
    while ((frame = OPEN_getNextMovieFrame(env)) == NULL) {
        if (--timeout == 0) {
            return -1;
        }
    }
    env->vlcid = env->vlcid == 0;
    DecDCTvlc2(frame, env->vlcbuf[env->vlcid], OPEN_VLC_TABLE);
    StFreeRing(frame);
    return 0;
}

u32 *OPEN_getNextMovieFrame(void) {
    u32 *addr;
    StHeader *header;
    s32 timeout;

    timeout = 2000;
    while (StGetNext(&addr, &header) != 0) {
        if (--timeout == 0) {
            return NULL;
        }
    }
    if (header->frameCount >= OPEN_MOVIE_END_FRAME || header->frameCount < OPEN_MOVIE_FRAME) {
        OPEN_MOVIE_ENDED = 1;
        OPEN_muteCdAudio();
    }
    OPEN_MOVIE_FRAME = header->frameCount;
    return addr;
}

void OPEN_waitMovieFrame(DecEnv *env, s32 unused) {
    volatile s32 timeout;

    timeout = 0x800000;
    if (++OPEN_MOVIE_FRAMES_SHOWN >= OPEN_MOVIE_END_FRAME) {
        OPEN_MOVIE_ENDED = 1;
    }
    while (env->isdone == 0) {
        if (--timeout == 0) {
            env->isdone = 1;
            env->rectid = env->rectid == 0;
            env->slice.x = env->rect[env->rectid].x;
            env->slice.y = env->rect[env->rectid].y;
        }
    }
    env->isdone = 0;
}

void OPEN_startCdStream(CdLocation *loc) {
    u8 param[4];

    param[0] = 0x80;
    do {
        while (CdControlB(2, (u8 *)loc, 0) == 0) {
            func_8005A364(0, 0);
        }
        while (CdControlB(0xE, param, 0) == 0) {
            func_8005A364(0, 0);
        }
        VSync(3);
    } while (CdRead2(0x1E0) == 0);
}

typedef struct {
    s32 id;
    s32 sector;
    s32 endFrame;
} Movie;

extern Movie OPEN_MOVIES[3];
void OPEN_runMovieRenderLoop();
typedef struct {
    s32 heights[5];
} MovieHeights;
/* the rodata blob that holds this table also holds the sort menu's strings, so it stays as data */
extern const MovieHeights OPEN_MOVIE_HEIGHTS;

/* main calls it as func_801E055C, declared s32 in game.h; nothing is returned */
s32 OPEN_playMovie(s32 index) {
    MovieHeights movie = OPEN_MOVIE_HEIGHTS;
    s32 wait;

    func_80014A00(0x1F);
    func_80014A00(0x19);
    func_80014C08(0x10);
    OPEN_clearScreen(0, 0, 0);
    resetDisplay(320, 240, 1);
    ((Graphics *)&GRAPHICS)->unk48 = -30;
    ((Graphics *)&GRAPHICS)->vblanksPerFrame = 2;
    func_800149B8(0x1F, 0, 0, 0x1000, OPEN_runMovieRenderLoop);
    func_80014C08(0x1E);
    setTaskVsyncMode(0);
    OPEN_startMovie(OPEN_MOVIES[index].sector, OPEN_MOVIES[index].endFrame, 0x3FFF, 1, movie.heights[index]);
    while (!(PAD_STATES[0]->repeat & 0x800) && OPEN_MOVIE_ENDED == 0) {
        func_80014AC8();
    }
    if (!(PAD_STATES[0]->repeat & 0x860)) {
        wait = 29;
        do {
            func_80014AC8();
            if (PAD_STATES[0]->repeat & 0x860) {
                break;
            }
        } while (--wait != -1);
    }
    func_80014A00(0x1F);
    OPEN_clearScreen(0, 0, 0);
    OPEN_stopMovie();
    resetDisplay(320, 240, 0);
    func_80014C08(0x3C);
}

void OPEN_runMovieRenderLoop(void) {
    Graphics *gfx;

    gfx = (Graphics *)&GRAPHICS;
    gfx->unk8[0] = 0;
    VBLANK_COUNTER = 0;
    SetDispMask(0);
    for (; gfx->unk48 <= 0; gfx->unk48++) {
        pollPads();
        func_80014C08(1);
        gfx->vblanksPerFrame = VBLANK_COUNTER;
        if (VBLANK_COUNTER == 0) {
            gfx->vblanksPerFrame = 1;
        }
        VBLANK_COUNTER = 0;
    }
    SetDispMask(1);
    FRAME_BUFFER_INDEX = 0;
    CURRENT_FRAME_BUFFER = &gfx->buffers[0];
    for (;;) {
        pollPads();
        FRAME_BUFFER_INDEX ^= 1;
        CURRENT_FRAME_BUFFER = &gfx->buffers[FRAME_BUFFER_INDEX];
        if (gfx->unk8[0] != 0) {
            OPEN_showMovieFrame(CURRENT_FRAME_BUFFER, FRAME_BUFFER_INDEX);
        }
        VSync(0);
        PutDispEnv(&CURRENT_FRAME_BUFFER->disp);
        PutDrawEnv(&CURRENT_FRAME_BUFFER->draw);
        func_80014AC8();
        gfx->vblanksPerFrame = VBLANK_COUNTER;
        if (VBLANK_COUNTER == 0) {
            gfx->vblanksPerFrame = 1;
        }
        VBLANK_COUNTER = 0;
    }
}

s32 OPEN_compareByFire(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 0) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 0) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByIce(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 1) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 1) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByNature(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 2) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 2) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByDarkness(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 3) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 3) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByRare(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 4) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 4) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByOption(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    keyA = (*a)->type == 1;
    keyB = (*b)->type == 1;
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByDigivolve(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    keyA = (*a)->type == 2;
    keyB = (*b)->type == 2;
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByLevel0(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr & 0xF;
    }
    if (keyA == 0) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 0) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByLevel2(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr & 0xF;
    }
    if (keyA == 2) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 2) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByLevel3(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr & 0xF;
    }
    if (keyA == 3) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 3) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

void OPEN_drawSortMenu(PlayerWindow *window) {
    char text[72]; /* unused, but it sizes the frame */
    Menu *menu;
    s32 player;
    s32 i;
    s32 x;
    s32 z;

    player = window->player;
    x = window->window.originX;
    z = window->window.z;
    for (i = 0; i < OPEN_SORT_MENUS[player].nrows; i++) {
        if (i < window->window.view.y / OPEN_SORT_MENUS[player].rowH) {
            continue;
        }
        if ((window->window.view.y + window->window.rect.h) / OPEN_SORT_MENUS[player].rowH < i) {
            break;
        }
        drawText(x, window->window.originY + i * OPEN_SORT_MENUS[player].rowH + 1, (s32)OPEN_SORT_OPTIONS[i], 7, z);
    }
    menu = &OPEN_SORT_MENUS[player];
    updateMenuCursor(menu);
    if (menu->active && (PAD_STATES[player]->pressed & 0x40)) {
        playMenuSound(1);
        if (OPEN_SORT_COMPARES[menu->row] != NULL) {
            OPEN_SORT_PLAYER = player;
            sortArray((s8 *)OPEN_TRADE_CARD_LISTS[player], 301, 4, OPEN_SORT_COMPARES[menu->row]);
        } else {
            OPEN_initTradeCardList(player);
        }
        OPEN_CARD_LIST_MENUS[player].row = 0;
        centerMenuOnCursor(&OPEN_CARD_LIST_MENUS[player]);
    }
}

void OPEN_drawTradeBanner(void) {
    if (isSpritePoolFull() == 0) {
        if (OPEN_TRADE_BANNER_SHOWN != 0) {
            OPEN_TRADE_BANNER_Y += 4;
            if (OPEN_TRADE_BANNER_Y > 8) {
                OPEN_TRADE_BANNER_Y = 8;
            }
        } else {
            OPEN_TRADE_BANNER_Y -= 4;
            if (OPEN_TRADE_BANNER_Y < -32) {
                OPEN_TRADE_BANNER_Y = -32;
            }
        }
        CUR_SPRT->sp.x0 = 6;
        CUR_SPRT->sp.y0 = OPEN_TRADE_BANNER_Y;
        CUR_SPRT->sp.u0 = 0x58;
        CUR_SPRT->sp.v0 = 0x4A;
        CUR_SPRT->sp.clut = 0x7FB8;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x20;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

s32 OPEN_countSharedCardCopies(s32 player, s32 card) {
    s32 other;
    s32 otherCount;
    s32 count;
    s32 i;
    s32 j;
    s32 shared;

    other = player ^ 1;
    otherCount = getOwnedCardCount(other, card);
    count = getOwnedCardCount(player, card);
    shared = 0;
    for (i = 0; i < count; i++) {
        for (j = 0; j < otherCount; j++) {
            if (((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][i] == ((PlayerProfile *)PLAYER_PROFILES)[other].cardCopySerials[card][j]) {
                shared++;
                break;
            }
        }
    }
    return shared;
}

u16 OPEN_findUnsharedCardSerial(s32 player, s32 card) {
    s32 other;
    s32 otherCount;
    s32 count;
    s32 i;
    s32 j;

    other = player ^ 1;
    otherCount = getOwnedCardCount(other, card);
    count = getOwnedCardCount(player, card);
    for (i = 0; i < count; i++) {
        for (j = 0; j < otherCount; j++) {
            if (((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][i] != ((PlayerProfile *)PLAYER_PROFILES)[other].cardCopySerials[card][j]) {
                return ((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][i];
            }
        }
    }
    return ((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][0];
}

void OPEN_addCardCopy(s32 player, s32 card, s32 serial) {
    PLAYER_DATA(player).cardCopySerials[card][getOwnedCardCount(player, card)] = serial;
    if (PLAYER_DATA(player).cardCollection[card] == 0) {
        PLAYER_DATA(player).cardCollection[card] |= 0x20;
    }
    PLAYER_DATA(player).cardCollection[card]++;
    if ((PLAYER_DATA(player).cardCollection[card] & 7) == 6) {
        PLAYER_DATA(player).cardCollection[card] |= 0x10;
    }
    if (((u8 *)getCardData(card))[0x19] == 0) {
        PLAYER_DATA(player).cardCollection[card] |= 0x10;
    }
    PLAYER_DATA(player).cardCollection[card] |= 0xC8;
    updatePlayerRanks(player);
    if ((u16)++PLAYER_DATA(player).unk4C >= 10000) {
        PLAYER_DATA(player).unk4C = 9999;
    }
}

u16 OPEN_removeCardCopy(s32 player, s32 card) {
    s32 count;
    u16 serial;
    s32 i;
    s32 j;

    count = getOwnedCardCount(player, card);
    serial = OPEN_findUnsharedCardSerial(player, card);
    for (i = 0; i < count; i++) {
        if (PLAYER_DATA(player).cardCopySerials[card][i] == serial) {
            for (j = i; j < count - 1; j++) {
                PLAYER_DATA(player).cardCopySerials[card][j] = PLAYER_DATA(player).cardCopySerials[card][j + 1];
            }
            break;
        }
    }
    PLAYER_DATA(player).cardCollection[card]--;
    updatePlayerRanks(player);
    if ((u16)++PLAYER_DATA(player).unk4E >= 10000) {
        PLAYER_DATA(player).unk4E = 9999;
    }
    return serial;
}

void OPEN_countCardsInDecks(void) {
    s8 counts[3][301];
    s32 player;
    s32 i;
    s32 j;
    s32 max;

    for (player = 0; player < 2; player++) {
        for (i = 0; i < 301; i++) {
            OPEN_DECK_CARD_COUNTS[player][i] = 0;
            for (j = 0; j < 3; j++) {
                counts[j][i] = 0;
            }
        }
        for (i = 0; i < 3; i++) {
            if (PLAYER_DATA(player).savedDecks[i].inUse != 0) {
                for (j = 0; j < 30; j++) {
                    counts[i][PLAYER_DATA(player).savedDecks[i].cards[j].id]++;
                }
            }
        }
        for (i = 0; i < 301; i++) {
            max = 0;
            for (j = 0; j < 3; j++) {
                if (max < counts[j][i]) {
                    max = counts[j][i];
                }
            }
            OPEN_DECK_CARD_COUNTS[player][i] = max;
        }
    }
}

void OPEN_initTradeCardList(s32 player) {
    s32 n;
    s32 i;
    s32 shared;
    s32 other;

    n = 0;
    for (i = 0; i < 0xBF; i++) {
        OPEN_TRADE_CARD_LISTS[player][n++] = (CardEntry *)&((DigimonCardData *)DIGIMON_CARDS)[i];
    }
    for (i = 0; i < 0x66; i++) {
        OPEN_TRADE_CARD_LISTS[player][n++] = (CardEntry *)&((OptionCardData *)OPTION_CARDS)[i];
    }
    for (i = 0; i < 8; i++) {
        OPEN_TRADE_CARD_LISTS[player][n++] = (CardEntry *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[i];
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).partners[i].cardId != 0) {
            OPEN_TRADE_CARD_LISTS[player][PLAYER_DATA(player).partners[i].cardId] = (CardEntry *)&PLAYER_DATA(player).partners[i];
        }
    }
    for (i = 0; i < 301; i++) {
        shared = OPEN_countSharedCardCopies(player, i);
        OPEN_TRADABLE_COUNTS[player][i] = getOwnedCardCount(player, i) - OPEN_DECK_CARD_COUNTS[player][i];
        if (OPEN_TRADABLE_COUNTS[player][i] > getOwnedCardCount(player, i) - shared) {
            OPEN_TRADABLE_COUNTS[player][i] = getOwnedCardCount(player, i) - shared;
        }
        if (OPEN_TRADABLE_COUNTS[player][i] < 0) {
            OPEN_TRADABLE_COUNTS[player][i] = 0;
        }
        other = player ^ 1;
        if (OPEN_TRADABLE_COUNTS[player][i] + getOwnedCardCount(other, i) >= 7) {
            OPEN_TRADABLE_COUNTS[player][i] = 6 - getOwnedCardCount(other, i);
        }
    }
}

extern s16 OPEN_TRADE_PICK_X[2][3];
extern u32 *OPEN_CARD_IMAGE_ARC;

void OPEN_drawTradeList(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;
    s32 done;
    s32 i;
    s32 row;
    s32 col;
    s32 card;
    s32 specialty;

    x = window->originX;
    y = window->originY;
    z = window->z;
    done = 0;
    if (OPEN_TRADE_STATE != 0) {
        for (i = 0; i < 3; i++) {
            if (OPEN_TRADE_PICK_X[0][i] < i * 44 + 0xA6) {
                OPEN_TRADE_PICK_X[0][i] += 8;
            } else {
                OPEN_TRADE_PICK_X[0][i] = i * 44 + 0xA6;
                done++;
            }
        }
        for (i = 0; i < 3; i++) {
            if (OPEN_TRADE_PICK_X[1][i] > i * 44) {
                OPEN_TRADE_PICK_X[1][i] -= 8;
            } else {
                OPEN_TRADE_PICK_X[1][i] = i * 44;
                done++;
            }
        }
    }
    if (done == 6) {
        CUR_SPRT->sp.x0 = x + 0x54;
        CUR_SPRT->sp.y0 = y + 6;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0;
        CUR_SPRT->sp.clut = 0x7E78;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x21;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        OPEN_TRADE_STATE = 2;
    }
    for (row = 0; row < 2; row++) {
        for (col = 0; col < 3; col++) {
            if (OPEN_TRADE_PICKS[row][col] == -1) {
                continue;
            }
            card = OPEN_TRADE_PICKS[row][col];
            specialty = getCardSpecialty(card);
            if (specialty == 6) {
                specialty = 5;
            }
            uploadTim((u32 *)((u8 *)OPEN_CARD_IMAGE_ARC + OPEN_CARD_IMAGE_ARC[card]), col * 21 + 0x2C0, row * 41, 0x2C0, 0xFF - (row * 3 + col));
            if (isSpritePoolFull()) {
                return;
            }
            CUR_SPRT->sp.x0 = x + OPEN_TRADE_PICK_X[row][col];
            CUR_SPRT->sp.y0 = y + 3;
            CUR_SPRT->sp.u0 = col * 21 * 2;
            CUR_SPRT->sp.v0 = row * 41;
            CUR_SPRT->sp.clut = getClut(0x2C0, 0xFF - (row * 3 + col));
            CUR_SPRT->sp.w = 0x28;
            CUR_SPRT->sp.h = 0x28;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, 0x2C0, 0));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            if (isSpritePoolFull()) {
                return;
            }
            CUR_SPRT->sp.x0 = x + OPEN_TRADE_PICK_X[row][col];
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0x52;
            CUR_SPRT->sp.clut = getClut(0x2C0, specialty + 0xF0);
            CUR_SPRT->sp.w = 0x28;
            CUR_SPRT->sp.h = 0x30;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, 0x2C0, 0));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
    CUR_SPRT->sp.x0 = x + 0x84;
    CUR_SPRT->sp.y0 = y + 3;
    CUR_SPRT->sp.u0 = 0x80;
    CUR_SPRT->sp.v0 = 0x20;
    CUR_SPRT->sp.clut = 0x7EF8;
    CUR_SPRT->sp.w = 0x20;
    CUR_SPRT->sp.h = 0x29;
    setSemiTrans(&CUR_SPRT->sp, 0);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
    SPRITE_POOL_CURSOR += sizeof(SprtPacket);
}

/* not referenced by any code */
const s32 D_801DDF38 = 8;

const MovieHeights OPEN_MOVIE_HEIGHTS = { { 0xA0, 0xB0, 0xF0, 0xA0, 0xB0 } };

/* the opening movies */
Movie OPEN_MOVIES[3] = {
    { 0, 0, 0xB36 },
    { 1, 0x3A18, 0x717 },
    { 2, 0x8190, 0x52 },
};

s32 OPEN_MOVIE_HEIGHT = 0xB0;

/* the card list menus of both players */
Menu OPEN_CARD_LIST_MENUS[2] = {
    { NULL, NULL, { 0xA, 0x30, 0x90, 0x62 }, 0, -1, 0, -1, 0xA, 0x26, 0x78, 0xC, 1, 0x12D, 0x12, 1, 0, 0xE, 0, 0, 0 },
    { NULL, NULL, { 0xA6, 0x30, 0x90, 0x62 }, 0, -1, 0, -1, 0xA, 0x26, 0x78, 0xC, 1, 0x12D, 0x12, 1, 0, 0xE, 0, 0, 1 },
};

/* the options of the sort menu */
char *OPEN_SORT_OPTIONS[11] = {
    "Number",
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
};

/* the sort menus of both players */
Menu OPEN_SORT_MENUS[2] = {
    { NULL, NULL, { 0x32, 0x3C, 0x52, 0x54 }, 0, -1, 0, -1, 0xA, 0x16, 0xD8, 0xC, 1, 0xB, 0, 1, 0, 0xE, 0, 0, 0 },
    { NULL, NULL, { 0xC8, 0x3C, 0x52, 0x54 }, 0, -1, 0, -1, 0xA, 0x16, 0x48, 0xC, 1, 0xB, 0, 1, 0, 0xE, 0, 0, 1 },
};

/* how each option of the sort menu compares two cards ("Number" keeps the order) */
CompareFunc OPEN_SORT_COMPARES[11] = {
    NULL,
    (CompareFunc)OPEN_compareByFire,
    (CompareFunc)OPEN_compareByIce,
    (CompareFunc)OPEN_compareByNature,
    (CompareFunc)OPEN_compareByDarkness,
    (CompareFunc)OPEN_compareByRare,
    (CompareFunc)OPEN_compareByOption,
    (CompareFunc)OPEN_compareByDigivolve,
    (CompareFunc)OPEN_compareByLevel0,
    (CompareFunc)OPEN_compareByLevel2,
    (CompareFunc)OPEN_compareByLevel3,
};

void OPEN_drawTradeOk(UiWindow *window) {
    s32 held[2];
    s32 offset;
    char text[64];
    s32 count;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    count = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            if (OPEN_TRADE_PICKS[i][j] != -1) {
                count++;
            }
        }
    }
    if (count == 0) {
        strcpy(text, "You can't Trade between 0 Cards!");
        offset = (0x100 - measureText(text)) / 2;
        drawText(x + offset, y + 14, (s32)text, 7, z);
        return;
    }
    strcpy(text, "If these Cards are OK,\npress and hold *b2 Button.");
    offset = (0x100 - measureText(text)) / 2;
    drawText(x + offset, y + 8, (s32)text, 7, z);
    for (i = 0; i < 2; i++) {
        if ((PAD_STATES[i]->held & 0x40) || OPEN_TRADE_STATE != 0) {
            held[i] = 1;
            CUR_SPRT->sp.x0 = i * 0xD6 + x - 2;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0xA0;
            CUR_SPRT->sp.v0 = 0x20;
            CUR_SPRT->sp.clut = 0x7F78;
            CUR_SPRT->sp.w = 0x2C;
            CUR_SPRT->sp.h = 0x2A;
            setSemiTrans(&CUR_SPRT->sp, 1);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x3E);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        } else {
            held[i] = 0;
        }
        CUR_SPRT->sp.x0 = x + 10 + i * 0xD6;
        CUR_SPRT->sp.y0 = y + 10;
        CUR_SPRT->sp.u0 = 0xCC;
        CUR_SPRT->sp.v0 = 0x20;
        CUR_SPRT->sp.clut = 0x7F38;
        CUR_SPRT->sp.w = 0x14;
        CUR_SPRT->sp.h = 0x1D;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
    if (held[0] && held[1] && OPEN_TRADE_STATE == 0) {
        animateWindowTo(&OPEN_TRADE_OK_WINDOW, (Rect16 *)-1);
        OPEN_TRADE_STATE = 1;
        playMenuSound(1);
    }
}

typedef struct {
    UiWindow window;
    u8 port;
    u8 message;
    u8 unk46[2];
} MessageWindow;

extern MessageWindow OPEN_TRADE_WARNING_WINDOWS[2];

void OPEN_drawTradeWarning(MessageWindow *window) {
    char text[64];
    s32 port;
    s32 x;
    s32 y;
    s32 z;

    port = window->port;
    x = window->window.originX;
    y = window->window.originY;
    z = window->window.z;
    switch (window->message) {
    case 1:
        strcpy(text, "You can't trade\nthis Card!");
        break;
    case 2:
        strcpy(text, "You can't transfer\nany more Cards!");
        break;
    case 3:
        strcpy(text, "This Card is\nunidentified!");
        break;
    }
    drawText(x + (0x5E - measureText(text)) / 2, y, (s32)text, 7, z);
    if (PAD_STATES[port]->pressed & 0x50) {
        animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[port].window, (Rect16 *)-1);
        (&OPEN_CARD_LIST_MENUS[port])->active = 1;
        playMenuSound(4);
    }
}

extern s32 OPEN_TRADE_PLAYER_READY[2];
extern s32 OPEN_TRADE_QUIT;

void OPEN_drawCardList(PlayerWindow *window) {
    char text[72];
    u8 rgb[3];
    Rect16 rect;
    Rect16 rect2;
    s32 full;
    Menu *menu;
    UiWindow *win;
    s32 player;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 card;
    s32 palette;
    s32 count;
    s32 shade;

    win = &window->window;
    player = window->player;
    x = win->originX;
    z = win->z;
    full = 0;
    if (OPEN_TRADE_PLAYER_READY[player] != 0) {
        CUR_SPRT->sp.x0 = player * 0x9C + 0x12;
        CUR_SPRT->sp.y0 = 0x4B;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0x21;
        CUR_SPRT->sp.clut = 0x7E38;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x21;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        shade = 0x40;
    } else {
        shade = 0x80;
    }
    rgb[0] = shade;
    rgb[1] = shade;
    rgb[2] = shade;
    for (i = 0; i < OPEN_CARD_LIST_MENUS[player].nrows; i++) {
        if (i < win->view.y / OPEN_CARD_LIST_MENUS[player].rowH) {
            continue;
        }
        if ((win->view.y + win->rect.h) / OPEN_CARD_LIST_MENUS[player].rowH < i) {
            break;
        }
        y = win->originY + i * OPEN_CARD_LIST_MENUS[player].rowH + 1;
        card = OPEN_TRADE_CARD_LISTS[player][i]->id;
        palette = 8;
        if (OPEN_TRADABLE_COUNTS[player][card] > 0) {
            palette = 7;
        }
        if (OPEN_TRADE_CARD_LISTS[player][i]->unk18 == 0) {
            palette = 3;
        }
        if (PLAYER_DATA(player).cardCollection[OPEN_TRADE_CARD_LISTS[player][i]->id] & 0x40) {
            drawTextColored(x + 0x12, y, OPEN_TRADE_CARD_LISTS[player][i]->name, rgb, palette, z);
        } else {
            palette = 9;
            drawTextColored(x + 0x12, y, "-----------------", rgb, 9, z);
        }
        sprintf(text, "%3.3d", OPEN_TRADE_CARD_LISTS[player][i]->id);
        drawSmallTextColored(x, y + 6, text, palette, rgb, z);
        sprintf(text, "%d", OPEN_TRADABLE_COUNTS[player][card]);
        drawTextColored(x + 0x7E, y, text, rgb, palette, z);
    }
    menu = &OPEN_CARD_LIST_MENUS[player];
    updateMenuCursor(menu);
    if (menu->active) {
        card = OPEN_TRADE_CARD_LISTS[player][menu->row]->id;
        if (PAD_STATES[player]->pressed & 0x40) {
            if (PLAYER_DATA(player).cardCollection[card] & 0x40) {
                if (OPEN_TRADE_CARD_LISTS[player][menu->row]->unk18 == 0) {
                    rect.x = player * 0x9C + 0x21;
                    rect.y = 0x5A;
                    rect.w = 0x5E;
                    rect.h = 0x18;
                    animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[player].window, &rect);
                    OPEN_TRADE_WARNING_WINDOWS[player].message = 1;
                    menu->active = 0;
                    playMenuSound(3);
                } else if (OPEN_TRADABLE_COUNTS[player][card] == 0) {
                    rect2.x = player * 0x9C + 0x21;
                    rect2.y = 0x5A;
                    rect2.w = 0x5E;
                    rect2.h = 0x18;
                    animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[player].window, &rect2);
                    OPEN_TRADE_WARNING_WINDOWS[player].message = 2;
                    OPEN_CARD_LIST_MENUS[player].active = 0;
                    playMenuSound(3);
                } else {
                    for (i = 0; i < 3; i++) {
                        if (OPEN_TRADE_PICKS[player][i] == -1) {
                            OPEN_TRADE_PICKS[player][i] = card;
                            OPEN_TRADABLE_COUNTS[player][card]--;
                            if (i == 2) {
                                full = 1;
                            } else {
                                playMenuSound(1);
                            }
                            break;
                        }
                    }
                    if (i == 3) {
                        full = 1;
                    }
                }
            } else {
                rect.x = player * 0x9C + 0x21;
                rect.y = 0x5A;
                rect.w = 0x5E;
                rect.h = 0x18;
                animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[player].window, &rect);
                OPEN_TRADE_WARNING_WINDOWS[player].message = 3;
                OPEN_CARD_LIST_MENUS[player].active = 0;
                playMenuSound(3);
            }
        }
        if (PAD_STATES[player]->pressed & 0x10) {
            playMenuSound(0);
            for (i = 0, count = 0; i < 3; i++) {
                if (OPEN_TRADE_PICKS[player][i] != -1) {
                    count++;
                }
            }
            if (count == 0) {
                OPEN_TRADE_QUIT = player + 2;
                return;
            }
            for (i = 2; i >= 0; i--) {
                if (OPEN_TRADE_PICKS[player][i] != -1) {
                    OPEN_TRADABLE_COUNTS[player][OPEN_TRADE_PICKS[player][i]]++;
                    OPEN_TRADE_PICKS[player][i] = -1;
                    break;
                }
            }
        }
        if ((PAD_STATES[player]->pressed & 0x800) || full) {
            OPEN_CARD_LIST_MENUS[player].active = 0;
            playMenuSound(1);
            OPEN_TRADE_PLAYER_READY[player] = 1;
            if (OPEN_TRADE_PLAYER_READY[0] != 0 && OPEN_TRADE_PLAYER_READY[1] != 0) {
                rect.x = 0x20;
                rect.y = 0x78;
                rect.w = 0x100;
                rect.h = 0x28;
                animateWindowTo(&OPEN_TRADE_OK_WINDOW, &rect);
            }
        }
    } else if (OPEN_TRADE_PLAYER_READY[player] != 0 && OPEN_TRADE_STATE == 0 && (PAD_STATES[player]->pressed & 0x10)) {
        playMenuSound(0);
        OPEN_CARD_LIST_MENUS[player].active = 1;
        OPEN_TRADE_PLAYER_READY[player] = 0;
        animateWindowTo(&OPEN_TRADE_OK_WINDOW, (Rect16 *)-1);
    }
}

void OPEN_drawCardInfo(PlayerWindow *window) {
    char text[72];
    s32 player;
    s32 row;
    s32 x;
    s32 y;
    s32 z;

    player = window->player;
    x = window->window.originX + 1;
    y = window->window.originY + 1;
    z = window->window.z;
    row = OPEN_CARD_LIST_MENUS[player].row;
    if (PLAYER_DATA(player).cardCollection[OPEN_TRADE_CARD_LISTS[player][row]->id] & 0x40) {
        switch (OPEN_TRADE_CARD_LISTS[player][row]->type) {
        case 0:
            drawIcon(x, y, 0, OPEN_TRADE_CARD_LISTS[player][row]->attr >> 4, z);
            drawIcon(x + 14, y, 0, (OPEN_TRADE_CARD_LISTS[player][row]->attr & 0xF) + 16, z);
            break;
        case 1:
            drawIcon(x, y, 0, 5, z);
            break;
        case 2:
            drawIcon(x, y, 0, 6, z);
            break;
        }
        sprintf(text, "in Stock. \f\a%d\f\x06 Cards", getOwnedCardCount(player, OPEN_TRADE_CARD_LISTS[player][row]->id));
        drawSmallText(x + 0x22, y, (s32)text, 6, z);
        sprintf(text, "in a Deck. \f\a%d\f\x06 Cards", OPEN_DECK_CARD_COUNTS[player][OPEN_TRADE_CARD_LISTS[player][row]->id]);
        drawSmallText(x + 0x31, y + 6, (s32)text, 6, z);
    } else {
        strcpy(text, "Unidentified Card");
        drawText(x + (0x90 - measureText(text)) / 2, y, (s32)text, 7, z);
    }
}

extern UiWindow OPEN_TRADE_LIST_WINDOW;
extern PlayerWindow OPEN_SORT_MENU_WINDOWS[2];
extern PlayerWindow OPEN_CARD_LIST_WINDOWS[2];
extern PlayerWindow OPEN_CARD_INFO_WINDOWS[2];
void OPEN_drawTradeOk(UiWindow *window);
void OPEN_drawTradeList(UiWindow *window);
void OPEN_drawCardList(PlayerWindow *window);

void OPEN_drawTradeScreen(void) {
    s32 count;
    s32 i;
    s32 j;

    OPEN_drawTradeBanner();
    count = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            if (OPEN_TRADE_PICKS[i][j] != -1) {
                count++;
            }
        }
    }
    if (count == 0) {
        OPEN_TRADE_OK_WINDOW.label = (s32)"WARNING";
        OPEN_TRADE_OK_WINDOW.palette = 2;
    } else {
        OPEN_TRADE_OK_WINDOW.label = (s32)"TRADE OK?";
        OPEN_TRADE_OK_WINDOW.palette = 1;
    }
    drawWindow(&OPEN_TRADE_OK_WINDOW, OPEN_drawTradeOk, 1);
    drawWindow(&OPEN_TRADE_LIST_WINDOW, OPEN_drawTradeList, 1);
    for (i = 0; i < 2; i++) {
        drawWindow(&OPEN_SORT_MENU_WINDOWS[i].window, OPEN_drawSortMenu, 1);
        drawWindow(&OPEN_CARD_LIST_WINDOWS[i].window, OPEN_drawCardList, 2);
        drawWindow(&OPEN_CARD_INFO_WINDOWS[i].window, OPEN_drawCardInfo, 2);
        drawWindow(&OPEN_TRADE_WARNING_WINDOWS[i].window, OPEN_drawTradeWarning, 1);
    }
}

extern CursorHighlight OPEN_SORT_MENU_CURSORS[2];
extern CursorHighlight OPEN_CARD_LIST_CURSORS[2];
/* "Do you want to Quit Trading?": the string is followed by leftover bytes, so it stays as data */
extern const char OPEN_STR_QUIT_TRADING[];

void OPEN_runCardTrade(s32 parentTask) {
    s32 open[2];
    Rect16 rect;
    u8 dialog[0xB8];
    s32 i;
    s32 j;
    s16 card;

    OPEN_TRADE_BANNER_SHOWN = 1;
    OPEN_TRADE_BANNER_Y = -32;
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\TRADE.ARC", getCurrentTaskId());
    OPEN_CARD_IMAGE_ARC = (u32 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < (s32)(OPEN_CARD_IMAGE_ARC[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)OPEN_CARD_IMAGE_ARC + OPEN_CARD_IMAGE_ARC[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(OPEN_CARD_IMAGE_ARC);
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\CARD_F.TIM", getCurrentTaskId());
    OPEN_CARD_IMAGE_ARC = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTim(OPEN_CARD_IMAGE_ARC, 0x2C0, 0x52, 0x2C0, 0xF0);
    DrawSync(0);
    func_80014C08(FRAME_INTERVAL);
    freeHeapBlock(OPEN_CARD_IMAGE_ARC);
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    OPEN_CARD_IMAGE_ARC = (u32 *)func_80014C08(0x7FFFFFFF);
    rect.x = 0x20;
    rect.y = 0x78;
    rect.w = 0x100;
    rect.h = 0x28;
    openWindow(&OPEN_TRADE_OK_WINDOW, &rect, -1, (s16 *)-1, 8, 0x25, 0x80, 0xC);
    animateWindowTo(&OPEN_TRADE_OK_WINDOW, (Rect16 *)-1);
    OPEN_TRADE_OK_WINDOW.label = (s32)"TRADE OK?";
    rect.x = 0xC;
    rect.y = 0xB6;
    rect.w = 0x128;
    rect.h = 0x2E;
    openWindow(&OPEN_TRADE_LIST_WINDOW, &rect, -1, (s16 *)-1, 8, 0x25, 0x80, 0xC);
    OPEN_TRADE_LIST_WINDOW.label = (s32)"TRADE LIST";
    for (i = 0; i < 2; i++) {
        openMenu(&OPEN_SORT_MENUS[i], &OPEN_SORT_MENU_WINDOWS[i].window, &OPEN_SORT_MENU_CURSORS[i], (Bytes4 *)-1);
        animateWindowTo(&OPEN_SORT_MENU_WINDOWS[i].window, (Rect16 *)-1);
        OPEN_SORT_MENU_WINDOWS[i].window.label = (s32)"SORT MENU";
        OPEN_SORT_MENU_WINDOWS[i].player = i;
        rect.x = i * 0x9C + 0x21;
        rect.y = 0x5A;
        rect.w = 0x5E;
        rect.h = 0x18;
        openWindow(&OPEN_TRADE_WARNING_WINDOWS[i].window, &rect, -1, (s16 *)-1, 8, 0x11, 0x80, 0xC);
        animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[i].window, (Rect16 *)-1);
        OPEN_TRADE_WARNING_WINDOWS[i].window.label = (s32)"WARNING";
        OPEN_TRADE_WARNING_WINDOWS[i].window.palette = 2;
        OPEN_TRADE_WARNING_WINDOWS[i].port = i;
        rect.x = i * 0x9C + 0xA;
        rect.y = 0x9A;
        rect.w = 0x90;
        rect.h = 0xE;
        openWindow(&OPEN_CARD_INFO_WINDOWS[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 0xC);
        OPEN_CARD_INFO_WINDOWS[i].player = i;
        openMenu(&OPEN_CARD_LIST_MENUS[i], &OPEN_CARD_LIST_WINDOWS[i].window, &OPEN_CARD_LIST_CURSORS[i], (Bytes4 *)-1);
        if (i == 0) {
            OPEN_CARD_LIST_WINDOWS[i].window.label = (s32)"1P CARD LIST";
        } else {
            OPEN_CARD_LIST_WINDOWS[i].window.label = (s32)"2P CARD LIST";
        }
        OPEN_CARD_LIST_WINDOWS[i].player = i;
        OPEN_TRADE_PLAYER_READY[i] = 0;
        for (j = 0; j < 3; j++) {
            OPEN_TRADE_PICKS[i][j] = -1;
            OPEN_TRADE_PICK_X[i][j] = j * 44 + i * 166;
        }
        open[i] = 0;
    }
    OPEN_countCardsInDecks();
    for (i = 0; i < 2; i++) {
        OPEN_initTradeCardList(i);
        OPEN_CARD_LIST_MENUS[i].row = 0;
        centerMenuOnCursor(&OPEN_CARD_LIST_MENUS[i]);
    }
    OPEN_TRADE_STATE = 0;
    OPEN_TRADE_QUIT = 0;
    playMenuSound(3);
    addFrameCallback((s32)OPEN_drawTradeScreen);
    do {
        func_80014C08(FRAME_INTERVAL);
        for (i = 0; i < 2; i++) {
            if (OPEN_TRADE_PLAYER_READY[i] == 0) {
                if (open[i]) {
                    if (PAD_STATES[i]->pressed & 0x110) {
                        open[i] = 0;
                        playMenuSound(4);
                        OPEN_CARD_LIST_MENUS[i].active = 1;
                        animateWindowTo(&OPEN_SORT_MENU_WINDOWS[i].window, (Rect16 *)-1);
                    }
                } else if (PAD_STATES[i]->pressed & 0x100) {
                    open[i] = 1;
                    playMenuSound(3);
                    OPEN_CARD_LIST_MENUS[i].active = 0;
                    animateWindowTo(&OPEN_SORT_MENU_WINDOWS[i].window, &OPEN_SORT_MENUS[i].rect);
                }
            }
        }
        if (OPEN_TRADE_STATE == 2 && ((PAD_STATES[0]->pressed & 0x40) || (PAD_STATES[1]->pressed & 0x40))) {
            playMenuSound(1);
            PLAYER_DATA(0).unk28_11 = 1;
            PLAYER_DATA(1).unk28_11 = 1;
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 3; j++) {
                    card = OPEN_TRADE_PICKS[i][j];
                    if (card >= 0) {
                        OPEN_addCardCopy(i ^ 1, card, OPEN_removeCardCopy(i, card));
                    }
                }
            }
            OPEN_countCardsInDecks();
            for (i = 0; i < 2; i++) {
                OPEN_initTradeCardList(i);
                OPEN_CARD_LIST_MENUS[i].row = 0;
                centerMenuOnCursor(&OPEN_CARD_LIST_MENUS[i]);
            }
            OPEN_TRADE_STATE = 0;
            for (i = 0; i < 2; i++) {
                OPEN_CARD_LIST_MENUS[i].active = 1;
                OPEN_TRADE_PLAYER_READY[i] = 0;
                for (j = 0; j < 3; j++) {
                    OPEN_TRADE_PICKS[i][j] = -1;
                    OPEN_TRADE_PICK_X[i][j] = j * 44 + i * 166;
                }
            }
        }
        if (OPEN_TRADE_QUIT >= 2) {
            initDialog(dialog, OPEN_STR_QUIT_TRADING, 1);
            runDialogForPad((s32 *)dialog, OPEN_TRADE_QUIT - 2);
            switch ((s8)dialog[0xA5]) {
            case 1:
                OPEN_TRADE_QUIT = 1;
                break;
            case 0:
            case 2:
                OPEN_TRADE_QUIT = 0;
                break;
            }
        }
    } while (OPEN_TRADE_QUIT != 1);
    animateWindowTo(&OPEN_TRADE_OK_WINDOW, (Rect16 *)-1);
    animateWindowTo(&OPEN_TRADE_LIST_WINDOW, (Rect16 *)-1);
    for (i = 0; i < 2; i++) {
        animateWindowTo(&OPEN_SORT_MENU_WINDOWS[i].window, (Rect16 *)-1);
        animateWindowTo(&OPEN_CARD_LIST_WINDOWS[i].window, (Rect16 *)-1);
        animateWindowTo(&OPEN_CARD_INFO_WINDOWS[i].window, (Rect16 *)-1);
        animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[i].window, (Rect16 *)-1);
    }
    playMenuSound(4);
    OPEN_TRADE_BANNER_SHOWN = 0;
    func_80014C08(20);
    removeFrameCallback((s32)OPEN_drawTradeScreen);
    func_80014C08(2);
    freeHeapBlock(OPEN_CARD_IMAGE_ARC);
    func_80014A48(parentTask);
}

typedef struct {
    /* 0x00 */ s16 texX;
    /* 0x02 */ s16 texY;
    /* 0x04 */ s16 w;
    /* 0x06 */ s16 h;
    /* 0x08 */ s16 clutX;
    /* 0x0A */ s16 clutY;
    /* 0x0C */ u16 dx;
    /* 0x0E */ u16 dy;
    /* 0x10 */ u16 dw;
    /* 0x12 */ u16 dh;
    /* 0x14 */ u8 semiTrans;
    /* 0x15 */ u8 abr;
} TitlePart;
extern TitlePart OPEN_TITLE_PARTS[];
extern POLY_FT4 OPEN_TITLE_PART_PRIMS[2][40];

/* the title part's primitive; the tpage and clut stores index the array directly */
#define PRIM (&OPEN_TITLE_PART_PRIMS[FRAME_BUFFER_INDEX][OPEN_TITLE_PART_COUNT])

void OPEN_drawTitlePart(s32 x, s32 y, s32 part) {
    initPrimByType(0xC, PRIM, OPEN_TITLE_PARTS[part].semiTrans, 0);
    PRIM->r0 = 0x80;
    PRIM->g0 = 0x80;
    PRIM->b0 = 0x80;
    PRIM->x0 = x += OPEN_TITLE_PARTS[part].dx;
    PRIM->y0 = y += OPEN_TITLE_PARTS[part].dy;
    PRIM->x1 = x + OPEN_TITLE_PARTS[part].dw;
    PRIM->y1 = y;
    PRIM->x2 = x;
    PRIM->y2 = y + OPEN_TITLE_PARTS[part].dh;
    PRIM->x3 = x + OPEN_TITLE_PARTS[part].dw;
    PRIM->y3 = y + OPEN_TITLE_PARTS[part].dh;
    PRIM->u0 = OPEN_TITLE_PARTS[part].texX % 64 * 4;
    PRIM->v0 = OPEN_TITLE_PARTS[part].texY % 256;
    PRIM->u1 = OPEN_TITLE_PARTS[part].texX % 64 * 4 + (OPEN_TITLE_PARTS[part].w - 1);
    PRIM->v1 = OPEN_TITLE_PARTS[part].texY % 256;
    PRIM->u2 = OPEN_TITLE_PARTS[part].texX % 64 * 4;
    PRIM->v2 = OPEN_TITLE_PARTS[part].texY % 256 + (OPEN_TITLE_PARTS[part].h - 1);
    PRIM->u3 = OPEN_TITLE_PARTS[part].texX % 64 * 4 + (OPEN_TITLE_PARTS[part].w - 1);
    PRIM->v3 = OPEN_TITLE_PARTS[part].texY % 256 + (OPEN_TITLE_PARTS[part].h - 1);
    OPEN_TITLE_PART_PRIMS[FRAME_BUFFER_INDEX][OPEN_TITLE_PART_COUNT].tpage = getTPage(0, OPEN_TITLE_PARTS[part].abr, OPEN_TITLE_PARTS[part].texX, OPEN_TITLE_PARTS[part].texY);
    OPEN_TITLE_PART_PRIMS[FRAME_BUFFER_INDEX][OPEN_TITLE_PART_COUNT].clut = getClut(OPEN_TITLE_PARTS[part].clutX, OPEN_TITLE_PARTS[part].clutY);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFF6], PRIM);
    OPEN_TITLE_PART_COUNT++;
}

#undef PRIM

void OPEN_drawIntroImage(s32 x, s32 y, s32 z) {
    Rect16 rect; /* unused, but it sizes the frame */
    s32 next;
    s32 cur;
    s32 semiTrans;

    if (OPEN_INTRO_IMAGE != -1 || OPEN_INTRO_SHOWN_IMAGE != OPEN_INTRO_IMAGE) {
        if (OPEN_INTRO_IMAGE != OPEN_INTRO_SHOWN_IMAGE) {
            cur = OPEN_INTRO_SHOWN_IMAGE;
            next = OPEN_INTRO_IMAGE;
            OPEN_INTRO_IMAGE_FADE += 4;
            if (OPEN_INTRO_IMAGE_FADE > 0x80) {
                OPEN_INTRO_IMAGE_FADE = 0;
                OPEN_INTRO_SHOWN_IMAGE = next;
                cur = next;
                next = -1;
            }
        } else {
            cur = OPEN_INTRO_IMAGE;
            next = -1;
        }
        if (next != -1) {
            semiTrans = OPEN_INTRO_IMAGE_FADE != 0x80;
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = next % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, next + 0x1F1);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.g0 = OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.b0 = OPEN_INTRO_IMAGE_FADE;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 1, next / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = next % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, 0x1FF);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.g0 = OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.b0 = OPEN_INTRO_IMAGE_FADE;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 2, next / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
        if (cur != -1) {
            semiTrans = OPEN_INTRO_IMAGE_FADE != 0;
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = cur % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, cur + 0x1F1);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.g0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.b0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 1, cur / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = cur % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, 0x1FF);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.g0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            CUR_SPRT->sp.b0 = 0x80 - OPEN_INTRO_IMAGE_FADE;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 2, cur / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

void OPEN_drawPlayerName(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    drawText(x, y, PLAYER_PROFILES, 7, z);
}

typedef struct {
    /* 0x0 */ char *text;
    /* 0x4 */ u8 kind;
    /* 0x5 */ u8 next;
    /* 0x6 */ u8 alt;
    /* 0x7 */ s8 image;
} ScrollPage;
extern ScrollPage OPEN_INTRO_PAGES[];

void OPEN_drawIntroMessage(UiWindow *window) {
    char text[136];
    char *src;
    char *dst;
    s32 stop;
    s32 i;
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    if (OPEN_INTRO_TEXT.page != OPEN_INTRO_TEXT.shownPage) {
        OPEN_INTRO_TEXT.shownPage = OPEN_INTRO_TEXT.page;
        OPEN_INTRO_TEXT.length = 0;
    }
    OPEN_INTRO_TEXT.done = 0;
    for (i = 0; i < 133; i++) {
        text[i] = 0;
    }
    text[0] = '*';
    text[1] = 's';
    text[2] = '0';
    dst = &text[3];
    src = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].text;
    for (i = 0; i < OPEN_INTRO_TEXT.length; i++) {
        stop = 0;
        while (!stop) {
            if (*src == '*') {
                *dst++ = *src++;
                *dst++ = *src++;
            } else {
                *dst++ = *src++;
                stop = 1;
            }
            if (dst[-1] == 0) {
                OPEN_INTRO_TEXT.done = 1;
                break;
            }
        }
    }
    if (OPEN_INTRO_TEXT.done == 0) {
        OPEN_INTRO_TEXT.length++;
    }
    dst = text;
    drawText(x + 2, y + 1, (s32)dst, 7, z);
    if (OPEN_INTRO_TEXT.done != 0 && OPEN_INTRO_TEXT.waitInput != 0) {
        if (++OPEN_INTRO_TEXT.blink & 0x10) {
            drawIcon(x + 0x119, y + 0x1C, 0, 0x1B, z);
        }
    } else {
        OPEN_INTRO_TEXT.blink = 0;
    }
}

void OPEN_drawIntroImageWindow(UiWindow *window) {
    OPEN_drawIntroImage(window->originX, window->originY, window->z);
}

void OPEN_drawIntroScreen(void) {
    Rect16 rect;
    s32 i;

    if (OPEN_INTRO_IMAGE != OPEN_INTRO_SHOWN_IMAGE) {
        if (OPEN_INTRO_SHOWN_IMAGE == -1) {
            rect.x = 0x32;
            rect.y = 0x48;
            rect.w = 0xDC;
            rect.h = 0x52;
            animateWindowTo(&OPEN_IMAGE_WINDOW, &rect);
            playMenuSound(3);
            OPEN_INTRO_IMAGE_FADE = 0;
            OPEN_INTRO_SHOWN_IMAGE = OPEN_INTRO_IMAGE;
        } else if (OPEN_INTRO_IMAGE == -1) {
            if (OPEN_INTRO_IMAGE_FADE == 0) {
                animateWindowTo(&OPEN_IMAGE_WINDOW, (Rect16 *)-1);
                playMenuSound(4);
            } else if (OPEN_IMAGE_WINDOW.from.w == 0) {
                OPEN_INTRO_IMAGE_FADE = 0;
                OPEN_INTRO_SHOWN_IMAGE = -1;
            }
        }
    }
    drawWindow(&OPEN_PLAYER_NAME_WINDOW, OPEN_drawPlayerName, 0x19);
    drawWindow(&OPEN_MESSAGE_WINDOW, OPEN_drawIntroMessage, 0x19);
    drawWindow(&OPEN_IMAGE_WINDOW, OPEN_drawIntroImageWindow, 0x19);
    D_801F4F0C -= 16;
    if (D_801F4F0C < 16) {
        D_801F4F0C = 16;
    }
    D_801F4F10 += 8;
    if (D_801F4F10 > 0) {
        D_801F4F10 = 0;
    }
    D_801F4F04 += 4;
    if (D_801F4F04 > 28) {
        D_801F4F04 = 28;
    }
    D_801F4880 -= 12;
    if (D_801F4880 < 0xAA) {
        D_801F4880 = 0xAA;
    }
    D_801F4F1C -= 16;
    if (D_801F4F1C < 20) {
        D_801F4F1C = 20;
    }
    OPEN_TITLE_PART_COUNT = 0;
    OPEN_drawTitlePart(D_801F4F08, D_801F4F0C, 0);
    OPEN_drawTitlePart(D_801F4F08, D_801F4F0C, 1);
    OPEN_drawTitlePart(D_801F4F08, D_801F4F0C, 2);
    for (i = 0; i < OPEN_INTRO_TEXT.step; i++) {
        OPEN_drawTitlePart(i * 38 + D_801F4F00, D_801F4F04, 3);
    }
    OPEN_drawTitlePart(D_801F4F00, D_801F4F04, 4);
    OPEN_drawTitlePart(D_801F4F00, D_801F4F04, 5);
    OPEN_drawTitlePart(D_801F4F00 + 0x26, D_801F4F04, 5);
    OPEN_drawTitlePart(D_801F4F00 + 0x4C, D_801F4F04, 5);
    OPEN_drawTitlePart(D_801F4F00 + 0x72, D_801F4F04, 5);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 10);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 11);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 12);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 13);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 14);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 15);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 16);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 17);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 18);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 19);
    OPEN_drawTitlePart(D_801F4880, D_801F4884, 6);
    OPEN_drawTitlePart(D_801F4880, D_801F4884, 7);
    OPEN_drawTitlePart(D_801F4880, D_801F4884, 8);
    OPEN_drawTitlePart(D_801F4880, D_801F4884, 9);
    OPEN_drawTitlePart(D_801F4F10, D_801F4F14, 20);
    OPEN_drawTitlePart(D_801F4F10, D_801F4F14, 21);
}

void OPEN_startSceneTasks(void) {
    initScene3D(1);
    func_80014A00(0x19);
    func_800149B8(0x19, 0x1F, 0, 0x800, &runSceneCameraTask, 1);
    func_80014A00(0x1B);
    func_800149B8(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
}

void OPEN_stopSceneTasks(void) {
    func_80014A00(0x1B);
    func_80014A00(0x19);
    removeFrameCallback((s32)renderSceneModels);
    unloadAllModels();
    freeHeapBlocksByTag(0x7F);
}

void OPEN_showSceneModel(void) {
    ((Graphics *)&GRAPHICS)->snapCamera = 0;
    ((Graphics *)&GRAPHICS)->unk90 = 3000;
    ((Graphics *)&GRAPHICS)->unk92 = -((Model2220 *)SCENE_3D->models[0])->bonepos[0][1] * 3;
    SCENE_3D->modelState[0] = 1;
    applyAnimationFirstFrame(0, 0);
    startModelAnimation(0, 0, -2, 0);
}

void OPEN_unloadSceneModel(void) {
    freeHeapBlocksByTag(500);
    unloadModel(0);
    unloadModelAnimations(0);
}

/* the last three bytes are leftovers in the original, not zero padding */
const char OPEN_STR_QUIT_TRADING[32] = "Do you want to Quit Trading?\0\x10\x02\x02";

typedef struct {
    /* 0x00 */ Rect16 uv;
    /* 0x08 */ s32 duration;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 y;
} CardAnimFrame;

/* the pages of the introduction */
ScrollPage OPEN_INTRO_PAGES[33] = {
    { "Welcome to *c2Digital Card Battle*c7!", 0, 0x16, 0, -1 },
    { "Please proceed to *c4User Registration*c7.\nPlease enter your nickname\nfor this World.", 1, 2, 0, -1 },
    { "Please choose a Partner Card.\nYou can choose Veemon, Hawkmon,\nor Armadillomon.", 0, 3, 0, 0 },
    { "Each Partner Card comes with a matching\nStarter *c5Deck*c7, so you can\nbattle right from the start.", 0, 4, 0, 0 },
    { "Your Partner will grow with every battle\nand will become a reliable ally in \nthe battles you'll face.", 2, 5, 0, 1 },
    { "Now, please choose a Partner.\nUse the directional and *b2 buttons.", 0, 6, 0, -1 },
    { "This is a Veemon *c5Deck*c7,\nwith Fire and Darkness Digimon.\nIt's a strong offensive *c5Deck*c7.", 0, 9, 0, -1 },
    { "This is a Hawkmon *c5Deck*c7, with Nature\nand tricky Rare Digimon.\nIt's a well-balanced *c5Deck*c7.", 0, 9, 0, -1 },
    { "An Armadillomon *c5Deck*c7 has the\nstrong support of Rare Digimon.\nIt's a defensive *c5Deck*c7.", 0, 9, 0, -1 },
    { "Next is how you appear in this world.\nWe only have one character available.\nWe hope you like him.", 3, 0xA, 0, -1 },
    { "*c4User Registration*c7 is complete.\nWould you like to know more about\nthis world?", 4, 0xB, 0x20, -1 },
    { "In this Cyber World, you play Digimon\nDigital Card Battles. There are\nalways many opponents to play with.", 0, 0x10, 0, 2 },
    { "ERROR! (12)", 5, 0x15, 0x1B, 2 },
    { "ERROR! (13)", 0, 0xE, 0, -1 },
    { "ERROR! (14)", 6, 0x11, 0xF, -1 },
    { "ERROR! (15)", 0, 0x10, 0, -1 },
    { "Do you want to learn about the game?", 4, 0x1B, 0x20, -1 },
    { "ERROR! (17)", 0, 0x12, 0, -1 },
    { "ERROR! (18)", 0, 0x13, 0, -1 },
    { "ERROR! (19)", 7, 0x1A, 0x14, -1 },
    { "ERROR! (20)", 0, 0x10, 0, -1 },
    { "ERROR! (21)", 5, 0xD, 0x10, 2 },
    { "First, select the *c4Sound Settings*c7.\nThis can be changed during the game.", 8, 0x17, 0x16, -1 },
    { "Next, select *c4Polygon Battle Settings*c7.\nThis can also be changed during the game.", 9, 1, 0x17, -1 },
    { "Enjoy *c2Digital Card Battle*c7!", 0xB, 0x18, 0, -1 },
    { "Card data conversion had failed.\nDo you want to learn about this game?", 4, 0x1B, 0x20, -1 },
    { "ERROR! (26)", 4, 0x1B, 0x20, -1 },
    { "Then I'll quickly tell you about \n*c2Digital Card Battle*c7.", 0, 0x1C, 0, -1 },
    { "This is a fun world where a Player\ngets to play with Digimon and other\ncharacters, using Digimon Battle Cards.", 0, 0x1D, 0, 3 },
    { "Collect Cards through battles. Trade and\nFuse them to create your own Decks.", 0, 0x1E, 0, 4 },
    { "A *c5Deck *c7is a group of 30 Cards.\nYou'll use this to Battle Opponents.", 0, 0x1F, 0, 5 },
    { "Betamon in Beginner City will teach\nyou the basics. You'll learn the rest\nas you go along.", 0, 0x20, 0, 6 },
    { "Let's save your *c4Registration*c7.\nPlease insert a *c5MEMORY CARD*c7 with at\nleast 2 free blocks into MEMORY CARD slot 1.", 0xA, 0x18, 0, -1 },
};

/* not referenced by any code */
s16 D_801F0140[160] = {
    5, 8, 3, 9, 0xA, 0xE, 0x11, 0x12, 0x14, 0x13, 0x15, 0x17, 0x10, 0x18, 0x16, 0x1A,
    0x1E, 0x1F, 0x1B, 0x1D, 0x2A, 0x2B, 0x29, 0x2C, 0x2D, 0x36, 0x32, 0x31, 0x2E, 0x34, 0x37, 0x3B,
    0x3A, 0x38, 0x3C, 0x35, 0x34, 0x44, 0x42, 0x3F, 0x3E, 0x40, 0x4A, 0x8F, 0x50, 0x96, 0xB, 0x57,
    0x56, 0x52, 0x58, 0x55, 0x5A, 0x5B, 0x5D, 0x59, 0xA4, 0x60, 0x5C, 0x65, 0xA2, 0x5F, 0xA3, 0x5E,
    0x61, 0x64, -1, 0x66, 0x62, 0x6C, 0x8D, 0x6F, 0x71, 0x74, 0x70, 0x73, 0x78, 0x7A, 0x7B, 0x80,
    0x7D, 0x7C, 0x7E, 0x82, 0x81, 0x83, 0x7F, 0x89, 0x87, 0x8A, 0x88, 0x90, 0x91, 0x92, 0x93, 0x95,
    0x94, 0x9A, 0x9F, 0x9C, 0x9D, 0x9E, 0xA1, 0xA0, 0xAA, 0xAB, 0xA7, 0xA8, 0x51, 6, 0xC0, 0xDD,
    0xCB, 0xD4, 0xE2, 0xBF, 0xEB, 0xEC, 0xED, 0xEE, 0xE3, 0xE4, 0xE5, 0xCC, 0xCD, 0xE6, 0xEF, 0xF0,
    0xF5, 0xF6, 0x107, 0xFC, 0xFD, 0xFE, 0xFF, 0x100, 0x108, 0x10B, 0x10C, 0x10D, 0x10E, 0x10F, 0x110, 0x109,
    0x10A, -1, -1, -1, -1, -1, -1, -1, -1, 0x128, 0x127, 0x129, 0x12B, 0x12C, -1, 0,
};

/* the parts of the title screen */
TitlePart OPEN_TITLE_PARTS[22] = {
    { 0x340, 0xA0, 0x6C, 0x2E, 0x360, 0xD1, 9, 0xB, 0x6C, 0x2E, 1, 0 },
    { 0x340, 0, 0x80, 0x4A, 0x340, 0xCE, 0, 0, 0x80, 0x4A, 1, 1 },
    { 0x360, 0, 0x80, 0x4A, 0x340, 0xCF, 6, 6, 0x80, 0x4A, 1, 2 },
    { 0x35C, 0x83, 0x2C, 0x12, 0x340, 0xD1, -4, -3, 0x2C, 0x12, 1, 1 },
    { 0x350, 0x4A, 0x98, 0xC, 0x340, 0xD0, 0, 0, 0x98, 0xC, 1, 1 },
    { 0x350, 0x83, 0x30, 0x14, 0x340, 0xD2, 0, 0, 0x30, 0x14, 1, 2 },
    { 0x350, 0x6F, 0x78, 0x14, 0x350, 0xD0, 0, 0, 0x78, 0x13, 1, 0 },
    { 0x350, 0x56, 0x7C, 0x1A, 0x350, 0xD1, 0, 0, 0x7C, 0x19, 1, 2 },
    { 0x376, 0x4A, 4, 9, 0x350, 0xD3, 0, 0x10, 4, 9, 0, 0 },
    { 0x377, 0x4A, 4, 9, 0x350, 0xD4, 4, 0x10, 0x96, 9, 0, 0 },
    { 0x36E, 0x6F, 0x10, 0x10, 0x350, 0xCE, 0, 0, 0x10, 0x10, 0, 0 },
    { 0x372, 0x6F, 4, 8, 0x340, 0xD3, 0x10, 0, 0x104, 7, 0, 0 },
    { 0x373, 0x6F, 0x10, 0x10, 0x360, 0xCF, 0x114, 0, 0x10, 0x10, 0, 0 },
    { 0x36E, 0x7F, 9, 1, 0x350, 0xD5, 0, 0x10, 8, 0xF0, 0, 0 },
    { 0x36E, 0x7F, 9, 1, 0x350, 0xD5, 0x124, 0x10, -8, 0xF0, 0, 0 },
    { 0x36F, 0x56, 0x18, 0x18, 0x350, 0xCF, 4, 4, 0x18, 0x18, 1, 2 },
    { 0x375, 0x56, 4, 0xD, 0x340, 0xD4, 0x1C, 4, 0xF4, 0xC, 1, 2 },
    { 0x376, 0x56, 0x18, 0x18, 0x360, 0xD0, 0x110, 4, 0x18, 0x18, 1, 2 },
    { 0x36F, 0x6E, 0xD, 1, 0x360, 0xCE, 4, 0x1C, 0xC, 0xF0, 1, 2 },
    { 0x36F, 0x6E, 0xD, 1, 0x360, 0xCE, 0x128, 0x1C, -0xC, 0xF0, 1, 2 },
    { 0x340, 0x4A, 0x40, 0x56, 0x340, 0xD5, 0x1A, 0x5A, 0x40, 0x56, 1, 0 },
    { 0x350, 0x82, 0x60, 1, 0x350, 0xD2, 0, 0, 0x60, 0xF0, 0, 0 },
};

/* the letters of the name entry, ten per row */
u8 OPEN_NAME_ENTRY_LETTERS[] = "ABCDEabcde"
                  "FGHIJfghij"
                  "KLMNOklmno"
                  "PQRSTpqrst"
                  "UVWXYuvwxy"
                  "Z-   z    "
                  "          "
                  "          "
                  "0123456789";

/* not referenced by any code */
u8 D_801F04BF = 0xC;

/* the partner cards: Veemon, Hawkmon and Armadillomon */
u8 OPEN_PARTNER_CARDS[3] = { 0xAF, 0xB6, 0xBE };

/* the cards of each starter deck: two choices for each of five slots */
s16 OPEN_STARTER_BONUS_CARDS[30] = {
    0xB, 0x74, 0x19, 0x83, 0x1C, 0x89, 0x1F, 0x8A, 0xF9, 0x102,
    0x50, 0x96, 0x60, 0xA4, 0x62, 0xA6, 0x64, 0xAB, 0xFA, 0x109,
    0x2D, 0x95, 0x3C, 0xA0, 0x41, 0xA7, 0x44, 0xA9, 0xFB, 0x103,
};

s32 OPEN_TITLE_OPTION_DIMMED[3] = { 1, 0, 1 };

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

typedef struct {
    /* 0x00 */ u8 unk0[0x98];
    /* 0x98 */ char *options[2];
    /* 0xA0 */ u8 unkA0[5];
    /* 0xA5 */ s8 choice;
    /* 0xA6 */ u8 unkA6[0xF];
    /* 0xB5 */ s8 unkB5;
    /* 0xB6 */ u8 unkB6[2];
} ChoiceDialog;
extern u16 OPEN_WHITE_CLUT[256];
extern s16 D_80079584;
s32 loadDigimonModelPak(s32 slot, s32 id, s8 format, s32 loadAllAnims);
void func_80055730(void);
void OPEN_runNameEntry(char *name, s32 parentTask);
void OPEN_runStarterSelect(s32 parentTask);
void OPEN_createNewSave(void);

void OPEN_runUserRegistration(s32 parentTask) {
    /* their own copies: the MESSAGE and PLAYER NAME literals would be merged
     * with later functions'; the first two keep the original order */
    static const char arcPath[] = "B:\\OPENING.ARC";
    static const char imageLabel[] = "IMAGE";
    static const char messageLabel[] = "MESSAGE";
    static const char nameLabel[] = "PLAYER NAME";
    Rect16 rect;
    ChoiceDialog dialog;
    u32 *arc;
    s32 i;
    s32 start;
    s32 model;
    s32 done;

    loadMusicTrack(0, 0x6E, 0x7F);
    playLoadedMusic(0);
    i = 0;
    func_800149B8(0, -1, 0, 0x800, loadFile, arcPath, getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    for (i = 0; i < 256; i++) {
        OPEN_WHITE_CLUT[i] = 0xFFFF;
    }
    rect.x = 0;
    rect.y = 0x1FF;
    rect.w = 0x100;
    rect.h = 1;
    LoadImage((s16 *)&rect, (s32)OPEN_WHITE_CLUT);
    rect.x = 0x32;
    rect.y = 0x48;
    rect.w = 0xDC;
    rect.h = 0x52;
    openWindow(&OPEN_IMAGE_WINDOW, &rect, -1, (s16 *)-1, 8, 0x81, 0x80, 0xC);
    OPEN_IMAGE_WINDOW.label = (s32)imageLabel;
    OPEN_IMAGE_WINDOW.labelPalette = 7;
    animateWindowTo(&OPEN_IMAGE_WINDOW, (Rect16 *)-1);
    rect.x = 0xC;
    rect.y = 0xBC;
    rect.w = 0x128;
    rect.h = 0x2A;
    openWindow(&OPEN_MESSAGE_WINDOW, &rect, -1, (s16 *)-1, 8, 0x51, 0x80, 0xC);
    OPEN_MESSAGE_WINDOW.labelPalette = 8;
    OPEN_MESSAGE_WINDOW.label = (s32)messageLabel;
    rect.x = 0x18;
    rect.y = 0x18;
    rect.w = 0x48;
    rect.h = 0xC;
    openWindow(&OPEN_PLAYER_NAME_WINDOW, &rect, -1, (s16 *)-1, 8, 0x11, 0x80, 0xC);
    OPEN_PLAYER_NAME_WINDOW.label = (s32)nameLabel;
    animateWindowTo(&OPEN_PLAYER_NAME_WINDOW, (Rect16 *)-1);
    OPEN_INTRO_IMAGE = -1;
    OPEN_INTRO_SHOWN_IMAGE = -1;
    OPEN_INTRO_IMAGE_FADE = 0;
    D_801F4F08 = 0xC;
    D_801F4F0C = 0xF0;
    D_801F4F10 = -0x60;
    D_801F4F14 = 0;
    D_801F4F00 = 0x90;
    D_801F4F04 = -0x14;
    D_801F4880 = 0x140;
    D_801F4884 = 0x32;
    D_801F4F18 = 0xE;
    D_801F4F1C = 0xF0;
    OPEN_INTRO_TEXT.step = 0;
    OPEN_INTRO_TEXT.page = 0;
    OPEN_INTRO_TEXT.shownPage = 0;
    OPEN_INTRO_TEXT.length = 0;
    OPEN_INTRO_TEXT.waitInput = 1;
    OPEN_INTRO_TEXT.unk18 = 0;
    done = 0;
    OPEN_startSceneTasks();
    loadDigimonModelPak(0, 0xEB, 0, 1);
    D_80079584 = 0;
    addFrameCallback((s32)OPEN_drawIntroScreen);
    playMenuSound(3);
    model = 0;
    while (!done) {
        func_80014C08(FRAME_INTERVAL);
        OPEN_INTRO_IMAGE = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].image;
        if ((PAD_STATES[0]->pressed & 0x40) && OPEN_INTRO_TEXT.done != 0) {
            switch (OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].kind) {
            /* the dialog pages: the switch below handles them */
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 1:
                animateWindowTo(&OPEN_MESSAGE_WINDOW, (Rect16 *)-1);
                OPEN_INTRO_TEXT.step = 1;
                func_800149B8(0, -1, 0, 0x400, OPEN_runNameEntry, PLAYER_PROFILES, getCurrentTaskId(), 0, 0);
                func_80014C08(0x7FFFFFFF);
                rect.x = 0xC;
                rect.y = 0xBC;
                rect.w = 0x128;
                rect.h = 0x2A;
                animateWindowTo(&OPEN_MESSAGE_WINDOW, &rect);
                rect.x = 0x18;
                rect.y = 0x18;
                rect.w = 0x48;
                rect.h = 0xC;
                animateWindowTo(&OPEN_PLAYER_NAME_WINDOW, &rect);
                OPEN_INTRO_TEXT.step = 2;
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                break;
            case 2:
                OPEN_INTRO_TEXT.step = 3;
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                OPEN_INTRO_IMAGE = -1;
                OPEN_INTRO_TEXT.waitInput = 0;
                func_800149B8(0, -1, 0, 0x300, OPEN_runStarterSelect, getCurrentTaskId(), 0, 0, 0);
                func_80014C08(0x7FFFFFFF);
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                OPEN_showSceneModel();
                model = 1;
                OPEN_INTRO_TEXT.step = 4;
                start = ((PlayerProfile *)PLAYER_PROFILES)->playTime;
                do {
                    func_80014C08(FRAME_INTERVAL);
                } while (((PlayerProfile *)PLAYER_PROFILES)->playTime - start < 400);
                OPEN_INTRO_TEXT.waitInput = 1;
                break;
            case 0:
            case 3:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                playMenuSound(1);
                break;
            case 10:
                animateWindowTo(&OPEN_MESSAGE_WINDOW, (Rect16 *)-1);
                playMenuSound(4);
                func_80014C08(16);
                OPEN_createNewSave();
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                rect.x = 0xC;
                rect.y = 0xBC;
                rect.w = 0x128;
                rect.h = 0x2A;
                animateWindowTo(&OPEN_MESSAGE_WINDOW, &rect);
                playMenuSound(3);
                break;
            case 11:
                done = 1;
                break;
            }
        }
        switch (OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].kind) {
        case 4:
            dialog.options[0] = "Yes";
            dialog.options[1] = "No";
            initDialog((u8 *)&dialog, NULL, 2);
            dialog.choice = 1;
            OPEN_INTRO_TEXT.waitInput = 0;
            dialog.unkB5 = 1;
            runDialog(&dialog);
            OPEN_INTRO_TEXT.waitInput = 1;
            if (model) {
                startModelAnimation(0, 1, -2, 0);
                OPEN_INTRO_TEXT.waitInput = 0;
                func_80014C08(0x98);
                OPEN_INTRO_TEXT.waitInput = 1;
                OPEN_unloadSceneModel();
                OPEN_stopSceneTasks();
                model = 0;
            }
            switch (dialog.choice) {
            case 0:
            case 2:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].alt;
                break;
            case 1:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                break;
            }
            break;
        case 5:
            dialog.options[0] = "\x82\xa0\x82\xe9";
            dialog.options[1] = "\x82\xc8\x82\xa2";
            initDialog((u8 *)&dialog, NULL, 2);
            dialog.choice = 1;
            OPEN_INTRO_TEXT.waitInput = 0;
            dialog.unkB5 = 1;
            runDialog(&dialog);
            OPEN_INTRO_TEXT.waitInput = 1;
            switch (dialog.choice) {
            case 0:
            case 2:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].alt;
                break;
            case 1:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].alt;
                break;
            }
            break;
        case 8:
            dialog.options[0] = "Stereo";
            dialog.options[1] = "Mono";
            initDialog((u8 *)&dialog, "Sound Settings", 2);
            dialog.choice = 1;
            OPEN_INTRO_TEXT.waitInput = 0;
            dialog.unkB5 = 1;
            runDialog(&dialog);
            OPEN_INTRO_TEXT.waitInput = 1;
            switch (dialog.choice) {
            case 2:
                PLAYER_DATA(0).unk20_0 = 1;
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                func_80055730();
                break;
            case 1:
                PLAYER_DATA(0).unk20_0 = 0;
                func_80055740();
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                break;
            case 0:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].alt;
                break;
            }
            break;
        case 9:
            dialog.options[0] = "On";
            dialog.options[1] = "Off";
            initDialog((u8 *)&dialog, "Polygon Battle", 2);
            dialog.choice = 1;
            OPEN_INTRO_TEXT.waitInput = 0;
            dialog.unkB5 = 1;
            runDialog(&dialog);
            OPEN_INTRO_TEXT.waitInput = 1;
            switch (dialog.choice) {
            case 2:
                PLAYER_DATA(0).skipBattleAnimation = 1;
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                break;
            case 1:
                PLAYER_DATA(0).skipBattleAnimation = 0;
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].next;
                break;
            case 0:
                OPEN_INTRO_TEXT.page = OPEN_INTRO_PAGES[OPEN_INTRO_TEXT.page].alt;
                break;
            }
            break;
        }
    }
    animateWindowTo(&OPEN_MESSAGE_WINDOW, (Rect16 *)-1);
    playMenuSound(4);
    func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 2, 8, 0);
    func_80014C08(20);
    removeFrameCallback((s32)OPEN_drawIntroScreen);
    hideScrollingBackground();
    stopScreenFade();
    func_80014C08(10);
    func_80014A48(parentTask, -1);
}

/* moves a keyboard row by d, wrapping inside its page of 9 rows */
#define WRAP_ROW(row, d) (((s16)((row) + (d)) - (d)) / 9 * 9 + ((s16)((row) + (d)) + 9) % 9)

/* declared s32 although nothing is returned: v0 stays live at the exits */
s32 OPEN_moveNameEntryCursor(void) {
    UiWindow *window;
    Rect16 rect;

    window = &OPEN_NAME_ENTRY_WINDOW;
    if (OPEN_NAME_ENTRY.inputEnabled != 0) {
        if (OPEN_NAME_ENTRY.onSideMenu == 0) {
            if ((u16)PAD_STATES[0]->repeat & 0xF000) {
                playMenuSound(2);
            }
            do {
                if (PAD_STATES[0]->repeat & 0x1000) {
                    OPEN_NAME_ENTRY.row = WRAP_ROW(OPEN_NAME_ENTRY.row, -1);
                    if (OPEN_NAME_ENTRY.row % 9 == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & 0x4000) {
                    OPEN_NAME_ENTRY.row = WRAP_ROW(OPEN_NAME_ENTRY.row, 1);
                    if (OPEN_NAME_ENTRY.row % 9 == 8) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[0]->repeat & 0x8000) {
                    if (--OPEN_NAME_ENTRY.col < 0) {
                        OPEN_NAME_ENTRY.col = 9;
                        OPEN_NAME_ENTRY.onSideMenu = 1;
                    } else if (OPEN_NAME_ENTRY.col == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & 0x2000) {
                    if (++OPEN_NAME_ENTRY.col >= 10) {
                        OPEN_NAME_ENTRY.col = 0;
                        OPEN_NAME_ENTRY.onSideMenu = 1;
                    } else if (OPEN_NAME_ENTRY.col == 9) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                }
            } while (OPEN_NAME_ENTRY_LETTERS[OPEN_NAME_ENTRY.row * 10 + OPEN_NAME_ENTRY.col] == ' ');
        } else {
            if ((u16)PAD_STATES[0]->pressed & 0xA000) {
                playMenuSound(2);
            }
            if ((u16)PAD_STATES[0]->pressed & 0x8000) {
                OPEN_NAME_ENTRY.onSideMenu = 0;
                OPEN_NAME_ENTRY.prevSideRow = -1;
                OPEN_NAME_ENTRY.prevCol = -1;
                OPEN_NAME_ENTRY.prevRow = -1;
                OPEN_NAME_ENTRY.col = 9;
                while (OPEN_NAME_ENTRY_LETTERS[OPEN_NAME_ENTRY.row * 10 + OPEN_NAME_ENTRY.col] == ' ') {
                    OPEN_NAME_ENTRY.col--;
                }
            } else if (PAD_STATES[0]->pressed & 0x2000) {
                OPEN_NAME_ENTRY.onSideMenu = 0;
                OPEN_NAME_ENTRY.prevSideRow = -1;
                OPEN_NAME_ENTRY.prevCol = -1;
                OPEN_NAME_ENTRY.prevRow = -1;
                OPEN_NAME_ENTRY.col = 0;
                while (OPEN_NAME_ENTRY_LETTERS[OPEN_NAME_ENTRY.row * 10 + OPEN_NAME_ENTRY.col] == ' ') {
                    OPEN_NAME_ENTRY.col++;
                }
            }
        }
    }
    if (OPEN_NAME_ENTRY.onSideMenu == 0) {
        if (OPEN_NAME_ENTRY.row != OPEN_NAME_ENTRY.prevRow || OPEN_NAME_ENTRY.col != OPEN_NAME_ENTRY.prevCol) {
            OPEN_NAME_ENTRY.prevCol = OPEN_NAME_ENTRY.col;
            OPEN_NAME_ENTRY.prevRow = OPEN_NAME_ENTRY.row;
            rect.x = window->rect.x - window->scroll[2] + OPEN_NAME_ENTRY.col * 17 + 4;
            rect.y = window->rect.y - window->scroll[3] + OPEN_NAME_ENTRY.row * 14 + 1;
            rect.w = 12;
            rect.h = 12;
            if (OPEN_NAME_ENTRY.col >= 5) {
                rect.x = window->rect.x - window->scroll[2] + OPEN_NAME_ENTRY.col * 17 + 15;
            }
            moveCursorHighlight(&OPEN_NAME_ENTRY_CURSOR, &rect);
        }
    } else if (OPEN_NAME_ENTRY.sideRow != OPEN_NAME_ENTRY.prevSideRow) {
        OPEN_NAME_ENTRY.prevSideRow = OPEN_NAME_ENTRY.sideRow;
        switch (OPEN_NAME_ENTRY.sideRow) {
        case 0:
        case 1:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + OPEN_NAME_ENTRY.sideRow * 14 + 1;
            rect.w = 0x30;
            rect.h = 12;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + OPEN_NAME_ENTRY.sideRow * 14 + 1;
            rect.w = 0x24;
            rect.h = 12;
            break;
        case 7:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + 0x63;
            rect.w = 0x18;
            rect.h = 12;
            break;
        case 8:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + 0x71;
            rect.w = 0x30;
            rect.h = 12;
            break;
        }
        moveCursorHighlight(&OPEN_NAME_ENTRY_CURSOR, &rect);
    }
}

#undef WRAP_ROW


void OPEN_drawNameEntryKeyboard(UiWindow *window) {
    char text[64];
    s8 rows[7];
    s32 i;
    s32 pad;
    s32 col;
    s32 item;
    s32 x;
    s32 y;
    s32 z;

    x = window->originX + 4;
    y = window->originY + 1;
    z = window->z;
    for (i = 0; i < 90; i++) {
        sprintf(text, "%c", OPEN_NAME_ENTRY_LETTERS[i]);
        pad = 0;
        if (i % 10 >= 5) {
            pad = 11;
        }
        col = i % 10 * 17 + 4;
        drawText(x + col + pad, y + i / 10 * 14, (s32)text, 7, z);
    }
    for (i = 0; i < 7; i++) {
        rows[i] = 4;
    }
    rows[window->view.y / window->rect.h] = 5;
    drawText(window->rect.x + 0xD4, window->rect.y + 0x63, (s32)"OK", 6, z);
    OPEN_moveNameEntryCursor();
    if (OPEN_NAME_ENTRY.onSideMenu == 0) {
        if (PAD_STATES[0]->pressed & 0x40) {
            OPEN_NAME_ENTRY.name[OPEN_NAME_ENTRY.cursor] = OPEN_NAME_ENTRY_LETTERS[OPEN_NAME_ENTRY.row * 10 + OPEN_NAME_ENTRY.col];
            playMenuSound(1);
            if (OPEN_NAME_ENTRY.cursor == 0 && OPEN_NAME_ENTRY.replaceName == 1) {
                for (i = 1; i < 13; i++) {
                    OPEN_NAME_ENTRY.name[i] = 0;
                }
            }
            if (OPEN_NAME_ENTRY.cursor < 11) {
                OPEN_NAME_ENTRY.cursor++;
            } else {
                OPEN_NAME_ENTRY.onSideMenu = 1;
                OPEN_NAME_ENTRY.sideRow = 7;
            }
            OPEN_NAME_ENTRY.replaceName = 0;
        } else if (PAD_STATES[0]->pressed & 0x20) {
            for (i = 11; i >= OPEN_NAME_ENTRY.cursor + 1; i--) {
                OPEN_NAME_ENTRY.name[i] = OPEN_NAME_ENTRY.name[i - 1];
            }
            OPEN_NAME_ENTRY.name[OPEN_NAME_ENTRY.cursor] = OPEN_NAME_ENTRY_LETTERS[OPEN_NAME_ENTRY.row * 10 + OPEN_NAME_ENTRY.col];
            playMenuSound(1);
            if (OPEN_NAME_ENTRY.cursor < 11) {
                OPEN_NAME_ENTRY.cursor++;
            } else {
                OPEN_NAME_ENTRY.onSideMenu = 1;
                OPEN_NAME_ENTRY.sideRow = 7;
            }
            OPEN_NAME_ENTRY.replaceName = 0;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            if (OPEN_NAME_ENTRY.name[0] != 0) {
                playMenuSound(1);
            }
            if (OPEN_NAME_ENTRY.cursor == 0) {
                OPEN_NAME_ENTRY.cursor++;
            }
            for (i = OPEN_NAME_ENTRY.cursor; i < 13; i++) {
                OPEN_NAME_ENTRY.name[i - 1] = OPEN_NAME_ENTRY.name[i];
            }
            OPEN_NAME_ENTRY.cursor--;
            OPEN_NAME_ENTRY.replaceName = 0;
        }
    } else if (PAD_STATES[0]->pressed & 0x40) {
        playMenuSound(1);
        item = OPEN_NAME_ENTRY.sideRow;
        if (item >= 0) {
            if (item < 7) {
                OPEN_NAME_ENTRY.row = item * 9;
                OPEN_NAME_ENTRY.col = 0;
                scrollWindowTo((s16 *)window, 0, OPEN_NAME_ENTRY.row * 14);
            } else if (item < 9) {
                OPEN_NAME_ENTRY.state = OPEN_NAME_ENTRY.sideRow;
            }
        }
    } else if (PAD_STATES[0]->pressed & 0x10) {
        if (OPEN_NAME_ENTRY.name[0] != 0) {
            playMenuSound(1);
        }
        if (OPEN_NAME_ENTRY.cursor == 0) {
            OPEN_NAME_ENTRY.cursor++;
        }
        for (i = OPEN_NAME_ENTRY.cursor; i < 13; i++) {
            OPEN_NAME_ENTRY.name[i - 1] = OPEN_NAME_ENTRY.name[i];
        }
        OPEN_NAME_ENTRY.cursor--;
        OPEN_NAME_ENTRY.replaceName = 0;
    }
    if (PAD_STATES[0]->pressed & 0x800) {
        if (OPEN_NAME_ENTRY.onSideMenu != 1 || OPEN_NAME_ENTRY.sideRow != 7) {
            playMenuSound(1);
            OPEN_NAME_ENTRY.onSideMenu = 1;
            OPEN_NAME_ENTRY.sideRow = 7;
        }
    }
    drawCursorHighlight(&OPEN_NAME_ENTRY_CURSOR, z);
}

void OPEN_drawNameEntryName(UiWindow *window) {
    Rect16 target;
    char text[64];
    s32 x;
    s32 y;
    s32 z;

    x = window->originX + 1;
    y = window->originY;
    z = window->z;
    sprintf(text, "*s0%s", OPEN_NAME_ENTRY.name);
    drawText(x, y, (s32)text, 7, z);
    if (PAD_STATES[0]->repeat & 4) {
        if (OPEN_NAME_ENTRY.cursor != 0) {
            playMenuSound(2);
            OPEN_NAME_ENTRY.cursor--;
        }
    } else if (PAD_STATES[0]->repeat & 8) {
        if (OPEN_NAME_ENTRY.cursor != 11 && OPEN_NAME_ENTRY.name[OPEN_NAME_ENTRY.cursor] != 0) {
            playMenuSound(2);
            OPEN_NAME_ENTRY.cursor++;
        }
    }
    target.x = x + OPEN_NAME_ENTRY.cursor * 6;
    target.y = y + 13;
    target.w = 6;
    target.h = 0;
    moveCursorHighlight(&OPEN_NAME_CARET, &target);
    drawCursorHighlight(&OPEN_NAME_CARET, z);
}

void OPEN_drawNameEntryHelp(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;
    Rect16 rect; /* unused, but it sizes the frame */

    x = window->originX + 1;
    y = window->originY + 1;
    z = window->z;
    drawText(x, y, (s32)"*b1 Delete", 7, z);
    drawText(x + 8, y + 13, (s32)"*b2 OK", 7, z);
    drawText(x, y + 26, (s32)"*b0 Insert", 7, z);
}

void OPEN_drawNameEntry(void) {
    drawWindow(&OPEN_NAME_ENTRY_WINDOW, OPEN_drawNameEntryKeyboard, 1);
    drawWindow(&OPEN_NAME_WINDOW, OPEN_drawNameEntryName, 1);
    drawWindow(&OPEN_NAME_HELP_WINDOW, OPEN_drawNameEntryHelp, 1);
}

/* "Is this name OK?": the string is followed by leftover bytes in the ROM, so it stays as data */
extern const char OPEN_STR_IS_THIS_NAME_OK[];
void OPEN_runNameEntry(char *name, s32 parentTask) {
    Rect16 cursor;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];

    strcpy(OPEN_NAME_ENTRY.name, name);
    OPEN_NAME_ENTRY.col = 0;
    OPEN_NAME_ENTRY.row = 0;
    OPEN_NAME_ENTRY.prevCol = -1;
    OPEN_NAME_ENTRY.prevRow = -1;
    OPEN_NAME_ENTRY.inputEnabled = 1;
    OPEN_NAME_ENTRY.cursor = 0;
    OPEN_NAME_ENTRY.onSideMenu = 0;
    OPEN_NAME_ENTRY.sideRow = 7;
    OPEN_NAME_ENTRY.prevSideRow = -1;
    OPEN_NAME_ENTRY.state = 0;
    OPEN_NAME_ENTRY.replaceName = 1;
    rect.x = 0x18;
    rect.y = 0x5E;
    rect.w = 0x110;
    rect.h = 0x7E;
    view.x = 0;
    view.y = 0;
    view.w = 0x10E;
    view.h = 0x7E;
    openWindow(&OPEN_NAME_ENTRY_WINDOW, &rect, -1, (s16 *)&view, 10, 0x81, 0x80, 0xC);
    OPEN_NAME_ENTRY_WINDOW.label = (s32)"NAME ENTRY";
    OPEN_NAME_ENTRY_WINDOW.labelPalette = 7;
    cursor.x = OPEN_NAME_ENTRY_WINDOW.originX + 4;
    cursor.y = OPEN_NAME_ENTRY_WINDOW.originY + 1;
    cursor.w = 12;
    cursor.h = 12;
    initCursorHighlight(&OPEN_NAME_ENTRY_CURSOR, &cursor, (Bytes4 *)-1);
    rect.x = 0x1E;
    rect.y = 0x1C;
    rect.w = 0x36;
    rect.h = 0x28;
    openWindow(&OPEN_NAME_HELP_WINDOW, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 0xC);
    OPEN_NAME_HELP_WINDOW.label = (s32)"HELP";
    rect.x = 0xDE;
    rect.y = 0x1C;
    rect.w = 0x4A;
    rect.h = 0xE;
    openWindow(&OPEN_NAME_WINDOW, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 0xC);
    OPEN_NAME_WINDOW.label = (s32)"PLAYER NAME";
    cursor.x = OPEN_NAME_WINDOW.originX;
    cursor.y = OPEN_NAME_WINDOW.originY + 13;
    cursor.w = 12;
    cursor.h = 0;
    /* passes rect, not the cursor rect it just filled in */
    initCursorHighlight(&OPEN_NAME_CARET, &rect, (Bytes4 *)-1);
    playMenuSound(3);
    addFrameCallback((s32)OPEN_drawNameEntry);
    while (1) {
        func_80014C08(FRAME_INTERVAL);
        if (OPEN_NAME_ENTRY.state == 0) {
            continue;
        }
        if (OPEN_NAME_ENTRY.name[0] == 0) {
            initDialog(dialog, "A name has not been entered!", 0);
            runDialog(dialog);
            OPEN_NAME_ENTRY.state = 0;
        } else {
            initDialog(dialog, OPEN_STR_IS_THIS_NAME_OK, 1);
            runDialog(dialog);
            switch ((s8)dialog[0xA5]) {
            case 1:
                break;
            case 0:
            case 2:
                OPEN_NAME_ENTRY.state = 0;
                break;
            }
        }
        if (OPEN_NAME_ENTRY.state != 0) {
            break;
        }
    }
    if (OPEN_NAME_ENTRY.state == 7) {
        strcpy(name, OPEN_NAME_ENTRY.name);
    }
    playMenuSound(4);
    animateWindowTo(&OPEN_NAME_HELP_WINDOW, (Rect16 *)-1);
    animateWindowTo(&OPEN_NAME_ENTRY_WINDOW, (Rect16 *)-1);
    animateWindowTo(&OPEN_NAME_WINDOW, (Rect16 *)-1);
    func_80014C08(20);
    removeFrameCallback((s32)OPEN_drawNameEntry);
    func_80014A48(parentTask);
}

/* the last three bytes are leftovers in the original, not zero padding */
const char OPEN_STR_IS_THIS_NAME_OK[20] = "Is this name OK?\0\x18\x62\0";

void OPEN_giveStarterDeck(s32 deck) {
    u8 *file;
    DeckFile *decks;
    s32 i;
    u16 card;

    func_800149B8(0, -1, 0, 0x800, loadFileTagged, "B:\\DECK2.DEK", getCurrentTaskId(), -2);
    file = (u8 *)func_80014C08(0x7FFFFFFF);
    decks = (DeckFile *)(file + 8);
    obtainPartner(0, deck);
    for (i = 0; i < 30; i++) {
        card = (&decks->starters[deck])->cards[i];
        setCardSlotFromId((u8 *)&PLAYER_DATA(0).savedDecks[0].cards[i], card);
        if (findPartnerSlot(0, card) == -1) {
            addCardToCollection(0, card, 1);
        }
    }
    strcpy((char *)PLAYER_DATA(0).savedDecks[0].unk1, decks->starters[deck].name);
    storeSavedDeck(0, &PLAYER_DATA(0).savedDecks[0], 0);
    PLAYER_DATA(0).opponentDeckFlags[deck + 0x8E] |= 0x8000;
    for (i = 0; i < 5; i++) {
        addCardToCollection(0, OPEN_STARTER_BONUS_CARDS[deck * 10 + i * 2 + rand() % 2], 1);
    }
    PLAYER_DATA(0).unk56 = deck;
    changeScrollingBackground(PLAYER_DATA(0).unk56, 0x380, 0, 0x380, 0x80);
    func_801EBA74(deck);
    freeHeapBlock(file);
}

void OPEN_drawCardPortrait(s32 x, s32 y, s32 texX, s32 texY, s32 palette, u8 *rgb, s32 otIndex) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x + 2;
        CUR_SPRT->sp.y0 = y + 5;
        CUR_SPRT->sp.u0 = (texX % 64) * 2 + 2;
        CUR_SPRT->sp.v0 = texY % 256 + 2;
        CUR_SPRT->sp.clut = getClut(0x2C0, palette + 0xF9);
        CUR_SPRT->sp.w = 36;
        CUR_SPRT->sp.h = 36;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, texX, texY));
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0x80;
            CUR_SPRT->sp.clut = getClut(0x2C0, palette + 0xB1);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0xB);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

extern u8 *CROSS_EFFECT_SHORT_NAMES[];

void OPEN_drawStarterSelect(UiWindow *window) {
    Rect16 rect;
    char text[16];
    u8 rgb[3];
    s32 i;
    s32 card;
    s32 x;
    s32 y;
    s32 z;
    s32 j;
    s32 shade;
    s32 tx;

    z = window->z;
    y = window->originY + 1;
    for (i = 0; i < 3; i++) {
        card = OPEN_PARTNER_CARDS[i];
        shade = OPEN_STARTER_SELECT.deck;
        if (shade == i) {
            shade = 0x80;
        } else {
            shade = 0x40;
        }
        rgb[0] = shade;
        rgb[1] = shade;
        rgb[2] = shade;
        x = window->originX;
        tx = x + 4;
        OPEN_drawCardPortrait(tx, y + 4, i * 20 + 0x2C0, 0, getCardSpecialty(card), rgb, z);
        tx = x + 0x30;
        drawTextColored(tx, y + 1, ((DigimonCardData *)DIGIMON_CARDS)[card].name, rgb, 7, z);
        drawIconColored(tx, y + 13, 0, 7, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].attack[0].power);
        drawTextColored(x + 0x3E, y + 13, text, rgb, 7, z);
        drawIconColored(tx, y + 0x19, 0, 8, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].attack[1].power);
        drawTextColored(x + 0x3E, y + 0x19, text, rgb, 7, z);
        drawIconColored(tx, y + 0x25, 0, 9, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].attack[2].power);
        drawTextColored(x + 0x3E, y + 0x25, text, rgb, 7, z);
        sprintf(text, "(%s)", CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)DIGIMON_CARDS)[card].crossEffect]);
        drawLargeTextColored(tx, y + 0x31, text, 7, rgb, z);
        tx = x + 0x5C;
        drawIconColored(tx, y + 0x19, 0, 0x19, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].dpBonus);
        drawTextColored(x + 0x68, y + 0x19, text, rgb, 7, z);
        drawIconColored(tx, y + 0x25, 0, 0x1A, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].hp);
        drawTextColored(x + 0x6A, y + 0x25, text, rgb, 7, z);
        tx = x + 0x88;
        drawTextColored(tx, y, "Support Effect", rgb, 6, z);
        for (j = 0; j < 4; j++) {
            drawTextColored(tx, y + 15 + j * 12, ((DigimonCardData *)DIGIMON_CARDS)[card].supportText[j], rgb, 7, z);
        }
        rect.x = tx;
        rect.y = y + 15;
        rect.w = 0x6E;
        rect.h = 0x30;
        drawWindowFrame(&rect, 0x31, 0, 0x80, 1, z);
        y += 0x48;
    }
    if (OPEN_INTRO_TEXT.page != 5) {
        if (PAD_STATES[0]->repeat & 0x1000) {
            if (OPEN_STARTER_SELECT.deck != 0) {
                playMenuSound(2);
                OPEN_STARTER_SELECT.deck--;
                scrollWindowTo((s16 *)window, 0, OPEN_STARTER_SELECT.deck * 0x48);
            }
        }
        if (PAD_STATES[0]->repeat & 0x4000) {
            if (OPEN_STARTER_SELECT.deck < 2) {
                playMenuSound(2);
                OPEN_STARTER_SELECT.deck++;
                scrollWindowTo((s16 *)window, 0, OPEN_STARTER_SELECT.deck * 0x48);
            }
        }
        if (PAD_STATES[0]->pressed & 0x40) {
            playMenuSound(1);
            OPEN_STARTER_SELECT.chosen = 1;
        }
        OPEN_INTRO_TEXT.page = OPEN_STARTER_SELECT.deck + 6;
        rect.x = window->rect.x - window->scroll[2] + 1;
        rect.y = window->rect.y - window->scroll[3] + OPEN_STARTER_SELECT.deck * 0x48;
        rect.w = 0x104;
        rect.h = 0x48;
        moveCursorHighlight(&OPEN_STARTER_CURSOR, &rect);
        drawCursorHighlight(&OPEN_STARTER_CURSOR, z);
    }
}

void OPEN_drawStarterSelectWindow(void) {
    drawWindow(&OPEN_STARTER_WINDOW, OPEN_drawStarterSelect, 1);
}

/* "Is this Deck OK?": the string is followed by leftover bytes in the ROM, so it stays as data */
extern const char OPEN_STR_IS_THIS_DECK_OK[];

void OPEN_runStarterSelect(s32 parentTask) {
    Rect16 cursor;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];
    u32 *arc;
    s32 i;

    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\BCARD.ARC", getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\P_CARD.ARC", getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < 3; i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), i * 20 + 0x2C0, 0, -1, -1);
        DrawSync(0);
    }
    OPEN_STARTER_SELECT.chosen = 0;
    OPEN_STARTER_SELECT.deck = 0;
    rect.x = 0x1E;
    rect.y = 0x44;
    rect.w = 0x104;
    rect.h = 0x5A;
    view.x = 0;
    view.y = 0;
    view.w = 0x10E;
    view.h = 0xD8;
    openWindow(&OPEN_STARTER_WINDOW, &rect, -1, (s16 *)&view, 10, 0x81, 0x80, 0xC);
    OPEN_STARTER_WINDOW.label = (s32)"STARTER SELECT";
    OPEN_STARTER_WINDOW.labelPalette = 7;
    cursor.x = OPEN_STARTER_WINDOW.originX + 4;
    cursor.y = OPEN_STARTER_WINDOW.originY + 1;
    cursor.w = 12;
    cursor.h = 12;
    initCursorHighlight(&OPEN_STARTER_CURSOR, &cursor, (Bytes4 *)-1);
    func_80014C08(FRAME_INTERVAL);
    playMenuSound(3);
    addFrameCallback((s32)OPEN_drawStarterSelectWindow);
    OPEN_INTRO_TEXT.waitInput = 1;
    while (1) {
        func_80014C08(FRAME_INTERVAL);
        if (OPEN_INTRO_TEXT.page == 5 && (PAD_STATES[0]->pressed & 0x40)) {
            playMenuSound(1);
            OPEN_INTRO_TEXT.waitInput = 0;
            OPEN_INTRO_TEXT.page = 6;
        }
        if (OPEN_STARTER_SELECT.chosen == 0) {
            continue;
        }
        initDialog(dialog, OPEN_STR_IS_THIS_DECK_OK, 1);
        runDialog(dialog);
        switch ((s8)dialog[0xA5]) {
        case 1:
            break;
        case 0:
        case 2:
            OPEN_STARTER_SELECT.chosen = 0;
            break;
        }
        if (OPEN_STARTER_SELECT.chosen != 0) {
            break;
        }
    }
    animateWindowTo(&OPEN_STARTER_WINDOW, (Rect16 *)-1);
    playMenuSound(4);
    freeHeapBlock(arc);
    func_80014C08(20);
    removeFrameCallback((s32)OPEN_drawStarterSelectWindow);
    OPEN_giveStarterDeck(OPEN_STARTER_SELECT.deck);
    func_80014A48(parentTask);
}

void OPEN_drawTitleSpinQuad(s32 idx) {
    POLY_FT4 *poly;
    s32 cx;
    s32 cy;
    s32 angle;

    OPEN_TITLE_SPIN_ANGLE += 32;
    cx = rsin(OPEN_TITLE_SPIN_ANGLE) * OPEN_TITLE_SPIN_RADIUS / 4096 + 160;
    cy = OPEN_TITLE_SPIN_RADIUS * (rcos(OPEN_TITLE_SPIN_ANGLE) << 1) / 4096 + 60;
    if (idx & 1) {
        angle = OPEN_TITLE_SPIN_ANGLE;
    } else {
        angle = -OPEN_TITLE_SPIN_ANGLE;
    }
    poly = &OPEN_TITLE_SPIN_QUADS[idx][FRAME_BUFFER_INDEX];
    initPrimByType(0xC, poly, 1, 0);
    poly->r0 = OPEN_TITLE_SPIN_SHADE;
    poly->g0 = OPEN_TITLE_SPIN_SHADE;
    poly->b0 = OPEN_TITLE_SPIN_SHADE;
    poly->u0 = 0;
    poly->v0 = 0;
    poly->u1 = 0xEF;
    poly->v1 = 0;
    poly->u2 = 0;
    poly->v2 = 0xEF;
    poly->u3 = 0xEF;
    poly->v3 = 0xEF;
    poly->x0 = cx + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rsin(angle) / 4096;
    poly->y0 = cy + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rcos(angle) / 4096;
    poly->x1 = cx + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rsin(angle + 0x400) / 4096;
    poly->y1 = cy + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rcos(angle + 0x400) / 4096;
    poly->x2 = cx + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rsin(angle + 0xC00) / 4096;
    poly->y2 = cy + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rcos(angle + 0xC00) / 4096;
    poly->x3 = cx + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rsin(angle + 0x800) / 4096;
    poly->y3 = cy + OPEN_TITLE_SPIN_SIZE * 120 / 100 * rcos(angle + 0x800) / 4096;
    poly->tpage = 0x2A;
    poly->clut = 0x3E64;
    addPrim(&CURRENT_FRAME_BUFFER->ot[0], poly);
}

void OPEN_drawSprite(s32 x, s32 y, s32 texX, s32 texY, s32 w, s32 h, s32 clutX, s32 clutY, s32 texMode,
                   s32 semiTrans, s32 blendMode, s32 shade, s32 otIndex) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = (texX % 64) * (4 >> texMode);
        CUR_SPRT->sp.v0 = texY % 256;
        CUR_SPRT->sp.clut = getClut(clutX, clutY);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, semiTrans);
        CUR_SPRT->sp.r0 = shade;
        CUR_SPRT->sp.g0 = shade;
        CUR_SPRT->sp.b0 = shade;
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(texMode, blendMode, texX, texY));
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

/* the last three bytes are leftovers in the original, not zero padding */
const char OPEN_STR_IS_THIS_DECK_OK[20] = "Is this Deck OK?\0\x6D\x01\x0C";

/* libgpu's POLY_F4 */
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} POLY_F4;
extern POLY_F4 D_801F5250[2];
extern DR_MODE D_801F5280[2];

void OPEN_drawTitleScreen(void) {
    s32 shade;
    s32 i; /* also the shade of the dark band */

    switch (OPEN_TITLE_STATE) {
    case 7:
        if (OPEN_PRESS_START_SHADE > 0x80) {
            shade = 0x80;
        } else {
            shade = OPEN_PRESS_START_SHADE;
        }
        OPEN_drawSprite(0x48, 0xAC, 0x200, 0x98, 0xB0, 0x10, 0x240, 0xF8, 0, 1, 1, shade, 0xA);
        OPEN_drawSprite(0x48, 0xAC, 0x200, 0x98, 0xB0, 0x10, 0x250, 0xF8, 0, 1, 2, shade, 0xA);
        break;
    case 8:
        for (i = 0; i < 3; i++) {
            OPEN_drawSprite(i * 100 + 0xC, 0x9C, 0x2C0, i * 0x1C + 0x80, 0x60, 0x1C, OPEN_TITLE_OPTION_DIMMED[i] * 16 + 0x2C0, 0xFA, 0, 0, 0, 0x80, 5);
        }
        break;
    }
    switch (OPEN_TITLE_STATE) {
    case 8:
        D_801F52A8 += 2;
        if (D_801F52A8 > 16) {
            D_801F52A8 = 16;
        }
    case 7:
        if (OPEN_TITLE_STATE == 7) {
            D_801F52A8 -= 2;
            if (D_801F52A8 < 0) {
                D_801F52A8 = 0;
            }
        }
        i = D_801F52A8 * 8;
        initPrimByType(8, &D_801F5250[FRAME_BUFFER_INDEX], 1, 0);
        D_801F5250[FRAME_BUFFER_INDEX].r0 = i;
        D_801F5250[FRAME_BUFFER_INDEX].g0 = i;
        D_801F5250[FRAME_BUFFER_INDEX].b0 = i;
        D_801F5250[FRAME_BUFFER_INDEX].x0 = 0;
        D_801F5250[FRAME_BUFFER_INDEX].y0 = 0x9A;
        D_801F5250[FRAME_BUFFER_INDEX].x1 = 0x140;
        D_801F5250[FRAME_BUFFER_INDEX].y1 = 0x9A;
        D_801F5250[FRAME_BUFFER_INDEX].x2 = 0;
        D_801F5250[FRAME_BUFFER_INDEX].y2 = 0xBA;
        D_801F5250[FRAME_BUFFER_INDEX].x3 = 0x140;
        D_801F5250[FRAME_BUFFER_INDEX].y3 = 0xBA;
        setlen(&D_801F5280[FRAME_BUFFER_INDEX], 1);
        D_801F5280[FRAME_BUFFER_INDEX].code[0] = _get_mode(0, 0, 0x40);
        addPrim(&CURRENT_FRAME_BUFFER->ot[6], &D_801F5250[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[6], &D_801F5280[FRAME_BUFFER_INDEX]);
    case 6:
        if (OPEN_TITLE_STATE == 6) {
            if (++OPEN_TITLE_TIMER > 60) {
                OPEN_TITLE_STATE = 7;
                OPEN_TITLE_TIMER = 0;
            }
        }
        D_801F52AC += 4;
        if (D_801F52AC > 0x80) {
            D_801F52AC = 0x80;
        }
        OPEN_drawSprite(0x20, 0xC8, 0x200, 0xA8, 0xFF, 0x20, 0x240, 0xFA, 0, 1, 1, D_801F52AC, 0xA);
        OPEN_drawSprite(0x20, 0xC8, 0x200, 0xA8, 0xFF, 0x20, 0x250, 0xFA, 0, 1, 2, D_801F52AC, 0xA);
    case 5:
        if (OPEN_TITLE_STATE == 5) {
            D_801F52A4 -= 20;
            if (D_801F52A4 < 0) {
                OPEN_TITLE_STATE = 6;
                OPEN_TITLE_TIMER = 0;
                D_801F52A4 = 0;
            }
        }
        OPEN_drawSprite(D_801F52A4, 0x78 - D_801F52A8, 0x2C0, 0, 0x100, 0x30, 0x140, 0xF8, 1, 1, 0, 0x80, 5);
        OPEN_drawSprite(D_801F52A4 + 0x100, 0x78 - D_801F52A8, 0x340, 0, 0x40, 0x30, 0x140, 0xF8, 1, 1, 0, 0x80, 5);
    case 4:
        if (OPEN_TITLE_STATE == 4) {
            OPEN_TITLE_SPIN_SHADE -= 16;
            if (OPEN_TITLE_SPIN_SHADE < 0) {
                OPEN_TITLE_SPIN_SHADE = 0;
            }
            OPEN_TITLE_SPIN_SIZE += 25;
            if (++OPEN_TITLE_TIMER > 60) {
                OPEN_TITLE_STATE = 5;
                OPEN_TITLE_TIMER = 0;
                D_801F52A4 = 0x140;
                D_801F52A8 = 0;
            }
        }
        OPEN_drawSprite(0x20, 0x10 - D_801F52A8, 0x200, 0, 0x100, 0x88, 0x140, 0xF9, 1, 1, 0, 0x80, 0xA);
    case 2:
    case 3:
        if (OPEN_TITLE_STATE == 3) {
            if (++OPEN_TITLE_TIMER > 60) {
                OPEN_TITLE_STATE = 4;
                OPEN_TITLE_TIMER = 0;
            }
        }
        OPEN_drawSprite(0, 0, 0x140, 0, 0x100, 0xF0, 0x140, 0xFA, 1, 0, 0, 0x80, 0xA);
        OPEN_drawSprite(0x100, 0, 0x1C0, 0, 0x40, 0xF0, 0x140, 0xFA, 1, 0, 0, 0x80, 0xA);
    case 1:
        if (OPEN_TITLE_STATE == 1) {
            if (++OPEN_TITLE_TIMER > 100) {
                setScreenFadeParams(1, 1, 8);
                OPEN_TITLE_STATE = 3;
                OPEN_TITLE_TIMER = 0;
            }
        }
    case 0:
        if (OPEN_TITLE_STATE == 0) {
            OPEN_TITLE_TIMER++;
            OPEN_TITLE_SPIN_SHADE += 8;
            if (OPEN_TITLE_SPIN_SHADE > 0x40) {
                OPEN_TITLE_SPIN_SHADE = 0x40;
            }
            OPEN_TITLE_SPIN_SIZE += 2;
            if (OPEN_TITLE_SPIN_SIZE > 100) {
                OPEN_TITLE_SPIN_SIZE = 100;
            }
            OPEN_TITLE_SPIN_RADIUS -= 2;
            if (OPEN_TITLE_SPIN_RADIUS < 0) {
                OPEN_TITLE_SPIN_RADIUS = 0;
                func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 1, 4, 0);
                OPEN_TITLE_TIMER = 0;
                OPEN_TITLE_STATE = 1;
            }
        }
        OPEN_drawTitleSpinQuad(0);
        OPEN_drawTitleSpinQuad(1);
        break;
    }
}

void OPEN_drawTitleScreen();

void OPEN_runTitleScreen(s32 parentTask) {
    char text[192]; /* unused, but it sizes the frame */
    /* never read: only its empty string is left in .rodata. It is written
       "\0" so that it stays apart from the "" of the arena table
       OPEN_ARENA_NAMES, which the original built in another file */
    char *name;
    u32 *arc;
    s32 i;
    s32 choice;
    s32 idle;
    s32 fade;
    s32 done;

    loadMusicTrack(0, 0x66, 0x7F);
    i = 0;
    hideScrollingBackground();
    loadScrollingBackground();
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\TITLE.ARC", getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    name = "\0";
    choice = 1;
    for (i = 0; i < 3; i++) {
        OPEN_TITLE_OPTION_DIMMED[i] = 1;
    }
    OPEN_TITLE_OPTION_DIMMED[choice] = 0;
    fade = 0;
    playLoadedMusic(0);
    D_801F52AC = 0;
    D_801F52B0 = 0;
    OPEN_PRESS_START_SHADE = 0x80;
    OPEN_PRESS_START_STEP = 4;
    OPEN_TITLE_TIMER = 0;
    OPEN_TITLE_SPIN_RADIUS = 0xA0;
    OPEN_TITLE_SPIN_ANGLE = 0;
    OPEN_TITLE_SPIN_SIZE = 0;
    OPEN_TITLE_SPIN_SHADE = 0;
    OPEN_TITLE_STATE = 0;
    done = 0;
    D_801F52BC[0] = 0;
    D_801F52BC[1] = 0x80;
    D_801F52BC[2] = 0x80;
    D_801F52C0[0] = 0;
    D_801F52C0[1] = 0x80;
    D_801F52C0[2] = 0x80;
    D_801F52C4 = 0;
    D_801F52C8 = 1;
    resetPlayerData();
    addFrameCallback((s32)OPEN_drawTitleScreen);
    idle = 0;
    D_801F52A8 = 0;
    do {
        func_80014C08(FRAME_INTERVAL);
        switch (OPEN_TITLE_STATE) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if (PAD_STATES[0]->pressed & 0x800) {
                idle = 0;
                fade = 1;
                D_801F52A4 = 0;
                D_801F52A8 = 0x20;
                OPEN_TITLE_SPIN_SHADE = 0;
                OPEN_TITLE_STATE = 8;
                playMenuSound(1);
            }
            break;
        case 6:
        case 7:
            OPEN_PRESS_START_SHADE += OPEN_PRESS_START_STEP;
            if (OPEN_PRESS_START_SHADE >= 0x100) {
                OPEN_PRESS_START_SHADE = 0xFF;
                OPEN_PRESS_START_STEP = -4;
            } else if (OPEN_PRESS_START_SHADE < 0) {
                OPEN_PRESS_START_SHADE = 0;
                OPEN_PRESS_START_STEP = 4;
            }
            if (PAD_STATES[0]->pressed & 0x800) {
                idle = 0;
                OPEN_TITLE_STATE = 8;
                playMenuSound(1);
            }
            idle++;
            break;
        case 8:
            if (fade) {
                if (isScreenFadeActive()) {
                    setScreenFadeParams(1, 1, 8);
                }
                fade = 0;
            }
            if (isScreenFadeActive() == 0) {
                idle++;
                if ((u16)PAD_STATES[0]->pressed & 0x8000) {
                    idle = 0;
                    playMenuSound(2);
                    choice = (choice + 2) % 3;
                } else if (PAD_STATES[0]->pressed & 0x2000) {
                    idle = 0;
                    playMenuSound(2);
                    choice = (choice + 4) % 3;
                }
                for (i = 0; i < 3; i++) {
                    OPEN_TITLE_OPTION_DIMMED[i] = 1;
                }
                OPEN_TITLE_OPTION_DIMMED[choice] = 0;
                if (PAD_STATES[0]->pressed & 0x10) {
                    idle = 0;
                    playMenuSound(0);
                    OPEN_TITLE_STATE = 7;
                    OPEN_PRESS_START_SHADE = 0;
                    OPEN_PRESS_START_STEP = 4;
                } else if (PAD_STATES[0]->pressed & 0x40) {
                    idle = 0;
                    playMenuSound(1);
                    done = 1;
                }
            }
            break;
        }
    } while (idle < 0xE11 && !done);
    func_800149B8(0, -1, 0, 0x200, screenFadeTask, NULL, 2, 8, 0);
    func_80014C08(20);
    removeFrameCallback((s32)OPEN_drawTitleScreen);
    stopScreenFade();
    func_80014C08(10);
    if (idle >= 0xE11) {
        stopMusic();
        func_80014A48(0);
        func_80014A90();
    }
    func_80014A48(parentTask, choice);
}

void OPEN_drawFriendMenu(void) {
    s32 i;
    s32 same;
    s32 clutY;

    if (OPEN_FRIEND_MENU_SHOWN == 1) {
        D_801F4F00 += 8;
        if (D_801F4F00 > 8) {
            D_801F4F00 = 8;
        }
    } else {
        D_801F4F00 -= 8;
        if (D_801F4F00 < -140) {
            D_801F4F00 = -140;
        }
    }
    OPEN_drawSprite(D_801F4F00, D_801F4F04, 0x300, 0, 0x8C, 0x96, 0x300, 0x96, 0, 1, 1, 0x80, 2);
    same = (u16)PLAYER_DATA(0).unk10 == (u16)PLAYER_DATA(1).unk10;
    if ((OPEN_TRADE_ENABLED & 3) != 3) {
        same = 1;
    }
    for (i = 0; i < 7; i++) {
        if (*((s8 *)D_8006E054 + 0x1028) == i) {
            clutY = 0x98;
            if (i == 1 && same) {
                clutY = 0x99;
            }
        } else {
            clutY = 0x97;
            if (i == 1 && same) {
                clutY = 0x9A;
            }
        }
        OPEN_drawSprite(D_801F4F00 + 0x19, D_801F4F04 + 0x16 + i * 16, 0x323, i * 16, 0x58, 0x10, 0x300, clutY, 0, 1, 0, 0x80, 2);
    }
}

char *strcat(char *dst, const char *src);

void OPEN_drawPlayerRecord(PlayerWindow *window) {
    /* its own copy: a "Name" literal would be merged with the one in OPEN_drawSaveSummary */
    static const char nameLabel[] = "Name";
    char text[64];
    s32 total;
    s32 i;
    s32 x;
    s32 y;
    s32 z;

    x = window->window.originX;
    y = window->window.originY;
    z = window->window.z;
    total = 0;
    for (i = 0; i < 301; i++) {
        total += getOwnedCardCount(window->player, i);
    }
    drawText(x + 1, y, (s32)nameLabel, 6, z);
    drawText(x + 0x43, y, (s32)PLAYER_DATA(window->player).name, 7, z);
    drawText(x + 1, y + 12, (s32)"2 Player Battle", 6, z);
    sprintf(text, "*s0%3d    %3d", PLAYER_DATA(window->player).versusWins, PLAYER_DATA(window->player).versusLosses);
    drawText(x + 0x52, y + 12, (s32)text, 7, z);
    drawSmallText(x + 0x65, y + 19, (s32)"WINS", 6, z);
    drawSmallText(x + 0x90, y + 19, (s32)"LOSSES", 6, z);
    countSeenCards(window->player);
    drawText(x + 1, y + 24, (s32)"Cards in Stock.", 6, z);
    i = PLAYER_DATA(window->player).seenCardCount * 1000 / 301;
    sprintf(text, "*s0%3d.%1d*w4*c6%%", i / 10, i % 10);
    drawText(x + 0x67, y + 24, (s32)text, 7, z);
    drawText(x + 1, y + 36, (s32)"Cards in Possession.", 6, z);
    sprintf(text, "*s0%4d", total);
    drawText(x + 0x6D, y + 36, (s32)text, 7, z);
    drawSmallText(x + 0x87, y + 0x2B, (s32)"CARDS", 6, z);
    drawText(x + 1, y + 0x30, (s32)"Deck", 6, z);
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(window->player).savedDecks[i].inUse) {
            strcpy(text, (char *)PLAYER_DATA(window->player).savedDecks[i].unk1);
            strcat(text, "Deck");
        } else {
            strcpy(text, "Unused Deck");
        }
        drawText(x + 0x37, y + (i + 4) * 12, (s32)text, 7, z);
    }
    if (PLAYER_DATA(window->player).unk28_10) {
        drawIcon(x + 8, y + 0x44, 2, 11, z);
    }
}

void OPEN_drawFriendScreen(void) {
    s32 i;

    OPEN_drawFriendMenu();
    for (i = 0; i < 2; i++) {
        drawWindow(&OPEN_PLAYER_RECORD_WINDOWS[i].window, OPEN_drawPlayerRecord, 4);
    }
    if (OPEN_FRIEND_MENU_SHOWN == 0) {
        D_801F4F0C += 16;
        if (D_801F4F0C > 240) {
            D_801F4F0C = 240;
        }
        D_801F4F10 -= 8;
        if (D_801F4F10 < -96) {
            D_801F4F10 -= 96;
        }
        D_801F4F1C += 16;
        if (D_801F4F1C > 240) {
            D_801F4F1C = 240;
        }
    } else {
        D_801F4F0C -= 16;
        if (D_801F4F0C < 16) {
            D_801F4F0C = 16;
        }
        D_801F4F10 += 8;
        if (D_801F4F10 > 0) {
            D_801F4F10 = 0;
        }
        D_801F4F1C -= 16;
        if (D_801F4F1C < 20) {
            D_801F4F1C = 20;
        }
    }
    OPEN_TITLE_PART_COUNT = 0;
    OPEN_drawTitlePart(D_801F4F08, D_801F4F0C, 0);
    OPEN_drawTitlePart(D_801F4F08, D_801F4F0C, 1);
    OPEN_drawTitlePart(D_801F4F08, D_801F4F0C, 2);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 10);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 11);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 12);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 13);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 14);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 15);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 16);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 17);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 18);
    OPEN_drawTitlePart(D_801F4F18, D_801F4F1C, 19);
    OPEN_drawTitlePart(D_801F4F10, D_801F4F14, 21);
}

void OPEN_openPlayerRecordWindows(void) {
    Rect16 to;
    Rect16 from;
    s32 i;

    D_801F4F08 = 12;
    D_801F4F0C = 240;
    D_801F4F10 = -96;
    D_801F4F14 = 0;
    D_801F4F18 = 14;
    D_801F4F1C = 240;
    D_801F4F00 = -140;
    D_801F4F04 = 80;
    OPEN_FRIEND_MENU_SHOWN = 1;
    OPEN_FRIEND_MENU_DONE = 0;
    for (i = 0; i < 2; i++) {
        from.x = 140;
        from.y = i * 100 + 40;
        from.w = 172;
        from.h = 84;
        to.x = 320;
        to.y = i * 100 + 40;
        to.w = 172;
        to.h = 84;
        openWindow(&OPEN_PLAYER_RECORD_WINDOWS[i].window, &from, (s32)&to, (s16 *)-1, 8, 0x25, 0x80, 0xC);
        OPEN_PLAYER_RECORD_WINDOWS[i].player = i;
        if (i == 0) {
            OPEN_PLAYER_RECORD_WINDOWS[0].window.label = (s32)"PLAYER 1";
        } else {
            OPEN_PLAYER_RECORD_WINDOWS[1].window.label = (s32)"PLAYER 2";
        }
    }
}

void OPEN_closePlayerRecordWindows(void) {
    Rect16 rect;
    s32 i;

    for (i = 0; i < 2; i++) {
        rect.x = 320;
        rect.y = i * 100 + 40;
        rect.w = 172;
        rect.h = 84;
        animateWindowTo(&OPEN_PLAYER_RECORD_WINDOWS[i].window, &rect);
    }
}

void OPEN_drawFriendScreen(void);
void OPEN_runCardTrade();
s32 OPEN_saveFriendGame(void);
void func_8002F298(s32 player);
void func_8002F3C4(s32 player);
void startVersusDuel(void);
void runTitleMenu(void);

void OPEN_runBattleWithFriend(void) {
    Rect16 rect;
    u8 dialog[0xB8];
    u32 *arc;
    char *message;
    s32 i;
    s32 ok;

    changeScrollingBackground(7, 0x380, 0, 0x380, 0x80);
    loadMusicTrack(0, 0x6D, 0x7F);
    playLoadedMusic(0);
    i = 0;
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\FRIEND.ARC", getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    OPEN_openPlayerRecordWindows();
    playMenuSound(3);
    addFrameCallback((s32)OPEN_drawFriendScreen);
    do {
        func_80014C08(FRAME_INTERVAL);
        OPEN_TRADE_ENABLED = PLAYER_DATA(0).unk28_10 | (PLAYER_DATA(1).unk28_10 << 1);
        if (PAD_STATES[0]->pressed & 0x40) {
            switch (((SessionView *)D_8006E054)->menuRow) {
            case 1:
                message = NULL;
                if ((u16)PLAYER_DATA(0).unk10 == (u16)PLAYER_DATA(1).unk10) {
                    message = "You can't trade the same Data!";
                } else if (!(OPEN_TRADE_ENABLED & 3)) {
                    message = "Trade is disabled.";
                } else if (!(OPEN_TRADE_ENABLED & 1)) {
                    message = "Player 1's trade is disabled.";
                } else if (!(OPEN_TRADE_ENABLED & 2)) {
                    message = "Player 2's trade is disabled.";
                }
                if (message == NULL) {
                    playMenuSound(4);
                    OPEN_FRIEND_MENU_SHOWN = 0;
                    OPEN_closePlayerRecordWindows();
                    func_80014C08(20);
                    removeFrameCallback((s32)OPEN_drawFriendScreen);
                    func_800149B8(0, -1, 0, 0x800, OPEN_runCardTrade, getCurrentTaskId(), 0, 0, 0);
                    func_80014C08(0x7FFFFFFF);
                    OPEN_openPlayerRecordWindows();
                    addFrameCallback((s32)OPEN_drawFriendScreen);
                } else {
                    playMenuSound(1);
                    initDialog(dialog, message, 0);
                    runDialog(dialog);
                }
                break;
            case 2:
                playMenuSound(4);
                OPEN_FRIEND_MENU_SHOWN = 0;
                OPEN_closePlayerRecordWindows();
                func_80014C08(20);
                removeFrameCallback((s32)OPEN_drawFriendScreen);
                func_8002F298(0);
                OPEN_openPlayerRecordWindows();
                addFrameCallback((s32)OPEN_drawFriendScreen);
                break;
            case 3:
                playMenuSound(4);
                OPEN_FRIEND_MENU_SHOWN = 0;
                OPEN_closePlayerRecordWindows();
                func_80014C08(20);
                removeFrameCallback((s32)OPEN_drawFriendScreen);
                func_8002F298(1);
                OPEN_openPlayerRecordWindows();
                addFrameCallback((s32)OPEN_drawFriendScreen);
                break;
            case 4:
                playMenuSound(4);
                OPEN_FRIEND_MENU_SHOWN = 0;
                OPEN_closePlayerRecordWindows();
                func_80014C08(20);
                removeFrameCallback((s32)OPEN_drawFriendScreen);
                func_8002F3C4(0);
                OPEN_openPlayerRecordWindows();
                addFrameCallback((s32)OPEN_drawFriendScreen);
                break;
            case 5:
                playMenuSound(4);
                OPEN_FRIEND_MENU_SHOWN = 0;
                OPEN_closePlayerRecordWindows();
                func_80014C08(20);
                removeFrameCallback((s32)OPEN_drawFriendScreen);
                func_8002F3C4(1);
                OPEN_openPlayerRecordWindows();
                addFrameCallback((s32)OPEN_drawFriendScreen);
                break;
            default:
                playMenuSound(4);
                OPEN_FRIEND_MENU_DONE = 1;
                break;
            }
        } else if (PAD_STATES[0]->pressed & 0x10) {
            playMenuSound(4);
            ((SessionView *)D_8006E054)->menuRow = 6;
            OPEN_FRIEND_MENU_DONE = 1;
        } else if (PAD_STATES[0]->repeat & 0x1000) {
            playMenuSound(2);
            ((SessionView *)D_8006E054)->menuRow--;
        } else if (PAD_STATES[0]->repeat & 0x4000) {
            playMenuSound(2);
            ((SessionView *)D_8006E054)->menuRow++;
        }
        ((SessionView *)D_8006E054)->menuRow = (((SessionView *)D_8006E054)->menuRow + 7) % 7;
        if (OPEN_FRIEND_MENU_DONE != 0) {
            OPEN_FRIEND_MENU_SHOWN = 2;
            OPEN_closePlayerRecordWindows();
            if (((SessionView *)D_8006E054)->menuRow == 6) {
                initDialog((u8 *)&OPEN_DIALOG, "Save \"Battle with Friend\" game?", 1);
                runDialog(&OPEN_DIALOG);
                ok = 1;
                switch (OPEN_DIALOG.choice) {
                case 1:
                    ok = OPEN_saveFriendGame();
                    break;
                case 0:
                case 2:
                    initDialog((u8 *)&OPEN_DIALOG, "Return to the Title Screen?", 1);
                    runDialog(&OPEN_DIALOG);
                    if (OPEN_DIALOG.choice == 1) {
                        ok = 0;
                    }
                    break;
                }
                if (ok) {
                    playMenuSound(3);
                    OPEN_FRIEND_MENU_SHOWN = 1;
                    OPEN_FRIEND_MENU_DONE = 0;
                    for (i = 0; i < 2; i++) {
                        rect.x = 0x8C;
                        rect.y = i * 100 + 0x28;
                        rect.w = 0xAC;
                        rect.h = 0x54;
                        animateWindowTo(&OPEN_PLAYER_RECORD_WINDOWS[i].window, &rect);
                    }
                }
            }
        }
    } while (OPEN_FRIEND_MENU_DONE == 0);
    func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 2, 8, 0);
    func_80014C08(20);
    removeFrameCallback((s32)OPEN_drawFriendScreen);
    hideScrollingBackground();
    stopScreenFade();
    switch (((SessionView *)D_8006E054)->menuRow) {
    case 0:
        func_800149B8(0, -1, 0, 0x200, startVersusDuel, 0, 0, 0, 0);
        break;
    case 6:
        func_800149B8(0, -1, 0, 0x100, runTitleMenu, 0, 0, 0, 0);
        break;
    }
}

s32 func_801EBA74(s32 kind) {
    s32 id;
    s32 word;
    s32 shift;
    s32 bit;

    if (kind == 0) {
        id = 0x126;
    } else if (kind == 1) {
        id = 0x12A;
    } else if (kind == 2) {
        id = 0x12D;
    } else {
        id = 0x126;
    }
    id -= 12;
    word = id / 32;
    shift = id % 32;
    ((PlayerProfile *)PLAYER_PROFILES)->unk23FC[word] |= bit = 1 << shift;
    return (((PlayerProfile *)PLAYER_PROFILES)->unk23FC[word] & bit) != 0;
}

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

void OPEN_loadMemcardTextures(void);
void OPEN_initMemcardScreen(s32 port);
void OPEN_resetMemcardScreen(s32 port);
void OPEN_drawMemcardScreen();
void OPEN_runMemcardAccess();
void OPEN_runMemcardPrompts();

void OPEN_runMemcardScreen(s32 mode, s32 parentTask, s32 port) {
    OPEN_MEMCARD_MODE = mode;
    OPEN_SAVE_PLACE_IMAGES = NULL;
    OPEN_loadMemcardTextures();
    OPEN_initMemcardScreen(port);
    addFrameCallback((s32)OPEN_drawMemcardScreen);
    do {
        OPEN_resetMemcardScreen(port);
        func_800149B8(0, -1, 4, 0x800, OPEN_runMemcardAccess, 0, 0, 0, 0);
        func_800149B8(0, -1, 0, 0x800, OPEN_runMemcardPrompts, 0, 0, 0, 0);
        while (OPEN_MEMCARD.ready != 1) {
            func_80014C08(FRAME_INTERVAL);
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
    func_80014C08(20);
    removeFrameCallback((s32)OPEN_drawMemcardScreen);
    switch (D_801F80CA) {
    case 0:
        break;
    case 1:
        initDialog((u8 *)&OPEN_DIALOG, "Quit the game?", 1);
        runDialog(&OPEN_DIALOG);
        if (OPEN_DIALOG.choice == 1) {
            stopMusic();
            fadeOutScrollingBackground();
            func_80014C08(20);
            func_80014A48(0);
            func_80014A90();
        }
        break;
    }
    freeHeapBlock(OPEN_SAVE_PLACE_IMAGES);
    func_80014C08(20);
    freeHeapBlocksByTag(0x63);
    func_80014A48(parentTask);
}

s32 OPEN_loadFriendSaves(void) {
    s32 task;

    task = getCurrentTaskId();
    func_800149B8(0, -1, 0, 0x600, OPEN_runMemcardScreen, 7, task, 0, 0);
    func_80014C08(0x7FFFFFFF);
    if (OPEN_MEMCARD.cancelled == 0) {
        func_800149B8(0, -1, 0, 0x600, OPEN_runMemcardScreen, 7, task, 1, 0);
        func_80014C08(0x7FFFFFFF);
        if (OPEN_MEMCARD.cancelled == 0) {
            return 0;
        }
    }
    return 1;
}

s32 OPEN_saveFriendGame(void) {
    func_800149B8(0, -1, 0, 0x600, OPEN_runMemcardScreen, 6, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    return OPEN_MEMCARD_CANCELLED != 0;
}

void OPEN_createNewSave(void) {
    func_800149B8(0, -1, 0, 0x600, OPEN_runMemcardScreen, 0, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
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
    OPEN_MEMCARD.unk540 = 0;
    OPEN_MEMCARD.cancelled = 1;
    OPEN_MEMCARD.ready = 0;
    OPEN_MEMCARD.card = port;
    OPEN_MEMCARD.slot = 0;
    OPEN_MEMCARD.again = 0;
    switch (OPEN_MEMCARD.mode) {
    case 0:
        countSeenCards(port);
        OPEN_MEMCARD.loading = 0;
        ((SessionView *)D_8006E054)->saves[port].slot = 0;
        ((SessionView *)D_8006E054)->saves[port].file = 0;
        OPEN_MEMCARD.state = 1;
        OPEN_MEMCARD.unk542 = 0;
        break;
    case 2:
    case 4:
    case 5:
        countSeenCards(port);
        OPEN_MEMCARD.loading = 0;
        OPEN_MEMCARD.state = 0x11;
        OPEN_MEMCARD.unk542 = 1;
        break;
    case 8:
        countSeenCards(port);
        OPEN_MEMCARD.loading = 0;
        OPEN_MEMCARD.state = 0x11;
        OPEN_MEMCARD.unk542 = 2;
        PLAYER_DATA(0).unkE = 0;
        PLAYER_DATA(0).unkF = 1;
        break;
    case 7:
        OPEN_MEMCARD.loading = 1;
        ((SessionView *)D_8006E054)->saves[port].slot = port;
        ((SessionView *)D_8006E054)->saves[port].file = 0;
        OPEN_MEMCARD.state = 1;
        OPEN_MEMCARD.unk542 = 0;
        break;
    case 6:
        countSeenCards(port);
        OPEN_MEMCARD.loading = 0;
        OPEN_MEMCARD.state = 0x11;
        if (port == 0) {
            OPEN_MEMCARD.again = 1;
        }
        D_801F80CA = 0;
        break;
    case 0xFF:
        OPEN_MEMCARD.loading = 1;
        ((SessionView *)D_8006E054)->saves[port].slot = 0;
        ((SessionView *)D_8006E054)->saves[port].file = 0;
        OPEN_MEMCARD.state = 1;
        OPEN_MEMCARD.unk542 = 0;
        break;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            OPEN_MEMCARD_EMPTY[i][j] = 1;
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

    func_800149B8(0, -1, 0, 0x800, loadFile, "C:\\OBJECT\\saveload.TIS", getCurrentTaskId(), 0, 0);
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    switch (OPEN_MEMCARD_MODE) {
    case 0:
    case 7:
    case 0xFF:
        func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\SAVE.ARC", getCurrentTaskId());
        OPEN_SAVE_PLACE_IMAGES = (u8 *)func_80014C08(0x7FFFFFFF);
        break;
    }
}

void OPEN_initTransferArrow(POLY_FT4 *poly, s32 shade) {
    s32 u0;
    s32 u1;

    func_800677A4(poly);
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

void OPEN_drawSaveSummary(SlotDraw draw);

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

    sprintf(text, "Player %d : Slot %d", OPEN_MEMCARD_CARD + 1, ((SessionData *)D_8006E054)->unk1010[OPEN_MEMCARD_CARD * 8] + 1);
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

/* "Arena": the string is followed by two leftover bytes (E0 03) in the ROM, so it stays as data */
extern const char OPEN_STR_ARENA[];

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
    if (save->unk28_10) {
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

/* the last two bytes are leftovers in the original, not zero padding */
const char OPEN_STR_ARENA[8] = "Arena\0\xE0\x03";

extern s32 D_801D8198;
void OPEN_confirmOverwrite(s32 port);
void OPEN_confirmPlayWithoutSaving(s32 port);
void OPEN_confirmFormat(s32 pad);
void OPEN_selectSaveFile(s32 port);

void OPEN_runMemcardPrompts(void) {
    s32 port;

    do {
        func_80014C08(FRAME_INTERVAL);
        D_801D8198++;
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
                    OPEN_MEMCARD_STATE = 0x17;
                    break;
                case 0:
                case 2:
                    OPEN_MEMCARD_STATE = 0x1B;
                    break;
                }
            }
            break;
        case 24:
            if (OPEN_MEMCARD_MODE != 6) {
                OPEN_confirmOverwrite(port);
            } else {
                OPEN_MEMCARD.state = 0x12;
                if (port != 0) {
                    OPEN_MEMCARD.animPhase = 5;
                }
            }
            break;
        case 10:
            OPEN_MEMCARD_MESSAGE = 0x10;
            OPEN_selectSaveFile(port);
            break;
        case 3:
            OPEN_MEMCARD_MESSAGE = 0x11;
            OPEN_selectSaveFile(port);
            break;
        case 16:
            OPEN_MEMCARD_MESSAGE = 6;
            initDialog((u8 *)&OPEN_DIALOG, NULL, 1);
            runDialogForPad((s32 *)&OPEN_DIALOG, port);
            switch (OPEN_DIALOG.choice) {
            case 1:
                OPEN_MEMCARD_STATE = 0x1B;
                break;
            case 0:
            case 2:
                OPEN_MEMCARD_STATE = 1;
                break;
            }
            break;
        case 22:
            OPEN_MEMCARD_MESSAGE = 0x19;
            initDialog((u8 *)&OPEN_DIALOG, NULL, 1);
            runDialogForPad((s32 *)&OPEN_DIALOG, port);
            switch (OPEN_DIALOG.choice) {
            case 1:
                OPEN_MEMCARD_STATE = 0x1B;
                break;
            case 0:
            case 2:
                OPEN_MEMCARD_STATE = 0x11;
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
                    OPEN_MEMCARD_STATE = 0x10;
                }
            } else if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD_STATE = 1;
            }
            break;
        case 19:
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD_STATE = 0x11;
            } else if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                OPEN_MEMCARD_STATE = 0x16;
            }
            break;
        case 28:
            OPEN_MEMCARD.message = 1;
            if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                OPEN_MEMCARD.state = 0x10;
            } else if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD_STATE = 1;
            }
            break;
        case 21:
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD_STATE = 0x17;
            }
            break;
        case 11:
            if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                OPEN_MEMCARD_STATE = 0x10;
            } else if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD_STATE = 1;
            }
            break;
        case 14:
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                OPEN_MEMCARD_STATE = 1;
            }
            break;
        case 7:
            OPEN_MEMCARD_MESSAGE = 3;
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
            OPEN_MEMCARD_MESSAGE = 0xB;
            break;
        case 4:
            OPEN_MEMCARD_MESSAGE = 0xE;
            break;
        case 9:
            OPEN_MEMCARD_MESSAGE = 0x12;
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
            if (OPEN_MEMCARD_MODE == 6) {
                port = 0;
            }
            if (PAD_STATES[port]->pressed & 0x40) {
                OPEN_MEMCARD_READY = 1;
            }
            break;
        case 27:
            OPEN_MEMCARD_READY = 1;
            break;
        case 8:
            OPEN_MEMCARD_MESSAGE = 7;
            break;
        case 1:
            break;
        }
    } while (OPEN_MEMCARD_READY != 1);
    func_80014C08(FRAME_INTERVAL);
}

void OPEN_confirmOverwrite(s32 port) {
    char text[136];

    sprintf(text, "Will write over File%d.\nIs this OK?", ((SessionView *)D_8006E054)->saves[port].file + 1);
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
        ((SessionData *)D_8006E054)->unk1027 = 1;
        func_80014A90();
        break;
    case 0:
    case 2:
        OPEN_MEMCARD_STATE = 1;
        ((SessionData *)D_8006E054)->unk1027 = 0;
        break;
    }
}

void OPEN_confirmFormat(s32 pad) {
    initDialog((u8 *)&OPEN_DIALOG, "Format the Memory Card in Slot 1?", 1);
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

extern s8 OPEN_MEMCARD_SLOT;
extern u8 OPEN_MEMCARD_FREE_BLOCKS;

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
                ((SessionView *)D_8006E054)->saves[port].file = OPEN_MEMCARD.slot;
                if (OPEN_MEMCARD.mode == 0) {
                    OPEN_MEMCARD.state = 4;
                } else {
                    OPEN_MEMCARD.state = 9;
                    if ((OPEN_MEMCARD.slots[port] + OPEN_MEMCARD.slot)->unk56 < 6) {
                        changeScrollingBackground((OPEN_MEMCARD.slots[port] + OPEN_MEMCARD.slot)->unk56, 0x380, 0, 0x380, 0x80);
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
            ((SessionData *)D_8006E054)->unk1027 = 1;
            break;
        case 0:
        case 2:
            ((SessionData *)D_8006E054)->unk1027 = 0;
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

/* the save file names; the memory card functions take them as s32 */
s32 OPEN_SAVE_FILE_NAMES[3] = {
    (s32)"BASLUS-01328_A",
    (s32)"BASLUS-01328_B",
    (s32)"BASLUS-01328_C",
};

u8 *OPEN_formatSjisNumber(s32 value, s32 width, u8 *dst) {
    s32 i;
    s32 digit;
    s32 quot;
    s32 minus;

    minus = 0;
    if (width < 0) {
        if (value < 0) {
            *dst++ = 0x81;
            *dst++ = 0x7C;
        } else {
            *dst++ = 0x81;
            *dst++ = 0x7B;
        }
    } else if (value < 0) {
        minus = 1;
    }
    width = abs(width);
    value = abs(value);
    for (i = 0; i < width; i++) {
        *dst++ = 0x81;
        *dst++ = 0x40;
    }
    *dst = 0;
    if (minus) {
        width--;
    }
    do {
        if (width-- <= 0) {
            break;
        }
        dst--;
        quot = value / 10;
        digit = value % 10;
        *dst = digit + 0x4F;
        *--dst = (digit + 0x824F) >> 8;
        value = quot;
    } while (value != 0);
    if (minus) {
        *--dst = 0x7C;
        *--dst = 0x81;
    }
    return dst;
}

u8 *OPEN_formatSjisNumberZeros(s32 value, s32 width, u8 *dst) {
    s32 i;
    s32 digit;
    s32 quot;

    if (value < 0) {
        *dst++ = 0x81;
        *dst++ = 0x7C;
    } else if (width < 0) {
        *dst++ = 0x81;
        *dst++ = 0x7B;
    }
    width = abs(width);
    value = abs(value);
    for (i = 0; i < width; i++) {
        *dst++ = 0x82;
        *dst++ = 0x4F;
    }
    *dst = 0;
    do {
        if (width-- <= 0) {
            break;
        }
        dst--;
        quot = value / 10;
        digit = value % 10;
        *dst = digit + 0x4F;
        *--dst = (digit + 0x824F) >> 8;
        value = quot;
    } while (value != 0);
    return dst;
}

extern u8 D_801F8184;
void func_80055730(void);
void OPEN_applyLoadedSave();
void OPEN_prepareSaveData(s32 port);
s32 OPEN_readSavePreview(s32 port, s32 slot);
s32 OPEN_ensureMemoryCardReady(s32 port);
s32 OPEN_checkMemoryCard(s32 port);
s32 OPEN_waitMemoryCardSave(s32 part, s32 port);
s32 OPEN_waitMemoryCardLoad(s32 unused, s32 port);
s16 OPEN_countFreeBlocks(s32 port);
void OPEN_buildSaveHeader(s32 port, s32 slot);
u8 OPEN_checkSaveIsCurrent(s32 player, s32 port, s32 slot);

void OPEN_runMemcardAccess(void) {
    s32 player;
    s32 port;
    s32 slot;
    s32 status;
    s32 result;
    s32 i;
    s32 j;

    OPEN_MEMCARD_MESSAGE = -1;
    do {
        func_80014C08(FRAME_INTERVAL);
        player = OPEN_MEMCARD.card;
        port = ((SessionView *)D_8006E054)->saves[player].slot;
        slot = ((SessionView *)D_8006E054)->saves[player].file;
        status = 0;
        switch (OPEN_MEMCARD.state) {
        case 1:
            if (OPEN_MEMCARD_MODE == 7 && port == 0) {
                for (slot = 0; slot < 2; slot++) {
                    status = OPEN_checkMemoryCard(slot);
                    if (status != 0) {
                        break;
                    }
                }
            } else {
                status = OPEN_checkMemoryCard(port);
            }
            OPEN_MEMCARD.unk540 = 0;
            if (status == 1) {
                if (OPEN_MEMCARD.mode == 0) {
                    OPEN_MEMCARD.message = 4;
                    OPEN_MEMCARD.state = 2;
                } else {
                    OPEN_MEMCARD.message = 12;
                    OPEN_MEMCARD.state = 29;
                }
            } else if (status == 2) {
                if (OPEN_MEMCARD.mode == 0) {
                    OPEN_MEMCARD.state = 7;
                } else {
                    OPEN_MEMCARD.message = 3;
                    OPEN_MEMCARD.state = 11;
                }
            } else {
                if (OPEN_MEMCARD_MODE == 7 && port == 0) {
                    for (slot = 0; slot < 2; slot++) {
                        scanMemoryCardFiles(slot);
                        for (i = 0; i < 3; i++) {
                            OPEN_readSavePreview(slot, i);
                        }
                        status = (s8)(OPEN_MEMCARD.empty[slot][0] & OPEN_MEMCARD.empty[slot][1] & OPEN_MEMCARD.empty[slot][2]);
                        if (status == 1) {
                            OPEN_MEMCARD.messagePort = slot;
                            status = OPEN_checkMemoryCard(slot);
                            if (status == 1) {
                                OPEN_MEMCARD.message = 12;
                                OPEN_MEMCARD.state = 29;
                            } else if (status == 2) {
                                OPEN_MEMCARD.message = 3;
                                OPEN_MEMCARD.state = 11;
                            } else {
                                OPEN_MEMCARD.state = 28;
                            }
                            break;
                        }
                    }
                    if (OPEN_MEMCARD_STATE != 1) {
                        break;
                    }
                } else {
                    scanMemoryCardFiles(port);
                    for (i = 0; i < 3; i++) {
                        OPEN_readSavePreview(port, i);
                    }
                }
                OPEN_MEMCARD.freeBlocks = OPEN_countFreeBlocks(port);
                switch (OPEN_MEMCARD.mode) {
                case 0:
                    status = OPEN_MEMCARD_FREE_BLOCKS;
                    for (i = 0; i < 3; i++) {
                        if (OPEN_MEMCARD.empty[port][i] == 0) {
                            status += 2;
                        }
                    }
                    if (status < 2) {
                        OPEN_MEMCARD_STATE = 6;
                    } else {
                        OPEN_showSaveSlots();
                        OPEN_MEMCARD_STATE = 3;
                    }
                    break;
                case 7:
                case 0xFF:
                    status = (s8)(OPEN_MEMCARD.empty[port][0] & OPEN_MEMCARD.empty[port][1] & OPEN_MEMCARD.empty[port][2]);
                    if (status == 1) {
                        OPEN_MEMCARD.messagePort = port;
                        OPEN_MEMCARD.state = 28;
                    } else {
                        OPEN_showSaveSlots();
                        OPEN_MEMCARD_STATE = 10;
                    }
                    break;
                }
            }
            break;
        case 17:
            OPEN_MEMCARD_MESSAGE = -1;
            break;
        case 23:
            if (OPEN_MEMCARD_MODE == 6 && port == 0) {
                for (slot = 0; slot < 2; slot++) {
                    status = OPEN_checkSaveIsCurrent(slot, slot, ((SessionView *)D_8006E054)->saves[slot].file);
                    if (status != 3) {
                        break;
                    }
                }
            } else {
                status = OPEN_checkSaveIsCurrent(player, port, slot);
            }
            switch (status) {
            case 0:
            case 2:
                OPEN_MEMCARD.message = 13;
                OPEN_MEMCARD.state = 19;
                break;
            case 1:
                OPEN_MEMCARD.message = 12;
                OPEN_MEMCARD.state = 19;
                break;
            case 3:
                OPEN_MEMCARD.message = -1;
                OPEN_MEMCARD.state = 24;
                break;
            }
            break;
        case 24:
            status = OPEN_ensureMemoryCardReady(port);
            if (status == 1) {
                OPEN_MEMCARD.message = 12;
                OPEN_MEMCARD.state = 19;
                D_801F8184 = status;
            }
            break;
        case 4:
            OPEN_prepareSaveData(player);
            OPEN_buildSaveHeader(player, slot);
            writeSaveChecksum(0x2774, OPEN_MEMCARD.buffer);
            if (startMemoryCardSave(port, 2, (s32)OPEN_MEMCARD.buffer, OPEN_SAVE_FILE_NAMES[slot], (McHeader *)MEMORY_CARD_SAVE_HEADER) == -1) {
                OPEN_MEMCARD.state = 5;
            } else if (OPEN_waitMemoryCardSave(player, port) == -1) {
                OPEN_MEMCARD_STATE = 5;
            } else {
                OPEN_MEMCARD.message = 21;
                OPEN_MEMCARD.state = 25;
            }
            break;
        case 18:
            OPEN_prepareSaveData(player);
            OPEN_buildSaveHeader(player, slot);
            writeSaveChecksum(0x2774, OPEN_MEMCARD.buffer);
            if (startMemoryCardSave(port, 2, (s32)OPEN_MEMCARD.buffer, OPEN_SAVE_FILE_NAMES[slot], (McHeader *)MEMORY_CARD_SAVE_HEADER) == -1) {
                OPEN_MEMCARD.state = 20;
            } else if (OPEN_waitMemoryCardSave(player, port) == -1) {
                OPEN_MEMCARD_STATE = 20;
            } else {
                OPEN_MEMCARD.cancelled = 0;
                if (OPEN_MEMCARD.mode != 6 || player != 0) {
                    playMenuSound(1);
                    OPEN_MEMCARD.message = 22;
                    OPEN_MEMCARD.animPhase = 1;
                    OPEN_MEMCARD.state = 26;
                } else {
                    OPEN_MEMCARD.animPhase = 3;
                    OPEN_MEMCARD.state = 27;
                }
            }
            break;
        case 3:
        case 10:
            result = OPEN_ensureMemoryCardReady(port);
            if (result == 1) {
                D_801F8184 = result;
                OPEN_MEMCARD_STATE = result;
                OPEN_hideSaveSlots();
            }
            break;
        case 7:
            result = OPEN_ensureMemoryCardReady(port);
            if (result == 1) {
                D_801F8184 = result;
                OPEN_MEMCARD_STATE = result;
            }
            break;
        case 8:
            formatMemoryCard(port);
            OPEN_MEMCARD_STATE = 1;
            break;
        case 9:
            if (startMemoryCardLoad(port, (s32)OPEN_MEMCARD.buffer, OPEN_SAVE_FILE_NAMES[slot]) == -1) {
                OPEN_MEMCARD.state = 13;
                break;
            }
            OPEN_MEMCARD.animPhase = 0;
            if (OPEN_waitMemoryCardLoad(player, port) == -1) {
                OPEN_MEMCARD.state = 13;
                break;
            }
            if (verifySaveChecksum(((SaveSlot *)OPEN_MEMCARD.buffer)->size, OPEN_MEMCARD.buffer) == 1) {
                OPEN_MEMCARD.progress = 0;
                OPEN_MEMCARD.state = 15;
            } else {
                OPEN_MEMCARD.message = 20;
                OPEN_MEMCARD.state = 25;
                if (player == 0) {
                    if ((((PlayerProfile *)OPEN_MEMCARD.buffer)->unk20_0) == 1) {
                        func_80055730();
                    } else {
                        func_80055740();
                    }
                }
            }
            break;
        case 25:
            OPEN_MEMCARD.animPhase = 1;
            playMenuSound(1);
            OPEN_applyLoadedSave(player, port, slot);
            OPEN_MEMCARD.state = 12;
            break;
        case 20:
            D_801F8184 = 1;
            OPEN_MEMCARD.message = 15;
            OPEN_MEMCARD.state = 21;
            break;
        case 13:
            D_801F8184 = 1;
            OPEN_MEMCARD.message = 19;
            OPEN_MEMCARD.state = 14;
            break;
        case 29:
            break;
        }
    } while (OPEN_MEMCARD_READY != 1);
    func_80014C08(FRAME_INTERVAL);
}

void OPEN_applyLoadedSave(s32 port, s32 slot, s32 file) {
    PlayerProfile *src;

    switch (OPEN_MEMCARD_MODE) {
    case 0:
        break;
    case 7:
    case 0xFF:
        src = OPEN_MEMCARD_BUFFER;
        ((PlayerProfile *)PLAYER_PROFILES)[port] = *src;
        ((SessionView *)D_8006E054)->saves[port].playTime = src->playTime;
        break;
    }
}

extern u8 D_801F80C0;

void OPEN_prepareSaveData(s32 port) {
    PlayerProfile *buffer;

    buffer = OPEN_MEMCARD.buffer;
    if (OPEN_MEMCARD.mode == 6) {
        OPEN_MEMCARD.progress = port * 50;
    } else {
        OPEN_MEMCARD_PROGRESS = 0;
    }
    switch (OPEN_MEMCARD_MODE) {
    case 0:
        PLAYER_DATA(port).unkF = 1;
        PLAYER_DATA(port).unk28_13 = D_801F80C0;
        ((SessionView *)D_8006E054)->saves[port].playTime = PLAYER_DATA(port).playTime;
        *buffer = PLAYER_DATA(port);
        return;
    case 2:
    case 4:
    case 5:
    case 8:
        if (PLAYER_DATA(port).unkD < 255) {
            PLAYER_DATA(port).unkD++;
        }
    case 6:
        ((SessionView *)D_8006E054)->saves[port].playTime = PLAYER_DATA(port).playTime;
        *buffer = PLAYER_DATA(port);
        break;
    }
}


s32 OPEN_readSavePreview(s32 port, s32 slot) {
    s16 i;

    OPEN_MEMCARD.empty[port][slot] = 1;
    for (i = 0; i < 5; i++) {
        if ((OPEN_MEMCARD.empty[port][slot] = readMemoryCardSavePreview(port, &OPEN_MEMCARD.slots[port][slot], OPEN_SAVE_FILE_NAMES[slot])) == 0) {
            OPEN_MEMCARD.unk540++;
            return 0;
        }
    }
    return 1;
}

s32 OPEN_ensureMemoryCardReady(s32 port) {
    OPEN_MEMCARD_MESSAGE_PORT = port;
    return ensureMemoryCardReady(port);
}

s32 OPEN_checkMemoryCard(s32 port) {
    switch (OPEN_MEMCARD.mode) {
    case 6:
    case 7:
        OPEN_MEMCARD.message = 2;
        break;
    default:
        OPEN_MEMCARD_MESSAGE = 0;
        break;
    }
    OPEN_MEMCARD_MESSAGE_PORT = port;
    return getMemoryCardStatus(port);
}

s32 OPEN_waitMemoryCardSave(s32 part, s32 port) {
    s32 result;

    while (1) {
        func_80014C08(FRAME_INTERVAL);
        if (OPEN_ensureMemoryCardReady(port) != 0) {
            OPEN_MEMCARD.progress = 0;
            return -1;
        }
        result = stepMemoryCardSave();
        if (result == -1) {
            OPEN_MEMCARD.progress = 0;
            return -1;
        }
        if (OPEN_MEMCARD.mode == 6) {
            OPEN_MEMCARD.progress = part * 50 + result / 2;
        } else {
            OPEN_MEMCARD.progress = result;
        }
        if (MEMORY_CARD_SECTORS_DONE == MEMORY_CARD_SECTORS_TOTAL) {
            return 0;
        }
    }
}

s32 OPEN_waitMemoryCardLoad(s32 unused, s32 port) {
    s32 result;

    while (1) {
        func_80014C08(FRAME_INTERVAL);
        if (OPEN_ensureMemoryCardReady(port) != 0) {
            OPEN_MEMCARD.progress = 0;
            return -1;
        }
        result = stepMemoryCardLoad();
        if (result == -1) {
            OPEN_MEMCARD.progress = 0;
            return -1;
        }
        OPEN_MEMCARD.progress = result;
        if (MEMORY_CARD_SECTORS_DONE == MEMORY_CARD_SECTORS_TOTAL) {
            return 0;
        }
    }
}

s16 OPEN_countFreeBlocks(s32 port) {
    return 15 - MEMORY_CARD_DIRECTORIES[port]->blocks;
}

typedef struct {
    u8 data[0x20];
} IconClut;
typedef struct {
    u8 data[0x180];
} IconImage;
typedef struct {
    /* 0x00 */ char magic[2];
    /* 0x02 */ u8 type;
    /* 0x03 */ u8 blocks;
    /* 0x04 */ char title[64];
    /* 0x44 */ u8 reserve[28];
    /* 0x60 */ IconClut clut;
    /* 0x80 */ IconImage icon;
} SaveHeader;
void StoreImage(Rect16 *rect, void *p);

void OPEN_buildSaveHeader(s32 port, s32 slot) {
    SaveHeader *header = (SaveHeader *)MEMORY_CARD_SAVE_HEADER;
    IconImage icon;
    IconClut clut;
    s16 iconX[3] = { 0x158, 0x15C, 0x160 };
    Rect16 iconRect;
    Rect16 clutRect;
    char title[72];
    u8 file[16];
    u8 hours[16];
    u8 minutes[16];
    s32 time;
    s32 hour;
    s32 minute;
    s32 i;

    iconRect.x = iconX[slot];
    iconRect.y = 0x154;
    iconRect.w = 4;
    iconRect.h = 0x30;
    StoreImage(&iconRect, &icon);
    clutRect.x = 0x170;
    clutRect.y = slot + 0x1FA;
    clutRect.w = 0x10;
    clutRect.h = 1;
    StoreImage(&clutRect, &clut);
    DrawSync(0);
    bzero((Scene3D *)title, 0x42);
    time = ((SessionView *)D_8006E054)->saves[port].playTime;
    hour = time / 216000;
    minute = (time - hour * 216000) / 3600;
    if (hour >= 1000) {
        hour = 999;
        minute = 59;
    }
    OPEN_formatSjisNumber(slot + 1, 1, file);
    OPEN_formatSjisNumber(hour, 3, hours);
    OPEN_formatSjisNumberZeros(minute, 2, minutes);
    /* "ＤＣＢ［%s］%s：%s" */
    sprintf(title, "\x82" "c" "\x82" "b" "\x82" "a" "\x81" "m%s" "\x81" "n%s" "\x81" "F%s", file, hours, minutes);
    header->magic[0] = 'S';
    header->magic[1] = 'C';
    header->type = 0x13;
    header->blocks = 2;
    for (i = 0; i < 64; i++) {
        header->title[i] = 0;
    }
    strcpy(header->title, title);
    for (i = 0; i < 28; i++) {
        header->reserve[i] = 0;
    }
    header->clut = clut;
    header->icon = icon;
}

u8 OPEN_checkSaveIsCurrent(s32 player, s32 port, s32 slot) {
    SaveSlot *save;
    s32 status;

    save = &OPEN_MEMCARD.slots[port][slot];
    status = OPEN_checkMemoryCard(port);
    if (status == 0) {
        scanMemoryCardFiles(port);
        OPEN_readSavePreview(port, slot);
        if (OPEN_MEMCARD.empty[port][slot] == 0) {
            if ((u16)((PlayerProfile *)PLAYER_PROFILES)[player].unk10 != save->unk10) {
                return 0;
            }
            return 3;
        }
        OPEN_MEMCARD_MESSAGE_PORT = port;
    }
    return status;
}

/* The data the overlay starts with zeroed. The names that code uses inside
   OPEN_MEMCARD are in config/undefined_syms_openseg.txt. */
DecEnv OPEN_DEC_ENV = { { 0 } };
u8 *OPEN_MOVIE_IMAGE_BUFFER = NULL;
u8 *OPEN_MOVIE_VLC_BUFFER = NULL;
s32 OPEN_MOVIE_FRAMES_SHOWN = 0;
u16 *OPEN_VLC_TABLE = NULL;
s8 D_801F0850 = 0;
s32 OPEN_MOVIE_FILE_SECTOR = 0;
s32 OPEN_RING_FREE_SECTORS = 0;
u32 *OPEN_STREAM_RING = NULL;
s32 OPEN_RING_OVER_SECTORS = 0;
CdLocation OPEN_MOVIE_START_LOC = { 0 };
s8 OPEN_MOVIE_ENDED = 0;
CdLocation OPEN_MOVIE_LOC = { 0 };
u32 OPEN_MOVIE_FRAME = 0;
s8 OPEN_MOVIE_STARTED = 0;
s32 OPEN_MOVIE_END_FRAME = 0;
/* not referenced by any code */
s32 D_801F087C = 0xAFB40020;
UiWindow OPEN_TRADE_LIST_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F08C4[0xC] = { 0 };
UiWindow OPEN_TRADE_OK_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F0914[0xC] = { 0 };
PlayerWindow OPEN_CARD_INFO_WINDOWS[2] = { { { 0 } } };
PlayerWindow OPEN_CARD_LIST_WINDOWS[2] = { { { 0 } } };
MessageWindow OPEN_TRADE_WARNING_WINDOWS[2] = { { { 0 } } };
CursorHighlight OPEN_CARD_LIST_CURSORS[2] = { { { { 0 } } } };
PlayerWindow OPEN_SORT_MENU_WINDOWS[2] = { { { 0 } } };
CursorHighlight OPEN_SORT_MENU_CURSORS[2] = { { { { 0 } } } };
CardEntry *OPEN_TRADE_CARD_LISTS[2][301] = { { 0 } };
/* not referenced by any code */
u8 D_801F1608[0x8] = { 0 };
s16 OPEN_TRADE_PICKS[2][3] = { { 0 } };
u32 *OPEN_CARD_IMAGE_ARC = NULL;
s8 OPEN_TRADABLE_COUNTS[2][301] = { { 0 } };
/* not referenced by any code */
s16 D_801F187A = 0;
s32 D_801F187C = 0;
s8 OPEN_DECK_CARD_COUNTS[2][301] = { { 0 } };
/* not referenced by any code */
s16 D_801F1ADA = 0;
s32 D_801F1ADC = 0;
s32 OPEN_TRADE_PLAYER_READY[2] = { 0 };
s32 OPEN_TRADE_STATE = 0;
/* not referenced by any code */
s32 D_801F1AEC = 0;
s16 OPEN_TRADE_PICK_X[2][3] = { { 0 } };
s32 OPEN_TRADE_BANNER_SHOWN = 0;
s32 OPEN_TRADE_BANNER_Y = 0;
s32 OPEN_TRADE_QUIT = 0;
s32 OPEN_SORT_PLAYER = 0;
/* not referenced by any code */
s32 D_801F1B0C = 0;
UiWindow OPEN_MESSAGE_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F1B54[0xC] = { 0 };
UiWindow OPEN_PLAYER_NAME_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F1BA4[0xC] = { 0 };
UiWindow OPEN_IMAGE_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F1BF4[0x200C] = { 0 };
POLY_FT4 OPEN_TITLE_PART_PRIMS[2][40] = { { { 0 } } };
s32 D_801F4880 = 0;
s32 D_801F4884 = 0;
s32 OPEN_INTRO_IMAGE = 0;
s32 OPEN_INTRO_SHOWN_IMAGE = 0;
s32 OPEN_INTRO_IMAGE_FADE = 0;
/* not referenced by any code */
u8 D_801F4894[0x46C] = { 0 };
u16 OPEN_WHITE_CLUT[256] = { 0 };
s32 D_801F4F00 = 0;
s32 D_801F4F04 = 0;
s32 D_801F4F08 = 0;
s32 D_801F4F0C = 0;
s32 D_801F4F10 = 0;
s32 D_801F4F14 = 0;
s32 D_801F4F18 = 0;
s32 D_801F4F1C = 0;
s32 OPEN_TITLE_PART_COUNT = 0;
/* not referenced by any code */
s32 D_801F4F24 = 0;
TextScroll OPEN_INTRO_TEXT = { 0 };
UiWindow OPEN_NAME_ENTRY_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F4F8C[0xC] = { 0 };
CursorHighlight OPEN_NAME_ENTRY_CURSOR = { { { 0 } } };
UiWindow OPEN_NAME_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F502C[0xC] = { 0 };
CursorHighlight OPEN_NAME_CARET = { { { 0 } } };
UiWindow OPEN_NAME_HELP_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F50CC[0xC] = { 0 };
NameEntry OPEN_NAME_ENTRY = { 0 };
/* not referenced by any code */
s32 D_801F50F4 = 0;
UiWindow OPEN_STARTER_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F513C[0xC] = { 0 };
CursorHighlight OPEN_STARTER_CURSOR = { { { 0 } } };
StarterSelect OPEN_STARTER_SELECT = { 0 };
/* not referenced by any code */
s16 D_801F519A = 0;
s32 D_801F519C = 0;
s32 OPEN_TITLE_STATE = 0;
/* not referenced by any code */
u8 D_801F51A4[0xC] = { 0 };
POLY_FT4 OPEN_TITLE_SPIN_QUADS[2][2] = { { { 0 } } };
POLY_F4 D_801F5250[2] = { { 0 } };
DR_MODE D_801F5280[2] = { { 0 } };
s32 OPEN_TITLE_SPIN_RADIUS = 0;
s32 OPEN_TITLE_SPIN_ANGLE = 0;
s32 OPEN_TITLE_SPIN_SIZE = 0;
s32 OPEN_TITLE_SPIN_SHADE = 0;
s32 OPEN_TITLE_TIMER = 0;
s32 D_801F52A4 = 0;
s32 D_801F52A8 = 0;
s32 D_801F52AC = 0;
s32 D_801F52B0 = 0;
s32 OPEN_PRESS_START_SHADE = 0;
s32 OPEN_PRESS_START_STEP = 0;
u8 D_801F52BC[3] = { 0 };
u8 D_801F52C0[3] = { 0 };
s32 D_801F52C4 = 0;
s32 D_801F52C8 = 0;
/* not referenced by any code */
s32 D_801F52CC = 0;
s32 OPEN_FRIEND_MENU_SHOWN = 0;
s32 OPEN_FRIEND_MENU_DONE = 0;
/* not referenced by any code */
u8 D_801F52D8[0x8] = { 0 };
PlayerWindow OPEN_PLAYER_RECORD_WINDOWS[2] = { { { 0 } } };
s32 OPEN_TRADE_ENABLED = 0;
/* not referenced by any code */
s32 D_801F5374 = 0;
u8 *OPEN_SAVE_PLACE_IMAGES = NULL;
/* not referenced by any code */
s32 D_801F537C = 0;
UiWindow OPEN_MEMCARD_MESSAGE_WINDOW = { 0 };
/* not referenced by any code */
s32 D_801F53C4 = 0;
UiWindow OPEN_OPERATION_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F540C[0x277C] = { 0 };
MemcardScreen OPEN_MEMCARD = { { { 0 } } };
/* not referenced by any code */
s32 D_801F80CC = 0;
Window OPEN_DIALOG = { { 0 } };
/* not referenced by any code */
s16 D_801F8176 = 0;
u8 D_801F8178[0xC] = { 0 };
u8 D_801F8184 = 0;
/* not referenced by any code */
u8 D_801F8185 = 0;
s16 D_801F8186 = 0;
