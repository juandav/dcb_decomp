#include "dcb/effect_object.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/scroll_bg.h"
#include "dcb/archive.h"
#include "dcb/decompress.h"
#include "dcb/sort.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/menu.h"
#include "dcb/dialog.h"
#include "dcb/prim3d.h"
#include "dcb/prim_util.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/transform.h"

void updateEffectLinearMotion(void *fx) {
    s32 accelTerm;
    s32 distance;

    accelTerm = (*(s16 *)((s8 *)fx + 0x122)) * (*(s32 *)((s8 *)fx + 0x104)) * (*(s32 *)((s8 *)fx + 0x104));
    distance = (*(s16 *)((s8 *)fx + 0x120)) * (*(s32 *)((s8 *)fx + 0x100)) + accelTerm;
    (*(s32 *)((s8 *)fx + 0x20)) = (*(s16 *)((s8 *)fx + 0xD4)) + ((distance * (*(s32 *)((s8 *)fx + 0x9C))) >> 12);
    (*(s32 *)((s8 *)fx + 0x24)) = (*(s16 *)((s8 *)fx + 0xD6)) + ((distance * (*(s32 *)((s8 *)fx + 0xA0))) >> 12);
    (*(s32 *)((s8 *)fx + 0x28)) = (*(s16 *)((s8 *)fx + 0xD8)) + ((distance * (*(s32 *)((s8 *)fx + 0xA4))) >> 12);
}

void updateEffectArcMotion(void *fx) {
    updateEffectLinearMotion(fx);
    *(s32 *)((s8 *)fx + 0x24) +=
        -*(s16 *)((s8 *)fx + 0x120) * *(s32 *)((s8 *)fx + 0x100) +
        *(s16 *)((s8 *)fx + 0x120) * *(s32 *)((s8 *)fx + 0x100) *
            *(s32 *)((s8 *)fx + 0x100) / 56;
}

void updateEffectWaveXMotion(void *fx) {
    s32 phase;

    updateEffectLinearMotion(fx);
    phase = (*(s16 *)((s8 *)fx + 0x128)) + ((*(s16 *)((s8 *)fx + 0x120)) * (*(s32 *)((s8 *)fx + 0x100)));
    (*(s32 *)((s8 *)fx + 0x20)) = (s32) (((s32) ((*(s16 *)((s8 *)fx + 0x12A)) * rsin(phase * (*(s16 *)((s8 *)fx + 0x126)))) >> 0xA) + (*(s32 *)((s8 *)fx + 0x20)));
}

void updateEffectWaveYMotion(void *fx) {
    s32 phase;

    updateEffectLinearMotion(fx);
    phase = (*(s16 *)((s8 *)fx + 0x128)) + ((*(s16 *)((s8 *)fx + 0x120)) * (*(s32 *)((s8 *)fx + 0x100)));
    (*(s32 *)((s8 *)fx + 0x24)) = (s32) (((s32) ((*(s16 *)((s8 *)fx + 0x12A)) * rsin(phase * (*(s16 *)((s8 *)fx + 0x126)))) >> 0xA) + (*(s32 *)((s8 *)fx + 0x24)));
}

void updateEffectTiltedArcMotion(u8 *fx) {
    s32 height;
    s32 offsetX;
    s32 offsetY;

    updateEffectLinearMotion(fx);
    height = -*(s16 *)(fx + 0x120) * *(s32 *)(fx + 0x100) +
        *(s16 *)(fx + 0x120) * *(s32 *)(fx + 0x100) * *(s32 *)(fx + 0x100) / 56;
    offsetX = height * rsin(*(s16 *)(fx + 0xE8)) >> 12;
    offsetY = height * rcos(*(s16 *)(fx + 0xE8)) >> 12;
    *(s32 *)(fx + 0x20) -= offsetX;
    *(s32 *)(fx + 0x24) += offsetY;
}

