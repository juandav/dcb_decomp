#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/prim3d.h"

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
