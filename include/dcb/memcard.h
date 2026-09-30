#ifndef DCB_MEMCARD_H
#define DCB_MEMCARD_H

#include "game.h"

typedef struct {
    u8 data[0x200];
} McHeader;

extern s32 MEMORY_CARD_EVENT_DONE;
extern s32 MEMORY_CARD_EVENT_ERROR;
extern s32 MEMORY_CARD_EVENT_TIMEOUT;
extern s32 MEMORY_CARD_EVENT_NEW_CARD;
extern s32 MEMORY_CARD_HW_EVENT_DONE;
extern s32 MEMORY_CARD_HW_EVENT_ERROR;
extern s32 MEMORY_CARD_HW_EVENT_TIMEOUT;
extern s32 MEMORY_CARD_HW_EVENT_NEW_CARD;
extern CardDir *MEMORY_CARD_DIRECTORIES[2];
extern u8 *MEMORY_CARD_SAVE_HEADER;
extern s32 MEMORY_CARD_WAIT_COUNTER;
extern s32 MEMORY_CARD_TRANSFER_STEP;
extern s32 MEMORY_CARD_FILE;
extern s32 MEMORY_CARD_TRANSFER_DATA;
extern s32 MEMORY_CARD_SECTORS_DONE;
extern s32 MEMORY_CARD_SECTORS_TOTAL;
extern u8 COMPLETE_SET_CARD_COUNTS[6];

void initMemoryCard(void);
void playMenuSound(u32 kind);
s32 isMusicIdle(void);
void startMemoryCardEvents(void);
void clearMemoryCardEvents(void);
void clearMemoryCardHwEvents(void);
s32 waitForMemoryCardEvent(s32 pollInterval);
s32 waitForMemoryCardHwEvent(s32 pollInterval);
s32 ensureMemoryCardReady(s32 port);
s32 getMemoryCardStatus(s32 port);
s32 formatMemoryCard(s32 port);
s32 startMemoryCardSave(s32 port, u8 blocks, s32 data, s32 fileName, McHeader *header);
s32 stepMemoryCardSave(void);
s32 startMemoryCardLoad(s32 port, s32 data, s32 fileName);
s32 stepMemoryCardLoad(void);
s32 readMemoryCardSavePreview(s32 port, void *dst, s32 fileName);
void scanMemoryCardFiles(s32 port);

#endif /* DCB_MEMCARD_H */