void func_80030440(u8 *fx) {
    s32 distance;
    s32 height;
    s32 offsetX;
    s32 offsetY;

    distance = *(s16 *)(fx + 0x120) * *(s32 *)(fx + 0x100);
    *(s32 *)(fx + 0x20) = *(s16 *)(fx + 0xD4) + (distance * *(s32 *)(fx + 0x9C) >> 12);
    *(s32 *)(fx + 0x24) = *(s16 *)(fx + 0xD6) + (distance * *(s32 *)(fx + 0xA0) >> 12);
    *(s32 *)(fx + 0x28) = *(s16 *)(fx + 0xD8) + (distance * *(s32 *)(fx + 0xA4) >> 12);
    height = -*(s16 *)(fx + 0x120) * *(s32 *)(fx + 0x100) +
        *(s16 *)(fx + 0x122) * *(s32 *)(fx + 0x100) * *(s32 *)(fx + 0x100) / 56;
    offsetX = height * rsin(*(s16 *)(fx + 0xE8)) >> 12;
    offsetY = height * rcos(*(s16 *)(fx + 0xE8)) >> 12;
    *(s32 *)(fx + 0x20) -= offsetX;
    *(s32 *)(fx + 0x24) += offsetY;
}

void updateEffectShakeMotion(u8 *fx) {
    s32 x;
    s32 y;
    s32 z;

    x = *(s16 *)(fx + 0x120) / 2 - rand() % *(s16 *)(fx + 0x120);
    y = *(s16 *)(fx + 0x120) / 2 - rand() % *(s16 *)(fx + 0x120);
    z = *(s16 *)(fx + 0x120) / 2 - rand() % *(s16 *)(fx + 0x120);
    *(s32 *)(fx + 0x20) = *(s16 *)(fx + 0xD4) + x;
    *(s32 *)(fx + 0x24) = *(s16 *)(fx + 0xD6) + y;
    *(s32 *)(fx + 0x28) = *(s16 *)(fx + 0xD8) + z;
}

s32 getVectorDistance(SVECTOR *from, SVECTOR *to) {
    VECTOR delta;

    delta.vx = to->vx - from->vx;
    delta.vy = to->vy - from->vy;
    delta.vz = to->vz - from->vz;
    return SquareRoot0(delta.vx * delta.vx + delta.vy * delta.vy + delta.vz * delta.vz);
}

s32 isPointAlongSegment(SVECTOR *start, SVECTOR *end, SVECTOR *point) {
    VECTOR dir;
    VECTOR toEnd;
    VECTOR toStart;
    s32 dotEnd;
    s32 dotStart;

    dir.vx = end->vx - start->vx;
    dir.vy = end->vy - start->vy;
    dir.vz = end->vz - start->vz;
    toEnd.vx = end->vx - point->vx;
    toEnd.vy = end->vy - point->vy;
    toEnd.vz = end->vz - point->vz;
    toStart.vx = start->vx - point->vx;
    toStart.vy = start->vy - point->vy;
    toStart.vz = start->vz - point->vz;
    dotEnd = (toEnd.vx * dir.vx + toEnd.vy * dir.vy + toEnd.vz * dir.vz) >> 12;
    dotStart = (toStart.vx * dir.vx + toStart.vy * dir.vy + toStart.vz * dir.vz) >> 12;
    if ((dotEnd <= 0 && dotStart >= 0) || (dotEnd >= 0 && dotStart <= 0)) {
        return 1;
    }
    return 0;
}

void projectPointOntoLine(SVECTOR *start, SVECTOR *point, SVECTOR *end, VECTOR *out) {
    VECTOR dir;
    VECTOR toPoint;
    VECTOR unitDir;
    VECTOR proj;
    s32 along;

    dir.vx = end->vx - start->vx;
    dir.vy = end->vy - start->vy;
    dir.vz = end->vz - start->vz;
    if (SquareRoot0(dir.vx * dir.vx + dir.vy * dir.vy + dir.vz * dir.vz) < 20000) {
        VectorNormal(&dir, &unitDir);
    } else {
        dir.vx >>= 4;
        dir.vy >>= 4;
        dir.vz >>= 4;
        VectorNormal(&dir, &unitDir);
    }
    toPoint.vx = point->vx - start->vx;
    toPoint.vy = point->vy - start->vy;
    toPoint.vz = point->vz - start->vz;
    along = (unitDir.vx * toPoint.vx + unitDir.vy * toPoint.vy + unitDir.vz * toPoint.vz) >> 12;
    proj.vx = along * unitDir.vx >> 12;
    proj.vy = along * unitDir.vy >> 12;
    proj.vz = along * unitDir.vz >> 12;
    out->vx = start->vx + proj.vx;
    out->vy = start->vy + proj.vy;
    out->vz = start->vz + proj.vz;
}

