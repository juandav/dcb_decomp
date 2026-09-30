#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void ExitCriticalSection(void);
void func_8006AF84(long, u_char *);
void func_8006AF94(long, u_char *);

extern long D_80077998;
extern u_char D_800779C0[];
extern volatile long *D_80077938;
extern void (*D_80077964)(u_char *);
extern u_char *D_80077994;
typedef struct {
    long unk0;
    long unk4;
} PadUnk801DDF30;
extern PadUnk801DDF30 D_801DDF30;

void PadStartCom(void) {
    volatile long *p;

    D_80077998 = 0;
    EnterCriticalSection();
    func_8006AF94(2, D_800779C0);
    func_8006AF84(2, D_800779C0);
    p = D_80077938;
    p[0] = -2;
    p[1] |= 1;
    ChangeClearRCnt(3, 0);
    ExitCriticalSection();
    D_80077964(D_80077994);
    D_80077964(D_80077994 + 0xF0);
    D_801DDF30.unk0 = D_801DDF30.unk4 = 0;
    D_80077998 = 1;
}

extern u_char D_800779C0[];

void PadStopCom(void) {
    EnterCriticalSection();
    ChangeClearRCnt(3, 1);
    func_8006AF94(2, D_800779C0);
    ExitCriticalSection();
}
