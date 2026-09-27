#include "common.h"
#include "gte.h"
#include "game.h"

void func_80021964(u8 *m) {
    u8 *k;
    SVECTOR v;
    MATRIX mat;
    VECTOR lv;
    s32 flag;

    k = m + 0xD80;
    if (*(s16 *)(k + 0x52) == 0) {
        return;
    }
    v.vy = 0;
    v.vx = 0;
    v.vz = *(s16 *)(k + 0x52);
    PushMatrix();
    mat.t[2] = 0;
    mat.t[1] = 0;
    mat.t[0] = 0;
    gte_SetTransMatrix(&mat);
    RotMatrix(m + 0xA78, &mat);
    gte_SetRotMatrix(&mat);
    gte_ldv0(&v);
    gte_rtv0tr();
    gte_stlvnl(&lv);
    gte_stflg(&flag);
    *(s16 *)(k + 0x52) = 0;
    *(s32 *)(m + 8) += lv.vx;
    *(s32 *)(m + 0xC) += lv.vy;
    *(s32 *)(m + 0x10) += lv.vz;
    PopMatrix();
}

void func_80021AA8(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 start;
    s32 end;
    s32 d0;
    s32 d1;
    s32 mid;

    start = arg4 << 0x14;
    arg0[0] = start;
    end = arg5 << 0x14;
    mid = end - start;
    d0 = mid / arg1;
    d1 = (((arg6 - arg5) << 0x14) / arg2 + d0) / 2;
    mid = d0 * 2 - (d1 + arg0[1]) / 2;
    arg0[2] = (mid - arg0[1]) / arg3;
    arg0[3] = (d1 - mid) / arg3;
}

void func_80021B60(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 d1;
    s32 t;

    t = arg4 << 0x10;
    arg0[0] = t;
    t = ((arg5 << 0x10) - t) / arg1;
    d1 = (((arg6 - arg5) << 0x10) / arg2 + t) / 2;
    t = t * 2 - (d1 + arg0[1]) / 2;
    arg0[2] = (t - arg0[1]) / arg3;
    arg0[3] = (d1 - t) / arg3;
}

void func_80021C18(u8 *m) {
    BoneAnim *b;
    s32 i;
    s32 *t;

    b = (BoneAnim *)(m + 0xD80);
    t = (s32 *)(m + 0x2200);
    t[4]--;
    i = 0;
    if (*(s32 *)(m + 0x26D8) != 0) {
        i = *(s16 *)(m + 4);
        b += i;
    }
    for (; i < *(s16 *)(m + 4) + 1; b++, i++) {
        if (t[4] >= 0) {
                b->ch[0].val += b->ch[0].d0;
                b->ch[1].val += b->ch[1].d0;
                b->ch[2].val += b->ch[2].d0;
                b->ch[3].val += b->ch[3].d0;
                b->ch[4].val += b->ch[4].d0;
                b->ch[5].val += b->ch[5].d0;
                b->ch[6].val += b->ch[6].d0;
                b->ch[7].val += b->ch[7].d0;
                b->ch[8].val += b->ch[8].d0;
        } else {
                b->ch[0].val += b->ch[0].d1;
                b->ch[1].val += b->ch[1].d1;
                b->ch[2].val += b->ch[2].d1;
                b->ch[3].val += b->ch[3].d1;
                b->ch[4].val += b->ch[4].d1;
                b->ch[5].val += b->ch[5].d1;
                b->ch[6].val += b->ch[6].d1;
                b->ch[7].val += b->ch[7].d1;
                b->ch[8].val += b->ch[8].d1;
        }
    }
}

