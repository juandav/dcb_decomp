#include "common.h"

INCLUDE_ASM("asm/main/nonmatchings/game", main);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80013F04);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80013FA4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800141B8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800142D0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014364);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014614);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014748);

extern int func_80014A00(int);

typedef struct {
    /* 0x00 */ char unk0[0x14];
    /* 0x14 */ int unk14;
} Unk80077A0C;

extern Unk80077A0C *D_80077A0C;

int func_80014840(void) {
    int n = D_80077A0C->unk14;
    int i;
    int ret = 0;

    for (i = 1; i < 0x20; i++) {
        if (i == n) {
            continue;
        }
        if (func_80014A00(i) == 0) {
            ret++;
        }
    }
    return ret;
}


s32 func_800148B0(void) {
    return (*(s32 *)((s8 *)D_80077A0C + 0x14));
}

extern s32 D_80077BA0;
s32 func_800148C8(s32 arg0, s32 arg1) {
    void *temp_v1;

    temp_v1 = (s8 *)&D_80077BA0 + arg0 * 0xC0;
    if ((*(s32 *)((s8 *)temp_v1 + 0)) >= 0) {
        return -3;
    }
    if (temp_v1 == D_80077A0C) {
        return -0x84;
    }
    (*(s32 *)((s8 *)temp_v1 + 0x18)) = arg1;
    (*(s32 *)((s8 *)temp_v1 + 4)) = 0;
    return 0;
}

extern s32 D_80077BA0;

s32 func_8001491C(s32 arg0) {
    void *temp_v1;

    temp_v1 = (s8 *)&D_80077BA0 + arg0 * 0xC0;
    if ((*(s32 *)((s8 *)temp_v1 + 0)) >= 0) {
        return -3;
    }
    if (temp_v1 == D_80077A0C) {
        return -0x84;
    }
    return (*(s32 *)((s8 *)temp_v1 + 4));
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014970);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800149A0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800149A8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800149B8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014A00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014A48);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014A90);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014AC8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014C08);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014CF0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014D64);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014EF0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800152AC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80015328);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800157B0);

s32 func_80015AD8(s32, s32);
s32 func_80015EAC(s32 *arg0);
s32 func_80015F34(s32, s32, s32 *);
extern s32 D_800857D0;
extern s32 D_800857E0;

s32 func_80015848(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_80015AD8(arg0, 0);
    if ((temp_v0 != 0) && (func_80015F34(temp_v0, 0x4000, &D_800857E0) != 0)) {
        func_80015EAC(temp_v0);
        D_800857D0 = 1;
        return 0;
    }
    return 1;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_800158B0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80015A3C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80015AD8);

s32 func_8005A364(s32, s32);

s32 func_80015EAC(s32 *arg0) {
    *arg0 = 0;
    return func_8005A364(0, 0) == 5;
}

typedef struct {
    /* 0x0000 */ int unk0;
    /* 0x0004 */ char unk4[0x102C];
} Unk80081710;

extern Unk80081710 D_80081710[4];
extern int func_8005A364(int, int);

int func_80015EDC(void) {
    Unk80081710 *p = D_80081710;
    int i;

    for (i = 3; i >= 0; i--, p++) {
        if (p->unk0 > 0) {
            p->unk0 = 0;
        }
    }
    return func_8005A364(0, 0) == 5;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80015F34);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800161D8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800162F0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016500);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016724);

extern s32 D_80079500;
void func_8001683C(s32 arg0) {
    s32 *var_v0;
    s32 temp_v1;

    var_v0 = &D_80079500;
    if (arg0 != 0) {
loop_1:
        temp_v1 = (*(s32 *)((s8 *)var_v0 + 0));
        if (temp_v1 != arg0) {
            if (temp_v1 != 0) {
                var_v0 += 1;
                goto loop_1;
            }
            (*(s32 *)((s8 *)var_v0 + 0)) = arg0;
            (*(s32 *)((s8 *)var_v0 + 4)) = 0;
        }
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016878);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800168C4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016948);

typedef struct {
    /* 0x0000 */ u8 unk0[0x70];
    /* 0x0070 */ u32 ot[0x1000];
    /* 0x4070 */ u8 unk4070[0x48];
    /* 0x40B8 */ s32 unk40B8;
    /* 0x40BC */ s32 unk40BC;
} Unk800793A0;

extern Unk800793A0 *D_800793A0;
extern s32 D_800897E8;

void func_80016BEC(void) {
    D_800897E8 = D_800793A0->unk40BC;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016C08);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016F38);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001705C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800170F0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800176E4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800177E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80017B88);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80018694);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80018B8C);

extern int D_800897E8;
extern unsigned short D_800897EC;
extern char D_80010008[];
extern int printf(const char *, ...);

int func_80019084(void) {
    if (D_800897E8 == D_800793A0->unk40BC + D_800897EC * 0x294) {
        printf(D_80010008);
        return -1;
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_800190F4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800191C0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80019280);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800192E0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800192FC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001963C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800197AC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800198A8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80019EA4);

s32 func_800149B8();
void func_8001A1D8();

s32 func_80014C08(s32);
s8 func_8001A100(void *arg0) {
    func_800149B8(0, -1, 0, 0x400, &func_8001A1D8, arg0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    return (*(s8 *)((s8 *)arg0 + 0xA5));
}

s32 func_8001A164(s32 *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = func_800148B0();
    (*(s8 *)((s8 *)arg0 + 0xA6)) = (s8) arg1;
    func_800149B8(0, -1, 0, 0x400, func_8001A1D8, arg0, temp_v0, 0, 0);
    func_80014C08(0x7FFFFFFF);
    return (s32) (*(s8 *)((s8 *)arg0 + 0xA5));
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A1D8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A40C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A600);

extern void *D_80089840[];

void func_8001A688(s32 arg0, s16 arg1, s16 arg2) {
    (*(s16 *)((s8 *)D_80089840[arg0] + 0x16)) = arg1;
    (*(s16 *)((s8 *)D_80089840[arg0] + 0x18)) = arg2;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A6B0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A7A4);

s32 func_8001A7A4(s32, void *, void *);
extern s32 D_800897F8;
extern s32 D_8008983C;

void func_8001A9B0(void) {
    s32 var_s1;
    void *temp_s0;

    var_s1 = 0;
    do {
        temp_s0 = D_80089840[var_s1];
        func_8001A7A4(var_s1 * 0x10, temp_s0, (s8 *)&D_800897F8 + var_s1 * 0x22);
        if (D_8008983C != 0) {
            (*(u16 *)((s8 *)temp_s0 + 8)) = (u16) (*(u16 *)((s8 *)temp_s0 + 0));
            (*(u16 *)((s8 *)temp_s0 + 0xE)) = (u16) (*(u16 *)((s8 *)temp_s0 + 6));
            (*(u16 *)((s8 *)temp_s0 + 0xC)) = (u16) (*(u16 *)((s8 *)temp_s0 + 4));
            (*(u16 *)((s8 *)temp_s0 + 0xA)) = (u16) (*(u16 *)((s8 *)temp_s0 + 2));
        } else {
            (*(u16 *)((s8 *)temp_s0 + 8)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xE)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xC)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xA)) = 0U;
        }
        var_s1 += 1;
    } while (var_s1 < 2);
}

