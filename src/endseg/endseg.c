#include "common.h"
#include "game.h"
#include "dcb/text.h"
#include "dcb/vram_upload.h"

extern u8 *D_801E0D64;

INCLUDE_RODATA("asm/endseg/nonmatchings/endseg", D_801DDF38);

INCLUDE_ASM("asm/endseg/nonmatchings/endseg", func_801DE80C);

void func_801DEF8C(s32 x, s32 y, s32 frame, s32 index) {
    u8 *tim;
    s32 u;

    tim = D_801E0D64 + ((s32 *)D_801E0D64)[index];
    index %= 6;
    u = (index << 2) + index;
    uploadTim((u32 *)tim, u * 4 + 0x2C0, 0, -1, -1);
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x + 2;
        CUR_SPRT->sp.y0 = y + 5;
        CUR_SPRT->sp.u0 = u * 8 + 2;
        CUR_SPRT->sp.v0 = 2;
        CUR_SPRT->sp.clut = getClut(LOADED_TIM.crect->x, LOADED_TIM.crect->y);
        CUR_SPRT->sp.w = 36;
        CUR_SPRT->sp.h = 36;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x8B);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (frame == 6) {
            frame = 5;
        }
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0x80;
            CUR_SPRT->sp.clut = getClut(0x2C0, frame + 0xB1);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0xB);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

void func_801DF2BC(s32 x, s32 y) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0x40;
        CUR_SPRT->sp.v0 = 0x80;
        CUR_SPRT->sp.clut = 0x2DEC;
        CUR_SPRT->sp.w = 40;
        CUR_SPRT->sp.h = 48;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0xB);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void func_801DF408(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    x += 40;
    drawText(x, y + 1, (s32)"*s0*b4: Scroll L1, R1: Fast Scroll", 7, z);
    drawText(x, y + 15, (s32)"*s0L2: Previous R2: Next *b6: Quit", 7, z);
}

INCLUDE_ASM("asm/endseg/nonmatchings/endseg", func_801DF47C);
