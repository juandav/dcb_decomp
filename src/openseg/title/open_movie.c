#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/display.h"
#include "dcb/vblank.h"
#include "dcb/pad.h"
#include "dcb/openseg.h"
#include "dcb/render_loop.h"

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

typedef struct {
    u8 val0;
    u8 val1;
    u8 val2;
    u8 val3;
} CdAttenuation;

typedef struct {
    u8 pos[4];
    u32 size;
    char name[16];
} CdFileEntry;

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

typedef struct {
    s32 id;
    s32 sector;
    s32 endFrame;
} Movie;

typedef struct {
    s32 heights[5];
} MovieHeights;

extern s32 OPEN_MOVIE_FILE_SECTOR;
extern DecEnv OPEN_DEC_ENV;
extern CdLocation OPEN_MOVIE_START_LOC;
extern CdLocation OPEN_MOVIE_LOC;
extern s8 OPEN_MOVIE_STARTED;
extern u8 *OPEN_MOVIE_IMAGE_BUFFER;
extern u8 *OPEN_MOVIE_VLC_BUFFER;
extern s32 OPEN_MOVIE_FRAMES_SHOWN;
extern u16 *OPEN_VLC_TABLE;
extern u32 *OPEN_STREAM_RING;
extern u32 OPEN_MOVIE_FRAME;
extern s8 OPEN_MOVIE_ENDED;
extern s32 OPEN_MOVIE_END_FRAME;
extern s32 OPEN_RING_FREE_SECTORS;
extern s32 OPEN_RING_OVER_SECTORS;
extern s8 OPEN_MOVIE_BUFFER_INDEX;
extern s32 StCdIntrFlag;

void StUnSetRing(void);
s32 DecDCTvlc2(u32 *bs, u32 *buf, u16 *table);
s32 StFreeRing(u32 *base);
void SpuSetCommonAttr(SpuCommonAttr *attr);
s32 CdMix(CdAttenuation *atv);
void SsSetSerialAttr(s8 s_num, s8 attr, s8 mode);
void SsSetSerialVol(s8 s_num, s16 voll, s16 volr);
void DecDCTReset(s32 mode);
void DecDCToutCallback(void (*func)());
void StSetRing(u32 *ring_addr, u32 ring_size);
void StClearRing(void);
void StSetStream(u32 mode, u32 start_frame, u32 end_frame, void (*func1)(), void (*func2)());
s32 StGetNext(u32 **addr, StHeader **header);
s32 CdRead2(s32 mode);
void DecDCTvlcBuild(u16 *table);
void StRingStatus(s32 *freeSectors, s32 *overSectors);
void MoveImage(Rect16 *rect, s32 x, s32 y);
void DecDCTin(u8 *buf, s32 mode);
void DecDCTout(u8 *buf, s32 size);
s32 StGetBackloc(CdLocation *loc);
void StCdInterrupt(void);
void OPEN_startCdStream(CdLocation *loc);
void OPEN_initMovieStream(CdLocation *loc, void (*callback)());
void OPEN_initDecEnv(DecEnv *env, s32 x0, s32 y0, s32 x1, s32 y1);
s32 OPEN_decodeMovieFrame(DecEnv *env);
void OPEN_uploadMovieSlice(void);
void OPEN_waitMovieFrame(DecEnv *env, s32 unused);
void OPEN_runMovieRenderLoop();

/* jp links the movie player into ENDSEG and has five movies */
#if VERSION_JP
/* the vblank count when each frame was shown, and the vblanks since the last */
extern s32 OPEN_MOVIE_VSYNC;
extern s32 OPEN_MOVIE_VSYNC_DELTA;
extern s32 OPEN_MOVIE_LAST_VSYNC;

const MovieHeights OPEN_MOVIE_HEIGHTS = { { 0xA0, 0xF0, 0xF0, 0xA0, 0xB0 } };