extern s32 D_8008C848;

s32 func_8001AE90(void *);
extern s32 D_80089848;
void func_8001AA80(s32 arg0) {
    s32 *p;
    s32 i;

    if (arg0 != 0) {
        p = &D_80089848;
        p[0] = (s32)&D_8008C848 & 0x3FFFFFFF;
        p[1] = 0x148000;
        p[2] = -1;
        i = 0x3FF;
        do {
            p += 3;
            p[0] = 0;
            p[1] = 0;
            i--;
            p[2] = 0;
        } while (i > 0);
        return;
    }
    p = &D_80089848;
    i = 0x3FF;
    if (p[0] != 0) {
loop:
        if (p[0] < 0 && p[2] >= 0) {
            if (func_8001AE90((void *)p[0]) == 0) {
                goto loop;
            }
        }
        i--;
        p += 3;
        if (i >= 0 && p[0] != 0) {
            goto loop;
        }
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AB64);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001ABCC);

void *func_8001ABCC(s32, s32);
void *func_8001ACEC(s32 arg0) {
    return func_8001ABCC(arg0, -2);
}

s32 func_800148B0();
void *func_8001ABCC(s32, s32);

void *func_8001AD0C(s32 size) {
    return func_8001ABCC(size, func_800148B0());
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AD3C);

s32 func_8001AE90(void *);

void func_8001AE70(void *arg0) {
    func_8001AE90(arg0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AE90);

extern s32 D_80089848;

s32 func_8001AFF0(s32 arg0) {
    s32 *var_s0;
    s32 temp_a0;
    s32 var_s1;

    var_s0 = &D_80089848;
    var_s1 = 0x3FF;
    if (D_80089848 != 0) {
loop_1:
        temp_a0 = (*(s32 *)((s8 *)var_s0 + 0));
        if ((temp_a0 < 0) && ((*(s32 *)((s8 *)var_s0 + 8)) == arg0)) {
            if (func_8001AE90(temp_a0) == 0) {
                goto loop_1;
            }
        }
        var_s1 -= 1;
        var_s0 += 3;
        if ((var_s1 >= 0) && (*var_s0 != 0)) {
            goto loop_1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B088);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B10C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B144);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B248);

extern s32 D_8006DEF0;
extern s32 D_801D4848;

s32 func_80014A48(s32);
extern s32 D_800794F0;
void func_8001B358(s32 arg0, s32 *arg1, s32 arg2) {
    s32 temp_v0;
    s32 var_s2;

    var_s2 = 0;
    if (D_8006DEF0 != 0) {
        do {
            func_80014C08(D_800794F0);
        } while (D_8006DEF0 != 0);
    }
    D_8006DEF0 = 1;
    temp_v0 = func_80015AD8(arg0, 1);
    if (temp_v0 != 0) {
        var_s2 = (*(s32 *)((s8 *)temp_v0 + 0x24));
        func_80015F34(temp_v0, var_s2, arg1);
        func_80015EAC((s32 *) temp_v0);
    }
    D_801D4848 = var_s2;
    func_80014A48(arg2);
    D_8006DEF0 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B438);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B5BC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B634);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B734);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B7F4);

s32 func_8001B930();
extern s32 D_80079500;

void func_8001B90C(void) {
    D_80079500 = 0;
    func_8001B930();
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B930);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BB44);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BB94);

s32 func_8001BB94(s32, s32, s32);

void func_8001BC14(s32 arg0) {
    func_8001BB94(arg0, 5, -1);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BC38);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BCA4);

s32 func_8001BC38();
extern s32 D_801D4878;
extern s32 D_801D4888;
extern s32 D_801D5108;

s32 func_8001BCA4(s32);
s32 func_8001BD60(void) {
    s32 i;

    if (func_8001BC38() != 0) {
        i = D_801D4878++;
        if (i >= 0x21F) {
            return -1;
        }
        (&D_801D4888)[i] = func_8001BD60();
        (&D_801D5108)[i] = func_8001BD60();
    } else {
        i = func_8001BCA4(9);
    }
    return i;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BDEC);

void func_8001C078(s32);

void func_8001BFCC(s32 arg0, s32 arg1) {
    func_8001C078(arg0 + ((s32 *)arg0)[arg1]);
}

s32 func_8001BCA4(s32);
s32 func_8001BDEC(s32);
extern s32 D_801D4868;
extern s32 D_801D486C;
extern s32 D_801D4870;
extern s32 D_801D4874;

s32 func_8001BFF8(s32 arg0, s32 arg1) {
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_v0;

    D_801D4868 = 0;
    D_801D486C = 0;
    D_801D4870 = arg0;
    temp_s0 = func_8001BCA4(0x10);
    temp_s0_2 = (temp_s0 << 0x10) | func_8001BCA4(0x10);
    temp_v0 = func_8001ABCC(temp_s0_2, arg1);
    D_801D4874 = temp_v0;
    func_8001BDEC(temp_s0_2);
    return temp_v0;
}

s32 func_800148B0();
s32 func_8001BFF8(s32, s32);

void func_8001C078(s32 arg0) {
    func_8001BFF8(arg0, func_800148B0());
}

void func_8001C1E0(s8 *, s8 *, s32);

void func_8001C0A8(s8 *base, u32 n, s32 size, s32 (*cmp)(s8 *, s8 *)) {
    u32 i;
    u32 j;
    u32 k;
    s8 *a;
    s8 *b;

    if (n < 2) {
        return;
    }
    a = base;
    if (n == 2) {
        b = a + size;
        if (cmp(a, b) > 0) {
            func_8001C1E0(a, b, size);
        }
        return;
    }
    for (i = 0; i < n; i++, a += size) {
        b = a + size;
        for (j = i; j < n - 1; j++, b += size) {
            if (cmp(a, b) > 0) {
                for (k = i; k <= j; k++) {
                    func_8001C1E0(base + size * k, b, size);
                }
            }
        }
    }
}

void func_8001C1E0(s8 *a, s8 *b, s32 size) {
    u32 i;
    s8 t;

    for (i = 0; i < size; i++) {
        t = a[i];
        a[i] = b[i];
        b[i] = t;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C220);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C354);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C4DC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C6A4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C810);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001CA54);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001CCB4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001CE74);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001CFDC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001D1AC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001D33C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001D464);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001D5B4);

