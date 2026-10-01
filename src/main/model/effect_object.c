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

/* Moves the effect from its start along dir: distance = speed * t + accel * t^2 */
void updateEffectLinearMotion(EffectObject *fx) {
    s32 accelTerm;
    s32 distance;

    accelTerm = fx->moveAccel * fx->t2 * fx->t2;
    distance = fx->moveSpeed * fx->t + accelTerm;
    fx->posX = fx->px + ((distance * fx->dir.vx) >> 12);
    fx->posY = fx->py + ((distance * fx->dir.vy) >> 12);
    fx->posZ = fx->pz + ((distance * fx->dir.vz) >> 12);
}

/* The linear motion plus a hop on y */
void updateEffectArcMotion(EffectObject *fx) {
    updateEffectLinearMotion(fx);
    fx->posY += -fx->moveSpeed * fx->t + fx->moveSpeed * fx->t * fx->t / 56;
}

/* The linear motion plus a sine wave on x */
void updateEffectWaveXMotion(EffectObject *fx) {
    s32 phase;
    s32 wave;

    updateEffectLinearMotion(fx);
    phase = fx->wavePhase + fx->moveSpeed * fx->t;
    wave = fx->waveAmplitude * rsin(phase * fx->waveFreq);
    fx->posX = (wave >> 10) + fx->posX;
}

/* The linear motion plus a sine wave on y */
void updateEffectWaveYMotion(EffectObject *fx) {
    s32 phase;
    s32 wave;

    updateEffectLinearMotion(fx);
    phase = fx->wavePhase + fx->moveSpeed * fx->t;
    wave = fx->waveAmplitude * rsin(phase * fx->waveFreq);
    fx->posY = (wave >> 10) + fx->posY;
}

/* The arc motion, with the hop turned by the start rotation on z */
void updateEffectTiltedArcMotion(EffectObject *fx) {
    s32 height;
    s32 offsetX;
    s32 offsetY;

    updateEffectLinearMotion(fx);
    height = -fx->moveSpeed * fx->t + fx->moveSpeed * fx->t * fx->t / 56;
    offsetX = height * rsin((s16)fx->rz0) >> 12;
    offsetY = height * rcos((s16)fx->rz0) >> 12;
    fx->posX -= offsetX;
    fx->posY += offsetY;
}

/* Like updateEffectTiltedArcMotion, with moveAccel for the height of the hop */
void updateEffectTiltedAccelArcMotion(EffectObject *fx) {
    s32 distance;
    s32 height;
    s32 offsetX;
    s32 offsetY;

    distance = fx->moveSpeed * fx->t;
    fx->posX = fx->px + (distance * fx->dir.vx >> 12);
    fx->posY = fx->py + (distance * fx->dir.vy >> 12);
    fx->posZ = fx->pz + (distance * fx->dir.vz >> 12);
    height = -fx->moveSpeed * fx->t + fx->moveAccel * fx->t * fx->t / 56;
    offsetX = height * rsin((s16)fx->rz0) >> 12;
    offsetY = height * rcos((s16)fx->rz0) >> 12;
    fx->posX -= offsetX;
    fx->posY += offsetY;
}

/* Puts the effect at a random offset from its start, up to moveSpeed / 2 per axis */
void updateEffectShakeMotion(EffectObject *fx) {
    s32 x;
    s32 y;
    s32 z;

    x = fx->moveSpeed / 2 - rand() % fx->moveSpeed;
    y = fx->moveSpeed / 2 - rand() % fx->moveSpeed;
    z = fx->moveSpeed / 2 - rand() % fx->moveSpeed;
    fx->posX = fx->px + x;
    fx->posY = fx->py + y;
    fx->posZ = fx->pz + z;
}

#if VERSION_JP
/* Copies the transform recorded BACK rows before the trail's head (the
   last one recorded when BACK is 0) into the effect's fields. */
