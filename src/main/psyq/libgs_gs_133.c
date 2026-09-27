#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsGetLw);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsGetLs);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsGetLws);

typedef struct {
    u_long *vertop;
    u_long vern;
    u_long *nortop;
    u_long norn;
    u_long *primtop;
    u_long primn;
    u_long scale;
} TmdObj;

int printf(char *fmt, ...);

void GsLinkObject4(u_long tmd_base, GsDOBJ2 *objp, int n) {
    TmdObj *obj;
    u_long *p;
    u_long *top;
    u_long np;
    u_long i;
    long cnt = 0;
    u_long mode = 0;
    u_long flag = 0;
    u_long prevmode;
    u_long prevflag;

    obj = (TmdObj *)tmd_base + n;
    objp->tmd = (u_long *)obj;
    p = obj->primtop;
    np = obj->primn;
    top = p;
    for (i = 0; i < np; i++) {
        prevmode = mode;
        prevflag = flag;
        mode = *p >> 24;
        flag = *p >> 16;
        if ((u_char)prevmode != 0 && (mode != (u_char)prevmode || (u_char)flag != (u_char)prevflag)) {
            *(u_short *)top = cnt;
            cnt = 0;
            top = p;
        }
        switch (mode & 0xFD) {
        case 0x20:
            if (flag & 4) {
                p += 6;
            } else {
                p += 4;
            }
            break;
        case 0x25:
            p += 7;
            break;
        case 0x31:
            p += 6;
            break;
        case 0x29:
            p += 4;
            break;
        case 0x3C:
            p += 9;
            break;
        case 0x30:
            if (flag & 4) {
                p += 7;
            } else {
                p += 5;
            }
            break;
        case 0x28:
            if (flag & 4) {
                p += 8;
            } else {
                p += 5;
            }
            break;
        case 0x21:
            p += 4;
            break;
        case 0x24:
            p += 6;
            break;
        case 0x2C:
        case 0x2D:
            p += 8;
            break;
        case 0x3D:
            p += 11;
            break;
        case 0x35:
            p += 9;
            break;
        case 0x38:
            if (flag & 4) {
                p += 9;
            } else {
                p += 6;
            }
            break;
        case 0x39:
            p += 7;
            break;
        case 0x34:
            p += 7;
            break;
        default:
            printf("GPU CODE %02xH not assigned.\n", mode);
            break;
        }
        cnt++;
    }
    *(u_short *)top = cnt;
}

/* ASPSX padded the jump table of the object as well */
__asm__(".section .rodata\n\t.space 8\n");

OBJECT_END(3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSortObject4);
