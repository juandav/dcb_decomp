#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/stage.h"
#include "dcb/archive.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/model.h"
#include "dcb/model_anim.h"
#include "dcb/player_data.h"
#include "dcb/prim_util.h"
#include "dcb/sound.h"

BgEntry D_8006E0C0[56] = {
    { 0x50, 8, 0xA, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x51, 8, 0xA, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x52, 8, 0xA, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x53, 8, 0xA, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x58, 8, 5, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x59, 8, 2, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x5B, 8, 2, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x5D, 0x10, 0xC, -0x80, { 0, 0, 0 }, 0xFF },
    { 0x5F, 0x10, 6, -0x80, { 0, 0, 0 }, 0xFF },
    { 0x61, 0x10, 4, -0x80, { 0, 0, 0 }, 0xFF },
    { 0x5E, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x13, 4, 0, 0x20, { 0, 0, 0 }, 0x80 },
    { 0x62, 0x10, 0xA, -0x80, { 0, 0, 0 }, 0xFF },
    { 0x46, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x48, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4D, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4E, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4F, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4C, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4A, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x49, 8, 0xA, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x57, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x56, 0x10, 0xF, -0x7E, { 0, 0, 0 }, 0xFF },
    { 0x55, 0, 0, 2, { 0, 0, 0 }, 0xFF },
    { 0x45, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4B, 0, 0, 2, { 0, 0, 0 }, 0xFF },
    { 0x5A, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x54, 0, 0, 2, { 0, 0, 0 }, 0xFF },
    { 0x43, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x47, 0, 0, 2, { 0, 0, 0 }, 0xFF },
    { 0x42, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x40, 0x10, 0xF, -0x7E, { 0, 0, 0 }, 0xFF },
    { 0, 0xD, 0, 0x68, { 0, 0, 0 }, 0x46 },
    { 1, 7, 0, 0x38, { 0, 0, 0 }, 0x46 },
    { 2, 7, 0, 0x38, { 0, 0, 0 }, 0x80 },
    { 3, 0xA, 0, 0x52, { 0, 0, 0 }, 0x50 },
    { 4, 7, 0, 0x3A, { 0, 0, 0 }, 0x80 },
    { 5, 8, 0, 0x40, { 0, 0, 0 }, 0x80 },
    { 6, 8, 0, 0x40, { 0, 0, 0 }, 0x50 },
    { 7, 0xA, 0, 0x50, { 0, 0, 0 }, 0x80 },
    { 8, 3, 0, 0x18, { 0, 0x4C, 0x8C }, 0x46 },
    { 9, 6, 0, 0x30, { 0, 0, 0 }, 0x64 },
    { 0xA, 0xE, 0, 0x72, { 0, 0, 0 }, 0x80 },
    { 0xB, 6, 0, 0x30, { 0, 0, 0 }, 0x80 },
    { 0xC, 4, 0, 0x20, { 0xC8, 0xC8, 0xC8 }, 0x50 },
    { 0xD, 7, 0, 0x38, { 0, 0, 0 }, 0x80 },
    { 0xE, 0xC, 0, 0x62, { 0, 0, 0 }, 0x80 },
    { 0xF, 0xA, 0, 0x52, { 0, 0, 0 }, 0x50 },
    { 0x10, 0x10, 0, -0x7E, { 0, 0, 0 }, 0x80 },
    { 0x11, 0xA, 0, 0x52, { 0, 0, 0 }, 0x80 },
    { 0x12, 7, 0, 0x38, { 0, 0, 0 }, 0x80 },
    { 0x13, 4, 0, 0x20, { 0, 0, 0 }, 0x80 },
    { 0x14, 5, 0, 0x28, { 0, 0, 0 }, 0x80 },
    { 0x15, 0xA, 0, 0x50, { 0, 0, 0 }, 0x80 },
    { 0x16, 7, 0, 0x3A, { 0, 0, 0 }, 0x80 },
    { 0x17, 7, 0, 0x38, { 0, 0, 0 }, 0x80 },
};