void getEffectTrailEntry(EffectTrail *trail, s32 back, VECTOR *pos, SVECTOR *rot, VECTOR *scale, s16 *brightness) {
    s32 i;

    if (back == 0) {
        if (pos != NULL) {
            *pos = trail->pos[trail->last];
        }
        if (rot != NULL) {
            *rot = trail->rot[trail->last];
        }
        if (scale != NULL) {
            *scale = trail->scale[trail->last];
        }
        if (brightness != NULL) {
            *brightness = trail->brightness[trail->last];
        }
    } else {
        i = trail->head;
        i -= back * trail->rows;
        if (i < 0) {
            i += trail->count;
        }
        if (pos != NULL) {
            *pos = trail->pos[i];
        }
        if (rot != NULL) {
            *rot = trail->rot[i];
        }
        if (scale != NULL) {
            *scale = trail->scale[i];
        }
        if (brightness != NULL) {
            *brightness = trail->brightness[i];
        }
    }
}
#endif

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

    fx = allocTaskHeapBlock(sizeof(EffectTemplate));
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

/*
 * Starts the motion again: from the start position (and rotation and scale)
 * for a moving mode, or from where the effect is now for modes 0 and 90
 */
void restartEffectMotion(void *obj) {
    EffectObject *fx;
    s32 i;

    fx = obj;
    /* jp has no start delay (us's modes 91 and up) nor a mode 90 */
#if VERSION_JP
    fx->suspended = 0;
#elif VERSION_US || VERSION_EU
    fx->suspended = fx->mode >= 91;
#endif
    fx->state = -1;
    for (i = 0; i < 3; i++) {
        fx->done[i] = 0;
    }
    fx->cnt = 0;
    fx->flag = 0;
    fx->t = 0;
    fx->t2 = 0;
#if VERSION_JP
    if (fx->mode != 0) {
#elif VERSION_US || VERSION_EU
    if (fx->mode != 0 && fx->mode != 90) {
#endif
        fx->sx = fx->sx0;
        fx->sy = fx->sy0;
        fx->sz = fx->sz0;
        fx->rotX = fx->rx0;
        fx->rotY = fx->ry0;
        fx->rotZ = fx->rz0;
        fx->posX = fx->px;
        fx->posY = fx->py;
        fx->posZ = fx->pz;
        fx->targetX = fx->px2;
        fx->targetY = fx->py2;
        fx->targetZ = fx->pz2;
        getDirectionVector((SVECTOR *)&fx->px, (SVECTOR *)&fx->px2, &fx->dir);
    } else {
        fx->sx0 = fx->sx;
        fx->sy0 = fx->sy;
        fx->sz0 = fx->sz;
        fx->rx0 = fx->rotX;
        fx->ry0 = fx->rotY;
        fx->rz0 = fx->rotZ;
        fx->px = fx->posX;
        fx->py = fx->posY;
        fx->pz = fx->posZ;
    }
}

void *initEffectObject(void *obj) {
    EffectObject *fx;

    fx = obj;
    initTransform(fx, fx->parent, fx->px, fx->py, fx->pz, fx->rx0, fx->ry0, fx->rz0);
    initTransform(&fx->targetMatrix, fx->parent, fx->px2, fx->py2, fx->pz2, 0, 0, 0);
    fx->matrix.t[0] = 0;
    fx->matrix.t[1] = 0;
    fx->matrix.t[2] = 0;
    fx->sx = fx->sx0;
    fx->sy = fx->sy0;
    fx->sz = fx->sz0;
    fx->rotX = fx->rx0;
    fx->rotY = fx->ry0;
    fx->rotZ = fx->rz0;
    fx->posX = fx->px;
    fx->posY = fx->py;
    fx->posZ = fx->pz;
    restartEffectMotion(fx);
    fx->targetX = fx->px2;
    fx->targetY = fx->py2;
    fx->targetZ = fx->pz2;
    getDirectionVector((SVECTOR *)&fx->px, (SVECTOR *)&fx->px2, &fx->dir);
#if VERSION_JP
    fx->trail = NULL;
#endif
    fx->unkFC = 0;
    fx->brightness = 0;
    fx->fadeState = 0;
    return fx;
}

/*
 * Moves an effect one frame. mode picks the path (the case lists below: 1
 * linear, 2 arc, 3/4 wave on x/y, 5 tilted arc, 7 updateEffectTiltedAccelArcMotion, 8 shake;
 * the modes of the first list stay at the start) and what happens when the
 * effect hits its target or its period runs out: stop, stop at the target,
 * restart, suspend or fade out. Modes 10 and >= 90 keep their scale and
 * rotation.
 */
/* us's modes 90 and up only wait (a start delay, mode 90 a fixed effect); jp
   has none of them */
#if VERSION_JP
#define IS_MOVING_MODE(mode) 1
#elif VERSION_US || VERSION_EU
#define IS_MOVING_MODE(mode) ((mode) < 90)
#endif

/* the scale an effect stops at becomes its current one: jp and eu copy the
   four words as one VECTOR */
#if VERSION_JP || VERSION_EU
#define SET_TARGET_SCALE(fx) (*(VECTOR *)&(fx)->sxT = *(VECTOR *)&(fx)->sx)
#elif VERSION_US
#define SET_TARGET_SCALE(fx) ((fx)->sxT = (fx)->sx, (fx)->syT = (fx)->sy, (fx)->szT = (fx)->sz, (fx)->swT = (fx)->sw)
#endif

s32 tickEffectMotion(s32 fxAddr, s32 applyFlag) {
    EffectObject *fx = (EffectObject *)fxAddr;
    u8 applyMode = applyFlag;
    SVECTOR prevPos;
    SVECTOR curPos;
    SVECTOR targetPos;
    s32 hit;

    if (fx->mode != 10 && IS_MOVING_MODE(fx->mode)) {
        if (fx->sx != fx->sxT) {
            fx->sx = fx->dsx * fx->t + fx->sx0;
            if (fx->dsx >= 0) {
                if (fx->sx >= fx->sxT) {
                    fx->sx = fx->sxT;
                    fx->done[0] = 1;
                }
            } else if (fx->sx <= fx->sxT) {
                fx->sx = fx->sxT;
                fx->done[0] = 1;
            }
        } else {
            fx->done[0] = 1;
        }
        if (fx->sy != fx->syT) {
            fx->sy = fx->dsy * fx->t + fx->sy0;
            if (fx->dsy >= 0) {
                if (fx->sy >= fx->syT) {
                    fx->sy = fx->syT;
                    fx->done[1] = 1;
                }
            } else if (fx->sy <= fx->syT) {
                fx->sy = fx->syT;
                fx->done[1] = 1;
            }
        } else {
            fx->done[1] = 1;
        }
        if (fx->sz != fx->szT) {
            fx->sz = fx->dsz * fx->t + fx->sz0;
            if (fx->dsz >= 0) {
                if (fx->sz >= fx->szT) {
                    fx->sz = fx->szT;
                    fx->done[2] = 1;
                }
            } else if (fx->sz <= fx->szT) {
                fx->sz = fx->szT;
                fx->done[2] = 1;
            }
        } else {
            fx->done[2] = 1;
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
#if VERSION_US || VERSION_EU
    case 90:
#endif
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
        updateEffectTiltedArcMotion(fx);
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
        updateEffectTiltedAccelArcMotion(fx);
        break;
    case 8:
    case 89:
        updateEffectShakeMotion(fx);
        break;
#if VERSION_JP
    case 10:
        getEffectTrailEntry(fx->trail, fx->unkFC, (VECTOR *)&fx->posX, (SVECTOR *)&fx->rotX, (VECTOR *)&fx->sx, &fx->brightness);
        break;
#endif
    }
    if (fx->state != -1 && fx->mode != 0 && IS_MOVING_MODE(fx->mode)) {
        updateTransformMatrix(fx, applyMode);
        updateTransformMatrix(&fx->targetMatrix, 0);
        getTransformWorldPos(fx, &curPos);
        getTransformWorldPos(&fx->targetMatrix, &targetPos);
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
                restartEffectMotion(fx);
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
                restartEffectMotion(fx);
                SET_TARGET_SCALE(fx);
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
                restartEffectMotion(fx);
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
                /* jp also restarts it in place, as for 12 */
#if VERSION_JP
                restartEffectMotion(fx);
                SET_TARGET_SCALE(fx);
                fx->drx = 0;
                fx->dry = 0;
                fx->drz = 0;
                fx->ddrx = 0;
                fx->ddry = 0;
                fx->ddrz = 0;
#endif
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
    if (IS_MOVING_MODE(fx->mode) && fx->mode != 0 && fx->state == -1) {
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
            restartEffectMotion(fx);
            break;
        }
    }
    if (IS_MOVING_MODE(fx->mode)) {
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
                restartEffectMotion(fx);
                break;
            case 30:
            case 33:
            case 36:
            case 39:
            case 42:
            case 45:
            case 48:
                fx->mode = 0;
                restartEffectMotion(fx);
                SET_TARGET_SCALE(fx);
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
                restartEffectMotion(fx);
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
                restartEffectMotion(fx);
                SET_TARGET_SCALE(fx);
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

/* jp has no start delay */
#if VERSION_US || VERSION_EU
/* Modes >= 91 wait wavePhase frames, then run as mode - 100 */
void tickEffectStartDelay(void *obj) {
    EffectObject *fx;

    fx = obj;
    if (fx->mode >= 91) {
        if (fx->wavePhase > fx->t) {
            fx->t++;
            return;
        }
        fx->t = 0;
        fx->suspended = 0;
        fx->mode -= 100;
    }
}
#endif

/*
 * The brightness (0-0x100) of the effect this frame. fadeMode 1: from the
 * x scale; 2: rises by speed; 3: rises then falls once; 4: pulses; 5: a
 * byte counter mapped to 0x80-0xFF. Mode 10 keeps the saved brightness.
 */
s16 updateEffectBrightness(void *obj, s16 brightness) {
    EffectObject *fx;

    fx = obj;
    switch (fx->fadeMode) {
    case 1:
        brightness = fx->sx / 16;
        if (fx->sx > 0x1000) {
            brightness = 0x100 - (fx->sx - 0x1000) / 16;
        }
        break;
    case 2:
        brightness += fx->speed;
        break;
    case 3:
        if (fx->fadeState == 2) {
            break;
        }
        if (fx->fadeState == 0) {
            brightness += fx->speed;
            if (brightness > 0x100) {
                brightness = 0x100;
                fx->fadeState = 1;
            }
        } else {
            brightness -= fx->speed;
            if (brightness < 0) {
                brightness = 0;
                fx->fadeState = 2;
            }
        }
        break;
    case 4:
        if (fx->fadeState == 0) {
            brightness += fx->speed;
            if (brightness > 0x100) {
                brightness = 0x100;
                fx->fadeState = 1;
            }
        } else {
            brightness -= fx->speed;
            if (brightness < 0) {
                brightness = 0;
                fx->fadeState = 0;
            }
        }
        break;
    case 5:
        fx->fadeState += fx->speed;
        if ((s8)fx->fadeState >= 0) {
            brightness = fx->fadeState + 0x80;
        } else {
            brightness = 0xFF - (fx->fadeState & 0x7F);
        }
        break;
    }
    if (brightness < 0) {
        brightness = 0;
    }
    if (brightness > 0x100) {
        brightness = 0x100;
    }
    if (fx->mode == 10) {
        brightness = fx->brightness;
    } else {
        fx->brightness = brightness;
    }
    return brightness;
}