s32 isWithinDistance(SVECTOR *a, SVECTOR *b, s32 radius) {
    s32 dist;

    dist = getVectorDistance(a, b);
    if (-radius < dist && dist < radius) {
        return 1;
    }
    return 0;
}

s32 checkEffectHitTarget(SVECTOR *prevPos, SVECTOR *curPos, SVECTOR *target, s16 radius) {
    SVECTOR closest;
    VECTOR closestPoint;

    if (isPointAlongSegment(prevPos, curPos, target) != 0) {
        projectPointOntoLine(prevPos, target, curPos, &closestPoint);
        closest.vx = closestPoint.vx;
        closest.vy = closestPoint.vy;
        closest.vz = closestPoint.vz;
        if (isWithinDistance(target, &closest, radius) != 0) {
            return 1;
        }
        return -1;
    }
    return 0;
}

EffectTemplate *cloneEffectObject(EffectTemplate *template) {
    EffectTemplate *fx;

    fx = allocTaskHeapBlock(0x13C);
    *fx = *template;
    initEffectObject(fx);
    return fx;
}

void updateEffectObject(s32 fx) {
    PushMatrix();
    tickEffectMotion(fx, 0);
    PopMatrix();
}

void freeEffectObject(void *fx) {
    freeHeapBlock(fx);
}

s32 getDirectionVector(SVECTOR *from, SVECTOR *to, VECTOR *dir) {
    VECTOR delta;

    delta.vx = to->vx - from->vx;
    delta.vy = to->vy - from->vy;
    delta.vz = to->vz - from->vz;
    if (SquareRoot0(delta.vx * delta.vx + delta.vy * delta.vy + delta.vz * delta.vz) < 20000) {
        VectorNormal(&delta, dir);
        return 1;
    }
    delta.vx >>= 4;
    delta.vy >>= 4;
    delta.vz >>= 4;
    VectorNormal(&delta, dir);
    return -1;
}

void restartEffectMotion(u8 *fx) {
    s32 i;

    fx[0x139] = *(s16 *)(fx + 0x12E) >= 0x5B;
    *(s32 *)(fx + 0x118) = -1;
    for (i = 0; i < 3; i++) {
        ((Unk80030CA8 *)fx)->done[i] = 0;
    }
    *(s32 *)(fx + 0x108) = 0;
    *(s32 *)(fx + 0x11C) = 0;
    *(s32 *)(fx + 0x100) = 0;
    *(s32 *)(fx + 0x104) = 0;
    if (*(s16 *)(fx + 0x12E) != 0 && *(s16 *)(fx + 0x12E) != 0x5A) {
        *(s32 *)(fx + 0x38) = *(s32 *)(fx + 0xAC);
        *(s32 *)(fx + 0x3C) = *(s32 *)(fx + 0xB0);
        *(s32 *)(fx + 0x40) = *(s32 *)(fx + 0xB4);
        *(s16 *)(fx + 0x30) = *(s16 *)(fx + 0xE4);
        *(s16 *)(fx + 0x32) = *(s16 *)(fx + 0xE6);
        *(s16 *)(fx + 0x34) = *(s16 *)(fx + 0xE8);
        *(s32 *)(fx + 0x20) = *(s16 *)(fx + 0xD4);
        *(s32 *)(fx + 0x24) = *(s16 *)(fx + 0xD6);
        *(s32 *)(fx + 0x28) = *(s16 *)(fx + 0xD8);
        *(s32 *)(fx + 0x6C) = *(s16 *)(fx + 0xDC);
        *(s32 *)(fx + 0x70) = *(s16 *)(fx + 0xDE);
        *(s32 *)(fx + 0x74) = *(s16 *)(fx + 0xE0);
        getDirectionVector((SVECTOR *)(fx + 0xD4), (SVECTOR *)(fx + 0xDC), (VECTOR *)(fx + 0x9C));
    } else {
        *(s32 *)(fx + 0xAC) = *(s32 *)(fx + 0x38);
        *(s32 *)(fx + 0xB0) = *(s32 *)(fx + 0x3C);
        *(s32 *)(fx + 0xB4) = *(s32 *)(fx + 0x40);
        *(s16 *)(fx + 0xE4) = *(s16 *)(fx + 0x30);
        *(s16 *)(fx + 0xE6) = *(s16 *)(fx + 0x32);
        *(s16 *)(fx + 0xE8) = *(s16 *)(fx + 0x34);
        *(s16 *)(fx + 0xD4) = *(s32 *)(fx + 0x20);
        *(s16 *)(fx + 0xD6) = *(s32 *)(fx + 0x24);
        *(s16 *)(fx + 0xD8) = *(s32 *)(fx + 0x28);
    }
}

