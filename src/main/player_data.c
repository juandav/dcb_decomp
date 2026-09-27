#include "common.h"
#include "gte.h"
#include "game.h"

void func_8002D404(void) {
    void *p;

    func_800457FC();
    D_8006E050 = func_8001ACEC(0x4EE8);
    D_8006E054 = p = func_8001ACEC(0x102C);
    (*(void **)((s8 *)D_8006E054 + 0x100C)) = func_8001ACEC(0x1AC);
    func_8002D51C();
}

void func_8002D458(void) {
    s32 i;

    ((Unk8006E054 *)D_8006E054)->unk1027 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A4 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A2 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A9 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A8 = 0;
    for (i = 0; i < 12; i++) {
        ((Unk8006E050 *)D_8006E050)->unk23FC[i] = 0;
    }
    for (i = 0; i < 9; i++) {
        ((Unk8006E050 *)D_8006E050)->unk242C[i] = 0;
    }
    ((Unk8006E050 *)D_8006E050)->unk2C = 0;
    ((Unk8006E050 *)D_8006E050)->unk14 = 0;
}

void func_8002D51C(void) {
    Unk8006E050 *e;
    s32 p;
    s32 j;
    s32 i;

    e = (Unk8006E050 *)D_8006E050;
    for (i = 0; i < 12; i++) {
        ((Unk8006E050 *)D_8006E050)->unk23FC[i] = 0;
    }
    ((Unk8006E050 *)D_8006E050)->unk28_9 = 0;
    for (p = 0; p < 2; p++, e++) {
        e->name[0] = 0;
        e->unk18 = 0;
        e->unk1A = 0;
        e->unk1C = 0;
        e->unk1E = 0;
        e->unkE = 0;
        e->unk10 = rand();
        e->unk28_10 = 0;
        e->unk28_13 = 0;
        e->unkD = 0;
        e->unk28_11 = 0;
        e->unk28_12 = 0;
        e->rankA = 0;
        e->rankB = 0;
        e->rankC = 0;
        e->unk16 = 0x2774;
        e->unk4C = 0;
        e->unk4E = 0;
        e->unk50 = 0;
        e->unk52 = 0;
        e->unk54 = 0;
        e->unk56 = 0;
        for (i = 0; i < 3; i++) {
            e->unk36[i] = 0;
        }
        for (i = 0; i < 0x28; i++) {
            e->unk58[i] = 0;
        }
        for (i = 0; i < 0x12D; i++) {
            e->unk14B2[i] = 0;
            for (j = 0; j < 8; j++) {
                func_80045968(p, i, j);
            }
        }
        for (i = 0; i < 0xBF; i++) {
            for (j = 0; j < 3; j++) {
                e->unkD3C[i][j] = 0;
            }
            e->unk11B6[i] = 0;
            e->unk1334[i] = 0;
        }
        for (i = 0; i < 3; i++) {
            e->unk80[i].unk288 = 0;
        }
        for (i = 0; i < 0x10; i++) {
            e->unk3C[i] = 0;
        }
        for (i = 0; i < 3; i++) {
            e->unk2438[i].unk0 = 0;
            e->unk2438[i].unk108[0] = 0;
            e->unk2438[i].unk108[1] = 0;
            e->unk2438[i].unk108[2] = 0;
        }
        for (i = 0; i < 0x9F; i++) {
            e->unkAC0[i] = 0;
            e->unkBFE[i] = 0;
        }
        for (i = 0; i < 0x8E; i++) {
            e->unk888[i] = 0;
            e->unk9A4[i] = 0;
        }
        for (j = 0; j < 0x20; j++) {
            e->unk848[j] = 0;
        }
        e->unk20_0 = 0;
        e->unk20_1 = 0;
        e->unk20_2 = 0;
        e->unk20_3 = 0;
        e->unk24 = 0;
    }
    strcpy(((Unk8006E050 *)D_8006E050)->name, "Player");
    func_8002D458();
}

void func_8002D898(void) {
    CUR_SPRT->sp.x0 = 0;
    CUR_SPRT->sp.y0 = 0;
    CUR_SPRT->sp.u0 = 0;
    CUR_SPRT->sp.v0 = 0;
    CUR_SPRT->sp.clut = 0x3FD4;
    CUR_SPRT->sp.w = 0x100;
    CUR_SPRT->sp.h = 0xF0;
    setSemiTrans(&CUR_SPRT->sp, 0);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x85);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
    CUR_SPRT->sp.x0 = 0x100;
    CUR_SPRT->sp.y0 = 0;
    CUR_SPRT->sp.u0 = 0;
    CUR_SPRT->sp.v0 = 0;
    CUR_SPRT->sp.clut = 0x3FD4;
    CUR_SPRT->sp.w = 0x40;
    CUR_SPRT->sp.h = 0xF0;
    setSemiTrans(&CUR_SPRT->sp, 0);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x87);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
}

void func_8002DAAC(s32 arg0, s32 arg1) {
    void *temp_s1;

    temp_s1 = D_801D6A4C->unk13C[arg0];
    if ((*(s32 *)((s8 *)temp_s1 + 0x2200)) != arg1) {
        func_8001AFF0(arg0 + 0x84);
        func_80023094(temp_s1, (s32 *)func_8001BFF8((s32)func_8001BB44(*(Chunk **)((s8 *)temp_s1 + 0x26F4), 1, arg1), arg0 + 0x84), arg1);
    }
    func_80022D34(arg0, arg1, -2, 0);
}

void func_8002DB58(s32 arg0, s32 arg1) {
    s32 temp_s2;
    void *temp_s3;

    temp_s3 = D_801D6A4C->unk13C[arg0];
    temp_s2 = arg0 + 0x84;
    func_8001AFF0(temp_s2);
    func_80023094(temp_s3, (s32 *)func_8001BFF8((s32)func_8001BB44(*(Chunk **)((s8 *)temp_s3 + 0x26F4), 1, arg1), temp_s2), arg1);
    func_80023148(arg0, arg1);
}

void *func_8002DBEC(s32 arg0) {
    u8 *p;
    s32 i;

    p = D_801D8408;
    if (p[0xE5] != arg0) {
        i = 0;
        do {
            i++;
            p += 0x13C;
            if (i >= 0xBF) {
                break;
            }
        } while (p[0xE5] != arg0);
    }
    return p;
}

s32 func_8002DC30(s32 arg0, s32 arg1) {
    char sp10[32];
    s32 var_v0;

    var_v0 = (s32)func_8001BB44((Chunk *)arg1, 2, arg0);
    if (var_v0 == 0) {
        sprintf(sp10, &D_800107F8, arg0);
        var_v0 = func_8001B248(sp10, func_800148B0(), 0x81);
    }
    return var_v0;
}

void func_8002DC90(s32 arg0) {
    func_8002DC30(arg0, 0);
}

INCLUDE_RODATA("asm/main/nonmatchings/player_data", D_800107F8);
