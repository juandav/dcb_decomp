#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/effect.h"
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
#include "dcb/prim3d.h"
#include "dcb/prim_util.h"
#include "dcb/sound.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/transform.h"

s16 ATTACK_ICON_ORIGIN_X[3] = { 0x80, -0x40, 0x140 };
s16 ATTACK_ICON_ORIGIN_Y[2][3] = { { -0xF0, 0x99, 0x99 }, { 0xF0, 0x1C, 0x1C } };

void renderScrollingBackground(void) {
    Fade *bg;
    s32 tim;
    s16 texWindow[4];
    u8 buffer;

    if (SCROLL_BACKGROUND.tim == 0 || *(u16 *)&SCROLL_BACKGROUND.mode == 0xFFFF) {
        return;
    }
    switch (SCROLL_BACKGROUND.unk6E) {
    case 0:
        if ((s8)SCROLL_BACKGROUND.unk6F < 30) {
            SCROLL_BACKGROUND.unk6F++;
        }
        break;
    case 1:
        if ((s8)SCROLL_BACKGROUND.unk6F >= -59) {
            SCROLL_BACKGROUND.unk6F--;
        }
        break;
    }
    bg = &SCROLL_BACKGROUND;
    bg->unk70 = (bg->unk70 + (s8)bg->unk6F) % 7680;
    if (bg->mode != bg->unk6D) {
        if (bg->unk6D == -1) {
            if (bg->unk72 == 0) {
                tim = decompressArchiveEntry(bg->tim, bg->mode);
                uploadTim((u32 *)tim, bg->x, bg->y, bg->w, bg->h);
                if (bg->mode != 6) {
                    bg->unk7C = 0x40;
                } else {
                    SCROLL_BACKGROUND.unk7C = 0x80;
                }
                SCROLL_BACKGROUND.unk7E = 0x80;
                DrawSync(0);
                freeHeapBlock((void *)tim);
            }
            SCROLL_BACKGROUND.unk72 += 6;
            if (SCROLL_BACKGROUND.unk72 > 0x80) {
                SCROLL_BACKGROUND.unk72 = 0x80;
                SCROLL_BACKGROUND.unk6D = SCROLL_BACKGROUND.mode;
            }
        } else {
            SCROLL_BACKGROUND.unk72 -= 6;
            if (SCROLL_BACKGROUND.unk72 < 0) {
                SCROLL_BACKGROUND.unk72 = 0;
                SCROLL_BACKGROUND.unk6D = -1;
            }
        }
    }
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].twin0);
    buffer = FRAME_BUFFER_INDEX;
    SCROLL_BACKGROUND.buf[buffer].x0 = -((SCROLL_BACKGROUND.unk70 / 60) & 1);
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].y0 = 0;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].u0 = (SCROLL_BACKGROUND.unk70 / 60) & 0xFE;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].v0 = SCROLL_BACKGROUND.unk70 / 60;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].r0 = SCROLL_BACKGROUND.unk72;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].g0 = SCROLL_BACKGROUND.unk72;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].b0 = SCROLL_BACKGROUND.unk72;
    texWindow[0] = (SCROLL_BACKGROUND.x % 64) * 4;
    texWindow[1] = SCROLL_BACKGROUND.y % 256;
    texWindow[2] = SCROLL_BACKGROUND.unk7C;
    texWindow[3] = SCROLL_BACKGROUND.unk7E;
    SetTexWindow(SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].twin, texWindow);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], &SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].twin);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].tpage);
}

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

Unk13C *cloneEffectObject(Unk13C *template) {
    Unk13C *fx;

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
        ((Unk80030CA8 *)fx)->unk10C[i] = 0;
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

INCLUDE_RODATA("asm/main/nonmatchings/model/effect", PATH_OPENSEG);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect", PATH_SAISEG);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect", PATH_EVOSEG);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect", PATH_SUBSEG);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect", PATH_BG_ARC);

