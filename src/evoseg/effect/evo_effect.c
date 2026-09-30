#include "common.h"
#include "game.h"
#include "dcb/evo_effect.h"
#include "dcb/heap.h"
#include "dcb/model_load.h"
#include "dcb/anim_control.h"
#include "dcb/task.h"
#include "dcb/window.h"
#include "dcb/prim_util.h"
#include "dcb/decompress.h"
#include "dcb/effect_object.h"
#include "dcb/effect_prims.h"
#include "dcb/battle_hud.h"
#include "dcb/transform.h"
#include "dcb/loader.h"
#include "dcb/script.h"
#include "dcb/card_zones.h"
#include "dcb/sound_play.h"
#include "dcb/vblank.h"
#include "dcb/scroll_bg.h"
#include "dcb/evoseg.h"
#include "dcb/evo_fusion_result.h"

typedef struct {
    s16 kind;
    s16 active;
    EvoFx *handle;
} EvoEntry;

typedef struct {
    EvoMsd *data;
    Script *script;
    s32 *vars;
    EvoEntry entries[16];
    s32 counter;
    void *buffer;
} EvoEffectScript;

typedef struct {
    u8 pad[0x260];
    s32 x;
    s32 y;
    s32 z;
} EvoObject;

typedef struct {
    u8 pad0[0x98];
    s32 r;
    s32 g;
    s32 b;
} EvoLight;

typedef struct {
    u8 pad0[0x260];
    s32 args[34];
} EvoFxParams;

typedef struct {
    DrTPage tpage[2];
    PolyF4 poly[2];
    Bytes4 from;
    Bytes4 to;
    s16 speed;
    u8 state;
    u8 mode;
} EvoFadeRect;

typedef struct {
    EvoFx fx;
    Model *model;
    u8 pad140[0x160 - 0x140];
    EvoClut clut;
    s16 prevLevel;
    s8 slot;
    u8 flags;
    s8 unk570;
    s8 active;
    u8 pad572[2];
    s32 clutBank; /* 0 takes the model's CLUT from the row at y 0xF0, else 0x70 */
} EvoModelFx;

typedef void (*EvoFxFunc)(EvoFx *);

extern s8 EVO_EFFECT_PLAYER;
extern s8 EVO_EFFECT_SPRITE_1;
extern s8 EVO_EFFECT_SPRITE_2;
extern EvoFx EVO_EFFECT_ROOT;
extern void (*EVO_EFFECT_FREE_FUNCS[])(EvoFx *);
extern u8 CLEAR_BG_ON_DRAW;

/* KAWSEG's functions, at their KAWSEG addresses: this effect script was
   copied from KAWSEG's (the opcodes that call them are KAWSEG's) */
void KAW_fadeCardSprite(void *sprite, Color *color);
void KAW_hideCardLabel(void *sprite);
void KAW_showCardLabel(void *sprite, s32 number);
void initPolyF4Pair();
void EVO_playEffectScript(s32 index, s32 player1, s32 player2, s32 mode1, s32 mode2);
void EVO_runEffectScriptTask();
void EVO_runEffectScript(EvoEffectScript *loader);
void EVO_initEffectFromParams(EvoFx *fx, s32 *vars, EvoEffectScript *loader);
void EVO_getEffectParams(EvoFx *fx, EvoFxParams *params, s32 current);
void EVO_setEffectParams(EvoFx *fx, EvoFxParams *params);
void EVO_getEffectWorldPos(void *xform, EvoObject *obj);
void EVO_createEffectEntry(s32 index, s32 kind, s32 *vars, EvoEffectScript *loader);