s32 AddPrim(s32 *, s32);
s32 RotAverageNclip3(s32, s32, s32, s32, s32, s32, s32 *, s32 *, s32 *);
s32 RotTransPers3(s32, s32, s32, s32, s32, s32, s32 *, s32 *);


void func_8001D6D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg4 == 0) {
        sp30 = RotTransPers3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg5 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg5], arg0);
        }
    }
}

s32 RotAverageNclip4(s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32 *, s32 *);
s32 RotTransPers4(s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32 *);

void func_8001D7DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg5 == 0) {
        sp38 = RotTransPers4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg6 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg6], arg0);
        }
    }
}

void func_8001D900(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg5 == 0) {
        sp38 = RotTransPers4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x14, arg0 + 0x20, arg0 + 0x2C, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x14, arg0 + 0x20, arg0 + 0x2C, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg6 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg6], arg0);
        }
    }
}

void func_8001DA24(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, u8 arg6, s32 arg7) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg6 == 0) {
        sp30 = RotTransPers3(arg2, arg3, arg4, arg0 + 8, arg0 + 0xC, arg0 + 0x10, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg2, arg3, arg4, arg0 + 8, arg0 + 0xC, arg0 + 0x10, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg7 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp30], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg7], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg7], arg1);
                }
            }
        }
    }
}

void func_8001DBAC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 arg8) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg7 == 0) {
        sp38 = RotTransPers4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0xC, arg0 + 0x10, arg0 + 0x14, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0xC, arg0 + 0x10, arg0 + 0x14, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg8 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp38], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg8], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg8], arg1);
                }
            }
        }
    }
}

void func_8001DD4C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg4 == 0) {
        sp30 = RotTransPers3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x14, arg0 + 0x20, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x14, arg0 + 0x20, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg5 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg5], arg0);
        }
    }
}

void func_8001DE58(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, u8 arg6, s32 arg7) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg6 == 0) {
        sp30 = RotTransPers3(arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg7 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp30], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg7], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg7], arg1);
                }
            }
        }
    }
}

void func_8001DFE0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 arg8) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg7 == 0) {
        sp38 = RotTransPers4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg8 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp38], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg8], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg8], arg1);
                }
            }
        }
    }
}

s32 RotTransPers(s32, s32, s32 *, s32 *);

void func_8001E180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp10;
    s32 sp14;
    s32 temp_v0;

    RotTransPers(arg2, arg0 + 8, &sp10, &sp14);
    temp_v0 = RotTransPers(arg3, arg0 + 0xC, &sp10, &sp14);
    if ((u32) (temp_v0 - 2) < 0xFFFU) {
        if (arg5 == 0) {
            AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg1);
            }
        } else {
            AddPrim((s32 *) &D_800793A0->ot[arg5], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[arg5], arg1);
            }
        }
    }
}

void func_8001E2A4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp10;
    s32 sp14;
    s32 temp_v0;

    RotTransPers(arg2, arg0 + 8, &sp10, &sp14);
    temp_v0 = RotTransPers(arg3, arg0 + 0x10, &sp10, &sp14);
    if ((u32) (temp_v0 - 2) < 0xFFFU) {
        if (arg5 == 0) {
            AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg1);
            }
        } else {
            AddPrim((s32 *) &D_800793A0->ot[arg5], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[arg5], arg1);
            }
        }
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E3C8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E4E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E6A4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E6EC);

void func_8001E75C(void *arg0, s8 arg1, s8 arg2, s8 arg3) {
    (*(s8 *)((s8 *)arg0 + 4)) = arg1;
    (*(s8 *)((s8 *)arg0 + 5)) = arg2;
    (*(s8 *)((s8 *)arg0 + 6)) = arg3;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E76C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E7B8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E804);

s32 func_8001E8F4();
s32 func_8001E9AC();

void func_8001E850(void *arg0) {
    if ((*(u8 *)((s8 *)arg0 + 7)) & 4) {
        func_8001E9AC();
        return;
    }
    func_8001E8F4();
}

void func_8001E894(void *arg0, s8 arg1, s8 arg2, s8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0xC)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0xD)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0xE)) = arg3;
}

void func_8001E8A4(void *arg0, s8 arg1, s8 arg2, s8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x10)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x11)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x12)) = arg3;
}

void func_8001E8B4(void *arg0, s8 arg1, s8 arg2, s8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x14)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x15)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x16)) = arg3;
}

void func_8001E8C4(void *arg0, s8 arg1, s8 arg2, s8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x1C)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x1D)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x1E)) = arg3;
}

void func_8001E8D4(void *arg0, s8 arg1, s8 arg2, s8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x1C)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x1D)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x1E)) = arg3;
}

void func_8001E8E4(void *arg0, s8 arg1, s8 arg2, s8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x28)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x29)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x2A)) = arg3;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E8F4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E9AC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EA64);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EB1C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EB64);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EBAC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EBF4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EC3C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EC8C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001ECC8);

void func_8001ED04(void *arg0) {
    (*(s16 *)((s8 *)arg0 + 0)) = 0x1000;
    (*(s16 *)((s8 *)arg0 + 6)) = 0;
    (*(s16 *)((s8 *)arg0 + 0xC)) = 0;
    (*(s16 *)((s8 *)arg0 + 2)) = 0;
    (*(s16 *)((s8 *)arg0 + 8)) = 0x1000;
    (*(s16 *)((s8 *)arg0 + 0xE)) = 0;
    (*(s16 *)((s8 *)arg0 + 4)) = 0;
    (*(s16 *)((s8 *)arg0 + 0xA)) = 0;
    (*(s16 *)((s8 *)arg0 + 0x10)) = 0x1000;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001ED30);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EDE0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EEA0);

s32 SetRotMatrix(s32);
s32 func_8005C444();

void func_8001EFB0(s32 arg0) {
    func_8005C444();
    SetRotMatrix(arg0);
}

void func_8001EFDC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s16 arg5, s16 arg6, s16 arg7) {
    (*(s32 *)((s8 *)arg0 + 0x20)) = arg2;
    (*(s32 *)((s8 *)arg0 + 0x24)) = arg3;
    (*(s32 *)((s8 *)arg0 + 0x28)) = arg4;
    (*(s16 *)((s8 *)arg0 + 0x30)) = arg5;
    (*(s16 *)((s8 *)arg0 + 0x32)) = arg6;
    (*(s16 *)((s8 *)arg0 + 0x34)) = arg7;
    (*(s32 *)((s8 *)arg0 + 0x38)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x3C)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x40)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x48)) = arg1;
}

void func_8001F01C(void *arg0, void *arg1) {
    (*(u16 *)((s8 *)arg1 + 0)) = (u16) (*(u16 *)((s8 *)arg0 + 0x14));
    (*(u16 *)((s8 *)arg1 + 2)) = (u16) (*(u16 *)((s8 *)arg0 + 0x18));
    (*(u16 *)((s8 *)arg1 + 4)) = (u16) (*(u16 *)((s8 *)arg0 + 0x1C));
}

