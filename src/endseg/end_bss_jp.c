#include "common.h"
#include "game.h"
#include "dcb/openseg.h"

/* jp's ENDSEG data that starts zeroed: the records' specialty counts, then
   the data of the movie player jp links into ENDSEG (us's and eu's is
   OPENSEG's, open_bss.c), with the vblank counts jp's player keeps */
s32 END_SPECIALTY_WINS[8] = { 0 };
s32 END_SPECIALTY_LOSSES[8] = { 0 };
s32 END_SPECIALTY_CARDS[8] = { 0 };
s32 END_SPECIALTY_ORDER[6] = { 0 };
DecEnv OPEN_DEC_ENV = { { 0 } };
u8 *OPEN_MOVIE_IMAGE_BUFFER = NULL;
u8 *OPEN_MOVIE_VLC_BUFFER = NULL;
s32 OPEN_MOVIE_FRAMES_SHOWN = 0;
u16 *OPEN_VLC_TABLE = NULL;
s8 OPEN_MOVIE_BUFFER_INDEX = 0;
s32 OPEN_MOVIE_FILE_SECTOR = 0;
s32 OPEN_RING_FREE_SECTORS = 0;
u32 *OPEN_STREAM_RING = NULL;
s32 OPEN_RING_OVER_SECTORS = 0;
CdLocation OPEN_MOVIE_START_LOC = { 0 };
s32 OPEN_MOVIE_VSYNC_DELTA = 0;
s8 OPEN_MOVIE_ENDED = 0;
s32 OPEN_MOVIE_VSYNC = 0;
CdLocation OPEN_MOVIE_LOC = { 0 };
u32 OPEN_MOVIE_FRAME = 0;
s8 OPEN_MOVIE_STARTED = 0;
s32 OPEN_MOVIE_END_FRAME = 0;
s32 OPEN_MOVIE_LAST_VSYNC = 0;