s32 func_8002DCB0(s32 slot, s32 id, s8 kind, s32 anims) {
    char path[32];
    s32 pak;

    if (kind == 1) {
        sprintf(path, "G:\\%03d.PAK", id);
    } else {
        sprintf(path, "F:\\%03d.PAK", id);
    }
    pak = func_8001B248((s32 *)path, func_800148B0(), slot + 0x1F4);
    if (func_8002386C(slot, id, -1, pak, kind) == 0) {
        return pak;
    }
    if (anims != 0) {
        func_800230B8(slot, 0, 0, pak);
        func_800230B8(slot, 7, 7, pak);
        func_800230B8(slot, 1, 1, pak);
        func_800230B8(slot, 2, 2, pak);
        func_800230B8(slot, 3, 3, pak);
        func_800230B8(slot, 4, 4, pak);
        func_800230B8(slot, 5, 5, pak);
        func_800230B8(slot, 6, 6, pak);
    } else {
        func_80023094(D_801D6A4C->unk13C[slot],
                      (s32 *)func_8001BFF8(
                          (s32)func_8001BB44((Chunk *)((Model2220 *)D_801D6A4C->unk13C[slot])->unk26F4, 1, 7), slot + 0x84),
                      7);
        func_80023148(slot, 7);
    }
    D_801D6A4C->unk114[slot] = -1;
    func_8001BC14((Chunk *)pak);
    return pak;
}

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
            temp_s2 = findDigimonCardByModelId(temp_s5);
            temp_v0 = func_8002DCB0(arg0, temp_s5, 0, 0);
            if (temp_v0 != 0) {
                (*(s32 *)((s8 *)temp_s1 + 8)) = loadSkill((s32) (*(s16 *)((s8 *)temp_s2 + 0x22)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0xC)) = loadSkill((s32) (*(s16 *)((s8 *)temp_s2 + 0x3E)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x10)) = loadSkill((s32) (*(s16 *)((s8 *)temp_s2 + 0x5A)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x14)) = loadSkill((s32) (*(s16 *)((s8 *)temp_s2 + 0x24)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x18)) = loadSkill((s32) (*(s16 *)((s8 *)temp_s2 + 0x40)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x1C)) = loadSkill((s32) (*(s16 *)((s8 *)temp_s2 + 0x5C)), temp_v0);
                goto block_9;
            }
        } else {
block_9:
            (*(s32 *)((s8 *)temp_s1 + 0)) = temp_s5;
            (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 0;
        }
    }
}

void func_8002E034(s32 bg) {
    s32 pak;
    s32 i;
    u8 *s;

    if (*((s8 *)D_801D8340 + 0x811) == 1) {
        do {
            func_80014C08(D_800794F0);
        } while (*((s8 *)D_801D8340 + 0x811) == 1);
    }
    *((s8 *)D_801D8340 + 0x811) = 1;
    *((s8 *)D_801D8340 + 0x813) = 0;
    pak = func_8001B144((s32) "A:\\BATTLE.PAK", func_800148B0());
    if (pak != 0) {
        func_8001B5BC(func_8001BB44((Chunk *)pak, 5, 0x68));
        D_801D81AC = (void *)loadSkill(999, pak);
        D_801D81B0 = (void *)loadSkill(998, pak);
        func_8001BC14((Chunk *)pak);
    }
    func_8002E42C(bg);
    D_801D81B8 = (&D_801D81B8)[8] = -1;
    *((s8 *)D_801D8340 + 0x811) = 0;
    do {
        func_80014C08(D_800794F0);
        for (i = 0; i < 2; i++) {
            s = *(u8 **)(D_801D8348[i] + 0x114);
            if (s != 0 && s[0xE5] != (&D_801D81B8)[i * 8]) {
                func_8002DEA0(i, s);
            }
        }
    } while (*((s8 *)D_801D8340 + 0x813) == 0);
    for (i = 0; i < 2; i++) {
        if ((&D_801D81B8)[i * 8] > 0) {
            func_80022DBC(i);
            func_800235C8(i);
        }
    }
    func_8002E7B8();
    func_8001AFF0(0x1F4);
    func_8001AFF0(0x84);
    func_8001AFF0(0x1F5);
    func_8001AFF0(0x85);
    func_8001AFF0(0x81);
    *((s8 *)D_801D8340 + 0x813) = 0;
}