extern s32 D_801D69E0;

void func_8001F040(void) {
    D_801D69E0 = 0;
}

extern s32 D_801D69E0;

void func_8001F04C(void) {
    D_801D69E0 = 0;
}

extern s32 D_801D69E0;

s32 func_8001F058(void) {
    return D_801D69E0;
}

extern s32 D_801D69E4;
extern s32 D_801D69E8;
extern s32 D_801D69EC;
extern s32 D_801D69F0;

void func_8001F068(s32 arg0, s32 arg1, s32 arg2) {
    D_801D69E8 = arg0;
    D_801D69EC = arg1;
    D_801D69F0 = arg2;
    D_801D69E4 = arg0 * 0xFF;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F094);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F3C0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F518);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F580);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F5A4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F5CC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F5FC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F630);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F660);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F694);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F6C4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F768);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F824);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F8B0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F94C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800202D8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020370);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020440);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002060C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020638);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020674);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800206B0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020778);

void *func_8001AD0C(s32);
void func_80021954(void *arg0);

void *func_80020E34(void *arg0) {
    s32 temp_v0_2;
    void *temp_v0;

    temp_v0 = func_8001AD0C(0x28);
    (*(void **)((s8 *)temp_v0 + 0)) = arg0;
    temp_v0_2 = arg0 + 0x10;
    (*(s32 *)((s8 *)temp_v0 + 4)) = temp_v0_2;
    (*(s32 *)((s8 *)temp_v0 + 8)) = temp_v0_2;
    (*(s32 *)((s8 *)temp_v0 + 0xC)) = 0;
    (*(s32 *)((s8 *)temp_v0 + 0x10)) = (s32) (*(s32 *)((s8 *)arg0 + 8));
    func_80021954(temp_v0);
    return temp_v0;
}

void func_80020E94(void *arg0, void *arg1) {
    s32 temp_v0;

    (*(void **)((s8 *)arg1 + 0)) = arg0;
    temp_v0 = arg0 + 0x10;
    (*(s32 *)((s8 *)arg1 + 4)) = temp_v0;
    (*(s32 *)((s8 *)arg1 + 8)) = temp_v0;
    (*(s32 *)((s8 *)arg1 + 0xC)) = 0;
    (*(s32 *)((s8 *)arg1 + 0x10)) = (s32) (*(s32 *)((s8 *)arg0 + 8));
    func_80021954(arg1);
}

s32 *func_80020ED4(s32 n) {
    s32 *p = func_8001AD0C(n * 4);
    s32 *q = p;
    s32 i;

    for (i = 0; i < n; i++) {
        *q++ = 0;
    }
    return p;
}

void func_80020F24(void *arg0, void *arg1) {
    func_8001AE90(arg1);
    func_8001AE90(arg0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020F54);

void func_80021954(void *arg0) {
    (*(s16 *)((s8 *)arg0 + 0x24)) = 0;
}

void func_8002195C(void *arg0, s16 arg1) {
    (*(s16 *)((s8 *)arg0 + 0x24)) = arg1;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021964);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021AA8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021B60);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021C18);

s32 RotMatrixYXZ(void *, void *);
s32 ScaleMatrix(void *, void *);

typedef struct { u8 unk0[0x1F80]; s16 *unk1F80[8]; } Unk1F80;

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022100);

typedef struct {
    /* 0x000 */ u8 unk0[0x28];
    /* 0x028 */ u8 unk28[0xEC];
    /* 0x114 */ s8 unk114[0x28];
    /* 0x13C */ void *unk13C[88];
} Unk801D6A4C;

extern Unk801D6A4C *D_801D6A4C;

s32 func_80021C18(void *);
s32 func_80021DF8(void *);
s32 func_80022100(void *, s32, s32);
extern s32 D_80079544;

extern s32 D_800794F0;
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022C4C);

void func_80022CA4(s32 arg0) {
    void *temp_v1;
    void *temp_v1_2;

    temp_v1 = D_801D6A4C->unk13C[arg0];
    if ((*(s32 *)((s8 *)temp_v1 + 0x2208)) <= 0) {
        (*(s32 *)((s8 *)temp_v1 + 0x2208)) = -1;
        return;
    }
    temp_v1_2 = D_801D6A4C->unk13C[arg0];
    (*(s32 *)((s8 *)temp_v1_2 + 0x2208)) = (s32) -(*(s32 *)((s8 *)temp_v1_2 + 0x2208));
}

void func_80022D00(s32 arg0) {
    s32 temp_v0;
    void *temp_v1;

    temp_v1 = D_801D6A4C->unk13C[arg0];
    temp_v0 = (*(s32 *)((s8 *)temp_v1 + 0x2208));
    if (temp_v0 < 0) {
        (*(s32 *)((s8 *)temp_v1 + 0x2208)) = -temp_v0;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022D34);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022DBC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022E58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022ED0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022F34);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023094);

s32 func_80022F34(s16, s32, s32, s32);
s32 func_80023094(void *, s32, s32);

s32 func_800230B8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    void *temp_s0;

    temp_s0 = D_801D6A4C->unk13C[arg0];
    temp_v0 = func_80022F34((*(s16 *)((s8 *)temp_s0 + 6)), arg1, arg0, arg3);
    if (temp_v0 != 0) {
        func_80023094(temp_s0, temp_v0, arg2);
        return 1;
    }
    return 0;
}

s32 func_800230B8(s32, s32, s32, s32);

void func_80023128(s32 arg0, s32 arg1, s32 arg2) {
    func_800230B8(arg0, arg1, arg2, 0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023148);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023408);

void func_80023454(s32 arg0, void *arg1, s32 arg2) {
    (*(s32 *)((s8 *)arg1 + 0xC)) = (s32) (arg2 + 1);
    (*(s32 *)((s8 *)arg1 + 0)) = 0;
    (*(s32 *)((s8 *)arg1 + 8)) = arg0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023468);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800234AC);

void func_800235C8(s32 arg0) {
    D_801D6A4C->unk114[arg0] = 0;
    D_801D6A4C->unk13C[arg0] = 0;
    func_8001AFF0(arg0 + 0x40);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002360C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800236B4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002371C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002386C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023DA4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023DC8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023DF0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800243B0);

s32 GsSetAmbient(s32, s32, s32);
s32 GsSetLightMode(s32);
s32 func_8005C464(s32, s32, s32);

void func_80024420(void) {
    GsSetAmbient(0x40, 0x40, 0x40);
    func_8005C464(0x30, 0x30, 0x40);
    GsSetLightMode(0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80024460);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800246E0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80024B08);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80024DD4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80024E44);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800250F4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002583C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002584C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025854);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025874);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025BDC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025C00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025EE4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025F08);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026128);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002627C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002631C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026578);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026974);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026C70);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026D30);

