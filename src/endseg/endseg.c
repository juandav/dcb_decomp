#include "common.h"
#include "game.h"
#include "dcb/text.h"
#include "dcb/vram_upload.h"
#include "dcb/card_db.h"

extern u8 *D_801E0D64;
extern s32 D_801E0CEC[6];
extern s32 D_801E0D0C[6];
extern s32 D_801E0D2C[6];
extern s32 D_801E0D4C[6];

INCLUDE_RODATA("asm/endseg/nonmatchings/endseg", D_801DDF38);

s32 func_801DE80C(void) {
    s32 order[3];
    s32 i;
    s32 j;
    s32 k;
    s32 specialty;
    s32 tmp;

    for (i = 0; i < 6; i++) {
        D_801E0CEC[i] = 0;
        D_801E0D0C[i] = 0;
        D_801E0D2C[i] = 0;
        D_801E0D4C[i] = i;
    }
    for (i = 0; i < 3; i++) {
        order[i] = i;
    }
    for (i = 0; i < 0xBF; i++) {
        specialty = ((DigimonCardData *)DIGIMON_CARDS)[i].attr >> 4;
        D_801E0CEC[specialty] += ((PlayerProfile *)PLAYER_PROFILES)->unk11B6[i];
        D_801E0D0C[specialty] += ((PlayerProfile *)PLAYER_PROFILES)->unk1334[i];
        D_801E0D2C[specialty] += getOwnedCardCount(0, i);
        D_801E0CEC[5] += ((PlayerProfile *)PLAYER_PROFILES)->unk11B6[i];
        D_801E0D0C[5] += ((PlayerProfile *)PLAYER_PROFILES)->unk1334[i];
        D_801E0D2C[5] += getOwnedCardCount(0, i);
    }
    for (k = 0; k < 4; k++) {
        for (i = 0; i < 4; i++) {
            if (D_801E0CEC[D_801E0D4C[i]] < D_801E0CEC[D_801E0D4C[i + 1]]) {
                tmp = D_801E0D4C[i];
                D_801E0D4C[i] = D_801E0D4C[i + 1];
                D_801E0D4C[i + 1] = tmp;
            }
        }
    }
    for (k = 0; k < 2; k++) {
        for (i = 0; i < 2; i++) {
            if (((PlayerProfile *)PLAYER_PROFILES)->attackCounts[order[i]] < ((PlayerProfile *)PLAYER_PROFILES)->attackCounts[order[i + 1]]) {
                tmp = order[i];
                order[i] = order[i + 1];
                order[i + 1] = tmp;
            }
        }
    }
    if (D_801E0CEC[D_801E0D4C[0]] * 9 / 10 <= D_801E0CEC[D_801E0D4C[4]]) {
        return 0;
    }
    if (D_801E0CEC[D_801E0D4C[0]] * 4 / 5 <= D_801E0CEC[D_801E0D4C[4]]) {
        return 1;
    }
    if (D_801E0CEC[D_801E0D4C[0]] * 9 / 10 <= D_801E0CEC[D_801E0D4C[3]]) {
        return 2;
    }
    if (D_801E0CEC[D_801E0D4C[0]] * 4 / 5 <= D_801E0CEC[D_801E0D4C[3]]) {
        return 3;
    }
    if (D_801E0CEC[D_801E0D4C[0]] * 19 / 20 <= D_801E0CEC[D_801E0D4C[2]]) {
        return 4;
    }
    if (D_801E0CEC[D_801E0D4C[0]] * 9 / 10 <= D_801E0CEC[D_801E0D4C[2]]) {
        return 5;
    }
    if (D_801E0CEC[D_801E0D4C[0]] * 9 / 10 <= D_801E0CEC[D_801E0D4C[1]]) {
        switch ((1 << D_801E0D4C[0]) | (1 << D_801E0D4C[1])) {
        case 3:
            return 6;
        case 5:
            return 7;
        case 6:
            return 8;
        case 9:
            return 9;
        case 10:
            return 10;
        case 12:
            return 11;
        case 17:
            return 12;
        case 18:
            return 13;
        case 20:
            return 14;
        case 24:
            return 15;
        }
    }
    if (((PlayerProfile *)PLAYER_PROFILES)->attackCounts[order[0]] * 9 / 10 < ((PlayerProfile *)PLAYER_PROFILES)->attackCounts[order[2]]) {
        return D_801E0D4C[0] + 0x10;
    }
    switch (order[0]) {
    case 0:
        return D_801E0D4C[0] + 0x15;
    case 1:
        return D_801E0D4C[0] + 0x1A;
    case 2:
        return D_801E0D4C[0] + 0x1F;
    }
    return 0x24;
}

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