EvoModelFx *EVO_createModelEffect(s16 level, EvoFx *fx, s32 modelId, s32 anim, s32 unused, s32 vramSlot, u8 flags,
                          s32 loop, s32 pak, s32 clutBank) {
    EvoModelFx *obj;
    s32 slot;

    obj = allocTaskHeapBlock(sizeof(EvoModelFx));
    for (slot = 2; slot < 23; slot++) {
        if (SCENE_3D->modelState[slot] == 0) {
            break;
        }
    }
    if (slot >= 23 || loadModel(slot, modelId, vramSlot, pak, 0) == 0) {
        freeHeapBlock(obj);
        return NULL;
    }
    obj->slot = slot;
    obj->model = SCENE_3D->models[slot];
    if (anim >= 0 && loadModelAnimation(slot, anim, 0, pak) != 0) {
        applyAnimationFirstFrame(slot, 0);
        startModelAnimation(slot, 0, -2, loop ^ 1);
    }
    if (fx != NULL) {
        obj->fx = *fx;
        initEffectObject(obj);
        SCENE_3D->modelState[obj->slot] = -1;
        obj->active = 1;
    } else {
        SCENE_3D->modelState[obj->slot] = 1;
        obj->active = 0;
    }
    obj->clutBank = clutBank;
    obj->model->link = (s32)obj;
    obj->unk570 = -1;
    {
        Rect16 rect = { 0x30, 0x70, 0x10, 0x10 };

        if (obj->clutBank == 0) {
            rect.y = 0xF0;
        }
        rect.x += ((obj->model->tpageOffset / 0x10000 + 5) & 0xF) << 6;
        rect.y += ((obj->model->tpageOffset / 0x10000 + 5) >> 4) << 8;
        StoreImage2(&rect, (u32 *)obj->clut.src);
        obj->clut.rect = rect;
        obj->clut.brighten = 0;
        obj->prevLevel = level;
        obj->clut.level = level;
        if (level != 0xFF) {
            EVO_uploadShadedClut(&obj->clut, 0x8000);
        }
    }
    obj->flags = flags;
    return obj;
}

void EVO_tickModelEffect(EvoModelFx *obj) {
    if (obj->active != 0) {
        if (obj->fx.suspended != 0) {
            tickEffectStartDelay(obj);
            SCENE_3D->modelState[obj->slot] = -1;
        } else {
            obj->clut.level = updateEffectBrightness(obj, obj->clut.level);
            if (obj->clut.level != obj->prevLevel) {
                obj->prevLevel = obj->clut.level;
                EVO_uploadShadedClut(&obj->clut, 0x8000);
            }
            if (obj->clut.level == 0) {
                SCENE_3D->modelState[obj->slot] = -1;
            } else {
                SCENE_3D->modelState[obj->slot] = 3;
            }
        }
    }
}

void EVO_freeModelEffect(EvoModelFx *obj) {
    unloadModel(obj->slot);
    if (obj->clut.level != 0xFF) {
        obj->clut.level = 0xFF;
        EVO_uploadShadedClut(&obj->clut, 0x8000);
    }
    freeHeapBlock(obj);
}

EvoFadeRect *EVO_createFadeRect(s16 *rect, Bytes4 *from, Bytes4 *to, u8 blendMode, s16 speed, u8 mode) {
    EvoFadeRect *f;

    f = allocTaskHeapBlock(sizeof(EvoFadeRect));
    initPolyF4Pair(&f->poly[0], &f->poly[1], from, blendMode, &f->tpage[0], &f->tpage[1], rect, 1, 1);
    f->from = *from;
    f->to = *to;
    f->speed = speed;
    f->mode = mode;
    if (mode < 2 || mode == 3) {
        f->state = 1;
    } else {
        f->state = 0;
    }
    return f;
}

s32 EVO_tickFadeRect(EvoFadeRect *f) {
    PolyF4 *poly;
    DrTPage *tpage;
    s32 doneIn = 0;
    s32 doneOut = 0;

    if (f->state == 0) {
        return -1;
    }
    poly = &f->poly[FRAME_BUFFER_INDEX];
    tpage = &f->tpage[FRAME_BUFFER_INDEX];
    if (f->state == 1) {
        doneIn = stepColorToward(f->speed, &poly->r0, f->to.b[0], &poly->g0, f->to.b[1], &poly->b0, f->to.b[2]);
    } else if (f->state == 2) {
        doneOut = stepColorToward(f->speed, &poly->r0, f->from.b[0], &poly->g0, f->from.b[1], &poly->b0, f->from.b[2]);
        if (doneOut != 0 && f->mode == 3) {
            f->state = 1;
        }
    }
    if (*(u32 *)&poly->r0 & 0xFFFFFF) {
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[1], (s32)poly);
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[1], (s32)tpage);
    }
    if (doneIn != 0 && (f->mode == 1 || f->mode == 3)) {
        f->state = 2;
    }
    if (doneIn == 0 && doneOut == 0) {
        return 0;
    }
    if (doneIn != 0) {
        return 1;
    }
    if (doneOut != 0) {
        return 2;
    }
    return -1;
}