void func_80026D84(void) {
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026D8C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026E90);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027044);

extern s32 D_801D6B24;

void func_800271D0(void) {
    D_801D6B24 = D_800793A0->unk40B8;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_800271EC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027228);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027410);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027458);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027674);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800276C8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002790C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002793C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027DB8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027DE8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80028228);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80028258);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80028558);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80028588);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800289A0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800289D0);

extern s32 D_8006DF98;
extern void func_80028D48(s32, s32, s32, s32 *, s32, s32);

void func_80028D18(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028D48(arg0, arg1, arg2, &D_8006DF98, arg3, arg4);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80028D48);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800293FC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002961C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80029990);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800299DC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80029A0C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80029EC4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80029EFC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002A5B4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002A5DC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002A710);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002A7CC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002A820);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002A8D4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002A9D4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AA8C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AB20);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AB5C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AB84);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002ABAC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AC70);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002ACC4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AD58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002ADEC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AEA4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B024);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B258);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B2C0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B300);

s32 SsVabTransBody(s32, s16);
s32 SsVabTransCompleted(s32);

void func_8002B38C(void *arg0, s32 arg1) {
    if ((arg1 == 0) || (SsVabTransBody(arg1, (*(s16 *)((s8 *)arg0 + 2))) == (*(s16 *)((s8 *)arg0 + 2)))) {
        SsVabTransCompleted(1);
    }
}

void func_8002B3DC(void) {
}

void func_8002B3E4(void) {
}

s32 func_80014A48(s32);
s32 func_801DFBAC(s32 *);
s32 func_801E055C(s32);
extern s32 D_80010598;
extern s32 D_800105A8;
extern s32 D_80010C9C;
void func_8001B358();

void func_8002B3EC(s32 arg0, s32 arg1) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, &func_8001B358, &D_80010598, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_801DFBAC(&D_800105A8);
    func_801E055C(arg0);
    func_80014A48(arg1);
}

extern short SsUtKeyOnV(short voice, short vabId, short prog, short tone,
                        short note, short fine, short voll, short volr);
extern s32 D_8006E044;
extern s16 D_8006E048;
extern u16 D_8006E04C;
extern s16 D_801D813E;

void func_8002B498(s32 arg0) {
    s32 tone = arg0 & 0xF;

    SsUtKeyOnV(D_8006E044, D_801D813E, arg0 >> 4, tone, D_8006E048,
               D_8006E04C + tone, 0x6E, 0x6E);
    if (++D_8006E044 >= 0x16) {
        D_8006E044 = 0x12;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B530);

void func_8002B5D0(s32 arg0, s32 arg1) {
    s32 tone = arg1 & 0xF;

    SsUtKeyOnV(arg0, D_801D813E, arg1 >> 4, tone, D_8006E048,
               D_8006E04C + tone, 0x6E, 0x6E);
}

s32 SsUtKeyOffV(s16);

void func_8002B644(s16 arg0) {
    SsUtKeyOffV(arg0);
}

s32 SsUtAllKeyOff(s32);

void func_8002B668(void) {
    SsUtAllKeyOff(0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B688);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B6E4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B7DC);

void func_8002B850(void) {
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B858);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B900);

s32 func_80014C08(s32);
extern s32 D_8006E040;
extern s32 D_800794F0;

void func_8002BA24(void) {
    do {
        func_80014C08(D_800794F0);
    } while (D_8006E040 != 0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BA6C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BB58);

extern s32 D_8006E03C;
extern s32 D_8006E040;

s32 func_8002BC2C(void) {
    s32 var_v1;

    var_v1 = 0;
    if (D_8006E03C == 0) {
        var_v1 = D_8006E040 == 0;
    }
    return var_v1;
}

s32 InitCARD(s32);
s32 func_8002BC80();

void func_8002BC58(void) {
    InitCARD(0);
    func_8002BC80();
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BC80);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BE84);

s32 func_8006A7B4(s32);
extern s32 D_801D8160;
extern s32 D_801D8164;
extern s32 D_801D8168;
extern s32 D_801D816C;

void func_8002BF60(void) {
    func_8006A7B4(D_801D8160);
    func_8006A7B4(D_801D8164);
    func_8006A7B4(D_801D8168);
    func_8006A7B4(D_801D816C);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BFB8);

s32 func_8006A7B4(s32);
extern s32 D_801D8170;
extern s32 D_801D8174;
extern s32 D_801D8178;
extern s32 D_801D817C;

void func_8002C094(void) {
    func_8006A7B4(D_801D8170);
    func_8006A7B4(D_801D8174);
    func_8006A7B4(D_801D8178);
    func_8006A7B4(D_801D817C);
}

s32 _card_clear(s32);
s32 func_8002BE84(s32);
s32 func_8002BFB8(s32);
s32 func_80068814(s32);
s32 func_80068824(s32);

s32 func_8002C0EC(s32 arg0) {
    s32 temp_s0;
    s32 temp_v0;
    s32 var_s0;

    var_s0 = 0;
loop_1:
    func_8002BF60();
    func_80068814(arg0 * 0x10);
    temp_v0 = func_8002BE84(0);
    if ((u32) (temp_v0 - 1) < 2U) {
        if (var_s0 >= 5) {
            return 1;
        }
        goto block_6;
    }
    if (temp_v0 == 3) {
        if (var_s0 < 3) {
block_6:
            var_s0 += 1;
            func_80014C08(D_800794F0);
            goto loop_1;
        }
        if (temp_v0 == 3) {
            temp_s0 = arg0 * 0x10;
            func_8002C094();
            _card_clear(temp_s0);
            func_8002BFB8(1);
            func_8002BF60();
            func_80068824(temp_s0);
            func_8002BE84(0);
        }
        /* Duplicate return node #9. Try simplifying control flow for better match */
        return 0;
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C1C0);

s32 _card_format(s32);

s32 func_8002C2E4(s32 arg0) {
    return _card_format(arg0 * 0x10) == 1;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C30C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C468);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C6EC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C784);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C9E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002CAC8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002CBA0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002CC04);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002CC44);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002D404);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002D458);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002D51C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002D898);

s32 func_80022D34(s32, s32, s32, s32);

s32 func_8001BB44(s32, s32, s32);
void func_8002DAAC(s32 arg0, s32 arg1) {
    void *temp_s1;

    temp_s1 = D_801D6A4C->unk13C[arg0];
    if ((*(s32 *)((s8 *)temp_s1 + 0x2200)) != arg1) {
        func_8001AFF0(arg0 + 0x84);
        func_80023094(temp_s1, func_8001BFF8(func_8001BB44((*(s32 *)((s8 *)temp_s1 + 0x26F4)), 1, arg1), arg0 + 0x84), arg1);
    }
    func_80022D34(arg0, arg1, -2, 0);
}

