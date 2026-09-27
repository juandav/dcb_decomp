#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/prim3d.h"

void transformAndAddPolyFT3(s32 poly, s32 vert0, s32 vert1, s32 vert2, u8 cullBackface, s32 fixedOtz) {
    s32 depthCue;
    s32 flag;
    s32 otz;

    if (cullBackface == 0) {
        otz = RotTransPers3(vert0, vert1, vert2, poly + 8, poly + 0x10, poly + 0x18, &depthCue, &flag);
        goto block_3;
    }
    if (RotAverageNclip3(vert0, vert1, vert2, poly + 8, poly + 0x10, poly + 0x18, &depthCue, &otz, &flag) > 0) {
block_3:
        if ((u32) (otz - 2) < 0xFFFU) {
            if (fixedOtz == 0) {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
                return;
            }
            AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], poly);
        }
    }
}

void transformAndAddPolyFT4(s32 poly, s32 vert0, s32 vert1, s32 vert2, s32 vert3, u8 cullBackface, s32 fixedOtz) {
    s32 depthCue;
    s32 flag;
    s32 otz;

    if (cullBackface == 0) {
        otz = RotTransPers4(vert0, vert1, vert2, vert3, poly + 8, poly + 0x10, poly + 0x18, poly + 0x20, &depthCue, &flag);
        goto block_3;
    }
    if (RotAverageNclip4(vert0, vert1, vert2, vert3, poly + 8, poly + 0x10, poly + 0x18, poly + 0x20, &depthCue, &otz, &flag) > 0) {
block_3:
        if ((u32) (otz - 2) < 0xFFFU) {
            if (fixedOtz == 0) {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
                return;
            }
            AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], poly);
        }
    }
}

void transformAndAddPolyGT4(s32 poly, s32 vert0, s32 vert1, s32 vert2, s32 vert3, u8 cullBackface, s32 fixedOtz) {
    s32 depthCue;
    s32 flag;
    s32 otz;

    if (cullBackface == 0) {
        otz = RotTransPers4(vert0, vert1, vert2, vert3, poly + 8, poly + 0x14, poly + 0x20, poly + 0x2C, &depthCue, &flag);
        goto block_3;
    }
    if (RotAverageNclip4(vert0, vert1, vert2, vert3, poly + 8, poly + 0x14, poly + 0x20, poly + 0x2C, &depthCue, &otz, &flag) > 0) {
block_3:
        if ((u32) (otz - 2) < 0xFFFU) {
            if (fixedOtz == 0) {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
                return;
            }
            AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], poly);
        }
    }
}

void transformAndAddPolyF3(s32 poly, s32 tpagePrim, s32 vert0, s32 vert1, s32 vert2, u8 semiTrans, u8 cullBackface, s32 fixedOtz) {
    s32 depthCue;
    s32 flag;
    s32 otz;

    if (cullBackface == 0) {
        otz = RotTransPers3(vert0, vert1, vert2, poly + 8, poly + 0xC, poly + 0x10, &depthCue, &flag);
        goto block_3;
    }
    if (RotAverageNclip3(vert0, vert1, vert2, poly + 8, poly + 0xC, poly + 0x10, &depthCue, &otz, &flag) > 0) {
block_3:
        if ((u32) (otz - 2) < 0xFFFU) {
            if (fixedOtz == 0) {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
                if ((semiTrans != 0) && (tpagePrim != 0)) {
                    AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], tpagePrim);
                }
            } else {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], poly);
                if ((semiTrans != 0) && (tpagePrim != 0)) {
                    AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], tpagePrim);
                }
            }
        }
    }
}

void transformAndAddPolyF4(s32 poly, s32 tpagePrim, s32 vert0, s32 vert1, s32 vert2, s32 vert3, u8 semiTrans, u8 cullBackface, s32 fixedOtz) {
    s32 depthCue;
    s32 flag;
    s32 otz;

    if (cullBackface == 0) {
        otz = RotTransPers4(vert0, vert1, vert2, vert3, poly + 8, poly + 0xC, poly + 0x10, poly + 0x14, &depthCue, &flag);
        goto block_3;
    }
    if (RotAverageNclip4(vert0, vert1, vert2, vert3, poly + 8, poly + 0xC, poly + 0x10, poly + 0x14, &depthCue, &otz, &flag) > 0) {
block_3:
        if ((u32) (otz - 2) < 0xFFFU) {
            if (fixedOtz == 0) {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
                if ((semiTrans != 0) && (tpagePrim != 0)) {
                    AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], tpagePrim);
                }
            } else {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], poly);
                if ((semiTrans != 0) && (tpagePrim != 0)) {
                    AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], tpagePrim);
                }
            }
        }
    }
}

