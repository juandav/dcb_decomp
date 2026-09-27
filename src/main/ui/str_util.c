#include "common.h"
#include "gte.h"
#include "game.h"

s8 *func_8002A5DC(s8 *buf, s8 pad, s32 n, s32 width) {
    s8 *q;
    s8 *r;
    s32 cnt;
    s8 c;

    buf += width;
    q = buf;
    *buf = 0;
    cnt = 0;
    do {
        q--;
        c = n % 10 + '0';
        *q = c;
        n /= 10;
        if (--width <= 0 && n != 0) {
            buf++;
            for (r = buf; q < r; r--) {
                *r = r[-1];
            }
            q++;
        }
        if (++cnt % 3 == 0) {
            if (n == 0) {
                break;
            }
            *--q = ',';
            if (--width <= 0) {
                buf++;
                for (r = buf; q < r; r--) {
                    *r = r[-1];
                }
                q++;
            }
        }
    } while (n != 0);
    while (--width >= 0) {
        *--q = pad;
    }
    return buf;
}

s8 *func_8002A710(s8 *buf, s8 pad, s32 n, s32 width) {
    s8 *q;
    s8 *r;

    buf += width;
    q = buf;
    *buf = 0;
    do {
        *--q = n % 10 + '0';
        n /= 10;
        if (--width <= 0 && n != 0) {
            buf++;
            for (r = buf; q < r; r--) {
                *r = r[-1];
            }
            q++;
        }
    } while (n != 0);
    while (--width >= 0) {
        *--q = pad;
    }
    return buf;
}

void func_8002A7CC(s8 *arg0, s32 arg1, s32 arg2) {
    if (arg1 >= 0) {
        *arg0++ = '+';
    } else {
        *arg0++ = '-';
        arg1 = -arg1;
    }
    func_8002A710(arg0, '0', arg1, arg2 - 1);
}

s8 *func_8002A820(s8 *buf, s32 n) {
    s8 *s;

    switch (n) {
    case 1:
        s = "1ab";
        break;
    case 2:
        s = "2cd";
        break;
    case 3:
        s = "3ef";
        break;
    default:
        if (n < 10) {
            return func_8002A5B4(func_8002A710(buf, ' ', n, 1), "gh");
        }
        return func_8002A5B4(func_8002A710(buf, ' ', n, 2), "i");
    }
    return func_8002A5B4(buf, s);
}

s8 *func_8002A8D4(s8 *arg0, s8 *arg1, s32 arg2) {
    s32 pad;
    s32 i;
    s8 *p;

    pad = arg2 - strlen(arg1);
    if (pad < 0) {
        p = arg0;
        for (i = 0; i < arg2; i++) {
            *p++ = '*';
        }
    } else {
        pad /= 2;
        p = arg0;
        while (pad-- > 0) {
            *p++ = ' ';
            arg2--;
        }
        while ((*p = *arg1++) != 0) {
            p++;
            arg2--;
        }
        while (arg2-- > 0) {
            *p++ = ' ';
        }
    }
    *p = 0;
    return p;
}

u16 func_8002A9D4(u8 **ps) {
    u8 *s = *ps;

    if (s[0] > 0x80 && (s[0] < 0xA0 || (s[0] >= 0xE0 && s[0] < 0xF0))) {
        if (s[1] >= 0x40 && (s[1] < 0x7F || (s[1] >= 0x80 && s[1] < 0xFD))) {
            *ps += 2;
            return ((*ps)[-2] << 8) | (*ps)[-1];
        }
    }
    return *(*ps)++;
}

u16 func_8002AA8C(u8 *s) {
    if (s[0] > 0x80 && (s[0] < 0xA0 || (s[0] >= 0xE0 && s[0] < 0xF0))) {
        if (s[1] >= 0x40 && (s[1] < 0x7F || (s[1] >= 0x80 && s[1] < 0xFD))) {
            return (s[0] << 8) | s[1];
        }
    }
    return s[0];
}

s32 func_8002AB20(s16 *s) {
    s16 *p;
    s32 w;

    p = s;
    w = 0;
loop:
    p++;
    if (*p != 0) {
        if (*p < 0) {
            w += 1;
        } else {
            w += 2;
        }
        goto loop;
    }
    *s = w;
    return w;
}

s16 *func_8002AB5C(s16 *d, u8 *s) {
    if ((*d = -*s) == 0) {
        return d;
    }
    s++;
    d++;
    return func_8002AB5C(d, s);
}

s16 *func_8002AB84(s16 *d, s16 *s) {
    if ((*d = *s) == 0) {
        return d;
    }
    return func_8002AB84(d + 1, s + 1);
}

s16 *func_8002ABAC(s16 *buf, u8 pad, s32 n, s32 width) {
    s16 *q;
    s16 *r;
    s32 fill;

    fill = -pad;
    buf += width;
    q = buf;
    *buf = 0;
    do {
        *--q = -'0' - n % 10;
        n /= 10;
        if (--width <= 0 && n != 0) {
            buf++;
            for (r = buf; q < r; r--) {
                *r = r[-1];
            }
            q++;
        }
    } while (n != 0);
    while (--width >= 0) {
        *--q = fill;
    }
    return buf;
}

void func_8002AC70(s16 *arg0, s32 arg1, s32 arg2) {
    if (arg1 >= 0) {
        *arg0++ = -0x2B;
    } else {
        *arg0++ = -0x2D;
        arg1 = -arg1;
    }
    func_8002ABAC(arg0, 0x30, arg1, arg2 - 1);
}

s16 *func_8002ACC4(s16 *buf, s32 n) {
    u8 *s;

    switch (n) {
    case 1:
        s = "1st";
        break;
    case 2:
        s = "2nd";
        break;
    case 3:
        s = "3rd";
        break;
    default:
        return func_8002AB5C(func_8002ABAC(buf, ' ', n, 1), "th");
    }
    return func_8002AB5C(buf, s);
}

s8 *func_8002AD58(s8 *buf, s32 n) {
    s8 *s;

    switch (n) {
    case 1:
        s = "1ST";
        break;
    case 2:
        s = "2ND";
        break;
    case 3:
        s = "3RD";
        break;
    default:
        return func_8002A5B4(func_8002A710(buf, ' ', n, 1), "TH");
    }
    return func_8002A5B4(buf, s);
}