s32 func_8001BB44(s32, s32, s32);
s32 func_80023148(s32, s32);

void func_8002DB58(s32 arg0, s32 arg1) {
    s32 temp_s2;
    void *temp_s3;

    temp_s3 = D_801D6A4C->unk13C[arg0];
    temp_s2 = arg0 + 0x84;
    func_8001AFF0(temp_s2);
    func_80023094(temp_s3, func_8001BFF8(func_8001BB44((*(s32 *)((s8 *)temp_s3 + 0x26F4)), 1, arg1), temp_s2), arg1);
    func_80023148(arg0, arg1);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002DBEC);

s32 func_8001B248(s32 *, s32, s32);
s32 sprintf(s32 *, s32 *, s32);
extern s32 D_800107F8;

s32 func_8002DC30(s32 arg0, s32 arg1) {
    char sp10[32];
    s32 var_v0;

    var_v0 = func_8001BB44(arg1, 2, arg0);
    if (var_v0 == 0) {
        sprintf(sp10, &D_800107F8, arg0);
        var_v0 = func_8001B248(sp10, func_800148B0(), 0x81);
    }
    return var_v0;
}

s32 func_8002DC30(s32, s32);

void func_8002DC90(s32 arg0) {
    func_8002DC30(arg0, 0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002DCB0);

void *func_8002DBEC(s32);
s32 func_8002DCB0(s32, s32, s32, s32);
extern s32 D_801D81B8;

extern void *D_801D8340;
void func_8002DEA0(s32 arg0, void *arg1) {
    s32 temp_s5;
    s32 temp_s6;
    s32 temp_v0;
    void *temp_s1;
    void *temp_s2;

    temp_s1 = (void *)((s8 *)&D_801D81B8 + (arg0 << 5));
    temp_s5 = (*(u8 *)((s8 *)arg1 + 0xE5));
    temp_s6 = (*(s32 *)((s8 *)temp_s1 + 0));
    if (temp_s5 != temp_s6) {
        (*(s32 *)((s8 *)temp_s1 + 0)) = -2;
        if ((*(s8 *)((s8 *)D_801D8340 + 0x811)) == 1) {
            do {
                func_80014C08(D_800794F0);
            } while ((*(s8 *)((s8 *)D_801D8340 + 0x811)) == 1);
        }
        (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 1;
        if (temp_s6 > 0) {
            func_800235C8(arg0);
            func_8001AFF0(arg0 + 0x1F4);
            func_8001AFF0(arg0 + 0x84);
        }
        if (temp_s5 > 0) {
            temp_s2 = func_8002DBEC(temp_s5);
            temp_v0 = func_8002DCB0(arg0, temp_s5, 0, 0);
            if (temp_v0 != 0) {
                (*(s32 *)((s8 *)temp_s1 + 8)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x22)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0xC)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x3E)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x10)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x5A)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x14)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x24)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x18)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x40)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x1C)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x5C)), temp_v0);
                goto block_9;
            }
        } else {
block_9:
            (*(s32 *)((s8 *)temp_s1 + 0)) = temp_s5;
            (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 0;
        }
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E034);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E26C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E42C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E658);

extern s32 D_801D81A8;

void func_8002E7B8(void) {
    func_800235C8(0x17);
    func_8001AE90(D_801D81A8);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E7E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E8EC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002EB1C);

void D_801E8E88();
extern s32 D_80010884;

void func_8002ECDC(s8 arg0) {
    func_80014C08(2);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010884, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1600, D_801E8E88, (s32 *) arg0, 0, 0, 0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002ED9C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002EE50);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F074);

void D_801E8C04();
extern s32 D_80010864;
extern s32 D_80010894;

void func_8002F298(s32 *arg0) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801E8C04, arg0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
}

void D_801E4B34();

void func_8002F3C4(s32 *arg0) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, D_801E4B34, arg0, func_800148B0(), 1, 0);
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F4F4);

extern s32 D_801D8260;

void func_8002F79C(void) {
    D_801D8260 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F7A8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F8E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F920);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002FAA8);

extern s8 D_801D8264;

void func_8002FAC8(void) {
    D_801D8264 = -1;
}

extern s8 D_801D8266;

void func_8002FAD8(s8 arg0) {
    D_801D8266 = arg0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002FAE4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030130);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800301D0);

s32 func_80030130();
s32 rsin(s32, s32);

void func_80030264(void *arg0) {
    s32 temp_a1;

    func_80030130();
    temp_a1 = (*(s16 *)((s8 *)arg0 + 0x128)) + ((*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)));
    (*(s32 *)((s8 *)arg0 + 0x20)) = (s32) (((s32) ((*(s16 *)((s8 *)arg0 + 0x12A)) * rsin(temp_a1 * (*(s16 *)((s8 *)arg0 + 0x126)), temp_a1)) >> 0xA) + (*(s32 *)((s8 *)arg0 + 0x20)));
}

void func_800302E0(void *arg0) {
    s32 temp_a1;

    func_80030130();
    temp_a1 = (*(s16 *)((s8 *)arg0 + 0x128)) + ((*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)));
    (*(s32 *)((s8 *)arg0 + 0x24)) = (s32) (((s32) ((*(s16 *)((s8 *)arg0 + 0x12A)) * rsin(temp_a1 * (*(s16 *)((s8 *)arg0 + 0x126)), temp_a1)) >> 0xA) + (*(s32 *)((s8 *)arg0 + 0x24)));
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003035C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030440);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003058C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030694);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030718);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030828);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800309F0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030A34);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030AE4);

s32 PopMatrix();
s32 PushMatrix();
s32 func_80030F90(s32, s32);

void func_80030B6C(s32 arg0) {
    PushMatrix();
    func_80030F90(arg0, 0);
    PopMatrix();
}

