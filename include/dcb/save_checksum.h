#ifndef DCB_SAVE_CHECKSUM_H
#define DCB_SAVE_CHECKSUM_H

#include "game.h"
#include "dcb/memcard.h"

s32 verifySaveChecksum(s32 len, u8 *data);
void writeSaveChecksum(s32 len, u8 *data);

#endif /* DCB_SAVE_CHECKSUM_H */
