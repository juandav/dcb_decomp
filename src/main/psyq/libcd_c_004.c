#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", data_ready_callback);

extern long D_801DBD50;

extern CdlLOC D_801DBCF8;

extern long D_801DBCFC;

int StGetBackloc(CdlLOC *loc) {
    if (D_801DBD50 != 0) {
        return -1;
    }
    CdIntToPos(CdPosToInt(&D_801DBCF8) + 1, loc);
    return D_801DBCFC;
}

OBJECT_END(3);