void *initEffectObject(void *fx) {
    initTransform(fx, (*(s32 *)((s8 *)fx + 0x98)), (s32) (*(s16 *)((s8 *)fx + 0xD4)), (s32) (*(s16 *)((s8 *)fx + 0xD6)), (s32) (*(s16 *)((s8 *)fx + 0xD8)), (s16) (s32) (*(s16 *)((s8 *)fx + 0xE4)), (s16) (s32) (*(s16 *)((s8 *)fx + 0xE6)), (s16) (s32) (*(s16 *)((s8 *)fx + 0xE8)));
    initTransform(fx + 0x4C, (*(s32 *)((s8 *)fx + 0x98)), (s32) (*(s16 *)((s8 *)fx + 0xDC)), (s32) (*(s16 *)((s8 *)fx + 0xDE)), (s32) (*(s16 *)((s8 *)fx + 0xE0)), 0, 0, 0);
    (*(s32 *)((s8 *)fx + 0x14)) = 0;
    (*(s32 *)((s8 *)fx + 0x18)) = 0;
    (*(s32 *)((s8 *)fx + 0x1C)) = 0;
    (*(s32 *)((s8 *)fx + 0x38)) = (s32) (*(s32 *)((s8 *)fx + 0xAC));
    (*(s32 *)((s8 *)fx + 0x3C)) = (s32) (*(s32 *)((s8 *)fx + 0xB0));
    (*(s32 *)((s8 *)fx + 0x40)) = (s32) (*(s32 *)((s8 *)fx + 0xB4));
    (*(u16 *)((s8 *)fx + 0x30)) = (u16) (*(s16 *)((s8 *)fx + 0xE4));
    (*(u16 *)((s8 *)fx + 0x32)) = (u16) (*(s16 *)((s8 *)fx + 0xE6));
    (*(u16 *)((s8 *)fx + 0x34)) = (u16) (*(s16 *)((s8 *)fx + 0xE8));
    (*(s32 *)((s8 *)fx + 0x20)) = (s32) (*(s16 *)((s8 *)fx + 0xD4));
    (*(s32 *)((s8 *)fx + 0x24)) = (s32) (*(s16 *)((s8 *)fx + 0xD6));
    (*(s32 *)((s8 *)fx + 0x28)) = (s32) (*(s16 *)((s8 *)fx + 0xD8));
    restartEffectMotion(fx);
    (*(s32 *)((s8 *)fx + 0x6C)) = (s32) (*(s16 *)((s8 *)fx + 0xDC));
    (*(s32 *)((s8 *)fx + 0x70)) = (s32) (*(s16 *)((s8 *)fx + 0xDE));
    (*(s32 *)((s8 *)fx + 0x74)) = (s32) (*(s16 *)((s8 *)fx + 0xE0));
    getDirectionVector(fx + 0xD4, fx + 0xDC, fx + 0x9C);
    (*(s32 *)((s8 *)fx + 0xFC)) = 0;
    (*(s16 *)((s8 *)fx + 0x132)) = 0;
    (*(s8 *)((s8 *)fx + 0x138)) = 0;
    return fx;
}

