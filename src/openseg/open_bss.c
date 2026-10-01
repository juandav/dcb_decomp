#include "common.h"
#include "game.h"
#include "dcb/menu.h"
#include "dcb/openseg.h"

/* The data the overlay starts with zeroed. The names that code uses inside
   OPEN_MEMCARD are in config/<version>/undefined_syms_openseg.txt. */
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
s8 OPEN_MOVIE_ENDED = 0;
CdLocation OPEN_MOVIE_LOC = { 0 };
u32 OPEN_MOVIE_FRAME = 0;
s8 OPEN_MOVIE_STARTED = 0;
s32 OPEN_MOVIE_END_FRAME = 0;
/* not referenced by any code; a leftover value in us, zero in eu */
#if VERSION_US
s32 D_801F087C = 0xAFB40020;
#elif VERSION_EU
s32 D_801F087C = 0;
#else
#error "openseg/open_bss: version not checked"
#endif
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
s32 OPEN_LABEL_X = 0;
s32 OPEN_LABEL_Y = 0;
s32 OPEN_INTRO_IMAGE = 0;
s32 OPEN_INTRO_SHOWN_IMAGE = 0;
s32 OPEN_INTRO_IMAGE_FADE = 0;
/* not referenced by any code */
u8 D_801F4894[0x46C] = { 0 };
u16 OPEN_WHITE_CLUT[256] = { 0 };
s32 OPEN_PANEL_X = 0;
s32 OPEN_PANEL_Y = 0;
s32 OPEN_CORNER_X = 0;
s32 OPEN_CORNER_Y = 0;
s32 OPEN_SIDEBAR_X = 0;
s32 OPEN_SIDEBAR_Y = 0;
s32 OPEN_FRAME_X = 0;
s32 OPEN_FRAME_Y = 0;
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
POLY_F4 OPEN_TITLE_BAND_QUADS[2] = { { 0 } };
DR_MODE OPEN_TITLE_BAND_MODES[2] = { { 0 } };
s32 OPEN_TITLE_SPIN_RADIUS = 0;
s32 OPEN_TITLE_SPIN_ANGLE = 0;
s32 OPEN_TITLE_SPIN_SIZE = 0;
s32 OPEN_TITLE_SPIN_SHADE = 0;
s32 OPEN_TITLE_TIMER = 0;
s32 OPEN_TITLE_BANNER_X = 0;
s32 OPEN_TITLE_LOGO_RISE = 0;
s32 OPEN_TITLE_FOOTER_SHADE = 0;
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
u8 OPEN_MEMCARD_READY_RESULT = 0;
/* not referenced by any code */
u8 D_801F8185 = 0;
s16 D_801F8186 = 0;