void (*EVO_EFFECT_TICK_FUNCS[5])(EvoFx *) = {
    (EvoFxFunc)EVO_tickFadeRect,
    (EvoFxFunc)renderRingEffect,
    (EvoFxFunc)updateEffectObject,
    (EvoFxFunc)renderStreakParticles,
    (EvoFxFunc)EVO_tickModelEffect,
};

s32 EVO_loadEffectPak(s32 index) {
    char path[32];
    s32 file;

    sprintf(path, "C:\\EVO_PAK\\%d.PAK", index);
    file = loadFileTagged((s32 *)path, getCurrentTaskId(), 0x12C);
    if (file != 0) {
        return file;
    }
    return 0;
}

void EVO_loadEffectArchive(void) {
    spawnTask(0, -1, 0, 0x800, loadFileTagged, "C:\\Unit_eff.arc", getCurrentTaskId(), -2);
    EVO_EFFECT_ARCHIVE = (u8 *)waitFrames(0x7FFFFFFF);
}

void EVO_freeEffectArchive(void) {
    freeHeapBlock(EVO_EFFECT_ARCHIVE);
}

void EVO_playEffect(s32 index, s32 player) {
    EVO_playEffectScript(index, player, player, 0, 0);
}

void EVO_playCardEffect(s32 index, s32 player, s32 mode) {
    EVO_playEffectScript(index, player, player, mode, mode);
}