s32 tickEffectMotion(s32 fxAddr, s32 applyFlag) {
    Anim *fx = (Anim *)fxAddr;
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
        hit = checkEffectHitTarget(&prevPos, &curPos, &targetPos, fx->unk12C);
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
                fx->unk139 = 1;
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
                fx->unk137 = 2;
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
                fx->unk139 = 1;
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
                fx->unk137 = 2;
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

void buildRingEffectMesh(Obj32 *ring) {
    SVECTOR *vertex;
    s32 i;
    s16 x;
    s16 y;

    vertex = ring->unk16C;
    for (i = 0; i < ring->n; i++) {
        x = rsin((i << 12) / ring->n) * ring->unk19C[0] / 4096;
        y = rcos((i << 12) / ring->n) * ring->unk19C[0] / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[3];
        vertex++;
        x = rsin(((i + 1) << 12) / ring->n) * ring->unk19C[0] / 4096;
        y = rcos(((i + 1) << 12) / ring->n) * ring->unk19C[0] / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[3];
        vertex++;
        x = rsin((i << 12) / ring->n) * (ring->unk19C[0] + (ring->unk19C[1] - ring->unk19C[0]) * ring->unk19C[2] / 100) / 4096;
        y = rcos((i << 12) / ring->n) * (ring->unk19C[0] + (ring->unk19C[1] - ring->unk19C[0]) * ring->unk19C[2] / 100) / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[3] + (ring->unk19C[4] - ring->unk19C[3]) * ring->unk19C[2] / 100;
        vertex++;
        x = rsin(((i + 1) << 12) / ring->n) * (ring->unk19C[0] + (ring->unk19C[1] - ring->unk19C[0]) * ring->unk19C[2] / 100) / 4096;
        y = rcos(((i + 1) << 12) / ring->n) * (ring->unk19C[0] + (ring->unk19C[1] - ring->unk19C[0]) * ring->unk19C[2] / 100) / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[3] + (ring->unk19C[4] - ring->unk19C[3]) * ring->unk19C[2] / 100;
        vertex++;
        x = rsin((i << 12) / ring->n) * ring->unk19C[1] / 4096;
        y = rcos((i << 12) / ring->n) * ring->unk19C[1] / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[4];
        vertex++;
        x = rsin(((i + 1) << 12) / ring->n) * ring->unk19C[1] / 4096;
        y = rcos(((i + 1) << 12) / ring->n) * ring->unk19C[1] / 4096;
        vertex->vx = x;
        vertex->vy = y;
        vertex->vz = ring->unk19C[4];
        vertex++;
    }
}

Obj32 *createRingEffect(s16 brightness, Bytes4 *innerColor, Bytes4 *midColor, Bytes4 *outerColor, Unk13C *template, s32 segments, u8 abr, u8 texDepth, s32 primType,
                     s16 innerRadius, s16 outerRadius, s16 midPercent, s16 innerZ, s16 outerZ, Bytes8 *texCoords, s32 tpage, s32 clut, s32 texAnimId, u8 u1, u8 u2,
                     s32 w, s32 x) {
    Obj32 *ring;
    u8 *prim;
    s32 i;
    s32 j;

    ring = allocTaskHeapBlock(0x1B0);
    ring->type = primType;
    ring->n = segments;
    ring->unk19C[0] = innerRadius;
    ring->unk19C[1] = outerRadius;
    ring->unk19C[2] = midPercent;
    ring->unk19C[3] = innerZ;
    ring->unk19C[4] = outerZ;
    ring->unk16C = allocTaskHeapBlock(segments * 48);
    buildRingEffectMesh(ring);
    if (primType == 13) {
        ring->unk170 = *texCoords;
        ring->unk178 = tpage;
        ring->unk17C = clut;
        if (texAnimId >= 0 && func_801E6C78(texAnimId, 1, ring, ring->unk13C, x) != 0) {
            ring->unk1AD = 1;
        } else {
            ring->unk1AD = -1;
        }
    } else {
        ring->unk1AD = -1;
    }
    ring->unk198 = texDepth;
    ring->unk1A6 = brightness;
    ring->unk1A8 = -1;
    ring->unk185 = *innerColor;
    ring->unk189 = *midColor;
    ring->unk18D = *outerColor;
    *(Unk13C *)ring = *template;
    initEffectObject(ring);
    ring->unk1AB = u2;
    ring->unk194 = w;
    ring->unk1AA = u1;
    ring->unk1AC = abr;
    for (i = 0; i < 2; i++) {
        if (ring->type < 10) {
            ring->unk15C[i] = allocTaskHeapBlock(ring->n * 16);
        } else {
            ring->unk15C[i] = 0;
        }
        prim = ring->unk164[i] = allocTaskHeapBlock(PRIM_SIZES[ring->type] * ring->n * 2);
        for (j = 0; j < ring->n * 2; j++) {
            initPrimByType(ring->type, prim, abr, 0);
            if (ring->type < 10) {
                SetDrawTPage(ring->unk15C[i] + j * 8, 0, 0, GetTPage(0, texDepth, 0, 0));
            }
            prim += PRIM_SIZES[ring->type];
        }
    }
    return ring;
}

void renderRingEffect(Obj32 *ring) {
    u8 innerRgb[8];
    u8 midRgb[8];
    u8 outerRgb[8];
    SVECTOR *vertex;
    s32 i;

    if (ring->unk0[0x139] != 0) {
        tickEffectStartDelay(ring);
        return;
    }
    PushMatrix();
    tickEffectMotion((s32)ring, ring->unk1AA);
    ring->unk1A6 = updateEffectBrightness(ring, ring->unk1A6);
    if (ring->unk1A6 == 0) {
        PopMatrix();
        return;
    }
    vertex = ring->unk16C;
    if (ring->unk1A6 != ring->unk1A8) {
        innerRgb[0] = ring->unk185.b[0] * ring->unk1A6 / 256;
        innerRgb[1] = ring->unk185.b[1] * ring->unk1A6 / 256;
        innerRgb[2] = ring->unk185.b[2] * ring->unk1A6 / 256;
        midRgb[0] = ring->unk189.b[0] * ring->unk1A6 / 256;
        midRgb[1] = ring->unk189.b[1] * ring->unk1A6 / 256;
        midRgb[2] = ring->unk189.b[2] * ring->unk1A6 / 256;
        outerRgb[0] = ring->unk18D.b[0] * ring->unk1A6 / 256;
        outerRgb[1] = ring->unk18D.b[1] * ring->unk1A6 / 256;
        outerRgb[2] = ring->unk18D.b[2] * ring->unk1A6 / 256;
    }
    switch (ring->type) {
    case 9: {
        u8 *prim;
        u8 *otherPrim;
        u8 *tpagePrim;

        prim = ring->unk164[FRAME_BUFFER_INDEX];
        otherPrim = ring->unk164[FRAME_BUFFER_INDEX ^ 1];
        tpagePrim = ring->unk15C[FRAME_BUFFER_INDEX];
        for (i = 0; i < ring->n; i++) {
            if (ring->unk1A6 != ring->unk1A8) {
                setPrimRgb0(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb0(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
            }
            transformAndAddPolyG4((s32)prim, (s32)tpagePrim, (s32)&vertex[0], (s32)&vertex[1], (s32)&vertex[2], (s32)&vertex[3], ring->unk1AC, ring->unk1AB, ring->unk194);
            prim += 0x24;
            otherPrim += 0x24;
            tpagePrim += 8;
            if (ring->unk1A6 != ring->unk1A8) {
                setPrimRgb0(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb0(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
            }
            transformAndAddPolyG4((s32)prim, (s32)tpagePrim, (s32)&vertex[2], (s32)&vertex[3], (s32)&vertex[4], (s32)&vertex[5], ring->unk1AC, ring->unk1AB, ring->unk194);
            prim += 0x24;
            otherPrim += 0x24;
            tpagePrim += 8;
            vertex += 6;
        }
        break;
    }
    case 13: {
        u8 *prim;
        u8 *otherPrim;

        if (ring->unk1AD >= 0) {
            func_801E7020(ring->unk13C);
        }
        prim = ring->unk164[FRAME_BUFFER_INDEX];
        otherPrim = ring->unk164[FRAME_BUFFER_INDEX ^ 1];
        for (i = 0; i < ring->n; i++) {
            setPrimQuadUvRect(prim, ring->unk170.b[0], ring->unk170.b[2], ring->unk170.b[4], ring->unk170.b[6]);
            *(u16 *)(prim + 0x1A) = ring->unk178;
            *(u16 *)(prim + 0xE) = ring->unk17C;
            if (ring->unk1A6 != ring->unk1A8) {
                setPrimRgb0(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(prim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb0(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb1(otherPrim, innerRgb[0], innerRgb[1], innerRgb[2]);
                setPrimRgb2(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb3(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
            }
            transformAndAddPolyGT4((s32)prim, (s32)&vertex[0], (s32)&vertex[1], (s32)&vertex[2], (s32)&vertex[3], ring->unk1AB, ring->unk194);
            prim += 0x34;
            otherPrim += 0x34;
            setPrimQuadUvRect(prim, ring->unk170.b[0], ring->unk170.b[2], ring->unk170.b[4], ring->unk170.b[6]);
            *(u16 *)(prim + 0x1A) = ring->unk178;
            *(u16 *)(prim + 0xE) = ring->unk17C;
            if (ring->unk1A6 != ring->unk1A8) {
                setPrimRgb0(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(prim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(prim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb0(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb1(otherPrim, midRgb[0], midRgb[1], midRgb[2]);
                setPrimRgb2(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
                setPrimRgb3(otherPrim, outerRgb[0], outerRgb[1], outerRgb[2]);
            }
            transformAndAddPolyGT4((s32)prim, (s32)&vertex[2], (s32)&vertex[3], (s32)&vertex[4], (s32)&vertex[5], ring->unk1AB, ring->unk194);
            prim += 0x34;
            otherPrim += 0x34;
            vertex += 6;
        }
        break;
    }
    }
    PopMatrix();
    ring->unk1A8 = ring->unk1A6;
}

void freeRingEffect(Obj32 *ring) {
    s32 i;

    for (i = 0; i < 2; i++) {
        freeHeapBlock(ring->unk15C[i]);
        freeHeapBlock(ring->unk164[i]);
    }
    if (ring->unk1AD >= 0) {
        func_801E72D4(ring->unk13C);
    }
    freeHeapBlock(ring->unk16C);
    freeHeapBlock(ring);
}

Particles *createStreakParticles(u8 *startColor, u8 *endColor, Unk13C *template, s16 spreadX, s16 spreadY, s16 length, s16 endLength, s16 frames, s16 speedRange, s16 reverse,
                         s16 count, s16 zOffset, s16 spin, s16 pattern, s16 kind, s16 semi, s32 flags, s32 fixedOtz) {
    Particles *fx;
    Particle *particle;
    LINE_G2 *line;
    s32 i;
    s32 spinAngle;
    s32 angle;

    fx = allocTaskHeapBlock(0x15C);
    fx->p = particle = allocTaskHeapBlock(count * 0x88);
    spinAngle = 0;
    if (template == 0) {
        fx->parent = (u8 *)SCENE_3D + 0x78;
        fx->own = 0;
    } else {
        fx->base = *template;
        initEffectObject(fx);
        fx->parent = fx;
        fx->own = 1;
    }
    fx->unk14E = zOffset;
    fx->unk154 = fixedOtz;
    fx->unk15A = flags & 1;
    fx->count = count;
    fx->unk150 = 0;
    fx->frames = frames;
    fx->rgb[0] = startColor[0];
    fx->rgb[1] = startColor[1];
    fx->rgb[2] = startColor[2];
    fx->drgb[0] = (endColor[0] - fx->rgb[0]) / fx->frames;
    fx->drgb[1] = (endColor[1] - fx->rgb[1]) / fx->frames;
    fx->drgb[2] = (endColor[2] - fx->rgb[2]) / fx->frames;
    fx->unk158 = reverse == 0 ? 1 : -1;
    fx->kind = kind;
    for (i = 0; i < fx->count; i++, particle++) {
        if (fx->kind == 0) {
            line = &particle->line[0];
            func_800678E4(line);
            setSemiTrans(line, semi);
            line = &particle->line[1];
            func_800678E4(line);
            setSemiTrans(line, semi);
        } else {
            line = &particle->line[0];
            func_80067904(line);
            setSemiTrans(line, semi);
            line->r0 = startColor[0];
            line->g0 = startColor[1];
            line->b0 = startColor[2];
            line->r1 = endColor[0];
            line->g1 = endColor[1];
            line->b1 = endColor[2];
            line++;
            func_80067904(line);
            setSemiTrans(line, semi);
            line->r0 = startColor[0];
            line->g0 = startColor[1];
            line->b0 = startColor[2];
            line->r1 = endColor[0];
            line->g1 = endColor[1];
            line->b1 = endColor[2];
        }
        initTransform(particle, (s32)fx->parent, 0, 0, 0, 0, 0, 0);
        particle->unk7C = length * 8;
        particle->unk7E = rand() % speedRange + 1;
        if (spreadY == 0) {
            spreadY = 1;
        }
        if (spreadX == 0) {
            spreadX = 1;
        }
        if (pattern < 3) {
            particle->unk32 = rand() % spreadX - spreadX / 2;
            particle->unk30 = rand() % spreadY - spreadY / 2;
            particle->unk34 = 0;
            particle->unk7A = 0;
        } else {
            particle->unk32 = 0;
            particle->unk30 = 0;
            particle->unk34 = 0;
            particle->unk7A = spreadX - 0xB4;
        }
        particle->unk80 = particle->unk7E * frames;
        particle->unk78 = 0;
        particle->unk76 = 0;
        particle->unk74 = 0;
        if (spin != 0) {
            switch ((s16)(pattern % 3)) {
            case 0:
                angle = i << 12;
                spinAngle = angle / fx->count;
                particle->unk76 = spin;
                break;
            case 1:
                spinAngle = rand() % 4096;
                particle->unk76 = spin;
                break;
            case 2:
                spinAngle = rand() % 4096;
                particle->unk76 = rand() % spin;
                break;
            }
            particle->unk34 = spinAngle;
        }
        particle->unk82 = rand() & 0xFFF;
        particle->unk84 = rand() & 0x1FF;
        if (i & 1) {
            particle->unk84 = -particle->unk84;
        }
    }
    fx->unk147 = flags & 2;
    if (endLength == 0) {
        fx->unk156 = 0;
    } else {
        fx->unk156 = (endLength - length) * 8 / fx->frames;
    }
    return fx;
}

void renderStreakParticles(Particles *fx) {
    Particle *particle;
    LINE_G2 *line;
    SVECTOR *vertex;
    s32 limit;
    s32 i;
    s32 frame;
    s32 length;
    u32 otz;
    s16 dx;
    s16 dz;
    s32 interp;
    u8 r;
    u8 g;
    u8 b;
    s32 flag;

    particle = fx->p;
    limit = 10000;
    if (fx->own != 0) {
        if (((u8 *)fx)[0x139] != 0) {
            tickEffectStartDelay(fx);
            return;
        }
        PushMatrix();
        tickEffectMotion((s32)fx, fx->unk15A);
        PopMatrix();
        limit = (*(s16 *)((u8 *)fx + 0x124) + fx->frames - 1) / fx->frames * fx->frames;
    }
    PushMatrix();
    if (fx->kind == 0) {
        if (fx->unk147 == 0) {
        for (i = 0; i < fx->count; i++) {
            frame = fx->unk150 + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->unk74;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->unk158 < 0) {
                    vertex->vz = particle->unk80 - particle->unk7E * frame;
                } else {
                    vertex->vz = particle->unk7E * frame;
                }
                vertex->vz += fx->unk14E;
                length = (particle->unk7C + fx->unk156 * frame) * fx->unk158 / 8;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vz += length;
                    otz = RotTransPers((s32)vertex, (s32)&line->r1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->unk154 != 0) {
                            otz = fx->unk154;
                        }
                        r = fx->rgb[0] + fx->drgb[0] * frame;
                        g = fx->rgb[1] + fx->drgb[1] * frame;
                        b = fx->rgb[2] + fx->drgb[2] * frame;
                        line->r0 = r;
                        line->g0 = g;
                        line->b0 = b;
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle++;
        }
        } else {
        for (i = 0; i < fx->count; i++) {
            frame = fx->unk150 + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->unk74;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->unk158 < 0) {
                    vertex->vz = particle->unk80 - particle->unk7E * frame;
                } else {
                    vertex->vz = particle->unk7E * frame;
                }
                vertex->vz += fx->unk14E;
                length = (particle->unk7C + fx->unk156 * frame) * fx->unk158 / 16;
                vertex->vx += dx = length * rsin(particle->unk82) / 4096;
                vertex->vz += dz = length * rcos(particle->unk82) / 4096;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vx -= dx;
                    vertex->vz -= dz;
                    otz = RotTransPers((s32)vertex, (s32)&line->r1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->unk154 != 0) {
                            otz = fx->unk154;
                        }
                        r = fx->rgb[0] + fx->drgb[0] * frame;
                        g = fx->rgb[1] + fx->drgb[1] * frame;
                        b = fx->rgb[2] + fx->drgb[2] * frame;
                        line->r0 = r;
                        line->g0 = g;
                        line->b0 = b;
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle->unk82 += particle->unk84;
            particle++;
        }
        }
    } else {
        if (fx->unk147 == 0) {
        for (i = 0; i < fx->count; i++) {
            frame = fx->unk150 + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->unk74;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->unk158 < 0) {
                    vertex->vz = particle->unk80 - particle->unk7E * frame;
                } else {
                    vertex->vz = particle->unk7E * frame;
                }
                vertex->vz += fx->unk14E;
                length = (particle->unk7C + fx->unk156 * frame) * fx->unk158 / 8;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vz += length;
                    otz = RotTransPers((s32)vertex, (s32)&line->x1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->unk154 != 0) {
                            otz = fx->unk154;
                        }
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle++;
        }
        } else {
        for (i = 0; i < fx->count; i++) {
            frame = fx->unk150 + i;
            if (frame < limit) {
                line = &particle->line[FRAME_BUFFER_INDEX];
                vertex = (SVECTOR *)&particle->unk74;
                frame %= fx->frames;
                updateTransformMatrix(particle, 0);
                if (fx->unk158 < 0) {
                    vertex->vz = particle->unk80 - particle->unk7E * frame;
                } else {
                    vertex->vz = particle->unk7E * frame;
                }
                vertex->vz += fx->unk14E;
                length = (particle->unk7C + fx->unk156 * frame) * fx->unk158 / 16;
                vertex->vx += dx = length * rsin(particle->unk82) / 4096;
                vertex->vz += dz = length * rcos(particle->unk82) / 4096;
                if (RotTransPers((s32)vertex, (s32)&line->x0, &interp, &flag) < 0x1000U) {
                    vertex->vx -= dx;
                    vertex->vz -= dz;
                    otz = RotTransPers((s32)vertex, (s32)&line->x1, &interp, &flag);
                    if (otz < 0x1000U) {
                        if (fx->unk154 != 0) {
                            otz = fx->unk154;
                        }
                        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);
                    }
                }
            }
            particle->unk82 += particle->unk84;
            particle++;
        }
        }
    }
    fx->unk150++;
    PopMatrix();
}

void freeStreakParticles(void *fx) {
    freeHeapBlock((*(void **)((s8 *)fx + 0x140)));
    freeHeapBlock(fx);
}