void func_8002E26C(void) {
    do {
        func_80014C08(D_800794F0);
    } while (D_801D81B8 <= 0 || (&D_801D81B8)[8] <= 0 || *((s8 *)D_801D8340 + 0x811) == 1);
    *((s8 *)D_801D8340 + 0x811) = 1;
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\sugseg.bin", D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_8002B858(1);
    func_800149B8(0, -1, 0, 0x2000, D_801EEE90, 0, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    D_80079544 = 0;
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\kawseg.bin", D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_8002B858(0);
    *((s8 *)D_801D8340 + 0x811) = 0;
}

void func_8002E42C(s32 n) {
    char path[32];

    if (n < 0) {
        n = rand() % 12 + 0x2C;
    }
    sprintf(path, "F:\\bg%d.pak", D_8006E0C0[n].bg + 900);
    func_800149B8(0, -1, 0, 0x400, func_8001B248, path, func_800148B0(), 0x81);
    D_801D81A8 = func_80014C08(0x7FFFFFFF);
    func_8002386C(0x17, D_8006E0C0[n].bg + 900, 0, D_801D81A8, 0);
    D_801D6A4C->unk114[0x17] = -1;
    ((Model2220 *)D_801D6A4C->unk13C[23])->unk26D4 = 0xA0000;
    ((Model2220 *)D_801D6A4C->unk13C[23])->unk26D0 = 0x280000;
    if (D_8006E0C0[n].flags & 2) {
        func_800230B8(0x17, 0, 0, D_801D81A8);
        func_80023148(0x17, 0);
        func_80022D34(0x17, 0, -2, 0);
    }
    D_801D6A4C->unk114[0x18] = D_801D6A4C->unk114[0x19] = 0;
    D_801D6A4C->unk114[0x1B] = D_8006E0C0[n].unk2;
    D_801D6A4C->unk114[0x1A] = D_8006E0C0[n].unk1;
    *(s32 *)&D_801D6A4C->unk114[0x24] = D_8006E0C0[n].flags;
    D_801D6A60[0] = D_8006E0C0[n].rgb[0];
    D_801D6A60[1] = D_8006E0C0[n].rgb[1];
    D_801D6A60[2] = D_8006E0C0[n].rgb[2];
    D_8006DF80 = D_8006E0C0[n].unk7;
}

void func_8002E658(s16 id) {
    Unk801D6A4C *p;
    s32 tim;

    D_80079544 = 1;
    p = D_801D6A4C;
    *(s16 *)((u8 *)p->unk13C[23] + 0xA78) = id;
    tim = func_8001C078((s32)func_8001BB44((Chunk *)D_801D81A8, 5, *(s16 *)((u8 *)p->unk13C[23] + 6)));
    func_8001B438((u32 *)tim, 0x3C0, 0, 0x3F0, 0x70);
    DrawSync(0);
    func_8001AE90((void *)tim);
    if (id != 0 && (*(s32 *)&D_801D6A4C->unk114[0x24] & 2)) {
        func_80014A00(0x1B);
        func_800149B8(0x1B, -1, 0, 0x1000, func_80022B98, 1);
        func_80023148(0x17, 0);
        func_80022D34(0x17, 0, -2, 0);
    }
    ((Unk800794F8 *)&D_800794F8)->unk98[0].draw.r0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].draw.r0 = D_801D6A60[0];
    ((Unk800794F8 *)&D_800794F8)->unk98[0].draw.g0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].draw.g0 = D_801D6A60[1];
    ((Unk800794F8 *)&D_800794F8)->unk98[0].draw.b0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].draw.b0 = D_801D6A60[2];
}

void func_8002E7B8(void) {
    func_800235C8(0x17);
    func_8001AE90(D_801D81A8);
}

void func_8002E7E8(u8 *arg0) {
    s32 f;

    if (D_801D6A4C->unk114[0x1B] != 0) {
        if (++D_801D6A4C->unk114[0x19] >= D_801D6A4C->unk114[0x1B]) {
            f = *(s32 *)(arg0 + 0x26D4) / 0x10000 + 5;
            D_801D6A4C->unk114[0x19] = 0;
            if (++D_801D6A4C->unk114[0x18] >= (u8)D_801D6A4C->unk114[0x24] >> 3) {
                D_801D6A4C->unk114[0x18] = 0;
            }
            *(s32 *)(arg0 + 0x26D0) =
                (((((f & 0x10) << 4) + D_801D6A4C->unk114[0x18]) << 6 | (f & 0xF) << 2) - 0x14) << 16;
        }
    }
}

void func_8002E8EC(s32 mode) {
    func_80014C08(2);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801EBAFC, mode, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (mode) {
    case 2:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 1, 1, func_800148B0(), 0);
        break;
    case 4:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
        break;
    }
}

void func_8002EB1C(void) {
    func_800149B8(0, -1, 0, 0x600, D_801EBAFC, 0xFF, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    if (D_801F80C1 != 0) {
        func_8002FAC8();
        func_800149B8(0, -1, 0, 0x100, func_8002F4F4, 0, 0, 0, 0);
        return;
    }
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    if (*(u8 *)(PLAYER_PROFILES + 0xF) == 0) {
        func_8002B024(0, 0x6F, 0x7F);
        func_8002B858(0);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 0, func_800148B0(), 0);
    } else {
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
    }
}

void func_8002ECDC(s8 arg0) {
    func_80014C08(2);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010884, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1600, D_801E8E88, (s32 *) arg0, 0, 0, 0);
}

void func_8002ED9C(void) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
}

void func_8002EE50(s32 mode) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801E8C04, 0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (mode) {
    case 0:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 1, func_800148B0(), 0);
        break;
    case 1:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
        break;
    }
}

