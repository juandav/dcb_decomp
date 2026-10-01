#include "common.h"

/*
 * jp's first SUBSEG object: the overlay's number and the two window
 * callbacks that do nothing. The rest of jp's SUBSEG, a card shop, is
 * shop/sub_shop.c: its .rodata starts 4 bytes in, where its jump tables'
 * 8-byte alignment puts the start of an object.
 */

/* not referenced by any code */
const s32 D_801DDF38 = 8;

void SUB_ignoreShopWindowClose(void) {
}

void SUB_ignoreViewerWindowClose(void) {
}