void func_80030BA4(void *arg0) {
    func_8001AE90(arg0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030BC4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030CA8);

s32 func_80030BC4(void *, void *, void *);
s32 func_80030CA8(void *);

void *func_80030E3C(void *arg0) {
    func_8001EFDC(arg0, (*(s32 *)((s8 *)arg0 + 0x98)), (s32) (*(s16 *)((s8 *)arg0 + 0xD4)), (s32) (*(s16 *)((s8 *)arg0 + 0xD6)), (s32) (*(s16 *)((s8 *)arg0 + 0xD8)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE4)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE6)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE8)));
    func_8001EFDC(arg0 + 0x4C, (*(s32 *)((s8 *)arg0 + 0x98)), (s32) (*(s16 *)((s8 *)arg0 + 0xDC)), (s32) (*(s16 *)((s8 *)arg0 + 0xDE)), (s32) (*(s16 *)((s8 *)arg0 + 0xE0)), 0, 0, 0);
    (*(s32 *)((s8 *)arg0 + 0x14)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x18)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x1C)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x38)) = (s32) (*(s32 *)((s8 *)arg0 + 0xAC));
    (*(s32 *)((s8 *)arg0 + 0x3C)) = (s32) (*(s32 *)((s8 *)arg0 + 0xB0));
    (*(s32 *)((s8 *)arg0 + 0x40)) = (s32) (*(s32 *)((s8 *)arg0 + 0xB4));
    (*(u16 *)((s8 *)arg0 + 0x30)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE4));
    (*(u16 *)((s8 *)arg0 + 0x32)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE6));
    (*(u16 *)((s8 *)arg0 + 0x34)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE8));
    (*(s32 *)((s8 *)arg0 + 0x20)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD4));
    (*(s32 *)((s8 *)arg0 + 0x24)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD6));
    (*(s32 *)((s8 *)arg0 + 0x28)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD8));
    func_80030CA8(arg0);
    (*(s32 *)((s8 *)arg0 + 0x6C)) = (s32) (*(s16 *)((s8 *)arg0 + 0xDC));
    (*(s32 *)((s8 *)arg0 + 0x70)) = (s32) (*(s16 *)((s8 *)arg0 + 0xDE));
    (*(s32 *)((s8 *)arg0 + 0x74)) = (s32) (*(s16 *)((s8 *)arg0 + 0xE0));
    func_80030BC4(arg0 + 0xD4, arg0 + 0xDC, arg0 + 0x9C);
    (*(s32 *)((s8 *)arg0 + 0xFC)) = 0;
    (*(s16 *)((s8 *)arg0 + 0x132)) = 0;
    (*(s8 *)((s8 *)arg0 + 0x138)) = 0;
    return arg0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030F90);

void func_80031754(void *arg0) {
    if ((*(s16 *)((s8 *)arg0 + 0x12E)) >= 0x5B) {
        if ((*(s16 *)((s8 *)arg0 + 0x128)) > (*(s32 *)((s8 *)arg0 + 0x100))) {
            (*(s32 *)((s8 *)arg0 + 0x100)) += 1;
            return;
        }
        (*(s32 *)((s8 *)arg0 + 0x100)) = 0;
        (*(s8 *)((s8 *)arg0 + 0x139)) = 0;
        (*(s16 *)((s8 *)arg0 + 0x12E)) = (s16) ((u16) (*(s16 *)((s8 *)arg0 + 0x12E)) - 0x64);
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_800317A8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80031970);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80031F58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003230C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80032AA0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80032B44);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033258);

void func_80033CD4(void *arg0) {
    func_8001AE90((*(void **)((s8 *)arg0 + 0x140)));
    func_8001AE90(arg0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033D08);

s32 func_80014A90();

extern u8 *D_801D8348[];
s32 func_80033D9C(void) {
    void *var_v0_2;

    if ((*(s8 *)((s8 *)D_801D8340 + 0x815)) != 0) {
        (*(s8 *)((s8 *)D_801D8340 + 0x815)) = 0;
        func_80014A90();
        return -1;
    }
    if ((((u32) (*(u32 *)((s8 *)(D_801D8348[(*(s8 *)((s8 *)D_801D8340 + 0x817))]) + 0x178)) >> 0x11) & 3) == 1) {
        var_v0_2 = *D_80089840;
    } else {
        var_v0_2 = D_80089840[(*(s8 *)((s8 *)D_801D8340 + 0x817))];
    }
    if (!((*(u16 *)((s8 *)var_v0_2 + 0xA)) & 0x40)) {
        return 0;
    }
    func_8002B498(0xA0);
    return 1;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033E7C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033F34);

s32 func_8001A164(s32 *, s32);
extern s32 D_801D8278;

extern void *D_801D8340;
extern u8 *D_801D8348[];
void func_800341EC(void) {
    s32 var_a1;
    u32 temp_v0;

    temp_v0 = (*(u32 *)((s8 *)(D_801D8348[(*(s8 *)((s8 *)D_801D8340 + 0x817))]) + 0x178));
    var_a1 = (temp_v0 >> 0x11) & 1;
    if (((temp_v0 >> 0x11) & 3) == 1) {
        var_a1 = 0;
    }
    func_8001A164(&D_801D8278, var_a1);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80034260);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003917C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80039220);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80039354);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800395A0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80039730);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003B210);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003D4C4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003D9C0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003DA64);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003DB64);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003DBBC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003DD9C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003DF48);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003E11C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003E298);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003E3C8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003E4F0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003E844);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003E94C);

s32 func_801F8998(s32, s32, s32, s32, s32);
extern s32 D_80038F68;
extern void *D_8006E054;
extern s32 func_8002E034;
extern s32 func_80034260;
extern s32 func_80041E00;

void func_8003E9F4(s32 arg0) {
    s32 var_a0;
    s32 var_v1;

    (*(s32 *)((s8 *)D_801D8340 + 0x58)) = func_801F8998(0, 0x26, 0x2E, 0xA, 1);
    func_800149B8(0x1E, -1, 0, 0x800, &func_80034260, 0, 0, 0, 0);
    if ((arg0 != 0) && ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) == 0)) {
        func_800149B8(0, -1, 0, 0x800, &D_80038F68, 0, 0, 0, 0);
    }
    func_800149B8(0, -1, 0, 0x800, &func_80041E00, 0, 0, 0, 0);
    if (arg0 != 0) {
        var_a0 = (*(u8 *)((s8 *)D_8006E054 + 0x72));
        var_v1 = (*(u8 *)((s8 *)D_8006E054 + 0x71));
    } else {
        var_a0 = -1;
        var_v1 = -1;
    }
    func_800149B8(0, -1, 0, 0x1000, &func_8002E034, var_a0, var_v1, 0, 0);
}

s32 func_80014A00(s32);
s32 func_8001AFF0(s32);
s32 func_801F848C();
s32 func_801F88E8();

void func_8003EB50(void) {
    func_80014A00(0x19);
    func_801F848C();
    func_801F88E8();
    func_8001AFF0(0x7F);
}

s32 func_8003D4C4();
s32 func_8003E4F0();
s32 func_80042824(s16);
s32 func_80042E78();
s32 func_80043D00(s16);
s32 func_80044074(s16);
s32 func_801EB53C(u8);
s32 func_801F97F4();
extern void *D_801D8340;
extern u8 D_801D83D1;

