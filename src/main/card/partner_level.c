#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/partner_level.h"
#include "dcb/card_db.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/save_checksum.h"
#include "dcb/player_rank.h"
#include "dcb/menu.h"
#include "dcb/dialog.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
#include "dcb/scroll_bg.h"
#include "dcb/vblank.h"
#include "dcb/screen_copy.h"
#include "dcb/render_loop.h"
#include "dcb/boot.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"

s32 findNewPartnerAbility(AbilityLearnEntry *abilityTable, s32 player, s32 slot) {
    s32 level;
    s32 partnerIndex;
    s32 i;

    level = (s8)((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].level;
    partnerIndex = getSlotPartnerIndex(player, slot);
    if (partnerIndex >= 0) {
        for (i = 0; i < 0x80; i++) {
            if (abilityTable[i].learnLevels[partnerIndex] == level) {
                if (getPartnerAbilityState(player, i) == 0) {
                    return i;
                }
                return -1;
            }
        }
    }
    return -1;
}

s32 getExpForNextLevel(s32 level) {
    level++;
    return level * level + level * 2;
}

s32 rollPartnerAbility(s32 player, s32 slot) {
    if ((s8)((s8)((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].level % 5) != 0) {
        return -1;
    }
    return rand() % 4;
}