void func_8002F074(s32 mode) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, D_801E4B34, 0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (mode) {
    case 0:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 1, func_800148B0(), 0);
        break;
    case 1:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
        break;
    }
}

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

void func_8002F4F4(void) {
    s32 stack;
    s32 again;
    s32 r;

    stack = func_800148B0();
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    do {
        func_800149B8(0, -1, 0, 0x800, D_801EA2F8, stack, 0, 0, 0);
        r = func_80014C08(0x7FFFFFFF);
        again = 0;
        switch (r) {
        case 0:
            func_8002F920(6, 0x380, 0, 0x380, 0x80);
            func_800149B8(0, -1, 0, 0x800, D_801E6454, stack, 0, 0, 0);
            func_80014C08(0x7FFFFFFF);
            func_80014C08(2);
            func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
            func_80014C08(0x7FFFFFFF);
            func_80014C08(2);
            func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
            break;
        case 1:
            func_8002F920(6, 0x380, 0, 0x380, 0x80);
            func_800149B8(0, -1, 0, 0x800, func_8002EB1C, 0, 0, 0, 0);
            break;
        case 2:
            func_8002F920(7, 0x380, 0, 0x380, 0x80);
            *((u8 *)D_8006E054 + 0x1028) = 0;
            again = func_801EBD34();
            if (again == 0) {
                func_800149B8(0, -1, 0, 0x800, D_801EB2E8, stack, 0, 0, 0);
            } else {
                func_8002FAC8();
            }
            break;
        }
    } while (again);
}

void func_8002F79C(void) {
    D_801D8260 = 0;
}

void func_8002F7A8(void) {
    s16 r[4];
    s32 i;
    s8 *p;
    s8 *e;

    p = (s8 *)&D_801D81F8;
    if ((*(s32 *)(p + 0x68)) != 0) {
        return;
    }
    (*(s8 *)(p + 0x6C)) = -1;
    (*(s8 *)(p + 0x6D)) = -1;
    (*(s16 *)(p + 0x72)) = 0;
    (*(s16 *)(p + 0x70)) = 0;
    (*(s8 *)(p + 0x6E)) = 0;
    (*(s8 *)(p + 0x6F)) = 5;
    for (i = 0; i < 2; i++) {
        e = (s8 *)&D_801D81F8 + i * 0x34;
        func_8001E6EC(0xE, e, 0, 0);
        (*(s16 *)(e + 0x10)) = 0x141;
        (*(s16 *)(e + 0x12)) = 0xF0;
        r[0] = 0;
        r[1] = 0;
        r[2] = 0;
        r[3] = 0;
        SetTexWindow((s8 *)&D_801D8220 + i * 0x34, r);
    }
    func_800149B8(0, -1, 0, 0x800, func_8001B248, &D_800108A4, func_800148B0(), -2);
    D_801D8260 = func_80014C08(0x7FFFFFFF);
}

void func_8002F8E8(void) {
    s8 *p = (s8 *)&D_801D81F8;

    func_8001AE90(*(void **)(p + 0x68));
    *(void **)(p + 0x68) = 0;
    func_8002FAA8();
}

void func_8002F920(s32 mode, s32 x, s32 y, s32 w, s32 h) {
    s32 i;

    if (D_801D81F8.unk72 != 0 && D_801D81F8.unk72 != 0x80) {
        do {
            func_80014C08(D_800794F0);
        } while (D_801D81F8.unk72 != 0 && D_801D81F8.unk72 != 0x80);
    }
    if (D_801D81F8.unk72 == 0) {
        D_801D81F8.unk6D = -1;
    }
    D_801D81F8.mode = mode;
    if (mode >= 0) {
        D_801D81F8.x = x;
        D_801D81F8.y = y;
        D_801D81F8.w = w;
        D_801D81F8.h = h;
        for (i = 0; i < 2; i++) {
            (D_801D81F8.buf + i)->clut = getClut(w, h);
            SetDrawTPage((D_801D81F8.buf + i)->tpage, 0, 0, GetTPage(0, 0, x, y));
        }
    }
    D_801D81F8.unk6E = 0;
    D_801D81F8.unk6F = 0x1E;
}

void func_8002FAA8(void) {
    s8 *p;

    p = (s8 *)&D_801D81F8;
    p[0x6C] = -1;
    p[0x6D] = -1;
    (*(s16 *)(p + 0x72)) = 0;
    (*(s16 *)(p + 0x70)) = 0;
}

void func_8002FAC8(void) {
    D_801D8264 = -1;
}

void func_8002FAD8(s8 arg0) {
    D_801D8266 = arg0;
}