/* the players and modes are KAWSEG's; EVOSEG's scripts ignore them */
void EVO_playEffectScript(s32 index, s32 player1, s32 player2, s32 mode1, s32 mode2) {
    s32 data;

    EVO_EFFECT_PLAYER = 0;
    EVO_EFFECT_SPRITE_1 = 0;
    EVO_EFFECT_SPRITE_2 = 0;
    data = decompressArchiveEntry((s32)EVO_EFFECT_ARCHIVE, index);
    spawnTask(0, 0x1F, 0, 0x800, EVO_runEffectScriptTask, data, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    freeHeapBlock((void *)data);
}

s32 EVO_tickEffectScript(EvoEffectScript *loader) {
    s32 i;

    PushMatrix();
    tickEffectMotion((s32)&EVO_EFFECT_ROOT, 0);
    PopMatrix();
    loader->vars[0] = 1;
    EVO_runEffectScript(loader);
    for (i = 0; i < 16; i++) {
        if (loader->entries[i].active != 0 && EVO_EFFECT_TICK_FUNCS[loader->entries[i].kind] != NULL) {
            EVO_EFFECT_TICK_FUNCS[loader->entries[i].kind](loader->entries[i].handle);
            if (loader->entries[i].kind > 0) {
                loader->vars[i + 88] = loader->entries[i].handle->state;
                loader->vars[i + 120] = loader->entries[i].handle->flag;
            }
        }
    }
    return loader->vars[0];
}

void EVO_freeEffectEntries(EvoEffectScript *loader) {
    s32 i;

    waitFrames(FRAME_INTERVAL);
    for (i = 0; i < 16; i++) {
        if (loader->entries[i].kind != -1) {
            EVO_EFFECT_FREE_FUNCS[loader->entries[i].kind](loader->entries[i].handle);
        }
    }
    waitFrames(FRAME_INTERVAL);
    if (loader->buffer != NULL) {
        freeHeapBlock(loader->buffer);
    }
}

void EVO_setZonePosition(s32 kind, EvoObject *obj) {
    switch (kind) {
    case 0:
        obj->x = 28;
        obj->y = -13;
        obj->z = 0;
        break;
    case 1:
        obj->x = -106;
        obj->y = -13;
        obj->z = 0;
        break;
    case 2:
        obj->x = -12;
        obj->y = -21;
        obj->z = 0;
        break;
    case 3:
        obj->x = -90;
        obj->y = -21;
        obj->z = 0;
        break;
    case 4:
        obj->x = -59;
        obj->y = -21;
        obj->z = 0;
        break;
    default:
        obj->x = 0;
        obj->y = 0;
        obj->z = 0;
        break;
    }
}

void EVO_setCardSpriteColor(s32 index, EvoLight *light) {
    Color color;

    if (index >= 0) {
        color.r = light->r;
        color.g = light->g;
        color.b = light->b;
        KAW_fadeCardSprite(SPRITE(index), &color);
    }
}

void EVO_runEffectScript(EvoEffectScript *loader) {
    s32 *vars;
    s32 result;
    s32 index;

    if (loader->counter != 0) {
        loader->counter--;
        return;
    }
    vars = loader->vars;
    do {
        result = runScriptToNextEvent(loader->script, vars);
        if (result == 1) {
            switch (loader->script->eventOp) {
            case 10:
                switch (loader->script->eventArg) {
                case 0:
                    EVO_getEffectParams(&EVO_EFFECT_ROOT, (EvoFxParams *)vars, 0);
                    break;
                case 1:
                    EVO_setEffectParams(&EVO_EFFECT_ROOT, (EvoFxParams *)vars);
                    restartEffectMotion((u8 *)&EVO_EFFECT_ROOT);
                    break;
                case 2:
                    SCREEN_COPY_MODE = 1;
                    break;
                case 3:
                    SCREEN_COPY_MODE = 0;
                    break;
                case 4:
                    CLEAR_BG_ON_DRAW = 0;
                    break;
                case 5:
                    CLEAR_BG_ON_DRAW = 1;
                    break;
                case 6:
                    EVO_setZonePosition(EVO_EFFECT_SPRITE_1, (EvoObject *)vars);
                    break;
                case 7:
                    EVO_setZonePosition(EVO_EFFECT_SPRITE_2, (EvoObject *)vars);
                    break;
                case 8:
                    EVO_setCardSpriteColor(EVO_EFFECT_SPRITE_1, (EvoLight *)vars);
                    break;
                case 9:
                    EVO_setCardSpriteColor(EVO_EFFECT_SPRITE_2, (EvoLight *)vars);
                    break;
                case 10:
                    index = EVO_EFFECT_SPRITE_1;
                    if (index >= 0) {
                        KAW_hideCardLabel(SPRITE(index));
                    }
                    break;
                case 11:
                    index = EVO_EFFECT_SPRITE_2;
                    if (index >= 0) {
                        KAW_hideCardLabel(SPRITE(index));
                    }
                    break;
                case 12:
                    EVO_TRAYS[0].merge = 0;
                    EVO_TRAYS[1].merge = 1;
                    break;
                case 13:
                    EVO_SCREEN_FLASH.on = 1;
                    break;
                case 14:
                    EVO_SCREEN_FLASH.on = 0;
                    break;
                case 15:
                    EVO_TRAYS[0].x = 0x3A;
                    EVO_TRAYS[1].x = -0x58;
                    break;
                }
                break;
            case 11:
                switch (loader->script->eventArg) {
                case 0:
                    playSoundEffect((s16)loader->script->params[0]);
                    break;
                case 1:
                    EVO_getEffectParams(loader->entries[(s16)loader->script->params[0]].handle, (EvoFxParams *)vars, 0);
                    break;
                case 2:
                    EVO_initEffectFromParams(loader->entries[(s16)loader->script->params[0]].handle, vars, loader);
                    initEffectObject(loader->entries[(s16)loader->script->params[0]].handle);
                    break;
                case 3:
                    stopSoundVoice(loader->script->params[0]);
                    break;
                case 4:
                    if (loader->entries[(s16)loader->script->params[0]].handle != NULL) {
                        loader->entries[(s16)loader->script->params[0]].active = 1;
                    }
                    break;
                case 5:
                    loader->entries[(s16)loader->script->params[0]].active = 0;
                    break;
                case 6:
                    loader->counter = (s16)loader->script->params[0] - 1;
                    return;
                case 7:
                    EVO_getEffectWorldPos(loader->entries[(s16)loader->script->params[0]].handle, (EvoObject *)vars);
                    break;
                case 8:
                    EVO_getEffectParams(loader->entries[(s16)loader->script->params[0]].handle, (EvoFxParams *)vars, 1);
                    break;
                case 9:
                    EVO_setZonePosition(getActiveDigimonCard(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 10:
                    EVO_setZonePosition(getPlayedCard(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 11:
                    EVO_setZonePosition(peekOnlineDeckTop(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 12:
                    EVO_setZonePosition(peekOfflineDeckTop(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 15:
                    EVO_setCardSpriteColor(peekOnlineDeckTop(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoLight *)vars);
                    break;
                case 16:
                    EVO_setCardSpriteColor(peekOfflineDeckTop(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoLight *)vars);
                    break;
                case 18:
                    printf("NO USE\n");
                    break;
                case 19:
                    index = EVO_EFFECT_SPRITE_1;
                    if (index >= 0) {
                        KAW_showCardLabel(SPRITE(index), (s16)loader->script->params[0]);
                    }
                    break;
                case 20:
                    index = EVO_EFFECT_SPRITE_2;
                    if (index >= 0) {
                        KAW_showCardLabel(SPRITE(index), (s16)loader->script->params[0]);
                    }
                    break;
                case 21:
                    EVO_setZonePosition((s16)loader->script->params[0], (EvoObject *)vars);
                    break;
                case 22:
                    loader->buffer = (void *)EVO_loadEffectPak((s16)loader->script->params[0]);
                    break;
                case 23:
                    animateWindowTo(&EVO_WINDOWS[12].win, (Rect16 *)-1);
                    break;
                }
                break;
            case 12:
                switch (loader->script->eventArg) {
                case 6:
                    EVO_createEffectEntry((s16)loader->script->params[0], (s16)loader->script->params[1], vars, loader);
                    break;
                case 1:
                    vars[1] = rsin((s16)loader->script->params[1]) * (s16)loader->script->params[0] / 4096;
                    break;
                case 2:
                    vars[1] = rcos((s16)loader->script->params[1]) * (s16)loader->script->params[0] / 4096;
                    break;
                case 3:
                    playSoundEffectOnVoice((s16)loader->script->params[0], (s16)loader->script->params[1]);
                    break;
                case 5:
                    printf("NO USE\n");
                    break;
                }
                break;
            }
        }
        clearScriptBusy(loader->script);
    } while (result != 0);
}

void EVO_getEffectParams(EvoFx *fx, EvoFxParams *params, s32 current) {
    if (current == 0) {
        params->args[0] = fx->rot.vx;
        params->args[1] = fx->rot.vy;
        params->args[2] = fx->rot.vz;
        params->args[8] = fx->scale.vx;
        params->args[9] = fx->scale.vy;
        params->args[10] = fx->scale.vz;
        params->args[17] = fx->pos.vx;
        params->args[18] = fx->pos.vy;
        params->args[19] = fx->pos.vz;
    } else {
        params->args[0] = fx->curRot.vx;
        params->args[1] = fx->curRot.vy;
        params->args[2] = fx->curRot.vz;
        params->args[8] = fx->curScale.vx;
        params->args[9] = fx->curScale.vy;
        params->args[10] = fx->curScale.vz;
        params->args[17] = fx->curPos[0];
        params->args[18] = fx->curPos[1];
        params->args[19] = fx->curPos[2];
    }
    params->args[3] = fx->rotVel.vx;
    params->args[4] = fx->rotVel.vy;
    params->args[5] = fx->rotVel.vz;
    params->args[6] = fx->moveSpeed;
    params->args[7] = fx->moveAccel;
    params->args[11] = fx->scaleVel.vx;
    params->args[12] = fx->scaleVel.vy;
    params->args[13] = fx->scaleVel.vz;
    params->args[14] = fx->scaleAccel.vx;
    params->args[15] = fx->scaleAccel.vy;
    params->args[16] = fx->scaleAccel.vz;
    params->args[20] = fx->vel.vx;
    params->args[21] = fx->vel.vy;
    params->args[22] = fx->vel.vz;
    params->args[23] = fx->accel.vx;
    params->args[24] = fx->accel.vy;
    params->args[25] = fx->accel.vz;
    params->args[26] = fx->hitRadius;
    params->args[27] = fx->period;
    params->args[28] = fx->fadeMode;
    params->args[29] = fx->speed;
    params->args[31] = fx->wavePhase;
    params->args[32] = fx->waveAmplitude;
    params->args[33] = fx->waveFreq;
    params->args[30] = fx->mode;
}

void EVO_setEffectParams(EvoFx *fx, EvoFxParams *params) {
    fx->rot.vx = params->args[0];
    fx->rot.vy = params->args[1];
    fx->rot.vz = params->args[2];
    fx->rotVel.vx = params->args[3];
    fx->rotVel.vy = params->args[4];
    fx->rotVel.vz = params->args[5];
    fx->moveSpeed = params->args[6];
    fx->moveAccel = params->args[7];
    fx->scale.vx = params->args[8];
    fx->scale.vy = params->args[9];
    fx->scale.vz = params->args[10];
    fx->scaleVel.vx = params->args[11];
    fx->scaleVel.vy = params->args[12];
    fx->scaleVel.vz = params->args[13];
    fx->scaleAccel.vx = params->args[14];
    fx->scaleAccel.vy = params->args[15];
    fx->scaleAccel.vz = params->args[16];
    fx->pos.vx = params->args[17];
    fx->pos.vy = params->args[18];
    fx->pos.vz = params->args[19];
    fx->vel.vx = params->args[20];
    fx->vel.vy = params->args[21];
    fx->vel.vz = params->args[22];
    fx->accel.vx = params->args[23];
    fx->accel.vy = params->args[24];
    fx->accel.vz = params->args[25];
    fx->hitRadius = params->args[26];
    fx->period = params->args[27];
    fx->fadeMode = params->args[28];
    fx->speed = params->args[29];
    fx->wavePhase = params->args[31];
    fx->waveAmplitude = params->args[32];
    fx->waveFreq = params->args[33];
    fx->mode = params->args[30];
}

void EVO_getEffectWorldPos(void *xform, EvoObject *obj) {
    SVECTOR pos;

    getTransformWorldPos(xform, &pos);
    obj->x = pos.vx;
    obj->y = pos.vy;
    obj->z = pos.vz;
}

void EVO_initEffectFromParams(EvoFx *fx, s32 *vars, EvoEffectScript *loader) {
    EVO_setEffectParams(fx, (EvoFxParams *)vars);
    if (vars[186] == -2) {
        fx->parent = NULL;
    } else if (vars[186] == -1) {
        fx->parent = &EVO_EFFECT_ROOT;
    } else {
        fx->parent = loader->entries[vars[186]].handle;
    }
}

EvoFx *EVO_createFadeRectFromParams(s32 *vars) {
    Rect16 rect;
    Bytes4 from;
    Bytes4 to;

    rect.x = vars[75];
    rect.y = vars[76];
    rect.w = vars[77];
    rect.h = vars[78];
    from.b[0] = vars[38];
    from.b[1] = vars[39];
    from.b[2] = vars[40];
    to.b[0] = vars[41];
    to.b[1] = vars[42];
    to.b[2] = vars[43];
    return (EvoFx *)EVO_createFadeRect((s16 *)&rect, &from, &to, vars[28], vars[29], vars[69]);
}

EvoFx *EVO_createRingFromParams(s32 *vars, EvoEffectScript *loader) {
    Bytes4 inner;
    Bytes4 mid;
    Bytes4 outer;
    EvoFx fx;

    EVO_initEffectFromParams(&fx, vars, loader);
    inner.b[0] = vars[38];
    inner.b[1] = vars[39];
    inner.b[2] = vars[40];
    mid.b[0] = vars[41];
    mid.b[1] = vars[42];
    mid.b[2] = vars[43];
    outer.b[0] = vars[44];
    outer.b[1] = vars[45];
    outer.b[2] = vars[46];
    return (EvoFx *)createRingEffect(vars[56], &inner, &mid, &outer, (EffectTemplate *)&fx, vars[22], vars[27], vars[28],
                                     vars[71], vars[62], vars[63], vars[66], vars[64], vars[65], NULL, 0, 0, 0,
                                     vars[70], vars[73], vars[74], 0);
}

EvoFx *EVO_createEffectObjectFromParams(s32 *vars, EvoEffectScript *loader) {
    EvoFx fx;

    EVO_initEffectFromParams(&fx, vars, loader);
    return (EvoFx *)cloneEffectObject((EffectTemplate *)&fx);
}

EvoFx *EVO_createModelEffectFromParams(s32 *vars, EvoEffectScript *loader) {
    EvoFx fx;
    EvoFx *template = &fx;

    EVO_initEffectFromParams(template, vars, loader);
    return (EvoFx *)EVO_createModelEffect(vars[56], template, vars[84], vars[85], -1, vars[86], vars[70], vars[87], (s32)loader->buffer, 0);
}

EvoFx *EVO_createStreaksFromParams(s32 *vars, EvoEffectScript *loader) {
    EvoFx fx;
    Bytes4 start;
    Bytes4 end;

    start.b[0] = vars[38];
    start.b[1] = vars[39];
    start.b[2] = vars[40];
    end.b[0] = vars[41];
    end.b[1] = vars[42];
    end.b[2] = vars[43];
    EVO_initEffectFromParams(&fx, vars, loader);
    return (EvoFx *)createStreakParticles(start.b, end.b, (EffectTemplate *)&fx, vars[189], vars[188], vars[190], vars[195],
                                          vars[191], vars[192], vars[193], vars[194], vars[80], vars[81], vars[79],
                                          vars[82], vars[27], vars[70], vars[74]);
}

EvoFx *(*EVO_EFFECT_CREATE_FUNCS[5])(s32 *, EvoEffectScript *) = {
    (EvoFx * (*)(s32 *, EvoEffectScript *)) EVO_createFadeRectFromParams,
    EVO_createRingFromParams,
    EVO_createEffectObjectFromParams,
    EVO_createStreaksFromParams,
    EVO_createModelEffectFromParams,
};

void EVO_freeFadeRect(void *obj) {
    freeHeapBlock(obj);
}

void (*EVO_EFFECT_FREE_FUNCS[5])(EvoFx *) = {
    (EvoFxFunc)EVO_freeFadeRect,
    (EvoFxFunc)freeRingEffect,
    (EvoFxFunc)freeEffectObject,
    (EvoFxFunc)freeStreakParticles,
    (EvoFxFunc)EVO_freeModelEffect,
};

void EVO_createEffectEntry(s32 index, s32 kind, s32 *vars, EvoEffectScript *loader) {
    if (EVO_EFFECT_CREATE_FUNCS[kind] != NULL) {
        loader->entries[index].kind = kind;
        loader->entries[index].active = 0;
        loader->entries[index].handle = EVO_EFFECT_CREATE_FUNCS[kind](vars, loader);
        loader->counter++;
        if ((loader->counter & 0xF) == 0) {
            waitFrames(FRAME_INTERVAL);
        }
    }
}

EvoEffectScript *EVO_createEffectScript(EvoMsd *data) {
    EvoFx fx;
    EvoEffectScript *loader;
    s32 i;

    loader = allocTaskHeapBlock(sizeof(EvoEffectScript));
    loader->data = data;
    loader->script = createScriptContext(data);
    loader->vars = allocScriptRegisters(0xC4);
    for (i = 0; i < 16; i++) {
        loader->entries[i].kind = -1;
        loader->entries[i].active = 0;
        loader->entries[i].handle = NULL;
    }
    loader->counter = 0;
    loader->buffer = NULL;
    fx.rot.vx = 0;
    fx.rot.vy = 0;
    fx.rot.vz = 0;
    fx.rotVel.vx = 0;
    fx.rotVel.vy = 0;
    fx.rotVel.vz = 0;
    fx.scale.vx = 0;
    fx.scale.vy = 0;
    fx.scale.vz = 0;
    fx.scaleVel.vx = 0;
    fx.scaleVel.vy = 0;
    fx.scaleVel.vz = 0;
    fx.scaleAccel.vx = 0;
    fx.scaleAccel.vy = 0;
    fx.scaleAccel.vz = 0;
    fx.pos.vx = 0x1000;
    fx.pos.vy = 0x1000;
    fx.pos.vz = 0x1000;
    fx.vel.vx = 0x1000;
    fx.vel.vy = 0x1000;
    fx.vel.vz = 0x1000;
    fx.accel.vx = 0;
    fx.accel.vy = 0;
    fx.accel.vz = 0;
    fx.fadeMode = 0;
    fx.speed = 0;
    fx.hitRadius = 0x80;
    fx.parent = SCENE_3D->viewMatrix;
    fx.mode = 0;
    EVO_EFFECT_ROOT = fx;
    initEffectObject(&EVO_EFFECT_ROOT);
    EVO_runEffectScript(loader);
    return loader;
}

void EVO_runEffectScriptTask(EvoMsd *data, s32 parentTask) {
    EvoEffectScript *loader;

    loader = EVO_createEffectScript(data);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (EVO_tickEffectScript(loader) != 0);
    EVO_freeEffectEntries(loader);
    freeScriptContext(loader->script, loader->vars);
    freeHeapBlock(loader);
    resumeTask(parentTask);
}