INCLUDE_RODATA("asm/main/nonmatchings/model/effect_object", PATH_OPENSEG);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect_object", PATH_SAISEG);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect_object", PATH_EVOSEG);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect_object", PATH_SUBSEG);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect_object", PATH_BG_ARC);

s32 tickEffectMotion(s32 fxAddr, s32 applyFlag) {
    EffectObject *fx = (EffectObject *)fxAddr;
    u8 applyMode = applyFlag;
    SVECTOR prevPos;
    SVECTOR curPos;
    SVECTOR targetPos;
    s32 hit;

    if (fx->mode != 10 && fx->mode < 90) {
        if (fx->sx != fx->sxT) {
            fx->sx = fx->dsx * fx->t + fx->sx0;
            if (fx->dsx >= 0) {
                if (fx->sx >= fx->sxT) {
                    fx->sx = fx->sxT;
                    fx->doneX = 1;
                }
            } else if (fx->sx <= fx->sxT) {
                fx->sx = fx->sxT;
                fx->doneX = 1;
            }
        } else {
            fx->doneX = 1;
        }
        if (fx->sy != fx->syT) {
            fx->sy = fx->dsy * fx->t + fx->sy0;
            if (fx->dsy >= 0) {
                if (fx->sy >= fx->syT) {
                    fx->sy = fx->syT;
                    fx->doneY = 1;
                }
            } else if (fx->sy <= fx->syT) {
                fx->sy = fx->syT;
                fx->doneY = 1;
            }
        } else {
            fx->doneY = 1;
        }
        if (fx->sz != fx->szT) {
            fx->sz = fx->dsz * fx->t + fx->sz0;
            if (fx->dsz >= 0) {
                if (fx->sz >= fx->szT) {
                    fx->sz = fx->szT;
                    fx->doneZ = 1;
                }
            } else if (fx->sz <= fx->szT) {
                fx->sz = fx->szT;
                fx->doneZ = 1;
            }
        } else {
            fx->doneZ = 1;
        }
        fx->rotX = fx->rx0 + fx->drx * fx->t + fx->ddrx * fx->t2 * fx->t2 / 64;
        fx->rotY = fx->ry0 + fx->dry * fx->t + fx->ddry * fx->t2 * fx->t2 / 64;
        fx->rotZ = fx->rz0 + fx->drz * fx->t + fx->ddrz * fx->t2 * fx->t2 / 64;
    }
    getTransformWorldPos(fx, &prevPos);
    switch (fx->mode) {
    case 6:
        fx->state = -1;
    case 0:
    case 47:
    case 48:
    case 49:
    case 56:
    case 69:
    case 82:
    case 90:
        fx->posX = fx->px;
        fx->posY = fx->py;
        fx->posZ = fx->pz;
        break;
    case 1:
    case 11:
    case 12:
    case 13:
    case 29:
    case 30:
    case 31:
    case 50:
    case 57:
    case 63:
    case 70:
    case 76:
    case 83:
        updateEffectLinearMotion(fx);
        break;
    case 2:
    case 14:
    case 15:
    case 16:
    case 32:
    case 33:
    case 34:
    case 51:
    case 58:
    case 64:
    case 71:
    case 77:
    case 84:
        updateEffectArcMotion(fx);
        break;
    case 3:
    case 17:
    case 18:
    case 19:
    case 35:
    case 36:
    case 37:
    case 52:
    case 59:
    case 65:
    case 72:
    case 78:
    case 85:
        updateEffectWaveXMotion(fx);
        break;
    case 4:
    case 20:
    case 21:
    case 22:
    case 38:
    case 39:
    case 40:
    case 53:
    case 60:
    case 66:
    case 73:
    case 79:
    case 86:
        updateEffectWaveYMotion(fx);
        break;
    case 5:
    case 23:
    case 24:
    case 25:
    case 41:
    case 42:
    case 43:
    case 54:
    case 61:
    case 67:
    case 74:
    case 80:
    case 87:
        updateEffectTiltedArcMotion((u8 *)fx);
        break;
    case 7:
    case 26:
    case 27:
    case 28:
    case 44:
    case 45:
    case 46:
    case 55:
    case 62:
    case 68:
    case 75:
    case 81:
    case 88:
        func_80030440((u8 *)fx);
        break;
    case 8:
    case 89:
        updateEffectShakeMotion((u8 *)fx);
        break;
    }
    if (fx->state != -1 && fx->mode != 0 && fx->mode < 90) {
        updateTransformMatrix(fx, applyMode);
        updateTransformMatrix((u8 *)fx + 0x4C, 0);
        getTransformWorldPos(fx, &curPos);
        getTransformWorldPos((u8 *)fx + 0x4C, &targetPos);
        hit = checkEffectHitTarget(&prevPos, &curPos, &targetPos, fx->hitRadius);
        if (hit == 1) {
            switch (fx->mode) {
            case 11:
            case 14:
            case 17:
            case 20:
            case 23:
            case 26:
                fx->mode = 0;
                restartEffectMotion((u8 *)fx);
                break;
            case 12:
            case 15:
            case 18:
            case 21:
            case 24:
            case 27:
                fx->mode = 0;
                fx->posX = fx->px2;
                fx->posY = fx->py2;
                fx->posZ = fx->pz2;
                fx->px = fx->px2;
                fx->py = fx->py2;
                fx->pz = fx->pz2;
                restartEffectMotion((u8 *)fx);
                fx->sxT = fx->sx;
                fx->syT = fx->sy;
                fx->szT = fx->sz;
                fx->swT = fx->sw;
                fx->drx = 0;
                fx->dry = 0;
                fx->drz = 0;
                fx->ddrx = 0;
                fx->ddry = 0;
                fx->ddrz = 0;
                break;
            case 13:
            case 16:
            case 19:
            case 22:
            case 25:
            case 28:
                restartEffectMotion((u8 *)fx);
                break;
            case 50:
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
                fx->suspended = 1;
                break;
            case 76:
            case 77:
            case 78:
            case 79:
            case 80:
            case 81:
                fx->mode = 0;
                fx->posX = fx->px2;
                fx->posY = fx->py2;
                fx->posZ = fx->pz2;
                fx->px = fx->px2;
                fx->py = fx->py2;
                fx->pz = fx->pz2;
            case 63:
            case 64:
            case 65:
            case 66:
            case 67:
            case 68:
                fx->fadeMode = 2;
                fx->speed = -abs(fx->speed);
                break;
            }
            fx->state = 1;
        } else if (hit == -1) {
            fx->state = 2;
        }
    }
    if (fx->mode < 90 && fx->mode != 0 && fx->state == -1) {
        fx->state = 0;
    }
    if (fx->flag != 0) {
        switch (fx->mode) {
    case 31:
    case 34:
    case 37:
    case 40:
    case 43:
    case 46:
    case 49:
            restartEffectMotion((u8 *)fx);
            break;
        }
    }
    if (fx->mode < 90) {
        fx->t++;
        fx->t2++;
        if (fx->period != 0 && fx->flag == 0 && fx->period < fx->cnt++) {
            fx->cnt = fx->period;
            switch (fx->mode) {
            case 29:
            case 32:
            case 35:
            case 38:
            case 41:
            case 44:
            case 47:
                fx->mode = 0;
                restartEffectMotion((u8 *)fx);
                break;
            case 30:
            case 33:
            case 36:
            case 39:
            case 42:
            case 45:
            case 48:
                fx->mode = 0;
                restartEffectMotion((u8 *)fx);
                fx->sxT = fx->sx;
                fx->syT = fx->sy;
                fx->szT = fx->sz;
                fx->swT = fx->sw;
                fx->drx = 0;
                fx->dry = 0;
                fx->drz = 0;
                fx->ddrx = 0;
                fx->ddry = 0;
                fx->ddrz = 0;
                break;
            case 31:
            case 34:
            case 37:
            case 40:
            case 43:
            case 46:
            case 49:
                restartEffectMotion((u8 *)fx);
                break;
            case 56:
            case 57:
            case 58:
            case 59:
            case 60:
            case 61:
            case 62:
                fx->suspended = 1;
                break;
            case 82:
            case 83:
            case 84:
            case 85:
            case 86:
            case 87:
            case 88:
            case 89:
                fx->mode = 0;
                restartEffectMotion((u8 *)fx);
                fx->sxT = fx->sx;
                fx->syT = fx->sy;
                fx->szT = fx->sz;
                fx->swT = fx->sw;
                fx->drx = 0;
                fx->dry = 0;
                fx->drz = 0;
                fx->ddrx = 0;
                fx->ddry = 0;
                fx->ddrz = 0;
            case 69:
            case 70:
            case 71:
            case 72:
            case 73:
            case 74:
            case 75:
                fx->fadeMode = 2;
                fx->speed = -abs(fx->speed);
                break;
            }
            fx->flag = 1;
        }
    }
    updateTransformMatrix(fx, applyMode);
}

