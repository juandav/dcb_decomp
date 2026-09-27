#include "common.h"
#include "gte.h"
#include "game.h"

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

s32 func_8001AB64(void) {
    s32 *p;
    s32 i;
    s32 max;

    max = 0;
    p = &D_80089848;
    i = 0x3FF;
    if (D_80089848 != 0) {
        do {
            if (p[0] > 0 && max < p[1]) {
                max = p[1];
            }
            i--;
            p += 3;
        } while (i >= 0 && p[0] != 0);
    }
    return max;
}

void *func_8001ABCC(s32 size, s32 tag) {
    s32 *p;
    s32 *q;
    s32 i;
    s32 addr;
    s32 avail;
    s32 res;

    size = (size + 3) & ~3;
    if (size == 0) {
        return 0;
    }
    func_80014970();
    p = &D_80089848;
    i = 0x3FF;
    if ((addr = D_80089848) != 0) {
        do {
            if (addr >= 0) {
                avail = p[1];
                if (avail >= size) {
                    res = addr | 0x80000000;
                    p[0] = res;
                    p[1] = size;
                    avail -= size;
                    p[2] = tag;
                    if (avail != 0) {
                        addr += size;
                        q = &D_80089848 + 0x3FF * 3;
                        for (i--; i > 0; i--) {
                            q[0] = q[-3];
                            q[1] = q[-2];
                            q[2] = q[-1];
                            q -= 3;
                        }
                        q[0] = addr;
                        q[1] = avail;
                        q[2] = -1;
                    }
                    func_800149A0();
                    return (void *)res;
                }
            }
            i--;
            p += 3;
        } while (i >= 0 && (addr = p[0]) != 0);
    }
    func_800149A0();
    return 0;
}

void *func_8001ACEC(s32 arg0) {
    return func_8001ABCC(arg0, -2);
}

void *func_8001AD0C(s32 size) {
    return func_8001ABCC(size, func_800148B0());
}

void *func_8001AD3C(void *ptr, s32 size) {
    s32 *p;
    s32 i;
    s32 addr;
    s32 rest;

    size = (size + 3) & ~3;
    func_80014970();
    p = &D_80089848;
    i = 0x3FF;
    if ((addr = D_80089848) != 0) {
        do {
            if (addr == (s32)ptr) {
                rest = p[1] - size;
                if (rest < 0) {
                    func_800149A0();
                    return 0;
                }
                if (rest != 0 && i != 0) {
                    p[1] = size;
                    p += 3;
                    addr += size;
                    if (p[0] > 0) {
                        rest += p[1];
                    } else {
                        p = &D_80089848 + 0x3FF * 3;
                        for (i--; i > 0; i--) {
                            p[0] = p[-3];
                            p[1] = p[-2];
                            p[2] = p[-1];
                            p -= 3;
                        }
                    }
                    p[0] = addr & 0x3FFFFFFF;
                    p[1] = rest;
                    p[2] = -1;
                }
                func_800149A0();
                return ptr;
            }
            i--;
            p += 3;
        } while (i >= 0 && (addr = p[0]) != 0);
    }
    func_800149A0();
    return 0;
}

void func_8001AE70(void *arg0) {
    func_8001AE90(arg0);
}

s32 func_8001AE90(void *ptr) {
    s32 *p;
    s32 *q;
    s32 i;
    s32 addr;
    s32 size;

    if (ptr == 0) {
        return 0;
    }
    func_80014970();
    p = &D_80089848;
    i = 0x3FF;
    if ((addr = D_80089848) != 0) {
        do {
            if (addr == (s32)ptr) {
                addr &= 0x3FFFFFFF;
                size = p[1];
                q = p;
                if (p != &D_80089848 && p[-3] > 0) {
                    p -= 3;
                    addr = p[0];
                    size += p[1];
                    i++;
                }
                if (i > 0 && q[3] > 0) {
                    if (q != p) {
                        i--;
                    }
                    q += 3;
                    size += q[1];
                }
                p[0] = addr;
                p[1] = size;
                p[2] = -1;
                if (p != q) {
                    for (i--; i > 0; i--) {
                        p += 3;
                        q += 3;
                        p[0] = q[0];
                        p[1] = q[1];
                        p[2] = q[2];
                    }
                    while (p < q) {
                        p += 3;
                        p[0] = 0;
                        p[2] = 0;
                    }
                }
                func_800149A0();
                return 0;
            }
            i--;
            p += 3;
        } while (i >= 0 && (addr = p[0]) != 0);
    }
    func_800149A0();
    return 0;
}

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

s32 func_8001B088(s32 y, s32 x) {
    s32 a;

    if (x == 0) {
        if (y > 0) {
            return 0x400;
        }
        if (y < 0) {
            return -0x400;
        }
        return 0;
    }
    a = catan((y << 12) / x);
    if (x < 0) {
        if (y <= 0) {
            a -= 0x800;
        } else {
            a += 0x800;
        }
    }
    return a;
}
