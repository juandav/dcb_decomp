#include "common.h"
#include "game.h"
#include "dcb/intseg.h"

/* the windows the intro opens */
u8 *INT_WINDOWS[8] = { 0 };
/* the state of the intro */
IntState INT_STATE = { { { { 0 } } } };
