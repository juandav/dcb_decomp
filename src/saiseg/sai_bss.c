#include "common.h"
#include "game.h"
#include "dcb/menu.h"
#include "dcb/saiseg.h"

/* the u8 arrays among these are not referenced by any code */
UiWindow SAI_STATS_HINT_WINDOW = { 0 };
u8 D_801F437C[12] = { 0 };
CursorHighlight SAI_PLAYER_DATA_CURSOR = { { { 0 } } };
UiWindow SAI_WORD_GRID_WINDOW = { 0 };
u8 D_801F441C[12] = { 0 };
CursorHighlight SAI_WORD_GRID_CURSOR = { { { 0 } } };
UiWindow SAI_KEYWORD_WINDOW = { 0 };
u8 D_801F44BC[12] = { 0 };
CursorHighlight SAI_KEYWORD_CURSOR = { { { 0 } } };
UiWindow SAI_WORD_HELP_WINDOW = { 0 };
u8 D_801F455C[12] = { 0 };
WordInput SAI_WORD_INPUT = { 0 };
AreaState SAI_AREA = { { { 0 } } };
u8 D_801F46AC[4] = { 0 };
UiWindow SAI_ERROR_WINDOWS[5] = { { 0 } };
Splash *SAI_SPLASH = NULL;
u8 SAI_HACK_OVERLAY_STATE = 0;
u8 D_801F480C[4] = { 0 };
PlayerStats SAI_PLAYER_STATS = { { 0 } };
u8 SAI_HACK_OVERLAY_FLICKER = 0;
ScriptRunner *SAI_SCRIPT[1] = { NULL };
u8 D_801F483C[4] = { 0 };
TextLine SAI_TEXT_LINES[3] = { { { 0 } } };
u8 D_801F4900[8] = { 0 };
OpponentList SAI_OPPONENTS = { { 0 } };
u8 D_801F4A34[4] = { 0 };
u8 SAI_DIALOG[0xB8] = { 0 };
OpponentInfo *SAI_OPPONENT_INFO = NULL;
u8 D_801F4AF4[4] = { 0 };
s16 SAI_SCRIPT_REWARD_CARDS[3] = { 0 };
u8 D_801F4AFE = 0;
u8 SAI_REWARD_FROM_SCRIPT = 0;
UiWindow SAI_PLAYER_DATA_WINDOW = { 0 };
UiWindow SAI_MESSAGE_WINDOW = { 0 };
u8 D_801F4B88[0xD0] = { 0 };
s16 SAI_MAP_FRAME_BRIGHTNESS = 0;
u8 D_801F4C5C[0x1EC] = { 0 };
WorldMap SAI_WORLD_MAP = { 0 };
u8 D_801F5254[4] = { 0 };
MenuTab SAI_MAP_MENU_TAB = { 0 };
Sprite3D *SAI_SPRITES[60] = { NULL };
MapAnim *SAI_MAP_ANIMS = NULL;
u8 D_801F5354[4] = { 0 };
RewardScreen *SAI_REWARD_SCREEN = NULL;
u8 D_801F535C[4] = { 0 };
UiWindow SAI_PARTNER_GET_WINDOW = { 0 };
u8 D_801F53A4[12] = { 0 };
CursorHighlight SAI_PARTNER_GET_CURSOR = { { { 0 } } };
PartnerCursor SAI_PARTNER_CURSOR = { 0 };
u8 D_801F5404[4] = { 0 };
PartnerList SAI_PARTNER_LIST = { 0 };
UiWindow SAI_DIGI_PARTS_WINDOW = { 0 };
u8 D_801F5454[4] = { 0 };
u8 SAI_OWNED_DIGI_PARTS[16] = { 0 };