void tickEffectStartDelay(void *fx) {
    if ((*(s16 *)((s8 *)fx + 0x12E)) >= 0x5B) {
        if ((*(s16 *)((s8 *)fx + 0x128)) > (*(s32 *)((s8 *)fx + 0x100))) {
            (*(s32 *)((s8 *)fx + 0x100)) += 1;
            return;
        }
        (*(s32 *)((s8 *)fx + 0x100)) = 0;
        (*(s8 *)((s8 *)fx + 0x139)) = 0;
        (*(s16 *)((s8 *)fx + 0x12E)) = (s16) ((u16) (*(s16 *)((s8 *)fx + 0x12E)) - 0x64);
    }
}

s16 updateEffectBrightness(void *fxObj, s16 brightness) {
    u8 *fx;

    fx = fxObj;
    switch (fx[0x137]) {
    case 1:
        brightness = (*(s32 *)(fx + 0x38)) / 16;
        if ((*(s32 *)(fx + 0x38)) > 0x1000) {
            brightness = 0x100 - ((*(s32 *)(fx + 0x38)) - 0x1000) / 16;
        }
        break;
    case 2:
        brightness += (*(u16 *)(fx + 0x130));
        break;
    case 3:
        if (fx[0x138] == 2) {
            break;
        }
        if (fx[0x138] == 0) {
            brightness += (*(u16 *)(fx + 0x130));
            if (brightness > 0x100) {
                brightness = 0x100;
                fx[0x138] = 1;
            }
        } else {
            brightness -= (*(u16 *)(fx + 0x130));
            if (brightness < 0) {
                brightness = 0;
                fx[0x138] = 2;
            }
        }
        break;
    case 4:
        if (fx[0x138] == 0) {
            brightness += (*(u16 *)(fx + 0x130));
            if (brightness > 0x100) {
                brightness = 0x100;
                fx[0x138] = 1;
            }
        } else {
            brightness -= (*(u16 *)(fx + 0x130));
            if (brightness < 0) {
                brightness = 0;
                fx[0x138] = 0;
            }
        }
        break;
    case 5:
        fx[0x138] += (*(u16 *)(fx + 0x130));
        if ((s8)fx[0x138] >= 0) {
            brightness = fx[0x138] + 0x80;
        } else {
            brightness = 0xFF - (fx[0x138] & 0x7F);
        }
        break;
    }
    if (brightness < 0) {
        brightness = 0;
    }
    if (brightness > 0x100) {
        brightness = 0x100;
    }
    if ((*(s16 *)(fx + 0x12E)) == 0xA) {
        brightness = (*(s16 *)(fx + 0x132));
    } else {
        (*(s16 *)(fx + 0x132)) = brightness;
    }
    return brightness;
}