s32 func_80021DF8(void *arg0) {
    s32 temp_s0;
    s32 temp_v0;
    s32 var_s6;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    void *temp_s1;
    void *temp_v1;
    void *var_s2;
    void *var_s3;
    void *var_s4;

    var_s2 = arg0 + 0xD80;
    var_s3 = arg0 + 0x78;
    var_s4 = arg0 + 0xA80;
    var_s6 = 0;
    if ((*(s16 *)((s8 *)arg0 + 4)) > 0) {
        do {
            var_v0 = (*(s32 *)((s8 *)var_s2 + 0));
            if (var_v0 < 0) {
                var_v0 += 0xFFFFF;
            }
            (*(s16 *)((s8 *)var_s4 + 0)) = (s16) (var_v0 >> 0x14);
            var_v0_2 = (*(s32 *)((s8 *)var_s2 + 0x10));
            if (var_v0_2 < 0) {
                var_v0_2 += 0xFFFFF;
            }
            (*(s16 *)((s8 *)var_s4 + 2)) = (s16) (var_v0_2 >> 0x14);
            var_v0_3 = (*(s32 *)((s8 *)var_s2 + 0x20));
            if (var_v0_3 < 0) {
                var_v0_3 += 0xFFFFF;
            }
            (*(s16 *)((s8 *)var_s4 + 4)) = (s16) (var_v0_3 >> 0x14);
            (*(s32 *)((s8 *)var_s3 + 0x18)) = (s32) ((*(s16 *)((s8 *)var_s2 + 0x32)) + (*(s16 *)((s8 *)((Unk1F80 *)arg0)->unk1F80[var_s6] + 0)));
            (*(s32 *)((s8 *)var_s3 + 0x1C)) = (s32) ((*(s16 *)((s8 *)var_s2 + 0x42)) + (*(s16 *)((s8 *)((Unk1F80 *)arg0)->unk1F80[var_s6] + 2)));
            (*(s32 *)((s8 *)var_s3 + 0x20)) = (s32) ((*(s16 *)((s8 *)var_s2 + 0x52)) + (*(s16 *)((s8 *)((Unk1F80 *)arg0)->unk1F80[var_s6] + 4)));
            temp_s0 = var_s6 * 0x10;
            temp_v1 = arg0 + temp_s0;
            (*(s32 *)((s8 *)temp_v1 + 0x2000)) = (s32) (*(s16 *)((s8 *)var_s2 + 0x62));
            (*(s32 *)((s8 *)temp_v1 + 0x2004)) = (s32) (*(s16 *)((s8 *)var_s2 + 0x72));
            (*(s32 *)((s8 *)temp_v1 + 0x2008)) = (s32) (*(s16 *)((s8 *)var_s2 + 0x82));
            temp_s1 = var_s3 + 4;
            RotMatrixYXZ(var_s4, temp_s1);
            (*(s32 *)((s8 *)var_s3 + 0)) = 0;
            ScaleMatrix(temp_s1, arg0 + (temp_s0 + 0x2000));
            (*(s32 *)((s8 *)var_s2 + 0)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0)) + (*(s32 *)((s8 *)var_s2 + 4)));
            (*(s32 *)((s8 *)var_s2 + 0x10)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x10)) + (*(s32 *)((s8 *)var_s2 + 0x14)));
            (*(s32 *)((s8 *)var_s2 + 0x20)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x20)) + (*(s32 *)((s8 *)var_s2 + 0x24)));
            (*(s32 *)((s8 *)var_s2 + 0x30)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x30)) + (*(s32 *)((s8 *)var_s2 + 0x34)));
            (*(s32 *)((s8 *)var_s2 + 0x40)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x40)) + (*(s32 *)((s8 *)var_s2 + 0x44)));
            (*(s32 *)((s8 *)var_s2 + 0x50)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x50)) + (*(s32 *)((s8 *)var_s2 + 0x54)));
            (*(s32 *)((s8 *)var_s2 + 0x60)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x60)) + (*(s32 *)((s8 *)var_s2 + 0x64)));
            (*(s32 *)((s8 *)var_s2 + 0x70)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x70)) + (*(s32 *)((s8 *)var_s2 + 0x74)));
            (*(s32 *)((s8 *)var_s2 + 0x80)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x80)) + (*(s32 *)((s8 *)var_s2 + 0x84)));
            var_s6 += 1;
            var_s2 += 0x90;
            var_s3 += 0x50;
            var_s4 += 8;
        } while (var_s6 < (*(s16 *)((s8 *)arg0 + 4)));
    }
    (*(s32 *)((s8 *)var_s2 + 0)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0)) + (*(s32 *)((s8 *)var_s2 + 4)));
    (*(s32 *)((s8 *)var_s2 + 0x10)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x10)) + (*(s32 *)((s8 *)var_s2 + 0x14)));
    (*(s32 *)((s8 *)var_s2 + 0x20)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x20)) + (*(s32 *)((s8 *)var_s2 + 0x24)));
    (*(s32 *)((s8 *)var_s2 + 0x30)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x30)) + (*(s32 *)((s8 *)var_s2 + 0x34)));
    (*(s32 *)((s8 *)var_s2 + 0x40)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x40)) + (*(s32 *)((s8 *)var_s2 + 0x44)));
    (*(s32 *)((s8 *)var_s2 + 0x50)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x50)) + (*(s32 *)((s8 *)var_s2 + 0x54)));
    (*(s32 *)((s8 *)var_s2 + 0x60)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x60)) + (*(s32 *)((s8 *)var_s2 + 0x64)));
    (*(s32 *)((s8 *)var_s2 + 0x70)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x70)) + (*(s32 *)((s8 *)var_s2 + 0x74)));
    temp_v0 = (*(s32 *)((s8 *)var_s2 + 0x80)) + (*(s32 *)((s8 *)var_s2 + 0x84));
    (*(s32 *)((s8 *)var_s2 + 0x80)) = temp_v0;
    return temp_v0;
}

INCLUDE_ASM("asm/main/nonmatchings/model/model_anim", func_80022100);

void func_80022B98(void) {
    s32 temp_v0;
    s32 var_s1;
    void *temp_s0;
    void *temp_v1;

    D_80079544 = 1;
    var_s1 = 0;
loop_1:
    temp_s0 = D_801D6A4C->unk13C[var_s1];
    if (D_801D6A4C->unk114[var_s1] > 0) {
        temp_v1 = temp_s0 + 0x2200;
        if ((*(s32 *)((s8 *)temp_s0 + 0x2208)) >= 0) {
            temp_v0 = (*(s32 *)((s8 *)temp_v1 + 8)) - 1;
            (*(s32 *)((s8 *)temp_v1 + 8)) = temp_v0;
            if (temp_v0 <= 0) {
                func_80022100(temp_s0, (*(s32 *)((s8 *)temp_v1 + 0x18)), 0);
            }
            func_80021DF8(temp_s0);
            func_80021C18(temp_s0);
        }
    }
    var_s1 += 1;
    if (var_s1 < 0x18) {
        goto loop_1;
    }
    var_s1 = 0;
    func_80014C08(D_800794F0);
    goto loop_1;
}