void func_8003EB88(void) {
    s16 temp_a0;

    temp_a0 = (*(s16 *)((s8 *)D_801D8340 + 0x808));
    if (temp_a0 != 0) {
        func_80042824(temp_a0);
        func_80043D00((*(s16 *)((s8 *)D_801D8340 + 0x808)));
        func_80044074((*(s16 *)((s8 *)D_801D8340 + 0x808)));
    }
    func_80042E78();
    func_8003E4F0();
    func_8003D4C4();
    if ((*(s32 *)((s8 *)D_801D8340 + 0x83C)) == 0) {
        if ((*(s32 *)((s8 *)D_801D8340 + 0x828)) != -1) {
            func_801F97F4();
        }
        func_801EB53C(D_801D83D1);
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003EC4C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003F9EC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003FB3C);

extern u8 *D_801D8348[];

s32 func_80040064(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x19B;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 30);
    v = -1;
end:
    return v;
}

s32 func_800400B4(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 29; i >= 0; i--) {
        if ((((s8 *)D_801D8348[arg1]) + i)[0x19B] == arg0) {
            return -1;
        }
        base = (s8 *)D_801D8348[arg1] + 0x19B;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return 0;
        }
    }
    return -1;
}

s32 func_80040124(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x19B;
    do {
        if (p[i] != -1) {
            count++;
        }
        i++;
    } while (i < 30);
    return count;
}

s32 func_8004017C(s32 arg0) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 30; i++) {
        base = (s8 *)D_801D8348[arg0] + 0x19B;
        p = base + i;
        if (*p != -1) {
            s32 v = *p;
            *p = -1;
            return v;
        }
    }
    return -1;
}

s32 func_800401D0(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x17D;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 30);
    v = -1;
end:
    return v;
}

s32 func_80040220(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x17D;
    do {
        if (p[i] != -1) {
            count++;
        }
        i++;
    } while (i < 30);
    return count;
}

s32 func_80040278(s32 arg0) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 30; i++) {
        base = (s8 *)D_801D8348[arg0] + 0x17D;
        p = base + i;
        if (*p != -1) {
            s32 v = *p;
            *p = -1;
            return v;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_800402CC);

s32 func_800403F8(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 29; i >= 0; i--) {
        if ((((s8 *)D_801D8348[arg1]) + i)[0x17D] == arg0) {
            return -1;
        }
        base = (s8 *)D_801D8348[arg1] + 0x17D;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return 0;
        }
    }
    return -1;
}

s32 func_80040468(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1B9;
    do {
        if (p[i] == -1) {
            count++;
        }
        i++;
    } while (i < 4);
    return count;
}

s32 func_800404C0(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 4; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1B9;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return i;
        }
    }
    return -1;
}

s32 func_80040518(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 4; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1B9;
        p = base + i;
        if (*p == arg0) {
            *p = -1;
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040570);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040614);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800406BC);

s32 func_80040764(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1CA;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 3);
    v = -1;
end:
    return v;
}

s32 func_800407B4(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1CA;
    do {
        if (p[i] == -1) {
            count++;
        }
        i++;
    } while (i < 3);
    return count;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004080C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040A48);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040D88);

s32 func_800410B4(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 3; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1CA;
        p = base + i;
        if (*p == arg0) {
            *p = -1;
            return 0;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004110C);

s32 func_800411C4(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1C2;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 8);
    v = -1;
end:
    return v;
}

s32 func_80041214(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1C2;
    do {
        if (p[i] == -1) {
            count++;
        }
        i++;
    } while (i < 8);
    return count;
}

s32 func_8004126C(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 7; i >= 0; i--) {
        if ((((s8 *)D_801D8348[arg1]) + i)[0x1C2] == arg0) {
            return -1;
        }
        base = (s8 *)D_801D8348[arg1] + 0x1C2;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return 0;
        }
    }
    return -1;
}

s32 func_800412DC(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 8; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1C2;
        p = base + i;
        if (*p != -1 && *p == arg0) {
            *p = -1;
            return 0;
        }
    }
    return -1;
}

s32 func_80041340(s32 arg0) {
    return ((s8 *)D_801D8348[arg0])[0x1CD];
}

s32 func_80041364(s32 arg0) {
    return ((s8 *)D_801D8348[arg0])[0x1CD] == -1;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041390);

s32 func_80041408(s32 arg0) {
    s8 *p = (s8 *)D_801D8348[arg0];
    s32 v = p[0x1CD];

    p[0x1CD] = -1;
    return v;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041430);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041584);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800416D8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041A1C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041CA8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041E00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042174);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004269C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042824);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042BBC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042E78);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80043D00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80044074);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80044504);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800446A4);

extern s32 D_801D83F0;

void func_80044800(void) {
    D_801D83F0 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004480C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80044AB0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045700);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800457FC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045968);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045A58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045AB8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045B18);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045E1C);

extern s32 D_8006E050;

s32 func_80045F5C(s32 arg0, s32 arg1) {
    return (*(u8 *)((s8 *)(((arg0 * 0x2774) + D_8006E050 + arg1)) + 0x14B2)) & 7;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045F94);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045FE8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046038);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046088);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046118);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800461C0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004635C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046864);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046908);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800469A4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046A38);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046BAC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046C0C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046D68);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046FB8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004707C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800471F4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047248);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047364);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047438);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047620);

s32 func_80047620(s32, s32, s32);

void func_80047A38(s32 arg0, s32 arg1) {
    func_80047620(arg0, arg1, 1);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047A58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047A98);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047B84);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047C38);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047D5C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047E64);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80048014);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80048150);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80048230);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800493EC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004949C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004950C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800495B4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800496E4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80049840);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80049934);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004994C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80049A14);

extern s32 D_80012D68;

void func_80049DC0(void *arg0) {
    func_80028D18((*(s16 *)((s8 *)arg0 + 0)), (*(s16 *)((s8 *)arg0 + 2)), &D_80012D68, 0, (s32) (*(s16 *)((s8 *)arg0 + 0x3A)));
}

extern s32 D_80012DB8;

void func_80049E00(void *arg0) {
    func_80028D18((*(s16 *)((s8 *)arg0 + 0)), (*(s16 *)((s8 *)arg0 + 2)), &D_80012DB8, 7, (s32) (*(s16 *)((s8 *)arg0 + 0x3A)));
}

extern s32 D_80012DF8;

void func_80049E40(void *arg0) {
    func_80028D18((*(s16 *)((s8 *)arg0 + 0)), (*(s16 *)((s8 *)arg0 + 2)), &D_80012DF8, 7, (s32) (*(s16 *)((s8 *)arg0 + 0x3A)));
}

s32 func_800170F0(s32 *, s32 *, s32);
extern s32 D_801D8410;
extern s32 D_801D8460;
extern s32 D_801D84B0;
extern s32 D_801D84F4;
void func_80049A14();

void func_80049E80(void) {
    func_800170F0(&D_801D8460, &func_80049E40, 0xA);
    func_800170F0(&D_801D84F4, &func_80049E00, 0xA);
    func_800170F0(&D_801D84B0, &func_80049DC0, 0xA);
    func_800170F0(&D_801D8410, &func_80049A14, 0xA);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80049EF8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004A2DC);