void transformAndAddPolyGT3(s32 poly, s32 vert0, s32 vert1, s32 vert2, u8 cullBackface, s32 fixedOtz) {
    s32 depthCue;
    s32 flag;
    s32 otz;

    if (cullBackface == 0) {
        otz = RotTransPers3(vert0, vert1, vert2, poly + 8, poly + 0x14, poly + 0x20, &depthCue, &flag);
        goto block_3;
    }
    if (RotAverageNclip3(vert0, vert1, vert2, poly + 8, poly + 0x14, poly + 0x20, &depthCue, &otz, &flag) > 0) {
block_3:
        if ((u32) (otz - 2) < 0xFFFU) {
            if (fixedOtz == 0) {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
                return;
            }
            AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], poly);
        }
    }
}

void transformAndAddPolyG3(s32 poly, s32 tpagePrim, s32 vert0, s32 vert1, s32 vert2, u8 semiTrans, u8 cullBackface, s32 fixedOtz) {
    s32 depthCue;
    s32 flag;
    s32 otz;

    if (cullBackface == 0) {
        otz = RotTransPers3(vert0, vert1, vert2, poly + 8, poly + 0x10, poly + 0x18, &depthCue, &flag);
        goto block_3;
    }
    if (RotAverageNclip3(vert0, vert1, vert2, poly + 8, poly + 0x10, poly + 0x18, &depthCue, &otz, &flag) > 0) {
block_3:
        if ((u32) (otz - 2) < 0xFFFU) {
            if (fixedOtz == 0) {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
                if ((semiTrans != 0) && (tpagePrim != 0)) {
                    AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], tpagePrim);
                }
            } else {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], poly);
                if ((semiTrans != 0) && (tpagePrim != 0)) {
                    AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], tpagePrim);
                }
            }
        }
    }
}

void transformAndAddPolyG4(s32 poly, s32 tpagePrim, s32 vert0, s32 vert1, s32 vert2, s32 vert3, u8 semiTrans, u8 cullBackface, s32 fixedOtz) {
    s32 depthCue;
    s32 flag;
    s32 otz;

    if (cullBackface == 0) {
        otz = RotTransPers4(vert0, vert1, vert2, vert3, poly + 8, poly + 0x10, poly + 0x18, poly + 0x20, &depthCue, &flag);
        goto block_3;
    }
    if (RotAverageNclip4(vert0, vert1, vert2, vert3, poly + 8, poly + 0x10, poly + 0x18, poly + 0x20, &depthCue, &otz, &flag) > 0) {
block_3:
        if ((u32) (otz - 2) < 0xFFFU) {
            if (fixedOtz == 0) {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
                if ((semiTrans != 0) && (tpagePrim != 0)) {
                    AddPrim(&CURRENT_FRAME_BUFFER->ot[otz], tpagePrim);
                }
            } else {
                AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], poly);
                if ((semiTrans != 0) && (tpagePrim != 0)) {
                    AddPrim(&CURRENT_FRAME_BUFFER->ot[fixedOtz], tpagePrim);
                }
            }
        }
    }
}

void transformAndAddLineF2(s32 line, s32 tpagePrim, s32 vert0, s32 vert1, u8 semiTrans, s32 fixedOtz) {
    s32 depthCue;
    s32 flag;
    s32 otz;

    RotTransPers(vert0, line + 8, &depthCue, &flag);
    otz = RotTransPers(vert1, line + 0xC, &depthCue, &flag);
    if ((u32) (otz - 2) < 0xFFFU) {
        if (fixedOtz == 0) {
            AddPrim((s32 *) &CURRENT_FRAME_BUFFER->ot[otz], line);
            if ((semiTrans != 0) && (tpagePrim != 0)) {
                AddPrim((s32 *) &CURRENT_FRAME_BUFFER->ot[otz], tpagePrim);
            }
        } else {
            AddPrim((s32 *) &CURRENT_FRAME_BUFFER->ot[fixedOtz], line);
            if ((semiTrans != 0) && (tpagePrim != 0)) {
                AddPrim((s32 *) &CURRENT_FRAME_BUFFER->ot[fixedOtz], tpagePrim);
            }
        }
    }
}

void transformAndAddLineG2(s32 line, s32 tpagePrim, s32 vert0, s32 vert1, u8 semiTrans, s32 fixedOtz) {
    s32 depthCue;
    s32 flag;
    s32 otz;

    RotTransPers(vert0, line + 8, &depthCue, &flag);
    otz = RotTransPers(vert1, line + 0x10, &depthCue, &flag);
    if ((u32) (otz - 2) < 0xFFFU) {
        if (fixedOtz == 0) {
            AddPrim((s32 *) &CURRENT_FRAME_BUFFER->ot[otz], line);
            if ((semiTrans != 0) && (tpagePrim != 0)) {
                AddPrim((s32 *) &CURRENT_FRAME_BUFFER->ot[otz], tpagePrim);
            }
        } else {
            AddPrim((s32 *) &CURRENT_FRAME_BUFFER->ot[fixedOtz], line);
            if ((semiTrans != 0) && (tpagePrim != 0)) {
                AddPrim((s32 *) &CURRENT_FRAME_BUFFER->ot[fixedOtz], tpagePrim);
            }
        }
    }
}