/* the movies */
Movie OPEN_MOVIES[5] = {
    { 0, 0, 0x11C1 },
    { 1, 0x5910, 0xD34 },
    { 2, 0xDDA8, 0x52 },
    { 3, 0xE130, 0x11E5 },
    { 4, 0x13AF0, 0x117D },
};
#elif VERSION_US || VERSION_EU
/* not referenced by any code */
const s32 D_801DDF38 = 8;

const MovieHeights OPEN_MOVIE_HEIGHTS = { { 0xA0, 0xB0, 0xF0, 0xA0, 0xB0 } };

/* the opening movies */
Movie OPEN_MOVIES[3] = {
    { 0, 0, 0xB36 },
    { 1, 0x3A18, 0x717 },
    { 2, 0x8190, 0x52 },
};
#else
#error "openseg/title/open_movie: version not checked"
#endif

s32 OPEN_MOVIE_HEIGHT = 0xB0;

void OPEN_searchCdFile(void *file, char *name) {
    s32 found;

    while (1) {
        CdSync(0, 0);
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
#if VERSION_JP
    DrawSync(0);
#endif
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
    CdMix(&atv);
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

void OPEN_startMovie(s32 sector, s32 endFrame, s32 volume, s32 frames, s32 height) {
    s32 i;
    s32 y;

    OPEN_MOVIE_HEIGHT = height;
    OPEN_MOVIE_END_FRAME = endFrame;
    OPEN_muteCdAudio();
    OPEN_clearScreen(0, 0, 0);
    waitFrames(2);
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

/* a frame callback: called with the frame buffer and its index */
void OPEN_showMovieFrame(FrameBuffer *fb, s32 bufferIndex) {
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
#if VERSION_JP
    OPEN_MOVIE_VSYNC = VSync(-1);
    OPEN_MOVIE_VSYNC_DELTA = OPEN_MOVIE_VSYNC - OPEN_MOVIE_LAST_VSYNC;
    OPEN_MOVIE_LAST_VSYNC = OPEN_MOVIE_VSYNC;
#endif
    if (OPEN_MOVIE_STARTED != 0 || bufferIndex != 0) {
        OPEN_MOVIE_STARTED = 1;
        OPEN_MOVIE_BUFFER_INDEX = bufferIndex;
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
    *callbacks++ = (s32)OPEN_showMovieFrame;
    *callbacks = 0;
    DecDCTReset(0);
    DecDCToutCallback(callback);
    StSetRing(OPEN_STREAM_RING, 0x40);
#if VERSION_US || VERSION_EU
    StClearRing();
#endif
    StSetStream(1, 1, -1, 0, 0);
    OPEN_startCdStream(loc);
}

void OPEN_stopMovie(void) {
    VSync(0);
    OPEN_MOVIE_ENDED = 1;
    OPEN_muteCdAudio();
    waitFrames(2);
    freeHeapBlock(OPEN_MOVIE_IMAGE_BUFFER);
    freeHeapBlock(OPEN_MOVIE_VLC_BUFFER);
    freeHeapBlock(OPEN_STREAM_RING);
    freeHeapBlock(OPEN_VLC_TABLE);
    DecDCToutCallback(0);
    StUnSetRing();
    OPEN_setSpuVolume(0x3FFF);
}

void OPEN_uploadMovieSlice(void) {
    Rect16 snap;
    s32 id;

    /* StCdInterrupt raises it when DMA was busy and it left a sector for later */
    if (StCdIntrFlag != 0) {
        StCdInterrupt();
        StCdIntrFlag = 0;
    }
    id = OPEN_DEC_ENV.imgid;
    snap = OPEN_DEC_ENV.slice;
    OPEN_DEC_ENV.imgid = OPEN_DEC_ENV.imgid == 0;
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
            CdSync(0, 0);
        }
        while (CdControlB(0xE, param, 0) == 0) {
            CdSync(0, 0);
        }
        VSync(3);
    } while (CdRead2(0x1E0) == 0);
}

/* main declares it s32 (dcb/overlay_calls.h); nothing is returned */
#if VERSION_JP
/* clears the whole of VRAM's two frame buffers to black */
void OPEN_clearVram(void) {
    Rect16 rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 480;
    rect.h = 512;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
}

/* jp shows the movie through the executable's render loop */
s32 OPEN_playMovie(s32 index) {
    MovieHeights movie = OPEN_MOVIE_HEIGHTS;
    s32 wait;

    endTask(0x1F);
    waitFrames(FRAME_INTERVAL);
    OPEN_clearVram();
    resetDisplay(320, 240, 1);
    ((Graphics *)&GRAPHICS)->displayStartCounter = 0;
    ((Graphics *)&GRAPHICS)->vblanksPerFrame = 2;
    spawnTask(0x1F, 0, 0, 0x1000, runRenderLoop);
    waitFrames(60);
    OPEN_startMovie(OPEN_MOVIES[index].sector, OPEN_MOVIES[index].endFrame, 0x3FFF, 1, movie.heights[index]);
    while (!(PAD_STATES[0]->rawRepeat & 0x800) && OPEN_MOVIE_ENDED == 0) {
        yieldTask();
    }
    wait = 30;
    while (!(PAD_STATES[0]->rawRepeat & 0x860)) {
        if (--wait == -1) {
            break;
        }
        yieldTask();
    }
    endTask(0x1F);
    OPEN_clearVram();
    OPEN_stopMovie();
    resetDisplay(320, 240, 0);
    waitFrames(0x3C);
}
#elif VERSION_US || VERSION_EU
s32 OPEN_playMovie(s32 index) {
    MovieHeights movie = OPEN_MOVIE_HEIGHTS;
    s32 wait;

    endTask(0x1F);
    endTask(0x19);
    waitFrames(0x10);
    OPEN_clearScreen(0, 0, 0);
    resetDisplay(320, 240, 1);
    ((Graphics *)&GRAPHICS)->displayStartCounter = -30;
    ((Graphics *)&GRAPHICS)->vblanksPerFrame = 2;
    spawnTask(0x1F, 0, 0, 0x1000, OPEN_runMovieRenderLoop);
    waitFrames(0x1E);
    setTaskVsyncMode(0);
    OPEN_startMovie(OPEN_MOVIES[index].sector, OPEN_MOVIES[index].endFrame, 0x3FFF, 1, movie.heights[index]);
    while (!(PAD_STATES[0]->repeat & 0x800) && OPEN_MOVIE_ENDED == 0) {
        yieldTask();
    }
    wait = 30;
    while (!(PAD_STATES[0]->repeat & 0x860)) {
        if (--wait == -1) {
            break;
        }
        yieldTask();
    }
    endTask(0x1F);
    OPEN_clearScreen(0, 0, 0);
    OPEN_stopMovie();
    resetDisplay(320, 240, 0);
    waitFrames(0x3C);
}

void OPEN_runMovieRenderLoop(void) {
    Graphics *gfx;

    gfx = (Graphics *)&GRAPHICS;
    gfx->frameCallbacks[0] = 0;
    VBLANK_COUNTER = 0;
    SetDispMask(0);
    for (; gfx->displayStartCounter <= 0; gfx->displayStartCounter++) {
        pollPads();
        waitFrames(1);
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
        if (gfx->frameCallbacks[0] != 0) {
            OPEN_showMovieFrame(CURRENT_FRAME_BUFFER, FRAME_BUFFER_INDEX);
        }
        VSync(0);
        PutDispEnv(&CURRENT_FRAME_BUFFER->disp);
        PutDrawEnv(&CURRENT_FRAME_BUFFER->draw);
        yieldTask();
        gfx->vblanksPerFrame = VBLANK_COUNTER;
        if (VBLANK_COUNTER == 0) {
            gfx->vblanksPerFrame = 1;
        }
        VBLANK_COUNTER = 0;
    }
}
#endif
