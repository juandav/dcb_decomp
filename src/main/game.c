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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800148B0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800148C8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001491C);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80015848);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800158B0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80015A3C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80015AD8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80015EAC);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001683C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016878);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800168C4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016948);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016BEC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016C08);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016F38);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001705C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800170F0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800176E4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800177E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80017B88);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80018694);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80018B8C);

extern int *D_800793A0;
extern int D_800897E8;
extern unsigned short D_800897EC;
extern char D_80010008[];
extern int printf(const char *, ...);

int func_80019084(void) {
    if (D_800897E8 == D_800793A0[0x40BC / 4] + D_800897EC * 0x294) {
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A100);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A164);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A1D8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A40C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A600);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A688);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A6B0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A7A4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A9B0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AA80);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AB64);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001ABCC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001ACEC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AD0C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AD3C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AE70);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AE90);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AFF0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B088);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B10C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B144);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B248);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B358);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B438);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B5BC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B634);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B734);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B7F4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B90C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B930);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BB44);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BB94);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BC14);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BC38);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BCA4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BD60);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BDEC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BFCC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BFF8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C078);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C0A8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C1E0);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001D6D0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001D7DC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001D900);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001DA24);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001DBAC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001DD4C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001DE58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001DFE0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E180);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E2A4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E3C8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E4E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E6A4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E6EC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E75C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E76C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E7B8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E804);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E850);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E894);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E8A4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E8B4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E8C4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E8D4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E8E4);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001ED04);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001ED30);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EDE0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EEA0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EFB0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EFDC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F01C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F040);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F04C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F058);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F068);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020E34);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020E94);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020ED4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020F24);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020F54);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021954);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002195C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021964);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021AA8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021B60);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021C18);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021DF8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022100);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022B98);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022C4C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022CA4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022D00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022D34);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022DBC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022E58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022ED0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022F34);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023094);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800230B8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023128);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023148);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023408);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023454);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023468);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800234AC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800235C8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002360C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800236B4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002371C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002386C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023DA4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023DC8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023DF0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800243B0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80024420);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800271D0);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80028D18);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B38C);

void func_8002B3DC(void) {
}

void func_8002B3E4(void) {
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B3EC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B498);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B530);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B5D0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B644);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B668);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B688);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B6E4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B7DC);

void func_8002B850(void) {
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B858);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B900);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BA24);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BA6C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BB58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BC2C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BC58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BC80);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BE84);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BF60);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BFB8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C094);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C0EC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C1C0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C2E4);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002DAAC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002DB58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002DBEC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002DC30);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002DC90);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002DCB0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002DEA0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E034);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E26C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E42C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E658);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E7B8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E7E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E8EC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002EB1C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002ECDC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002ED9C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002EE50);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F074);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F298);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F3C4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F4F4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F79C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F7A8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F8E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F920);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002FAA8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002FAC8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002FAD8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002FAE4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030130);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800301D0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030264);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800302E0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003035C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030440);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003058C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030694);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030718);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030828);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800309F0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030A34);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030AE4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030B6C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030BA4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030BC4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030CA8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030E3C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030F90);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80031754);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800317A8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80031970);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80031F58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003230C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80032AA0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80032B44);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033258);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033CD4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033D08);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033D9C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033E7C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033F34);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800341EC);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003E9F4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003EB50);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003EB88);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003EC4C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003F9EC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003FB3C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040064);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800400B4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040124);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004017C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800401D0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040220);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040278);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800402CC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800403F8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040468);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800404C0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040518);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040570);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040614);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800406BC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040764);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800407B4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004080C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040A48);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040D88);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800410B4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004110C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800411C4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041214);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004126C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800412DC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041340);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041364);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041390);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041408);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80044800);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004480C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80044AB0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045700);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800457FC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045968);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045A58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045AB8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045B18);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045E1C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045F5C);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047A38);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80049DC0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80049E00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80049E40);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80049E80);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80049EF8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004A2DC);
