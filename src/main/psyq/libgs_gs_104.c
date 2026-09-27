#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsInit3D);

typedef struct {
    u_long vert_top;
    u_long n_vert;
    u_long normal_top;
    u_long n_normal;
    u_long primitive_top;
    u_long n_primitive;
    long scale;
} TmdObj;

void GsMapModelingData(u_long *p) {
    int i;
    int n;
    TmdObj *obj;

    if (*p & 1) {
        return;
    }
    *p++ |= 1;
    i = 0;
    n = *p++;
    if (n > 0) {
        obj = (TmdObj *)p;
        do {
            obj[i].vert_top += (u_long)p;
            obj[i].normal_top += (u_long)p;
            obj[i].primitive_top += (u_long)p;
            i++;
        } while (i < n);
    }
}

void func_80062484(void) {
    func_8005C4A4();
}
